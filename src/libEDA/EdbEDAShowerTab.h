#ifndef __EDA_ShowerTab_H__
#define __EDA_ShowerTab_H__

#include "EdbEDA.h"
#include "EdbShowerRec.h"
#include "EdbShowerAlg.h" // new: changed name from ShowAlg to ShowerAlg
#include <TG3DLine.h>
#include "EdbPVRQuality.h"

class EdbEDAShowerTab {
private:

    // The ShowerRec object:
    EdbShowerRec* eShowerRec;

    // The InitiatorBT array. Has to be variable otherwise it
    // will be out of scope:
    TObjArray* eInBTArray;

    // Two Pointers for the EdbPVRec object:
    EdbPVRec* pvrec_linkedtracks;
    EdbPVRec* pvrec_cpfiles;

    EdbPVRec* pvrec_cleaned_linkedtracks; // "fake" pvrec object with adapted BG quality cut in it.
    EdbPVRec* pvrec_cleaned_cpfiles; // "fake" pvrec object with adapted BG quality cut in it.
    EdbPVRec* pvrec_cleaned; // (==generic cleaned)
    

    // One Pointer for the generic EdbPVRec object:
    // This is the object where ALWASY the shower reco is performed!
    EdbPVRec* pvrec_generic;


    // Buttons and so on:
    TGComboBox* eComboBox_Parameter;
	TGComboBox* eComboBox_ParameterQualitySetting;
    TGCheckButton* eCheckButtonUseTSData;
    TGCheckButton* eCheckButtonDrawShowers;
    TGCheckButton* eCheckButtonCheckQualitycut;
    TGNumberEntry* eNumberEntry_ParaValue;
    TGNumberEntry* eTGNumberEntryDrawShowerNr;

  TGMainFrame *eParamWindow;

    // Various Variables used for settings:
    Bool_t eUseTSData;
    Bool_t eIsAliLoaded;
    Bool_t eIspvrec_cleaned; 
		
    Bool_t eDrawShowers;
    Bool_t eCheckQualitycut;

		Int_t eQualityCutSetting;
		

    // An own Shower array. Not the one of the ShowerRec object:
    Int_t  eNShowers;
    TObjArray* RecoShowerArray;

		// Actual Shower (current one, either for drawing or printing)
		EdbTrackP* eShower;



public:

    EdbEDAShowerTab();

    // -----------------------------------
    void MakeGUI();
		void MakeParameterWindowQualitySettings();
    void LoadShowerRecEdbPVRec();
    void CheckPVRec();
    void SetPVRecFromFile();

    void CheckBTDensity(EdbPVRec *pvr, TObjArray *tracks);
    void CheckBTDensityCanv(EdbPVRec *pvr = NULL, TObjArray *tracks = NULL);
		void CheckBTDensityUsingEdbPVRQuality();

    void CheckQualitycut();
    void CheckCosmicReco();
		
		void CheckAndSetPVRGeneric();
		

    void SetInBTFromSelected();
    void SetInBTFromLinkedTracks()        {
				cout << "EdbEDAShowerTab::WARNING: SetInBTFromLinkedTracks() not yet implemented." << endl;
        //eShowerRec->SetInBTArray(NULL);    /// TODO
    }
    void CheckInBT();

    void Reco();
    void Reset();

    void 		FindPairings();
    void 		FindPairingsNewAlgo();
    Bool_t 	CheckPairDuplications(Int_t nmax, Int_t SegPID,Int_t SegID,Int_t Seg2PID,Int_t Seg2ID,TArrayI* SegmentPIDArray,TArrayI* SegmentIDArray,TArrayI* Segment2PIDArray,TArrayI* Segment2IDArray);

    void PrintShowerRecInBT()             {
        cout << "DBG1"<<endl;
        eShowerRec->PrintInitiatorBTs();
        cout << "DBG2"<<endl;
        return;
    }
    void PrintShowerRecRecoShowerArray()  {
        eShowerRec->PrintRecoShowerArray();
        return;
    }

    void ChangeShowerRecParameters();
    void PrintShowerRecParameters()       {
        eShowerRec->PrintParameters();
        return;
    }
    void ResetShowerRecParameters()       {
        eShowerRec->ResetAlgoParameters();
        return;
    }

    void SetParaSetHighNBT();
    void SetParaSetHighPur();

    void ReadComboBoxParameter();
    void SetNumberEntryShowerRecParameter(Double_t val);

		void ReadComboBoxParameterQualitySetting();



    void DrawShower(int iShowerNr,bool eClearShower=0);
    void DrawShower();
    void DrawShowerAll();
    void DrawSingleShower(EdbTrackP* shower);
    void DrawShowerPrevious();
    void DrawShowerNext();

    void DrawShowers(); // For the toggle Button!

    void Button1() {
        printf("****************   NOT IMPLEMETED    *************************\n");
    }
    void Button2() {
        printf("Button2\n");
    }
    void Button3() {
        PrintShower();
        printf("Print\n");
    }
    void Help();

    void GO(); // Make EVERY POSSIBLE OPTIONS!! ....still to be done...

    void PrintShower();
		void PrintActualShower();
    void PlotShower();

		void PrintQualityCutSetting();


    //-------------------------------------------------------------------------------------------
    // Usually already decleared in EdbEdaPlotTab, but since this is for now only of interest
    // of shower tab we redeclarate it here....
    TCanvas * CreateCanvas(char *plot_name) {
        // --- Create an embedded canvas
        gEve->GetBrowser()->StartEmbedding(1);
        gROOT->ProcessLineFast("new TCanvas");
        TCanvas *c1 = (TCanvas*) gPad;
        gEve->GetBrowser()->StopEmbedding(plot_name);
        return c1;
    }
		//-------------------------------------------------------------------------------------------
    double CalcdR_NoPropagation(EdbSegP* s1,EdbSegP* stest);
    double CalcdR_WithPropagation(EdbSegP* s1,EdbSegP* stest);
    double CalcdTheta(EdbSegP* s1,EdbSegP* s2);

    ClassDef(EdbEDAShowerTab,0) // Tab for accessibililty to Shower-rec.

};




#endif //__EDA_ShowerTab_H__

