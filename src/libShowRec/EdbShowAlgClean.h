#ifndef ROOT_EdbShowAlgClean
#define ROOT_EdbShowAlgClean

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "TMultiLayerPerceptron.h"
#include "TMLPAnalyzer.h"
#include "TCanvas.h"
#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbPattern.h"
#include "EdbShowerP.h"
using namespace std;

//______________________________________________________________________________
//___
//___			Declaration of EdbShowAlgClean Class:
//______________________________________________________________________________


//______________________________________________________________________________

class EdbShowAlgClean : public TObject {
  
  private:

    // TObjArray storing 	Source  (to be cleaned)
    TObjArray*         eSource;
    Int_t		eSourceClassType;
    // TObjArray storing 	Dirt  (to be cleaned from, can of course be identical to the source...)
    TObjArray*         eDirt;
    Int_t		eDirtClassType;
    // TObjArray storing 	Target  (which is cleaned)
    TObjArray*         eTarget;
    Int_t		eTargetClassType;
    
    //Numerator to tell which kind of cleaning should be done
    TArrayI*		eCleaningType; 
		
    // Flag if source/dirt/target is already set.
    Bool_t		eIsSet[3];
    
    
    
    // Init;
    void		Init();
    // Set0;
    void		Set0();
    
  public:
        
    EdbShowAlgClean();
    EdbShowAlgClean(TObjArray* Source);
    EdbShowAlgClean(TObjArray* Source,TObjArray* Dirt);
    EdbShowAlgClean(TObjArray* Source,TObjArray* Dirt,TObjArray* Target);
    

    inline TObjArray*   GetSource( )  const       { return eSource; }
    inline TObjArray*   GetDirt( )  const       { return eDirt; }
    inline TObjArray*   GetTarget( )  const       { return eTarget; }

		void SetSource(TObjArray* Source);
    void SetDirt(TObjArray* Dirt);
    void SetTarget(TObjArray* Target);

		void SetSource(EdbShowerP* SourceShower);
    void SetDirt(EdbTrackP* DirtTrack);
    void SetTarget(EdbShowerP* TargetShower);

		inline void SetCleaningType(Int_t CleaningType, Int_t Value) { eCleaningType[CleaningType]=Value; return; }
    
    virtual ~EdbShowAlgClean();          // virtual constructor due to inherited class
        
    void CheckClassType();
    void CheckClassTypeEntries();
    
    
    void RemovePassingDirtFromSource();  	// General source/dirt remove function. one cleaning step
    void RemoveStoppingDirtFromSource();  	// General source/dirt remove function. another cleaning step
    
    
    // Specialized source/dirt remove functions. 
    // To have them quickly there...
    void  SimpleRemoveTrackSegmentsFromShower(); 				// source = shower; dirt = track;
    Int_t SimpleRemoveTrackSegmentsFromShower(EdbShowerP* shower, EdbTrackP* track);
    void  SimpleRemoveDoubleBTViewOverlap();			  		// source = shower; dirt = shower;
    Int_t SimpleRemoveDoubleBTViewOverlap(EdbShowerP* shower);
		void  SimpleRemoveBGSegmentsFromShower();			  		// source = shower; dirt = shower;
    Int_t SimpleRemoveBGSegmentsFromShower(EdbShowerP* shower);
		
    
    void Execute();			// does the actual cleaning steps
    
    // Any Helper Functions:
    Bool_t CompareSegmentsByPosition(EdbSegP* s1,EdbSegP* s2, Int_t type);
    
    
    void Print();
    void Help();
    ClassDef(EdbShowAlgClean,1);         // Root Class Definition for my Objects
};

//______________________________________________________________________________

#endif /* EdbShowAlgClean */
