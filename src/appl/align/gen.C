EdbPVRec    *ali=0;
EdbPVGen    *gener = 0;
EdbScanCond *scanCond=0;
EdbScanCond *scanCond_mc=0;
TIndexCell  *tracks=0;

void gen()
{
 gROOT->LoadMacro("ScanCond.C");
 gROOT->LoadMacro("IO.C");

  TFile *f = new TFile("gen.root","RECREATE");

  TTree *tree = inittree("couples",
                         "gen.root",
                         "RECREATE");

  ali      = new EdbPVRec();
  scanCond = new EdbScanCond();
  Set_Prototype_OPERA_basetrack( scanCond );
  ali->SetScanCond(scanCond);

  genMC((EdbPatternsVolume*)ali, 1000,0);

  ali->PrintAff();

  ali->GetScanCond()->Print();
  ali->SetSegmentsErrors();

  ali->SetCouplesAll();
  ali->SetChi2Max(15);
  ali->SetOffsetsMax(60,60);
  ali->Align();
  ali->PrintAff();
  ali->Link();

  filltree(tree,ali,1);
}

///------------------------------------------------------------
void Set_Prototype_OPERA_basetrack_mc( EdbScanCond *cond )
{
  cond->SetSigma0( 5., 5.,.004,.004 );   // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 5. );                 // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,5,5);                // bins in [sigma] for checks
  cond->SetPulsRamp0(  15.,15. );        // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 15.,15. );        //
  cond->SetName("Prototype_OPERA_basetrack");
}

///------------------------------------------------------------
void genMC(EdbPatternsVolume *pv, int nsg, int nbg)
{
  gener = new EdbPVGen();
  gener->SetVolume(pv);
  scanCond_mc = new EdbScanCond();
  Set_Prototype_OPERA_basetrack_mc(scanCond_mc);
  gener->SetScanCond(scanCond_mc);

  //makeVolumeOPERAud(pv);
  makeVolumeOPERAn(pv,5);

  genPVBeam(gener,nsg);
  genPVbg(gener,nbg);
  //genPVBeam_6picchi(gener,nsg);

  gener->SmearSegments();
  
  gener->SmearPatterns(20.,.001);

  pv->SetPatternsID();
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
  float xx[4] ={0,0,0,-0.05};                   // starting point, starting angle
  float sxx[4]={60000,60000,0.003,0.003};       // beam dispersions
  float lim[4]={-60000,-60000,60000.,60000.};   // xmin,ymin,xmax,ymax at z0

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



