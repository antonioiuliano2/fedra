//-- Author :  Valeri Tioukov   30.03.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRun                                                               //
//                                                                      //
// Class for handling data of one run of emulsion scanning              //
// EdbRun is always assosiated with the correspondent root file
//
// EdbRun could be used in 2 ways: 
// a) to put row scanning data
// b) to put the processed data
//
// in case a) normally new run should be creted via EdbOnline class
// 
// in case b) the following example could be used to create and fill analysis run:
//
//    EdbRun    *runIN  = new EdbRun("/row_data/run0093.root");
//    EdbRun    *runOUT = new EdbRun( *runIN,"/analysed_data/run0093_seg.root");
//    EdbView  *view = 0;
// 
//    int N = runIN->GetEntries();
// 
//    for( int i=0; i<N; i++ ){
// 
//      view = runIN->GetEntry(i);
// 
//      ... process view as you want ...
// 
//      runOUT->AddView(view);
//    }
//
//    runOUT->Close();
//
//
//////////////////////////////////////////////////////////////////////////

#include "TBranchClones.h"
#include "TUnixSystem.h"

#ifndef ROOT_EdbRun
#include "EdbRun.h"
#endif

#ifndef ROOT_EdbAffine
#include "EdbAffine.h"
#endif

ClassImp(EdbRun)

EdbRun      *gRUN;         // global pointer in the current Run

//______________________________________________________________________________
EdbRun::EdbRun()
{
  Init();
}

//______________________________________________________________________________
EdbRun::EdbRun( int id, const char *status, const char *path )
{
  Init();

  eHeader        = new EdbRunHeader(id);

  ePath = path;

  char fname[256]="";
  sprintf(fname,"%s/run%4.4d.root",path,id);

  SelectOpenMode( fname, status );
}

//______________________________________________________________________________
EdbRun::EdbRun( const char *fname, const char *status )
{
  Init();

  eHeader        = new EdbRunHeader(0);

  SelectOpenMode( fname, status );
}

//______________________________________________________________________________
EdbRun::EdbRun( EdbRun &run, const char *fname )
{
  Init();
  EdbRunHeader &h = *run.GetHeader();
  eHeader   = new EdbRunHeader(h);
  SelectOpenMode( fname, "RECREATE" );
  printf("Run headers are copied\n");
}

//______________________________________________________________________________
void EdbRun::Init( )
{
  eView        = new EdbView();      //each run has his own view

  eHeader      = 0;
  eTree        = 0;
  eFile        = 0;
  ePath        = "";
  ePredictions = 0;
  eMarks       = 0;
  gRUN      = this;
}

//______________________________________________________________________________
void EdbRun::SelectOpenMode( const char *fname, const char *status )
{
  if     ( !strcmp(status, "RECREATE") )  Create(fname);
  else if( !strcmp(status,"READ") )       Open(fname);
  else if( !strcmp(status,"UPDATE") )     OpenUpdate(fname);
  else{
    if( !gSystem->AccessPathName(fname, kFileExists) ) Open(fname);
    else                                               Create(fname);
  }
}

//______________________________________________________________________________
void EdbRun::Open( const char *fname )
{
  printf("\nOpen an existing file %s \n", fname);
  eFile = new TFile(fname);

  eTree = (TTree*)eFile->Get("Views");
  SetView();

  if(eHeader) delete eHeader;
  eHeader = (EdbRunHeader*)eFile->Get("RunHeader");

  if(ePredictions) delete ePredictions;
  ePredictions = (EdbPredictionsBox*)eFile->Get("Predictions");

  if(eMarks) delete eMarks;
  eMarks = (EdbMarksSet*)eFile->Get("Marks");

  Print();
}

//______________________________________________________________________________
void EdbRun::OpenUpdate( const char *fname )
{
  printf("\nOpen an existing file for updation %s \n", fname);
  eFile = new TFile(fname,"UPDATE");

  eTree = (TTree*)eFile->Get("Views");
  SetView();

  if(eHeader) delete eHeader;
  eHeader = (EdbRunHeader*)eFile->Get("RunHeader");

  if(ePredictions) delete ePredictions;
  ePredictions = (EdbPredictionsBox*)eFile->Get("Predictions");

  if(eMarks) delete eMarks;
  eMarks = (EdbMarksSet*)eFile->Get("Marks");

  Print();
}

//______________________________________________________________________________
void EdbRun::Create( const char *fname )
{
  ePredictions   = new EdbPredictionsBox();
  eMarks         = new EdbMarksSet();

  if( !gSystem->AccessPathName(fname, kFileExists) )
    printf("WARNING : file %s  is already exist!\n", fname);

  printf("\nOpen new file %s \n\n", fname);
  eFile = new TFile(fname,"RECREATE");

  eFile->SetCompressionLevel(2);

  eTree = new TTree("Views","Scanning Viewes data");
  eTree->SetAutoSave(100000000);                     // autosave each 100Mb
  int bufsize=32000;

  eTree->Branch("clusters",  eView->GetClustersAddr() , bufsize, 1);
  eTree->Branch("segments",  eView->GetSegmentsAddr() , bufsize, 1);
  eTree->Branch("tracks",    eView->GetTracksAddr() , bufsize, 1);
  eTree->Branch("frames",    eView->GetFramesAddr() , bufsize, 1);

  eTree->Branch("headers", "EdbViewHeader", eView->GetHeaderAddr() , bufsize, 1);

  //  TObjArray *frames = new TObjArray();
  //  eTree->Branch("frames", "TObjArray", eView->GetFramesAddr() , bufsize*4, 0);
}

//______________________________________________________________________________
void EdbRun::SetView( EdbView *view )
{
  if(!view) { printf("ERROR: EdbRun::SetView: *view=0\n");    return; }

  if(eView != view) {
    if(eView) { eView->Clear(); delete eView; }
    eView = view;
  }

  SetView();
}

//______________________________________________________________________________
void EdbRun::SetView()
{
  if(!eView) { 
    printf("WARNING: EdbRun::SetView: *eView=0\n");    
    eView = new EdbView();
  } 
  else if( !eView->GetClusters() ) {
    printf("ERROR: EdbRun::SetView: eView was deleted!\n");
    eView = new EdbView();
  }

  printf("Note: EdbRun::SetView \n");

  eTree->SetBranchAddress("headers",  eView->GetHeaderAddr() );
  eTree->SetBranchAddress("clusters", eView->GetClustersAddr() );
  eTree->SetBranchAddress("segments", eView->GetSegmentsAddr() );
  eTree->SetBranchAddress("tracks",   eView->GetTracksAddr() );
}

//______________________________________________________________________________
void EdbRun::AddView()
{
  AddView(eView);
}

//______________________________________________________________________________
void EdbRun::AddView( EdbView *view )
{

  if( view != eView ) { 
    printf("WARNING: EdbRun::AddView: view!=eView - inefficient cycle!\n");
    SetView(view);
  }

  EdbViewHeader *viewHeader = eView->GetHeader();

  Long_t oldtime = eView->GetLastSystemTime();
  Long_t newtime = gSystem->Now(); 
  viewHeader->SetTime( (Int_t)((ULong_t)newtime-(ULong_t)oldtime) );
  eView->SetLastSystemTime(newtime);

  viewHeader->SetNclusters(eView->Nclusters());
  viewHeader->SetNsegments(eView->Nsegments());

  eTree->Fill();
  eView->Clear();
}

//______________________________________________________________________________
void EdbRun::Save()
{
  if(eHeader)        eHeader->Write("RunHeader");
  if(ePredictions)   ePredictions->Write("Predictions");
  if(eMarks)         eMarks->Write("Marks");
  //SaveViews();
  //eFile->Purge();
}

//______________________________________________________________________________
void EdbRun::Close()
{
  const char *status = eFile->GetOption();
  printf("File status: %s\n",status);

  eHeader->GetFinishTime()->Set();
  //*** eHeader->SetCPU( TStopwatch::GetCPUTime() );

  //  printf("real time: %f CPU: %f\n", timer->GetRealTime(),timer->GetCPUTime() );

  if( strcmp(status,"READ") ) {             // file is in "write" mode
    if( !strcmp(status,"CREATE") ) {        // save header in CREATE mode only
      if(eHeader)        eHeader->Write("RunHeader");
      if(ePredictions)   ePredictions->Write("Predictions");
      if(eMarks)         eMarks->Write("Marks");
    }
    eTree->BuildIndex("eAreaID","eViewID");
    eTree->Write();
  }
  //  eFile->ls();
  eFile->Purge();
  eFile->Close();
}

//______________________________________________________________________________
EdbView *EdbRun::GetEntry( int entry )
{
  if(eView)     eView->Clear();
  else          SetView();

  if( !eView->GetClusters() )  SetView();

  GetEntryHeader(   entry );
  GetEntryClusters( entry );
  GetEntrySegments( entry );
  //GetEntryTracks(   entry );
  //GetEntryFrames(   entry );

  return eView;
}

//______________________________________________________________________________
TClonesArray *EdbRun::GetEntryTracks( int entry ) const
{
  TClonesArray  *tracks=eView->GetTracks();
  TBranchClones *tracksBranch = (TBranchClones*)(eTree->GetBranch("tracks"));
  if(!tracksBranch)                                            return 0;

  tracksBranch->ResetReadEntry();
  tracksBranch->SetAddress(&tracks);
  tracksBranch->GetEntry(entry);

  return tracks;
}

//______________________________________________________________________________
TClonesArray *EdbRun::GetEntrySegments( int entry ) const
{
  TClonesArray *segments = eView->GetSegments();
  TBranchClones *segmentsBranch = (TBranchClones*)(eTree->GetBranch("segments"));
  if(!segmentsBranch)                                            return 0;

  segmentsBranch->ResetReadEntry();
  segmentsBranch->SetAddress(&segments);
  segmentsBranch->GetEntry(entry);

  return segments;
}

//______________________________________________________________________________
TClonesArray *EdbRun::GetEntryClusters( int entry ) const
{
  TClonesArray *clusters = eView->GetClusters();
  TBranchClones *clustersBranch = (TBranchClones*)(eTree->GetBranch("clusters"));
  if(!clustersBranch)                                            return 0;
  //clustersBranch->SetAutoDelete( kTRUE );
  clustersBranch->ResetReadEntry();
  clustersBranch->SetAddress(&clusters);
  clustersBranch->GetEntry(entry);

  return clusters;
}

//______________________________________________________________________________
EdbViewHeader *EdbRun::GetEntryHeader( int entry ) const
{
  EdbViewHeader *header = eView->GetHeader();
  TBranch *headerBranch = eTree->GetBranch("headers");
  if(!headerBranch)                                            return 0;
  headerBranch->SetAddress(&header);
  headerBranch->GetEntry(entry);
  //  eTree->GetEntry(entry);

  return header;
}

//______________________________________________________________________________
TObjArray *EdbRun::GetEntryFrames( int entry )  const
{
  TObjArray *frames = eView->GetFrames();
  TBranch *framesBranch = eTree->GetBranch("frames");
  if(!framesBranch)                                            return 0;
  framesBranch->SetAddress(&frames);
  framesBranch->GetEntry(entry);

  return frames;
}

//______________________________________________________________________________
void EdbRun::PrintBranchesStatus() const
{
  // not supported...

  EdbViewHeader *header   =  GetEntryHeader( 0 );
  if(header)      printf("header branch exist\n");
  TClonesArray  *clusters =  GetEntryClusters( 0 );
  if(clusters)    printf("clusters branch exist\n");
  TObjArray     *frames   =  GetEntryFrames( 0 );
  if(frames)    printf("frames branch exist\n");
}

//______________________________________________________________________________
void EdbRun::PrintLog( const char *fname ) const
{
  char buf[256];

  sprintf( buf,"run%4.4d  %s \t%s \t%s",
	   GetRunID(),
	   eHeader->GetName(),
	   eHeader->GetPlate()->GetName(),
	   eHeader->GetTitle()	   );

  printf("**************************************************************************\n");
  printf("\n%s\n", buf);
  printf("**************************************************************************\n");

  FILE *fp = fopen( fname,"a" );
  fprintf(fp,"%s", buf);
  fclose(fp);

}

//______________________________________________________________________________
void EdbRun::Print() const
{
  if(eHeader) eHeader->Print();

  int nentr=0;
  if(eTree) nentr = (Int_t)eTree->GetEntries();
  printf("Number of entries: \t %d \n", nentr);
  printf("--------------------------------------------------------------------\n\n");

  //PrintBranchesStatus();
}


//______________________________________________________________________________
void EdbRun::TransformDC()
{
  if(ePredictions){
    if(eMarks){
      EdbAffine2D *aff = eMarks->Abs2Stage();
      ePredictions->Transform(aff);
      printf("transfrom predictions according to fiducial marks:\n");
      aff->Print();
      delete aff;
    }
  }
}

//______________________________________________________________________________
void EdbRun::GeneratePredictions( int n )
{
  if(!ePredictions)    ePredictions = new EdbPredictionsBox(n);

  ePredictions->Generate(n);
}
