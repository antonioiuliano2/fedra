EdbPVRec    *ali=0;

//------------------------------------------------------------
void align()
{
  gROOT->LoadMacro("ScanCond.C");
  gROOT->LoadMacro("IO.C");

  EdbScanCond *scanCond = new EdbScanCond();
  Set_Prototype_OPERA_basetrack( scanCond );
  EdbPatternsVolume *pvol = (EdbPatternsVolume *)ali;

  ali      = new EdbPVRec();
  ali->SetScanCond(scanCond);

  getNapoliSepData(ali);

  ali->SetPatternsID();
  ali->GetScanCond()->Print();
  ali->SetSegmentsErrors();

  ali->SetCouplesAll();
  ali->SetChi2Max(5.);
  ali->SetOffsetsMax(200,200);

  ali->Align();

  //EdbAffine2D a;
  //a.Set(0.999125, -0.000185, 0.000264, 0.999657,    -13.793780, 42.138744);
  //ali->GetPattern(0)->Transform(&a);

  ali->Link();
  ali->PrintAff();

  TTree *tree = inittree("couples",
			 "align.root",
			 "RECREATE");



  filltree(tree, ali,1);
}

//------------------------------------------------------------
int cut_seg_bern(float x0, float y0, float tx0, float ty0, float puls )
{
  if( TMath::Abs(tx0) < .1 ) return 0;
  if( TMath::Abs(ty0) > .1 ) return 0;
  if( puls < 15 )    return 0;

  return 1;
}

//------------------------------------------------------------
int cut_seg_nap(float x0, float y0, float tx0, float ty0, float puls )
{
  if( x0 >  50000. )  return 0;
  if( y0 >  40000. )  return 0;

  return 1;
}

//------------------------------------------------------------
int cut_seg_cp(float x0, float y0, float tx0, float ty0, float puls )
{
  // this function is called by GetDataCouples() to reject bad segments
  return    cut_seg_nap(x0,y0,tx0,ty0,puls);
  return 1;
}

//-------------------------------------------------------------------------------
void getNapoliData(EdbPVRec    *al)
{
  EdbPattern *pat=0;
  pat = new EdbPattern(0.,0., 0. );
  getDataCouples(
	 "/mnt/operalabdb_e/data/proc/b1_jun2003/pl14/lnk_Jun2003_02010114.root"
	 ,"couples", pat );
  al->AddPattern(pat);


  pat = new EdbPattern(0.,0., -285. );
  getDataCouples(
      "/mnt/operalabdb_e/data/proc/b1_jun2003/pl15/lnk_Jun2003_02010115.root"
      ,"couples", pat );

  al->AddPattern(pat);
}

//-------------------------------------------------------------------------------
void getNapoliSepData(EdbPVRec    *al)
{
  EdbPattern *pat=0;
  pat = new EdbPattern(0.,0., 0. );
  getDataCouples(
		 "link_pl1_1.root"
		 //"/home/dicapua/fedra/src/appl/align/pl01.root"
	 ,"couples", pat );
  al->AddPattern(pat);


  pat = new EdbPattern(0.,0., 1300. );
  getDataCouples(
		 "link_pl2_1.root"
		 //"/home/dicapua/fedra/src/appl/align/pl02.root"
      ,"couples", pat );

  al->AddPattern(pat);
}

//-------------------------------------------------------------------------------
void getBernData(EdbPVRec    *al)
{
  EdbPattern *pat=0;
  pat = new EdbPattern(0.,0., 0. );
  getDataCouples(
  		 "/mnt/nusrv4_2/ftpuser/aedb1204.root"
  		 ,"couples", pat );
  al->AddPattern(pat);


  pat = new EdbPattern(0.,0., 1100. );
  getDataCouples(
		 "/mnt/nusrv4_2/ftpuser/aedb1304.root"
		 ,"couples", pat );
  al->AddPattern(pat);


//    pat = new EdbPattern(0.,0., -2600. );
//    getDataCouples(
//  		 "/mnt/nusrv4_2/ftpuser/aedb1404.root"
//  		 ,"couples", pat );
//    al->AddPattern(pat);
}
