//-------------------------------------------------------------------------------------------
void ReconstructShowers_N3()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_N3() ---");




    //-----------------------------------------------------------------
    // Main function for reconstruction of N3 Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Read options for easy set up N3_Algorithm structures
    N3_ReadOptionFile();

    // Option for quick switching training/running mode of the ANN
    cout << " cmd_ALN3TRAIN = "   <<   cmd_ALN3TRAIN << endl;
    if (cmd_ALN3TRAIN==1) {
        N3_DoTrain = kTRUE;
    }
    else {
        N3_DoTrain = kFALSE;
    }



    // Create the root file first, otherwise the trees are not connected with the
    // specified file...
    TFile* N3_ALG_ANN_TrainingsFile;
    if (cmd_ALN3TRAIN==1) {
        N3_ALG_ANN_TrainingsFile = new TFile(Form("N3_ALG_ANN_TrainingsFile_PARASET_%d.root",GLOBAL_PARASETNR),"RECREATE");
    }


    // Variables and things important for neural Network:
    TTree *simu = new TTree("TreeSignalBackgroundBT", "TreeSignalBackgroundBT");
    simu->Branch("N3_Type",   &N3_Type,   "N3_Type/I");
    simu->Branch("N3_Inputvar", N3_Inputvar, "N3_Inputvar[29]/D");
    //------------------------------
    // Attention: the cloned tree has not the same values after filling.
    // Is it possible that the Clone() function somehow destroys the tree branches
    // and their addresses??? That means, dont use the method here:
    // TTree *simu_SG = (TTree *)simu->Clone();
    // Try creating the trees in the "uncloned" way!
    // Yes, it seems to be like that. With this method,
    // the trees _SG and _BG are filled with the normal values.
    TTree *simu_SG = new TTree("TreeSignalBT", "TreeSignalBT");
    simu_SG->Branch("N3_Type",   &N3_Type,   "N3_Type/I");
    simu_SG->Branch("N3_Inputvar", N3_Inputvar, "N3_Inputvar[24]/D");
    TTree *simu_BG = new TTree("TreeBackgroundBT", "TreeBackgroundBT");
    simu_BG->Branch("N3_Type",   &N3_Type,   "N3_Type/I");
    simu_BG->Branch("N3_Inputvar", N3_Inputvar, "N3_Inputvar[24]/D");
    // Reminder: be aware, that the Variables for TTrees only exist in this
    // local function ReconstructShowers_N3()and not outside it.
    //------------------------------


    // If "running" (i.e. not training) mode, then the neural network is
    // created at the beginning of the reconstruction routine.
    if (N3_DoTrain!=kTRUE) {
        N3_Create_ALG_MLP(simu, GLOBAL_PARASETNR);
        N3_Load_ALG_MLP_weights(N3_TMLP_ANN,GLOBAL_PARASETNR);
    }


    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        //GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_TanTheta=InBT->Theta(); // same value as above.
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------

        // Clone InBT, because it is modified a lot of times,
        // avoid rounding errors by propagating back and forth
        EdbSegP* InBTClone = (EdbSegP*)InBT->Clone();


        //-----------------------------------
        // 1a) Reset characteristic variables:
        //-----------------------------------
        for (Int_t loop=0; loop<24; ++loop) N3_Inputvar[loop]=0;
        Int_t NaddedSGToTrainTree=0;
        Int_t NaddedBGToTrainTree=0;


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        //cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            // Needed to have StepX,Y,TX,TY bins set for the FindCompliments Function.
            //EdbPattern* ActualPattern   =   (EdbPattern*)local_gAli->GetPattern(patterloop_cnt);
            //ActualPattern               ->  FillCell(20,20,0.01,0.01);

            EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_cnt);
            Int_t btloop_cnt_N=TestPattern->GetN();

            for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                //cout << "Doing segment  " << btloop_cnt << " of " << btloop_cnt_N << " now: " << endl;
                //seg->PrintNice();

                // Now calculate NN Histogram Inputvariables:
                N3_Type = 0;  // Track is "Background"
                if (seg->MCEvt()>0) N3_Type = 1;  // Track is "Signal"

                // Calculate the first four inputvariables, which depend on InitiatorBT only:
                // Important: dR, dMindist is symmetric, dR is NOT!
                // Do propagation from InBT to seg! (i.e. to downstream segments)
                // Loose PreCuts, in order not to get too much BG BT into trainings sample
                N3_Inputvar[0] = seg->Z()-InBT->Z();
                // Update: It is better to use DistToAxis instead of dR, since dR measures
                // only distance to InBT without taking care of the direction.
                // (does not matter for relatively straight tracks, but for tracks in direction)
                // N3_Inputvar[1] = GetdR(InBT, seg);
                N3_Inputvar[1] = GetDistToAxis(InBTClone, seg);
                if (N3_DoTrain==kTRUE && N3_Inputvar[1] > 1200) continue;

                N3_Inputvar[2] = GetdeltaThetaSingleAngles(InBT, seg);
                if (N3_DoTrain==kTRUE && N3_Inputvar[2] >  0.4) continue;

                N3_Inputvar[3] = GetdMinDist(InBT, seg);
                if (N3_DoTrain==kTRUE && N3_Inputvar[3] >  800) continue;



                // Originally, I tried here a routine to use the
                // "FindCompliments"-Function of EdbPattern.
                // Unfortunately the sigma of the X,Y,TX,TY is somehow very
                // small and then it returns only one compatible segment,
                // the segment itself.
                // So I choose another method: I find the best value for one
                // segment in the dR, dT, dMinDist space.
                // If a segment is connected to this one, then these values
                // should belong to same segment.
                // If a segment is only BG, the best dR, dT, dMinDist compatible
                // segments are different, so the number of different segments
                // to give the best value should be 0 for a SG basetrack and
                // 3 for a BG basetrack.

                Float_t mindeltaZ=0;
                Float_t mindT=0;
                Float_t mindR=0;
                Float_t mindMinDist=0;
                Int_t   nDifferentSegs=-1;
                Int_t   OffsetNVar=4;
                // Order: same plate, one plate upstream,  one plate downstream,
                // two plate upstream,  two plate downstream
                // This order is important, dont mix it anymore!!
                Int_t deltaNpl[5]= {0,-1,1,-2,2};
                Int_t dNpl=0;

                for (Int_t loop_deltaNpl = 0; loop_deltaNpl<5; ++loop_deltaNpl) {
                    dNpl=deltaNpl[loop_deltaNpl];
                    mindT=0;
                    mindR=0;
                    mindMinDist=0;
                    nDifferentSegs=-1;
                    N3_FindBestCompliments( seg, TestPattern, local_gAli, dNpl, mindeltaZ,  mindT, mindR, mindMinDist, nDifferentSegs );
                    // mindeltaZ;// no effect, take it out...
                    N3_Inputvar[OffsetNVar+0] = mindR;
                    N3_Inputvar[OffsetNVar+1] = mindT;
                    N3_Inputvar[OffsetNVar+2] = mindMinDist;
                    N3_Inputvar[OffsetNVar+3] = nDifferentSegs;
                    OffsetNVar+=4;
                }

                // TO HAVE AROUND SAME SG AND BG Events, TAKE ABOUT EVERY 100TH BG
                // BG_BASETRACK ONLY (otherwise too much BG Tracks, too long training time)
                // dont cut. .... Better do it in the default.par cut instead of here...
                // But no! It needs the full BG-density. So better here!
                // if (N3_DoTrain==kTRUE && N3_Type ==0 && gRandom->Uniform()*1000>10) continue;
                // if (N3_DoTrain==kTRUE && N3_Type ==0 && gRandom->Uniform()*1000>500) continue;
                // maybe not good after all ....

                if (gEDBDEBUGLEVEL>3) {
                    cout << "*******************************************************************"<<endl;
                    cout << "Print out all variables for BT i  ___ connected with InBT :   *****"<<endl;
                    OffsetNVar=0;
                    printf("BT i->ID(), Type, dZ, dR, dT, dMinDist: %07d %d %.03f %.03f %.03f %.03f\n",seg->ID(),N3_Type, N3_Inputvar[OffsetNVar+0], N3_Inputvar[OffsetNVar+1], N3_Inputvar[OffsetNVar+2], N3_Inputvar[OffsetNVar+3]);
                    OffsetNVar=4;
                    cout << "Print out all variables for BT i  NOT connected with InBT :   *****"<<endl;
                    for (Int_t loop_deltaNpl = 0; loop_deltaNpl<5; ++loop_deltaNpl) {
                        dNpl=deltaNpl[loop_deltaNpl];
                        printf("BT i->ID(), Type, dNpl, mindR, mindT, mindMinDist nDifferentSegs, : %07d %d %d %.03f %.03f %.03f %.03f %d \n",seg->ID(),N3_Type, dNpl,  N3_Inputvar[OffsetNVar+0], N3_Inputvar[OffsetNVar+1], N3_Inputvar[OffsetNVar+2], N3_Inputvar[OffsetNVar+3]);
                        OffsetNVar+=4;
                    }
                    cout << "*******************************************************************"<<endl;
                }


// cout << "*******************************************************************"<<endl;


                // Now    fill NN SG/BG Trees with the Inputvariables:  --------------------
                // Only in Case of Training:
                if (N3_DoTrain==kTRUE) {
                    if ( N3_Type == 1 ) {
                        simu->Fill();
                        simu_SG->Fill();
                        NaddedSGToTrainTree++;
                    }
                    else {
                        // if this state is enabled, then BG-tracks will only be added in a
                        // proportion that the number is equal to SG-tracks
                        Double_t currentBGSGRatio= Double_t (NaddedBGToTrainTree)/(1+Double_t(NaddedSGToTrainTree));
                        Double_t currentBGTotalRatio= Double_t (NaddedBGToTrainTree)/(1+Double_t(NaddedSGToTrainTree+NaddedBGToTrainTree));

                        //cout << "NaddedSGToTrainTree  NaddedBGToTrainTree currentBGTotalRatio "<< endl;
                        //cout << NaddedSGToTrainTree << "  " << NaddedBGToTrainTree << " " << currentBGTotalRatio << endl;

                        Bool_t addBGTrack=kFALSE;
                        Double_t UniformTestNumber=gRandom->Uniform();

                        //cout << "UniformTestNumber    = " << UniformTestNumber << endl;
                        //cout << "currentBGTotalRatio  = " << currentBGTotalRatio << endl;

                        if ( UniformTestNumber > currentBGTotalRatio) addBGTrack=kTRUE;
                        if (N3_ANN_EQUALIZESGBG==0) addBGTrack=kTRUE;


                        //cout << "Do we add this (BG-)track info in the simu-tree?" << addBGTrack << endl;
                        if (addBGTrack) {
                            simu->Fill();
                            simu_BG->Fill();
                            NaddedBGToTrainTree++;
                        }
                    }

                } // end of  if (N3_DoTrain==kTRUE); fill NN SG/BG Trees with the Inputvariables
                else {
                    N3_OutputValue=0;
                    // Adapt: array params should have as many entries as there are inputvariables.
                    //N3_OutputValue =TMlpANN->Evaluate(0, N3_Inputvar);
//                     cout << "        Evaluated value.... STILL to BE ADAPTED!!! "<< N3_OutputValue <<endl;

                    // This array is larger than possible used array for evaluation.
                    // The array with the right size is created, when the N3_ANN_INPUTNEURONS
                    // variable is fixed (TMLP class demands #arraysize = #inputneurons)
                    // This is a kind of dump workaround, but for now it should work.
//                     cout << "N3_ANN_INPUTNEURONS = "  << N3_ANN_INPUTNEURONS << endl;
                    Double_t EvalValue=0;
                    Double_t    N3_Evalvar4[4];
                    Double_t    N3_Evalvar8[8];
                    Double_t    N3_Evalvar12[12];
                    Double_t    N3_Evalvar16[16];
                    Double_t    N3_Evalvar20[20];
                    Double_t    N3_Evalvar24[24];

                    if (N3_ANN_INPUTNEURONS==4) {
                        for (int k=0; k<N3_ANN_INPUTNEURONS; ++k) N3_Evalvar4[k]= N3_Inputvar[k];
                        EvalValue=N3_TMLP_ANN->Evaluate(0,N3_Evalvar4);
                    }
                    else if (N3_ANN_INPUTNEURONS==8) {
                        for (int k=0; k<N3_ANN_INPUTNEURONS; ++k) N3_Evalvar8[k]= N3_Inputvar[k];
                        EvalValue=N3_TMLP_ANN->Evaluate(0,N3_Evalvar8);
                    }
                    else if (N3_ANN_INPUTNEURONS==12) {
                        for (int k=0; k<N3_ANN_INPUTNEURONS; ++k) N3_Evalvar12[k]= N3_Inputvar[k];
                        EvalValue=N3_TMLP_ANN->Evaluate(0,N3_Evalvar12);
                    }
                    else if (N3_ANN_INPUTNEURONS== 16) {
                        for (int k=0; k<N3_ANN_INPUTNEURONS; ++k) N3_Evalvar16[k]= N3_Inputvar[k];
                        EvalValue=N3_TMLP_ANN->Evaluate(0,N3_Evalvar16);
                    }
                    else if (N3_ANN_INPUTNEURONS==20) {
                        for (int k=0; k<N3_ANN_INPUTNEURONS; ++k) N3_Evalvar20[k]= N3_Inputvar[k];
                        EvalValue=N3_TMLP_ANN->Evaluate(0,N3_Evalvar20);
                    }
                    else {
                        for (int k=0; k<N3_ANN_INPUTNEURONS; ++k) N3_Evalvar24[k]= N3_Inputvar[k];
                        EvalValue=N3_TMLP_ANN->Evaluate(0,N3_Evalvar24);
                    }


                    // Set Output value:
                    N3_OutputValue = EvalValue;
                    //-------------------------------------------


                    /*
                                        Bool_t DebugCHECKMATCH=kTRUE;
                    //                     cout << "DEBUG-------------------------------------------------- " << endl;
                    //                     cout << "DEBUG   N3_ANN_INPUTNEURONS= " << N3_ANN_INPUTNEURONS << endl;
                                        // DEBUG CHECK OUTPUT: checked track is SG, and the evalutation output is wrong:
                                        if( InBT->MCEvt()==seg->MCEvt() ) {
                                            if (N3_OutputValue>CUT_PARAMETER[4]) {
                    //                             cout << "Checked Value is good!" << endl;
                                            }
                                            else {
                                                cout << "Checked Value is wrong!" << endl;
                                                DebugCHECKMATCH=kFALSE;
                                            }
                                        }
                                        else {
                                              if (N3_OutputValue<CUT_PARAMETER[4]) {
                    //                             cout << "Checked Value is good!" << endl;
                                            }
                                            else {
                                                cout << "Checked Value is wrong!" << endl;
                                                DebugCHECKMATCH=kFALSE;
                                            }
                                        }

                                        if (DebugCHECKMATCH==kFALSE) {
                                            cout << "DEBUG-------------------------------------------------- " << endl;
                                            cout << "!!! This output value does not mach the MC-Truth information. Print Segments:" << endl;
                                            cout << "Print InBT   and to be checked segment:" << endl;
                                        InBT->PrintNice();
                                        seg->PrintNice();
                                        cout << "EvalValue=Evaluate(0,N3_Evalvar[N3_ANN_INPUTNEURONS]) =    " << EvalValue << endl;
                                        cout << "DEBUG-------------------------------------------------- " << endl;
                                        }
                           */



                    /*
                    cout << "DEBUG   N3_ANN_INPUTNEURONS= " << N3_ANN_INPUTNEURONS << endl;
                    cout << "N3_Inputvar [0]= " <<  N3_Inputvar[0] << endl;
                    cout << "N3_Evalvar24[N3_ANN_INPUTNEURONS-1]= " << N3_Evalvar24[N3_ANN_INPUTNEURONS-1] << endl;
                    cout << "Print InBT   and to be checked segment:" << endl;
                    InBT->PrintNice();
                    seg->PrintNice();
                    cout << "EvalValue=Evaluate(0,N3_Evalvar[N3_ANN_INPUTNEURONS]) =    " << EvalValue << endl;
                    cout << "------------------------------" << endl;
                    */



                    CUT_PARAMETER[4]=N3_ANN_OUTPUTTHRESHOLD;
                    /*
                    if (N3_OutputValue>CUT_PARAMETER[4]) {
                        cout << "CUT_PARAMETER[4]=" << CUT_PARAMETER[4] << endl;
                        cout << "N3_OutputValue=" << N3_OutputValue << endl;
                        cout << "Add segment to GLOBAL_ShowerSegArray!: print segment once again: " << endl;
                        seg->PrintNice();
                    }
                    */

                } // end of  if (N3_DoTrain==kTRUE) else




                // Now apply cut conditions: NEW NEURAL NETWORK Alg  --------------------
                // CUT_PARAMETER[4] is  N3_ANN_OUTPUTTHRESHOLD
                if (N3_OutputValue<CUT_PARAMETER[4]) continue;
                // end of    cut conditions: NEW NEURAL NETWORK Alg  --------------------
//                 cout << "N3_OutputValue=" << N3_OutputValue << endl;

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    //cout << "TODO  REWRITE IF CONDITION USING EDBSEG METHODS!!!!!"<< endl;
                    ;    // do nothing;
                }
                else {
//                     cout << "add segment to GLOBAL_ShowerSegArray! " << endl;
//                     seg->PrintNice();
                    GLOBAL_ShowerSegArray -> Add(seg);
                }
            }

            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli
        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);

//         cout << "--- NaddedSGToTrainTree= " << NaddedSGToTrainTree << endl;
//         cout << "--- NaddedBGToTrainTree= " << NaddedBGToTrainTree << endl;


        // Delete the cloned, now unneccessary object, otherwise memory overload:
        delete InBTClone;

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }

        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;






    // SPECIAL ROUTINE FOR N3 ALG: Only in Case of Training:
    if (N3_DoTrain==kTRUE) {
        cout << " Now    write NN SG/BG Trees with the Inputvariables:  --------------------"<<endl;
        simu_SG->Print();
        simu_BG->Print();
        simu->Print();
        cout << " Now show first entry of SG/BG Trees with the Inputvariables:  --------------------"<<endl;
        cout << " Signal     Tree:  --------------------"<<endl;
        simu_SG->Show(0);
        cout << " Background Tree:  --------------------"<<endl;
        simu_BG->Show(0);
        simu->Show(0);
        cout << "--------------------"<<endl;
        // Create and Get the right TMLP for the right parameterset:
        // simu has to be filled first, thats the reason why the mlp is created here and not before.
        N3_Create_ALG_MLP(simu, GLOBAL_PARASETNR);
        cout << "void ReconstructShowers_N3()  N3_TMLP_ANN->GetStructure() " << N3_TMLP_ANN->GetStructure() << endl;
//         cout << "return now ............" << endl;
//         return;



        cout << " ------------------------------------------------------------" << endl;
        cout << "  NOW TRAIN THE NETWORK .... " << endl;
        N3_TMLP_ANN->Train(N3_ANN_NTRAINEPOCHS, "text,update=1");
        cout << "  NOW TRAIN THE NETWORK .... done." << endl;
        cout << "  Dump Weights into the file .... " << endl;
        N3_TMLP_ANN->DumpWeights("weights.txt");
        N3_Dump_ALG_MLP_weights(N3_TMLP_ANN,GLOBAL_PARASETNR);
        cout << "  Dump Weights into the file done. " << endl;

        // Use TMLPAnalyzer to see what it looks for
        cout << "  Use TMLPAnalyzer to see what it looks for " << endl;
        TCanvas* mlpa_canvas = new TCanvas("mlpa_canvas","Network analysis");
        mlpa_canvas->Divide(3,1);
        TMLPAnalyzer* ana = new TMLPAnalyzer(N3_TMLP_ANN);
        // Initialisation
        ana->GatherInformations();
        // output to the console
        ana->CheckNetwork();
        mlpa_canvas->cd(1);
        // shows how each variable influences the network
        ana->DrawDInputs();
        mlpa_canvas->cd(2);
        // shows the network structure
        N3_TMLP_ANN->Draw();
        mlpa_canvas->cd(3);
        // draws the resulting network
        ana->DrawNetwork(0,"N3_Type==1","N3_Type==0");
        mlpa_canvas->cd();

        cout << "  Use TMLPAnalyzer to see what it looks for... done." << endl;
        cout << " ------------------------------------------------------------" << endl;
        cout << "  NOW WRITE FILES to THE ROOT FILES .... " << endl;
        simu_SG->Write();
        simu_BG->Write();
        simu->Write();
        N3_TMLP_ANN->Write();
        mlpa_canvas->Write();
        ana->Write();
        N3_ALG_ANN_TrainingsFile->Close();
        cout << "  NOW WRITE FILES to THE ROOT FILES .... done. " << endl;

    } // Train==TRUE

    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_N3() done.");
    return;
}
//-------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------

void N3_Create_Layout()
{
    // only knowlegde about number of input neurons  and hidden layers needed.
    Log(2, "ShowRec.cpp", "--- void N3_Create_Layout() ---");
    TString layout="";
    TString newstring="";

    cout << "N3_Create_Layout()   N3_ANN_INPUTNEURONS =  " <<  N3_ANN_INPUTNEURONS  << endl;
    cout << "N3_Create_Layout()   N3_ANN_NHIDDENLAYER =  " <<  N3_ANN_NHIDDENLAYER  << endl;

    // ANN Input Layer
    for (Int_t loop=0; loop<N3_ANN_INPUTNEURONS-1; ++loop) {
        newstring=Form("@N3_Inputvar[%d],",loop);
        //newstring=Form("N3_Inputvar[%d],",loop);
        layout += newstring;
    }
    newstring=Form("@N3_Inputvar[%d]:",N3_ANN_INPUTNEURONS-1);
    //newstring=Form("N3_Inputvar[%d]:",N3_ANN_INPUTNEURONS-1);
    layout += newstring;

    //     Hidden Layers
    for (Int_t loop=0; loop<N3_ANN_NHIDDENLAYER; ++loop) {
        newstring=Form("%d:",N3_ANN_INPUTNEURONS);
        layout += newstring;
    }
    newstring="N3_Type";
    layout += newstring;

    // cout << "N3_Create_Layout() Print layout: " << endl;
    // cout << layout.Data() << endl;

    N3_ANN_Layout = layout;
    Log(2, "ShowRec.cpp", "--- void N3_Create_Layout() done.");
    return;
}

//-------------------------------------------------------------------------------------------

void N3_Create_ALG_MLP(TTree* simu, Int_t parasetnr)
{
    Log(2, "ShowRec.cpp", "--- void N3_Create_ALG_MLP() ---");

    if (!gROOT->GetClass("TMultiLayerPerceptron")) {
        gSystem->Load("libMLP");
    }

    N3_Create_Layout();
    cout << "N3_ANN_Layout:   " << N3_ANN_Layout.Data() << endl;
    // Full layout would look like:
//     N3_ANN_Layout:   N3_Inputvar[0],N3_Inputvar[1],N3_Inputvar[2],N3_Inputvar[3],N3_Inputvar[4],N3_Inputvar[5],N3_Inputvar[6],N3_Inputvar[7],N3_Inputvar[8],N3_Inputvar[9],N3_Inputvar[10],N3_Inputvar[11],N3_Inputvar[12],N3_Inputvar[13],N3_Inputvar[14],N3_Inputvar[15],N3_Inputvar[16],N3_Inputvar[17],N3_Inputvar[18],N3_Inputvar[19],N3_Inputvar[20],N3_Inputvar[21],N3_Inputvar[22],N3_Inputvar[23],N3_Inputvar[24],N3_Inputvar[25],N3_Inputvar[26],N3_Inputvar[27],N3_Inputvar[28]:29:29:29:29:29:N3_Type

    // Create the network:
    // Attention: for training case, the tree "simu" has already to be filled with the entries.
    N3_TMLP_ANN = new TMultiLayerPerceptron(N3_ANN_Layout, simu, "(Entry$)%2", "(Entry$+1)%2");
//     N3_TMLP_ANN = new TMultiLayerPerceptron(N3_ANN_Layout, simu, "(Entry$)%5", "(Entry$+1)%5");
    // This network does use the dZ to InBT (variable N3_Inputvar[0]) as primary weight
// N3_TMLP_ANN = new TMultiLayerPerceptron(N3_ANN_Layout, "N3_Inputvar[0]", simu, "(Entry$)%2", "(Entry$+1)%2");
    //N3_TMLP_ANN->Print();

    Log(2, "ShowRec.cpp", "--- void N3_Create_ALG_MLP() done.");
    return;
}

//-------------------------------------------------------------------------------------------

void N3_Load_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr)
{
    Log(2, "ShowRec.cpp", "--- void N3_Load_ALG_MLP_weights() ---");
    cout << "parasetnr = " << parasetnr << endl;
    cout << "weightfilestring = " << Form("N3_ANN_WEIGHTS_PARASET_%d.txt",parasetnr) << endl;
    mlp->LoadWeights(Form("N3_ANN_WEIGHTS_PARASET_%d.txt",parasetnr));
    mlp->GetStructure();
    mlp->Print();
    Log(2, "ShowRec.cpp", "--- void N3_Load_ALG_MLP_weights() done.");
    return;
}

//-------------------------------------------------------------------------------------------

void N3_Dump_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr)
{
    Log(2, "ShowRec.cpp", "--- void N3_Dump_ALG_MLP_weights() ---");
    cout << "parasetnr = " << parasetnr << endl;
    cout << "weightfilestring = " << Form("N3_ANN_WEIGHTS_PARASET_%d.txt",parasetnr) << endl;
    mlp->DumpWeights(Form("N3_ANN_WEIGHTS_PARASET_%d.txt",parasetnr));
    mlp->GetStructure();
    mlp->Print();
    Log(2, "ShowRec.cpp", "--- void N3_Dump_ALG_MLP_weights() done.");
    return;
}


//-------------------------------------------------------------------------------------------

void N3_ReadOptionFile()
{
    Log(2, "ShowRec.cpp", "--- void N3_ReadOptionFile() ---");
    cout << "Read OptionFile N3_Options.txt (no change of name possible currently)" << endl;

    // read parameters from par-file
    // return: 0 if ok
    //        -1 if file access failed
    char buf[256];
    char key[256];
    char name[256];
    const char *file="N3_Options.txt";

    FILE *fp = fopen(file,"r");
    if (!fp) {
        Log(1,"N3_ReadOptionFile","ERROR open file: %s", file);
        return;
    }
    else Log(2,"N3_ReadOptionFile","Read shower parameters from file: %s", file );

    Int_t id;
    Int_t int_eUse;
    int   dospec;
    TString tmpString;
    char tmpchar[256];

    while (fgets(buf, sizeof(buf), fp)) {
        for (Int_t i = 0; i < (Int_t)strlen(buf); ++i)
            if (buf[i]=='#')  {
                buf[i]='\0';                       // cut out comments starting from #
                break;
            }

        if ( sscanf(buf,"%s",key)!=1 ) continue;

        if ( !strcmp(key,"N3_TRAIN")  )
        {
            int train;
            sscanf(buf+strlen(key),"%d",&train);
            cout << "N3_ReadOptionFile   train = " << train << endl;
            N3_DoTrain=train;
        }
        else if ( !strcmp(key,"N3_TRAIN_NMAX")  )
        {
            int trainNMax;
            sscanf(buf+strlen(key),"%d",&trainNMax);
            cout << "N3_ReadOptionFile   trainNMax = " << trainNMax << endl;
            N3_TrainNMax=trainNMax;
        }

    } // end of while (fgets(buf, sizeof(buf), fp)) loop
    fclose(fp);


    Log(2, "ShowRec.cpp", "--- void N3_ReadOptionFile() done.");
    return;
}


//-------------------------------------------------------------------------------------------

void N3_FindBestCompliments( EdbSegP* seg, EdbPattern* TestPattern, EdbPVRec* local_gAli, Int_t Downstream, Float_t&  mindeltaZ,  Float_t& mindT, Float_t& mindR, Float_t& mindMinDist, Int_t &nDifferentSegs ) {

    Log(4, "ShowRec.cpp", "--- void N3_FindBestCompliments() ---");

    if (gEDBDEBUGLEVEL>3) {
        cout << "Looking for best compliments for segment " << seg << " of pattern " << TestPattern << " ... " << endl;
        cout << "Looking for pattern with Downstream = " << Downstream << endl;
        seg->PrintNice();
    }

    EdbPattern* OtherPattern = NULL;
    EdbPattern* OtherPatternInterim = NULL;

    // Speculative calculation of delta Z value. To be refined later.
    // See further down below.
    // It is basically seg->Z() - OtherPattern->Z().
    mindeltaZ = 0.0 - Downstream * 1300.0;

    // Dumb workaround for the selection method, but working.
    // Quick and dirty approach:
    if (Downstream==0) {
        // Will be the same plate.
        // Take care not to compare segment with segment itself further down!!
        OtherPattern = TestPattern;
    }
    else if (Downstream==1) {
        OtherPattern = local_gAli->GetPatternSucceding(TestPattern);
    }
    else if (Downstream==2) {
        OtherPatternInterim = local_gAli->GetPatternSucceding(TestPattern);
        if (NULL==OtherPatternInterim) {
            OtherPattern=NULL;
        }
        else {
            OtherPattern = local_gAli->GetPatternSucceding(OtherPatternInterim);
        }
    }
    else if (Downstream==-1) {
        OtherPattern = local_gAli->GetPatternPreceding(TestPattern);
    }
    else if (Downstream==-2) {
        OtherPatternInterim = local_gAli->GetPatternPreceding(TestPattern);
        if (NULL==OtherPatternInterim) {
            OtherPattern=NULL;
        }
        else {
            OtherPattern = local_gAli->GetPatternPreceding(OtherPatternInterim);
        }
    }
    else {
//         cout << "WARNING Downstream variable out of bounds. Return NULL as OtherPattern." << endl;
    }

    if (NULL == OtherPattern) {
//         cout << "WARNING NULL == OtherPattern. Return now." << endl;
        return;
    }

//     cout << "TestPattern has Z() of: " << TestPattern << " and next/overnext pattern has Z of: " <<  OtherPattern->Z() << endl;

    // How many segments are there in the check_pattern?
    Int_t nSeg = OtherPattern->N();
//     cout << "TestPattern has N entries: " << nSeg << " (one maybe the test segment)" <<  endl;
    // Whats the Z difference for segment and actual pattern?
    mindeltaZ = seg->Z() - OtherPattern->Z();

    Double_t testval_dR=0;
    Double_t testval_dT=0;
    Double_t testval_dminDins=0;
    Int_t    IDmindR = -1;
    Int_t    IDmindT = -1;
    Int_t    IDmindminDist = -1;

    // Reset minimal values to incredibly large values for first time check:
    mindT=9999999;
    mindR=9999999;
    mindMinDist=9999999;

    // Do finding routine now, simple comparison...
    for (Int_t i=0; i<nSeg; ++i) {
        EdbSegP* otherSeg = OtherPattern->GetSegment(i);
        // dont match segment with itsself:
        if (otherSeg==seg) continue;
        // always propagate from seg to other seg (from i to j)
        testval_dR = GetdR(seg, otherSeg);
        testval_dT = GetdeltaThetaSingleAngles(seg, otherSeg);
        testval_dminDins = GetdMinDist(seg, otherSeg);
        //printf("i otherSeg->ID()  testval_dR  testval_dT testval_dminDins %d, %d, %.03f  %.03f  %.03f \n",i,otherSeg->ID(), testval_dR ,testval_dT ,testval_dminDins );
        if (testval_dR < mindR ) {
            mindR = testval_dR;
            IDmindR = otherSeg->ID();
        }
        if (testval_dT < mindT ) {
            mindT = testval_dT;
            IDmindT = otherSeg->ID();
        }
        if (testval_dminDins < mindMinDist ) {
            mindMinDist = testval_dminDins;
            IDmindminDist = otherSeg->ID();
        }
    }

    /*
    cout << "Comparison for all segments gives following results: " << endl;
    printf("IDmindR =  %d, mindR = %.03f\n", IDmindR , mindR);
    printf("IDmindT =  %d, mindT = %.03f\n", IDmindT , mindT);
    printf("IDmindminDist =  %d, mindMinDist = %.03f\n", IDmindminDist , mindMinDist);
    */

    // Now check, how many different otherSeg ID() values we have...
//     cout << "Now check, how many different otherSeg ID() values we have..." << endl;
    // Comparing ID of mindR mit mindT
    Int_t ndifferent=3;
    if (IDmindR==IDmindT || IDmindR==IDmindminDist || IDmindminDist==IDmindT) {
        ndifferent=2;
        if (IDmindR==IDmindT && IDmindR==IDmindminDist && IDmindminDist==IDmindT) ndifferent=1;
    }
    if (nSeg==0) ndifferent=0; // no, if no other segment was around...
    if (nSeg==1 && OtherPattern==TestPattern)  ndifferent=0; // no, if no other segment was around...
    // Hand over new value:
    nDifferentSegs= ndifferent;
//     cout << "nDifferentSegs contributing to min values " << nDifferentSegs    <<   endl;

    Log(4, "ShowRec.cpp", "--- void N3_FindBestCompliments() done.");
    return;
}
