{
	
	//=C= ------------------------------------------------------------------------------------------------------
	//=C= FOR SHORT AND FAST RECONSTRUCTION STUDIES: NOT CONTAINING SO MUCH PARAMETERSET (instead of 5k only 128) 
	//=C= COMPARED TO   CreateParameterSetFile.C   
	//=C= ------------------------------------------------------------------------------------------------------
	
	TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_LONG_OI_ALG.root","RECREATE");

	//=C=	Values valid for ShowerReco_Algorithm_0 = FJ Algo: ConeTube
	//=C=	Values valid for ShowerReco_Algorithm_2 = FJ Algo: ConeTube2
	//=C=	Values valid for ShowerReco_Algorithm_5 = OI2 Algo: ConeTube (Official implementation as closest as possible"


	Double_t	CUT_SHOWERFOLLOWERBT_DR_MAX;								// Distance to the next follower BT
	Double_t	CUT_SHOWERFOLLOWERBT_DTAN_MAX;							// Angle to the next follower BT
	Double_t	CUT_ZYLINDER_R_MAX;												// Radius when cone becomes a cylinder
	Double_t	CUT_ZYLINDER_ANGLE_MAX;								// Opening Angle of the cone to collect all BTs
	Int_t			CUT_NHOLES_MAX;												// Maximum plate holes for tracks to tracks in shower

//	Double_t CUT_SHOWERFOLLOWERBT_DR_MAX_ARRAY={};
//	Double_t CUT_SHOWERFOLLOWERBT_DTAN_MAX_ARRAY={};
//	Double_t CUT_ZYLINDER_R_MAX_ARRAY={};
//	Double_t CUT_ZYLINDER_ANGLE_MAX_ARRAY={};



	
	ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
	ParaSet->Branch("CUT_SHOWERFOLLOWERBT_DR_MAX",&CUT_SHOWERFOLLOWERBT_DR_MAX,"CUT_SHOWERFOLLOWERBT_DR_MAX/D");
	ParaSet->Branch("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&CUT_SHOWERFOLLOWERBT_DTAN_MAX,"CUT_SHOWERFOLLOWERBT_DTAN_MAX/D");
	ParaSet->Branch("CUT_ZYLINDER_R_MAX",&CUT_ZYLINDER_R_MAX,"CUT_ZYLINDER_R_MAX/D");
	ParaSet->Branch("CUT_ZYLINDER_ANGLE_MAX",&CUT_ZYLINDER_ANGLE_MAX,"CUT_ZYLINDER_ANGLE_MAX/D");
	ParaSet->Branch("CUT_NHOLES_MAX",&CUT_NHOLES_MAX,"CUT_NHOLES_MAX/I");
	
	Int_t ParaSetNr=0;
	ofstream outstream;
	TString OutputFile="PARAMETERSET_DEFINITIONFILE_LONG_OI_ALG.txt";
	outstream.open(OutputFile);
	outstream << " ParaSetNr  CUT_ZYLINDER_R_MAX CUT_ZYLINDER_ANGLE_MAX CUT_SHOWERFOLLOWERBT_DR_MAX CUT_SHOWERFOLLOWERBT_DTAN_MAX    CUT_NHOLES_MAX "<< endl;
	
	
	
// 				dr_max=120.0;dt_max=0.12;coneangle=0.020;tubedist=700.0;
	///===========================================================================

 for (Int_t i0=0; i0 <3; ++i0) {
	if (i0==0) CUT_NHOLES_MAX=3;
	if (i0==1) CUT_NHOLES_MAX=5;
	if (i0==2) CUT_NHOLES_MAX=9;
	

	for (Int_t i1=0; i1 <=7; ++i1) {
		for (Int_t i2=0; i2 <=7; ++i2) {
			for (Int_t i3=0; i3 <=7; ++i3) {
				for (Int_t i4=0; i4 <=7; ++i4) {
						CUT_ZYLINDER_R_MAX=600+50*i1;
						CUT_ZYLINDER_ANGLE_MAX=0.015+0.005*i2; 
						CUT_SHOWERFOLLOWERBT_DR_MAX=80+10*i3;
						CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.08+0.01*i4;
						ParaSet->Fill();
						outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_NHOLES_MAX << endl;
						cout  << "  " << ParaSetNr << "  " << "  " << i1 << "  " << i2 << "  " << i3 << "  " << i4 << "  " << CUT_NHOLES_MAX << endl;
						++ParaSetNr;
					}
				}
			}
		}
	

	// Manually adapt frederics high purity cut:
	// THIS IS NUMBER 4096
	CUT_ZYLINDER_R_MAX=400;
	CUT_ZYLINDER_ANGLE_MAX=0.02;
	CUT_SHOWERFOLLOWERBT_DR_MAX=100;
	CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.05;
	ParaSet->Fill();
	outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_NHOLES_MAX << endl;
	++ParaSetNr;

	// Manually adapt frederics standard cut:
	// THIS IS NUMBER 4097
	CUT_ZYLINDER_R_MAX=800;
	CUT_ZYLINDER_ANGLE_MAX=0.02;
	CUT_SHOWERFOLLOWERBT_DR_MAX=150;
	CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.15;
	ParaSet->Fill();
	outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_NHOLES_MAX << endl;
	++ParaSetNr;


	}
 ///===========================================================================



















	
	
	ParaSet->Print();
	PARAMETERSET_DEFINITIONFILE_SHORT->cd();
	ParaSet->Write();
	//PARAMETERSET_DEFINITIONFILE_SHORT->Write();
	return;
}
