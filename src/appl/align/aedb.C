EdbPVRec    *ali=0;

//------------------------------------------------------------
int cut_seg(float x0, float y0, float tx0=0, float ty0=0, float puls=0)
{
  // this function is called by GetDataEdb() to reject bad segments

  if( TMath::Abs(y0- 139.70)<1.35 && TMath::Abs(x0+  64.45)<1.4)   return 0;
  if( TMath::Abs(y0+ 158.2 )<1.3  && TMath::Abs(x0+ 108.55)<1.3)   return 0;
  if( TMath::Abs(y0- 139.38)<1.5  && TMath::Abs(x0+  44.95)<1.5)   return 0;
  if( TMath::Abs(y0+ 121.30)<1.35 && TMath::Abs(x0- 104.45)<1.35)  return 0;
  if( TMath::Abs(y0+  13.90)<1.45 && TMath::Abs(x0- 118.50)<1.35)  return 0;
  if( TMath::Abs(y0+ 143.75)<1.35 && TMath::Abs(x0- 133.25)<1.35)  return 0;
  return 1;
}

//------------------------------------------------------------
void aedb()
{
  gROOT->LoadMacro("ScanCond.C");
  gROOT->LoadMacro("IO.C");

  EdbRun *edbRun =  new EdbRun(
			       //    "/mnt/operalabdb_e/data/rawr/b1_jun2003/pl09/raw_Jun2003_01010109.root"
     "/mnt/operalabdb_e/data/rawr/b1_sep2002/PL03/Sept2002_pl3_1.root"
    ,"READ");

  float  shrU  = 1./1.1;       //
  float  shrD  = 1./1.2;       //
  float  uOff[2] = {0.,0.};    // s1.eTX-s.eTX, s1.eTY-s.eTY  (at 0 angle)
  float  dOff[2] = {0.,0.};    // s2.eTX-s.eTX, -(s2.eTY-s.eTY)

  TTree *tree = inittree("couples",
			 "aedb.root",
			 "RECREATE");  // "RECREATE" "NEW" or "UPDATE" open modes

  EdbScanCond *scanCond = new EdbScanCond();
  Set_Prototype_OPERA_microtrack( scanCond );
  EdbPatternsVolume *pvol = (EdbPatternsVolume *)ali;

  int nareas= edbRun->GetHeader()->GetNareas();

  for(int aid=0; aid<nareas; aid++) {
  
    ali      = new EdbPVRec();
    ali->SetScanCond(scanCond);

    getDataEdb( edbRun, ali,   aid, shrU, shrD, uOff, dOff );

    ali->GetScanCond()->Print();
    ali->SetSegmentsErrors();

    ali->SetCouplesAll();
    ali->SetChi2Max(2.5);
    ali->Link();

    filltree(tree, ali);
    delete pvol;
    delete ali;
  }

}



//-------------------------------------------------------------------------------

//    edbRun = new EdbRun("/scratch/opera/Jul2002/04010301.root","READ"); //shr =1
//    float  shrU  = 1.30;       //
//    float  shrD  = 1.25;       //
//    float  uOff[2] = {0.005,-0.001};  // s1.eTX-s.eTX, s1.eTY-s.eTY  (at 0 angle)
//    float  dOff[2] = {0.013,0.011};   // s2.eTX-s.eTX, s2.eTY-s.eTY



  //edbRun = new EdbRun("/scratch/gabriele/acq24_00000003000000030000000300000003.root","READ");
  //edbRun = new EdbRun("/scratch/gabriele/acq25_00000002000000020000000200000002.root","READ");
  //edbRun = new EdbRun("/scratch/gabriele/acq26_00000001000000010000000100000001.root","READ");
  //edbRun = new EdbRun("/scratch/gabriele/acq27_00000004000000040000000400000004.root","READ");
//float  shrU  = 1.1;         //
//float  shrD  = 1.15;         //
//float  uOff[2] = {0.,0.};  // s1.eTX-s.eTX, s1.eTY-s.eTY  (at 0 angle)
//float  dOff[2] = {0.,0.};  // s2.eTX-s.eTX, s2.eTY-s.eTY


//    edbRun = new EdbRun("/scratch/opera/Jul2002/data_BO_plate11.root","READ"); //shr =1
//    float  shrU  = 1.11;       //
//    float  shrD  = 0.98;       //
//    float  uOff[2] = {0.,0.};  // s1.eTX-s.eTX, s1.eTY-s.eTY  (at 0 angle)
//    float  dOff[2] = {0.,0.010};   // s2.eTX-s.eTX, -(s2.eTY-s.eTY)

//    edbRun = new EdbRun("/home/valeri/luillo/acq27_prova000007D3000000050000001B00000672.root","READ"); //shr =1
//    float  shrU  = 1.45;       //
//    float  shrD  = 1.4;       //
//    float  uOff[2] = {0.,0.};  // s1.eTX-s.eTX, s1.eTY-s.eTY  (at 0 angle)
//    float  dOff[2] = {0.,0.0};   // s2.eTX-s.eTX, -(s2.eTY-s.eTY)
