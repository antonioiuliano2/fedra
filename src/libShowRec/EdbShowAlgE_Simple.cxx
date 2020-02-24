#include "EdbShowAlgE_Simple.h"
using namespace std;

ClassImp(EdbShowAlgE_Simple)




//______________________________________________________________________________

EdbShowAlgE_Simple::EdbShowAlgE_Simple()
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple()   Default Constructor"<<endl;
    Set0();
    Init();
}


//______________________________________________________________________________

/// CONSTRUCTOR USED FOR libShowRec with EdbShowerP class available.
// EdbShowAlgE_Simple::EdbShowAlgE_Simple(EdbShowerP* shower)
// {
//     // Default Constructor...
//     if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple(EdbShowerP* shower)   Default Constructor"<<endl;
//     Set0();
//     Init();
//     // ... with Shower:
//     eRecoShowerArray=new TObjArray();
//     eRecoShowerArray->Add(shower);
//     SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
//     if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple(EdbShowerP* shower)   Default Constructor...done."<<endl;
// }

//______________________________________________________________________________

EdbShowAlgE_Simple::EdbShowAlgE_Simple(EdbTrackP* track)
{
    // Default Constructor...
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple(EdbTrackP* track)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with Shower:
    eRecoShowerArray=new TObjArray();
    eRecoShowerArray->Add(track);
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple(EdbTrackP* track)   Default Constructor...done."<<endl;
}


//______________________________________________________________________________

EdbShowAlgE_Simple::EdbShowAlgE_Simple(TObjArray* RecoShowerArray)
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple(TObjArray* RecoShowerArray)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with ShowerArray:
    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::EdbShowAlgE_Simple(TObjArray* RecoShowerArray)   Default Constructor...done."<<endl;
}

//______________________________________________________________________________

EdbShowAlgE_Simple::~EdbShowAlgE_Simple()
{
    // Default Destructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::~EdbShowAlgE_Simple()"<<endl;
    // Delete Histograms (on heap):
    delete eHisto_nbtk_av;
    delete eHisto_longprofile_av;
    delete eHisto_transprofile_av;
    delete eHisto_deltaR_mean;
    delete eHisto_deltaT_mean;
    delete eHisto_deltaR_rms;
    delete eHisto_deltaT_rms;
    delete eHisto_nbtk;
    delete eHisto_longprofile;
    delete eHisto_transprofile;
    delete eHisto_deltaR;
    delete eHisto_deltaT;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::~EdbShowAlgE_Simple()...done."<<endl;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::Set0()
{
    // Reset Values
    eCalibrationOffset=0;
    eCalibrationSlope=1;
    eRecoShowerArray=NULL;
    eRecoShowerArrayN=0;
    ePlateNumberType=0;
    ePlateNumber=20;
    eSpecificationIsChanged=kFALSE;

    eWeightFileString="NULL";
    for (int i=0; i<6; i++) {
        eSpecificationType[i]=-1;
        eSpecificationTypeString[i]="";
    }
    for (int i=0; i<15; i++) {
        ANN_MLP_ARRAY[i]=NULL;
        eANN_MLP_CORR_0[i]=0.0;
        eANN_MLP_CORR_1[i]=1.0;
    }
    eEnergyArrayCount=0;

    // Reset "eEfficiencyParametrisation" to the "Neuch" eff.
    cout << "EdbShowAlgE_Simple::Set0()   Reset to the Neuch eff." << endl;
    Double_t xarr[6]= {0,0.1,0.2,0.3,0.4,0.5};
    Double_t yarr[6]= {0.95,0.9,0.80,0.75,0.6,0.5};
    delete eEfficiencyParametrisation;
    eEfficiencyParametrisation = new TSpline3("",xarr,yarr,5,0,0,0.6);
    cout << "EdbShowAlgE_Simple::Set0()   Reset to the Neuch eff. ...done." << endl;


    cout << "EdbShowAlgE_Simple::Set0()...done."<< endl;
    return;
}


//______________________________________________________________________________

void EdbShowAlgE_Simple::Init()
{
    // This Plate Binnning is the _final_binning and will not be refined furthermore!
    ePlateBinning[0]=10;
    ePlateBinning[1]=12;
    ePlateBinning[2]=14;
    ePlateBinning[3]=16;
    ePlateBinning[4]=18;
    ePlateBinning[5]=20;
    ePlateBinning[6]=23;
    ePlateBinning[7]=26;
    ePlateBinning[8]=29;
    ePlateBinning[9]=32;
    ePlateBinning[10]=35;
    ePlateBinning[11]=40;
    ePlateBinning[12]=45;
    ePlateBinning[13]=45;
    ePlateBinning[14]=45; // 12,13,14 are the same, cause I have seen that it makes no sense to produce these,
    // since E,and sigmaE for 45 plates doesnt change anymore at all...

    ePlateNumber=20; // 20 plates by default...
    eSpecificationType[0] = 1;  // cp files by default...
    eSpecificationType[1] = 1;  // ele by default...
    eSpecificationType[2] = 0;  // neuch eff by default...
    eSpecificationType[3] = 0;  // 0..20 GeV by default...
    eSpecificationType[4] = 0;  // next before weight by default...
    eSpecificationType[5] = 5;  // 20 plates by default...

    eSpecificationTypeString[0]="CP";
    eSpecificationTypeString[1]="GAMMA";
    eSpecificationTypeString[2]="Neuch";
    eSpecificationTypeString[3]="TypeA";
    eSpecificationTypeString[4]="Next";
    eSpecificationTypeString[5]="20";

    for (int i=0; i<15; i++) {
        ANN_nPlates_ARRAY[i]=ePlateBinning[i];
    }

    EffFunc_all = new TF1("all","1.0-0.00000001*x",0,0.95);
    EffFunc_edefault = new TF1("default","0.94-0.216*x-0.767*x*x+1.865*x*x*x",0,0.95);
    EffFunc_elletroni = new TF1("elletroni","0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x",0,0.95);
    EffFunc_neuchmicro = new TF1("neuchmicro","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95);
    EffFunc_MiddleFix = new TF1("MiddleFix","0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    EffFunc_LowEff = new TF1("LowEff","0.85*0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    EffFunc_UserEfficiency = new TF1("EffFunc_UserEfficiency","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95); //new TSpline3();


    // Standard supposed efficiency of the showers/tracks that are to be evaluated:
    // The correct EffFunc_* is then choosen by taking the closes eff func to that one.
    // Initial parameters are the efficiency parameters for the "Neuch" parametrisation
    Double_t xarr[7]= {0,0.1,0.2,0.3,0.4,0.5,0.6};
    Double_t yarr[7]= {0.95,0.9,0.80,0.75,0.6,0.5,0.4};
    delete eEfficiencyParametrisation;
    eEfficiencyParametrisation = new TSpline3("",xarr,yarr,5,0,0,0.65);

    eHisto_nbtk_av          = new TH1D("eHisto_nbtk_av","Average basetrack numbers",21,0.0,10.0);
    eHisto_longprofile_av   = new TH1D("eHisto_longprofile_av","Basetracks per emulsion number",57,0.0,57.0);
    eHisto_transprofile_av  = new TH1D("eHisto_transprofile_av","Basetracks in trans distance",8,0.0,800.0);
    eHisto_deltaR_mean      = new TH1D("eHisto_deltaR_mean","Mean #deltar of all BTs in one shower",100,0.0,100.0);
    eHisto_deltaT_mean      = new TH1D("eHisto_deltaT_mean","Mean #delta#theta of all BTs in one shower",100,0.0,0.1);
    eHisto_deltaR_rms       = new TH1D("eHisto_deltaR_rms","RMS #deltar of all BTs in one shower",100,0.0,100.0);
    eHisto_deltaT_rms       = new TH1D("eHisto_deltaT_rms","RMS #delta#theta of all BTs in one shower",100,0.0,0.1);
    eHisto_nbtk             = new TH1D("eHisto_nbtk","Basetracks in the shower",50,0.0,100.0);
    eHisto_longprofile      = new TH1D("eHisto_longprofile","Basetracks per emulsion number",57,0.0,57.0);
    eHisto_deltaR           = new TH1D("eHisto_deltaR","Single #deltar of all BTs in Shower",100,0.0,150.0);
    eHisto_deltaT           = new TH1D("eHisto_deltaT","Single #delta#theta of all BTs in Shower",150,0.0,0.15);
    eHisto_transprofile     = new TH1D("eHisto_transprofile","Basetracks in trans distance",8,0.0,800.0);

    // CreateANN
    CreateANN();

    //First Time, we also Update()
    Update();

    // Set Strings:
    InitStrings();

    // Create EnergyArray
    eEnergyArray=new TArrayF(99999); // no way to adapt tarrayF on the fly...
    eEnergyArrayUnCorrected=new TArrayF(99999);
    eEnergyArraySigmaCorrected=new TArrayF(99999);

    // Read Energy Resolution Lookup tables:
    ReadTables();

    cout << "EdbShowAlgE_Simple::Init()...done."<< endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlgE_Simple::InitStrings()
{
    cout << "EdbShowAlgE_Simple::InitStrings()"<< endl;
    eSpecificationTypeStringArray[0][0]="LT";
    eSpecificationTypeStringArray[0][1]="CP";
    eSpecificationTypeStringArray[1][0]="GAMMA";
    eSpecificationTypeStringArray[1][1]="ELECTRON";
    eSpecificationTypeStringArray[2][0]="Neuch";
    eSpecificationTypeStringArray[2][1]="All";
    eSpecificationTypeStringArray[2][2]="MiddleFix";
    eSpecificationTypeStringArray[2][3]="Low";
    eSpecificationTypeStringArray[3][0]="TypeA";
    eSpecificationTypeStringArray[3][1]="TypeABCD";
    eSpecificationTypeStringArray[4][0]="Next";
    eSpecificationTypeStringArray[4][1]="Before";
    eSpecificationTypeStringArray[5][0]="10";
    cout << "EdbShowAlgE_Simple::InitStrings()...done."<< endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::CreateANN()
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::CreateANN()"<<endl;

    // Create ANN Tree and MLP:
    eParaName=2; // Standard Labeling taken over from EdbShowerP
    ANNTree = new TTree("ANNTree", "ANNTree");
    ANNTree->SetDirectory(0);
    ANNTree->Branch("inANN", inANN, "inANN[70]/D");

    // 	Plate Binning: 10,12,14,16,18,20,23,26,29,32,35,40,45,45
    // see ePlateBinning[]

    for (int k=0; k<15; k++) {
        //cout << "creating ANN " << k << endl;
        ANN_Layout="";
        ANN_n_InputNeurons_ARRAY[k]=6+ePlateBinning[k];
        ANN_n_InputNeurons=ANN_n_InputNeurons_ARRAY[k];
        for (Int_t i=1; i<ANN_n_InputNeurons; ++i) ANN_Layout += "@inANN["+TString(Form("%d",i))+"],";
        ANN_Layout += "@inANN["+TString(Form("%d",ANN_n_InputNeurons))+"]:"+TString(Form("%d",ANN_n_InputNeurons+1))+":"+TString(Form("%d",ANN_n_InputNeurons));
        ANN_Layout+=":inANN[0]/1000";
        //---------------------------
        ANN_MLP_ARRAY[k]	= 	new TMultiLayerPerceptron(ANN_Layout,ANNTree,"","");
        ANN_MLP=ANN_MLP_ARRAY[k];
        ANN_Layout=ANN_MLP_ARRAY[k]->GetStructure();
        //---------------------------
        if (gEDBDEBUGLEVEL>2) {
            ANN_MLP->Print();
            cout << ANN_Layout << endl;
        }
    }
    //---------------------------

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::CreateANN()...done."<<endl;
    return;
}

//____________________________________________________________________________________________________

void EdbShowAlgE_Simple::DoRun()
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun()" << endl;

    for (int i=0; i<eRecoShowerArrayN; i++) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Doing i= " << i << endl;
        continue;
        eEnergyArrayCount=i;

        //EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        EdbTrackP* shower=(EdbTrackP*) eRecoShowerArray->At(i);

        DoRun(shower);

    } // (int i=0; i<eRecoShowerArrayN; i++)

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun()...done." << endl;
    return;
}


//____________________________________________________________________________________________________

void EdbShowAlgE_Simple::DoRun(TObjArray* trackarray)
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun(TObjArray* trackarray)" << endl;

    eRecoShowerArrayN=trackarray->GetEntries();
    eRecoShowerArray=trackarray;

    for (int i=0; i<eRecoShowerArrayN; i++) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Doing i= " << i << endl;

        eEnergyArrayCount=i;

        //EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        EdbTrackP* shower=(EdbTrackP*) eRecoShowerArray->At(i);

        DoRun(shower);

    } // (int i=0; i<eRecoShowerArrayN; i++)

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun(TObjArray* trackarray)...done." << endl;
    return;
}

//____________________________________________________________________________________________________

void EdbShowAlgE_Simple::DoRun(EdbTrackP* shower)
{

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgE_Simple::DoRun(EdbTrackP* shower)" << endl;

    // Get internal variables from the parametrisation filled:
    GetPara(shower);


    int EffNr=FindClosestEfficiencyParametrization(eParaShowerAxisAngle,eEfficiencyParametrisation->Eval(eParaShowerAxisAngle));

    // Check if efficiency is the one we have or
    // if we have to change/reload the ANN weightfiles:
    if (GetSpecType(2)!=EffNr) {
        cout << "EdbShowAlgE_Simple::DoRun()   Hello! Calulated Efficiency is more compatible with another one: Change Specifiaction! Call SetSpecificationType(2,"<< EffNr <<")." << endl;
        SetSpecificationType(2,EffNr);
    }



    // This is to select the suited ANN to the shower, i.e.
    // the one that matches closest the number of plates:
    int check_Npl_index	=0;
    GetNplIndexNr(shower->Npl(),check_Npl_index,ePlateNumberType);


    ANN_n_InputNeurons=ANN_n_InputNeurons_ARRAY[check_Npl_index];
    ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];
    eWeightFileString=ANN_WeightFile_ARRAY[check_Npl_index];
    eCalibrationOffset = eANN_MLP_CORR_0[check_Npl_index];
    eCalibrationSlope = eANN_MLP_CORR_1[check_Npl_index];

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   ANN_n_InputNeurons_ARRAY[check_Npl_index]="<< ANN_n_InputNeurons_ARRAY[check_Npl_index] << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   ANN_MLP_ARRAY[check_Npl_index]="<< ANN_MLP_ARRAY[check_Npl_index] << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Using the following layout: " << endl;
    if (gEDBDEBUGLEVEL >2) cout << ANN_MLP->GetStructure() << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   And the following weightfile: " << endl;
    if (gEDBDEBUGLEVEL >2) cout << eWeightFileString << endl;

    //  Reset InputVariables:
    for (int k=0; k<70; k++) {
        inANN[k]=0;
    }


    // inANN[0] is ALWAYS reserved for the quantity value to be estimated
    // (E,Id,...)
    // Test with private variables:
    inANN[1]=eParaShowerAxisAngle;
    inANN[2]=eParanseg;
    inANN[3]=eParaBT_deltaR_mean;
    inANN[4]=eParaBT_deltaR_rms;
    inANN[5]=eParaBT_deltaT_mean;
    inANN[6]=eParaBT_deltaT_rms;
    for (int ii=0; ii<57; ii++) {
        inANN[7+ii]= eParalongprofile[ii];
    }


    // Fill Tree:
    ANNTree->Fill();

    Double_t params[70];
    Double_t val;

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Print Inputvariables: " << endl;
    for (Int_t j=1; j<=ANN_n_InputNeurons; ++j) {
        params[j-1]=inANN[j];
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   :  j  params[j-1]=inANN[j]   " << j<< "  " << params[j-1] << endl;
    }
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Print Inputvariables...done." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Evaluate Neural Network Output:" << endl;

    // ---------------------------------
    // Evaluation of the ANN:
    val=(ANN_MLP->Evaluate(0,params));
    // ---------------------------------

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   Before Correction: ANN_MLP->Evaluate(0,params)= " <<  ANN_MLP->Evaluate(0,params) << " (Inputvar=" << inANN[0]  << ")." << endl;

    if ( ::isnan(val) ) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   ERROR! ANN_MLP->Evaluate(0,params) is NAN! Setting value to -1. " << endl;
        val=-1;
    }
    eEnergyUnCorr=val;

    // Linear Correction According to the Parameters matching the right weightfile.
    val=(val-eCalibrationOffset)/eCalibrationSlope;

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   After  Correction: ANN_MLP->Evaluate(0,params)= " <<  val << " (Inputvar=" << inANN[0]  << ")." << endl;



    // ---------------------------------
    // Evaluation of the statistical and systematical errors by lookup tables and splines:
    // For further info and more details, I will report the methology in the thesis, and at
    // some next collaboration meetings.. (As of october 2010,fwm.)
    TSpline3* spline;
    Float_t val_sigma_stat, val_sigma_sys, val_sigma_tot;
    cout << "TSpline3* spline=eSplineArray_Energy_Stat_Gamma[check_Npl_index];" << endl;
    spline=(TSpline3*)eSplineArray_Energy_Stat_Gamma->At(check_Npl_index);
    cout << "Float_t val_sigma_stat=spline->Eval(val);  "  << spline->Eval(val) << endl;
    val_sigma_stat=spline->Eval(val);

    cout << "TSpline3* spline=eSplineArray_Energy_Sys_Gamma[check_Npl_index];" << endl;
    spline=(TSpline3*)eSplineArray_Energy_Sys_Gamma->At(check_Npl_index);
    cout << "Float_t val_sigma_sys=spline->Eval(val);  "  << spline->Eval(val) << endl;
    val_sigma_sys=spline->Eval(val);

    // Addition of errors: stat(+)sys.  (+)=quadratic addition.
    val_sigma_tot=TMath::Sqrt(val_sigma_stat*val_sigma_stat+val_sigma_sys*val_sigma_sys);
    cout << "Float_t val_sigma_tot=...  "  << val_sigma_tot << endl;


    cout << "Doing only statistics and one source of systematics at the moment! " << endl;
    cout << "Notice also that we dont have storage variable in EdbTrackP for the error of P() ... " << endl;
    // ---------------------------------

    // Quick Estimation of the sigma (from ANN_MEGA_ENERGY)
    // This is later to be read from a lookup table....
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun()   ERROR! Sigma ONLY FROM STATISTICAL UNCERTATINTY NOWNOT correctly set up to now."<<endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun()   ERROR! Sigma will be fully implemented when lookup tables for all"<<endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun()   ERROR! systematic uncertainties are availible!"<<endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::DoRun()   Estimated Energy = " << val << " +- " << val_sigma_tot  << "..." << endl;



    // Finally, set values...
    shower->SetP(val);
    eEnergy=val;
    eEnergyCorr=val;
    eEnergySigmaCorr=val_sigma_stat;

    eEnergyArray->AddAt(eEnergy,eEnergyArrayCount);
    eEnergyArrayUnCorrected->AddAt(eEnergyUnCorr,eEnergyArrayCount);
    eEnergyArraySigmaCorrected->AddAt(eEnergySigmaCorr,eEnergyArrayCount);


    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::DoRun()   ...Done." << endl;
    return;
}

//______________________________________________________________________________________________________________


Int_t EdbShowAlgE_Simple::FindClosestEfficiencyParametrization(Double_t TestAngle=0.0,Double_t ReferenceEff=1.0)
{
    // Returns the number of the closest EfficiencyParametrization to use. Supported are:
    // ((in this order numbering, different from the ordering, fwm used for his calculations!:))
    // 0:Neuch
    // 1:All
    // 2:MiddleFix
    // 3:Low

    // 4: UserEfficiency (can take input from eda, for example; or user measured input)

    if (gEDBDEBUGLEVEL >2) cout << "TestAngle " << TestAngle << endl;
    if (gEDBDEBUGLEVEL >2) cout << "ReferenceEff " << ReferenceEff << endl;
    if (gEDBDEBUGLEVEL >2) cout << "neuchmicro->Eval(TestAngle) "<< EffFunc_neuchmicro->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "all->Eval(TestAngle) "<< EffFunc_all->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "MiddleFix->Eval(TestAngle) "<< EffFunc_MiddleFix->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "LowEff->Eval(TestAngle) "<< EffFunc_LowEff->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << " eEfficiencyParametrisation->Eval(TestAngle) "<< eEfficiencyParametrisation->Eval(TestAngle)<< endl;

//Measure distance of angle to estimated angle:
    Double_t dist[9];
    dist[0]=TMath::Abs(ReferenceEff-EffFunc_neuchmicro->Eval(TestAngle));
    dist[1]=TMath::Abs(ReferenceEff-EffFunc_all->Eval(TestAngle));
    dist[2]=TMath::Abs(ReferenceEff-EffFunc_MiddleFix->Eval(TestAngle));
    dist[3]=TMath::Abs(ReferenceEff-EffFunc_LowEff->Eval(TestAngle));
    dist[4]=TMath::Abs(ReferenceEff-eEfficiencyParametrisation->Eval(TestAngle));

    Double_t dist_min=1;
    Int_t best_i=-1;
    for (int i=0; i<5; i++) if (abs(dist[i])<dist_min) {
            dist_min=dist[i];
            best_i=i;
        }

    if (gEDBDEBUGLEVEL >2) {
        cout << "Miminum distance = " << dist_min << ", best matching efficiency is =  ";
        if (best_i==0) cout << " neuchmicro " <<endl;
        if (best_i==2) cout << " MiddleFix " <<endl;
        if (best_i==3) cout << " LowEff " <<endl;
        if (best_i==1) cout << " All " <<endl;
        if (best_i==4) cout << " User Set eEfficiencyParametrisation " <<endl;
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::FindClosestEfficiencyParametrization()  ...Done." <<  endl;
    return best_i;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::SetWeightFileString(TString weightstring)
{
    eWeightFileString=weightstring;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::SetWeightFileString()     eWeightFileString = " << eWeightFileString << endl;
    return;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::GetNplIndexNr(Int_t check_Npl,Int_t &check_Npl_index, Int_t ePlateNumberType)
{
    // ---	What is the sense of this?
    // ---  According to the shower it can have any different length between [1..57] plates.
    // ---  We cannot produce weights for each plate....
    // ---  So we have only some weights for different plates: acoording to the array set in plateBinning:
    // ---  10,12,14,16,18,20,23,26,29,32,35,40,45,50
    // ---  So what if a shower has npl() of 19? take the weight for 18 or for 20 plates?
    // ---  therefore, we introduced ePlateNumberType. This will take the weight as like:
    // ---  In case ePlateNumberType > 0 the it will take he trained ANN file with Npl >= shower->Npl();
    // ---  in such a way that |ePlateNumberType| is the difference to the ePlateBinning[].
    // ---  Example: ePlateNumberType=1 -> 1st next availible weight will be taken: (npl==19->nplANN==20);
    // ---  Example: ePlateNumberType=2 -> 2nd next availible weight will be taken: (npl==19->nplANN==23);
    // ---  Example: ePlateNumberType=3 -> 3rd next availible weight will be taken: (npl==19->nplANN==26);
    // ---  Example: ePlateNumberType=-1-> 1st befo availible weight will be taken: (npl==19->nplANN==18);
    // ---  Example: ePlateNumberType=-2-> 2nd befo availible weight will be taken: (npl==19->nplANN==16);

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::GetNplIndexNr("<<check_Npl<<","<<check_Npl_index<<","<<ePlateNumberType<<")"<<endl;

    // The Neural Network Training was done in a way that we had always Npl(Train)>=shower->Npl()

    // First we find closest (bigger) ePlateBinning index:
    int diff=9999;
    int closestIndex=-1;
    for (int i=0; i<15; i++) {
        if (ePlateBinning[i]-check_Npl>0) {
            closestIndex=i;
            break;
        }
    }
    cout << "closestIndex= " << closestIndex << " ePlateBinning[closestIndex]= " << ePlateBinning[closestIndex] << endl;
    //-----------------------------------
    // for ePlateNumberType we have it already exact:
    ePlateNumber = closestIndex + ePlateNumberType;
    if (ePlateNumberType==1) ePlateNumber = closestIndex;
    cout << "ePlateNumber = closestIndex + ePlateNumberType = " <<  ePlateNumber << endl;

    //  check for over/underbound
    if (ePlateNumber<0) ePlateNumber=0;
    if (ePlateNumber>14) ePlateNumber=14;
    check_Npl_index=ePlateNumber;
    //-----------------------------------
//     // This Plate Binnning is the _final_binning and will not be refined furthermore!
//     ePlateBinning[0]=10;//     ePlateBinning[1]=12;
//     ePlateBinning[2]=14;//     ePlateBinning[3]=16;
//     ePlateBinning[4]=18;//     ePlateBinning[5]=20;
//     ePlateBinning[6]=23;//     ePlateBinning[7]=26;
//     ePlateBinning[8]=29;//     ePlateBinning[9]=32;
//     ePlateBinning[10]=35;//     ePlateBinning[11]=40;
//     ePlateBinning[12]=45;//     ePlateBinning[13]=45;
//     ePlateBinning[14]=45;
    //-----------------------------------
    cout << "All plates:"<<endl;
    cout << "Shower Npl:"<<endl;
    cout << "Available weights:"<<endl;
    cout << "Taken weight:"<<endl;
    cout << "[";
    for (int i=1; i<=57; i++) {
        cout << ".";
    }
    cout << "]" << endl;
    //-----------------------------------
    cout << "[";
    for (int i=1; i<=check_Npl; i++) {
        cout << ".";
    }
    cout << "]" << endl;
    //-----------------------------------
    cout << "[";
    for (int i=1; i<=57; i++) {
        Bool_t isExact=kFALSE;
        for (int j=0; j<15; j++) {
            if (i==ePlateBinning[j]) isExact=kTRUE;
        }
        if (isExact) {
            cout << "x";
        }
        else {
            cout << ".";
        }
    }
    cout << "]" << endl;
    //-----------------------------------
    cout << "[";
    for (int i=1; i<=57; i++) {
        if (i==ePlateBinning[ePlateNumber]) {
            cout << "X";
        }
        else {
            cout << ".";
        }
    }
    cout << "]" << endl;
    //-----------------------------------

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::check_Npl_index Shower:Npl()=" << check_Npl << "."<< endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::check_Npl_index ePlateNumber=" << ePlateNumber << "."<< endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple::check_Npl_index ePlateBinning[=" << ePlateBinning[ePlateNumber] << "."<< endl;


    check_Npl=ePlateBinning[ePlateNumber];
    return;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::GetSpecifications() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::GetSpecifications" << endl;
    cout << "EdbShowAlgE_Simple::GetSpecifications ... nothing done here yet ... " << endl;
    return;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::PrintSpecifications() {
    cout << "EdbShowAlgE_Simple::PrintSpecifications" << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationType[0]  (Dataset:    (linked tracks/full cp) LT/CP)   = " << GetSpecType(0) << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationType[1]  (ShowerID:   gamma/electron/pion weight) = " << GetSpecType(1) << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationType[2]  (ScanEff:    Neuch/All/MiddleFix/LowEff) = " << GetSpecType(2) << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationType[3]  (E range:    0..20/0..40) = " << GetSpecType(3) << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationType[4]  (Npl weight: next before/after) = " <<GetSpecType(4) << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationType[5]  (Npl weight: 10,12,...,45) = " << GetSpecType(5) << endl;
    cout << "EdbShowAlgE_Simple::   " << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationTypeString[0]  (take CP or linked_tracks)   = " << eSpecificationTypeString[0] << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationTypeString[1]  (gamma/electron/pion weight) = " << eSpecificationTypeString[1] << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationTypeString[2]  (ScanEff: Neuch/All/MiddleFix/LowEff) = " << eSpecificationTypeString[2] << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationTypeString[3]  (E range: 0..20/0..40) = " << eSpecificationTypeString[3] << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationTypeString[4]  (Npl weight: next before/after) = " << eSpecificationTypeString[4] << endl;
    cout << "EdbShowAlgE_Simple::   eSpecificationTypeString[5]  (Npl weight: 10,12,...,45) = " << eSpecificationTypeString[5] << endl;
    cout << "EdbShowAlgE_Simple::   " << endl;
    cout << "EdbShowAlgE_Simple::   In case you want to change a specification then do for example:" << endl;
    cout << "EdbShowAlgE_Simple::   EdbShowAlgE_Simple->SetSpecification(2,3) " << endl;
    cout << "EdbShowAlgE_Simple::   It will change the ScanEff from the actual value to _Low_." << endl;

    return;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::LoadSpecificationWeightFile() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::LoadSpecificationWeightFile  from the following specifications:" << endl;
    PrintSpecifications();
    TString weigth="NULL";
    return;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::SetSpecifications(Int_t sp0, Int_t sp1, Int_t sp2, Int_t sp3, Int_t sp4, Int_t sp5) {
    eSpecificationType[0]=sp0;
    eSpecificationType[1]=sp1;
    eSpecificationType[2]=sp2;
    eSpecificationType[3]=sp3;
    eSpecificationType[4]=sp4;
    eSpecificationType[5]=sp5;
    PrintSpecifications();
    eSpecificationIsChanged=kTRUE;
    Update();
    return;
}


//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::SetSpecificationType(Int_t SpecificationType, Int_t SpecificationTypeVal)
{
    if (GetSpecType(SpecificationType)==SpecificationTypeVal) return;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowAlgE_Simple:: Change Specification (" << SpecificationType << ") from " << GetSpecType(SpecificationType) << " -> " << SpecificationTypeVal << " . Reprint the changed Specification String: " << endl;

    eSpecificationType[SpecificationType]=SpecificationTypeVal;

    eSpecificationTypeString[SpecificationType] = eSpecificationTypeStringArray[SpecificationType][SpecificationTypeVal];
    if (gEDBDEBUGLEVEL >1) cout << eSpecificationTypeString[SpecificationType]  << endl;
    eSpecificationIsChanged=kTRUE;

    cout << "Reprint Specifications: " << endl;
    if (gEDBDEBUGLEVEL >1) PrintSpecifications();

    Update();
    return;
}

//__________________________________________________________________________________________________

void EdbShowAlgE_Simple::Update() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  Does the following things in the order:" << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  * According to the switch: set the right ANN of the Array as generic one." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  * According to the switch: load the right weightfile as generic one." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  * According to the switch: set the right correction parameters...." << endl;

    TString basicstring = TString(gSystem->ExpandPathName("$FEDRA_ROOT"));
    TString addstring = ("/src/libShower/weights/Energy/");

    // (*) SpecType 0 :
    if (eSpecificationType[0]==1) {
        addstring+="volumeSpec_CP/";
        eSpecificationTypeString[0]="CP";
    }
    else {
        addstring+="volumeSpec_LT/";
        eSpecificationTypeString[0]="LT";
        cout << "EdbShowAlgE_Simple::Update  WARNING::eSpecificationTypeString[0]=LT NOT YET SUPPORTED!" << endl;
    }

    // (*) SpecType 1 :
    if (eSpecificationType[1]==2) {
        addstring+="particleSpec_pion/";
        eSpecificationTypeString[1]="pion";
        cout << "EdbShowAlgE_Simple::Update  WARNING::eSpecificationTypeString[1]=pion NOT YET SUPPORTED!" << endl;
    }
    else if (eSpecificationType[1]==1)  {
        addstring+="particleSpec_electron/";
        eSpecificationTypeString[1]="electron";
    }
    else {
        addstring+="particleSpec_gamma/";
        eSpecificationTypeString[1]="gamma";
    }

    // (*) SpecType 2 :
    if (eSpecificationType[2]==3) {
        addstring+="efficiencySpec_LowEff/";
        eSpecificationTypeString[2]="LowEff";
    }
    else if (eSpecificationType[2]==2)  {
        addstring+="efficiencySpec_MiddleFix/";
        eSpecificationTypeString[2]="MiddleFix";
    }
    else if (eSpecificationType[2]==1)  {
        addstring+="efficiencySpec_All/";
        eSpecificationTypeString[2]="All";
    }
    else {
        addstring+="efficiencySpec_Neuch/";
        eSpecificationTypeString[2]="Neuch";
    }

    // (*) SpecType 3 :
    if (eSpecificationType[3]==1) {
        addstring+="trainrangeSpec_extended/";
        eSpecificationTypeString[3]="extended";
        cout << "EdbShowAlgE_Simple::Update  WARNING::eSpecificationTypeString[3]=extended NOT YET SUPPORTED!" << endl;
    }
    else {
        addstring+="trainrangeSpec_normal/";
        eSpecificationTypeString[3]="normal";
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  " << addstring << endl;

    if (NULL==ANN_MLP_ARRAY[0]) {
        CreateANN();
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  Now load the different ANN weightstrings:" << endl;
    // This was the loading part for the ANN and
    // this is the loading part for the Correction Factors, which we take from the
    // generic file EnergyCorrections_Npl_%d.txt
    for (int ll=0; ll<15; ll++) {
        TString weigthstring=basicstring+addstring+TString(Form("weights_Npl_%d.txt",ANN_nPlates_ARRAY[ll]));
        if (gEDBDEBUGLEVEL >2) cout << "weigthstring = " << weigthstring << endl;
        ANN_MLP_ARRAY[ll]->LoadWeights(weigthstring);
        ANN_WeightFile_ARRAY[ll]=weigthstring;

        TString correctionsfactorstring=basicstring+addstring+TString(Form("EnergyCorrections_Npl_%d.txt",ANN_nPlates_ARRAY[ll]));
        if (gEDBDEBUGLEVEL >2) cout << "correctionsfactorstring = " << correctionsfactorstring << endl;
        Float_t p0,p1;
        p0=0.0;
        p1=1.0;
        if (ll<10) ReadCorrectionFactors(correctionsfactorstring,p0,p1);
// Correction files for ll>11 dont exist yet..
        eANN_MLP_CORR_0[ll]=p0;
        eANN_MLP_CORR_1[ll]=p1;

    }
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  * According to the switch: set the right ANN of the Array as generic one." << endl;

    // (*) SpecType 4:
    // Get NplIndexNr:
    int check_Npl_index=0;
    GetNplIndexNr(ePlateNumber,check_Npl_index, eSpecificationType[4]);

    if (check_Npl_index==0) eSpecificationTypeString[4]="Next";
    if (check_Npl_index==1) eSpecificationTypeString[4]="Before";
    eSpecificationTypeString[5]=TString(Form("%d",ePlateNumber));

    // Set Generic ANN_Layout
    ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  WARNING:: Weightfiles for _LT_  not produced yet!...." << endl;
    return;
}

//__________________________________________________________________________________________________


void EdbShowAlgE_Simple::GetPara(EdbTrackP* track)
{
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::GetPara    Fill the Para structure with values from a track." << endl;

    // This parametrisation consists of these variables:
    //  0)  Axis TanTheta
    //  1)  NBT
    //  2)  BT_deltaR_mean
    //  3)  BT_deltaR_rms
    //  4)  BT_deltaT_mean
    //  5)  BT_deltaT_rms
    //  6)  longprofile[0]=eHisto_longprofile->GetBinContent(1) // number of basetracks in the SAME plate as the Initiator basetrack.
    //  7)  longprofile[1]=eHisto_longprofile->GetBinContent(2)
    //  ...
    //  5+Npl())  longprofile[Npl()-1]=eHisto_longprofile->GetBinContent(Npl()) // number of basetracks in the LAST plate of the reconstructed shower.
    //
    //==C==  DUMMY routine to get the values   deltarb  and   deltathetab  filled:
    //==C==  necessary since its an old relict from old times where this was saved only
    //==C==  as root TTree.

    EdbSegP* seg;
    Float_t shower_xb[5000];
    Float_t shower_yb[5000];
    Float_t shower_zb[5000];
    Float_t shower_txb[5000];
    Float_t shower_tyb[5000];
    Float_t shower_deltathetab[5000];
    Float_t shower_deltarb[5000];
    Float_t min_shower_deltathetab=99999;   // Reset
    Float_t min_shower_deltar=99999;        // Reset
    Float_t extrapo_diffz=0;
    Float_t extrapol_x=0;
    Float_t extrapol_y=0;

    Float_t test_shower_deltathetab;
    Float_t test_shower_deltar;
    Float_t test_shower_deltax;
    Float_t test_shower_deltay;

    Int_t TrackNSeg=track->N();

    for (int ii=0; ii<TrackNSeg; ii++)  {
        // the ii<N() => ii<TrackNSeg replacement is just an adaption from the
        // code snipplet taken from EdbShowRec
        seg=(EdbSegP*)track->GetSegment(ii);
        shower_xb[ii]=seg->X();
        shower_yb[ii]=seg->Y();
        shower_txb[ii]=seg->TX();
        shower_tyb[ii]=seg->TY();
        shower_zb[ii]=seg->Z();
        shower_deltathetab[ii]=0.5;
        shower_deltarb[ii]=200;
    }

    //-------------------------------------
    for (int ii=0; ii<TrackNSeg; ii++)  {
        seg=(EdbSegP*)track->GetSegment(ii);
        if (gEDBDEBUGLEVEL>2) {
            if (gEDBDEBUGLEVEL >2) cout << "====== --- DOING   "  << ii << endl;
            seg->PrintNice();
        }
        //-------------------------------------
        // InBT:
        if (ii==0) {
            shower_deltathetab[ii]=0.5;
            shower_deltarb[ii]=200;
        }
        // All other BTs:
        if (ii>0) {

            // PUT HERE:  calculation routine for shower_deltathetab, shower_deltarb
            // Exrapolate the BT [ii] to the position [jj] and then calc the
            // position and slope differences for the best matching next segment.
            // For the backward extrapolation of the   shower_deltathetab and shower_deltarb
            // calulation for BaseTrack(ii), Basetrack(jj)->Z() hast to be smaller.
            min_shower_deltathetab=99999;   // Reset
            min_shower_deltar=99999;        // Reset

            for (int jj=0; jj<track->N(); jj++)  {
                if (ii==jj) continue;

                // since we do not know if BTs are ordered by their Z positions:
                // and cannot cut directly on the number in the shower entry:
                if (shower_zb[ii]<shower_zb[jj]) continue;

                extrapo_diffz=shower_zb[ii]-shower_zb[jj];
                if (TMath::Abs(extrapo_diffz)>4*1300+1.0) continue;
                if (TMath::Abs(extrapo_diffz)<1.0) continue; // remove same positions.

                extrapol_x=shower_xb[ii]-shower_txb[ii]*extrapo_diffz; // minus, because its ii after jj.
                extrapol_y=shower_yb[ii]-shower_tyb[ii]*extrapo_diffz; // minus, because its ii after jj.

                // Delta radius we need to extrapolate.
                test_shower_deltax=extrapol_x;//shower_txb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_xb[ii];
                test_shower_deltay=extrapol_y;//shower_tyb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_yb[ii];
                test_shower_deltax=test_shower_deltax-shower_xb[jj];
                test_shower_deltay=test_shower_deltay-shower_yb[jj];
                test_shower_deltar=TMath::Sqrt(test_shower_deltax*test_shower_deltax+test_shower_deltay*test_shower_deltay);

                // Delta theta we do not need to extrapolate. (old version...)
                //test_shower_deltathetab=TMath::Sqrt(shower_txb[ii]*shower_txb[ii]+shower_tyb[ii]*shower_tyb[ii]);
                //test_shower_deltathetab=test_shower_deltathetab-TMath::Sqrt(shower_txb[jj]*shower_txb[jj]+shower_tyb[jj]*shower_tyb[jj]);
                //test_shower_deltathetab=TMath::Abs(test_shower_deltathetab);
                //----
                // As before in ShowRec this way of calculation is not equivalent as calculating
                // DeltaTheta domponentwise:
                // Code from libShower:
                // delta = sqrt((SX0-a->GetTXb(l2))*(SX0-a->GetTXb(l2))+((SY0-a->GetTYb(l2))*(SY0-a->GetTYb(l2))));
                test_shower_deltathetab=TMath::Sqrt(TMath::Power(shower_txb[ii]-shower_txb[jj],2)+TMath::Power(shower_tyb[ii]-shower_tyb[jj],2));

                // Check if both dr,dt match parameter criteria and then just take these values.....
                // Maybe a change is necessary because it is not exactly the same as in the off. algorithm:
                if (test_shower_deltar<1000 && test_shower_deltathetab<2.0 ) { // open cut values
                    // Make these values equal to the one in the "official algorithm"..... 150microns and 100mrad.
                    //if (test_shower_deltar<150 && test_shower_deltathetab<0.15 ) {   // frederics cut values
                    /// -----     IMPORTANT::  these areopen cut values for best combifinding of pair BT deltaR/Theta values
                    /// -----     IMPORTANT::  then you do NOT necessarily get back your values which you put in durign
                    /// -----     IMPORTANT::  your shower reconstruction cone ( deltaR/Theta cutvalues could be NO cutvalues
                    /// -----     IMPORTANT::  for some reconstruction algorithms for example, but we wanna have these values anyway.
                    ///  In Any Case:
                    ///  Frederics Cut looks only for best min_shower_deltar so we do also.
                    if (test_shower_deltar<min_shower_deltar) {
                        min_shower_deltathetab=test_shower_deltathetab;
                        min_shower_deltar=test_shower_deltar;
                        shower_deltathetab[ii]=min_shower_deltathetab;
                        shower_deltarb[ii]=min_shower_deltar;
                    }
                }
            }
        }
        //-------------------------------------

    } // for (int ii=0;ii<N();ii++)
    //-------------------------------------

    //-------------------------------------
    if (gEDBDEBUGLEVEL>2) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbTrackP:  N() nsegments= " << track->N() << endl;
        for (int ii=0; ii<track->N(); ii++)  {
            if (gEDBDEBUGLEVEL >2) cout << "Shower:  nentry= " << ii << "  shower_zb[ii] =  " << shower_zb[ii] << "  shower_deltathetab[ii] =  " << shower_deltathetab[ii] << "  shower_deltarb[ii] =  " << shower_deltarb[ii] <<endl;
        }
    }
    //-------------------------------------

    // Profile starting with (arrayindex ==0): number of basetracks in the SAME plate as the Initiator basetrack.
    // Profile ending with (arrayindex ==56): // number of basetracks in the LAST plate of the reconstructed shower.
    Int_t   longprofile[57];

    //Int_t numberofilms=Npl(); // Historical reasons why there are more names for the same variable...
    //Int_t nbfilm=Npl();       // Historical reasons why there are more names for the same variable...
    Int_t nbfilm=track->Npl();

    // Int_t   numberoffilsforlongprofile=numberofilms+1; // it is one more going from [1..nbfilm] !!!// Not used in this routine.

    Float_t Dr[57];
    Float_t X0[57];
    Float_t Y0[57];
    Float_t TX0,TY0;
    Float_t theta[57];
    Float_t dist;
    Float_t xb[5000];
    Float_t yb[5000];
    Float_t zb[5000];
    Float_t txb[5000];
    Float_t tyb[5000];
    Int_t   nfilmb[5000];
    //Float_t deltathetab[5000]; // Not used in this routine.
    //Float_t deltarb[5000];    // Not used in this routine.
//     Int_t sizeb=N();
    Int_t sizeb=track->N();
    //Int_t nentries_withisizeb=0;

    //=C= =====================================================================
    eHisto_nbtk_av          ->Reset();
    eHisto_nbtk             ->Reset();
    eHisto_longprofile_av   ->Reset();
    eHisto_deltaR_mean      ->Reset();
    eHisto_deltaT_mean      ->Reset();
    eHisto_deltaR_rms       ->Reset();
    eHisto_deltaT_rms       ->Reset();
    eHisto_longprofile      ->Reset();
    eHisto_deltaR           ->Reset();
    eHisto_deltaT           ->Reset();
    //=C= =====================================================================
    for (int id=0; id<57; id++) {
        theta[id]= 0;
        X0[id] = id*1300.*track->GetSegment(0)->TX() + track->GetSegment(0)->X();
        Y0[id] = id*1300.*track->GetSegment(0)->TY() + track->GetSegment(0)->Y();
        longprofile[id]=-1;
        // this is important, cause it means that where is -1 there is no BT reconstructed anymore
        // this is different for example to holese where N=0 , so one can distinguish!
    }

    if (gEDBDEBUGLEVEL>3) if (gEDBDEBUGLEVEL >2) cout << "eHisto_longprofile->GetBinWidth() "  << eHisto_longprofile->GetBinWidth(1)  << endl;
    if (gEDBDEBUGLEVEL>3) if (gEDBDEBUGLEVEL >2) cout << "eHisto_longprofile->GetBinCenter() "  << eHisto_longprofile->GetBinCenter(1)  << endl;
    if (gEDBDEBUGLEVEL>3) if (gEDBDEBUGLEVEL >2) cout << "eHisto_longprofile->GetNbinsX() "  << eHisto_longprofile->GetNbinsX()  << endl;

    TX0 = track->GetSegment(0)->TX();
    TY0 = track->GetSegment(0)->TY();
    for (Int_t j = 0; j<57; ++j) {
        Dr[j] = 0.03*j*1300. +20.0;
        //if (gEDBDEBUGLEVEL >2) cout << " DEBUG   j= " <<  j << "  Dr[j]= " <<  Dr[j] << endl;
    } // old relict from FJ. Do not remove. //
    // it represents somehow conesize......(??)

    //=C= =====================================================================
    for (Int_t ibtke = 0; ibtke < track->N(); ibtke++) {
        xb[ibtke]=track->GetSegment(ibtke)->X();
        yb[ibtke]=track->GetSegment(ibtke)->Y();
        zb[ibtke]=track->GetSegment(ibtke)->Z();
        txb[ibtke]=track->GetSegment(ibtke)->TX();
        tyb[ibtke]=track->GetSegment(ibtke)->TY();
        // abs() of filmPID with respect to filmPID of first BT, plus 1: (nfilmb(0):=1 per definition):
        // Of course PID() have to be read correctly (by ReadEdbPVrec) correctly.
        // Fedra should do it.
        nfilmb[ibtke]=TMath::Abs(track->GetSegment(ibtke)->PID()-track->GetSegment(0)->PID())+1;

        if (gEDBDEBUGLEVEL>2) {
            if (gEDBDEBUGLEVEL >2) cout << "ibtke= " <<ibtke << " xb[ibtke]= " << xb[ibtke] << " nfilmb[ibtke]= " << nfilmb[ibtke] << endl;
        }
    }
    //=C= =====================================================================
    //=C= loop over the basetracks in the shower (boucle sur les btk)
    for (Int_t ibtke = 0; ibtke < track->N(); ibtke++) {
        dist = sqrt((xb[ibtke]- X0[nfilmb[ibtke]-1])*(xb[ibtke]- X0[nfilmb[ibtke]-1])+(yb[ibtke]- Y0[nfilmb[ibtke]-1])*(yb[ibtke]- Y0[nfilmb[ibtke]-1]));

        // inside the cone
        //if (gEDBDEBUGLEVEL >2) cout << "ibtke= " <<ibtke << "   dist =  "  <<  dist << "  Dr[nfilmb[ibtke]-1] = " <<  Dr[nfilmb[ibtke]-1] << endl;
        //if (gEDBDEBUGLEVEL >2) cout << "    nfilmb[ibtke]  =  " << nfilmb[ibtke] <<   "   nbfilm =  " << nbfilm << endl;

        // In old times there was here an additional condition which checked the BTs for the ConeDistance.
        // Since at this point in buildparametrisation_FJ the shower is -already fully reconstructed -
        // (by either EdbShoAlgo, or ShowRec program or manual list) this intruduces an additional cut
        // which is not correct because conetube size is algorithm specifiv (see Version.log.txt #20052010)
        // Thats wy we drop it here....
        // 	    if (dist<Dr[nfilmb[ibtke]-1]&&nfilmb[ibtke]<=nbfilm) {  // original if line comdition
        if (dist>Dr[nfilmb[ibtke]-1]) {
            if (gEDBDEBUGLEVEL>2) {
                if (gEDBDEBUGLEVEL >2) cout << " WARNING , In old times this cut (dist>Dr[nfilmb[ibtke]-1]) (had also to be fulfilled!"<<endl;
                if (gEDBDEBUGLEVEL >2) cout << "            For this specific shower it seems not the case....." << endl;
                if (gEDBDEBUGLEVEL >2) cout << "           You might want to check this shower again manualy to make sure everything is correct....." << endl;
            }
        }
        if (nfilmb[ibtke]<=nbfilm) {
            //if (gEDBDEBUGLEVEL >2) cout << "DEBUG CUTCONDITION WITHOUT THE (?_?_? WRONG ?_?_?) CONE DIST CONDITION....." << endl;
            // if (gEDBDEBUGLEVEL >2) cout << yes, this additional cut is not necessary anymore, see above....

            eHisto_longprofile        ->Fill(nfilmb[ibtke]);
            eHisto_longprofile_av     ->Fill(nfilmb[ibtke]);

            Double_t DR=0;  //Extrapolate the old stlye way:
            Double_t Dx=xb[ibtke]-(xb[0]+(zb[ibtke]-zb[0])*txb[0]);
            Double_t Dy=yb[ibtke]-(yb[0]+(zb[ibtke]-zb[0])*tyb[0]);
            DR=TMath::Sqrt(Dx*Dx+Dy*Dy);
            eHisto_transprofile_av->Fill(DR);
            eHisto_transprofile->Fill(DR);

            theta[nfilmb[ibtke]]+= (TX0-txb[ibtke])*(TX0-txb[ibtke])+(TY0-tyb[ibtke])*(TY0-tyb[ibtke]);
            if (ibtke>0&&nfilmb[ibtke]<=nbfilm) {
                // eHisto_deltaR           ->Fill(deltarb[ibtke]);
                // eHisto_deltaT           ->Fill(deltathetab[ibtke]);
                // uses shower_deltarb,shower_deltathetab just calculated in dummy routine above.
                // The first BT is NOT used for this filling since the extrapolated values of
                // shower_deltarb and shower_deltathetab are set manually to 0.5 and 200, due to
                // historical reasons (these variables com from the e/pi separation stuff).
                eHisto_deltaR           ->Fill(shower_deltarb[ibtke]);
                eHisto_deltaT           ->Fill(shower_deltathetab[ibtke]);
            }
        }
    }//==C== END OF loop over the basetracks in the shower
    //if (gEDBDEBUGLEVEL >2) cout <<"---------------------------------------"<<endl;
    //=======================================================================================
    //==C== Fill NumberBT Histogram for all showers:
    eHisto_nbtk                   ->Fill(sizeb);
    eHisto_nbtk_av                ->Fill(sizeb);
    //==C== Fill dR,dT Mean and RMS Histos for all showers:
    eHisto_deltaR_mean            ->Fill(eHisto_deltaR->GetMean());
    eHisto_deltaT_mean            ->Fill(eHisto_deltaT->GetMean());
    eHisto_deltaR_rms             ->Fill(eHisto_deltaR->GetRMS());
    eHisto_deltaT_rms             ->Fill(eHisto_deltaT->GetRMS());
    //=======================================================================================

    // Fill the longprofile array:  (NEW VERSION (arrayindex 0 is same plate as Initiator BT))
    for (Int_t i=1; i<=nbfilm; ++i) {
        // longprofile[i-1] = eHisto_longprofile->GetBinContent(i);    /// OLD VERSION for (Int_t i=1; i<=nbfilm+1; ++i)
        longprofile[i-1] = (Int_t)(eHisto_longprofile->GetBinContent(i+1));     //  NEW VERSION (arrayindex 0 is same plate as Initiator BT)
        //test+=longprofile[i-1] ;
        if (gEDBDEBUGLEVEL>1) {
            if (gEDBDEBUGLEVEL >2) cout << "i= " << i << " longprofile[i-1] "<< longprofile[i-1] << " eHisto_longprofile->GetBinContent(i) " << eHisto_longprofile->GetBinContent(i+1)<< endl;
        }
        if (i==nbfilm) {
            if (gEDBDEBUGLEVEL >2) cout << "i==nbfilm:" << endl;
            longprofile[i-1] = (Int_t)(eHisto_longprofile->GetBinContent(i+1));
        }
        //
    }
    // Rather strange but I have to put it explicetly here, otherwise last bin isnt filled...
    longprofile[nbfilm] = (Int_t)(eHisto_longprofile->GetBinContent(nbfilm+1));
    for (Int_t i=nbfilm+2; i<57; ++i) {
        longprofile[i-1] = -1;
    }
    //----------------------------------------------------------------------------------------

    // - Inclusion. only for libShower, EdbShowerRec class!
    Float_t eShowerAxisAngle=track->GetSegment(0)->Theta();
    // - Inclusion. only for libShower, EdbShowerRec class; END;


    // Now set parametrisation values:
    eParaShowerAxisAngle=eShowerAxisAngle;
    eParanseg=track->N();
    eParaBT_deltaR_mean = eHisto_deltaR->GetMean();
    eParaBT_deltaR_rms  = eHisto_deltaR->GetRMS();
    eParaBT_deltaT_mean = eHisto_deltaT->GetMean();
    eParaBT_deltaT_rms  = eHisto_deltaT->GetRMS();
    for (int ii=0; ii<57; ii++) eParalongprofile[ii]=longprofile[ii];

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::GetPara    Fill the Para structure with values from a track.... done." << endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::ReadCorrectionFactors(TString weigthstring, Float_t &p0, Float_t &p1) {
    // Read Linear Correction factors p0 p1 from this file.
    // Format: p0 p1
    float pp0,pp1;
    const char* name=weigthstring.Data();
    FILE * pFile=NULL;
    if (gEDBDEBUGLEVEL >2) cout << " open file " << endl;
    pFile = fopen (name,"r");
    if (NULL==pFile) return;
    int dummy = fscanf (pFile, "%f %f", &pp0, &pp1);
    if (dummy!=2) {
        cout << "EdbShowAlgE_Simple::ReadCorrectionFactors   ERROR! Wrong formatted file! Could not read the two parameters! Set them to default values! Please check if the weightfiles in...: "<< endl;
        cout << weigthstring.Data() << endl;
        cout << "EdbShowAlgE_Simple::ReadCorrectionFactors  ... exist!"<< endl;
        p0=0.0;
        p1=1.0;
    }
    p0=pp0;
    p1=pp1;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::ReadCorrectionFactors   p0: " << p0 << "  p1 " << p1 << endl;
    if (gEDBDEBUGLEVEL >2) printf ("I have read: %f and %f , in total %d arguments.\n",pp0,pp1,dummy);
    fclose (pFile);
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::Print() {
    cout << "-------------------------------------------------------------------------------------------"<< endl;
    cout << "EdbShowAlgE_Simple::Print " << endl;
    PrintSpecifications();

    cout << "EdbShowAlgE_Simple::Print    Now correction factors and weightfilestrings:" << endl;
    for (int i=0; i<15; i++) {
        cout << "EdbShowAlgE_Simple::Print       eANN_MLP_CORR_0[" << i << "]="<<eANN_MLP_CORR_0[i] << endl;
        cout << "EdbShowAlgE_Simple::Print       eANN_MLP_CORR_1[" << i << "]="<<eANN_MLP_CORR_1[i] << endl;
        cout << "EdbShowAlgE_Simple::Print       ANN_WeightFile_ARRAY[" << i << "]="<<ANN_WeightFile_ARRAY[i] << endl;
    }

    cout << "EdbShowAlgE_Simple::Print ...done." << endl;
    cout << "-------------------------------------------------------------------------------------------"<< endl << endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::WriteNewRootFile()
{
    cout << "-------------------------------------------------------------------------------------------"<< endl;
    cout << endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       -------------------------------------------"<< endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       This is NOT a good solution! But what shall" << endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       I do?" << endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       We rewrite completely the new tree and add " << endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       the new calculated energy values to it..." << endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       -------------------------------------------"<< endl;
    cout << endl;

    //-     VARIABLES: shower_  "treebranch"  reconstruction
    Int_t shower_number_eventb, shower_sizeb, shower_isizeb,shower_showerID;
    Int_t shower_sizeb15, shower_sizeb20, shower_sizeb30;
    Float_t shower_energy_shot_particle;
    Float_t shower_xb[5000];
    Float_t shower_yb[5000];
    Float_t shower_zb[5000];
    Float_t shower_txb[5000];
    Float_t shower_tyb[5000];
    Float_t shower_deltarb[5000];
    Float_t shower_deltathetab[5000];
    Float_t shower_deltaxb[5000];
    Float_t shower_deltayb[5000];
    Int_t   shower_nfilmb[5000];
    Float_t shower_chi2btkb[5000];
    Int_t shower_ntrace1simub[5000]; // MCEvt
    Int_t shower_ntrace2simub[5000]; // s->W()
    Float_t shower_ntrace3simub[5000]; // s->P()
    Int_t shower_ntrace4simub[5000]; // s->Flag()
    Float_t shower_tagprimary[5000];
    Int_t   shower_idb[5000];
    Int_t   shower_plateb[5000];
    Float_t shower_deltasigmathetab[58];
    Int_t   shower_numberofilms;
    Float_t shower_purb; // purity of shower
    Int_t shower_eProb90;
    Int_t shower_eProb1;
    Int_t shower_Size;            // number of BT in the shower
    Int_t shower_Size15;          // number of BT in the shower (for 15 films crossed)
    Int_t shower_Size20;          // number of BT in the shower (for 20 films crossed)
    Int_t shower_Size30;          // number of BT in the shower (for 30 films crossed)
    Float_t shower_output;        // Neural Network output for e/pi separation
    Float_t shower_output15;      // Neural Network output for e/pi separation (for 15 films crossed)
    Float_t shower_output20;      // Neural Network output for e/pi separation (for 20 films crossed)
    Float_t shower_output30;      // Neural Network output for e/pi separation (for 30 films crossed)
    Float_t shower_output50;      // Neural Network output for e/pi separation (for 50 films crossed)
    Float_t shower_purityb;
    Float_t shower_trackdensb;
    Float_t shower_E_MC;
    Float_t shower_EnergyCorrectedb;
    Float_t shower_EnergyUnCorrectedb;
    Float_t shower_EnergySigmaCorrectedb;
    Float_t shower_EnergySigmaUnCorrectedb;

    Float_t shower_OldEnergyCorrectedb;
    Float_t shower_OldEnergyUnCorrectedb;
    Float_t shower_OldEnergySigmaCorrectedb;
    Float_t shower_OldEnergySigmaUnCorrectedb;

    //- Old Shower File:
    TFile* fileOld = new TFile("Shower.root","READ");
    // Set Addresses of treebranch tree:
    TTree* eShowerTree = (TTree*)fileOld->Get("treebranch");
    cout << "eShowerTree = " << eShowerTree << endl;
    eShowerTree->SetBranchAddress("number_eventb",&shower_number_eventb);
    eShowerTree->SetBranchAddress("sizeb",&shower_sizeb);
    eShowerTree->SetBranchAddress("sizeb15",&shower_sizeb15);
    eShowerTree->SetBranchAddress("sizeb20",&shower_sizeb20);
    eShowerTree->SetBranchAddress("sizeb30",&shower_sizeb30);
/// 		eShowerTree->SetBranchAddress("output", &shower_output);
    eShowerTree->SetBranchAddress("output15", &shower_output15);
    eShowerTree->SetBranchAddress("output20", &shower_output20);
    eShowerTree->SetBranchAddress("output30", &shower_output30);
    eShowerTree->SetBranchAddress("output50", &shower_output50);
    eShowerTree->SetBranchAddress("eProb1", &shower_eProb1);
    eShowerTree->SetBranchAddress("eProb90", &shower_eProb90);
    eShowerTree->SetBranchAddress("isizeb",&shower_isizeb);
    eShowerTree->SetBranchAddress("xb",shower_xb);
    eShowerTree->SetBranchAddress("yb",shower_yb);
    eShowerTree->SetBranchAddress("zb",shower_zb);
    eShowerTree->SetBranchAddress("txb",shower_txb);
    eShowerTree->SetBranchAddress("tyb",shower_tyb);
    eShowerTree->SetBranchAddress("nfilmb",shower_nfilmb);
    eShowerTree->SetBranchAddress("ntrace1simub",shower_ntrace1simub);  // s.eMCEvt
    eShowerTree->SetBranchAddress("ntrace2simub",shower_ntrace2simub); // s.eW
    eShowerTree->SetBranchAddress("ntrace3simub",shower_ntrace3simub); // s.eP
    eShowerTree->SetBranchAddress("ntrace4simub",shower_ntrace4simub); // s.eFlag
    eShowerTree->SetBranchAddress("chi2btkb",shower_chi2btkb);
    eShowerTree->SetBranchAddress("deltarb",shower_deltarb);
    eShowerTree->SetBranchAddress("deltathetab",shower_deltathetab);
    eShowerTree->SetBranchAddress("deltaxb",shower_deltaxb);
    eShowerTree->SetBranchAddress("deltayb",shower_deltayb);
    eShowerTree->SetBranchAddress("tagprimary",shower_tagprimary);
//     eShowerTree->SetBranchAddress("energy_shot_particle",&shower_energy_shot_particle);
    eShowerTree->SetBranchAddress("E_MC",&shower_E_MC);
    eShowerTree->SetBranchAddress("showerID",&shower_showerID);
    eShowerTree->SetBranchAddress("idb",shower_idb);
    eShowerTree->SetBranchAddress("plateb",shower_plateb);
//     eShowerTree->SetBranchAddress("deltasigmathetab",shower_deltasigmathetab);
//     eShowerTree->SetBranchAddress("lenghtfilmb",&shower_numberofilms);
    eShowerTree->SetBranchAddress("purityb",&shower_purityb);
    eShowerTree->SetBranchAddress("Energy",&shower_OldEnergyCorrectedb);
    eShowerTree->SetBranchAddress("EnergyUnCorrected",&shower_OldEnergyUnCorrectedb);
    eShowerTree->SetBranchAddress("EnergySigma",&shower_OldEnergySigmaCorrectedb);
    eShowerTree->SetBranchAddress("EnergySigmaUnCorrected",&shower_OldEnergySigmaUnCorrectedb);


    Int_t nent=eShowerTree->GetEntries();
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       eShowerTree="<< eShowerTree <<"      ------"<< endl;
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       eShowerTree->GetEntries()="<< nent <<"      ------"<< endl;


    // Create the new File
    TFile* fileNew = new TFile("New.root","RECREATE");
    // Create the new Tree
    TTree* ShowerTreeNew=new TTree("treebranch","treebranch");
    cout << "EdbShowAlgE_Simple::WriteNewRootFile       ShowerTreeNew="<< ShowerTreeNew <<"      ------"<< endl;
    // Create the new Branches:
    ShowerTreeNew->Branch("number_eventb",&shower_number_eventb,"number_eventb/I");
    ShowerTreeNew->Branch("sizeb",&shower_sizeb,"sizeb/I");
    ShowerTreeNew->Branch("sizeb15",&shower_sizeb15,"sizeb15/I");
    ShowerTreeNew->Branch("sizeb20",&shower_sizeb20,"sizeb20/I");
    ShowerTreeNew->Branch("sizeb30",&shower_sizeb30,"sizeb30/I");
    ShowerTreeNew->Branch("output15",&shower_output15,"output15/F");
    ShowerTreeNew->Branch("output20",&shower_output20,"output20/F");
    ShowerTreeNew->Branch("output30",&shower_output30,"output30/F");
    ShowerTreeNew->Branch("output50",&shower_output50,"output50/F");
    ShowerTreeNew->Branch("eProb90",&shower_eProb90,"eProb90/I");
    ShowerTreeNew->Branch("eProb1",&shower_eProb1,"eProb1/I");
    ShowerTreeNew->Branch("E_MC",&shower_E_MC,"E_MC/F");
    ShowerTreeNew->Branch("idb",shower_idb,"idb[sizeb]/I");
    ShowerTreeNew->Branch("plateb",shower_plateb,"plateb[sizeb]/I");
    ShowerTreeNew->Branch("showerID",&shower_showerID,"showerID/I");
    ShowerTreeNew->Branch("isizeb",&shower_isizeb,"isizeb/I");
    ShowerTreeNew->Branch("xb",shower_xb,"xb[sizeb]/F");
    ShowerTreeNew->Branch("yb",shower_yb,"yb[sizeb]/F");
    ShowerTreeNew->Branch("zb",shower_zb,"zb[sizeb]/F");
    ShowerTreeNew->Branch("txb",shower_txb,"txb[sizeb]/F");
    ShowerTreeNew->Branch("tyb",shower_tyb,"tyb[sizeb]/F");
    ShowerTreeNew->Branch("nfilmb",shower_nfilmb,"nfilmb[sizeb]/I");
    ShowerTreeNew->Branch("ntrace1simub",shower_ntrace1simub,"ntrace1simu[sizeb]/I");
    ShowerTreeNew->Branch("ntrace2simub",shower_ntrace2simub,"ntrace2simu[sizeb]/I");
    ShowerTreeNew->Branch("ntrace3simub",shower_ntrace3simub,"ntrace3simu[sizeb]/F");
    ShowerTreeNew->Branch("ntrace4simub",shower_ntrace4simub,"ntrace4simu[sizeb]/I");
    ShowerTreeNew->Branch("chi2btkb",shower_chi2btkb,"chi2btkb[sizeb]/F");
    ShowerTreeNew->Branch("deltarb",shower_deltarb,"deltarb[sizeb]/F");
    ShowerTreeNew->Branch("deltathetab",shower_deltathetab,"deltathetab[sizeb]/F");
    ShowerTreeNew->Branch("deltaxb",shower_deltaxb,"deltaxb[sizeb]/F");
    ShowerTreeNew->Branch("deltayb",shower_deltayb,"deltayb[sizeb]/F");
    ShowerTreeNew->Branch("tagprimary",shower_tagprimary,"tagprimary[sizeb]/F");
    ShowerTreeNew->Branch("purityb",&shower_purityb,"purityb/F");
    ShowerTreeNew->Branch("trackdensb",&shower_trackdensb,"trackdensb/F");

    ShowerTreeNew->Branch("Energy",&shower_EnergyCorrectedb,"EnergyCorrectedb/F");
    ShowerTreeNew->Branch("EnergyUnCorrected",&shower_EnergyUnCorrectedb,"EnergyUnCorrectedb/F");
    ShowerTreeNew->Branch("EnergySigma",&shower_EnergySigmaCorrectedb,"EnergySigmaCorrectedb/F");
    ShowerTreeNew->Branch("EnergySigmaUnCorrected",&shower_EnergySigmaUnCorrectedb,"EnergySigmaUnCorrectedb/F");

    ShowerTreeNew->Branch("OldEnergy",&shower_OldEnergyCorrectedb,"OldEnergy/F");
    ShowerTreeNew->Branch("OldEnergyUnCorrected",&shower_OldEnergyUnCorrectedb,"OldEnergyUnCorrected/F");
    ShowerTreeNew->Branch("OldEnergySigma",&shower_OldEnergySigmaCorrectedb,"OldEnergySigma/F");
    ShowerTreeNew->Branch("OldEnergySigmaUnCorrected",&shower_OldEnergySigmaUnCorrectedb,"OldEnergySigmaUnCorrected/F");


    // Loop over Tree Entries (==different showers):
    for (int i=0; i<nent; ++i) {
        eShowerTree->GetEntry(i);
        //cout << "i = " << i << " " << eEnergyArrayCount <<  endl;
//         eShowerTree->Show(i);
        shower_EnergyCorrectedb=eEnergyArray->At(i);
        shower_EnergyUnCorrectedb=eEnergyArrayUnCorrected->At(i);
        shower_EnergySigmaCorrectedb=eEnergyArraySigmaCorrected->At(i);
        shower_EnergySigmaUnCorrectedb=-12345;

        // Fill new Tree
        ShowerTreeNew->Fill();
    }
    ShowerTreeNew->Write();
    fileNew->Close();
    fileOld->Close();


    gSystem->Exec("mv -vf Shower.root Shower.Orig.root");
    gSystem->Exec("mv -vf New.root Shower.root");
    cout << "EdbShowAlgE_Simple::WriteNewRootFile...done."<<endl;
    cout << "-------------------------------------------------------------------------------------------"<< endl<< endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::Help()
{
    cout << "-------------------------------------------------------------------------------------------"<< endl<< endl;
    cout << "EdbShowAlgE_Simple::Help:"<<endl;
    cout << "EdbShowAlgE_Simple::Help:"<<endl;
    cout << "EdbShowAlgE_Simple::Help:  The easiest way:  "<<endl;
    cout << "EdbShowAlgE_Simple::Help:  EdbShowAlgE_Simple* ShowerAlgE1 = new EdbShowAlgE_Simple();  "<<endl;
    cout << "EdbShowAlgE_Simple::Help:  ShowerAlgE1->DoRun(RecoShowerArray);  // RecoShowerArray an TObjArray of EdbTrackP* "<<endl;
    cout << "EdbShowAlgE_Simple::Help:  ShowerAlgE1->WriteNewRootFile(); "<<endl;
    cout << "EdbShowAlgE_Simple::Help:  "<<endl;
    cout << "EdbShowAlgE_Simple::Help:  In shower_E.C // E.C you find more hints...."<<endl;
    cout << "EdbShowAlgE_Simple::Help:  Updates will follow........"<<endl;
    cout << "-------------------------------------------------------------------------------------------"<< endl<< endl;
}


//______________________________________________________________________________


void EdbShowAlgE_Simple::PrintEfficiencyParametrisation()
{
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation for angles theta=0,0.1,..,0.6:" << endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0): "<< eEfficiencyParametrisation->Eval(0)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.1): "<< eEfficiencyParametrisation->Eval(0.1)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.2): "<< eEfficiencyParametrisation->Eval(0.2)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.3): "<< eEfficiencyParametrisation->Eval(0.3)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.4): "<< eEfficiencyParametrisation->Eval(0.4)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.5): "<< eEfficiencyParametrisation->Eval(0.5)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.6): "<< eEfficiencyParametrisation->Eval(0.6)<< endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE_Simple::ReadTables()
{
    cout << "EdbShowAlgE_Simple::ReadTables"<<endl;

    ReadTables_Energy();

    cout << "EdbShowAlgE_Simple::ReadTables...done."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::ReadTables_Energy()
{
    cout << "EdbShowAlgE_Simple::ReadTables_Energy"<<endl;

    char name [100];
    FILE * fFile;

    Int_t N_E=12;
    Int_t N_NPL=13;
    Int_t NPL[13]= {10,12,14,16,18,20,23,26,29,32,35,40,45};
    Double_t	E[12]  = {0.5,1.0,2.0, 4.0, 0.75, 1.5, 3.0, 6.0, 8.0, 16.0, 32.0 ,64.0};
    Int_t   	E_ASCEND[12]  = {0,4,1,5,2,6,3,7,8,9,10,11};
    Int_t npl;
    Float_t energyresolution;
    Double_t E_Array_Ascending[12]= {0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0, 8.0, 16.0, 32.0 ,64.0};
    Double_t E_Resolution[12]= {0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0, 8.0, 16.0, 32.0 ,64.0}; // initialize with 100% resolution...

    // First Create the ObjArray storing the Splines...
    eSplineArray_Energy_Stat_Electron = new TObjArray();
    eSplineArray_Energy_Stat_Gamma = new TObjArray();
    eSplineArray_Energy_Sys_Electron = new TObjArray();
    eSplineArray_Energy_Sys_Gamma = new TObjArray();

    // A) Table: Statistics: Electrons: "libShower_Energy_Statistics_Electron.txt"
    sprintf(name,"tables/libShower_Energy_Statistics_Electron.txt");
    fFile = fopen (name,"r");
    cout << "EdbShowAlgE_Simple::ReadTables_Energy() name = " << name << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Stat_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Stat_Electron->Add(Spline);
    }
    fclose (fFile);


    // B) Table: Statistics: Gamma: "libShower_Energy_Statistics_Gamma.txt"
    sprintf(name,"tables/libShower_Energy_Statistics_Gamma.txt");
    fFile = fopen (name,"r");
    cout << "EdbShowAlgE_Simple::ReadTables_Energy() name = " << name << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Stat_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Stat_Gamma->Add(Spline);
    }
    fclose (fFile);



    // C) Table: Systematics: Electrons: "libShower_Energy_Systematics_Electron.txt"
    sprintf(name,"tables/libShower_Energy_Systematics_Electron.txt");
    fFile = fopen (name,"r");
    cout << "EdbShowAlgE_Simple::ReadTables_Energy() name = " << name << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Sysy_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Sys_Electron->Add(Spline);
    }
    fclose (fFile);


    // D) Table: Systematics: Gamma: "libShower_Energy_Systematics_Gamma.txt"
    sprintf(name,"tables/libShower_Energy_Systematics_Gamma.txt");
    fFile = fopen (name,"r");
    cout << "EdbShowAlgE_Simple::ReadTables_Energy() name = " << name << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Sysy_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Sys_Gamma->Add(Spline);
    }
    fclose (fFile);


    cout << "EdbShowAlgE_Simple::ReadTables_Energy...done."<<endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE_Simple::SetEfficiencyParametrisationAngles()
{
    //EffFunc_UserEfficiency->Set
    return;
}

//______________________________________________________________________________

void EdbShowAlgE_Simple::SetEfficiencyParametrisationValues(Double_t* Angles, Double_t* EffValuesAtAngles)
{
    cout << "EdbShowAlgE_Simple::SetEfficiencyParametrisationValues()" << endl;
    cout << "ATTENTION: Array has to consist of efficiencies at !seven! angles: 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6..." << endl;
    cout << "ATTENTION: If not, then it might crash! " << endl;
    // Set User Efficiency Angles by hand!
    // This is interface routine which than can be used by eda...
    delete eEfficiencyParametrisation;
    eEfficiencyParametrisation = new TSpline3("",Angles,EffValuesAtAngles,5,0,0,0.6);

    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation for angles theta=0,0.1,..,0.6:" << endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0): "<< eEfficiencyParametrisation->Eval(0)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.1): "<< eEfficiencyParametrisation->Eval(0.1)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.2): "<< eEfficiencyParametrisation->Eval(0.2)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.3): "<< eEfficiencyParametrisation->Eval(0.3)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.4): "<< eEfficiencyParametrisation->Eval(0.4)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.5): "<< eEfficiencyParametrisation->Eval(0.5)<< endl;
    cout << "EdbShowAlgE_Simple::eEfficiencyParametrisation->Eval(0.6): "<< eEfficiencyParametrisation->Eval(0.6)<< endl;
    return;
}
