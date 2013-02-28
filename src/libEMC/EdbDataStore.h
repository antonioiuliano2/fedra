#ifndef EDBDATASTORE_H
#define EDBDATASTORE_H

#include <assert.h>
#include <TObject.h>
#include <TObjArray.h>
#include "EdbBrick.h"
#include "EdbLayer.h"
#include "EdbSegP.h"
#include "EdbPattern.h"
#include "EdbVertex.h"
#include "EdbScanTracking.h"
#include "EdbMomentumEstimator.h"

class EdbDataStore: public TObject{
  public:
    EdbDataStore();
    ~EdbDataStore();
    ///transfer methods
    void TransferTo(EdbDataStore* ds, char level, EdbSegmentCut* cut=0,int FromPlate=0, int ToPlate=57);
    void TransferGeometry(EdbDataStore* ds);
    static void TransferSegs(EdbPatternsVolume* pv0, EdbPatternsVolume* pv1,EdbSegmentCut* cut=0,int FromPlate=0, int ToPlate=57);
    ///restore MC info methods
    void LoadMCVertices(TObjArray* vtx);
    void Restore_PIDFromID();
    void Restore_PatFromGeom(int np0=0, int np1=1000);
    void Restore_TrxFromVtx();
    void Restore_SegFromTrx(EdbSegmentCut* cut=0,int Plt0=0, int Plt1=1000);
    ///clear methods
    void Clear(bool hard=false){ClearTracks(hard);ClearVTX();ClearRaw(hard);ClearSeg(hard);}
    void ClearTracks(bool hard=false);
    void ClearRaw(bool hard=false);
    void ClearSeg(bool hard=false);
    void ClearVTX();
    void ClearGeom();
    /// count methods:
    int Nt(){return eTracks.GetEntries();}
    int Nv(){return eVTX.GetEntries();}
    int Nplt(){return eRawPV.Npatterns()/2;}
    ///setown methods:
    void SetOwnTracks(bool own=true){eTracks.SetOwner(own);}
    void SetOwnVertices(bool own=true){eVTX.SetOwner(own);}
    void SetOwnTrkSegs(){for(int nt=0;nt<Nt();++nt)GetTrack(nt)->SetOwner();}
    ///get methods
    EdbTrackP*  GetTrack (int n){return (n<eTracks.GetEntries())?(EdbTrackP*)eTracks.At(n):0;}
    EdbVertex*  GetVertex(int n){return (n<eVTX.GetEntries())?(EdbVertex*)eVTX.At(n):0;}
    EdbPattern* GetSegPat(int n){return (n<eSegPV.Npatterns())?eSegPV.GetPattern(n):0;}
    EdbPattern* GetRawPat(int n){return (n<eRawPV.Npatterns())?eRawPV.GetPattern(n):0;}
    EdbPattern* GetPattern(int n, bool btk=true){return GetPV(btk)->GetPattern(n);}
    EdbPatternsVolume* GetPV(bool btk=true){return btk?(&eSegPV):(&eRawPV);}
    
    ///find methods
    EdbTrackP* FindTrack(int id);
    EdbVertex* FindVertex(int id);
    EdbPattern* FindPattern(int plate, int side=0);
    EdbLayer*   FindLayer(int plate, int side=0);
    
    EdbTrackP* FindLongTrk(int nsmin=8);
    EdbVertex* FindPrimVtx();
    ///add methods
    void AddTrack(EdbTrackP* tr){assert(tr!=0); eTracks.Add(tr);}
    void AddVertex(EdbVertex* v){assert(v!=0);  eVTX.Add(v);}
    void AddPattern(EdbPattern* pat);
    void MakePattern(double z,int plate,int side);
    ///print methods
    void PrintBrief();
    void PrintPatterns();
//     void PrintLayers();
    void PrintTracks(int vlev=0);
//     void PrintVertices();
    ///save methods:
    void SaveToRaw(char* dir="./",int id=1234);
    void SavePlateToRaw(int PID,const char* dir="./",int id=1234);
    ///methods for simulation:
    void DoSmearing(EdbScanCond* cond_btk,EdbScanCond* cond_mtk=0);
//     void DoSmearTrack(EdbTrackP*);
  public:
    EdbBrickP * eBrick;  ///geometry
    EdbPatternsVolume eRawPV; ///
    EdbPatternsVolume eSegPV;
    TObjArray eTracks;
    TObjArray eVTX;
    
    ClassDef(EdbDataStore,1)   //OPERA Brick data container
};

class EdbDSRec: public EdbDataStore{
  public:
    EdbDSRec();
    ~EdbDSRec(){};
    
    void Clear(bool hard=false);
    
    int DoTracking(bool use_btk=true,int p0=0, int p1=100);
    int DoTracking0(bool use_btk=true,int p0=0, int p1=100);
    int DoMomEst();  
    int DoVertexing();
    int DoDecaySearch(); ///TODO
    int DoFindBlkSeg(EdbVertex* v,int w0,double ImpMax=50., double RMax=3000, int Dpat=1);
    ///prepare segments' cov matrix
    void FillECovPV(EdbPatternsVolume*,EdbScanCond* cnd=0);
    void FillECovSeg(EdbSegP* seg,EdbScanCond* cnd=0);
    
    void FillECovTrks();
    void FillErrorsCOV();
  public:
    EdbVertexRec      eVRec;
    EdbMomentumEstimator eMomEst;
    EdbScanCond eCond_b,eCond_m;
    ClassDef(EdbDSRec,1)  //OPERA event reconstruction
};

#endif
