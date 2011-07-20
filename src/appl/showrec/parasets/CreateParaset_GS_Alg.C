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
    while (!stop) {
        cut_gs_cut_dip=gRandom->Uniform(-50,50)+200.;
        cut_gs_cut_dmin=gRandom->Uniform(-40,40)+80;
        cut_gs_cut_dr=gRandom->Uniform(-100,100)+50;
        cut_gs_cut_dz=gRandom->Uniform(-3000,3000)+20000;
        cut_gs_cut_dtheta=gRandom->Uniform(-0.15,0.15)+0.15;
        cut_gs_cut_piddiff=Int_t(gRandom->Uniform(0,5));
        cut_gs_cut_oppositeflag=0;
//  4893    237.43  80.8433  49.4419  20911.5  0.17852  2  0  

				ParaSet->Fill();
        outstream << "  " << ParaSetNr << "  " << "  " << cut_gs_cut_dip << "  " << cut_gs_cut_dmin << "  " << cut_gs_cut_dr << "  " << cut_gs_cut_dz << "  " << cut_gs_cut_dtheta << "  " << cut_gs_cut_piddiff << "  " << cut_gs_cut_oppositeflag << "  "  << endl;
        ++ParaSetNr;

// 		if (ParaSetNr>1000) stop=kTRUE;
        if (ParaSetNr>8000) stop=kTRUE;
    }
    ///===========================================================================
    stop=kFALSE;
    while (!stop) {
        cut_gs_cut_dip=gRandom->Uniform(0,300);
        cut_gs_cut_dmin=gRandom->Uniform(0,200);
        cut_gs_cut_dr=gRandom->Uniform(0,500);
        cut_gs_cut_dz=gRandom->Uniform(0,30000);
        cut_gs_cut_dtheta=gRandom->Uniform(0,0.5);
        cut_gs_cut_piddiff=Int_t(gRandom->Uniform(0,6));
        cut_gs_cut_oppositeflag=0;
//  4893    237.43  80.8433  49.4419  20911.5  0.17852  2  0  

				ParaSet->Fill();
        outstream << "  " << ParaSetNr << "  " << "  " << cut_gs_cut_dip << "  " << cut_gs_cut_dmin << "  " << cut_gs_cut_dr << "  " << cut_gs_cut_dz << "  " << cut_gs_cut_dtheta << "  " << cut_gs_cut_piddiff << "  " << cut_gs_cut_oppositeflag << "  "  << endl;
        ++ParaSetNr;

// 		if (ParaSetNr>1000) stop=kTRUE;
        if (ParaSetNr>10000) stop=kTRUE;
    }
    ///===========================================================================
    cut_gs_cut_dip=9999;
    cut_gs_cut_dmin=9999;
		cut_gs_cut_dr=9999;
		cut_gs_cut_dz=99999;
		cut_gs_cut_dtheta=9999;
		cut_gs_cut_piddiff=9;
    ParaSet->Fill();
		outstream << "  " << ParaSetNr << "  " << "  " << cut_gs_cut_dip << "  " << cut_gs_cut_dmin << "  " << cut_gs_cut_dr << "  " << cut_gs_cut_dz << "  " << cut_gs_cut_dtheta << "  " << cut_gs_cut_piddiff << "  " << cut_gs_cut_oppositeflag << "  "  << endl;
        ++ParaSetNr;
    ///===========================================================================
    ParaSet->Print();
    PARAMETERSET_DEFINITIONFILE_SHORT->cd();
    ParaSet->Write();
    return;
}
