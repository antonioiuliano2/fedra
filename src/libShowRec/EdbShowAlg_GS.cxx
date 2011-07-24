#include "EdbShowAlg.h"
#include "EdbShowAlg_GS.h"
#include "EdbShowerP.h"

using namespace std;

ClassImp(EdbShowAlg_GS)

//______________________________________________________________________________









//______________________________________________________________________________

EdbShowAlg_GS::EdbShowAlg_GS()
{
    // Default Constructor
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","EdbShowAlg_GS:: Default Constructor");

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

EdbShowAlg_GS::EdbShowAlg_GS(EdbPVRec* ali)
{
    // Default Constructor with EdbPVRec object
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS(EdbPVRec* ali)","EdbShowAlg_GS:: Default Constructor  with EdbPVRec object");
    // Default Constructor
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","EdbShowAlg_GS:: Default Constructor");

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

    this->SetEdbPVRec(ali);

}
//______________________________________________________________________________

EdbShowAlg_GS::~EdbShowAlg_GS()
{
    // Default Destructor
    Log(2,"EdbShowAlg_GS::~EdbShowAlg_GS","EdbShowAlg_GS:: Default Destructor");
}

//______________________________________________________________________________

void EdbShowAlg_GS::Init()
{
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Init()");

    //  Init with values according to GS Alg:
    //  Took over from "FindGamma.C" script I develoved before:
    // IP CUT; this cut is used for the -better- IP of both BTs to Vertex/BT
    eParaValue[0]=150;
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
    eParaValue[5]=1;
    eParaString[5]="PARA_GS_CUT_PIDDIFF";

    // in MC case: check for opposite flag sign
    eParaValue[6]=1;
    eParaString[6]="PARA_GS_CUT_OPPOSITEFLAG";

    if (!eRecoShowerArray) eRecoShowerArray= new TObjArray();
    eRecoShowerArrayN=0;

    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Init()...done.");
    return;
}

//______________________________________________________________________________


void EdbShowAlg_GS::Initialize()
{
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Initialize()");
    return;
}

//______________________________________________________________________________




void EdbShowAlg_GS::SetInVtx( EdbVertex* vtx )
{
    Log(2,"EdbShowAlg_GS::SetInVtx","SetInVtx()");
    if (!eInVtxArray)  eInVtxArray = new TObjArray();
    eInVtxArray->Add(vtx);
    ++eInVtxArrayN;
    return;
}



//______________________________________________________________________________


void EdbShowAlg_GS::Convert_InVtxArray_To_InBTArray()
{
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Convert_InVtxArray_To_InBTArray()");
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

    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Convert_InVtxArray_To_InBTArray()...start loop");
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
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_GS::Convert_InVtxArray_To_InBTArray   Converted " << eInBTArrayN << "InVtx to InBT." << endl;
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Convert_InVtxArray_To_InBTArray()...done.");
    return;
}

//______________________________________________________________________________


void EdbShowAlg_GS::Convert_TracksArray_To_InBTArray()
{
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Convert_TracksArray_To_InBTArray()");
    if (eInBTArray!=NULL) {
        eInBTArray->Clear();
        cout << " eInBTArray->Clear();" << endl;
    }
    if (eInBTArray==NULL) {
        eInBTArray = new TObjArray();
        cout << " eInBTArray = new TObjArray()" << endl;
    }

    TObjArray* tracks=eAli->eTracks;
    Int_t ntracks=tracks->GetEntries();
    EdbSegP*      sg=0;
    EdbTrackP* track=0;

    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Convert_TracksArray_To_InBTArray()...start loop");
    for (Int_t i=0; i<ntracks; i++) {
        track= (EdbTrackP*)tracks->At(i);
        sg= (EdbSegP*)track->GetSegmentFirst();
        //vtx->Print();
        EdbSegP* seg = new EdbSegP(i,sg->X(),sg->Y(),0,0);
        seg->SetZ(sg->Z());
        seg->SetTX(sg->TX());
        seg->SetTY(sg->TY());
        seg->SetMC(sg->MCEvt(),sg->MCEvt());
        seg->SetFlag(0);
        seg->PropagateTo(seg->Z()-1300); // propagate one plate downstream;
        //seg->Print();
        eInBTArray->Add(seg);
    }

    eInBTArrayN=eInBTArray->GetEntries();
    if (gEDBDEBUGLEVEL>1) cout << "EdbShowAlg_GS::Convert_TracksArray_To_InBTArray   Converted " << eInBTArrayN << " to InBT." << endl;
    Log(2,"EdbShowAlg_GS::EdbShowAlg_GS","Convert_TracksArray_To_InBTArray()...done.");
    return;
}


//______________________________________________________________________________


Bool_t EdbShowAlg_GS::CheckPairDuplications(Int_t SegPID,Int_t SegID,Int_t Seg2PID,Int_t Seg2ID,TArrayI* SegmentPIDArray,TArrayI* SegmentIDArray,TArrayI* Segment2PIDArray,TArrayI* Segment2IDArray, int RecoShowerArrayN)
{
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_GS::CheckPairDuplications for 	Pair (" << SegPID << "," << SegID<< ";"<< Seg2PID << "," << Seg2ID << ") in RecoShowerArrayN=" << RecoShowerArrayN << endl;

    for (Int_t i=0; i<RecoShowerArrayN; i++) {
        // PID and ID of Seg and Seg2 to be exchanged for duplications
        if (gEDBDEBUGLEVEL>3)  cout << "EdbShowAlg_GS::CheckPairDuplications in eRecoShowerArray: i="<< i<<"  (" << Segment2PIDArray->At(i) << "," << Segment2IDArray->At(i)<< ";"<< SegmentPIDArray->At(i) << "," << SegmentIDArray->At(i) << "). "<< endl;
        if ( SegPID==Segment2PIDArray->At(i) && Seg2PID==SegmentPIDArray->At(i) && SegID==Segment2IDArray->At(i) && Seg2ID==SegmentIDArray->At(i)) {
            if (gEDBDEBUGLEVEL>3) cout << "Found duplication for i="<< i<< ".. return true"<<endl;
            return kTRUE;
        }
    }
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_GS::CheckPairDuplications ... no duplication found."<<endl;
    return kFALSE;
}


//______________________________________________________________________________

void EdbShowAlg_GS::Execute()
{
    Log(2,"EdbShowAlg_GS::Execute","Execute()");
    cout << "EdbShowAlg_GS::Execute()" << endl;
    Log(2,"EdbShowAlg_GS::Execute","Execute()   DOING MAIN SHOWER RECONSTRUCTION HERE");

    if (!eAli) cout << "no eAli pointer!" << endl;

    if (!eInBTArray) {
        Log(2,"EdbShowAlg_GS::Execute","Execute()   No eInBTArray.... Check for eInVtxArray:");
        Log(2,"EdbShowAlg_GS::Execute","Execute()   Check if there is an Convert_InVtxArray_To_InBTArray():");
        // Check if there is an Convert_InVtxArray_To_InBTArray();
        if (!eInVtxArray) {
            Log(2,"EdbShowAlg_GS::Execute","Execute()   No eInVtxArray. RETURN.");

            cout << "Check if EdbPVrecObject has Tracks..." << endl;
            if (NULL==eAli->eTracks) cout << " EdbPVrecObject has no Tracks" << endl;
            cout << "... yes, seems so, so take the track starting segments, propagate their first segemnts one plate downstream" << endl;
            cout << "and use this as invtx array...." << endl;
            Convert_TracksArray_To_InBTArray();
            return;
        }
        cout << "eInVtxArray there. Converting to InBTArray() now. "<< endl;
        Convert_InVtxArray_To_InBTArray();
    }

    EdbSegP* InBT=NULL;
    Int_t     STEP=-1;
    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>1) cout << "EdbShowAlg_GS::Execute    STEP for patternloop direction =  " << STEP << endl;
    if (gEDBDEBUGLEVEL>1) cout << "EdbShowAlg_GS::Execute    eRecoShowerArrayN =  " << eRecoShowerArrayN << endl;

    //--- Loop over InBTs:
    if (gEDBDEBUGLEVEL>1) cout << "EdbShowAlg_GS::Execute    Loop over InBTs:" << endl;

    // Since eInBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    // InBT doest have to be necessary a real BaseTrack, it can also be a vertex (with its positions and angle zero) !!!
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {

        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) ;
        //     cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        cout << "EdbShowAlg_GS::Execute     ----------------------------------------------"<< endl;
        cout << "EdbShowAlg_GS::Execute     InBT in total, still to do: " << i << endl;

        // Get InitiatorBT from eInBTArray  InBT
        InBT=(EdbSegP*)eInBTArray->At(i);
        cout << "EdbShowAlg_GS::Execute     InBT->XYZ() " << InBT->X() << " " << InBT->Y() << " " << InBT->Z() << " " << endl;

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        // Transform (make size smaller, extract only events having same MC) the  eAli  object:
        Transform_eAli(InBT,999999);
        if (gEDBDEBUGLEVEL>2)  eAli_Sub->Print();

        //-----------------------------------
        // 2) Find Pairs
        //-----------------------------------
        TObjArray* Pairs = FindPairs(InBT,eAli_Sub);
        cout << "EdbShowAlg_GS::Execute     TObjArray* Pairs = FindPairs(InBT,eAli_Sub); done." << endl;
        cout << "EdbShowAlg_GS::Execute     Found Pair Entries= " << Pairs->GetEntries() << ". Do clean of pairings now." << endl;


        //-----------------------------------
        // 2) Add Clean Pairs to eRecoShowerArray
        //-----------------------------------
        TObjArray* CleanPairs = CheckCleanPairs( InBT, Pairs);
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlg_GS::Execute     CleanPairs->Print(): "<< endl;
        if (gEDBDEBUGLEVEL>2) CleanPairs->Print();
        for (int j=0; j<CleanPairs->GetEntries(); ++j) {
            //cout << "j= " << j << endl;
            EdbShowerP* pair=(EdbShowerP*)CleanPairs->At(j);
            eRecoShowerArray->Add(pair);
            ++eRecoShowerArrayN;
        }

        cout << "EdbShowAlg_GS::Execute     eRecoShowerArrayN now: " << eRecoShowerArrayN << endl;
        cout << "EdbShowAlg_GS::Execute     ----------------------------------------------"<< endl;
    } // Loop over InBT


    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbShowAlg_GS::Execute      Print Loop for N shower pairsegments: = " << eRecoShowerArrayN << endl;
        for (Int_t i=0; i <eRecoShowerArray->GetEntries();  i++) {
            EdbShowerP* sh = (EdbShowerP* )eRecoShowerArray->At(i);
            sh->Print();
            sh->PrintSegments();
        }
        cout << "EdbShowAlg_GS::Execute      Print Loop for N shower pairsegments ..... done."<< endl;
    }

    cout << "EdbShowAlg_GS::Execute   Found eRecoShowerArrayN Gamma Pairing Candidates: = " << eRecoShowerArrayN << endl;

    Log(2,"EdbShowAlg_GS::Execute","Execute()...done.");
    return;
}

//______________________________________________________________________________




TObjArray* EdbShowAlg_GS::FindPairs(EdbSegP* InitiatorBT, EdbPVRec* eAli_Sub)
{
    Log(3,"EdbShowAlg_GS::FindPairs","FindPairs()");

    TObjArray* RecoShowerArray= new TObjArray(99);
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
    Int_t		IP_Pair_To_InBT_SegSmaller=0;

    EdbSegP* InBT=NULL;
    if (NULL==InitiatorBT) {
        InBT= new EdbSegP();
        EdbPattern* pat = eAli_Sub->GetPatternZLowestHighest(1);
        InBT->SetX(pat->X());
        InBT->SetY(pat->Y());
        InBT->SetZ(pat->Z());
        InBT->SetTX(0);
        InBT->SetTY(0);
        InBT->SetMC(-999,-999);
        cout << "WARNING   EdbShowAlg_GS::FindPairs   InBT==NULL. Create a dummy InBT:" << endl;
        InBT->PrintNice();
    }
    else {
        InBT=InitiatorBT;
    }


// 	from ShowRec.cpp:
// 	       CUT_PARAMETER[0]=cut_gs_cut_dip;
//         CUT_PARAMETER[1]=cut_gs_cut_dmin;
//         CUT_PARAMETER[2]=cut_gs_cut_dr;
//         CUT_PARAMETER[3]=cut_gs_cut_dz;
//         CUT_PARAMETER[4]=cut_gs_cut_dtheta;
//         CUT_PARAMETER[5]=cut_gs_cut_piddiff;
//         CUT_PARAMETER[6]=cut_gs_cut_oppositeflag;


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

    Int_t npat=eAli_Sub->Npatterns();
    Int_t pat_one_bt_cnt_max,pat_two_bt_cnt_max=0;
    EdbPattern* pat_one=0;
    EdbPattern* pat_two=0;

    for (Int_t pat_one_cnt=0; pat_one_cnt<npat; ++pat_one_cnt) {
        pat_one=(EdbPattern*)eAli_Sub->GetPattern(pat_one_cnt);
        pat_one_bt_cnt_max=eAli_Sub->GetPattern(pat_one_cnt)->GetN();

        // Check if dist Z to vtx (BT) is ok:
        distZ=pat_one->Z()-InBT->Z();
        if (gEDBDEBUGLEVEL>4) {
            cout << "EdbShowerAlg_GS::FindPairs  Check if dist Z to vtx (BT) is ok:  distZ=" << distZ << endl;
        }
        if (distZ>eParaValue[3]) continue;
        if (distZ<0) continue;

        if (gEDBDEBUGLEVEL>2) cout << "Searching patterns: pat_one_cnt=" << pat_one_cnt << "  pat_one->Z() = " << pat_one->Z() << " pat_one_bt_cnt_max= "<< pat_one_bt_cnt_max <<endl;


        for (Int_t pat_two_cnt=0; pat_two_cnt<npat; ++pat_two_cnt) {

            // Now apply cut conditions: GS  GAMMA SEARCH Alg:
            if (TMath::Abs(pat_one_cnt-pat_two_cnt)>eParaValue[5]) continue;

            pat_two=(EdbPattern*)eAli_Sub->GetPattern(pat_two_cnt);
            pat_two_bt_cnt_max=eAli_Sub->GetPattern(pat_two_cnt)->GetN();


            if (gEDBDEBUGLEVEL>2) cout << "	Searching patterns: pat_two_cnt=" << pat_two_cnt << "  pat_two->Z() = " << pat_two->Z() << " pat_two_bt_cnt_max= "<< pat_two_bt_cnt_max <<endl;

            for (Int_t pat_one_bt_cnt=0; pat_one_bt_cnt<pat_one_bt_cnt_max; ++pat_one_bt_cnt) {
                Segment =  (EdbSegP*)pat_one->GetSegment(pat_one_bt_cnt);

                Float_t IP_Pair_To_InBT_Seg	=CalcIP(Segment, InBT->X(),InBT->Y(),InBT->Z());
                // Check if IP to vtx (BT) is ok:
                if (IP_Pair_To_InBT>eParaValue[0]) continue;


                for (Int_t pat_two_bt_cnt=0; pat_two_bt_cnt<pat_two_bt_cnt_max; ++pat_two_bt_cnt) {

                    Segment2 = (EdbSegP*)pat_two->GetSegment(pat_two_bt_cnt);
                    if (Segment2==Segment) continue;
                    if (Segment2->ID()==Segment->ID()&&Segment2->PID()==Segment->PID()) continue;

                    // At first:  Check for already duplicated pairings:
                    if (CheckPairDuplications(Segment->PID(),Segment->ID(),Segment2->PID(),Segment2->ID(), SegmentPIDArray,SegmentIDArray,Segment2PIDArray,Segment2IDArray, RecoShowerArrayN)) continue;

                    // Now apply cut conditions: GS  GAMMA SEARCH Alg  --------------------
                    // if InBT is flagged as MC InBT, take care that only BG or same MC basetracks are taken:
                    if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment->MCEvt()!=Segment2->MCEvt()) continue;
                    if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment->MCEvt()!=InBT->MCEvt()) continue;
                    if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment2->MCEvt()!=InBT->MCEvt()) continue;

                    // In case of two MC events, check for e+ e- pairs
                    // Do this ONLY IF parameter eParaValue[6] is set to choose different Flag() pairs:
                    if (InBT->MCEvt()>0 && eParaValue[6]==1) {
                        if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) {
                            if ((Segment2->Flag()+Segment->Flag())!=0) continue;
                        }
                    }

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


                    // Check if IP to vtx (BT) is ok:
                    Float_t IP_Pair_To_InBT_Seg2	=CalcIP(Segment2, InBT->X(),InBT->Y(),InBT->Z());
                    if (IP_Pair_To_InBT_Seg2>eParaValue[0]) continue;



                    Float_t IP_Pair_To_InBT_SegSum=CalcIP(Segment_Sum, InBT->X(),InBT->Y(),InBT->Z());
                    Float_t IP_Pair_To_InBT_SegSmaller=0;

                    cout << "IP_Pair_To_InBT_Seg   = " << IP_Pair_To_InBT_Seg << endl;
                    cout << "IP_Pair_To_InBT_Seg2  = " << IP_Pair_To_InBT_Seg2 << endl;
                    cout << "IP_Pair_To_InBT_SegSum= " << IP_Pair_To_InBT_SegSum << endl;

                    // Save the segment which has smaller IP, this will be the first BT in the RecoShower
                    if ( IP_Pair_To_InBT_Seg>IP_Pair_To_InBT_Seg2 ) {
                        IP_Pair_To_InBT_SegSmaller=0; // take Segment first
                    }
                    else {
                        IP_Pair_To_InBT_SegSmaller=1; // take Segment2 first
                    }



                    // f) Check if this is not a possible fake doublet which is
                    //	  sometimes caused by view overlap in the scanning:
                    //    in the EdbPVRQuality class this will be done at start for the whole
                    //    PVR object so this will be later on obsolete.
                    if (IsPossibleFakeDoublet(Segment,Segment2) ) continue;
                    //
                    // end of    cut conditions: GS  GAMMA SEARCH Alg  --------------------
                    //


                    // Chi2 Variable:
                    Float_t dminDist=GetMinimumDist(Segment,Segment2);
                    Float_t dtheta=DeltaThetaSingleAngles(Segment,Segment2);
                    // preliminary mean and sigma values (taken from 1Gev, with highpur cutset), to be checked if they are the same for 0.5,1,2,4 GeV Photons
                    Float_t GammaChi2 =  ((IP_Pair_To_InBT-80)*(IP_Pair_To_InBT-80)/60/60)+((dminDist-3.5)*(dminDist-3.5)/4.7/4.7)+((dtheta-0.021)*(dtheta-0.021)/0.012/0.012);
                    GammaChi2=GammaChi2/3.0;


                    if (gEDBDEBUGLEVEL>2) {
                        cout << "EdbShowAlg_GS::FindPairs	Pair (" << Segment->PID() << "," << Segment->ID()<< ";"<< Segment2->PID() << "," << Segment2->ID() << ") has passed all cuts w.r.t to InBT:" << endl;
                        cout << "EdbShowAlg_GS::FindPairs	IP_Pair_To_InBT  = " << IP_Pair_To_InBT << endl;
                        cout << "EdbShowAlg_GS::FindPairs	(IP_Pair_To_InBT==0 can mean that there was no InVtxArray and Vtx has been extrapolated from tracks.)" << endl;
                        cout << "EdbShowAlg_GS::FindPairs	GetMinimumDist(Segment,Segment2)  = " << GetMinimumDist(Segment,Segment2) << endl;
                        cout << "EdbShowAlg_GS::FindPairs	CalcIP(BetterSegment,InBT)  = " << IP_Pair_To_InBT << endl;
                        cout << "EdbShowAlg_GS::FindPairs	CalcIP(Segment_Sum,InBT)  = " << IP_Pair_To_InBT_SegSum << endl;
                        cout << "EdbShowAlg_GS::FindPairs	GetSpatialDist(Segment_Sum,InBT)  = " << GetSpatialDist(Segment_Sum,InBT) << endl;
                        cout << "EdbShowAlg_GS::FindPairs	GammaChi2 = " << GammaChi2 << endl;
                    }


                    // Add IDs and PIDs for storage
                    SegmentPIDArray->AddAt(Segment->PID(),RecoShowerArrayN);
                    SegmentIDArray->AddAt(Segment->ID(),RecoShowerArrayN);
                    Segment2PIDArray->AddAt(Segment2->PID(),RecoShowerArrayN);
                    Segment2IDArray->AddAt(Segment2->ID(),RecoShowerArrayN);

                    // Create new EdbShowerP Object for storage;
                    EdbShowerP* RecoShower = new EdbShowerP();
                    if (IP_Pair_To_InBT_SegSmaller==0) {
                        RecoShower -> AddSegment(Segment);
                        RecoShower -> AddSegment(Segment2);
                    }
                    else {
                        RecoShower -> AddSegment(Segment2);
                        RecoShower -> AddSegment(Segment);
                    }
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

                    if (gEDBDEBUGLEVEL>2)  cout <<"------------"<< endl;


                    // Add Shower to interim  Array:
                    RecoShowerArray->Add(RecoShower);
                    ++RecoShowerArrayN;

                } //for (Int_t pat_two_bt_cnt=0; ...
            } //for (Int_t pat_one_bt_cnt=0; ...
        } //for (Int_t pat_two_cnt=0; ...
    }  //for (Int_t pat_one_cnt=0; ...


    cout << "EdbShowAlg_GS::FindPairs    For the InBT/Vtx at __" << InBT << "__, we have found "  <<  RecoShowerArray->GetEntries()  << " compatible pairs in the PVRec volume." << endl;
    // cout << "RecoShowerArrayN= " << RecoShowerArrayN << endl;
    // cout << "RecoShowerArray->GetEntries()= " << RecoShowerArray->GetEntries() << endl;
    // cout << "eRecoShowerArray->GetEntries()= " << eRecoShowerArray->GetEntries() << endl;

    // Delete unnecessary objects:
    // important, else memory overflow!!
    delete SegmentPIDArray;
    delete Segment2PIDArray;
    delete SegmentIDArray;
    delete Segment2IDArray;

    Log(3,"EdbShowAlg_GS::FindPairs","FindPairs()....done.");
    return RecoShowerArray;
}

//______________________________________________________________________________


TObjArray* EdbShowAlg_GS::CheckCleanPairs(EdbSegP* InBT, TObjArray* RecoShowerArray)
{

    cout << "EdbShowAlg_GS::CheckCleanPairs"  <<endl;
    if (NULL==RecoShowerArray) return NULL;

    TObjArray* NewRecoShowerArray= new TObjArray(999);
    int NewRecoShowerArrayN=0;

    EdbShowerP* TrackPair1=NULL;
    EdbShowerP* TrackPair2=NULL;

    int ntrack=RecoShowerArray->GetEntriesFast();
    cout << ntrack << endl;

    for (Int_t pat_one_cnt=0; pat_one_cnt<ntrack; ++pat_one_cnt) {
        // (if -1) then the last one is not checked
        TrackPair1=(EdbShowerP*)RecoShowerArray->At(pat_one_cnt);
        bool taketrack1=true;
        for (Int_t pat_two_cnt=pat_one_cnt; pat_two_cnt<ntrack; ++pat_two_cnt) {
            //if only one track at all take it anyway:
            if (ntrack==1) continue;
            TrackPair2=(EdbShowerP*)RecoShowerArray->At(pat_two_cnt);
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

        if (!taketrack1) continue;

        // Add TrackPair1
        NewRecoShowerArray->Add(TrackPair1);
        ++NewRecoShowerArrayN;


        // Print
        if (TrackPair1->N()<2) cout << "This Track has only ONE entry" << endl;

        EdbSegP* s1=(EdbSegP* )TrackPair1->GetSegment(0);
        EdbSegP* s2=(EdbSegP* )TrackPair1->GetSegment(1);
        if (gEDBDEBUGLEVEL>2) {
            s1->PrintNice();
            s2->PrintNice();
            cout <<  s1->MCEvt() << "  " <<  s2->MCEvt() << "  " << s1->Flag() << "  " << s2->Flag() << "  " << s1->P() << "  " << s2->P() << "  " <<  "  " << s1->Z() << "  " << s2->Z() << "  " <<endl;
            cout<< "--------"<<endl;
        }
    }
    cout << " From " <<ntrack << "  originally, there are now after Zposition and overlap cuts: " << NewRecoShowerArrayN << " left." << endl;

    return NewRecoShowerArray;
}





//______________________________________________________________________________

void EdbShowAlg_GS::Finalize()
{
    // do nothing yet.
}
//______________________________________________________________________________


double EdbShowAlg_GS::CalcIP(EdbSegP *s, double x, double y, double z) {
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

double EdbShowAlg_GS::CalcIP(EdbSegP *s, EdbVertex *v) {
    // calculate IP between a given segment and a given vertex.
    // return the IP value.
    // this is used for IP cut.

    // if vertex is not given, use the selected vertex.
    // if(v==NULL) v=gEDA->GetSelectedVertex();

    if (v==NULL) return -1.;
    return CalcIP(s, v->X(), v->Y(), v->Z());
}

//______________________________________________________________________________

Bool_t EdbShowAlg_GS::IsPossibleFakeDoublet(EdbSegP* s1,EdbSegP* s2) {
    if (TMath::Abs(s1->X()-s2->X())<1) return kTRUE;  // minimum distance of 1micron
    if (TMath::Abs(s1->Y()-s2->Y())<1) return kTRUE;// minimum distance of 1micron
    if (TMath::Abs(s1->TX()-s2->TX())<0.005) return kTRUE;// minimum angle of 5mrad
    if (TMath::Abs(s1->TY()-s2->TY())<0.005) return kTRUE;// minimum angle of 5mrad
    return kFALSE;
}