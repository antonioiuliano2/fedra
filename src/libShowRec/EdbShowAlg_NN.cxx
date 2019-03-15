#include "EdbShowAlg.h"
#include "EdbShowAlg_NN.h"
#include "EdbShowerP.h"

using namespace std;

ClassImp(EdbShowAlg_NN)
ClassImp(EdbShowAlg_N3)

//______________________________________________________________________________









//______________________________________________________________________________

EdbShowAlg_NN::EdbShowAlg_NN()
{
    // Default Constructor
    Log(2,"EdbShowAlg_NN::EdbShowAlg_NN","Default Constructor");

    // Reset all:
    Set0();

    eAlgName="NN";
    eAlgValue=3; // see default.par_SHOWREC for labeling (labeling identical with ShowRec program)

    //  Init with values according to NN Alg:
    Init();

}

//______________________________________________________________________________

EdbShowAlg_NN::~EdbShowAlg_NN()
{
    // Default Destructor
    Log(2,"EdbShowAlg_NN::~EdbShowAlg_NN","Default Destructor");
    if (eANNTree) {
        delete eANNTree;
        eANNTree=0;
    }
}

//______________________________________________________________________________

void EdbShowAlg_NN::Init()
{
    Log(2,"EdbShowAlg_NN::EdbShowAlg_NN","Init()");

    //  Init with values according to NN Alg:
    eParaValue[0]=5;
    eParaString[0]="INPUTNEURONS";
    eParaValue[1]=0.75;
    eParaString[1]="OUTPUT";       // NN output value

    eWeightFileString="weights.txt";
    eWeightFileLayoutString="layout";

    CreateANNTree();
    //cout << " eANNTree->Show(0)  " <<endl; eANNTree->Show(0);
    eTMlpANN = Create_NN_ALG_MLP(eANNTree, eParaValue[0]);

    // Standard Weights:
    SetANNWeightString();
    LoadANNWeights();

    return;
}

//______________________________________________________________________________


void EdbShowAlg_NN::Initialize()
{
    Log(2,"EdbShowAlg_NN::EdbShowAlg_NN","Initialize()");
    return;
}

//______________________________________________________________________________

void EdbShowAlg_NN::CreateANNTree()
{
    Log(2,"EdbShowAlg_NN::CreateANNTree","CreateANNTree()");

    if (!eANNTree) eANNTree = new TTree("EdbShowAlg_NN_eANNTree", "EdbShowAlg_NN_eANNTree");

    // Variables and things important for neural Network:
    eANNTree->Branch("dT_InBT_To_TestBT", &eANN_var_InBT_To_TestBT, "dT_InBT_To_TestBT/F");
    eANNTree->Branch("dR_InBT_To_TestBT",  &eANN_var_dR_InBT_To_TestBT,  "dR_InBT_To_TestBT/F");
    eANNTree->Branch("dR_TestBT_To_InBT",    &eANN_var_dR_TestBT_To_InBT,    "dR_TestBT_To_InBT/F");
    eANNTree->Branch("zDiff_TestBT_To_InBT",  &eANN_var_zDiff_TestBT_To_InBT,  "zDiff_TestBT_To_InBT/F");
    eANNTree->Branch("SpatialDist_TestBT_To_InBT",  &eANN_var_SpatialDist_TestBT_To_InBT,  "SpatialDist_TestBT_To_InBT/F");
    //---------
    eANNTree->Branch("nseg_1before",  &eANN_var_nseg_1before,  "nseg_1before/I");
    eANNTree->Branch("nseg_2before",  &eANN_var_nseg_2before,  "nseg_2before/I");
    eANNTree->Branch("nseg_same",  &eANN_var_nseg_same,  "nseg_same/I");
    eANNTree->Branch("nseg_1after",  &eANN_var_nseg_1after,  "nseg_1after/I");
    eANNTree->Branch("nseg_2after",  &eANN_var_nseg_2after,  "nseg_2after/I");
    //---------
    eANNTree->Branch("mean_dT_2before",  &eANN_var_mean_dT_2before,  "mean_dT_2before/F");
    eANNTree->Branch("mean_dT_before",  &eANN_var_mean_dT_before,  "mean_dT_before/F");
    eANNTree->Branch("mean_dT_same",  &eANN_var_mean_dT_same,  "mean_dT_same/F");
    eANNTree->Branch("mean_dT_after",  &eANN_var_mean_dT_after,  "mean_dT_after/F");
    eANNTree->Branch("mean_dT_2after",  &eANN_var_mean_dT_2after,  "mean_dT_2after/F");
    //---------
    eANNTree->Branch("mean_dR_2before",  &eANN_var_mean_dR_2before,  "mean_dR_2before/F");
    eANNTree->Branch("mean_dR_before",  &eANN_var_mean_dR_before,  "mean_dR_before/F");
    eANNTree->Branch("mean_dR_same",  &eANN_var_mean_dR_same,  "mean_dR_same/F");
    eANNTree->Branch("mean_dR_after",  &eANN_var_mean_dR_after,  "mean_dR_after/F");
    eANNTree->Branch("mean_dR_2after",  &eANN_var_mean_dR_2after,  "mean_dR_2after/F");
    //---------
    eANNTree->Branch("min_dT_2before",  &eANN_var_min_dT_2before,  "min_dT_2before/F");
    eANNTree->Branch("min_dT_before",  &eANN_var_min_dT_before,  "min_dT_before/F");
    eANNTree->Branch("min_dT_same",  &eANN_var_min_dT_same,  "min_dT_same/F");
    eANNTree->Branch("min_dT_after",  &eANN_var_min_dT_after,  "min_dT_after/F");
    eANNTree->Branch("min_dT_2after",  &eANN_var_min_dT_2after,  "min_dT_2after/F");
    //---------
    eANNTree->Branch("min_dR_2before",  &eANN_var_min_dR_2before,  "min_dR_2before/F");
    eANNTree->Branch("min_dR_before",  &eANN_var_min_dR_before,  "min_dR_before/F");
    eANNTree->Branch("min_dR_same",  &eANN_var_min_dR_same,  "min_dR_same/F");
    eANNTree->Branch("min_dR_after",  &eANN_var_min_dR_after,  "min_dR_after/F");
    eANNTree->Branch("min_dR_2after",  &eANN_var_min_dR_2after,  "min_dR_2after/F");
    //---------
    eANNTree->Branch("type",   &eANN_Inputtype,   "type/I");
    //---------
    Log(2,"EdbShowAlg_NN::CreateANNTree","CreateANNTree()...done.");
    return;
}


//______________________________________________________________________________

TMultiLayerPerceptron* EdbShowAlg_NN::Create_NN_ALG_MLP(TTree* simu, Int_t inputneurons)
{
    Log(2,"EdbShowAlg_NN::Create_NN_ALG_MLP","Creates the TTree for the data to be filled.");

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_NN::Create_NN_ALG_MLP()   inputneurons= " << inputneurons << endl;

    const char* layout="";
    if (inputneurons==5) {
        layout="eANN_var_dT_InBT_To_TestBT,eANN_var_dR_InBT_To_TestBT,eANN_var_dR_TestBT_To_InBT,eANN_var_zDiff_TestBT_To_InBT,eANN_var_SpatialDist_TestBT_To_InBT:6:5:type";
    }
    if (inputneurons==10) {
        layout="@eANN_var_dT_InBT_To_TestBT,@eANN_var_dR_InBT_To_TestBT,@eANN_var_dR_TestBT_To_InBT,@eANN_var_zDiff_TestBT_To_InBT,eANN_var_SpatialDist_TestBT_To_InBT,eANN_var_nseg_2before,eANN_var_nseg_1before,eANN_var_nseg_same,eANN_var_nseg_1after,eANN_var_nseg_2after:11:10:type";
    }
    if (inputneurons==20) {
        layout="@eANN_var_dT_InBT_To_TestBT,@eANN_var_dR_InBT_To_TestBT,@eANN_var_dR_TestBT_To_InBT,@eANN_var_zDiff_TestBT_To_InBT,eANN_var_SpatialDist_TestBT_To_InBT,eANN_var_nseg_2before,eANN_var_nseg_1before,eANN_var_nseg_same,eANN_var_nseg_1after,eANN_var_nseg_2after,eANN_var_mean_dT_2before,eANN_var_mean_dT_before,eANN_var_mean_dT_same,eANN_var_mean_dT_after,eANN_var_mean_dT_2after,eANN_var_mean_dR_2before,eANN_var_mean_dR_before,eANN_var_mean_dR_same,eANN_var_mean_dR_after,eANN_var_mean_dR_2after:21:20:type";
    }
    if (inputneurons==30) {
        layout="@eANN_var_dT_InBT_To_TestBT,@eANN_var_dR_InBT_To_TestBT,@eANN_var_dR_TestBT_To_InBT,@eANN_var_zDiff_TestBT_To_InBT,eANN_var_SpatialDist_TestBT_To_InBT,eANN_var_nseg_2before,eANN_var_nseg_1before,eANN_var_nseg_same,eANN_var_nseg_1after,eANN_var_nseg_2after,eANN_var_mean_dT_2before,eANN_var_mean_dT_before,eANN_var_mean_dT_same,eANN_var_mean_dT_after,eANN_var_mean_dT_2after,eANN_var_mean_dR_2before,eANN_var_mean_dR_before,eANN_var_mean_dR_same,eANN_var_mean_dR_after,eANN_var_mean_dR_2after,eANN_var_min_dT_2before,eANN_var_min_dT_before,eANN_var_min_dT_same,eANN_var_min_dT_after,eANN_var_min_dT_2after,eANN_var_min_dR_2before,eANN_var_min_dR_before,eANN_var_min_dR_same,eANN_var_min_dR_after,eANN_var_min_dR_2after:31:30:type";
    }

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_NN::Create_NN_ALG_MLP()   layout:    " << layout << endl;

    // Create the network:
    TMultiLayerPerceptron* TMlpANN = new TMultiLayerPerceptron(layout,simu);

    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbShowAlg_NN::Create_NN_ALG_MLP()   GetStructure:    " << endl;
        cout << TMlpANN->GetStructure() << endl;
    }
    Log(2,"EdbShowAlg_NN::Create_NN_ALG_MLP","Creates the TTree for the data to be filled...done.");
    return TMlpANN;
}

//______________________________________________________________________________


void EdbShowAlg_NN::SetANNWeightString()
{
    Log(2,"EdbShowAlg_NN::SetANNWeightString","SetANNWeightString()");
    int inputneurons=eParaValue[0];
    if (inputneurons==5)  eWeightFileString="ANN_WEIGHTS_PARASET_0_To_20.txt";
    if (inputneurons==10) eWeightFileString="ANN_WEIGHTS_PARASET_20_To_40.txt";
    if (inputneurons==20) eWeightFileString="ANN_WEIGHTS_PARASET_40_To_60.txt";
    if (inputneurons==30) eWeightFileString="ANN_WEIGHTS_PARASET_60_To_80.txt";

    if (gEDBDEBUGLEVEL>2) cout <<  "EdbShowAlg_NN::SetANNWeightString   "  << eWeightFileString << endl;
    Log(2,"EdbShowAlg_NN::SetANNWeightString","SetANNWeightString()...done.");
    return;
}


//______________________________________________________________________________


void EdbShowAlg_NN::LoadANNWeights()
{
    eTMlpANN->LoadWeights(eWeightFileString);
    return;
}


//______________________________________________________________________________


void EdbShowAlg_NN::LoadANNWeights(TMultiLayerPerceptron* TMlpANN, TString WeightFileString)
{
    TMlpANN->LoadWeights(WeightFileString);
    if (gEDBDEBUGLEVEL>2) cout <<  "EdbShowAlg_NN::LoadANNWeights   "  << WeightFileString << endl;
    return;
}






//______________________________________________________________________________

void EdbShowAlg_NN::Execute()
{
    Log(2,"EdbShowAlg_NN::Execute","Execute() DOING MAIN SHOWER RECONSTRUCTION HERE");

    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbSegP* seg;
    EdbShowerP* RecoShower;

    Bool_t    StillToLoop=kTRUE;
    Int_t     ActualPID;
    Int_t     newActualPID;
    Int_t     STEP=-1;
    Int_t     NLoopedPattern=0;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_NN::Execute--- STEP for patternloop direction =  " << STEP << endl;


    Double_t params[30]; // Used for ANN Evaluation

    //--- Loop over InBTs:

    // Since eInBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

        //-----------------------------------
        // 0a) Reset characteristic variables:
        //-----------------------------------
        eANN_var_dT_InBT_To_TestBT=0;
        eANN_var_dR_InBT_To_TestBT=0;
        eANN_var_dR_TestBT_To_InBT=0;
        eANN_var_zDiff_TestBT_To_InBT=0;
        eANN_var_mean_dT_2before=0;
        eANN_var_mean_dR_2before=0;
        eANN_var_mean_dT_before=0;
        eANN_var_mean_dR_before=0;
        eANN_var_mean_dT_same=0;
        eANN_var_mean_dR_same=0;
        eANN_var_mean_dT_after=0;
        eANN_var_mean_dR_after=0;
        eANN_var_mean_dT_2after=0;
        eANN_var_mean_dR_2after=0;
        eANN_var_nseg_1before=0;
        eANN_var_nseg_2before=0;
        eANN_var_nseg_1after=0;
        eANN_var_nseg_2after=0;
        eANN_var_nseg_same=0;
        eANN_var_min_dT_2before=0;
        eANN_var_min_dR_2before=0;
        eANN_var_min_dT_before=0;
        eANN_var_min_dR_before=0;
        eANN_var_min_dT_same=0;
        eANN_var_min_dR_same=0;
        eANN_var_min_dT_after=0;
        eANN_var_min_dR_after=0;
        eANN_var_min_dT_2after=0;
        eANN_var_min_dR_2after=0;

        //-----------------------------------
        // 1) Make eAli with cut parameters:
        //-----------------------------------

        // Create new EdbShowerP Object for storage;
        // See EdbShowerP why I have to call the Constructor as "unique" ideficable value
        //RecoShower = new EdbShowerP(i,eAlgValue);
        RecoShower = new EdbShowerP(i,eAlgValue,-1);

        // Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();

        // Add InBT to RecoShower:
        // This has to be done, since by definition the first BT in the RecoShower is the InBT.
        // Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
        cout << "Segment  (InBT) " << InBT << " was added to RecoShower." <<  endl;

        // Transform (make size smaller, extract only events having same MC) the  eAli  object:
        // See in Execute_CA for description.
        Transform_eAli(InBT,1400);

        // Add InBT to RecoShower // obsolete, since we loop also over the plate containing the InBT
        //RecoShower --> AddSegment(InBT);

        //-----------------------------------
        // 2) Loop over (whole) eAli, check BT for Cuts
        //-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_NN::Execute--- --- Doing patterloop " << ActualPID << endl;

            // just to adapt to this nomenclature of ShowRec program:
            int patterloop_cnt=ActualPID;


            for (Int_t btloop_cnt=0; btloop_cnt<eAli->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                Segment = (EdbSegP*)eAli->GetPattern(ActualPID)->GetSegment(btloop_cnt);

                // just to adapt to this nomenclature of ShowRec program
                seg=Segment;

                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();

                // Reset characteristic variables:
                eANN_var_dT_InBT_To_TestBT=0;
                eANN_var_dR_InBT_To_TestBT=0;
                eANN_var_dR_TestBT_To_InBT=0;
                eANN_var_zDiff_TestBT_To_InBT=0;
                eANN_var_mean_dT_2before=0;
                eANN_var_mean_dR_2before=0;
                eANN_var_mean_dT_before=0;
                eANN_var_mean_dR_before=0;
                eANN_var_mean_dT_same=0;
                eANN_var_mean_dR_same=0;
                eANN_var_mean_dT_after=0;
                eANN_var_mean_dR_after=0;
                eANN_var_mean_dT_2after=0;
                eANN_var_mean_dR_2after=0;
                eANN_var_nseg_1before=0;
                eANN_var_nseg_2before=0;
                eANN_var_nseg_1after=0;
                eANN_var_nseg_2after=0;
                eANN_var_nseg_same=0;
                eANN_var_min_dT_2before=0;
                eANN_var_min_dR_2before=0;
                eANN_var_min_dT_before=0;
                eANN_var_min_dR_before=0;
                eANN_var_min_dT_same=0;
                eANN_var_min_dR_same=0;
                eANN_var_min_dT_after=0;
                eANN_var_min_dR_after=0;
                eANN_var_min_dT_2after=0;
                eANN_var_min_dR_2after=0;

                // Now    calculate NN Inputvariables:  --------------------
                // 1) zDiff_TestBT_To_InBT and SpatialDist_TestBT_To_InBT
                eANN_var_zDiff_TestBT_To_InBT=seg->Z()-InBT->Z();
                eANN_var_SpatialDist_TestBT_To_InBT=GetSpatialDist(seg,InBT);
                // 2) dT_InBT_To_TestBT
                eANN_var_dT_InBT_To_TestBT=DeltaThetaSingleAngles(seg,InBT);
                // 3) dR_InBT_To_TestBT (propagation order matters)
                //    In calculation it makes a difference if InBT is extrapolated to Z-pos of seg or vice versa.
                eANN_var_dR_InBT_To_TestBT=DeltaR_WithPropagation(InBT, seg);
                eANN_var_dR_TestBT_To_InBT=DeltaR_WithPropagation(seg,InBT);

                // 4) nseg_1before,nseg_2before,nseg_1after,nseg_2after:
                if (eParaValue[0]>=10) {
                    eANN_var_nseg_2before = GetNSegBeforeAndAfter(eAli,patterloop_cnt,seg,2,-1);
                    eANN_var_nseg_1before = GetNSegBeforeAndAfter(eAli,patterloop_cnt,seg,1,-1);
                    eANN_var_nseg_same    = GetNSegBeforeAndAfter(eAli,patterloop_cnt,seg,0, 1);
                    eANN_var_nseg_1after  = GetNSegBeforeAndAfter(eAli,patterloop_cnt,seg,1, 1);
                    eANN_var_nseg_2after  = GetNSegBeforeAndAfter(eAli,patterloop_cnt,seg,2, 1);
                }

                // 5) mean_dT,dR nseg_1before,nseg_2before,nseg_1after,nseg_2after: mean of all dTheta and dR compliment segments:
                if (eParaValue[0]>=20) {
                    GetMeansBeforeAndAfter(eANN_var_mean_dT_2before,eANN_var_mean_dR_2before,eAli,patterloop_cnt,seg,2,-1);
                    GetMeansBeforeAndAfter(eANN_var_mean_dT_before,eANN_var_mean_dR_before,eAli,patterloop_cnt,seg,1,-1);
                    GetMeansBeforeAndAfter(eANN_var_mean_dT_same,eANN_var_mean_dR_same,eAli,patterloop_cnt,seg,0, 1);
                    GetMeansBeforeAndAfter(eANN_var_mean_dT_after,eANN_var_mean_dR_after,eAli,patterloop_cnt,seg,1, 1);
                    GetMeansBeforeAndAfter(eANN_var_mean_dT_2after,eANN_var_mean_dR_2after,eAli,patterloop_cnt,seg,2, 1);
                }

                // 6) nseg_1before,nseg_2before,nseg_1after,nseg_2after: mean of all dTheta and dR compliment segments:
                if (eParaValue[0]>=30) {
                    GetMinsBeforeAndAfter(eANN_var_min_dT_2before,eANN_var_min_dR_2before,eAli,patterloop_cnt,seg,2,-1);
                    GetMinsBeforeAndAfter(eANN_var_min_dT_before,eANN_var_min_dR_before,eAli,patterloop_cnt,seg,1,-1);
                    GetMinsBeforeAndAfter(eANN_var_min_dT_same,eANN_var_min_dR_same,eAli,patterloop_cnt,seg,0, 1);
                    GetMinsBeforeAndAfter(eANN_var_min_dT_after,eANN_var_min_dR_after,eAli,patterloop_cnt,seg,1, 1);
                    GetMinsBeforeAndAfter(eANN_var_min_dT_2after,eANN_var_min_dR_2after,eAli,patterloop_cnt,seg,2, 1);
                }
                // end of calculate NN Inputvariables:  --------------------

                // Fill ANN params Array:
                params[0]=eANN_var_dT_InBT_To_TestBT;
                params[1]=eANN_var_dR_InBT_To_TestBT;
                params[2]=eANN_var_dR_TestBT_To_InBT;
                params[3]=eANN_var_zDiff_TestBT_To_InBT;
                params[4]=eANN_var_SpatialDist_TestBT_To_InBT;
                params[5]=eANN_var_nseg_2before;
                params[6]=eANN_var_nseg_1before;
                params[7]=eANN_var_nseg_same;
                params[8]=eANN_var_nseg_1after;
                params[9]=eANN_var_nseg_2after;
                params[10]=eANN_var_mean_dT_2before;
                params[11]=eANN_var_mean_dT_before;
                params[12]=eANN_var_mean_dT_same;
                params[13]=eANN_var_mean_dT_after;
                params[14]=eANN_var_mean_dT_2after;
                params[15]=eANN_var_mean_dR_2before;
                params[16]=eANN_var_mean_dR_before;
                params[17]=eANN_var_mean_dR_same;
                params[18]=eANN_var_mean_dR_after;
                params[19]=eANN_var_mean_dR_2after;
                params[20]=eANN_var_min_dT_2before;
                params[21]=eANN_var_min_dT_before;
                params[22]=eANN_var_min_dT_same;
                params[23]=eANN_var_min_dT_after;
                params[24]=eANN_var_min_dT_2after;
                params[25]=eANN_var_min_dR_2before;
                params[26]=eANN_var_min_dR_before;
                params[27]=eANN_var_min_dR_same;
                params[28]=eANN_var_min_dR_after;
                params[29]=eANN_var_min_dR_2after;



                // Now apply cut conditions: NN Neural Network Alg --------------------
                Double_t value=0;
                value=eTMlpANN->Evaluate(0, params);
                if (gEDBDEBUGLEVEL>3) {
                    cout << "Inputvalues: ";
                    for (int hj=0; hj<5; hj++) cout << "  " << params[hj];
                    cout << "  , Value= " << value << "  eParaValue= " << eParaValue[1] << endl;
                }
                if (value<eParaValue[1]) continue;
                // end of    cut conditions: NN Neural Network Alg --------------------

                // If we arrive here, Basetrack  Segment  has passed criteria
                // and is then added to the RecoShower:
                // Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
                }
                cout << "Segment  " << Segment << " was added to &RecoShower  : " << &RecoShower  <<  endl;
            } // of btloop_cnt

            //------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_NN::Execute--- --- newActualPID= " << newActualPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_NN::Execute--- --- NLoopedPattern= " << NLoopedPattern << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_NN::Execute--- --- eNumberPlate_eAliPID= " << eNumberPlate_eAliPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_NN::Execute--- --- StillToLoop= " << StillToLoop << endl;

            // This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID) cout << "EdbShowAlg_NN::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
            // This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID) cout << "EdbShowAlg_NN::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
            // This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID) cout << "EdbShowAlg_NN::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

        // Obligatory when Shower Reconstruction is finished!
        RecoShower ->Update();
        //RecoShower ->PrintBasics();


        // Add Shower Object to Shower Reco Array.
        // Not, if its empty:
        // Not, if its containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

        // Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


    // Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    cout << "EdbShowAlg_NN::eRecoShowerArray() Entries: " << eRecoShowerArray->GetEntries() << endl;
    Log(2,"EdbShowAlg_NN::Execute","Execute() DOING MAIN SHOWER RECONSTRUCTION HERE...done.");
    return;
}

//______________________________________________________________________________







//______________________________________________________________________________


void EdbShowAlg_NN::Finalize()
{
    Log(2,"EdbShowAlg_NN::Finalize","Finalize(). Nothing done here yet.");
    return;
}


//______________________________________________________________________________
Int_t EdbShowAlg_NN::GetNSegBeforeAndAfter(EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter)
{
//   cout << "GetNSegBeforeAndAfter( XX, " << patterloop_cnt <<  " , Seg,  " <<  n_patterns << ",  " << BeforeOrAfter << endl;

    float Z_minus1=0;
    float Z_normal=local_gAli->GetPattern(patterloop_cnt)->Z();
    float Z_plus1=0;

    int npat=local_gAli->Npatterns();
    Bool_t edge_npat_upper=kFALSE;
    Bool_t edge_npat_lower=kFALSE;
    Int_t Factor=-1;

    if (patterloop_cnt==npat-1) {
        edge_npat_upper=kTRUE;
    }
    if (patterloop_cnt==0) {
        edge_npat_lower=kTRUE;
    }

    if (!edge_npat_lower) {
        Z_minus1=local_gAli->GetPattern(patterloop_cnt-1)->Z();
//     cout << "WHAT IS GREATER?   Z_normal  Z_minus1  " << Z_normal << "  " << Z_minus1 << endl;
        //Factor=(Int_t)TMath::Sign(Z_normal,Z_minus1);
        if (Z_minus1<Z_normal) Factor=1;
    }
    if (!edge_npat_upper) {
        Z_plus1 =local_gAli->GetPattern(patterloop_cnt+1)->Z();
//     cout << "WHAT IS GREATER?   Z_normal  Z_plus1  " << Z_normal << "  " << Z_plus1 << endl;
        if (Z_plus1>Z_normal) Factor=1;
    }

//   cout << Z_minus1 << endl;
//   cout << Z_normal << endl;
//   cout << Z_plus1 << endl;
//   cout << "Is edge_npat_lower = " << edge_npat_lower << endl;
//   cout << "Is edge_npat_upper = " << edge_npat_upper << endl;
//   cout << "Factor =  " << Factor << endl;

    // New PID we want to have:
    Int_t patterloop_test=patterloop_cnt+Factor*n_patterns*BeforeOrAfter;
//   cout << "So NEW n_patterns would be " << patterloop_test << endl;

    // Does this plate exist? If not, return 0 directly:
    if (patterloop_test>=npat || patterloop_test<0) {
        //cout << "So NEW n_patterns would be " << patterloop_test << " BUT IT DOES NOT MATHC in our local_gAli sceheme, which means its not existing. RETURNING 0 " << endl;
        return 0;
    }

    // Since we have checked now for bounds we can FindCompliments:
    TObjArray array;
    array.Clear();
    EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_test);
    TestPattern               ->  FillCell(20,20,0.01,0.01);
    int n_return = TestPattern->FindCompliments(*seg,array,3,3);
    //cout << " Found  " << n_return  << "  compliments in 2,2 sigma area:" << endl;

    return n_return;
}
//______________________________________________________________________________


Int_t EdbShowAlg_NN::GetMinsBeforeAndAfter(Float_t& min_dT, Float_t& min_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter)
{
//   cout << "GetMinsBeforeAndAfter( XX, " << patterloop_cnt <<  " , Seg,  " <<  n_patterns << ",  " << BeforeOrAfter << endl;
    min_dT=-1;
    min_dR=-1;

    float Z_minus1=0;
    float Z_normal=local_gAli->GetPattern(patterloop_cnt)->Z();
    float Z_plus1=0;

    int npat=local_gAli->Npatterns();
    Bool_t edge_npat_upper=kFALSE;
    Bool_t edge_npat_lower=kFALSE;
    Int_t Factor=-1;

    if (patterloop_cnt==npat-1) {
        edge_npat_upper=kTRUE;
    }
    if (patterloop_cnt==0) {
        edge_npat_lower=kTRUE;
    }

    if (!edge_npat_lower) {
        Z_minus1=local_gAli->GetPattern(patterloop_cnt-1)->Z();
        if (Z_minus1<Z_normal) Factor=1;
    }
    if (!edge_npat_upper) {
        Z_plus1 =local_gAli->GetPattern(patterloop_cnt+1)->Z();
        if (Z_plus1>Z_normal) Factor=1;
    }
    // New PID we want to have:
    Int_t patterloop_test=patterloop_cnt+Factor*n_patterns*BeforeOrAfter;

    // Does this plate exist? If not, return 0 directly:
    if (patterloop_test>=npat || patterloop_test<0) {
        //cout << "So NEW n_patterns would be " << patterloop_test << " BUT IT DOES NOT MATHC in our local_gAli sceheme, which means its not existing. RETURNING 0 " << endl;
        return 0;
    }

    // Since we have checked now for bounds we can FindCompliments:
    Int_t n_return=0;
    TObjArray array;
    array.Clear();
    EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_test);
    TestPattern               ->  FillCell(20,20,0.01,0.01);
    n_return = TestPattern->FindCompliments(*seg,array,3,3);
    //cout << " Found  " << n_return  << "  compliments in 2,2 sigma area:" << endl;

    if (n_return==0) return n_return;

    if (n_return==1) {
        EdbSegP* s_of_array=(EdbSegP*)array.At(0);
        min_dT=DeltaThetaSingleAngles(seg,s_of_array);
        min_dR=DeltaR_NoPropagation(seg,s_of_array);
    }

    Float_t tmp_min_dT=-1;
    Float_t tmp_min_dR=-1;
    Float_t tmp2_min_dT=-1;
    Float_t tmp2_min_dR=-1;
    Float_t angle;
    Float_t dist;

    if (n_return>1) {
        for (int i=0; i<n_return; i++) {
            EdbSegP* s_of_array=(EdbSegP*)array.At(i);
            if (i==0) {
                min_dT=999999;
                min_dR=9999999;
            }
            angle=(Float_t)DeltaThetaSingleAngles(seg,s_of_array);
            tmp_min_dT=min_dT;
            tmp2_min_dT=TMath::Min(angle, tmp_min_dT);
            min_dT=tmp2_min_dT;

            dist=(Float_t)DeltaR_NoPropagation(seg,s_of_array);
            tmp_min_dR=min_dR;
            tmp2_min_dR=TMath::Min(dist, tmp_min_dR);
            min_dR=tmp2_min_dR;
        }
    }
    return n_return;
}
//______________________________________________________________________________


//______________________________________________________________________________
Int_t EdbShowAlg_NN::GetMeansBeforeAndAfter(Float_t& mean_dT, Float_t& mean_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter)
{
//   cout << "GetMeansBeforeAndAfter( XX, " << patterloop_cnt <<  " , Seg,  " <<  n_patterns << ",  " << BeforeOrAfter << endl;
    mean_dT=-1;
    mean_dR=-1;

    float Z_minus1=0;
    float Z_normal=local_gAli->GetPattern(patterloop_cnt)->Z();
    float Z_plus1=0;

    int npat=local_gAli->Npatterns();
    Bool_t edge_npat_upper=kFALSE;
    Bool_t edge_npat_lower=kFALSE;
    Int_t Factor=-1;

    if (patterloop_cnt==npat-1) {
        edge_npat_upper=kTRUE;
    }
    if (patterloop_cnt==0) {
        edge_npat_lower=kTRUE;
    }

    if (!edge_npat_lower) {
        Z_minus1=local_gAli->GetPattern(patterloop_cnt-1)->Z();
        if (Z_minus1<Z_normal) Factor=1;
    }
    if (!edge_npat_upper) {
        Z_plus1 =local_gAli->GetPattern(patterloop_cnt+1)->Z();
        if (Z_plus1>Z_normal) Factor=1;
    }
    // New PID we want to have:
    Int_t patterloop_test=patterloop_cnt+Factor*n_patterns*BeforeOrAfter;

    // Does this plate exist? If not, return 0 directly:
    if (patterloop_test>=npat || patterloop_test<0) {
        //cout << "So NEW n_patterns would be " << patterloop_test << " BUT IT DOES NOT MATHC in our local_gAli sceheme, which means its not existing. RETURNING 0 " << endl;
        return 0;
    }

    // Since we have checked now for bounds we can FindCompliments:
    Int_t n_return=0;
    TObjArray array;
    array.Clear();
    EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_test);
    TestPattern               ->  FillCell(20,20,0.01,0.01);
    n_return = TestPattern->FindCompliments(*seg,array,3,3);
    //cout << " Found  " << n_return  << "  compliments in 2,2 sigma area:" << endl;

    if (n_return==0) return n_return;

    //seg->PrintNice();
    for (int i=0; i<n_return; i++) {
        EdbSegP* s_of_array=(EdbSegP*)array.At(i);
        if (i==0) {
            mean_dT=0;
            mean_dR=0;
        }
        //s_of_array->PrintNice();
        mean_dT+=DeltaThetaSingleAngles(seg,s_of_array);
        mean_dR+=DeltaR_NoPropagation(seg,s_of_array);
    }
    if (n_return>0) mean_dT=mean_dT/(Double_t)n_return;
    if (n_return>0) mean_dR=mean_dR/(Double_t)n_return;

    return n_return;
}
//______________________________________________________________________________










//______________________________________________________________________________

EdbShowAlg_N3::EdbShowAlg_N3(Bool_t ANN_DoTrain)
{
    // Constructor with Train/Run Switch
    Log(2,"EdbShowAlg_N3::EdbShowAlg_N3","Default Constructor ANN_DoTrain=%d",ANN_DoTrain);

    // Reset all:
    // Calls Set0 from inheriting function, so some values must be reset to NULL
    // manually, unless a new Set0() function is implemented -- which is not at
    // the moment:
    Set0();

    eWeightFileString="";
    eWeightFileLayoutString="";
    eANNTree=NULL;
    eTMlpANN=NULL;

    // see default.par_SHOWREC for labeling (labeling identical with ShowRec program)
    eAlgName="N3";
    eAlgValue=11;

    // Mostly it will be runnung, but can be set now here:
    eANN_DoTrain=ANN_DoTrain;

    //  Init with values according to N3 Alg:
    Init();

    Log(2,"EdbShowAlg_N3::EdbShowAlg_N3","Default Constructor ANN_DoTrain=%d...done.",ANN_DoTrain);
}



//______________________________________________________________________________

EdbShowAlg_N3::~EdbShowAlg_N3()
{
    // Default Destructor
    Log(2,"EdbShowAlg_N3::~EdbShowAlg_N3","Default Destructor");
    if (eANNTree) {
        delete eANNTree;
        eANNTree=0;
    }
}

//______________________________________________________________________________

void EdbShowAlg_N3::Init()
{
    Log(2,"EdbShowAlg_N3::EdbShowAlg_N3","Init()");

    Log(2,"EdbShowAlg_N3::EdbShowAlg_N3","Init with values according to N3 Alg   TO BE CHECKED !!");
    // Init with values according to N3 Alg:
    // TO BE CHECKED !!
    // Structure should be equal to the one in file:
    // PARAMETERSET_DEFINITIONFILE_N3_ALG.root
    eParaValue[0]=5;
    eParaString[0]="ANN_PLATE_DELTANMAX";
    eParaValue[1]=100;
    eParaString[1]="ANN_NTRAINEPOCHS";
    eParaValue[2]=7;
    eParaString[2]="ANN_NHIDDENLAYER";
    eParaValue[3]=0.8;
    eParaString[3]="ANN_OUTPUTTHRESHOLD";
    eParaValue[4]=0;
    eParaString[4]="ANN_EQUALIZESGBG";
    eParaValue[5]=24;
    eParaString[5]="N3_ANN_INPUTNEURONS";

    cout << "DEBUG::AGAIN, WHERE ARE THE ePARAVALUES SET???????"  << endl;

    eWeightFileString="weightsN3.txt";
    eWeightFileLayoutString="layoutN3";

    // Create Tree where the Variables for the N3 Neural Net are stored:
    CreateANNTree();
    eTMlpANN = Create_NN_ALG_MLP(eANNTree, eParaValue[5]);

    // Standard Weights:
    SetANNWeightString();
    LoadANNWeights();

    return;
}

//______________________________________________________________________________


void EdbShowAlg_N3::Initialize()
{
    Log(2,"EdbShowAlg_N3::EdbShowAlg_N3","Initialize()");
    return;
}

//______________________________________________________________________________

void EdbShowAlg_N3::CreateANNTree()
{
    Log(2,"EdbShowAlg_N3::CreateANNTree","CreateANNTree()");

    if (!eANNTree) eANNTree = new TTree("EdbShowAlg_N3_eANNTree", "EdbShowAlg_N3_eANNTree");

    // Variables and things important for neural Network:
    TTree *simu = new TTree("TreeSignalBackgroundBT", "TreeSignalBackgroundBT");
    eANNTree->Branch("eANN_Inputvar", eANN_Inputvar, "eANN_Inputvar[24]/D");
    eANNTree->Branch("eANN_Inputtype",   &eANN_Inputtype,   "eANN_Inputtype/I");

    eANNTree->Print();

    // Default, maximal settings. Same plate, Two plates up- downstream connections looking,
    // that for 4 inputvariables there
    // plus 4 fixed input variables for BT(i) to InBT connections: 4+5*4 = 24
    /*
    eANN_PLATE_DELTANMAX=5;
    eANN_NTRAINEPOCHS=100;
    eANN_NHIDDENLAYER=5;
    eANN_EQUALIZESGBG=0;
    eANN_OUTPUTTHRESHOLD=0.85;
    eANN_INPUTNEURONS=24;
    eANN_Inputtype=1;
    */

    cout << "DEBUG: Fill Tree with DUMMY variables --- TO BE CHANGED LATER"  <<  endl;
    for (int i=0; i<10; ++i) {
        for (int l=0; l<24; ++l) {
            eANN_Inputvar[l]=gRandom->Uniform();
        }
        eANN_Inputtype=i%2;
        eANNTree->Fill();
    }
    eANNTree->Show(0);
    eANNTree->Show(eANNTree->GetEntries()-1);
    cout << "DEBUG: Fill Tree with DUMMY variables --- TO BE CHANGED LATER  DONE.."  <<  endl;

    //---------
    Log(2,"EdbShowAlg_N3::CreateANNTree","CreateANNTree()...done.");
    return;
}


//______________________________________________________________________________

TMultiLayerPerceptron* EdbShowAlg_N3::Create_NN_ALG_MLP(TTree* simu, Int_t inputneurons)
{
    Log(2,"EdbShowAlg_N3::Create_NN_ALG_MLP","Create_NN_ALG_MLP().");

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_N3::Create_NN_ALG_MLP()   inputneurons= " << inputneurons << endl;

    if ( NULL == simu ) {
        cout << "EdbShowAlg_N3::Create_NN_ALG_MLP() WARNING simu tree is NULL pointer. Return NULL."<< endl;
        return NULL;
    }

    // DEBUG START
    // THIS IS TO BE WRITTEN BETTER, CAUSE THE HANDOVER OF THE PARAMETERS SHOULD
    // BE BETTER .....
    eANN_NHIDDENLAYER=eParaValue[2];
    eANN_INPUTNEURONS=eParaValue[5];

    // TO DO HERE.... TAKE OVER THE CORRECT LAYOUT.
    // only knowlegde about number of input neurons  and hidden layers is needed.
    cout << "EdbShowAlg_N3::Create_NN_ALG_MLP()   eANN_NHIDDENLAYER =  " <<  eANN_NHIDDENLAYER  << endl;
    cout << "EdbShowAlg_N3::Create_NN_ALG_MLP()   eANN_INPUTNEURONS =  " <<  eANN_INPUTNEURONS  << endl;

    // Create the layout here:
    TString layout="";
    TString newstring="";
    // ANN Input Layer
    for (Int_t loop=0; loop<eANN_INPUTNEURONS-1; ++loop) {
        newstring=Form("eANN_Inputvar[%d],",loop);
        layout += newstring; // "+" works only with TStrings!
    }
    newstring=Form("eANN_Inputvar[%d]:",eANN_INPUTNEURONS-1);
    ///
    layout += newstring;
    // Hidden Layers
    for (Int_t loop=0; loop<eANN_NHIDDENLAYER; ++loop) {
        newstring=Form("%d:",eANN_INPUTNEURONS);
        ////
        layout += newstring;
    }
    // Output Layer, one output neuron
    newstring="eANN_Inputtype";
    layout += newstring;

    // Set Layout String as internal variable now:
    eLayout = layout;

    // DEBUG MESSAGES:
    cout << "simu = "  << simu << endl;
    cout << "simu->Show(0): "  << endl;
    simu->Show(0);

    // Create the network:
    TMultiLayerPerceptron* TMlpANN = new TMultiLayerPerceptron(layout,simu);

    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbShowAlg_N3::Create_NN_ALG_MLP()   GetStructure:    " << endl;
        cout << TMlpANN->GetStructure() << endl;
    }
    Log(2,"EdbShowAlg_N3::Create_NN_ALG_MLP","Create_NN_ALG_MLP()...done.");
    return TMlpANN;
}

//______________________________________________________________________________


void EdbShowAlg_N3::SetANNWeightString()
{
    Log(2,"EdbShowAlg_N3::SetANNWeightString","SetANNWeightString()");
    int inputneurons=eParaValue[5];
    if (inputneurons==5)  eWeightFileString="WEIGHTFILESTRING_N3.txt";
    // TO DO HERE.... TAKE OVER THE CORRECT WEIGHTFILE STRING.
    cout << "EdbShowAlg_N3::SetANNWeightString()   TO DO HERE.... TAKE OVER THE CORRECT WEIGHTFILE STRING. " << endl;

//     if (gEDBDEBUGLEVEL>2)
    cout <<  "EdbShowAlg_N3::SetANNWeightString   "  << eWeightFileString << endl;
    Log(2,"EdbShowAlg_N3::SetANNWeightString","SetANNWeightString()...done.");
    return;
}


//______________________________________________________________________________


void EdbShowAlg_N3::LoadANNWeights()
{
    if (eWeightFileString=="") {
        cout <<  "EdbShowAlg_N3::SetANNWeightString  IS EMPTY. Reset to default string!"  << endl;
        eWeightFileString="WEIGHTFILESTRING_N3.txt";
    }
    eTMlpANN->LoadWeights(eWeightFileString);
    return;
}


//______________________________________________________________________________


void EdbShowAlg_N3::LoadANNWeights(TMultiLayerPerceptron* TMlpANN, TString WeightFileString)
{
    TMlpANN->LoadWeights(WeightFileString);
    if (gEDBDEBUGLEVEL>2) cout <<  "EdbShowAlg_N3::LoadANNWeights   "  << WeightFileString << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlg_N3::Print()
{
    Log(2,"EdbShowAlg_N3::Print","Print()");

    cout << "Number of Inputvariables: " << eParaValue[5] << endl;
    cout << "Algorithm method related inputs:" << endl;
    cout << "eANN_PLATE_DELTANMAX " << eParaValue[0] << endl;
    cout << "eANN_NTRAINEPOCHS " << eParaValue[1] << endl;
    cout << "eANN_NHIDDENLAYER " << eParaValue[2] << endl;
    cout << "eANN_OUTPUTTHRESHOLD " << eParaValue[3] << endl;
    cout << "eANN_EQUALIZESGBG " << eParaValue[4] << endl;

    cout << "Structure of the Net:" << endl;
    cout << eLayout.Data() << endl;

    Log(2,"EdbShowAlg_N3::Print","Print()...done.");
    return;
}



//______________________________________________________________________________

void EdbShowAlg_N3::Execute()
{
    Log(2,"EdbShowAlg_N3::Execute","DOING MAIN SHOWER RECONSTRUCTION HERE");

    if (eInBTArrayN==0) {
        Log(2,"EdbShowAlg_N3::Execute","Warning: No InitiatorBTs in the array. Return now");
        return;
    }

    // TO BE DONE HERE:
    // FILL THE ROUTINE WITH THE CODE FROM ShowReco PROGRAM
    cout << "EdbShowAlg_N3::Execute()...FILL THE ROUTINE WITH THE CODE FROM ShowReco PROGRAM." << endl;


    // Create the root file that contains the trainingsfile tree data first,
    // otherwise the trees are not connected with the specified file.
    if (eANN_DoTrain==kTRUE) {
        eANNTrainingsTreeFile = new TFile(Form("N3_ANN_TrainingsTreeFile.root",0),"RECREATE");
    }

    // Variables and things important for neural Network:
    TTree *eANNTrainingsTree = new TTree("TreeSignalBackgroundBT", "TreeSignalBackgroundBT");
    eANNTrainingsTree->Branch("N3_Type",   &eANN_Inputtype,   "N3_Type/I");
    eANNTrainingsTree->Branch("N3_Inputvar", eANN_Inputvar, "N3_Inputvar[24]/D");



    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbSegP* seg;
    EdbShowerP* RecoShower;

    Bool_t    StillToLoop=kTRUE;
    Int_t     ActualPID;
    Int_t     newActualPID;
    Int_t     STEP=-1;
    Int_t     NLoopedPattern=0;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_N3::Execute--- STEP for patternloop direction =  " << STEP << endl;


    Double_t params[30]; // Used for ANN Evaluation  // TO BE ADAPTED!!!

    //--- Loop over InBTs:
    cout << "Loop over InBTs N=" << eInBTArrayN << endl;

    // Since eInBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

        //-----------------------------------
        // 0a) Reset characteristic variables:
        //-----------------------------------

        //-----------------------------------
        // 1) Make eAli with cut parameters:
        //-----------------------------------

        // Create new EdbShowerP Object for storage;
        // See EdbShowerP why I have to call the Constructor as "unique" ideficable value
        //RecoShower = new EdbShowerP(i,eAlgValue);
        RecoShower = new EdbShowerP(i,eAlgValue,-1);

        // Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();

        // Clone InBT, because it is modified a lot of times,
        // avoid rounding errors by propagating back and forth
        EdbSegP* InBTClone = (EdbSegP*)InBT->Clone();

        // Add InBT to RecoShower:
        // This has to be done, since by definition the first BT in the RecoShower is the InBT.
        // Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
        cout << "Segment  (InBT) " << InBT << " was added to RecoShower." <<  endl;

        // Transform (make size smaller, extract only events having same MC) the  eAli  object:
        // See in Execute_CA for description.
        // Transform_eAli(InBT,1400);
        Transform_eAli(InBT,2400);

        //-----------------------------------
        // 2) Loop over (whole) eAli, check BT for Cuts
        //-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_N3::Execute--- --- Doing patterloop " << ActualPID << endl;

            // just to adapt to this nomenclature of ShowRec program:
            int patterloop_cnt=ActualPID;

            for (Int_t btloop_cnt=0; btloop_cnt<eAli->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                Segment = (EdbSegP*)eAli->GetPattern(ActualPID)->GetSegment(btloop_cnt);

                // just to adapt to this nomenclature of ShowRec program
                seg=Segment;

                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();

                // Reset characteristic variables:
                for (Int_t loop=0; loop<24; ++loop) eANN_Inputvar[loop]=0;

                // Now    calculate NN Inputvariables:  --------------------
                // Calculate the first four inputvariables, which depend on InitiatorBT only:
                // Important: dT, dMindist is symmetric, dR is NOT!
                // Do propagation from InBT to seg! (i.e. to downstream segments)
                // Loose PreCuts, in order not to get too much BG BT into trainings sample
                eANN_Inputvar[0] = seg->Z()-InBT->Z();
                // Update: It is better to use DistToAxis instead of dR, since dR measures
                // only distance to InBT without taking care of the direction.
                // (does not matter for relatively straight tracks, but for tracks in direction)
                /// eANN_Inputvar[1] = GetdR(InBT, seg);
                //eANN_Inputvar[1] = GetDistToAxis(InBTClone, seg);
                cout << "// TO BE CHECKED WHERE THE FUNCTION GetDistToAxis(InBTClone, seg);  IS!!" << endl;
                // TO BE CHECKED WHERE THE FUNCTION GetDistToAxis(InBTClone, seg);  IS!!
                if (eANN_DoTrain==kTRUE && eANN_Inputvar[1] > 1200) continue;

                //eANN_Inputvar[2] = GetdeltaThetaSingleAngles(InBT, seg);
                // TO BE CHECKED WHERE THE FUNCTION GetdeltaThetaSingleAngles(InBT, seg);  IS!!
                cout << "// TO BE CHECKED WHERE THE FUNCTION GetdeltaThetaSingleAngles(InBT, seg);  IS!!" << endl;
                if (eANN_DoTrain==kTRUE && eANN_Inputvar[2] >  0.4) continue;

                //eANN_Inputvar[3] = GetdMinDist(InBT, seg);
                // TO BE CHECKED WHERE THE FUNCTION GetdMinDist(InBT, seg);  IS!!
                cout << "// TO BE CHECKED WHERE THE FUNCTION GetdMinDist(InBT, seg);  IS!!" << endl;
                if (eANN_DoTrain==kTRUE && eANN_Inputvar[3] >  800) continue;
                // 1)
                // 2) .... // 24)
                // TO BE FILLED WITH THE CODE FROM    SHOWREC   PROGRAMM
                // end of calculate NN Inputvariables:  --------------------

                // ---------------------------------------------------------
                // Calculate eANN Output now:
                eANN_OutputValue=0;
                // Adapt: array params should have as many entries as there are inputvariables.
                // This array is larger than possible used array for evaluation.
                // The array with the right size is created, when the eANN_INPUTNEURONS
                // variable is fixed (TMLP class demands #arraysize = #inputneurons)
                // This is a kind of dump workaround, but for now it should work.
                Double_t EvalValue=0;
                Double_t    N3_Evalvar4[4];
                Double_t    N3_Evalvar8[8];
                Double_t    N3_Evalvar12[12];
                Double_t    N3_Evalvar16[16];
                Double_t    N3_Evalvar20[20];
                Double_t    N3_Evalvar24[24];

                if (eANN_INPUTNEURONS==4) {
                    for (int k=0; k<eANN_INPUTNEURONS; ++k) N3_Evalvar4[k]= eANN_Inputvar[k];
                    eANN_OutputValue=eTMlpANN->Evaluate(0,N3_Evalvar4);
                }
                else if (eANN_INPUTNEURONS==8) {
                    for (int k=0; k<eANN_INPUTNEURONS; ++k) N3_Evalvar8[k]= eANN_Inputvar[k];
                    eANN_OutputValue=eTMlpANN->Evaluate(0,N3_Evalvar8);
                }
                else if (eANN_INPUTNEURONS==12) {
                    for (int k=0; k<eANN_INPUTNEURONS; ++k) N3_Evalvar12[k]= eANN_Inputvar[k];
                    eANN_OutputValue=eTMlpANN->Evaluate(0,N3_Evalvar12);
                }
                else if (eANN_INPUTNEURONS== 16) {
                    for (int k=0; k<eANN_INPUTNEURONS; ++k) N3_Evalvar16[k]= eANN_Inputvar[k];
                    eANN_OutputValue=eTMlpANN->Evaluate(0,N3_Evalvar16);
                }
                else if (eANN_INPUTNEURONS==20) {
                    for (int k=0; k<eANN_INPUTNEURONS; ++k) N3_Evalvar20[k]= eANN_Inputvar[k];
                    eANN_OutputValue=eTMlpANN->Evaluate(0,N3_Evalvar20);
                }
                else {
                    for (int k=0; k<eANN_INPUTNEURONS; ++k) N3_Evalvar24[k]= eANN_Inputvar[k];
                    eANN_OutputValue=eTMlpANN->Evaluate(0,N3_Evalvar24);
                }
                // ---------------------------------------------------------


                // Now apply cut conditions: NN Neural Network Alg --------------------
                Double_t value=0;

                // These conditions have to be calculated, still!!!
                cout << "TO BE DONE" << endl;

                value=eTMlpANN->Evaluate(0, params);
                if (gEDBDEBUGLEVEL>3) {
                    cout << "eANN_OutputValue: " << eANN_OutputValue << " Inputvalues: ";
                    for (int i=0; i<5; i++) cout << "  " << eANN_Inputvar[i];
                }
                if (eANN_OutputValue<eParaValue[1]) continue;
                // end of    cut conditions: NN Neural Network Alg --------------------



                // If we arrive here, Basetrack  Segment  has passed criteria
                // and is then added to the RecoShower:
                // Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
                }
                cout << "Segment  " << Segment << " was added to &RecoShower  : " << &RecoShower  <<  endl;
            } // of btloop_cnt

            //------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_N3::Execute--- --- newActualPID= " << newActualPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_N3::Execute--- --- NLoopedPattern= " << NLoopedPattern << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_N3::Execute--- --- eNumberPlate_eAliPID= " << eNumberPlate_eAliPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_N3::Execute--- --- StillToLoop= " << StillToLoop << endl;

            // This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID) cout << "EdbShowAlg_N3::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
            // This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID) cout << "EdbShowAlg_N3::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
            // This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID) cout << "EdbShowAlg_N3::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

        // Obligatory when Shower Reconstruction is finished!
        RecoShower ->Update();
        //RecoShower ->PrintBasics();


        // Add Shower Object to Shower Reco Array.
        // Not, if its empty:
        // Not, if its containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

        // Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


    // Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    Log(2,"EdbShowAlg_N3::Execute","eRecoShowerArray():Entries = %d",eRecoShowerArray->GetEntries());
    Log(2,"EdbShowAlg_N3::Execute","DOING MAIN SHOWER RECONSTRUCTION HERE...done.");
    return;
}

//______________________________________________________________________________






//______________________________________________________________________________


void EdbShowAlg_N3::Finalize()
{
    Log(2,"EdbShowAlg_N3::Finalize","Finalize()");
    cout << "TO BE DONE HERE:  DELETE THE UNNECESSARY OBJECTS CREATED ON THE HEAP..." << endl;
    return;
}
