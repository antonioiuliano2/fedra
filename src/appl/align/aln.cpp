//----------------------------------------------------------------------------
//
//  test alignment
//
//  VT: 10.04.2003
//----------------------------------------------------------------------------
#include "TIndexCell.h"
#include "EdbAffine.h"
#include "EdbVirtual.h"
#include "EdbRun.h"
#include "EdbSegment.h"
#include "EdbPVRec.h"

EdbPVRec    *ali=0;
TTree       *treeCP=0;   //couples tree
EdbScanCond *scanCond=0;

///------------------------------------------------------------
void Set_UTS_OPERA_microtrack( EdbScanCond *cond )
{
  cond->SetSigma0( 1., 1.,.025,.025 );    // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 1. );                  // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,1,1);                 // bins in [sigma] for checks
  cond->SetPulsRamp0(  60000.,100000. );  // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 50000., 90000. );  //
  cond->SetName("MC_basetrack");
}

//--------------------------------------------------------------
void getDataEdb( EdbRun &edbRun,
                 int aid, float shr,
                 EdbPVRec *pvol )
{
  EdbPattern *patU = 0;
  EdbPattern *patD = 0;
  EdbSegP    *segP = new EdbSegP();

  EdbView    *view=edbRun.GetView();
  EdbSegment *seg=0;

  patU = new EdbPattern();
  patU->SetZ(0);
  patD = new EdbPattern();
  patD->SetZ(200);

  for(int iv=0; iv<edbRun.GetEntries(); iv++ ) {

    view = edbRun.GetEntry(iv);

    if( view->GetAreaID() != aid )   continue;

    for(int j=0;j<view->Nsegments();j++) {
      
      seg = view->GetSegment(j);

      //if( seg->GetTx() > 0.15) continue;
      //if( TMath::Abs(seg->GetTy()) > 0.25) continue;

      segP->Set( seg->GetID(),
                 seg->GetX0(),
                 seg->GetY0(),
                 seg->GetTx()*shr,
                 seg->GetTy()*shr );
      segP->SetW(seg->GetPuls());

      if(seg->GetSide()==1) 	{
	segP->SetZ( patU->Z() );
	patU->AddSegment( *segP);
      }
      if(seg->GetSide()==2) 	{
	segP->SetZ( patD->Z() );
	patD->AddSegment( *segP);
      }

    }
  }

  pvol->AddPattern(patU);
  pvol->AddPattern(patD);

}

///------------------------------------------------------------
void inittree( )
{
  int pid1,pid2;
  EdbSegCouple *cp=0;
  EdbSegP      *s1=0;
  EdbSegP      *s2=0;
  EdbSegP      *s=0;

  treeCP = new TTree("couples","couples");
  treeCP->Branch("pid1",&pid1,"pid1/I");
  treeCP->Branch("pid2",&pid2,"pid2/I");
  treeCP->Branch("cp","EdbSegCouple",&cp,32000,99);
  treeCP->Branch("s1.","EdbSegP",&s1,32000,99);
  treeCP->Branch("s2.","EdbSegP",&s2,32000,99);
  treeCP->Branch("s." ,"EdbSegP",&s,32000,99);
}

///------------------------------------------------------------
void maketree( EdbPVRec *al )
{
  EdbPatCouple *patc=0;

  int pid1,pid2;
  EdbSegCouple *cp=0;
  EdbSegP      *s1=0;
  EdbSegP      *s2=0;
  EdbSegP      *s=0;

  treeCP->SetBranchAddress("pid1",&pid1);
  treeCP->SetBranchAddress("pid2",&pid2);
  treeCP->SetBranchAddress("cp"  ,&cp);
  treeCP->SetBranchAddress("s1." ,&s1);
  treeCP->SetBranchAddress("s2." ,&s2);
  treeCP->SetBranchAddress("s."  ,&s );

  // **** fill tree with raw segments ****
  EdbPattern *pat=0;
  for( int ip=0; ip<al->Npatterns(); ip++ ) { 
    pat  = al->GetPattern(ip);
    pid1 = pat->ID();
    pid2 = -1;
    printf("maketree: pattern %d of %d has %d segments\n",ip,al->Npatterns(),pat->N());

    for( int ic=0; ic<pat->N(); ic++ ) {
      s1 = pat->GetSegment(ic);
      treeCP->Fill();
    }
  }


  for( int ip=0; ip<al->Ncouples(); ip++ ) {
    patc = al->GetCouple(ip);
    pid1 = patc->Pat1()->ID();
    pid2 = patc->Pat2()->ID();

    for( int ic=0; ic<patc->Ncouples(); ic++ ) {
      cp = patc->GetSegCouple(ic);
      s1 = patc->Pat1()->GetSegment(cp->ID1());
      s2 = patc->Pat2()->GetSegment(cp->ID2());
      s = new EdbSegP(*s1);
      *s += *s2;
      treeCP->Fill();
      delete s;
    }
  }

  treeCP->Write();

}

///------------------------------------------------------------------------------------
void align10edb()
{

  EdbRun  edbRun("/home/valeri/public/ns2root/src/appl/fxx2edb/fxx_10.25.root","READ");
  TFile *f = new TFile("10edb.root","RECREATE");
  inittree();

  scanCond = new EdbScanCond();
  Set_UTS_OPERA_microtrack( scanCond );

  //for(int aid=100001; aid<100036; aid++) {
  for(int aid=100001; aid<100002; aid++) {

    ali = new EdbPVRec();
    ali->SetScanCond(scanCond);
 
    float  shr  = 1.3;    //30/dz1;
    getDataEdb( edbRun,    aid, shr, ali );

    ali->SetPatternsID();

    scanCond->Print();
    ali->SetSegmentsErrors();

    EdbPatCouple *c = new EdbPatCouple();
    c->SetID(0,1);
    c->SetOffset(0,0,0,0);       // expected offsets (max)
    c->SetSigma(1,1,.003,.003);  // the expected fraction of pat<->pat error due to deformations,etc
    c->SetCond(scanCond);
    ali->AddCouple(c);
    ali->SetCouples();
    c->FindOffset01(50,50);
    //ali->Link();

    maketree(ali);
    delete ali;
  }

  f->Purge();
}


//------------------------------------------------------------
int main()
{
  align10edb();
  return 0;
}
