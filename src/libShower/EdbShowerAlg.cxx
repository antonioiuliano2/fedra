#include "EdbShowerAlg.h"
// #include "EdbShowerP.h"

using namespace std;
using namespace TMath;

ClassImp(EdbShowerAlg)
ClassImp(EdbShowerAlg_GS)
ClassImp(EdbShowerAlgESimple)
ClassImp(EdbShowerAlgIDSimple)


//______________________________________________________________________________



EdbShowerAlg::EdbShowerAlg()
{
    // Default Constructor
    cout << "EdbShowerAlg::EdbShowerAlg()   Default Constructor"<<endl;

    // Reset all:
    Set0();
}

//______________________________________________________________________________

EdbShowerAlg::EdbShowerAlg(TString AlgName, Int_t AlgValue)
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

EdbShowerAlg::~EdbShowerAlg()
{
    // Default Destructor
    cout << "EdbShowerAlg::~EdbShowerAlg()"<<endl;
}

//______________________________________________________________________________


void EdbShowerAlg::Set0()
{
    // Set0()
    eAlgName="UNSPECIFIED";
    eAlgValue=-999999;
    for (int i=0; i<10; ++i) {
        eParaValue[i]=-99999.0;
        eParaString[i]="UNSPECIFIED";
    }
    eAli_Sub=0;

    // do not use use small eAli Object by default:
    // (this solution is memory safe....)
    eUseAliSub=0;

    eActualAlgParametersetNr=0;

    eRecoShowerArrayN=0;
    eRecoShowerArray=NULL;
    eInBTArrayN=0;
    eInBTArray=NULL;
}

//______________________________________________________________________________

void EdbShowerAlg::SetParameters(Float_t* par)
{
    // SetParameters
    for (int i=0; i<10; ++i) {
        eParaValue[i]=par[i];
    }
    cout << "EdbShowerAlg::SetParameters()...done"<<endl;
}

//______________________________________________________________________________







//______________________________________________________________________________

void EdbShowerAlg::Transform_eAli(EdbSegP* InitiatorBT, Float_t ExtractSize=1500)
{
// 	cout << "-----------------   void EdbShowerAlg::Transform_eAli(EdbSegP* InitiatorBT, Float_t ExtractSize=1500)   ------------------"<<endl;
    // 		Transform eAli to eAli_Sub:
    // 		the lenght of eAli_Sub is not changed.
    // 		Only XY-size (and MC) cuts are applied.

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
    // ---	For few InBTs (EDA use for data...): 		use eAliSub and dont delete it.
    // ---	For many InBTs (parameterstudies):			use eAli---.
    // ---	Use always eAliSub, search BT correspond in eAli, add BT from eAli....May take also long time...
    // ---
    // ---
    // ---
    // ---
    // --------------------------------------------------------------------------------------


    /*
    cout << "void EdbShowerAlg::Transform_eAli()   SEVERE WARNING:  IF gAli is in wrong order it can be that no showers " << endl;
    cout << "void EdbShowerAlg::Transform_eAli()   SEVERE WARNING:  are reconstructed since most implemented algorithms " << endl;
    cout << "void EdbShowerAlg::Transform_eAli()   SEVERE WARNING:  rely on the order that plate 2 comes directly behind " << endl;
    cout << "void EdbShowerAlg::Transform_eAli()   SEVERE WARNING:  the InBT.   /// DEBUG  TODO  // DEBUG TODO " << endl;
    */


// 	gEDBDEBUGLEVEL=3;
// 	cout << "void EdbShowerAlg::Transform_eAli()     ===== eUseAliSub "  << eUseAliSub << endl;

    // IF TO RECREATE THE gALI_SUB in RECONSTRUCTION  or if to use gAli global (slowlier but maybe memory leak safe).
    if (!eUseAliSub)
    {
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlg::Transform_eAli   UseAliSub==kFALSE No new eAli_Sub created. Use eAli instead. "<<endl;
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
// 	eAli_Sub = new EdbPVRec();


    if (eUseAliSub) {
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlg::Transform_eAli   UseAliSub==kTRUE Will now create new eAli_Sub. "<<endl;

/// 		if (eAli_Sub) { delete eAli_Sub;eAli_Sub=0;} Try not to delete it maybe then it works.....
        eAli_Sub = new EdbPVRec();
// 		cout << "Adress of eAli_Sub " << eAli_Sub << endl;
// 		cout << "eAli_Sub->Npatterns();  " << eAli_Sub->Npatterns() << endl;
// 		eAli_Sub->Print();
    }



// 	cout << "PROBLE THRANFOMING, CAUSE ADRESES ARE DELETED ALSO...." << endl;
//  	cout << "TEMPORAY SOLUTION:     comment the delete 	eAli_Sub  ( will lead to large memory consumption when run for long time" << endl;


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
        cout << "EdbShowerAlg::Transform_eAli   eAli_Sub  created."<<endl;
        cout << "Adress of eAli_Sub " << eAli_Sub << endl;
        cout << "eAli_Sub->Npatterns();  " << eAli_Sub->Npatterns() << endl;
    }
    return;
}



//______________________________________________________________________________


Bool_t EdbShowerAlg::IsInConeTube(EdbSegP* TestingSegment, EdbSegP* StartingSegment, Double_t CylinderRadius, Double_t ConeAngle)
{
    // General Function which returns Bool if the Testing BaeTrack is in a cone defined
    // by the StartingBaseTrack. In case of starting same Z position, a distance cut of
    // 20microns is assumed....
    // In case of  TestingSegment==StartingSegment this function should correctly return kTRUE also...
    if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube() Test Segment " << TestingSegment << " vs. Starting Segment " << StartingSegment << endl;

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

void EdbShowerAlg::Initialize()
{

    return;
}
//______________________________________________________________________________

void EdbShowerAlg::Execute()
{
    cout << "EdbShowerAlg::Execute()-----------------------------------------------------------------------" << endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlg::Finalize()
{

    return;
}

//______________________________________________________________________________

void EdbShowerAlg::Print()
{
    cout << "EdbShowerAlg::Print()" << endl;
    cout << eAlgName << "  ;  AlgValue=  " <<  eAlgName << "  ." << endl;
    for (int i=0; i<10; i++) cout << eParaString[i] << "=" << eParaValue[i] << ";  ";
    cout << eFirstPlate_eAliPID << "  " <<  eLastPlate_eAliPID << "  " << eMiddlePlate_eAliPID << "  " << eNumberPlate_eAliPID << "  " << endl;
    cout << "UseAliSub=  " <<  eUseAliSub << "  ." << endl;
    cout << "EdbShowerAlg::Print()...done." << endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlg::PrintParameters()
{
    cout << "EdbShowerAlg::PrintParameters()" << endl;
    cout << eAlgName<< " :" << endl;
    for (int i=0; i<5; i++) cout << setw(6) << eParaString[i];
    cout << endl;
    for (int i=0; i<5; i++) cout << setw(6) << eParaValue[i];
    cout <<  " ."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlg::PrintParametersShort()
{
    cout << eAlgName<< " :";
    for (int i=0; i<5; i++) cout << setw(6) << eParaValue[i];
    cout <<  " ."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlg::PrintMore()
{
    cout << "EdbShowerAlg::PrintMore()" << endl;
    cout << "eInBTArray->GetEntries();=  " <<  eInBTArray->GetEntries() << "  ." << endl;
    cout << "EdbShowerAlg::PrintMore()...done." << endl;
    return;
}

//______________________________________________________________________________


void EdbShowerAlg::PrintAll()
{
    return;
}


//______________________________________________________________________________


Double_t EdbShowerAlg::DeltaR_NoPropagation(EdbSegP* s,EdbSegP* stest)
{
    // SAME function as DeltaR_WithoutPropagation !!!
    return DeltaR_WithoutPropagation(s,stest);
}

//______________________________________________________________________________

Double_t EdbShowerAlg::DeltaR_WithoutPropagation(EdbSegP* s,EdbSegP* stest)
{
    return TMath::Sqrt((s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()));
}

//______________________________________________________________________________

Double_t EdbShowerAlg::DeltaR_WithPropagation(EdbSegP* s,EdbSegP* stest)
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

Double_t EdbShowerAlg::DeltaTheta(EdbSegP* s1,EdbSegP* s2)
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


Double_t EdbShowerAlg::DeltaThetaComponentwise(EdbSegP* s1,EdbSegP* s2)
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
Double_t EdbShowerAlg::DeltaThetaSingleAngles(EdbSegP* s1,EdbSegP* s2)
{
    // SAME function as DeltaThetaComponentwise !!!
    return DeltaThetaComponentwise(s1,s2);
}

//______________________________________________________________________________

Double_t EdbShowerAlg::GetSpatialDist(EdbSegP* s1,EdbSegP* s2)
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


Double_t EdbShowerAlg::GetMinimumDist(EdbSegP* seg1,EdbSegP* seg2) {
    // calculate minimum distance of 2 lines.
    // use the data of (the selected object)->X(), Y(), Z(), TX(), TY().
    // means, if the selected object == segment, use the data of the segment. or it == track, the use the fitted data.
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
//______________________________________________________________________________



//______________________________________________________________________________

EdbShowerAlg_GS::EdbShowerAlg_GS()
{
    // Default Constructor
    Log(2,"EdbShowerAlg_GS::EdbShowerAlg_GS","EdbShowerAlg_GS:: Default Constructor");


    cout << "Howto do:  " << endl;
    cout << "	Alg = new EdbShowerAlg_GS();" << endl;
    cout << "	Alg->SetEdbPVRec(EdbPVRec*);" << endl;
    cout << "	Alg->SetInVtx(EdbVertex*)" << endl;
    cout << "	Alg->Execute();" << endl;
    cout << "	Alg->GetRecoShowerArray();   .... Returns you the compatible Pair Segments (stored as EdbTrackP*)" << endl;
    cout << "	" << endl;


    // Reset all:
    Set0();

    eAlgName="GS";
    eAlgValue=999; // see default.par_SHOWREC for labeling (labeling identical with ShowRec program)

    eInVtxArrayN=0;
    eInVtxArray=NULL;

    eInBTArrayN=0;
    eInBTArray=NULL;

    //  Init with values according to GS Alg:
    Init();



}

//______________________________________________________________________________

EdbShowerAlg_GS::~EdbShowerAlg_GS()
{
    // Default Destructor
    Log(2,"EdbShowerAlg_GS::~EdbShowerAlg_GS","EdbShowerAlg_GS:: Default Destructor");
}

//______________________________________________________________________________

void EdbShowerAlg_GS::Init()
{
    Log(2,"EdbShowerAlg_GS::EdbShowerAlg_GS","Init()");

    //  Init with values according to GS Alg:
    //  Took over from "FindGamma.C" script I develoved before:
    // IP CUT; this cut is used for the -better- IP of both BTs to Vertex/BT
    eParaValue[0]=250;
    eParaString[0]="PARA_GS_CUT_dIP";
    // min minDist.e between pair BTs
    eParaValue[1]=40;
    eParaString[1]="PARA_GS_CUT_dMin";
    // min dR between pair BTs
    eParaValue[2]=60;
    eParaString[2]="PARA_GS_CUT_dR";
    // max Z distance between pair BTs and Vertex/BT
    eParaValue[3]=19000;
    eParaString[3]="PARA_GS_CUT_dZ";
    // max Angle between pair BTs
    eParaValue[4]=0.060;
    eParaString[4]="PARA_GS_CUT_dtheta";
    // max plates difference between pair BTs
    eParaValue[5]=0;
    eParaString[5]="PARA_GS_CUT_PIDDIFF";


    if (!eRecoShowerArray) eRecoShowerArray= new TObjArray(999);
    eRecoShowerArrayN=0;
    return;
}

//______________________________________________________________________________


void EdbShowerAlg_GS::Initialize()
{
    Log(2,"EdbShowerAlg_GS::EdbShowerAlg_GS","Initialize()");
    return;
}

//______________________________________________________________________________




void EdbShowerAlg_GS::SetInVtx( EdbVertex* vtx )
{
    Log(2,"EdbShowerAlg_GS::SetInVtx","SetInVtx()");
    if (!eInVtxArray)  eInVtxArray = new TObjArray();
    eInVtxArray->Add(vtx);
    ++eInVtxArrayN;
    return;
}



//______________________________________________________________________________


void EdbShowerAlg_GS::Convert_InVtxArray_To_InBTArray()
{
    Log(2,"EdbShowerAlg_GS::EdbShowerAlg_GS","Convert_InVtxArray_To_InBTArray()");
    if (eInBTArray!=NULL) {
        eInBTArray->Clear();
        cout << " eInBTArray->Clear();" << endl;
    }
    if (eInBTArray==NULL) {
        eInBTArray = new TObjArray();
        cout << " eInBTArray = new TObjArray()" << endl;
    }

    if (eInVtxArray==NULL) cout << "NO  eInVtxArray  " << endl;
    EdbVertex* vtx;
    cout << "EdbVertex* vtx;" << endl;
    cout <<eInVtxArrayN << endl;
    cout << "now eInVtxArray->Print();" << endl;
    eInVtxArray->Print();
    cout << eInVtxArray->GetEntries() << endl;

    Log(2,"EdbShowerAlg_GS::EdbShowerAlg_GS","Convert_InVtxArray_To_InBTArray()...start loop");
    for (Int_t i=0; i<eInVtxArrayN; i++) {
        vtx= (EdbVertex*)eInVtxArray->At(i);
        //vtx->Print();
        EdbSegP* seg = new EdbSegP(i,vtx->X(),vtx->Y(),0,0);
        seg->SetZ(vtx->Z());
        // vtx can have by default initialization MCEvt==0,
        // this we dont want, in case: we set MCEvt of vtx from 0 to -999
        if (vtx->MCEvt()==0) vtx->SetMC(-999);
        seg->SetMC(vtx->MCEvt(),vtx->MCEvt());
        seg->SetFlag(0);
        //seg->Print();
        eInBTArray->Add(seg);
    }

    eInBTArrayN=eInBTArray->GetEntries();
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlg_GS::Convert_InVtxArray_To_InBTArray   Converted " << eInBTArrayN << "InVtx to InBT." << endl;
    Log(2,"EdbShowerAlg_GS::EdbShowerAlg_GS","Convert_InVtxArray_To_InBTArray()...done.");
    return;
}

//______________________________________________________________________________


Bool_t EdbShowerAlg_GS::CheckPairDuplications(Int_t SegPID,Int_t SegID,Int_t Seg2PID,Int_t Seg2ID,TArrayI* SegmentPIDArray,TArrayI* SegmentIDArray,TArrayI* Segment2PIDArray,TArrayI* Segment2IDArray)
{

    for (Int_t i=0; i<eRecoShowerArrayN; i++) {
        // PID and ID of Seg and Seg2 to be exchanged for duplications
        if ( SegPID==Segment2PIDArray->At(i) && Seg2PID==SegmentPIDArray->At(i) && SegID==Segment2IDArray->At(i) && Seg2ID==SegmentIDArray->At(i)) {
            //cout << "Found duplocation for... return true"<<endl;
            return kTRUE;
        }
    }
    return kFALSE;
}


//______________________________________________________________________________



void EdbShowerAlg_GS::Execute()
{
    Log(2,"EdbShowerAlg_GS::Execute","Execute()");
    cout << "EdbShowerAlg_GS::Execute()" << endl;
    Log(2,"EdbShowerAlg_GS::Execute","Execute()   DOING MAIN SHOWER RECONSTRUCTION HERE");


    if (!eInBTArray) {
        Log(2,"EdbShowerAlg_GS::Execute","Execute()   No eInBTArray. Check for eInVtxArray:");
        // Check if there is an Convert_InVtxArray_To_InBTArray();
        if (!eInVtxArray) {
            Log(2,"EdbShowerAlg_GS::Execute","Execute()   No eInVtxArray. RETURN.");
            return;
        }
        cout << "eInVtxArray there. Converting to InBTArray() now. "<< endl;
        Convert_InVtxArray_To_InBTArray();
    }

    EdbSegP* InBT=NULL;

//   EdbShowerP* RecoShower;
    EdbTrackP* RecoShower;

    Int_t     STEP=-1;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlg_GS::Execute--- STEP for patternloop direction =  " << STEP << endl;
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlg_GS::Execute--- eRecoShowerArrayN =  " << eRecoShowerArrayN << endl;

    //--- Loop over InBTs:
    if (gEDBDEBUGLEVEL>=2) cout << "EdbShowerAlg_GS::Execute    Loop over InBTs:" << endl;

    // Since eInBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    // InBT doest have to be necessary a real BaseTrack, it can also be a vertex (with its positions and angle zero) !!!
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%100)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;

        // Get InitiatorBT from eInBTArray  InBT
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) InBT->PrintNice();

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        // Transform (make size smaller, extract only events having same MC) the  eAli  object:
        Transform_eAli(InBT,999999);
        if (gEDBDEBUGLEVEL>2)  eAli_Sub->Print();


        //-----------------------------------
        // 2) FindPairs
        //-----------------------------------
        TObjArray* Pairs = FindPairs(InBT,eAli_Sub);
        if (gEDBDEBUGLEVEL>2) cout << "TObjArray* Pairs = FindPairs(InBT,eAli_Sub); done. Entries= " << Pairs->GetEntries() << endl;


        //-----------------------------------
        // 2) Clear Found Pairs
        //-----------------------------------
        TObjArray* CleanPairs = CheckCleanPairs( InBT, Pairs);
        for (int j=0; j<CleanPairs->GetEntries(); ++j) {
            cout << "j= " << j << endl;
            EdbTrackP* pair=(EdbTrackP*)CleanPairs->At(j);
            eRecoShowerArray->Add(pair);
            ++eRecoShowerArrayN;
        }



    } // Loop over InBT
    cout << " Loop over InBT finished."<< endl;


    cout << " eRecoShowerArray=  " <<  eRecoShowerArray << endl;
    cout << " eRecoShowerArrayN=  " <<  eRecoShowerArrayN << endl;

    for (Int_t i=0; i <eRecoShowerArray->GetEntries();  i++) {
//   EdbShowerP* sh = (EdbShowerP* )eRecoShowerArray->At(i);
//   sh->PrintSegments();
        EdbTrackP* sh = (EdbTrackP* )eRecoShowerArray->At(i);
        sh->PrintNice();
    }

    Log(2,"EdbShowerAlg_GS::Execute","Execute()...done.");
    return;
}

//______________________________________________________________________________



TObjArray* EdbShowerAlg_GS::FindPairs(EdbSegP* InBT, EdbPVRec* eAli_Sub)
{
    if (gEDBDEBUGLEVEL>2) cout << "Starting FindPairs(InBT,eAli_Sub) now" << endl;

    TObjArray* RecoShowerArray= new TObjArray(99);
    RecoShowerArray->Clear();
    Int_t RecoShowerArrayN=0;
    TArrayI* SegmentPIDArray = new TArrayI(9999);
    TArrayI* SegmentIDArray = new TArrayI(9999);
    TArrayI* Segment2PIDArray = new TArrayI(9999);
    TArrayI* Segment2IDArray = new TArrayI(9999);
    EdbSegP* Segment=NULL;
    EdbSegP* Segment2=NULL;
    Float_t x_av,y_av,z_av,tx_av,ty_av,distZ;
    EdbSegP* Segment_Sum=new EdbSegP(0,0,0,0,0,0);
    Float_t	 IP_Pair_To_InBT=0;

    if (NULL==InBT) {
        EdbSegP* InBT= new EdbSegP();
        InBT->SetX(0);
        InBT->SetY(0);
        InBT->SetZ(0);
        InBT->SetTX(0);
        InBT->SetTY(0);
        InBT->SetMC(-999,-999);
    }

    //-----------------------------------
    // 2) Loop over (whole) eAli, check BT for Cuts
    // eAli_Sub
    // Loop structure:
    // Loop over plates [0..NPatterns-1] for pattern one
    // Loop over plates [0..NPatterns-1] for pattern two
    // Take only plate pairings for |PID diff|<=3
    // Loop over all BT of pattern one
    // Loop over all BT of pattern two

    // This doesnt yet distinguish the FIRSTPLATE, MIDDLEPLATE, LATPLATE,NUMBERPLATES
    // labelling, this will be built in later....
    //-----------------------------------

    Int_t npat=eAli_Sub->Npatterns()-1;
    Int_t pat_one_bt_cnt_max,pat_two_bt_cnt_max=0;
    EdbPattern* pat_one=0;
    EdbPattern* pat_two=0;

    for (Int_t pat_one_cnt=0; pat_one_cnt<npat; ++pat_one_cnt) {
        pat_one=(EdbPattern*)eAli_Sub->GetPattern(pat_one_cnt);
        pat_one_bt_cnt_max=eAli_Sub->GetPattern(pat_one_cnt)->GetN();

        // Check if dist Z to vtx (BT) is ok:
        distZ=pat_one->Z()-InBT->Z();
        if (distZ>eParaValue[3]) continue;
        if (distZ<1000) continue;

        if (gEDBDEBUGLEVEL>2) cout << "pat_one_cnt=" << pat_one_cnt << "  pat_one->Z() = " << pat_one->Z() << " pat_one_bt_cnt_max= "<< pat_one_bt_cnt_max <<endl;


        for (Int_t pat_two_cnt=0; pat_two_cnt<npat; ++pat_two_cnt) {

            // Now apply cut conditions: GS  GAMMA SEARCH Alg:
            if (TMath::Abs(pat_one_cnt-pat_two_cnt)>eParaValue[5]) continue;

            pat_two=(EdbPattern*)eAli_Sub->GetPattern(pat_two_cnt);
            pat_two_bt_cnt_max=eAli_Sub->GetPattern(pat_two_cnt)->GetN();



            if (gEDBDEBUGLEVEL>2) cout << "	pat_two_cnt=" << pat_two_cnt << "  pat_two->Z() = " << pat_two->Z() << " pat_two_bt_cnt_max= "<< pat_two_bt_cnt_max <<endl;

            for (Int_t pat_one_bt_cnt=0; pat_one_bt_cnt<pat_one_bt_cnt_max; ++pat_one_bt_cnt) {
                Segment =  (EdbSegP*)pat_one->GetSegment(pat_one_bt_cnt);

                for (Int_t pat_two_bt_cnt=0; pat_two_bt_cnt<pat_two_bt_cnt_max; ++pat_two_bt_cnt) {

                    Segment2 = (EdbSegP*)pat_two->GetSegment(pat_two_bt_cnt);
                    if (Segment2==Segment) continue;
                    if (Segment2->ID()==Segment->ID()&&Segment2->PID()==Segment->PID()) continue;

                    // At first:  Check for already duplicated pairings:
                    if (CheckPairDuplications(Segment->PID(),Segment->ID(),Segment2->PID(),Segment2->ID(), SegmentPIDArray,SegmentIDArray,Segment2PIDArray,Segment2IDArray)) continue;

                    // Now apply cut conditions: GS  GAMMA SEARCH Alg  --------------------
                    // if InBT is flagged as MC InBT, take care that only BG or same MC basetracks are taken:
                    if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment->MCEvt()!=Segment2->MCEvt()) continue;
                    if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment->MCEvt()!=InBT->MCEvt()) continue;
                    if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment2->MCEvt()!=InBT->MCEvt()) continue;

                    // In case of two MC events, check for e+ e- pairs
                    // Only if Parameter is set to choose different Flag() pairs:
                    if (InBT->MCEvt()>0 && eParaValue[5]==1)
                        if (Segment->MCEvt()>0&&Segment2->MCEvt()>0)
                            if ((Segment2->Flag()+Segment->Flag())!=0) continue;

                    // a) Check dR between tracks:
                    if (DeltaR_WithPropagation(Segment,Segment2)>eParaValue[2]) continue;
                    // b) Check dT between tracks:
                    if (DeltaThetaSingleAngles(Segment,Segment2)>eParaValue[4]) continue;
                    // c) Check dMinDist between tracks:
                    if (GetMinimumDist(Segment,Segment2)>eParaValue[1]) continue;

                    // d) Check if dist Z to vtx (BT) is ok:
                    distZ=Segment->Z()-InBT->Z();
                    if (distZ>eParaValue[3]) continue;

                    x_av=Segment2->X()+(Segment->X()-Segment2->X())/2.0;
                    y_av=Segment2->Y()+(Segment->Y()-Segment2->Y())/2.0;
                    z_av=Segment2->Z()+(Segment->Z()-Segment2->Z())/2.0;
                    tx_av=Segment2->TX()+(Segment->TX()-Segment2->TX())/2.0;
                    ty_av=Segment2->TY()+(Segment->TY()-Segment2->TY())/2.0;
                    Segment_Sum->SetX(x_av);
                    Segment_Sum->SetY(y_av);
                    Segment_Sum->SetTX(tx_av);
                    Segment_Sum->SetTY(ty_av);
                    Segment_Sum->SetZ(z_av);

                    // d) Check if IP to vtx (BT) is ok:
// 	  IP_Pair_To_InBT=CalcIP(Segment_Sum, InBT->X(),InBT->Y(),InBT->Z());
// 	  if (IP_Pair_To_InBT>eParaValue[0]) continue;

                    Float_t IP_Pair_To_InBT_Seg	=CalcIP(Segment, InBT->X(),InBT->Y(),InBT->Z());
                    Float_t IP_Pair_To_InBT_Seg2	=CalcIP(Segment2, InBT->X(),InBT->Y(),InBT->Z());

                    // d) Check if IP to vtx (BT) is ok:
                    if (TMath::Min(IP_Pair_To_InBT_Seg,IP_Pair_To_InBT_Seg2)>eParaValue[0]) continue;

                    cout << "IP_Pair_To_InBT_Seg= " << IP_Pair_To_InBT_Seg << endl;
                    cout << "IP_Pair_To_InBT_Seg2= " << IP_Pair_To_InBT_Seg2 << endl;

                    // e) Check if this is not a possible fake doublet which is
                    //	sometimes caused by view overlap in the scanning:
                    //cout << " Check for IsPossibleFakeDoublet by view overlap" << endl;
                    if (IsPossibleFakeDoublet(Segment,Segment2) ) continue;
                    // end of    cut conditions: GS  GAMMA SEARCH Alg  --------------------
                    //


                    // Chi2 Variable:
                    Float_t dminDist=GetMinimumDist(Segment,Segment2);
                    Float_t dtheta=DeltaThetaSingleAngles(Segment,Segment2);
                    // preliminary meand and sigma values (taken from 1Gev, with highpur cutset), to be checked if they are the same for 0.5,1,2,4 GeV Photons
                    Float_t GammaChi2 =  ((IP_Pair_To_InBT-80)*(IP_Pair_To_InBT-80)/60/60)+((dminDist-3.5)*(dminDist-3.5)/4.7/4.7)+((dtheta-0.021)*(dtheta-0.021)/0.012/0.012);



                    if (gEDBDEBUGLEVEL>2) {
                        cout << "Pair has passed all cuts w.r.t to InBT:" << endl;
                        cout << "IP_Pair_To_InBT  = " << IP_Pair_To_InBT << endl;
                        cout << "GetMinimumDist(Segment,Segment2)  = " << GetMinimumDist(Segment,Segment2) << endl;
                        cout << "CalcIP(Segment_Sum,InBT)  = " << IP_Pair_To_InBT << endl;
                        cout << "GetSpatialDist(Segment_Sum,InBT)  = " << GetSpatialDist(Segment_Sum,InBT) << endl;
                        cout << "GammaChi2 = " << GammaChi2 << endl;
                    }

                    SegmentPIDArray->AddAt(Segment->PID(),RecoShowerArrayN);
                    SegmentIDArray->AddAt(Segment->ID(),RecoShowerArrayN);
                    Segment2PIDArray->AddAt(Segment2->PID(),RecoShowerArrayN);
                    Segment2IDArray->AddAt(Segment2->ID(),RecoShowerArrayN);


                    // Create new EdbTrackP Object for storage;
                    EdbTrackP* RecoShower = new EdbTrackP();
                    RecoShower -> AddSegment(Segment);
                    RecoShower -> AddSegment(Segment2);
                    // Set X and Y and Z values: /Take lower Z of both BTs)
                    RecoShower->SetZ(TMath::Min(Segment->Z(),Segment2->Z()));
                    RecoShower->SetX(Segment_Sum->X());
                    RecoShower->SetY(Segment_Sum->Y());
                    RecoShower->SetTX(Segment_Sum->TX());
                    RecoShower->SetTY(Segment_Sum->TY());
                    RecoShower->SetMC(InBT->MCEvt(),InBT->MCEvt());
                    RecoShower->SetID(RecoShowerArrayN);
                    RecoShower->SetPID(Segment->PID());
                    RecoShower ->PrintNice();

                    cout <<"------------"<< endl;


                    // Add Shower to interim  Array:
                    RecoShowerArray->Add(RecoShower);
                    ++RecoShowerArrayN;

                }
            }
        }
    }  //for (Int_t pat_one_cnt=0; ...

    cout << "EdbShowerAlg_GS::FindPairs For the InBT/Vtx at __" << InBT << "__, we have found "  <<  RecoShowerArray->GetEntries()  << " compatible pairs in the PVRec volume." << endl;

    // Delete unnecessary objects:
    // important, else memory overflow!!
    delete SegmentPIDArray;
    delete Segment2PIDArray;
    delete SegmentIDArray;
    delete Segment2IDArray;

    return RecoShowerArray;
}

//______________________________________________________________________________


TObjArray* EdbShowerAlg_GS::CheckCleanPairs(EdbSegP* InBT, TObjArray* RecoShowerArray)
{

    cout << "CheckCleanPairs"  <<endl;
    if (NULL==RecoShowerArray) return NULL;

    TObjArray* NewRecoShowerArray= new TObjArray(999999);
    NewRecoShowerArray->Clear();
    int NewRecoShowerArrayN=0;

    EdbTrackP* TrackPair1=NULL;
    EdbTrackP* TrackPair2=NULL;

    int ntrack=RecoShowerArray->GetEntriesFast();
    cout << ntrack << endl;

    for (Int_t pat_one_cnt=0; pat_one_cnt<ntrack; ++pat_one_cnt) {
        cout << "Doing pat_one_cnt = " << pat_one_cnt << endl;
        // (if -1) then the last one is not checked
        TrackPair1=(EdbTrackP*)RecoShowerArray->At(pat_one_cnt);
        bool taketrack1=true;
        for (Int_t pat_two_cnt=pat_one_cnt; pat_two_cnt<ntrack; ++pat_two_cnt) {
            cout << "Doing pat_two_cnt = " << pat_two_cnt << endl;
            //if only one track at all take it anyway:
            if (ntrack==1) continue;
            TrackPair2=(EdbTrackP*)RecoShowerArray->At(pat_two_cnt);
            //      cout << pat_one_cnt << "  " << pat_two_cnt << endl;
            //      cout << pat_one_cnt << "  " << pat_two_cnt << endl;

            // Check  if track1 has a track before (smaller Z value)
            // and if so, if dtheta is smaller than 0.1:
            // If both is so, then track 1 is NOT taken for final array:
            //       cout << TrackPair1->Z() << "  " <<  TrackPair2->Z()  << endl;
            EdbSegP* s1=(EdbSegP* )TrackPair1->GetSegment(0);
//        s1->PrintNice();
            EdbSegP* s2=(EdbSegP* )TrackPair2->GetSegment(0);
//        s2->PrintNice();
            if (TrackPair1->Z()>TrackPair2->Z() && DeltaThetaSingleAngles((EdbSegP*)s1,(EdbSegP*)s2)<0.05) taketrack1=false;
            if (!taketrack1) break;
        }

        cout << "taketrack1 =  " << taketrack1 << endl;
        if (!taketrack1) continue;

        // Add TrackPair1
        NewRecoShowerArray->Add(TrackPair1);
        ++NewRecoShowerArrayN;


        // Print
        if (TrackPair1->N()<2) cout << "This Track has only ONE entry" << endl;

        EdbSegP* s1=(EdbSegP* )TrackPair1->GetSegment(0);
        s1->PrintNice();
        EdbSegP* s2=(EdbSegP* )TrackPair1->GetSegment(1);
        s2->PrintNice();


        cout <<  s1->MCEvt() << "  " <<  s2->MCEvt() << "  " << s1->Flag() << "  " << s2->Flag() << "  " << s1->P() << "  " << s2->P() << "  " <<  "  " << s1->Z() << "  " << s2->Z() << "  " <<endl;
        cout<< "--------"<<endl;
    }
    cout << " From " <<ntrack << "  originally, there are now after Zposition and overlap cuts: " << NewRecoShowerArrayN << " left." << endl;


    return NewRecoShowerArray;
}





//______________________________________________________________________________

void EdbShowerAlg_GS::Finalize()
{
    // do nothing yet.
}
//______________________________________________________________________________


double EdbShowerAlg_GS::CalcIP(EdbSegP *s, double x, double y, double z) {
    // Calculate IP between a given segment and a given x,y,z.
    // return the IP value.
    double ax = s->TX();
    double ay = s->TY();
    double bx = s->X()-ax*s->Z();
    double by = s->Y()-ay*s->Z();
    double a;
    double r;
    double xx,yy,zz;
    a = (ax*(x-bx)+ay*(y-by)+1.*(z-0.))/(ax*ax+ay*ay+1.);
    xx = bx +ax*a;
    yy = by +ay*a;
    zz = 0. +1.*a;
    r = sqrt((xx-x)*(xx-x)+(yy-y)*(yy-y)+(zz-z)*(zz-z));
    return r;
}
double EdbShowerAlg_GS::CalcIP(EdbSegP *s, EdbVertex *v) {
    // calculate IP between a given segment and a given vertex.
    // return the IP value.
    // this is used for IP cut.

    // if vertex is not given, use the selected vertex.
    // if(v==NULL) v=gEDA->GetSelectedVertex();

    if (v==NULL) return -1.;
    return CalcIP(s, v->X(), v->Y(), v->Z());
}

//______________________________________________________________________________

Bool_t EdbShowerAlg_GS::IsPossibleFakeDoublet(EdbSegP* s1,EdbSegP* s2) {
    if (TMath::Abs(s1->X()-s2->X())<1) return kTRUE;  // minimum distance of 1micron
    if (TMath::Abs(s1->Y()-s2->Y())<1) return kTRUE;// minimum distance of 1micron
    if (TMath::Abs(s1->TX()-s2->TX())<0.005) return kTRUE;// minimum angle of 5mrad
    if (TMath::Abs(s1->TY()-s2->TY())<0.005) return kTRUE;// minimum angle of 5mrad
    return kFALSE;
}

//______________________________________________________________________________

















//______________________________________________________________________________

EdbShowerAlgESimple::EdbShowerAlgESimple()
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple()   Default Constructor"<<endl;
    Set0();
    Init();
}


//______________________________________________________________________________

/// CONSTRUCTOR USED FOR libShowRec with EdbShowerP class available.
// EdbShowerAlgESimple::EdbShowerAlgESimple(EdbShowerP* shower)
// {
//     // Default Constructor...
//     if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(EdbShowerP* shower)   Default Constructor"<<endl;
//     Set0();
//     Init();
//     // ... with Shower:
//     eRecoShowerArray=new TObjArray();
//     eRecoShowerArray->Add(shower);
//     SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
//     if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(EdbShowerP* shower)   Default Constructor...done."<<endl;
// }

//______________________________________________________________________________

EdbShowerAlgESimple::EdbShowerAlgESimple(EdbTrackP* track)
{
    // Default Constructor...
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(EdbTrackP* track)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with Shower:
    eRecoShowerArray=new TObjArray();
    eRecoShowerArray->Add(track);
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(EdbTrackP* track)   Default Constructor...done."<<endl;
}


//______________________________________________________________________________

EdbShowerAlgESimple::EdbShowerAlgESimple(TObjArray* RecoShowerArray)
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(TObjArray* RecoShowerArray)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with ShowerArray:
    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(TObjArray* RecoShowerArray)   Default Constructor...done."<<endl;
}

//______________________________________________________________________________

EdbShowerAlgESimple::~EdbShowerAlgESimple()
{
    // Default Destructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::~EdbShowerAlgESimple()"<<endl;
    // Delete Histograms (on heap):
    delete eHisto_nbtk_av;
    delete eHisto_longprofile_av;
    delete eHisto_transprofile_av;
    delete eHisto_deltaR_mean;
    delete eHisto_deltaT_mean;
    delete eHisto_deltaR_rms;
    delete eHisto_deltaT_rms;
    delete eHisto_nbtk;
    delete eHisto_longprofile;
    delete eHisto_transprofile;
    delete eHisto_deltaR;
    delete eHisto_deltaT;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::~EdbShowerAlgESimple()...done."<<endl;
}

//______________________________________________________________________________

void EdbShowerAlgESimple::Set0()
{
    // Reset Values
    eCalibrationOffset=0;
    eCalibrationSlope=1;
    eRecoShowerArray=NULL;
    eRecoShowerArrayN=0;
    ePlateNumberType=0;
    ePlateNumber=20;
    eSpecificationIsChanged=kFALSE;
    eForceSpecificationReload=kFALSE;

    eWeightFileString="NULL";
    for (int i=0; i<7; i++) {
        eSpecificationType[i]=-1;
        eSpecificationTypeString[i]="";
        for (int j=0; j<7; j++) {
            eSpecificationTypeStringArray[i][j]="";
        }
    }
    for (int i=0; i<15; i++) {
        ANN_MLP_ARRAY[i]=NULL;
        eANN_MLP_CORR_0[i]=0.0;
        eANN_MLP_CORR_1[i]=1.0;
    }
    eEnergyArrayCount=0;

    // Reset "eEfficiencyParametrisation" to the "Neuch" eff.
    cout << "EdbShowerAlgESimple::Set0()   Reset to the Neuch eff." << endl;
    cout << "EdbShowerAlgESimple::Set0()   eEfficiencyParametrisation= " << eEfficiencyParametrisation << endl;
    //if (NULL!=eEfficiencyParametrisation) delete eEfficiencyParametrisation;
    Double_t xarr[6]= {0,0.1,0.2,0.3,0.4,0.5};
    Double_t yarr[6]= {0.95,0.9,0.80,0.75,0.6,0.5};
    eEfficiencyParametrisation = new TSpline3("",xarr,yarr,5,0,0,0.6);
    cout << "EdbShowerAlgESimple::Set0()   Reset to the Neuch eff. ...done." << endl;


    cout << "EdbShowerAlgESimple::Set0()...done."<< endl;
    return;
}


//______________________________________________________________________________

void EdbShowerAlgESimple::Init()
{
    // This Plate Binnning is the _final_binning and will not be refined furthermore!
    ePlateBinning[0]=10;
    ePlateBinning[1]=12;
    ePlateBinning[2]=14;
    ePlateBinning[3]=16;
    ePlateBinning[4]=18;
    ePlateBinning[5]=20;
    ePlateBinning[6]=23;
    ePlateBinning[7]=26;
    ePlateBinning[8]=29;
    ePlateBinning[9]=32;
    ePlateBinning[10]=35;
    ePlateBinning[11]=40;
    ePlateBinning[12]=45;
    ePlateBinning[13]=45;
    ePlateBinning[14]=45; // 12,13,14 are the same, cause I have seen that it makes no sense to produce these,
    // since E,and sigmaE for 45 plates doesnt change anymore at all...
    // and how often !can! we scan more than 45 plates for a shower?

    ePlateNumber=20; // 20 plates by default...
    eSpecificationType[0] = 1;  // cp files by default...
    eSpecificationType[1] = 1;  // ele by default...
    eSpecificationType[2] = 0;  // neuch eff by default...
    eSpecificationType[3] = 0;  // 0..20 GeV by default...
    eSpecificationType[4] = 0;  // next before weight by default...
    eSpecificationType[5] = 5;  // 20 plates by default...

    eSpecificationTypeString[0]="CP";
    eSpecificationTypeString[1]="GAMMA";
    eSpecificationTypeString[2]="Neuch";
    eSpecificationTypeString[3]="TypeA";
    eSpecificationTypeString[4]="Next";
    eSpecificationTypeString[5]="20";

    eForceSpecificationReload=kFALSE;

    for (int i=0; i<15; i++) {
        ANN_nPlates_ARRAY[i]=ePlateBinning[i];
    }

    EffFunc_all = new TF1("all","1.0-0.00000001*x",0,0.95);
    EffFunc_edefault = new TF1("default","0.94-0.216*x-0.767*x*x+1.865*x*x*x",0,0.95);
    EffFunc_elletroni = new TF1("elletroni","0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x",0,0.95);
    EffFunc_neuchmicro = new TF1("neuchmicro","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95);
    EffFunc_MiddleFix = new TF1("MiddleFix","0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    EffFunc_LowEff = new TF1("LowEff","0.85*0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    EffFunc_UserEfficiency = new TF1("EffFunc_UserEfficiency","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95); //new TSpline3();


    // Standard supposed efficiency of the showers/tracks that are to be evaluated:
    // The correct EffFunc_* is then choosen by taking the closes eff func to that one.
    // Initial parameters are the efficiency parameters for the "Neuch" parametrisation
    Double_t xarr[7]= {0,0.1,0.2,0.3,0.4,0.5,0.6};
    Double_t yarr[7]= {0.95,0.9,0.80,0.75,0.6,0.5,0.4};
    //if (NULL!=eEfficiencyParametrisation) delete eEfficiencyParametrisation;
    eEfficiencyParametrisation = new TSpline3("",xarr,yarr,5,0,0,0.65);

    eHisto_nbtk_av          = new TH1D("eHisto_nbtk_av","Average basetrack numbers",21,0.0,10.0);
    eHisto_longprofile_av   = new TH1D("eHisto_longprofile_av","Basetracks per emulsion number",57,0.0,57.0);
    eHisto_transprofile_av  = new TH1D("eHisto_transprofile_av","Basetracks in trans distance",8,0.0,800.0);
    eHisto_deltaR_mean      = new TH1D("eHisto_deltaR_mean","Mean #deltar of all BTs in one shower",100,0.0,100.0);
    eHisto_deltaT_mean      = new TH1D("eHisto_deltaT_mean","Mean #delta#theta of all BTs in one shower",100,0.0,0.1);
    eHisto_deltaR_rms       = new TH1D("eHisto_deltaR_rms","RMS #deltar of all BTs in one shower",100,0.0,100.0);
    eHisto_deltaT_rms       = new TH1D("eHisto_deltaT_rms","RMS #delta#theta of all BTs in one shower",100,0.0,0.1);
    eHisto_nbtk             = new TH1D("eHisto_nbtk","Basetracks in the shower",50,0.0,100.0);
    eHisto_longprofile      = new TH1D("eHisto_longprofile","Basetracks per emulsion number",57,0.0,57.0);
    eHisto_deltaR           = new TH1D("eHisto_deltaR","Single #deltar of all BTs in Shower",100,0.0,150.0);
    eHisto_deltaT           = new TH1D("eHisto_deltaT","Single #delta#theta of all BTs in Shower",150,0.0,0.15);
    eHisto_transprofile     = new TH1D("eHisto_transprofile","Basetracks in trans distance",8,0.0,800.0);

    // CreateANN
    CreateANN();

    //First Time, we also Update()
    Update();

    // Set Strings:
    InitStrings();

    // Create EnergyArray
    eEnergyArray=new TArrayF(99999); // no way to adapt tarrayF on the fly...
    eEnergyArrayUnCorrected=new TArrayF(99999);
    eEnergyArraySigmaCorrected=new TArrayF(99999);


    // Read Energy Resolution Lookup tables:
    ReadTables();

    cout << "EdbShowerAlgESimple::Init()...done."<< endl;
    return;
}

//______________________________________________________________________________


void EdbShowerAlgESimple::InitStrings()
{
    eSpecificationTypeStringArray[0][0]="LT";
    eSpecificationTypeStringArray[0][1]="CP";
    eSpecificationTypeStringArray[1][0]="GAMMA";
    eSpecificationTypeStringArray[1][1]="ELECTRON";
    eSpecificationTypeStringArray[2][0]="Neuch";
    eSpecificationTypeStringArray[2][1]="All";
    eSpecificationTypeStringArray[2][2]="MiddleFix";
    eSpecificationTypeStringArray[2][3]="Low";
    eSpecificationTypeStringArray[3][0]="TypeA";
    eSpecificationTypeStringArray[3][1]="TypeABCD";
    eSpecificationTypeStringArray[4][0]="Next";
    eSpecificationTypeStringArray[4][1]="Before";
    eSpecificationTypeStringArray[5][0]="10";
    cout << "EdbShowerAlgESimple::InitStrings()...done."<< endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlgESimple::CreateANN()
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::CreateANN()"<<endl;

    // Create ANN Tree and MLP:
    eParaName=2; // Standard Labeling taken over from EdbShowerP
    ANNTree = new TTree("ANNTree", "ANNTree");
    ANNTree->SetDirectory(0);
    ANNTree->Branch("inANN", inANN, "inANN[70]/D");

    // 	Plate Binning: 10,12,14,16,18,20,23,26,29,32,35,40,45,45
    // see ePlateBinning[]

    for (int k=0; k<15; k++) {
        //cout << "creatin ANN " << k << endl;
        ANN_Layout="";
        ANN_n_InputNeurons_ARRAY[k]=6+ePlateBinning[k];
        ANN_n_InputNeurons=ANN_n_InputNeurons_ARRAY[k];
        for (Int_t i=1; i<ANN_n_InputNeurons; ++i) ANN_Layout += "@inANN["+TString(Form("%d",i))+"],";
        ANN_Layout += "@inANN["+TString(Form("%d",ANN_n_InputNeurons))+"]:"+TString(Form("%d",ANN_n_InputNeurons+1))+":"+TString(Form("%d",ANN_n_InputNeurons));
        ANN_Layout+=":inANN[0]/1000";
        //---------------------------
        ANN_MLP_ARRAY[k]	= 	new TMultiLayerPerceptron(ANN_Layout,ANNTree,"","");
        ANN_MLP=ANN_MLP_ARRAY[k];
        ANN_Layout=ANN_MLP_ARRAY[k]->GetStructure();
        //---------------------------
        if (gEDBDEBUGLEVEL>2) {
            ANN_MLP->Print();
            cout << ANN_Layout << endl;
        }
    }
    //---------------------------

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::CreateANN()...done."<<endl;
    return;
}

//____________________________________________________________________________________________________

void EdbShowerAlgESimple::DoRun()
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::DoRun()" << endl;

    for (int i=0; i<eRecoShowerArrayN; i++) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Doing i= " << i << endl;
        continue;
        eEnergyArrayCount=i;

        //EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        EdbTrackP* shower=(EdbTrackP*) eRecoShowerArray->At(i);

        DoRun(shower);

    } // (int i=0; i<eRecoShowerArrayN; i++)

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::DoRun()...done." << endl;
    return;
}


//____________________________________________________________________________________________________

void EdbShowerAlgESimple::DoRun(TObjArray* trackarray)
{
    // Runs over the object array of showers.
    
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::DoRun(TObjArray* trackarray)" << endl;

    eRecoShowerArrayN=trackarray->GetEntries();
    eRecoShowerArray=trackarray;

    for (int i=0; i<eRecoShowerArrayN; i++) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Doing i= " << i << endl;

        eEnergyArrayCount=i;

        //EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        EdbTrackP* shower=(EdbTrackP*) eRecoShowerArray->At(i);

        DoRun(shower);

    } // (int i=0; i<eRecoShowerArrayN; i++)

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::DoRun(TObjArray* trackarray)...done." << endl;
    return;
}

//____________________________________________________________________________________________________

void EdbShowerAlgESimple::DoRun(EdbTrackP* shower)
{
    // What i write here? It will appear in html root documentation.
    

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlgESimple::DoRun(EdbTrackP* shower)" << endl;

    // Get internal variables from the parametrisation filled:
    GetPara(shower);


    int EffNr=FindClosestEfficiencyParametrization(eParaShowerAxisAngle,eEfficiencyParametrisation->Eval(eParaShowerAxisAngle));

    // Check If Efficiency is the one we have or if we have to change/reload
    // the ANN weightfiles:
    if (GetSpecType(2)!=EffNr&&eForceSpecificationReload==kFALSE) {
        cout << "EdbShowerAlgESimple::DoRun()   INFO! Calulated Efficiency is more compatible with another one: Change Specifiaction! Call SetSpecificationType(2,"<< EffNr <<")." << endl;
        SetSpecificationType(2,EffNr);
    }
    if (GetSpecType(2)!=EffNr&&eForceSpecificationReload==kTRUE) {
        cout << "EdbShowerAlgESimple::DoRun()   INFO! Calulated Efficiency would be more compatible with another one: But eForceSpecificationReload is set to kFALSE. So we keep current Efficiency used." << endl;

    }




    // This is to select the suited ANN to the shower, i.e. the one that matches closest
    // the number of plates:
    int check_Npl_index	=0;
    GetNplIndexNr(shower->Npl(),check_Npl_index,ePlateNumberType);


    ANN_n_InputNeurons=ANN_n_InputNeurons_ARRAY[check_Npl_index];
    ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];
    eWeightFileString=ANN_WeightFile_ARRAY[check_Npl_index];
    eCalibrationOffset = eANN_MLP_CORR_0[check_Npl_index];
    eCalibrationSlope = eANN_MLP_CORR_1[check_Npl_index];

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ANN_n_InputNeurons_ARRAY[check_Npl_index]="<< ANN_n_InputNeurons_ARRAY[check_Npl_index] << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ANN_MLP_ARRAY[check_Npl_index]="<< ANN_MLP_ARRAY[check_Npl_index] << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Using the following layout: " << endl;
    if (gEDBDEBUGLEVEL >2) cout << ANN_MLP->GetStructure() << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   And the following weightfile: " << endl;
    if (gEDBDEBUGLEVEL >2) cout << eWeightFileString << endl;

    //  Reset InputVariables:
    for (int k=0; k<70; k++) {
        inANN[k]=0;
    }

    // inANN[0] is ALWAYS Reseverd for the quantity value to be estimated
    // (E,Id,...)
    // Test with private variables:
    inANN[1]=eParaShowerAxisAngle;
    inANN[2]=eParanseg;
    inANN[3]=eParaBT_deltaR_mean;
    inANN[4]=eParaBT_deltaR_rms;
    inANN[5]=eParaBT_deltaT_mean;
    inANN[6]=eParaBT_deltaT_rms;
    for (int ii=0; ii<57; ii++) {
        inANN[7+ii]= eParalongprofile[ii];
    }

    // Fill Tree:
    ANNTree->Fill();

    Double_t params[70];
    Double_t val;

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Print Inputvariables: " << endl;
    for (Int_t j=1; j<=ANN_n_InputNeurons; ++j) {
        params[j-1]=inANN[j];
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   :  j  params[j-1]=inANN[j]   " << j<< "  " << params[j-1] << endl;
    }
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Print Inputvariables...done." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Evaluate Neural Network Output:" << endl;

    // ---------------------------------
    // Evaluation of the ANN:
    val=(ANN_MLP->Evaluate(0,params));
    // ---------------------------------

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   Before Correction: ANN_MLP->Evaluate(0,params)= " <<  ANN_MLP->Evaluate(0,params) << " (Inputvar=" << inANN[0]  << ")." << endl;

    if ( isnan(val) ) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ERROR! ANN_MLP->Evaluate(0,params) is NAN! Setting value to -1. " << endl;
        val=-1;
    }
    eEnergyUnCorr=val;

    // Linear Correction According to the Parameters matching the right weightfile.
    val=(val-eCalibrationOffset)/eCalibrationSlope;

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   After  Correction: ANN_MLP->Evaluate(0,params)= " <<  val << " (Inputvar=" << inANN[0]  << ")." << endl;

    // ---------------------------------
    // Evaluation of the statistical andsystematical errors by lookup tables and splines:
    // For further info and more details, I will report the methology in the thesis, and at
    // some next collaboration meetings.. (As of october 2010,fwm.)
    TSpline3* spline;
    Float_t val_sigma_stat, val_sigma_sys, val_sigma_tot;
    cout << "TSpline3* spline=eSplineArray_Energy_Stat_Gamma[check_Npl_index];" << endl;
    spline=(TSpline3*)eSplineArray_Energy_Stat_Gamma->At(check_Npl_index);
    cout << "Float_t val_sigma_stat=spline->Eval(val);  "  << spline->Eval(val) << endl;
    val_sigma_stat=spline->Eval(val);

    cout << "TSpline3* spline=eSplineArray_Energy_Sys_Gamma[check_Npl_index];" << endl;
    spline=(TSpline3*)eSplineArray_Energy_Sys_Gamma->At(check_Npl_index);
    cout << "Float_t val_sigma_sys=spline->Eval(val);  "  << spline->Eval(val) << endl;
    val_sigma_sys=spline->Eval(val);

    // Addition of errors: stat(+)sys.  (+)=quadratic addition.
    val_sigma_tot=TMath::Sqrt(val_sigma_stat*val_sigma_stat+val_sigma_sys*val_sigma_sys);
    cout << "Float_t val_sigma_tot=...  "  << val_sigma_tot << endl;


    cout << "Doing only statistics and one source of systematicsat the moment! " << endl;
    cout << "Notice also that we dont have storage variable in EdbTrackP for the error of P() ... " << endl;
    // ---------------------------------

    // Quick Estimation of the sigma (from ANN_MEGA_ENERGY)
    // This is later to be read from a lookup table....
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ERROR! Sigma ONLY FROM STATISTICAL UNCERTATINTY NOWNOT correctly set up to now."<<endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ERROR! Sigma will be fully implemented when lookup tables for all"<<endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ERROR! systematic uncertainties are availible!"<<endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::DoRun()   Estimated Energy = " << val << " +- " << val_sigma_tot  << "..." << endl;



    // Finally, set values...
    shower->SetP(val);
    eEnergy=val;
    eEnergyCorr=val;
    eEnergySigmaCorr=val_sigma_stat;

    eEnergyArray->AddAt(eEnergy,eEnergyArrayCount);
    eEnergyArrayUnCorrected->AddAt(eEnergyUnCorr,eEnergyArrayCount);
    eEnergyArraySigmaCorrected->AddAt(eEnergySigmaCorr,eEnergyArrayCount);


    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::DoRun()   ...Done." << endl;
    return;
}

//______________________________________________________________________________________________________________


Int_t EdbShowerAlgESimple::FindClosestEfficiencyParametrization(Double_t TestAngle=0.0,Double_t ReferenceEff=1.0)
{
    // Returns the number of the closest EfficiencyParametrization to use. Supported are:
    // ((in this order numbering, different from the ordering, fwm used for his calculations!:))
    // 0:Neuch
    // 1:All
    // 2:MiddleFix
    // 3:Low

    // 4: UserEfficiency (can take input from eda, for example; or user measured input) // this cannot be used here,
    // because the weightfiles exist only for Neuch,All,MiddleFix,Low.   UserEfficiency is just for different
    // angle<->efficiency assignment!

    if (gEDBDEBUGLEVEL >2) cout << "TestAngle " << TestAngle << endl;
    if (gEDBDEBUGLEVEL >2) cout << "ReferenceEff " << ReferenceEff << endl;
    if (gEDBDEBUGLEVEL >2) cout << "neuchmicro->Eval(TestAngle) "<< EffFunc_neuchmicro->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "all->Eval(TestAngle) "<< EffFunc_all->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "MiddleFix->Eval(TestAngle) "<< EffFunc_MiddleFix->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "LowEff->Eval(TestAngle) "<< EffFunc_LowEff->Eval(TestAngle)<< endl;
    if (gEDBDEBUGLEVEL >2) cout << "eEfficiencyParametrisation->Eval(TestAngle) "<< eEfficiencyParametrisation->Eval(TestAngle)<< endl;

//Measure distance of angle to estimated angle:
    Double_t dist[9];
    dist[0]=TMath::Abs(ReferenceEff-EffFunc_neuchmicro->Eval(TestAngle));
    dist[1]=TMath::Abs(ReferenceEff-EffFunc_all->Eval(TestAngle));
    dist[2]=TMath::Abs(ReferenceEff-EffFunc_MiddleFix->Eval(TestAngle));
    dist[3]=TMath::Abs(ReferenceEff-EffFunc_LowEff->Eval(TestAngle));
    dist[4]=TMath::Abs(ReferenceEff-eEfficiencyParametrisation->Eval(TestAngle));

    Double_t dist_min=1;
    Int_t best_i=-1;
    for (int i=0; i<4; i++) if (abs(dist[i])<dist_min) {
            dist_min=dist[i];
            best_i=i;
        }

    if (gEDBDEBUGLEVEL >2) {
        cout << "Miminum distance = " << dist_min << ", best matching efficiency is =  ";
        if (best_i==0) cout << " neuchmicro " <<endl;
        if (best_i==2) cout << " MiddleFix " <<endl;
        if (best_i==3) cout << " LowEff " <<endl;
        if (best_i==1) cout << " All " <<endl;
        //if (best_i==4) cout << " User Set eEfficiencyParametrisation " <<endl;
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::FindClosestEfficiencyParametrization()  ...Done." <<  endl;
    return best_i;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::SetWeightFileString(TString weightstring)
{
    eWeightFileString=weightstring;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::SetWeightFileString()     eWeightFileString = " << eWeightFileString << endl;
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::GetNplIndexNr(Int_t check_Npl,Int_t &check_Npl_index, Int_t ePlateNumberType)
{
    // ---	What is the sense of this?
    // ---  According to the shower it can have any different length between [1..57] plates.
    // ---  We cannot produce weights for each plate....
    // ---  So we have only some weights for different plates: acoording to the array set in plateBinning:
    // ---  10,12,14,16,18,20,23,26,29,32,35,40,45,50
    // ---  So what if a shower has npl() of 19? take the weight for 18 or for 20 plates?
    // ---  therefore, we introduced ePlateNumberType. This will take the weight as like:
    // ---  In case ePlateNumberType > 0 the it will take he trained ANN file with Npl >= shower->Npl();
    // ---  in such a way that |ePlateNumberType| is the difference to the ePlateBinning[].
    // ---  Example: ePlateNumberType=1 -> 1st next availible weight will be taken: (npl==19->nplANN==20);
    // ---  Example: ePlateNumberType=2 -> 2nd next availible weight will be taken: (npl==19->nplANN==23);
    // ---  Example: ePlateNumberType=3 -> 3rd next availible weight will be taken: (npl==19->nplANN==26);
    // ---  Example: ePlateNumberType=-1-> 1st befo availible weight will be taken: (npl==19->nplANN==18);
    // ---  Example: ePlateNumberType=-2-> 2nd befo availible weight will be taken: (npl==19->nplANN==16);

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::GetNplIndexNr("<<check_Npl<<","<<check_Npl_index<<","<<ePlateNumberType<<")"<<endl;

    // The Neural Network Training was done in a way that we had always Npl(Train)>=shower->Npl()

    // First we find closest (bigger) ePlateBinning index:
    int diff=9999;
    int closestIndex=-1;
    for (int i=0; i<15; i++) {
        if (ePlateBinning[i]-check_Npl>0) {
            closestIndex=i;
            break;
        }
    }
    cout << "closestIndex= " << closestIndex << " ePlateBinning[closestIndex]= " << ePlateBinning[closestIndex] << endl;
    //-----------------------------------
    // for ePlateNumberType we have it already exact:
    ePlateNumber = closestIndex + ePlateNumberType;
    if (ePlateNumberType==1) ePlateNumber = closestIndex;
    cout << "ePlateNumber = closestIndex + ePlateNumberType = " <<  ePlateNumber << endl;

    //  check for over/underbound
    if (ePlateNumber<0) ePlateNumber=0;
    if (ePlateNumber>14) ePlateNumber=14;
    check_Npl_index=ePlateNumber;
    //-----------------------------------
//     // This Plate Binnning is the _final_binning and will not be refined furthermore!
//     ePlateBinning[0]=10;//     ePlateBinning[1]=12;
//     ePlateBinning[2]=14;//     ePlateBinning[3]=16;
//     ePlateBinning[4]=18;//     ePlateBinning[5]=20;
//     ePlateBinning[6]=23;//     ePlateBinning[7]=26;
//     ePlateBinning[8]=29;//     ePlateBinning[9]=32;
//     ePlateBinning[10]=35;//     ePlateBinning[11]=40;
//     ePlateBinning[12]=45;//     ePlateBinning[13]=45;
//     ePlateBinning[14]=45;
    //-----------------------------------
    cout << "All plates:"<<endl;
    cout << "Shower Npl:"<<endl;
    cout << "Available weights:"<<endl;
    cout << "Taken weight:"<<endl;
    cout << "[";
    for (int i=1; i<=57; i++) {
        cout << ".";
    }
    cout << "]" << endl;
    //-----------------------------------
    cout << "[";
    for (int i=1; i<=check_Npl; i++) {
        cout << ".";
    }
    cout << "]" << endl;
    //-----------------------------------
    cout << "[";
    for (int i=1; i<=57; i++) {
        Bool_t isExact=kFALSE;
        for (int j=0; j<15; j++) {
            if (i==ePlateBinning[j]) isExact=kTRUE;
        }
        if (isExact) {
            cout << "x";
        }
        else {
            cout << ".";
        }
    }
    cout << "]" << endl;
    //-----------------------------------
    cout << "[";
    for (int i=1; i<=57; i++) {
        if (i==ePlateBinning[ePlateNumber]) {
            cout << "X";
        }
        else {
            cout << ".";
        }
    }
    cout << "]" << endl;
    //-----------------------------------

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::check_Npl_index Shower:Npl()=" << check_Npl << "."<< endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::check_Npl_index ePlateNumber=" << ePlateNumber << "."<< endl;
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple::check_Npl_index ePlateBinning[=" << ePlateBinning[ePlateNumber] << "."<< endl;


    check_Npl=ePlateBinning[ePlateNumber];
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::GetSpecifications() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::GetSpecifications" << endl;
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::PrintSpecifications() {
    cout << "EdbShowerAlgESimple::PrintSpecifications" << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationType[0]  (Dataset:    (linked tracks/full cp) LT/CP)   = " << GetSpecType(0) << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationType[1]  (ShowerID:   gamma/electron/pion weight) = " << GetSpecType(1) << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationType[2]  (ScanEff:    Neuch/All/MiddleFix/LowEff) = " << GetSpecType(2) << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationType[3]  (E range:    0..20/0..40) = " << GetSpecType(3) << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationType[4]  (Npl weight: next before/after) = " <<GetSpecType(4) << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationType[5]  (Npl weight: 10,12,...,45) = " << GetSpecType(5) << endl;
    cout << "EdbShowerAlgESimple::   " << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationTypeString[0]  (take CP or linked_tracks)   = " << eSpecificationTypeString[0] << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationTypeString[1]  (gamma/electron/pion weight) = " << eSpecificationTypeString[1] << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationTypeString[2]  (ScanEff: Neuch/All/MiddleFix/LowEff) = " << eSpecificationTypeString[2] << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationTypeString[3]  (E range: 0..20/0..40) = " << eSpecificationTypeString[3] << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationTypeString[4]  (Npl weight: next before/after) = " << eSpecificationTypeString[4] << endl;
    cout << "EdbShowerAlgESimple::   eSpecificationTypeString[5]  (Npl weight: 10,12,...,45) = " << eSpecificationTypeString[5] << endl;
    cout << "EdbShowerAlgESimple::   " << endl;
    cout << "EdbShowerAlgESimple::   In case you want to change a specification then do for example:" << endl;
    cout << "EdbShowerAlgESimple::   EdbShowerAlgESimple->SetSpecification(2,3) " << endl;
    cout << "EdbShowerAlgESimple::   It will change the ScanEff from the actual value to _Low_." << endl;
    cout << "EdbShowerAlgESimple::" << endl;
    cout << "EdbShowerAlgESimple::PrintSpecifications...done." << endl;
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::LoadSpecificationWeightFile() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::LoadSpecificationWeightFile  from the following specifications:" << endl;
    PrintSpecifications();
    TString weigth="NULL";
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::SetSpecifications(Int_t sp0, Int_t sp1, Int_t sp2, Int_t sp3, Int_t sp4, Int_t sp5) {
    eSpecificationType[0]=sp0;
    eSpecificationType[1]=sp1;
    eSpecificationType[2]=sp2;
    eSpecificationType[3]=sp3;
    eSpecificationType[4]=sp4;
    eSpecificationType[5]=sp5;
    PrintSpecifications();
    eSpecificationIsChanged=kTRUE;
    eForceSpecificationReload=kTRUE;
    Update();
    return;
}


//__________________________________________________________________________________________________

void EdbShowerAlgESimple::SetSpecificationType(Int_t SpecificationType, Int_t SpecificationTypeVal)
{
//     if (GetSpecType(SpecificationType)==SpecificationTypeVal) {
// 	cout << "dbShowerAlgESimple::SetSpecificationType()   The given value is the same value as before. So just set the eForceSpecificationReload to False."<<endl;
// 	eForceSpecificationReload=kTRUE;
// 	return;
//     }
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgESimple:: Change Specification (" << SpecificationType << ") from " << GetSpecType(SpecificationType) << " -> " << SpecificationTypeVal << " . Reprint the changed Specification String: " << endl;

    eSpecificationType[SpecificationType]=SpecificationTypeVal;
    eSpecificationTypeString[SpecificationType] = eSpecificationTypeStringArray[SpecificationType][SpecificationTypeVal];

    if (gEDBDEBUGLEVEL >1) cout << eSpecificationTypeString[SpecificationType]  << endl;

    eSpecificationIsChanged=kTRUE;
    eForceSpecificationReload=kTRUE;

    cout << "EdbShowerAlgESimple::SetSpecificationType eSpecificationIsChanged: " << eSpecificationIsChanged << endl;
    cout << "EdbShowerAlgESimple::SetSpecificationType eForceSpecificationReload: " << eForceSpecificationReload << endl;

    cout << "Reprint Specifications: " << endl;
    if (gEDBDEBUGLEVEL >1) PrintSpecifications();

    Update();
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgESimple::Update() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::Update  Does the following things in the order:" << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::Update  * According to the switch: set the right ANN of the Array as generic one." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::Update  * According to the switch: load the right weightfile as generic one." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::Update  * According to the switch: set the right correction parameters...." << endl;

    TString basicstring = TString(gSystem->ExpandPathName("$FEDRA_ROOT"));
    TString addstring = ("/src/libShower/weights/Energy/");

    // (*) SpecType 0 :
    if (eSpecificationType[0]==1) {
        addstring+="volumeSpec_CP/";
        eSpecificationTypeString[0]="CP";
    }
    else {
        addstring+="volumeSpec_LT/";
        eSpecificationTypeString[0]="LT";
        cout << "EdbShowerAlgESimple::Update  WARNING::eSpecificationTypeString[0]=LT NOT YET SUPPORTED!" << endl;
    }

    // (*) SpecType 1 :
    if (eSpecificationType[1]==2) {
        addstring+="particleSpec_pion/";
        eSpecificationTypeString[1]="pion";
        cout << "EdbShowerAlgESimple::Update  WARNING::eSpecificationTypeString[1]=pion NOT YET SUPPORTED!" << endl;
    }
    else if (eSpecificationType[1]==1)  {
        addstring+="particleSpec_electron/";
        eSpecificationTypeString[1]="electron";
    }
    else {
        addstring+="particleSpec_gamma/";
        eSpecificationTypeString[1]="gamma";
    }

    // (*) SpecType 2 :
    if (eSpecificationType[2]==3) {
        addstring+="efficiencySpec_LowEff/";
        eSpecificationTypeString[2]="LowEff";
    }
    else if (eSpecificationType[2]==2)  {
        addstring+="efficiencySpec_MiddleFix/";
        eSpecificationTypeString[2]="MiddleFix";
    }
    else if (eSpecificationType[2]==1)  {
        addstring+="efficiencySpec_All/";
        eSpecificationTypeString[2]="All";
    }
    else {
        addstring+="efficiencySpec_Neuch/";
        eSpecificationTypeString[2]="Neuch";
    }

    // (*) SpecType 3 :
    if (eSpecificationType[3]==1) {
        addstring+="trainrangeSpec_extended/";
        eSpecificationTypeString[3]="extended";
        cout << "EdbShowerAlgESimple::Update  WARNING::eSpecificationTypeString[3]=extended NOT YET SUPPORTED!" << endl;
    }
    else {
        addstring+="trainrangeSpec_normal/";
        eSpecificationTypeString[3]="normal";
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  " << addstring << endl;

    if (NULL==ANN_MLP_ARRAY[0]) {
        CreateANN();
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  Now load the different ANN weightstrings:" << endl;
    // This was the loading part for the ANN and
    // this is the loading part for the Correction Factors, which we take from the
    // generic file EnergyCorrections_Npl_%d.txt
    for (int ll=0; ll<15; ll++) {
        TString weigthstring=basicstring+addstring+TString(Form("weights_Npl_%d.txt",ANN_nPlates_ARRAY[ll]));
        if (gEDBDEBUGLEVEL >2) cout << "weigthstring = " << weigthstring << endl;
        ANN_MLP_ARRAY[ll]->LoadWeights(weigthstring);
        ANN_WeightFile_ARRAY[ll]=weigthstring;

        TString correctionsfactorstring=basicstring+addstring+TString(Form("EnergyCorrections_Npl_%d.txt",ANN_nPlates_ARRAY[ll]));
        if (gEDBDEBUGLEVEL >2) cout << "correctionsfactorstring = " << correctionsfactorstring << endl;
        Float_t p0,p1;
        p0=0.0;
        p1=1.0;
        if (ll<10) ReadCorrectionFactors(correctionsfactorstring,p0,p1);
// Correction files for ll>11 dont exist yet..
        eANN_MLP_CORR_0[ll]=p0;
        eANN_MLP_CORR_1[ll]=p1;

    }
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::Update  * According to the switch: set the right ANN of the Array as generic one." << endl;

    // (*) SpecType 4:
    // Get NplIndexNr:
    int check_Npl_index=0;
    GetNplIndexNr(ePlateNumber,check_Npl_index, eSpecificationType[4]);

    if (check_Npl_index==0) eSpecificationTypeString[4]="Next";
    if (check_Npl_index==1) eSpecificationTypeString[4]="Before";
    eSpecificationTypeString[5]=TString(Form("%d",ePlateNumber));

    // Set Generic ANN_Layout
    ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowAlgE_Simple::Update  WARNING:: Weightfiles for _LT_  not produced yet!...." << endl;
    return;
}

//__________________________________________________________________________________________________


void EdbShowerAlgESimple::GetPara(EdbTrackP* track)
{
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::GetPara    Fill the Para structure with values from a track." << endl;

    // This parametrisation consists of these variables:
    //  0)  Axis TanTheta
    //  1)  NBT
    //  2)  BT_deltaR_mean
    //  3)  BT_deltaR_rms
    //  4)  BT_deltaT_mean
    //  5)  BT_deltaT_rms
    //  6)  longprofile[0]=eHisto_longprofile->GetBinContent(1) // number of basetracks in the SAME plate as the Initiator basetrack.
    //  7)  longprofile[1]=eHisto_longprofile->GetBinContent(2)
    //  ...
    //  5+Npl())  longprofile[Npl()-1]=eHisto_longprofile->GetBinContent(Npl()) // number of basetracks in the LAST plate of the reconstructed shower.
    //
    //==C==  DUMMY routine to get the values   deltarb  and   deltathetab  filled:
    //==C==  necessary since its an old relict from old times where this was saved only
    //==C==  as root TTree.

    EdbSegP* seg;
    Float_t shower_xb[5000];
    Float_t shower_yb[5000];
    Float_t shower_zb[5000];
    Float_t shower_txb[5000];
    Float_t shower_tyb[5000];
    Float_t shower_deltathetab[5000];
    Float_t shower_deltarb[5000];
    Float_t min_shower_deltathetab=99999;   // Reset
    Float_t min_shower_deltar=99999;        // Reset
    Float_t extrapo_diffz=0;
    Float_t extrapol_x=0;
    Float_t extrapol_y=0;

    Float_t test_shower_deltathetab;
    Float_t test_shower_deltar;
    Float_t test_shower_deltax;
    Float_t test_shower_deltay;

    Int_t TrackNSeg=track->N();

    for (int ii=0; ii<TrackNSeg; ii++)  {
        // the ii<N() => ii<TrackNSeg replacement is just an adaption from the
        // code snipplet taken from EdbShowRec
        seg=(EdbSegP*)track->GetSegment(ii);
        shower_xb[ii]=seg->X();
        shower_yb[ii]=seg->Y();
        shower_txb[ii]=seg->TX();
        shower_tyb[ii]=seg->TY();
        shower_zb[ii]=seg->Z();
        shower_deltathetab[ii]=0.5;
        shower_deltarb[ii]=200;
    }

    //-------------------------------------
    for (int ii=0; ii<TrackNSeg; ii++)  {
        seg=(EdbSegP*)track->GetSegment(ii);
        if (gEDBDEBUGLEVEL>2) {
            if (gEDBDEBUGLEVEL >2) cout << "====== --- DOING   "  << ii << endl;
            seg->PrintNice();
        }
        //-------------------------------------
        // InBT:
        if (ii==0) {
            shower_deltathetab[ii]=0.5;
            shower_deltarb[ii]=200;
        }
        // All other BTs:
        if (ii>0) {

            // PUT HERE:  calculation routine for shower_deltathetab, shower_deltarb
            // Exrapolate the BT [ii] to the position [jj] and then calc the
            // position and slope differences for the best matching next segment.
            // For the backward extrapolation of the   shower_deltathetab and shower_deltarb
            // calulation for BaseTrack(ii), Basetrack(jj)->Z() hast to be smaller.
            min_shower_deltathetab=99999;   // Reset
            min_shower_deltar=99999;        // Reset

            for (int jj=0; jj<track->N(); jj++)  {
                if (ii==jj) continue;

                // since we do not know if BTs are ordered by their Z positions:
                // and cannot cut directly on the number in the shower entry:
                if (shower_zb[ii]<shower_zb[jj]) continue;

                extrapo_diffz=shower_zb[ii]-shower_zb[jj];
                if (TMath::Abs(extrapo_diffz)>4*1300+1.0) continue;
                if (TMath::Abs(extrapo_diffz)<1.0) continue; // remove same positions.

                extrapol_x=shower_xb[ii]-shower_txb[ii]*extrapo_diffz; // minus, because its ii after jj.
                extrapol_y=shower_yb[ii]-shower_tyb[ii]*extrapo_diffz; // minus, because its ii after jj.

                // Delta radius we need to extrapolate.
                test_shower_deltax=extrapol_x;//shower_txb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_xb[ii];
                test_shower_deltay=extrapol_y;//shower_tyb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_yb[ii];
                test_shower_deltax=test_shower_deltax-shower_xb[jj];
                test_shower_deltay=test_shower_deltay-shower_yb[jj];
                test_shower_deltar=TMath::Sqrt(test_shower_deltax*test_shower_deltax+test_shower_deltay*test_shower_deltay);

                // Delta theta we do not need to extrapolate. (old version...)
                //test_shower_deltathetab=TMath::Sqrt(shower_txb[ii]*shower_txb[ii]+shower_tyb[ii]*shower_tyb[ii]);
                //test_shower_deltathetab=test_shower_deltathetab-TMath::Sqrt(shower_txb[jj]*shower_txb[jj]+shower_tyb[jj]*shower_tyb[jj]);
                //test_shower_deltathetab=TMath::Abs(test_shower_deltathetab);
                //----
                // As before in ShowRec this way of calculation is not equivalent as calculating
                // DeltaTheta domponentwise:
                // Code from libShower:
                // delta = sqrt((SX0-a->GetTXb(l2))*(SX0-a->GetTXb(l2))+((SY0-a->GetTYb(l2))*(SY0-a->GetTYb(l2))));
                test_shower_deltathetab=TMath::Sqrt(TMath::Power(shower_txb[ii]-shower_txb[jj],2)+TMath::Power(shower_tyb[ii]-shower_tyb[jj],2));

                // Check if both dr,dt match parameter criteria and then just take these values.....
                // Maybe a change is necessary because it is not exactly the same as in the off. algorithm:
                if (test_shower_deltar<1000 && test_shower_deltathetab<2.0 ) { // open cut values
                    // Make these values equal to the one in the "official algorithm"..... 150microns and 100mrad.
                    //if (test_shower_deltar<150 && test_shower_deltathetab<0.15 ) {   // frederics cut values
                    /// -----     IMPORTANT::  these areopen cut values for best combifinding of pair BT deltaR/Theta values
                    /// -----     IMPORTANT::  then you do NOT necessarily get back your values which you put in durign
                    /// -----     IMPORTANT::  your shower reconstruction cone ( deltaR/Theta cutvalues could be NO cutvalues
                    /// -----     IMPORTANT::  for some reconstruction algorithms for example, but we wanna have these values anyway.
                    ///  In Any Case:
                    ///  Frederics Cut looks only for best min_shower_deltar so we do also.
                    if (test_shower_deltar<min_shower_deltar) {
                        min_shower_deltathetab=test_shower_deltathetab;
                        min_shower_deltar=test_shower_deltar;
                        shower_deltathetab[ii]=min_shower_deltathetab;
                        shower_deltarb[ii]=min_shower_deltar;
                    }
                }
            }
        }
        //-------------------------------------

    } // for (int ii=0;ii<N();ii++)
    //-------------------------------------

    //-------------------------------------
    if (gEDBDEBUGLEVEL>2) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbTrackP:  N() nsegments= " << track->N() << endl;
        for (int ii=0; ii<track->N(); ii++)  {
            if (gEDBDEBUGLEVEL >2) cout << "Shower:  nentry= " << ii << "  shower_zb[ii] =  " << shower_zb[ii] << "  shower_deltathetab[ii] =  " << shower_deltathetab[ii] << "  shower_deltarb[ii] =  " << shower_deltarb[ii] <<endl;
        }
    }
    //-------------------------------------

    // Profile starting with (arrayindex ==0): number of basetracks in the SAME plate as the Initiator basetrack.
    // Profile ending with (arrayindex ==56): // number of basetracks in the LAST plate of the reconstructed shower.
    Int_t   longprofile[57];

    //Int_t numberofilms=Npl(); // Historical reasons why there are more names for the same variable...
    //Int_t nbfilm=Npl();       // Historical reasons why there are more names for the same variable...
    Int_t nbfilm=track->Npl();

    // Int_t   numberoffilsforlongprofile=numberofilms+1; // it is one more going from [1..nbfilm] !!!// Not used in this routine.

    Float_t Dr[57];
    Float_t X0[57];
    Float_t Y0[57];
    Float_t TX0,TY0;
    Float_t theta[57];
    Float_t dist;
    Float_t xb[5000];
    Float_t yb[5000];
    Float_t zb[5000];
    Float_t txb[5000];
    Float_t tyb[5000];
    Int_t   nfilmb[5000];
    //Float_t deltathetab[5000]; // Not used in this routine.
    //Float_t deltarb[5000];    // Not used in this routine.
//     Int_t sizeb=N();
    Int_t sizeb=track->N();
    //Int_t nentries_withisizeb=0;

    //=C= =====================================================================
    eHisto_nbtk_av          ->Reset();
    eHisto_nbtk             ->Reset();
    eHisto_longprofile_av   ->Reset();
    eHisto_deltaR_mean      ->Reset();
    eHisto_deltaT_mean      ->Reset();
    eHisto_deltaR_rms       ->Reset();
    eHisto_deltaT_rms       ->Reset();
    eHisto_longprofile      ->Reset();
    eHisto_deltaR           ->Reset();
    eHisto_deltaT           ->Reset();
    //=C= =====================================================================
    for (int id=0; id<57; id++) {
        theta[id]= 0;
        X0[id] = id*1300.*track->GetSegment(0)->TX() + track->GetSegment(0)->X();
        Y0[id] = id*1300.*track->GetSegment(0)->TY() + track->GetSegment(0)->Y();
        longprofile[id]=-1;
        // this is important, cause it means that where is -1 there is no BT reconstructed anymore
        // this is different for example to holese where N=0 , so one can distinguish!
    }

    if (gEDBDEBUGLEVEL>3) if (gEDBDEBUGLEVEL >2) cout << "eHisto_longprofile->GetBinWidth() "  << eHisto_longprofile->GetBinWidth(1)  << endl;
    if (gEDBDEBUGLEVEL>3) if (gEDBDEBUGLEVEL >2) cout << "eHisto_longprofile->GetBinCenter() "  << eHisto_longprofile->GetBinCenter(1)  << endl;
    if (gEDBDEBUGLEVEL>3) if (gEDBDEBUGLEVEL >2) cout << "eHisto_longprofile->GetNbinsX() "  << eHisto_longprofile->GetNbinsX()  << endl;

    TX0 = track->GetSegment(0)->TX();
    TY0 = track->GetSegment(0)->TY();
    for (Int_t j = 0; j<57; ++j) {
        Dr[j] = 0.03*j*1300. +20.0;
        //if (gEDBDEBUGLEVEL >2) cout << " DEBUG   j= " <<  j << "  Dr[j]= " <<  Dr[j] << endl;
    } // old relict from FJ. Do not remove. //
    // it represents somehow conesize......(??)

    //=C= =====================================================================
    for (Int_t ibtke = 0; ibtke < track->N(); ibtke++) {
        xb[ibtke]=track->GetSegment(ibtke)->X();
        yb[ibtke]=track->GetSegment(ibtke)->Y();
        zb[ibtke]=track->GetSegment(ibtke)->Z();
        txb[ibtke]=track->GetSegment(ibtke)->TX();
        tyb[ibtke]=track->GetSegment(ibtke)->TY();
        // abs() of filmPID with respect to filmPID of first BT, plus 1: (nfilmb(0):=1 per definition):
        // Of course PID() have to be read correctly (by ReadEdbPVrec) correctly.
        // Fedra should do it.
        nfilmb[ibtke]=TMath::Abs(track->GetSegment(ibtke)->PID()-track->GetSegment(0)->PID())+1;

        if (gEDBDEBUGLEVEL>2) {
            if (gEDBDEBUGLEVEL >2) cout << "ibtke= " <<ibtke << " xb[ibtke]= " << xb[ibtke] << " nfilmb[ibtke]= " << nfilmb[ibtke] << endl;
        }
    }
    //=C= =====================================================================
    //=C= loop over the basetracks in the shower (boucle sur les btk)
    for (Int_t ibtke = 0; ibtke < track->N(); ibtke++) {
        dist = sqrt((xb[ibtke]- X0[nfilmb[ibtke]-1])*(xb[ibtke]- X0[nfilmb[ibtke]-1])+(yb[ibtke]- Y0[nfilmb[ibtke]-1])*(yb[ibtke]- Y0[nfilmb[ibtke]-1]));

        // inside the cone
        //if (gEDBDEBUGLEVEL >2) cout << "ibtke= " <<ibtke << "   dist =  "  <<  dist << "  Dr[nfilmb[ibtke]-1] = " <<  Dr[nfilmb[ibtke]-1] << endl;
        //if (gEDBDEBUGLEVEL >2) cout << "    nfilmb[ibtke]  =  " << nfilmb[ibtke] <<   "   nbfilm =  " << nbfilm << endl;

        // In old times there was here an additional condition which checked the BTs for the ConeDistance.
        // Since at this point in buildparametrisation_FJ the shower is -already fully reconstructed -
        // (by either EdbShoAlgo, or ShowRec program or manual list) this intruduces an additional cut
        // which is not correct because conetube size is algorithm specifiv (see Version.log.txt #20052010)
        // Thats wy we drop it here....
        // 	    if (dist<Dr[nfilmb[ibtke]-1]&&nfilmb[ibtke]<=nbfilm) {  // original if line comdition
        if (dist>Dr[nfilmb[ibtke]-1]) {
            if (gEDBDEBUGLEVEL>2) {
                if (gEDBDEBUGLEVEL >2) cout << " WARNING , In old times this cut (dist>Dr[nfilmb[ibtke]-1]) (had also to be fulfilled!"<<endl;
                if (gEDBDEBUGLEVEL >2) cout << "            For this specific shower it seems not the case....." << endl;
                if (gEDBDEBUGLEVEL >2) cout << "           You might want to check this shower again manualy to make sure everything is correct....." << endl;
            }
        }
        if (nfilmb[ibtke]<=nbfilm) {
            //if (gEDBDEBUGLEVEL >2) cout << "DEBUG CUTCONDITION WITHOUT THE (?_?_? WRONG ?_?_?) CONE DIST CONDITION....." << endl;
            // if (gEDBDEBUGLEVEL >2) cout << yes, this additional cut is not necessary anymore, see above....

            eHisto_longprofile        ->Fill(nfilmb[ibtke]);
            eHisto_longprofile_av     ->Fill(nfilmb[ibtke]);

            Double_t DR=0;  //Extrapolate the old stlye way:
            Double_t Dx=xb[ibtke]-(xb[0]+(zb[ibtke]-zb[0])*txb[0]);
            Double_t Dy=yb[ibtke]-(yb[0]+(zb[ibtke]-zb[0])*tyb[0]);
            DR=TMath::Sqrt(Dx*Dx+Dy*Dy);
            eHisto_transprofile_av->Fill(DR);
            eHisto_transprofile->Fill(DR);

            theta[nfilmb[ibtke]]+= (TX0-txb[ibtke])*(TX0-txb[ibtke])+(TY0-tyb[ibtke])*(TY0-tyb[ibtke]);
            if (ibtke>0&&nfilmb[ibtke]<=nbfilm) {
                // eHisto_deltaR           ->Fill(deltarb[ibtke]);
                // eHisto_deltaT           ->Fill(deltathetab[ibtke]);
                // uses shower_deltarb,shower_deltathetab just calculated in dummy routine above.
                // The first BT is NOT used for this filling since the extrapolated values of
                // shower_deltarb and shower_deltathetab are set manually to 0.5 and 200, due to
                // historical reasons (these variables com from the e/pi separation stuff).
                eHisto_deltaR           ->Fill(shower_deltarb[ibtke]);
                eHisto_deltaT           ->Fill(shower_deltathetab[ibtke]);
            }
        }
    }//==C== END OF loop over the basetracks in the shower
    //if (gEDBDEBUGLEVEL >2) cout <<"---------------------------------------"<<endl;
    //=======================================================================================
    //==C== Fill NumberBT Histogram for all showers:
    eHisto_nbtk                   ->Fill(sizeb);
    eHisto_nbtk_av                ->Fill(sizeb);
    //==C== Fill dR,dT Mean and RMS Histos for all showers:
    eHisto_deltaR_mean            ->Fill(eHisto_deltaR->GetMean());
    eHisto_deltaT_mean            ->Fill(eHisto_deltaT->GetMean());
    eHisto_deltaR_rms             ->Fill(eHisto_deltaR->GetRMS());
    eHisto_deltaT_rms             ->Fill(eHisto_deltaT->GetRMS());
    //=======================================================================================

    // Fill the longprofile array:  (NEW VERSION (arrayindex 0 is same plate as Initiator BT))
    for (Int_t i=1; i<=nbfilm; ++i) {
        // longprofile[i-1] = eHisto_longprofile->GetBinContent(i);    /// OLD VERSION for (Int_t i=1; i<=nbfilm+1; ++i)
        longprofile[i-1] = (Int_t)(eHisto_longprofile->GetBinContent(i+1));     //  NEW VERSION (arrayindex 0 is same plate as Initiator BT)
        //test+=longprofile[i-1] ;
        if (gEDBDEBUGLEVEL>1) {
            if (gEDBDEBUGLEVEL >2) cout << "i= " << i << " longprofile[i-1] "<< longprofile[i-1] << " eHisto_longprofile->GetBinContent(i) " << eHisto_longprofile->GetBinContent(i+1)<< endl;
        }
        if (i==nbfilm) {
            if (gEDBDEBUGLEVEL >2) cout << "i==nbfilm:" << endl;
            longprofile[i-1] = (Int_t)(eHisto_longprofile->GetBinContent(i+1));
        }
        //
    }
    // Rather strange but I have to put it explicetly here, otherwise last bin isnt filled...
    longprofile[nbfilm] = (Int_t)(eHisto_longprofile->GetBinContent(nbfilm+1));
    for (Int_t i=nbfilm+2; i<57; ++i) {
        longprofile[i-1] = -1;
    }
    //----------------------------------------------------------------------------------------

    // - Inclusion. only for libShower, EdbShowerRec class!
    Float_t eShowerAxisAngle=track->GetSegment(0)->Theta();
    // - Inclusion. only for libShower, EdbShowerRec class; END;


    // Now set parametrisation values:
    eParaShowerAxisAngle=eShowerAxisAngle;
    eParanseg=track->N();
    eParaBT_deltaR_mean = eHisto_deltaR->GetMean();
    eParaBT_deltaR_rms  = eHisto_deltaR->GetRMS();
    eParaBT_deltaT_mean = eHisto_deltaT->GetMean();
    eParaBT_deltaT_rms  = eHisto_deltaT->GetRMS();
    for (int ii=0; ii<57; ii++) eParalongprofile[ii]=longprofile[ii];
    //for (int ii=0; ii<57; ii++) cout << " ii= " << ii <<  "  eParalongprofile[ii]=  " <<  eParalongprofile[ii] << "   longprofile[ii]= " << longprofile[ii] << endl;

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::GetPara    Fill the Para structure with values from a track.... done." << endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlgESimple::ReadCorrectionFactors(TString weigthstring, Float_t &p0, Float_t &p1) {
    // Read Linear Correction factors p0 p1 from this file.
    // Format: p0 p1
    float pp0,pp1;
    const char* name=weigthstring.Data();
    FILE * pFile=NULL;
    if (gEDBDEBUGLEVEL >2) cout << " open file " << endl;
    pFile = fopen (name,"r");
    if (NULL==pFile) return;
    int dummy = fscanf (pFile, "%f %f", &pp0, &pp1);
    if (dummy!=2) {
        cout << "EdbShowerAlgESimple::ReadCorrectionFactors   ERROR! Wrong formatted file! Could not read the two parameters! Set them to default values! Please check if the weightfiles in...: "<< endl;
        cout << weigthstring.Data() << endl;
        cout << "EdbShowerAlgESimple::ReadCorrectionFactors  ... exist!"<< endl;
        p0=0.0;
        p1=1.0;
    }
    p0=pp0;
    p1=pp1;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::ReadCorrectionFactors   p0: " << p0 << "  p1 " << p1 << endl;
    if (gEDBDEBUGLEVEL >2) printf ("I have read: %f and %f , in total %d arguments.\n",pp0,pp1,dummy);
    fclose (pFile);
    return;
}

//______________________________________________________________________________

void EdbShowerAlgESimple::Print() {
    cout << "-------------------------------------------------------------------------------------------"<< endl;
    cout << "EdbShowerAlgESimple::Print " << endl;
    PrintSpecifications();

    cout << "EdbShowerAlgESimple::Print    Now correction factors and weightfilestrings:" << endl;
    for (int i=0; i<15; i++) {
        cout << "EdbShowerAlgESimple::Print       eANN_MLP_CORR_0[" << i << "]="<<eANN_MLP_CORR_0[i] << endl;
        cout << "EdbShowerAlgESimple::Print       eANN_MLP_CORR_1[" << i << "]="<<eANN_MLP_CORR_1[i] << endl;
        cout << "EdbShowerAlgESimple::Print       ANN_WeightFile_ARRAY[" << i << "]="<<ANN_WeightFile_ARRAY[i] << endl;
    }

    cout << "EdbShowerAlgESimple::Print ...done." << endl;
    cout << "-------------------------------------------------------------------------------------------"<< endl << endl;
    return;
}

//______________________________________________________________________________
//______________________________________________________________________________

void EdbShowerAlgESimple::WriteNewRootFile(TString sourcefilename, TString treename)
{
    cout << "-------------------------------------------------------------------------------------------"<< endl;
    cout << endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       -------------------------------------------"<< endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       This is NOT a good solution! But what shall" << endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       I do?" << endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       We rewrite completely the new tree and add " << endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       the new calculated energy values to it..." << endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       -------------------------------------------"<< endl;
    cout << endl;

    //if (sourcefilename!="Shower.root") cout << "EdbShowerAlgESimple::WriteNewRootFile       Attention: sourcefilename is different: "<< sourcefilename << endl;
    //if (treename!="treebranch") cout << "EdbShowerAlgESimple::WriteNewRootFile       Attention: treename is different: "<< treename << endl;

    cout << "EdbShowerAlgESimple::WriteNewRootFile       Open sourcefilename:: "<< sourcefilename << endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       Open treename:: "<< treename << endl;

    //-     VARIABLES: shower_  "treebranch"  reconstruction
    Int_t shower_number_eventb, shower_sizeb, shower_isizeb,shower_showerID;
    Int_t shower_sizeb15, shower_sizeb20, shower_sizeb30;
    Float_t shower_energy_shot_particle;
    Float_t shower_xb[5000];
    Float_t shower_yb[5000];
    Float_t shower_zb[5000];
    Float_t shower_txb[5000];
    Float_t shower_tyb[5000];
    Float_t shower_deltarb[5000];
    Float_t shower_deltathetab[5000];
    Float_t shower_deltaxb[5000];
    Float_t shower_deltayb[5000];
    Int_t   shower_nfilmb[5000];
    Float_t shower_chi2btkb[5000];
    Int_t shower_ntrace1simub[5000]; // MCEvt
    Int_t shower_ntrace2simub[5000]; // s->W()
    Float_t shower_ntrace3simub[5000]; // s->P()
    Int_t shower_ntrace4simub[5000]; // s->Flag()
    Float_t shower_tagprimary[5000];
    Int_t   shower_idb[5000];
    Int_t   shower_plateb[5000];
    Float_t shower_deltasigmathetab[58];
    Int_t   shower_numberofilms;
    Float_t shower_purb; // purity of shower
    Int_t shower_eProb90;
    Int_t shower_eProb1;
    Int_t shower_Size;            // number of BT in the shower
    Int_t shower_Size15;          // number of BT in the shower (for 15 films crossed)
    Int_t shower_Size20;          // number of BT in the shower (for 20 films crossed)
    Int_t shower_Size30;          // number of BT in the shower (for 30 films crossed)
    Float_t shower_output;        // Neural Network output for e/pi separation
    Float_t shower_output15;      // Neural Network output for e/pi separation (for 15 films crossed)
    Float_t shower_output20;      // Neural Network output for e/pi separation (for 20 films crossed)
    Float_t shower_output30;      // Neural Network output for e/pi separation (for 30 films crossed)
    Float_t shower_output50;      // Neural Network output for e/pi separation (for 50 films crossed)
    Float_t shower_purityb;
    Float_t shower_trackdensb;
    Float_t shower_E_MC;
    Float_t shower_EnergyCorrectedb;
    Float_t shower_EnergyUnCorrectedb;
    Float_t shower_EnergySigmaCorrectedb;
    Float_t shower_EnergySigmaUnCorrectedb;

    Float_t shower_OldEnergyCorrectedb;
    Float_t shower_OldEnergyUnCorrectedb;
    Float_t shower_OldEnergySigmaCorrectedb;
    Float_t shower_OldEnergySigmaUnCorrectedb;

    //- Old Shower File:
    TFile* fileOld = new TFile(sourcefilename.Data(),"READ");
    // Set Addresses of treebranch tree:
    TTree* eShowerTree = (TTree*)fileOld->Get(treename);
    cout << "eShowerTree = " << eShowerTree << endl;
    eShowerTree->SetBranchAddress("number_eventb",&shower_number_eventb);
    eShowerTree->SetBranchAddress("sizeb",&shower_sizeb);
    eShowerTree->SetBranchAddress("sizeb15",&shower_sizeb15);
    eShowerTree->SetBranchAddress("sizeb20",&shower_sizeb20);
    eShowerTree->SetBranchAddress("sizeb30",&shower_sizeb30);
/// 		eShowerTree->SetBranchAddress("output", &shower_output);
    eShowerTree->SetBranchAddress("output15", &shower_output15);
    eShowerTree->SetBranchAddress("output20", &shower_output20);
    eShowerTree->SetBranchAddress("output30", &shower_output30);
    eShowerTree->SetBranchAddress("output50", &shower_output50);
    eShowerTree->SetBranchAddress("eProb1", &shower_eProb1);
    eShowerTree->SetBranchAddress("eProb90", &shower_eProb90);
    eShowerTree->SetBranchAddress("isizeb",&shower_isizeb);
    eShowerTree->SetBranchAddress("xb",shower_xb);
    eShowerTree->SetBranchAddress("yb",shower_yb);
    eShowerTree->SetBranchAddress("zb",shower_zb);
    eShowerTree->SetBranchAddress("txb",shower_txb);
    eShowerTree->SetBranchAddress("tyb",shower_tyb);
    eShowerTree->SetBranchAddress("nfilmb",shower_nfilmb);
    eShowerTree->SetBranchAddress("ntrace1simub",shower_ntrace1simub);  // s.eMCEvt
    eShowerTree->SetBranchAddress("ntrace2simub",shower_ntrace2simub); // s.eW
    eShowerTree->SetBranchAddress("ntrace3simub",shower_ntrace3simub); // s.eP
    eShowerTree->SetBranchAddress("ntrace4simub",shower_ntrace4simub); // s.eFlag
    eShowerTree->SetBranchAddress("chi2btkb",shower_chi2btkb);
    eShowerTree->SetBranchAddress("deltarb",shower_deltarb);
    eShowerTree->SetBranchAddress("deltathetab",shower_deltathetab);
    eShowerTree->SetBranchAddress("deltaxb",shower_deltaxb);
    eShowerTree->SetBranchAddress("deltayb",shower_deltayb);
    eShowerTree->SetBranchAddress("tagprimary",shower_tagprimary);
//     eShowerTree->SetBranchAddress("energy_shot_particle",&shower_energy_shot_particle);
    eShowerTree->SetBranchAddress("E_MC",&shower_E_MC);
    eShowerTree->SetBranchAddress("showerID",&shower_showerID);
    eShowerTree->SetBranchAddress("idb",shower_idb);
    eShowerTree->SetBranchAddress("plateb",shower_plateb);
//     eShowerTree->SetBranchAddress("deltasigmathetab",shower_deltasigmathetab);
//     eShowerTree->SetBranchAddress("lenghtfilmb",&shower_numberofilms);
    eShowerTree->SetBranchAddress("purityb",&shower_purityb);
    eShowerTree->SetBranchAddress("Energy",&shower_OldEnergyCorrectedb);
    eShowerTree->SetBranchAddress("EnergyUnCorrected",&shower_OldEnergyUnCorrectedb);
    eShowerTree->SetBranchAddress("EnergySigma",&shower_OldEnergySigmaCorrectedb);
    eShowerTree->SetBranchAddress("EnergySigmaUnCorrected",&shower_OldEnergySigmaUnCorrectedb);


    Int_t nent=eShowerTree->GetEntries();
    cout << "EdbShowerAlgESimple::WriteNewRootFile       eShowerTree="<< eShowerTree <<"      ------"<< endl;
    cout << "EdbShowerAlgESimple::WriteNewRootFile       eShowerTree->GetEntries()="<< nent <<"      ------"<< endl;


    // Create the new File
    TFile* fileNew = new TFile("New.root","RECREATE");
    // Create the new Tree
    TTree* ShowerTreeNew=new TTree("treebranch","treebranch");
    cout << "EdbShowerAlgESimple::WriteNewRootFile       ShowerTreeNew="<< ShowerTreeNew <<"      ------"<< endl;
    // Create the new Branches:
    ShowerTreeNew->Branch("number_eventb",&shower_number_eventb,"number_eventb/I");
    ShowerTreeNew->Branch("sizeb",&shower_sizeb,"sizeb/I");
    ShowerTreeNew->Branch("sizeb15",&shower_sizeb15,"sizeb15/I");
    ShowerTreeNew->Branch("sizeb20",&shower_sizeb20,"sizeb20/I");
    ShowerTreeNew->Branch("sizeb30",&shower_sizeb30,"sizeb30/I");
    ShowerTreeNew->Branch("output15",&shower_output15,"output15/F");
    ShowerTreeNew->Branch("output20",&shower_output20,"output20/F");
    ShowerTreeNew->Branch("output30",&shower_output30,"output30/F");
    ShowerTreeNew->Branch("output50",&shower_output50,"output50/F");
    ShowerTreeNew->Branch("eProb90",&shower_eProb90,"eProb90/I");
    ShowerTreeNew->Branch("eProb1",&shower_eProb1,"eProb1/I");
    ShowerTreeNew->Branch("E_MC",&shower_E_MC,"E_MC/F");
    ShowerTreeNew->Branch("idb",shower_idb,"idb[sizeb]/I");
    ShowerTreeNew->Branch("plateb",shower_plateb,"plateb[sizeb]/I");
    ShowerTreeNew->Branch("showerID",&shower_showerID,"showerID/I");
    ShowerTreeNew->Branch("isizeb",&shower_isizeb,"isizeb/I");
    ShowerTreeNew->Branch("xb",shower_xb,"xb[sizeb]/F");
    ShowerTreeNew->Branch("yb",shower_yb,"yb[sizeb]/F");
    ShowerTreeNew->Branch("zb",shower_zb,"zb[sizeb]/F");
    ShowerTreeNew->Branch("txb",shower_txb,"txb[sizeb]/F");
    ShowerTreeNew->Branch("tyb",shower_tyb,"tyb[sizeb]/F");
    ShowerTreeNew->Branch("nfilmb",shower_nfilmb,"nfilmb[sizeb]/I");
    ShowerTreeNew->Branch("ntrace1simub",shower_ntrace1simub,"ntrace1simu[sizeb]/I");
    ShowerTreeNew->Branch("ntrace2simub",shower_ntrace2simub,"ntrace2simu[sizeb]/I");
    ShowerTreeNew->Branch("ntrace3simub",shower_ntrace3simub,"ntrace3simu[sizeb]/F");
    ShowerTreeNew->Branch("ntrace4simub",shower_ntrace4simub,"ntrace4simu[sizeb]/I");
    ShowerTreeNew->Branch("chi2btkb",shower_chi2btkb,"chi2btkb[sizeb]/F");
    ShowerTreeNew->Branch("deltarb",shower_deltarb,"deltarb[sizeb]/F");
    ShowerTreeNew->Branch("deltathetab",shower_deltathetab,"deltathetab[sizeb]/F");
    ShowerTreeNew->Branch("deltaxb",shower_deltaxb,"deltaxb[sizeb]/F");
    ShowerTreeNew->Branch("deltayb",shower_deltayb,"deltayb[sizeb]/F");
    ShowerTreeNew->Branch("tagprimary",shower_tagprimary,"tagprimary[sizeb]/F");
    ShowerTreeNew->Branch("purityb",&shower_purityb,"purityb/F");
    ShowerTreeNew->Branch("trackdensb",&shower_trackdensb,"trackdensb/F");

    ShowerTreeNew->Branch("Energy",&shower_EnergyCorrectedb,"EnergyCorrectedb/F");
    ShowerTreeNew->Branch("EnergyUnCorrected",&shower_EnergyUnCorrectedb,"EnergyUnCorrectedb/F");
    ShowerTreeNew->Branch("EnergySigma",&shower_EnergySigmaCorrectedb,"EnergySigmaCorrectedb/F");
    ShowerTreeNew->Branch("EnergySigmaUnCorrected",&shower_EnergySigmaUnCorrectedb,"EnergySigmaUnCorrectedb/F");

    ShowerTreeNew->Branch("OldEnergy",&shower_OldEnergyCorrectedb,"OldEnergy/F");
    ShowerTreeNew->Branch("OldEnergyUnCorrected",&shower_OldEnergyUnCorrectedb,"OldEnergyUnCorrected/F");
    ShowerTreeNew->Branch("OldEnergySigma",&shower_OldEnergySigmaCorrectedb,"OldEnergySigma/F");
    ShowerTreeNew->Branch("OldEnergySigmaUnCorrected",&shower_OldEnergySigmaUnCorrectedb,"OldEnergySigmaUnCorrected/F");


    // Loop over Tree Entries (==different showers):
    for (int i=0; i<nent; ++i) {
        eShowerTree->GetEntry(i);
        //cout << "i = " << i << " " << eEnergyArrayCount << "  shower_EnergyCorrectedb = " <<  eEnergyArray->At(i) <<  endl;
//         eShowerTree->Show(i);
        shower_EnergyCorrectedb=eEnergyArray->At(i);
        shower_EnergyUnCorrectedb=eEnergyArrayUnCorrected->At(i);
        shower_EnergySigmaCorrectedb=eEnergyArraySigmaCorrected->At(i);
        shower_EnergySigmaUnCorrectedb=-12345;

        // Fill new Tree
        ShowerTreeNew->Fill();
    }
    ShowerTreeNew->Write();
    fileNew->Close();
    fileOld->Close();


    gSystem->Exec("mv -vf Shower.root Shower.Orig.root");
    gSystem->Exec("mv -vf New.root Shower.root");
    cout << "EdbShowerAlgESimple::WriteNewRootFile...done."<<endl;
    cout << "-------------------------------------------------------------------------------------------"<< endl<< endl;
    return;
}


//______________________________________________________________________________

void EdbShowerAlgESimple::Help()
{
    cout << "-------------------------------------------------------------------------------------------"<< endl<< endl;
    cout << "EdbShowerAlgESimple::Help:"<<endl;
    cout << "EdbShowerAlgESimple::Help:"<<endl;
    cout << "EdbShowerAlgESimple::Help:  The easiest way:  "<<endl;
    cout << "EdbShowerAlgESimple::Help:  EdbShowerAlgESimple* ShowerAlgE1 = new EdbShowerAlgESimple();  "<<endl;
    cout << "EdbShowerAlgESimple::Help:  ShowerAlgE1->DoRun(RecoShowerArray);  // RecoShowerArray an TObjArray of EdbTrackP* "<<endl;
    cout << "EdbShowerAlgESimple::Help:  ShowerAlgE1->WriteNewRootFile(); "<<endl;
    cout << "EdbShowerAlgESimple::Help:  "<<endl;
    cout << "EdbShowerAlgESimple::Help:  In shower_E.C // E.C you find more hints...."<<endl;
    cout << "EdbShowerAlgESimple::Help:  Updates will follow........"<<endl;
    cout << "-------------------------------------------------------------------------------------------"<< endl<< endl;
}


//______________________________________________________________________________


void EdbShowerAlgESimple::PrintEfficiencyParametrisation()
{
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation for angles theta=0,0.1,..,0.6:" << endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0): "<< eEfficiencyParametrisation->Eval(0)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.1): "<< eEfficiencyParametrisation->Eval(0.1)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.2): "<< eEfficiencyParametrisation->Eval(0.2)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.3): "<< eEfficiencyParametrisation->Eval(0.3)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.4): "<< eEfficiencyParametrisation->Eval(0.4)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.5): "<< eEfficiencyParametrisation->Eval(0.5)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.6): "<< eEfficiencyParametrisation->Eval(0.6)<< endl;
    return;
}


//______________________________________________________________________________


void EdbShowerAlgESimple::ReadTables()
{
    cout << "EdbShowerAlgESimple::ReadTables"<<endl;

    ReadTables_Energy();

    cout << "EdbShowerAlgESimple::ReadTables...done."<<endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlgESimple::ReadTables_Energy()
{
    cout << "EdbShowerAlgESimple::ReadTables_Energy"<<endl;

    char name [100];
    FILE * fFile;

    Int_t N_E=12;
    Int_t N_NPL=13;
    Int_t NPL[13]= {10,12,14,16,18,20,23,26,29,32,35,40,45};
    Double_t	E[12]  = {0.5,1.0,2.0, 4.0, 0.75, 1.5, 3.0, 6.0, 8.0, 16.0, 32.0 ,64.0};
    Int_t   	E_ASCEND[12]  = {0,4,1,5,2,6,3,7,8,9,10,11};
    Int_t npl;
    Float_t energyresolution;
    Double_t E_Array_Ascending[12]= {0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0, 8.0, 16.0, 32.0 ,64.0};
    Double_t E_Resolution[12]= {0.5,0.75,1.0,1.5,2.0,3.0,4.0,6.0, 8.0, 16.0, 32.0 ,64.0}; // initialize with 100% resolution...

    // First Create the ObjArray storing the Splines...
    eSplineArray_Energy_Stat_Electron = new TObjArray();
    eSplineArray_Energy_Stat_Gamma = new TObjArray();
    eSplineArray_Energy_Sys_Electron = new TObjArray();
    eSplineArray_Energy_Sys_Gamma = new TObjArray();

    //We need to give the full path to look for the Statistics tables:
    TString basicstring = TString(gSystem->ExpandPathName("$FEDRA_ROOT"));
    TString addstring = ("/src/libShower/");
    TString tablestring;

    // A) Table: Statistics: Electrons: "libShower_Energy_Statistics_Electron.txt"
    //sprintf(name,"tables/libShower_Energy_Statistics_Electron.txt");
    tablestring=basicstring+addstring+TString("tables/libShower_Energy_Statistics_Electron.txt");
    fFile = fopen (tablestring.Data(),"r");
    cout << "EdbShowerAlgESimple::ReadTables_Energy() name = " << tablestring << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        if (gEDBDEBUGLEVEL>2) cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Stat_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        if (gEDBDEBUGLEVEL>2) cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Stat_Electron->Add(Spline);
    }
    fclose (fFile);


    // B) Table: Statistics: Gamma: "libShower_Energy_Statistics_Gamma.txt"
    tablestring=basicstring+addstring+TString("tables/libShower_Energy_Statistics_Gamma.txt");
    fFile = fopen (tablestring.Data(),"r");
    cout << "EdbShowerAlgESimple::ReadTables_Energy() name = " << tablestring << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        if (gEDBDEBUGLEVEL>2) cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Stat_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        if (gEDBDEBUGLEVEL>2) cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Stat_Gamma->Add(Spline);
    }
    fclose (fFile);



    // C) Table: Systematics: Electrons: "libShower_Energy_Systematics_Electron.txt"
    tablestring=basicstring+addstring+TString("tables/libShower_Energy_Systematics_Electron.txt");
    fFile = fopen (tablestring.Data(),"r");
    cout << "EdbShowerAlgESimple::ReadTables_Energy() name = " << tablestring << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        if (gEDBDEBUGLEVEL>2) cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Sysy_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        if (gEDBDEBUGLEVEL>2) cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Sys_Electron->Add(Spline);
    }
    fclose (fFile);


    // D) Table: Systematics: Gamma: "libShower_Energy_Systematics_Gamma.txt"
    tablestring=basicstring+addstring+TString("tables/libShower_Energy_Systematics_Gamma.txt");
    fFile = fopen (tablestring.Data(),"r");
    cout << "EdbShowerAlgESimple::ReadTables_Energy() name = " << tablestring << endl;

    for (int i=0; i<N_NPL; i++) {
        // Read energy values into array:
        int narg=fscanf(fFile, "%d ",&npl);
        if (gEDBDEBUGLEVEL>2) cout << "NPL= " << npl << endl;
        for (int j=0; j<N_E-1; j++) {
            int narg=fscanf(fFile, "%f",&energyresolution);
            E_Resolution[j]=(Double_t)energyresolution;
            if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[j] << " = " << energyresolution << endl;
        }
        narg=fscanf(fFile, "%f \n",&energyresolution);
        E_Resolution[N_E-1]=(Double_t)energyresolution;
        if (gEDBDEBUGLEVEL>2) cout << " energyresolution @ " << E_Array_Ascending[N_E-1] << " = " << energyresolution << endl;

        TString splinename=TString(Form("Spline_Sysy_Electron_Npl_%d",NPL[i]));
        TSpline3* Spline = new TSpline3(splinename,E_Array_Ascending,E_Resolution,10);

        if (gEDBDEBUGLEVEL>2) cout << "Created Spline. Add Spline to ObjArray." << endl;
        eSplineArray_Energy_Sys_Gamma->Add(Spline);
    }
    fclose (fFile);


    cout << "EdbShowerAlgESimple::ReadTables_Energy...done."<<endl;
    return;
}


//______________________________________________________________________________


void EdbShowerAlgESimple::SetEfficiencyParametrisationAngles()
{
    //EffFunc_UserEfficiency->Set
    return;
}

//______________________________________________________________________________

void EdbShowerAlgESimple::SetEfficiencyParametrisationValues(Double_t* Angles, Double_t* EffValuesAtAngles)
{
    cout << "EdbShowerAlgESimple::SetEfficiencyParametrisationValues()" << endl;
    cout << "ATTENTION: Array has to consist of efficiencies at !seven! angles: 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6..." << endl;
    cout << "ATTENTION: If not, then it might crash! " << endl;
    // Set User Efficiency Angles by hand!
    // This is interface routine which than can be used by eda...
    //delete eEfficiencyParametrisation;
    eEfficiencyParametrisation = new TSpline3("",Angles,EffValuesAtAngles,5,0,0,0.6);

    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation for angles theta=0,0.1,..,0.6:" << endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0): "<< eEfficiencyParametrisation->Eval(0)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.1): "<< eEfficiencyParametrisation->Eval(0.1)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.2): "<< eEfficiencyParametrisation->Eval(0.2)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.3): "<< eEfficiencyParametrisation->Eval(0.3)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.4): "<< eEfficiencyParametrisation->Eval(0.4)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.5): "<< eEfficiencyParametrisation->Eval(0.5)<< endl;
    cout << "EdbShowerAlgESimple::eEfficiencyParametrisation->Eval(0.6): "<< eEfficiencyParametrisation->Eval(0.6)<< endl;

    cout << "EdbShowerAlgESimple::SetEfficiencyParametrisationValues()  Do UnSetForceSpecificationReload" << endl;
    UnSetForceSpecificationReload();
    return;
}















//______________________________________________________________________________

EdbShowerAlgIDSimple::EdbShowerAlgIDSimple()
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple()   Default Constructor"<<endl;
    Set0();
    Init();
    cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple()   Default Constructor...done."<<endl;
}


//______________________________________________________________________________

/// CONSTRUCTOR USED FOR libShowRec with EdbShowerP class available.
// EdbShowerAlgESimple::EdbShowerAlgESimple(EdbShowerP* shower)
// {
//     // Default Constructor...
//     if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(EdbShowerP* shower)   Default Constructor"<<endl;
//     Set0();
//     Init();
//     // ... with Shower:
//     eRecoShowerArray=new TObjArray();
//     eRecoShowerArray->Add(shower);
//     SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
//     if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgESimple::EdbShowerAlgESimple(EdbShowerP* shower)   Default Constructor...done."<<endl;
// }

//______________________________________________________________________________

EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(EdbTrackP* track)
{
    // Default Constructor...
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(EdbTrackP* track)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with Shower:
    eRecoShowerArray=new TObjArray();
    eRecoShowerArray->Add(track);
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(EdbTrackP* track)   Default Constructor...done."<<endl;
}


//______________________________________________________________________________

EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(TObjArray* RecoShowerArray)
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(TObjArray* RecoShowerArray)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with ShowerArray:
    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(TObjArray* RecoShowerArray)   Default Constructor...done."<<endl;
}

//______________________________________________________________________________

EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(EdbTrackP* track, EdbVertex* vtx)
{
    // Default Constructor...
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(EdbTrackP* track, EdbVertex* vtx)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with Shower:
    eRecoShowerArray=new TObjArray();
    eRecoShowerArray->Add(track);
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(EdbTrackP* track, EdbVertex* vtx)   Default Constructor...done."<<endl;
}


//______________________________________________________________________________

EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(TObjArray* RecoShowerArray, TObjArray* VtxArray)
{
    // Default Constructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(TObjArray* RecoShowerArray, TObjArray* VtxArray)   Default Constructor"<<endl;
    Set0();
    Init();
    // ... with ShowerArray:
    eRecoShowerArray=RecoShowerArray;
    eRecoShowerArrayN=eRecoShowerArray->GetEntries();
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::EdbShowerAlgIDSimple(TObjArray* RecoShowerArray, TObjArray* VtxArray)   Default Constructor...done."<<endl;
}

//______________________________________________________________________________

EdbShowerAlgIDSimple::~EdbShowerAlgIDSimple()
{
    // Default Destructor
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::~EdbShowerAlgIDSimple()"<<endl;
    // Delete Histograms (on heap):
    delete eHisto_nbtk_av;
    delete eHisto_longprofile_av;
    delete eHisto_transprofile_av;
    delete eHisto_deltaR_mean;
    delete eHisto_deltaT_mean;
    delete eHisto_deltaR_rms;
    delete eHisto_deltaT_rms;
    delete eHisto_nbtk;
    delete eHisto_longprofile;
    delete eHisto_transprofile;
    delete eHisto_deltaR;
    delete eHisto_deltaT;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::~EdbShowerAlgIDSimple()...done."<<endl;
}

//______________________________________________________________________________


void EdbShowerAlgIDSimple::ReadTables()
{
    cout << "EdbShowerAlgIDSimple::ReadTables"<<endl;
    ReadTables_ID();
    cout << "EdbShowerAlgIDSimple::ReadTables...done."<<endl;
    return;
}


//______________________________________________________________________________

void EdbShowerAlgIDSimple::ReadTables_ID()
{
    cout << "EdbShowerAlgIDSimple::ReadTables_ID"<<endl;

    /// First guess until tables are fully read in.
    cout << "First guess until tables are fully read in." << endl;
    eIDCutTypeValue[0]=0.5;
    eIDCutTypeValue[1]=0.75;
    eIDCutTypeValue[2]=0.25;
    cout << "EdbShowerAlgIDSimple::ReadTables_ID...done"<<endl;
    return;
}


//______________________________________________________________________________

void EdbShowerAlgIDSimple::Print()
{
    cout << "EdbShowerAlgIDSimple::Print"<<endl;
    cout << "EdbShowerAlgIDSimple::Print...done"<<endl;
    return;
}


//______________________________________________________________________________

void EdbShowerAlgIDSimple::Help()
{
    cout << "EdbShowerAlgIDSimple::Help"<<endl;
    cout << "EdbShowerAlgIDSimple::Help...done"<<endl;
    return;
}


//__________________________________________________________________________________________________

void EdbShowerAlgIDSimple::SetSpecificationType(Int_t SpecificationType, Int_t SpecificationTypeVal)
{

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple:: Change Specification (" << SpecificationType << ") from " << GetSpecType(SpecificationType) << " -> " << SpecificationTypeVal << " . Reprint the changed Specification String: " << endl;

    eSpecificationType[SpecificationType]=SpecificationTypeVal;
    eSpecificationTypeString[SpecificationType] = eSpecificationTypeStringArray[SpecificationType][SpecificationTypeVal];

    if (gEDBDEBUGLEVEL >1) cout << eSpecificationTypeString[SpecificationType]  << endl;

    eSpecificationIsChanged=kTRUE;
    eForceSpecificationReload=kTRUE;

    cout << "EdbShowerAlgIDSimple::SetSpecificationType eSpecificationIsChanged: " << eSpecificationIsChanged << endl;
    cout << "EdbShowerAlgIDSimple::SetSpecificationType eForceSpecificationReload: " << eForceSpecificationReload << endl;

    cout << "Reprint Specifications: " << endl;
    if (gEDBDEBUGLEVEL >1) PrintSpecifications();

    Update();
    return;
}

//__________________________________________________________________________________________________

void EdbShowerAlgIDSimple::Update() {
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  Does the following things in the order:" << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  * According to the switch: set the right ANN of the Array as generic one." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  * According to the switch: load the right weightfile as generic one." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  * According to the switch: set the right correction parameters...." << endl;

    TString basicstring = TString(gSystem->ExpandPathName("$FEDRA_ROOT"));
    TString addstring = ("/src/libShower/weights/ID/");

    // (*) SpecType 0 :
    if (eSpecificationType[0]==1) {
        addstring+="volumeSpec_CP/";
        eSpecificationTypeString[0]="CP";
    }
    else {
        addstring+="volumeSpec_LT/";
        eSpecificationTypeString[0]="LT";
        cout << "EdbShowerAlgIDSimple::Update  WARNING::eSpecificationTypeString[0]=LT NOT YET SUPPORTED!" << endl;
    }

    // (*) SpecType 1 :
    // SHID_vs_SPID
    if (eSpecificationType[1]==2) {
        addstring+="separationSpec_pion_vs_gamma/";
        eSpecificationTypeString[1]="pion_vs_gamma";
    }
    else if (eSpecificationType[1]==1)  {
        addstring+="separationSpec_electron_vs_pion/";
        eSpecificationTypeString[1]="electron_vs_pion";
    }
    else {
        addstring+="separationSpec_gamma_vs_electron/";
        eSpecificationTypeString[1]="gamma_vs_electron";
    }

    // (*) SpecType 2 :
    if (eSpecificationType[2]==4) {
        addstring+="efficiencySpec_MiddleVar/";
        eSpecificationTypeString[2]="MiddleVar";
    }
    if (eSpecificationType[2]==3) {
        addstring+="efficiencySpec_LowEff/";
        eSpecificationTypeString[2]="LowEff";
    }
    else if (eSpecificationType[2]==2)  {
        addstring+="efficiencySpec_MiddleFix/";
        eSpecificationTypeString[2]="MiddleFix";
    }
    else if (eSpecificationType[2]==1)  {
        addstring+="efficiencySpec_All/";
        eSpecificationTypeString[2]="All";
    }
    else {
        addstring+="efficiencySpec_Neuch/";
        eSpecificationTypeString[2]="Neuch";
    }

    // (*) SpecType 3 :
    // Which addiditonal variables to use....
// 	0: showershape only
// 	1: and vtx IP
// 	2: and vtx delta Z
// 	3: shoowershape only, No first plate
// 	4: shoowershape only, No first & second plate
    if (eSpecificationType[3]==4) {
        addstring+="showerInfoSpec_SHAPEWOPL12/";
        eSpecificationTypeString[3]="SHAPEWOPL12";
    }
    else if (eSpecificationType[3]==3) {
        addstring+="showerInfoSpec_SHAPEWOPL1/";
        eSpecificationTypeString[3]="SHAPEWOPL1";
    }
    else if (eSpecificationType[3]==2) {
        addstring+="showerInfoSpec_ANDVTXDELTAZ/";
        eSpecificationTypeString[3]="ANDVTXDELTAZ";
    }
    else if (eSpecificationType[3]==1) {
        addstring+="showerInfoSpec_ANDVTXIP/";
        eSpecificationTypeString[3]="ANDVTXIP";
    }
    else {
        addstring+="showerInfoSpec_SHAPEONLY/";
        eSpecificationTypeString[3]="SHAPEONLY";
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  " << addstring << endl;

    if (NULL==ANN_MLP_ARRAY[0]) {
        CreateANN();
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  Now load the different ANN weightstrings:" << endl;
    // This was the loading part for the ANN and
    // this is the loading part for the Correction Factors, which we take from the
    // generic file EnergyCorrections_Npl_%d.txt
    // For the ID class we do NOT need the correction factors, so we skip these files anyway....
    for (int ll=0; ll<15; ll++) {
        TString mkdirstring=basicstring+addstring;
        TString weigthstring=basicstring+addstring+TString(Form("weights_Npl_%d.txt",ANN_nPlates_ARRAY[ll]));
        if (gEDBDEBUGLEVEL >1) cout << "weigthstring = " << weigthstring << endl;
        ANN_MLP_ARRAY[ll]->LoadWeights(weigthstring);
        ANN_WeightFile_ARRAY[ll]=weigthstring;
        Float_t p0,p1;
        p0=0.0;
        p1=1.0;
        eANN_MLP_CORR_0[ll]=p0;
        eANN_MLP_CORR_1[ll]=p1;
    }
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  * According to the switch: set the right ANN of the Array as generic one." << endl;

    // (*) SpecType 4:
    // Get NplIndexNr:
    int check_Npl_index=0;
    GetNplIndexNr(ePlateNumber,check_Npl_index, eSpecificationType[4]);

    if (check_Npl_index==0) eSpecificationTypeString[4]="Next";
    if (check_Npl_index==1) eSpecificationTypeString[4]="Before";
    eSpecificationTypeString[5]=TString(Form("%d",ePlateNumber));

    // (*) SpecType 5:
    // HigEfficiency Cut, HighPurityCut, MinDistCut
    if (eSpecificationType[6]==1) {
        addstring+="cutType/";
        eSpecificationTypeString[6]="cutType_HighEff";
    }
    else if (eSpecificationType[6]==2) {
        addstring+="cutType/";
        eSpecificationTypeString[6]="cutType_HighPur";
    }
    else {
        addstring+="cutType/";
        eSpecificationTypeString[6]="cutType_minDist";
    }

    if (gEDBDEBUGLEVEL >2)  cout << "EdbShowerAlgIDSimple:: The path for the ID weightfile is. " << endl;
    if (gEDBDEBUGLEVEL >2)  cout << addstring << endl;


    // Set Generic ANN_Layout
    ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];


    // Re-Read ID lookup tables, because of the possibility to
    // change the cutType threshold.
    ReadTables_ID();

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::Update  WARNING:: Weightfiles for _LT_  not produced yet!...." << endl;
    cout << "EdbShowerAlgIDSimple::Update...done." << endl;
    return;
}


//______________________________________________________________________________

void EdbShowerAlgIDSimple::Init()
{
    // This Plate Binnning is the _final_binning and will not be refined furthermore!
    ePlateBinning[0]=10;
    ePlateBinning[1]=12;
    ePlateBinning[2]=14;
    ePlateBinning[3]=16;
    ePlateBinning[4]=18;
    ePlateBinning[5]=20;
    ePlateBinning[6]=23;
    ePlateBinning[7]=26;
    ePlateBinning[8]=29;
    ePlateBinning[9]=32;
    ePlateBinning[10]=35;
    ePlateBinning[11]=40;
    ePlateBinning[12]=45;
    ePlateBinning[13]=45;
    ePlateBinning[14]=45; // 12,13,14 are the same, cause I have seen that it makes no sense to produce these,
    // since E,and sigmaE for 45 plates doesnt change anymore at all...
    // and how often !can! we scan more than 45 plates for a shower?

    ePlateNumber=20; // 20 plates by default...
    eSpecificationType[0] = 1;  // cp files by default...
    eSpecificationType[1] = 0;  // gamma_vs_electron by default...
    eSpecificationType[2] = 0;  // neuch eff by default...
    eSpecificationType[3] = 0;  // SHAPEONLY by default...
    eSpecificationType[4] = 0;  // next before weight by default...
    eSpecificationType[5] = 5;  // 20 plates by default...
    eSpecificationType[6] = 0;  // cutType_minDist by default...

    eSpecificationTypeString[0]="CP";
    eSpecificationTypeString[1]="gamma_vs_electron";
    eSpecificationTypeString[2]="Neuch";
    eSpecificationTypeString[3]="SHAPEONLY";
    eSpecificationTypeString[4]="Next";
    eSpecificationTypeString[5]="20";
    eSpecificationTypeString[6]="cutType_minDist";

    eForceSpecificationReload=kFALSE;

    for (int i=0; i<15; i++) {
        ANN_nPlates_ARRAY[i]=ePlateBinning[i];
    }

    EffFunc_all = new TF1("all","1.0-0.00000001*x",0,0.95);
    EffFunc_edefault = new TF1("default","0.94-0.216*x-0.767*x*x+1.865*x*x*x",0,0.95);
    EffFunc_elletroni = new TF1("elletroni","0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x",0,0.95);
    EffFunc_neuchmicro = new TF1("neuchmicro","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95);
    EffFunc_MiddleFix = new TF1("MiddleFix","0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    EffFunc_LowEff = new TF1("LowEff","0.85*0.5*(0.888361-1.299*x+1.49198*x*x+1.64668*x*x*x-2.63758*x*x*x*x+0.79+0.38*x-7.63*x*x+25.13*x*x*x-24.6*x*x*x*x)",0,0.95);
    EffFunc_UserEfficiency = new TF1("EffFunc_UserEfficiency","0.94-0.955*x+1.80*x*x-0.95*x*x*x",0,0.95); //new TSpline3();


    // Standard supposed efficiency of the showers/tracks that are to be evaluated:
    // The correct EffFunc_* is then choosen by taking the closes eff func to that one.
    // Initial parameters are the efficiency parameters for the "Neuch" parametrisation
    Double_t xarr[7]= {0,0.1,0.2,0.3,0.4,0.5,0.6};
    Double_t yarr[7]= {0.95,0.9,0.80,0.75,0.6,0.5,0.4};
    if (NULL!=eEfficiencyParametrisation) delete eEfficiencyParametrisation;
    eEfficiencyParametrisation = new TSpline3("",xarr,yarr,5,0,0,0.65);

    eHisto_nbtk_av          = new TH1D("eHisto_nbtk_av","Average basetrack numbers",21,0.0,10.0);
    eHisto_longprofile_av   = new TH1D("eHisto_longprofile_av","Basetracks per emulsion number",57,0.0,57.0);
    eHisto_transprofile_av  = new TH1D("eHisto_transprofile_av","Basetracks in trans distance",8,0.0,800.0);
    eHisto_deltaR_mean      = new TH1D("eHisto_deltaR_mean","Mean #deltar of all BTs in one shower",100,0.0,100.0);
    eHisto_deltaT_mean      = new TH1D("eHisto_deltaT_mean","Mean #delta#theta of all BTs in one shower",100,0.0,0.1);
    eHisto_deltaR_rms       = new TH1D("eHisto_deltaR_rms","RMS #deltar of all BTs in one shower",100,0.0,100.0);
    eHisto_deltaT_rms       = new TH1D("eHisto_deltaT_rms","RMS #delta#theta of all BTs in one shower",100,0.0,0.1);
    eHisto_nbtk             = new TH1D("eHisto_nbtk","Basetracks in the shower",50,0.0,100.0);
    eHisto_longprofile      = new TH1D("eHisto_longprofile","Basetracks per emulsion number",57,0.0,57.0);
    eHisto_deltaR           = new TH1D("eHisto_deltaR","Single #deltar of all BTs in Shower",100,0.0,150.0);
    eHisto_deltaT           = new TH1D("eHisto_deltaT","Single #delta#theta of all BTs in Shower",150,0.0,0.15);
    eHisto_transprofile     = new TH1D("eHisto_transprofile","Basetracks in trans distance",8,0.0,800.0);

    // CreateANN
    CreateANN();

    //First Time, we also Update()
    Update();

    // Set Strings:
    InitStrings();

    // Create EnergyArray
    eEnergyArray=new TArrayF(99999); // no way to adapt tarrayF on the fly...
    eEnergyArrayUnCorrected=new TArrayF(99999);
    eEnergyArraySigmaCorrected=new TArrayF(99999);

    // Read ID Lookup tables:
    ReadTables_ID();

    cout << "EdbShowerAlgIDSimple::Init()...done."<< endl;
    return;
}

//______________________________________________________________________________


void EdbShowerAlgIDSimple::InitStrings()
{
    eSpecificationTypeStringArray[0][0]="LT";
    eSpecificationTypeStringArray[0][1]="CP";

    eSpecificationTypeStringArray[1][0]="gamma_vs_electron";
    eSpecificationTypeStringArray[1][1]="electron_vs_pion";
    eSpecificationTypeStringArray[1][2]="pion_vs_gamma";

    eSpecificationTypeStringArray[2][0]="Neuch";
    eSpecificationTypeStringArray[2][1]="All";
    eSpecificationTypeStringArray[2][2]="MiddleFix";
    eSpecificationTypeStringArray[2][3]="Low";

    eSpecificationTypeStringArray[3][0]="SHAPEONLY";
    eSpecificationTypeStringArray[3][1]="ANDVTXIP";
    eSpecificationTypeStringArray[3][2]="ANDVTXDELTAZ";
    eSpecificationTypeStringArray[3][3]="SHAPEWOPL1";
    eSpecificationTypeStringArray[3][4]="SHAPEWOPL12";

    eSpecificationTypeStringArray[4][0]="Next";
    eSpecificationTypeStringArray[4][1]="Before";

    eSpecificationTypeStringArray[5][0]="10";

    eSpecificationTypeStringArray[6][0]="cutType_minDist";
    eSpecificationTypeStringArray[6][1]="cutType_HighEff";
    eSpecificationTypeStringArray[6][2]="cutType_HighPur";

    cout << "EdbShowerAlgIDSimple::InitStrings()...done."<< endl;
    return;
}

//______________________________________________________________________________

void EdbShowerAlgIDSimple::CreateANN()
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::CreateANN()"<<endl;

    // Compared to EdbShowerAlgESimple this ANN has two BIG differences:
    // a) It 2 InputNeurons more (vtxIP, vtxDeltaZ)
    // b) :inANN[0]/1000 is changed to :inANN[0]  (int instead of float variable)


    // Create ANN Tree and MLP:
    eParaName=2; // Standard Labeling taken over from EdbShowerP
    ANNTree = new TTree("ANNTree", "ANNTree");
    ANNTree->SetDirectory(0);
    ANNTree->Branch("inANN", inANN, "inANN[70]/D");

    // 	Plate Binning: 10,12,14,16,18,20,23,26,29,32,35,40,45,45
    // see ePlateBinning[]
    for (int k=0; k<15; k++) {
// 		for (int k=0; k<1; k++) {
        cout << "EdbShowerAlgIDSimple::CreateANN() Number k= " << k << endl;
        ANN_Layout="";
        ANN_n_InputNeurons_ARRAY[k]=6+ePlateBinning[k]+2; // Two extra for ID (vtxIP, vtxDeltaZ)
        ANN_n_InputNeurons=ANN_n_InputNeurons_ARRAY[k];
        cout << "EdbShowerAlgIDSimple::CreateANN() Number ANN_n_InputNeurons= " << ANN_n_InputNeurons << endl;

        for (Int_t i=1; i<ANN_n_InputNeurons; ++i) ANN_Layout += "@inANN["+TString(Form("%d",i))+"],";
        ANN_Layout += "@inANN["+TString(Form("%d",ANN_n_InputNeurons))+"]:"+TString(Form("%d",ANN_n_InputNeurons+1))+":"+TString(Form("%d",ANN_n_InputNeurons));
        ANN_Layout+=":inANN[0]";
        //---------------------------
        ANN_MLP_ARRAY[k]	= 	new TMultiLayerPerceptron(ANN_Layout,ANNTree,"","");
        ANN_MLP=ANN_MLP_ARRAY[k];
        ANN_Layout=ANN_MLP_ARRAY[k]->GetStructure();
        //---------------------------
        if (gEDBDEBUGLEVEL>2) {
            ANN_MLP->Print();
            cout << ANN_Layout << endl;
        }
    }
    //---------------------------

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::CreateANN()...done."<<endl;
    return;
}





//____________________________________________________________________________________________________

void EdbShowerAlgIDSimple::DoRun(EdbTrackP* shower)
{

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerAlgIDSimple::DoRun(EdbTrackP* shower)" << endl;

    // Get internal variables from the parametrisation filled:
    GetPara(shower);

    /*
         int EffNr=FindClosestEfficiencyParametrization(eParaShowerAxisAngle,eEfficiencyParametrisation->Eval(eParaShowerAxisAngle));

        // Check If Efficiency is the one we have or if we have to change/reload
        // the ANN weightfiles:
         if (GetSpecType(2)!=EffNr&&eForceSpecificationReload==kFALSE) {
            cout << "EdbShowerAlgIDSimple::DoRun()   INFO! Calulated Efficiency is more compatible with another one: Change Specifiaction! Call SetSpecificationType(2,"<< EffNr <<")." << endl;
            SetSpecificationType(2,EffNr);
        }
        if (GetSpecType(2)!=EffNr&&eForceSpecificationReload==kTRUE) {
            cout << "EdbShowerAlgIDSimple::DoRun()   INFO! Calulated Efficiency would be more compatible with another one: But eForceSpecificationReload is set to kFALSE. So we keep current Efficiency used." << endl;
        }
    */



    // This is to select the suited ANN to the shower, i.e. the one that matches closest
    // the number of plates:
    int check_Npl_index	=0;
    GetNplIndexNr(shower->Npl(),check_Npl_index,ePlateNumberType);


    ANN_n_InputNeurons=ANN_n_InputNeurons_ARRAY[check_Npl_index];
    ANN_MLP=ANN_MLP_ARRAY[check_Npl_index];
    eWeightFileString=ANN_WeightFile_ARRAY[check_Npl_index];
    eCalibrationOffset = eANN_MLP_CORR_0[check_Npl_index];
    eCalibrationSlope = eANN_MLP_CORR_1[check_Npl_index];

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   ANN_n_InputNeurons_ARRAY[check_Npl_index]="<< ANN_n_InputNeurons_ARRAY[check_Npl_index] << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   ANN_MLP_ARRAY[check_Npl_index]="<< ANN_MLP_ARRAY[check_Npl_index] << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   Using the following layout: " << endl;
    if (gEDBDEBUGLEVEL >2) cout << ANN_MLP->GetStructure() << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   And the following weightfile: " << endl;
    if (gEDBDEBUGLEVEL >2) cout << eWeightFileString << endl;

    //ANN_MLP->LoadWeights(eWeightFileString);

    //  Reset InputVariables:
    for (int k=0; k<70; k++) {
        inANN[k]=0;
    }


    // inANN[0] is ALWAYS Reseverd for the quantity value to be estimated
    // (E,Id,...)
    // Test with private variables:
    inANN[1]=eParaShowerAxisAngle;
    inANN[2]=eParanseg;
    inANN[3]=eParaBT_deltaR_mean;
    inANN[4]=eParaBT_deltaR_rms;
    inANN[5]=eParaBT_deltaT_mean;
    inANN[6]=eParaBT_deltaT_rms;
    for (int ii=0; ii<57; ii++) {
        inANN[7+ii]= eParalongprofile[ii];
    }
    /// Last two variables are reserved for vtxIP and vtxDeltaZ values!!

    /// ALSO, IF first (two) plate information shall be used...
    /// DONE IN THE WEIGHTFILE (weight of pl1&2 is then zero),
    /// so dont have to do it here...


    // Fill Tree:
    ANNTree->Fill();
    ANNTree->Show(0);

    Double_t params[70];
    Double_t val;


    cout << "EdbShowerAlgIDSimple::DoRun()   ------- TODO HERE:  INTERFACE WITH VERTEX TO CALCULATE VTXIP AND VTXDELTAZ TO SHOWER ----------"<<endl;
    params[ANN_n_InputNeurons-3]=-1;
    params[ANN_n_InputNeurons-2]=-1;
    cout << "EdbShowerAlgIDSimple::DoRun()   ------- TODO HERE:  INTERFACE WITH VERTEX TO CALCULATE VTXIP AND VTXDELTAZ TO SHOWER ----------"<<endl;



    for (Int_t j=1; j<=ANN_n_InputNeurons; ++j) {
        params[j-1]=inANN[j];
    }

    for (Int_t j=1; j<=ANN_n_InputNeurons; ++j) cout << "EdbShowerAlgIDSimple::DoRun()   :  j  params[j-1]=inANN[j]   " << j<< "  " << params[j-1] << endl;


    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   Print Inputvariables...done." << endl;
    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   Evaluate Neural Network Output:" << endl;

    // ---------------------------------
    // Evaluation of the ANN:
    val=(ANN_MLP->Evaluate(0,params));
    // ---------------------------------

    if ( isnan(val) ) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   ERROR! ANN_MLP->Evaluate(0,params) is NAN! Setting value to -1. " << endl;
        val=-1;
    }

    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   After  Correction: ANN_MLP->Evaluate(0,params)= " <<  val << " (Inputvar=" << inANN[0]  << ")." << endl;


    // Determining the type: if its SHID like or if its SPID like...
    Int_t type=DetermineIDType(val,eSpecificationType[6]);
    DetermineIDFlag(type);

// 		if (val>eANNIDCutValue) cout << "val>eANNIDCutValue" << endl;
//
// 		cout << "UUUUUUUUUUUUUUUUUU   val= " <<  val << endl;
//
// 		shower->SetP(val);
//
//     cout << "EdbShowerAlgIDSimple::DoRun() Finally, set values..." <<  endl;
// 		cout << "EdbShowerAlgIDSimple::DoRun() /// VERY PRELIMINARY...." <<  endl;
// 		cout << "EdbShowerAlgIDSimple::DoRun() Finally, set values..." <<  endl;


    if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   ...Done." << endl;
    return;
}

//______________________________________________________________________________________________________________

Int_t EdbShowerAlgIDSimple::DetermineIDType(Double_t val, Int_t SpecificationType)
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::DetermineIDType()" << endl;

    // Depending on cutthresholdvalue and -type we assign the ANN output into
    // 0 (i.e. SHID) or 1 (i.e. SPID):
    if (val>eIDCutTypeValue[SpecificationType]) return 1;
    return 1;
}

//______________________________________________________________________________________________________________

Int_t EdbShowerAlgIDSimple::DetermineIDFlag(Int_t IDType)
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::DetermineIDFlag()" << endl;

    // Depending on the SHID<->SPID type we assign the ID output into
    // 22, -11, -211 (PdgIds of Gamma,Electron,Pion)

    Int_t Flag=0;
    if (eSpecificationType[1]==2) {
        if (IDType==0) Flag=-211;
        if (IDType==1) Flag=22;
    }
    else if (eSpecificationType[1]==1) {
        if (IDType==0) Flag=-11;
        if (IDType==1) Flag=-211;
    }
    else {
        if (IDType==0) Flag=22;
        if (IDType==1) Flag=-11;
    }
    return Flag;
}

//____________________________________________________________________________________________________

void EdbShowerAlgIDSimple::DoRun()
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::DoRun()" << endl;

    for (int i=0; i<eRecoShowerArrayN; i++) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   Doing i= " << i << endl;
        continue;
        eEnergyArrayCount=i;

        //EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        EdbTrackP* shower=(EdbTrackP*) eRecoShowerArray->At(i);

        DoRun(shower);

    } // (int i=0; i<eRecoShowerArrayN; i++)

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::DoRun()...done." << endl;
    return;
}


//____________________________________________________________________________________________________

void EdbShowerAlgIDSimple::DoRun(TObjArray* trackarray)
{
    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::DoRun(TObjArray* trackarray)" << endl;

    eRecoShowerArrayN=trackarray->GetEntries();
    eRecoShowerArray=trackarray;

    for (int i=0; i<eRecoShowerArrayN; i++) {
        if (gEDBDEBUGLEVEL >2) cout << "EdbShowerAlgIDSimple::DoRun()   Doing i= " << i << endl;

        eEnergyArrayCount=i;

        //EdbShowerP* shower=(EdbShowerP*) eRecoShowerArray->At(i);
        EdbTrackP* shower=(EdbTrackP*) eRecoShowerArray->At(i);

        DoRun(shower);

    } // (int i=0; i<eRecoShowerArrayN; i++)

    if (gEDBDEBUGLEVEL >1) cout << "EdbShowerAlgIDSimple::DoRun(TObjArray* trackarray)...done." << endl;
    return;
}



//__________________________________________________________________________________________________

void EdbShowerAlgIDSimple::PrintSpecifications() {
    cout << "EdbShowerAlgIDSimple::PrintSpecifications" << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[0]  (Dataset:    (linked tracks/full cp) LT/CP)   = " << GetSpecType(0) << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[1]  (ShowerID:   gamma_vs_electron/electron_vs_pion/pion_vs_gamma) = " << GetSpecType(1) << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[2]  (ScanEff:    Neuch/All/MiddleFix/LowEff) = " << GetSpecType(2) << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[3]  (E range:    0..20/0..40) = " << GetSpecType(3) << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[4]  (Npl weight: next before/after) = " <<GetSpecType(4) << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[5]  (Npl weight: 10,12,...,45) = " << GetSpecType(5) << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationType[6]  (CutType: MinDist/HighEff/HighPur) = " << GetSpecType(6) << endl;
    cout << "EdbShowerAlgIDSimple::   " << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[0]  (take CP or linked_tracks)   = " << eSpecificationTypeString[0] << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[1]  (gamma_vs_electron/electron_vs_pion/pion_vs_gamma) = " << eSpecificationTypeString[1] << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[2]  (ScanEff: Neuch/All/MiddleFix/LowEff) = " << eSpecificationTypeString[2] << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[3]  (E range: 0..20/0..40) = " << eSpecificationTypeString[3] << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[4]  (Npl weight: next before/after) = " << eSpecificationTypeString[4] << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[5]  (Npl weight: 10,12,...,45) = " << eSpecificationTypeString[5] << endl;
    cout << "EdbShowerAlgIDSimple::   eSpecificationTypeString[6]  (CutType: MinDist/HighEff/HighPur) = " << eSpecificationTypeString[6] << endl;
    cout << "EdbShowerAlgIDSimple::   " << endl;
    cout << "EdbShowerAlgIDSimple::   In case you want to change a specification then do for example:" << endl;
    cout << "EdbShowerAlgIDSimple::   EdbShowerAlgIDSimple->SetSpecification(2,3) " << endl;
    cout << "EdbShowerAlgIDSimple::   It will change the ScanEff from the actual value to _Low_." << endl;
    cout << "EdbShowerAlgIDSimple::" << endl;
    cout << "EdbShowerAlgIDSimple::PrintSpecifications...done." << endl;
    return;
}