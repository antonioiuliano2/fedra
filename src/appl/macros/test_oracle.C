
char user[10]="OPERA";        // da settare
char password[10]="ScanLab";  // da settare
char database[1000]="(DESCRIPTION = (ADDRESS_LIST = (ADDRESS = (PROTOCOL = TCP)(HOST = 10.10.10.51)(PORT = 1521)))(CONNECT_DATA = (SERVICE_NAME = dboperan.na.infn.it)))"; // da settare

int brick_id=9; // da settare
int plate_id_is_irrelevant=0;

long long int proc_op_id=7000000000400115LL; // da settare

EoiIO *io=0;
EdbPVRec *pvrec=0;

EdbScanCond *cond=0;

EdbScanCond *SetScanCond_Linking()
{
  EdbDataPiece p;
  p.ReadPiecePar("default.par");
  EdbScanCond* cond=p.GetCond(1);

  return new EdbScanCond(*cond);
}

EdbScanCond *SetScanCond_Tracking()
{
  EdbDataPiece p;
  p.ReadPiecePar("default.par");
  EdbScanCond* cond=p.GetCond(0);

  return new EdbScanCond(*cond);
}


test_oracle()
{
  cout << "Start" << endl;
  gSystem->Exec("date");
  
  pvrec = new EdbPVRec;

  cond = SetScanCond_Tracking();
  pvrec->SetScanCond( cond );

  
  cout << "Start connection to DB" << endl;
  gSystem->Exec("date");

  io = new EoiIO(user, password, database, brick_id, plate_id_is_irrelevant);


  //  io->ReadBasetrackPatterns(proc_op_id, pvrec);

  //  io->ReadBasetrackPattern(proc_op_id, 57, pvrec);
  io->ReadBasetrackPattern(proc_op_id, 56, pvrec);
//   io->ReadBasetrackPattern(proc_op_id, 55, pvrec);
//   io->ReadBasetrackPattern(proc_op_id, 54, pvrec);

  cout << "End connection to DB" << endl;
  gSystem->Exec("date");

  pvrec->SetPatternsID();
  pvrec->SetSegmentsErrors();
  pvrec->SetCouplesAll();
  pvrec->SetChi2Max(pvrec->GetScanCond()->Chi2PMax());

  pvrec->Print();
}


align()
{
  cout << "Start  alignment" << endl;
  gSystem->Exec("date");

  pvrec->SetOffsetsMax(cond->OffX(),cond->OffY());
  pvrec->Align(1);
  pvrec->PrintAff();

  cout << "End alignment" << endl;
  gSystem->Exec("date");
}

align(float x, float y)
{
  cout << "Start  alignment" << endl;
  gSystem->Exec("date");

  pvrec->SetOffsetsMax(x,y);
  pvrec->Align(1);
  pvrec->PrintAff();

  cout << "End alignment" << endl;
  gSystem->Exec("date");
}


tracking()
{
  
  cout << "Start tracking" << endl;
  gSystem->Exec("date");

  pvrec->Link();

  pvrec->FillTracksCell();

  pvrec->MakeTracks();
  pvrec->FitTracks();
  pvrec->FillCell(50,50,0.015,0.015);

  EdbTrackP *tr=0;
  Int_t ntr=0;
  if(pvrec->eTracks) ntr = pvrec->eTracks->GetEntries();
  for (int itr=0; itr<ntr; itr++) {
    tr = (EdbTrackP*)(pvrec->eTracks->At(itr));
    tr->SetSegmentsTrack();
  }

  EdbDataProc proc;
  proc.MakeTracksTree(pvrec);

  cout << "End tracking" << endl;
  gSystem->Exec("date");

}

void patterns()
{
  // test for microtracks reading?

  EoiIO *io = new EoiIO(user, password, "bernopdb", 0, 0);
  pvrec = new EdbPVRec;
  for (int i=55; i>39; i--){
    (io->ReadMicrotracks(5000000000500291ll, i, pvrec));
  }
}
