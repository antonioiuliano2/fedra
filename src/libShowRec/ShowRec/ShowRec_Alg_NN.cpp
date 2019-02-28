//-------------------------------------------------------------------------------------------
void ReconstructShowers_NN()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_NN() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of NN Algorithm
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


    // Define NN_ALG charactereistic variables:
    Float_t dT_InBT_To_TestBT=0;
    Float_t dR_InBT_To_TestBT=0;
    Float_t dR_TestBT_To_InBT=0;
    Float_t zDist_TestBT_To_InBT=0;
    Float_t SpatialDist_TestBT_To_InBT=0;
    Float_t zDiff_TestBT_To_InBT=0;
    Float_t dT_NextBT_To_TestBT=0;
    Float_t dR_NextBT_To_TestBT=0;
    Float_t mean_dT_2before=0;
    Float_t mean_dR_2before=0;
    Float_t  mean_dT_before=0;
    Float_t mean_dR_before=0;
    Float_t  mean_dT_same=0;
    Float_t mean_dR_same=0;
    Float_t  mean_dT_after=0;
    Float_t mean_dR_after=0;
    Float_t  mean_dT_2after=0;
    Float_t  mean_dR_2after=0;

    Float_t min_dT_2before=0;
    Float_t min_dR_2before=0;
    Float_t  min_dT_before=0;
    Float_t min_dR_before=0;
    Float_t  min_dT_same=0;
    Float_t min_dR_same=0;
    Float_t  min_dT_after=0;
    Float_t min_dR_after=0;
    Float_t  min_dT_2after=0;
    Float_t  min_dR_2after=0;
    Int_t nseg_1before=0;
    Int_t nseg_2before=0;
    Int_t nseg_3before=0;
    Int_t nseg_1after=0;
    Int_t nseg_2after=0;
    Int_t nseg_3after=0;
    Int_t nseg_same=0;
    Int_t type;

    // Variables and things important for neural Network:
    TTree *simu = new TTree("MonteCarlo", "Filtered Monte Carlo Events");
    simu->Branch("dT_InBT_To_TestBT", &dT_InBT_To_TestBT, "dT_InBT_To_TestBT/F");
    simu->Branch("dR_InBT_To_TestBT",  &dR_InBT_To_TestBT,  "dR_InBT_To_TestBT/F");
    simu->Branch("dR_TestBT_To_InBT",    &dR_TestBT_To_InBT,    "dR_TestBT_To_InBT/F");
    simu->Branch("zDiff_TestBT_To_InBT",  &zDiff_TestBT_To_InBT,  "zDiff_TestBT_To_InBT/F");
    simu->Branch("SpatialDist_TestBT_To_InBT",  &SpatialDist_TestBT_To_InBT,  "SpatialDist_TestBT_To_InBT/F");
    simu->Branch("nseg_1before",  &nseg_1before,  "nseg_1before/I");
    simu->Branch("nseg_2before",  &nseg_2before,  "nseg_2before/I");
    simu->Branch("nseg_same",  &nseg_same,  "nseg_same/I");
    simu->Branch("nseg_1after",  &nseg_1after,  "nseg_1after/I");
    simu->Branch("nseg_2after",  &nseg_2after,  "nseg_2after/I");
    //---------
    simu->Branch("mean_dT_2before",  &mean_dT_2before,  "mean_dT_2before/F");
    simu->Branch("mean_dT_before",  &mean_dT_before,  "mean_dT_before/F");
    simu->Branch("mean_dT_same",  &mean_dT_same,  "mean_dT_same/F");
    simu->Branch("mean_dT_after",  &mean_dT_after,  "mean_dT_after/F");
    simu->Branch("mean_dT_2after",  &mean_dT_2after,  "mean_dT_2after/F");
    //---------
    simu->Branch("mean_dR_2before",  &mean_dR_2before,  "mean_dR_2before/F");
    simu->Branch("mean_dR_before",  &mean_dR_before,  "mean_dR_before/F");
    simu->Branch("mean_dR_same",  &mean_dR_same,  "mean_dR_same/F");
    simu->Branch("mean_dR_after",  &mean_dR_after,  "mean_dR_after/F");
    simu->Branch("mean_dR_2after",  &mean_dR_2after,  "mean_dR_2after/F");
    //---------
    simu->Branch("min_dT_2before",  &min_dT_2before,  "min_dT_2before/F");
    simu->Branch("min_dT_before",  &min_dT_before,  "min_dT_before/F");
    simu->Branch("min_dT_same",  &min_dT_same,  "min_dT_same/F");
    simu->Branch("min_dT_after",  &min_dT_after,  "min_dT_after/F");
    simu->Branch("min_dT_2after",  &min_dT_2after,  "min_dT_2after/F");
    //---------
    simu->Branch("min_dR_2before",  &min_dR_2before,  "min_dR_2before/F");
    simu->Branch("min_dR_before",  &min_dR_before,  "min_dR_before/F");
    simu->Branch("min_dR_same",  &min_dR_same,  "min_dR_same/F");
    simu->Branch("min_dR_after",  &min_dR_after,  "min_dR_after/F");
    simu->Branch("min_dR_2after",  &min_dR_2after,  "min_dR_2after/F");

    simu->Branch("type",   &type,   "type/I");
    Double_t params[8];
    //------------------------------
    TTree *simu_SG = (TTree *)simu->Clone();//new TTree("TreeSignalBT", "TreeSignalBT");
    simu_SG->SetName("TreeSignalBT");
    simu_SG->SetTitle("TreeSignalBT");
    TTree *simu_BG = (TTree *)simu->Clone();// TTree *simu_BG = new TTree("TreeBackgroundBT", "TreeBackgroundBT");
    simu_BG->SetName("TreeBackgroundBT");
    simu_BG->SetTitle("TreeBackgroundBT");
    //------------------------------


    // Create Histograms:
    // Only in Case of Training:
    if (var_NN_DoTrain==kTRUE) {
        Create_NN_Alg_Histograms();
        cout << "histos created"<<endl;
    }

    //  Get the right TMLP for the right Parameterset:
    Get_NN_ALG_MLP(simu, GLOBAL_PARASETNR);
    Load_NN_ALG_MLP_weights(TMlpANN,GLOBAL_PARASETNR);
    cout << TMlpANN->GetStructure() << endl;
    //  return;

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
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
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


        //-----------------------------------
        // 1a) Reset characteristic variables:
        //-----------------------------------
        dT_InBT_To_TestBT=0;
        dR_InBT_To_TestBT=0;
        dR_TestBT_To_InBT=0;
        zDiff_TestBT_To_InBT=0;
        mean_dT_2before=0;
        mean_dR_2before=0;
        mean_dT_before=0;
        mean_dR_before=0;
        mean_dT_same=0;
        mean_dR_same=0;
        mean_dT_after=0;
        mean_dR_after=0;
        mean_dT_2after=0;
        mean_dR_2after=0;
        nseg_1before=0;
        nseg_2before=0;
        nseg_1after=0;
        nseg_2after=0;
        nseg_same=0;
        min_dT_2before=0;
        min_dR_2before=0;
        min_dT_before=0;
        min_dR_before=0;
        min_dT_same=0;
        min_dR_same=0;
        min_dT_after=0;
        min_dR_after=0;
        min_dT_2after=0;
        min_dR_2after=0;

        int ann_inputneurons=CUT_PARAMETER[1];


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            // Needed to have StepX,Y,TX,TY bins set for the FindCompliments Function.
            //EdbPattern* ActualPattern   =   (EdbPattern*)local_gAli->GetPattern(patterloop_cnt);
            //ActualPattern               ->  FillCell(20,20,0.01,0.01);

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();


                // Now    calculate NN Histogram Inputvariables:  --------------------
                // 1) zDiff_TestBT_To_InBT and SpatialDist_TestBT_To_InBT
                zDiff_TestBT_To_InBT=seg->Z()-InBT->Z();
                SpatialDist_TestBT_To_InBT=GetSpatialDist(seg,InBT);
                // 2) dT_InBT_To_TestBT
                dT_InBT_To_TestBT=GetdeltaThetaSingleAngles(seg,InBT);
                // 3) dR_InBT_To_TestBT (propagation order matters)
                //    In calculation it makes a difference if InBT is extrapolated to Z-pos of seg or vice versa.
                dR_InBT_To_TestBT=GetdeltaRWithPropagation(InBT, seg);
                dR_TestBT_To_InBT=GetdeltaRWithPropagation(seg,InBT);


                // 4) nseg_1before,nseg_2before,nseg_1after,nseg_2after:
                if (ann_inputneurons==10) {
                    nseg_2before = GetNSegBeforeAndAfter(local_gAli,patterloop_cnt,seg,2,-1);
                    nseg_1before = GetNSegBeforeAndAfter(local_gAli,patterloop_cnt,seg,1,-1);
                    nseg_same    = GetNSegBeforeAndAfter(local_gAli,patterloop_cnt,seg,0, 1);
                    nseg_1after  = GetNSegBeforeAndAfter(local_gAli,patterloop_cnt,seg,1, 1);
                    nseg_2after  = GetNSegBeforeAndAfter(local_gAli,patterloop_cnt,seg,2, 1);
                }

                // 5) mean_dT,dR nseg_1before,nseg_2before,nseg_1after,nseg_2after: mean of all dTheta and dR compliment segments:
                if (ann_inputneurons==20) {
                    GetMeansBeforeAndAfter(mean_dT_2before,mean_dR_2before,local_gAli,patterloop_cnt,seg,2,-1);
                    GetMeansBeforeAndAfter(mean_dT_before,mean_dR_before,local_gAli,patterloop_cnt,seg,1,-1);
                    GetMeansBeforeAndAfter(mean_dT_same,mean_dR_same,local_gAli,patterloop_cnt,seg,0, 1);
                    GetMeansBeforeAndAfter(mean_dT_after,mean_dR_after,local_gAli,patterloop_cnt,seg,1, 1);
                    GetMeansBeforeAndAfter(mean_dT_2after,mean_dR_2after,local_gAli,patterloop_cnt,seg,2, 1);
                }

                // 6) nseg_1before,nseg_2before,nseg_1after,nseg_2after: mean of all dTheta and dR compliment segments:
                if (ann_inputneurons==30) {
                    GetMinsBeforeAndAfter(min_dT_2before,min_dR_2before,local_gAli,patterloop_cnt,seg,2,-1);
                    GetMinsBeforeAndAfter(min_dT_before,min_dR_before,local_gAli,patterloop_cnt,seg,1,-1);
                    GetMinsBeforeAndAfter(min_dT_same,min_dR_same,local_gAli,patterloop_cnt,seg,0, 1);
                    GetMinsBeforeAndAfter(min_dT_after,min_dR_after,local_gAli,patterloop_cnt,seg,1, 1);
                    GetMinsBeforeAndAfter(min_dT_2after,min_dR_2after,local_gAli,patterloop_cnt,seg,2, 1);
                }
                // end of calculate NN Histogram Inputvariables:  --------------------



                // Now    fill NN SG/BG Trees with the Inputvariables:  --------------------
                // Only in Case of Training:
                if (var_NN_DoTrain==kTRUE) {
                    if (seg->MCEvt()>0) {
                        var_NN__SG__SpatialDist_TestBT_To_InBT->Fill(SpatialDist_TestBT_To_InBT);
                        var_NN__SG__zDiff_TestBT_To_InBT->Fill(zDiff_TestBT_To_InBT);
                        var_NN__SG__dT_InBT_To_TestBT->Fill(dT_InBT_To_TestBT);
                        var_NN__SG__dR_TestBT_To_InBT->Fill(dR_InBT_To_TestBT);
                        var_NN__SG__dR_InBT_To_TestBT->Fill(dR_TestBT_To_InBT);
                        var_NN__SG__nseg_TestBT_To2BeforePlate->Fill(nseg_2before);
                        var_NN__SG__nseg_TestBT_ToBeforePlate->Fill(nseg_1before);
                        var_NN__SG__nseg_TestBT_ToSamePlate->Fill(nseg_same);
                        var_NN__SG__nseg_TestBT_ToAfterPlate->Fill(nseg_1after);
                        var_NN__SG__nseg_TestBT_To2AfterPlate->Fill(nseg_2after);
                        //---------------
                        if (mean_dT_2before!=-1) var_NN__SG__mean_dT_TestBT_To2BeforePlate->Fill(mean_dT_2before);
                        if (mean_dR_2before!=-1) var_NN__SG__mean_dR_TestBT_To2BeforePlate->Fill(mean_dR_2before);
                        if (mean_dT_before!=-1) var_NN__SG__mean_dT_TestBT_ToBeforePlate->Fill(mean_dT_before);
                        if (mean_dR_before!=-1) var_NN__SG__mean_dR_TestBT_ToBeforePlate->Fill(mean_dR_before);
                        if (mean_dT_same!=-1) var_NN__SG__mean_dT_TestBT_ToSamePlate->Fill(mean_dT_same);
                        if (mean_dR_same!=-1) var_NN__SG__mean_dR_TestBT_ToSamePlate->Fill(mean_dR_same);
                        if (mean_dT_after!=-1) var_NN__SG__mean_dT_TestBT_ToAfterPlate->Fill(mean_dT_after);
                        if (mean_dR_after!=-1) var_NN__SG__mean_dR_TestBT_ToAfterPlate->Fill(mean_dR_after);
                        if (mean_dT_2after!=-1) var_NN__SG__mean_dT_TestBT_To2AfterPlate->Fill(mean_dT_2after);
                        if (mean_dR_2after!=-1) var_NN__SG__mean_dR_TestBT_To2AfterPlate->Fill(mean_dR_2after);
                        if (min_dT_2before!=-1) var_NN__SG__min_dT_TestBT_To2BeforePlate->Fill(min_dT_2before);
                        if (min_dR_2before!=-1) var_NN__SG__min_dR_TestBT_To2BeforePlate->Fill(min_dR_2before);
                        if (min_dT_before!=-1) var_NN__SG__min_dT_TestBT_ToBeforePlate->Fill(min_dT_before);
                        if (min_dR_before!=-1) var_NN__SG__min_dR_TestBT_ToBeforePlate->Fill(min_dR_before);
                        if (min_dT_same!=-1) var_NN__SG__min_dT_TestBT_ToSamePlate->Fill(min_dT_same);
                        if (min_dR_same!=-1) var_NN__SG__min_dR_TestBT_ToSamePlate->Fill(min_dR_same);
                        if (min_dT_after!=-1) var_NN__SG__min_dT_TestBT_ToAfterPlate->Fill(min_dT_after);
                        if (min_dR_after!=-1) var_NN__SG__min_dR_TestBT_ToAfterPlate->Fill(min_dR_after);
                        if (min_dT_2after!=-1) var_NN__SG__min_dT_TestBT_To2AfterPlate->Fill(min_dT_2after);
                        if (min_dR_2after!=-1) var_NN__SG__min_dR_TestBT_To2AfterPlate->Fill(min_dR_2after);
                        type=1;
                        simu_SG->Fill();
                        simu->Fill();
                    }
                    if (seg->MCEvt()<0) {
                        var_NN__BG__SpatialDist_TestBT_To_InBT->Fill(SpatialDist_TestBT_To_InBT);
                        var_NN__BG__zDiff_TestBT_To_InBT->Fill(zDiff_TestBT_To_InBT);
                        var_NN__BG__dT_InBT_To_TestBT->Fill(dT_InBT_To_TestBT);
                        var_NN__BG__dR_TestBT_To_InBT->Fill(dR_TestBT_To_InBT);
                        var_NN__BG__dR_InBT_To_TestBT->Fill(dR_InBT_To_TestBT);
                        var_NN__BG__nseg_TestBT_To2BeforePlate->Fill(nseg_2before);
                        var_NN__BG__nseg_TestBT_ToBeforePlate->Fill(nseg_1before);
                        var_NN__BG__nseg_TestBT_ToSamePlate->Fill(nseg_same);
                        var_NN__BG__nseg_TestBT_ToAfterPlate->Fill(nseg_1after);
                        var_NN__BG__nseg_TestBT_To2AfterPlate->Fill(nseg_2after);
                        //---------------
                        if (mean_dT_2before!=-1) var_NN__BG__mean_dT_TestBT_To2BeforePlate->Fill(mean_dT_2before);
                        if (mean_dR_2before!=-1) var_NN__BG__mean_dR_TestBT_To2BeforePlate->Fill(mean_dR_2before);
                        if (mean_dT_before!=-1) var_NN__BG__mean_dT_TestBT_ToBeforePlate->Fill(mean_dT_before);
                        if (mean_dR_before!=-1) var_NN__BG__mean_dR_TestBT_ToBeforePlate->Fill(mean_dR_before);
                        if (mean_dT_same!=-1) var_NN__BG__mean_dT_TestBT_ToSamePlate->Fill(mean_dT_same);
                        if (mean_dR_same!=-1) var_NN__BG__mean_dR_TestBT_ToSamePlate->Fill(mean_dR_same);
                        if (mean_dT_after!=-1) var_NN__BG__mean_dT_TestBT_ToAfterPlate->Fill(mean_dT_after);
                        if (mean_dR_after!=-1) var_NN__BG__mean_dR_TestBT_ToAfterPlate->Fill(mean_dR_after);
                        if (mean_dT_2after!=-1) var_NN__BG__mean_dT_TestBT_To2AfterPlate->Fill(mean_dT_2after);
                        if (mean_dR_2after!=-1) var_NN__BG__mean_dR_TestBT_To2AfterPlate->Fill(mean_dR_2after);
                        //---------------
                        if (min_dT_2before!=-1) var_NN__BG__min_dT_TestBT_To2BeforePlate->Fill(min_dT_2before);
                        if (min_dR_2before!=-1) var_NN__BG__min_dR_TestBT_To2BeforePlate->Fill(min_dR_2before);
                        if (min_dT_before!=-1) var_NN__BG__min_dT_TestBT_ToBeforePlate->Fill(min_dT_before);
                        if (min_dR_before!=-1) var_NN__BG__min_dR_TestBT_ToBeforePlate->Fill(min_dR_before);
                        if (min_dT_same!=-1) var_NN__BG__min_dT_TestBT_ToSamePlate->Fill(min_dT_same);
                        if (min_dR_same!=-1) var_NN__BG__min_dR_TestBT_ToSamePlate->Fill(min_dR_same);
                        if (min_dT_after!=-1) var_NN__BG__min_dT_TestBT_ToAfterPlate->Fill(min_dT_after);
                        if (min_dR_after!=-1) var_NN__BG__min_dR_TestBT_ToAfterPlate->Fill(min_dR_after);
                        if (min_dT_2after!=-1) var_NN__BG__min_dT_TestBT_To2AfterPlate->Fill(min_dT_2after);
                        if (min_dR_2after!=-1) var_NN__BG__min_dR_TestBT_To2AfterPlate->Fill(min_dR_2after);
                        type=0;
                        simu_BG->Fill();
                        simu->Fill();
                    }
                    if (seg->MCEvt()<0 || seg->MCEvt()>0) {
                        //simu->Show(simu->GetEntries()-1); // do nothing yet...
                    }

                } // if Train=TRUE
                // end of   fill NN SG/BG Trees with the Inputvariables:  --------------------


                Double_t params[30];
                //     Double_t params[5];
                params[0]=dT_InBT_To_TestBT;
                params[1]=dR_InBT_To_TestBT;
                params[2]=dR_TestBT_To_InBT;
                params[3]=zDiff_TestBT_To_InBT;
                params[4]=SpatialDist_TestBT_To_InBT;
                // /*
                //     Double_t params[15];
                params[5]=nseg_2before;
                params[6]=nseg_1before;
                params[7]=nseg_same;
                params[8]=nseg_1after;
                params[9]=nseg_2after;

                params[10]=mean_dT_2before;
                params[11]=mean_dT_before;
                params[12]=mean_dT_same;
                params[13]=mean_dT_after;
                params[14]=mean_dT_2after;

                params[15]=mean_dR_2before;
                params[16]=mean_dR_before;
                params[17]=mean_dR_same;
                params[18]=mean_dR_after;
                params[19]=mean_dR_2after;

                params[20]=min_dT_2before;
                params[21]=min_dT_before;
                params[22]=min_dT_same;
                params[23]=min_dT_after;
                params[24]=min_dT_2after;

                params[25]=min_dR_2before;
                params[26]=min_dR_before;
                params[27]=min_dR_same;
                params[28]=min_dR_after;
                params[29]=min_dR_2after;

                //---------
                //     */
                Double_t value=0;

                //for (int hj=0; hj<5; hj++) cout << "  " << params[hj];
                //cout << endl;
                value=TMlpANN->Evaluate(0, params);
                //if (gEDBDEBUGLEVEL>0) { for (int hj=0; hj<10; hj++) cout << "  " << params[hj];cout << "  , Vlaue= " << value << endl; }
                //      cout << "        Evaluated value...."<< value<<endl;

                // Now apply cut conditions: NEURAL NETWORK Alg  --------------------
                if (value<CUT_PARAMETER[0]) continue;
                // end of    cut conditions: NEURAL NETWORK Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    ;    // do nothing;
                }
                else {
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


        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
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




    // Only in Case of Training:
    if (var_NN_DoTrain==kTRUE) {
        cout << " Now    write NN SG/BG Trees with the Inputvariables:  --------------------"<<endl;

        TFile* fil = new TFile("NN_ALG_ANN_TrainingsFile_DefaultName.root","RECREATE");
        TCanvas* canv_SG = new TCanvas("canv_SG","canv_SG",800,800);
        canv_SG->Divide(5,4);
        canv_SG->cd(1);
        var_NN__SG__SpatialDist_TestBT_To_InBT->Draw();
        var_NN__SG__SpatialDist_TestBT_To_InBT->SetLineColor(2);
        var_NN__SG__SpatialDist_TestBT_To_InBT->Scale(1.0/var_NN__SG__SpatialDist_TestBT_To_InBT->Integral());
        var_NN__BG__SpatialDist_TestBT_To_InBT->Draw("same");
        var_NN__BG__SpatialDist_TestBT_To_InBT->SetLineColor(4);
        var_NN__BG__SpatialDist_TestBT_To_InBT->Scale(1.0/var_NN__BG__SpatialDist_TestBT_To_InBT->Integral());

        canv_SG->cd(2);
        var_NN__SG__dT_InBT_To_TestBT->Draw();
        var_NN__SG__dT_InBT_To_TestBT->SetLineColor(2);
        var_NN__SG__dT_InBT_To_TestBT->Scale(1.0/var_NN__SG__dT_InBT_To_TestBT->Integral());
        var_NN__BG__dT_InBT_To_TestBT->Draw("same");
        var_NN__BG__dT_InBT_To_TestBT->SetLineColor(4);
        var_NN__BG__dT_InBT_To_TestBT->Scale(1.0/var_NN__BG__dT_InBT_To_TestBT->Integral());

        canv_SG->cd(3);
        var_NN__SG__dR_InBT_To_TestBT->Draw();
        var_NN__SG__dR_InBT_To_TestBT->Draw();
        var_NN__SG__dR_InBT_To_TestBT->SetLineColor(2);
        var_NN__SG__dR_InBT_To_TestBT->Scale(1.0/var_NN__SG__dR_InBT_To_TestBT->Integral());
        var_NN__BG__dR_InBT_To_TestBT->Draw("same");
        var_NN__BG__dR_InBT_To_TestBT->SetLineColor(4);
        var_NN__BG__dR_InBT_To_TestBT->Scale(1.0/var_NN__BG__dR_InBT_To_TestBT->Integral());

        canv_SG->cd(4);
        var_NN__SG__nseg_TestBT_To2BeforePlate->Draw();
        var_NN__SG__nseg_TestBT_To2BeforePlate->SetLineColor(2);
        var_NN__SG__nseg_TestBT_To2BeforePlate->Scale(1.0/var_NN__SG__nseg_TestBT_To2BeforePlate->Integral());
        var_NN__BG__nseg_TestBT_To2BeforePlate->Draw("same");
        var_NN__BG__nseg_TestBT_To2BeforePlate->SetLineColor(4);
        var_NN__BG__nseg_TestBT_To2BeforePlate->Scale(1.0/var_NN__BG__nseg_TestBT_To2BeforePlate->Integral());

        canv_SG->cd(5);
        var_NN__SG__nseg_TestBT_ToBeforePlate->Draw();
        var_NN__SG__nseg_TestBT_ToBeforePlate->SetLineColor(2);
        var_NN__SG__nseg_TestBT_ToBeforePlate->Scale(1.0/var_NN__SG__nseg_TestBT_ToBeforePlate->Integral());
        var_NN__BG__nseg_TestBT_ToBeforePlate->Draw("same");
        var_NN__BG__nseg_TestBT_ToBeforePlate->SetLineColor(4);
        var_NN__BG__nseg_TestBT_ToBeforePlate->Scale(1.0/var_NN__BG__nseg_TestBT_ToBeforePlate->Integral());

        canv_SG->cd(6);
        var_NN__SG__nseg_TestBT_ToSamePlate->Draw();
        var_NN__SG__nseg_TestBT_ToSamePlate->SetLineColor(2);
        var_NN__SG__nseg_TestBT_ToSamePlate->Scale(1.0/var_NN__SG__nseg_TestBT_ToSamePlate->Integral());
        var_NN__BG__nseg_TestBT_ToSamePlate->Draw("same");
        var_NN__BG__nseg_TestBT_ToSamePlate->SetLineColor(4);
        var_NN__BG__nseg_TestBT_ToSamePlate->Scale(1.0/var_NN__BG__nseg_TestBT_ToSamePlate->Integral());

        canv_SG->cd(7);
        var_NN__SG__nseg_TestBT_ToAfterPlate->Draw();
        var_NN__SG__nseg_TestBT_ToAfterPlate->SetLineColor(2);
        var_NN__SG__nseg_TestBT_ToAfterPlate->Scale(1.0/var_NN__SG__nseg_TestBT_ToAfterPlate->Integral());
        var_NN__BG__nseg_TestBT_ToAfterPlate->Draw("same");
        var_NN__BG__nseg_TestBT_ToAfterPlate->SetLineColor(4);
        var_NN__BG__nseg_TestBT_ToAfterPlate->Scale(1.0/var_NN__BG__nseg_TestBT_ToAfterPlate->Integral());

        canv_SG->cd(8);
        var_NN__SG__nseg_TestBT_To2AfterPlate->Draw();
        var_NN__SG__nseg_TestBT_To2AfterPlate->SetLineColor(2);
        var_NN__SG__nseg_TestBT_To2AfterPlate->Scale(1.0/var_NN__SG__nseg_TestBT_To2AfterPlate->Integral());
        var_NN__BG__nseg_TestBT_To2AfterPlate->Draw("same");
        var_NN__BG__nseg_TestBT_To2AfterPlate->SetLineColor(4);
        var_NN__BG__nseg_TestBT_To2AfterPlate->Scale(1.0/var_NN__BG__nseg_TestBT_To2AfterPlate->Integral());


        //-----------------//-----------------//-----------------

        canv_SG->cd(9);
        var_NN__SG__mean_dT_TestBT_To2BeforePlate->Draw();
        var_NN__SG__mean_dT_TestBT_To2BeforePlate->SetLineColor(2);
        var_NN__SG__mean_dT_TestBT_To2BeforePlate->Scale(1.0/var_NN__SG__mean_dT_TestBT_To2BeforePlate->Integral());
        var_NN__BG__mean_dT_TestBT_To2BeforePlate->Draw("same");
        var_NN__BG__mean_dT_TestBT_To2BeforePlate->SetLineColor(4);
        var_NN__BG__mean_dT_TestBT_To2BeforePlate->Scale(1.0/var_NN__BG__mean_dT_TestBT_To2BeforePlate->Integral());

        canv_SG->cd(10);
        var_NN__SG__mean_dR_TestBT_To2BeforePlate->Draw();
        var_NN__SG__mean_dR_TestBT_To2BeforePlate->SetLineColor(2);
        var_NN__SG__mean_dR_TestBT_To2BeforePlate->Scale(1.0/var_NN__SG__mean_dR_TestBT_To2BeforePlate->Integral());
        var_NN__BG__mean_dR_TestBT_To2BeforePlate->Draw("same");
        var_NN__BG__mean_dR_TestBT_To2BeforePlate->SetLineColor(4);
        var_NN__BG__mean_dR_TestBT_To2BeforePlate->Scale(1.0/var_NN__BG__mean_dR_TestBT_To2BeforePlate->Integral());

        //-----------------

        canv_SG->cd(11);
        var_NN__SG__mean_dT_TestBT_ToBeforePlate->Draw();
        var_NN__SG__mean_dT_TestBT_ToBeforePlate->SetLineColor(2);
        var_NN__SG__mean_dT_TestBT_ToBeforePlate->Scale(1.0/var_NN__SG__mean_dT_TestBT_ToBeforePlate->Integral());
        var_NN__BG__mean_dT_TestBT_ToBeforePlate->Draw("same");
        var_NN__BG__mean_dT_TestBT_ToBeforePlate->SetLineColor(4);
        var_NN__BG__mean_dT_TestBT_ToBeforePlate->Scale(1.0/var_NN__BG__mean_dT_TestBT_ToBeforePlate->Integral());

        canv_SG->cd(12);
        var_NN__SG__mean_dR_TestBT_ToBeforePlate->Draw();
        var_NN__SG__mean_dR_TestBT_ToBeforePlate->SetLineColor(2);
        var_NN__SG__mean_dR_TestBT_ToBeforePlate->Scale(1.0/var_NN__SG__mean_dR_TestBT_ToBeforePlate->Integral());
        var_NN__BG__mean_dR_TestBT_ToBeforePlate->Draw("same");
        var_NN__BG__mean_dR_TestBT_ToBeforePlate->SetLineColor(4);
        var_NN__BG__mean_dR_TestBT_ToBeforePlate->Scale(1.0/var_NN__BG__mean_dR_TestBT_ToBeforePlate->Integral());

        //-----------------

        canv_SG->cd(13);
        var_NN__SG__mean_dT_TestBT_ToSamePlate->Draw();
        var_NN__SG__mean_dT_TestBT_ToSamePlate->SetLineColor(2);
        var_NN__SG__mean_dT_TestBT_ToSamePlate->Scale(1.0/var_NN__SG__mean_dT_TestBT_ToSamePlate->Integral());
        var_NN__BG__mean_dT_TestBT_ToSamePlate->Draw("same");
        var_NN__BG__mean_dT_TestBT_ToSamePlate->SetLineColor(4);
        var_NN__BG__mean_dT_TestBT_ToSamePlate->Scale(1.0/var_NN__BG__mean_dT_TestBT_ToSamePlate->Integral());

        canv_SG->cd(14);
        var_NN__SG__mean_dR_TestBT_ToSamePlate->Draw();
        var_NN__SG__mean_dR_TestBT_ToSamePlate->SetLineColor(2);
        var_NN__SG__mean_dR_TestBT_ToSamePlate->Scale(1.0/var_NN__SG__mean_dR_TestBT_ToSamePlate->Integral());
        var_NN__BG__mean_dR_TestBT_ToSamePlate->Draw("same");
        var_NN__BG__mean_dR_TestBT_ToSamePlate->SetLineColor(4);
        var_NN__BG__mean_dR_TestBT_ToSamePlate->Scale(1.0/var_NN__BG__mean_dR_TestBT_ToSamePlate->Integral());

        //-----------------

        canv_SG->cd(15);
        var_NN__SG__mean_dT_TestBT_ToAfterPlate->Draw();
        var_NN__SG__mean_dT_TestBT_ToAfterPlate->SetLineColor(2);
        var_NN__SG__mean_dT_TestBT_ToAfterPlate->Scale(1.0/var_NN__SG__mean_dT_TestBT_ToAfterPlate->Integral());
        var_NN__BG__mean_dT_TestBT_ToAfterPlate->Draw("same");
        var_NN__BG__mean_dT_TestBT_ToAfterPlate->SetLineColor(4);
        var_NN__BG__mean_dT_TestBT_ToAfterPlate->Scale(1.0/var_NN__BG__mean_dT_TestBT_ToAfterPlate->Integral());

        canv_SG->cd(16);
        var_NN__SG__mean_dR_TestBT_ToAfterPlate->Draw();
        var_NN__SG__mean_dR_TestBT_ToAfterPlate->SetLineColor(2);
        var_NN__SG__mean_dR_TestBT_ToAfterPlate->Scale(1.0/var_NN__SG__mean_dR_TestBT_ToAfterPlate->Integral());
        var_NN__BG__mean_dR_TestBT_ToAfterPlate->Draw("same");
        var_NN__BG__mean_dR_TestBT_ToAfterPlate->SetLineColor(4);
        var_NN__BG__mean_dR_TestBT_ToAfterPlate->Scale(1.0/var_NN__BG__mean_dR_TestBT_ToAfterPlate->Integral());


        canv_SG->cd(17);
        var_NN__SG__mean_dT_TestBT_To2AfterPlate->Draw();
        var_NN__SG__mean_dT_TestBT_To2AfterPlate->SetLineColor(2);
        var_NN__SG__mean_dT_TestBT_To2AfterPlate->Scale(1.0/var_NN__SG__mean_dT_TestBT_To2AfterPlate->Integral());
        var_NN__BG__mean_dT_TestBT_To2AfterPlate->Draw("same");
        var_NN__BG__mean_dT_TestBT_To2AfterPlate->SetLineColor(4);
        var_NN__BG__mean_dT_TestBT_To2AfterPlate->Scale(1.0/var_NN__BG__mean_dT_TestBT_To2AfterPlate->Integral());

        canv_SG->cd(18);
        var_NN__SG__mean_dR_TestBT_To2AfterPlate->Draw();
        var_NN__SG__mean_dR_TestBT_To2AfterPlate->SetLineColor(2);
        var_NN__SG__mean_dR_TestBT_To2AfterPlate->Scale(1.0/var_NN__SG__mean_dR_TestBT_To2AfterPlate->Integral());
        var_NN__BG__mean_dR_TestBT_To2AfterPlate->Draw("same");
        var_NN__BG__mean_dR_TestBT_To2AfterPlate->SetLineColor(4);
        var_NN__BG__mean_dR_TestBT_To2AfterPlate->Scale(1.0/var_NN__BG__mean_dR_TestBT_To2AfterPlate->Integral());

        canv_SG->Update();



        //-----------------//-----------------//-----------------
        TCanvas* canv_SG_2 = new TCanvas("canv_SG_2","canv_SG_2",800,800);
        canv_SG_2->Divide(4,4);
        canv_SG_2->cd(1);
        canv_SG_2->cd(1);
        var_NN__SG__min_dT_TestBT_To2BeforePlate->Draw();
        var_NN__SG__min_dT_TestBT_To2BeforePlate->SetLineColor(2);
        var_NN__SG__min_dT_TestBT_To2BeforePlate->Scale(1.0/var_NN__SG__min_dT_TestBT_To2BeforePlate->Integral());
        var_NN__BG__min_dT_TestBT_To2BeforePlate->Draw("same");
        var_NN__BG__min_dT_TestBT_To2BeforePlate->SetLineColor(4);
        var_NN__BG__min_dT_TestBT_To2BeforePlate->Scale(1.0/var_NN__BG__min_dT_TestBT_To2BeforePlate->Integral());

        canv_SG_2->cd(2);
        var_NN__SG__min_dR_TestBT_To2BeforePlate->Draw();
        var_NN__SG__min_dR_TestBT_To2BeforePlate->SetLineColor(2);
        var_NN__SG__min_dR_TestBT_To2BeforePlate->Scale(1.0/var_NN__SG__min_dR_TestBT_To2BeforePlate->Integral());
        var_NN__BG__min_dR_TestBT_To2BeforePlate->Draw("same");
        var_NN__BG__min_dR_TestBT_To2BeforePlate->SetLineColor(4);
        var_NN__BG__min_dR_TestBT_To2BeforePlate->Scale(1.0/var_NN__BG__min_dR_TestBT_To2BeforePlate->Integral());

        //-----------------

        canv_SG_2->cd(3);
        var_NN__SG__min_dT_TestBT_ToBeforePlate->Draw();
        var_NN__SG__min_dT_TestBT_ToBeforePlate->SetLineColor(2);
        var_NN__SG__min_dT_TestBT_ToBeforePlate->Scale(1.0/var_NN__SG__min_dT_TestBT_ToBeforePlate->Integral());
        var_NN__BG__min_dT_TestBT_ToBeforePlate->Draw("same");
        var_NN__BG__min_dT_TestBT_ToBeforePlate->SetLineColor(4);
        var_NN__BG__min_dT_TestBT_ToBeforePlate->Scale(1.0/var_NN__BG__min_dT_TestBT_ToBeforePlate->Integral());

        canv_SG_2->cd(4);
        var_NN__SG__min_dR_TestBT_ToBeforePlate->Draw();
        var_NN__SG__min_dR_TestBT_ToBeforePlate->SetLineColor(2);
        var_NN__SG__min_dR_TestBT_ToBeforePlate->Scale(1.0/var_NN__SG__min_dR_TestBT_ToBeforePlate->Integral());
        var_NN__BG__min_dR_TestBT_ToBeforePlate->Draw("same");
        var_NN__BG__min_dR_TestBT_ToBeforePlate->SetLineColor(4);
        var_NN__BG__min_dR_TestBT_ToBeforePlate->Scale(1.0/var_NN__BG__min_dR_TestBT_ToBeforePlate->Integral());

        //-----------------

        canv_SG_2->cd(5);
        var_NN__SG__min_dT_TestBT_ToSamePlate->Draw();
        var_NN__SG__min_dT_TestBT_ToSamePlate->SetLineColor(2);
        var_NN__SG__min_dT_TestBT_ToSamePlate->Scale(1.0/var_NN__SG__min_dT_TestBT_ToSamePlate->Integral());
        var_NN__BG__min_dT_TestBT_ToSamePlate->Draw("same");
        var_NN__BG__min_dT_TestBT_ToSamePlate->SetLineColor(4);
        var_NN__BG__min_dT_TestBT_ToSamePlate->Scale(1.0/var_NN__BG__min_dT_TestBT_ToSamePlate->Integral());

        canv_SG_2->cd(6);
        var_NN__SG__min_dR_TestBT_ToSamePlate->Draw();
        var_NN__SG__min_dR_TestBT_ToSamePlate->SetLineColor(2);
        var_NN__SG__min_dR_TestBT_ToSamePlate->Scale(1.0/var_NN__SG__min_dR_TestBT_ToSamePlate->Integral());
        var_NN__BG__min_dR_TestBT_ToSamePlate->Draw("same");
        var_NN__BG__min_dR_TestBT_ToSamePlate->SetLineColor(4);
        var_NN__BG__min_dR_TestBT_ToSamePlate->Scale(1.0/var_NN__BG__min_dR_TestBT_ToSamePlate->Integral());

        //-----------------

        canv_SG_2->cd(7);
        var_NN__SG__min_dT_TestBT_ToAfterPlate->Draw();
        var_NN__SG__min_dT_TestBT_ToAfterPlate->SetLineColor(2);
        var_NN__SG__min_dT_TestBT_ToAfterPlate->Scale(1.0/var_NN__SG__min_dT_TestBT_ToAfterPlate->Integral());
        var_NN__BG__min_dT_TestBT_ToAfterPlate->Draw("same");
        var_NN__BG__min_dT_TestBT_ToAfterPlate->SetLineColor(4);
        var_NN__BG__min_dT_TestBT_ToAfterPlate->Scale(1.0/var_NN__BG__min_dT_TestBT_ToAfterPlate->Integral());

        canv_SG_2->cd(8);
        var_NN__SG__min_dR_TestBT_ToAfterPlate->Draw();
        var_NN__SG__min_dR_TestBT_ToAfterPlate->SetLineColor(2);
        var_NN__SG__min_dR_TestBT_ToAfterPlate->Scale(1.0/var_NN__SG__min_dR_TestBT_ToAfterPlate->Integral());
        var_NN__BG__min_dR_TestBT_ToAfterPlate->Draw("same");
        var_NN__BG__min_dR_TestBT_ToAfterPlate->SetLineColor(4);
        var_NN__BG__min_dR_TestBT_ToAfterPlate->Scale(1.0/var_NN__BG__min_dR_TestBT_ToAfterPlate->Integral());


        canv_SG_2->cd(9);
        var_NN__SG__min_dT_TestBT_To2AfterPlate->Draw();
        var_NN__SG__min_dT_TestBT_To2AfterPlate->SetLineColor(2);
        var_NN__SG__min_dT_TestBT_To2AfterPlate->Scale(1.0/var_NN__SG__min_dT_TestBT_To2AfterPlate->Integral());
        var_NN__BG__min_dT_TestBT_To2AfterPlate->Draw("same");
        var_NN__BG__min_dT_TestBT_To2AfterPlate->SetLineColor(4);
        var_NN__BG__min_dT_TestBT_To2AfterPlate->Scale(1.0/var_NN__BG__min_dT_TestBT_To2AfterPlate->Integral());

        canv_SG_2->cd(10);
        var_NN__SG__min_dR_TestBT_To2AfterPlate->Draw();
        var_NN__SG__min_dR_TestBT_To2AfterPlate->SetLineColor(2);
        var_NN__SG__min_dR_TestBT_To2AfterPlate->Scale(1.0/var_NN__SG__min_dR_TestBT_To2AfterPlate->Integral());
        var_NN__BG__min_dR_TestBT_To2AfterPlate->Draw("same");
        var_NN__BG__min_dR_TestBT_To2AfterPlate->SetLineColor(4);
        var_NN__BG__min_dR_TestBT_To2AfterPlate->Scale(1.0/var_NN__BG__min_dR_TestBT_To2AfterPlate->Integral());

        canv_SG_2->cd(11);
        var_NN__SG__zDiff_TestBT_To_InBT->Draw();
        var_NN__SG__zDiff_TestBT_To_InBT->SetLineColor(2);
        var_NN__SG__zDiff_TestBT_To_InBT->Scale(1.0/var_NN__SG__zDiff_TestBT_To_InBT->Integral());
        var_NN__BG__zDiff_TestBT_To_InBT->Draw("same");
        var_NN__BG__zDiff_TestBT_To_InBT->SetLineColor(4);
        var_NN__BG__zDiff_TestBT_To_InBT->Scale(1.0/var_NN__BG__zDiff_TestBT_To_InBT->Integral());


        canv_SG_2->cd(12);
        var_NN__SG__dR_TestBT_To_InBT->Draw();
        var_NN__SG__dR_TestBT_To_InBT->SetLineColor(2);
        var_NN__SG__dR_TestBT_To_InBT->Scale(1.0/var_NN__SG__dR_TestBT_To_InBT->Integral());
        var_NN__BG__dR_TestBT_To_InBT->Draw("same");
        var_NN__BG__dR_TestBT_To_InBT->SetLineColor(4);
        var_NN__BG__dR_TestBT_To_InBT->Scale(1.0/var_NN__BG__dR_TestBT_To_InBT->Integral());



        TCanvas* canv_BG = new TCanvas("canv_BG","canv_BG",800,800);
        simu_SG->Write();
        simu_BG->Write();
        simu->Write();
        canv_SG->Write();
        canv_SG_2->Write();
        fil->Close();

        delete canv_SG;
        delete canv_SG_2;
        delete canv_BG;
        delete fil;

    } // Train==TRUE


    return;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------

void Create_NN_Alg_Histograms()
{
    var_NN__BG__SpatialDist_TestBT_To_InBT = new TH1F("var_NN__BG__SpatialDist_TestBT_To_InBT","var_NN__BG__SpatialDist_TestBT_To_InBT",1000,0,60000);
    var_NN__SG__SpatialDist_TestBT_To_InBT = new TH1F("var_NN__SG__SpatialDist_TestBT_To_InBT","var_NN__SG__SpatialDist_TestBT_To_InBT",1000,0,60000);
    var_NN__ALL__SpatialDist_TestBT_To_InBT = new TH1F("var_NN__ALL__SpatialDist_TestBT_To_InBT","var_NN__ALL__SpatialDist_TestBT_To_InBT",1000,0,60000);

    var_NN__BG__zDiff_TestBT_To_InBT = new TH1F("var_NN__BG__zDiff_TestBT_To_InBT","var_NN__BG__zDiff_TestBT_To_InBT",1000,0,60000);
    var_NN__SG__zDiff_TestBT_To_InBT = new TH1F("var_NN__SG__zDiff_TestBT_To_InBT","var_NN__SG__zDiff_TestBT_To_InBT",1000,0,60000);
    var_NN__ALL__zDiff_TestBT_To_InBT = new TH1F("var_NN__ALL__zDiff_TestBT_To_InBT","var_NN__ALL__zDiff_TestBT_To_InBT",1000,0,60000);


    var_NN__SG__dT_InBT_To_TestBT = new TH1F("var_NN__SG__dT_InBT_To_TestBT","var_NN__SG__dT_InBT_To_TestBT",100,0,1);
    var_NN__BG__dT_InBT_To_TestBT = new TH1F("var_NN__BG__dT_InBT_To_TestBT","var_NN__BG__dT_InBT_To_TestBT",100,0,1);
    var_NN__ALL__dT_InBT_To_TestBT = new TH1F("var_NN__ALL__dT_InBT_To_TestBT","var_NN__ALL__dT_InBT_To_TestBT",100,0,1);

    var_NN__SG__dR_InBT_To_TestBT = new TH1F("var_NN__SG__dR_InBT_To_TestBT","var_NN__SG__dR_InBT_To_TestBT",100,0,2000);
    var_NN__BG__dR_InBT_To_TestBT = new TH1F("var_NN__BG__dR_InBT_To_TestBT","var_NN__BG__dR_InBT_To_TestBT",100,0,2000);
    var_NN__ALL__dR_InBT_To_TestBT = new TH1F("var_NN__ALL__dR_InBT_To_TestBT","var_NN__ALL__dR_InBT_To_TestBT",100,0,2000);

    var_NN__SG__dR_TestBT_To_InBT = new TH1F("var_NN__SG__dR_TestBT_To_InBT","var_NN__SG__dR_TestBT_To_InBT",100,0,2000);
    var_NN__BG__dR_TestBT_To_InBT = new TH1F("var_NN__BG__dR_TestBT_To_InBT","var_NN__BG__dR_TestBT_To_InBT",100,0,2000);
    var_NN__ALL__dR_TestBT_To_InBT = new TH1F("var_NN__ALL__dR_TestBT_To_InBT","var_NN__ALL__dR_TestBT_To_InBT",100,0,2000);



    var_NN__SG__nseg_TestBT_To2BeforePlate = new TH1F("var_NN__SG__nseg_TestBT_To2BeforePlate","var_NN__SG__nseg_TestBT_To2BeforePlate",10,0,10);
    var_NN__BG__nseg_TestBT_To2BeforePlate = new TH1F("var_NN__BG__nseg_TestBT_To2BeforePlate","var_NN__BG__nseg_TestBT_To2BeforePlate",10,0,10);
    var_NN__ALL__nseg_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__nseg_TestBT_To2BeforePlate","var_NN__ALL__nseg_TestBT_To2BeforePlate",10,0,10);

    var_NN__BG__nseg_TestBT_ToBeforePlate = new TH1F("var_NN__BG__nseg_TestBT_ToBeforePlate","var_NN__BG__nseg_TestBT_ToBeforePlate",10,0,10);
    var_NN__SG__nseg_TestBT_ToBeforePlate = new TH1F("var_NN__SG__nseg_TestBT_ToBeforePlate","var_NN__SG__nseg_TestBT_ToBeforePlate",10,0,10);
    var_NN__ALL__nseg_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__nseg_TestBT_ToBeforePlate","var_NN__ALL__nseg_TestBT_ToBeforePlate",10,0,10);

    var_NN__BG__nseg_TestBT_ToSamePlate = new TH1F("var_NN__BG__nseg_TestBT_ToSamePlate","var_NN__BG__nseg_TestBT_ToSamePlate",10,0,10);
    var_NN__SG__nseg_TestBT_ToSamePlate = new TH1F("var_NN__SG__nseg_TestBT_ToSamePlate","var_NN__SG__nseg_TestBT_ToSamePlate",10,0,10);
    var_NN__ALL__nseg_TestBT_ToSamePlate = new TH1F("var_NN__ALL__nseg_TestBT_ToSamePlate","var_NN__ALL__nseg_TestBT_ToSamePlate",10,0,10);

    var_NN__BG__nseg_TestBT_To2AfterPlate = new TH1F("var_NN__BG__nseg_TestBT_To2AfterPlate","var_NN__BG__nseg_TestBT_To2AfterPlate",10,0,10);
    var_NN__SG__nseg_TestBT_To2AfterPlate = new TH1F("var_NN__SG__nseg_TestBT_To2AfterPlate","var_NN__SG__nseg_TestBT_To2AfterPlate",10,0,10);
    var_NN__ALL__nseg_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__nseg_TestBT_To2AfterPlate","var_NN__ALL__nseg_TestBT_To2AfterPlate",10,0,10);

    var_NN__BG__nseg_TestBT_ToAfterPlate = new TH1F("var_NN__BG__nseg_TestBT_ToAfterPlate","var_NN__BG__nseg_TestBT_ToAfterPlate",10,0,10);
    var_NN__SG__nseg_TestBT_ToAfterPlate = new TH1F("var_NN__SG__nseg_TestBT_ToAfterPlate","var_NN__SG__nseg_TestBT_ToAfterPlate",10,0,10);
    var_NN__ALL__nseg_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__nseg_TestBT_ToAfterPlate","var_NN__ALL__nseg_TestBT_ToAfterPlate",10,0,10);



    var_NN__BG__mean_dT_TestBT_ToSamePlate = new TH1F("var_NN__BG__mean_dT_TestBT_ToSamePlate","var_NN__BG__mean_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__SG__mean_dT_TestBT_ToSamePlate = new TH1F("var_NN__SG__mean_dT_TestBT_ToSamePlate","var_NN__SG__mean_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_ToSamePlate = new TH1F("var_NN__ALL__mean_dT_TestBT_ToSamePlate","var_NN__ALL__mean_dT_TestBT_ToSamePlate",100,0,0.2);

    var_NN__BG__mean_dR_TestBT_ToSamePlate = new TH1F("var_NN__BG__mean_dR_TestBT_ToSamePlate","var_NN__BG__mean_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__SG__mean_dR_TestBT_ToSamePlate = new TH1F("var_NN__SG__mean_dR_TestBT_ToSamePlate","var_NN__SG__mean_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_ToSamePlate = new TH1F("var_NN__ALL__mean_dR_TestBT_ToSamePlate","var_NN__ALL__mean_dR_TestBT_ToSamePlate",100,0,800);


    var_NN__SG__mean_dR_TestBT_ToAfterPlate = new TH1F("var_NN__SG__mean_dR_TestBT_ToAfterPlate","var_NN__SG__mean_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_ToAfterPlate = new TH1F("var_NN__BG__mean_dR_TestBT_ToAfterPlate","var_NN__BG__mean_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__mean_dR_TestBT_ToAfterPlate","var_NN__ALL__mean_dR_TestBT_ToAfterPlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_ToAfterPlate = new TH1F("var_NN__SG__mean_dT_TestBT_ToAfterPlate","var_NN__SG__mean_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_ToAfterPlate = new TH1F("var_NN__BG__mean_dT_TestBT_ToAfterPlate","var_NN__BG__mean_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__mean_dT_TestBT_ToAfterPlate","var_NN__ALL__mean_dT_TestBT_ToAfterPlate",100,0,0.2);


    var_NN__SG__mean_dR_TestBT_To2AfterPlate = new TH1F("var_NN__SG__mean_dR_TestBT_To2AfterPlate","var_NN__SG__mean_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_To2AfterPlate = new TH1F("var_NN__BG__mean_dR_TestBT_To2AfterPlate","var_NN__BG__mean_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__mean_dR_TestBT_To2AfterPlate","var_NN__ALL__mean_dR_TestBT_To2AfterPlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_To2AfterPlate = new TH1F("var_NN__SG__mean_dT_TestBT_To2AfterPlate","var_NN__SG__mean_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_To2AfterPlate = new TH1F("var_NN__BG__mean_dT_TestBT_To2AfterPlate","var_NN__BG__mean_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__mean_dT_TestBT_To2AfterPlate","var_NN__ALL__mean_dT_TestBT_To2AfterPlate",100,0,0.2);

    var_NN__SG__mean_dR_TestBT_ToBeforePlate = new TH1F("var_NN__SG__mean_dR_TestBT_ToBeforePlate","var_NN__SG__mean_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_ToBeforePlate = new TH1F("var_NN__BG__mean_dR_TestBT_ToBeforePlate","var_NN__BG__mean_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__mean_dR_TestBT_ToBeforePlate","var_NN__ALL__mean_dR_TestBT_ToBeforePlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_ToBeforePlate = new TH1F("var_NN__SG__mean_dT_TestBT_ToBeforePlate","var_NN__SG__mean_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_ToBeforePlate = new TH1F("var_NN__BG__mean_dT_TestBT_ToBeforePlate","var_NN__BG__mean_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__mean_dT_TestBT_ToBeforePlate","var_NN__ALL__mean_dT_TestBT_ToBeforePlate",100,0,0.2);


    var_NN__SG__mean_dR_TestBT_To2BeforePlate = new TH1F("var_NN__SG__mean_dR_TestBT_To2BeforePlate","var_NN__SG__mean_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_To2BeforePlate = new TH1F("var_NN__BG__mean_dR_TestBT_To2BeforePlate","var_NN__BG__mean_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__mean_dR_TestBT_To2BeforePlate","var_NN__ALL__mean_dR_TestBT_To2BeforePlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_To2BeforePlate = new TH1F("var_NN__SG__mean_dT_TestBT_To2BeforePlate","var_NN__SG__mean_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_To2BeforePlate = new TH1F("var_NN__BG__mean_dT_TestBT_To2BeforePlate","var_NN__BG__mean_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__mean_dT_TestBT_To2BeforePlate","var_NN__ALL__mean_dT_TestBT_To2BeforePlate",100,0,0.2);

    //---------

    var_NN__BG__min_dT_TestBT_ToSamePlate = new TH1F("var_NN__BG__min_dT_TestBT_ToSamePlate","var_NN__BG__min_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__SG__min_dT_TestBT_ToSamePlate = new TH1F("var_NN__SG__min_dT_TestBT_ToSamePlate","var_NN__SG__min_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_ToSamePlate = new TH1F("var_NN__ALL__min_dT_TestBT_ToSamePlate","var_NN__ALL__min_dT_TestBT_ToSamePlate",100,0,0.2);

    var_NN__BG__min_dR_TestBT_ToSamePlate = new TH1F("var_NN__BG__min_dR_TestBT_ToSamePlate","var_NN__BG__min_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__SG__min_dR_TestBT_ToSamePlate = new TH1F("var_NN__SG__min_dR_TestBT_ToSamePlate","var_NN__SG__min_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_ToSamePlate = new TH1F("var_NN__ALL__min_dR_TestBT_ToSamePlate","var_NN__ALL__min_dR_TestBT_ToSamePlate",100,0,800);


    var_NN__SG__min_dR_TestBT_ToAfterPlate = new TH1F("var_NN__SG__min_dR_TestBT_ToAfterPlate","var_NN__SG__min_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__BG__min_dR_TestBT_ToAfterPlate = new TH1F("var_NN__BG__min_dR_TestBT_ToAfterPlate","var_NN__BG__min_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__min_dR_TestBT_ToAfterPlate","var_NN__ALL__min_dR_TestBT_ToAfterPlate",100,0,800);

    var_NN__SG__min_dT_TestBT_ToAfterPlate = new TH1F("var_NN__SG__min_dT_TestBT_ToAfterPlate","var_NN__SG__min_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_ToAfterPlate = new TH1F("var_NN__BG__min_dT_TestBT_ToAfterPlate","var_NN__BG__min_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__min_dT_TestBT_ToAfterPlate","var_NN__ALL__min_dT_TestBT_ToAfterPlate",100,0,0.2);


    var_NN__SG__min_dR_TestBT_To2AfterPlate = new TH1F("var_NN__SG__min_dR_TestBT_To2AfterPlate","var_NN__SG__min_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__BG__min_dR_TestBT_To2AfterPlate = new TH1F("var_NN__BG__min_dR_TestBT_To2AfterPlate","var_NN__BG__min_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__min_dR_TestBT_To2AfterPlate","var_NN__ALL__min_dR_TestBT_To2AfterPlate",100,0,800);

    var_NN__SG__min_dT_TestBT_To2AfterPlate = new TH1F("var_NN__SG__min_dT_TestBT_To2AfterPlate","var_NN__SG__min_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_To2AfterPlate = new TH1F("var_NN__BG__min_dT_TestBT_To2AfterPlate","var_NN__BG__min_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__min_dT_TestBT_To2AfterPlate","var_NN__ALL__min_dT_TestBT_To2AfterPlate",100,0,0.2);

    var_NN__SG__min_dR_TestBT_ToBeforePlate = new TH1F("var_NN__SG__min_dR_TestBT_ToBeforePlate","var_NN__SG__min_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__BG__min_dR_TestBT_ToBeforePlate = new TH1F("var_NN__BG__min_dR_TestBT_ToBeforePlate","var_NN__BG__min_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__min_dR_TestBT_ToBeforePlate","var_NN__ALL__min_dR_TestBT_ToBeforePlate",100,0,800);

    var_NN__SG__min_dT_TestBT_ToBeforePlate = new TH1F("var_NN__SG__min_dT_TestBT_ToBeforePlate","var_NN__SG__min_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_ToBeforePlate = new TH1F("var_NN__BG__min_dT_TestBT_ToBeforePlate","var_NN__BG__min_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__min_dT_TestBT_ToBeforePlate","var_NN__ALL__min_dT_TestBT_ToBeforePlate",100,0,0.2);


    var_NN__SG__min_dR_TestBT_To2BeforePlate = new TH1F("var_NN__SG__min_dR_TestBT_To2BeforePlate","var_NN__SG__min_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__BG__min_dR_TestBT_To2BeforePlate = new TH1F("var_NN__BG__min_dR_TestBT_To2BeforePlate","var_NN__BG__min_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__min_dR_TestBT_To2BeforePlate","var_NN__ALL__min_dR_TestBT_To2BeforePlate",100,0,800);

    var_NN__SG__min_dT_TestBT_To2BeforePlate = new TH1F("var_NN__SG__min_dT_TestBT_To2BeforePlate","var_NN__SG__min_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_To2BeforePlate = new TH1F("var_NN__BG__min_dT_TestBT_To2BeforePlate","var_NN__BG__min_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__min_dT_TestBT_To2BeforePlate","var_NN__ALL__min_dT_TestBT_To2BeforePlate",100,0,0.2);

    return;
}

//-------------------------------------------------------------------------------------------

void Delete_NN_Alg_Histograms()
{
    delete var_NN__BG__SpatialDist_TestBT_To_InBT;
    delete var_NN__SG__SpatialDist_TestBT_To_InBT;
    delete var_NN__ALL__SpatialDist_TestBT_To_InBT;
    delete var_NN__BG__zDiff_TestBT_To_InBT;
    delete var_NN__SG__zDiff_TestBT_To_InBT;
    delete var_NN__ALL__zDiff_TestBT_To_InBT;
// -------------------------------
    delete var_NN__BG__dT_InBT_To_TestBT;
    delete var_NN__SG__dT_InBT_To_TestBT;
    delete var_NN__ALL__dT_InBT_To_TestBT;
    delete var_NN__SG__dR_InBT_To_TestBT;
    delete var_NN__BG__dR_InBT_To_TestBT;
    delete var_NN__ALL__dR_InBT_To_TestBT;
    delete var_NN__SG__dR_TestBT_To_InBT;
    delete var_NN__BG__dR_TestBT_To_InBT;
    delete var_NN__ALL__dR_TestBT_To_InBT;
// -------------------------------
    delete var_NN__SG__nseg_TestBT_ToBeforePlate;
    delete var_NN__BG__nseg_TestBT_ToBeforePlate;
    delete var_NN__ALL__nseg_TestBT_ToBeforePlate;
    delete var_NN__SG__nseg_TestBT_To2BeforePlate;
    delete var_NN__BG__nseg_TestBT_To2BeforePlate;
    delete var_NN__ALL__nseg_TestBT_To2BeforePlate;
    delete var_NN__BG__nseg_TestBT_ToSamePlate;
    delete var_NN__SG__nseg_TestBT_ToSamePlate;
    delete var_NN__ALL__nseg_TestBT_ToSamePlate;
    delete var_NN__SG__nseg_TestBT_ToAfterPlate;
    delete var_NN__BG__nseg_TestBT_ToAfterPlate;
    delete var_NN__ALL__nseg_TestBT_ToAfterPlate;
    delete var_NN__SG__nseg_TestBT_To2AfterPlate;
    delete var_NN__BG__nseg_TestBT_To2AfterPlate;
    delete var_NN__ALL__nseg_TestBT_To2AfterPlate;
    delete var_NN__SG__mean_dR_TestBT_ToBeforePlate;
    delete var_NN__BG__mean_dR_TestBT_ToBeforePlate;
    delete var_NN__ALL__mean_dR_TestBT_ToBeforePlate;
    delete var_NN__SG__mean_dR_TestBT_To2BeforePlate;
    delete var_NN__BG__mean_dR_TestBT_To2BeforePlate;
    delete var_NN__ALL__mean_dR_TestBT_To2BeforePlate;
    delete var_NN__SG__mean_dT_TestBT_ToBeforePlate;
    delete var_NN__BG__mean_dT_TestBT_ToBeforePlate;
    delete var_NN__ALL__mean_dT_TestBT_ToBeforePlate;
    delete var_NN__SG__mean_dT_TestBT_To2BeforePlate;
    delete var_NN__BG__mean_dT_TestBT_To2BeforePlate;
    delete var_NN__ALL__mean_dT_TestBT_To2BeforePlate;
    delete var_NN__SG__mean_dR_TestBT_ToSamePlate;
    delete var_NN__BG__mean_dR_TestBT_ToSamePlate;
    delete var_NN__ALL__mean_dR_TestBT_ToSamePlate;
    delete var_NN__SG__mean_dT_TestBT_ToSamePlate;
    delete var_NN__BG__mean_dT_TestBT_ToSamePlate;
    delete var_NN__ALL__mean_dT_TestBT_ToSamePlate;
    delete var_NN__SG__mean_dR_TestBT_ToAfterPlate;
    delete var_NN__BG__mean_dR_TestBT_ToAfterPlate;
    delete var_NN__ALL__mean_dR_TestBT_ToAfterPlate;
    delete var_NN__SG__mean_dT_TestBT_ToAfterPlate;
    delete var_NN__BG__mean_dT_TestBT_ToAfterPlate;
    delete var_NN__ALL__mean_dT_TestBT_ToAfterPlate;
    delete var_NN__SG__mean_dR_TestBT_To2AfterPlate;
    delete var_NN__BG__mean_dR_TestBT_To2AfterPlate;
    delete var_NN__ALL__mean_dR_TestBT_To2AfterPlate;
    delete var_NN__SG__mean_dT_TestBT_To2AfterPlate;
    delete var_NN__BG__mean_dT_TestBT_To2AfterPlate;
    delete var_NN__ALL__mean_dT_TestBT_To2AfterPlate;
    delete var_NN__SG__min_dR_TestBT_ToBeforePlate;
    delete var_NN__BG__min_dR_TestBT_ToBeforePlate;
    delete var_NN__ALL__min_dR_TestBT_ToBeforePlate;
    delete var_NN__SG__min_dR_TestBT_To2BeforePlate;
    delete var_NN__BG__min_dR_TestBT_To2BeforePlate;
    delete var_NN__ALL__min_dR_TestBT_To2BeforePlate;
    delete var_NN__SG__min_dT_TestBT_ToBeforePlate;
    delete var_NN__BG__min_dT_TestBT_ToBeforePlate;
    delete var_NN__ALL__min_dT_TestBT_ToBeforePlate;
    delete var_NN__SG__min_dT_TestBT_To2BeforePlate;
    delete var_NN__BG__min_dT_TestBT_To2BeforePlate;
    delete var_NN__ALL__min_dT_TestBT_To2BeforePlate;
    delete var_NN__SG__min_dR_TestBT_ToSamePlate;
    delete var_NN__BG__min_dR_TestBT_ToSamePlate;
    delete var_NN__ALL__min_dR_TestBT_ToSamePlate;
    delete var_NN__SG__min_dT_TestBT_ToSamePlate;
    delete var_NN__BG__min_dT_TestBT_ToSamePlate;
    delete var_NN__ALL__min_dT_TestBT_ToSamePlate;
    delete var_NN__SG__min_dR_TestBT_ToAfterPlate;
    delete var_NN__BG__min_dR_TestBT_ToAfterPlate;
    delete var_NN__ALL__min_dR_TestBT_ToAfterPlate;
    delete var_NN__SG__min_dT_TestBT_ToAfterPlate;
    delete var_NN__BG__min_dT_TestBT_ToAfterPlate;
    delete var_NN__ALL__min_dT_TestBT_ToAfterPlate;
    delete var_NN__SG__min_dR_TestBT_To2AfterPlate;
    delete var_NN__BG__min_dR_TestBT_To2AfterPlate;
    delete var_NN__ALL__min_dR_TestBT_To2AfterPlate;
    delete var_NN__SG__min_dT_TestBT_To2AfterPlate;
    delete var_NN__BG__min_dT_TestBT_To2AfterPlate;
    delete var_NN__ALL__min_dT_TestBT_To2AfterPlate;
}

//-------------------------------------------------------------------------------------------

void Get_NN_ALG_MLP(TTree* simu, Int_t parasetnr)
{
    if (!gROOT->GetClass("TMultiLayerPerceptron")) {
        gSystem->Load("libMLP");
    }

//-----------------------------------------------------------------------------
//-
//-
//-    WE USE FOR NN_ALG and the PARAMETERSETS For it the following conventions:
//-
//-    NCUTSteps =   20;
//-    MLP Structure:
//-
//-    a)  Paraset [0..20[
//-           first five variables.
//-    b)  Paraset [20..40[
//-           first ten variables.
//-    c)  Paraset [40..60[
//-           all (30) variables.
//-    d)  ....(something else)
//-
//-    As Trainingssample we use EXP2500 Events, Padi 199 ... 196 with b11845 BG
//-    additionally as BG samples b99999GENERICBG  and b99998GENERICBG
//-    (both with no signal...)
//-
//-
//-    Take care thet the added Traingssample has about the same traingssampledata for BG and SG
//-
//-----------------------------------------------------------------------------

// Define NN_ALG charactereistic variables:
    Float_t dT_InBT_To_TestBT=0;
    Float_t dR_InBT_To_TestBT=0;
    Float_t dR_TestBT_To_InBT=0;
    Float_t zDist_TestBT_To_InBT=0;
    Float_t SpatialDist_TestBT_To_InBT=0;
    Float_t zDiff_TestBT_To_InBT=0;
    Float_t dT_NextBT_To_TestBT=0;
    Float_t dR_NextBT_To_TestBT=0;
    Float_t mean_dT_2before=0;
    Float_t mean_dR_2before=0;
    Float_t  mean_dT_before=0;
    Float_t mean_dR_before=0;
    Float_t  mean_dT_same=0;
    Float_t mean_dR_same=0;
    Float_t  mean_dT_after=0;
    Float_t mean_dR_after=0;
    Float_t  mean_dT_2after=0;
    Float_t  mean_dR_2after=0;

    Float_t min_dT_2before=0;
    Float_t min_dR_2before=0;
    Float_t  min_dT_before=0;
    Float_t min_dR_before=0;
    Float_t  min_dT_same=0;
    Float_t min_dR_same=0;
    Float_t  min_dT_after=0;
    Float_t min_dR_after=0;
    Float_t  min_dT_2after=0;
    Float_t  min_dR_2after=0;
    Int_t nseg_1before=0;
    Int_t nseg_2before=0;
    Int_t nseg_3before=0;
    Int_t nseg_1after=0;
    Int_t nseg_2after=0;
    Int_t nseg_3after=0;
    Int_t nseg_same=0;
    Int_t type;

    TString layout="";

    int ann_inputneurons=CUT_PARAMETER[1];

    if (ann_inputneurons==5) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,@SpatialDist_TestBT_To_InBT:6:5:type";
    }
    if (ann_inputneurons==10) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,SpatialDist_TestBT_To_InBT,nseg_2before,nseg_1before,nseg_same,nseg_1after,nseg_2after:11:10:type";
    }
    if (ann_inputneurons==20) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,SpatialDist_TestBT_To_InBT,nseg_2before,nseg_1before,nseg_same,nseg_1after,nseg_2after,mean_dT_2before,mean_dT_before,mean_dT_same,mean_dT_after,mean_dT_2after,mean_dR_2before,mean_dR_before,mean_dR_same,mean_dR_after,mean_dR_2after:21:20:type";
    }
    if (ann_inputneurons==30) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,SpatialDist_TestBT_To_InBT,nseg_2before,nseg_1before,nseg_same,nseg_1after,nseg_2after,mean_dT_2before,mean_dT_before,mean_dT_same,mean_dT_after,mean_dT_2after,mean_dR_2before,mean_dR_before,mean_dR_same,mean_dR_after,mean_dR_2after,min_dT_2before,min_dT_before,min_dT_same,min_dT_after,min_dT_2after,min_dR_2before,min_dR_before,min_dR_same,min_dR_after,min_dR_2after:31:30:type";
    }

    cout << "ann_inputneurons:   " << ann_inputneurons << endl;
    cout << "Layout of ANN:   " << layout << endl;

// Create the network:
    TMlpANN = new TMultiLayerPerceptron(layout,simu,"(Entry$)%2","(Entry$+1)%2");

    return;
}

//-------------------------------------------------------------------------------------------

void Load_NN_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr)
{
    int ann_inputneurons=CUT_PARAMETER[1];

    if (ann_inputneurons==5) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_0_To_20.txt");
        gSystem->Exec("ls -ltr ANN_WEIGHTS_PARASET_0_To_20.txt");
    }
    if (ann_inputneurons==10) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_20_To_40.txt");
    }
    if (ann_inputneurons==20) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_40_To_60.txt");
    }
    if (ann_inputneurons==30) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_60_To_80.txt");
    }

    mlp->GetStructure();

    return;
}

//-------------------------------------------------------------------------------------------
