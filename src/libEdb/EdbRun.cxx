//-- Author :  Valeri Tioukov   30.03.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRun                                                               //
//                                                                      //
// Class for handling data of one session of emulsion scanning.         //
// Session means that all scanned data are in the same reference system //
// EdbRun is always associated with the correspondent root file
//
// EdbRun could be used in 2 ways: 
// a) to put raw scanning data
// b) to put the processed data
//
// in case a) normally new run should be creted via EdbOnline class
// 
// in case b) the following example could be used to create and fill analysis run:
//
//    EdbRun    *runIN  = new EdbRun("/row_data/run0093.root");
//    EdbRun    *runOUT = new EdbRun( *runIN,"/analysed_data/run0093_seg.root");
//    EdbView   *view = 0;
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
#include "TError.h"
#include "TSystem.h"
#include "TObjString.h"
#include "EdbRun.h"
#include "EdbAffine.h"
#include "EdbLog.h"

ClassImp(EdbRun)

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

  if(run.GetHeader()) 
    eHeader = new EdbRunHeader(*(run.GetHeader()));
  if(run.GetMarks())  
    eMarks  = new EdbMarksSet(*(run.GetMarks()));
  if(run.GetPredictions())  
    ePredictions  = new EdbPredictionsBox(*(run.GetPredictions()));
  Log(3,"EdbRun::EdbRun","Run headers are copied");
  SelectOpenMode( fname, "RECREATE" );
}

//______________________________________________________________________________
EdbRun::~EdbRun()
{
  //SafeDelete(eTree);
  SafeDelete(eFile); 
  SafeDelete(eHeader);
  SafeDelete(ePredictions);
  SafeDelete(eMarks);
  SafeDelete(eView);    // provocate crash in EdbRunAccess::CopyRawDataXY  - to understand!
}

//______________________________________________________________________________
void EdbRun::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbRun.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbRun::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      R__b >> eHeader;
      R__b >> eView;
      R__b >> eTree;
      R__b >> eFile;
      ePath.Streamer(R__b);
      R__b >> ePredictions;
      R__b >> eMarks;
      R__b.CheckByteCount(R__s, R__c, EdbRun::IsA());
      //====end of old versions
   } else {
     EdbRun::Class()->WriteBuffer(R__b,this);
   }
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
}

//______________________________________________________________________________
void EdbRun::SelectOpenMode( const char *fname, const char *status )
{
  if     ( !strcmp(status,"RECREATE") )   Create(fname);
  else if( !strcmp(status,"READ") )       Open(fname);
  else if( !strcmp(status,"UPDATE") )     OpenUpdate(fname);
  else{
    if( !gSystem->AccessPathName(fname, kFileExists) ) Open(fname);
    else                                               Create(fname);
  }

  Log(3,"EdbRun::SelectOpenMode","root file version: %d",eFile->GetVersion());
}

//______________________________________________________________________________
void EdbRun::Open( const char *fname )
{
  Log(3,"EdbRun::Open","Open an existing file %s", fname);
  eFile = new TFile(fname);
  if(!eFile) return;
  eTree = (TTree*)eFile->Get("Views");
  if(!eTree) {
    Log(1,"EdbRun::Open","ERROR: %s has no Views tree",fname);
    return;
  }

  SetView();

  SafeDelete(eHeader);
  eHeader = (EdbRunHeader*)eFile->Get("RunHeader");

  SafeDelete(ePredictions);
  ePredictions = (EdbPredictionsBox*)eFile->Get("Predictions");

  SafeDelete(eMarks);
  eMarks = (EdbMarksSet*)eFile->Get("Marks");

  if(gEDBDEBUGLEVEL>2) Print();
}

//______________________________________________________________________________
void EdbRun::OpenUpdate( const char *fname )
{
  Log(3,"EdbRun::OpenUpdate","Open an existing file for update %s", fname);
  eFile = new TFile(fname,"UPDATE");

  eTree = (TTree*)eFile->Get("Views");
  SetView();

  SafeDelete(eHeader);
  eHeader = (EdbRunHeader*)eFile->Get("RunHeader");

  SafeDelete(ePredictions);
  ePredictions = (EdbPredictionsBox*)eFile->Get("Predictions");

  SafeDelete(eMarks);
  eMarks = (EdbMarksSet*)eFile->Get("Marks");

  if(gEDBDEBUGLEVEL>2) Print();
}

//______________________________________________________________________________
void EdbRun::Create( const char *fname )
{
  if(!ePredictions) ePredictions   = new EdbPredictionsBox();
  if(!eMarks)       eMarks         = new EdbMarksSet();

  if( !gSystem->AccessPathName(fname, kFileExists) )
    Log(2,"EdbRun::Create","WARNING : file %s already exists!", fname);

  Log(2,"EdbRun::Create","Open new file %s", fname);
  eFile = new TFile(fname,"RECREATE");

  eFile->SetCompressionLevel(2);

  eTree = new TTree("Views","Scanning Viewes data");
  eTree->SetAutoSave(100000000);                     // autosave each 100Mb
  eTree->SetMaxTreeSize(32000000000LL);              //set 32 Gb file size limit
  Log(3,"EdbRun::Create","set maxtreesize as: %lld",eTree->GetMaxTreeSize());
 
  TClonesArray     *eClusters = eView->GetClusters();
  TClonesArray     *eSegments = eView->GetSegments();
  TClonesArray     *eTracks   = eView->GetTracks();
  TClonesArray     *eFrames   = eView->GetFrames();
  int savelevel = gErrorIgnoreLevel;
  gErrorIgnoreLevel = kError;
  eTree->Branch( "clusters", &eClusters);
  eTree->Branch( "segments", &eSegments);
  eTree->Branch( "tracks",   &eTracks);
  eTree->Branch( "frames",   &eFrames);
  eTree->Branch("headers", "EdbViewHeader", eView->GetHeaderAddr());
  gErrorIgnoreLevel = savelevel;
  SetView();
}

//______________________________________________________________________________
void EdbRun::SetView( EdbView *view )
{
  if(!view) { Log(1,"EdbRun::SetView","ERROR: EdbRun::SetView: *view=0");    return; }

  if(eView != view) {
    if(eView) { eView->Clear(); SafeDelete(eView); }
    eView = view;
  }

  SetView();
}

//______________________________________________________________________________
void EdbRun::SetView()
{
  if(!eView) { 
    Log(2,"EdbRun::SetView","WARNING: *eView=0");    
    eView = new EdbView();
  } 
  else if( !eView->GetClusters() ) {
    Log(1,"EdbRun::SetView","ERROR: eView was deleted!");
    eView = new EdbView();
  }

  Log(3,"EdbRun::SetView","Note: EdbRun::SetView");
  if(eView->GetHeader())   eTree->SetBranchAddress("headers",  eView->GetHeaderAddr()   );
  if(eView->GetClusters()) eTree->SetBranchAddress("clusters", eView->GetClustersAddr() );
  if(eView->GetSegments()) eTree->SetBranchAddress("segments", eView->GetSegmentsAddr() );
  if(eView->GetTracks())   eTree->SetBranchAddress("tracks",   eView->GetTracksAddr()   );
  if(eView->GetFrames())   eTree->SetBranchAddress("frames",   eView->GetFramesAddr()   );
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
    Log(3," EdbRun::AddView","WARNING: view!=eView - inefficient cycle!");
    SetView(view);
  }

  EdbViewHeader *viewHeader = eView->GetHeader();

  //Long_t oldtime = eView->GetLastSystemTime();
  //Long_t newtime = gSystem->Now(); 
  //viewHeader->SetTime( (Int_t)((ULong_t)newtime-(ULong_t)oldtime) );
  //eView->SetLastSystemTime(newtime);

  viewHeader->SetNclusters(eView->Nclusters());
  viewHeader->SetNsegments(eView->Nsegments());
  viewHeader->SetEvent( GetHeader()->GetFlag(8) );
  viewHeader->SetTrack( GetHeader()->GetFlag(9) );

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
  eFile = eTree->GetCurrentFile();
  const char *status = eFile->GetOption();

  GetFile()->cd();
  eHeader->GetFinishTime()->Set();

  if( strcmp(status,"READ") ) {             // file is in "write" mode
    if( !strcmp(status,"CREATE") ) {        // save header in CREATE mode only
      if(eHeader)        eHeader->Write("RunHeader");
      if(ePredictions)   ePredictions->Write("Predictions");
      if(eMarks)         eMarks->Write("Marks");
    }
    //eTree->BuildIndex("eAreaID","eViewID");
    eTree->Write();
  }
  eFile->Purge();
  eFile->Close();
}

//_________________________________________________________________________________
EdbView *EdbRun::GetEntry( int entry, int ih, int icl, int iseg, int itr, int ifr )
{
  if(eView)     eView->Clear();
  else          SetView();

  if( !eView->GetClusters() )  SetView();

  if(ih)   GetEntryHeader(   entry );
  if(icl)  GetEntryClusters( entry );
  if(iseg) GetEntrySegments( entry );
  if(itr)  GetEntryTracks(   entry );
  if(ifr)  GetEntryFrames(   entry );

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
  if(header)     printf("header branch exists\n");
  TClonesArray  *clusters =  GetEntryClusters( 0 );
  if(clusters)    printf("clusters branch exists\n");
  TObjArray     *frames   =  GetEntryFrames( 0 );
  if(frames)    printf("frames branch exists\n");
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
int EdbRun::AddAsciiFile(const char *fname, const char *objname)
{
  // can be used for reading  relativly small ascii files from fname and 
  // save into the current run with name objname as TObjString

  FILE *fp=fopen(fname,"r");
  if (fp==NULL)   {
    printf("ERROR open file: %s n", fname);    return(-1);
  }else
    printf( "Read Ascii file: %s\n", fname );
  TObjString  str;
  char     buf[256];
  while( fgets(buf,256,fp)!=NULL ) str.String().Append(buf);
  fclose(fp);
  return  str.Write(objname);
}

//______________________________________________________________________________
int EdbRun::ExtractAsciiFile(const char *fname, const char *objname)
{
  FILE *fp=fopen(fname,"w");
  if (fp==NULL)   {
    printf("ERROR open file: %s n", fname);    return(-1);
  }else
    printf( "Write to Ascii file: %s\n", fname );
  TObjString  *str=(TObjString *)(eFile->Get(objname));
  fprintf(fp,"%s",str->String().Data());
  fclose(fp);
  return  str->String().Sizeof();
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
      SafeDelete(aff);
    }
  }
}

//______________________________________________________________________________
void EdbRun::GeneratePredictions( int n )
{
  if(!ePredictions)    ePredictions = new EdbPredictionsBox(n);

  ePredictions->Generate(n);
}

