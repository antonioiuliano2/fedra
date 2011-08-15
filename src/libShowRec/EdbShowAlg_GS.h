#ifndef ROOT_EdbShowAlg_GS
#define ROOT_EdbShowAlg_GS

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
#include "TMultiLayerPerceptron.h"
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
//___     Declaration of EdbShowAlg Class:
//___       Following structure is foreseen: The (inherited) class instances of
//___       EdbShowAlg DO the reconstruction. They DONT manage the data.
//___       Following the principle "separate algorithms and data" (like ATHENA,
//___       the ATLAS framework). Therefore the  EdbPVRec Object, the Initiator
//___       Base Tracks and the shower arrays are only handed over either as
//___       reference or as pointers. Saving/manipulating/preparing of these
//___       objects will only be done in the EdbShowRec class instances!
//___
//______________________________________________________________________________



//______________________________________________________________________________

class EdbShowAlg_GS : public EdbShowAlg {


private:

    // TObjArray storing Initiator Vertices:
    TObjArray*         eInVtxArray;
    Int_t              eInVtxArrayN;

    TH1F* h_GammaChi2;

public:

    EdbShowAlg_GS();
    EdbShowAlg_GS(EdbPVRec* ali);
    virtual ~EdbShowAlg_GS();          // virtual constructor due to inherited class

    void    Init();

    inline void         SetInVtxArray( TObjArray* InVtxArray ) {
        eInVtxArray = InVtxArray;
        eInVtxArrayN = eInVtxArray->GetEntries();
        cout << eInVtxArrayN << "  entries set"<<endl;
    }
    void         	    SetInVtx( EdbVertex* vtx );

    inline Int_t        GetInVtxArrayN()     	const       {
        return eInVtxArrayN;
    }
    inline TObjArray*   GetInVtxArray()     	const       {
        return eInVtxArray;
    }


    // Helper Functions for this class:
    void 		  Convert_InVtxArray_To_InBTArray();
    void 			Convert_TracksArray_To_InBTArray();
    Bool_t		CheckPairDuplications(Int_t SegPID,Int_t SegID,Int_t Seg2PID,Int_t Seg2ID,TArrayI* SegmentPIDArray,TArrayI* SegmentIDArray,TArrayI* Segment2PIDArray,TArrayI* Segment2IDArray,int RecoShowerArrayN);
    Double_t 	CalcIP(EdbSegP *s, double x, double y, double z);
    Double_t 	CalcIP(EdbSegP *s, EdbVertex *v);
    Bool_t 		IsPossibleFakeDoublet(EdbSegP* s1,EdbSegP* s2);


    TObjArray* 	FindPairs(EdbSegP* InBT, EdbPVRec* eAli_Sub);
    TObjArray* 	CheckCleanPairs(EdbSegP* InBT, TObjArray* RecoShowerArrayFromFindPairs);

    // Main functions for using this ShowerAlgorithm Object.
    // Structure is made similar to OpRelease, where
    //  Initialize, Execute, Finalize
    // give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_GS,1);         // Root Class Definition for my Objects
};


#endif /* ROOT_EdbShowAlg_GS */
