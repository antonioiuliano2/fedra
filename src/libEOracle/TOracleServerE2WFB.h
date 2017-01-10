#ifndef ROOT_TOracleServerE2WFB
#define ROOT_TOracleServerE2WFB

#include "TOracleServerE2.h"
#include "EdbView.h"
#include "EdbBrick.h"
#include "TObjString.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;

//------------------------------------------------------------------------------------
class TOracleServerE2WFB : public TOracleServerE2 {

  public:

    Bool_t     eDoCommit;  // true - commit current transaction (default is false)
    TString    eLab;       //! as "NAPOLI"
    TString    eLa;        //! as "NA"

  public:
    TOracleServerE2WFB(const char *db, const char *uid, const char *pw):TOracleServerE2(db, uid, pw) {Set0();}
  ~TOracleServerE2WFB(){}

  void Set0();
  Int_t MyQuery(const char *sqlquery); // to be moved into parent class?
  Int_t SetTransactionRW() {return MyQuery("SET TRANSACTION READ WRITE");}
  Bool_t FinishTransaction();
  
  const char *Timestamp();
    
  ULong64_t  IfEventRec( ULong64_t id_eventbrick );
  ULong64_t  IfEventBrick( ULong64_t id_eventbrick, const char *id_set );
  Int_t AddEventBrick(const char *databrick);
  Int_t AddPlate(ULong64_t id_eventbrick, const char *dataplate);
  Int_t AddTemplateMarkSets(const char *datamarks);
  
  ULong64_t  AddProcessOperationBrick( 
      ULong64_t id_machine,
  ULong64_t id_programsettings, 
  ULong64_t id_requester, 
  ULong64_t id_eventbrick, 
  ULong64_t id_parent_operation, 
  const char *starttime, 
  const char *notes);
  
  ULong64_t  AddFeedbackReconstruction(ULong64_t id_eventbrick, ULong64_t id_processoperation);
  ULong64_t  AddFeedbackReconstructionTest(ULong64_t id_eventbrick, ULong64_t id_processoperation);

  Int_t AddFeedbackVertex
      (
      ULong64_t   id_eventbrick,
  ULong64_t   id_reconstruction,
  Int_t       id_vertex,
  Float_t     posx,
  Float_t     posy,
  Float_t     posz,
  const char *isprimary,
  const char *ischarm,
  const char *istau, 
  const char *outofbrick
      );

  Int_t  AddFeedbackTrack(
      ULong64_t   id_eventbrick, 
  ULong64_t   id_reconstruction, 
  Int_t id_track, 
  Int_t id_upvtx, 
  Int_t id_downvtx,
  Float_t x, 
  Float_t y, 
  Float_t z, 
  Float_t sx, 
  Float_t sy, 
  const char *manual, 
  const char *particle, 
  const char *scanback, 
  const char *darkness,
  Float_t upip, 
  Float_t downip, 
  Float_t p, 
  Float_t pmin, 
  Float_t pmax,
  const char *outofbrick,
  Int_t lastplate,
  Float_t rslopet,
  Float_t rslopel,
  Float_t rmsslopet,
  Float_t rmsslopel,
  Int_t kinkplatedown,
  Int_t kinkplateup,
  const char *decaysearch,
  ULong64_t   event
                         );
  
  Int_t  AddFeedbackSegment(
      ULong64_t   id_eventbrick, 
  ULong64_t   id_reconstruction, 
  Int_t id_plate,
  Int_t id_track,
  const char *tracktype,
  Float_t posx, 
  Float_t posy, 
  Float_t posz, 
  Float_t slopex, 
  Float_t slopey, 
  Int_t grains,
  const char *trackmode
                                               );
  Int_t  CloseFeedbackDataset(    ULong64_t id_operation,
                                  const char *endtime
                             );
  
  const char *Ostr(ULong64_t num);
  const char *Ostr(Int_t num);
  void Print();
  
  ClassDef(TOracleServerE2WFB,1)  // write feedback files into central DB directly
};

 struct fbsegment {
   Int_t   id_plate;
   Float_t x; 
   Float_t y; 
   Float_t z;
   Float_t sx; 
   Float_t sy;
   Int_t   type; // 0=BT, 1=microT top, 2=microT bottom
   Int_t   irec; //0 Automatic, 1 SB, 2 Manual
   Int_t   grains;
 };
  
 struct fbtrack {
   Int_t   id_track;
   Int_t   id_upvtx;
   Int_t   id_downvtx;
   Float_t x;
   Float_t y;
   Float_t z;
   Float_t sx;
   Float_t sy;
   Float_t upip;
   Float_t downip;
   Float_t p;
   Float_t pmin;
   Float_t pmax;
   Int_t   isman;
   Int_t   type;        // 1-muon, 2-charm, 3-electron, 4 e+e-, 5 tau
   Int_t   scanback;
   Int_t   darkness;
   Int_t   ofb;             // 1=pass through, 2=edge out, 0 others
   Int_t   lastplate;       // 1= pass though, n=edge out track last plate, 0 others 
   Int_t   nseg;
   Float_t rmsslopet;
   Float_t rmsslopel;
   Float_t rslopet;
   Float_t rslopel;
   Int_t   kinkplatedown;
   Int_t   kinkplateup;
   Int_t   flag;            // 1-primary, 2 - e+e-, 3-lowmom, 4-sf to do, 5-sf done
   fbsegment **segments;
 };

 struct fbvertex {
    //VTX   X        Y          Z      Pri Cha Tau NDow Nup OfB
   Int_t  idvtx;
   Float_t x;
   Float_t y;
   Float_t z;
   Int_t   isprim;
   Int_t   ischarm;
   Int_t   istau;
   Int_t   ndown;
   Int_t   nup;
   Int_t   ofb;   //1-dead, 0-ok
   fbtrack **tracks;
 };
//------------------------------------------------------------------------------------
class EdbFeedback : public TObject {
  
  public:
  
  TOracleServerE2WFB *eDB;
  ULong64_t eIdMachine;         // as 6000000000010002
  ULong64_t eIdProgramsettings; // as 81000100000000087
  ULong64_t eIdRequester;       // as 6000000000100375
  ULong64_t eBrick;
  ULong64_t eEventBrick;
  ULong64_t eEvent;
  ULong64_t eProcOp;
  ULong64_t eRecID;
  
  Int_t eERROR;   // 0 by default, 1 if error happened
  int eNvtxLim;   // limit for vtx array size
  int eNvtx;      // number of vertices
  int eNtrLim;    // limit for tracks array size
  int eNtr;       // number of tracks
  fbvertex **eV;  //! array of vertices
  fbtrack  **eT;  //! array of tracks
  
  public:
    EdbFeedback();
  ~EdbFeedback(){}
    
  int ReadFBFile( const char *file );
  fbvertex  *ReadVertex( const char *str );
  fbtrack   *ReadTrack( const char *str );
  fbsegment *ReadSegment( FILE *f );
  
  void Print();
  void PrintFB();
  void Print( fbvertex *v );
  void Print( fbtrack *t );
  void Print( fbsegment *s );
  
  int InitDB(const char *conn, const char *user, const char *pwd);
  int LoadFBintoDB();

  const char *DecayFlag(int flag);
  const char *Ofb(int ofb);
  const char *PartType(int parttype);
  const char *Darkness(int drk);
  
  const char *SegmentRecMode(int mode);
  const char *SegmentType(int type);
  
  ClassDef(EdbFeedback,1)  // feedback container class
};

#endif

