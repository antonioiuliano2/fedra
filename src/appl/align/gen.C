EdbPVRec *ali=0;
EdbPVGen *gener = 0;
EdbScanCond *scanCond=0;
TIndexCell *tracks=0;


void gen()
{
 gROOT->LoadMacro("ScanCond.C");
 gROOT->LoadMacro("IO.C");

  TFile *f = new TFile("gen.root","RECREATE");
  tree = new TTree("couples","couples");
  inittree(tree);

  ali      = new EdbPVRec();
  scanCond = new EdbScanCond();
  Set_Prototype_OPERA_microtrack( scanCond );
  ali->SetScanCond(scanCond);

  genMC((EdbPatternsVolume*)ali, 2000,120000);

  ali->GetScanCond()->Print();
  ali->SetSegmentsErrors();

  ali->SetCouplesAll();
  //ali->SetChi2Max(5.);
  //ali->SetOffsetsMax(50,50);
  //ali->Align();
  ali->Link();

  //ali->PrintAff();

  filltree(tree,ali,1);
}

///------------------------------------------------------------
void genMC(EdbPatternsVolume *pv, int nsg, int nbg)
{
  gener = new EdbPVGen();
  gener->SetVolume(pv);
  gener->SetScanCond(scanCond);

  makeVolumeOPERAud(pv);
  //makeVolumeOPERAn(pv,6);

  genPVBeam(gener,nsg);
  genPVbg(gener,nbg);
  //genPVBeam_6picchi(gener,nsg);

  gener->SmearSegments();
  
  //gener->SmearPatterns(10.,.01);

  pv->SetPatternsID();
}

///------------------------------------------------------------
void Set_MC_microtrack( EdbScanCond *cond )
{
  cond->SetSigma0( 1., 1.,.025,.025 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 5. );                // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,5,5);               // bins in [sigma] for checks
  cond->SetPulsRamp0(  4.,5. );         // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 3.,4. );         //
  cond->SetName("MC_microtrack");
}

///-------------------------------------------------------------------
void makeVolumeOPERAud( EdbPatternsVolume *v, float z0=0 )
{
  float dz=240;
  EdbPattern *pat =0;

  pat = new EdbPattern(0,0,z0);
  gener->GetVolume()->AddPattern(pat);
 
  pat = new EdbPattern(0,0,z0+dz);
  gener->GetVolume()->AddPattern(pat);
}

///-------------------------------------------------------------------
void makeVolumeOPERAn( EdbPatternsVolume *v, int npat, float z0=0 )
{
  float dz=1000;
  EdbPattern *pat =0;

  for(int i=0; i<npat; i++) {
    pat = new EdbPattern(0,0,z0+dz*i);
    gener->GetVolume()->AddPattern(pat);
  }
}

///------------------------------------------------------------
void genPVBeam( EdbPVGen *gener, int npart=100 )
{
  float xx[4] ={0,0,0,-0.05};                      // starting point, starting angle
  float sxx[4]={60000,60000,0.003,0.003};      // beam dispersions
  float lim[4]={-60000,-60000,60000.,60000.};      // xmin,ymin,xmax,ymax at z0

  float z0 = -1000.;
  int flag = 1;     //beam particles
  gener->GenerateBeam(npart,xx,sxx,lim,z0,flag);
  //gener->GeneratePulsPoisson(10.,1.,5,16,1);
  gener->GeneratePulsGaus(1,12.8,1.8,8,16,1);       // amp,mean,sigma,wmin,wmax,flag
}

///------------------------------------------------------------
void genPVbg( EdbPVGen *gener, int npart=100 )
{
  float xmin[4] ={-60000,-60000,-0.4,-0.4};              // limits
  float xmax[4]={60000,60000,0.4,0.4};                       
  int flag = 2;                                          //background particles
  gener->GenerateBackground(npart,xmin,xmax,flag);
  gener->GeneratePulsPoisson(5.,1.,8,16,2);
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

  treeCP = new TTree("couples","couples");
  treeCP->Branch("pid1",&pid1,"pid1/I");
  treeCP->Branch("pid2",&pid2,"pid2/I");
  treeCP->Branch("cp","EdbSegCouple",&cp,32000,99);
  treeCP->Branch("s1.","EdbSegP",&s1,32000,99);
  treeCP->Branch("s2.","EdbSegP",&s2,32000,99);
  treeCP->Branch("s." ,"EdbSegP",&s,32000,99);

  // **** fill tree with raw segments ****
  EdbPattern *pat=0;
  for( int ip=0; ip<al->Npatterns(); ip++ ) { 
    pat  = al->GetPattern(ip);
    pid1 = pat->ID();
    pid2 = -1;

    for( int ic=0; ic<pat->N(); ic++ ) {
      s1 = pat->GetSegment(ic);
      treeCP->Fill();
    }
  }

  // **** fill tree with couples ****
  for( int ip=0; ip<al->Ncouples(); ip++ ) {
    patc = al->GetCouple(ip);
    pid1 = patc->Pat1()->ID();
    pid2 = patc->Pat2()->ID();

    for( int ic=0; ic<patc->Ncouples(); ic++ ) {
      //      printf("%d %d\n",ip,ic);
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


///------------------------------------------------------------
void genPVBeam_6picchi( EdbPVGen *gener, int npart=100 )
{
  float ang = 0;
  float xx1[4] ={0,0,0,0};                  // starting point, starting angle

  float ang = 6;
  float xx2[4] ={0,0,TMath::DegToRad()*ang,0};

  float ang = 12;
  float xx3[4] ={0,0,TMath::DegToRad()*ang,0};

  float ang = 18;
  float xx4[4] ={0,0,TMath::DegToRad()*ang,0};

  float ang = 24;
  float xx5[4] ={0,0,TMath::DegToRad()*ang,0};

  float ang = 30;
  float xx6[4] ={0,0,TMath::DegToRad()*ang,0};

  float sxx[4]={10000,10000,0.003,0.003};      // beam dispersions
  float z0 = -1000.;
  int flag = 1;     //beam particles

  gener->GenerateBeam(npart,xx1,sxx,z0,flag);
  gener->GenerateBeam(npart,xx2,sxx,z0,flag);
  gener->GenerateBeam(npart,xx3,sxx,z0,flag);
  gener->GenerateBeam(npart,xx4,sxx,z0,flag);
  gener->GenerateBeam(npart,xx5,sxx,z0,flag);
  gener->GenerateBeam(npart,xx6,sxx,z0,flag);

  //gener->GeneratePulsPoisson(10.,1.,5,16,1);
  gener->GeneratePulsGaus(1,10,1,5,16,0);       // amp,mean,sigma,wmin,wmax,flag
}



