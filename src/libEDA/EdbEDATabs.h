#ifndef __EDA_TABS_H__
#define __EDA_TABS_H__

#include"EdbEDAUtil.h"
#include"EdbEDATrackSet.h"
#include"EdbEDASets.h"

using namespace EdbEDAUtil;


class EdbEDAListTab {
	private :
	TGListBox *fListFiles;
	TGListBox *fListFilesOpened;
	TGTextEditor *fListFileEditor;
	TGCheckButton *fCheck_list_clear_previous;

	public:
	
	EdbEDAListTab () {MakeGUI();}

	void ReviceFileList();
	void ReadSelectedList();
	void MakeGUI();
	void WriteListFile();
};





class EdbEDAVertexTab: public EdbVertexRec{
	public:
	int eNtrk;
	int eVtxTrk;

	TGNumberEntryField *eGNEImpMax;
	TGNumberEntryField *eGNEProbMinV;
	TGNumberEntryField *eGNEDZmax;
	TGNumberEntry *eGNEVNtrk;
	TGCheckButton *eGCBUseMom;
	TGCheckButton *eGCBUseSegPar;
	TGComboBox *eGCBVtxTrk;
	
	TGCheckButton *eGCBAngularCut;
	int eAngularCut;
	double eTX;
	double eTY;
	double eDT;
	
	EdbEDATrackSet *eSet;
	
	enum {
		USE_CURRENT_TRACKS,
		USE_ALL_TRACKS
	};
	
	EdbEDAVertexTab(){
		// initial values;
		eDZmax      = 5000.;  // maximum z-gap in the track-vertex group
		eProbMin    = 0.001;  // minimum acceptable probability for chi2-distance between tracks
		eImpMax     = 5.;    // maximal acceptable impact parameter [microns] (for preliminary check)
		eUseMom     = false;  // use or not track momentum for vertex calculations
		eUseSegPar  = true;  // use only the nearest measured segments for vertex fit (as Neuchatel)
		eQualityMode= 0;      // vertex quality estimation method (0:=Prob/(sigVX^2+sigVY^2); 1:= inverse average track-vertex distance
		eVtxTrk = USE_ALL_TRACKS;
		eNtrk = 3;
		eVTX = NULL;
		MakeGUI();
	}
	
	void MakeGUI();
	void DoVertexing(EdbEDATrackSet *set = NULL);
	void DoVertexingButton();
	void SetVertices();
	void Redraw();
	void ApplyParams();
	
	void Clear();
	
	ClassDef(EdbEDAVertexTab,1) // vertexing control tab
};



class EdbEDAMCTab{
	// Draw MC data set.
	private:
	
	bool eLoaded;
	
	EdbEDATrackSet *eSet;
	EdbDataProc *eProc;
	EdbPVRec *ePVR;
	TGNumberEntry *fMCEvt;
	
	public:
	EdbEDAMCTab() : eLoaded(kFALSE), eProc(0){ 
		MakeGUI();
	}
	
	void MakeGUI();
	void ShowMCEvt();
	void ShowMCEvtPP();
	void ShowMCEvtMM();
	void Draw(int iMCEvt);
};

class EdbEDAPredTab{
	
	private:
	TGNumberEntry *fIplPropagateTo;
	TGTextEntry   *fManChkFile;
	int IplPropagateTo;
	char manchkfile[50];

	int pred_use_angle;
	int pred_ipl_from;
	int pred_ipl_to;
	int pred_run;
	int pred_dx;
	int pred_dy;

	TGCheckButton *fUseAngle;
	TGNumberEntry *fIplFrom;
	TGNumberEntry *fIplTo;
	TGNumberEntryField *fPredRun;
	TGNumberEntryField *fVolumeRun;
	TGNumberEntryField *fVolumeDX;
	TGNumberEntryField *fVolumeDY;
	TGNumberEntryField *fTx;
	TGNumberEntryField *fTy;

	TObjArray *preds;

	public:
	EdbEDAPredTab():
		IplPropagateTo(0),
		pred_use_angle(0),
		pred_ipl_from(0),
		pred_ipl_to(0),
		pred_run(2000),
		pred_dx(3000),
		pred_dy(3000){
		preds = new TObjArray();
		MakeGUI();
	}
	
	void WritePred();
	void MakeManChkPred();
	void MakePredScanPred();
	void MakeManChkPredGivenAngleFromVertex(int id = -999, double ax = 999, double ay = 999);
	void RemoveFile();	
	void SetManChkFile(Long_t ipl);
	void MakeGUI();

};

class EdbEDAOperationTab {
	private:
	TEveBrowser * browser;
	TGMainFrame * frame;
	TGNumberEntryField *fRun;
	
	EdbTrackP *eT0org, *eT1org, *eT0p, *eT1p;       // store values for Redo
	EdbEDATrackSet *eSet;
	int eLastOperation;
	enum{ kNone, kConnect, kDisconnect, kRemoveSegment, kAttarchToVertex};
	
	public:
	EdbEDAOperationTab(void);
	void ConnectTracks(EdbTrackP *t0, EdbTrackP *t1);
	void ConnectTracksButton(void);
	void RemoveSegment(void);
	void DisconnectTracks(void);
	void Redo(void);
	void DumpTracksButton(void);
	void DumpTrack(EdbTrackP *t, FILE *fp=NULL, int run=99, int plate_coordinate=1);
	
	void AttachToVertex(EdbVertex *v, TObjArray *tracks);
	void AttachToVertexButton();
	void MoveToTrackSetButton();
	
};



class EdbEDATrackSetList: public TGTransientFrame {
	
	private:
	TGVerticalFrame *fVerticalFrame;
	TGListBox *fListBox;
	TObjArray *fTrackSets;
	int *fSelected;
	public:
	EdbEDATrackSetList(TObjArray *tracksets, int *iselected, char *message=NULL);
	~EdbEDATrackSetList();
	void SetSelected();
	
	
	ClassDef(EdbEDATrackSetList, 0) // TrackSet List
};






#endif // __EDA_TABS_H__
