#ifndef ROOT_EdbScanProc
#define ROOT_EdbScanProc

#include "EdbRunAccess.h"
#include "EdbDataSet.h"
#include "EdbScanClient.h"
#include "EdbScanSet.h"

class EdbScanProc : public TNamed
{
 public:

  TString eProcDirClient;    // directory path for root data

public: 
  EdbScanProc();
  virtual ~EdbScanProc(){}

  bool    CheckDir(const char *dir, bool create=true);
  bool    CheckDirWritable(const char *dir);
  bool    CheckAFFDir(int brick, bool create=true);
  bool    CheckBrickDir(EdbID id, bool create=true);
  bool    CheckPlateDir(EdbID id, bool create=true);
  bool    CheckProcDir(int id[4], bool create=true);
  bool    CheckProcDir( EdbID id, bool create=true) {int id4[4]; id.Get(id4); return CheckProcDir(id4,create); }
  void    MakeFileName(TString &s, int id[4], const char *suffix, bool inplate=true);
  void    MakeFileName(TString &s, EdbID id, const char *suffix, bool inplate=true) {int id4[4]; id.Get(id4); return MakeFileName(s,id4,suffix,inplate);}
  void    MakeAffName(TString &s, int id1[4], int id2[4], const char *suffix="aff.par");
  void    MakeAffName(TString &s, EdbID id1, EdbID id2, const char *suffix="aff.par") 
    { int id14[4]; id1.Get(id14); int id24[4]; id2.Get(id24); return MakeAffName(s,id14,id24,suffix); }
  bool    GetMap(int brick, TString &map);
  bool    AddParLine(const char *file, const char *line);
  bool    MakeInPar(int id[4], const char *option);
  bool    MakeInPar(EdbID id, const char *option)  {int id4[4]; id.Get(id4); return MakeInPar(id4,option);}
  int     CopyFile(int id1[4], int id2[4], const char *suffix, bool overwrite);
  int     CopyPar(int id1[4], int id2[4], bool overwrite=true) {return CopyFile(id1,id2,"par",overwrite);}
  int     CopyPred(int id1[4],int id2[4], bool overwrite=true) {return CopyFile(id1,id2,"pred.root",overwrite);}
  int     CopyAFFPar(int id1c[4],int id2c[4], int id1p[4], int id2p[4], bool overwrite=true);
  int     RemoveFile(EdbID id, const char *suffix);

  void    CheckFiles( EdbScanSet &sc, const char *suffix );

  int     ReadPatTXT(EdbPattern &pred, int id[4], const char *suffix, int flag=-1);
  int     ReadPatTXT(const char *file, EdbPattern &pred, int flag=-1);
  int     WritePatTXT(EdbPattern &pred, int id[4], const char *suffix, int flag=-1);
  int     ReadPatRoot(EdbPattern &pred, int id[4], const char *suffix, int flag=-1);
  int     WritePatRoot(EdbPattern &pred, int id[4], const char *suffix, int flag=-1);
  int     ReadPred(EdbPattern &pred, int id[4], int flag=-1) {return ReadPatRoot(pred,id,"pred.root",flag);}
  int     WritePred(EdbPattern &pred, int id[4], int flag=-1) {return WritePatRoot(pred,id,"pred.root",flag);}
  int     ReadFound(EdbPattern &pred, int id[4], int flag=-1) {return ReadPatRoot(pred,id,"found.root",flag);}
  int     WriteFound(EdbPattern &pred, int id[4], int flag=-1) {return WritePatRoot(pred,id,"found.root",flag);}
  int     ReadFound(EdbPattern &pred, EdbID id, int flag=-1) {int id4[4]; id.Get(id4); return ReadFound(pred,id4,flag);}
  int     ReadPred(EdbPattern &pred, EdbID id, int flag=-1) {int id4[4]; id.Get(id4); return ReadPred(pred,id4,flag);}

  EdbRun *InitRun(int id[4]);
  bool    FlashRawDir(EdbScanClient &scan, int id[4]);
  int     LoadPlate(EdbScanClient &scan, int id[4], int attempts=1);
  int     ScanAreas(EdbScanClient &scan, int id[4], int flag=-1, const char *opt="NOCLCLFRAMESUM");
  int     ScanAreas(EdbScanClient &scan, EdbPattern &pred, int id[4], const char *opt="NOCLCLFRAMESUM"); // NEW!!!

  bool    InitPiece(EdbDataPiece &piece, int id[4]);
  bool    InitPiece(EdbDataPiece &piece, EdbID id) {int id4[4]; id.Get(id4); return InitPiece(piece,id4);}
  int     ReadPiece(EdbDataPiece &piece, EdbPattern &pat);
  int     ReadPatCP(EdbPattern &pat, int id[4], TCut c="1");
  int     ReadPatCP(EdbPattern &pat, EdbID id, TCut c="1") {int id4[4]; id.Get(id4); return ReadPatCP(pat,id4,c);}
  int     ReadPatCPnopar(EdbPattern &pat, EdbID id, TCut cut="1", bool do_erase=true);
  int     ReadPatCPnopar(EdbPattern &pat, const char *file, TCut cut="1", EdbMask *erase_mask=0);
  EdbMask* ReadEraseMask(EdbID id);
  bool    ApplyAffZ(EdbPattern &pat,int id1[4],int id2[4]);
  bool    GetAffZ(EdbAffine2D &aff, float &z,int id1[4],int id2[4]);
  bool    SetAFFDZ(int id1[4], int id2[4], float dz);
  bool    SetAFF0(int id1[4], int id2[4]);
  bool    MakeAFFSet(EdbScanSet &sc);
  bool    PrepareSetStructure(EdbScanSet &sc);

  int     ConvertAreas(EdbScanClient &scan, int id[4], int flag=-1, const char *opt="NOCLCLFRAMESUM");
  int     CorrectAngles(int id[4]);
  int     LinkRun(int id[4], int noUpdate=1);
  int     LinkRunAll(int id[4], int npre=3, int nfull=1, int correct_ang=1);
  int     LinkRunAll(EdbID id, int npre=3, int nfull=1, int correct_ang=1)  {int id4[4]; id.Get(id4); return LinkRunAll(id4,npre,nfull,correct_ang);}
  int     LinkSet(EdbScanSet &sc, int npre=3, int nfull=1, int correct_ang=1);
  int     Align(int id1[4], int id2[4], const char *option="");
  int     AlignAll(int id1[4], int id2[4], int npre=1, int nfull=3, const char *opt="-z");
  int     AlignAll(EdbID id1, EdbID id2, int npre=1, int nfull=3, const char *opt="-z")
    {int id41[4]; id1.Get(id41); int id42[4]; id2.Get(id42); return AlignAll(id41, id42, npre, nfull, opt);}
  int     AlignSet( EdbScanSet &sc, int npre=1, int nfull=3, const char *opt="-z");
  int     TrackSetBT( EdbScanSet &sc, EdbScanCond &cond, TCut c="1");

  bool    CorrectPredWithFound(int id1[4], int id2[4], const char *opt="-z", int patmin=6);
  bool    CorrectAffWithPred(int id1[4], int id2[4], const char *opt="-z", int patmin=6, const char *parfile="fullalignment");
  bool    ProjectFound(int id1[4],int id2[4]);

  int     FindPredictions(EdbPattern &pred, int id[4], EdbPattern &found, int maxholes=3);
  int     FindPredictions(int id[4], int flag=-1, int maxholes=3);


  bool    InitRunAccess(EdbRunAccess &ra, int id[4], bool do_update=false);
  int     FindPredictionsRaw(int idp[4], int idr[4]);
  int     FindPredictionsRaw(EdbPattern &pred, EdbPattern &found, EdbRunAccess &ra, 
			     EdbScanCond &condBT, EdbScanCond &condMT, 
			     float delta_theta=0.1, float puls_min=5., float puls_mt=9., float chi2max=1.6, FILE *out=0 );
  int     FindCompliments( EdbSegP &s, EdbPattern &pat, TObjArray &found, float chi2max, TArrayF &chiarr );
  void    SetDefaultCondBT(EdbScanCond &cond);
  void    SetDefaultCondMT(EdbScanCond &cond);

  void    OptimizeScanPath(EdbPattern &pin, EdbPattern &pout,int brick);
  int     RemoveDublets(EdbPattern &pin, EdbPattern &pout,int brick);

  bool    AddAFFtoScanSet(EdbScanSet &sc, EdbID id1, EdbID id2);
  bool    AddAFFtoScanSet(EdbScanSet &sc, int id1[4], int id2[4]);
  bool    AddAFFtoScanSet(EdbScanSet &sc, int b1, int p1, int s1, int e1,int b2, int p2, int s2, int e2);

  int     AssembleScanSet(  EdbScanSet &ss);
  int     ReadScanSetCP(    EdbScanSet &ss, EdbPVRec &ali, TCut c="1", bool do_erase=true);
  int     ReadFoundSegment( EdbID id,  EdbSegP &s, int flag=-1);
  int     ReadFoundTrack(   EdbScanSet &ss, EdbTrackP &track, int flag=-1);
  int     ReadFoundTracks(   EdbScanSet &ss,  EdbPVRec &ali, int flag=-1);

  int     WriteScanSet(EdbID id, EdbScanSet &ss);
  EdbScanSet  *ReadScanSet(EdbID id);

  int     WriteSBTrack(EdbTrackP &t, int path, EdbID id);

  void    PrepareVolumesPred(int id[4], EdbPattern &points, int before=5, int after=5, 
			     int pmin=1, int pmax=57, EdbScanSet *sc=0);

  int     TestAl(int id1[4], int id2[4]);
  int     TestAl(EdbID id1, EdbID id2)   {int id14[4]; id1.Get(id14); int id24[4]; id2.Get(id24); return TestAl(id14,id24); }
  int     TestAl(EdbPattern &p1, EdbPattern &p2);
  int     TestAl(const char *cpfile1, const char *cpfile2, TCut &cut, float dz);

  int     ReadMarksSet(EdbMarksSet &ms, int brick, const char *filename, char spacer='_');
  int     WriteMarksSet(EdbMarksSet &ms, int brick, const char *filename, char spacer='_');

  void    LogPrint(int brick, int level, const char *rout, const char *msgfmt, ...);
  void    Print();
  ClassDef(EdbScanProc,1)  // scanned data processing
};
#endif /* ROOT_EdbScanProc */
