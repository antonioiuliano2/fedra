#ifndef ROOT_EdbDataSet
#define ROOT_EdbDataSet

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDataSet                                                           //
//                                                                      //
// OPERA data set definition&reconstruction                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObjArray.h"
#include "EdbAffine.h"
#include "TFile.h"
#include "EdbRun.h"
#include "EdbPVRec.h"

//______________________________________________________________________________
class EdbSegmentCut : public TObject {

 private:
  Int_t    eXI;         // 0-exclusive; 1-inclusive cut
  Float_t  eMin[5];     // min  x:y:tx:ty:puls
  Float_t  eMax[5];     // max  x:y:tx:ty:puls

 public:
  EdbSegmentCut() {}
  EdbSegmentCut( int xi, float var[10] );
  virtual ~EdbSegmentCut() {}

  void SetXI(int xi)           {eXI=xi;}
  void SetMin(  float min[5] ) { for(int i=0;i<5;i++) eMin[i]=min[i]; }
  void SetMax(  float max[5] ) { for(int i=0;i<5;i++) eMax[i]=max[i]; }
  int   XI() const {return eXI;}
  float Min(int i) const {return eMin[i];}
  float Max(int i) const {return eMax[i];}
  int  PassCut( float var[5] );
  int  PassCutX( float var[5] );
  int  PassCutI( float var[5] );
  void Print();
  const char *CutLine(char *str, int i=0, int j=0) const;

  ClassDef(EdbSegmentCut,1)  // segment cut
};

//______________________________________________________________________________
class EdbLayer : public TObject {

 private:
  Int_t   eID;                // emulsion layer id (11,12 21,22, ...)
  Float_t eZ;                 // the z-coord where X and Y are calculated
  Float_t eZmin,eZmax;        // begin and the end of layer

  Float_t eX;                 // the center (0) of the layer's data
  Float_t eY;                 //
  Float_t eDX;                // acceptance in x:y
  Float_t eDY;                //
  Float_t eTX;                // direction
  Float_t eTY;                //

  Float_t eShr;               // shrinkage along axis z

  EdbAffine2D eAffXY;         // coordinate (XY) affine transformation

  EdbAffine2D eAffTXTY;       // tangents affine transformation

 public:
  EdbLayer();
  virtual ~EdbLayer(){}

  int   ID()   const {return eID;}
  float X()    const {return eX;}
  float Y()    const {return eY;}
  float Z()    const {return eZ;}
  float TX()   const {return eTX;}
  float TY()   const {return eTY;}
  float Zmin() const {return eZmin;}
  float Zmax() const {return eZmax;}
  float DX()   const {return eDX;}
  float DY()   const {return eDY;}
  float Xmin() const {return X()-DX();}
  float Xmax() const {return X()+DX();}
  float Ymin() const {return Y()-DY();}
  float Ymax() const {return Y()+DY();}

  float Shr()  const {return eShr;}

  void SetXY(float x, float y )    { eX=x; eY=y; }
  void SetDXDY(float dx, float dy) { eDX=dx; eDY=dy; }
  void SetTXTY(float tx, float ty )    { eTX=tx; eTY=ty; }
  void SetShrinkage(float shr) {eShr=shr;}
  void SetZlayer(float z,float zmin,float zmax) { eZ=z; eZmin=zmin; eZmax=zmax; }
  void SetAffXY(float a11,float a12,float a21,float a22,float b1,float b2) 
    {eAffXY.Set(a11,a12,a21,a22,b1,b2);}
  void SetAffTXTY(float a11,float a12,float a21,float a22,float b1,float b2) 
    {eAffTXTY.Set(a11,a12,a21,a22,b1,b2);}

  EdbAffine2D *GetAffineXY()   {return &eAffXY;}
  EdbAffine2D *GetAffineTXTY() {return &eAffTXTY;}

  void Print();

  ClassDef(EdbLayer,1)  // shrinked layer
};

//______________________________________________________________________________
class EdbDataPiece : public TNamed {

 private:
  Int_t        ePlate;          // plate id
  Int_t        ePiece;          // piece id in this plate
  Int_t        eFlag;           // 0-do nothing, 1-do something
  TObjArray    eRunFiles;       //
  //  TString      eFileNameRaw;    // name of the raw data file (run)
  TString      eFileNameCP;     // name of the couples data file
  TString      eFileNamePar;    // name of the parameters file
  Int_t        eAFID;           // 1-use fiducial marks transformations, 0 - do not
  Int_t        eCLUST;          // 1-use clusters, 0 - do not

  EdbLayer    *eLayers[3];      // base(0),up(1),down(2)  layers
  EdbScanCond *eCond[3];        //
  TIndexCell  *eAreas[3];       // base/up/down  surface areas list
  TObjArray   *eCuts[3];        // array of cuts
  Float_t      eCutCP[6];       //

  EdbRun      *eRun;            //!
  TTree       *eCouplesTree;    //!

 public:
  EdbDataPiece();
  EdbDataPiece(int plate, int piece, char* file, int flag);
  virtual ~EdbDataPiece();

  void Set0();
  void SetVolume0( float x0, float y0, float z0, float tx=0, float ty=0 );
  void SetVolumeA( float dx, float dy ) { GetLayer(0)->SetDXDY( dx, dy); }
  void AddRunFile( const char *name );
  const char *GetRunFile(int i) const;
  const char *MakeName();
  const char *MakeNameCP(const char *dir);
  const char *GetNameCP() const {return eFileNameCP.Data();}
  void  Print();
  void  WriteCuts();
  int   CheckCCD(int maxentr=1000);
  int   RemoveCCDPeak(int matr[1000][1000]);
  int   UpdateSegmentCut(EdbSegmentCut cut);

  void SetCouplesTree(TTree *tree) {eCouplesTree=tree;}
  int       Nruns() const { return eRunFiles.GetEntries(); }
  int       Flag() const {return eFlag;}
  EdbLayer *GetMakeLayer(int id);
  EdbLayer *GetLayer(int id)
    { if(eLayers[id]) return (EdbLayer *)eLayers[id]; else return 0; }
  EdbScanCond *GetMakeCond(int id);
  EdbScanCond *GetCond(int id)
    { if(eCond[id]) return (EdbScanCond *)eCond[id]; else return 0; }


  void           AddCutCP(float var[6]);
  void           AddSegmentCut(int layer, int xi, float var[5]);
  int            NCuts(int layer);
  EdbSegmentCut *GetCut(int layer, int i)
    { return (EdbSegmentCut *)(eCuts[layer]->UncheckedAt(i)); }

  int  AcceptViewHeader(const EdbViewHeader *head);
  void MakeNamePar(const char *dir);
  int  CorrectAngles(TTree *tree);
  void CorrectShrinkage( int layer, float shr );
  int  UpdateShrPar( int layer );
  int  UpdateAffPar(  int layer, EdbAffine2D &aff);
  int  UpdateAffTPar( int layer, EdbAffine2D &aff);
  int  UpdateZPar( int layer, float z );
  int  TakePiecePar();
  int  ReadPiecePar(const char *file);
  int  MakeLinkListArea(int irun);
  int  MakeLinkListCoord(int irun);
  int  GetAreaData(EdbPVRec *ali, int const area, int const side);
  int  TakeRawSegment(EdbView *view, int id, EdbSegP &segP, int side);
  int  PassCuts(int id, float var[5]);
  int  PassCutCP(float var[6]);

  float GetRawSegmentPix( EdbSegment *seg, EdbView *view );
  int  GetRawData(EdbPVRec *ali);
  int  GetCPData(EdbPVRec *ali);
  int  TakeCPSegment(EdbSegCouple &cp, EdbSegP &segP);

  ClassDef(EdbDataPiece,1)  // Edb raw data unit (scanned plate) associated with run file
};


//______________________________________________________________________________
class EdbDataSet : public TNamed {

 private:

  TString    eInputList;    // list of input data (runs)
  TString    eAnaDir;       // path for analysis data directory
  TString    eParDir;       // path for parameters directory
  TString    eDBFileName;   // root file to keep pieces parameters
  TFile      *eDBFile;      // the file (database) to save all parameters

  TObjArray  ePieces;       // array of runs

 public:
  EdbDataSet();
  EdbDataSet(const char *file);
  virtual ~EdbDataSet();

  void Set0();
  int N() const { return ePieces.GetEntries(); }
  EdbDataPiece *GetPiece(int id) 
    { if(id<ePieces.GetEntries()) return (EdbDataPiece *)ePieces.At(id); else return 0;}

  const char *GetAnaDir() const {return eAnaDir.Data();}
  const char *GetParDir() const {return eParDir.Data();}
  int  ReadDataSetDef(const char *file);
  int  GetRunList(const char *file);
  void PrintRunList();
  void WriteRunList();

  EdbDataPiece *FindPiece(const char *name);

  ClassDef(EdbDataSet,1)  // OPERA emulsion data set
};

//______________________________________________________________________________
class EdbDataProc : public TObject {

 private:

  EdbDataSet *eDataSet;

 public:
  EdbDataProc() {eDataSet=0;}
  EdbDataProc(const char *file);
  virtual ~EdbDataProc();

  int  InitVolume(EdbPVRec *ali);
  int  InitVolumeRaw(EdbPVRec *ali);
  int  Process(){ return Link(); }  // to be removed
  int  CheckCCD();
  int  Link();
  int  Link(EdbDataPiece &piece);
  void Align();
  void LinkTracks(int alg=0);
  void LinkRawTracks(int alg=0);
  void AlignLinkTracks(int alg=0);

  int    ShrinkCorr() {return 1;}
  int    CheckShrinkage( EdbPVRec *ali, int couple, float &shr1, float &shr2 );
  void   CorrectAngles();

  void   FineAlignment();
  void   FillCouplesTree( TTree *tree, EdbPVRec *al, int fillraw=0 );
  void   CloseCouplesTree( TTree *tree );

  TTree *InitCouplesTree( const char *file, const char *mode="READ" );

  ClassDef(EdbDataProc,1)  // emulsion data processing
};

#endif /* ROOT_EdbDataSet */
