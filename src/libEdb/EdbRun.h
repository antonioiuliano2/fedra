#ifndef ROOT_EdbRun
#define ROOT_EdbRun

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRun                                                               //
//                                                                      //
// main run class contained all objects                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TFile.h"
#include "TTree.h"
#include "TObjString.h"

#ifndef ROOT_EdbView
#include "EdbView.h"
#endif

#ifndef ROOT_EdbRunHeader
#include "EdbRunHeader.h"
#endif

#ifndef ROOT_EdbPrediction
#include "EdbPrediction.h"
#endif

#ifndef ROOT_EdbFiducial
#include "EdbFiducial.h"
#endif


//______________________________________________________________________________
class EdbRun : public TObject {

private:

  EdbRunHeader      *eHeader;         // run header

  EdbView           *eView;           // view using for import and export data

  TTree             *eTree;           // tree with View objects
  TFile             *eFile;           // file associated with the Run
  TString            ePath;           // runs directory path

  EdbPredictionsBox *ePredictions;    // predictions to scan ($c)

  EdbMarksSet       *eMarks;          // fiducial marks

public:
  EdbRun();
  EdbRun( int id, const char *status="READ" , const char *path="." );
  EdbRun( const char *fname, const char *status="READ" );
  EdbRun( EdbRun &run, const char *fname );
  virtual ~EdbRun() {}


  EdbView             *GetView() const { return eView; }
  void                 SetView(EdbView *view);
  void                 SetView();

  EdbPredictionDC    *GetPrediction(int ip)  { return ePredictions->GetPrediction(ip); }
  int                 Npredictions()      const { return ePredictions->N(); }

  EdbPredictionsBox   *GetPredictions() const { return ePredictions; }
  EdbMarksSet         *GetMarks()       const { return eMarks; }
  void                 TransformDC();   // transform predictions according to fid marks
  void                 GeneratePredictions( int n );

  void SelectOpenMode( const char *fname, const char *status="READ" );

  int        GetRunID()     const { return eHeader->GetRunID();     }
  TDatime   *GetStartTime()  const { return eHeader->GetStartTime();  }
  TDatime   *GetFinishTime()  const { return eHeader->GetFinishTime();  }

  void       SetRunID( int id )          { eHeader->SetRunID(id);  }
  void       SetComment( const char *com ) { eHeader->SetComment(com);   }
  void       SetTitle( const char *tit ) { eHeader->SetTitle(tit); }


  int  GetEntries() const { return (Int_t)eTree->GetEntries(); }

  EdbRunHeader  *GetHeader()     const { return eHeader; }
  //  EdbStage      *GetStage()      const { return eHeader->GetStage();       }
  //  EdbPlate      *GetPlate()      const { return eHeader->GetPlate();       }

  void AddView();
  void AddView( EdbView *view );
  void Create( const char *fname );
  void Open( const char *fname );
  void OpenUpdate( const char *fname );

  void Close();
  TFile    *GetFile(){ return eFile; }

  EdbView        *GetEntry(         int entry, int ih=1, int icl=0, int iseg=1, int itr=0, int ifr=0 );
  EdbViewHeader  *GetEntryHeader(   int entry ) const;
  TClonesArray   *GetEntryClusters( int entry ) const;
  TClonesArray   *GetEntrySegments( int entry ) const;
  TClonesArray   *GetEntryTracks(   int entry ) const;
  TObjArray      *GetEntryFrames(   int entry ) const;

  void Print() const;
  void PrintLog( const char *fname ) const;

  void PrintBranchesStatus() const;
  void Init();

  void Save();
  void SaveViews() { eTree->AutoSave(); }

  //  void Test();

  ClassDef(EdbRun,1)  // main run class contained all objects
};

R__EXTERN EdbRun  *gRUN;

#endif /* ROOT_EdbRun */

