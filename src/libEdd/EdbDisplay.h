#ifndef ROOT_EdbDisplay
#define ROOT_EdbDisplay

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TCanvas.h"
#include "TSlider.h"
#include "TView.h"
#include "TPolyLine3D.h"
#include "TClonesArray.h"
#include "TArrayF.h"
#include "TArrayI.h"
#include "EdbPattern.h"
#include "TButton.h"
#include "EdbAffine.h"


#include <TGClient.h>
#include <TGButton.h>


//_________________________________________________________________________
class EdbDisplay: public TObject {
private:
         TCanvas* fCanvas;
	 TButton* fPickButton;
	 TPad* fCutpad;
	 TPad* fImagepad;
         EdbAffine2D* fAff;
	 TView*   fView;
	 TPolyLine3D* pl;
	 TSlider  *fXSlider,*fYSlider,*fTXSlider,*fTYSlider;
	 Float_t cutX0,cutX1,cutY0,cutY1;
	 Float_t cutTX0,cutTX1,cutTY0,cutTY1;
	 Float_t cutW,cutCHI2low,cutCHI2high,cutN1,cutN2;
	 TClonesArray* pats;
	 TArrayI* colors;
	 TArrayF* DZs;
	 Int_t Npats;
	 Float_t vx0,vy0,vz0,vx1,vy1,vz1;
	 Int_t eNsegmax,eNsegmin,eNpieces;
	 Bool_t eTr_Co; // Tracks/Couples switch

public:
Bool_t MaxChi;
         EdbDisplay(){};
         EdbDisplay(const char *title, Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1);
    void Clears();
    void SetNsegmax(Int_t v){eNsegmax=v;};
    void SetNsegmin(Int_t v){eNsegmin=v;};
    void SetShowTracks(){eTr_Co=kTRUE;};
    void SetShowCouples(){eTr_Co=kFALSE;};
    void SetCuts(Float_t x0, Float_t x1 , Float_t y0, Float_t y1){cutX0=x0;cutX1=x1;cutY0=y0;cutY1=y1;}
    void SetAffine(Float_t a1, Float_t a2, Float_t a3, Float_t a4, Float_t ax, Float_t ay);
    void AddPattern(EdbPattern* pat, Int_t color);
    void AddPatternsVolume(EdbPatternsVolume* pat, Int_t colorU, Int_t colorD);
    void AddCouplesTree(TTree* tree,Float_t Zoffs=0, Float_t tx0=-1., Float_t tx1=1., Float_t ty0=-1., Float_t ty1=1.);
    void AddTracksTree(TTree* tree, Float_t Zoffs=0, Float_t tx0=-1., Float_t tx1=1., Float_t ty0=-1., Float_t ty1=1.);
//    void AddLinkedCouplesTree(TTree* tree, Float_t* plate, Float_t Zoffs=0, );
    void Refresh();
    virtual void ExecuteEvent(Int_t event, Int_t px, Int_t py);
	 ~EdbDisplay(){};

  ClassDef(EdbDisplay,1)
};



#endif /* ROOT_EdbDisplay */
