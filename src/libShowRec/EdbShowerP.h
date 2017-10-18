#ifndef ROOT_EdbShowerP
#define ROOT_EdbShowerP

#include "TROOT.h"
#include "TFile.h"
#include "TVector3.h"
#include "TIndexCell.h"
#include "TArrayF.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TBenchmark.h"
#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "EdbMomentumEstimator.h"
#include "EdbLog.h"
#include "EdbPattern.h"

class EdbShowerP;

using namespace std;



//______________________________________________________________________________

class EdbShowerP : public EdbSegP {

private:

    //-C- shower meta data (related to reconstruction) here:

    // To know nameStrings of Profile histograms:
    TString      eLongitudinalProfileName;
    TString      eTransversalProfileName;

    // To know which Algorithm and Parameters were used for this shower to be reconstructed:
    TString      eAlgName;
    Int_t        eAlgValue;
    Float_t      eParaValue[10];
    TString      eParaString[10];

    Bool_t        eParametrisationIsDone[10];   // number of parametrization which is already extracted or not.
    Bool_t        eExtraInfoIsDone[2];    // if Para_ExtraInfo has been read already.
    Bool_t        eMCInfoIsDone[2];    // if PGun or MCEvt Info has been read already.

    Short_t      eFirstPlate;       // FirstPlate is always to the starting of beam!
    Short_t      eLastPlate;        // LastPlate is always to the starting of beam!
    Short_t      eFirstPlatePID;    // PID of Plate containing the InitiatorBT
    Short_t      eLastPlatePID;      // PID of last plate which was used for reconstruction
    Short_t      eFirstPlateZ;      // Z of Plate containing the InitiatorBT
    Short_t      eLastPlateZ;        // Z of last plate which was used for reconstruction
    Float_t      eFirstZ;        // Z of Plate containing the InitiatorBT
    Float_t      eLastZ;    // Z of last plate which was used for reconstruction


    //-C- shower data (directly related to shower characteristics) here:

    //-C- For class backward compability to EdbTrackP ....
    TObjArray     *eS;              // array of segments which set up the shower
    Int_t         eNpl;             // number of plates passed throw
    Int_t         eN0;              // number of holes (if any)
    EdbTrackP     *eTrackAS;        //! track to which the shower start is attached to
    EdbVTA        *eVTAS;           //! vertex to which the shower start is attached to

    Float_t      eM;     // invariant mass of the particle (from EdbTrackP; to be filled with use of ID algo)
    Int_t        ePDG;   // particle ID from PDG (from EdbTrackP; to be filled with use of ID algo)

    //-C- For new class settings:
    Int_t         eNBT;             // Number of BT in the shower
    Int_t         eNBTMC;           // Number of BT in the shower, only MC>=0 events
    Int_t         eNBTMCFlag[99];   // NBT, only MC>=0, flagged with PDG Code but Internal array!! See corresponding fill function!
    Int_t         eNBTPlate[57];    // Number of BT in the shower per plate w.r.t to Plate of InBT (Plate of InBT===[0])
    Int_t         eNBTMCPlate[57];  // Number of BT in the shower, only MC>=0 events,w.r.t to Plate of InBT (Plate of InBT===[0])
    Int_t         eNBTMCFlagPlate[57][99];   // NBT, only MC>=0, flagged with PDG Code but Internal array!! See corresponding fill function!, w.r.t to Plate of InBT (!)
    Int_t         eN00;              // largest number of consecutive holes (if any)

    Float_t       ePurity;           // simple purity (eNBTMC/eNBT)
    Float_t       eEnergyMC;        // Energy, defined by P() of the Initiator BaseTrack  // this will be revised later!
    Int_t         eMC;              // MC number, defined by MCEvt() of the Initiator BaseTrack
    Float_t       eSphericity;      // Value to express spherical distribution around the ShowerAxisCenterGravity

    // Internal histograms to store binentries of longitudinal and transversal shower profiles
    // for exact definition how bins are defined see in the .cxx code implementation
    TH1F*          eProfileLongitudinal;
    TH1F*          eProfileTransversal;



    // shower Axis angle (tan theta)
    // see in the .cxx code how it is exactly calculated
    Float_t       eShowerAxisAngle;
    //  a basetrack with information of the shower axis and its direction
    EdbSegP*      eShowerAxisCenterGravityBT;


    // Histogram of all segments deviation: TX_showeraxis-TX_segment, and for TY
    Float_t	eShowerAngularDeviationTXDistribution_mean;
    Float_t	eShowerAngularDeviationTXDistribution_sigma;
    Float_t	eShowerAngularDeviationTYDistribution_mean;
    Float_t	eShowerAngularDeviationTYDistribution_sigma;


    //-C- shower data (extracted from shower characteristics) here:
    Short_t       eId;
    Short_t       ePDGId;
    Float_t       eEnergy;
    Float_t       eEnergyUncorrected;
    Float_t       eEnergyCorrected;

    //-C- Internal Helper Functions:
    Double_t       GetdR(EdbSegP* seg1,EdbSegP* seg2);
    Double_t       GetdMin(EdbSegP* seg1,EdbSegP* seg2);
    Double_t       GetdT(EdbSegP* seg1,EdbSegP* seg2);
    Int_t         GetLastFilledBin(TH1* hist);


public:

    // default constructor, empty shower
    EdbShowerP(int nseg=0);

    // default constructor, empty shower numbers for setting histogram names...
    EdbShowerP(int number1, int number2, int number3, int nseg=0);

    // virtual destructor due to inherited class
    virtual ~EdbShowerP();




    //-C- shower data: structures for the different parametrisations:
    //
    //                 made public to have access by other classes.
    //                 The abbreviations follow either the persons,
    //                 first time implementing them, or
    //                 the type of physics case they follow...
    //
    // Attention after declarating "struct" ..:
    // then after the "{}" brackets a semikolon is necessary here!

    struct Para_FJ {
        // Variables used for FJ ( "F_rederic J_uget" ) parametrisation
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Float_t   BT_deltaR_mean;
        Float_t   BT_deltaR_rms;
        Float_t   BT_deltaT_mean;
        Float_t   BT_deltaT_rms;
        Int_t     longprofile[57]; // not identical to the ProfileLongitudinalBincontent of Para_YY !!

        ClassDef (Para_FJ,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_LT {
        // Variables used for LT ( "L_ongitudinal & T_ransversal" ) parametrisation
        // Identical to "Variables used for FJ parametrisation" but with transprofile in addition
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Float_t   BT_deltaR_mean;
        Float_t   BT_deltaR_rms;
        Float_t   BT_deltaT_mean;
        Float_t   BT_deltaT_rms;
        Int_t   transprofile[8];
        Int_t   longprofile[57]; // not identical to the ProfileLongitudinalBincontent of Para_YY !!

        ClassDef (Para_LT,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_YC {
        // Variables used for YC ( "Y_van C_affari" ) like parametrisation
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Float_t   C1;
        Float_t   a1;
        Float_t   alpha;
        Float_t   nmax;

        ClassDef (Para_YC,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_JC {
        // Variables used for JC ( "J_uget C_affari" ) like parametrisation
        // YC part
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Float_t   C1;
        Float_t   a1;
        Float_t   alpha;
        Float_t   nmax;
        // FJ part
        //Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)   // already there from YC part.
        //Int_t     nseg;  // identical to <<eNBT>>       // already there from YC part.
        Float_t   BT_deltaR_mean;
        Float_t   BT_deltaR_rms;
        Float_t   BT_deltaT_mean;
        Float_t   BT_deltaT_rms;
        Int_t     longprofile[57]; // not identical to the ProfileLongitudinalBincontent of Para_YY !!

        ClassDef (Para_JC,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_XX {
        // Variables used for XX parametrisation
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Float_t   Mean_ProfileLongitudinal;
        Float_t   RMS_ProfileLongitudinal;
        Float_t   Max_ProfileLongitudinal;
        Float_t   Mean_ProfileTransversal;
        Float_t   RMS_ProfileTransversal;
        Float_t   Max_ProfileTransversal;
        Float_t   Last_ProfileLongitudinal;
        Float_t   Last_ProfileTransversal;

        ClassDef (Para_XX,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_YY {
        // Variables used for YY parametrisation
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Int_t     ProfileLongitudinalBincontent[57];
        Int_t     ProfileTransversalBincontent[8];

        ClassDef (Para_YY,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_PP {
        // Variables used for PP ( "P_hoton P_arametrisation" ) parametrisation
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;
        Float_t   ePairOpeningAngle;
        Float_t   ePairOpeningDist_dMin;
        Float_t   ePairOpeningDist_dR;
        Float_t   ePairChi2;

        ClassDef (Para_PP,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_AS {
        // Variables used for AS ( "A_ll S_egemts" ) parametrisation
        // (But only the first 50 segments will be used. Isnt that a contradiction ???)
        Int_t     nseg;  // identical to <<eNBT>>
        Double_t edIP[50];
        Double_t edMin[50];
        Double_t edRLong[50];
        Double_t edRTrans[50];
        Double_t edeltarb[50];
        Double_t edeltathetab[50];

        ClassDef (Para_AS,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_SE {
        // Variables used for SE ( "S_imple E_asy" ) like parametrisation
        Float_t   ShowerAxisAngle; // shower Axis angle (tan theta)
        Int_t     nseg;  // identical to <<eNBT>>
        Int_t     npl;   // identical to <<eNPl>>
        Float_t   efficiencyAtShowerAxisAngle;

        ClassDef (Para_SE,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct Para_ExtraInfo {
        // Parametrisation Variables to store some extraInfo for the shower, which
        // dont have necessarily have something to to with the PARA_!! parametrisation
        Float_t   InBT_E;  // InBT->P()
        Float_t   InBT_Flag;  // InBT->Flag()
        Float_t   InPairBT_E;  // In0BT->P()+In1BT->P() if sum of Flag() ==0 (photon=electron+positron); else identical to InBT_E
        Int_t   ShowerLength;   // max of Z()max -Z()min over all segments
        Float_t    InBT_IPToVtx; // IP from InBT to generic vertex. Commonly used: Vertex of G4 position vertex.
        Float_t    InBT_DeltaZToVtx; // DeltaZ from InBT to generic vertex. Commonly used: Vertex of G4 position vertex.

        ClassDef (Para_ExtraInfo,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct MCInfo_PGun {
        // Variables MCInfo_PGun to get MC data from the simulation by MC info.
        // Use this when the simulation was done with G4Particle generator
        // And the corresponding outputfile is in FWM like format.
        // FWM format to be specified: up to now:
        // #MCEvt energy tantheta dirx diry dirz vtxposx vtxposy vtxposz TX TY X Y Z
        Int_t MCEvt;
        Int_t PDGId;
        Float_t energy;
        Float_t tantheta;
        Float_t dirx;
        Float_t diry;
        Float_t dirz;
        Float_t vtxposx;
        Float_t vtxposy;
        Float_t vtxposz;
        Float_t TX;
        Float_t TY;
        Float_t Y;
        Float_t X;
        Float_t Z;
        // These are the values for the incident particle! If its a gamma, for example,
        // the do not necessarily coincide with these from the shower! Remember!

        ClassDef (MCInfo_PGun,1) // maybe it works, when writing //ClassDef hereafter....?
    };
    struct MCInfo_Event {
        // Variables MCInfo_PGun to get MC data from the simulation by MC info.
        // Use this when the simulation was done with G4 Dario generator
        // And the corresponding outputfile is in FWM like format.
        Int_t       eDUMMYVARIABLE; // not yet used.

        ClassDef (MCInfo_Event,1) // maybe it works, when writing //ClassDef hereafter....?
    };


    // General Implementation of the Reco_ID structure:
    // Instances are use for e-pi separation, or e-gamma separation and so on.
    // Instances are dependent on the type of Parametriasation with which the shower
    // was parametrized, so the eParaNr is saved.
    struct Reco_ID {
        Float_t   eProb90;   // Signal efficency
        Float_t   eProb5;   // Background rejection

        TString eReco_ID_Name;   //"E_GAMMA";
        Float_t eReco_ID_Nr;  //=  0;

        Int_t    eParaNr; // refers to labelling of Parametrisations (0==FJ,1==YC...)

        ClassDef (Reco_ID,1) // maybe it works, when writing //ClassDef hereafter....?
    };

    // General Implementation of the Reco_E structure:
    // Instances are use for estimating the shower energy.
    // Instances are dependent on the type of parametrisation with which the shower
    // was parametrized, so the eParaNr is saved.
    // Instances may also be dependent on the type of the shower, if its a
    // gammaShower or a electronShower.
    struct Reco_E {
        Short_t       eId;
        Short_t       ePDGId;
        Float_t       eEnergy;
        Float_t       eEnergyUncorrected;
        Float_t       eEnergyCorrected;
        Int_t    eParaNr; // refers to labelling of Parametrisations (0==FJ,1==YC...)

        ClassDef (Reco_E,1) // maybe it works, when writing //ClassDef hereafter....?
    };

    // General Implementation of the Reco_Vtx structure:
    // Dummy structure...to be specified later....
    struct Reco_Vtx {
        Int_t       eDUMMYVARIABLE; // not yet used.

        ClassDef (Reco_Vtx,1) // maybe it works, when writing //ClassDef hereafter....?
    };


    // Object for the shower class description
    // which contain additional information...
    Para_FJ     ePara_FJ;
    Para_LT     ePara_LT;
    Para_YC     ePara_YC;
    Para_JC     ePara_JC;
    Para_XX     ePara_XX;
    Para_YY     ePara_YY;
    Para_PP     ePara_PP;
    Para_AS     ePara_AS;
    Para_SE     ePara_SE;
    // Number of parameters used for the specific parametrisation.
    Int_t       ePara_N;

    Para_ExtraInfo     ePara_ExtraInfo;
    MCInfo_PGun    eMCInfo_PGun;
    MCInfo_Event   eMCInfo_Event;
    
    Reco_E	eReco_E;
    Reco_Vtx	eReco_Vtx;

    TObjArray*  eReco_ID_Array; // create crash when not initialized??? yes, so better initialize it
    TObjArray*  eReco_E_Array; // even when  not yet needed....
    TObjArray*  eReco_Vtx_Array; // implementation will come later ...


    void        Init();
    void        Set0();
    void        Set0Parametrizations();
    
    EdbTrackP   *TrackAS() const {
        return eTrackAS;
    }
    EdbVTA      *VTAS() const {
        return eVTAS;
    }


    //-C-  Similar to TrackClass: basic functions which set or return
    //-C-   simple values for the shower

    inline void    SetCounters()           {
        N();
        SetNpl();
        SetN0();
        SetN00();
    }
    inline void    SetN0( int n0 )         {
        eN0 = n0;
    }
    inline void    SetN0()                 {
        eN0 = eNpl-N();
    }
    inline Int_t   N0()      const         {
        return eN0;
    }
    inline void    SetNpl( int npl )       {
        eNpl=npl;
    }
    inline void    SetNpl()                {
        if (eS) eNpl = 1+TMath::Abs(GetSegment(0)->PID() - GetSegment(N()-1)->PID());
    }
    inline Int_t   Npl()     const         {
        return eNpl;
    }
    inline Int_t   N()       const         {
        return (eS) ?  eS->GetEntries()  : 0;
    }
    inline void    SetN00( int n00 )         {
        eN00 = n00;
    }
    inline void    SetN00()                 {
        eN00 = eNpl-N();
    }
    inline Int_t   N00()      const         {
        return eN00;
    }

    inline TObjArray* GetSegmentArray()    {
        return eS;
    }

    inline EdbSegP *GetSegmentFirst()  const {
        return (eS) ? (EdbSegP*)(eS->First()) : 0;
    }
    inline EdbSegP *GetSegmentLast()   const {
        return (eS) ? (EdbSegP*)(eS->Last()) : 0;
    }
    inline EdbSegP *GetSegment(int i)  const {
        return (eS) ? (EdbSegP*)(eS->At(i)) : 0;
    }

    inline Float_t GetSegmentFirstZ()  const {
        return (eS) ? ((EdbSegP*)eS->First())->Z() : 9999999;
    }
    inline Float_t GetSegmentLastZ()   const {
        return (eS) ? ((EdbSegP*)eS->Last())->Z() : 9999999;
    }
    inline Float_t GetFirstZ()  const {
        return eFirstZ;
    }
    inline Float_t GetLastZ()   const {
        return eLastZ;
    }

    // EdbTrackP copied... needs to be filled with the corresponding ID values, obtained by the ID algos:
    void    SetPDG( int pdg )  {
        ePDG=pdg;
    }
    Int_t   PDG()      const {
        return ePDG;
    }
    Int_t   GetPDG()   const {
        return PDG();
    }
    void    SetM( float m )  {
        eM=m;
    }
    Float_t M()      const {
        return eM;
    }
    Int_t   GetM()   const {
        return M();
    }


    // Having EdbShowerP specific functions in it (no EdbTrackP equivalent here)

    // Gravity Center, Axis:
    inline EdbSegP* GetSegmentGravity()  const {
        return eShowerAxisCenterGravityBT;
    }
    inline Float_t  GetSegmentAngle()    const {
        return eShowerAxisAngle;
    }

    // Add a segment:
    void  AddSegment(EdbSegP *s);
    void  AddSegmentAndUpdate(EdbSegP *s);    // Attention, will execute the whole Update/Calculation Procedure
    // for each BT which is attached.
    // void  RemoveSegment(EdbSegP *s);       // NOT YET SUPPORTED.... isBUG in fedra. Solved from svn_1000
    void  RemoveSegment(EdbSegP *s, Bool_t UpdateAll=kFALSE);

    // Set an array of (EdbSegP) as shower array:
    inline void    SetSegmentArray(TObjArray* segArray)     {
        eS=segArray;
        Update();
    }

    // Sort ShowerSegments after ascending Z position:
    void    Sort();
    Bool_t  IsSorted();

    // Clear functions:
    void    ClearAll();
    inline void  Clear()                     {
        if (eS)  eS->Clear();
    }
    inline void  ClearProfileLongitudinalHisto() {
        if (eProfileLongitudinal)  eProfileLongitudinal->Reset();
    }
    inline void  ClearProfileTransversalHisto() {
        if (eProfileTransversal)  eProfileTransversal->Reset();
    }


    inline Int_t   GetNBT()       const         {
        return eNBT;
    }
    inline Int_t   GetNBTMC()     const         {
        return eNBTMC;
    }
    inline Int_t   GetNBTMCFlag(Int_t PdgArray)     const         {
        return eNBTMCFlag[PdgArray];
    }
    inline Int_t   GetNBTPlate(Int_t PIDdiff)      const         {
        return eNBTPlate[PIDdiff];
    }
    inline Int_t   GetNBTMCPlate(Int_t PIDdiff)    const         {
        return eNBTMCPlate[PIDdiff];
    }
    inline Int_t   GetNBTMCFlagPlate(Int_t PIDdiff,Int_t PdgArray)     const {
        return eNBTMCFlagPlate[PIDdiff][PdgArray];
    }

    inline Float_t GetPurity()        const         {
        return ePurity;
    }
    inline Float_t GetSphericity()    const         {
        return eSphericity;
    }
    inline TH1F*   GetProfileLongitudinal()   const {
        return eProfileLongitudinal;
    }
    inline TH1F*   GetProfileTransversal()   const {
        return eProfileTransversal;
    }
    TH1F*          GetProfileLongitudinalPlate();

    inline Bool_t   GetParametrisationIsDone(Int_t nr) const  {
        return eParametrisationIsDone[nr];
    }
    Float_t         GetParaVariable(Int_t ParaVariable, Int_t ParaNr);

    inline Para_FJ   GetPara_FJ()       const         {
        return ePara_FJ;
    }
    inline Para_LT   GetPara_LT()       const         {
        return ePara_LT;
    }
    inline Para_YC   GetPara_YC()       const         {
        return ePara_YC;
    }
    inline Para_JC   GetPara_JC()       const         {
        return ePara_JC;
    }
    inline Para_XX   GetPara_XX()       const         {
        return ePara_XX;
    }
    inline Para_YY   GetPara_YY()       const         {
        return ePara_YY;
    }
    inline Para_PP   GetPara_PP()       const         {
        return ePara_PP;
    }
    inline Para_AS   GetPara_AS()       const         {
        return ePara_AS;
    }
    inline Para_SE   GetPara_SE()       const         {
        return ePara_SE;
    }
    inline Para_ExtraInfo   GetPara_ExtraInfo()       const         {
        return ePara_ExtraInfo;
    }
    // See comment in implementation, why we do this function:
    void         SetParaVariable(Float_t fParVar, Int_t ParaVariable, Int_t ParaNr);

    inline Bool_t        GetMCInfoIsDone(Int_t nr) const  {
        return eMCInfoIsDone[nr];
    }
    inline Bool_t        GetExtraInfoIsDone(Int_t nr) const  {
        return eExtraInfoIsDone[nr];
    }
    inline MCInfo_PGun   GetMCInfo_PGun()       const         {
        return eMCInfo_PGun;
    }
    inline MCInfo_Event  GetMCInfo_Event()       const         {
        return eMCInfo_Event;
    }

    inline void          SetMCInfoIsDone(Int_t nr, Bool_t IsDone) {
        eMCInfoIsDone[nr]=IsDone;
        return;
    }
    inline void          SetExtraInfoIsDone(Int_t nr, Bool_t IsDone) {
        eExtraInfoIsDone[nr]=IsDone;
        return;
    }
    inline void          SetMCInfo_PGun(MCInfo_PGun fMCInfo_PGun) {
        eMCInfo_PGun=fMCInfo_PGun;
    }
    inline void          SetMCInfo_Event(MCInfo_Event fMCInfo_Event) {
        eMCInfo_Event=fMCInfo_Event;
    }

    //-C- Functions that calculate shower properties
    void  BuildNplN0();
    void  BuildNMC();
    void  BuildProfiles();
    void  BuildPlateProfile();
    void  BuildShowerAxis();
    void  BuildParametrisation(Int_t ParaNr);
    void  BuildParametrisation_FJ(); // 0
    void  BuildParametrisation_LT(); // 1
    void  BuildParametrisation_YC(); // 2
    void  BuildParametrisation_JC(); // 3
    void  BuildParametrisation_XX(); // 4
    void  BuildParametrisation_YY(); // 5
    void  BuildParametrisation_PP(); // 6
    void  BuildParametrisation_AS(); // 7
    void  BuildParametrisation_SE(); // 8
    void  BuildParametrisation_ExtraInfo();

    void  CalcPurity();
    void  CalcSphericity();
    void  CalcShowerAngularDeviationDistribution();


    Float_t  CalcOpenAngle(Int_t type);
    Float_t  CalcOpenAngleSimple();
    Double_t CalcIP(EdbSegP*, EdbVertex*);
    Double_t CalcIP(EdbSegP*, double, double, double);
    Double_t CalcDistLongitudinal(EdbSegP*, EdbSegP*);
    Double_t CalcDistTransversal(EdbSegP*, EdbSegP*);

    void  Update();  // Update all shower properties. Can be called each time after BT is added or
    // when last BT was added.
    void  UpdateX(); // Update all shower properties _AND_ build all parametrisations.


    /// ----   EXPERIMENTAL STATUS ---
    //-C- Functions for advanced features:
    void  MergeFromShower(EdbShowerP* show,Int_t MergingType=1);
    void  MergeFromShowerByAddress(EdbShowerP* show);
    void  MergeFromShowerByPosition(EdbShowerP* show);
    void  AddShower(EdbShowerP* show); // calls MergeFromShower...
    void  Finalize();  /// Delete the histograms only... /// ----   EXPERIMENTAL STATUS ---
    /// ----   EXPERIMENTAL STATUS ---


    //-C- Print functions with different detail//output level
    void  Print();
    void  PrintNice();
    void  PrintBasics();
    void  PrintMetaData();
    void  PrintSegments();
    void  PrintParametrisation(Int_t ParaNr=0);
    void  PrintParametrisation_FJ();
    void  PrintParametrisation_LT();
    void  PrintParametrisation_YC();
    void  PrintParametrisation_JC();
    void  PrintParametrisation_XX();
    void  PrintParametrisation_YY();
    void  PrintParametrisation_PP();
    void  PrintParametrisation_AS();
    void  PrintParametrisation_SE();
    void  PrintParametrisation_ExtraInfo();
    void  PrintMCInfo_PGun();
    void  PrintAll();

    //-C- HELP function , in case you dont know anything about it .....
    void Help();

    ClassDef(EdbShowerP,1)         // Root Class Definition for object EdbShowerP

};

#endif /* ROOT_EdbShowerP */
