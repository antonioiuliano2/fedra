{
	
	//=C= ------------------------------------------------------------------------------------------------------
	//=C= FOR SHORT AND FAST RECONSTRUCTION STUDIES: NOT CONTAINING SO MUCH PARAMETERSET (instead of 5k only 128) 
	//=C= COMPARED TO   CreateParameterSetFile.C   
	//=C= ------------------------------------------------------------------------------------------------------
	
	TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_LONG_SA_ALG.root","RECREATE");

	//=C=	Values valid for ShowerReco_Algorithm_0 = FJ Algo: ConeTube
	//=C=	Values valid for ShowerReco_Algorithm_2 = FJ Algo: ConeTube2
	//=C=	Values valid for ShowerReco_Algorithm_4 = OI Algo: ConeTube (Official implementation as closest as possible"
  //=C= Values valid for ShowerReco_Algorithm_5 = SA Algo: MC Evts ...
  


	Double_t	CUT_P;												// s->P()
	Double_t	CUT_ALISUBSIZE;								// eAli_local_half_size

//	Double_t CUT_SHOWERFOLLOWERBT_DR_MAX_ARRAY={};
//	Double_t CUT_SHOWERFOLLOWERBT_DTAN_MAX_ARRAY={};
//	Double_t CUT_P_ARRAY={};
//	Double_t CUT_ALISUBSIZE_ARRAY={};



	
	ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
	ParaSet->Branch("CUT_P",&CUT_P,"CUT_P/D");
	ParaSet->Branch("CUT_ALISUBSIZE",&CUT_ALISUBSIZE,"CUT_ALISUBSIZE/D");	
	
	Int_t ParaSetNr=0;
	ofstream outstream;
	TString OutputFile="PARAMETERSET_DEFINITIONFILE_LONG_SA_ALG.txt";
	outstream.open(OutputFile);
	outstream << " ParaSetNr  CUT_P CUT_ALISUBSIZE  "<< endl;
	
	
	
	///===========================================================================

	for (Int_t i1=0; i1 <=9; ++i1) {
		for (Int_t i2=0; i2 <=6; ++i2) {


						CUT_P=0+20*i1;
						CUT_ALISUBSIZE=600+200*i2; 
						

						ParaSet->Fill();
						outstream << "  " << ParaSetNr << "  " << "  " << CUT_P << "  " << CUT_ALISUBSIZE << "  "  << endl;
						++ParaSetNr;
						
						
						
			}
		}
	///===========================================================================

	ParaSet->Print();
	PARAMETERSET_DEFINITIONFILE_SHORT->cd();
	ParaSet->Write();
	//PARAMETERSET_DEFINITIONFILE_SHORT->Write();
	return;
}
