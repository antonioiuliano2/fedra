using namespace VERTEX;
using namespace MATRIX;

EdbPVRec    *ali=0;
EdbPVGen    *gener = 0;
EdbScanCond *scanCond=0;
EdbScanCond *scanCond_mc=0;
TIndexCell  *tracks=0;

void gen( int nstat )
{
  gROOT->LoadMacro("ScanCond.C");

  ali      = new EdbPVRec();
  scanCond = new EdbScanCond();
  Set_Prototype_OPERA_basetrack( scanCond );
  ali->SetScanCond(scanCond);

  genMC((EdbPatternsVolume*)ali, nstat);

  VertexMC();
}

///------------------------------------------------------------
void Set_Prototype_OPERA_basetrack_mc( EdbScanCond *cond )
{
  cond->SetSigma0( 5.0, 5.0,.004,.004 );   // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 5. );                 // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,5,5);                // bins in [sigma] for checks
  cond->SetPulsRamp0(  15.,15. );        // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 15.,15. );        //
  cond->SetName("Prototype_OPERA_basetrack");
}

///------------------------------------------------------------
void genMC(EdbPatternsVolume *pv, int nstat)
{
  gener = new EdbPVGen();
  gener->SetVolume(pv);
  scanCond_mc = new EdbScanCond();
  Set_Prototype_OPERA_basetrack_mc(scanCond_mc);
  gener->SetScanCond(scanCond_mc);

  makeVolumeOPERAud(pv);

  genPVBeam(gener, nstat);

//  gener->SmearSegments();
  
//  pv->SetPatternsID();
}

///-------------------------------------------------------------------
void makeVolumeOPERAud( EdbPatternsVolume *v, float z0=0 )
{
  EdbPattern *pat =0;

  pat = new EdbPattern(0,0,z0);
  gener->GetVolume()->AddPattern(pat);
 
}

///------------------------------------------------------------
void genPVBeam( EdbPVGen *gener, int nstat)
{
  float ang = 0.;
  float xx1[4] ={vx_mc,vy_mc,0.,0.};                  // starting point, starting angle

  float ang = 6.;
  float xx2[4] ={vx_mc,vy_mc,TMath::DegToRad()*ang,0.};

  float ang = 12.;
  float xx3[4] ={vx_mc,vy_mc,TMath::DegToRad()*ang,0.};

  float ang = 18.;
  float xx4[4] ={vx_mc,vy_mc,TMath::DegToRad()*ang,0.};

  float ang = 24.;
  float xx5[4] ={vx_mc,vy_mc,TMath::DegToRad()*ang,0.};

  float ang = 30.;
  float xx6[4] ={vx_mc,vy_mc,TMath::DegToRad()*ang,0.};

  float sxx[4]={0.,0.,0.,0.};      // beam dispersions
  float lim[4]={-10000000.,-10000000.,+10000000.,+10000000.};      // beam dispersions
  float z0 = vz_mc;
  int flag = 1;     //beam particles

  for (int i = 0; i < nstat; i++)
  {
    gener->GenerateBeam(1,xx1,sxx,lim,z0,i+1000);
    gener->GenerateBeam(1,xx2,sxx,lim,z0,i+1000);
    gener->GenerateBeam(1,xx3,sxx,lim,z0,i+1000);
    gener->GenerateBeam(1,xx4,sxx,lim,z0,i+1000);
    gener->GenerateBeam(1,xx5,sxx,lim,z0,i+1000);
    gener->GenerateBeam(1,xx6,sxx,lim,z0,i+1000);
  }
}
