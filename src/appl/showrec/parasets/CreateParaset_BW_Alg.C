{
	//=C= ------------------------------------------------------------------------------------------------------
	TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_LONG_BW_ALG.root","RECREATE");

	//=C=	Values valid for ShowerReco_Algorithm_0 = FJ Algo: ConeTube
	//=C=	Values valid for ShowerReco_Algorithm_2 = FJ Algo: ConeTube2
	//=C=	Values valid for ShowerReco_Algorithm_4 = OI Algo: ConeTube (Official implementation as closest as possible"
  //=C= Values valid for ShowerReco_Algorithm_5 = SA Algo: MC Evts ...
	//=C= Values valid for ShowerReco_Algorithm_8 = BW Algo: Backward Algorithm.,,,
  
	Double_t cut_back_dmin,cut_for_dmin,cut_back_dtheta,cut_for_dtheta,cut_back_dr,cut_for_dr,cut_back_dz,cut_for_dz;
	
	ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
	ParaSet -> Branch("CUT_BACK_DMIN",&cut_back_dmin,"CUT_BACK_DMIN/D");
	ParaSet -> Branch("CUT_BACK_DTHETA",&cut_back_dtheta,"CUT_BACK_DTHETA/D");
	ParaSet -> Branch("CUT_BACK_DR",&cut_back_dr,"CUT_BACK_DR/D");
	ParaSet -> Branch("CUT_BACK_DZ",&cut_back_dz,"CUT_BACK_DZ/D");
	ParaSet -> Branch("CUT_FOR_DMIN",&cut_for_dmin,"CUT_FOR_DMIN/D");
	ParaSet -> Branch("CUT_FOR_DTHETA",&cut_for_dtheta,"CUT_FOR_DTHETA/D");
	ParaSet -> Branch("CUT_FOR_DR",&cut_for_dr,"CUT_FOR_DR/D");
	ParaSet -> Branch("CUT_FOR_DZ",&cut_for_dz,"CUT_FOR_DZ/D");
	
	Int_t ParaSetNr=0;
	ofstream outstream;
	TString OutputFile="PARAMETERSET_DEFINITIONFILE_LONG_BW_ALG.txt";
	outstream.open(OutputFile);
	outstream << "#ParaSetNr  CUT_BACK_DMIN CUT_BACK_DTHETA CUT_BACK_DR CUT_BACK_DZ CUT_FOR_DMIN CUT_FOR_DTHETA CUT_FOR_DR CUT_FOR_DZ  "<< endl;
	
	///===========================================================================

	int cnt[8]={0,0,0,0,0,0,0,0};
	bool stop=kFALSE;
	while (!stop) {
		cut_back_dmin=gRandom->Uniform(0,140)+80;
		cut_back_dtheta=gRandom->Uniform(0,0.2)+0.08;
		cut_back_dr=gRandom->Uniform(0,160)+80;
		cut_back_dz=gRandom->Uniform(0,4000)+4000;
		cut_for_dmin=gRandom->Uniform(0,140)+80;
		cut_for_dtheta=gRandom->Uniform(0,0.2)+0.08;
		cut_for_dr=gRandom->Uniform(0,160)+80;
		cut_for_dz=gRandom->Uniform(0,4000)+4000;

		if (ParaSetNr>1000) {
			cut_for_dmin=gRandom->Uniform(0,200)+0;
			cut_for_dtheta=gRandom->Uniform(0,0.15)+0.05;
			cut_for_dr=gRandom->Uniform(0,250)+0;
			cut_for_dz=gRandom->Uniform(0,6000)+2000;
		}


		ParaSet->Fill();
		outstream << "  " << ParaSetNr << "  " << "  " << cut_back_dmin << "  " << cut_back_dtheta << "  " << cut_back_dr << "  " << cut_back_dz << "  " << cut_for_dmin << "  " << cut_for_dtheta << "  " << cut_for_dr << "  " << cut_for_dz << endl;
		++ParaSetNr;
						
// 		if (ParaSetNr>1000) stop=kTRUE;
		if (ParaSetNr>2000) stop=kTRUE;
	}
	///===========================================================================

	ParaSet->Print();
	PARAMETERSET_DEFINITIONFILE_SHORT->cd();
	ParaSet->Write();
	return;
}
