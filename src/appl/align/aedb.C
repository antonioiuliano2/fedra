//**:  /home/pistillo/data03010111.root
// peak1    = "eX0>-44.8&&eX0<-44.2&&eY0>139.2&&eY0<139.8";
// notpeak1 = "((eX0<-44.8||eX0>-44.2)&&(eY0<139.2||eY0>139.8))";
// peak2    = "eX0>120.6&&eX0<121.2&&eY0>-163.8&&eY0<-163.2";
// notpeak2 = "((eX0<120.6||eX0>121.2)&&(eY0<-163.8||eY0>-163.2))";

TIndexCell  *tracks=0;
TTree       *tree=0;   //couples tree
EdbPVRec    *ali=0;
EdbRun      *edbRun =0;
EdbScanCond *scanCond =0;


//------------------------------------------------------------
void aedb()
{
  gROOT->LoadMacro("ScanCond.C");
  gROOT->LoadMacro("IO.C");


  edbRun = new EdbRun("/home/valeri/luillo/acq27_prova000007D3000000050000001B00000672.root","READ"); //shr =1
  float  shrU  = 1.45;       //
  float  shrD  = 1.4;       //
  float  uOff[2] = {0.,0.};  // s1.eTX-s.eTX, s1.eTY-s.eTY  (at 0 angle)
  float  dOff[2] = {0.,0.0};   // s2.eTX-s.eTX, -(s2.eTY-s.eTY)


  TFile *f = new TFile("aedb.root","RECREATE");
  tree = new TTree("couples","couples");
  inittree(tree);

  scanCond = new EdbScanCond();
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
    f->Purge();
  }

}

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
