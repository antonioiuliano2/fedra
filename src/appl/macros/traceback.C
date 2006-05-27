//---------------------------------------------------------------------
// in the couples tree: 
// 
// s1.eAid[0] - area ID of the raw segment (in Run tree)
// s1.eAid[1] - view ID in this area of the raw segment (in Run tree)
//
// s1.eVid[0] - entry (view object) ID in the Run tree
// s1.eVid[1] - entry id of the segment in this view
//---------------------------------------------------------------------

EdbRun  *run;
TTree   *tree_s1;
TTree   *tree_s2;

EdbSegCouple *cp=0;
EdbSegP      *s1=0;
EdbSegP      *s2=0;
EdbSegP      *s =0;

//------------------------------------------------------------------------------------
void traceback()
{
  //  in this example is illustrated how to find raw segments for a given linked one  
  //  and vice versa

  init();

  EdbView *view = run->GetView();
  EdbSegment *seg;

  int nentr = (int)(tree_s1->GetEntries());
  for(int i=0; i<20; i++ ) {

    printf("\ns1 (TOP):\n");
    tree_s1->GetEntry(i);
    printf("by using Vid of EdbSegP: %f %f %f %f\n", s1->TX(),s1->TY(),s1->W(), s1->Z()); 
    seg = getRawSeg(s1->Vid(0), s1->Vid(1));
    printf(  "find raw EdbSegment    : %f %f %f %f\n", seg->GetTx(),seg->GetTy(),seg->GetPuls(),seg->GetZ0()); 
    EdbSegP *ss1=getS1( view->GetAreaID(), view->GetViewID(), seg->GetID() );
    printf(  "and then get again s1  : %f %f %f %f\n", ss1->TX(),ss1->TY(),ss1->W(), ss1->Z());

    printf("\ns2 (BOTTOM):\n");
    tree_s2->GetEntry(i);
    printf("by using Vid of EdbSegP: %f %f %f %f\n", s2->TX(),s2->TY(),s2->W(),s2->Z()); 
    seg = getRawSeg(s2->Vid(0), s2->Vid(1));
    printf(  "find raw EdbSegment    : %f %f %f %f\n", seg->GetTx(),seg->GetTy(),seg->GetPuls(),seg->GetZ0()); 
    EdbSegP *ss2=getS2( view->GetAreaID(), view->GetViewID(), seg->GetID() );
    printf(  "and then get again s2  : %f %f %f %f\n", ss2->TX(),ss2->TY(),ss2->W(),ss2->Z());
  }
}

//------------------------------------------------------------------------------------
void drawXYview()
{
  // plot position of linked microtrack in view RS

  TH2F *top = new TH2F("top","top",180, -180.,180., 150, -150.,150.);
  TH2F *bot = new TH2F("bot","bot",180, -180.,180., 150, -150.,150.);

  init();

  EdbView *view = run->GetView();
  EdbSegment *seg;

  int nentr = (int)(tree_s1->GetEntries());
  for(int i=0; i<nentr; i++ ) {
    tree_s1->GetEntry(i);
    seg = getRawSeg(s1->Vid(0), s1->Vid(1));
    top->Fill( seg->GetX0(), seg->GetY0() );
    tree_s2->GetEntry(i);
    seg = getRawSeg(s2->Vid(0), s2->Vid(1));
    bot->Fill( seg->GetX0(), seg->GetY0() );
  }
  TCanvas *c1 = new TCanvas("c1");
  top->Draw();
  TCanvas *c2 = new TCanvas("c2");
  bot->Draw();
}

//------------------------------------------------------------------------------------
void init( char *frun="data/seg.root",
	   char *fcp ="data/02_011.cp.root")
{
  run = new EdbRun(frun);

  TFile *file_s1 = new TFile(fcp);
  tree_s1 = (TTree*)file_s1->Get("couples");
  tree_s1->SetBranchAddress("s1."  , &s1  );
  tree_s1->BuildIndex("s1.eVid[0]","s1.eVid[1]");

  TFile *file_s2 = new TFile(fcp);
  tree_s2 = (TTree*)file_s2->Get("couples");
  tree_s2->SetBranchAddress("s2."  , &s2  );
  tree_s2->BuildIndex("s2.eVid[0]","s2.eVid[1]");

  int nentr = (int)(tree_s1->GetEntries());
  printf("nentr = %d\n",nentr);
}

//------------------------------------------------------------------------------------
EdbSegment *getRawSeg(int vid, int sid)
{
  view = run->GetEntry(vid);
  return  view->GetSegment(sid);
}

//------------------------------------------------------------------------------------
EdbSegP *getS1(int areaID=28, int viewID=56, int segID=74 )
{
  int entry = run->GetEntryNumberWithIndex(areaID,viewID);
  tree_s1->GetEntryWithIndex(entry,segID);
  //s1->Print();
  return s1;
}

//------------------------------------------------------------------------------------
EdbSegP *getS2(int areaID=28, int viewID=56, int segID=74 )
{
  int entry = run->GetEntryNumberWithIndex(areaID,viewID);
  tree_s2->GetEntryWithIndex(entry,segID);
  //s2->Print();
  return s2;
}

//------------------------------------------------------------------------------------
