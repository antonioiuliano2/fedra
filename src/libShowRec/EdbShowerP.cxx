#include "EdbShowerP.h"
#include "EdbMath.h"
using namespace std;
using namespace TMath;


//______________________________________________________________________________


EdbShowerP::EdbShowerP(int nseg)
{
    if (gEDBDEBUGLEVEL>4) cout << "EdbShowerP::EdbShowerP(int nseg)   CONSTRUCTOR" << endl;


    // default constructor, empty shower
    Init();
    SetNpl(-999);
    SetN0(-999);
    Set0();

    // Create ObjArray for the Segments:
    if (nseg>0) eS = new TObjArray(9999);

    //cout << "CONSTRUCTOR: eProfileLongitudinal " << eProfileLongitudinal << endl;

    if (!eProfileLongitudinal) {
        eProfileLongitudinal = new TH1F("ProfileLongitudinal","ProfileLongitudinal",56,0,56.0*1300.0);
        if (gEDBDEBUGLEVEL>3) cout << "CONSTRUCTOR: eProfileLongitudinal created..." << eProfileLongitudinal << endl;
        //cout << eProfileLongitudinal->GetBinWidth(1) << " =    eProfileLongitudinal->GetBinWidth(1) " << endl;
        //cout << eProfileLongitudinal->GetBinCenter(1) << " =    eProfileLongitudinal->GetBinCenter(1) " << endl;
    }
    if (!eProfileTransversal) {
        eProfileTransversal = new TH1F("ProfileTransversal","ProfileTransversal",8,0,800);
        if (gEDBDEBUGLEVEL>3) cout << "CONSTRUCTOR: eProfileTransversal created..." << eProfileTransversal << endl;
    }

    eLongitudinalProfileName="ProfileLongitudinal";
    eTransversalProfileName="ProfileTransversal";
}


//______________________________________________________________________________


EdbShowerP::EdbShowerP(int number1, int number2, int number3, int nseg)
{
    if (gEDBDEBUGLEVEL>4) {
        cout << "EdbShowerP::EdbShowerP(int number1, int number2, int number3, int nseg)   CONSTRUCTOR" << endl;
        cout << "EdbShowerP::EdbShowerP   number 1 = " << number1 << endl;
        cout << "EdbShowerP::EdbShowerP   number 2 = " << number2 << endl;
        cout << "EdbShowerP::EdbShowerP   number 3 = " << number3 << endl;
    }

    // default constructor, empty shower
    Init();
    SetNpl(-999);
    SetN0(-999);
    Set0();

    // Create ObjArray for the Segments:
    if (nseg>0) eS = new TObjArray(9999);

    // To make each histogram unique... Strange workaround....
    // But otherwise the single histograms will be overwritten and root will give warnings...
    // But be careful! In this mode you will get rid of the warnings, but you will
    // __NOT__ be able to access the histograms by their names anymore..:

    char hname[40];
    sprintf(hname,"ProfileLongitudinal_%d_%d_%d",number1,number2,number3);
    eLongitudinalProfileName=TString(hname);
    if (!eProfileLongitudinal) eProfileLongitudinal = new TH1F(hname,"ProfileLongitudinal",56,0,56.0*1300.0);
    sprintf(hname,"ProfileTransversal_%d_%d_%d",number1,number2,number3);
    eTransversalProfileName=TString(hname);
    if (!eProfileTransversal) eProfileTransversal = new TH1F(hname,"ProfileTransversal",8,0,800);

}

//______________________________________________________________________________

EdbShowerP::~EdbShowerP()
{
    // default destructor:
    if (gEDBDEBUGLEVEL>4)  cout<<"EdbShowerP::~EdbShowerP()   DESTRUCTOR"<<endl;
    if (eS)                       {
        eS->Clear();
        delete eS;
        eS=0;
    }
    if (eProfileLongitudinal)     {
        delete eProfileLongitudinal;
        eProfileLongitudinal=0;
    }
    if (eProfileTransversal)      {
        delete eProfileTransversal;
        eProfileTransversal=0;
    }
    if (eShowerAxisCenterGravityBT) {
        delete eShowerAxisCenterGravityBT;
        eShowerAxisCenterGravityBT=0;
    }

    delete eReco_ID_Array;
    delete eReco_E_Array;
    delete eReco_Vtx_Array;

    if (gEDBDEBUGLEVEL>4)  cout<<"EdbShowerP::~EdbShowerP()   DESTRUCTOR...done."<<endl;
}


//______________________________________________________________________________

void EdbShowerP::Init()
{
    // Default Init function:
    // all Objects persistently there, created with a "new" operator,
    // i.e. objects which are created only once..
    eReco_ID_Array = new TObjArray; // create crash when not initialized???
    eReco_E_Array = new TObjArray;
    eReco_Vtx_Array = new TObjArray;

}

//______________________________________________________________________________

void EdbShowerP::Set0()
{
    // Default Reseting function:
    ((EdbSegP*)this)->Set0();
    eS=0;
    eVTAS = 0;
    eNpl=0;
    eN0=0;
    eN00=0;
    //----- start initializing as a photon:
    eM=0;
    ePDG=22;
    //-----
    eAlgName="UNSPECIFIED";
    eAlgValue=-999;
    for (int i=0; i<10; ++i) {
        eParaValue[i]=-99999.0;
        eParaString[i]="UNSPECIFIED";
    }
    //-----
    eMC=0;
    for (int i=0; i<57; i++) {
        eNBTPlate[i]=0;
        eNBTMCPlate[i]=0;
    }
    eNBTMC=0;
    eNBT=0;
    ePurity=0;
    //-----
    eId=0;
    ePDGId=0;
    eEnergy=0;
    eEnergyUncorrected=0;
    eEnergyCorrected=0;
    eEnergyMC=0;
    //-----
    eShowerAxisCenterGravityBT=0;
    eShowerAxisAngle=0;
    eProfileLongitudinal=0;
    eProfileTransversal=0;
    eSphericity=0;
    eShowerAngularDeviationTXDistribution_mean = 0;
    eShowerAngularDeviationTXDistribution_sigma = 0;
    eShowerAngularDeviationTYDistribution_mean = 0;
    eShowerAngularDeviationTYDistribution_sigma = 0;
    //-----
    for (int i=0; i<10; i++) eParametrisationIsDone[i]=kFALSE;
    for (int i=0; i<2; i++) {
        eMCInfoIsDone[i]=kFALSE;
        eExtraInfoIsDone[i]=kFALSE;
    }
    return;
}


//______________________________________________________________________________

void EdbShowerP::ClearAll()
{
    cout<<"EdbShowerP::ClearAll()   "<<endl;
    Clear();
    ClearProfileTransversalHisto();
    ClearProfileLongitudinalHisto();
    Set0();
    cout<<"EdbShowerP::ClearAll()   done."<<endl;
}
//     inline void  Clear()                     { if(eS)  eS->Clear(); }
//     inline void  ClearProfileLongitudinalHisto() { if(eProfileLongitudinal)  eProfileLongitudinal->Reset(); }
//     inline void  ClearProfileTransversalHisto() { if(eProfileTransversal)  eProfileTransversal->Reset(); }


//______________________________________________________________________________

void  EdbShowerP::AddSegment(EdbSegP *s)
{
    // Preliminary check if array is already existing:
    if (!eS) eS = new TObjArray(9999);

    // Set XY... for the first (only!) BaseTrack in the shower.
    if (eNBT==0) {
        this->SetX(s->X());
        this->SetY(s->Y());
        this->SetZ(s->Z());
        this->SetTX(s->TX());
        this->SetTY(s->TY());
        this->SetPID(s->PID());
        eEnergyMC=s->P();
        eMC=s->MCEvt();
        SetMC(s->MCEvt(),s->MCEvt());
    }

    // Add BaseTrack (segment) to shower
    eS->Add(s);

    // Increase number of BaseTracks in shower
    ++eNBT;
    // Increase number of MC-BaseTracks in shower only in case of same(!) MCEvent number
    if  (s->MCEvt()>=0 && s->MCEvt()==eMC) ++eNBTMC;

    return;
}

//______________________________________________________________________________


void EdbShowerP::AddSegmentAndUpdate(EdbSegP *s)
{
    AddSegment(s);
    Update();
    return;
}


//______________________________________________________________________________

void EdbShowerP::RemoveSegment(EdbSegP *s, Bool_t UpdateAll)
{
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::RemoveSegment()" << endl;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::RemoveSegment()  Object s* to  be removed " << s << endl;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::RemoveSegment()  shower->N() " <<  N() << endl;
    if (!eS) return;
    eS->Remove(s);
    eS->Compress();
    eNBT=N();
    // It is ABSOLUTELY Necessary to run Compress() on the object array. Otherwise the
    // removed object will stay as NullPointer. And then any operation on this
    // will lead to a crash! See:
    // http://root.cern.ch/phpBB3/viewtopic.php?f=3&t=10371&p=44643#p44643
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::RemoveSegment()  Removal done. Adapt eNBT:" << endl;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::RemoveSegment()  shower->N() " <<   N() << endl;
    if (UpdateAll) Update();
    return;
}

//______________________________________________________________________________

void EdbShowerP::Sort()
{
    // Sort Shower Segments in Ascending Z position:
    // Makes use of the TSortedList.
    // EdbTrackP the segment array is already in TSortedList.
    // In EdbShowerP its used as a TObjArray thats why we have to make this
    // turnaround here....
    TObjArray* array=new TObjArray();
    TSortedList* f = new TSortedList() ;
    for (int i=0; i< this->N(); i++) {
        EdbSegP* sh = this->GetSegment(i);
        f->Add(sh);
    }
    for (int i=0; i< f->GetEntries(); i++) {
        EdbSegP* sh = (EdbShowerP*)f->At(i);
        array->Add(sh);
    }
    this->SetSegmentArray(array);
    this->PrintSegments();
    return;
}

//______________________________________________________________________________

Bool_t EdbShowerP::IsSorted()
{
    // returns 1 if shower segments are all sorted in ascending Z direction.
    for (int i=0; i< this->N()-1; i++) {
        EdbSegP* seg = this->GetSegment(i);
        EdbSegP* segnext = this->GetSegment(i+1);
        if (segnext->Z()<seg->Z()) return 0;
    }
    return 1;
}


//______________________________________________________________________________

void EdbShowerP::BuildNMC()
{
    Log(3,"EdbShowerP::BuildNMC()","BuildNMC()...done.");
    // Increase number of MC-BaseTracks in shower only in case of same(!) MCEvent number
    EdbSegP* s=0;
    eNBTMC=0; // reset from beginning
    for (int i=0; i<N(); ++i) {
        s=GetSegment(i);
        if  (s->MCEvt()>=0 && s->MCEvt()==eMC) ++eNBTMC;
    }
    Log(3,"EdbShowerP::BuildNMC()","BuildNMC()...done.");
    return;
}

//______________________________________________________________________________

void EdbShowerP::BuildNplN0()
{
    Log(3,"EdbShowerP::BuildNplN0()","BuildNplN0()");
    EdbSegP*	First = this->GetSegmentFirst();
    EdbSegP*	Last = this->GetSegmentLast();

    // For eN0, eNBTPlate has to be filled by BuildProfiles()
    eNpl=TMath::Abs(First->PID()-Last->PID())+1;

    eN0=0;
    for (int i=0; i<eNpl; i++) if (eNBTPlate[i]==0) ++eN0;

    int eN00int=0;
    eN00=0;
    for (int i=0; i<eNpl; i++) {
        if (eNBTPlate[i]==0) {
            eN00++;
            if (eN00>eN00int) eN00int=eN00;
        }
        if (eNBTPlate[i]!=0) eN00=0;
    }
    eN00=eN00int;

    Log(3,"EdbShowerP::BuildNplN0()","BuildNplN0()...done.");
    return;
}

//______________________________________________________________________________

void EdbShowerP::BuildProfiles()
{
    Log(3,"EdbShowerP::BuildProfiles()","BuildProfiles()");

    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowerP::BuildProfiles()" << endl;
        cout << "EdbShowerP::Building Transversal Profile:" << endl;
        cout << "EdbShowerP::Loop over BTs and calculate their distance w.r.t to the InBT axis" << endl;
    }
    if (eNBT<=0) {
        cout << "EdbShowerP::BuildProfiles()   Empty shower. Return." << endl;
        return;
    }


    // Need to reset Profile Histograms, otherwise they are filled more than one time,
    // for example, if you call Update() more often than once:
    Log(4,"EdbShowerP::BuildProfiles()","ClearProfileHistograms()");
    ClearProfileLongitudinalHisto();
    ClearProfileTransversalHisto();

    // Generally we build the Profile Histograms w.r.t. the ShowerAxis !
    // Here (just locally) we choose now:
    EdbSegP*	TemporaryAxis;
    if (!eShowerAxisCenterGravityBT) {
        cout << "WARNING ----- NO  eShowerAxisCenterGravityBT  YET !!!!! " << endl;
        TemporaryAxis=this->GetSegmentFirst();
    }
    else {
        TemporaryAxis=eShowerAxisCenterGravityBT;
    }

    Float_t zorig=TemporaryAxis->Z();

    // eShowerAxisCenterGravityBT->PrintNice();
    // TemporaryAxis=this->GetSegmentFirst();
    // TemporaryAxis->PrintNice();

    // Check if first BT is unchanged:
    //cout << "check if first BT is unchanged:" << endl;
    //this->GetSegment(0)->PrintNice();

    Float_t TransDist=0;
    Float_t LongDist=0;
    Float_t TotalDist=0;
    Float_t Point[3];
    Float_t LineStart[3];
    Float_t LineEnd[3];
    //   cout << "--"<<endl;
    //  	EdbSegP*	InBT = this->GetSegmentFirst();
    EdbSegP*	InBT = TemporaryAxis;
    // 	InBT->PrintNice();
    //   cout << "--"<<endl;
    LineStart[0]=InBT->X();
    LineStart[1]=InBT->Y();
    LineStart[2]=InBT->Z();
    // 	EdbSegP*	InBTProp = this->GetSegmentFirst();
    EdbSegP*	InBTProp = TemporaryAxis;
    InBTProp->PropagateTo(zorig+10000.0);
    // 	InBTProp->PrintNice();
    //   cout << "--"<<endl;
    LineEnd[0]=InBTProp->X();
    LineEnd[1]=InBTProp->Y();
    LineEnd[2]=InBTProp->Z();
    InBTProp->PropagateTo(zorig);
    // Propagate back, since the address is the one of the stored Basetrack.
    // this is only a workaround to get the axis coordinate points...

    // For the InBT, the values xxxDist are taken into account as well:
    LongDist=0;
    TransDist=0;
    TotalDist=0;

    for (int i=0; i<eNBT; i++) {

        // Reset Interim calcultiion values - of course.
        LongDist=0;
        TransDist=0;
        TotalDist=0;

        EdbSegP*	TestBT = this->GetSegment(i);
        Point[0]=TestBT->X();
        Point[1]=TestBT->Y();
        Point[2]=TestBT->Z();
        TransDist=EdbMath::DistancePointLine3(Point, LineStart,LineEnd, true);
        TotalDist+=(Point[0]-LineStart[0])*(Point[0]-LineStart[0]);
        TotalDist+=(Point[1]-LineStart[1])*(Point[1]-LineStart[1]);
        TotalDist+=(Point[2]-LineStart[2])*(Point[2]-LineStart[2]);
        TotalDist=sqrt(TotalDist);
        LongDist=sqrt(TotalDist*TotalDist-TransDist*TransDist);

        if (gEDBDEBUGLEVEL>3) {
            cout << "i= "<< i<< " TotalDist (microns)= " << setw(14) << TotalDist << endl;
            cout << "i= "<< i<< " LongDist (microns)= " << setw(14) << LongDist << endl;
            cout << "i= "<< i<< " TransDist (microns)= " << setw(14) << TransDist << endl;
        }

        eProfileLongitudinal->Fill(LongDist);
        eProfileTransversal->Fill(TransDist);

        // DEBUG VERSION, Weighting Profile Histograms with the number of segments
        // eProfileLongitudinal->Fill(LongDist,weight);
        // eProfileTransversal->Fill(TransDist,weight);
    }

    //cout << "eProfileLongitudinal->Integral() =  " << eProfileLongitudinal->Integral()  << endl;
    //cout << "eProfileTransversal->Integral() =  " <<  eProfileTransversal->Integral() << endl;

    // "check if first BT is unchanged:
    //cout << "check if first BT is unchanged:" << endl;
    //this->GetSegment(0)->PrintNice();

    Log(3,"EdbShowerP::BuildProfiles()","BuildProfiles()...done.");
    return;
}

//______________________________________________________________________________

void EdbShowerP::BuildPlateProfile()
{
    Log(3,"EdbShowerP::BuildPlateProfile()","BuildPlateProfile()");
    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowerP::BuildPlateProfile()" << endl;
    }

    // ReInit Values:
    eN0=0;
    for (int i=0; i<57; i++) {
        eNBTPlate[i]=0;
        eNBTMCPlate[i]=0;
    }

    EdbSegP*	InBT = this->GetSegment(0);
    int nr;
    for (int i=0; i<eNBT; i++) {
        EdbSegP*	TestBT = this->GetSegment(i);
        nr=TMath::Abs( InBT->PID()-TestBT->PID() );
        eNBTPlate[nr]=eNBTPlate[nr]+1;
        if (TestBT->MCEvt()==InBT->MCEvt())  eNBTMCPlate[nr]=eNBTMCPlate[nr]+1;
    }

    // Now the necesseary plate arrays are filled then npl and n0 can be build:
    BuildNplN0();

    eLastZ  = GetSegmentLastZ();
    eFirstZ = GetSegmentFirstZ();

    Log(3,"EdbShowerP::BuildPlateProfile()","BuildPlateProfile()...done.");
    return;
}


//______________________________________________________________________________


void EdbShowerP::BuildShowerAxis()
{
    Log(3,"EdbShowerP::BuildShowerAxis()","BuildShowerAxis()");

    // Code taken from EdbMath::LFIT3 to fit line to a collection of points in spce!
    //int EdbMath::LFIT3( float *X, float *Y, float *Z, float *W, int L,
    //float &X0, float &Y0, float &Z0, float &TX, float &TY, float &EX, float &EY )
    // Linear fit in 3-d case (microtrack-like)
    // Input: X,Y,Z - coords, W -weight - arrays of the lengh >=L
    // Note that X,Y,Z modified by function
    // Output: X0,Y0,Z0 - center of gravity of segment
    // TX,TY : direction tangents in respect to Z axis

    Float_t X0,Y0,Z0;
    Float_t TX,TY;
    Float_t EX,EY;
    Float_t TX0,TY0;
    // Float_t x[eNBT];	Float_t z[eNBT];	Float_t y[eNBT];	Float_t W[eNBT];
    // Compiled with -pedantic -Wall -W -Wstrict-prototypes this gives error message: Fehler: ISO-C++ verbietet Feld »x« variabler Länge
    // So we take it as fixed length:
    Float_t x[5000];
    Float_t y[5000];
    Float_t tx[5000];
    Float_t ty[5000];
    Float_t z[5000];
    Float_t W[5000];
    Float_t sumTX=0;
    Float_t sumTY=0;
    Float_t sumW=0;

    // The weighting of the segments is a crucial point here, since it influences
    // the fitting to the shower axis!
    // We choose the weighting with distance to Initator BT. So that large scattered BTs far off
    // do not contribute too much.

    // Attention: If the shower was ordered in such a way that this->GetSegment(0) is NOT the lowest Z in
    // the shower, then the results will be totally crapped, since the fit starts then from the end!!!

    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::BuildShowerAxis()   eNBT="<< eNBT << endl;
    if (gEDBDEBUGLEVEL>3) PrintSegments();
    //Print();

    for (int i=0; i<eNBT; i++) {
        EdbSegP*	TestBT = this->GetSegment(i);
        //TestBT->PrintNice();
        x[i]=TestBT->X();
        y[i]=TestBT->Y();
        z[i]=TestBT->Z();
        tx[i]=TestBT->TX();
        ty[i]=TestBT->TY();

        W[i]=1.0;
        W[i]=1.0/ (TMath::Abs(z[i]-z[0])/1300.0*TMath::Abs(z[i]-z[0])/1300.0 +1.0);

        sumTX+=W[i]*TestBT->TX();
        sumTY+=W[i]*TestBT->TY();
        sumW+=W[i];
    }

    for (int i=eNBT; i<5000; i++) {
        W[i]=0;
        x[i]=0;
        y[i]=0;
        z[i]=0;
        tx[i]=0;
        ty[i]=0;
    }

    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::BuildShowerAxis()   Resting of initial values done."<< endl;

    // For the VERY special case of only two segments at same Z position,
    // like electron/positron pair, EdbMath::LFIT3 returns inf cause by zero division.
    // In this case we return the average of both segments:
    // This issue is fixed in new fedra version svn>???
    if (eNBT==2 && z[0]==z[1]) {
        if (gEDBDEBUGLEVEL>3)  cout << "Special case of only two segments at same Z position"<<endl;
        //     cout << "Print First:" << endl;
        //     GetSegment(0)->PrintNice();
        //     cout << "Print Second:" << endl;
        //     GetSegment(1)->PrintNice();
        X0=x[0]+(x[1]-x[0])/2.0;
        Y0=y[0]+(y[1]-y[0])/2.0;
        TX0=tx[0]+(tx[1]-tx[0])/2.0;
        TY0=ty[0]+(ty[1]-ty[0])/2.0;
        if (!eShowerAxisCenterGravityBT) eShowerAxisCenterGravityBT = new EdbSegP(-1,X0,Y0,TX0,TY0,0,0);
        eShowerAxisCenterGravityBT->SetZ(z[0]);
        eShowerAxisCenterGravityBT->SetP(GetSegment(0)->P());
        eShowerAxisCenterGravityBT->SetMC(GetSegment(0)->MCEvt(),GetSegment(0)->MCEvt());
        eShowerAxisCenterGravityBT->SetPID(GetSegment(0)->PID());
        eShowerAxisAngle=TMath::Sqrt(TX0*TX0+TY0*TY0);
        //eShowerAxisCenterGravityBT->PrintNice();
        return;
    }

    // Invoke fit function:
    EdbMath::LFIT3( x,y,z,W,eNBT,X0,Y0,Z0,TX,TY,EX,EY);
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::BuildShowerAxis()   Invoke fit function done"<< endl;

    // Normierung:
    sumTX=sumTX/sumW;
    sumTY=sumTY/sumW;

    // Set Z0 to first BT:
    Z0=this->GetSegment(0)->Z();

    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::BuildShowerAxis   Axis  X0,Y0,Z0, TX, TY Z0" <<  X0 << " " << Y0 << " " << Z0 << " " << TX << " " << TY << " " << Z0 << endl;

    if (!eShowerAxisCenterGravityBT) eShowerAxisCenterGravityBT = new EdbSegP(-1,X0,Y0,sumTX,sumTY,0,0);
    eShowerAxisCenterGravityBT->SetZ(Z0);
    eShowerAxisAngle=TMath::Sqrt(TX*TX+TY*TY);

    // In this case, we take ShowerAxisAngle and eShowerAxisCenterGravityBT to be as the first starting BT:
    if (isnan(eShowerAxisAngle) || isinf(eShowerAxisAngle) )  {
        if (gEDBDEBUGLEVEL>3) {
            if (isnan(eShowerAxisAngle)) cout << "eShowerAxisAngle==isnan() " << endl;
            if (isinf(eShowerAxisAngle)) cout << "eShowerAxisAngle==isinf() " << endl;
            cout << "In this case, we take ShowerAxisAngle and eShowerAxisCenterGravityBT to be as the first starting BT"<<endl;
            cout << " Shower->N(): " << N() << endl;
        }
        eShowerAxisCenterGravityBT=GetSegment(0);
        eShowerAxisAngle=TMath::Sqrt(eShowerAxisCenterGravityBT->TX()*eShowerAxisCenterGravityBT->TX()+eShowerAxisCenterGravityBT->TY()*eShowerAxisCenterGravityBT->TY());
        if (gEDBDEBUGLEVEL>3) eShowerAxisCenterGravityBT->PrintNice();
        //this->PrintSegments();
    }

    // Set Z GenterGravity Position to InitiabtorBT Z position (otherwise the center
    // Basetrack can be at Z in the middle of the shower)...
    // But have to propagate since it is not correctly put at XY
    // eShowerAxisCenterGravityBT -> SetZ(GetSegment(0)->Z());
    // eShowerAxisCenterGravityBT->PropagateTo(GetSegment(0)->Z());
    // eShowerAxisCenterGravityBT->SetPID(GetSegment(0)->PID());

    Log(3,"EdbShowerP::BuildShowerAxis()","BuildShowerAxis()...done.");
    return;
}


//______________________________________________________________________________


void EdbShowerP::CalcShowerAngularDeviationDistribution()
{
    Log(3,"EdbShowerP::CalcShowerAngularDeviationDistribution()","CalcShowerAngularDeviationDistribution()");

    // First check if shower axis has been built:
    if (!eShowerAxisCenterGravityBT) BuildShowerAxis();

    // Loop over all tracks of the shower, for each one calc TX, TY - TXY_gravity center
    Double_t deltaTX,deltaTY;
    TH1F histDeltaTX("histDeltaTX","histDeltaTX",100,-0.5,0.5);
    TH1F histDeltaTY("histDeltaTY","histDeltaTY",100,-0.5,0.5);

    // Loop over all tracks of the shower, for each one calc TX, TY - TXY_gravity center
    // If eShowerAxisCenterGravityBT is first BT in shower (because fit did not work, or
    // just two segments on same z) then the first one entry in deltaTXY is always zero
    for (Int_t i=0; i<eNBT; ++i) {
        deltaTX=GetSegment(i)->TX()-eShowerAxisCenterGravityBT->TX();
        deltaTY=GetSegment(i)->TY()-eShowerAxisCenterGravityBT->TY();
        histDeltaTX.Fill(deltaTX);
        histDeltaTY.Fill(deltaTY);
    }
    // Get mean and sigma of these distributions.
    eShowerAngularDeviationTXDistribution_mean = histDeltaTX.GetMean();
    eShowerAngularDeviationTXDistribution_sigma = histDeltaTX.GetRMS();
    eShowerAngularDeviationTYDistribution_mean = histDeltaTY.GetMean();
    eShowerAngularDeviationTYDistribution_sigma = histDeltaTY.GetRMS();

    Log(3,"EdbShowerP::CalcShowerAngularDeviationDistribution()","CalcShowerAngularDeviationDistribution()...done");
    return;
}


//______________________________________________________________________________


void EdbShowerP::CalcPurity()
{
    if (eNBT>0) ePurity=(Float_t)eNBTMC/(Float_t)eNBT;
    if (gEDBDEBUGLEVEL>3) cout <<"EdbShowerP::CalcPurity():  eNBTMC=" << eNBTMC << " eNBT= " << eNBT << "  ePurity="  << ePurity << endl;
    return;
}


//______________________________________________________________________________


void EdbShowerP::CalcSphericity()
{
    Log(3,"EdbShowerP::CalcSphericity()","CalcSphericity().");

    // Calculates the Sphericity of all BTs around shower axis:
    // "Sphericity" may not have a common definition so refer to this code when
    // describing it.

    eSphericity=0;
    if (gEDBDEBUGLEVEL>3) this->PrintSegments();

    // 1) Get Shower Axis as TVector:
    TVector3 axis(eShowerAxisCenterGravityBT->TX(),eShowerAxisCenterGravityBT->TY(),1);

    TVector3  axisUnit=axis.Unit();
    TVector3  TestVectorUnit;

    if (gEDBDEBUGLEVEL>3) axisUnit.Print();
    TVector3 TestVector;
    TVector3 SumVector;
    TVector3 CrossVector;
    SumVector.SetXYZ(0,0,0);

    for (int i=0; i<eNBT; i++) {
        EdbSegP*  TestBT = this->GetSegment(i);
        TestVector.SetXYZ(TestBT->TX(),TestBT->TY(),1);
        TestVectorUnit=TestVector.Unit();
        SumVector+=TestVectorUnit;
    }

    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowerP::CalcSphericity()   SumVector" << endl;
        SumVector.Print();
    }

    TVector3  SumVecUnit=SumVector.Unit();
    TVector3  SumVecUnitDotAxis=SumVecUnit.Cross(axisUnit);

    if (gEDBDEBUGLEVEL>3) {
        cout << "EdbShowerP::CalcSphericity()   Final value: Mag " << SumVector.Mag() << endl;
        cout << "EdbShowerP::CalcSphericity()   Final value: DotMag (=eSphericity) " << SumVecUnitDotAxis.Mag() << endl;
    }

    eSphericity=SumVecUnitDotAxis.Mag();

    Log(3,"EdbShowerP::CalcSphericity()","CalcSphericity()...done.");
    return;
}

//______________________________________________________________________________


Float_t  EdbShowerP::CalcOpenAngleSimple()
{
    Log(3,"EdbShowerP::CalcOpenAngleSimple()","CalcOpenAngleSimple()");
    // Simply Return Angle between first two segments in the
    // shower array:
    if (N()<2) return 0;

    EdbSegP* s1 = GetSegment(0);
    EdbSegP* s2 = GetSegment(1);

    return GetdT(s1,s2);
}

//______________________________________________________________________________

Float_t  EdbShowerP::CalcOpenAngle(Int_t type)
{
    Log(3,"EdbShowerP::CalcOpenAngle()","CalcOpenAngle()");

    // Calculates opening angle using following assumptions and
    // requiries:
    // TwoByTwo Segment Pairs:
    // a)	dMin<60;
    // b)	dTheta<200;
    // c)	dZ<4000 (ca. 3plates differences)
    // c)	dZ_To_Z0<5500 (ca. 4plates differences)

    // Out of these possible options:
    // Choose SegmentPairing which occurst at first Z position; give preference to
    // pairings appearing at same plate

    EdbSegP* s1=0;
    EdbSegP* s2=0;
    Double_t tmp_dMin,tmp_dTheta;
    Float_t tmp_minZ=0, abs_minZ=999999999;
    Int_t tmp_diffPID=0;
    Int_t min_diffPID=99;
    Int_t tmp_n1=0;
    Int_t tmp_n2=0;
    Double_t min_dMin=0;
    Double_t min_dTheta=0;
    Double_t min_dR=0;
    Float_t tmp_dR;
    Float_t tmp_diffZ;
    Bool_t found_for_PIDDIFF=kFALSE;

    Int_t ncombination=(N()*(N()-1))/2;
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::CalcOpenAngle   Number of combinations to be searched: " << ncombination << endl;

    Float_t minZ=TMath::Min(GetSegmentFirstZ(),GetSegmentLastZ());
    //cout << "GetSegmentFirstZ() " << GetSegmentFirstZ() << endl;
    //cout << "GetSegmentLastZ() " << GetSegmentLastZ() << endl;

    for (Int_t n0=0; n0<=3; ++n0) {
        if (gEDBDEBUGLEVEL>3)  cout << "EdbShowerP::CalcOpenAngle   Doing PIDDIFF_"  << n0 << "   (found yet: " <<  found_for_PIDDIFF << " ) " << endl;

        abs_minZ=999999999;
        min_diffPID=99;

        for (Int_t n1=0; n1<N()-1; ++n1) {
            for (Int_t n2=n1+1; n2<N(); ++n2) {
                // 			cout << " Combi found for n1= " << n1 << " and n2 = " << n2 << endl;
                s1=GetSegment(n1);
                s2=GetSegment(n2);

                tmp_minZ=TMath::Min(s1->Z(),s2->Z());
                tmp_diffZ=TMath::Abs(s1->Z()-s2->Z());
                tmp_diffPID=TMath::Abs(s1->PID()-s2->PID());

                if (tmp_diffPID>n0) continue;
                if (tmp_diffZ>4000) continue;
                if (TMath::Abs(minZ-tmp_minZ)>5500) continue;

                if (GetdMin(s1,s2)>60) continue;
                if (GetdT(s1,s2)>0.2) continue;

                tmp_dMin=GetdMin(s1,s2);
                tmp_dTheta=GetdT(s1,s2);
                tmp_dR=GetdR(s1,s2);
                // cout << " Combi found for n1= " << n1 << " and n2 = " << n2 << endl;
                // cout << " With tmp_dMin= " << tmp_dMin << " and tmp_dTheta = " << tmp_dTheta << endl;

                if (tmp_minZ<=abs_minZ) {
                    abs_minZ=tmp_minZ;

                    if (tmp_diffPID<=min_diffPID) {
                        min_diffPID=tmp_diffPID;

                        tmp_n1=n1;
                        tmp_n2=n2;
                        min_dTheta=tmp_dTheta;
                        min_dMin=tmp_dMin;
                        min_dR=tmp_dR;

                        found_for_PIDDIFF=kTRUE;
                    }
                }
                //if (tmp_minZ<=abs_minZ)
            }
            // for (Int_t n2=n1+1; n2<N(); ++n2)
        }
        //for (Int_t n1=0; n1<N()-1; ++n1)

        // Comment:
        if (gEDBDEBUGLEVEL>3)  {
            cout << "EdbShowerP::CalcOpenAngle   Best Combi found for tmp_n1= " << tmp_n1 << " and tmp_n2 = " << tmp_n2 << "  having a tmp_Z of  " << tmp_minZ << "  min_dTheta= " <<  min_dTheta <<  "  min_dMin= " <<  min_dMin <<  "  min_dR= " <<  min_dR <<   "  tmp_diffPID= " <<  tmp_diffPID << endl;
        }


    }  // for (Int_t n0=0; n0<=3; ++n0) //



    if (type==0) {
        return min_dTheta;		// equivalent to ePairOpeningAngle in BuildParametrisation_PP()
    }
    else if (type==1) {
        return min_dMin;			// equivalent to ePairOpeningDist_dMin in BuildParametrisation_PP()
    }
    else if (type==2) {
        return min_dR;				// equivalent to ePairOpeningDist_dR in BuildParametrisation_PP()
    }
    else {
        return 0;
    }

    return 0;
}



//______________________________________________________________________________



//______________________________________________________________________________

void EdbShowerP::Update()
{
    Log(3,"EdbShowerP::Update()","Update()");
    if (eNBT==0) {
        cout << "Empty shower. Dont Update. return;" << endl;
        return;
    }

    if (this->IsSorted()==kFALSE) Sort();
    this->BuildNMC();
    this->BuildShowerAxis();
    this->BuildProfiles();
    this->BuildPlateProfile();
    this->BuildNplN0();
    this->CalcPurity();
    CalcShowerAngularDeviationDistribution();
    Log(3,"EdbShowerP::Update()","Update()...done.");

    // Maybe we should put the BuildParametrisations right now also in here?
    // -> Put  in UpdateX() if needed to BuildParametrisations
    return;
}

//______________________________________________________________________________

void EdbShowerP::UpdateX()
{
    Log(3,"EdbShowerP::UpdateX()","UpdateX() Update() and Build Shower Parametrisations from scratch");
    if (eNBT==0) {
        cout << "Empty shower. Dont Update. return;" << endl;
        return;
    }
    this->Update();
    this->BuildParametrisation(0); // FJ()
    this->BuildParametrisation(1); // LT()
    this->BuildParametrisation(2); // YC()
    this->BuildParametrisation(3); // JC()
    this->BuildParametrisation(4); // XX()
    this->BuildParametrisation(5); // YY()
    //this->BuildParametrisation(6); // PP() // not yet totally finished, needs
    // refinement for the e+e-search when BT miss due to scan efficency...
    // also angular plane fit still lacking...
    this->BuildParametrisation(7); // AS()
    this->BuildParametrisation(8); // SE()
    Log(3,"EdbShowerP::UpdateX()","UpdateX()...done.");
    return;
}

//______________________________________________________________________________


void EdbShowerP::Finalize()
{
    Log(2,"EdbShowerP::Finalize()","Finalize() Deleting Profile Histograms.");
    if (eProfileLongitudinal)     {
        delete eProfileLongitudinal;
        eProfileLongitudinal=0;
    }
    if (eProfileTransversal)      {
        delete eProfileTransversal;
        eProfileTransversal=0;
    }
    Log(2,"EdbShowerP::Finalize()","Finalize()...done.");
    return;
}


//______________________________________________________________________________









//______________________________________________________________________________

void EdbShowerP::MergeFromShower(EdbShowerP* show, Int_t MergingType)
{
    switch (MergingType) {
    case 0 :
        MergeFromShowerByAddress(show);
        break;
    case 1 :
        MergeFromShowerByPosition(show);
        break;
    default :
        MergeFromShowerByPosition(show);
        break;
    }
    return;
}

//______________________________________________________________________________

void EdbShowerP::MergeFromShowerByAddress(EdbShowerP* show)
{
    // Should only be used for data from the SAME EdbPVRec Object since there by definition each segment has different
    // address. Otherwise duplications can appear!
    cout << "EdbShowerP::MergeFromShowerByAddress().... EXPERIMENTAL STATUS ... PLEASE BE CAREFUL AND CHECK YOUR " << endl;
    cout << "EdbShowerP::MergeFromShowerByAddress().... EXPERIMENTAL STATUS ... RESULTS TO CONSITENCY" << endl;

    // First Check if the two showers are different. If they are the same
    // it makes no sense to merge them...
    cout <<" &this   =  "  <<  this << endl;
    cout <<" &show   =  "  <<  show << endl;
    if (this==show) {
        cout << "EdbShowerP::MergeFromShowerByAddress()   Shower and shower to merge are identical. Skip this function and return." << endl;
        return;
    }

    // No do the comparison: for the shower segments of "this" and the object "show" to merge in
    // are looped and memory addresses (or positions and angles???) are compared.

    cout << "EdbShowerP::MergeFromShowerByAddress()   Loop over  " << eNBT << " BTs  comparing with  "<< show->GetNBT() << " BTs from the outher shower"<<endl;
    EdbSegP* seg1;
    EdbSegP* seg2;

    Int_t   NewAdded=0;
    Bool_t  IsNewAdded=kFALSE;

    for (Int_t j=0; j<show->GetNBT() ; j++) {
        for (Int_t i=0; i<eNBT; i++) {

            seg1=this->GetSegment(i);
            seg2=show->GetSegment(j);

            // Compare using Adresses:
            if (seg1!=seg2) {
                cout << "EdbShowerP::MergeFromShowerByAddress()   seg1 and seg2 are not identical. Add to shower." << endl;

                this->AddSegment(seg2);

                ++NewAdded;
                IsNewAdded=kTRUE;
            }
            //cout << "----------"<<endl;

            if (IsNewAdded) {
                IsNewAdded=kFALSE;
                break;
            }
        }
    }

    cout << "EdbShowerP::MergeFromShowerByAddress()   NewAdded BaseTracks:  " << NewAdded << endl;

    // ReUpdate shower, since we have most likely a different shower now:
    this->Update();

    cout << "EdbShowerP::MergeFromShowerByAddress()  ...done." << endl;
    return;
}



//______________________________________________________________________________


void EdbShowerP::MergeFromShowerByPosition(EdbShowerP* show)
{
    cout << "EdbShowerP::MergeFromShowerByPosition()" << endl;
    cout << "EdbShowerP::MergeFromShowerByPosition().... EXPERIMENTAL STATUS ... PLEASE BE CAREFUL AND CHECK YOUR " << endl;
    cout << "EdbShowerP::MergeFromShowerByPosition().... EXPERIMENTAL STATUS ... RESULTS TO CONSITENCY" << endl;

    Float_t space_diff=0.1;
    Float_t angular_diff=0.001;

    // No do the comparison: for the shower segments of "this" and the object "show" to merge in
    // are looped and positions and angles are compared.

    cout << "EdbShowerP::MergeFromShowerByPosition()   Loop over  " << eNBT << " BTs  comparing with  "<< show->GetNBT() << " BTs from the outher shower"<<endl;
    EdbSegP* seg1=0;
    EdbSegP* seg2=0;

    Int_t   NewAdded=0;
    Bool_t  IsNewAdded=kFALSE;
    Bool_t  DontAdd=kTRUE;

    for (Int_t j=0; j<show->GetNBT() ; j++) {
        DontAdd=kFALSE;
        for (Int_t i=0; i<eNBT; i++) {

            seg1=this->GetSegment(i);
            seg2=show->GetSegment(j);

            // Compare using positions:
            if (TMath::Abs(seg1->X()-seg2->X())< space_diff || TMath::Abs(seg1->Y()-seg2->Y())<space_diff || TMath::Abs(seg1->TX()-seg2->TX())< angular_diff || TMath::Abs(seg1->TY()-seg2->TY())<angular_diff ) {
                cout << "EdbShowerP::MergeFromShowerByPosition()   seg1 (from base shower) and seg2 (from adding shower) are identical. DONT Add to shower." << endl;
                DontAdd=kTRUE;
            }
        } // for (Int_t i=0; i<eNBT; i++)

        if (DontAdd) continue;

        this->AddSegment(seg2);
        ++NewAdded;
        IsNewAdded=kTRUE;
    }
    cout << "EdbShowerP::MergeFromShowerByPosition()   NewAdded BaseTracks:  " << NewAdded << endl;

    // ReUpdate shower, since we have most likely a different shower now:
    if (NewAdded>0) this->Update();
    cout << "EdbShowerP::MergeFromShowerByPosition()  ...done." << endl;
    return;
}





//______________________________________________________________________________

void EdbShowerP::BuildParametrisation(Int_t ParaNr)
{
    Log(3,"EdbShowerP::BuildParametrisation()","EdbShowerP::BuildParametrisation().");
    Log(3,"EdbShowerP::BuildParametrisation()","SOME ARE NOT YET IMPLEMENTED!");
    Log(3,"EdbShowerP::BuildParametrisation()","SOME DEPEND ON OTHERS, SO IF BUILD THESE, BUILD THE OTHERS BEFORE");

    switch (ParaNr) {
    case 0 :
        BuildParametrisation_FJ();
        break;
    case 1 :
        BuildParametrisation_LT(); // needs FJ values.
        break;
    case 2 :
        BuildParametrisation_YC();
        break;
    case 3 :
        BuildParametrisation_JC(); // needs FJ and YC values.
        break;
    case 4 :
        BuildParametrisation_XX();
        break;
    case 5 :
        BuildParametrisation_YY();
        break;
    case 6 :
        BuildParametrisation_PP();
        break;
    case 7 :
        BuildParametrisation_AS();
        break;
    case 8 :
        BuildParametrisation_SE();
        break;
    default :
        cout << "BuildParametrisation  //do nothing;" << endl;
        break;
    }
    Log(3,"EdbShowerP::BuildParametrisation()","EdbShowerP::BuildParametrisation()...done.");
    return;
}


//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_FJ()
{
    Log(3,"EdbShowerP::BuildParametrisation_FJ()","EdbShowerP::BuildParametrisation_FJ()");

    // This parametrisation consists of these variables:
    //  0)  Axis TanTheta
    //  1)  NBT
    //  2)  BT_deltaR_mean
    //  3)  BT_deltaR_rms
    //  4)  BT_deltaT_mean
    //  5)  BT_deltaT_rms
    //  6)  longprofile[0]=histo_longprofile->GetBinContent(1) // number of basetracks in the SAME plate as the Initiator basetrack.
    //  7)  longprofile[1]=histo_longprofile->GetBinContent(2)
    //  ...
    //  5+Npl())  longprofile[Npl()-1]=histo_longprofile->GetBinContent(Npl()) // number of basetracks in the LAST plate of the reconstructed shower.
    //


    //=======================================================================================
    //==C==  DUMMY routine to get the values   deltarb  and   deltathetab  filled:
    //==C==  necessary since its an OLD relict from OLD times where this was saved only
    //==C==  as ROOT TTree ("treebranch").
    //=======================================================================================

    EdbSegP* seg;
    //EdbSegP* InSeg=(EdbSegP*)this->GetSegment(0);
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

    for (int ii=0; ii<N(); ii++)  {
        seg=(EdbSegP*)this->GetSegment(ii);
        shower_xb[ii]=seg->X();
        shower_yb[ii]=seg->Y();
        shower_txb[ii]=seg->TX();
        shower_tyb[ii]=seg->TY();
        shower_zb[ii]=seg->Z();
        shower_deltathetab[ii]=0.5;
        shower_deltarb[ii]=200;
    }

    //-------------------------------------
    for (int ii=0; ii<N(); ii++)  {
        seg=(EdbSegP*)this->GetSegment(ii);
        if (gEDBDEBUGLEVEL>2) {
            cout << "====== --- DOING   "  << ii << endl;
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

            for (int jj=0; jj<N(); jj++)  {
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



    if (N()==2) {
        // behelfsfunktion:
        // but this was only for same Z case...(cause not extrapolated deltar calculation)
        // decided to skip this
        // 	  shower_deltathetab[1]=TMath::Sqrt(TMath::Power(shower_txb[0]-shower_txb[1],2)+TMath::Power(shower_tyb[0]-shower_tyb[1],2));
        // 	  shower_deltarb[1]=TMath::Sqrt(TMath::Power(shower_xb[0]-shower_xb[1],2)+TMath::Power(shower_yb[0]-shower_yb[1],2));
    }

    //-------------------------------------
    if (gEDBDEBUGLEVEL>2) {
        cout << "Shower:  nentries= " << N() << endl;
        for (int ii=0; ii<N(); ii++)  {
            cout << "Shower:  nentry= " << ii << "  shower_zb[ii] =  " << shower_zb[ii] << "  shower_deltathetab[ii] =  " << shower_deltathetab[ii] << "  shower_deltarb[ii] =  " << shower_deltarb[ii] <<endl;
        }
    }
    //-------------------------------------


    //=C= =====================================================================
    TString histname;
    TH1D* histo_nbtk_av;
    TH1D* histo_longprofile_av;
    TH1D* histo_transprofile_av;
    TH1D* histo_deltaR_mean;
    TH1D* histo_deltaT_mean;
    TH1D* histo_deltaR_rms;
    TH1D* histo_deltaT_rms;
    TH1D* histo_nbtk;
    TH1D* histo_longprofile;
    TH1D* histo_transprofile;
    TH1D* histo_deltaR;
    TH1D* histo_deltaT;
    // Profile starting with (arrayindex ==0): number of basetracks in the SAME plate as the Initiator basetrack.
    // Profile ending with (arrayindex ==56): // number of basetracks in the LAST plate of the reconstructed shower.
    Int_t   longprofile[57];

    //Int_t numberofilms=Npl(); // Historical reasons why there are more names for the same variable...
    Int_t nbfilm=Npl();       // Historical reasons why there are more names for the same variable...
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
    Int_t sizeb=N();
    //Int_t nentries_withisizeb=0;
    //=C= =====================================================================
    histname="histo_nbtk_av"+TString(Form("_%d", 0));
    histo_nbtk_av    = new TH1D(histname,"Average basetrack numbers",21,0.0,10.0);
    histname="histo_longprofile_av"+TString(Form("_%d", 0));
    histo_longprofile_av   = new TH1D(histname,"Basetracks per emulsion number",57,0.0,57.0);
    histname="histo_transprofile_av"+TString(Form("_%d", 0));
    histo_transprofile_av  = new TH1D(histname,"Basetracks in trans distance",8,0.0,800.0);
    histname="histo_deltaR_mean"+TString(Form("_%d", 0));
    histo_deltaR_mean      = new TH1D(histname,"Mean #deltar of all BTs in one shower",100,0.0,100.0);
    histname="histo_deltaT_mean"+TString(Form("_%d", 0));
    histo_deltaT_mean      = new TH1D(histname,"Mean #delta#theta of all BTs in one shower",100,0.0,0.1);
    histname="histo_deltaR_rms"+TString(Form("_%d", 0));
    histo_deltaR_rms       = new TH1D(histname,"RMS #deltar of all BTs in one shower",100,0.0,100.0);
    histname="histo_deltaT_rms"+TString(Form("_%d", 0));
    histo_deltaT_rms       = new TH1D(histname,"RMS #delta#theta of all BTs in one shower",100,0.0,0.1);
    //==C== These Histos are for each Shower Entrie filled new
    histname="histo_nbtk"+TString(Form("_%d", 0));
    histo_nbtk             = new TH1D(histname,"Basetracks in the shower",50,0.0,100.0);
    histname="histo_longprofile"+TString(Form("_%d", 0));
    histo_longprofile      = new TH1D(histname,"Basetracks per emulsion number",57,0.0,57.0);
    histname="histo_deltaR"+TString(Form("_%d", 0));
    histo_deltaR           = new TH1D(histname,"Single #deltar of all BTs in Shower",100,0.0,150.0);
    histname="histo_deltaT"+TString(Form("_%d", 0));
    histo_deltaT           = new TH1D(histname,"Single #delta#theta of all BTs in Shower",150,0.0,0.15);
    histname="histo_transprofile"+TString(Form("_%d", 0));
    histo_transprofile  = new TH1D(histname,"Basetracks in trans distance",8,0.0,800.0);
    //=C= =====================================================================
    histo_nbtk_av          ->Reset();
    histo_nbtk             ->Reset();
    histo_longprofile_av   ->Reset();
    histo_deltaR_mean      ->Reset();
    histo_deltaT_mean      ->Reset();
    histo_deltaR_rms       ->Reset();
    histo_deltaT_rms       ->Reset();
    histo_longprofile      ->Reset();
    histo_deltaR           ->Reset();
    histo_deltaT           ->Reset();
    //=C= =====================================================================
    for (int id=0; id<57; id++) {
        theta[id]= 0;
        X0[id] = id*1300.*GetSegment(0)->TX() + GetSegment(0)->X();
        Y0[id] = id*1300.*GetSegment(0)->TY() + GetSegment(0)->Y();
        longprofile[id]=-1;
        // this is important, cause it means that where is -1 there is no BT reconstructed anymore
        // this is different for example to holese where N=0 , so one can distinguish!
    }

    if (gEDBDEBUGLEVEL>3) cout << "histo_longprofile->GetBinWidth() "  << histo_longprofile->GetBinWidth(1)  << endl;
    if (gEDBDEBUGLEVEL>3) cout << "histo_longprofile->GetBinCenter() "  << histo_longprofile->GetBinCenter(1)  << endl;
    if (gEDBDEBUGLEVEL>3) cout << "histo_longprofile->GetNbinsX() "  << histo_longprofile->GetNbinsX()  << endl;

    TX0 = GetSegment(0)->TX();
    TY0 = GetSegment(0)->TY();
    for (Int_t j = 0; j<57; ++j) {
        Dr[j] = 0.03*j*1300. +20.0;
        //cout << " DEBUG   j= " <<  j << "  Dr[j]= " <<  Dr[j] << endl;
    } // old relict from FJ. Do not remove. //
    // it represents somehow conesize......(??)

    //=C= =====================================================================
    for (Int_t ibtke = 0; ibtke < N(); ibtke++) {
        xb[ibtke]=GetSegment(ibtke)->X();
        yb[ibtke]=GetSegment(ibtke)->Y();
        zb[ibtke]=GetSegment(ibtke)->Z();
        txb[ibtke]=GetSegment(ibtke)->TX();
        tyb[ibtke]=GetSegment(ibtke)->TY();
        // abs() of filmPID with respect to filmPID of first BT, plus 1: (nfilmb(0):=1 per definition):
        // Of course PID() have to correctly (by ReadEdbPVrec) correctly.
        // Fedra should do it.
        nfilmb[ibtke]=TMath::Abs(GetSegment(ibtke)->PID()-GetSegment(0)->PID())+1;

        if (gEDBDEBUGLEVEL>2) {
            cout << "ibtke= " <<ibtke << " xb[ibtke]= " << xb[ibtke] << " nfilmb[ibtke]= " << nfilmb[ibtke] << endl;
        }
    }
    //=C= =====================================================================
    //=C= loop over the basetracks in the shower (boucle sur les btk)
    for (Int_t ibtke = 0; ibtke < N(); ibtke++) {
        dist = sqrt((xb[ibtke]- X0[nfilmb[ibtke]-1])*(xb[ibtke]- X0[nfilmb[ibtke]-1])+(yb[ibtke]- Y0[nfilmb[ibtke]-1])*(yb[ibtke]- Y0[nfilmb[ibtke]-1]));

        // inside the cone
        //cout << "ibtke= " <<ibtke << "   dist =  "  <<  dist << "  Dr[nfilmb[ibtke]-1] = " <<  Dr[nfilmb[ibtke]-1] << endl;
        //cout << "    nfilmb[ibtke]  =  " << nfilmb[ibtke] <<   "   nbfilm =  " << nbfilm << endl;

        // In old times there was here an additional condition which checked the BTs for the ConeDistance.
        // Since at this point in buildparametrisation_FJ the shower is -already fully reconstructed -
        // (by either EdbShoAlgo, or ShowRec program or manual list) this intruduces an additional cut
        // which is not correct because conetube size is algorithm specifiv (see Version.log.txt #20052010)
        // Thats wy we drop it here....
        // 	    if (dist<Dr[nfilmb[ibtke]-1]&&nfilmb[ibtke]<=nbfilm) {  // original if line comdition
        if (dist>Dr[nfilmb[ibtke]-1]) {
            if (gEDBDEBUGLEVEL>2) {
                cout << " WARNING , In old times this cut (dist>Dr[nfilmb[ibtke]-1]) (had also to be fulfilled!"<<endl;
                cout << "           For this specific shower it seems not the case....." << endl;
                cout << "           You might want to check this shower again manualy to make sure everything is correct....." << endl;
            }
        }
        if (nfilmb[ibtke]<=nbfilm) {
            //cout << "DEBUG CUTCONDITION WITHOUT THE (?_?_? WRONG ?_?_?) CONE DIST CONDITION....." << endl;
            // cout << yes, this additional cut is not necessary anymore, see above....

            histo_longprofile        ->Fill(nfilmb[ibtke]);
            histo_longprofile_av     ->Fill(nfilmb[ibtke]);

            Double_t DR=0;  //Extrapolate the old stlye way:
            Double_t Dx=xb[ibtke]-(xb[0]+(zb[ibtke]-zb[0])*txb[0]);
            Double_t Dy=yb[ibtke]-(yb[0]+(zb[ibtke]-zb[0])*tyb[0]);
            DR=TMath::Sqrt(Dx*Dx+Dy*Dy);
            histo_transprofile_av->Fill(DR);
            histo_transprofile->Fill(DR);

            theta[nfilmb[ibtke]]+= (TX0-txb[ibtke])*(TX0-txb[ibtke])+(TY0-tyb[ibtke])*(TY0-tyb[ibtke]);
            if (ibtke>0&&nfilmb[ibtke]<=nbfilm) {
                // histo_deltaR           ->Fill(deltarb[ibtke]);
                // histo_deltaT           ->Fill(deltathetab[ibtke]);
                // uses shower_deltarb,shower_deltathetab just calculated in dummy routine above.
                // The first BT is NOT used for this filling since the extrapolated values of
                // shower_deltarb and shower_deltathetab are set manually to 0.5 and 200, due to
                // historical reasons (these variables com from the e/pi separation stuff).
                histo_deltaR           ->Fill(shower_deltarb[ibtke]);
                histo_deltaT           ->Fill(shower_deltathetab[ibtke]);
            }
        }
    }//==C== END OF loop over the basetracks in the shower
    //cout <<"---------------------------------------"<<endl;
    //=======================================================================================
    //==C== Fill NumberBT Histogram for all showers:
    histo_nbtk                   ->Fill(sizeb);
    histo_nbtk_av                ->Fill(sizeb);
    //==C== Fill dR,dT Mean and RMS Histos for all showers:
    histo_deltaR_mean            ->Fill(histo_deltaR->GetMean());
    histo_deltaT_mean            ->Fill(histo_deltaT->GetMean());
    histo_deltaR_rms             ->Fill(histo_deltaR->GetRMS());
    histo_deltaT_rms             ->Fill(histo_deltaT->GetRMS());
    //=======================================================================================

    // Fill the longprofile array:  (NEW VERSION (arrayindex 0 is same plate as Initiator BT))
    for (Int_t i=1; i<=nbfilm; ++i) {
        // longprofile[i-1] = histo_longprofile->GetBinContent(i);    /// OLD VERSION for (Int_t i=1; i<=nbfilm+1; ++i)
        longprofile[i-1] = histo_longprofile->GetBinContent(i+1);     //  NEW VERSION (arrayindex 0 is same plate as Initiator BT)
        //test+=longprofile[i-1] ;
        if (gEDBDEBUGLEVEL>2) {
            cout << "i= " << i << " longprofile[i-1] "<< longprofile[i-1] << " histo_longprofile->GetBinContent(i) " << histo_longprofile->GetBinContent(i)<< endl;
        }
        //
    }


    //----------------------------------------------------------------------------------------
    // 	  PrintSegments();
    Log(3,"EdbShowerP::BuildParametrisation_AS()","EdbShowerP::BuildParametrisation_AS()");

    //  		EdbSegP* seg0=(EdbSegP*)GetSegment(0)->Clone();
    EdbSegP* seg0=GetSegment(0);
    Float_t zorig=seg0->Z();
    // 		seg0->PrintNice();

    EdbVertex* helperVertex = new EdbVertex();
    helperVertex->SetXYZ(seg0->X(),seg0->Y(),seg0->Z());
    // 	  helperVertex->Print();

    Float_t TransDist,LongDist,TotalDist=0;
    Float_t Point[3];
    Float_t LineStart[3];
    Float_t LineEnd[3];
    EdbSegP* InBT = seg0;

    LineStart[0]=InBT->X();
    LineStart[1]=InBT->Y();
    LineStart[2]=InBT->Z();
    EdbSegP* InBTProp = seg0;
    InBTProp->PropagateTo(zorig+10000.0);
    LineEnd[0]=InBTProp->X();
    LineEnd[1]=InBTProp->Y();
    LineEnd[2]=InBTProp->Z();
    InBTProp->PropagateTo(zorig);

    Double_t edIP[50];
    Double_t edMin[50];
    Double_t edRLong[50];
    Double_t edRTrans[50];
    Double_t edeltarb[50];
    Double_t edeltathetab[50];

    Int_t Nmax=TMath::Min(N(),50); // take only the first 50 segments for this parametrisation:
    for (int i=0; i<50; ++i) {
        ePara_AS.edIP[i]=0;
        ePara_AS.edMin[i]=0;
        ePara_AS.edRLong[i]=0;
        ePara_AS.edRTrans[i]=0;
        ePara_AS.edeltarb[i]=0;
        ePara_AS.edeltathetab[i]=0;
    }
    ePara_AS.nseg=N();

    // First Segment we dont need to calculate by definition (all vals are zero anyway).
    for (Int_t j=1; j<Nmax; ++j) {
        // 		  cout << "j= " << j << endl;
        EdbSegP* seg=GetSegment(j);
        // 		  cout << "seg= " << seg << endl;
        // 		  cout << "now printnice" << endl;
        // 		  seg->PrintNice();

        LongDist=0;
        TransDist=0;
        TotalDist=0;
        EdbSegP*	TestBT = seg;
        Point[0]=TestBT->X();
        Point[1]=TestBT->Y();
        Point[2]=TestBT->Z();
        TransDist=EdbMath::DistancePointLine3(Point, LineStart,LineEnd, true);
        TotalDist+=(Point[0]-LineStart[0])*(Point[0]-LineStart[0]);
        TotalDist+=(Point[1]-LineStart[1])*(Point[1]-LineStart[1]);
        TotalDist+=(Point[2]-LineStart[2])*(Point[2]-LineStart[2]);
        TotalDist=sqrt(TotalDist);
        LongDist=sqrt(TotalDist*TotalDist-TransDist*TransDist);

        // 		  cout << "LongDist= " << LongDist << endl;

        edIP[j]=CalcIP(seg,helperVertex);
        edMin[j]=GetdMin(seg0,seg);
        edRLong[j]=LongDist;
        edRTrans[j]=TransDist;
        edeltarb[j]=shower_deltarb[j];
        edeltathetab[j]=shower_deltathetab[j];


        ePara_AS.edIP[j]=edIP[j];
        ePara_AS.edMin[j]=edMin[j];
        ePara_AS.edRLong[j]=edRLong[j];
        ePara_AS.edRTrans[j]=edRTrans[j];
        ePara_AS.edeltarb[j]=edeltarb[j];
        ePara_AS.edeltathetab[j]=edeltathetab[j];

        if (gEDBDEBUGLEVEL>3) cout << j << "  " << ePara_AS.edIP[j]  << " " << ePara_AS.edMin[j] << "  " << ePara_AS.edRLong[j] <<  "  "  << ePara_AS.edRTrans[j] <<  "  "  <<  ePara_AS.edeltarb[j] << "  " << ePara_AS.edeltathetab[j] << endl;
    }
    // 		cout << "delete cloned object  seg0;"<<endl;
    // delete cloned object  seg0;
    // delete cloned object  helperVertex;
    // 	  delete seg0;
    delete helperVertex;
    eParametrisationIsDone[7]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_AS()","EdbShowerP::BuildParametrisation_AS()  ...done.");

    // 	  PrintParametrisation_AS();
    // Helper Routine for Parametrisation_AS()...done.
    //-------------------------------------


    //----------------------------------------------------------------------------------------

    // Now set parametrisation values:
    ePara_FJ.ShowerAxisAngle=eShowerAxisAngle;
    ePara_FJ.nseg=N();
    ePara_FJ.BT_deltaR_mean = histo_deltaR->GetMean();
    ePara_FJ.BT_deltaR_rms  = histo_deltaR->GetRMS();
    ePara_FJ.BT_deltaT_mean = histo_deltaT->GetMean();
    ePara_FJ.BT_deltaT_rms  = histo_deltaT->GetRMS();
    for (int ii=0; ii<57; ii++) ePara_FJ.longprofile[ii]=longprofile[ii];

    // Now set parametrisation values (This routine calculates also the
    // values for ePara_LT since it is composed of FJ para plus
    // transversal profile)
    ePara_LT.ShowerAxisAngle=eShowerAxisAngle;
    ePara_LT.nseg=N();
    ePara_LT.BT_deltaR_mean = histo_deltaR->GetMean();
    ePara_LT.BT_deltaR_rms  = histo_deltaR->GetRMS();
    ePara_LT.BT_deltaT_mean = histo_deltaT->GetMean();
    ePara_LT.BT_deltaT_rms  = histo_deltaT->GetRMS();
    // 0 bin is overflow bin...
    for (int ii=0; ii<8; ii++) ePara_LT.transprofile[ii]=histo_transprofile->GetBinContent(ii+1);
    for (int ii=0; ii<57; ii++) ePara_LT.longprofile[ii]=longprofile[ii];

    // Now set parametrisation values (This routine calculates also the
    // values for ePara_JC since it is composed of FJ and JC)
    ePara_JC.ShowerAxisAngle=eShowerAxisAngle;
    ePara_JC.nseg=N();
    ePara_JC.BT_deltaR_mean = histo_deltaR->GetMean();
    ePara_JC.BT_deltaR_rms  = histo_deltaR->GetRMS();
    ePara_JC.BT_deltaT_mean = histo_deltaT->GetMean();
    ePara_JC.BT_deltaT_rms  = histo_deltaT->GetRMS();
    for (int ii=0; ii<57; ii++) ePara_JC.longprofile[ii]=longprofile[ii];



    // Delete Histograms (on heap):
    delete histo_nbtk_av;
    delete histo_longprofile_av;
    delete histo_transprofile_av;
    delete histo_deltaR_mean;
    delete histo_deltaT_mean;
    delete histo_deltaR_rms;
    delete histo_deltaT_rms;
    delete histo_nbtk;
    delete histo_longprofile;
    delete histo_transprofile;
    delete histo_deltaR;
    delete histo_deltaT;

    eParametrisationIsDone[0]=kTRUE;
    eParametrisationIsDone[1]=kTRUE;
    eParametrisationIsDone[7]=kTRUE;

    Log(3,"EdbShowerP::BuildParametrisation_FJ()","EdbShowerP::BuildParametrisation_FJ()  ...done.");
    Log(3,"EdbShowerP::BuildParametrisation_LT()","EdbShowerP::BuildParametrisation_LT()  ...done.");
    Log(3,"EdbShowerP::BuildParametrisation_AS()","EdbShowerP::BuildParametrisation_AS()  ...done.");
    return;
}



//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_LT()
{
    Log(3,"EdbShowerP::BuildParametrisation_LT()","EdbShowerP::BuildParametrisation_LT()");
    // Since this Parametrisation inherits from _FJ we execute BuildParametrisation_FJ
    // then it is automatically done!
    if (!eParametrisationIsDone[0]) BuildParametrisation_FJ();
    eParametrisationIsDone[1]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_LT()","EdbShowerP::BuildParametrisation_LT()  ...done.");
    return;
}


//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_YC()
{
    Log(3,"EdbShowerP::BuildParametrisation_YC()","EdbShowerP::BuildParametrisation_YC()");

    // This parametrisation consists of these variables:
    //	0)	Axis TanTheta
    //  1)  NBT
    //  2)  C1...
    //  3)  a1
    //  4)  alphacalc
    //  5)  nmaxcalc
    // The code for the parametrisation is (mostly) copied verbatim from YC (Yvan Caffari).
    // For the calculation we refer to his PhD thesis...

    // Copy variables used normally to local ones:
    int sizeb    =	N();
    int nbfilm   =	Npl();

    Float_t xb[5000];
    Float_t yb[5000];
    Float_t zb[5000];
    Float_t txb[5000];
    Float_t tyb[5000];

    for (int k=0; k<sizeb; k++) {
        xb[k]   =GetSegment(k)->X();
        yb[k]   =GetSegment(k)->Y();
        zb[k]   =GetSegment(k)->Z();
        txb[k]  =GetSegment(k)->TX();
        tyb[k]  =GetSegment(k)->TY();
    }

    // nfilmb[] index ranging from 0..nseg-1;  eNBTPlate[] index  ranging from 0..56 (plates)
    // Now:: it is totally reversed:
    // index of nfilmb[]  is segment number returning plate where segment []  is
    // index of nfilmb[]  is number of how many segments in plate [].
    // Note that here "int nfilmb[9999]" is a internal variable and not kept later.

    int nfilmb[9999];
    nfilmb[0]=0;

    Log(4,"EdbShowerP::BuildParametrisation_YC()","Fill correctly nfilmb[k].");

    // So fill correctly:
    for (int k=0; k<sizeb; k++) {
        //GetSegment(k)->PrintNice();
        //     cout << "GetSegment(k)->PID()   GetSegment(0)->PID() " << GetSegment(k)->PID() << " " << GetSegment(0)->PID() << endl;
        nfilmb[k]=TMath::Abs(GetSegment(k)->PID()-GetSegment(0)->PID())+1;
        // 		cout << k << " " << nfilmb[k] << "  " << GetSegment(k)->PID()-GetSegment(0)->PID() << endl;
    }

    Log(4,"EdbShowerP::BuildParametrisation_YC()","Fill correctly nfilmb[k].Done.");

    // Code from YC: ----------------------------------
    Float_t radiuslimit = 400;
    Float_t step = 40;
    Int_t itermax = Int_t(radiuslimit/step);
    Float_t rayonstep[100];
    for (int i=0; i<=itermax; i++) rayonstep[i] = step*i;
    //cout << "itermax = " << itermax << endl;
    //for (int i=0;i<=itermax;i++) cout << "rayonstep[i] = " << rayonstep[i] << endl;
    Float_t nbtkfilm[57];
    Float_t nbtkfilmtmp[57];
    Float_t nbtkcylinder[57];
    Float_t nbtkcylindertmp[57];
    Float_t nbtktmp=0;
    Float_t alphacalc[1]= {0};
    Float_t nmaxcalc[1]= {0};
    Float_t C1calc[1]= {0};
    Float_t a1calc[1]= {0};
    Int_t		nbshower=0;
    for (int i=0; i<nbfilm; i++) {
        nbtkfilm[i]=0;
        nbtkfilmtmp[i]=0;
    }
    for (int i=0; i<=itermax; i++) {
        nbtkcylinder[i]=0;
        nbtkcylindertmp[i]=0;
    }

    Log(4,"EdbShowerP::BuildParametrisation_YC()","Internal Parametrisation Variables Reseted.");

    if (sizeb>0) {
        //---definition of shower axis // First Basetrack!
        Float_t xaxis=0;
        Float_t yaxis=0;
        Float_t zaxis=0;
        Float_t txaxis=0;
        Float_t tyaxis=0;

        // Shower Axis defined by first Basetrack
        // Ob defined by showergravitycenter.
        // not good because in average showergravitycenter is 200 microns in x,y away from the ("real") initiator basetrack
        // or in average 300 mrad!!
        xaxis = xb[0];
        yaxis = yb[0];
        zaxis = zb[0];
        txaxis = txb[0];
        tyaxis = tyb[0];

        if (gEDBDEBUGLEVEL>3) cout << "EdbShowerP::BuildParametrisation_YC()   ShowerAxis Defined by First BT: " << xaxis << "  " << yaxis << "  "  << zaxis << "  "  << txaxis << "  "  << tyaxis << "  "  << endl;

        Bool_t UseShowerAxisCenterGravityBT=kTRUE;
        // 		UseShowerAxisCenterGravityBT=kFALSE;
        if (UseShowerAxisCenterGravityBT) {
            xaxis = eShowerAxisCenterGravityBT->X();
            yaxis = eShowerAxisCenterGravityBT->Y();
            zaxis = eShowerAxisCenterGravityBT->Z();
            txaxis = eShowerAxisCenterGravityBT->TX();
            tyaxis = eShowerAxisCenterGravityBT->TY();
        }

        for (int j=0; j<sizeb; j++) {
            Float_t xproj = xaxis + (zb[j]-zaxis)*txaxis;
            Float_t yproj = yaxis + (zb[j]-zaxis)*tyaxis;
            Float_t radius = TMath::Sqrt((xproj-xb[j])*(xproj-xb[j]) + (yproj-yb[j])*(yproj-yb[j]));
            // 			cout << "j radius " << j << "  " << radius << endl;
            if (radius<=radiuslimit) {
                // 				cout << "in if(radius<=radiuslimit){  ::   " << nfilmb[j] << "  " <<  nbfilm << endl;
                if (nfilmb[j]<=nbfilm) {
                    if (nfilmb[j]>nbfilm) cout<<" warning 1 "<<endl;
                    nbtktmp++;
                    for (Int_t k=0; k<nbfilm; k++) {
                        //cout << "k=  nfilmb[j] k+1 " << k << "  "<<  nfilmb[j] << "   " <<  k+1    << endl;
                        if (nfilmb[j]==k+1) {
                            // 							cout << "increasing (k="<< k<< ")  " << endl;
                            nbtkfilm[k]=nbtkfilm[k]+1;
                            nbtkfilmtmp[k]=nbtkfilmtmp[k]+1;
                            // 							cout << "increasing result: " << nbtkfilm[k] << "  " << nbtkfilmtmp[k] << endl;
                        }
                        //cout << "increasing result  2: " << nbtkfilm[k] << "  " << nbtkfilmtmp[k] << endl;
                    }
                    //for (Int_t k=0; k<nbfilm; k++) cout<<" nbtkfilmtmp[k]    "<<   nbtkfilmtmp[k];
                    //cout << endl;
                    Int_t ii=0;
                    for (int toto=0; toto<itermax+1; toto++) {
                        if ((radius>=rayonstep[toto])&&(radius<=rayonstep[toto+1])) ii = toto;
                    }
                    nbtkcylindertmp[ii]++;
                    nbtkcylinder[ii]++;
                }
            }
        }//loop on j

        // gEDBDEBUGLEVEL=4;
        Log(4,"EdbShowerP::BuildParametrisation_YC()","//---longitudinal profile");
        //---longitudinal profile
        Double_t mean = 0;
        Double_t RMS = 0;
        Double_t a = 0;
        Double_t b = 0;
        TH1F* htmplg = new TH1F("htmplg","htmplg",29,0,29);
        for (int l=0; l<nbfilm; l++) {
            //cout <<"  l= " << l << "   nbtkfilmtmp[l]  = " << nbtkfilmtmp[l] << endl;
            if (nbtkfilmtmp[l]>0) {
                htmplg->Fill(l+1,nbtkfilmtmp[l]);
                if (l>=nbfilm) cout<<" warning 2 "<<endl;
            }
        }

        mean = htmplg->GetMean();
        RMS = htmplg->GetRMS();
        //  		cout << "---------------------------------  htmplg->GetMean() " << htmplg->GetMean() << endl;
        // 		cout << "---------------------------------  htmplg->GetRMS() " << htmplg->GetRMS() << endl;

        if ((RMS!=0)&&(mean!=0)) {
            a = (mean*mean)/(RMS*RMS);
            b = mean/(RMS*RMS);
        } else {
            a=0;
            b=0;
        }
        Double_t numerateur = 0;
        Double_t denominateur = 0;
        if ((a!=0)&&(b!=0)) {
            for (int m=0; m<nbfilm; m++) {
                if (m>=nbfilm) cout<<" warning 3 "<<endl;
                if (nbtkfilmtmp[m]>0) {
                    numerateur += TMath::Power(b*(m+1),a-1)*TMath::Exp(-b*(m+1));
                    denominateur += (b*TMath::Power(b*(m+1),2*(a-1))*TMath::Exp(-2*b*(m+1)))/(TMath::Gamma(a)*(nbtkfilmtmp[m]));
                }
            }
        }
        if ((denominateur!=0)||(a!=0)||(b!=0)) {
            alphacalc[nbshower] = 1.4*numerateur/(denominateur);
            nmaxcalc[nbshower] = (a-1)/b;
        } else {
            alphacalc[nbshower] = 0;
            nmaxcalc[nbshower] = 0;
        }

        // Sometimes alpha is NotANumber nan and therefore we set by hand the alpha to zero
        // (FWM, 10082009)
        if ( isnan(alphacalc[nbshower]) ) alphacalc[nbshower]=0.0;



        Log(4,"EdbShowerP::BuildParametrisation_YC()","//---transversal profile");
        //---transversal profile
        TH1F*htmptr = new TH1F("htmptr","htmptr",(itermax+1),0,(itermax+1)*step);
        for (int n=0; n<itermax; n++) {
            if (nbtkcylindertmp[n]!=0) {
                htmptr->Fill((n+1)*step,nbtkcylindertmp[n]);
            }
        }
        Double_t mu = htmptr->GetMean();
        Double_t sigma = htmptr->GetRMS();

        if (((sigma*sigma+mu*mu)-step*mu)!=0) {
            a1calc[nbshower] = TMath::Log( (step*mu+(sigma*sigma+mu*mu)) / ((sigma*sigma+mu*mu)-step*mu) )/(step);
            Double_t alpha1 = TMath::Exp(a1calc[nbshower]*step);
            C1calc[nbshower] = (nbtktmp)*(alpha1-1);
        }
        else {
            C1calc[nbshower] = 0;
            a1calc[nbshower] = 0;
        }

        for (int k1tmp=0; k1tmp<nbfilm; k1tmp++) nbtkfilmtmp[k1tmp] = 0;
        for (int k2tmp=0; k2tmp<itermax; k2tmp++) nbtkcylindertmp[k2tmp] = 0;
        nbtktmp=0;

        delete htmplg;
        htmplg=0;
        delete htmptr;
        htmptr=0;

    } //if(sizeb)

    // Now set parametrisation values:
    ePara_YC.ShowerAxisAngle=eShowerAxisAngle;
    ePara_YC.nseg=N();
    ePara_YC.C1 = C1calc[nbshower];
    ePara_YC.a1 = a1calc[nbshower];
    ePara_YC.alpha = alphacalc[nbshower];
    ePara_YC.nmax = nmaxcalc[nbshower];

    // Now set parametrisation values (This routine calculates also the
    // values for ePara_JC since it is composed of FJ and JC)
    ePara_JC.ShowerAxisAngle=eShowerAxisAngle;
    ePara_JC.nseg=N();
    ePara_JC.C1 = C1calc[nbshower];
    ePara_JC.a1 = a1calc[nbshower];
    ePara_JC.alpha = alphacalc[nbshower];
    ePara_JC.nmax = nmaxcalc[nbshower];

    eParametrisationIsDone[2]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_YC()","EdbShowerP::BuildParametrisation_YC()  ...done.");
    return;
}


//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_JC()
{

    Log(3,"EdbShowerP::BuildParametrisation_JC()","EdbShowerP::BuildParametrisation_JC()");
    // Since this Parametrisation inherits from _FJ and from _JC we execute
    // BuildParametrisation_FJ and BuildParametrisation_YC then it is automatically done!
    if (!eParametrisationIsDone[0]) BuildParametrisation_FJ();
    if (!eParametrisationIsDone[2]) BuildParametrisation_YC();
    eParametrisationIsDone[3]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_JC()","EdbShowerP::BuildParametrisation_JC()  ...done.");

    return;
}


//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_XX()
{
    Log(3,"EdbShowerP::BuildParametrisation_XX()","EdbShowerP::BuildParametrisation_XX()");

    // This parametrisation consists of these variables:
    //	0)	Axis TanTheta
    //  1)  NBT
    //  2)  Mean of     eProfileLongitudinal
    //  3)  RMS  of     eProfileLongitudinal
    //  4)  Maximum of  eProfileLongitudinal
    //  5)  Mean of     eProfileTransversal
    //  6)  RMS  of     eProfileTransversal
    //  7)  Maximum of  eProfileTransversal

    //  8)  LastFilled BinCenter  of     eProfileLongitudinal
    //  9)  LastFilled BinCenter of  eProfileTransversal


    ePara_XX.ShowerAxisAngle=eShowerAxisAngle;
    ePara_XX.nseg=N();

    ePara_XX.Mean_ProfileLongitudinal=eProfileLongitudinal->GetMean();
    ePara_XX.RMS_ProfileLongitudinal=eProfileLongitudinal->GetRMS();

    ePara_XX.Mean_ProfileTransversal=eProfileTransversal->GetMean();
    ePara_XX.RMS_ProfileTransversal=eProfileTransversal->GetRMS();

    // Necessary because the histograms are listed with Z values,
    // but GetMaximum returns the bin in where the maximum occurs:

    // Nota bene: if there is no clear maximum (i.e. all bins have same
    // content, then the first filled bin is returned. This maybe we dont
    // wanna have....in this case we take the mean bin.
    // No.. pretty useless since continous variables and binary variables
    // are mixed. We stick rather to the normal definition. of MaximumBin
    //
    //
    int maxbin;

    maxbin=eProfileLongitudinal->GetMaximumBin();
    ePara_XX.Max_ProfileLongitudinal=(Float_t)eProfileLongitudinal->GetBinCenter(maxbin);
    // 		if (maxbin==1) ePara_XX.Max_ProfileLongitudinal=eProfileLongitudinal->GetMean();

    maxbin=eProfileTransversal->GetMaximumBin();
    ePara_XX.Max_ProfileTransversal=(Float_t)eProfileTransversal->GetBinCenter(maxbin);
    // 		if (maxbin==1) ePara_XX.Max_ProfileTransversal=eProfileTransversal->GetMean();

    maxbin=GetLastFilledBin(eProfileLongitudinal);
    ePara_XX.Last_ProfileLongitudinal=(Float_t)eProfileLongitudinal->GetBinCenter(maxbin);
    maxbin=GetLastFilledBin(eProfileTransversal);
    ePara_XX.Last_ProfileTransversal=(Float_t)eProfileTransversal->GetBinCenter(maxbin);

    eParametrisationIsDone[4]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_XX()","EdbShowerP::BuildParametrisation_XX()  ...done.");
    return;
}


//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_YY()
{
    Log(3,"EdbShowerP::BuildParametrisation_YY()","EdbShowerP::BuildParametrisation_YY()");

    //PrintBasics();

    // This parametrisation consists of these variables:
    //	0)	Axis TanTheta
    //  1)  NBT
    //  2)  eProfileTransversal[0]->BinContent(Bin1);
    //  3)  eProfileTransversal[1]->BinContent(Bin2);
    //  4)  eProfileTransversal[2]->BinContent(Bin3);
    //  5)  eProfileTransversal->BinContent(Bin4);
    //  6)  eProfileTransversal->BinContent(Bin5);
    //  7)  eProfileTransversal->BinContent(Bin6);
    //  8)  eProfileTransversal[6]->BinContent(Bin7);
    //  9)  eProfileTransversal[7]->BinContent(Bin8);
    //  9+1)  eProfileLongitudinal->BinContent(Bin1);
    //      ...
    //  9+56)  eProfileLongitudinal->BinContent(Bin56);

    ePara_YY.ShowerAxisAngle=eShowerAxisAngle;
    ePara_YY.nseg=N();
    for (int i=0; i<8; i++) ePara_YY.ProfileTransversalBincontent[i] = (int)eProfileTransversal->GetBinContent(i+1);
    for (int i=0; i<57; i++) ePara_YY.ProfileLongitudinalBincontent[i] = (int)eProfileLongitudinal->GetBinContent(i+1);
    // Remember, bin(0) is Overflow bin.

    // 	cout << eProfileLongitudinal->GetBinCenter(1) << endl;;
    // 	cout << eProfileLongitudinal->GetBinWidth(1) << endl;;

    eParametrisationIsDone[5]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_YY()","EdbShowerP::BuildParametrisation_YY()  ...done.");
    return;
}


//______________________________________________________________________________


void EdbShowerP::BuildParametrisation_PP()
{
    Log(3,"EdbShowerP::BuildParametrisation_PP()","EdbShowerP::BuildParametrisation_PP()");
    cout << "EdbShowerP::BuildParametrisation_PP()"<< endl;
    cout << "EdbShowerP::BuildParametrisation_PP().....THIS IS STILL IN EXPERIMENTAL STATUS....."<< endl;

    // This parametrisation consists of these variables:
    //	0)	Axis TanTheta
    //  1)  NBT
    //  2)  OpeningAngle of FIRST BT pair in Z (and in same plate) (this is assumed to be the ePlus eMinus pair);
    //  3)  OpeningDist of FIRST BT pair in Z (and in same plate) (this is assumed to be the ePlus eMinus pair);

    // What if at first Z position there is only one BT ? Then we go down the plates until we find a pair...
    // (temporary solution for now.., later: check mathichg BT over different plates....)

    ePara_PP.ShowerAxisAngle=eShowerAxisAngle;
    ePara_PP.nseg=N();
    // See CalcOpenAngle() how to calculate the values...
    ePara_PP.ePairOpeningAngle=CalcOpenAngle(0);
    ePara_PP.ePairOpeningDist_dMin=CalcOpenAngle(1);
    ePara_PP.ePairOpeningDist_dR=CalcOpenAngle(2);
    // Variable to tell if BT Pair is good related to a vertex. Can only be calculated for a given vertex.
    ePara_PP.ePairChi2=0;

    eParametrisationIsDone[6]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_PP()","EdbShowerP::BuildParametrisation_PP() NOT YET INCLUDED FINALLY!!!!");

    return;
}



//______________________________________________________________________________


void EdbShowerP::BuildParametrisation_AS()
{
    Log(3,"EdbShowerP::BuildParametrisation_AS()","EdbShowerP::BuildParametrisation_AS()");
    // Calculate this alues is contained in BuildParametrisation_FJ
    if (eParametrisationIsDone[0]==kFALSE) BuildParametrisation_FJ();
    eParametrisationIsDone[7]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_AS()","EdbShowerP::BuildParametrisation_AS()  ...done.");
    return;
}

//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_SE()
{
    Log(3,"EdbShowerP::BuildParametrisation_SE()","EdbShowerP::BuildParametrisation_SE()");

    // This parametrisation consists of these variables:
    //	0)	Axis TanTheta
    //  1)  NBT
    //  2)  NPL
    //  3)  Efficiency At  Axis TanTheta  (if given anyway. since it can not be measured from
    //      the shower directly we have to find another method.)

    // Now set parametrisation values:
    ePara_SE.ShowerAxisAngle=eShowerAxisAngle;
    ePara_SE.nseg=N();
    ePara_SE.npl=Npl();
    ePara_SE.efficiencyAtShowerAxisAngle = -1;

    eParametrisationIsDone[8]=kTRUE;
    Log(3,"EdbShowerP::BuildParametrisation_SE()","EdbShowerP::BuildParametrisation_SE()  ...done.");
}



//______________________________________________________________________________

void EdbShowerP::BuildParametrisation_ExtraInfo()
{
    Log(3,"EdbShowerP::BuildParametrisation_ExtraInfo()","EdbShowerP::BuildParametrisation_ExtraInfo()");
    ePara_ExtraInfo.InBT_E=GetSegmentFirst()->P();
    ePara_ExtraInfo.InBT_Flag=GetSegmentFirst()->Flag();
    ePara_ExtraInfo.InPairBT_E=ePara_ExtraInfo.InBT_E;
    if (N()>1 && (GetSegment(0)->Flag()+GetSegment(1)->Flag())==0) {
        ePara_ExtraInfo.InPairBT_E=GetSegment(0)->P()+GetSegment(1)->P();
    }
    else {
        ePara_ExtraInfo.InPairBT_E=GetSegment(0)->P();
    }
    ePara_ExtraInfo.ShowerLength=Npl();

    eExtraInfoIsDone[0]=kTRUE;
    return;
}


//______________________________________________________________________________


Float_t EdbShowerP::GetParaVariable(Int_t ParaVariable, Int_t ParaNr)
{
    Log(3,"EdbShowerP::GetParaVariable()","EdbShowerP::GetParaVariable()");

    // Stupid Workaround to return the parameters of the Parametrizations ....
    // But it is not necessary anymore since we can get Parametrisation
    // structures direcly using EdbShowerP::Para_PP  new_Para_PP like this.
    if (ParaNr==6) {
        if (ParaVariable==0) return (Float_t)ePara_PP.ShowerAxisAngle;
        if (ParaVariable==1) return (Float_t)ePara_PP.nseg;
        if (ParaVariable==2) return (Float_t)ePara_PP.ePairOpeningAngle;
        if (ParaVariable==3) return (Float_t)ePara_PP.ePairOpeningDist_dMin;
        if (ParaVariable==4) return (Float_t)ePara_PP.ePairOpeningDist_dR;
        return 0;
    }
    if (ParaNr==0) {
        if (ParaVariable==0) return (Float_t)ePara_FJ.ShowerAxisAngle;
        if (ParaVariable==1) return (Float_t)ePara_FJ.nseg;
        if (ParaVariable==2) return (Float_t)ePara_FJ.BT_deltaR_mean;
        if (ParaVariable==3) return (Float_t)ePara_FJ.BT_deltaR_rms;
        if (ParaVariable==4) return (Float_t)ePara_FJ.BT_deltaT_mean;
        if (ParaVariable==5) return (Float_t)ePara_FJ.BT_deltaT_rms;
        return 0;
    }
    if (ParaNr==2) {
        if (ParaVariable==0) return (Float_t)ePara_YC.ShowerAxisAngle;
        if (ParaVariable==1) return (Float_t)ePara_YC.nseg;
        if (ParaVariable==2) return (Float_t)ePara_YC.C1;
        if (ParaVariable==3) return (Float_t)ePara_YC.a1;
        if (ParaVariable==4) return (Float_t)ePara_YC.alpha;
        if (ParaVariable==5) return (Float_t)ePara_YC.nmax;
        return 0;
    }
    if (ParaNr==4) {
        if (ParaVariable==0) return (Float_t)ePara_XX.ShowerAxisAngle;
        if (ParaVariable==1) return (Float_t)ePara_XX.nseg;
        if (ParaVariable==2) return (Float_t)ePara_XX.Mean_ProfileLongitudinal;
        if (ParaVariable==3) return (Float_t)ePara_XX.RMS_ProfileLongitudinal;
        if (ParaVariable==4) return (Float_t)ePara_XX.Max_ProfileLongitudinal;
        if (ParaVariable==5) return (Float_t)ePara_XX.Mean_ProfileTransversal;
        if (ParaVariable==6) return (Float_t)ePara_XX.RMS_ProfileTransversal;
        if (ParaVariable==7) return (Float_t)ePara_XX.Max_ProfileTransversal;
        if (ParaVariable==8) return (Float_t)ePara_XX.Last_ProfileLongitudinal;
        if (ParaVariable==9) return (Float_t)ePara_XX.Last_ProfileTransversal;
        return 0;
    }
    if (ParaNr==5) {
        if (ParaVariable==0) return (Float_t)ePara_YY.ShowerAxisAngle;
        if (ParaVariable==1) return (Float_t)ePara_YY.nseg;
        if (ParaVariable>=2&&ParaVariable<=6) return (Float_t)ePara_YY.ProfileTransversalBincontent[ParaVariable];
        if (ParaVariable>=7) return (Float_t)(Float_t)ePara_YY.ProfileLongitudinalBincontent[ParaVariable-6+1];
        return 0;
    }
    if (ParaNr==8) {
        if (ParaVariable==0) return (Float_t)ePara_SE.ShowerAxisAngle;
        if (ParaVariable==1) return (Float_t)ePara_SE.nseg;
        if (ParaVariable==2) return (Float_t)ePara_SE.npl;
        if (ParaVariable==3) return (Float_t)ePara_SE.efficiencyAtShowerAxisAngle;
        return 0;
    }
    else {
        cout << "Other parametrisations are not yet supported...." << endl;
    }

    return 0;
}


//______________________________________________________________________________

void EdbShowerP::SetParaVariable(Float_t fParVar, Int_t ParaVariable, Int_t ParaNr)
{
    Log(3,"EdbShowerP::SetParaVariable()","EdbShowerP::SetParaVariable()");

    // Stupid Workaround to return the parameters of the Parametrizations ....
    Log(2,"EdbShowerP::SetParaVariable()","EdbShowerP::SetParaVariable()... Works for now only for ePara_ExtraInfo...!!!");

    if (ParaVariable==4) ePara_ExtraInfo.InBT_IPToVtx=fParVar;
    if (ParaVariable==5) ePara_ExtraInfo.InBT_DeltaZToVtx=fParVar;

    return;
}
//______________________________________________________________________________








//______________________________________________________________________________

void EdbShowerP::Print()
{
    PrintNice();
    return;
    cout << "EdbShowerP::Print()" << endl;
    cout << "EdbShowerP::Print()...done." << endl;
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintNice()
{
    cout << "EdbShowerP::PrintNice():" << endl;
    printf( "EdbShowerP: ID= %6d, N=%6d, NMC=%6d, N0=%6d, NPl=%6d, x= %14.3f, y= %14.3f, z= %14.3f, tx= %7.4f, ty= %7.4f, MCEvt= %6d: \n", ID(), N(), eNBTMC, N0(), Npl(), X(),Y(),Z(),TX(),TY(),eMC );
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintMetaData()
{
    cout << "EdbShowerP::PrintMetaData()" << endl;
    cout << "eAlgName= " << setw(14) << eAlgName <<  ", eAlgValue= " << setw(14) << eAlgValue << endl;
    for (int i=0; i<6; i++) {
        cout << setw(10) << eParaValue[i]<< "  ";
    }
    cout << endl;
    for (int i=0; i<6; i++) {
        cout << setw(10) << eParaString[i]<< "  ";
    }
    cout << endl;
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintBasics()
{
    cout << "EdbShowerP::PrintBasics()" << endl;
    cout << setw(12) << "eNBT= " << setw(18) << N() << endl;
    cout << setw(12) << "eNpl= " << setw(18) << Npl() << endl;
    cout << setw(12) << "eN0= " << setw(18) << N0() << endl;
    cout << setw(12) << "eN00= " << setw(18) << N00() << endl;
    cout << setw(12) << "eNBTMC= " << setw(18) << GetNBTMC() << endl;
    cout << setw(12) << "ePurity= " << setw(18) << GetPurity() << endl;
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintSegments()
{
    cout << "EdbShowerP::PrintSegments:()" << endl;
    for (int i=0; i<eNBT; i++) {
        this->GetSegment(i)->PrintNice();
    }
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintParametrisation(Int_t ParaNr)
{
    cout << "EdbShowerP::PrintParametrisation("<<ParaNr<<")"<< endl;

    if (ParaNr==0) PrintParametrisation_FJ();
    if (ParaNr==1) PrintParametrisation_LT();
    if (ParaNr==2) PrintParametrisation_YC();
    if (ParaNr==3) PrintParametrisation_JC();
    if (ParaNr==4) PrintParametrisation_XX();
    if (ParaNr==5) PrintParametrisation_YY();
    if (ParaNr==6) PrintParametrisation_PP();
    if (ParaNr==7) PrintParametrisation_AS();
    if (ParaNr==8) PrintParametrisation_SE();
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_FJ()
{
    cout << "EdbShowerP::PrintParametrisation_FJ"<< endl;
    if (!eParametrisationIsDone[0]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.ShowerAxisAngle= " << ePara_FJ.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.nseg= " << ePara_FJ.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.BT_deltaR_mean= " << ePara_FJ.BT_deltaR_mean << endl;
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.BT_deltaR_rms= " << ePara_FJ.BT_deltaR_rms << endl;
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.BT_deltaT_mean= " << ePara_FJ.BT_deltaT_mean << endl;
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.BT_deltaT_rms= " << ePara_FJ.BT_deltaT_rms << endl;
    cout << "EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.longprofile= " << endl;

    for (int i=0; i<57; i++) {
        cout << setw(3) << i;
    }
    cout << endl;
    for (int i=0; i<57; i++) {
        cout << setw(3) << ePara_FJ.longprofile[i];
    }
    cout << endl;
    return;
}

//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_LT()
{
    cout << "EdbShowerP::PrintParametrisation_LT"<< endl;
    if (!eParametrisationIsDone[1]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.ShowerAxisAngle= " << ePara_LT.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.nseg= " << ePara_LT.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.BT_deltaR_mean= " << ePara_LT.BT_deltaR_mean << endl;
    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.BT_deltaR_rms= " << ePara_LT.BT_deltaR_rms << endl;
    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.BT_deltaT_mean= " << ePara_LT.BT_deltaT_mean << endl;
    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.BT_deltaT_rms= " << ePara_LT.BT_deltaT_rms << endl;

    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.transprofile= " << endl;
    for (int i=0; i<9; i++) {
        cout << setw(3) << i;
    }
    cout << endl;
    for (int i=0; i<9; i++) {
        cout << setw(3) << ePara_LT.transprofile[i];
    }
    cout << endl;

    cout << "EdbShowerP::PrintParametrisation_LT()    ePara_LT.longprofile= " << endl;
    for (int i=0; i<57; i++) {
        cout << setw(3) << i;
    }
    cout << endl;
    for (int i=0; i<57; i++) {
        cout << setw(3) << ePara_LT.longprofile[i];
    }
    cout << endl;
    return;
}


//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_YC()
{
    cout << "EdbShowerP::PrintParametrisation_YC"<< endl;
    if (!eParametrisationIsDone[2]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_YC()    ePara_YC.ShowerAxisAngle= " << ePara_YC.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_YC()    ePara_YC.nseg= " << ePara_YC.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_YC()    ePara_YC.C1= " << ePara_YC.C1 << endl;
    cout << "EdbShowerP::PrintParametrisation_YC()    ePara_YC.a1= " << ePara_YC.a1 << endl;
    cout << "EdbShowerP::PrintParametrisation_YC()    ePara_YC.alpha= " << ePara_YC.alpha << endl;
    cout << "EdbShowerP::PrintParametrisation_YC()    ePara_YC.nmax= " << ePara_YC.nmax << endl;

    if (ePara_YC.C1==0 || ePara_YC.a1 == 0 ) {
        PrintNice();
        GetSegment(0)->Print();
        PrintSegments();
    }
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_JC()
{
    cout << "EdbShowerP::PrintParametrisation_JC"<< endl;
    if (!eParametrisationIsDone[3]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.ShowerAxisAngle= " << ePara_JC.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.nseg= " << ePara_JC.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.C1= " << ePara_JC.C1 << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.a1= " << ePara_JC.a1 << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.alpha= " << ePara_JC.alpha << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.nmax= " << ePara_JC.nmax << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.ShowerAxisAngle= " << ePara_JC.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.nseg= " << ePara_JC.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.BT_deltaR_mean= " << ePara_JC.BT_deltaR_mean << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.BT_deltaR_rms= " << ePara_JC.BT_deltaR_rms << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.BT_deltaT_mean= " << ePara_JC.BT_deltaT_mean << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.BT_deltaT_rms= " << ePara_JC.BT_deltaT_rms << endl;
    cout << "EdbShowerP::PrintParametrisation_JC()    ePara_JC.longprofile= " << endl;

    for (int i=0; i<57; i++) {
        cout << setw(3) << i;
    }
    cout << endl;
    for (int i=0; i<57; i++) {
        cout << setw(3) << ePara_JC.longprofile[i];
    }
    cout << endl;
    return;
}


//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_XX()
{
    cout << "EdbShowerP::PrintParametrisation_XX"<< endl;
    if (!eParametrisationIsDone[4]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.ShowerAxisAngle= " << ePara_XX.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.nseg= " << ePara_XX.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.Mean_ProfileLongitudinal= " << ePara_XX.Mean_ProfileLongitudinal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.RMS_ProfileLongitudinal= " << ePara_XX.RMS_ProfileLongitudinal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.Max_ProfileLongitudinal= " << ePara_XX.Max_ProfileLongitudinal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.Mean_ProfileTransversal= " << ePara_XX.Mean_ProfileTransversal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.RMS_ProfileTransversal= " << ePara_XX.RMS_ProfileTransversal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.Max_ProfileTransversal= " << ePara_XX.Max_ProfileTransversal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.Last_ProfileLongitudinal= " << ePara_XX.Last_ProfileLongitudinal << endl;
    cout << "EdbShowerP::PrintParametrisation_XX()    ePara_XX.Last_ProfileTransversal= " << ePara_XX.Last_ProfileTransversal << endl;
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_YY()
{
    cout << "EdbShowerP::PrintParametrisation_YY"<< endl;
    if (!eParametrisationIsDone[5]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_YY()    ePara_YY.ShowerAxisAngle= " << ePara_YY.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_YY()   Longitudinal Profile (not plates!, but dist w.r.t to shower axis!)"<<endl;
    cout << "EdbShowerP::PrintParametrisation_YY()   (take Plate Profile instead if you want have this):"<<endl;

    for (int i=1; i<57; i++) {
        cout << setw(3) << i;
    }
    cout << endl;
    for (int i=1; i<57; i++) {
        cout << setw(3) << ePara_YY.ProfileLongitudinalBincontent[i];
    }
    cout << endl;
    cout << "EdbShowerP::PrintParametrisation_YY()   TransVersal Profile (not plates!, but dist w.r.t to shower axis!)"<<endl;
    for (int i=1; i<8; i++) {
        cout << setw(3) << ePara_YY.ProfileTransversalBincontent[i];
    }
    cout << endl;

    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_PP()
{
    cout << "EdbShowerP::PrintParametrisation_PP"<< endl;
    if (!eParametrisationIsDone[6]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << "EdbShowerP::PrintParametrisation_PP()    ePara_PP.ShowerAxisAngle= " << ePara_PP.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_PP()   ePara_PP.nseg= " << ePara_PP.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_PP()   ePara_PP.ePairOpeningAngle= " << ePara_PP.ePairOpeningAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_PP()   ePara_PP.ePairOpeningDist_dR= " << ePara_PP.ePairOpeningDist_dR << endl;
    cout << "EdbShowerP::PrintParametrisation_PP()   ePara_PP.ePairOpeningDist_dMin= " << ePara_PP.ePairOpeningDist_dMin << endl;
    cout << "EdbShowerP::PrintParametrisation_PP ...  STILL TO IMPROVE WITH MORE VARIABLES..  ..  ... ..."<< endl;
    return;
}
//______________________________________________________________________________
void EdbShowerP::PrintParametrisation_AS()
{
    cout << "EdbShowerP::PrintParametrisation_AS"<< endl;
    if (!eParametrisationIsDone[7]) {
        cout << "EdbShowerP::PrintParametrisation()     PARA NOT YET BUILD ! "<< endl;
        return;
    }
    cout << ePara_AS.nseg << endl;
    for (int i=0; i<50; i++) {
        cout << i << "  " << ePara_AS.edIP[i]  << " " << ePara_AS.edMin[i] << "  " << ePara_AS.edRLong[i] <<  "  "  << ePara_AS.edRTrans[i] <<  "  "  <<  ePara_AS.edeltarb[i] << "  " << ePara_AS.edeltathetab[i] << endl;
    }
    return;
}
//______________________________________________________________________________

void EdbShowerP::PrintParametrisation_SE()
{

    cout << "EdbShowerP::PrintParametrisation_SE"<< endl;
    if (!eParametrisationIsDone[8]) cout << "EdbShowerP::PrintParametrisation_SE()     PARA NOT YET BUILD ! "<< endl;
    cout << "EdbShowerP::PrintParametrisation_SE()    ePara_SE.ShowerAxisAngle= " << ePara_SE.ShowerAxisAngle << endl;
    cout << "EdbShowerP::PrintParametrisation_SE()    ePara_SE.nseg= " << ePara_SE.nseg << endl;
    cout << "EdbShowerP::PrintParametrisation_SE()    ePara_SE.npl= " << ePara_SE.npl << endl;
    cout << "EdbShowerP::PrintParametrisation_SE()    ePara_SE.efficiencyAtShowerAxisAngle= " << ePara_SE.efficiencyAtShowerAxisAngle << endl;
    return;
}


void EdbShowerP::PrintParametrisation_ExtraInfo()
{
    cout << "EdbShowerP::PrintParametrisation_ExtraInfo"<< endl;
    if (!eExtraInfoIsDone[0]) cout << "EdbShowerP::PrintParametrisation_ExtraInfo()     PARA NOT YET BUILD ! "<< endl;
    cout << "EdbShowerP::PrintParametrisation_ExtraInfo()    ePara_ExtraInfo.InBT_E = " << ePara_ExtraInfo.InBT_Flag << endl;
    cout << "EdbShowerP::PrintParametrisation_ExtraInfo()   ePara_ExtraInfo.InBT_Flag= " << ePara_ExtraInfo.InBT_Flag << endl;
    cout << "EdbShowerP::PrintParametrisation_ExtraInfo()   ePara_ExtraInfo.InPairBT_E= " << ePara_ExtraInfo.InPairBT_E << endl;

    cout << "EdbShowerP::PrintParametrisation_ExtraInfo()   ePara_ExtraInfo.ShowerLength= " << ePara_ExtraInfo.ShowerLength << endl;
    cout << "EdbShowerP::PrintParametrisation_ExtraInfo()   ePara_ExtraInfo.InBT_IPToVtx= " << ePara_ExtraInfo.InBT_IPToVtx << endl;
    cout << "EdbShowerP::PrintParametrisation_ExtraInfo()   ePara_ExtraInfo.InBT_DeltaZToVtx= " << ePara_ExtraInfo.InBT_DeltaZToVtx << endl;
    return;
}
//______________________________________________________________________________


//______________________________________________________________________________
void EdbShowerP::PrintMCInfo_PGun()
{
    cout << "EdbShowerP::PrintMCInfo_PGun"<< endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.MCEvt= " << eMCInfo_PGun.MCEvt << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.PDGId= " << eMCInfo_PGun.PDGId << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.energy= " << eMCInfo_PGun.energy << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.tantheta= " << eMCInfo_PGun.tantheta << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.dirx= " << eMCInfo_PGun.dirx << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.diry= " << eMCInfo_PGun.diry << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.dirz= " << eMCInfo_PGun.dirz << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.TX= " << eMCInfo_PGun.TX << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.TY= " << eMCInfo_PGun.TY << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.X= " << eMCInfo_PGun.X << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.Y= " << eMCInfo_PGun.Y << endl;
    cout << "EdbShowerP::PrintMCInfo_PGun()   eMCInfo_PGun.Z= " << eMCInfo_PGun.Z << endl;
    return;
}
//______________________________________________________________________________




//______________________________________________________________________________
void EdbShowerP::PrintAll()
{
    cout << "EdbShowerP::PrintAll()" << endl;
    PrintMetaData();
    PrintBasics();
    PrintSegments();
    return;
}
//______________________________________________________________________________





//______________________________________________________________________________
void EdbShowerP::Help()
{
    cout << "---------------------------------------------------------" <<
         endl;
    cout << "EdbShowerP::Help()" << endl << endl;
    cout << "  What is this?" << endl;
    cout << "        An  EdbShowerP  is similar to an EdbTrackP is similar to an EdbSegP object." << endl;
    cout << "        Usually a shower is a collection of some BaseTracks. Good showers have many, bad ones have few ;-) " << endl;
    cout << "        A shower spreads over same plates, leaving a distinct signal in your viewer (EDA od EDD)." << endl << endl;
    cout << "  What can we do with this?" << endl;
    cout << "        Usually a shower is used to detect electron (rarely in OPERA) or photon interactions (more often)." << endl;
    cout << "        Then -if the shower is well reconstructed (see-> EdbShowRec object)- the EdbShowerP has particular" << endl;
    cout << "        characteristics from which we can conclude interesting values, like energy / particle Id and so on..." << endl << endl;
    cout << "   Technical details, information about the shower:" << endl;
    cout << "   ->N() :     returns number of segments (basetracks) in this shower." << endl;
    cout << "   ->Npl() :   returns number of plates, the shower passes." << endl;
    cout << "   ->N0() :    returns number of (connected) plates, in which there are no basetracks at all." << endl;
    cout << "   The shower profile:" << endl;
    cout << "     Two profiles exist: The Longitudinal and the Transversal Profile:" << endl;
    cout << "     The Longitudinal Profile:" << endl;
    cout << "     You can get the longitudinal profile with a ROOT histogram:" << endl;
    cout << "   ->GetHistogramProfileLongitudinal() . It uses Z postion values w.r.t the starting point of the shower." << endl;
    cout << "   ->GetHistogramProfileLongitudinalPlate() . It uses plate postion values w.r.t the starting plate of the shower." << endl;
    cout << "   ->GetHistogramProfileTransversal() . It uses Z postion values w.r.t the starting point of the shower." << endl;
    cout << "   The shower parametrisations:" << endl;
    cout << "   ->GetPara_..() Get the corresponding parametrisation structure:" << endl;
    cout << "   ->GetPara_..() Possible Options: _FJ _YC _XX _YY _PP _AS _SE  _ExtraInfo" << endl;
    cout << "   ->PrintPara_..() Print the corresponding parametrisation structure." << endl;
    cout << "   ->BuildPara_..() Build the corresponding parametrisation structure." << endl;
    cout << "   Technical details, manipulation of the shower:" << endl;
    cout << "   ->AddSegment(EdbSegP*)." << endl;
    cout << "   ->RemoveSegment(EdbSegP*)." << endl;
    cout << "   ->AddShower(EdbShowerP*)." << endl;
    cout << "   ->RemoveShower(EdbShowerP*)." << endl;
    cout << "   ->Sort() Sort Segments of the shower by ascending Z." << endl;
    cout << "   Technical details, Update of the shower:" << endl;
    cout << "   ->Update()  Partial update (no   rebuild of parametrisations)" << endl;
    cout << "   ->UpdateX() Full    update (with rebuild of parametrisations." << endl;
    cout << endl;
    cout << "---------------------------------------------------------" <<
         endl;
    return;
}

//______________________________________________________________________________








//______________________________________________________________________________
Double_t EdbShowerP::GetdR(EdbSegP* seg1,EdbSegP* seg2)
{
    return TMath::Sqrt((seg1->X()-seg2->X())*(seg1->X()-seg2->X())+(seg1->Y()-seg2->Y())*(seg1->Y()-seg2->Y()));
}
//______________________________________________________________________________
Double_t EdbShowerP:: GetdMin(EdbSegP* seg1,EdbSegP* seg2) {
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
Double_t EdbShowerP::GetdT(EdbSegP* seg1,EdbSegP* seg2)
{
    return TMath::Sqrt((seg1->TX()-seg2->TX())*(seg1->TX()-seg2->TX())+(seg1->TY()-seg2->TY())*(seg1->TY()-seg2->TY()));
}
//______________________________________________________________________________
Double_t EdbShowerP::CalcIP(EdbSegP* s, EdbVertex* v) {
    // calculate IP for the selected tracks wrt the given vertex.
    // if the vertex is not given, use the selected vertex.
    if (NULL==v) {
        printf("select a vertex!\n");
        return 0;
    }
    if (gEDBDEBUGLEVEL>3) printf(" /// Calc IP w.r.t. Vertex %d %8.1lf %8.1lf %8.1lf (red vertex) ///\n",v->ID(), v->X(),v->Y(),v->Z() );
    double r = CalcIP(s,v->X(),v->Y(),v->Z());
    return r;
}
//______________________________________________________________________________
Double_t EdbShowerP::CalcIP(EdbSegP *s, double x, double y, double z) {
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
//______________________________________________________________________________
Double_t EdbShowerP::CalcDistLongitudinal(EdbSegP* seg0, EdbSegP* segTest) {

    return 0;
}
//______________________________________________________________________________
Double_t EdbShowerP::CalcDistTransversal(EdbSegP* seg0, EdbSegP* segTest) {

    return 0;
}
//______________________________________________________________________________
Int_t EdbShowerP::GetLastFilledBin(TH1* hist)
{
    // return last filled X bin of a histogram (leave out under/overflow bins):
    Int_t lastbin=1;
    for (Int_t i=1; i<hist->GetNbinsX()-1; ++i) {
        if (hist->GetBinContent(i)>0) lastbin=i;
    }
    return lastbin;
}

//______________________________________________________________________________
TH1F* EdbShowerP::GetProfileLongitudinalPlate()
{

    // This function returns a histogram using filmnumbers
    // this is equivalent like "treebranch->Draw("nfilmb")..."
    //    EdbShowerP::PrintParametrisation_FJ()    ePara_FJ.longprofile=
    //   0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 ..... 52 53 54 55 56
    //   1  2  2  2  2  2  2  2 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 -1 ....-1 -1 -1 -1 -1 -1

    // Returns a Histogram, equivalent to  ePara_FJ.longprofile bin entries;
    TH1F* eProfileLongitudinalPlate = new TH1F("eProfileLongitudinalPlate","eProfileLongitudinalPlate",57,0,57);

    // Do not fill with -1 entries, use here instead 0s.
    for (int i=1; i<57; ++i) if ( ePara_FJ.longprofile[i-1]>=0) eProfileLongitudinalPlate->SetBinContent( i, ePara_FJ.longprofile[i-1] );

    return eProfileLongitudinalPlate;
}

//______________________________________________________________________________

ClassImp(EdbShowerP)
