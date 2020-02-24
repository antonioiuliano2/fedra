#include "EdbShowAlgID_E_G_Simple.h"
using namespace std;

ClassImp(EdbShowAlgID_E_G_Simple)

//______________________________________________________________________________

EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple()
{
    // Default Constructor
    cout << "EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple()   Default Constructor"<<endl;

    Set0();
}


//______________________________________________________________________________

EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple(EdbShowerP* shower)
{
    // Default Constructor
    cout << "EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple(EdbShowerP* shower)   Default Constructor"<<endl;
    Set0();
    eRecoShowerArray=new TObjArray();

    eRecoShowerArray->Add(shower);
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    //cout << "EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple(EdbShowerP* shower)   Default Constructor    Doing Execute right now:"<<endl;
    //Execute();
}

//______________________________________________________________________________

EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple(TObjArray* RecoShowerArray)
{
    // Default Constructor
    cout << "EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple(TObjArray* RecoShowerArray)   Default Constructor"<<endl;

    Set0();
    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();
    cout << "EdbShowAlgID_E_G_Simple::EdbShowAlgID_E_G_Simple(TObjArray* RecoShowerArray)   Default Constructor...done."<<endl;
}

//______________________________________________________________________________

EdbShowAlgID_E_G_Simple::~EdbShowAlgID_E_G_Simple()
{
    // Default Destructor
    cout << "EdbShowAlgID_E_G_Simple::~EdbShowAlgID_E_G_Simple()"<<endl;
}


//______________________________________________________________________________

void EdbShowAlgID_E_G_Simple::Set0()
{
    eCalibrationOffset=0;
    eCalibrationSlope=1;
    eRecoShowerArray=NULL;
    eRecoShowerArrayN=0;
    eWeightFileString="NULL";
    return;
}

//______________________________________________________________________________

void EdbShowAlgID_E_G_Simple::Execute()
{
    cout << "EdbShowAlgID_E_G_Simple::Execute()"<<endl;
    if (eRecoShowerArrayN==0) {
        cout << "EdbShowAlgID_E_G_Simple::Execute()     eRecoShowerArrayN==0"<<endl;
        return;
    }

    /// Create ANN Tree and MLP:
    eParaName=2;
// 	TFile* f= new TFile("eee.root","RECREATE");
    TTree *ANNTree = new TTree("ANNTree", "ANNTree");
    ANNTree->SetDirectory(0);

    Float_t   inANN[70];
    ANNTree->Branch("inANN", inANN, "inANN[70]/F");

    TMultiLayerPerceptron *ANN_MLP;
    TMultiLayerPerceptron *ANN_MLP_ARRAY[15];

    TString layout="";
    int NrOfANNInputNeurons=12;

    int n_ANN_Neurons_FJ[15];
    n_ANN_Neurons_FJ[0]=6+10;
    n_ANN_Neurons_FJ[1]=6+12;
    n_ANN_Neurons_FJ[2]=6+14;
    n_ANN_Neurons_FJ[3]=6+16;
    n_ANN_Neurons_FJ[4]=6+18;
    n_ANN_Neurons_FJ[5]=6+20;
    n_ANN_Neurons_FJ[6]=6+23;
    n_ANN_Neurons_FJ[7]=6+26;
    n_ANN_Neurons_FJ[8]=6+29;
    n_ANN_Neurons_FJ[9]=6+32;
    n_ANN_Neurons_FJ[10]=6+35;
    n_ANN_Neurons_FJ[11]=6+40;
    n_ANN_Neurons_FJ[12]=6+45;
    n_ANN_Neurons_FJ[13]=6+50;
    n_ANN_Neurons_FJ[14]=6+57;
// 	10,12,14,16,18,20,23,26,29,32,35,40,45,50

    for (int k=0; k<15; k++) {
        layout="";
        NrOfANNInputNeurons=n_ANN_Neurons_FJ[k];
        for (Int_t i=1; i<NrOfANNInputNeurons; ++i) layout += "@inANN["+TString(Form("%d",i))+"],";
        layout += "@inANN["+TString(Form("%d",NrOfANNInputNeurons))+"]:"+TString(Form("%d",NrOfANNInputNeurons+1))+":"+TString(Form("%d",NrOfANNInputNeurons));
        layout+=":inANN[0]"; // ID CASE
        //==C== =======================//==C== =======================
        //cout << "===   DEBUG   ===  NOT ALL ENTRIES FOR ANN TRAINING USED, ONLY A QUICK SOLUTION" << endl;
        ANN_MLP_ARRAY[k]	= 	new TMultiLayerPerceptron(layout,ANNTree,"","");
        ANN_MLP=ANN_MLP_ARRAY[k];
        // 	if (!quick_run) ANN_MLP	= 	new TMultiLayerPerceptron(layout,ANNInterimTreeInputvariables,"Entry$>5000","Entry$<5000");
        cout << "===   DEBUG   ===  ANN_MLP->GetStructure() : " << ANN_MLP->GetStructure() << endl;
        TString ANN_LAYOUT=ANN_MLP_ARRAY[k]->GetStructure();
    }




    for (int ll=0; ll<15; ll++) ANN_MLP_ARRAY[ll]->LoadWeights("/home/meisel/Software/ANN_MEGA_ID/weigths_ID_E_GAMMA.txt");
    cout << "/home/meisel/Software/ANN_MEGA_ID/weigths_ID_E_GAMMA.txt" << endl;

    // 	Depending on the Number of Plates for the reconstructed shower,
    // 	we choose the right ANN (+layout) and weightfile to be loaded:
    // 	Npl() element of   [0..10]	=>	layout[0]
    // 	Npl() element of   [11..12]	=>	layout[1]
    // 	Npl() element of   [13..14]	=>	layout[2]
    // 	Npl() element of   [15..16]	=>	layout[3]
    // 	Npl() element of   [17..18]	=>	layout[4]
    // 	Npl() element of   [19..20]	=>	layout[5]
    // 	Npl() element of   [21..23]	=>	layout[6]
    // 	Npl() element of   [24..26]	=>	layout[7]
    // 	Npl() element of   [27..29]	=>	layout[8]
    // 	Npl() element of   [30..32]	=>	layout[9]
    // 	Npl() element of   [33..35]	=>	layout[10]
    // 	Npl() element of   [36..40]	=>	layout[11]
    // 	Npl() element of   [41..45]	=>	layout[12]
    // 	Npl() element of   [46..50]	=>	layout[13]

    TH1F* hist_E = new TH1F("hist_E","hist_E",100,0,10);

    for (int i=0; i<eRecoShowerArrayN; i++) {
// for (int i=0; i<4; i++) {

        EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        shower->Print();

        shower->BuildParametrisation_FJ();
        EdbShowerP::Para_FJ eShowAlgE_Para_FJ=shower->GetPara_FJ();
// 	Para_FJ 			eShowAlgE_Para_FJ=shower->GetPara_FJ();
        shower->PrintParametrisation_FJ();

        cout << "shower->Npl()  " << shower->Npl()  << endl;
        int check_Npl= shower->Npl();
        int check_Npl_index=0;
        if (check_Npl<=10) check_Npl_index=0;
        if (check_Npl>=11 && check_Npl<=12) check_Npl_index=1;
        if (check_Npl>=13 && check_Npl<=14) check_Npl_index=2;
        if (check_Npl>=15 && check_Npl<=16) check_Npl_index=3;
        if (check_Npl>=17 && check_Npl<=18) check_Npl_index=4;
        if (check_Npl>=19 && check_Npl<=20) check_Npl_index=5;
        if (check_Npl>=21 && check_Npl<=23) check_Npl_index=6;
        if (check_Npl>=24 && check_Npl<=26) check_Npl_index=7;
        if (check_Npl>=27 && check_Npl<=29) check_Npl_index=8;
        if (check_Npl>=30 && check_Npl<=32) check_Npl_index=9;
        if (check_Npl>=33 && check_Npl<=35) check_Npl_index=10;
        cout << "check_Npl_index " << check_Npl_index << endl;


// 		cout << "  ===========================    FORCE check_Npl_index  TO BE 20 PLATES !!!   ======================="<<endl;
// 		check_Npl_index=5;
// 		cout << "  ===========================    check_Npl_index=5; !!!   ======================="<<endl;

// 		cout << "  ===========================    FORCE check_Npl_index  TO BE 32 PLATES !!!   ======================="<<endl;
// 		check_Npl_index=9;
// 		cout << "  ===========================    check_Npl_index=9; !!!   ======================="<<endl;



        if (check_Npl>20) check_Npl_index=9;
        if (check_Npl<=20) check_Npl_index=5;
        if (check_Npl<=10) check_Npl_index=0;


        check_Npl_index=5;

        cout << "  ===========================    check_Npl_index= " << check_Npl_index << " ; !!!   ======================="<<endl;

        NrOfANNInputNeurons=n_ANN_Neurons_FJ[check_Npl_index];
        ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];
        cout << "Using the following layout: " << ANN_MLP->GetStructure() << endl;
        ANN_MLP->Print();

        // inANN[0] is ALWAYS reserved for the quantity value to be estimated
        // (E,Id,...)

        inANN[1]=eShowAlgE_Para_FJ.ShowerAxisAngle;
        inANN[2]=eShowAlgE_Para_FJ.nseg;
        inANN[3]=eShowAlgE_Para_FJ.BT_deltaR_mean;
        inANN[4]=eShowAlgE_Para_FJ.BT_deltaR_rms;
        inANN[5]=eShowAlgE_Para_FJ.BT_deltaT_mean;
        inANN[6]=eShowAlgE_Para_FJ.BT_deltaT_rms;
        for (int ii=0; ii<57; ii++) {
            cout << setw(3) << eShowAlgE_Para_FJ.longprofile[ii];
            inANN[7+ii]= eShowAlgE_Para_FJ.longprofile[ii];
        }
        cout << endl;
        // Fill Tree:
        ANNTree->Fill();

        Double_t params[70];
        Double_t val;
        Double_t EnergyCalibration_Offset=0.8;
        Double_t EnergyCalibration_Slope=0.94;


        EnergyCalibration_Offset=0.0;
        EnergyCalibration_Slope=1.0;


        for (Int_t j=1; j<=NrOfANNInputNeurons; ++j) {
            params[j-1]=inANN[j];
            cout << "=== DEBUG ===  :  j  params[j-1]=inANN[j]   " << j<< "  " << params[j-1] << endl;
        }
        val=(ANN_MLP->Evaluate(0,params));
        cout << "DEBUG: i,   val  (=mlp1->Evaluate(0,params))  ======================= real energy    " << i << "   " << val << "    ==  " << inANN[0]  << endl;

        if ( ::isnan(val) ) cout << "   val is NAN " << endl;


        val=(val-EnergyCalibration_Offset)/EnergyCalibration_Slope;
        cout << "----   With Correctiono factor  val=(val-EnergyCalibration_Offset)/EnergyCalibration_Slope:  " << EnergyCalibration_Offset << " " << EnergyCalibration_Slope << "    " << val<< endl;



        // Quick Estimation of the sigma (from ANN_MEGA_ENERGY)
        Double_t sigma=0.83*TMath::Sqrt(val)-0.45;

        cout << "Estimated Energy = " <<  val << " +- " << sigma  << "..." << endl;


// 		FindClosestEfficiencyParametrization(inANN[1],0.85);

// 		hist_E->Fill(val);
    }

// 	hist_E->Draw();

    return;
}




void EdbShowAlgID_E_G_Simple::FindClosestEfficiencyParametrization(Double_t TestAngle=0.0,Double_t ReferenceEff=1.0)
{

    cout << "TestAngle " << TestAngle << endl;
    cout << "ReferenceEff " << ReferenceEff << endl;

    TF1 *all = new TF1("all","1.0-0.00000001*x",0,0.95);
    TF1 *edefault = new TF1("default","0.94-0.216*x-0.767*x*x+1.865*x*x*x",0,0.95);
    TF1 *elletroni = new TF1("elletroni","0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x",0,0.95);
    TF1 *neuchmicro = new TF1("neuchmicro","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95);
    TF1 *MiddleFix = new TF1("MiddleFix","0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    TF1 *LowEff = new TF1("LowEff","0.85*0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);

// Double_t TestAngle=0.05;
// Double_t ReferenceEff=0.776;

    cout << "all->Eval(TestAngle) "<< all->Eval(TestAngle)<< endl;
    cout << "LowEff->Eval(TestAngle) "<< LowEff->Eval(TestAngle)<< endl;
    cout << "MiddleFix->Eval(TestAngle) "<< MiddleFix->Eval(TestAngle)<< endl;
    cout << "elletroni->Eval(TestAngle) "<< elletroni->Eval(TestAngle)<< endl;
    cout << "neuchmicro->Eval(TestAngle) "<< neuchmicro->Eval(TestAngle)<< endl;
    cout << "default->Eval(TestAngle) "<< edefault->Eval(TestAngle)<< endl;

//Measure distance of angle to estimated angle:

    Double_t dist[9];

    dist[0]=TMath::Abs(ReferenceEff-edefault->Eval(TestAngle));
    dist[1]=TMath::Abs(ReferenceEff-neuchmicro->Eval(TestAngle));
    dist[2]=TMath::Abs(ReferenceEff-elletroni->Eval(TestAngle));
    dist[3]=TMath::Abs(ReferenceEff-MiddleFix->Eval(TestAngle));
    dist[4]=TMath::Abs(ReferenceEff-LowEff->Eval(TestAngle));
    dist[5]=TMath::Abs(ReferenceEff-all->Eval(TestAngle));

    Double_t dist_min=1;
    Int_t best_i=-1;
    for (int i=0; i<6; i++) if (abs(dist[i])<dist_min) {
            dist_min=dist[i];
            best_i=i;
        }

    cout << "Miminum distance = " << dist_min << endl;
    cout << "Best numerb efficiency = ";
    if (best_i==0) cout << " default " <<endl;
    if (best_i==1) cout << " neuchmicro " <<endl;
    if (best_i==2) cout << " elletroni " <<endl;
    if (best_i==3) cout << " MiddleFix " <<endl;
    if (best_i==4) cout << " LowEff " <<endl;
    if (best_i==5) cout << " all " <<endl;

    return;

}



void EdbShowAlgID_E_G_Simple::SetWeightFileString(TString weightstring)
{
    eWeightFileString=weightstring;
    cout << "EdbShowAlgID_E_G_Simple::SetWeightFileString()     eWeightFileString = " << eWeightFileString << endl;
    return;
}
