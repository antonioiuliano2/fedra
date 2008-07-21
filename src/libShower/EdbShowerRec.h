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
#include "EdbLog.h"
#include "TROOT.h"

class EdbShowerRec : public TObject
{
 private:
    Int_t      eID;         // shower ID
    Float_t    eX0;         // |_coordinates of the first shower segment
    Float_t    eY0;         // | in the SAME FOR ALL SEGMENTS aligned  coordinate system
    Float_t    eZ0;         // |
    Float_t    eTx;         // tanX: deltaX/deltaZ for the first shower segment
    Float_t    eTy;         // tanY: deltaY/deltaZ for the first shower segment
    Int_t      eTrID;       // ID of the tracks of the first BT (in cp file if simulation!)

    Float_t    eDz;         // length of the shower along Z 
    Int_t      eL;          // length of the shower in number of film

    Int_t      eIDb[1000];  // ID of basetrack
    Int_t      ePlateb[1000];  // Plate ID of basetrack    
    Float_t    eXb[1000];   // X position of basetrack
    Float_t    eYb[1000];   // Y position of basetrack
    Float_t    eZb[1000];   // Z position of basetrack
    Float_t    eTXb[1000];   // Slope X position of basetrack
    Float_t    eTYb[1000];   // Slope Y position of basetrack
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
   
    
   TObjArray* eInitiatorBTs;    // Starting Basetracks from where to start reconstruction.
                                // May be real BTs (from linked_tracks) or virtual 
                                // ones (just positions).
   Bool_t     eInitiatorBTIsCreated;
   Bool_t     eInitiatorBTIsFilled;
   
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


 public:  
   
    EdbShowerRec(); 
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

#ifdef WIN32
#else
  TMultiLayerPerceptron *mlp1();
  TMultiLayerPerceptron *mlp2();
  TMultiLayerPerceptron *mlp3();
  TMultiLayerPerceptron *mlp4();
#endif

  char cmd[500];
  
  Int_t      GetID() const { return eID; }
  Float_t    GetX0() const { return eX0; }
  Float_t    GetY0() const { return eY0; }
  Float_t    GetZ0() const { return eZ0; }
  Float_t    GetTx() const { return eTx; }
  Float_t    GetTy() const { return eTy; }
  Float_t    GetDz() const { return eDz; }
  Int_t      GetL() const { return eL; }
  Int_t      GetTrID() const { return eTrID; }
  
  Int_t      GetSize() const { return eSize; }
  Int_t      GetSize15() const { return eSize15; }
  Int_t      GetSize20() const { return eSize20; }
  Int_t      GetSize30() const { return eSize30; }
  Int_t      GetPlateb(int i) const { return ePlateb[i]; }
  Int_t      GetIDb(int i) const { return eIDb[i]; }
  
  Float_t    GetXb(int i) const { return eXb[i]; }
  Float_t    GetYb(int i) const { return eYb[i]; }
  Float_t    GetZb(int i) const { return eZb[i]; }
  Float_t    GetTXb(int i) const { return eTXb[i]; }
  Float_t    GetTYb(int i) const { return eTYb[i]; }
  Int_t      GetNFilmb(int i) const { return eNFilmb[i]; }
  Float_t    GetDeltarb(int i) const { return eDeltarb[i]; }
  Float_t    GetDeltathetab(int i) const { return  eDeltathetab[i]; }
  Int_t    GetTagPrimaryb(int i) const { return  eTagPrimary[i]; }
  

  Float_t    GetOutput() const { return eOutput; }
  Float_t    GetOutput15() const { return eOutput15; }
  Float_t    GetOutput20() const { return eOutput20; }
  Float_t    GetOutput30() const { return eOutput30; }

  Float_t    GetEnergy()            const { return eEnergyCorrectedb; }
  Float_t    GetEnergyUn()          const { return eEnergyUnCorrectedb; }
  Float_t    GetEnergySigma()       const { return eEnergySigmaCorrectedb; }
  Float_t    GetEnergyUnSigma()     const { return eEnergySigmaUnCorrectedb; }  
  void    SetID(int id)    { eID = id; }
  
  void    SetX0( float x ) { eX0 = x; }
  void    SetY0( float y ) { eY0 = y; }
  void    SetZ0( float z ) { eZ0 = z; }
  void    SetTx( float tx ) { eTx = tx; }
  void    SetTy( float ty ) { eTy = ty; }
  void    SetTrID( int id  ) { eTrID = id; }
  
  void    SetSize( int size ) { eSize = size; }
  void    SetSize15( int size ) { eSize15 = size; }
  void    SetSize20( int size ) { eSize20 = size; }
  void    SetSize30( int size ) { eSize30 = size; }
  
  void    SetDz( float dz ) { eDz = dz; }
  void    SetL( int L ) { eL = L; }
  
  void    SetOutput( float output ) { eOutput = output; }
  void    SetOutput15( float output ) { eOutput15= output; }
  void    SetOutput20( float output ) { eOutput20 = output; }
  void    SetOutput30( float output ) { eOutput30 = output; }
  
  //    void    SetXb(float xb[1000]) {for(int i=0; i<1000; i++) {eXb[i]= xb[i];}}
  void    SetXb(float xb, int i) {eXb[i]= xb;}
  void    SetIDb(int idb2, int i) {eIDb[i] = idb2;}
  void    SetPlateb(int plate2, int i) {ePlateb[i] = plate2;}
  void    SetYb(float yb, int i) {eYb[i]= yb;}
  void    SetZb(float zb, int i) {eZb[i]= zb;}
  void    SetTXb(float txb, int i)  {eTXb[i]= txb;}
  void    SetTYb(float tyb, int i)  {eTYb[i]= tyb;}
  void    SetNFilmb(int nfilmb, int i)  {eNFilmb[i]= nfilmb;}
  void    SetDeltarb(float deltarb, int i) {eDeltarb[i]= deltarb;}
  void    SetDelthetab(float deltathetab, int i)  {eDeltathetab[i]= deltathetab;}
  void    SetPrimary(int tagprimary, int i)  {eTagPrimary[i]= tagprimary;}
  
  void InitPiece(EdbDataPiece &piece, const char *cpfile, const char *parfile);
  int  ReadPiece(EdbDataPiece &piece, EdbPattern &pat);
 // int  FindPredictions(EdbPattern &pred, const char *cpfile, const char *parfile, EdbPattern &found);
  /*
    void rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par, int DOWN, float Rcut,  float Tcut);
    void recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par, float Rcut,  float Tcut);
    void recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par,float Rcut, float Tcut);
  */
    void rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par, int DOWN);
    void recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par);
    void recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par);
    //    void remove(char *shfname, char *def,float Rcut, float Tcut);
    //    void vert(const char *name2,const char *def,float Rcut, float Tcut);
    void remove(char *shfname, char *def,int MAXPLATE, int piece2par);
    void vert(const char *name2,const char *def,int MAXPLATE, int piece2par);

    void SaveResults();
    void initproc( const char *def, int iopt,   const char *rcut);    
    void ReadAffAndZ( char *fname,  Float_t *pZoffs, Float_t *a11,  Float_t *a12,  Float_t *a21,  Float_t *a22, Float_t *b1,  Float_t *b2);
    void NeuralNet() ;
    void Shower2Tracks() ;

  //-------------------------------
    // To estimate the energy values, either 
    // after(!) the reconstruction was done, or
    // we have an already reconstructed shower file.
    void    SetDoEnergyCalculation(Bool_t DoEnergyCalculation) {eDoEnergyCalculation=DoEnergyCalculation;}
    void    CalculateEnergyValues(); 
    void    Energy_ExtractShowerParametrisationProfile();
    void    Energy_CreateANNTree();
    Float_t Energy_CorrectEnergy(Float_t MeasuredEnergy, Int_t NPlatesNr);    
    void    Energy_CreateEnergyResolutionFitFunction();
    Float_t Energy_CalcSigma(Float_t Energy, Int_t NPlatesNr);
    void    LoadShowerFile(TString ShowerFileName);
    
    TH1F*  GetHistEnergyNominal()              {return eHistEnergyNominal;}
    TH1F*  GetHistEnergyCorrectedb()           {return eHistEnergyCorrectedb;}
    TH1F*  GetHistEnergyUnCorrectedb()         {return eHistEnergyUnCorrectedb;}
    TH1F*  GetHistEnergySigmaCorrectedb()      {return eHistEnergySigmaCorrectedb;}
    TH1F*  GetHistEnergySigmaUnCorrectedb()    {return eHistEnergySigmaUnCorrectedb;}
    
    
    TH1D*  histo_nbtk_av;
    TH1D*  histo_longprofile_av;
    TH1D*  histo_deltaR_mean;
    TH1D*  histo_deltaT_mean;
    TH1D*  histo_deltaR_rms;
    TH1D*  histo_deltaT_rms;
    
    
    TH1D*  GetHistLongProfile()                {return histo_longprofile_av;}
    TH1D*  GetHistNBaseTracks()                {return histo_nbtk_av;}
    TH1D*  GetHistdeltaR_mean()                {return histo_deltaR_mean;}
    TH1D*  GetHistdeltaT_mean()                {return histo_deltaT_mean;}
    TH1D*  GetHistdeltaR_rms()                 {return histo_deltaR_rms;}
    TH1D*  GetHistdeltaT_rms()                 {return histo_deltaT_rms;}    
    
    
    
    
    //-------------------------------
    // NEW FUNCTIONS ADDED BY FWM (not related to energy);
    //-------------------------------    
    void  PrintInitiatorBTs();
    int   AddInitiatorBT(EdbSegP* seg);
    
    
 
    ClassDef(EdbShowerRec,3)  
	};
#endif /* ROOT_EdbShowerRec */
