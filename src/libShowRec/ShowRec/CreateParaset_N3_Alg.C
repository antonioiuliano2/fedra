{
    TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_N3_ALG.root","RECREATE");

    //=C=	Values valid for ShowerReco_Algorithm_11 = N3 ALG: NeuralNetwork
    // Brick data related inputs
    // The number of Input Variables is calculated only from these two.
    Int_t        ANN_PLATE_DELTANMAX; // 0,1,2,3,4,5.
    // 0: no separate information BT(i,j) is used.
    // 1: additional information BT(i,j) for DeltNPL==0  (same plate) is used.
    // 2: additional information BT(i,j) for DeltNPL==-1 ( one plate upstream) is used.
    // 3: additional information BT(i,j) for DeltNPL==+1 ( one plate downstream) is used.
    // 4: additional information BT(i,j) for DeltNPL==-2 ( two plate upstream) is used.
    // 5: additional information BT(i,j) for DeltNPL==+2 ( two plate downstream) is used.
    // Order: same plate, one plate upstream,  one plate downstream,  two plate upstream,  two plate downstream

    // algorithm method related inputs
    Int_t        ANN_NTRAINEPOCHS; // 1,2,3,4 = 50,100,150,200
    Int_t        ANN_NHIDDENLAYER; // 1,2,3,4 = 2,3,5,7
    Double_t     ANN_OUTPUTTHRESHOLD; // 0..10 = 0.5, 0.55, 0.6 ...
    Int_t        ANN_EQUALIZESGBG; // 0: no, 1: yes (not all BG BTs will be taken for NN training.)
    // this is dependent by the other variables, thus it
    // is explicetly caclulated for ease of view
    Int_t        ANN_INPUTNEURONS;

// ParaSetNr  ANN_INPUTLEVEL ANN_PLATE_DELTANMAX ANN_PLATEDIRECTION  ANN_NHIDDENLAYER ANN_OUTPUTTHRESHOLD   ANN_INPUTNEURONS
    ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
    ParaSet->Branch("ANN_PLATE_DELTANMAX",&ANN_PLATE_DELTANMAX,"ANN_PLATE_DELTANMAX/I");
    ParaSet->Branch("ANN_NTRAINEPOCHS",&ANN_NTRAINEPOCHS,"ANN_NTRAINEPOCHS/I");
    ParaSet->Branch("ANN_NHIDDENLAYER",&ANN_NHIDDENLAYER,"ANN_NHIDDENLAYER/I");
    ParaSet->Branch("ANN_OUTPUTTHRESHOLD",&ANN_OUTPUTTHRESHOLD,"ANN_OUTPUTTHRESHOLD/D");
    ParaSet->Branch("ANN_INPUTNEURONS",&ANN_INPUTNEURONS,"ANN_INPUTNEURONS/I");
    ParaSet->Branch("ANN_EQUALIZESGBG",&ANN_EQUALIZESGBG,"ANN_EQUALIZESGBG/I");



    Int_t ParaSetNr=0;
    ofstream outstream;
    TString OutputFile="PARAMETERSET_DEFINITIONFILE_N3_ALG.txt";
    outstream.open(OutputFile);
    outstream << "ParaSetNr  ANN_INPUTLEVEL ANN_PLATE_DELTANMAX ANN_NTRAINEPOCHS  ANN_NHIDDENLAYER ANN_EQUALIZESGBG   ANN_OUTPUTTHRESHOLD   ANN_INPUTNEURONS"<< endl;

    // Calculate number of inputneurons out of the variables: ANN_INPUTLEVEL,ANN_PLATE_DELTANMAX
    // Formula is simple:
    // ANN_INPUTNEURONS = 4 + 4 x ............
    // Explanation: 4 for BT_IN to BT_i : dZ, dR, dT, dMinDist
    // Explanation: 4 for BT_i to BT_j : min value of dR, dT, dMinDist, n different segments contributing to min value (for same plate difference all)
    Int_t nANN_INPUTLEVEL = 0;
    Int_t nANN_PLATE_DELTANMAX = 0;



    ///===========================================================================



    // ANN_PLATE_DELTANMAX
    for (Int_t i2=0; i2 <6; ++i2) {

        // ANN_PLATE_DELTANMAX
        ANN_PLATE_DELTANMAX= i2;

        // 3 Neurons for correlations to INBT always
        ANN_INPUTNEURONS = 4 + 4*ANN_PLATE_DELTANMAX;

        Int_t nANN_NTRAINEPOCHS[4]= {50,100,150,200};
        // ANN_NTRAINEPOCHS
        for (Int_t i3=0; i3 <4; ++i3) {
            ANN_NTRAINEPOCHS=nANN_NTRAINEPOCHS[i3];

            Int_t nANN_NHIDDENLAYER[4]= {2,3,5,7};
            // ANN_NHIDDENLAYER
            for (Int_t i4=1; i4 <4; ++i4) {

                ANN_NHIDDENLAYER=nANN_NHIDDENLAYER[i4];


                // ANN_EQUALIZESGBG either 0 or 1
                for (Int_t i6=0; i6 <2; ++i6) {
                    ANN_EQUALIZESGBG = i6;

                    // ANN_OUTPUTTHRESHOLD
                    for (Int_t i5=0; i5 <15; ++i5) {

                        ANN_OUTPUTTHRESHOLD=0.4+(Double_t)i5*0.04;

                        ParaSet->Fill();
                        outstream << "  " << ParaSetNr  <<  "  " << ANN_PLATE_DELTANMAX << "  " << ANN_NTRAINEPOCHS <<  "  " << ANN_NHIDDENLAYER << "  " << ANN_EQUALIZESGBG << "   " << ANN_OUTPUTTHRESHOLD << "  " << ANN_INPUTNEURONS <<endl;


                        cout << "  " << ParaSetNr <<  "  " << ANN_PLATE_DELTANMAX << "  " << ANN_NTRAINEPOCHS <<  "  " << ANN_NHIDDENLAYER << "  " << ANN_EQUALIZESGBG << "  " << ANN_OUTPUTTHRESHOLD << "  " << ANN_INPUTNEURONS <<endl;

                        ++ParaSetNr;
                    } // for (Int_t i5=0; i5 <15; ++i5) ANN_OUTPUTTHRESHOLD

                } //for (Int_t i6=0; i6 <1; ++i6)ANN_EQUALIZESGBG
            }
        } // ANN_NTRAINEPOCHS
    }


    ///===========================================================================

    ParaSet->Print();
// 	return;
    PARAMETERSET_DEFINITIONFILE_SHORT->cd();
    ParaSet->Write();
    //PARAMETERSET_DEFINITIONFILE_SHORT->Write();
    return;
}
