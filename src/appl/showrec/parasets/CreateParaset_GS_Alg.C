{
    //=C= ------------------------------------------------------------------------------------------------------
    TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_LONG_GS_ALG.root","RECREATE");

    //=C=	Values valid for ShowerReco_Algorithm_0 = FJ Algo: ConeTube
    //=C=	Values valid for ShowerReco_Algorithm_2 = FJ Algo: ConeTube2
    //=C=	Values valid for ShowerReco_Algorithm_4 = OI Algo: ConeTube (Official implementation as closest as possible"
    //=C= Values valid for ShowerReco_Algorithm_5 = SA Algo: MC Evts ...
    //=C= Values valid for ShowerReco_Algorithm_8 = BW Algo: Backward Algorithm.,,,
    //=C= Values valid for ShowerReco_Algorithm_10= GS Algo: Gamma Search (Pair) Algorithm.,,,

    // ALTP 10:  GS from libShowRec
    Double_t cut_gs_cut_dip=150;
    Double_t cut_gs_cut_dmin=40;
    Double_t cut_gs_cut_dr=60;
    Double_t cut_gs_cut_dz=19000;
    Double_t cut_gs_cut_dtheta=0.06;
    Double_t cut_gs_cut_piddiff=1;
    Int_t cut_gs_cut_oppositeflag=0;

    ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
    ParaSet -> Branch("CUT_GS_CUT_DIP",&cut_gs_cut_dip,"CUT_GS_CUT_DIP/D");
    ParaSet -> Branch("CUT_GS_CUT_DMIN",&cut_gs_cut_dmin,"CUT_GS_CUT_DMIN/D");
    ParaSet -> Branch("CUT_GS_CUT_DR",&cut_gs_cut_dr,"CUT_GS_CUT_DR/D");
    ParaSet -> Branch("CUT_GS_CUT_DZ",&cut_gs_cut_dz,"CUT_GS_CUT_DZ/D");
    ParaSet -> Branch("CUT_GS_CUT_DTHETA",&cut_gs_cut_dtheta,"CUT_GS_CUT_DTHETA/D");
    ParaSet -> Branch("CUT_GS_CUT_PIDDIFF",&cut_gs_cut_piddiff,"CUT_GS_CUT_PIDDIFF/D");
    ParaSet -> Branch("CUT_GS_CUT_OPPOSITEFLAG",&cut_gs_cut_oppositeflag,"CUT_GS_CUT_OPPOSITEFLAG/I");

    Int_t ParaSetNr=0;
    ofstream outstream;
    TString OutputFile="PARAMETERSET_DEFINITIONFILE_LONG_GS_ALG.txt";
    outstream.open(OutputFile);
    outstream << "#ParaSetNr  CUT_GS_CUT_DIP CUT_GS_CUT_DMIN CUT_GS_CUT_DR CUT_GS_CUT_DZ CUT_GS_CUT_DTHETA CUT_GS_CUT_PIDDIFF CUT_GS_CUT_OPPOSITEFLAG  "<< endl;
    ///===========================================================================
    int cnt[7]={0,0,0,0,0,0,0};
    bool stop=kFALSE;
		
		Double_t array_gs_cut_dip[5000];
		Double_t array_gs_cut_dmin[5000];
		Double_t array_gs_cut_dr[5000];
		Double_t array_gs_cut_dz[5000];
		Double_t array_gs_cut_dtheta[5000];
		
		  for (Int_t i=0;i<5000;i++) {
        array_gs_cut_dip[i]=gRandom->Uniform(0,400);
        array_gs_cut_dmin[i]=gRandom->Uniform(0,200);
        array_gs_cut_dr[i]=gRandom->Uniform(0,500);
        array_gs_cut_dz[i]=gRandom->Uniform(0,30000);
        array_gs_cut_dtheta[i]=gRandom->Uniform(0,0.5);
			}
		
    ///===========================================================================
    stop=kFALSE;
		for (Int_t j=0;j<2;j++) {
    for (Int_t k=0;k<5;k++) {
		for (Int_t i=0;i<5000;i++) {
        cut_gs_cut_dip=array_gs_cut_dip[i];
        cut_gs_cut_dmin=array_gs_cut_dmin[i];
        cut_gs_cut_dr=array_gs_cut_dr[i];
        cut_gs_cut_dz=array_gs_cut_dz[i];
        cut_gs_cut_dtheta=array_gs_cut_dtheta[i];

						cut_gs_cut_piddiff=k;
						cut_gs_cut_oppositeflag=j;
						
						
						
						ParaSet->Fill();
						outstream << "  " << ParaSetNr << "  " << "  " << cut_gs_cut_dip << "  " << cut_gs_cut_dmin << "  " << cut_gs_cut_dr << "  " << cut_gs_cut_dz << "  " << cut_gs_cut_dtheta << "  " << cut_gs_cut_piddiff << "  " << cut_gs_cut_oppositeflag << "  "  << endl;
						++ParaSetNr;
					}
				}
			}
		///===========================================================================
//     cut_gs_cut_dip=999999;
//     cut_gs_cut_dmin=132.8;
// 		cut_gs_cut_dr=30.2;
// 		cut_gs_cut_dz=999999;
// 		cut_gs_cut_dtheta=0.15;
// 		cut_gs_cut_piddiff=2;
//     ParaSet->Fill();
// 		outstream << "  " << ParaSetNr << "  " << "  " << cut_gs_cut_dip << "  " << cut_gs_cut_dmin << "  " << cut_gs_cut_dr << "  " << cut_gs_cut_dz << "  " << cut_gs_cut_dtheta << "  " << cut_gs_cut_piddiff << "  " << cut_gs_cut_oppositeflag << "  "  << endl;
//         ++ParaSetNr;
    ///===========================================================================
    ParaSet->Print();
    PARAMETERSET_DEFINITIONFILE_SHORT->cd();
    ParaSet->Write();
    return;
}