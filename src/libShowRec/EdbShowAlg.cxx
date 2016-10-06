#include "EdbShowAlg.h"
#include "EdbShowerP.h"

using namespace std;
using namespace TMath;

ClassImp(EdbShowAlg)
ClassImp(EdbShowAlg_SA)
ClassImp(EdbShowAlg_CA)
ClassImp(EdbShowAlg_OI)

//______________________________________________________________________________



EdbShowAlg::EdbShowAlg()
{
// Default Constructor
    cout << "EdbShowAlg::EdbShowAlg()   Default Constructor"<<endl;

// Reset all:
    Set0();
}

//______________________________________________________________________________

EdbShowAlg::EdbShowAlg(TString AlgName, Int_t AlgValue)
{
// Reset all:
    Set0();

    eAlgName=AlgName;
    eAlgValue=AlgValue;
    for (int i=0; i<10; ++i) {
        eParaValue[i]=-99999.0;
        eParaString[i]="UNSPECIFIED";
    }
    eAli_Sub=0;
}

//______________________________________________________________________________

EdbShowAlg::~EdbShowAlg()
{
// Default Destructor
    cout << "EdbShowAlg::~EdbShowAlg()"<<endl;
}

//______________________________________________________________________________


void EdbShowAlg::Set0()
{
// Reset all internal variables.
    eAlgName="UNSPECIFIED";
    eAlgValue=-999999;
    for (int i=0; i<10; ++i) {
        eParaValue[i]=-99999.0;
        eParaString[i]="UNSPECIFIED";
    }
    eAli_Sub=0;

// Do not use use small eAli Object by default:
// (this solution is memory safe....)
    eUseAliSub=0;


    eActualAlgParametersetNr=0;

    eRecoShowerArrayN=0;
    eRecoShowerArray=NULL;
    eInBTArrayN=0;
    eInBTArray=NULL;
}

//______________________________________________________________________________

void EdbShowAlg::SetParameters(Float_t* par)
{
// SetParameters from the given array.
    for (int i=0; i<10; ++i) {
        eParaValue[i]=par[i];
    }
    cout << "EdbShowAlg::SetParameters()...done"<<endl;
}

//______________________________________________________________________________







//______________________________________________________________________________

void EdbShowAlg::Transform_eAli(EdbSegP* InitiatorBT, Float_t ExtractSize=1500)
{

//     Transform eAli to eAli_Sub:
//     the lenght of eAli_Sub is not changed.
//     Only XY-size (and MC) cuts are applied.
// --------------------------------------------------------------------------------------
// ---
// --- Whereas in ShowRec.cpp the treebranch file is written directly after each
// --- BT reconstruction, it was not a problem when the eAliSub was deleted each time.
// --- But now situation is different, since the BTs of the showers have the adresses from
// --- the eAli_Sub !each! so if one eAli_Sub is deleted the EdbShowerP object has lost its
// --- BT adresses.
// --- So if we do not delete the eAli_Sub, reconstrtuction is fast (compared to eAli) but
// --- memory increases VERY fast.
// --- If we do use eAli, then memory consumption will not increase fast, but
// --- reconstruction is slow.
// ---
// --- Possible workarounds:
// ---  For few InBTs (EDA use for data...):     use eAliSub and dont delete it.
// ---  For many InBTs (parameterstudies):      use eAli---.
// ---  Use always eAliSub, search BT correspond in eAli, add BT from eAli....May take also long time...
// ---
// ---  SEVERE wARNING:  IF gAli is in wrong order it can be that no showers.
// ---  SEVERE wARNING:  are reconstructed since most implemented algorithms
// ---  SEVERE wARNING:  rely on the order that plate 2 comes directly behind
// ---                   the InBT. on the todo list....
// ---
// --------------------------------------------------------------------------------------


    Log(3,"EdbShowAlg::Transform_eAli(EdbSegP* InitiatorBT, Float_t ExtractSize=1500)","EdbShowAlg::Transform_eAli");

// IF TO RECREATE THE gALI_SUB in RECONSTRUCTION  or if to use gAli global (slowlier but maybe memory leak safe).
    if (!eUseAliSub)
    {
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg::Transform_eAli   UseAliSub==kFALSE No new eAli_Sub created. Use eAli instead. "<<endl;
        eAli_Sub=eAli;
        eAli_SubNpat=eAli_Sub->Npatterns();  //number of plates
        if (gEDBDEBUGLEVEL>3) eAli_Sub->Print();
        return;
    }

    Int_t npat;
    npat = eAli->Npatterns();  //number of plates

// has to be deleted in some part of the script outside this function...
// Dont forget , otherwise memory heap overflow!
/// DEBUG       if (eAli_Sub) { delete eAli_Sub;eAli_Sub=0;} // original, but keeps not adresses of segment in eAli.
    if (eAli_Sub) {
        ;
    } /// do nothing now... let it live... delete eAli_Sub;eAli_Sub=0;}
//   eAli_Sub = new EdbPVRec();


    if (eUseAliSub) {
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg::Transform_eAli   UseAliSub==kTRUE Will now create new eAli_Sub. "<<endl;

///     if (eAli_Sub) { delete eAli_Sub;eAli_Sub=0;} Try not to delete it maybe then it works.....
        eAli_Sub = new EdbPVRec();
//     cout << "Adress of eAli_Sub " << eAli_Sub << endl;
//     cout << "eAli_Sub->Npatterns();  " << eAli_Sub->Npatterns() << endl;
//     eAli_Sub->Print();
    }



//   cout << "PROBLE THRANFOMING, CAUSE ADRESES ARE DELETED ALSO...." << endl;
//    cout << "TEMPORAY SOLUTION:     comment the delete   eAli_Sub  ( will lead to large memory consumption when run for long time" << endl;


// Create SubPattern objects
    EdbSegP* ExtrapolateInitiatorBT=0;
    ExtrapolateInitiatorBT = (EdbSegP*)InitiatorBT->Clone();

    Int_t InitiatorBTMCEvt=InitiatorBT->MCEvt();

// Create Variables For ExtractSubpattern boundaries
    Float_t mini[5];
    Float_t maxi[5];
//Float_t ExtractSize=1000;// now in fucntion header
    mini[0]=ExtrapolateInitiatorBT->X()-ExtractSize;
    mini[1]=ExtrapolateInitiatorBT->Y()-ExtractSize;
    maxi[0]=ExtrapolateInitiatorBT->X()+ExtractSize;
    maxi[1]=ExtrapolateInitiatorBT->Y()+ExtractSize;
    mini[2]=-0.7;
    mini[3]=-0.7;
    mini[4]=0.0;
    maxi[2]=0.7;
    maxi[3]=0.7;
    maxi[4]=100.0;

    EdbPattern* singlePattern;
    Float_t ExtrapolateInitiatorBT_zpos_orig=ExtrapolateInitiatorBT->Z();

// Add the subpatterns in a loop for the plates:
// in reverse ordering.due to donwstream behaviour (!):
// (Only downstream is supported now...)
    for (Int_t ii=0; ii<npat; ++ii) {

        Float_t zpos=eAli->GetPattern(ii)->Z();

        ExtrapolateInitiatorBT->PropagateTo(zpos);

        mini[0]=ExtrapolateInitiatorBT->X()-ExtractSize;
        mini[1]=ExtrapolateInitiatorBT->Y()-ExtractSize;
        maxi[0]=ExtrapolateInitiatorBT->X()+ExtractSize;
        maxi[1]=ExtrapolateInitiatorBT->Y()+ExtractSize;

        singlePattern=(EdbPattern*)eAli->GetPattern(ii)->ExtractSubPattern(mini,maxi,InitiatorBTMCEvt);
// This sets PID() analogue to (upstream), nut not PID of the BTs !
        singlePattern-> SetID(eAli->GetPattern(ii)->ID());
// This sets PID() analogue to (upstream), nut not PID of the BTs !
        singlePattern-> SetPID(eAli->GetPattern(ii)->PID());

        eAli_Sub->AddPattern(singlePattern);

// Propagate back...! (in order not to change the original BT)
        ExtrapolateInitiatorBT->PropagateTo(ExtrapolateInitiatorBT_zpos_orig);
    }

    delete ExtrapolateInitiatorBT;

    eAli_SubNpat=eAli_Sub->Npatterns();  //number of plates


    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbShowAlg::Transform_eAli   eAli_Sub  created at : "<< eAli_Sub << endl;
        cout << "EdbShowAlg::Transform_eAli   eAli_Sub->Npatterns(): "<< eAli_Sub->Npatterns() << endl;
    }

    Log(3,"EdbShowAlg::Transform_eAli(EdbSegP* InitiatorBT, Float_t ExtractSize=1500)...done.","EdbShowAlg::Transform_eAli...done.");

    return;
}



//______________________________________________________________________________


Bool_t EdbShowAlg::IsInConeTube(EdbSegP* TestingSegment, EdbSegP* StartingSegment, Double_t CylinderRadius, Double_t ConeAngle)
{
// General Function which returns Bool if the Testing BaeTrack is in a cone defined
// by the StartingBaseTrack. In case of starting same Z position, a distance cut of
// 20microns is assumed....
// In case of  TestingSegment==StartingSegment this function should correctly return kTRUE also...
    if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowAlg::IsInConeTube() Test Segment " << TestingSegment << " vs. Starting Segment " << StartingSegment << endl;

// We reject any TestingSegment segments which have lower Z than the StartingSegment .
    if (StartingSegment->Z()>TestingSegment->Z() ) return kFALSE;

    TVector3 x1(StartingSegment->X(),StartingSegment->Y(),StartingSegment->Z());
    TVector3 x2(TestingSegment->X(),TestingSegment->Y(),TestingSegment->Z());
    TVector3 direction_x1(StartingSegment->TX()*1300,StartingSegment->TY()*1300,1300);

// u1 is the difference vector of the position!
    TVector3 u1=x2-x1;

    Double_t direction_x1_norm= direction_x1.Mag();
    Double_t cosangle=  (direction_x1*u1)/(u1.Mag()*direction_x1_norm);
    Double_t angle = TMath::ACos(cosangle);

// This is the old version of angle calculation. It does not give the same results as in ROOT
// when use TVector3.Angle(&TVector3). // For this IsInConeTube() we use therefore the ROOT calculation.
    angle=u1.Angle(direction_x1);

// For the case where the two basetracks have same z position
// the angle is about 90 degree so it makes no sense to calculate it...
// therefore we set it artificially to zero:
    if (StartingSegment->Z()==TestingSegment->Z() ) {
        if (gEDBDEBUGLEVEL>3) cout << "same Z position of TestingSegment and StartingSegment, Set angle artificially to zero" << endl;
        angle=0.0;
// Check here for dR manually:
//cout << DeltaR_WithoutPropagation(StartingSegment,TestingSegment) << endl;
//StartingSegment->PrintNice();
//TestingSegment->PrintNice();
//   cout << StartingSegment->Flag() << " " << TestingSegment->Flag() << endl;
//   cout << StartingSegment->P() << " " << TestingSegment->P() << endl;

// Check for position distance for 20microns if
// Testing Segment is in same Z as StartingSegment
        if (gEDBDEBUGLEVEL>3) cout << "Check for position distance for 20microns if Testing Segment is in same Z as StartingSegment" << endl;
        if (gEDBDEBUGLEVEL>3) cout << "DeltaR_WithoutPropagation(StartingSegment,TestingSegment) = "<< DeltaR_WithoutPropagation(StartingSegment,TestingSegment) << endl;
        if (DeltaR_WithoutPropagation(StartingSegment,TestingSegment)<20) return kTRUE;
        if (DeltaR_WithoutPropagation(StartingSegment,TestingSegment)>=20) return kFALSE;
    }

/// Outside if angle greater than ConeAngle (to be fulfilled for Cone and Tube in both cases)
    if (gEDBDEBUGLEVEL>3) cout << "Check if AngleVector now within the ConeAngleVector (<"<< ConeAngle<<"): " <<   angle << endl;
    if (angle>ConeAngle) {
        return kFALSE;
    }

/// if angle smaller than ConeAngle, then you can differ between Tuberadius and CylinderRadius
    Double_t TubeDistance = 1.0/direction_x1_norm  *  ( (x2-x1).Cross(direction_x1) ).Mag();

    if (gEDBDEBUGLEVEL>3) cout << "Check if TestingSegment is now within the Tube (<"<< CylinderRadius<<"): " <<   TubeDistance << endl;

    if (TubeDistance>CylinderRadius) {
        return kFALSE;
    }

    return kTRUE;
}



//______________________________________________________________________________

void EdbShowAlg::Initialize()
{

    return;
}
//______________________________________________________________________________

void EdbShowAlg::Execute()
{
    cout << "EdbShowAlg::Execute()-----------------------------------------------------------------------" << endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlg::Finalize()
{

    return;
}

//______________________________________________________________________________

void EdbShowAlg::Print()
{
    cout << "EdbShowAlg::Print()" << endl;
    cout << eAlgName << "  ;  AlgValue=  " <<  eAlgName << "  ." << endl;
    for (int i=0; i<10; i++) cout << eParaString[i] << "=" << eParaValue[i] << ";  ";
    cout << eFirstPlate_eAliPID << "  " <<  eLastPlate_eAliPID << "  " << eMiddlePlate_eAliPID << "  " << eNumberPlate_eAliPID << "  " << endl;
    cout << "UseAliSub=  " <<  eUseAliSub << "  ." << endl;
    cout << "EdbShowAlg::Print()...done." << endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlg::PrintParameters()
{
    cout << "EdbShowAlg::PrintParameters()" << endl;
    cout << eAlgName<< " :";
    for (int i=0; i<5; i++) cout << setw(6) << eParaValue[i];
    cout <<  "."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlg::PrintParametersShort()
{
    cout << eAlgName<< " :";
    for (int i=0; i<5; i++) cout << setw(6) << eParaValue[i];
    cout <<  "."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowAlg::PrintMore()
{
    cout << "EdbShowAlg::PrintMore()" << endl;
    cout << "eInBTArray->GetEntries();=  " <<  eInBTArray->GetEntries() << "  ." << endl;
    cout << "EdbShowAlg::PrintMore()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlg::PrintAll()
{
    return;
}


//______________________________________________________________________________


Double_t EdbShowAlg::DeltaR_NoPropagation(EdbSegP* s,EdbSegP* stest)
{
// SAME function as DeltaR_WithoutPropagation !!!
    return DeltaR_WithoutPropagation(s,stest);
}

//______________________________________________________________________________

Double_t EdbShowAlg::DeltaR_WithoutPropagation(EdbSegP* s,EdbSegP* stest)
{
    return TMath::Sqrt((s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()));
}

//______________________________________________________________________________

Double_t EdbShowAlg::DeltaR_WithPropagation(EdbSegP* s,EdbSegP* stest)
{
    if (s->Z()==stest->Z()) return TMath::Sqrt((s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()));
    Double_t zorig;
    Double_t dR;
    zorig=s->Z();
    s->PropagateTo(stest->Z());
    dR=TMath::Sqrt( (s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()) );
    s->PropagateTo(zorig);
    return dR;
}

//______________________________________________________________________________

Double_t EdbShowAlg::DeltaTheta(EdbSegP* s1,EdbSegP* s2)
{
// Be aware that this DeltaTheta function returns the abs() difference between the
// ABSOLUTE values of dTheta!!! (not componentwise!
    Double_t tx1,tx2,ty1,ty2;
    tx1=s1->TX();
    tx2=s2->TX();
    ty1=s1->TY();
    ty2=s2->TY();
    Double_t dt= TMath::Abs(TMath::Sqrt(tx1*tx1+ty1*ty1) - TMath::Sqrt(tx2*tx2+ty2*ty2));
    return dt;
}

//______________________________________________________________________________


Double_t EdbShowAlg::DeltaThetaComponentwise(EdbSegP* s1,EdbSegP* s2)
{
// Be aware that this DeltaTheta function returns the difference between the
// component values of dTheta!!!
// Acutally this function should be the normal way to calculate dTheta correctly...
    Double_t tx1,tx2,ty1,ty2;
    tx1=s1->TX();
    tx2=s2->TX();
    ty1=s1->TY();
    ty2=s2->TY();
    Double_t dt= TMath::Sqrt( (tx1-tx2)*(tx1-tx2) + (ty1-ty2)*(ty1-ty2) );
    return dt;
}
//______________________________________________________________________________
Double_t EdbShowAlg::DeltaThetaSingleAngles(EdbSegP* s1,EdbSegP* s2)
{
// SAME function as DeltaThetaComponentwise !!!
    return DeltaThetaComponentwise(s1,s2);
}

//______________________________________________________________________________

Double_t EdbShowAlg::GetSpatialDist(EdbSegP* s1,EdbSegP* s2)
{
// Mainly Z values should dominate... since the are at the order of 10k microns and x,y of 1k microns
    Double_t x1,x2,y1,y2,z1,z2;
    x1=s1->X();
    x2=s2->X();
    y1=s1->Y();
    y2=s2->Y();
    z1=s1->Z();
    z2=s2->Z();
    Double_t dist= TMath::Sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2)  );
//cout << "dist = "  <<  dist << endl;
    return dist;
}
//______________________________________________________________________________


void EdbShowAlg::Help()
{
    cout << "---------------------------------------------------------" <<
         endl;
    cout << "EdbShowAlg::Help()" << endl << endl;
    cout << "  What is this?" << endl;
    cout << "        Primary Reconstruction Class for the Showers" << endl;
    cout << "        Commonly, it takes as basic data input an EdbPVRec object (gAli)" << endl;
    cout << endl;
    cout << "  What can we do with this?" << endl;
    cout << "        First of all, you can choose different types of algos:" << endl;
    cout << "        ShowAlg_OI" << endl;
    cout << "        ShowAlg_AS" << endl;
    cout << "        ShowAlg_RC" << endl;
    cout << "        ShowAlg_TC" << endl;
    cout << "        ShowAlg_NN" << endl;
    cout << "        ShowAlg_RC (*** to be implemented ***)" << endl;
    cout << "        ShowAlg_BW (*** to be implemented ***)" << endl;
    cout << "        ShowAlg_CL (*** to be skipped ***)" << endl;
    cout << "        ShowAlg_GS (*** to be implemented ***)" << endl;
    cout << "        ...." << endl;
    cout << "        Then you set you input via: " << endl;
    cout << "        ->SetEdbPVRec(gAli) : " << endl;
    cout << "        Then you execute the algorithm via: " << endl;
    cout << "        ->Execute() : " << endl;
    cout << "        Finally you get the TObjArray of reconstructed showers (EdbShowerP*) via: " << endl;
    cout << "        ->GetRecoShowerArray() : " << endl;
    cout << endl;
    cout << "   Technical details, information about the class:" << endl;
    cout << "        ...." << endl;
    cout << "        ...." << endl;
    cout << endl;
    cout << "---------------------------------------------------------" <<
         endl;
    return;
}
//______________________________________________________________________________


















//______________________________________________________________________________

EdbShowAlg_SA::EdbShowAlg_SA()
{
// Default Constructor
    cout << "EdbShowAlg_SA::EdbShowAlg_SA()   Default Constructor"<<endl;

// Reset all:
    Set0();

    eAlgName="SA";
    eAlgValue=5; // see default.par_SHOWREC for labeling (lableing identical with ShowRec program)

// Set the two Cuts and their names:
    Init();
}

//______________________________________________________________________________

EdbShowAlg_SA::~EdbShowAlg_SA()
{
    cout << "EdbShowAlg_SA::~EdbShowAlg_SA()"<<endl;
// Default Destructor
}

//______________________________________________________________________________


void EdbShowAlg_SA::Init()
{
    Log(2,"EdbShowAlg_SA::EdbShowAlg_SA","Init()");

//  Init with values according to NN Alg:

// threshold cut on s->P()
    eParaValue[0]=0;
    eParaString[0]="MINIMUM_P";

// size cut on the eAliSub to choose
    eParaValue[1]=1000;
    eParaString[1]="EXTRACTIONSIZE";


    return;
}

//______________________________________________________________________________


void EdbShowAlg_SA::Initialize()
{
    return;
}
//______________________________________________________________________________

void EdbShowAlg_SA::Execute()
{
    cout << "EdbShowAlg_SA::Execute()" << endl;
    cout << "EdbShowAlg_SA::Execute   DOING MAIN SHOWER RECONSTRUCTION HERE" << endl;

    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbShowerP* RecoShower;

    Bool_t     StillToLoop=kTRUE;
    Int_t       ActualPID;
    Int_t      newActualPID;
    Int_t      STEP=-1;
    Int_t     NLoopedPattern=0;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- STEP for patternloop direction =  " << STEP << endl;

//--- Loop over InBTs:

// Since eInBTArray is filled in ascending ordering by zpositon
// We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

// CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%100)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

//-----------------------------------
// 1) Make local_gAli with cut parameters:
//-----------------------------------

// Create new EdbShowerP Object for storage;
// See EdbShowerP why I have to call the Constructor as "unique" ideficable value
        RecoShower = new EdbShowerP(i,eAlgValue,eActualAlgParametersetNr);

// Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();

// Add InBT to RecoShower:
// This has to be done, since by definition the first BT in the RecoShower is the InBT.
// Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
        if (gEDBDEBUGLEVEL>4)  cout << "Segment  (InBT) " << Segment << " was added to RecoShower." <<  endl;

// Transform (make size smaller, extract only events having same MC) the  eAli  object:
// See in Execute_CA for description.
        Transform_eAli(InBT,eParaValue[1]); // here with ExtractSize set!

// Add InBT to RecoShower // obsolete, since we loop also over the plate containing the InBT
//RecoShower --> AddSegment(InBT);

//-----------------------------------
// 2) Loop over (whole) eAli, check BT for Cuts
//-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- --- Doing patterloop " << ActualPID << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<eAli->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                Segment = (EdbSegP*)eAli->GetPattern(ActualPID)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();

// Now apply cut conditions: SA Simple Alg  --------------------
//   ___  TO FILL IN HERE ___
                if (Segment->MCEvt()!=InBT->MCEvt()) continue;
                if (Segment->P()<eParaValue[0]) continue;
// end of    cut conditions: SA Simple Alg  --------------------

// If we arrive here, Basetrack  Segment  has passed criteria
// and is then added to the RecoShower:
// Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
                }
                if (gEDBDEBUGLEVEL>4)  cout << "Segment  " << Segment << " was added at  &Segment : " << &Segment  <<  endl;

            } // of btloop_cnt

//------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- --- newActualPID= " << newActualPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- --- NLoopedPattern= " << NLoopedPattern << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- --- eNumberPlate_eAliPID= " << eNumberPlate_eAliPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- --- StillToLoop= " << StillToLoop << endl;

// This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID) cout << "EdbShowAlg_SA::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
// This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
// This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

// Obligatory when Shower Reconstruction is finished!
        RecoShower ->Update();
//RecoShower ->PrintBasics();


//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- Before adding to array delete the histograms...by finalize() of shower."<<endl;
//     RecoShower ->Finalize();
//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- Finalize done..."<<endl;


// Add Shower Object to Shower Reco Array.
// Not, if its empty:
// Not, if its containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

// Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


// Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    cout << "EdbShowAlg_SA::eRecoShowerArray() Entries: " << eRecoShowerArray->GetEntries() << endl;
    cout << "EdbShowAlg_SA::Execute()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlg_SA::Finalize()
{

    return;
}

//______________________________________________________________________________











//______________________________________________________________________________

EdbShowAlg_CA::EdbShowAlg_CA()
{
// Default Constructor
    cout << "EdbShowAlg_CA::EdbShowAlg_CA()   Default Constructor"<<endl;

// Reset all:
    Set0();

    eAlgName="CA";
    eAlgValue=2; // see default.par_SHOWREC for labelling

    eParaValue[0]=700;
    eParaString[0]="CylinderRadius";  // micrometer
    eParaValue[1]=0.025;
    eParaString[1]="ConeAngle";       // radiant
    eParaValue[2]=110;
    eParaString[2]="DeltaRToPreceedingBT";  // micrometer
    eParaValue[3]=0.05;
    eParaString[3]="DeltaThetaToPreceedingBT";  // radiant

//   700  0.025  110  0.05
}

//______________________________________________________________________________

EdbShowAlg_CA::~EdbShowAlg_CA()
{
    cout << "EdbShowAlg_CA::~EdbShowAlg_CA()"<<endl;
// Default Destructor
}

//______________________________________________________________________________


void EdbShowAlg_CA::Initialize()
{
    return;
}
//______________________________________________________________________________

void EdbShowAlg_CA::Execute()
{
    cout << "EdbShowAlg_CA::Execute()" << endl;
    cout << "EdbShowAlg_CA::Execute   DOING MAIN SHOWER RECONSTRUCTION HERE" << endl;

    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbShowerP* RecoShower=0;

    Bool_t    StillToLoop=kTRUE;
    Int_t     ActualPID;
    Int_t     newActualPID;
    Int_t     STEP=-1;
    Int_t     NLoopedPattern=0;

    if (gEDBDEBUGLEVEL>3) {
        cout << "======================================================"<<endl;
        cout << "  eFirstPlate_eAliPID =  " <<  eFirstPlate_eAliPID  << endl;
        cout << "  eLastPlate_eAliPID =  " <<   eLastPlate_eAliPID << endl;
        cout << "  eNumberPlate_eAliPID =  " <<   eNumberPlate_eAliPID << endl;
        cout << "  eAli->GetPattern(eFirstPlate_eAliPID)->Z()   =  " <<  eAli->GetPattern(eFirstPlate_eAliPID)->Z()   << endl;
        cout << "  eAli->GetPattern(eLastPlate_eAliPID)->Z()   =  " <<  eAli->GetPattern(eLastPlate_eAliPID)->Z()   << endl;
        cout << "  eInBTArrayN =  " <<  eInBTArrayN  << endl;
        cout << "======================================================"<<endl;
    }

    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- STEP for patternloop direction =  " << STEP << endl;


//--- Loop over InBTs:

// Since eInBTArray is filled in ascending ordering by zpositon
// We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

// CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%50)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        if (gEDBDEBUGLEVEL>=4) cout << "Doing  i=" << i << endl;
//-----------------------------------
// 1) Make local_gAli with cut parameters:
//-----------------------------------

// Create new EdbShowerP Object for storage;
// See EdbShowerP why I have to call the Constructor as "unique" ideficable value
        // cout << "Create new EdbShowerP Object for storage."<<endl;
        // cout << i << endl;
        // cout << eAlgValue << endl;
        // cout << eActualAlgParametersetNr << endl;
        RecoShower = new EdbShowerP(i,eAlgValue,eActualAlgParametersetNr);
        // cout << "Create new EdbShowerP Object for storage;...done."<<endl;

// Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();
        Float_t X0=InBT->X();
        Float_t Y0=InBT->Y();

// Add InBT to RecoShower:
// This has to be done, since by definition the first BT in the RecoShower is the InBT.
// Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
        if (gEDBDEBUGLEVEL>4)  cout << "Segment  (InBT) " << Segment << " was added to RecoShower." <<  endl;


// Transform (make size smaller, extract only events having same MC) the  eAli  object:
        Transform_eAli(InBT);
        if (gEDBDEBUGLEVEL>3) eAli_Sub->Print();

        if (gEDBDEBUGLEVEL>2) cout << "  eAli_Sub->GetPattern(eFirstPlate_eAliPID)->Z()   =  " <<  eAli_Sub->GetPattern(eFirstPlate_eAliPID)->Z()   << endl;
        if (gEDBDEBUGLEVEL>2) cout << "  eAli_Sub->GetPattern(eLastPlate_eAliPID)->Z()   =  " <<  eAli_Sub->GetPattern(eLastPlate_eAliPID)->Z()   << endl;


//-----------------------------------
// 2) Loop over (whole) eAli, check BT for Cuts
// eAli_Sub
//-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- --- Doing patterloop id= " << ActualPID << " for pattern with Z position= " << eAli_Sub->GetPattern(ActualPID)->Z() << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<eAli_Sub->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                Segment = (EdbSegP*)eAli_Sub->GetPattern(ActualPID)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();


// Now apply cut conditions: CA ConeAdvanced Alg  --------------------
                if ( Segment->MCEvt() > 0 ) if ( Segment->MCEvt()!=InBT->MCEvt() ) continue; // MCEvtNr (>0) or BgMCNr (-999)
                if ( Abs(Segment->X()-X0) > 7000 ) continue;
                if ( Abs(Segment->Y()-Y0) > 7000 ) continue;
                if ( !GetConeTubeDistanceToInBT(Segment, InBT, eParaValue[0], eParaValue[1]) ) continue;
//  if ( !FindPrecedingBTs(Segment, InBT, eAli, RecoShower)) continue;
                if ( !FindPrecedingBTs(Segment, InBT, eAli_Sub, RecoShower)) continue;
// end of    cut conditions: CA ConeAdvanced Alg  --------------------

// If we arrive here, Basetrack  Segment  has passed criteria
// and is then added to the RecoShower:
// Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
                }
                if (gEDBDEBUGLEVEL>4)  cout << "Segment  " << Segment << " was added at  &Segment : " << &Segment  <<  endl;


            } // of btloop_cnt

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- --- ActualPID= " << newActualPID << "  done. Reconstructed shower has up to now: " << RecoShower->N()  << " Segments." << endl;

//------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- --- newActualPID= " << newActualPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- --- NLoopedPattern= " << NLoopedPattern << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- --- eNumberPlate_eAliPID= " << eNumberPlate_eAliPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- --- StillToLoop= " << StillToLoop << endl;

// This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID) cout << "EdbShowAlg_CA::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
// This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
// This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

// Obligatory when Shower Reconstruction is finished!
        RecoShower->Update();


        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintBasics();
        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintNice();
        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintSegments();

//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- Before adding to array delete the histograms...by finalize() of shower."<<endl;
//     RecoShower ->Finalize();
//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_CA::Execute--- Finalize done..."<<endl;


// Add Shower Object to Shower Reco Array.
// Not, if its empty or containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

//cout << " eRecoShowerArray->GetEntries()  " <<  eRecoShowerArray->GetEntries()   << endl;


// Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


// Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());


    /*
    /// DDEBUG  HOW TO GED ADRESSES OF EALI_SU WHICH WAS DELETED???
    RecoShower=(EdbShowerP*)eRecoShowerArray->At(0);
    Segment=(EdbSegP*)RecoShower->GetSegment(0);
    cout << "segment  " << Segment << " is tested " << &Segment  <<  endl;
    Segment->PrintNice();
    Segment=(EdbSegP*)RecoShower->GetSegment(1);
    cout << "segment  " << Segment << " is tested " << &Segment  <<  endl;
    Segment->PrintNice();
    */


    cout << "EdbShowAlg_CA::eRecoShowerArray() Entries: " << eRecoShowerArray->GetEntries() << endl;
    cout << "EdbShowAlg_CA::Execute()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlg_CA::Finalize()
{
    return;
}

//______________________________________________________________________________
Bool_t EdbShowAlg_CA::GetConeTubeDistanceToInBT(EdbSegP* sa, EdbSegP* InBT, Double_t CylinderRadius, Double_t ConeAngle)
{
// This is an updated version since the TMath::ACos(cosangle) does not really refer to a coneangle:
// We rather use from now on the ROOT implementation of TVector3::Angle()

    TVector3 x1(InBT->X(),InBT->Y(),InBT->Z());
    TVector3 x2(sa->X(),sa->Y(),sa->Z());
    TVector3 direction_x1(InBT->TX()*1300,InBT->TY()*1300,1300);
    TVector3 direction_x2(sa->TX()*1300,sa->TY()*1300,1300);
    TVector3 u1=x2-x1;

    Double_t direction_x1_norm= direction_x1.Mag();
    Double_t cosangle=  (direction_x1*u1)/(u1.Mag()*direction_x1_norm);
    Double_t angle = TMath::ACos(cosangle);  // NO THIS IS NOT THE CONE ANGLE!! (this is rather an old relict)

    TVector3 direction_1(InBT->TX()*1300,InBT->TY()*1300,1300);
    TVector3 direction_2(sa->TX()*1300,sa->TY()*1300,1300);

    /*
    // DEBUG STUFF ... TO BE REMOVED IF EVERYTHING IS CLEAR
    cout <<"------"<<endl;
    cout << "angle =  " << angle << endl;
    cout << "angle V2 =  " << x1.Angle(x2) << endl;
    cout << "angle V3 =  " << direction_x1.Angle(direction_x2) << endl;
    cout << "angle V4 =  " << direction_1.Angle(direction_2) << "   " <<  direction_1.Angle(direction_2)/3.14*180.0 << endl;

    cout << "angle V5   direction_x1.Angle(u1  =  " << direction_x1.Angle(u1) << endl;
    cout << "angle V5   u1.Angle(direction_x1  =  " << u1.Angle(direction_x1) << endl;
    */
    angle=u1.Angle(direction_x1);

// For the case where the two basetracks have same z position
// the angle is about 90 degree so it makes no sense to calculate it...
// therefore we set it artificially to zero:
    if (InBT->Z()==sa->Z() ) {
        angle=0.0;
//if (gEDBDEBUGLEVEL>3) //cout << "same z position, set angle artificially to zero" << endl;
    }

/// Outside if angle greater than ConeAngle (to be fulfilled for Cone and Tube in both cases)
    if (angle>ConeAngle) {
        return kFALSE;
    }

/// if angle smaller than ConeAngle, then you can differ between Tuberadius and CylinderRadius
    Double_t TubeDistance = 1.0/direction_x1_norm  *  ( (x2-x1).Cross(direction_x1) ).Mag();
//   cout << "CylinderRadius= " <<  CylinderRadius << "  TubeDistance= " << TubeDistance << endl;
    if (TubeDistance>CylinderRadius) {
        return kFALSE;
    }

    return kTRUE;
}






//______________________________________________________________________________

Bool_t EdbShowAlg_CA::FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbShowerP* shower)
{
    EdbSegP* s_TestBT;
//       Int_t nentries=shower->GetEntries();
    Int_t nentries=shower->N();
    Double_t dZ;

// In case the shower is empty we do not have to search for a preceeding BT:
    if (nentries==0) return kTRUE;

// We do not test BaseTracks which are before the initiator BaseTrack!
    if (s->Z()<InBT->Z()) {
        cout << "--- --- EdbShowAlg_CA::FindPrecedingBTs(): s->Z()<InBT->Z()..: We do not test BaseTracks which are before the initiator BaseTrack!  return kFALSE;" << endl;
        return kFALSE;
    }

// For the very first Z position we do not test
// if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
// Take 50microns and 80mrad in (dR/dT) around.
// This does not affect the normal results, but helps for
// events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
//cout << DeltaTheta(s, InBT) << "  " << DeltaR_WithPropagation(s, InBT) << endl;
        if (DeltaTheta(s, InBT) < 0.08 && DeltaR_WithoutPropagation(s, InBT) < 50.0 ) return kTRUE;
    }

    if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_CA::FindPrecedingBTs(): Testing " << nentries << " entries of the shower." << endl;

    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( shower->GetSegment(i) );

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_CA::FindPrecedingBTs(): Do  s_TestBT->ID()  s->ID()  s_TestBT->MCEvt() s_TestBT->Z()  s->Z() "<< s_TestBT->ID() << " " << s->ID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << "  " <<  s->Z()  <<endl;
        if (gEDBDEBUGLEVEL>3) s_TestBT->PrintNice();
        if (gEDBDEBUGLEVEL>3) s->PrintNice();


        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(3*1300.0)+30.0) continue;     // Exclude the case of more than 4 plates before...

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_CA::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaTheta(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << endl;

        if (DeltaTheta(s, s_TestBT) >  eParaValue[3] ) continue;
        if (DeltaR_WithPropagation(s, s_TestBT) > eParaValue[2]) continue;

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_CA::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaTheta(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << "   ok!"<<endl;
        return kTRUE;
    }
//---------------------------------------------
    return kFALSE;
}






























//______________________________________________________________________________

EdbShowAlg_OI::EdbShowAlg_OI()
{
// Default Constructor
    cout << "EdbShowAlg_OI::EdbShowAlg_OI()   Default Constructor  (OfficialImplementation, as it is in libShower module."<<endl;

// Reset all:
    Set0();

    eAlgName="OI";
    eAlgValue=4; // see default.par_SHOWREC for labeling (lableing identical with ShowRec program)

    eParaValue[0]=700.0;
    eParaString[0]="CylinderRadius";  // micrometer
    eParaValue[1]=0.025;
    eParaString[1]="ConeAngle";       // MILLIradiant
    eParaValue[2]=150.0;
    eParaString[2]="DeltaRToPreceedingBT";  // micrometer
    eParaValue[3]=0.13;
    eParaString[3]="DeltaThetaComponentwiseToPreceedingBT";  // radiant

// 700  0.025  150  0.13
}

//______________________________________________________________________________

EdbShowAlg_OI::~EdbShowAlg_OI()
{
    cout << "EdbShowAlg_OI::~EdbShowAlg_OI()"<<endl;
// Default Destructor
}

//______________________________________________________________________________


void EdbShowAlg_OI::Initialize()
{
    return;
}
//______________________________________________________________________________

void EdbShowAlg_OI::Execute()
{
    cout << "EdbShowAlg_OI::Execute()" << endl;
    cout << "EdbShowAlg_OI::Execute   DOING MAIN SHOWER RECONSTRUCTION HERE" << endl;

    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbShowerP* RecoShower;

    Bool_t    StillToLoop=kTRUE;
    Int_t     ActualPID;
    Int_t     newActualPID;
    Int_t     STEP=-1;
    Int_t     NLoopedPattern=0;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- STEP for patternloop direction =  " << STEP << endl;

//--- Loop over InBTs:
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_OI::Execute    Loop over InBTs:" << endl;


    cout << "============DEBGU     eUseAliSub =  " << eUseAliSub << endl;
//   return;



// Since eInBTArray is filled in ascending ordering by zpositon
// We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

// CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%100)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

//-----------------------------------
// 1) Make local_gAli with cut parameters:
//-----------------------------------

// Create new EdbShowerP Object for storage;
// See EdbShowerP why I have to call the Constructor as "unique" ideficable value
        RecoShower = new EdbShowerP(i,eAlgValue,eActualAlgParametersetNr);

// Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();
        Float_t X0=InBT->X();
        Float_t Y0=InBT->Y();

// Add InBT to RecoShower:
// This has to be done, since by definition the first BT in the RecoShower is the InBT.
// Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
        if (gEDBDEBUGLEVEL>4)  cout << "Segment  (InBT) " << Segment << " was added to RecoShower." <<  endl;

// Transform (make size smaller, extract only events having same MC) the  eAli  object:
        Transform_eAli(InBT);
        if (gEDBDEBUGLEVEL>3) eAli_Sub->Print();

//-----------------------------------
// 2) Loop over (whole) eAli, check BT for Cuts
// eAli_Sub
//-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- --- Doing patterloop " << ActualPID << " for patterns Z position=" << eAli_Sub->GetPattern(ActualPID)->Z() << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<eAli_Sub->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                Segment = (EdbSegP*)eAli_Sub->GetPattern(ActualPID)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();


// Now apply cut conditions: OI OfficialImplementation Alg  --------------------
                if (Segment->MCEvt() > 0 ) if ( Segment->MCEvt()!=InBT->MCEvt() ) continue; // MCEvtNr (>0) or BgMCNr (-999)
                if ( Abs(Segment->X()-X0) > 7000 ) continue;
                if ( Abs(Segment->Y()-Y0) > 7000 ) continue;
                if ( !IsInConeTube(Segment, InBT, eParaValue[0], eParaValue[1]) ) continue;
                if ( !FindPrecedingBTs(Segment, InBT, eAli_Sub, RecoShower)) continue;
// end of    cut conditions: OI OfficialImplementation Alg  --------------------

// If we arrive here, Basetrack  Segment  has passed criteria
// and is then added to the RecoShower:
// Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
                }
                if (gEDBDEBUGLEVEL>4)  cout << "Segment  " << Segment << " was added at  &Segment : " << &Segment  <<  endl;

            } // of btloop_cnt

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- --- ActualPID= " << newActualPID << "  done. Reconstructed shower has up to now: " << RecoShower->N()  << " Segments." << endl;

//------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- --- newActualPID= " << newActualPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- --- NLoopedPattern= " << NLoopedPattern << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- --- eNumberPlate_eAliPID= " << eNumberPlate_eAliPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- --- StillToLoop= " << StillToLoop << endl;

// This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID) cout << "EdbShowAlg_OI::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
// This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
// This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

// Obligatory when Shower Reconstruction is finished:
        RecoShower ->Update();

        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintBasics();
        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintNice();
        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintSegments();

//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- Before adding to array delete the histograms...by finalize() of shower."<<endl;
//     RecoShower ->Finalize();
//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::Execute--- Finalize done..."<<endl;


// Add Shower Object to Shower Reco Array.
// Not, if its empty or containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

//cout << " eRecoShowerArray->GetEntries()  " <<  eRecoShowerArray->GetEntries()   << endl;


// Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


// Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    cout << "EdbShowAlg_OI::eRecoShowerArray() Entries: " << eRecoShowerArray->GetEntries() << endl;
    cout << "EdbShowAlg_OI::Execute()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlg_OI::Finalize()
{
    return;
}

//______________________________________________________________________________








//______________________________________________________________________________

Bool_t EdbShowAlg_OI::FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbShowerP* shower)
{
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_OI::FindPrecedingBTs   Find BTs to be connected with the Test BT:" << endl;

    EdbSegP* s_TestBT;
    Int_t nentries=shower->N();
    Double_t dZ;

// In case the shower is empty we do not have to search for a preceeding BT:
    if (nentries==0) return kTRUE;

// We do not test BaseTracks which are before the initiator BaseTrack!
    if (s->Z()<InBT->Z()) {
        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_OI::FindPrecedingBTs(): s->Z()<InBT->Z()..: We do not test BaseTracks which are before the initiator BaseTrack!  return kFALSE;" << endl;
        return kFALSE;
    }

// For the very first Z position we do not test
// if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
// Take 50microns and 80mrad in (dR/dT) around.
// This does not affect the normal results, but helps for
// events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
//cout << DeltaThetaComponentwise(s, InBT) << "  " << DeltaR_WithPropagation(s, InBT) << endl;
        if (DeltaThetaComponentwise(s, InBT) < 0.08 && DeltaR_WithoutPropagation(s, InBT) < 50.0 ) return kTRUE;
    }

    if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_OI::FindPrecedingBTs(): Testing " << nentries << " entries of the shower." << endl;

    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( shower->GetSegment(i) );

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_OI::FindPrecedingBTs(): Do  s_TestBT->ID()  s->ID()  s_TestBT->MCEvt() s_TestBT->Z()  s->Z() "<< s_TestBT->ID() << " " << s->ID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << "  " <<  s->Z()  <<endl;
        if (gEDBDEBUGLEVEL>3) s_TestBT->PrintNice();
        if (gEDBDEBUGLEVEL>3) s->PrintNice();


        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(3*1300.0)+30.0) continue;     // Exclude the case of more than 3 plates before...

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_OI::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaThetaComponentwise(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << endl;

        if (DeltaThetaComponentwise(s, s_TestBT) >  eParaValue[3] ) continue;
        if (DeltaR_WithPropagation(s, s_TestBT) > eParaValue[2]) continue;

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_OI::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaThetaComponentwise(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << "   ok!"<<endl;
        return kTRUE;
    }
//---------------------------------------------
    return kFALSE;
}










































//______________________________________________________________________________

EdbShowAlg_RC::EdbShowAlg_RC()
{
// Default Constructor
    cout << "EdbShowAlg_RC::EdbShowAlg_RC()   Default Constructor  Recursive Cone."<<endl;

// Reset all:
    Set0();

    eAlgName="RC";
    eAlgValue=7; // see default.par_SHOWREC for labeling (labeling identical with ShowRec program)

    eParaValue[0]=700.0;
    eParaString[0]="CylinderRadius";  // micrometer
    eParaValue[1]=0.025;
    eParaString[1]="ConeAngle";       // radiant
    eParaValue[2]=150.0;
    eParaString[2]="DeltaRToPreceedingBT";  // micrometer
    eParaValue[3]=0.13;
    eParaString[3]="DeltaThetaComponentwiseToPreceedingBT";  // radiant
}

//______________________________________________________________________________

EdbShowAlg_RC::~EdbShowAlg_RC()
{
    cout << "EdbShowAlg_RC::~EdbShowAlg_RC()"<<endl;
// Default Destructor
}

//______________________________________________________________________________


void EdbShowAlg_RC::Initialize()
{
    return;
}
//______________________________________________________________________________

void EdbShowAlg_RC::Execute()
{
    cout << "EdbShowAlg_RC::Execute()" << endl;
    cout << "EdbShowAlg_RC::Execute   DOING MAIN SHOWER RECONSTRUCTION HERE" << endl;

    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbShowerP* RecoShower;

    Bool_t    StillToLoop=kTRUE;
    Int_t     ActualPID;
    Int_t     newActualPID;
    Int_t     STEP=-1;
    Int_t     NLoopedPattern=0;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_SA::Execute--- STEP for patternloop direction =  " << STEP << endl;

//--- Loop over InBTs:
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_RC::Execute    Loop over InBTs:" << endl;



// Since eInBTArray is filled in ascending ordering by zpositon
// We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

// CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%100)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

//-----------------------------------
// 1) Make local_gAli with cut parameters:
//-----------------------------------

// Create new EdbShowerP Object for storage;
// See EdbShowerP why I have to call the Constructor as "unique" ideficable value
        RecoShower = new EdbShowerP(i,eAlgValue,eActualAlgParametersetNr);

// Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();
        Float_t X0=InBT->X();
        Float_t Y0=InBT->Y();

// Add InBT to RecoShower:
// This has to be done, since by definition the first BT in the RecoShower is the InBT.
// Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
        if (gEDBDEBUGLEVEL>4)  cout << "Segment  (InBT) " << Segment << " was added to RecoShower." <<  endl;

// Transform (make size smaller, extract only events having same MC) the  eAli  object:
        Transform_eAli(InBT);
        if (gEDBDEBUGLEVEL>3) eAli_Sub->Print();

//-----------------------------------
// 2) Loop over (whole) eAli, check BT for Cuts
// eAli_Sub
//-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- --- Doing patterloop " << ActualPID << " for patterns Z position=" << eAli_Sub->GetPattern(ActualPID)->Z() << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<eAli_Sub->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                Segment = (EdbSegP*)eAli_Sub->GetPattern(ActualPID)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();


// Now apply cut conditions: RC RECURSIVE CONE Alg  --------------------
                if (Segment->MCEvt() > 0 ) if ( Segment->MCEvt()!=InBT->MCEvt() ) continue; // MCEvtNr (>0) or BgMCNr (-999)
                if ( Abs(Segment->X()-X0) > 7000 ) continue;
                if ( Abs(Segment->Y()-Y0) > 7000 ) continue;
                if (!GetConeOrTubeDistanceToBTOfShowerArray(Segment, InBT, RecoShower, eParaValue[0], eParaValue[1])) continue;
                if (!FindPrecedingBTs(Segment, InBT, eAli_Sub, RecoShower)) continue;
// end of    cut conditions: RC RECURSIVE CONE Alg  --------------------


// If we arrive here, Basetrack  Segment  has passed criteria
// and is then added to the RecoShower:
// Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
                }
                if (gEDBDEBUGLEVEL>4)  cout << "Segment  " << Segment << " was added at  &Segment : " << &Segment  <<  endl;

            } // of btloop_cnt

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- --- ActualPID= " << newActualPID << "  done. Reconstructed shower has up to now: " << RecoShower->N()  << " Segments." << endl;

//------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- --- newActualPID= " << newActualPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- --- NLoopedPattern= " << NLoopedPattern << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- --- eNumberPlate_eAliPID= " << eNumberPlate_eAliPID << endl;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- --- StillToLoop= " << StillToLoop << endl;

// This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID) cout << "EdbShowAlg_RC::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
// This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
// This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID && gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

// Obligatory when Shower Reconstruction is finished:
        RecoShower ->Update();

        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintBasics();
        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintNice();
        if (gEDBDEBUGLEVEL>3) RecoShower ->PrintSegments();

//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- Before adding to array delete the histograms...by finalize() of shower."<<endl;
//     RecoShower ->Finalize();
//     if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::Execute--- Finalize done..."<<endl;


// Add Shower Object to Shower Reco Array.
// Not, if its empty or containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

//cout << " eRecoShowerArray->GetEntries()  " <<  eRecoShowerArray->GetEntries()   << endl;


// Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


// Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    cout << "EdbShowAlg_RC::eRecoShowerArray() Entries: " << eRecoShowerArray->GetEntries() << endl;
    cout << "EdbShowAlg_RC::Execute()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowAlg_RC::Finalize()
{
    return;
}

//______________________________________________________________________________








//______________________________________________________________________________

Bool_t EdbShowAlg_RC::FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbShowerP* shower)
{
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlg_RC::FindPrecedingBTs   Find BTs to be connected with the Test BT:" << endl;

    EdbSegP* s_TestBT;
    Int_t nentries=shower->N();
    Double_t dZ;

// In case the shower is empty we do not have to search for a preceeding BT:
    if (nentries==0) return kTRUE;

// We do not test BaseTracks which are before the initiator BaseTrack!
    if (s->Z()<InBT->Z()) {
        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_RC::FindPrecedingBTs(): s->Z()<InBT->Z()..: We do not test BaseTracks which are before the initiator BaseTrack!  return kFALSE;" << endl;
        return kFALSE;
    }

// For the very first Z position we do not test
// if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
// Take 50microns and 80mrad in (dR/dT) around.
// This does not affect the normal results, but helps for
// events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
//cout << DeltaThetaComponentwise(s, InBT) << "  " << DeltaR_WithPropagation(s, InBT) << endl;
        if (DeltaThetaComponentwise(s, InBT) < 0.08 && DeltaR_WithoutPropagation(s, InBT) < 50.0 ) return kTRUE;
    }

    if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_RC::FindPrecedingBTs(): Testing " << nentries << " entries of the shower." << endl;

    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( shower->GetSegment(i) );

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_RC::FindPrecedingBTs(): Do  s_TestBT->ID()  s->ID()  s_TestBT->MCEvt() s_TestBT->Z()  s->Z() "<< s_TestBT->ID() << " " << s->ID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << "  " <<  s->Z()  <<endl;
        if (gEDBDEBUGLEVEL>3) s_TestBT->PrintNice();
        if (gEDBDEBUGLEVEL>3) s->PrintNice();


        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(3*1300.0)+30.0) continue;     // Exclude the case of more than 3 plates before...

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_RC::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaThetaComponentwise(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << endl;

        if (DeltaThetaComponentwise(s, s_TestBT) >  eParaValue[3] ) continue;
        if (DeltaR_WithPropagation(s, s_TestBT) > eParaValue[2]) continue;

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowAlg_RC::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaThetaComponentwise(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << "   ok!"<<endl;
        return kTRUE;
    }
//---------------------------------------------
    return kFALSE;
}



//______________________________________________________________________________

Bool_t EdbShowAlg_RC::GetConeOrTubeDistanceToBTOfShowerArray(EdbSegP* sa, EdbSegP* InBT, EdbShowerP* shower, Double_t CylinderRadius, Double_t ConeAngle)
{
    Bool_t isTrueForBT=kFALSE;
    Int_t lastI=0;
    Double_t factor=1.0;

    EdbSegP* s_TestBT;
    Int_t nentries=shower->GetNBT();

// Now call IsInConeTube for every BT which was reconstructed up to now in the shower:
    for (Int_t i=0; i<nentries; i++) {
        s_TestBT = (EdbSegP*)( shower->GetSegment(i) );
// Dont check the BT which is before or same Z as the BaseTrack(i) position:
// But ---not--- for the first Basetrack. There we allow it!
//if (s_TestBT->Z()>=sa->Z() && i>0) continue;
        if (s_TestBT->Z()>sa->Z() ) continue;
        lastI=i;
        if (i==0) {
            factor=1.0;
        }
        else {
            factor=3.0;
        }

//     Bool_t EdbShowAlg::IsInConeTube(EdbSegP* TestingSegment, EdbSegP* StartingSegment, Double_t CylinderRadius, Double_t ConeAngle)
//     if (GetConeOrTubeDistanceToInBT(sa, s_TestBT, factor*CylinderRadius, factor*ConeAngle)==kTRUE) {
        if (IsInConeTube(sa, s_TestBT, factor*CylinderRadius, factor*ConeAngle)==kTRUE) {
            isTrueForBT=kTRUE;
            break;
        }
    }

    if (isTrueForBT) {
//if (lastI>0) cout <<"for i= " << lastI<< " return true with factor "<< factor << endl;
        return kTRUE;
    }

    return kFALSE;
}


//______________________________________________________________________________

Double_t EdbShowAlg::GetMinimumDist(EdbSegP* seg1,EdbSegP* seg2)
{

// calculate minimum distance of 2 lines.
// use the data of (the selected object)->X(), Y(), Z(), TX(), TY().
// means, if the selected object == segment, use the data of the segment.
// or it == track, the // use the fitted data.
// original code from Tomoko Ariga
// double calc_dmin(EdbSegP *seg1, EdbSegP *seg2, double *dminz = NULL){

    double x1,y1,z1,ax1,ay1;
    double x2,y2,z2,ax2,ay2;
    double s1,s2,s1bunsi,s1bunbo,s2bunsi,s2bunbo;
    double p1x,p1y,p1z,p2x,p2y,p2z,p1p2;

    if (seg1->ID()==seg2->ID()&&seg1->PID()==seg2->PID()) return 0.0;

    x1 = seg1->X();
    y1 = seg1->Y();
    z1 = seg1->Z();
    ax1= seg1->TX();
    ay1= seg1->TY();

    x2 = seg2->X();
    y2 = seg2->Y();
    z2 = seg2->Z();
    ax2= seg2->TX();
    ay2= seg2->TY();

    s1bunsi=(ax2*ax2+ay2*ay2+1)*(ax1*(x2-x1)+ay1*(y2-y1)+z2-z1) - (ax1*ax2+ay1*ay2+1)*(ax2*(x2-x1)+ay2*(y2-y1)+z2-z1);
    s1bunbo=(ax1*ax1+ay1*ay1+1)*(ax2*ax2+ay2*ay2+1) - (ax1*ax2+ay1*ay2+1)*(ax1*ax2+ay1*ay2+1);
    s2bunsi=(ax1*ax2+ay1*ay2+1)*(ax1*(x2-x1)+ay1*(y2-y1)+z2-z1) - (ax1*ax1+ay1*ay1+1)*(ax2*(x2-x1)+ay2*(y2-y1)+z2-z1);
    s2bunbo=(ax1*ax1+ay1*ay1+1)*(ax2*ax2+ay2*ay2+1) - (ax1*ax2+ay1*ay2+1)*(ax1*ax2+ay1*ay2+1);
    s1=s1bunsi/s1bunbo;
    s2=s2bunsi/s2bunbo;
    p1x=x1+s1*ax1;
    p1y=y1+s1*ay1;
    p1z=z1+s1*1;
    p2x=x2+s2*ax2;
    p2y=y2+s2*ay2;
    p2z=z2+s2*1;
    p1p2=sqrt( (p1x-p2x)*(p1x-p2x)+(p1y-p2y)*(p1y-p2y)+(p1z-p2z)*(p1z-p2z) );

    return p1p2;
}


//______________________________________________________________________________