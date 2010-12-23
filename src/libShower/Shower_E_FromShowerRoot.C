{

	// Open File. Get Tree.
	TFile *_file0 = TFile::Open("Shower.root");
	TTree *tr = (TTree*)_file0->Get("treebranch");     

	// ShowerRec Object for managing data:
	EdbShowerRec* ShowerRec = new EdbShowerRec()  ;
	
	// Convert "treebranch" into array of EdbTrackP's
	ShowerRec->TransferTreebranchShowerTreeIntoShowerObjectArray(tr);
	TObjArray* RecoShowerArray = ShowerRec->GetRecoShowerArray() ;
	
	// Check the number of entries:
	cout << tr->GetEntries() << endl;
	cout << ShowerRec->GetRecoShowerArrayN() << endl;
	
	
	
	// Instantate ShowerAlgorithmEnergy Class
	EdbShowerAlgESimple* ShowerAlgEnergyInstance = new EdbShowerAlgESimple();
	
	// Run Shower Energy Algorith on all shower/tracks
// 	ShowerAlgEnergyInstance->DoRun(RecoShowerArray);

	// Run Shower Energy Algorith on first shower/tracks
	EdbTrackP* showertrack=(EdbTrackP*)RecoShowerArray->At(0);
	showertrack->PrintNice();
	ShowerAlgEnergyInstance->DoRun(showertrack);
	// Write the values then into "Shower.root" (old Shower.root will be overwritten!");
//ShowerAlgEnergyInstance->WriteNewRootFile();



	// Instantate EdbShowerAlgIDSimple Class
	EdbShowerAlgIDSimple* ShowerAlgIDInstance = new EdbShowerAlgIDSimple();
	
	EdbTrackP* showertrack=(EdbTrackP*)RecoShowerArray->At(0);
	showertrack->PrintNice();
	gEDBDEBUGLEVEL=3;
	ShowerAlgIDInstance->DoRun(showertrack);
	return;


}
