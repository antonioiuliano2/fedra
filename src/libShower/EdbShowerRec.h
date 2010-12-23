#ifndef ROOT_EdbShowerRec
#define ROOT_EdbShowerRec

#include "EdbDataSet.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include "EdbCluster.h"
#include "TTree.h"
#include "TSortedList.h"
#include "TArrayL.h"
#include "TMatrixD.h"
#include "EdbVirtual.h"
#include "TMultiLayerPerceptron.h"
#include "smatrix/Functions.hh"
#include "EdbTrackFitter.h"
#include "TH1.h"
#include "TSystem.h"
#include "TF1.h"
#include "TVector3.h"
#include "EdbLog.h"
#include "TROOT.h"

class EdbShowerRec : public TObject
{
private:

    // Variables related to algorithm settings
    Double_t    eAlgoParameterConeRadius;      // Radius of spanning cone from first BT
    Double_t    eAlgoParameterConeAngle;       // Opening angle of spanning cone from first BT

    Double_t    eAlgoParameterConnectionDR;      // Connection Criterium: delta R
    Double_t    eAlgoParameterConnectionDT;      // Connection Criterium: delta T

    Int_t      eAlgoParameterNPropagation;      // N plates backpropagating

    // Variables to individually set QualityCut value (additionally to default.par)
    Double_t   eQualityPar[2];
    Bool_t		 eUseQualityPar;


    // Variables concerning the Shower Object and the Reco Object:
    Int_t      eID;         // shower ID
    Float_t    eX0;         // |_coordinates of the first shower segment
    Float_t    eY0;         // | in the SAME FOR ALL SEGMENTS aligned  coordinate system
    Float_t    eZ0;         // |
    Float_t    eTx;         // tanX: deltaX/deltaZ for the first shower segment
    Float_t    eTy;         // tanY: deltaY/deltaZ for the first shower segment
    Int_t      eTrID;       // ID of the tracks of the first BT (in cp file if simulation!)
    Float_t    eChi20;      // Chi2 of the first shower segment
    Int_t      eW0;         // W of the first shower segment
    Float_t    eP0;         // P of the first shower segment
    Int_t      eFlag0;      // Flag of the first shower segment

    Float_t    eDz;         // length of the shower along Z
    Int_t      eL;          // length of the shower in number of film

    Int_t      eIDb[1000];  // ID of basetrack
    Int_t      ePlateb[1000];  // Plate ID of basetrack
    Float_t    eXb[1000];   // X position of basetrack
    Float_t    eYb[1000];   // Y position of basetrack
    Float_t    eZb[1000];   // Z position of basetrack
    Float_t    eTXb[1000];   // Slope X position of basetrack
    Float_t    eTYb[1000];   // Slope Y position of basetrack
    Float_t    eChi2b[1000];   // Chi2 of basetrack
    Float_t    ePb[1000];   // P of basetrack
    Int_t      eWb[1000];   // W of basetrack
    Int_t      eFlagb[1000];   // Flag of basetrack
    Int_t      eNFilmb[1000]; // Plate number of basetrack in the shower reference
    Float_t    eDeltarb[1000];  // Distance criteria of  basetrack
    Float_t    eDeltathetab[1000]; // Angular criteria of  basetrack
    Int_t    eTagPrimary[1000]; // 1 for first Basetrack - 0 for the other

    Int_t eSize;            // number of BT in the shower
    Int_t eSize15;          // number of BT in the shower (for 15 films crossed)
    Int_t eSize20;          // number of BT in the shower (for 20 films crossed)
    Int_t eSize30;          // number of BT in the shower (for 30 films crossed)

    Float_t eOutput;        // Neural Network output for e/pi separation
    Float_t eOutput15;      // Neural Network output for e/pi separation (for 15 films crossed)
    Float_t eOutput20;      // Neural Network output for e/pi separation (for 20 films crossed)
    Float_t eOutput30;      // Neural Network output for e/pi separation (for 30 films crossed)



    Bool_t     eInitiatorBTIsCreated;
    Bool_t     eInBTArrayIsFilled;

    // Variables used for naming the files to read/write the specified
    // data structures or specified objects.
    //  Read:
    TString           eFilename_LinkedTracks;


    // EdbPVRec object:
    EdbPVRec*          eAli;
    EdbPVRec*          eAli_Sub;
    Float_t            eAli_Sub_halfsize;
    Int_t              eAliNpat;
    Int_t              eAli_SubNpat;
    Int_t              eAliNpatM1;
    Int_t              eNumberPlate_eAliPID;

    Bool_t             eAliLoaded;
    Bool_t             eUseAliSub;


    // TObjArray storing Initiator Basetracks:
    // May be real BTs (from linked_tracks) or virtual
    // ones (just positions).
    TObjArray*         eInBTArray;
    Int_t              eInBTArrayN;
    Bool_t             eInBTArrayLoaded;
    Int_t              eInBTArrayMaxSize;

    // TObjArray storing reconstructed showers (objects of EdbTrackP class):
    TObjArray*         eRecoShowerArray;
    Int_t              eRecoShowerArrayN;






    //----------------------------------------------------------------

    Float_t eEnergyCorrectedb;            // Neural Network output for   Corrected Energy
    Float_t eEnergyUnCorrectedb;          // Neural Network output for UnCorrected Energy
    Float_t eEnergySigmaCorrectedb;       // Neural Network output for   Corrected Energy Sigma
    Float_t eEnergySigmaUnCorrectedb;     // Neural Network output for UnCorrected Energy Sigma
    Float_t energy_shot_particle;

    TH1F*   eHistEnergyNominal;
    TH1F*   eHistEnergyCorrectedb;
    TH1F*   eHistEnergyUnCorrectedb;
    TH1F*   eHistEnergySigmaCorrectedb;
    TH1F*   eHistEnergySigmaUnCorrectedb;

    Bool_t    eShowerTreeIsDone;            // ShowerReconstruction already done or not
    Bool_t    eDoEnergyCalculation;         // EnergyEstimation to do or not.
    Bool_t    eEnergyIsDone;                // EnergyEstimation already done or not
//     TMultiLayerPerceptron* eEnergyANN[9];   // Container for the ANNs trained on different
    // shower length ("nfilmb");
    TObjArray* eEnergyANN;   // Container for the ANNs trained on different
    TMultiLayerPerceptron* eEANN();// shower length ("nfilmb");


    TString*  eEnergyWeightFileString[9];   // Container for the weightfiles trained on different
    // shower length ("nfilmb");
    TTree*    ANNInterimTreeInputvariables;
    Double_t inANN[70];
    TF1*      eEnergyResolutionFitFunction_All;

    Int_t     eShowersN; // Number how many showers have been reconstructed.

public:

    EdbShowerRec();
    EdbShowerRec(TObjArray* InBTArray, int num,int MAXPLATE,  int DATA, int piece2, int piece2par,int DOWN,EdbPVRec  *pvr);
//    EdbShowerRec(float x, float y, float z, float tx, float ty, int trid, int size=0, float dz=0, float output=0, int id=0);
//    EdbShowerRec(float x, float y, float z, float tx, float ty, int trid);

    virtual ~EdbShowerRec();

//definition of treebranch
    Int_t number_eventb;
    Int_t sizeb15;
    Int_t sizeb20;
    Int_t sizeb30;
    Float_t output15;
    Float_t output20;
    Float_t output30;
    Float_t E_MC;
    Float_t output50;

    Int_t sizeb;
    Int_t isizeb;
    Int_t showerID;
    Int_t idb[1000];
    Int_t plateb[1000];
    Float_t xb[1000];
    Float_t yb[1000];
    Float_t zb[1000];
    Float_t txb[1000];
    Float_t tyb[1000];
    Int_t nfilmb[1000];
    Int_t ngrainb[1000];
    Int_t ntrace1simub[1000];
    Int_t ntrace2simub[1000];
    Float_t ntrace3simub[1000];
    Int_t ntrace4simub[1000];
    Float_t chi2btkb[1000];
    Float_t deltarb[1000];
    Float_t deltathetab[1000];
    Float_t deltaxb[1000];
    Float_t deltayb[1000];
    Float_t tagprimary[1000];

    Float_t EnergyCorrectedb;
    Float_t EnergyUnCorrectedb;
    Float_t EnergySigmaCorrectedb;
    Float_t EnergySigmaUnCorrectedb;

    Float_t purityb;
    Float_t trackdensb; // Track density __around__ the shower (BT/mm2) (using eAli_Sub_halfsize).

    Int_t eProb90;
    Int_t eProb1;

    TFile *fileout;
    TTree *treesaveb;
    TFile *file;

    TFile *fileout2;
    TTree *treesaveb2;

    TFile *fileout3;
    TTree *treesaveb3;
    EdbPVRec     *gAli;

    TFile *FileReconstructedShowerTree;
    TFile *FileShowerParametrisationTree;
    TTree *ShowerParametrisationTree;

    TTree *varia ;

    EdbDataProc *dproc;
    EdbDataProc *a;

    Double_t bin1, bin2, bin3, bin4, bin5, bin6, bin7, bin8, bin9, bin10;
    Double_t bin11, bin12, bin13, bin14, bin15, bin16, bin17, bin18, bin19, bin20;
    Double_t bin21,bin22,bin23,bin24,bin25, bin26, bin27, bin28, bin29, bin30;
    Double_t bin31,bin32,bin33,bin34,bin35;
    Double_t bin41,bin42,bin43,bin44,bin45, bin46, bin47, bin48, bin49, bin50;
    Double_t bin51,bin52,bin53,bin54,bin55, bin56, bin57, bin58, bin59, bin60;
    Int_t type;

    TMultiLayerPerceptron *mlp1;
    TMultiLayerPerceptron *mlp2;
    TMultiLayerPerceptron *mlp3;
    TMultiLayerPerceptron *mlp4;

    char cmd[500];

    inline Int_t GetN() const {
        return eShowersN;
    }

    Int_t      GetID() const {
        return eID;
    }
    Float_t    GetX0() const {
        return eX0;
    }
    Float_t    GetY0() const {
        return eY0;
    }
    Float_t    GetZ0() const {
        return eZ0;
    }
    Float_t    GetChi20() const {
        return eChi20;
    }
    Int_t      GetW0() const {
        return eW0;
    }
    Float_t    GetP0() const {
        return eP0;
    }
    Int_t      GetFlag0() const {
        return eFlag0;
    }
    Float_t    GetTx() const {
        return eTx;
    }
    Float_t    GetTy() const {
        return eTy;
    }
    Float_t    GetDz() const {
        return eDz;
    }
    Int_t      GetL() const {
        return eL;
    }
    Int_t      GetTrID() const {
        return eTrID;
    }

    Int_t      GetSize() const {
        return eSize;
    }
    Int_t      GetSize15() const {
        return eSize15;
    }
    Int_t      GetSize20() const {
        return eSize20;
    }
    Int_t      GetSize30() const {
        return eSize30;
    }
    Int_t      GetPlateb(int i) const {
        return ePlateb[i];
    }
    Int_t      GetIDb(int i) const {
        return eIDb[i];
    }

    Float_t    GetXb(int i) const {
        return eXb[i];
    }
    Float_t    GetYb(int i) const {
        return eYb[i];
    }
    Float_t    GetZb(int i) const {
        return eZb[i];
    }
    Float_t    GetTXb(int i) const {
        return eTXb[i];
    }
    Float_t    GetTYb(int i) const {
        return eTYb[i];
    }
    Float_t    GetChi2b(int i) const {
        return eChi2b[i];
    }
    Int_t      GetWb(int i) const {
        return eWb[i];
    }
    Float_t    GetPb(int i) const {
        return ePb[i];
    }
    Int_t      GetFlagb(int i) const {
        return eFlagb[i];
    }
    Int_t      GetNFilmb(int i) const {
        return eNFilmb[i];
    }
    Float_t    GetDeltarb(int i) const {
        return eDeltarb[i];
    }
    Float_t    GetDeltathetab(int i) const {
        return  eDeltathetab[i];
    }
    Int_t    GetTagPrimaryb(int i) const {
        return  eTagPrimary[i];
    }


    Float_t    GetOutput() const {
        return eOutput;
    }
    Float_t    GetOutput15() const {
        return eOutput15;
    }
    Float_t    GetOutput20() const {
        return eOutput20;
    }
    Float_t    GetOutput30() const {
        return eOutput30;
    }

    Float_t    GetEnergy()            const {
        return eEnergyCorrectedb;
    }
    Float_t    GetEnergyUn()          const {
        return eEnergyUnCorrectedb;
    }
    Float_t    GetEnergySigma()       const {
        return eEnergySigmaCorrectedb;
    }
    Float_t    GetEnergyUnSigma()     const {
        return eEnergySigmaUnCorrectedb;
    }
    void    SetID(int id)    {
        eID = id;
    }

    void    SetX0( float x ) {
        eX0 = x;
    }
    void    SetY0( float y ) {
        eY0 = y;
    }
    void    SetZ0( float z ) {
        eZ0 = z;
    }
    void    SetChi20(float chi2) {
        eChi20=chi2;
    }
    void    SetW0(int w) {
        eW0=w;
    }
    void    SetP0(float p) {
        eP0=p;
    }
    void    SetFlag0(int flag) {
        eFlag0=flag;
    }
    void    SetTx( float tx ) {
        eTx = tx;
    }
    void    SetTy( float ty ) {
        eTy = ty;
    }
    void    SetTrID( int id  ) {
        eTrID = id;
    }

    void    SetSize( int size ) {
        eSize = size;
    }
    void    SetSize15( int size ) {
        eSize15 = size;
    }
    void    SetSize20( int size ) {
        eSize20 = size;
    }
    void    SetSize30( int size ) {
        eSize30 = size;
    }

    void    SetDz( float dz ) {
        eDz = dz;
    }
    void    SetL( int L ) {
        eL = L;
    }

    void    SetOutput( float output ) {
        eOutput = output;
    }
    void    SetOutput15( float output ) {
        eOutput15= output;
    }
    void    SetOutput20( float output ) {
        eOutput20 = output;
    }
    void    SetOutput30( float output ) {
        eOutput30 = output;
    }

    void    SetXb(float xb, int i) {
        eXb[i]= xb;
    }
    void    SetIDb(int idb2, int i) {
        eIDb[i] = idb2;
    }
    void    SetPlateb(int plate2, int i) {
        ePlateb[i] = plate2;
    }
    void    SetYb(float yb, int i) {
        eYb[i]= yb;
    }
    void    SetZb(float zb, int i) {
        eZb[i]= zb;
    }
    void    SetTXb(float txb, int i)  {
        eTXb[i]= txb;
    }
    void    SetTYb(float tyb, int i)  {
        eTYb[i]= tyb;
    }
    void    SetChi2b(float chi2b, int i)  {
        eChi2b[i]= chi2b;
    }
    void    SetWb(int w, int i)  {
        eWb[i]= w;
    }
    void    SetPb(float p, int i)  {
        ePb[i]= p;
    }
    void    SetFlagb(int flag, int i)  {
        eFlagb[i]= flag;
    }
    void    SetNFilmb(int nfilmb, int i)  {
        eNFilmb[i]= nfilmb;
    }
    void    SetDeltarb(float deltarb, int i) {
        eDeltarb[i]= deltarb;
    }
    void    SetDelthetab(float deltathetab, int i)  {
        eDeltathetab[i]= deltathetab;
    }
    void    SetPrimary(int tagprimary, int i)  {
        eTagPrimary[i]= tagprimary;
    }

    void InitPiece(EdbDataPiece &piece, const char *cpfile, const char *parfile);
    int  ReadPiece(EdbDataPiece &piece, EdbPattern &pat);
// int  FindPredictions(EdbPattern &pred, const char *cpfile, const char *parfile, EdbPattern &found);
    /*
      void rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par, int DOWN, float Rcut,  float Tcut);
      void recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par, float Rcut,  float Tcut);
      void recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par,float Rcut, float Tcut);
    */

    /// OLD  --- not used anymore....
    //     void rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par, int DOWN);
    //     void recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par);
    //     void recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par);
    /// NEW  --- to be used from now on .... (svn. Revision 889)
    void rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim,int piece2, int piece2par, int DOWN,EdbPVRec  *pvr);
    void rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, double* chi20, int* W0, double* P0, int* Flag0, int *plate0, int *id0, int *TRid, double *Esim,int piece2, int piece2par,int DOWN,EdbPVRec  *pvr);

    void recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, double* chi20, int* W0, double* P0,int* Flag0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par,EdbPVRec  *pvr);
    void recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, double* chi20, int* W0, double* P0,int* Flag0,int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par);


    void rec(TObjArray *sarr, EdbPVRec  *pvr);



    //    void remove(char *shfname, char *def,float Rcut, float Tcut);
    //    void vert(const char *name2,const char *def,float Rcut, float Tcut);
    void remove(char *shfname, char *def,int MAXPLATE, int piece2par);
    void vert(const char *name2,const char *def,int MAXPLATE, int piece2par);

    void SaveResults();
    void initproc( const char *def, int iopt,   const char *rcut);
    void ReadAffAndZ( char *fname,  Float_t *pZoffs, Float_t *a11,  Float_t *a12,  Float_t *a21,  Float_t *a22, Float_t *b1,  Float_t *b2);
    void NeuralNet() ;
    int SaveLNK();
    TObjArray* ShowerToEdbSegPArray();

    //-------------------------------
    // To estimate the energy values, either
    // after(!) the reconstruction was done, or
    // we have an already reconstructed shower file.
    void    SetDoEnergyCalculation(Bool_t DoEnergyCalculation) {
        eDoEnergyCalculation=DoEnergyCalculation;
    }
    void    CalculateEnergyValues();
    void    Energy_ExtractShowerParametrisationProfile();
    void    Energy_CreateANNTree();
    Float_t Energy_CorrectEnergy(Float_t MeasuredEnergy, Int_t NPlatesNr);
    void    Energy_CreateEnergyResolutionFitFunction();
    Float_t Energy_CalcSigma(Float_t Energy, Int_t NPlatesNr);
    void    LoadShowerFile(TString ShowerFileName);

    TH1F*  GetHistEnergyNominal()              {
        return eHistEnergyNominal;
    }
    TH1F*  GetHistEnergyCorrectedb()           {
        return eHistEnergyCorrectedb;
    }
    TH1F*  GetHistEnergyUnCorrectedb()         {
        return eHistEnergyUnCorrectedb;
    }
    TH1F*  GetHistEnergySigmaCorrectedb()      {
        return eHistEnergySigmaCorrectedb;
    }
    TH1F*  GetHistEnergySigmaUnCorrectedb()    {
        return eHistEnergySigmaUnCorrectedb;
    }


    TH1D*  histo_nbtk_av;
    TH1D*  histo_longprofile_av;
    TH1D*  histo_deltaR_mean;
    TH1D*  histo_deltaT_mean;
    TH1D*  histo_deltaR_rms;
    TH1D*  histo_deltaT_rms;


    TH1D*  GetHistLongProfile()                {
        return histo_longprofile_av;
    }
    TH1D*  GetHistNBaseTracks()                {
        return histo_nbtk_av;
    }
    TH1D*  GetHistdeltaR_mean()                {
        return histo_deltaR_mean;
    }
    TH1D*  GetHistdeltaT_mean()                {
        return histo_deltaT_mean;
    }
    TH1D*  GetHistdeltaR_rms()                 {
        return histo_deltaR_rms;
    }
    TH1D*  GetHistdeltaT_rms()                 {
        return histo_deltaT_rms;
    }




    //-------------------------------
    // NEW FUNCTIONS ADDED BY FWM (not related to energy);
    //-------------------------------
    void  PrintInitiatorBTs();
    int   AddInitiatorBT(EdbSegP* seg);

    Float_t*  GetEnergyValues(Int_t TreeEntry);
    Float_t*  GetShowerValues(Int_t TreeEntry);
    Int_t     GetTreeBranchEntryNr(EdbSegP* seg);


    // Inline Set Functions:
    inline void         SetEdbPVRec( EdbPVRec* Ali )          {
        eAli = Ali;
        eAliNpat=eAli->Npatterns();
        eAliNpatM1=eAliNpat-1;
        eNumberPlate_eAliPID=eAliNpatM1;
        eAliLoaded=kTRUE;
    }
    inline void         SetInBTArray( TObjArray* InBTArray ) {
        eInBTArray = InBTArray;
        eInBTArrayN=eInBTArray->GetEntries();
        if (eInBTArrayN>0) {
            eInBTArrayIsFilled=kTRUE;
        }
    }

    inline void         SetInBTArrayN(Int_t InBTArrayN)      {
        eInBTArrayN = InBTArrayN;
    }

    inline void         SetInBTArray( EdbSegP* InBT) {
        //   check if   eInBTArray  was created, if not do it now.:
        if (!eInBTArray) eInBTArray=new TObjArray();
        eInBTArray->Clear();
        eInBTArray->Add(InBT);
        eInBTArrayN=eInBTArray->GetEntries();
        if (eInBTArrayN>0) {
            eInBTArrayIsFilled=kTRUE;
        }
    }

    inline void         SetRecoShowerArrayN(Int_t RecoShowerArrayN)      {
        eRecoShowerArrayN = RecoShowerArrayN;
    }
    inline void         SetRecoShowerArray(TObjArray* RecoShowerArray)      {
        eRecoShowerArray = RecoShowerArray;
    }

    void                SetAlgoParameter(Double_t paravalue, Int_t paranr);
    void                ResetAlgoParameters();
    inline	void				SetQualityCutValues(Double_t p0, Double_t p1) {
        eQualityPar[0]=p0;
        eQualityPar[1]=p1;
        eUseQualityPar=kTRUE;
    }


    // Inline Get Functions:
    inline EdbPVRec*    GetEdbPVRec( )      const       {
        return eAli;
    }
    inline Int_t        GetEdbPVRecNpat()     const       {
        return eAliNpat;
    }
    inline Int_t        GetEdbPVRecNplateNumber()     const       {
        return eNumberPlate_eAliPID;
    }
    inline Bool_t       GetAliLoaded()      const       {
        return eAliLoaded;
    }

    inline TObjArray*   GetInBTArray( )     const       {
        return eInBTArray;
    }
    inline EdbSegP*     GetInBT(Int_t i)     const       {
        return (EdbSegP*)eInBTArray->At(i);
    }
    inline TObjArray*   GetRecoShowerArray( )     const       {
        return eRecoShowerArray;
    }

    inline Int_t        GetInBTArrayN()     const       {
        return eInBTArrayN;
    }
    inline Int_t        GetRecoShowerArrayN()     const       {
        return eRecoShowerArrayN;
    }


    Double_t            GetAlgoParameter(Int_t paranr);

    // Inline Reset Functions:
    inline void         ResetInBTArray()      {
        eInBTArray=NULL;
        eInBTArrayN = 0;
    }
    inline void         ResetRecoShowerArray()      {
        eRecoShowerArray=NULL;
        eRecoShowerArrayN = 0;
    }




    // Function to fill the  eInBTArray  if it is not already done (in Constructor or by SetInBTArray())
    void                Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks();

    void                ClearInBTArray();
    void                ClearRecoShowerArray();

    // Function for  EdbPVRec  Object for analysis use.
    void                LoadEdbPVRec();
    void                Transform_eAli( EdbSegP* InitiatorBT, Float_t ExtractSize );
    void                CalcTrackDensity(EdbPattern* pat_interim,Int_t pat_interim_halfsize,Int_t& npat_int,Int_t& npat_total,Int_t& npatN);

    // Print:
    inline void         PrintEdbPVRec() const {
        eAli->Print();
    };
    void                PrintRecoShowerArray();
    void                PrintParameters();

    // "Other Functions"
    Double_t  DeltaThetaComponentwise(EdbSegP* s1,EdbSegP* s2);
    Double_t  DeltaR_WithPropagation(EdbSegP* s,EdbSegP* stest);
    Double_t  DeltaR_WithoutPropagation(EdbSegP* s,EdbSegP* stest);
    Bool_t    FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbTrackP* shower);
    Bool_t    IsInConeTube(EdbSegP* TestingSegment, EdbSegP* StartingSegment, Double_t CylinderRadius, Double_t ConeAngle);

    // Execute  -- the reconstruction function:
    void      Execute();

    // Execute  -- Transfer ShowerObjectArray into Treebrach Tree;
    void TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TObjArray* segarray);

    // Execute  -- Transfer Treebrach Tree into ShowerObjectArray  ;
    void TransferTreebranchShowerTreeIntoShowerObjectArray(TTree* treebranch);


    // Standard Reset Function:
    void								Set0();

    ClassDef(EdbShowerRec,3)
};
#endif /* ROOT_EdbShowerRec */
