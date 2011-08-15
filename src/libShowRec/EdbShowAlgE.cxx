#include "EdbShowAlgE.h"

using namespace std;

ClassImp(EdbShowAlgE)

//______________________________________________________________________________





EdbShowAlgE::EdbShowAlgE()
{
    // Default Constructor
    cout << "EdbShowAlgE::EdbShowAlgE()   Default Constructor"<<endl;

    // Reset all:
    Set0();

    // Init now with default parametrization (XX)
    Init();
}



//______________________________________________________________________________


EdbShowAlgE::EdbShowAlgE(TString paraname="XX")
{
    // Default Constructor
    cout << "EdbShowAlgE::EdbShowAlgE()   Default Constructor with Paraname ="<<  paraname<< endl;

    // Reset all:
    Set0();

    // Init now with given parametrization
    Init(paraname);
}




//______________________________________________________________________________


EdbShowAlgE::EdbShowAlgE(TObjArray* RecoShowerArray)
{
    // Default Constructor
    cout << "EdbShowAlgE::EdbShowAlgE(TObjArray* RecoShowerArray)   Default Constructor"<<endl;

    // Reset all:
    Set0();

    // Init now with default parametrization (XX)
    Init();

    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();

    cout << "EdbShowAlgE::EdbShowAlgE(TObjArray* RecoShowerArray)   Default Constructor...done."<<endl;
}



//______________________________________________________________________________


EdbShowAlgE::EdbShowAlgE(TObjArray* RecoShowerArray,TString paraname="XX")
{
    // Default Constructor
    cout << "EdbShowAlgE::EdbShowAlgE(TObjArray* RecoShowerArray,TString paraname)   Default Constructor with Paraname ="<<  paraname<< endl;

    // Reset all:
    Set0();

    // Init now with given parametrization
    Init(paraname);

    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();

    cout << "EdbShowAlgE::EdbShowAlgE(TObjArray* RecoShowerArray)   Default Constructor...done."<<endl;
}


//______________________________________________________________________________


EdbShowAlgE::~EdbShowAlgE()
{
    // Default Destructor
    cout << "EdbShowAlgE::~EdbShowAlgE()"<<endl;
}

//______________________________________________________________________________


void EdbShowAlgE::Set0()
{
    // Set0()
    eRecoShowerArray=0;
    eRecoShowerArrayN=0;

    eDoANNTrain=kFALSE;
    eDoANNRun=kFALSE;

    if (!gROOT->GetClass("TMultiLayerPerceptron")) cout << "NOT"<<endl;
    if (gROOT->GetClass("TMultiLayerPerceptron")) cout << "YES"<<endl;

    eMLP_Simple=0;
    eMLP_Layout="";
    eMLP_LayoutArray=0;
    eMLP_LayoutArrayIsDone=kFALSE;
    eMLP_WeightFileName="";
    eMLP_SimpleTree=0;
    eMLP_TrainEpochs=100;


    // Reset All  eInANN  Values to Zero:
    eInANN = new Float_t[70];
    eInfoANN = new Float_t[70];
    for (Int_t j=0; j<70; j++) {
        eInANN[j]=0;
        eInfoANN[j]=0;
        eInANN_Hist[j]=0;
    }
    eOutANN=0;


    eMLP_InputNeurons=0;
    eMLP_InputNeuronsArray=0;
    eMLP_InputNeuronsArrayN=0;

    eMLP_ReadMLPInputFile=kFALSE;
    eMLP_WriteMLPInputFile=kFALSE;
    eMLP_InputFileName="EMPTY";

    eMLP_WriteWeightFile=kFALSE;
    eMLP_WeightFileName="EMPTY";

    eMLPArrayN=0;
    eMLPArray=0;

    eShowAlgEPlateBinning=0;

    eShowAlgEPlatesN=0;

    return;
}

//______________________________________________________________________________

void EdbShowAlgE::Init()
{
    cout << "EdbShowAlgE::Init()"<<endl;
    //  default ShowAlge name is set to "XX" ( cause it is implemented now.)
    //  later its gonna be "FJ"
    eShowAlgEParaName="XX";
    eShowAlgEParaNr=4;

    // Init to set Paraname and ParaNr
    Init(eShowAlgEParaName);

    return;
}

//______________________________________________________________________________

void EdbShowAlgE::Init(TString ParaName)
{

    cout << "EdbShowAlgE::Init("<<ParaName<<")"<<endl;

    if (ParaName=="FJ") eShowAlgEParaNr=0;
    if (ParaName=="LT") eShowAlgEParaNr=1;
    if (ParaName=="YC") eShowAlgEParaNr=2;
    if (ParaName=="JC") eShowAlgEParaNr=3;
    if (ParaName=="XX") eShowAlgEParaNr=4;
    if (ParaName=="YY") eShowAlgEParaNr=5;
    if (ParaName=="PP") eShowAlgEParaNr=6;
    if (ParaName=="AS") eShowAlgEParaNr=7;

    eShowAlgEParaName=ParaName;

    cout << "EdbShowAlgE::Init("<<eShowAlgEParaNr<<")"<<endl;


    // Now be careful !!
    // These functions have to be called in THE RIGHT ORDER to have the necessary variables correctly filled:

    // Read Parameter Values for settings to be taken into account:
    ReadShowAlgEPar("default.par");


    // If by "default.par" still no weightfiles and therefore no eMLPArrays are given,
    // create them by the standard way:
    if (eMLPArrayN==0) {
        CreateStandardWeightFileStrings();
        CreateStandardMLPArrayN();
    }
    if (eMLP_InputFileName=="EMPTY") {
        // jaja ich weiss, super stil...
        // Set Input file name (default)
        TString tmpString="MLPInput_"+TString(Form("%d_",eShowAlgEPlatesN))+ParaName+".root";
        SetMLPInputFileName(tmpString);
    }

    // Create (on heap) input histograms:
    CreateInputHistos();
    // Specific bins for Parametrization:
    SetBinsInputHistos();

    // Make Layouts (default);
    CreateMLPLayouts();

    // Make MLPs (default);
    CreateMLPs();

    return;
}



//______________________________________________________________________________


void EdbShowAlgE::Initialize()
{
    return;
}
//______________________________________________________________________________

void EdbShowAlgE::Execute()
{
    return;
}

//______________________________________________________________________________

void EdbShowAlgE::Finalize()
{
    return;
}

//______________________________________________________________________________

void EdbShowAlgE::Print()
{
    cout << "EdbShowAlgE::Print()" << endl;
    cout << "EdbShowAlgE::Print()...done." << endl;
    return;
}

//______________________________________________________________________________














//______________________________________________________________________________
void EdbShowAlgE::Run()
{
    cout << "void EdbShowAlgE::Run() "<< endl;

    if (!eMLP_SimpleTree) {
        cout << "void EdbShowAlgE::Run() NO  eMLP_SimpleTree   Read  eMLP_SimpleTree  from file:";
        cout << "void EdbShowAlgE::Run() "<<   eMLP_InputFileName << endl;
        if (eMLP_ReadMLPInputFile) ReadMLPInputFile();
    }

    if (!eMLP_SimpleTree) {
        cout << "void EdbShowAlgE::Run() NO  eMLP_SimpleTree   Some Error has occured! RETURN."<< endl;
        return;
    }

    // Set Branch Addresses for the tree:
    eMLP_SimpleTree->SetBranchAddress("eInANN",eInANN);
    eMLP_SimpleTree->SetBranchAddress("eInfoANN",eInfoANN);


    PrintWeightFileStrings(); // DEBUG()



    //------------------------------
    /// Load Weights : Absolutely neccessary!
    /// MISSING:  A CHECK  IF THE WEIGHTFILE IS IN THE CURRENT DIRECTORY AND THERE AT ALL  !!!
    /// (otherwise link from source dir)
    LoadMLPWeights();
    //------------------------------


    // Set TTree as source for the MLP:
    // Absolutely neccessary otherwise its empty.
    for (Int_t i=0; i<10; ++i) {
        TEventList*	list;
        list= (TEventList*)eMLP_SimpleTree->GetEventList();
        eMLP_Simple= (TMultiLayerPerceptron*)eMLPArray->At(i);
        cout << eMLP_Simple->GetStructure() << endl;
        cout << eMLP_SimpleTree->GetEntries() << endl;
        eMLP_Simple->Print();
        eMLP_Simple->SetData(eMLP_SimpleTree);
        eMLP_Simple->SetTrainingDataSet(list);
        eMLP_Simple->SetTestDataSet(list);
        eMLP_Simple->SetTrainingDataSet("Entry$%2");
        eMLP_Simple->SetTestDataSet("(Entry$+1)%2");
    }


    Int_t nMLPArray=-1;
    Double_t  params[70]; // MLP works only with Double_t
    Float_t val;


    TH1F* h1= new TH1F("h1","h1",100,0,10);
    // Loop over entries:
// 	for (Int_t i=1; i<eMLP_SimpleTree->GetEntries(); ++i) {
    for (Int_t i=0; i<3; ++i) {
        eMLP_SimpleTree->GetEntry(i);

        eMLP_SimpleTree->Show(i);
        cout << "i= " << i << "    " << eInfoANN[0] << "  " <<  eInfoANN[1] << endl;
        cout << "i= " << i << "    " << eInANN[0] << "  " <<  eInANN[1] << endl;

        // Get Right ANN  (depending on reconstructed plates).
        /// Weights have to be already loaded ??????
        nMLPArray=GetMLPArrayNr(eInfoANN[1]);

        cout << nMLPArray << endl;

        eMLP_Simple= (TMultiLayerPerceptron*)eMLPArray->At(nMLPArray);
        cout << eMLP_Simple->GetStructure() << endl;







        for (Int_t j=1; j<=70; ++j) {
// 			cout << "DEBUG eInANN[j] " << eInANN[j]  << endl;
            params[j-1]=eInANN[j];
        }
        val=(eMLP_Simple->Evaluate(0,params));


        h1->Fill(val);

        cout << "DEBUG " << eInANN[1] << " " << eInANN[2] << " " << eInANN[3] << " " << eInANN[4] << " " << eInANN[5] << " " << eInANN[6] << " " << eInANN[7] << " " << endl;
        cout << "DEBUG: i,   val  (=mlp1->Evaluate(0,params))  == real energy    " << i << "   " << val << "    ==  " << eInANN[0]  << endl;


    }



    h1->Draw();


// 	eMLP_Simple= (TMultiLayerPerceptron*)eMLPArray->At(0);

// 	cout << eMLP_Simple->GetStructure() << endl;
// 	cout << eMLP_SimpleTree->GetEntries() << endl;


    return;
}


//______________________________________________________________________________


Int_t EdbShowAlgE::GetMLPArrayNr(Int_t NPlToCheck)
{
    cout <<"dbShowAlgE::GetMLPArray"<<endl;

    Int_t n=-1;
    for (Int_t i=0; i<10; ++i) {
        //cout << "i =  eShowAlgEPlateBinning->At(i)  =   NPlToCheck  " << i << "  " <<  eShowAlgEPlateBinning->At(i) << "  " << NPlToCheck << endl;
        n=i;
        if ( eShowAlgEPlateBinning->At(i) >= NPlToCheck) return n;
    }

    // In standard case it retuns 0 for the first MLP
    return 0;
}





//______________________________________________________________________________



void EdbShowAlgE::TESTRUN()
{

    // Fill Tree with existing entries of para of  eRecoShowerArray :
    Fill_MLPTree();



    FillInputHistosPara_XX();





    TString layout;
    int NrOfANNInputNeurons=7;
    layout=CreateMLPLayout(NrOfANNInputNeurons);




    SetMLPLayout(layout);


    cout <<"__________"<<endl;
    cout << layout << endl;
    cout <<"__________"<<endl;

    Double_t  params[70]; // MLP works only with Double_t
    Float_t val;



    /// DEBUG  DEBUG  DEBUG

    cout << " eMLP_Simple  =  " <<  eMLP_Simple << endl;

    eMLP_Simple =0;

    cout << " eMLP_Simple  =  " <<  eMLP_Simple << endl;

// 	TMultiLayerPerceptron*
    eMLP_Simple	= 	new TMultiLayerPerceptron(layout,eMLP_SimpleTree,"Entry$%2","(Entry$+1)%2");



    cout << " eMLP_Simple  =  " <<  eMLP_Simple << endl;


    /// DEBUG  DEBUG  DEBUG


    eMLP_Simple->LoadWeights("weights.txt");

    eMLP_Simple->Print();


    TH1F* h1 = new TH1F("hh","hh",100,0,10);


    for (Int_t i=1; i<eMLP_SimpleTree->GetEntries(); ++i) {
        eMLP_SimpleTree->GetEntry(i);

        for (Int_t j=1; j<=NrOfANNInputNeurons; ++j) {
            params[j-1]=eInANN[j];
        }
        val=(eMLP_Simple->Evaluate(0,params));


        h1->Fill(val);

        cout << "DEBUG " << eInANN[1] << " " << eInANN[2] << " " << eInANN[3] << " " << eInANN[4] << " " << eInANN[5] << " " << eInANN[6] << " " << eInANN[7] << " " << endl;
        cout << "DEBUG: i,   val  (=mlp1->Evaluate(0,params))  == real energy    " << i << "   " << val << "    ==  " << eInANN[0]  << endl;
    }


    TCanvas* cc= new TCanvas();
    h1->Draw();

    return;
}




//______________________________________________________________________________
void EdbShowAlgE::WriteMLPInputFile()
{
    cout <<"void EdbShowAlgE::WriteMLPInputFile() "  << eMLP_InputFileName << endl;

    if (!eMLP_WriteMLPInputFile) {
        cout <<"void EdbShowAlgE::WriteMLPInputFile() WARNING::eMLP_WriteMLPInputFile= "  << eMLP_WriteMLPInputFile << " RETURN."  << endl;
        return;
    }

    TFile* file= new TFile(eMLP_InputFileName,"RECREATE");
    eMLP_SimpleTree->Write();
    eMLP_SimpleTree->Print();
    file->Close();
    delete file;
    cout <<"void EdbShowAlgE::WriteMLPInputFile()...done."<<endl;
    return;
}



//______________________________________________________________________________
void EdbShowAlgE::ReadMLPInputFile()
{
    if (!eMLP_ReadMLPInputFile) return;
    cout << eMLP_InputFileName << endl;
    TFile* file= new TFile(eMLP_InputFileName,"READ");
    eMLP_SimpleTree=(TTree*)file->Get("eMLP_SimpleTree");
    eMLP_SimpleTree->Print();
    cout <<"void EdbShowAlgE::ReadMLPInputFile()...done."<<endl;
    return;
}


//______________________________________________________________________________
void EdbShowAlgE::FillMLP_SimpleTree()
{
    cout << "EdbShowAlgE::FillMLP_SimpleTree()" << endl;


    cout <<"===============" << GetMLPInputFileName() << endl;

    // ---------
    if (!eMLP_SimpleTree) {
        cout << "EdbShowAlgE::FillMLP_SimpleTree()   NO eMLP_SimpleTree. Create now. " << endl;
        Create_MLPTree();
    }


    cout <<"===============" << GetMLPInputFileName() << endl;

    if (!eMLPArray) {
        cout << "EdbShowAlgE::FillMLP_SimpleTree()   NO eMLPArray " << endl;
        return;
    }


    cout <<"===============" << GetMLPInputFileName() << endl;

    // Do this in case that  eMLP_SimpleTree  is read by ReadMLPInputFile
    if (eMLP_ReadMLPInputFile) {
        cout << "EdbShowAlgE::FillMLP_SimpleTree()   Read  eMLP_SimpleTree  from file:" << endl;
        ReadMLPInputFile();
        return;
    }

    cout << "EdbShowAlgE::FillMLP_SimpleTree()   Call  FillMLP_SimpleTree___() now:"<< endl;
    if (eShowAlgEParaNr==2) FillMLP_SimpleTree_YC();
    if (eShowAlgEParaNr==4) FillMLP_SimpleTree_XX();
    if (eShowAlgEParaNr==5) FillMLP_SimpleTree_YY();


    cout <<"===============" << GetMLPInputFileName() << endl;

    cout << "EdbShowAlgE::FillMLP_SimpleTree()    done." << endl;
    return;
}

//______________________________________________________________________________
void EdbShowAlgE::FillMLP_SimpleTree_YC()
{
    cout << "EdbShowAlgE::FillMLP_SimpleTree_YC()" << endl;



    // Do this in case that  eMLP_SimpleTree  is filled by eRecoShowerArrayN
    EdbShowerP* show=0;
    for (int i=0; i<eRecoShowerArrayN; i++ ) {
        //cout << "EdbShowAlgE::FILL   i= " << i << endl;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        FillMLP_SimpleTree_YC(show,eShowAlgEParaNr);
    }
    eMLP_SimpleTree->Print();
    cout << "eMLP_SimpleTree>GetEntries():"<< eMLP_SimpleTree->GetEntries() << endl;

    eMLP_SimpleTree->Show(eMLP_SimpleTree->GetEntries()-1);
    cout << "EdbShowAlgE::FillMLP_SimpleTree_YC()...done." << endl;
    return;
}


//______________________________________________________________________________
void EdbShowAlgE::FillMLP_SimpleTree_XX()
{
    cout << "EdbShowAlgE::FillMLP_SimpleTree_XX()" << endl;



    // Do this in case that  eMLP_SimpleTree  is filled by eRecoShowerArrayN
    EdbShowerP* show=0;
    for (int i=0; i<eRecoShowerArrayN; i++ ) {
        //cout << "EdbShowAlgE::FILL   i= " << i << endl;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        FillMLP_SimpleTree_XX(show,eShowAlgEParaNr);
    }
    eMLP_SimpleTree->Print();
    cout << "eMLP_SimpleTree>GetEntries():"<< eMLP_SimpleTree->GetEntries() << endl;

    eMLP_SimpleTree->Show(eMLP_SimpleTree->GetEntries()-1);
    cout << "EdbShowAlgE::FillMLP_SimpleTree_XX()...done." << endl;
    return;
}

//______________________________________________________________________________
void EdbShowAlgE::FillMLP_SimpleTree_YY()
{
    cout << "EdbShowAlgE::FillMLP_SimpleTree_YY()" << endl;

    // Do this in case that  eMLP_SimpleTree  is filled by eRecoShowerArrayN
    EdbShowerP* show=0;
    for (int i=0; i<eRecoShowerArrayN; i++ ) {
        //cout << "EdbShowAlgE::FILL   i= " << i << endl;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        FillMLP_SimpleTree_YY(show,eShowAlgEParaNr);
    }
    eMLP_SimpleTree->Print();
    cout << "eMLP_SimpleTree>GetEntries():"<< eMLP_SimpleTree->GetEntries() << endl;

    eMLP_SimpleTree->Show(eMLP_SimpleTree->GetEntries()-1);
    cout << "EdbShowAlgE::FillMLP_SimpleTree_YY()...done." << endl;
    return;
}



//______________________________________________________________________________

void EdbShowAlgE::WriteMLP_SimpleTree(TString filename)
{
    if (!eMLP_WriteMLPInputFile) return;
// 	TFile* file= new TFile(filename,"RECREATE");
    TFile* file= new TFile(eMLP_InputFileName,"RECREATE");
    eMLP_SimpleTree->Write();
    file->Close();
    delete file;
    return;
}




//______________________________________________________________________________

void EdbShowAlgE::DumpWeights()
{
    if (!eMLP_WriteWeightFile) {
        cout << "void EdbShowAlgE::DumpWeights() eMLP_WriteWeightFile="<< eMLP_WriteWeightFile << "  . So write nothing. RETURN"<<endl;
        return;
    }
    if (GetMLPWeightFileName()==""||GetMLPWeightFileName()=="EMPTY") SetMLPWeightFileName("weights.txt");

    cout << "void EdbShowAlgE::DumpWeights() "<< endl;
    cout << eMLP_WeightFileName << endl;
    eMLP_Simple->DumpWeights(eMLP_WeightFileName);
    cout << "void EdbShowAlgE::DumpWeights() ... done."<< endl;
    return;
}







//______________________________________________________________________________
void EdbShowAlgE::Train()
{
    cout << "void EdbShowAlgE::Train() "<< endl;
    cout << "void EdbShowAlgE::Train()    We have to set the tree data from the ANN since at "<< endl;
    cout << "void EdbShowAlgE::Train()    creation time the  eMLP_SimpleTree  was empty."<< endl;

    if (!eMLP_SimpleTree) {
        cout << "void EdbShowAlgE::Train() NO  eMLP_SimpleTree   Read  eMLP_SimpleTree  from file:"<< endl;
        if (eMLP_ReadMLPInputFile) ReadMLPInputFile();
    }
    if (!eMLP_SimpleTree) {
        cout << "void EdbShowAlgE::Train() NO  eMLP_SimpleTree   Some ERROR has occured! RETURN."<< endl;
        return;
    }


    // In order not to tell which plates we have to train the ANN, we grep out the information
    // out of eINfoAnn[1], which contains plates. The maximumBin is used for the layout structure:
    eMLP_SimpleTree->Draw("eInfoANN[1]>>h1");
    TH1F *h1 = (TH1F*)gDirectory->Get("h1");
    h1->Draw();

    int MaxBinCenter = h1->GetBinCenter(h1->GetMaximumBin()) ;
    int MaxBinEntryCenter=h1->GetBinCenter(GetLastBinHistoFilles(h1));
    int MLPArrayNr = GetMLPArrayNr(MaxBinEntryCenter);



    cout << " MaxBinCenter = " << MaxBinCenter << endl;
    cout << " MLPArrayNr = " << MLPArrayNr << endl;
    cout << " MaxBinEntryCenter = " << MaxBinEntryCenter << endl;



    // check MaxBinCenter for Bounds
    if (MaxBinEntryCenter>57 || MaxBinEntryCenter<1) {
        cout << "void EdbShowAlgE::Train()  Something is wrong with MaxBinEntryCenter. Check your data (especially eInfoANN[1]) RETURN! "<<endl;
        return;
    }
    cout << "void EdbShowAlgE::Train() MLPArrayNr= " << MLPArrayNr << " So use this eMLPArray for Training."<<endl;


    TEventList*	list;
    list= (TEventList*)eMLP_SimpleTree->GetEventList();
    eMLP_Simple= (TMultiLayerPerceptron*)eMLPArray->At(MLPArrayNr);

    cout << eMLP_Simple->GetStructure() << endl;
    cout << eMLP_SimpleTree->GetEntries() << endl;

// 	eMLP_SimpleTree->Show(0);
// 	eMLP_SimpleTree->Show(eMLP_SimpleTree->GetEntries()-1);
// 	cout << eMLP_TrainEpochs << endl;

    eMLP_Simple->Print();

    eMLP_Simple->SetData(eMLP_SimpleTree);
    eMLP_Simple->SetTrainingDataSet(list);
    eMLP_Simple->SetTestDataSet(list);
    eMLP_Simple->SetTrainingDataSet("Entry$%2");
    eMLP_Simple->SetTestDataSet("(Entry$+1)%2");
    eMLP_Simple->Train(eMLP_TrainEpochs, "text,graph,update=10");



    TCanvas* mlpa_canvas = new TCanvas("mlpa_canvas","Network analysis");
    mlpa_canvas->Divide(1,2);
    TMLPAnalyzer ana(eMLP_Simple);
    ana.GatherInformations();
    ana.CheckNetwork();
    mlpa_canvas->cd(1);
    ana.DrawDInputs();
    mlpa_canvas->cd(2);
    eMLP_Simple->Draw();

    cout << "void EdbShowAlgE::Train() ...done."<< endl;
    return;
}

//______________________________________________________________________________



//______________________________________________________________________________

void EdbShowAlgE::Create_MLP()
{
    cout << "EdbShowAlgE::Create_MLP()" << endl;

    TString testlayout="HALLO";
    // This class implementation of TMultiLayerPerceptron has strange behaviors...
    // The usual eMLP_Simple=0 resetings dont take effect and in the header file this has to be declared with
    // Brackets.....I do not know why and try to keep it working in the standard way as much as possible!
    TMultiLayerPerceptron* eMLP_Simple = new TMultiLayerPerceptron(testlayout);

    eMLP_Simple->Print();

    eMLP_SimpleIsDone=kTRUE;

    cout << "EdbShowAlgE::Create_MLP()...done." << endl;
    return;
}

//______________________________________________________________________________





//______________________________________________________________________________



int EdbShowAlgE::ReadShowAlgEPar(const char *file="default.par")
{
    // read parameters from par-file
    // return: 0 if ok
    //        -1 if file access failed
    char buf[256];
    char key[256];
    char name[256];

    FILE *fp = fopen(file,"r");
    if (!fp) {
        Log(1,"ReadShowAlgEPar","ERROR open file: %s", file);
        return -1;
    }
    else Log(2,"ReadShowAlgEPar","Read shower parameters from file: %s", file );

    int dospec;
    int narg;
    TString tmpString;
    char tmpchar[256];
    TString tmpString2;
    char tmpchar2[256];

    // Since ShowerParametrisation if Fixed by default in the Constructor,
    // we can check here already if in a String the correct parametrisation is contained.

    while (fgets(buf, sizeof(buf), fp)) {
        for (Int_t i = 0; i < (Int_t)strlen(buf); i++)
            if (buf[i]=='#')  {
                buf[i]='\0';                       // cut out comments starting from #
                break;
            }

        if ( sscanf(buf,"%s",key)!=1 ) continue;

        if      ( !strcmp(key,"INCLUDE")   )
        {
            sscanf(buf+strlen(key),"%s",name);
            ReadShowAlgEPar(name);
        }

        else if ( !strcmp(key,"SHOW_ENERGY_TRAIN")  )
        {
            narg=sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_TRAIN = " << dospec << "   narg= " << narg << endl;
            if (dospec==0 || dospec==1) SetDoTrain(dospec);
        }
        else if ( !strcmp(key,"SHOW_ENERGY_RUN")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_RUN = " << dospec << endl;
            if (dospec==0 || dospec==1) SetDoRun(dospec);
        }

        else if ( !strcmp(key,"SHOW_ENERGY_PLATES")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_PLATES = " << dospec << endl;
            SetPlatesN(dospec);
        }





        else if ( !strcmp(key,"SHOW_ENERGY_FILE_WEIGHTFILE")  )
        {
            sscanf(buf+strlen(key),"%s %s",tmpchar2, tmpchar);
            tmpString=TString(tmpchar);
            tmpString2=TString(tmpchar2);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_FILE_WEIGHTFILE = " << tmpString << endl;
            if (!CheckStringToShowAlgEParaName(tmpString2)) continue;
// 				if (!CheckStringToShowAlgEParaName(tmpString2)) break;

            SetMLPWeightFileName(tmpString);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_FILE_WEIGHTFILE = " << eMLP_WeightFileName << endl;
        }


        else if ( !strcmp(key,"SHOW_ENERGY_WRITE_WEIGHTFILE")  )
        {
            sscanf(buf+strlen(key),"%s %d",tmpchar2, &dospec);
            tmpString2=TString(tmpchar2);
            if (!CheckStringToShowAlgEParaName(tmpString2)) continue;
            SetWriteMLPWeightFile(dospec);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_WRITE_WEIGHTFILE = " << dospec << endl;
        }


        /// TO IMPLEMENT TAHTS ITS REALLY WRITTEN !!!!
        /// sakjdhskjadhlaskjdhlasdlsakdhaslkd




        else if ( !strcmp(key,"SHOW_ENERGY_FILE_MLPINPUTFILE")  )
        {
            sscanf(buf+strlen(key),"%s %s",tmpchar2, tmpchar);
            tmpString=TString(tmpchar);
            tmpString2=TString(tmpchar2);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_FILE_MLPINPUTFILE = " << tmpString << endl;
            cout << CheckStringToShowAlgEParaName(tmpString2) << endl;
            if (!CheckStringToShowAlgEParaName(tmpString2)) continue;
// 				if (!CheckStringToShowAlgEParaName(tmpString2)) break;
            SetMLPInputFileName(tmpString);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_FILE_MLPINPUTFILE = " << eMLP_InputFileName << endl;
        }


        else if ( !strcmp(key,"SHOW_ENERGY_WRITE_MLPINPUTFILE")  )
        {
            sscanf(buf+strlen(key),"%s %d",tmpchar2, &dospec);
            tmpString2=TString(tmpchar2);
            if (!CheckStringToShowAlgEParaName(tmpString2)) continue;
            SetWriteMLPInputFile(dospec);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_WRITE_MLPINPUTFILE = " << dospec << endl;
        }


        else if ( !strcmp(key,"SHOW_ENERGY_READ_MLPINPUTFILE")  )
        {
            sscanf(buf+strlen(key),"%s %d",tmpchar2, &dospec);
            tmpString2=TString(tmpchar2);
            if (!CheckStringToShowAlgEParaName(tmpString2)) continue;
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_READ_MLPINPUTFILE = " << dospec << endl;
            SetReadMLPInputFile(dospec);
        }


        /// TO IMPLEMENT TAHTS ITS REALLY WRITTEN !!!!
        /// sakjdhskjadhlaskjdhlasdlsakdhaslkd






        else if ( !strcmp(key,"SHOW_ENERGY_TRAINEPOCHS")  )
        {
            sscanf(buf+strlen(key),"%d",&dospec);
            SetMLPTrainEpochs(dospec);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_TRAINEPOCHS = " << dospec << endl;
        }



        else if ( !strcmp(key,"SHOW_ENERGY_ADD_WEIGHTFILE_PLATE")  )
        {
            sscanf(buf+strlen(key),"%s %d",tmpchar, &dospec);
            tmpString=TString(tmpchar);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_ADD_WEIGHTFILE_PLATE = " << tmpString <<  "  " << dospec << endl;
            AddWeightFilePlate(tmpString,dospec);
        }

        else if ( !strcmp(key,"SHOW_ENERGY_READ_WEIGHTFILE_PLATES")  )
        {
            int nspec;
            int spec[10];
            nspec=sscanf(buf+strlen(key),"%s %d %d %d %d %d %d %d %d %d %d " ,tmpchar, &spec[0], &spec[1], &spec[2], &spec[3], &spec[4], &spec[5], &spec[6], &spec[7], &spec[8], &spec[9]);
            cout << "nspec= " << nspec << endl;
            tmpString=TString(tmpchar);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_ADD_WEIGHTFILE_PLATES = " << tmpString;
            for (int i=0; i<nspec-1; i++ ) {
                cout <<"  " << spec[i];
            }
            for (int i=0; i<nspec-1; i++ ) AddWeightFilePlate(tmpString,spec[i]);
        }

        else if ( !strcmp(key,"SHOW_ENERGY_ADD_WEIGHTFILE")  )
        {
            sscanf(buf+strlen(key),"%s %d %s",tmpchar, &dospec, tmpchar2);
            tmpString=TString(tmpchar);
            tmpString2=TString(tmpchar2);
            cout << "EdbShowRec::ReadShowAlgEPar   SHOW_ENERGY_ADD_WEIGHTFILE_PLATE = " << tmpString <<  "  " << dospec << "  " << tmpString2 <<endl;
            AddWeightFile(tmpString,dospec,tmpString2);
        }



    }
    fclose(fp);


    PrintWeightFileStrings();
    cout <<"===============" << GetMLPInputFileName() << endl;

    Log(2,"ReadShowAlgEPar","Read shower parameters...done.");
    return 0;
}

//______________________________________________________________________________


void EdbShowAlgE::SetDoTrain(Int_t type)
{
    if (type==1) 	eDoANNTrain=kTRUE;
    else 					eDoANNTrain=kFALSE;
    return;
}

//______________________________________________________________________________

void EdbShowAlgE::SetDoRun(Int_t type)
{
    if (type==1) 	eDoANNRun=kTRUE;
    else 					eDoANNRun=kFALSE;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::SetWriteMLPWeightFile(Int_t type)
{
    if (type==1) 	eMLP_WriteWeightFile=kTRUE;
    else 					eMLP_WriteWeightFile=kFALSE;
    return;
}

//______________________________________________________________________________



void EdbShowAlgE::SetWriteMLPInputFile(Int_t type)
{
    if (type==1) 	eMLP_WriteMLPInputFile=kTRUE;
    else 		eMLP_WriteMLPInputFile=kFALSE;
    return;
}

//______________________________________________________________________________


void EdbShowAlgE::SetReadMLPInputFile(Int_t type)
{
    if (type==1) 	eMLP_ReadMLPInputFile=kTRUE;
    else 		eMLP_ReadMLPInputFile=kFALSE;
    return;
}



//______________________________________________________________________________

void EdbShowAlgE::Create_MLPTree()
{

    Log(2,"EdbShowAlgE::Create_MLPTree","EdbShowAlgE::Create_MLPTree.");

    if (eMLP_SimpleTree) {
        delete eMLP_SimpleTree;
        eMLP_SimpleTree=0;
        cout << "deletion done"<<endl;
    }

    if (!eMLP_SimpleTree) {
        eMLP_SimpleTree = new TTree("eMLP_SimpleTree","eMLP_SimpleTree");
        eMLP_SimpleTree	->	Branch("eInANN", eInANN, "eInANN[70]/F");
        eMLP_SimpleTree	->	Branch("eInfoANN", eInfoANN, "eInfoANN[70]/F");
    }

    Log(2,"EdbShowAlgE::Create_MLPTree","EdbShowAlgE::Create_MLPTree....done");
    return;
}



//______________________________________________________________________________

void EdbShowAlgE::Fill_MLPTree()
{

    /// DRPECIATED !!!!!!!!!!!!

    return;


    cout << "EdbShowAlgE::Fill_MLPTree()" << endl;

    cout << "EdbShowAlgE::Fill_MLPTree()  // Get correct Parametrisation.  ONLY XX IS SUPPORTED AT THE RIGHT MOMENT !!!" << endl;

    // Get correct Parametrisation.  ONLY XX IS SUPPORTED AT THE RIGHT MOMENT !!!


    // Create and Clear Tree:
    if (!eMLP_SimpleTree) Create_MLPTree();
    eMLP_SimpleTree->Clear();



    EdbShowerP* show=0;
    for (int i=0; i<eRecoShowerArrayN; i++ ) {
        cout << "EdbShowAlgE::TESTTRAIN   i= " << i << endl;
        show=(EdbShowerP*)eRecoShowerArray->At(i);
        show->PrintNice();
        if (! show->GetParametrisationIsDone(4)) show->BuildParametrisation(4);

        /// Get Suited Parametrization:
        /// TO BE ADAPTED FOR ALL PARAMETRIZATIONS !!!!!
        EdbShowerP::Para_XX     ePara_XX;
        ePara_XX=show->GetPara_XX();

        // Get each Variables: InputType
        // Energy
        eInANN[0]=show->GetSegment(0)->P();
        // This can be different in later modifications (for example gamma Showers, where the Energy is not defined by single BT P(), but
        // by both starting BT close to each other (e+,e-)pair.

        // Variables: TestingType
        eInANN[1]=ePara_XX.nseg;
        eInANN[2]=ePara_XX.Mean_ProfileLongitudinal;
        eInANN[3]=ePara_XX.RMS_ProfileLongitudinal;
        eInANN[4]=ePara_XX.Max_ProfileLongitudinal;
        eInANN[5]=ePara_XX.Mean_ProfileTransversal;
        eInANN[6]=ePara_XX.RMS_ProfileTransversal;
        eInANN[7]=ePara_XX.Max_ProfileTransversal;

        if (gEDBDEBUGLEVEL>3) {
            for (int i=0; i<eRecoShowerArrayN; i++ ) cout << "EdbShowAlgE::_XX()    eInANN["<<i<<"]= " << eInANN[i] << endl;
            cout << "EdbShowAlgE:: ...."<<endl;
            cout << "EdbShowAlgE:: ...."<<endl;
            cout << "EdbShowAlgE::    eInANN[69]= " << eInANN[69] << endl;
        }

        eMLP_SimpleTree->Fill();
    }


    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgE::   Fill_MLPTree done." << endl;
    if (gEDBDEBUGLEVEL>2) eMLP_SimpleTree->Print();


    cout << "EdbShowAlgE::Fill_MLPTree()...done." << endl;
    return;
}

//______________________________________________________________________________





void EdbShowAlgE::CreateInputHistos()
{
    // Create Input Histos:
    // Bin Width and Bounds have to be overwritten later when parametrization is specified
    for (Int_t j=0; j<70; j++) {
        char hname[40];
        sprintf(hname,"Hist_eInANN_Hist_%d",j);
        if (!eInANN_Hist[j]) eInANN_Hist[j] = new TH1F(hname,hname,100,0, 1000.0);
    }
    return;
}


//______________________________________________________________________________


TCanvas* EdbShowAlgE::GetHistoCanvas()
{
    TCanvas* canv= new TCanvas();

    eMLP_InputNeurons=7;
    Int_t n=TMath::Sqrt(eMLP_InputNeurons)+1;
    //cout << "n=   " << n << endl;
    canv->Divide(n,n);
    for (Int_t j=1; j<=eMLP_InputNeurons+1; j++) {
        canv->cd(j);
        eInANN_Hist[j-1]->Draw();
    }

    return canv;

}

//______________________________________________________________________________

void EdbShowAlgE::SetInputHistos(TH1F* h, Int_t nbin, Double_t lower, Double_t upper)
{
    h->SetBins(nbin,lower,upper);
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::SetInputHistosPara(Int_t ParaType)
{
    if (ParaType==0) SetInputHistosPara_FJ();

    if (ParaType==2) SetInputHistosPara_YC();
    if (ParaType==4) SetInputHistosPara_XX();
    if (ParaType==5) SetInputHistosPara_YY();


    /// TO IMPLEMENT ALSO THE OTHERS....

    return;
}
//______________________________________________________________________________

void EdbShowAlgE::SetInputHistosPara_YC()
{
    // ---------

    return;
}

//______________________________________________________________________________

void EdbShowAlgE::SetInputHistosPara_FJ()
{

    return;
}


//______________________________________________________________________________

void EdbShowAlgE::SetInputHistosPara_XX()
{

    // ---------
    // Specific bins for XX Parametrization:
    SetInputHistos(eInANN_Hist[0],100,0,20000);

    SetInputHistos(eInANN_Hist[1],200,0,1000);

    SetInputHistos(eInANN_Hist[2],100,0,70000);
    SetInputHistos(eInANN_Hist[3],100,0,30000);
    SetInputHistos(eInANN_Hist[4],30,0,30);

    SetInputHistos(eInANN_Hist[5],100,0,5000);
    SetInputHistos(eInANN_Hist[6],100,0,5000);
    SetInputHistos(eInANN_Hist[7],100,0,100);
    // ---------


    return;
}

//______________________________________________________________________________

void EdbShowAlgE::SetInputHistosPara_YY()
{
    // ---------
    // Specific bins for XX Parametrization:
    SetInputHistos(eInANN_Hist[0],100,0,20000);

    SetInputHistos(eInANN_Hist[1],200,0,1000);

    SetInputHistos(eInANN_Hist[2],100,0,100);
    SetInputHistos(eInANN_Hist[3],100,0,100);
    SetInputHistos(eInANN_Hist[4],100,0,100);
    SetInputHistos(eInANN_Hist[5],100,0,100);
    SetInputHistos(eInANN_Hist[6],100,0,100);
    for (int i=0; i<56; i++)	SetInputHistos(eInANN_Hist[6+i],100,0,100);
    // ---------
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::FillInputHistosPara_XX()
{
    // Reset
    for (Int_t j=0; j<70; j++) eInANN_Hist[j]->Reset();
    //Set bins:
    SetInputHistosPara_XX();

    //Fill with entries from the tree:


    for (int i=0; i<eMLP_SimpleTree->GetEntries(); ++i) {
        eMLP_SimpleTree->GetEntry(i);
        // Fill InputHistograms now...
        for (Int_t j=0; j<70; j++) {
            eInANN_Hist[j]->Fill(eInANN[j]);
        }
    }


    return;
}






//______________________________________________________________________________


TString EdbShowAlgE::CreateMLPLayout(Int_t NrOfANNInputNeurons)
{
    TString layout;

    for (Int_t i=1; i<NrOfANNInputNeurons; ++i) layout += "@eInANN["+TString(Form("%d",i))+"],";
    layout +="@eInANN["+TString(Form("%d",NrOfANNInputNeurons))+"]:"+TString(Form("%d",NrOfANNInputNeurons+1))+":"+TString(Form("%d",NrOfANNInputNeurons));

    //layout+=":eInANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)
    layout+=":eInfoANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)

    return layout;
}


//______________________________________________________________________________


void EdbShowAlgE::ResetPlateBinning()
{
    Log(2,"EdbShowAlgE::ResetPlateBinning","EdbShowAlgE::ResetPlateBinning.");


    // Restore Standard Plate Binning:
    // (11,16,21..51,56)
    if (!eShowAlgEPlateBinning) eShowAlgEPlateBinning = new TArrayI(10);

    cout << "void EdbShowAlgE::ResetPlateBinning(): Steps for:";
    for (Int_t i=0; i<10; ++i) {
        eShowAlgEPlateBinning->AddAt(11+5*i,i);
        cout << eShowAlgEPlateBinning->At(i);
    }
    cout << " plates availible."<<endl;
    Log(2,"EdbShowAlgE::ResetPlateBinning","EdbShowAlgE::ResetPlateBinning...done.");
    return;
}

//______________________________________________________________________________


void EdbShowAlgE::ClearPlateBinning()
{
    cout << "void EdbShowAlgE::ClearPlateBinning() " << endl;
    // Restore Standard Plate Binning
    if (eShowAlgEPlateBinning) {
        delete eShowAlgEPlateBinning;
        eShowAlgEPlateBinning=0;

    }
    else {
        // do nothing;
    }

    // Not more than 10 values of weightfiles will be allowed!!!
    eShowAlgEPlateBinning = new TArrayI(10);
    for (Int_t i=0; i<10; ++i) eShowAlgEPlateBinning->AddAt(0,i);

    cout << "void EdbShowAlgE::ClearPlateBinning()...done. " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::ResetWeightFileNameArray()
{
    for (Int_t i=0; i<10; ++i) eMLP_WeightFileNameArray[i]="";
    return;
}



//______________________________________________________________________________


void EdbShowAlgE::CreateStandardMLPArrayN()
{
    cout << "void EdbShowAlgE::CreateStandardMLPArrayN() " << endl;
    cout << "void EdbShowAlgE::CreateStandardMLPArrayN() According to the standard weightfile strings" << endl;
    cout << "void EdbShowAlgE::CreateStandardMLPArrayN() (weights_energy_11.txt,...,weights_energy_46.txt)." << endl;

    if (!eShowAlgEPlateBinning) ResetPlateBinning();

    // Set Number that tells how many MLPs are instantiated.
    // 11,16,21,26,31,36,41,46 (51,56)
    // the array itsself contains 10 entries but for 51 and 56 plate the weightfiles do not exist yet.
    eMLPArrayN=10;

    // These two variables are always same!
    eMLP_InputNeuronsArrayN=eMLPArrayN;

// 	cout << "------"<< endl;
// 	cout << "eMLPArrayN  = " <<  eMLPArrayN << endl;
// 	cout << "eMLP_InputNeuronsArrayN  = " <<  eMLP_InputNeuronsArrayN << endl;

    cout << "void EdbShowAlgE::CreateStandardMLPArrayN() done." << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateStandardWeightFileStrings()
{
    cout << "void EdbShowAlgE::CreateStandardWeightFileStrings() " << endl;


    if (!eShowAlgEPlateBinning) ResetPlateBinning();

    // There can be some cases when default par is read in and eShowAlgEPlateBinning was creadted, but due to wrong input settings
    // of default.par. In this case, eShowAlgEPlateBinning is there, but Cleared (filled with zeros).
    // So if this, we have to ResetPlateBinning again:
    if (eShowAlgEPlateBinning && eShowAlgEPlateBinning->At(0)==0 && eShowAlgEPlateBinning->At(9)==0 ) {
        cout << "void EdbShowAlgE::CreateStandardWeightFileStrings()    ResetPlateBinning again, cause eShowAlgEPlateBinning is filled with zeros." << endl;
        ResetPlateBinning();
    }

    cout << "void EdbShowAlgE::CreateStandardWeightFileStrings() ResetPlateBinning done" << endl;

    // Create Standard weight file strings:
    // weight_energy_YY_"i".txt, i=11,16,21,26,31,36,41,46,51,56
    // .Data() to convert to const char
    Int_t tmpint;
    for (Int_t i=0; i<10; ++i) {
        tmpint=eShowAlgEPlateBinning->At(i);
        eMLP_WeightFileNameArray[i]=TString(Form("weights_energy_%s_%d.txt",eShowAlgEParaName.Data(),tmpint));
        //cout << eMLP_WeightFileNameArray[i] << endl;
    }

    cout << "void EdbShowAlgE::CreateStandardWeightFileStrings() ...done." << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::AddWeightFilePlate(TString tmpString, Int_t Plate )
{
    cout << "void EdbShowAlgE::AddWeightFilePlate() " << endl;

    cout << "eMLPArrayN  = " <<  eMLPArrayN << endl;
    cout << "tmpString  = " <<  tmpString << endl;
    cout << "Plate  = " <<  Plate << endl;


    // Check if the weightfile type and the ShowAlgeE paratye match!
    // (otherwise a weightfile for a wrong parametrisation is maybe taken)
    if (tmpString!=eShowAlgEParaName) {
        cout << tmpString << "  !=  "<< eShowAlgEParaName << endl;
        cout << "void EdbShowAlgE::AddWeightFilePlate() WARNING: WRONG PARANAME. WeightFilePlate NOT ADDED!" << endl;
        return;
    }


    // Things to do when called the first time....
    if (eMLPArrayN<1) ClearPlateBinning();

    // Not more than 10 values of weightfiles will be allowed!!!
    if (eMLPArrayN>=10) {
        cout << "void EdbShowAlgE::AddWeightFilePlate() WARNING: Not more than 10 values of weightfiles will be allowed!!. WeightFilePlate NOT ADDED!" << endl;
        return;
    }


    // Check if this weightfile to be added (for the plates) is not added already:
    Int_t tmpint;
    tmpint=CheckPlateAndPlateBinning(Plate);

    if (tmpint==1) {
        cout << "void EdbShowAlgE::AddWeightFilePlate() WARNING: A weightfile for this plate Number was alread added. NOT ADDED!" << endl;
        return;
    }


    // Set weightfilename string and increase number of weightfile options
    eShowAlgEPlateBinning->AddAt(Plate,eMLPArrayN);
    tmpint=eShowAlgEPlateBinning->At(eMLPArrayN);
    eMLP_WeightFileNameArray[eMLPArrayN]="ANN_Weights_Energy_"+tmpString+TString(Form("_%d.txt",tmpint));

    cout << "eMLP_WeightFileNameArray[eMLPArrayN] =  " << eMLP_WeightFileNameArray[eMLPArrayN] << endl;

    // Increase now...
    ++eMLPArrayN;

    cout << "------"<< endl;
    cout << "eMLPArrayN  = " <<  eMLPArrayN << endl;

    // These two variables are always same!
    eMLP_InputNeuronsArrayN=eMLPArrayN;
    cout << "void EdbShowAlgE::AddWeightFilePlate() done." << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::AddWeightFile(TString tmpString, Int_t Plate, TString tmpString2)
{
    cout << "void EdbShowAlgE::AddWeightFile()." << endl;

    // Check if the weightfile type and the ShowAlgeE paratye match!
    // (otherwise a weightfile for a wrong parametrisation is maybe taken)
    if (tmpString!=eShowAlgEParaName) {
        cout << tmpString << "  !=  "<< eShowAlgEParaName << endl;
        cout << "void EdbShowAlgE::AddWeightFilePlate() WARNING: WRONG PARANAME. WeightFilePlate NOT ADDED!" << endl;
        return;
    }

    // Things to do when called the first time....
    if (eMLPArrayN<1) ClearPlateBinning();




    // Not more than 10 values of weightfiles will be allowed!!!
    if (eMLPArrayN>=10) {
        cout << "void EdbShowAlgE::AddWeightFile() WARNING: Not more than 10 values of weightfiles will be allowed!!. WeightFilePlate NOT ADDED!" << endl;
        return;
    }

    // Check if this weightfile to be added (for the plates) is not added already:
    Int_t tmpint;
    tmpint=CheckPlateAndPlateBinning(Plate);

    if (tmpint==1) {
        cout << "void EdbShowAlgE::AddWeightFile() WARNING: A weightfile for this plate Number was alread added. NOT ADDED!" << endl;
        return;
    }


    // Set weightfilename string and increase number of weightfile options
    eShowAlgEPlateBinning->AddAt(Plate,eMLPArrayN);
    tmpint=eShowAlgEPlateBinning->At(eMLPArrayN);
    eMLP_WeightFileNameArray[eMLPArrayN]=tmpString2;

    cout << "eMLP_WeightFileNameArray[eMLPArrayN] =  " << eMLP_WeightFileNameArray[eMLPArrayN] << endl;



    // Increase now...
    ++eMLPArrayN;

    cout << "------"<< endl;
    cout << "eMLPArrayN  = " <<  eMLPArrayN << endl;

    // These two variables are always same!
    eMLP_InputNeuronsArrayN=eMLPArrayN;


    cout << "void EdbShowAlgE::AddWeightFile() done." << endl;

    return;
}



//______________________________________________________________________________

void EdbShowAlgE::PrintWeightFileStrings()
{
    if (!eShowAlgEPlateBinning) return;
    if (eMLPArrayN<1) return;

    cout << "void EdbShowAlgE::PrintWeightFileStrings() " << endl;

    // Create Standard weight file strings:
    // weights_energy_"i".txt,   i=11,16,21,26,31,36,41,46
    for (Int_t i=0; i<eMLPArrayN; ++i) {
        cout << "void EdbShowAlgE::PrintWeightFileStrings() Name ("<<i<<"): "<< eMLP_WeightFileNameArray[i] << endl;
    }

    return;
}



//______________________________________________________________________________


Int_t EdbShowAlgE::CheckPlateAndPlateBinning(Int_t Plate)
{
    if (eMLPArrayN<1) return 0;

    Int_t tmpint;
    for (Int_t i=0; i<eMLPArrayN; ++i) {
        tmpint=eShowAlgEPlateBinning->At(i);
        //cout <<"CheckPlateAndPlateBinning , Plate, i   =eShowAlgEPlateBinning->At(i) " << Plate << " " << i << " " << tmpint<< endl;
        if (Plate==tmpint) return 1;
    }
    return 0;
}



//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts() " << endl;

    cout << "void EdbShowAlgE::CreateMLPLayouts()    Calling fuction suited to the eShowAlgEParaName" << endl;
    cout << "void EdbShowAlgE::CreateMLPLayouts()    Respectively to the eShowAlgEParaNr" << endl;

    switch (eShowAlgEParaNr) {
    case 0 :
        CreateMLPLayouts_FJ();
        break;
    case 1 :
        CreateMLPLayouts_LT();
        break;
    case 2 :
        CreateMLPLayouts_YC();
        break;
    case 3 :
        CreateMLPLayouts_JC();
        break;
    case 4 :
        CreateMLPLayouts_XX();
        break;
    case 5 :
        CreateMLPLayouts_YY();
        break;
    default :
        CreateMLPLayouts_XX();
        break;
    }

    return;
}





//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts_FJ()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts_FJ() " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts_LT()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts_LT() " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts_YC()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts_YY() " << endl;

    if (!eMLP_InputNeuronsArray) CreateInputNeuronsArray();

    FillInputNeuronsArray_YC();

    // This is the loop for the number of eMLPs to be availible. For each plate number weightfile
    // a new layout is created.
    for (Int_t i=0; i<eMLPArrayN; ++i) CreateMLPLayout_YC(i);

    // Now we know that it was done.
    // But no check if layouts are correctly, only the function steps have been done!
    eMLP_LayoutArrayIsDone=kTRUE;

    cout << "void EdbShowAlgE::CreateMLPLayouts_YC()...done." << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts_JC()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts_JC() " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts_XX()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts_XX() " << endl;

    if (!eMLP_InputNeuronsArray) CreateInputNeuronsArray();

    FillInputNeuronsArray_XX();

    // This is the loop for the number of eMLPs to be availible. For each plate number weightfile
    // a new layout is created.
    for (Int_t i=0; i<eMLPArrayN; ++i) CreateMLPLayout_XX(i);

    // Now we know that it was done.
    // But no check if layouts are correctly, only the function steps have been done!
    eMLP_LayoutArrayIsDone=kTRUE;

    cout << "void EdbShowAlgE::CreateMLPLayouts_XX()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayouts_YY()
{
    cout << "void EdbShowAlgE::CreateMLPLayouts_YY() " << endl;

    if (!eMLP_InputNeuronsArray) CreateInputNeuronsArray();

    FillInputNeuronsArray_YY();

    // This is the loop for the number of eMLPs to be availible. For each plate number weightfile
    // a new layout is created.
    for (Int_t i=0; i<eMLPArrayN; ++i) CreateMLPLayout_YY(i);

    // Now we know that it was done.
    // But no check if layouts are correctly, only the function steps have been done!
    eMLP_LayoutArrayIsDone=kTRUE;

    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayout_YC(Int_t nr)
{
    // This Implementation if now depending on the parametrization:
    // Either it is depending on the numer of reconstructed plates;
    // FJ,LT,YY it is.
    // YC,JC,XX it is not.

    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowAlgE::CreateMLPLayout_YC("<<nr<<") " << endl;

//	cout <<  "eShowAlgEPlateBinning->GetSize()  "  << eShowAlgEPlateBinning->GetSize() <<endl;
// 	cout <<  "eMLPArrayN  "  <<eMLPArrayN <<endl;
// 	cout <<  "eShowAlgEPlateBinning->At(nr)  "  << eShowAlgEPlateBinning->At(nr) <<endl;
// 	cout <<  "eMLP_InputNeuronsArray->At(nr)  "  << eMLP_InputNeuronsArray->At(nr) <<endl;


    // Here the important change it acoording to the needs of the parametrization:
    Int_t NrOfANNInputNeurons=0;
    //XX:
    //NrOfANNInputNeurons=7;
    //FJ:
    //NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr)+1+2+2;
    // Simple Test
    //NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr);
    //YY:
    //NrOfANNInputNeurons=1+5+eMLP_InputNeuronsArray->At(nr);
    //YC:
    NrOfANNInputNeurons=1+2+2;
    //---

    TString layout;
    for (Int_t i=1; i<NrOfANNInputNeurons; ++i) layout += "@eInANN["+TString(Form("%d",i))+"],";
    layout +="@eInANN["+TString(Form("%d",NrOfANNInputNeurons))+"]:"+TString(Form("%d",NrOfANNInputNeurons+1))+":"+TString(Form("%d",NrOfANNInputNeurons));
    //layout+=":eInANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)
    layout+=":eInfoANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)

    if (!eMLP_LayoutArray) eMLP_LayoutArray = new TObjArray(eMLP_InputNeuronsArrayN);


    // Add layout to the arrays of layouts:
    // 	-------------
    // 	TString
    // 	Basic string class.
    // 	Cannot be stored in a TCollection... use TObjString instead.
    // 	-------------
    eMLP_Layout=layout;
    TObjString* objstr = new TObjString(eMLP_Layout.Data());
    eMLP_LayoutArray->Add(objstr);


    if (gEDBDEBUGLEVEL>3) eMLP_LayoutArray->Print();
    if (gEDBDEBUGLEVEL>3) cout << "void EdbShowAlgE::CreateMLPLayout_YC   layout =  " << layout << endl;

    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowAlgE::CreateMLPLayout_YC("<<nr<<") ...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayout_XX(Int_t nr)
{
    // This Implementation if now depending on the parametrization:
    // Either it is depending on the numer of reconstructed plates;
    // FJ,LT,YY it is.
    // YC,JC,XX it is not.

    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowAlgE::CreateMLPLayout_XX("<<nr<<") " << endl;

//	cout <<  "eShowAlgEPlateBinning->GetSize()  "  << eShowAlgEPlateBinning->GetSize() <<endl;
// 	cout <<  "eMLPArrayN  "  <<eMLPArrayN <<endl;
// 	cout <<  "eShowAlgEPlateBinning->At(nr)  "  << eShowAlgEPlateBinning->At(nr) <<endl;
// 	cout <<  "eMLP_InputNeuronsArray->At(nr)  "  << eMLP_InputNeuronsArray->At(nr) <<endl;


    // Here the important change it acoording to the needs of the parametrization:
    Int_t NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr);
    //XX:
    NrOfANNInputNeurons=7;
    //FJ:
    //NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr)+1+2+2;
    // Simple Test
    //NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr);


    TString layout;
    for (Int_t i=1; i<NrOfANNInputNeurons; ++i) layout += "@eInANN["+TString(Form("%d",i))+"],";
    layout +="@eInANN["+TString(Form("%d",NrOfANNInputNeurons))+"]:"+TString(Form("%d",NrOfANNInputNeurons+1))+":"+TString(Form("%d",NrOfANNInputNeurons));
    //layout+=":eInANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)
    layout+=":eInfoANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)

    if (!eMLP_LayoutArray) eMLP_LayoutArray = new TObjArray(eMLP_InputNeuronsArrayN);


    // Add layout to the arrays of layouts:
    // 	-------------
    // 	TString
    // 	Basic string class.
    // 	Cannot be stored in a TCollection... use TObjString instead.
    // 	-------------
    eMLP_Layout=layout;
    TObjString* objstr = new TObjString(eMLP_Layout.Data());
    eMLP_LayoutArray->Add(objstr);



    if (gEDBDEBUGLEVEL>3) eMLP_LayoutArray->Print();

    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowAlgE::CreateMLPLayout_XX("<<nr<<") ...done." << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPLayout_YY(Int_t nr)
{
    // This Implementation if now depending on the parametrization:
    // Either it is depending on the numer of reconstructed plates;
    // FJ,LT,YY it is.
    // YC,JC,XX it is not.

    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowAlgE::CreateMLPLayout_YY("<<nr<<") " << endl;

//	cout <<  "eShowAlgEPlateBinning->GetSize()  "  << eShowAlgEPlateBinning->GetSize() <<endl;
// 	cout <<  "eMLPArrayN  "  <<eMLPArrayN <<endl;
// 	cout <<  "eShowAlgEPlateBinning->At(nr)  "  << eShowAlgEPlateBinning->At(nr) <<endl;
// 	cout <<  "eMLP_InputNeuronsArray->At(nr)  "  << eMLP_InputNeuronsArray->At(nr) <<endl;


    // Here the important change it acoording to the needs of the parametrization:
    Int_t NrOfANNInputNeurons=0;
    //XX:
    //NrOfANNInputNeurons=7;
    //FJ:
    //NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr)+1+2+2;
    // Simple Test
    //NrOfANNInputNeurons=eMLP_InputNeuronsArray->At(nr);
    //YY:
    NrOfANNInputNeurons=1+5+eMLP_InputNeuronsArray->At(nr);
    //---


    TString layout;
    for (Int_t i=1; i<NrOfANNInputNeurons; ++i) layout += "@eInANN["+TString(Form("%d",i))+"],";
    layout +="@eInANN["+TString(Form("%d",NrOfANNInputNeurons))+"]:"+TString(Form("%d",NrOfANNInputNeurons+1))+":"+TString(Form("%d",NrOfANNInputNeurons));
    //layout+=":eInANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)
    layout+=":eInfoANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)

    if (!eMLP_LayoutArray) eMLP_LayoutArray = new TObjArray(eMLP_InputNeuronsArrayN);


    // Add layout to the arrays of layouts:
    // 	-------------
    // 	TString
    // 	Basic string class.
    // 	Cannot be stored in a TCollection... use TObjString instead.
    // 	-------------
    eMLP_Layout=layout;
    TObjString* objstr = new TObjString(eMLP_Layout.Data());
    eMLP_LayoutArray->Add(objstr);


    if (gEDBDEBUGLEVEL>3) eMLP_LayoutArray->Print();
    if (gEDBDEBUGLEVEL>3) cout << "void EdbShowAlgE::CreateMLPLayout_YY   layout =  " << layout << endl;

    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowAlgE::CreateMLPLayout_YY("<<nr<<") ...done." << endl;
    return;
}


//______________________________________________________________________________



void EdbShowAlgE::CreateInputNeuronsArray()
{
    cout << "void EdbShowAlgE::CreateInputNeuronsArray() " << endl;

    switch (eShowAlgEParaNr) {
    case 0 :  //CreateInputNeuronsArray_FJ();
        break;
    case 1 :  //CreateInputNeuronsArray_LT();
        break;
    case 2 :
        CreateInputNeuronsArray_YC();
        break;
    case 3 :  //CreateInputNeuronsArray_JC();
        break;
    case 4 :
        CreateInputNeuronsArray_XX();
        break;

    case 5 :
        CreateInputNeuronsArray_YY();
        break;

    default :
        CreateInputNeuronsArray_XX();
        break;
    }

    cout << "void EdbShowAlgE::CreateInputNeuronsArray() done..." << endl;
    return;
}



//______________________________________________________________________________


void EdbShowAlgE::CreateInputNeuronsArray_YC()
{
    cout << "void EdbShowAlgE::CreateInputNeuronsArray_YC() " << endl;
    // Restore Standard Plate Binning
    if (eMLP_InputNeuronsArray) {
        delete eMLP_InputNeuronsArray;
        eMLP_InputNeuronsArray=0;
    }
    else {
        // do nothing;
    }

    // Not more than 10 values of weightfiles will be allowed!
    eMLP_InputNeuronsArray = new TArrayI(10);
    for (Int_t i=0; i<10; ++i) eMLP_InputNeuronsArray->AddAt(0,i);

    cout << "void EdbShowAlgE::CreateInputNeuronsArray_YC()...done. " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::FillInputNeuronsArray_YC()
{
    cout << "void EdbShowAlgE::FillInputNeuronsArray_YC() " << endl;
    // Restore Standard Plate Binning
    if (!eMLP_InputNeuronsArray) return;

    // Not more than 10 values of weightfiles will be allowed!
    Int_t tmpint;

    for (Int_t i=0; i<10; ++i) {
        tmpint=eShowAlgEPlateBinning->At(i);
        eMLP_InputNeuronsArray->AddAt(tmpint,i);
        //cout << i  << "    " <<  tmpint << endl;
    }

    cout << "void EdbShowAlgE::FillInputNeuronsArray_YC()...done. " << endl;
    return;
}
//______________________________________________________________________________


void EdbShowAlgE::CreateInputNeuronsArray_XX()
{
    cout << "void EdbShowAlgE::CreateInputNeuronsArray_XX() " << endl;
    // Restore Standard Plate Binning
    if (eMLP_InputNeuronsArray) {
        delete eMLP_InputNeuronsArray;
        eMLP_InputNeuronsArray=0;
    }
    else {
        // do nothing;
    }

    // Not more than 10 values of weightfiles will be allowed!
    eMLP_InputNeuronsArray = new TArrayI(10);
    for (Int_t i=0; i<10; ++i) eMLP_InputNeuronsArray->AddAt(0,i);

    cout << "void EdbShowAlgE::CreateInputNeuronsArray_XX()...done. " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::FillInputNeuronsArray_XX()
{
    cout << "void EdbShowAlgE::FillInputNeuronsArray_XX() " << endl;
    // Restore Standard Plate Binning
    if (!eMLP_InputNeuronsArray) return;

    // Not more than 10 values of weightfiles will be allowed!
    Int_t tmpint;

    for (Int_t i=0; i<10; ++i) {
        tmpint=eShowAlgEPlateBinning->At(i);
        eMLP_InputNeuronsArray->AddAt(tmpint,i);
        //cout << i  << "    " <<  tmpint << endl;
    }

    cout << "void EdbShowAlgE::FillInputNeuronsArray_XX()...done. " << endl;
    return;
}



//______________________________________________________________________________


void EdbShowAlgE::CreateInputNeuronsArray_YY()
{
    cout << "void EdbShowAlgE::CreateInputNeuronsArray_YY() " << endl;
    // Restore Standard Plate Binning
    if (eMLP_InputNeuronsArray) {
        delete eMLP_InputNeuronsArray;
        eMLP_InputNeuronsArray=0;
    }
    else {
        // do nothing;
    }

    // Not more than 10 values of weightfiles will be allowed!
    eMLP_InputNeuronsArray = new TArrayI(10);
    for (Int_t i=0; i<10; ++i) eMLP_InputNeuronsArray->AddAt(0,i);

    cout << "void EdbShowAlgE::CreateInputNeuronsArray_YY()...done. " << endl;
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::FillInputNeuronsArray_YY()
{
    cout << "void EdbShowAlgE::FillInputNeuronsArray_YY() " << endl;
    // Restore Standard Plate Binning
    if (!eMLP_InputNeuronsArray) return;

    // Not more than 10 values of weightfiles will be allowed!
    Int_t tmpint;

    for (Int_t i=0; i<10; ++i) {
        tmpint=eShowAlgEPlateBinning->At(i);
        eMLP_InputNeuronsArray->AddAt(tmpint,i);
        //cout << i  << "    " <<  tmpint << endl;
    }

    cout << "void EdbShowAlgE::FillInputNeuronsArray_YY()...done. " << endl;
    return;
}


//______________________________________________________________________________




void EdbShowAlgE::CreateMLPs()
{
    // Since the layout if sixed by the parametrisation, this function
    // can be equall for all types of parametrisations:

    // Check if layouts have been done:
    if (!eMLP_LayoutArrayIsDone) {
        cout << "void EdbShowAlgE::CreateMLPs() ERROR: eMLP_LayoutArrayIsDone HAS NOT BEEN DONE. RETURN!" << endl;
        return;
    }


    Log(3,"EdbShowAlgE::CreateMLPs()","Since the layout if sixed by the parametrisation, this function ");
    Log(3,"EdbShowAlgE::CreateMLPs()","can be equall for all types of parametrisations.");
    Log(3,"EdbShowAlgE::CreateMLPs()","It is now only different for some parametrisations by the number of plates. ");
    Log(2,"EdbShowAlgE::CreateMLPs()","If you see now a message like:");
    Log(2,"EdbShowAlgE::CreateMLPs()","Data not set. Cannot define datasets.");
    Log(2,"EdbShowAlgE::CreateMLPs()","Then this means that we have not yet defined the  eMLP_SimpleTree  for the ANN. This has to be done later. In any case the ANN can be created without it.");


    Int_t tmpint;

    if (!eMLPArray) CreateMLPArray();


    for (Int_t i=0; i<eMLP_InputNeuronsArrayN; ++i) {

        tmpint= eMLP_InputNeuronsArray->At(i);

        TString layout = ((TObjString*)eMLP_LayoutArray->At(i))->GetString();

        eMLP_Simple	= 	new TMultiLayerPerceptron(layout, eMLP_SimpleTree,"Entry$%2","(Entry$+1)%2");

        //cout << " eMLP_Simple  =  " <<  eMLP_Simple << endl;

        eMLPArray->Add(eMLP_Simple);
    }

    cout << " eMLP_Simple  =  " <<  eMLP_Simple << endl;
    cout << " eMLPArray->GetEntries()  =  " <<  eMLPArray->GetEntries()   << endl;
    cout << " eMLPArrayN  =  " <<  eMLPArrayN << endl;

    // This should have no effect since eMLPArrayN is already filled with correct value;
    SetMLPArrayN(eMLPArray->GetEntries());

    cout << " eMLPArrayN  =  " <<  eMLPArrayN << endl;

    return;
}


//______________________________________________________________________________


void EdbShowAlgE::CreateMLPArray()
{
    if (!eMLPArray) eMLPArray = new TObjArray(10);
    return;
}


//______________________________________________________________________________


void EdbShowAlgE::LoadMLPWeights()
{
    // Since the layout if sixed by the parametrisation, this function
    // can be equall for all types of parametrisations:

    cout << "void EdbShowAlgE::LoadMLPWeights() " << endl;
    if (!eMLPArray) {
        cout << "void EdbShowAlgE::LoadMLPWeights() ERROR: eMLPArray IS EMPTY. RETURN!" << endl;
        return;
    }
    for (Int_t i=0; i<eMLPArrayN; ++i) LoadMLPWeights(i);

    return;
}

//______________________________________________________________________________

void EdbShowAlgE::LoadMLPWeights(Int_t nr)
{
    TMultiLayerPerceptron* mlp;
    mlp = (TMultiLayerPerceptron*)eMLPArray->At(nr);
    cout << "void EdbShowAlgE::LoadMLPWeights() Name ("<< nr <<"): "<< eMLP_WeightFileNameArray[nr] << endl;
    mlp->LoadWeights( eMLP_WeightFileNameArray[nr]);
    return;
}

//______________________________________________________________________________

void EdbShowAlgE::SetBinsInputHistos() {

    switch (eShowAlgEParaNr) {
    case 0 :
        SetBinsInputHistos_FJ();
        break;
    case 1 :  //CreateInputNeuronsArray_LT();
        break;
    case 2 :
        SetBinsInputHistos_YY();
        break;
    case 3 :  //CreateInputNeuronsArray_JC();
        break;
    case 4 :
        SetBinsInputHistos_XX();
        break;
    case 5 :
        SetBinsInputHistos_YY();
        break;
    default :
        SetBinsInputHistos_XX();
        break;
    }
    return;
}




//______________________________________________________________________________

void EdbShowAlgE::SetBinsInputHistos_FJ() {
    // Specific bins for FJ Parametrization:
    return;
}



//______________________________________________________________________________

void EdbShowAlgE::SetBinsInputHistos_YC() {
    // Specific bins for YC Parametrization:
    SetInputHistos(eInANN_Hist[0],100,0,20000);
    SetInputHistos(eInANN_Hist[1],200,0,1000);
    SetInputHistos(eInANN_Hist[2],100,0,100);
    SetInputHistos(eInANN_Hist[3],100,0,100);
    SetInputHistos(eInANN_Hist[4],100,0,100);
    SetInputHistos(eInANN_Hist[5],100,0,100);
    return;
}


//______________________________________________________________________________

void EdbShowAlgE::SetBinsInputHistos_XX() {
    // Specific bins for XX Parametrization:
    SetInputHistos(eInANN_Hist[0],100,0,20000);
    SetInputHistos(eInANN_Hist[1],200,0,1000);
    SetInputHistos(eInANN_Hist[2],100,0,70000);
    SetInputHistos(eInANN_Hist[3],100,0,30000);
    SetInputHistos(eInANN_Hist[4],30,0,30);
    SetInputHistos(eInANN_Hist[5],100,0,5000);
    SetInputHistos(eInANN_Hist[6],100,0,5000);
    SetInputHistos(eInANN_Hist[7],30,0,30);
    return;
}

//______________________________________________________________________________

void EdbShowAlgE::SetBinsInputHistos_YY() {
    // Specific bins for YY Parametrization:
    SetInputHistos(eInANN_Hist[0],100,0,20000);
    SetInputHistos(eInANN_Hist[1],200,0,1000);
    SetInputHistos(eInANN_Hist[2],100,0,100);
    SetInputHistos(eInANN_Hist[3],100,0,100);
    SetInputHistos(eInANN_Hist[4],100,0,100);
    SetInputHistos(eInANN_Hist[5],100,0,100);
    SetInputHistos(eInANN_Hist[6],100,0,100);
    for (int i=0; i<56; i++)	SetInputHistos(eInANN_Hist[6+i],100,0,100);
    return;
}

//______________________________________________________________________________


void EdbShowAlgE::FillMLP_SimpleTree_YC(EdbShowerP* show, Int_t ShowAlgEParaNr) {

    if (! show->GetParametrisationIsDone(ShowAlgEParaNr)) show->BuildParametrisation(ShowAlgEParaNr);

    // Get Suited Parametrization:
    EdbShowerP::Para_YC     ePara_YC;
    ePara_YC=show->GetPara_YC();


    cout << "EdbShowerP::FillMLP_SimpleTree_YC()    ePara_YC.nseg= " << ePara_YC.nseg << endl;
    cout << "EdbShowerP::FillMLP_SimpleTree_YC()    ePara_YC.C1= " << ePara_YC.C1 << endl;
    cout << "EdbShowerP::FillMLP_SimpleTree_YC()    ePara_YC.a1= " << ePara_YC.a1 << endl;
    cout << "EdbShowerP::FillMLP_SimpleTree_YC()    ePara_YC.alpha= " << ePara_YC.alpha << endl;
    cout << "EdbShowerP::FillMLP_SimpleTree_YC()    ePara_YC.nmax= " << ePara_YC.nmax << endl;

    // Get each Variables: InputType
    // Energy
    eInANN[0]=show->GetSegment(0)->P();
    // This can be different in later modifications (for example gamma Showers, where the Energy is not defined by single BT P(), but
    // by both starting BT close to each other (e+,e-)pair.

    // Variables: TestingType
    eInANN[1]=ePara_YC.nseg;
    eInANN[2]=ePara_YC.C1;
    eInANN[3]=ePara_YC.a1;
    eInANN[4]=ePara_YC.alpha;
    eInANN[5]=ePara_YC.nmax;

    // Fill also related eInfoANN variables:
    eInfoANN[0]=show->GetSegment(0)->P();
    eInfoANN[1]=show->Npl();


    if (gEDBDEBUGLEVEL>3) {
        for (int i=0; i<13; i++ ) cout << "EdbShowAlgE::_YC()    eInANN["<<i<<"]= " << eInANN[i] << endl;
        cout << "EdbShowAlgE:: ...."<<endl;
        for (int i=70-13; i<70; i++ ) cout << "EdbShowAlgE::_YC()    eInANN["<<i<<"]= " << eInANN[i] << endl;

        cout << "EdbShowAlgE::_YC()    eInfoANN[0]= " << eInfoANN[0] << endl;
        cout << "EdbShowAlgE::_YC()    eInfoANN[1]= " << eInfoANN[1] << endl;
    }

    // Fill InputHistograms now...
    for (Int_t j=0; j<70; j++) {
        eInANN_Hist[j]->Fill(eInANN[j]);
    }


    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowAlgE::   eMLP_SimpleTree " << eMLP_SimpleTree << endl;
        cout << "EdbShowAlgE::   eMLP_SimpleTree->ClassName()  " << eMLP_SimpleTree->ClassName()  << endl;
        cout << "EdbShowAlgE::   eMLP_SimpleTree->GetEntries():" << endl;
        cout << eMLP_SimpleTree->GetEntries() <<endl;
    }


    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgE::   eMLP_SimpleTree->Fill() do:" << endl;
// 		gDebug=2;
    eMLP_SimpleTree->Fill();
// 		gDebug=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgE::   eMLP_SimpleTree->Fill() done." << endl;

    return;
}



//______________________________________________________________________________


void EdbShowAlgE::FillMLP_SimpleTree_XX(EdbShowerP* show, Int_t ShowAlgEParaNr) {

    if (! show->GetParametrisationIsDone(ShowAlgEParaNr)) show->BuildParametrisation(ShowAlgEParaNr);

    // Get Suited Parametrization:
    EdbShowerP::Para_XX     ePara_XX;
    ePara_XX=show->GetPara_XX();

    // Get each Variables: InputType
    // Energy
    eInANN[0]=show->GetSegment(0)->P();
    // This can be different in later modifications (for example gamma Showers, where the Energy is not defined by single BT P(), but
    // by both starting BT close to each other (e+,e-)pair.

    // Variables: TestingType
    eInANN[1]=ePara_XX.nseg;
    eInANN[2]=ePara_XX.Mean_ProfileLongitudinal;
    eInANN[3]=ePara_XX.RMS_ProfileLongitudinal;
    eInANN[4]=ePara_XX.Max_ProfileLongitudinal;
    eInANN[5]=ePara_XX.Mean_ProfileTransversal;
    eInANN[6]=ePara_XX.RMS_ProfileTransversal;
    eInANN[7]=ePara_XX.Max_ProfileTransversal;

    // Fill also related eInfoANN variables:
    eInfoANN[0]=show->GetSegment(0)->P();
    eInfoANN[1]=show->Npl();


    if (gEDBDEBUGLEVEL>3) {
        for (int i=0; i<13; i++ ) cout << "EdbShowAlgE::_XX()    eInANN["<<i<<"]= " << eInANN[i] << endl;
        cout << "EdbShowAlgE:: ...."<<endl;
        for (int i=70-13; i<70; i++ ) cout << "EdbShowAlgE::_XX()    eInANN["<<i<<"]= " << eInANN[i] << endl;

        cout << "EdbShowAlgE::_XX()    eInfoANN[0]= " << eInfoANN[0] << endl;
        cout << "EdbShowAlgE::_XX()    eInfoANN[1]= " << eInfoANN[1] << endl;
    }

    // Fill InputHistograms now...
    for (Int_t j=0; j<70; j++) {
        eInANN_Hist[j]->Fill(eInANN[j]);
    }


    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowAlgE::   eMLP_SimpleTree " << eMLP_SimpleTree << endl;
        cout << "EdbShowAlgE::   eMLP_SimpleTree->ClassName()  " << eMLP_SimpleTree->ClassName()  << endl;
        cout << "EdbShowAlgE::   eMLP_SimpleTree->GetEntries():" << endl;
        cout << eMLP_SimpleTree->GetEntries() <<endl;
    }


    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgE::   eMLP_SimpleTree->Fill() do:" << endl;
// 		gDebug=2;
    eMLP_SimpleTree->Fill();
// 		gDebug=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgE::   eMLP_SimpleTree->Fill() done." << endl;

    return;
}



//______________________________________________________________________________




void EdbShowAlgE::FillMLP_SimpleTree_YY(EdbShowerP* show, Int_t ShowAlgEParaNr)
{

    cout << "DEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUGDEBUG"<<endl;
    if (! show->GetParametrisationIsDone(ShowAlgEParaNr)) {
        show->PrintNice();
        cout << "NO PARAMETRISATION DONE FOR show= " << show << "with " << show->N() << "Baestracks"<< endl;
    }

    if (! show->GetParametrisationIsDone(ShowAlgEParaNr)) show->BuildParametrisation(ShowAlgEParaNr);

    // Get Suited Parametrization:
    EdbShowerP::Para_YY     ePara_YY;
    ePara_YY=show->GetPara_YY();

    // Get each Variables: InputType
    // Energy
    eInANN[0]=show->GetSegment(0)->P();
    // This can be different in later modifications (for example gamma Showers, where the Energy is not defined by single BT P(), but
    // by both starting BT close to each other (e+,e-)pair.

    // Variables: TestingType
    eInANN[1]=ePara_YY.nseg;
    for (Int_t j=0; j<5; j++)  	eInANN[2+j]=ePara_YY.ProfileTransversalBincontent[1+j]; // 0 is Underflow bin.
    for (Int_t j=0; j<57; j++)  	eInANN[7+j]=ePara_YY.ProfileLongitudinalBincontent[1+j]; // 0 is Underflow bin.

    // Fill also related eInfoANN variables:
    eInfoANN[0]=show->GetSegment(0)->P();
    eInfoANN[1]=show->Npl();


    if (gEDBDEBUGLEVEL>3) {
        for (int i=0; i<13; i++ ) cout << "EdbShowAlgE::_YY()    eInANN["<<i<<"]= " << eInANN[i] << endl;
        cout << "EdbShowAlgE:: ...."<<endl;
        for (int i=70-13; i<70; i++ ) cout << "EdbShowAlgE::_YY()    eInANN["<<i<<"]= " << eInANN[i] << endl;

        cout << "EdbShowAlgE::_YY()    eInfoANN[0]= " << eInfoANN[0] << endl;
        cout << "EdbShowAlgE::_YY()    eInfoANN[1]= " << eInfoANN[1] << endl;
    }


    if (gEDBDEBUGLEVEL>3)  show->PrintBasics();
    if (gEDBDEBUGLEVEL>3)  show->PrintNice();
    if (gEDBDEBUGLEVEL>3)  ;
    show->PrintParametrisation_YY();



    // Fill InputHistograms now...
    for (Int_t j=0; j<70; j++) {
        eInANN_Hist[j]->Fill(eInANN[j]);
    }


    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowAlgE::   eMLP_SimpleTree " << eMLP_SimpleTree << endl;
        cout << "EdbShowAlgE::   eMLP_SimpleTree->ClassName()  " << eMLP_SimpleTree->ClassName()  << endl;
        cout << "EdbShowAlgE::   eMLP_SimpleTree->GetEntries():" << endl;
        cout << eMLP_SimpleTree->GetEntries() <<endl;
    }


    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgE::   eMLP_SimpleTree->Fill() do:" << endl;
// 		gDebug=2;
    eMLP_SimpleTree->Fill();
// 		gDebug=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgE::   eMLP_SimpleTree->Fill() done." << endl;

    return;
}

//______________________________________________________________________________

Bool_t EdbShowAlgE::CheckStringToShowAlgEParaName(TString tmpString2)
{
    if (	tmpString2 == eShowAlgEParaName ) return kTRUE;
    cout <<"Bool_t EdbShowAlgE::CheckStringToShowAlgEParaName()  " <<tmpString2 << " does not matcht to " << eShowAlgEParaName;
    cout << "  Return False."<< endl;
    return  kFALSE;
}

//______________________________________________________________________________

int EdbShowAlgE::GetLastBinHistoFilles(TH1* h1)
{
    int nbin=h1->GetNbinsX(); //overflow=nbin
    for (int i=nbin-1; i>0; i--) if (h1->GetBinContent(i)>0) return i;
    return 0;
}



//______________________________________________________________________________
