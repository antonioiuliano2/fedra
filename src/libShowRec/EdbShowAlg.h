#ifndef ROOT_EdbShowAlg
#define ROOT_EdbShowAlg

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TBenchmark.h"
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
//___   Declaration of EdbShowAlg Class:
//___    Following structure is foreseen: The (inherited) class instances of
//___    EdbShowAlg DO the reconstruction. They DONT manage the data.
//___    Following the principle "separate algorithms and data" (like ATHENA,
//___    the ATLAS framework). Therefore the  EdbPVRec Object, the Initiator
//___    Base Tracks and the shower arrays are only handed over either as
//___    reference or as pointers. Saving/manipulating/preparing of these
//___    objects will only be done in the EdbShowRec class instances!
//___
//______________________________________________________________________________






//______________________________________________________________________________

class EdbShowAlg : public TObject {

private:
// nothing...

protected:

// A algorithm here should not have more than 10 different parameters, otherwise we
// will have to use TObjArray implementations.
    TString      eAlgName;
    Int_t        eAlgValue;
    Int_t        eParaN;
    Float_t      eParaValue[10];
    TString      eParaString[10];
    Int_t     eActualAlgParametersetNr; // Used when more sets of same algorithm


// Pointer copies from class EdbShowRec to be used for reconstruction of showers
// EdbPVRec object:
    EdbPVRec*          eAli;
    Int_t              eAliNpat;
// TObjArray storing Initiator Basetracks:
    TObjArray*         eInBTArray;
    Int_t              eInBTArrayN;
// Variables describing plate number cuts.
    Int_t              eFirstPlate_eAliPID;
    Int_t              eLastPlate_eAliPID;
    Int_t              eMiddlePlate_eAliPID;
    Int_t              eNumberPlate_eAliPID;
// TObjArray storing Reconstructed Showers:
    TObjArray*         eRecoShowerArray;
    Int_t              eRecoShowerArrayN;

// Transformed (smaller) EdbPVRec object:
    EdbPVRec*          eAli_Sub;
    Int_t              eAli_SubNpat;
// Flag wheter to use the small EdbPvrec Object or not. Using small object, this will not be deleted ue to
// pointer problems. See in implementation of Transform() for details.
    Int_t               eUseAliSub;


// In constructor this is created on heap...
    EdbShowerP*         eRecoShower;

// Reset All Default Variables:
    void         Set0();


public:

    EdbShowAlg();
    EdbShowAlg(TString AlgName, Int_t AlgValue);
//   EdbShowAlg(TString AlgName, Int_t AlgValue, Float_t* ParaValue[10], TString ParaString[10]);
//   EdbShowAlg(EdbPVRec* gAli, TObjArray* InBTArray);


    virtual ~EdbShowAlg();          // virtual constructor due to inherited class


// Hand over  eAli  eInBTArray  from EdbShowerRec
    inline void         SetEdbPVRec( EdbPVRec* Ali )          {
        eAli = Ali;
        eAliNpat=eAli->Npatterns();
    }
    inline void         SetInBTArray( TObjArray* InBTArray ) {
        eInBTArray = InBTArray;
        eInBTArrayN=eInBTArray->GetEntries();
    }
    inline void         SetEdbPVRecPIDNumbers(Int_t FirstPlate_eAliPID, Int_t LastPlate_eAliPID, Int_t MiddlePlate_eAliPID, Int_t NumberPlate_eAliPID) {
        eFirstPlate_eAliPID=FirstPlate_eAliPID;
        eLastPlate_eAliPID=LastPlate_eAliPID;
        eMiddlePlate_eAliPID=MiddlePlate_eAliPID;
        eNumberPlate_eAliPID=NumberPlate_eAliPID;
    }
    inline void         SetRecoShowerArray(TObjArray* RecoShowerArray) {
        eRecoShowerArray=RecoShowerArray;
    }
    inline void         SetRecoShowerArrayN(Int_t RecoShowerArrayN) {
        eRecoShowerArrayN=RecoShowerArrayN;
    }
    inline void         AddRecoShowerArray(EdbShowerP* shower) {
        eRecoShowerArray->Add(shower);
        ++eRecoShowerArrayN;
    }


    inline void     SetActualAlgParameterset(Int_t ActualAlgParametersetNr) {
        eActualAlgParametersetNr=ActualAlgParametersetNr;
    }

    inline Int_t        GetAlgValue()  const {
        return eAlgValue;
    }
    inline TString      GetAlgName()  const {
        return eAlgName;
    }
    inline Int_t        GetRecoShowerArrayN()     const       {
        return eRecoShowerArrayN;
    }
    inline TObjArray*   GetRecoShowerArray()     const       {
        return eRecoShowerArray;
    }
    inline EdbShowerP*  GetShower(Int_t i)       const       {
        EdbShowerP* shower = (EdbShowerP*)eRecoShowerArray->At(i);
        return shower;
    }

    inline void         SetUseAliSub(Bool_t UseAliSub) {
        eUseAliSub=UseAliSub;
    }


    Double_t            DeltaR_WithPropagation(EdbSegP* s,EdbSegP* stest);
    Double_t            DeltaR_WithoutPropagation(EdbSegP* s,EdbSegP* stest);
    Double_t            DeltaR_NoPropagation(EdbSegP* s,EdbSegP* stest);

    Double_t            DeltaTheta(EdbSegP* s1,EdbSegP* s2);
    Double_t            DeltaThetaComponentwise(EdbSegP* s1,EdbSegP* s2);
    Double_t            DeltaThetaSingleAngles(EdbSegP* s1,EdbSegP* s2);
    Double_t            GetSpatialDist(EdbSegP* s1,EdbSegP* s2);
    Double_t            GetMinimumDist(EdbSegP* seg1,EdbSegP* seg2);


    void             SetParameters(Float_t* par);
//void                Transform_eAli(EdbSegP* InitiatorBT);
    void                Transform_eAli( EdbSegP* InitiatorBT, Float_t ExtractSize);
    Bool_t              IsInConeTube(EdbSegP* sTest, EdbSegP* sStart, Double_t CylinderRadius, Double_t ConeAngle);

    void 		UpdateShowerIDs();
    void		UpdateShowerMetaData();

    void Print();
    void PrintParameters();
    void PrintParametersShort();
    void PrintMore();
    void PrintAll();

    void Help();


// Main functions for using this ShowerAlgorithm Object.
// Structure is made similar to OpRelease, where
//  Initialize, Execute, Finalize
// give the three columns of the whole thing.
// Since these functions depend on the algorithm type they are made virtual
// and implemented in the inherited classes.
    virtual void Initialize();
    virtual void Execute();
    virtual void Finalize();

    ClassDef(EdbShowAlg,1);         // Root Class Definition for my Objects
};


//______________________________________________________________________________

class EdbShowAlg_SA : public EdbShowAlg {

// S_imple A_lg, just collecting every BT which belongs to same MC Event as InBT

public:

    EdbShowAlg_SA();
    virtual ~EdbShowAlg_SA();          // virtual constructor due to inherited class

    void Init();

// Main functions for using this ShowerAlgorithm Object.
// Structure is made similar to OpRelease, where
//  Initialize, Execute, Finalize
// give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_SA,1);         // Root Class Definition for my Objects
};



//______________________________________________________________________________

class EdbShowAlg_CA : public EdbShowAlg {

// C_one A_dvanced Alg

private:
    Bool_t    GetConeTubeDistanceToInBT(EdbSegP* sa, EdbSegP* s, Double_t CylinderRadius, Double_t ConeAngle);
    Bool_t    FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbShowerP* shower);

public:

    EdbShowAlg_CA();
    virtual ~EdbShowAlg_CA();          // virtual constructor due to inherited class

// Main functions for using this ShowerAlgorithm Object.
// Structure is made similar to OpRelease, where
//  Initialize, Execute, Finalize
// give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_CA,1);         // Root Class Definition for my Objects
};




//______________________________________________________________________________

class EdbShowAlg_OI : public EdbShowAlg {

// O_fficial I_mplementation Alg. Try of the "libShower recdown" algorithm to keep backward compability.

private:
    Bool_t    FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbShowerP* shower);

public:

    EdbShowAlg_OI();
    virtual ~EdbShowAlg_OI();          // virtual constructor due to inherited class

// Main functions for using this ShowerAlgorithm Object.
// Structure is made similar to OpRelease, where
//  Initialize, Execute, Finalize
// give the three columns of the whole thing.
    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_OI,1);         // Root Class Definition for my Objects
};


//______________________________________________________________________________

class EdbShowAlg_RC : public EdbShowAlg {

// R_ecursive C_one algorithm.

private:
    Bool_t    FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbShowerP* shower);

public:
    EdbShowAlg_RC();
    virtual ~EdbShowAlg_RC();          // virtual constructor due to inherited class

    Bool_t   GetConeOrTubeDistanceToBTOfShowerArray(EdbSegP* sa, EdbSegP* InBT, EdbShowerP* shower, Double_t CylinderRadius, Double_t ConeAngle);

    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_RC,1);         // Root Class Definition for my Objects
};


//______________________________________________________________________________

class EdbShowAlg_BW : public EdbShowAlg {

    // B_ack_W_ard algorithm.

private:
    Bool_t    DummyBWFunction();

public:
    EdbShowAlg_BW();
    virtual ~EdbShowAlg_BW();          // virtual constructor due to inherited class

    void Initialize();
    void Execute();
    void Finalize();

    ClassDef(EdbShowAlg_BW,1);         // Root Class Definition for my Objects
};

//______________________________________________________________________________

/*
TO BE IMPLEMENTED ALGORITHMS IF TIME (and needed, if not available from "showrec" standalone program):
0 	CT
1 	CL
2 	CA
3 	NN
4 	OI
5 	SA
6 	TC
7 	RC
8 	BW
9 	AG
10 	GS
11	N3 (NewNeuralNet)
*/
//______________________________________________________________________________


#endif /* ROOT_EdbShowAlg */
