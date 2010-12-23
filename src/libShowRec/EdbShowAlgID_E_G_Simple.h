#ifndef ROOT_EdbShowAlgID_E_G_Simple
#define ROOT_EdbShowAlgID_E_G_Simple

#include "TROOT.h"
#include "TFile.h"
#include "TF1.h"
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
//___			Declaration of EdbShowAlgID_E_G_Simple Class:
//______________________________________________________________________________


//______________________________________________________________________________

class EdbShowAlgID_E_G_Simple : public TObject {
  
  private:
		
    // TObjArray storing Reconstructed Showers:
    TObjArray*         eRecoShowerArray;
    Int_t              eRecoShowerArrayN;// nothing...
    Int_t		eParaName;

    // Variables related for calculation Issues
    Float_t		eCalibrationOffset;
    Float_t		eCalibrationSlope;
    
    
    TString		eWeightFileString;
    
    protected:
		
      // Reset All Default Variables:
	void 								Set0();

		
  public:
        
    EdbShowAlgID_E_G_Simple();
    EdbShowAlgID_E_G_Simple(EdbShowerP* shower);
		EdbShowAlgID_E_G_Simple(TObjArray* RecoShowerArray);
		
		virtual ~EdbShowAlgID_E_G_Simple();          // virtual constructor due to inherited class
        
		// Hand over  eAli  eInBTArray  from EdbShowerRec
    inline void         SetRecoShowerArray(TObjArray* RecoShowerArray) { eRecoShowerArray=RecoShowerArray; eRecoShowerArrayN=eRecoShowerArray->GetEntriesFast(); }
    inline void         SetRecoShowerArrayN(Int_t RecoShowerArrayN) { eRecoShowerArrayN=RecoShowerArrayN; }
		
    inline Int_t        GetRecoShowerArrayN()     const       { return eRecoShowerArrayN; }
    

    inline void SetCalibrationOffset(Float_t CalibrationOffset) {eCalibrationOffset=CalibrationOffset;}
    inline void SetCalibrationSlope(Float_t CalibrationSlope) {eCalibrationSlope=CalibrationSlope;}
    
    void SetWeightFileString(TString weightstring);
    

    void FindClosestEfficiencyParametrization(Double_t TestAngle,Double_t ReferenceEff);

		void Execute();

    void Print();
    ClassDef(EdbShowAlgID_E_G_Simple,1);         // Root Class Definition for my Objects
};

//______________________________________________________________________________

#endif /* EdbShowAlgID_E_G_Simple */
