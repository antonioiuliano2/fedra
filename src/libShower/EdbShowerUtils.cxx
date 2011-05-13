#include "EdbShowerUtils.h"

//_____________________________________________________________________________________________
void EdbShowerRec::PrintRecoShowerArray()
{
    // Print the array for the Reconstructed Showers.
    Log(2,"EdbShowerRec::PrintRecoShowerArray","EdbShowerRec::PrintRecoShowerArray");
    EdbTrackP* show=0;
    for (int i=0; i<GetRecoShowerArrayN(); ++i) {
        show=(EdbTrackP*)eRecoShowerArray->At(i);
        show->PrintNice();
    }
    cout << "Printed all " <<  GetRecoShowerArrayN()  << " showers of the eRecoShowerArray" << endl;
    return;
}


//_____________________________________________________________________________________________

Double_t EdbShowerRec::DeltaThetaComponentwise(EdbSegP* s1,EdbSegP* s2)
{
    // Calculate Delta Theta of two segments.
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

//_____________________________________________________________________________________________

Double_t EdbShowerRec::DeltaR_WithPropagation(EdbSegP* s,EdbSegP* stest)
{
    // Calculate Delta R of two segments.
    // If they have different Z positions, then first segment in the argument of the bracket
    // is propagated to the position of the second segment.
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

Double_t EdbShowerRec::DeltaR_WithoutPropagation(EdbSegP* s,EdbSegP* stest)
{
  // Calculate Delta R of two segments using no z propagation.
    return TMath::Sqrt((s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()));
}

//_____________________________________________________________________________________________

Bool_t EdbShowerRec::IsInConeTube(EdbSegP* TestingSegment, EdbSegP* StartingSegment, Double_t CylinderRadius, Double_t ConeAngle)
{
    // General Function which returns kTRUE if the Testing BaeTrack is in a cone defined
    // by the StartingBaseTrack. In case of same starting Z position, a max distance cut ( in Delta R) 
    // of 20microns is assumed (i.e. the cone is not totally closed at the start).
    // In case of TestingSegment==StartingSegment this function should also correctly return kTRUE.
    
    if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube("<<TestingSegment<< ","<< StartingSegment <<","<< CylinderRadius << "," << ConeAngle << endl;
    if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube() Test Segment " << TestingSegment << " vs. Starting Segment " << StartingSegment << endl;

    if (TestingSegment->Z()-StartingSegment->Z()<2.0) return kFALSE;

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
    if (TMath::Abs(StartingSegment->Z()-TestingSegment->Z())<2.0) {
        if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube()   Same Z position of TestingSegment and StartingSegment, Set angle artificially to zero" << endl;
        angle=0.0;

        // Check for position distance for 20microns if
        // Testing Segment is in same Z as StartingSegment
        if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube()   Check for position distance for 20microns if Testing Segment is in same Z as StartingSegment" << endl;
        if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube()   DeltaR_WithoutPropagation(StartingSegment,TestingSegment) = "<< DeltaR_WithoutPropagation(StartingSegment,TestingSegment) << endl;
        if (DeltaR_WithoutPropagation(StartingSegment,TestingSegment)<20) return kTRUE;
        if (DeltaR_WithoutPropagation(StartingSegment,TestingSegment)>=20) return kFALSE;
    }


    /// Outside if angle greater than ConeAngle (to be fulfilled for Cone and Tube in both cases)
    if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube()   Check if AngleVector now within the ConeAngleVector (<"<< ConeAngle<<"): " <<   angle << endl;
    if (angle>ConeAngle) {
        return kFALSE;
    }


    /// if angle smaller than ConeAngle, then you can differ between Tuberadius and CylinderRadius
    Double_t TubeDistance = 1.0/direction_x1_norm  *  ( (x2-x1).Cross(direction_x1) ).Mag();

    if (gEDBDEBUGLEVEL>3) cout << "Bool_t EdbShowerAlg::IsInConeTube()   Check if TestingSegment is now within the Tube (<"<< CylinderRadius<<"): " <<   TubeDistance << endl;

    if (TubeDistance>CylinderRadius) {
        return kFALSE;
    }

    return kTRUE;
}


//_____________________________________________________________________________________________


void EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()
{
    Log(2,"EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks","EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks.");

    // -----------------
    // Check if file  eFilename_LinkedTracks  exits and if it has tracks inside....
    // -----------------  tODO
    cout << "EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()   TODO::CHECK IF " << eFilename_LinkedTracks   <<" exists and if it has tracks inside!!!!" << endl;
    /// -----------------

    EdbSegP* segment=0;
    EdbSegP*    s2=0;
    EdbTrackP*  t=0;

    TFile * fil = new TFile(eFilename_LinkedTracks);
    TTree* tr= (TTree*)fil->Get("tracks");
    int nentr = 0; 
    nentr =int(tr->GetEntries());

    //   check if tracks has entries: if so then ok, otherwise return directly:
    if (nentr>0) {
        ;
    }
    else {
        cout << "EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks()   tracks file has  NO  entries...Return." << endl;
        return;
    }

    TClonesArray *seg= new TClonesArray("EdbSegP",60);
    
    //   check if   eInBTArray  was created, if not do it now.:
    if (!eInBTArray) eInBTArray=new TObjArray(nentr);

    int nseg,n0,npl;
    cout << nentr << "  entries Total"<<endl;
    tr->SetBranchAddress("t."  , &t  );
    tr->SetBranchAddress("s"  , &seg  );
    tr->SetBranchAddress("nseg"  , &nseg  );
    tr->SetBranchAddress("n0"  , &n0  );
    tr->SetBranchAddress("npl"  , &npl  );

    for (int i=0; i<nentr; i++ ) {
        tr->GetEntry(i);

        // Take first BT of the tracks:
        s2=(EdbSegP*) seg->At(0);
        // Clone it (otherwise it will be overtaken by the next one...)
        segment=(EdbSegP*)s2->Clone();
        // Add it to array
        eInBTArray->Add(segment);
    }
    delete tr;
    delete fil;

    // Update Entry numbers.
    SetInBTArrayN(eInBTArray->GetEntries());

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks   Printing InBTs" << endl;
    EdbSegP* testseg=0;
    for (int i=0; i<eInBTArrayN; i++ ) {
        if (gEDBDEBUGLEVEL>2) {
            testseg=(EdbSegP*)eInBTArray->At(i);
            testseg->PrintNice();
        }
    }

    eInBTArrayIsFilled=kTRUE;

    Log(2,"EdbShowerRec::Fill_eInBTArray_ByBaseTracksOf_eAli","EdbShowerRec::Fill_eInBTArray_ByBaseTracksOf_eAli.   After TCut: InBT N=%d", eInBTArrayN );
    Log(2,"EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks","EdbShowerRec::Fill_eInBTArray_ByLinkTracks_eFilename_LinkedTracks...done.");
    return;
}



//_____________________________________________________________________________________________



void EdbShowerRec::LoadEdbPVRec()
{
    // Function to load the EdbPatterVolumeReconstruction object into memory.
    // Shower reco needs to have full basetrack data to improve performance.
    // Because full basetrack data (corresponds to cp.root files) is memory extensive,
    // standard operations rely on linked tracks only.
    // Warning: some hardcoded strings are inside herebject for analysis use!
  
    Log(2,"EdbShowerRec::LoadEdbPVRec","Loads the full cp Basetracks into the  eAli  object.");
    // Loads the full cp Basetracks into the  eAli  object:
    if (!eAli) eAli=new EdbPVRec();

    // **** Data set initialization
    EdbDataProc* dset = new EdbDataProc("lnk.def");
    // **** Volume initialization
    dset->InitVolume(0);
    // **** Get Pattern Volume Reconstruction object
    eAli = dset->PVR();

    if (gEDBDEBUGLEVEL>2) eAli->Print();

    // Set private variables for the EdbPVrec Object now:
    eAliNpat=eAli->Npatterns();
    eAliNpatM1=eAliNpat-1;
    eNumberPlate_eAliPID=eAliNpatM1;

    eAliLoaded=kTRUE;
    Log(2,"EdbShowerRec::LoadEdbPVRec","Loads the full cp Basetracks into the  eAli  object.Done.");
}



//_____________________________________________________________________________________________


void EdbShowerRec::Transform_eAli( EdbSegP* InitiatorBT, Float_t ExtractSize=1000)
{
    //    Transform an EdbPVRec Object inot a smaller EdbPVRec object.
    //    The length (number of plates/patterns) of eAli_Sub is not changed.
    //    Only XY-size (and MC) cuts are applied.
    // 	  Increases speed of processing.
    
    Log(3,"EdbShowerRec::Transform_eAli","Transform eAli to eAli_Sub:");

    eUseAliSub=kTRUE;
    eAli_Sub_halfsize=ExtractSize;

    // IF TO RECREATE THE gALI_SUB in RECONSTRUCTION  or if to use gAli global (slowlier but maybe memory leak safe).
    if (eUseAliSub==0)
    {
        if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Transform_eAli   UseAliSub==0 No new eAli_Sub created. Use eAli instead. "<<endl;
        eAli_Sub=eAli;
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
    eAli_Sub = new EdbPVRec();

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Transform_eAli   eAli_Sub  created."<<endl;

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

    //InitiatorBT->PrintNice();

    // Add the subpatterns in a loop for the plates:
    // in reverse ordering.due to donwstream behaviour (!):
    // (Only downstream is supported now...)
    for (Int_t ii=0; ii<npat; ++ii) {

        Float_t zpos=eAli->GetPattern(ii)->Z();
        //cout << " Doing pattern " << ii << " at Z postion = "  << zpos << endl;

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


        //cout << "singlePattern: N segments filled:  " << singlePattern->N() << endl;
    }

    delete ExtrapolateInitiatorBT;
    eAli_SubNpat=eAli_Sub->Npatterns();  //number of plates

    if (gEDBDEBUGLEVEL>2) eAli_Sub->Print();
    return;
}



//_____________________________________________________________________________________________


void EdbShowerRec::Execute()
{
    cout << "EdbShowerRec::Execute()" << endl;
    cout << "EdbShowerRec::Execute   DOING MAIN SHOWER RECONSTRUCTION HERE" << endl;

    // Check if the eRecoShowerArray exists, if not create it;
    if (!eRecoShowerArray) eRecoShowerArray=new TObjArray();
    cout << "eRecoShowerArray->GetEntries() " << eRecoShowerArray->GetEntries()  << endl;

    EdbSegP* InBT;
    EdbSegP* Segment;
    EdbTrackP* RecoShower;

    Bool_t    StillToLoop=kTRUE;
    Int_t     ActualPID;
    Int_t     newActualPID;
    Int_t     STEP=-1;
    Int_t     NLoopedPattern=0;

    Int_t     eLastPlate_eAliPID=0;
    Int_t     eFirstPlate_eAliPID=0;
    Float_t   eLastPlate_eAliPIDZ=0;
    Float_t   eFirstPlate_eAliPIDZ=0;

    // Already defined as private variable:
    eAliNpatM1=eAliNpat-1;
    eNumberPlate_eAliPID=eAliNpatM1;


    if  (eAli->GetPattern(0)->Z() > eAli->GetPattern(eAliNpatM1)->Z()) {
        eLastPlate_eAliPID=eAli->GetPattern(0)->PID();
        eFirstPlate_eAliPID=eAli->GetPattern(eAliNpatM1)->PID();
        eLastPlate_eAliPIDZ=eAli->GetPattern(0)->Z();
        eFirstPlate_eAliPIDZ=eAli->GetPattern(eAliNpatM1)->Z();
    }
    else {
        eLastPlate_eAliPID=eAli->GetPattern(eAliNpatM1)->PID();
        eFirstPlate_eAliPID=eAli->GetPattern(0)->PID();
        eLastPlate_eAliPIDZ=eAli->GetPattern(eAliNpatM1)->Z();
        eFirstPlate_eAliPIDZ=eAli->GetPattern(0)->Z();
    }

    if (gEDBDEBUGLEVEL>2) {
        cout << "eAli->GetPattern(0)->Z() > eAli->GetPattern(eAliNpatM1)->Z() " << eAli->GetPattern(0)->Z()<< " " << eAli->GetPattern(eAliNpatM1)->Z() << endl;
        cout << "  eLastPlate_eAliPID  = " << eLastPlate_eAliPID << endl;
        cout << "  eFirstPlate_eAliPID  = " << eFirstPlate_eAliPID << endl;
        cout << "  eLastPlate_eAliPIDZ  = " << eLastPlate_eAliPIDZ << endl;
        cout << "  eFirstPlate_eAliPIDZ  = " << eFirstPlate_eAliPIDZ << endl;
        cout << endl;
    }


    if (eFirstPlate_eAliPID-eLastPlate_eAliPID<0) STEP=1;
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Execute--- STEP for patternloop direction =  " << STEP << endl;

    //--- Loop over InBTs:
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Execute    Loop over InBTs:" << endl;



    // Since eInBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=eInBTArrayN-1; i>=0; --i) {
//  for (Int_t i=eInBTArrayN-1; i==eInBTArrayN-1; --i) {

        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << eInBTArrayN <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        if (gEDBDEBUGLEVEL==3) cout << eInBTArrayN <<" InBT in total, still to do:"<< i << endl;

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------

        // Create new Shower Object for storage;
        // For the moment this is treated as an "EdbTrackP" object...
        RecoShower = new EdbTrackP();

        // Get InitiatorBT from eInBTArray
        InBT=(EdbSegP*)eInBTArray->At(i);
        if (gEDBDEBUGLEVEL>2) {
            cout << "EdbShowerRec::Execute() Start reconstruction for Initiator BaseTrack:" <<  endl;
            InBT->PrintNice();
        }

        // Add InBT to RecoShower:
        // This has to be done, since by definition the first BT in the RecoShower is the InBT.
        // Otherwise, also the definition of shower axis and transversal profiles is wrong!
        RecoShower -> AddSegment(InBT);
	// Attention: If you have a EdbTrackP object and you add a segment, then
	// you have to manually set Counters for N(), NPl(). It is not done auto.
	RecoShower -> SetCounters();
        //cout << "Segment  (InBT) " << Segment << " was added to RecoShower." <<  endl;

        // Transform (make size smaller, extract only events having same MC) the  eAli  object:
        Transform_eAli(InBT);
        if (gEDBDEBUGLEVEL>3) eAli_Sub->Print();
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        // Get greatest Z Value:
        Double_t greatestZValueofeAliSub=TMath::Max(eAli_Sub->GetPattern(0)->Z(),eAli_Sub->GetPattern(eAli_Sub->Npatterns()-1)->Z());
        cout << "EdbShowerRec::Execute--- --- greatest Z value of eAliSub= " << greatestZValueofeAliSub;


        //-----------------------------------
        // 2) Loop over (whole) eAli, check BT for Cuts
        // eAli_Sub
        //-----------------------------------
        ActualPID= InBT->PID() ;
        newActualPID= InBT->PID() ;

        while (StillToLoop) {
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowerRec::Execute--- --- Doing patterloop " << ActualPID << " for patterns Z position=" << eAli_Sub->GetPattern(ActualPID)->Z() << endl;

            if (eAli_Sub->GetPattern(ActualPID)->Z() < InBT->Z() ) continue;

            //cout << "If  eAli_Sub->GetPattern(ActualPID)->Z()  is less than InBT->Z() we directly go on..";
            //cout << "Doing plate Z = "  << eAli_Sub->GetPattern(ActualPID)->Z() << " w.r.t.   InBT->Z() " <<  InBT->Z() << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<eAli_Sub->GetPattern(ActualPID)->GetN(); ++btloop_cnt) {

                //cout << "Checking Segment " << btloop_cnt << endl;

                Segment = (EdbSegP*)eAli_Sub->GetPattern(ActualPID)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>4) Segment->PrintNice();


                // Now apply cut conditions: Cut for Quality: --------------------
                if ( (eUseQualityPar==kTRUE) && (Segment->Chi2()>Segment->W()*eQualityPar[0]-eQualityPar[1]) ) {
                    //cout << "Checking Segment " << btloop_cnt << "   Segment->Chi2(): " << Segment->Chi2() << "Segment->W(): " << Segment->W() << "   Wcut:  " <<   Segment->W()*eQualityPar[0]-eQualityPar[1] << endl;
                    continue;
                }


                // Now apply cut conditions: OI OfficialImplementation Alg  --------------------
                if ( !IsInConeTube(Segment, InBT, eAlgoParameterConeRadius , eAlgoParameterConeAngle) ) continue;
                //cout << "Segment " << Segment << " passed IsInConeTube"<<endl;
                if ( !FindPrecedingBTs(Segment, InBT, eAli_Sub, RecoShower)) continue;
                if (gEDBDEBUGLEVEL>4) cout << "Segment " << Segment << " passed FindPrecedingBTs"<<endl;
                // end of    cut conditions: OI OfficialImplementation Alg  --------------------

                // If we arrive here, Basetrack  Segment  has passed criteria
                // and is then added to the RecoShower:
                // Check if its not the InBT which is already added:
                if (Segment->X()==InBT->X()&&Segment->Y()==InBT->Y()) {
                    ;    // is InBT, do nothing;
                }
                else {
                    RecoShower -> AddSegment(Segment);
		    	// Attention: If you have a EdbTrackP object and you add a segment, then
			// you have to manually set Counters for N(), NPl(). It is not done auto.
			RecoShower -> SetCounters();
                }
                if (gEDBDEBUGLEVEL>4) cout << "Segment  " << Segment << " was added at  &Segment : " << &Segment  <<  endl;

            } // of btloop_cnt

            if (gEDBDEBUGLEVEL>3) cout << "EdbShowerRec::Execute--- --- ActualPID= " << newActualPID << "  done. Reconstructed shower has up to now: " << RecoShower->N()  << " Segments." << endl;

            // Calc BT density around shower:
            EdbPattern* pat_interim=eAli_Sub->GetPattern(ActualPID);
            CalcTrackDensity(pat_interim,eAli_Sub_halfsize,npat_int,npat_total,npatN);

            //------------
            newActualPID=ActualPID+STEP;
            ++NLoopedPattern;

            // DAU user solution for stopping the loop:
            if (InBT->Z()>=greatestZValueofeAliSub) {
                StillToLoop=kFALSE;
                cout << "EdbShowerRec::Execute--- ---Stopp Loop since: InBT->Z()>=greatestZValueofeAliSub"<<endl;
            }



            if (gEDBDEBUGLEVEL>3) cout << "EdbShowerRec::Execute--- --- StillToLoop= " << StillToLoop << endl;

            // This if holds in the case of STEP== +1
            if (STEP==1) {
                if (newActualPID>eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID>eLastPlate_eAliPID && gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Execute--- ---Stopp Loop since: newActualPID>eLastPlate_eAliPID"<<endl;
            }
            // This if holds in the case of STEP== -1
            if (STEP==-1) {
                if (newActualPID<eLastPlate_eAliPID) StillToLoop=kFALSE;
                if (newActualPID<eLastPlate_eAliPID && gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Execute--- ---Stopp Loop since: newActualPID<eLastPlate_eAliPID"<<endl;
            }
            // This if holds  general, since eNumberPlate_eAliPID is not dependent of the structure of the gAli subject:
            if (NLoopedPattern>eNumberPlate_eAliPID) StillToLoop=kFALSE;
            if (NLoopedPattern>eNumberPlate_eAliPID && gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Execute--- ---Stopp Loop since: NLoopedPattern>eNumberPlate_eAliPID"<<endl;

            ActualPID=newActualPID;
        } // of // while (StillToLoop)

        if (gEDBDEBUGLEVEL>2) cout << "Finshed  __while (StillToLoop)__  Now Adding reconstructed shower to array (only in case it has two or more Basetracks) ..." << endl;
        if (gEDBDEBUGLEVEL>2) cout << "Shower Has  ..." << RecoShower->N() << "  basetracks in it." << endl;

        // Add Shower Object to Shower Reco Array.
        // Not, if its empty or containing only one BT:
        if (RecoShower->N()>1) eRecoShowerArray->Add(RecoShower);

        // Set back loop values:
        StillToLoop=kTRUE;
        NLoopedPattern=0;
    } // of  //   for (Int_t i=eInBTArrayN-1; i>=0; --i) {


    // Set new value for  eRecoShowerArrayN  (may now be < eInBTArrayN).
    SetRecoShowerArrayN(eRecoShowerArray->GetEntries());

    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::eRecoShowerArray() Entries: " << eRecoShowerArray->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Execute()...done." << endl;

    ///========================   WRITE TO FILE..... ======================================================
    ///========================   TEMPORARY SOLUTION TO ACCES FUNCTION NEURALNET AND ENERGY .....==========
    ///========================  Transfer ..... ======================================================
    TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(eRecoShowerArray);
    NeuralNet();
    CalculateEnergyValues();



    cout << "======================== " << eQualityPar[0]  << endl;
    cout << "======================== " << eQualityPar[1]  << endl;
    return;
}

//______________________________________________________________________________




//______________________________________________________________________________

Bool_t EdbShowerRec::FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, EdbTrackP* shower)
{
    //cout << "EdbShowerRec::FindPrecedingBTs   Find BTs to be connected with the Test BT:" << endl;

    EdbSegP* s_TestBT;
    Int_t nentries=shower->N();
    Double_t dZ;

    // In case the shower is empty we do not have to search for a preceeding BT:
    if (nentries==0) return kTRUE;

    // We do not test BaseTracks which are before the initiator BaseTrack!
    if (s->Z()<InBT->Z()) {
        cout << "--- --- EdbShowerRec::FindPrecedingBTs(): s->Z()<InBT->Z()..: We do not test BaseTracks which are before the initiator BaseTrack!  return kFALSE;" << endl;
        return kFALSE;
    }

    // For the very first Z position we do not test
    // if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
    // Take 50microns and 80mrad in (dR/dT) around.
    // This does not affect the normal results, but helps for
    // events which may have a second BT close to InBT (like in e+e-)
    if (TMath::Abs(s->Z()-InBT->Z())<2.0) {
        //cout << "--- --- EdbShowerRec::FindPrecedingBTs(): s->Z()~InBT->Z()..: DeltaThetaComponentwise(s, InBT)= " << DeltaThetaComponentwise(s, InBT) <<  " <<  DeltaR_WithoutPropagation(s, InBT) = " << DeltaR_WithoutPropagation(s, InBT) << endl;
        if (DeltaThetaComponentwise(s, InBT) < 0.08 && DeltaR_WithoutPropagation(s, InBT) < 50.0 ) return kTRUE;
    }

    if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowerRec::FindPrecedingBTs(): Testing " << nentries << " entries of the shower." << endl;

    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( shower->GetSegment(i) );

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowerRec::FindPrecedingBTs(): Do  s_TestBT->ID()  s->ID()  s_TestBT->MCEvt() s_TestBT->Z()  s->Z() "<< s_TestBT->ID() << " " << s->ID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << "  " <<  s->Z()  <<endl;
        if (gEDBDEBUGLEVEL>3) s_TestBT->PrintNice();
        if (gEDBDEBUGLEVEL>3) s->PrintNice();


        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(eAlgoParameterNPropagation*1300.0)+30.0) continue;     // Exclude the case of more than eAlgoParameterNPropagation plates before...

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowerRec::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaThetaComponentwise(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << endl;

        if (DeltaThetaComponentwise(s, s_TestBT) >  eAlgoParameterConnectionDT ) continue;
        if (DeltaR_WithPropagation(s, s_TestBT) > eAlgoParameterConnectionDR ) continue;

        if (gEDBDEBUGLEVEL>3) cout << "--- --- EdbShowerRec::FindPrecedingBTs(): Checking dT,dR and dZ for i:  " << i << "  " << DeltaThetaComponentwise(s, s_TestBT)  << "  " << DeltaR_WithPropagation(s, s_TestBT) << "  "<<dZ << "   ok!"<<endl;
        return kTRUE;
    }
    //---------------------------------------------
    return kFALSE;
}




//------------------------------------------------------------------------------------------------------

void EdbShowerRec::ClearInBTArray() {
    if (eInBTArrayN>0) eInBTArray->Clear();
    eInBTArrayN=0;
    return;
}
void EdbShowerRec::ClearRecoShowerArray() {
    if (eRecoShowerArrayN>0) eRecoShowerArray->Clear();
    eRecoShowerArrayN=0;
    return;
}

//------------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void EdbShowerRec::TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TObjArray* showrecoarray)
{
    Log(2, "EdbShowerRec", "--- void* TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree() ---Interim SOLUTIION !!!! ");

    // Interim SOLUTIION !!!!  shower tree definition

    //---------------------------------------------------------------------------------
    //-     VARIABLES: shower_  "treebranch"  reconstruction
    //---------------------------------------------------------------------------------
    Int_t shower_number_eventb, shower_sizeb, shower_isizeb,shower_showerID;
    Int_t shower_sizeb15, shower_sizeb20, shower_sizeb30;
    Float_t shower_energy_shot_particle;
    Float_t shower_xb[9999];
    Float_t shower_yb[9999];
    Float_t shower_zb[9999];
    Float_t shower_txb[9999];
    Float_t shower_tyb[9999];
    Float_t shower_deltarb[9999];
    Float_t shower_deltathetab[9999];
    Float_t shower_deltaxb[9999];
    Float_t shower_deltayb[9999];
    Int_t   shower_nfilmb[9999];
    //Int_t   shower_ngrainb[9999];
    Float_t shower_chi2btkb[9999];
    Int_t shower_ntrace1simub[9999]; // MCEvt
    Int_t shower_ntrace2simub[9999]; // s->W()
    Float_t shower_ntrace3simub[9999]; // s->P()
    Int_t shower_ntrace4simub[9999]; // s->Flag()
    Float_t shower_tagprimary[9999];
    Int_t   shower_idb[9999];
    Int_t   shower_plateb[9999];
    Float_t shower_deltasigmathetab[58];
    Int_t   shower_numberofilms;
    Float_t shower_purityb,shower_E_MC,shower_trackdensb;
    //---------------------------------------------------------------------------------

    TTree* interim_treesaveb = new TTree("treebranch","tree of branchtrack");
    interim_treesaveb->Branch("number_eventb",&shower_number_eventb,"number_eventb/I");
    interim_treesaveb->Branch("sizeb",&shower_sizeb,"sizeb/I");
    interim_treesaveb->Branch("sizeb15",&shower_sizeb15,"sizeb15/I");
    interim_treesaveb->Branch("sizeb20",&shower_sizeb20,"sizeb20/I");
    interim_treesaveb->Branch("sizeb30",&shower_sizeb30,"sizeb30/I");
    interim_treesaveb->Branch("E_MC",&shower_E_MC,"E_MC/F");
    interim_treesaveb->Branch("isizeb",&shower_isizeb,"isizeb/I");
    interim_treesaveb->Branch("showerID",&shower_showerID,"showerID/I");
    interim_treesaveb->Branch("idb",shower_idb,"idb[sizeb]/I");
    interim_treesaveb->Branch("plateb",shower_plateb,"plateb[sizeb]/I");
    interim_treesaveb->Branch("xb",shower_xb,"xb[sizeb]/F");
    interim_treesaveb->Branch("yb",shower_yb,"yb[sizeb]/F");
    interim_treesaveb->Branch("zb",shower_zb,"zb[sizeb]/F");
    interim_treesaveb->Branch("txb",shower_txb,"txb[sizeb]/F");
    interim_treesaveb->Branch("tyb",shower_tyb,"tyb[sizeb]/F");
    interim_treesaveb->Branch("nfilmb",shower_nfilmb,"nfilmb[sizeb]/I");
    interim_treesaveb->Branch("ntrace1simub",shower_ntrace1simub,"ntrace1simu[sizeb]/I");
    interim_treesaveb->Branch("ntrace2simub",shower_ntrace2simub,"ntrace2simu[sizeb]/I"); // s->W()
    interim_treesaveb->Branch("ntrace3simub",shower_ntrace3simub,"ntrace3simu[sizeb]/F"); // s->P()
    interim_treesaveb->Branch("ntrace4simub",shower_ntrace4simub,"ntrace4simu[sizeb]/I"); // s->Flag()
    interim_treesaveb->Branch("chi2btkb",shower_chi2btkb,"chi2btkb[sizeb]/F");
    interim_treesaveb->Branch("deltarb",shower_deltarb,"deltarb[sizeb]/F");
    interim_treesaveb->Branch("deltathetab",shower_deltathetab,"deltathetab[sizeb]/F");
    interim_treesaveb->Branch("deltaxb",shower_deltaxb,"deltaxb[sizeb]/F");
    interim_treesaveb->Branch("deltayb",shower_deltayb,"deltayb[sizeb]/F");
    interim_treesaveb->Branch("tagprimary",shower_tagprimary,"tagprimary[sizeb]/F");
    interim_treesaveb->Branch("purityb",&shower_purityb,"purityb/F");
    interim_treesaveb->Branch("trackdensb",&shower_trackdensb,"trackdensb/F");


    Int_t showrecoarrayN=showrecoarray->GetEntries();
    cout << "showrecoarray->GetEntries(); " <<   showrecoarray->GetEntries() << endl;

    // LOOP OVER ENTRIES FROM showrecoarray:
    for (Int_t ooo=0; ooo< showrecoarrayN; ++ooo) {

        // Get Track from  showrecoarray
        EdbTrackP* segarray = (EdbTrackP*)showrecoarray->At(ooo);
        if (gEDBDEBUGLEVEL>2) segarray->Print();

        EdbSegP* seg;
        EdbSegP* Inseg;
        Int_t diff_pid;
        Float_t min_shower_deltathetab=99999;
        Float_t min_shower_deltar=99999;
        Float_t test_shower_deltathetab=99999;
        Float_t test_shower_deltar=99999;
        Float_t test_shower_deltax,test_shower_deltay;

        Float_t shower_sizebNHELP=0;
        Float_t shower_sizebMCNHELP=0;

        Float_t extrapol_x,extrapol_y, extrapo_diffz;

        // Initialize arrays...
        shower_sizeb15=0;
        shower_sizeb20=0;
        shower_sizeb30=0;
        shower_sizeb=0;
        shower_energy_shot_particle=0.0;
        shower_numberofilms=0;
        for (int ii=0; ii<9999; ii++)  {
            shower_xb[ii]=0;
            shower_yb[ii]=0;
            shower_zb[ii]=0;
            shower_txb[ii]=0;
            shower_tyb[ii]=0;
            shower_nfilmb[ii]=0;
            shower_tagprimary[ii]=0;
            shower_ntrace1simub[ii]=0;
            shower_ntrace2simub[ii]=0;
            shower_ntrace3simub[ii]=0;
            shower_ntrace4simub[ii]=0;
            shower_deltaxb[ii]=0;
            shower_deltayb[ii]=0;
            shower_chi2btkb[ii]=0;
            shower_idb[ii]=0;
            shower_plateb[ii]=0;
        }
        for (int i=1; i<59; ++i) {
            shower_deltasigmathetab[i]=0;
        }

        // Part To calculate the TransfereedVariables....
        shower_sizeb=segarray->N();
        Inseg=(EdbSegP*)segarray->GetSegment(0);
        shower_energy_shot_particle=Inseg->P();

        if (gEDBDEBUGLEVEL>3) cout << "--- --- ---------------------"<<endl;
        //-------------------------------------
        for (int ii=0; ii<shower_sizeb; ii++)  {

            if (ii>=9999) {
                cout << "WARNING: shower_sizeb ( " << shower_sizeb<< ") greater than SHOWERARRAY.   Set sizeb to 9999 and  Stop filling!."<<endl;
                shower_sizeb=9999;
                break;
            }
//       seg=(EdbSegP*)segarray->At(ii); // works when TObjArray
            seg=(EdbSegP*)segarray->GetSegment(ii);  // works when EdbTrackP

            //-------------------------------------
            shower_xb[ii]=seg->X();
            shower_yb[ii]=seg->Y();
            shower_txb[ii]=seg->TX();
            shower_tyb[ii]=seg->TY();
            shower_zb[ii]=seg->Z();
            shower_chi2btkb[ii]=seg->Chi2();
            shower_deltathetab[ii]=0.5;
            shower_deltarb[ii]=200;
            shower_tagprimary[ii]=0;
            if (ii==0) shower_tagprimary[ii]=1;
            shower_isizeb=1; // always 1, not needed anymore
            if (seg->MCEvt()>0) shower_number_eventb=seg->MCEvt();
            shower_ntrace1simub[ii]=0;
            if (seg->MCEvt()>0) shower_ntrace1simub[ii]=seg->MCEvt();
            shower_ntrace2simub[ii]=seg->W();
            shower_ntrace3simub[ii]=seg->P();
            shower_ntrace4simub[ii]=seg->Flag();
            shower_idb[ii]=seg->ID();
            shower_plateb[ii]=seg->PID();
            shower_E_MC=seg->P();
            //-------------------------------------
            // PUT HERE:  deltarb,deltarb, nflimb, sizeb15......
            diff_pid=TMath::Abs( Inseg->PID()-seg->PID() )+1;
            // (does this work for up/downsream listing??)
            // (yes, since InBT->PID is also changed.)
            // but works only if the gAli Object has no missing plates
            // otherwise f.e. PID(1) and PID(2) are not necessaryly abay by dZ=1300
            // (could be Z(1)=1300 and Z(2)=3900...)

            // Calc pur:
            shower_sizebNHELP++;
            if (seg->MCEvt()>0) shower_sizebMCNHELP++;

            // InBT:
            if (ii==0) {
                shower_deltathetab[0]=0.5;
                shower_deltarb[0]=200;
                shower_nfilmb[0]=1;
            }
            // All other BTs:
            if (ii>0) {
                // its correct like this, since this is the way it is done in
                // the official FJ-Algorithm:
                shower_nfilmb[ii]=diff_pid;
                if (gEDBDEBUGLEVEL>3) cout << "--- ---Inseg->PID() seg->PID() ii diif_pid shower_nfilmb[ii]  " << Inseg->PID()<< "   "  <<  seg->PID() << "   " << ii<< "  " << diff_pid<<"  "<< shower_nfilmb[ii]<<"  " << endl;

                if (diff_pid >= 15 ) shower_sizeb15++;
                if (diff_pid >= 20 ) shower_sizeb20++;
                if (diff_pid >= 30 ) shower_sizeb30++;

                // PUT HERE:  calculation routine for shower_deltasigmathetab
                // see referenc in thesis of Luillo Esposito, page 109.
                shower_deltasigmathetab[diff_pid]=shower_deltasigmathetab[diff_pid]+(TMath::Power(shower_txb[ii]-shower_txb[0],2)+TMath::Power(shower_tyb[ii]-shower_tyb[0],2));

                // PUT HERE:  calculation routine for shower_deltathetab, shower_deltarb
                // ExSetTreebranchNametrapolate the BT [ii] to the position [jj] and then calc the
                // position and slope differences;
                // For the backward extrapolation of the   shower_deltathetab and shower_deltarb
                // calulation for BaseTrack(ii), Basetrack(jj)->Z() hast to be smaller.
                min_shower_deltathetab=99999;   // Reset
                min_shower_deltar=99999;        // Reset

                for (int jj=0; jj<shower_sizeb; jj++)  {
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
                    // Maybe a change is necessary because it is not exactly the same as in the off. algorithm.
                    //if (test_shower_deltar<400 && test_shower_deltathetab<0.8 ) {
                    if (test_shower_deltar<150 && test_shower_deltathetab<0.15 ) {
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

            shower_purityb=shower_sizebMCNHELP/shower_sizebNHELP;
            shower_trackdensb=trackdensb;
        }
        //-------------------------------------
        for (int i=1; i<59; ++i) {
            shower_deltasigmathetab[i]=TMath::Sqrt(shower_deltasigmathetab[i]);
        }
        shower_numberofilms=shower_nfilmb[shower_sizeb-1]; // there we assume (this is correct always?) when
        // the last shower BT is in the last film...(otherwise we would again have to loop on sizeb array);


        // Fill Tree:
        interim_treesaveb->Fill();
        if (gEDBDEBUGLEVEL>2) interim_treesaveb->Show(interim_treesaveb->GetEntries()-1);

        if (gEDBDEBUGLEVEL>2) cout << "Now we have interim_treesaveb  Entries: "  << interim_treesaveb->GetEntries() << endl;
        if (gEDBDEBUGLEVEL>3) interim_treesaveb->Show(interim_treesaveb->GetEntries()-1);


    } // LOOP OVER ENTRIES FROM showrecoarray: END.
    //-----------------------------------------------------------------------------------------------



    ///------------  ONLY FOR EDBSHOWERREC.... NOT FOR MY OWN STUFF!!---------------------------
    TFile* interim_fileout = new TFile("shower1.root","RECREATE");
    interim_treesaveb->Write();
    interim_fileout->Close();
    delete interim_fileout;
    delete interim_treesaveb;
    ///------------  ONLY FOR EDBSHOWERREC.... NOT FOR MY OWN STUFF!!---------------------------


    return;
}

//-------------------------------------------------------------------------------------------




void EdbShowerRec::TransferTreebranchShowerTreeIntoShowerObjectArray(TTree* treebranch) {
    Log(2, "EdbShowerRec", "--- void* TransferTreebranchShowerTreeIntoShowerObjectArray() ---Interim SOLUTIION !!!! ");

    // Decaring ObjArray which are needed. Use out of private member variables of this class.
    TObjArray* showarray;
    showarray = new TObjArray();
    TTree* eShowerTree = treebranch;

    // EdbShowerP* show=0; // libShowRec
    EdbTrackP* show=0; // libShower

    EdbSegP*    showseg=0;

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


    // Set Addresses of treebranch tree:
    eShowerTree->SetBranchAddress("number_eventb",&shower_number_eventb);
    eShowerTree->SetBranchAddress("sizeb",&shower_sizeb);
    eShowerTree->SetBranchAddress("sizeb15",&shower_sizeb15);
    eShowerTree->SetBranchAddress("sizeb20",&shower_sizeb20);
    eShowerTree->SetBranchAddress("sizeb30",&shower_sizeb30);
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
    eShowerTree->SetBranchAddress("energy_shot_particle",&shower_energy_shot_particle);
    eShowerTree->SetBranchAddress("E_MC",&shower_energy_shot_particle);
    eShowerTree->SetBranchAddress("showerID",&shower_showerID);
    eShowerTree->SetBranchAddress("idb",shower_idb);
    eShowerTree->SetBranchAddress("plateb",shower_plateb);
    eShowerTree->SetBranchAddress("deltasigmathetab",shower_deltasigmathetab);
    eShowerTree->SetBranchAddress("lenghtfilmb",&shower_numberofilms);
    eShowerTree->SetBranchAddress("purityb",&shower_purb); // shower purity


    Int_t nent=eShowerTree->GetEntries();
    cout << "nent = " << nent << endl;

    Int_t nenttotalcount=0;
    cout << "nenttotalcount = " << nenttotalcount << endl;

    Bool_t QUICK_READ=kFALSE;
// 	QUICK_READ=kTRUE; cout << "QUICK_READ=kTRUE; "<< endl;
    if (QUICK_READ) nent=1; // read only first shower... (mainly for debug purposes).

    // Loop over Tree Entries (==different showers):
    for (int i=0; i<nent; ++i) {
        eShowerTree->GetEntry(i);
        //cout << "i = " << i << endl;
        //cout << "shower_tagprimary[0] = " << shower_tagprimary[0] << endl;
        if (gEDBDEBUGLEVEL>3) eShowerTree->Show(i);


        // Since we do not take showers with below 1segment
        if (shower_sizeb<=1) continue;

//      show = new EdbShowerP(shower_sizeb); // libShowRec
        show = new EdbTrackP(shower_sizeb); // libShower

        // For Frederics algorithm, shower_number_eventb gives NOT -999 as for a BG track,
        // but "0". So we need to disticnct also for "0" cases:
        show ->SetMC(shower_number_eventb,shower_number_eventb);
        if (shower_number_eventb==0) show ->SetMC(-999,999);

        // To Have unique profile histograms in the memory:
        // show = new EdbShowerP(0,eRecoShowerArrayN,shower_sizeb);

        //cout << "------------------- shower_tagprimary[0]  shower_tagprimary[shower_sizeb-1]  " << shower_tagprimary[0] << "  " <<  shower_tagprimary[shower_sizeb-1] << endl;

        int nplmax=0;

        // Loop over shower Entries (==different basetracks):
        for (int j=0; j<shower_sizeb; ++j) {

            //cout << "------------------ --- j= " << j << endl;

            // Keep Notice of the order!!!
            // shower_tagprimary  has to be added first!!
            // temporary solution: if that is not the case as below, no segment will be added....
            // ...
            if (shower_tagprimary[0]!=1) continue;

            // int id, float x, float y, float tx, float ty, float w = 0, int flag = 0
            showseg = new EdbSegP(j,shower_xb[j],shower_yb[j],shower_txb[j],shower_tyb[j],shower_ntrace2simub[j],0);

            showseg->SetZ(shower_zb[j]);
            showseg->SetChi2(shower_chi2btkb[j]);
            showseg->SetP(shower_ntrace3simub[j]);
            showseg->SetFlag(shower_ntrace4simub[j]);
            showseg->SetMC(shower_ntrace1simub[j],shower_ntrace1simub[j]);
            // For Frederics algorithm, ntrace1simub gives NOT -999 as for a BG track,
            // but "0". So we need to disticnct also for "0" cases:
            if (shower_ntrace1simub[j]==0) showseg->SetMC(-999,-999);

            showseg->SetPID(shower_plateb[j]);
            // in real data plateb is 57 for emulsion 57...(sim may be different, depending on orfeo...)

            /// DEBUGTEST
            /// DEBUG TEST FOR FREDERIC !!! (FWM, 27.08.2010)
            showseg->SetPID(shower_nfilmb[j]);
            /// showseg->SetPID(shower_nfilmb[j]);
            // in real data plateb is 57 for emulsion 57...(sim may be different, depending on orfeo...)

            showseg->SetID(shower_idb[j]);
            showseg->SetProb(1);

            //Set Npl right. In libShowRec we dont have to to this since we call the EdbShowerP::Update() function
            //int npl = TMath::Abs(shower_plateb[j]-shower_plateb[0]);
            // The first plate of shower_plateb[0] when used in ::rec() mode is somehow wrong,
            // therefore we rather take nfilmb for plate references...
            int npl = TMath::Abs(shower_nfilmb[j]-shower_nfilmb[0]);
            if (npl>nplmax) {
                show->SetNpl(npl);
            }

            //showseg->PrintNice();

            show->AddSegment(showseg);
	    	// Attention: If you have a EdbTrackP object and you add a segment, then
	    // you have to manually set Counters for N(), NPl(). It is not done auto.
	    show -> SetCounters();
        } // Loop over shower Entries (==different basetracks):


        // This ID should be unique!
        // (Means every shower has one different!)
        show->SetID(nenttotalcount);
        nenttotalcount++;

        // Add Shower Object to Shower Reco Array.
        // Not, if its empty:
        // Not, if its containing only one BT:
        if (show->N()>1) {
            showarray->Add(show);
            ++eRecoShowerArrayN;
            if (gEDBDEBUGLEVEL>3) cout << "EdbShowRec::Treebranch_To_RecoShowerArray   showarray->Add(show) ...   done." << endl;
        }

        if (gEDBDEBUGLEVEL>3) show->PrintNice();

    }// Loop over Tree Entries (==different showers):
    //---------------------------------------------------

    if (gEDBDEBUGLEVEL>2) eShowerTree->Show(0);
    if (gEDBDEBUGLEVEL>2) eShowerTree->Show(nent-1);

    cout << "EdbShowRec::Treebranch_To_RecoShowerArray   Loop over Tree Entries (==different showers)  done." << endl;
    cout << "EdbShowRec::Treebranch_To_RecoShowerArray   showarray->N()" << showarray->GetEntries() <<endl;

    SetRecoShowerArray(showarray);
    SetRecoShowerArrayN(showarray->GetEntries());

    Log(2,"EdbShowRec::Treebranch_To_RecoShowerArray","EdbShowRec::Treebranch_To_RecoShowerArray...done.");
    return;
}






//------------------------------------------------------------------------------------------------------



void EdbShowerRec::ResetAlgoParameters()
{
    eAlgoParameterConeRadius=800;      // Radius of spanning cone from first BT
    eAlgoParameterConeAngle=0.015;       // Opening angle of spanning cone from first BT
    eAlgoParameterConnectionDR=150;      // Connection Criterium: delta R
    eAlgoParameterConnectionDT=0.15;      // Connection Criterium: delta T
    eAlgoParameterNPropagation=3;      // N plates backpropagating
    return;
}

//------------------------------------------------------------------------------------------------------

void EdbShowerRec::SetAlgoParameter(Double_t paravalue, Int_t paranr)
{
    if (paranr==0) eAlgoParameterConeRadius=paravalue;
    else if (paranr==1) eAlgoParameterConeAngle=paravalue;
    else if (paranr==2) eAlgoParameterConnectionDR=paravalue;
    else if (paranr==3) eAlgoParameterConnectionDT=paravalue;
    else if (paranr==4) eAlgoParameterNPropagation=paravalue;
    else {
        return;
    }
}

//------------------------------------------------------------------------------------------------------

Double_t EdbShowerRec::GetAlgoParameter(Int_t paranr)
{
    if (paranr==0) return eAlgoParameterConeRadius;
    else if (paranr==1) return eAlgoParameterConeAngle;
    else if (paranr==2) return eAlgoParameterConnectionDR;
    else if (paranr==3) return eAlgoParameterConnectionDT;
    else if (paranr==4) return eAlgoParameterNPropagation;
    else {
        return 0;
    }
}

//------------------------------------------------------------------------------------------------------

void EdbShowerRec::PrintParameters() {
    cout << "void            EdbShowerRec::PrintParameters() " << endl;
    cout << "eAlgoParameterConeRadius = " << eAlgoParameterConeRadius << endl;
    cout << "eAlgoParameterConeAngle = " << eAlgoParameterConeAngle << endl;
    cout << "eAlgoParameterConnectionDR = " << eAlgoParameterConnectionDR << endl;
    cout << "eAlgoParameterConnectionDT = " << eAlgoParameterConnectionDT << endl;
    cout << "eAlgoParameterNPropagation = " << eAlgoParameterNPropagation << endl;
    return;
}

//------------------------------------------------------------------------------------------------------

void EdbShowerRec::CalcTrackDensity(EdbPattern* pat_interim,Int_t pat_interim_halfsize,Int_t& npat_int,Int_t& npat_total,Int_t& npatN)
{
    if (gEDBDEBUGLEVEL>3) cout << "-------------void EdbShowerRec::CalcTrackDensity(&pat_interim,pat_interim_halfsize,&npat_int,&npat_total)"<<endl;
    npat_int=pat_interim->GetN();
    if (npat_int<=0) return;
    npat_total+=npat_int;
    ++npatN;
    if (npatN>0) trackdensb=(Float_t)npat_total/(Float_t)npatN/pat_interim_halfsize/pat_interim_halfsize/4.0*1000.0*1000.0; // BT/mm2
    if (gEDBDEBUGLEVEL>3) {
        cout << "npat_int = " << npat_int << endl;
        cout << "npat_total = " << npat_total << endl;
        cout << "npatN = " << npatN << endl;
        cout << "shower_trackdensb = " << trackdensb << endl;
    }
    return;
}


//------------------------------------------------------------------------------------------------------

Double_t EdbShowerRec::InvariantMass(EdbSegP* s1, EdbSegP* s2, Double_t Momentum1,Double_t Momentum2, Double_t ID1, Double_t ID2)
{
    cout << "EdbShowerRec::InvariantMass()" << endl;
    cout << "s1->P()  " << s1->P() << endl;
    cout << "s2->P()  " << s2->P() << endl;

    // If no momentum is specified, then it will be taken from segments momentum:
    if (Momentum1<0) Momentum1=s1->P();
    if (Momentum2<0) Momentum2=s2->P();

    //mass reconstruction for Pi0 using 2 tracks
    //you have to know the momentum of the 2 tracks and their slopes (Tx,Ty)
    //first implementation: frederic juget
    TLorentzVector v1;
    TLorentzVector v2;
    Double_t energy1=0;
    Double_t energy2=0;
    v1.SetPxPyPzE(s1->TX(),s1->TY(),1,Momentum1);
    v2.SetPxPyPzE(s2->TX(),s2->TY(),1,Momentum2);
    TLorentzVector vsum=v1+v1;
    Double_t invM=vsum.Mag2();
    cout << v1.Mag() << endl;
    cout << v2.Mag() << endl;
    cout << vsum.Mag() << endl;
    return invM;
}

//------------------------------------------------------------------------------------------------------
