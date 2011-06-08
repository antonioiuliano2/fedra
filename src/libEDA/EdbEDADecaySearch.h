
#ifndef __EDA_DecaySearch_h__
#define __EDA_DecaySearch_h__

#include"EdbEDAUtil.h"
#include"EdbEDATrackSet.h"
using namespace EdbEDAUtil;

class EdbEDATrackP : public EdbTrackP {
	
	private:

	EdbTrackP *eOriginal;     // pointer to original track
	TString    eComment;      // comment
	EdbVertex *eVertexUp;     // pointer to upstream vertex
	EdbVertex *eVertexDown;   // pointer to downstream vertex
	EdbSegP   *eSegmentFirst; // if 1st segment come from different data-set to be used for IP calculation.
	TObjArray *eFriends;      // same tracks in defferent data-set
	EdbTrackP *eParent;       // pointer of Parent track.
	TObjArray *eDaughters;    // pointers of Daughter tracks.
	
	int eFlagDS;              // flag for Decay search
	int eClassification;      // result of DecaySearch procedure.
	int eTrackSearch;         // flag for Track search
	int eFlagKink;            // flag for Kink search
	int eKinkPlate;           // kink plate
	int eParticle;            // Particle ID, mu, e,,,,
	int eDarkness;            // Darkness
	int eCSTrack;             // CS information
	int eRun;                 // data-set id
	
	double eP;                // momentum
	double ePmin;             // momentum min
	double ePmax;             // momentum max
	
	double eIPUp;             // IP to upstream vertex
	double eIPDown;           // IP to downstream vertex

	public:
	
	EdbEDATrackP () : EdbTrackP(), eOriginal(0){ Init();}
	EdbEDATrackP (EdbTrackP *track) : EdbTrackP(*track), eOriginal(track) { Init();}
	EdbEDATrackP (EdbTrackP *track, EdbVertex *vtxup): EdbTrackP(*track), eOriginal(track){ 
		Init(); 
		SetVertexUp(vtxup);
	}
	void Init(){
		eVertexUp = eVertexDown = NULL;
		eSegmentFirst = NULL;
		eParent       = NULL;
		eFriends   = new TObjArray;
		eDaughters = new TObjArray;
		eFlagDS = eFlagKink = eKinkPlate = eParticle = eDarkness = eCSTrack = eRun = eClassification = -1;
		eTrackSearch = 0;
		eP = ePmin = ePmax = -1;
		eIPUp = eIPDown = -1;

		SetFlagsAuto();
	}
	
	void SetFlagsAuto();
	
	void SetComment(char *comment) { eComment = comment;}
	const char *Comment(){ return eComment.Data();}
	
	void SetVertexUp(EdbVertex *v) { eVertexUp = v;}
	EdbVertex *VertexUp() { return eVertexUp;}
	void SetVertexDown(EdbVertex *v) { eVertexDown = v; eIPDown = CalcIP(SegmentFirst(), eVertexDown);}
	EdbVertex *VertexDown() { return eVertexDown;}

	void SetSegmentFirst(EdbSegP *s) { eSegmentFirst = s;}
	EdbSegP * SegmentFirst(){ return eSegmentFirst ? eSegmentFirst : EdbTrackP::GetSegmentFirst();}

	void AddFriend(EdbTrackP *t) { if(eFriends->FindObject(t)==NULL) eFriends->Add(t);}
	int NFriends() { return eFriends->GetEntries();}
	EdbTrackP *GetFriend(int i){ return (EdbTrackP *) eFriends->At(i);}
	TObjArray *GetFriends() { return eFriends;}
	
	void AddDaughter(EdbTrackP *t) { if(eDaughters->FindObject(t)==NULL) eDaughters->Add(t);}
	int NDaughters() { return eDaughters->GetEntries();}
	EdbTrackP *GetDaughter(int i){ return (EdbTrackP *) eDaughters->At(i);}
	TObjArray *GetDaughters() { return eDaughters;}
	
	void SetFlagDS(int v){ eFlagDS = v;}
	int  FlagDS() { return eFlagDS;}
	void SetTrackSearch(int v){ eTrackSearch = v;}
	int  TrackSearch() { return eTrackSearch;}
	void SetFlagKink(int v){ eFlagKink = v;}
	int  FlagKink() { return eFlagKink;}
	void SetKinkPlate(int v){ eKinkPlate = v;}
	int  KinkPlate() { return eKinkPlate;}
	void SetParticle(int v){ eParticle = v;}
	int  Particle() { return eParticle;}
	void SetDarkness(int v){ eDarkness = v;}
	int  Darkness() { return eDarkness;}
	void SetCSTrack(int v){ eCSTrack = v;}
	int  CSTrack() { return eCSTrack;}
	void SetRun(int v){ eRun = v;}
	int  Run() { return eRun;}
	void SetClassification(int v){ eClassification = v;}
	int  Classification() { return eClassification;}
	
	void SetIPUp  ( double ip) { eIPUp   = ip;}
	void SetIPDown( double ip) { eIPDown = ip;}
	double IPUp()  { return eIPUp;}
	double IPDown(){ return eIPDown;}
	
	void SetP(double p, double pmin, double pmax){ eP=p; ePmin=pmin; ePmax=pmax;}
	double P()   { return eP;}
	double Pmin(){ return ePmin;}
	double Pmax(){ return ePmax;}
	
	EdbTrackP *GetOriginal(){ return eOriginal;}
	EdbTrackP *GetTrack(){ return eOriginal;}
	void Print();
	
	ClassDef(EdbEDATrackP, 0) // Tracks with DecaySearch information.
};

class EdbEDADecayVertex : public EdbVertex {
	
	private:
	
	EdbEDATrackP *eParent;
	EdbEDATrackP *ePartner;
	TObjArray *eDaughters;
	EdbVertex *ePrimaryVertex;
	void AddTrack(EdbEDATrackP *t);
	int type;
	
	public:
	
	enum TopologyType{ kShort, kLong};
	
	EdbEDADecayVertex(EdbVertex *v2ry, EdbVertex *v1ry, EdbEDATrackP *parent=NULL, EdbEDATrackP *daugter1=NULL, EdbEDATrackP *daugheter2=NULL, EdbEDATrackP *daughter3=NULL, EdbEDATrackP *daughter4=NULL, EdbEDATrackP *daughter5=NULL);
	EdbEDADecayVertex(EdbVertex *v2ry, EdbVertex *v1ry, EdbEDATrackP *parent, TObjArray *daughters);
	EdbEDADecayVertex():ePrimaryVertex(NULL), eParent(NULL), eDaughters(NULL){};
	~EdbEDADecayVertex();
	
	void SetParent( EdbEDATrackP *parent);
	void SetDaughter( EdbEDATrackP *daughter);
	void SetDaughters( TObjArray *daughters);
	void SetPartner( EdbEDATrackP *partner) { ePartner = partner;}
	void SetPrimaryVertex( EdbVertex *v){ ePrimaryVertex = v;}
	
	EdbTrackP *GetParent() { return eParent?eParent->GetOriginal():NULL;}
	EdbTrackP *GetDaughter(int i=0) { return eDaughters ? ((EdbEDATrackP *) eDaughters->At(i))->GetOriginal() : NULL;}
	EdbTrackP *GetPartner() { return ePartner?ePartner->GetOriginal():NULL;}
	int NDaughters(){ return eDaughters? eDaughters->GetEntries() : 0;}
	EdbVertex *GetPrimaryVertex(){ return ePrimaryVertex;}
	
	
	ClassDef(EdbEDADecayVertex, 0) // General decay vertex.
	
};

class EdbEDASmallKink : public EdbVertex{
	public:

	EdbTrackP *eTrack;
	EdbSegP *eS1;
	EdbSegP *eS2;
	double eDTT;
	double eDTL;
	double eDXT;
	double eDXL;
	int eNdau;
	double eP;
	double ePmin;
	double ePmax;
	double ePT;
	double eRMST;
	double eRMSL;
	
	EdbTrackP *Track(){return eTrack;}
	EdbSegP *S1(){return eS1;}
	EdbSegP *S2(){return eS2;}
	double DTT(){return eDTT;}
	double DTL(){return eDTL;}
	double DXT(){return eDXT;}
	double DXL(){return eDXL;}
	int Ndau(){return eNdau;}
	double P(){return eP;}
	double Pmin(){return ePmin;}
	double Pmax(){return ePmax;}
	double PT(){return ePT;}
	double RMST(){return eRMST;}
	double RMSL(){return eRMSL;}
	int IPL1();
	int IPL2();


	EdbEDASmallKink( EdbVertex *v, EdbTrackP *track, EdbSegP *s1, EdbSegP *s2, double dTT, double dTL, double dXT, double dXL, 
			int ndau, double p, double pmin, double pmax, double pt, double rmst, double rmsl): 
			eTrack(track), eS1(s1),  eS2(s2), eDTT(dTT), eDTL(dTL), eDXT(dXT), eDXL(dXL), 
			eNdau(ndau), eP(p), ePmin(pmin), ePmax(pmax),ePT(pt), eRMST(rmst), eRMSL(rmsl)
		{
			SetXYZ(v->X(),v->Y(),v->Z());
		}


	void Draw(Option_t *option="");
	ClassDef(EdbEDASmallKink,0) //test
};

class EdbEDADecaySearch{
	// Decay Search class

	public:
	
	enum{
		kParentSearch       =0x01,
		kBaseTrackSearch    =0x02,
		kDaughterSearch     =0x04
	};
	
	int eDSVer;
	
	EdbVertex *eVertex;
	int eVertexPlatePID; // vertex plate. 1st stream plate from vertex
	
	// Found tracks
	TObjArray *eTracks;
	TObjArray *eSegments;
	TObjArray *eVertices;
	EdbPVRec  *ePVR;
	EdbEDATrackSet *eSet;
	
	int    eTSDau;      // Daughter track search
	int    eTSDauNseg;  // Nseg cut for Daughter tracks
	double eTSDauPH;    // PH cut   for Daughter tracks
	TH1F  *eTSDauIPHist1;   // Histgram for IP function along dz.
	
	int    eMT2seg;     // Microtrack search for 2 segment.
	int    eMTAll;      // Microtrack search to fill holes and upstream, downstream.
	
	int    eTSDau2;     // Daughter track search. 2nd (after microtrack search)
	int    eTSDauNseg2; // Nseg cut for Daughter tracks. 2nd.
	TH1F  *eTSDauIPHist2;   // Histgram for IP function along dz.
	
	int    eSmallKink;  // Small-Kink search
	int    eSmallKinkNpl;
	int    eSmallKinkRthreshold;
	
	
	int    eTSPar;      // Parent track search
	int    eTSParPlate; // nPlate to be searched for Parent tracks. eTSParPlate==1:vertex-plate only. eTSParPlate==2:vertex-plate+1plate...
	int    eTSParNseg;  // Nseg cut for Parent tracks
	double eTSParIP;    // IP cut   for Parent tracks
	double eTSParPH;    // PH cut   for Parent tracks
	
	
	int    eBT;          // Basetrack search
	int    eBTPlateUp;   // nplates upstream or Basetrack search
	int    eBTPlateDown; // nplates downstream for Basetrack Search
	double eBTIP;        // IP cut for Basetrack
	double eBTPH;        // PH cut for Basetrack

	int    eVtxUp;       // Search Upstream vertex if the ntrk of selected vertex <=3.
	int    eVtxUpDZ;     // dz from selected vertex and upstream vertex to be searched.
	int    eVtxUpIP;     // Ip cut from the tracks belong to the selected vertex.
	
	
	// Kink Search
	double    eKinkSigma;
	TObjArray *eKinks;
	TObjArray *eDecayVertices;
	
	EdbEDADecaySearch(int DSversion=2):
		eDSVer     (DSversion),
		eVertex    (NULL),
		eTracks    (new TObjArray),
		eSegments  (new TObjArray),
		eVertices  (new TObjArray),
		eSet       (NULL),
		
		eTSDau      (1),
		eTSDauNseg  (2),
		eTSDauPH    (17),
		
		eMT2seg     (0),
		eMTAll      (1),
		
		eTSDau2     (1),
		eTSDauNseg2 (3),
		
		eSmallKink    (0),
		eSmallKinkNpl (5),
		eSmallKinkRthreshold(3),
		
		eTSPar     (0),
		eTSParPlate(2),
		eTSParNseg (1),
		eTSParIP   (20),
		eTSParPH   (17),
		
		eBT            (0),
		eBTPlateUp     (0),
		eBTPlateDown   (2),
		eBTIP          (20),
		eBTPH          (17),
		
		eVtxUp   (0),
		eVtxUpDZ (3000),
		eVtxUpIP (500),
		
		eKinkSigma(3.),
		eKinks (new TObjArray),
		eDecayVertices(new TObjArray)
		
		{
			
			float ipcutdau_lt_1 = eDSVer==1 ? 500 : 300;
			float ipcutdau_gt_1 = eDSVer==1 ? 800 : 500;
			
			eTSDauIPHist1 = new TH1F ("eTSDauIPHist1","IP cut function along dZ, 1st.", 121, -100, 12000);
			// Fill function
			TH1F *h = eTSDauIPHist1;
			h->SetFillColor(kBlue);
			h->SetXTitle("dZ (#mum)");
			h->SetYTitle("IP cut (#mum)");
			for(int i=1;i<=100;i++){
				if(0<=h->GetBinLowEdge(i)&&h->GetBinLowEdge(i)<1000) h->SetBinContent(i, ipcutdau_lt_1);
				else if(1000<=h->GetBinLowEdge(i)&&h->GetBinLowEdge(i)<11700) h->SetBinContent(i, ipcutdau_gt_1);
				else if(11700<=h->GetBinLowEdge(i)) h->SetBinContent(i,0);
			}
			eTSDauIPHist2 = new TH1F ("eTSDauIPHist2","IP cut function along dZ, 2nd", 121, -100, 12000);
			// Fill function
			h = eTSDauIPHist2;
			h->SetFillColor(kBlue);
			h->SetXTitle("dZ (#mum)");
			h->SetYTitle("IP cut (#mum)");
			for(int i=1;i<=100;i++){
				if(0<=h->GetBinLowEdge(i)&&h->GetBinLowEdge(i)<1000) h->SetBinContent(i, ipcutdau_lt_1);
				else if(1000<=h->GetBinLowEdge(i)&&h->GetBinLowEdge(i)<6000) h->SetBinContent(i, ipcutdau_gt_1);
				else if(6000<=h->GetBinLowEdge(i)) h->SetBinContent(i,0);
			}
		}
	virtual ~EdbEDADecaySearch(){}
	
	EdbVertex *GetPrimaryVertex(){ return eVertex;}
	EdbVertex *FindPrimaryVertex();
	void SetVertex (EdbVertex *v);

	TObjArray * TSDaughterTracks (TObjArray *base);
	double GetTSDauIP1(double dz);

	void MTSearch2seg(TObjArray *tracks);
	void MTSearchAll(TObjArray *tracks);

	void TSDaughterTracks2 ();
	double GetTSDauIP2(double dz);

	void SmallKinkSearch();
	void KinkSearch();
	float GetIPCut(EdbVertex *v, EdbSegP *s);
	void ShortDecaySearch();

	TObjArray * TSParentTracks   (TObjArray *base);
	TObjArray * TSBaseTracks(int ipl);
	TObjArray * FindUpstreamVertices();

	TObjArray * DoSearch();

	EdbEDATrackP *FindTrack(EdbTrackP *t){
		for(int i=0;i<Ntracks();i++){
			if(GetTrack(i)->ID()==t->ID()) return GetTrack(i);
		}
		return NULL;
	}
	
	int Ntracks(){ return eTracks->GetEntries();}
	EdbEDATrackP *GetTrack(int i) { return (EdbEDATrackP *) eTracks->At(i);}
	int NDecayVertices(){ return eDecayVertices->GetEntries();}
	EdbEDADecayVertex *GetDecayVertex(int i) { return (EdbEDADecayVertex *)eDecayVertices->At(i);}
	
	void SetPVR(EdbPVRec *pvr) { ePVR = pvr;}
	EdbPVRec *GetPVR() { return ePVR;}
	
	void SetTrackSet(EdbEDATrackSet *set){ 
		eSet = set;
		SetPVR(set->GetPVRec());
	}
	EdbEDATrackSet *GetTrackSet(){ return eSet;}
	
	void SetIPHist1(TH1F *h) { eTSDauIPHist1=h;}
	void SetIPHist2(TH1F *h) { eTSDauIPHist2=h;}
	
	void SetTracks(TObjArray *array) {eTracks=array;}
	
	TObjArray *CheckInTrackKinks(TObjArray *tracks = NULL) { 
		if(NULL==tracks) tracks=eTracks; 
		for(int i=0;i<tracks->GetEntries();i++) {
			CheckInTrackKink((EdbTrackP *) tracks->At(i)); 
		}
		return eKinks;
	}
	TObjArray *CheckInTrackKink(EdbTrackP *trk);
	TObjArray *GetKinks(void){ return eKinks;}
	
	void SetBTSearch( int do_search=1, int npl_up=0, int npl_down=2, float ipcut=20, float phcut=17){
		eBT = do_search; eBTPlateUp = npl_up; eBTPlateDown = npl_down; eBTIP = ipcut; eBTPH = phcut;}
	void SetParentSearch( int do_search=1, int npl_down=2, float ipcut=20, float phcut=17){
		eTSPar = do_search; eTSParPlate = npl_down; eTSParIP = ipcut; eTSParPH = phcut;}
	
	//void MicroTrackSearch(EdbSegP *s, int ipl); 
	// moved to TrackSet.
	void PrintTracks();
	void PrintRunTracking(){
		if(eSet) eSet->eRunTracking.Print();
		else printf("No EdbEDATrackSet is set\n");
	}
	EdbVertex * MakeFakeVertex(EdbTrackP *t, double dz=1150);
	ClassDef(EdbEDADecaySearch, 0) // Class for pick up tracks 
};


class EdbEDADecaySearchTab: public EdbEDADecaySearch{
	private:
	TGCheckButton *fTSDau;
	TGNumberEntry *fTSDauPH;
	TGNumberEntry *fTSDauNseg;
	
	TGCheckButton *fMT2seg;
	TGCheckButton *fMTAll;
	
	TGCheckButton *fTSDau2;
	TGNumberEntry *fTSDauNseg2;

	TGCheckButton *fSmallKink;
	TGNumberEntry *fSmallKinkNpl;
	TGNumberEntry *fSmallKinkRthreshold;
	

	TGCheckButton *fTSPar;
	TGNumberEntry *fTSParIP;
	TGNumberEntry *fTSParPH;
	TGNumberEntry *fTSParNseg;
	TGNumberEntry *fTSParPlate;
	
	// Basetrack search
	TGCheckButton *fBT;
	TGNumberEntry *fBTPlateUp;   
	TGNumberEntry *fBTPlateDown; 
	TGNumberEntry *fBTIP;        
	TGNumberEntry *fBTPH;        
	
	// Microtrack search
	TGNumberEntry *fMTIpl;
	
	TGMainFrame *eParamWindow;
	TGMainFrame *eTab;
	
	public:
	EdbEDADecaySearchTab(int DSversion=2) : EdbEDADecaySearch(DSversion), eParamWindow(NULL){ 
		MakeGUI();
	}
	~EdbEDADecaySearchTab(){ 
		if(eParamWindow) eParamWindow->CloseWindow();
	}
	void MakeGUI();
	void ApplyParams();
	void CloseParamWindow();
	void DoSearchButton();
	void DoMicroTrackSearchButton();
	void Button3(){ printf("Button3\n");}

	void FindUpstreamVertex();

	void MakeParamWindow();
	void MakeFakeVertexButton();
	ClassDef(EdbEDADecaySearchTab, 0) // Interface for decay search
};





class EdbEDAFeedbackEntryT : public TNamed{
	
	private:
	EdbTrackP *eTrack;
	EdbTrackP *eTrackMom;
	EdbTrackP *eTrackSeg;
	EdbSegP   *eSeg;
	
	TGTabElement *eTabElement;
	TGCompositeFrame *eCompositeFrame;
	TGTextEntry *eTextEntry;
	TGTextEntry *eTextEntryFS;
	TGTextEntry *eTextEntryMom;
	
	public:
	EdbEDAFeedbackEntryT(EdbTrackP *t, TGTab * parent);
	~EdbEDAFeedbackEntryT(){
		if(eCompositeFrame) delete eCompositeFrame;
		if(eTextEntry)      delete eTextEntry;
	}
	void HilightElement();
	void ReplaceEntry();
	void UpdateGUI();
	ClassDef(EdbEDAFeedbackEntryT, 0) // class for feedback entry
};

class EdbEDAFeedbackEntryV : public TNamed{
	
	private:
	EdbVertex *eVertex;
	TGTabElement *eTabElement;
	TGCompositeFrame *eCompositeFrame;
	TGTextEntry *eTextEntry;
	
	public:
	EdbEDAFeedbackEntryV(EdbVertex *v, TGTab * parent);
	~EdbEDAFeedbackEntryV(){
		if(eCompositeFrame) delete eCompositeFrame;
		if(eTextEntry)      delete eTextEntry;
	}
	void HilightElement();
	
	ClassDef(EdbEDAFeedbackEntryV, 0) // class for feedback entry
};



class EdbEDAFeedbackEditor{
	private:
	TGTextButton *eButton1, *eButton2, *eButton3;
	
	TGCompositeFrame *fMainFrame;
	TGTab * fTabTracks;
	TGTab * fTabVertices;
	TObjArray *fTrackEntries;
	TObjArray *fVertexEntries;
	
	public:
	EdbEDAFeedbackEditor();
	~EdbEDAFeedbackEditor();
	void AddTracksButton();
	void AddVerticesButton();
	void HilightElementT(Int_t id);
	void HilightElementV(Int_t id);
	void HandleButtons(Int_t id);
	void RemoveTrackTab(Int_t id);
	void RemoveVertexTab(Int_t id);
};








































#endif // __EDA_DecaySearch_h__

