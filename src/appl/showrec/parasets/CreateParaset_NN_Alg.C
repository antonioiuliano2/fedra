{
	
	//=C= ------------------------------------------------------------------------------------------------------
	//=C= FOR SHORT AND FAST RECONSTRUCTION STUDIES: NOT CONTAINING SO MUCH PARAMETERSET (instead of 5k only 128) 
	//=C= COMPARED TO   CreateParameterSetFile.C   
	//=C= ------------------------------------------------------------------------------------------------------
	
	TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_LONG_NN_ALG.root","RECREATE");

	//=C=	Values valid for ShowerReco_Algorithm_3 = NN ALG: NeuralNetwork


	Double_t	CUT_ANN_OUTPUT;								//ann output node cutvalue
	Int_t	CUT_ANN_INPUTNEURONS;								//ann input nodes nr


// 	TREE_ParaSetDefinitions->SetBranchAddress("CUT_ANN_OUTPUT",&ann_output);

	
	ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
	ParaSet->Branch("CUT_ANN_INPUTNEURONS",&CUT_ANN_INPUTNEURONS,"CUT_ANN_INPUTNEURONS/I");
	ParaSet->Branch("CUT_ANN_OUTPUT",&CUT_ANN_OUTPUT,"CUT_ANN_OUTPUT/D");
	
	Int_t ParaSetNr=0;
	ofstream outstream;
	TString OutputFile="PARAMETERSET_DEFINITIONFILE_LONG_NN_ALG.txt";
	outstream.open(OutputFile);
	outstream << " ParaSetNr  CUT_ANN_INPUTNEURONS CUT_ANN_OUTPUT  "<< endl;
	
	Int_t ann_neurons[4]={5,10,20,30};
	
// 				dr_max=120.0;dt_max=0.12;coneangle=0.020;tubedist=700.0;
	///===========================================================================

	for (Int_t i1=0; i1 <=3; ++i1) {
	for (Int_t i2=0; i2 <=19; ++i2) {
		CUT_ANN_INPUTNEURONS=ann_neurons[i1];
		CUT_ANN_OUTPUT=0.1+(Double_t)i2*0.04;
						
		ParaSet->Fill();
		outstream << "  " << ParaSetNr << "  " << CUT_ANN_INPUTNEURONS <<  "  " << CUT_ANN_OUTPUT << endl;
		++ParaSetNr;
		}
	}
  
  
  for (Int_t i1=0; i1 <=3; ++i1) {
  for (Int_t i2=0; i2 <=4; ++i2) {
    CUT_ANN_INPUTNEURONS=ann_neurons[i1];
    CUT_ANN_OUTPUT=0.86+(Double_t)i2*0.02;
            
    ParaSet->Fill();
    outstream << "  " << ParaSetNr << "  " << CUT_ANN_INPUTNEURONS <<  "  " << CUT_ANN_OUTPUT << endl;
    ++ParaSetNr;
    }
  }
  
  
    for (Int_t i1=0; i1 <=3; ++i1) {
  for (Int_t i2=0; i2 <=8; ++i2) {
    CUT_ANN_INPUTNEURONS=ann_neurons[i1];
    CUT_ANN_OUTPUT=0.90+(Double_t)i2*0.01;
            
    ParaSet->Fill();
    outstream << "  " << ParaSetNr << "  " << CUT_ANN_INPUTNEURONS <<  "  " << CUT_ANN_OUTPUT << endl;
    ++ParaSetNr;
    }
  }
  
  
	///===========================================================================

	ParaSet->Print();
// 	return;
	PARAMETERSET_DEFINITIONFILE_SHORT->cd();
	ParaSet->Write();
	//PARAMETERSET_DEFINITIONFILE_SHORT->Write();
	return;
}
