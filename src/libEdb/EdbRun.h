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

struct AlignmentParView
{
  Int_t view1;  // 1-st view
  Int_t view2;  // 2-d view
	Int_t area1;
	Int_t area2;
	Int_t side1;
	Int_t side2;
  Float_t dx;   // found offset
  Float_t dy;   // found offset
  Float_t dz;   // found offset
  Int_t n1tot;  // total number of grains in the view
  Int_t n2tot;
  Int_t n1;     // number of grains in intersection area
  Int_t n2;
  Int_t nsg;   // peak height
  Int_t nbg;   // bg level (random coinsidences)
  Int_t flag;  // 0 - found offsets not applied to saved data; 1-applied
};

struct AlignmentParFrame
{
	Int_t frame1; // 1-st frame id
	Int_t frame2; // 2-d frame id
  Int_t view;   // view id
	Int_t area;
	Int_t side;
	Float_t dxglobal;  // global offset applied for all frames of this view (XfrStage=Xframecorrected-dxglobal-flag*dx)
	Float_t dyglobal;  // global offset applied for all frames of this view
  Float_t dx;        // found offset
  Float_t dy;        // found offset
  Float_t z1;        // z of the first frame
  Float_t z2;        // z of the second frame
  Int_t n1tot;       // total number of clusters in the frame
  Int_t n2tot;
  Int_t n1;          // number of grains in intersection area
  Int_t n2;
  Int_t nsg;         // peak height
  Int_t nbg;         // bg level (random coinsidences)
  Int_t flag;        // 0 - found offsets not applied to saved data; 1-applied
};

//______________________________________________________________________________
class EdbRun : public TObject {

private:

  EdbRunHeader      *eHeader;         // run header

  EdbView           *eView;           // view using for import and export data

  TTree             *eTree;           // tree with View objects
  TFile             *eFile;           // file associated with the Run
  TString            ePath;           // runs directory path

  EdbPredictionsBox *ePredictions;    // predictions to scan ($c)

  TTree             *eViewMerge;      // view merging alignment
  TTree             *eViewAlign;      // view neighbours  alignment
  TTree             *eFrameAlign;     // frames alignment
  TTree             *ePinViews;       // pinned views
  AlignmentParView      eVM;
  AlignmentParView      eVA;
  AlignmentParFrame     eFA;
  EdbViewHeader        *ePVH;        // to add pinned view header
      
public:
  EdbMarksSet       *eMarks;          // fiducial marks

public:
  EdbRun();
  EdbRun( int id, const char *status="READ" , const char *path="." );
  EdbRun( const char *fname, const char *status="READ" );
  EdbRun( EdbRun &run, const char *fname );
  virtual ~EdbRun();

  EdbView             *GetView() const { return eView; }
  void                 SetView(EdbView *view);
  void                 SetView();
  TTree               *GetTree() const {return eTree; }

  EdbPredictionDC    *GetPrediction(int ip)  { return ePredictions->GetPrediction(ip); }
  int                 Npredictions()      const { return ePredictions->N(); }

  EdbPredictionsBox   *GetPredictions() const { return ePredictions; }
  EdbMarksSet         *GetMarks()       const { return eMarks; }
  void                 SetMarks(EdbMarksSet* marks) { eMarks = marks; }
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

  int    GetEntryNumberWithIndex( int event, int view ) const
    { return eTree->GetEntryNumberWithIndex( event,view ); }

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

  int  AddAsciiFile(const char *fname, const char *objname);
  int  ExtractAsciiFile(const char *fname, const char *objname);
  
  void AddViewMerge( Int_t view1, Int_t view2, Int_t area1, Int_t area2, Int_t side1, Int_t side2,
                   Float_t dx, Float_t dy, Float_t dz, 
                   Int_t n1tot, Int_t n2tot, Int_t n1, Int_t n2, Int_t nsg, Int_t nbg, Int_t flag);

  void AddViewAlign( Int_t view1, Int_t view2, Int_t area1, Int_t area2, Int_t side1, Int_t side2,
                   Float_t dx, Float_t dy, Float_t dz, 
                   Int_t n1tot, Int_t n2tot, Int_t n1, Int_t n2, Int_t nsg, Int_t nbg, Int_t flag);
  
  void AddFrameAlign( Int_t frame1, Int_t frame2, Int_t view, Int_t area, Int_t side,
                    Float_t dxglobal, Float_t dyglobal, Float_t dx, Float_t dy, Float_t z1, Float_t z2, 
                    Int_t n1tot, Int_t n2tot, Int_t n1, Int_t n2, Int_t nsg, Int_t nbg, Int_t flag);
  void AddPinViewHeader(EdbViewHeader &h);

  ClassDef(EdbRun,4)  // main run class contained all objects
};
#endif /* ROOT_EdbRun */

