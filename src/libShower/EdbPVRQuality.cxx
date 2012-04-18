#include "EdbPVRQuality.h"
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVRQuality                                                        //
//
// Base class for checks of the scanned data, especially for shower reconstruction.
// The observed basetrack density can vary from brick to brick with a large
// fluctuation.
// The shower algorithm in its standard implementation is sensitive to the
// level of tracks per unit area. In case there are too many of them, the purity
// of shower reconstruction goes down and does not give reliable results anymore.
// The EdbPVRQuality class basically equals the number of basetracks per unit area
// to a maximum upper value (if necessary) by adapting the quality cut for a single
// basetrack.
//                                                                      //
//////////////////////////////////////////////////////////////////////////


ClassImp(EdbPVRQuality)

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality()
{
    // Default Constructor
    cout << "EdbPVRQuality::EdbPVRQuality()   Default Constructor"<<endl;
    Init();
    Set0();
}

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)
{
    // Default Constructor with a EdbPVRec object.
    // (the EdbPVRec object corresponds to the collection of plates scanned with the collection
    //  of basetracks (and additionally linked tracks)).
    // This constructor automatically checks the original data for background level and
    // creates a new EdbPVRec object that contains only segments that fulfill the quality
    // cut in accordance with the desired (predefined) background level.
    // If general basetrack density is lower than 20BT/mm2 then no cleaning is done.
    cout << "EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)   Constructor (does automatically all in one...)"<<endl;

    Init();
    Set0();

    if (NULL == ali || ali->Npatterns()<1) {
        cout << "WARNING   EdbPVRQuality(EdbPVRec* ali)   ali is no good EdbPVRec object! Check!" << endl;
        return;
    }

    // Set ali as eAli_orig
    eAli_orig=ali;
    eIsSource=kTRUE;

    CheckEdbPVRec();

// 		Remove_Passing(eAli_orig);
// 		Remove_DoubleBT(eAli_orig);

//     Execute_ConstantBTDensity();
//    Execute_ConstantQuality();
//     CreateEdbPVRec();
    Print();
}

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali,  Float_t BTDensityTargetLevel)
{
    // Default Constructor with a EdbPVRec object and the desired basetrack density target level.
    // Does same as constructor EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali) but now with adjustable
    // background level.
    // If general basetrack density is lower than BTDensityTargetLevel BT/mm2 then no cleaning is done.
    cout << "EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)   Constructor (does automatically all in one...)"<<endl;

    Init();
    Set0();

    if (NULL == ali || ali->Npatterns()<1) {
        cout << "WARNING   EdbPVRQuality(EdbPVRec* ali)   ali is no good EdbPVRec object! Check!" << endl;
        return;
    }

    // Set ali as eAli_orig
    eAli_orig=ali;
    eIsSource=kTRUE;

    // Set BTDensityTargetLevel
    SetBTDensityLevel(BTDensityTargetLevel);
    cout << "EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)  GetBTDensityLevel() " << GetBTDensityLevel() << endl;

    CheckEdbPVRec();

    Remove_Passing(eAli_orig);
    Remove_DoubleBT(eAli_orig);
    Execute_ConstantBTDensity();
    CreateEdbPVRec();
    Print();
}

//______________________________________________________________________________

EdbPVRQuality::~EdbPVRQuality()
{
    // Default Destructor
    cout << "EdbPVRQuality::~EdbPVRQuality()"<<endl;
    delete 		eHistChi2W;
    delete 		eHistYX;
    delete 		eHistTYTX;
    delete 		eHistTT;

    delete 		eProfileBTdens_vs_PID_source;
}

//______________________________________________________________________________

void EdbPVRQuality::Set0()
{
    // Reset Values

    Log(2,"EdbPVRQuality::Set0","EdbPVRQuality::Set0");

    eAli_orig=NULL;
    eAli_modified=NULL;
    eNeedModified=kFALSE;
    eIsSource=kFALSE;
    eIsTarget=kFALSE;
    eAli_maxNpatterns=0;
    for (int i=0; i<3; i++) eCutMethodIsDone[i]=kFALSE;

    // Default BT density level for which the standard cutroutine
    // will be put:
    eBTDensityLevel=20; // #BT/mm2

    // Default BT density level will use only segments for
    // data calculation .
    eBTDensityLevelCalcMethodMC=kFALSE;
    eBTDensityLevelCalcMethodMCConfirmationNumber=0;

    // Reset Default Geometry: 0 OperaGeometry, 1: MC Geometry
    eHistGeometry=0;



    eHistYX->Reset();
    eHistChi2W->Reset();

    for (int i=0; i<114; i++) {
        ePatternBTDensity_orig[i]=0;
        ePatternBTDensity_modified[i]=0;
        eCutp1[i]=0.15;
        eCutp0[i]=1.0; // Maximum Cut Value for const, BT dens
        eAgreementChi2WDistCut[i]=5.0;  // Maximum Cut Value for const, BT dens

        for (int j=0; j<20; j++) {
            // Maximum Cut Value for const, BT dens, also in tanges theta space
            eCutTTp0[i][j]=1.00;
            eCutTTp1[i][j]=0.15;
        }
    }


    eProfileBTdens_vs_PID_source_meanX=0;
    eProfileBTdens_vs_PID_source_meanY=0;
    eProfileBTdens_vs_PID_source_rmsX=0;
    eProfileBTdens_vs_PID_source_rmsY=0;
    eProfileBTdens_vs_PID_target_meanX=0;
    eProfileBTdens_vs_PID_target_meanY=0;
    eProfileBTdens_vs_PID_target_rmsX=0;
    eProfileBTdens_vs_PID_target_rmsY=0;

    // Default values for cosmics, taken from a brick data:
    // (I cant remember which one, I hope it doesnt matter).
    eAgreementChi2CutMeanChi2=1.0;
    eAgreementChi2CutRMSChi2=0.3;
    eAgreementChi2CutMeanW=23;
    eAgreementChi2CutRMSW=3;

    Log(2,"EdbPVRQuality::Set0","EdbPVRQuality::Set0...done.");
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Init()
{
    // Basic Init function that creates objects in the memory which have
    // to be created only ONE time per class instance.

    Log(2,"EdbPVRQuality::Init","EdbPVRQuality::Init");

    eProfileBTdens_vs_PID_source = new TProfile("eProfileBTdens_vs_PID_source","eProfileBTdens_vs_PID_source",114,-0.5,113.5,0,200);
    eProfileBTdens_vs_PID_target = new TProfile("eProfileBTdens_vs_PID_target","eProfileBTdens_vs_PID_target",114,-0.5,113.5,0,200);

    eHistChi2W = new TH2F("eHistChi2W","eHistChi2W",40,0,40,90,0,3);
    eHistYX = new TH2F("eHistYX","eHistYX",100,0,1,100,0,1);
    eHistYX->SetMinimum(0);
    eHistYX->SetMaximum(150);
    eHistTYTX = new TH2F("eHistTYTX","eHistTYTX",100,-0.7,0.7,100,-0.7,0.7);
//     eHistTT = new TH1F("eHistTT","eHistTT",20,0,1);
    eHistTT = new TH1F("eHistTT","eHistTT",10,0,1);
//     cout << eHistTT->GetBinCenter(1) << endl;
//     cout << eHistTT->GetBinWidth(1) << endl;

    /// TEMPORARY, later the Standard Geometry should default be OPERA.
    /// For now we use a very large histogram that can contain both case
    /// x-y ranges. This takes slightly more memory but it shouldnt matter.
//     if (eHistGeometry==0) SetHistGeometry_OPERA();
//     cout << "EdbPVRQuality::Init()   /// TEMPORARY  SetHistGeometry_OPERAandMC " <<  endl;
    SetHistGeometry_OPERAandMC();


    Log(2,"EdbPVRQuality::Init","EdbPVRQuality::Init...done.");
    return;
}


//______________________________________________________________________________
void EdbPVRQuality::SetCutMethod(Int_t CutMethod)
{
    // Set Cut Method of the background reduction:
    // 0: (default): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b
    // 1: (testing): Do cut based on a chi2-variable that compares with passing tracks
    // (if available), i.e. cosmics.
    // 2: (testing): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b
    // split into tangens theta space


    Log(2,"EdbPVRQuality::SetCutMethod","EdbPVRQuality::SetCutMethod");

    eCutMethod=CutMethod;
    cout << "EdbPVRQuality::SetCutMethod  eCutMethod=  " << eCutMethod << endl;
    if (CutMethod==0) cout << "// 0: (default): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b" << endl;
    if (CutMethod==1) cout << "// 1: (testing): Do cut based on a chi2-variable that compares with passing tracks" << endl;
    if (CutMethod==1) cout << "// (if available), i.e. cosmics." << endl;
    if (CutMethod==2) cout << "// 2: (testing): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b" << endl;
    if (CutMethod==2) cout << "// split into tangens theta space" << endl;

    if (CutMethod>2) {
        eCutMethod=0;
        cout << "WARNING   EdbPVRQuality::SetCutMethod  eCutMethod invalid, Set back to default eCutMethod=  " << eCutMethod << endl;
    }

    Log(2,"EdbPVRQuality::SetCutMethod","EdbPVRQuality::SetCutMethod...done.");
    return;
}



//______________________________________________________________________________

void EdbPVRQuality::CheckEdbPVRec()
{
    // Main function to check if the EdbPVRec object of the scanned data is of low/high background.
    // Following steps are carried out:
    //  Get plate, count number of basetracks in the unit area (1x1cm^2).
    //  Fill (draw if desired (like in EDA display)) histogram with the entries of the unit area.
    //  Get mean of the histogram, compare this value with the reference value.
    // The histogram covers all the area of one emulsion. (for the record: the old ORFEO MC
    // simulation gives not the same position as data does. The area of the histogramm was largely
    // increased to cover both cases).
    // It gives a good estimation of the density. Spikes in some plates, or in some zones are not
    // checked for, this is on the todo list, but maybe not so important.

    Log(2,"EdbPVRQuality::CheckEdbPVRec","EdbPVRQuality::CheckEdbPVRec");

    if (!eIsSource) {
        cout << "EdbPVRQuality::CheckEdbPVRec  eIsSource=  " << eIsSource << ". This means no source set. Return!" << endl;
        return;
    }
    // Check the patterns of the EdbPVRec:
    eAli_maxNpatterns= eAli_orig->Npatterns();
//     cout << "EdbPVRQuality::CheckEdbPVRec  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << endl;
    if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
    if (eAli_maxNpatterns>114) {
        cout << "ERROR! EdbPVRQuality::CheckEdbPVRec  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }

    int Npat = eAli_maxNpatterns;
    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);

    // Loop over the patterns of the EdbPVRec:
    for (Int_t i=0; i<Npat; i++) {

        if (i>56) {
            cout << "WARNING     EdbPVRQuality::CheckEdbPVRec()    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING     EdbPVRQuality::CheckEdbPVRec()    Check it! " << endl;
        }
        if (gEDBDEBUGLEVEL>2) cout << "CheckEdbPVRec   Doing Pattern " << i << endl;


        eHistYX->Reset(); // important to clean the histogram
        eHistYX->SetMinimum(0);
        eHistYX->SetMaximum(150);
        eHistChi2W->Reset(); // important to clean the histogram

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();

        EdbSegP* seg=0;
        // Loop over the segments of the pattern
        for (Int_t j=0; j<npat; j++) {
            seg=pat->GetSegment(j);
            // Very important:
            // For the data case, we assume the following:
            // Data (MCEvt<0) will     be taken for BTdensity calculation
            // Sim (MCEvt>0)      will NOT be taken for BTdensity calculation
            // We take it ONLY and ONLY into account if it is especially wished
            // by the user!
            // Therefore (s)he needs to know how many Gauge Coupling Parameters
            // in the Standard Model exist (at least)...
            Bool_t result=kTRUE;
            if (seg->MCEvt()>0) {
                if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                    result = kTRUE;
                    // cout << "result = kTRUE !! " << endl;
                }
                else {
                    result = kFALSE;
                }
            }

            if (gEDBDEBUGLEVEL>4)  cout << "Doing segment " << j << " result for bool query is: " << result << endl;

            // Main decision for segment to be kept or not  (seg is of MC or data type).
            if ( kFALSE == result ) continue;

            // For the check, fill the histograms in any case:
            eHistYX->Fill(seg->Y(),seg->X());
            eHistTYTX->Fill(seg->TY(),seg->TX());
            eHistTT->Fill(TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX()));
            eHistChi2W->Fill(seg->W(),seg->Chi2());

        } // for (Int_t j=0; j<npat; j++)

        if (gEDBDEBUGLEVEL>2) cout << "I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

        // Important to reset histogram before it is filled.
        histPatternBTDensity->Reset();

        // Search for empty bins, because they can spoil the overall calulation
        // of the mean value.
        Int_t nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
        Int_t nemptybinsXY=0;
        Int_t bincontentXY=0;
        for (int k=1; k<nbins-1; k++) {
            if (eHistYX->GetBinContent(k)==0) {
                ++nemptybinsXY;
                continue;
            }
            bincontentXY=eHistYX->GetBinContent(k);
            histPatternBTDensity->Fill(bincontentXY);
            eProfileBTdens_vs_PID_source->Fill(i,bincontentXY);
        }

        // failsafe warning in case that there are many bins with zero content.
        // for now we print a error message: TODO  REBIN THE YX HISTOGRA WITH 2.5x2.5 mm!!!!
        CheckFilledXYSize();

        // Save the density in the variable.
        ePatternBTDensity_orig[i]=histPatternBTDensity->GetMean();

    }

    eProfileBTdens_vs_PID_source_meanX=eProfileBTdens_vs_PID_source->GetMean(1);
    eProfileBTdens_vs_PID_source_meanY=eProfileBTdens_vs_PID_source->GetMean(2);
    eProfileBTdens_vs_PID_source_rmsX=eProfileBTdens_vs_PID_source->GetRMS(1);
    eProfileBTdens_vs_PID_source_rmsY=eProfileBTdens_vs_PID_source->GetRMS(2);

    // No assignment for the  eProfileBTdens_vs_PID_target  histogram yet.
    // This will be done in one of the two Execute_ functions.

// eHistTT->DrawCopy("");

    // This will be commented when using in batch mode...
    // For now its there for clarity reasons.
    TCanvas* c1 = new TCanvas();
    c1->Divide(3,2);
    c1->cd(1);
    eHistYX->DrawCopy("colz");
    c1->cd(2);
    eHistChi2W->DrawCopy("colz");
    c1->cd(3);
    eHistTYTX->DrawCopy("colz");
    c1->cd(4);
    histPatternBTDensity->DrawCopy("");
    c1->cd(5);
    eProfileBTdens_vs_PID_source->Draw("profileZ");
    eProfileBTdens_vs_PID_source->GetXaxis()->SetRangeUser(0,eAli_maxNpatterns+2);
    c1->cd(6);
    eHistTT->DrawCopy("");
    c1->cd();

    eHistYX->Reset();
    eHistTYTX->Reset();
    eHistTT->Reset();
    eHistChi2W->Reset();

    Log(2,"EdbPVRQuality::CheckEdbPVRec","EdbPVRQuality::CheckEdbPVRec...done");
    return;
}


//______________________________________________________________________________

void EdbPVRQuality::CheckEdbPVRecThetaSpace(Int_t AliType)
{
    // Alternative Implementation.
    // Following a suggestion to Akitaka Ariga when doing reco of a specified shower:
    // ------------------------------------------------------------------------------
    // Main function to check if the EdbPVRec object of the scanned data is of low/high background.
    // But w.r.t. the TanTheta Space of scanned tracks.
    //
    // AliType
    // ------------------------------------------------------------------------------


    // Following steps are carried out:
    //  Get plate, count number of basetracks in the unit area (1x1cm^2).
    //  Fill (draw if desired (like in EDA display)) histogram with the entries of the unit area.
    //  Get mean of the histogram, compare this value with the reference value.
    // The histogram covers all the area of one emulsion. (for the record: the old ORFEO MC
    // simulation gives not the same position as data does. The area of the histogramm was largely
    // increased to cover both cases).
    // It gives a good estimation of the density. Spikes in some plates, or in some zones are not
    // checked for, this is on the todo list, but maybe not so important.

    Log(2,"EdbPVRQuality::CheckEdbPVRecThetaSpace","EdbPVRQuality::CheckEdbPVRecThetaSpace");

    EdbPVRec* AliSource=NULL;
    cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace   AliType = " << AliType << endl;
    if (AliType==1) {
        AliSource=eAli_orig;
    }
    else {
        AliSource = eAli_modified;
        if (NULL == eAli_modified) {
            AliSource=eAli_orig;
            cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace  NULL == eAli_modified. This means that source will be set to eAli_orig." << endl;
        }
    }

    if (!eIsSource) {
        cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace  eIsSource=  " << eIsSource << ". This means no source set. Return!" << endl;
        return;
    }
    // Check the patterns of the EdbPVRec:
    eAli_maxNpatterns= eAli_orig->Npatterns();

    cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << endl;

    if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
    if (eAli_maxNpatterns>114) {
        cout << "ERROR! EdbPVRQuality::CheckEdbPVRecThetaSpace  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }

    int Npat = eAli_maxNpatterns;
    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);
    TH1F* histPatternBTDensityTanTheta = new TH1F("histPatternBTDensityTanTheta","histPatternBTDensityTanTheta",40,0,1);

    TH2F* histPatternBTDensityTanThetaVsPID = new TH2F("histPatternBTDensityTanThetaVsPID","histPatternBTDensityTanThetaVsPID",57,0.5,57.5,40,0,1);


    TH1F* QualityValue_Chi2 = new TH1F("QualityValue_Chi2","QualityValue_Chi2",100,0,10);
    TH1F* QualityValue_W = new TH1F("QualityValue_W","QualityValue_W",100,0,100);
    TH1F* QualityValue_Total = new TH1F("QualityValue_Total","QualityValue_Total",100,0,3);


    TProfile* 	eProfileBTdens_vs_TanTheta = new TProfile("eProfileBTdens_vs_TanTheta","eProfileBTdens_vs_TanTheta",40,0,1,0,200);

//     Float_t  		eProfileBTdens_vs_PID_target_meanX,eProfileBTdens_vs_PID_target_meanY;
//     Float_t  		eProfileBTdens_vs_PID_target_rmsX,eProfileBTdens_vs_PID_target_rmsY;



    // Loop over the patterns of the EdbPVRec:
    for (Int_t i=0; i<Npat; i++) {

        if (i>56) {
            cout << "WARNING     EdbPVRQuality::CheckEdbPVRecThetaSpace()    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING     EdbPVRQuality::CheckEdbPVRecThetaSpace()    Check it! " << endl;
        }
        if (gEDBDEBUGLEVEL>2) cout << "CheckEdbPVRec   Doing Pattern " << i << endl;


        eHistYX->Reset(); // important to clean the histogram
        eHistChi2W->Reset(); // important to clean the histogram
        histPatternBTDensityTanTheta->Reset(); // important to clean the histogram


        EdbPattern* pat = (EdbPattern*)AliSource->GetPattern(i);
        Int_t npat=pat->N();

        EdbSegP* seg=0;
        // Loop over the segments of the pattern
        for (Int_t j=0; j<npat; j++) {
            seg=pat->GetSegment(j);
            // Very important:
            // For the data case, we assume the following:
            // Data (MCEvt<0) will     be taken for BTdensity calculation
            // Sim (MCEvt>0)      will NOT be taken for BTdensity calculation
            // We take it ONLY and ONLY into account if it is especially wished
            // by the user!
            // Therefore (s)he needs to know how many Gauge Coupling Parameters
            // in the Standard Model exist (at least)...
            Bool_t result=kTRUE;
            if (seg->MCEvt()>0) {
                if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                    result = kTRUE;
                    // cout << "result = kTRUE !! " << endl;
                }
                else {
                    result = kFALSE;
                }
            }

            if (gEDBDEBUGLEVEL>4)  cout << "Doing segment " << j << " result for bool query is: " << result << endl;

            // Main decision for segment to be kept or not  (seg is of MC or data type).
            if ( kFALSE == result ) continue;


            /// Decision if, cut out strong different TTheta values,
            /// because for shower reco there is only the TTheta space around
            /// the InBT of interest.
            Float_t sx=0.3;
            Float_t sy=0.3;

            // For the check, fill the histograms in any case:
            eHistYX->Fill(seg->Y(),seg->X());
            eHistChi2W->Fill(seg->W(),seg->Chi2());

            // New: Fill also histPatternBTDensityTanTheta
            histPatternBTDensityTanTheta->Fill(TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX()));

            histPatternBTDensityTanThetaVsPID->Fill(i,TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX()));

            eProfileBTdens_vs_TanTheta->Fill(TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX()),1);

            QualityValue_Chi2->Fill(seg->Chi2());
            QualityValue_W->Fill(seg->W());

            Double_t xxx = TMath::Sqrt((seg->Chi2()-0.8)*(seg->Chi2()-0.8)/0.2/0.2+(seg->W()-17.)*(seg->W()-17.)/2./2.);
            QualityValue_Total->Fill(xxx);


        }

        if (gEDBDEBUGLEVEL>2) cout << "I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

        // Important to reset histogram before it is filled.
        histPatternBTDensity->Reset();

        // Search for empty bins, because they can spoil the overall calulation
        // of the mean value.
        Int_t nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
        Int_t nemptybinsXY=0;
        Int_t bincontentXY=0;
        for (int k=1; k<nbins-1; k++) {
            if (eHistYX->GetBinContent(k)==0) {
                ++nemptybinsXY;
                continue;
            }
            bincontentXY=eHistYX->GetBinContent(k);
            histPatternBTDensity->Fill(bincontentXY);
            eProfileBTdens_vs_PID_source->Fill(i,bincontentXY);
        }


        cout << "histPatternBTDensity->GetMean() = " << histPatternBTDensity->GetMean() << endl;

        // failsafe warning in case that there are many bins with zero content.
        // for now we print a error message: TODO  REBIN THE YX HISTOGRA WITH 2.5x2.5 mm!!!!
        CheckFilledXYSize();

        // Save the density in the variable.
        ePatternBTDensity_orig[i]=histPatternBTDensity->GetMean();

    }

    eProfileBTdens_vs_PID_source_meanX=eProfileBTdens_vs_PID_source->GetMean(1);
    eProfileBTdens_vs_PID_source_meanY=eProfileBTdens_vs_PID_source->GetMean(2);
    eProfileBTdens_vs_PID_source_rmsX=eProfileBTdens_vs_PID_source->GetRMS(1);
    eProfileBTdens_vs_PID_source_rmsY=eProfileBTdens_vs_PID_source->GetRMS(2);





    // No assignment for the  eProfileBTdens_vs_PID_target  histogram yet.
    // This will be done in one of the two Execute_ functions.


    // This will be commented when using in batch mode...
    // For now its there for clarity reasons.
    // ...
    TFile* file = new TFile(TString(Form("Histograms_CheckEdbPVRecThetaSpace_aliType_%d.root",AliType)),"RECREATE");
    // Write the Histograms into root file:
    eHistYX->Write();
    eHistChi2W->Write();
    eHistChi2W->Write();
    histPatternBTDensity->Write();
    eProfileBTdens_vs_PID_source->Write();
    histPatternBTDensityTanTheta->Write();
    QualityValue_Chi2->Write();
    QualityValue_Total->Write();
    histPatternBTDensityTanThetaVsPID->Write();
    file->Close();

    TCanvas* c1 = new TCanvas();
    c1->Divide(2,2);
    c1->cd(1);
    eHistYX->DrawCopy("colz");
    c1->cd(2);
    eHistChi2W->DrawCopy("colz");
    c1->cd(3);
    histPatternBTDensity->DrawCopy("");
    c1->cd(4);
    eProfileBTdens_vs_PID_source->Draw("profileZ");
    eProfileBTdens_vs_PID_source->GetXaxis()->SetRangeUser(0,eAli_maxNpatterns+2);
    c1->cd();

    eHistYX->Reset();
    eHistChi2W->Reset();

    TCanvas* c3 = new TCanvas();
    histPatternBTDensityTanTheta->Draw("");

    TCanvas* c5 = new TCanvas();
    QualityValue_Chi2->Draw("");
    TCanvas* c6 = new TCanvas();
    QualityValue_Total->Draw("");

    TCanvas* c4 = new TCanvas();
    histPatternBTDensityTanThetaVsPID->Draw("colz");

    histPatternBTDensityTanTheta->Smooth();
    histPatternBTDensityTanTheta->Smooth();
    histPatternBTDensityTanTheta->Smooth();
    TSpectrum* spec2 = new TSpectrum();
    spec2->Search(histPatternBTDensityTanTheta);

    TList *functions = histPatternBTDensityTanTheta->GetListOfFunctions();
    TPolyMarker *pm = (TPolyMarker*)functions->FindObject("TPolyMarker");
    pm->Print();

    cout << spec2->GetNPeaks() << endl;
    cout << spec2->GetNPeaks() << endl;
    Float_t*	xarr;
    Float_t*	yarr;
    xarr =  spec2-> GetPositionX();
    yarr =  spec2-> GetPositionY();
    cout << xarr[0] << endl;
    cout << xarr[1] << endl;
    cout << yarr[0] << endl;
    cout << yarr[1] << endl;




    Log(2,"EdbPVRQuality::CheckEdbPVRec","EdbPVRQuality::CheckEdbPVRecThetaSpace...done");
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::SetHistGeometry_OPERA()
{
    // Set the geometry of the basetrack density evaluation using OPERA case,
    // European Scanning System size conventions: x=0..125000;y=0..100000).
    // BinArea is 1mmx1mm.
    eHistYX->Reset();
    eHistYX->SetBins(100,0,100000,120,0,120000);
    //cout << "SetHistGeometry_OPERA :binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_MC()
{
    // Set the geometry of the basetrack density evaluation using simulation case,
    // MC ORFEO size conventions: x=-xmax..0..+xmax;y=-ymax..0..ymax).
    // BinArea is 1mmx1mm
    eHistYX->Reset();
    eHistYX->SetBins(100,-50000,50000,100,-50000,50000);
    //cout << "SetHistGeometry_MC :binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_OPERAandMC()
{
    // Set the geometry of the basetrack density evaluation covering MC and DATA case,
    // size conventions: x=-125000..0..+125000;y=-125000..0..125000).
    // BinArea is 1mmx1mm

    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMC","EdbPVRQuality::SetHistGeometry_OPERAandMC");
    eHistYX->Reset();
    eHistYX->SetBins(250,-125000,125000,250,-125000,125000);
    if (gEDBDEBUGLEVEL>2) cout << "SetHistGeometry_OPERAandMC::binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMC","EdbPVRQuality::SetHistGeometry_OPERAandMC...done");
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_OPERAandMCBinArea625()
{
    // Set the geometry of the basetrack density evaluation covering MC and DATA case,
    // size conventions: x=-125000..0..+125000;y=-125000..0..125000).
    // BinArea is 2.5mmx2.5mm

    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMC","EdbPVRQuality::SetHistGeometry_OPERAandMC");
    eHistYX->Reset();
    eHistYX->SetBins(100,-125000,125000,100,-125000,125000);
    if (gEDBDEBUGLEVEL>2) cout << "SetHistGeometry_OPERAandMC::binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMC","EdbPVRQuality::SetHistGeometry_OPERAandMC...done");
    return;
}

//______________________________________________________________________________
void EdbPVRQuality::PrintCutType()
{
    // Print PrintCutType data for this class.
    cout << "EdbPVRQuality::Print()   " << endl;
    cout << "EdbPVRQuality::Print()   eCutMethodIsDone[0] " << eCutMethodIsDone[0] << endl;
    cout << "EdbPVRQuality::Print()   eCutMethodIsDone[1] " << eCutMethodIsDone[1] << endl;
    cout << "EdbPVRQuality::Print()   eCutMethodIsDone[2] " << eCutMethodIsDone[2] << endl;
    if (eCutMethodIsDone[0]) PrintCutType0();
    if (eCutMethodIsDone[1]) PrintCutType1();
    if (eCutMethodIsDone[2]) PrintCutType2();
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Print()
{
    Log(2,"EdbPVRQuality::Print","EdbPVRQuality::Print");
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Print()" << endl;
    cout << "-";
    cout << endl;
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-" << endl;
    cout << "EdbPVRQuality::Print() --- GENERAL SETTINGS: ---" << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethod = " << eCutMethod << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[0] = " << eCutMethodIsDone[0] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[1] = " << eCutMethodIsDone[1] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[2] = " << eCutMethodIsDone[2] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eBTDensityLevel = " << eBTDensityLevel << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eBTDensityLevelCalcMethodMC = " << eBTDensityLevelCalcMethodMC << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eBTDensityLevelCalcMethodMCConfirmationNumber = " << eBTDensityLevelCalcMethodMCConfirmationNumber << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eHistGeometry = " << eHistGeometry << 	endl;
    cout << "-" << endl;
    cout << "EdbPVRQuality::Print() --- SETTINGS: Input data:" << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eAli_orig = " << eAli_orig << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eAli_maxNpatterns = " << eAli_maxNpatterns << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eIsSource = " << eIsSource << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_source_meanX = " << eProfileBTdens_vs_PID_source_meanX << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_source_meanY = " << eProfileBTdens_vs_PID_source_meanY << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_source_rmsX = " << eProfileBTdens_vs_PID_source_rmsX << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_source_rmsY = " << eProfileBTdens_vs_PID_source_rmsY << endl;
    cout << "-" << endl;
    cout << "EdbPVRQuality::Print() --- SETTINGS: Output data:" << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eAli_modified = " << eAli_modified << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eAli_maxNpatterns = " << eAli_maxNpatterns << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eIsTarget = " << eIsTarget << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_target_meanX = " << eProfileBTdens_vs_PID_target_meanX << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_target_meanY = " << eProfileBTdens_vs_PID_target_meanY << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_target_rmsX = " << eProfileBTdens_vs_PID_target_rmsX << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eProfileBTdens_vs_PID_target_rmsY = " << eProfileBTdens_vs_PID_target_rmsY << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eNeedModified = " << eNeedModified << endl;
    cout << "-" << endl;
    cout << "EdbPVRQuality::Print()....done." << endl;
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;

    Log(2,"EdbPVRQuality::Print","EdbPVRQuality::Print...done.");
    return;
}

//______________________________________________________________________________
void EdbPVRQuality::PrintCutType0()
{
    //     Prints quality cut values for each plate of the original EdbPVRec object that were
    //     applied to achieve the basetrack density level, after application the specific cut
    //     on the segments of the specific plate (=pattern).

    if (!eIsSource) return;

    if (!eCutMethodIsDone[0]) {
        cout << "---   EdbPVRQuality::PrintCutType0()----------" << endl;
        cout << "---      This CutType has not been done. Run  Execute_ConstantBTDensity() ---" << endl;
        cout << "---      to see the results (return now). ---" << endl;
        return;
    }

    cout << "----------void EdbPVRQuality::PrintCutType0()----------" << endl;
    cout << "Pattern || Z() || Nseg || eCutp0[i] || eCutp1[i] || BTDensity_orig ||...|| Nseg_modified || BTDensity_modified ||"<< endl;

    if (NULL==eAli_modified) {
        eAli_modified=eAli_orig;
        cout << "WARNING eAli_modified==NULL  ==>>  Take eAli_orig instead. To build eAli_modified please run CreateEdbPVRec(eAli_orig)" << endl;
    }

    int Npat_orig = eAli_orig->Npatterns();
    for (int i=0; i<Npat_orig; i++) {
        EdbPattern* pat_orig = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npatO=pat_orig->N();
        EdbPattern* pat_modified = (EdbPattern*)eAli_modified->GetPattern(i);
        Int_t npatM=pat_modified->N();
        //
        cout << i;
        cout << "	";
        printf("%.1f  %d  %.3f  %.3f  %.2f",pat_orig->Z(),npatO, eCutp0[i], eCutp1[i] , ePatternBTDensity_orig[i]);
        cout << "	...	";
        printf("%.1f  %d  %.3f  %.3f  %.2f",pat_modified->Z(),npatM,  eCutp0[i] ,eCutp1[i],  ePatternBTDensity_modified[i]);
        cout << endl;
    }

    return;
}

//______________________________________________________________________________
void EdbPVRQuality::PrintCutType1()
{
    //     Prints quality cut values for each plate of the original EdbPVRec object that were
    //     applied to achieve the basetrack density level, after application the specific cut
    //     on the segments of the specific plate (=pattern).

    if (!eIsSource) return;
    if (!eCutMethodIsDone[1]) {
        cout << "---   EdbPVRQuality::PrintCutType1()----------" << endl;
        cout << "---      This CutType has not been done. Run  Execute_ConstantQuality() ---" << endl;
        cout << "---      to see the results (return now). ---" << endl;
        return;
    }

    cout << "----------void EdbPVRQuality::PrintCutType1()----------" << endl;

    cout << "Pattern || Z() || Nseg || BTDensity_orig || Chi2CutMeanChi2 || Chi2CutRMSChi2 || Chi2CutMeanW || Chi2CutRMSW || Chi2Cut[i] || BTDensity_modified ||"<< endl;

    if (NULL==eAli_modified) {
        eAli_modified=eAli_orig;
        cout << "WARNING eAli_modified==NULL  ==>>  Take eAli_orig instead. To build eAli_modified please run CreateEdbPVRec(eAli_orig)" << endl;
    }

    int Npat_orig = eAli_orig->Npatterns();
    for (int i=0; i<Npat_orig; i++) {
        EdbPattern* pat_orig = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npatO=pat_orig->N();
        EdbPattern* pat_modified = (EdbPattern*)eAli_modified->GetPattern(i);
        Int_t npatM=pat_modified->N();
        cout << i;
        cout << "	";
        printf("%.1f  %d  %.2f  %.2f  %.2f  %.2f  %.2f  %.2f",pat_orig->Z(),npatO, eAgreementChi2CutMeanChi2 , eAgreementChi2CutRMSChi2,  eAgreementChi2CutMeanW , eAgreementChi2CutRMSW, eAgreementChi2WDistCut[i], ePatternBTDensity_orig[i]);
        cout << "	...	";
        printf("%.1f  %d  %.2f  %.2f  %.2f  %.2f  %.2f  %.2f",pat_modified->Z(),npatM, eAgreementChi2CutMeanChi2 , eAgreementChi2CutRMSChi2,  eAgreementChi2CutMeanW , eAgreementChi2CutRMSW, eAgreementChi2WDistCut[i], ePatternBTDensity_modified[i]);
        cout << endl;
    }
    return;
}


//______________________________________________________________________________


void EdbPVRQuality::PrintCutType2()
{
    cout << " NOT  YET IMPLEMENTED" << endl;
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Execute()
{
    // Main Execution Routine.
    // Does the following things:
    //
    // a) Check EdbPVRec object of data.
    // b) Remove DoubleBasetracks
    // c) Remove passing tracks (in any stored, either in .root file
    //    or in pvr itself)
    // d) Execute quality cuts if necessary: default is Constant BT density
    // e) Store cleaned object. Can be retrieved via GetPVR().

    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Execute " << endl;
    cout << "-";
    cout << endl;
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;

    Log(2,"EdbPVRQuality::Execute","Main Execution Routine.");
    Log(2,"EdbPVRQuality::Execute","Does the following things:");
    Log(2,"EdbPVRQuality::Execute","  a) Check EdbPVRec object of data.");
    Log(2,"EdbPVRQuality::Execute","  b) Remove DoubleBasetracks");
    Log(2,"EdbPVRQuality::Execute","  c) Remove passing tracks (in any stored, either ");
    Log(2,"EdbPVRQuality::Execute","     .root file or in pvr) itself)");
    Log(2,"EdbPVRQuality::Execute","  d) Execute quality cuts if necessary: default is Constant BT density");
    Log(2,"EdbPVRQuality::Execute","  e) Store cleaned object. Can be retrieved via GetPVR().");


    Log(2,"EdbPVRQuality::Execute","Main Execution Routine...done.");
    return;
}



//______________________________________________________________________________

void EdbPVRQuality::Execute_ConstantBTDensity()
{
    // Execute the modified cut routines to achieve the basetrack density level,
    // after application the specific cut on the segments of the specific plate (pattern).
    // The Constant BT Density is defined by the number of BT/mm2 in the histogram.

    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Execute_ConstantBTDensity " << endl;
    cout << "-";
    cout << endl;
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;

    if (!eIsSource) return;
    if (NULL==eAli_orig) return;

    // Check the patterns of the EdbPVRec:
    eAli_maxNpatterns= eAli_orig->Npatterns();
    if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
    if (eAli_maxNpatterns>114) {
        cout << "ERROR!  EdbPVRQuality::Execute_ConstantBTDensity  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);

    // Loop over the patterns:
    for (int i=0; i<eAli_maxNpatterns; i++) {
        if (i>56) {
            cout << "WARNING     EdbPVRQuality::Execute_ConstantBTDensity()    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING     EdbPVRQuality::Execute_ConstantBTDensity()    Check it! " << endl;
        }
        if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantBTDensity   Doing Pattern " << i << endl;

        // Now the condition loop:
        // Loop over 20 steps a 0.15,0.145,0.14 ...  down to 0.07
        for (int l=0; l<20; l++) {
            eHistYX->Reset(); // important to clean the histogram
            eHistChi2W->Reset(); // important to clean the histogram
            histPatternBTDensity->Reset(); // important to clean the histogram

            EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
            Int_t npat=pat->N();
            EdbSegP* seg=0;
            // Loop over the segments.
            for (int j=0; j<npat; j++) {
                seg=pat->GetSegment(j);
                // Very important:
                // For the data case, we assume the following:
                // Data (MCEvt<0) will     be taken for BTdensity calculation
                // Sim (MCEvt>0)      will NOT be taken for BTdensity calculation
                // We take it ONLY and ONLY into account if it is especially wished
                // by the user!
                // Therefore (s)he needs to know how many Gauge Coupling Parameters
                // in the Standard Model exist (at least)...
                Bool_t result=kTRUE;
                if (seg->MCEvt()>0) {
                    if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                        result = kTRUE;
                        // cout << "result = kTRUE !! " << endl;
                    }
                    else {
                        result = kFALSE;
                    }
                }

                if (gEDBDEBUGLEVEL>4)  cout << "Doing segment " << j << " result for bool query is: " << result << endl;

                // Main decision for segment to be kept or not (seg is of MC or data type).
                if ( kFALSE == result ) continue;
                // Constant BT density cut:
                if (seg->Chi2() >= seg->W()* eCutp1[i] - eCutp0[i]) continue;

                eHistYX->Fill(seg->Y(),seg->X());
                eHistChi2W->Fill(seg->W(),seg->Chi2());
            }

            if (gEDBDEBUGLEVEL>2) cout << "I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

            int nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
            for (int k=1; k<nbins-1; k++) {
                if (eHistYX->GetBinContent(k)==0) continue;
                histPatternBTDensity->Fill(eHistYX->GetBinContent(k));
            }

            ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();
            if (gEDBDEBUGLEVEL>2) cout <<"Execute_ConstantBTDensity      Loop l= " << l << ":  for the eCutp1[i] : " << eCutp1[i] <<   "  we have a dens: "  << ePatternBTDensity_modified[i] << endl;

            // Now the condition check:
            if (ePatternBTDensity_modified[i]<=eBTDensityLevel) {
                if (gEDBDEBUGLEVEL>2)  cout << "Execute_ConstantBTDensity      We reached the loop end due to good BT density level ... and break loop." << endl;
                break;
            }
            else {
                // Next step, tighten cut:
                eCutp1[i] += -0.005;
            }

        } // of condition loop...

        // Fill target profile histogram:
        Float_t bincontentXY=histPatternBTDensity->GetMean();
        eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

    } // of Npattern loops..

    eCutMethodIsDone[0]=kTRUE;

    // Check if modified or original PVRec object should be returned:
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() Check if modified or original PVRec object should be returned: " << endl;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() " << eNeedModified << endl;
    }


    // This will be commented when using in batch mode...
    // For now its there for clarity reasons.
    TCanvas* c1 = new TCanvas();
    c1->Divide(2,2);
    c1->cd(1);
    eHistYX->DrawCopy("colz");
    c1->cd(2);
    eHistChi2W->DrawCopy("colz");
    c1->cd(3);
    histPatternBTDensity->DrawCopy("");
    c1->cd(4);
    eProfileBTdens_vs_PID_source->Draw("profileZ");
    eProfileBTdens_vs_PID_source->GetXaxis()->SetRangeUser(0,eAli_maxNpatterns+2);
    eProfileBTdens_vs_PID_target->SetLineStyle(2);
    eProfileBTdens_vs_PID_target->Draw("profileZsame");
    c1->cd();
    histPatternBTDensity->Reset();
    eHistYX->Reset();
    eHistChi2W->Reset();


    eProfileBTdens_vs_PID_source_meanX=eProfileBTdens_vs_PID_source->GetMean(1);
    eProfileBTdens_vs_PID_source_meanY=eProfileBTdens_vs_PID_source->GetMean(2);
    eProfileBTdens_vs_PID_source_rmsX=eProfileBTdens_vs_PID_source->GetRMS(1);
    eProfileBTdens_vs_PID_source_rmsY=eProfileBTdens_vs_PID_source->GetRMS(2);

    eProfileBTdens_vs_PID_target_meanX=eProfileBTdens_vs_PID_target->GetMean(1);
    eProfileBTdens_vs_PID_target_meanY=eProfileBTdens_vs_PID_target->GetMean(2);
    eProfileBTdens_vs_PID_target_rmsX=eProfileBTdens_vs_PID_target->GetRMS(1);
    eProfileBTdens_vs_PID_target_rmsY=eProfileBTdens_vs_PID_target->GetRMS(2);

    delete histPatternBTDensity;

    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity()....done." << endl;
    return;
}

//______________________________________________________________________________


void EdbPVRQuality::Execute_ConstantQuality()
{
    // The cut method that compares the passing muon tracks with all scanned segments.
    // This may help to improve, since it takes into account the actual segment quality,
    // which varies from scan to scan anyway.
    // Works for tracks passing the volume to extract their mean chi2/W. Then a distance
    // measurement Sqrt{0.5*[ ((chi2-chi2mean)/chi2rms)^2 + ((W-Wmean)/Wrms)^2 )] } is
    // build and the value of single basetracks is compared to this variable. Starting from
    // dist_max = 3 going down until desired target level is achieved.
    // If eAli->Tracks is there we take them from there.
    // If not, we try if there is a file linked_tracks.root, and we take tracks from there.
    // If that doesnt work either, nothing is done.

    if (!eIsSource) return;

    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Execute_ConstantQuality " << endl;
    cout << "-";
    cout << endl;
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "----------    Works for tracks passing the volume to extract their mean chi2/W " << endl;
    cout << "----------    If eAli->Tracks is there we take them from there." << endl;
    cout << "----------    If not, we try if there is a file linked_tracks.root " << endl;
    cout << "----------    we take tracks from there. " << endl;
    cout << "----------    If that doesnt work either, nothing is done." << endl;
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;

    cout << "EdbPVRQuality::Execute_ConstantQuality()   eAli_orig.eTracks :" << eAli_orig->eTracks << endl;

    Float_t meanChi2=0.5;
    Float_t rmsChi2=0.2;
    Float_t meanW=22;
    Float_t rmsW=4;
    Float_t agreementChi2=100;

    // No  eAli.Tracks ? Look for tracks in linked_track.root:
    if (NULL == eAli_orig->eTracks) {
        cout << "EdbPVRQuality::Execute_ConstantQuality()   No eAli.Tracks. Look for tracks in linked_track.root" << endl;
        TFile* trackfile = new TFile("linked_tracks.root");
        trackfile->ls();
        TTree* tracks = (TTree*)trackfile->Get("tracks");
        if (NULL == tracks) {
            cout << "EdbPVRQuality::Execute_ConstantQuality()   No tracks in linked_track.root file. Return, leave eAli_orig unchanged and dont do any cleaning. You might try Execute_ConstantBTDensity instead. " << endl;
            return;
        }
        // 		TH1F* h1; Attention the usage of  _npl_ is disvavoured
        // 		because this can cause problems.
        tracks->Draw("nseg>>h(60,0,60)","","");
        TH1F *h1 = (TH1F*)gPad->GetPrimitive("h");

        // Short implementation of getting the last bin filled:
        int lastfilledbin=0;
        for (int k=1; k<h1->GetNbinsX()-1; k++) {
            if (h1->GetBinContent(k)>0) lastfilledbin=k;
        }

        Float_t travellenghtpercentage=Float_t(lastfilledbin)/Float_t(eAli_maxNpatterns);

        TString cutstring = TString(Form("nseg>=%d",int(h1->GetBinCenter(lastfilledbin-3)) ));
        tracks->Draw("s.eChi2>>hChi2(100,0,2)",cutstring);
        TH1F *hChi2 = (TH1F*)gPad->GetPrimitive("hChi2");

        cout << "EdbPVRQuality::Execute_ConstantQuality()   Found tracks in the file! Good. Info:" << endl;
        cout << "EdbPVRQuality::Execute_ConstantQuality()   The long tracks in the file have an average percentage length of:" << travellenghtpercentage  <<  endl;
        cout << "EdbPVRQuality::Execute_ConstantQuality()   Mean(RMS) of Chi2 distribution of passing tracks: " << hChi2->GetMean() << "+-"  << hChi2->GetRMS() << endl;



        TCanvas* c1 = new TCanvas();
        c1->cd();
        tracks->Draw("s.eW>>hW(50,0,50)",cutstring);
        TH1F *hW = (TH1F*)gPad->GetPrimitive("hW");
        cout << "EdbPVRQuality::Execute_ConstantQuality()   Mean(RMS) of W distribution of passing tracks: " << hW->GetMean() << "+-"  << hW->GetRMS() << endl;

        meanChi2=hChi2->GetMean();
        rmsChi2=hChi2->GetRMS();
        meanW=hW->GetMean();
        rmsW=hW->GetRMS();

        // Quick check if these values are reasonable:
        if (TMath::Abs(meanChi2-0.5)>1 || TMath::Abs(meanW-22)>6) {
            cout << "WARNING   EdbPVRQuality::Execute_ConstantQuality()   The tracks might have a strange distribution. You are urgently requested to check these manually!!!"<<endl;
        }
        else {
            cout << "EdbPVRQuality::Execute_ConstantQuality()   The tracks have reasonable values."<<endl;
        }


        // since the values for the passing tracks are
        // calculated for the whole volume, we assume that the cutvalues
        // are valid for all plates.
        eAgreementChi2CutMeanChi2=meanChi2;
        eAgreementChi2CutRMSChi2=rmsChi2;
        eAgreementChi2CutMeanW=meanW;
        eAgreementChi2CutRMSW=rmsW;
    }

    /// ______  now same code as in the function Execute_ConstantBTDensity  ___________________

    // Check the patterns of the EdbPVRec:
    eAli_maxNpatterns= eAli_orig->Npatterns();
    cout << "EdbPVRQuality::Execute_ConstantQuality  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << endl;
    if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
    if (eAli_maxNpatterns>114) {
        cout << "ERROR!  EdbPVRQuality::Execute_ConstantQuality  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",100,0,200);
    TH1F* histagreementChi2 = new TH1F("histagreementChi2","histagreementChi2",100,0,5);


    cout << "Execute_ConstantQuality   Loop over the patterns..." << endl;
    for (int i=0; i<eAli_maxNpatterns; i++) {
        if (i>56) {
            cout << "ERROR     EdbPVRQuality::Execute_ConstantQuality() Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "ERROR     EdbPVRQuality::Execute_ConstantQuality() Check it! " << endl;
        }

        if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantQuality   Doing Pattern " << i << endl;

        // Now the condition loop:
        // Loop over 30 steps agreementChi2 step 0.025
        for (int l=0; l<30; l++) {

            if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantQuality   Doing condition loop = " << l << endl;

            eHistYX->Reset(); // important to clean the histogram
            eHistChi2W->Reset(); // important to clean the histogram
            histPatternBTDensity->Reset(); // important to clean the histogram

            EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
            Int_t npat=pat->N();
            EdbSegP* seg=0;

            if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantQuality   Loop over segments of pattern " << i << ",Number of segments= " << npat <<  endl;
            for (int j=0; j<npat; j++) {
                seg=pat->GetSegment(j);

                if (gEDBDEBUGLEVEL>4) cout << "Execute_ConstantQuality   Doing segment= " << j << endl;
                // Very important:
                // For the data case, we assume the following:
                // Data (MCEvt<0) will     be taken for BTdensity calculation
                // Sim (MCEvt>0)      will NOT be taken for BTdensity calculation
                // We take it ONLY and ONLY into account if it is especially wished
                // by the user!
                // Therefore (s)he needs to know how many Gauge Coupling Parameters
                // in the Standard Model exist (at least)...
                Bool_t result=kTRUE;
                if (seg->MCEvt()>0) {
                    if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                        result = kTRUE;
                        // cout << "result = kTRUE !! " << endl;
                    }
                    else {
                        result = kFALSE;
                    }
                }

                if (gEDBDEBUGLEVEL>4)  cout << "Doing segment " << j << " result for bool query is: " << result << endl;

                // Main decision for segment to be kept or not (seg is of MC or data type).
                if ( kFALSE == result ) continue;

                // Change here to the quality with values obtained from the tracks.
                // Constant BT quality cut:
                agreementChi2=TMath::Sqrt(0.5 *( (seg->Chi2()-meanChi2)/rmsChi2)*((seg->Chi2()-meanChi2)/rmsChi2)  +   ((seg->W()-meanW)/rmsW)*((seg->W()-meanW)/rmsW) );
                histagreementChi2->Fill(agreementChi2);


                // Constant BT quality cut:
                if (agreementChi2>eAgreementChi2WDistCut[i]) continue;

                eHistYX->Fill(seg->Y(),seg->X());
                eHistChi2W->Fill(seg->W(),seg->Chi2());
            }

            if (gEDBDEBUGLEVEL>2) cout << "I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

            Int_t nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
            Int_t nemptybinsXY=0;
            for (int k=1; k<nbins-1; k++) {
                if (eHistYX->GetBinContent(k)==0) {
                    ++nemptybinsXY;
                    continue;
                }
                histPatternBTDensity->Fill(eHistYX->GetBinContent(k));
            }

            // failsafe warning in case that there are many bins with zero content.
            // for now we print a error message: TODO  REBIN THE YX HISTOGRA WITH 2.5x2.5 mm!!!!
            CheckFilledXYSize();

            ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();

            if (gEDBDEBUGLEVEL>2) cout <<"Execute_ConstantQuality      Loop l= " << l << ":  for the eAgreementChi2WDistCut : " << eAgreementChi2WDistCut[i] <<   "  we have a dens: "  << ePatternBTDensity_modified[i] << endl;

            // Now the condition check:
            if (ePatternBTDensity_modified[i]<=eBTDensityLevel) {
                if (gEDBDEBUGLEVEL>2)  cout << "Execute_ConstantQuality      We reached the loop end due to good BT density level ... and break loop." << endl;
                // But dont forget to set values:
                eCutDistChi2[i]=meanChi2;
                eCutDistW[i]=meanW;
                eAgreementChi2CutMeanChi2=meanChi2;
                eAgreementChi2CutRMSChi2=rmsChi2;
                eAgreementChi2CutMeanW=meanW;
                eAgreementChi2CutRMSW=rmsW;
                break;
            }
            else {
                // Next step, tighten cut:
                eAgreementChi2WDistCut[i]+=  -0.025;
            }

        } // of condition loop...

        // Fill target profile histogram:
        Float_t bincontentXY=histPatternBTDensity->GetMean();
        cout << "Execute_ConstantQuality   histPatternBTDensity->GetMean()." << histPatternBTDensity->GetMean() <<  endl;
        eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

    } // of Npattern loops..
    cout << "Execute_ConstantQuality   Loop over the patterns...done." << endl;

    // Check if modified or original PVRec object should be returned:
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "----------void EdbPVRQuality::Execute_ConstantQuality() Check if modified or original PVRec object should be returned: " << endl;
        cout << "----------void EdbPVRQuality::Execute_ConstantQuality() " << eNeedModified << endl;
    }

    eCutMethodIsDone[1]=kTRUE;

    // This will be commented when using in batch mode...
    // For now its there for clarity reasons.
    TCanvas* c1 = new TCanvas();
    c1->Divide(2,2);
    c1->cd(1);
    eHistYX->DrawCopy("colz");
    c1->cd(2);
    eHistChi2W->DrawCopy("colz");
    c1->cd(3);
    histPatternBTDensity->DrawCopy("");
    c1->cd(4);
    eProfileBTdens_vs_PID_source->Draw("profileZ");
    eProfileBTdens_vs_PID_source->GetXaxis()->SetRangeUser(0,eAli_maxNpatterns+2);
    eProfileBTdens_vs_PID_target->SetLineStyle(2);
    eProfileBTdens_vs_PID_target->Draw("profileZsame");
    c1->cd();

    eProfileBTdens_vs_PID_source_meanX=eProfileBTdens_vs_PID_source->GetMean(1);
    eProfileBTdens_vs_PID_source_meanY=eProfileBTdens_vs_PID_source->GetMean(2);
    eProfileBTdens_vs_PID_source_rmsX=eProfileBTdens_vs_PID_source->GetRMS(1);
    eProfileBTdens_vs_PID_source_rmsY=eProfileBTdens_vs_PID_source->GetRMS(2);

    eProfileBTdens_vs_PID_target_meanX=eProfileBTdens_vs_PID_target->GetMean(1);
    eProfileBTdens_vs_PID_target_meanY=eProfileBTdens_vs_PID_target->GetMean(2);
    eProfileBTdens_vs_PID_target_rmsX=eProfileBTdens_vs_PID_target->GetRMS(1);
    eProfileBTdens_vs_PID_target_rmsY=eProfileBTdens_vs_PID_target->GetRMS(2);

    histPatternBTDensity->Reset();
    eHistYX->Reset();
    eHistChi2W->Reset();
    delete histPatternBTDensity;

    TCanvas* c2 = new TCanvas();
    histagreementChi2->Draw();

    cout << "----------void EdbPVRQuality::Execute_ConstantQuality() Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantQuality() If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;

    cout << "----------void EdbPVRQuality::Execute_ConstantQuality()....done." << endl;
    return;
}

//___________________________________________________________________________________

/// TO BE RE-WRITTEN FROM Akitaka Ariga Code:

void EdbPVRQuality::Execute_ConstantBTDensityInAngularBins()
{
    // Execute the modified cut routines to achieve the basetrack density level,
    // after application the specific cut on the segments of the specific plate (pattern).
    // The Constant BT Density is defined by the number of BT/mm2 in the histogram.
    // Inbetween loop over tan theta bins additionally!

    cout << "EdbPVRQuality::Execute_ConstantBTDensityInAngularBins " << endl;


    // We need to set the binareasize larger, since we divide into thata bins itsself
    cout << "EdbPVRQuality::Execute_ConstantBTDensityInAngularBins    We need to set the binareasize larger, since we divide into thata bins itsself" << endl;
    cout << "EdbPVRQuality::Execute_ConstantBTDensityInAngularBins    SetHistGeometry_OPERAandMCBinArea625()" << endl;
    SetHistGeometry_OPERAandMCBinArea625();

    if (!eIsSource) return;
    if (NULL==eAli_orig) return;

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);
    TH1F* histPatternBTDensities[20]; // for the angular bins:
    // memory creation done down there...
    for (Int_t angspacecounter=0; angspacecounter<20; angspacecounter++) {
        histPatternBTDensities[angspacecounter] = new TH1F("histPatternBTDensities","histPatternBTDensities",200,0,200);
    }

//     Double_t angularspacebinningwidth=0.05;
    Double_t angularspacebinningwidth=0.1;
    Double_t angularspacebinningstart=0.00;
    Double_t angularspacebinningend=0.00;
    Double_t angularspacebinningScaleFactor=1;
    Double_t tt=0;
    Double_t ni[20];

    Double_t NumberOfFilledTTbins=0;


    //---------------------------------------------------------------------
    // Loop over the patterns:
//     for (int i=0; i<eAli_maxNpatterns; i++) {
    for (int i=0; i<1; i++) {

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();
        cout << " For this (" << i << ") pattern, I have " << npat << " Edb segments to check..." << endl << endl << endl;





        // Reset eBTDensity levels:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
            eBTDensityLevelAngularSpace[angspacecounter]=eBTDensityLevel;
            // Maximum Cut Value for const, BT dens, also in tangens theta space
            eCutTTp0[i][angspacecounter]=1.00;
            eCutTTp1[i][angspacecounter]=0.15;
        }




        // Now the angular space binning loop:
//  	for (Int_t angspacecounter=0; angspacecounter<20; angspacecounter++) {
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
            cout << endl << "Doing // Now the angspacecounter loop:  angspacecounter = " << angspacecounter << endl << endl;

            // Calculate right end of angular space bin
            angularspacebinningstart=(Double_t)angspacecounter*angularspacebinningwidth;
            angularspacebinningend=angularspacebinningstart+angularspacebinningwidth;


            // Now the condition loop:
            Int_t lmax=40;
            // Loop over 20 ( or 40 ) steps a 0.15,0.145,0.14 ...  down to 0.07
//        for (int l=0; l<40; l++) {
            for (Int_t l=0; l<lmax; l++) {
                cout << "Doing // Now the condition loop:  l = " << l << endl;

                // Shorter Handings:
                Double_t Cutp0 = eCutTTp0[i][angspacecounter];
                Double_t Cutp1 = eCutTTp1[i][angspacecounter];

                cout << "For this l, the cut condition reads:  seg->Chi2() >= seg->W()* " << Cutp1 << " - " << Cutp0 << endl;


                // Important to clean the histograms:
                eHistYX->Reset(); // important to clean the histogram
                eHistYX->SetMinimum(0);
                eHistYX->SetMaximum(250);
                eHistChi2W->Reset(); // important to clean the histogram
                histPatternBTDensity->Reset(); // important to clean the histogram
                eHistTT->Reset();
                histPatternBTDensities[angspacecounter]->Reset(); // important to clean the histogram

                // Reset Number of Segments Counter
                ni[angspacecounter]=0;

                // Now Get Pattern:
                EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
                Int_t npat=pat->N();
                EdbSegP* seg=0;

                // Loop over the segments.
                for (int j=0; j<npat; j++) {
                    seg=pat->GetSegment(j);
                    Bool_t result=kTRUE;
                    if (seg->MCEvt()>0) {
                        if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                            result = kTRUE;
                            // cout << "result = kTRUE !! " << endl;
                        }
                        else {
                            result = kFALSE;
                        }
                    }

                    // Main decision for segment to be kept or not (seg is of MC or data type).
                    if ( kFALSE == result ) continue;
                    // Calculate Angle
                    tt=TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX());

                    // Check if angle is in the desired angular space bin:
                    Int_t segttspacebin=GetAngularSpaceBin(seg);
                    if (segttspacebin!=angspacecounter)  continue;

                    // Constant BT density cut for angular space:
                    if (seg->Chi2() >= seg->W()* eCutTTp1[i][angspacecounter] - eCutTTp0[i][angspacecounter]) continue;

                    // Increase counter for number of taken segments in tan theta bin:
                    ni[angspacecounter] += 1;
                    // Fill other histograms anyway
                    eHistYX->Fill(seg->Y(),seg->X());
                    eHistTYTX->Fill(seg->TY(),seg->TX());
                    eHistTT->Fill(tt);
                    eHistChi2W->Fill(seg->W(),seg->Chi2());
                } // Loop over the segments.



                if (l==0) {
                    // Calulate the Scale Factor for Calculation  for the zeroth cut only
                    angularspacebinningScaleFactor=Double_t(ni[angspacecounter]) / Double_t(npat); //  NumberOfFilledTTbins;
                    // Calculate the max BT level for the zeroth cut only, otherwise its gonna change!
                    eBTDensityLevelAngularSpace[angspacecounter]=TMath::Power((1.-TMath::Power(angularspacebinningScaleFactor,1)),1)*eBTDensityLevel;


                    /// TODO RECALCULATE THIS APPROACH
                    /// alternative approach:
// 	      angularspacebinningScaleFactor= Double_t(npat) / Double_t(1+ni[angspacecounter]) ;
//  	      eBTDensityLevelAngularSpace[angspacecounter]=angularspacebinningScaleFactor*eBTDensityLevel;

// 	      if (angularspacebinningScaleFactor<0.01) {
// 		angularspacebinningScaleFactor=0.01;
// 		eBTDensityLevelAngularSpace[angspacecounter]=1.0;
// 	      }
                }



                /// TODO MAKE THIS A NICE OUTPUT
                if (gEDBDEBUGLEVEL>2) {
                    cout << "I have filled the eHistYX Histogram. Entries for specified theta range (" << angularspacebinningstart << "..." << angularspacebinningend <<"): = " << eHistYX->GetEntries() << "  ni[angspacecounter]= " << ni[angspacecounter] << endl;
                    cout << "The ScaleFactor is therefore defined by: angularspacebinningScaleFactor= nEntries(per bin) / nEntries (alle bins) = " << angularspacebinningScaleFactor <<  endl;
                    cout << "The ScaleTargetDensity is therefore defined by: angularspacebinningScaleFactor* BTTarget density(no qual-cut) =  eBTDensityLevelAngularSpace[angspacecounter] = " << eBTDensityLevelAngularSpace[angspacecounter] <<  endl;
                }


                /// TODO CHECK THIS LATER IF THE OUTPTU HERE IS CORRECT  ///----------------------------------------------------
                /// ---  TEST
// 	      gEDBDEBUGLEVEL=4;
// 	      CheckFilledXYSize();
// 	      gEDBDEBUGLEVEL=1;
                ///----------------------------------------------------

                int nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
                for (int k=1; k<nbins-1; k++) {
                    if (eHistYX->GetBinContent(k)==0) continue;
                    histPatternBTDensity->Fill(Double_t(eHistYX->GetBinContent(k))/ 6.25 );// due to the changed areasize in eHistYX
                    histPatternBTDensities[angspacecounter]->Fill(eHistYX->GetBinContent(k)/ 6.25);// due to the changed areasize in eHistYX
                }

                ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();



                // leave also if only one entry in the histogram
                if (ePatternBTDensity_modified[i]<=1) l=lmax;


//             if (gEDBDEBUGLEVEL>2)
                cout <<"Execute_ConstantBTDensity      Loop l= " << l << ":  for the Cutp1 : " << Cutp1 <<   "  we have a dens: "  << ePatternBTDensity_modified[i] << endl;


                cout << "Now we compare this density with the desired density for the angular space bin:  eBTDensityLevelAngularSpace[angspacecounter] = " <<  eBTDensityLevelAngularSpace[angspacecounter] <<  endl;

                cout <<  ePatternBTDensity_modified[i] << "  ... " <<  eBTDensityLevelAngularSpace[angspacecounter] << endl;
                if (ePatternBTDensity_modified[i]<=eBTDensityLevelAngularSpace[angspacecounter]) {
                    cout << " is .....    <  " << endl;
                }
                else {
                    cout << " is .....    >  " << endl;
                }


                // Now the condition check for angular space cut
                if (ePatternBTDensity_modified[i]<=eBTDensityLevelAngularSpace[angspacecounter]) {
                    if (gEDBDEBUGLEVEL>2)  ;
                    l=lmax;
                    cout << "Execute_ConstantBTDensity      We reached the loop end due to good BT density level in this angular bin ... and finish loop.  set l to lmax: " << l << endl;


                    ///----------------------------------------------------
                    /// ---  TEST DRAWINGS .....
                    if (gEDBDEBUGLEVEL>1) {
                        TCanvas* ch = new TCanvas();
                        ch->Divide(3,1);
                        ch->cd(1);
                        eHistYX->DrawCopy("colz");
                        ch->cd(2);
                        histPatternBTDensity->DrawCopy("");
                        ch->cd(3);
			double scalefac=eHistTT->Integral();
			if (eHistTT->Integral()==0) scalefac=1;
			eHistTT->Scale(1./scalefac);
                        eHistTT->DrawCopy("");
                        ch->cd();
                    }

                    /// ---  TEST DRAWINGS ..... END

//                 break; // no need to brake anymore since we set l to lmax....
                }
                else {
                    cout << "The actual BT density in angular region is still too big, we have to tighten the cut: " << endl;
                    cout << "  Make one more time   eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.0025; " << endl;

                    // Next step, tighten cut:
                    // l=20:
//                eCutp1[i] += -0.005;
// 		eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.005;
                    // l=40:
                    eCutp1[i] += -0.0025;
                    eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.0025;
                    if (l==lmax-1) cout << "--------   SORRY, BUT WE AINT GONNA TIGHTEN UP MERE CUTS; NOW YOU GOTTA LIVE WITH THAT!" << endl;
                }

            } // of condition loop...




            // Fill target profile histogram:
            Float_t bincontentXY=histPatternBTDensity->GetMean();
            eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

            angularspacebinningstart+=angularspacebinningwidth;
        } // for (int angspacecounter=0; angspacecounter<20; angspacecounter++)

    } // of Npattern loops..
    //---------------------------------------------------------------------

    eCutMethodIsDone[2]=kTRUE;




    // Check if modified or original PVRec object should be returned:
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() Check if modified or original PVRec object should be returned: " << endl;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() " << eNeedModified << endl;
    }




    ///-----------------------------------------------------------------------------------------
    cout <<"--------------------------------------------------------------------" << endl;
    for (int i=0; i<1; i++) {
        // npatterns of eali...
        for (int angspacecounter=0; angspacecounter<10; angspacecounter++) {
            printf(" %1.3f ", eCutTTp1[i][angspacecounter]);
        }
        cout << endl;
    }
    cout <<"--------------------------------------------------------------------" << endl;
    ///-----------------------------------------------------------------------------------------

    delete histPatternBTDensity;

    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensityInAngularBins() Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensityInAngularBins() If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensityInAngularBins()....done." << endl;
    return;

 
    
    ///-----------------------------------------------------------------------------------------
    /*
    // Get Your Volume
TFile* f = new TFile("ScanVolume_Ali.root","READ");
EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");

// Create Your Cleaning Instance
EdbPVRQuality* qc = new EdbPVRQuality(ali);

// Start Cleaning in Angular Bins now (NEW!)
qc->Execute_ConstantBTDensityInAngularBins();

// Create with the cuts obtained now the new volume.
qc->CreateEdbPVRec();    

// Get the new Volume back.
ali2=qc->GetEdbPVRec(1);


// Create a new Cleaning Instance and see new values of new volume
EdbPVRQuality* ww = new EdbPVRQuality(ali2);
*/
///-----------------------------------------------------------------------------------------
}

//___________________________________________________________________________________

Int_t EdbPVRQuality::GetAngularSpaceBin(EdbSegP* seg)
{

    // Returns the number of bin for which the Basetrack corresponds to in tangens theta space.
// Goes from 0 to 20.

// ATTENTION and TODO :
// Make the values in GetAngularSpaceBin() and Execute_ConstantBTDensityInAngularBins() equal !

// done by hand... should by donesomehow automatic

//  Double_t angularspacebinningwidth=0.05;
    Double_t angularspacebinningwidth=0.1;
    Double_t angularspacebinningstart=0.00;
    Double_t angularspacebinningend=0.00;

// Calculate Angle
    Double_t tt=TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX());
//  cout << "Int_t EdbPVRQuality::GetAngularSpaceBin(EdbSegP* seg)   tt= "  << tt << endl;

    // Now the angular space binning loop:
    for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {

        // Calculate right end of angular space bin
        angularspacebinningstart=(Double_t)angspacecounter*angularspacebinningwidth;
        angularspacebinningend=angularspacebinningstart+angularspacebinningwidth;

        // Check if angle is in the desired angular space bin
        if (tt<angularspacebinningstart) continue;
        if (tt>=angularspacebinningend) continue;



//  	cout << "Int_t EdbPVRQuality::GetAngularSpaceBin(EdbSegP* seg)   return angspacecounter = " << angspacecounter  << " now." << endl;
        return angspacecounter;
    }

    // failsaife value, should never be given back:
    return 0;
}

//___________________________________________________________________________________

Bool_t EdbPVRQuality::CheckSegmentQualityInPattern_ConstBTDens(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg)
{
    // Core function to check if a basetrack of the specific pattern matches the expectations
    // for the desired quality cut (calculated on the estimations in CheckEdbPVRec(). ).
    // Implementation for the Cuttype 0: Constant BT Density
    // ---
    // Note: since the eCutp1[i] values are calculated with
    // this pattern->At() scheme labelling,
    // its not necessarily guaranteed that seg->PID gives correct this
    // number back. Thats why we have to give the PatternAtNr here again.
    //
    // And: it is not checked here if seg is contained in this specific
    // pattern. It looks only for the quality cut!
    if (gEDBDEBUGLEVEL>3)  cout << "seg->W()* eCutp1[PatternAtNr] - eCutp0[PatternAtNr] = " << seg->W()* eCutp1[PatternAtNr] - eCutp0[PatternAtNr] << endl;

    // Constant BT density cut:
    if (seg->Chi2() >= seg->W()* eCutp1[PatternAtNr] - eCutp0[PatternAtNr]) return kFALSE;

    if (gEDBDEBUGLEVEL>3) cout <<"EdbPVRQuality::CheckSegmentQualityInPattern_ConstBTDens()   Segment " << seg << " has passed ConstBTDens cut!" << endl;
    return kTRUE;
}

//___________________________________________________________________________________

Bool_t EdbPVRQuality::CheckSegmentQualityInPattern_ConstQual(EdbPVRec* ali, Int_t PatternAtNr, EdbSegP* seg)
{
    // Core function to check if a basetrack of the specific pattern matches the expectations
    // for the desired quality cut (calculated on the estimations in CheckEdbPVRec(). ).
    // Implementation for the Cuttype 1: Constant Quality.
    // ---
    // See comments in CheckSegmentQualityInPattern_ConstBTDens
    // Constant BT quality cut:
    Float_t agreementChi2=TMath::Sqrt( ( (seg->Chi2()-eAgreementChi2CutMeanChi2)/eAgreementChi2CutRMSChi2)*((seg->Chi2()-eAgreementChi2CutMeanChi2)/eAgreementChi2CutRMSChi2)  +   ((seg->W()-eAgreementChi2CutMeanW)/eAgreementChi2CutRMSW)*((seg->W()-eAgreementChi2CutMeanW)/eAgreementChi2CutRMSW) );
    if (agreementChi2>eAgreementChi2WDistCut[PatternAtNr]) return kFALSE;

    if (gEDBDEBUGLEVEL>3) cout <<"EdbPVRQuality::CheckSegmentQualityInPattern_ConstQual()   Segment " << seg << " has passed ConstQual cut!" << endl;
    return kTRUE;
}

//___________________________________________________________________________________


void EdbPVRQuality::CreateEdbPVRec()
{
    // Creates the cleaned EdbPVRec object, containing only segments that
    // satisfied the cutcriteria.
    // Attention: the couples structure and the tracking structure will be lost,
    // this EdbPVRec object is only useful for the list of segments (shower reco).
    // DO NOT USE THIS ROUTINE FOR GENERAL I/O and/or GENERAL EdbPVRec operations!

    cout << "-----     void EdbPVRQuality::CreateEdbPVRec()     -----" << endl;
    if (gEDBDEBUGLEVEL>2) {
        cout << "-----     " << endl;
        cout << "-----     This function makes out of the original eAli" << endl;
        cout << "-----     a new EdbPVRec object having only those seg-" << endl;
        cout << "-----     ments in it which satisfy the cutcriteria " << endl;
        cout << "-----     determined in Execute_ConstantBTDensity, Execute_ConstantQuality" << endl;
        cout << "-----     " << endl;
        cout << "-----     WARNING: the couples structure and the tracking structure" << endl;
        cout << "-----     will be lost, this PVR object is only useful for the" << endl;
        cout << "-----     list of Segments (==ShowReco...) ... " << endl;
        cout << "-----     DO NOT USE THIS ROUTINE FOR GENERAL I/O and/or EdbPVRec operations!" << endl;
        cout << "-----     " << endl;
        cout << "CreateEdbPVRec()  Mode 0:" << eCutMethodIsDone[0] << endl;
        cout << "CreateEdbPVRec()  Mode 1:" << eCutMethodIsDone[1] << endl;
        cout << "CreateEdbPVRec()  Mode 2:" << eCutMethodIsDone[2] << endl;
        cout << "-----     " << endl;
        cout << "-----     ----------------------------------------------" << endl;
    }

    // Checks
    if (NULL==eAli_orig || eIsSource==kFALSE) {
        cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    NULL==eAli_orig   || eIsSource==kFALSE    return."<<endl;
        return;
    }
    // Checks
    if (eCutMethodIsDone[0]==kFALSE && eCutMethodIsDone[1]==kFALSE && eCutMethodIsDone[2]==kFALSE ) {
        cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    eCutMethodIsDone[0]==kFALSE && eCutMethodIsDone[1]==kFALSE && eCutMethodIsDone[2]==kFALSE  return."<<endl;
        return;
    }

    // Make a new PVRec object anyway
    eAli_modified = new EdbPVRec();

    // These two lines dont compile yet ... (???) ...
    // 	EdbScanCond* scancond = eAli_orig->GetScanCond();
    // 	eAli_modified->SetScanCond(*scancond);

    Float_t agreementChi2;
    Int_t angularspacebin=0;

    // This makes pointer copies of patterns with segments list.
    // wARNING: the couples structure and the tracking structure
    // will be lost, this PVR object is only useful for the
    // list of Segments (==ShowReco...) ...

    // Priority has the Execute_ConstantQuality cut.
    // If this was done we take this...:

    // Loop over patterns
    for (int i = 0; i <eAli_orig->Npatterns(); i++ ) {
        EdbPattern* pat = eAli_orig->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());

        // Loop over segments
        for (int j = 0; j <pat->N(); j++ ) {
            EdbSegP* seg = pat->GetSegment(j);

            // Put here the cut condition ...
            if (eCutMethodIsDone[0]==kTRUE && eCutMethodIsDone[1]==kFALSE) {
                // Constant BT density cut:
                if (seg->Chi2() >= seg->W()* eCutp1[i] - eCutp0[i]) continue;
            }
            else if (eCutMethodIsDone[1]==kTRUE) {
                // Constant Quality cut:
                agreementChi2=TMath::Sqrt( ( (seg->Chi2()-eAgreementChi2CutMeanChi2)/eAgreementChi2CutRMSChi2)*((seg->Chi2()-eAgreementChi2CutMeanChi2)/eAgreementChi2CutRMSChi2)  +   ((seg->W()-eAgreementChi2CutMeanW)/eAgreementChi2CutRMSW)*((seg->W()-eAgreementChi2CutMeanW)/eAgreementChi2CutRMSW) );
                if (agreementChi2>eAgreementChi2WDistCut[i]) continue;
            }
            else if (eCutMethodIsDone[2]==kTRUE) {
                // Constant BT density cut also in angular space:
                angularspacebin = GetAngularSpaceBin(seg);
                /*
                seg->PrintNice();
                cout << " angularspacebin = " <<   angularspacebin << endl;
                cout << " eCutTTp0[i][angularspacebin] = " <<   eCutTTp1[i][angularspacebin] << endl;
                cout << " eCutTTp1[i][angularspacebin] = " <<   eCutTTp1[i][angularspacebin] << endl;
                */
                if (seg->Chi2() >= seg->W()* eCutTTp1[i][angularspacebin] - eCutTTp0[i][angularspacebin]) continue;
                /// ...............
            }
            else {
                // do nothing;
                cout << "WARNING! YOU CHOSE AN INVALID OPTION. DO NO CUT AT ALL! TAKE ALL SEGMENTS!" << endl;
            }

            // Add segment:
            pt->AddSegment(*seg);
        }
        eAli_modified->AddPattern(pt);
    }

    eIsTarget=kTRUE;

    //---------------------
    cout << "-----     void EdbPVRQuality::CreateEdbPVRec()...Created new EdbPVR object at address " << eAli_modified << endl;
    cout << "-----     void EdbPVRQuality::CreateEdbPVRec()...You can retrieve this object via   ->GetEdbPVRec(1);" << endl;
    //---------------------
    cout << "-----     void EdbPVRQuality::CreateEdbPVRec()...done." << endl;
    return;
}




//___________________________________________________________________________________

Int_t EdbPVRQuality::CheckFilledXYSize()
{
    // ...............................................
    // Check the bins filled in the actual pattern.
    // The histogram of the XY distribution is analysed.
    // A warning is given if more than 10 percent of
    // the bins are empty. Becauseempty bins are NOT counted
    // in the BT density distribution.
    // In this case one should look closer at the specific
    // plate distribution, or ...
    // rebin the XY histogram to get a better statistics,
    // i.e. switch to larger bin areas (for example 1mm^2 to 2.5x2.5)
    // ...............................................

    if (gEDBDEBUGLEVEL>3)   cout << "-----     void EdbPVRQuality::CheckFilledXYSize() return maximum/minimum entries of  eHistYX    -----" << endl;
    Int_t nbx,nby=0;
    nbx=eHistYX->GetNbinsX();
    nby=eHistYX->GetNbinsY();

    Int_t n1x= FindFirstBinAbove(eHistYX,0,1);
    Int_t n1y= FindFirstBinAbove(eHistYX,0,2);
    Int_t n2x= FindLastBinAbove(eHistYX,0,1);
    Int_t n2y= FindLastBinAbove(eHistYX,0,2);
    Int_t width_x=0;
    Int_t width_y=0;
    width_x=TMath::Abs(eHistYX->GetXaxis()->GetBinCenter(n1x)-eHistYX->GetXaxis()->GetBinCenter(n2x));
    width_y=TMath::Abs(eHistYX->GetYaxis()->GetBinCenter(n1y)-eHistYX->GetYaxis()->GetBinCenter(n2y));

    // Now check the number of empty bins between! the filled area
    // within (FindFirstBinAbove,FindLastBinAbove)
    // This function is NOT optimized for speed :-)
    Int_t NonEmptyBins=1;
    Int_t nBins=1;
    for (Int_t i=n1x; i<n2x; ++i) {
        for (Int_t j=n1y; j<n2y; ++j) {
            ++nBins;
            if (eHistYX->GetBinContent(i,j)==0) continue;
            if (eHistYX->GetBinContent(i,j)==0) continue;
            ++NonEmptyBins;
        }
    }
    Float_t FractionOfEmptyBins=1-(Float_t(NonEmptyBins)/Float_t(nBins));
    cout << "WARNING: void EdbPVRQuality::CheckFilledXYSize() FractionOfEmptyBins = " << FractionOfEmptyBins << endl;

    if (FractionOfEmptyBins>0.1) {
        cout << "WARNING: void EdbPVRQuality::CheckFilledXYSize() FractionOfEmptyBins = " << FractionOfEmptyBins << endl;
        return 1;
    }

    if (gEDBDEBUGLEVEL>3) {
        cout << "----      NonEmptyBins bins in covered area:  = " << NonEmptyBins << endl;
        cout << "----      nBins totale bins in covered area:  = " << nBins << endl;
        cout << "----      Extrem Center Endpoints of eHistYX --- " << endl;
        cout << "----      nbxMin= " << n1x << endl;
        cout << "----      nbxMax= " << n1y << endl;
        cout << "----      nbyMin= " << n2x << endl;
        cout << "----      nbyMax= " << n2y << endl;
        cout << "----      nbxMin= " << eHistYX->GetXaxis()->GetBinCenter(n1x) << endl;
        cout << "----      nbxMax= " << eHistYX->GetYaxis()->GetBinCenter(n1y) << endl;
        cout << "----      nbxMin= " << eHistYX->GetXaxis()->GetBinCenter(n2x) << endl;
        cout << "----      nbxMax= " << eHistYX->GetYaxis()->GetBinCenter(n2y) << endl;
        cout << "-----     void EdbPVRQuality::CheckFilledXYSize() ... done. " << endl;
    }
    return 0;
}


//___________________________________________________________________________________

void EdbPVRQuality::Help()
{
    // Basic Help Function.

    cout << "----------------------------------------------" << endl;
    cout << "-----     void EdbPVRQuality::Help()     -----" << endl;
    cout << "-----" << endl;
    cout << "-----     This Class helps you to determine the Quality Cut Plate by Plate" << endl;
    cout << "-----     for suited BG level for shower reco." << endl;
    cout << "-----     You use it like this:" << endl;
    cout << "-----        EdbPVRQuality* QualityClass = new EdbPVRQuality(gAli)" << endl;
    cout << "-----     where  gAli is an EdbPVRec object pointer (usually the one" << endl;
    cout << "-----     from the cp files). Then you can get the CutValues with" << endl;
    cout << "-----     GetCutp0() (for each//all plate) back." << endl;
    cout << "-----" << endl;
    cout << "-----     On TODO list: to be interfaced with the libShower reconstruction mode." << endl;
    cout << "-----" << endl;
    cout << "-----" << endl;
    cout << "-----" << endl;
    cout << "-----  " << endl;

    cout << "-----     void EdbPVRQuality::Help()     -----" << endl;
    cout << "----------------------------------------------" << endl;
}

//___________________________________________________________________________________



TObjArray* EdbPVRQuality::Find_DoubleBT(EdbPVRec* aliSource)
{
    // Find double counted basetracks.
    // More explanation in Remove_DoubleBT() function.
    // Cutvalues used are the same.

    cout << "EdbPVRQuality::Find_DoubleBT()" << endl;
    cout << "EdbPVRQuality::Find_DoubleBT()  aliSource = " << aliSource << endl;
    cout << "EdbPVRQuality::Find_DoubleBT()  eAli_orig = " << eAli_orig << endl;

    if (NULL==aliSource) {
        cout << "WARNING!   EdbPVRQuality::Find_DoubleBT()  Source EdbPVRec is NULL. Try to change to object eAli_orig: " << eAli_orig << endl;
        if (NULL==eAli_orig) {
            cout << "WARNING!   EdbPVRQuality::Find_DoubleBT() Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
            return NULL;
        }
        else {
            aliSource=eAli_orig;
        }
    }

    Bool_t seg_seg_close=kFALSE;
    EdbSegP* seg=0;
    EdbSegP* seg1=0;
    Int_t NdoubleFoundSeg=0;
    TObjArray* DoubleBTArray = new TObjArray();

    //gEDBDEBUGLEVEL=3;
    cout << "EdbPVRQuality::Find_DoubleBT()   Start looping now. Attention, this might take a while!" << endl;

    if (aliSource->Npatterns()==0) return NULL;

    for (int i = 0; i <aliSource->Npatterns(); i++ ) {

        // Get Target Pattern:
        EdbPattern* pat = aliSource->GetPattern(i);
        // Helper Variable for cout purpose
        Int_t nPat=pat->N();

        if (gEDBDEBUGLEVEL>1) cout << "EdbPVRQuality::Find_DoubleBT()   Looping over Ali_source->Pat()=" << i << " (PID=" << pat->PID() << ") with N= " <<  nPat << " segs. Until now double Candidates found: " << NdoubleFoundSeg << endl;

        for (int j = 0; j < nPat; j++ ) {

            // condition due avoid floating point exception (divide by zero)
            if (gEDBDEBUGLEVEL>2) if (nPat>10) if (j%(nPat/10)==0) cout << "." << flush;

            seg = pat->GetSegment(j);
            seg_seg_close=kFALSE;

            for (int k = j; k < nPat; k++ ) {
                if (k==j) continue;


                if (seg_seg_close) continue;

                if (gEDBDEBUGLEVEL>3) cout << "Looping over pattern for segment pair nr=" << j << "," << k << endl;
                seg1 = pat->GetSegment(k);

                // Here decide f.e. which segments to check...
                Bool_t toKeep=kFALSE;

                // Skip already large distances
                if (TMath::Abs(seg->X()-seg1->X())>20.1) continue;
                if (TMath::Abs(seg->Y()-seg1->Y())>20.1) continue;
                if (TMath::Abs(seg->TX()-seg1->TX())>0.1) continue;
                if (TMath::Abs(seg->TY()-seg1->TY())>0.1) continue;
                //
                // a) all BT parings within a square DeltaR and Delta Theta:
                //
                // b) all BT pairings within dT<->dR line:
                //
                Float_t dR_allowed=0;
                Float_t deltaR=0;
                Float_t deltaTheta=0;
                Float_t dR_allowed_m_deltaR=0;
                // Calculate cut values:
                deltaTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                if (deltaTheta>0.1) continue;

                deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );

                // line defined by experimental values:
                // dR_allowed = 11(micron)/0.1(rad) * dTheta
                dR_allowed = 10.75/0.1*deltaTheta;
                dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);

                // For this function now -in contrary to Remove_DoubleBT- we
                // ONLY keep double basetrack pairings
                if (deltaR<3.0&&deltaTheta<0.01) toKeep=kTRUE;
                if (dR_allowed_m_deltaR<1.5) toKeep=kTRUE;
                if (!toKeep) continue;

                if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::Find_DoubleBT()   Found segment compatible close to another one: seg (" <<seg->PID() << ","<<  seg->ID() << ") is close to seg  (" << seg1->PID()<< ","<< seg1->ID()  << ")." << endl;

                // if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Find_DoubleBT()   Do last check if both are MCEvt segments of different event number! " << endl;
                // if ((seg->MCEvt()!=seg1->MCEvt())&&seg->MCEvt()<0) continue;
                if ((seg->MCEvt()!=seg1->MCEvt())&&(seg->MCEvt()>0&&seg1->MCEvt()>0)) continue;
                // I have doubts if I shall take MC events also out, but I guess Yes, because if
                // in data these small pairings appear, then they will fall also under the
                // category "fake doublet" and then be removed, even if they are real double BT
                // from a signal.

                seg_seg_close=kTRUE;
                ++NdoubleFoundSeg;
            } // for (int k = j+1; k <pat->N(); k++ )

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "// Add segment:" << endl;
            if (seg_seg_close) DoubleBTArray->Add(seg);
            seg_seg_close=kFALSE;
        } // of for (int j = 0; j < nPat-1; j++ )
    } // for (int i = 0; i <aliSource->Npatterns(); i++ )

    cout << "EdbPVRQuality::Find_DoubleBT() Statistics: " << endl;
    cout << "EdbPVRQuality::Find_DoubleBT() We found " << DoubleBTArray->GetEntries()  << " double segments too close to each other to be different BTracks." << endl;

    cout << "EdbPVRQuality::Find_DoubleBT()...done." << endl;
    return DoubleBTArray;

}


//___________________________________________________________________________________



EdbPVRec* EdbPVRQuality::Remove_DoubleBT(EdbPVRec* aliSource)
{
    // Remove double counted basetracks. This is a scanning induced error, at the
    // edges of the objective field, corrections are different, thus one
    // basetrack is seen differently in two different views.
    // In Bern data, we do see clearly this double bump, that is characterized by
    // _very_ close values of two BTs in X,Y,TX,TY (and also Chi2 and W).
    // Separation threshold values are 2microns in postion and 10 mrad in angle.
    // (again obtained from Bern data).

    // Quick and Dirty implementation !
    cout << "EdbPVRQuality::Remove_DoubleBT()" << endl;
    cout << "EdbPVRQuality::Remove_DoubleBT()  aliSource = " << aliSource << endl;
    cout << "EdbPVRQuality::Remove_DoubleBT()  eAli_orig = " << eAli_orig << endl;

    if (NULL==aliSource) {
        cout << "WARNING!   EdbPVRQuality::Remove_DoubleBT()  Source EdbPVRec is NULL. Try to change to object eAli_orig: " << eAli_orig << endl;
        if (NULL==eAli_orig) {
            cout << "WARNING!   EdbPVRQuality::Remove_DoubleBT() Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
            return NULL;
        }
        else {
            aliSource=eAli_orig;
        }
    }

    // Make a new PVRec object anyway
    EdbPVRec* aliTarget = new EdbPVRec();
    aliTarget->Print();

    Bool_t seg_seg_close=kFALSE;
    EdbSegP* seg=0;
    EdbSegP* seg1=0;
    Int_t NdoubleFoundSeg=0;

    //gEDBDEBUGLEVEL=3;
    cout << "EdbPVRQuality::Remove_DoubleBT()   Start looping now. Attention, this might take a while!" << endl;

    if (aliSource->Npatterns()==0) return aliSource;

    for (int i = 0; i <aliSource->Npatterns(); i++ ) {


        // Create Target Pattern:
        EdbPattern* pat = aliSource->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());
        // Helper Variable for cout purpose
        Int_t nPat=pat->N();

        if (gEDBDEBUGLEVEL>1) cout << "EdbPVRQuality::Remove_DoubleBT()   Looping over Ali_source->Pat()=" << i << " (PID=" << pat->PID() << ") with N= " <<  nPat << " segs. Until now double Candidates found: " << NdoubleFoundSeg << endl;


        for (int j = 0; j < nPat; j++ ) {

            // condition due avoid floating point exception (divide by zero)
            if (gEDBDEBUGLEVEL>2) if (nPat>10) if (j%(nPat/10)==0) cout << "." << flush;

            seg = pat->GetSegment(j);
            seg_seg_close=kFALSE;

            for (int k = j; k < nPat; k++ ) {
                if (k==j) continue;


                if (seg_seg_close) continue;

                if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_DoubleBT()   Doing segment pair nr=" << j << "," << k << endl;
                seg1 = pat->GetSegment(k);

                // Here decide f.e. which segments to check...
                Bool_t toContinue=kTRUE;

                // Skip already large distances
                if (TMath::Abs(seg->X()-seg1->X())>20.1) continue;
                if (TMath::Abs(seg->Y()-seg1->Y())>20.1) continue;
                if (TMath::Abs(seg->TX()-seg1->TX())>0.1) continue;
                if (TMath::Abs(seg->TY()-seg1->TY())>0.1) continue;

                //
                // a) all BT parings within a square DeltaR and Delta Theta:
                //
                // b) all BT pairings within dT<->dR line:
                //
                Float_t dR_allowed=0;
                Float_t deltaR=0;
                Float_t deltaTheta=0;
                Float_t dR_allowed_m_deltaR=0;

                deltaTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                if (deltaTheta>0.1) continue;

                deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );

                // line defined by experimental values:
                // dR_allowed = 11(micron)/0.1(rad) * dTheta
                dR_allowed = 10.75/0.1*deltaTheta;
                dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);

                if (deltaR<3.0&&deltaTheta<0.01) toContinue=kFALSE;
                if (dR_allowed_m_deltaR<1.5) toContinue=kFALSE;
                if (toContinue) continue;

                if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_DoubleBT()   Found incompatible segment for dR_dT line condition: " << endl;

                // if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_DoubleBT()   Do last check if both are MCEvt segments of different event number! " << endl;
                if ((seg->MCEvt()!=seg1->MCEvt())&&(seg->MCEvt()>0&&seg1->MCEvt()>0)) continue;
                // I have doubts if I shall take MC events also out, but I guess Yes, because if
                // in data these small pairings appear, then they will fall also under the
                // category "fake doublet" and then be removed, even if they are real double BT
                // from a signal.


                ++NdoubleFoundSeg;
                seg_seg_close=kTRUE;
            } // for (int k = j+1; k <pat->N(); k++ )

            if (seg_seg_close) continue;

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "// Add segment at " << seg  << endl;
            pt->AddSegment(*seg);

        }// for (int k)

        if (gEDBDEBUGLEVEL>2) cout << "// Add Pattern at " << pt << endl;
        aliTarget->AddPattern(pt);
    } // for (int j)

    if (gEDBDEBUGLEVEL>1) aliSource->Print();
    if (gEDBDEBUGLEVEL>1) aliTarget->Print();

    cout << "EdbPVRQuality::Remove_DoubleBT() Statistics: " << endl;
    cout << "EdbPVRQuality::Remove_DoubleBT() We found " << NdoubleFoundSeg  << " double segments too close to each other to be different BTracks." << endl;
    cout << "EdbPVRQuality::Remove_DoubleBT()...done." << endl;
    return aliTarget;
}

//___________________________________________________________________________________

EdbPVRec* EdbPVRQuality::Remove_Passing(EdbPVRec* aliSource)
{
    // Removes Passing Tracks from the EdbPVRec source object.
    // Unfortunately, there does Not Exist an easy function like
    // ->RemoveSegment from EdbPVRec object. That makes implementation complicated.

    cout << "EdbPVRQuality::Remove_Passing()." << endl;
    cout << "EdbPVRQuality::Remove_Passing()  aliSource = " << aliSource << endl;
    cout << "EdbPVRQuality::Remove_Passing()  eAli_orig = " << eAli_orig << endl;

    if (NULL==aliSource) {
        cout << "WARNING!----EdbPVRQuality::Remove_Passing()  Source EdbPVRec is NULL. Try to change to object eAli_orig: " << eAli_orig << endl;
        if (NULL==eAli_orig) {
            cout << "WARNING!----EdbPVRQuality::Remove_Passing() Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
            return NULL;
        }
        else {
            aliSource=eAli_orig;
        }
    }

    // Get the tracks from the source object:
    TObjArray* Tracks=aliSource->eTracks;

    // If eAli_source has no tracks, we return here and stop.
    if (NULL == Tracks) {
        cout << "WARNING!----EdbPVRQuality::Remove_Passing() NULL == eTracks." << endl;
        cout << "EdbPVRQuality::Remove_Passing() Read tracks from a linked_tracks.root file if there is any." << endl;
        Tracks=GetTracksFromLinkedTracksRootFile();

        if (NULL == Tracks) {
            cout << "WARNING!----EdbPVRQuality::GetTracksFromLinkedTracksRootFile() NULL == eTracks." << endl;
            cout << "WARNING!----EdbPVRQuality::Remove_Passing() Reading tracks from a linked_tracks.root file failed." << endl;
            cout << "WARNING!----EdbPVRQuality::Remove_Passing() Return the original object back." << endl;
            return aliSource;
        }
    }

    // Now the object array  Tracks  should be filled anyway, either from the source EdbPVR
    // or from the linked_tracks.root file:
    Int_t TracksN=Tracks->GetEntries();
    EdbTrackP* track;
    EdbSegP* trackseg;

    if (gEDBDEBUGLEVEL>1) {
        cout << "EdbPVRQuality::Remove_Passing()  aliSource/linked_tracks.root has Tracks N=" << TracksN << endl;
    }

    // Make a new PVRec object anyway
    EdbPVRec* aliTarget = new EdbPVRec();
    Bool_t seg_in_eTracks=kFALSE;
    Int_t totallyRemovedSegments=0;
    Int_t totallyAddedSegments=0;
    Int_t aliSourceNPat=aliSource->Npatterns();

    for (int i = 0; i < aliSourceNPat; i++ ) {
        cout << "EdbPVRQuality::Remove_Passing()  Looping over aliSource->Pat()=" << i << endl;

        EdbPattern* pat = aliSource->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());

        for (int j = 0; j <pat->N(); j++ ) {
            EdbSegP* seg = pat->GetSegment(j);
            seg_in_eTracks=kFALSE;

            if (gEDBDEBUGLEVEL>3) cout << "Checking segment j= " << j << " and loop over tracks to check correspondance." <<endl;

            for (int ll = 0; ll<TracksN; ll++ ) {
                track=(EdbTrackP*)Tracks->At(ll);
                Int_t TrackSegN=track->N();

                // Compare if this track is regarded as "passing"
                // when its less than5  plates than the original pvr object
                if (track->Npl()<aliSourceNPat-5) continue;
// 									cout << track->Npl() << " " << aliSourceNPat << endl;

                // Here decide f.e. which tracks to check...
                // On cosmics it would be nice that f.e. Npl()>=Npatterns-4 ...
                // if ....()....
                // Since addresses of objects can vary, its better to compare them
                // by absolute positions.

                for (int kk = 0; kk<TrackSegN; kk++ ) {
                    if (seg_in_eTracks) continue;
                    trackseg=track->GetSegment(kk);
                    if (TMath::Abs(seg->Z()-trackseg->Z())>10.1) continue;
                    if (TMath::Abs(seg->X()-trackseg->X())>5.1) continue;
                    if (TMath::Abs(seg->Y()-trackseg->Y())>5.1) continue;
                    if (TMath::Abs(seg->TX()-trackseg->TX())>0.05) continue;
                    if (TMath::Abs(seg->TY()-trackseg->TY())>0.05) continue;
                    //cout << "Found compatible segment!! " << endl;
                    totallyRemovedSegments++;
                    seg_in_eTracks=kTRUE;
                }
                if (seg_in_eTracks) continue;
            }
            if (seg_in_eTracks) continue;
            // Arrived here then the segmen has no equivalent in any segments
            // of the tracks array.

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_Passing() Add segment:" << endl;
            pt->AddSegment(*seg);
            totallyAddedSegments++;
        }
        if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::Remove_Passing()  TotallyAddedSegments (up to now) = " << totallyAddedSegments << " // Add Pattern:" << endl;
        aliTarget->AddPattern(pt);
    }

    if (gEDBDEBUGLEVEL>1) aliSource->Print();
    if (gEDBDEBUGLEVEL>1) aliTarget->Print();



    cout << "EdbPVRQuality::Remove_Passing() Totally removed segments= " << totallyRemovedSegments << endl;
    cout << "EdbPVRQuality::Remove_Passing()...done." << endl;
    return aliTarget;
}

//___________________________________________________________________________________




EdbPVRec* EdbPVRQuality::Remove_SegmentArray(TObjArray* segarray, EdbPVRec* aliSource, Int_t Option)
{
    // Every "Remove_XXY()" function will call Remove_SegmentArray() since the removal of
    // a segment array is easiest to implement. As said earlier, there is no easy function
    // to remove tracks from a EdbPVRec object directly. So instead we have to create a
    // new EdbPVRec object, fill with all basetracks from the old one, except where the
    // segments of the SegmentArray coincide with those which are in the source EdbPVRec.

    cout << "EdbPVRQuality::Remove_SegmentArray():" << endl;

    if (NULL==aliSource) {
        cout << "EdbPVRQuality::Remove_SegmentArray()   ERROR   aliSource=NULL pointer. I cannot" << endl;
        cout << "EdbPVRQuality::Remove_SegmentArray()   ERROR   do something here and I will return a NULL pointer now. Stop." << endl;
        return NULL;
    }

    if (gEDBDEBUGLEVEL>2) {
        cout << "EdbPVRQuality::Remove_SegmentArray()   INFO   " << endl;
        cout << "EdbPVRQuality::Remove_SegmentArray()   segarray =    " << segarray << " with " << segarray->GetEntries() << " entries." << endl;
        cout << "EdbPVRQuality::Remove_SegmentArray()   aliSource =    " << aliSource << " with " << aliSource->Npatterns() << " patterns." << endl;
        cout << "EdbPVRQuality::Remove_SegmentArray()   Option =    " << Option << "." << endl;
    }


    cout << "EdbPVRQuality::Remove_SegmentArray()...done." << endl;
    return NULL;
}

//___________________________________________________________________________________

EdbPVRec* EdbPVRQuality::Remove_TrackArray(TObjArray* trackArray, EdbPVRec* aliSource, Int_t Option) {
    // Remove a whole track array (Array of EdbTrackP type).
    // Return a new EdbPVRec* with previous element removed.
    cout << "EdbPVRQuality::Remove_TrackArray():" << endl;
    TObjArray* segArray= new TObjArray();
    segArray=  TrackArrayToSegmentArray(trackArray);
    Remove_SegmentArray(segArray,aliSource,Option);
    delete segArray;
    cout << "EdbPVRQuality::Remove_TrackArray()...done." << endl;
    return NULL;
}

//___________________________________________________________________________________

EdbPVRec* EdbPVRQuality::Remove_Segment(EdbSegP* seg, EdbPVRec* aliSource, Int_t Option) {
    // Remove one segment (EdbSegP* seg type).
    // Return a new EdbPVRec* with previous element removed.
    cout << "EdbPVRQuality::Remove_Segment():" << endl;
    TObjArray* segArray= new TObjArray();
    segArray->Add(seg);
    Remove_SegmentArray(segArray,aliSource,Option);
    delete segArray;
    cout << "EdbPVRQuality::Remove_Segment()...done." << endl;
    return NULL;
}

//___________________________________________________________________________________

EdbPVRec* EdbPVRQuality::Remove_Track(EdbTrackP* track, EdbPVRec* aliSource, Int_t Option) {
    // Remove one track (EdbTrackP* track type).
    // Return a new EdbPVRec* with previous element removed.
    cout << "EdbPVRQuality::Remove_Track():" << endl;
    TObjArray* segArray= new TObjArray();
    segArray=  TrackToSegmentArray(track);
    Remove_SegmentArray(segArray,aliSource,Option);
    delete segArray;
    cout << "EdbPVRQuality::Remove_Track()...done." << endl;
    return NULL;
}

//___________________________________________________________________________________

TObjArray* EdbPVRQuality::TrackToSegmentArray(EdbTrackP* track) {
    // Convert segments of a track into an TObjArray containing segments.
    TObjArray* segArray= new TObjArray();
    Int_t nSegTotal=0;
    for (int i=0; i<track->N(); i++) {
        EdbSegP* seg = track->GetSegment(i);
        segArray->Add(seg);
        ++nSegTotal;
    }
    if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::TrackToSegmentArray() Totally added: " << nSegTotal << " segments from the track."<<endl;
    return segArray;
}

//___________________________________________________________________________________

TObjArray* EdbPVRQuality::TrackArrayToSegmentArray(TObjArray* trackArray) {
    // Convert segments of tracks of a track array into an TObjArray containing segments.
    TObjArray* segArray= new TObjArray();
    Int_t nSegTotal=0;
    for (int j=0; j<trackArray->GetEntries(); j++) {
        EdbTrackP* track = (EdbTrackP*)trackArray->At(j);
        for (int i=0; i<track->N(); i++) {
            EdbSegP* seg = track->GetSegment(i);
            segArray->Add(seg);
            ++nSegTotal;
        }
    }
    if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::TrackArrayToSegmentArray() Totally added: " << nSegTotal << " segments from the track array."<<endl;
    return segArray;
}

//___________________________________________________________________________________

Int_t EdbPVRQuality::FindFirstBinAbove(TH1* hist, Double_t threshold, Int_t axis) {
    // The TH1 function  FindFirstBinAbove  is only implemented in
    // root version >= 5.24. But since many scanning labs use old root
    // versions persistently, I had to copy the TH1 function as a
    // memberfunction of EdbPVRQuality
    // Code taken from
    // http://root.cern.ch/root/html/src/TH1.cxx.html#biA7FC
    TAxis* ax=0;
    if (axis==1) ax = hist->GetXaxis();
    if (axis==2) ax = hist->GetYaxis();
    if (axis==3) ax = hist->GetZaxis();
    int nb = ax->GetNbins();
    for (Int_t i=0; i<=nb; i++) {
        if (hist->GetBinContent(i)>threshold) return i;
    }
    return 0;
}

//___________________________________________________________________________________

Int_t EdbPVRQuality::FindLastBinAbove(TH1* hist, Double_t threshold, Int_t axis) {
    // The TH1 function  FindFirstBinAbove  is only implemented in
    // root version >= 5.24. But since many scanning labs use old root
    // versions persistently, I had to copy the TH1 function as a
    // memberfunction of EdbPVRQuality
    // Code taken from
    // http://root.cern.ch/root/html/src/TH1.cxx.html#biA7FC
    TAxis* ax=0;
    if (axis==1) ax = hist->GetXaxis();
    if (axis==2) ax = hist->GetYaxis();
    if (axis==3) ax = hist->GetZaxis();
    int nb = ax->GetNbins();
    for (Int_t i=nb; i>=1; i--) {
        if (hist->GetBinContent(i)>threshold) return i;
    }
    return 0;
}

//___________________________________________________________________________________

TObjArray* EdbPVRQuality::GetTracksFromLinkedTracksRootFile()
{
    cout << "EdbPVRQuality::GetTracksFromLinkedTracksRootFile()" << endl;
// cout << "___TODO___    STEP  1 :  Check if linked_tracks.root exists   " << endl;
// cout << "___TODO___    STEP  2 :  Open linked_tracks   " << endl;
// cout << "___TODO___    STEP  3 :  Check if there are tracks inside   " << endl;
// cout << "___TODO___    STEP  4 :  Get Tracks as this TObjArray  " << endl;

    // Array containing EdbTrackP objects:
    TObjArray* trackarray = new TObjArray();

    TFile*  file = new TFile("linked_tracks.root");
    TTree* tr= (TTree*)file->Get("tracks");
    TClonesArray *seg= new TClonesArray("EdbSegP",60);
    EdbSegP* segment=0;
    EdbSegP*    s2=0;
    EdbSegP*    t=0;

    int nentr = int(tr->GetEntries());
    //   check if tracks has entries: if so then ok, otherwise return directly:
    if (nentr>0) {
        cout << "EdbPVRQuality::GetTracksFromLinkedTracksRootFile()   " << nentr << "  entries Total"<<endl;
    }
    else {
        cout << "EdbPVRQuality::GetTracksFromLinkedTracksRootFile()   linked_tracks.root file has  NO  entries...Return NULL." << endl;
        return NULL;
    }

    int nseg,n0,npl;
    tr->SetBranchAddress("t."  , &t  );
    tr->SetBranchAddress("s"  , &seg  );
    tr->SetBranchAddress("nseg"  , &nseg  );
    tr->SetBranchAddress("n0"  , &n0  );
    tr->SetBranchAddress("npl"  , &npl  );

    for (int i=0; i<nentr; i++ ) {
        tr->GetEntry(i);
        EdbTrackP* realTrack = new EdbTrackP(t);
        for (int j=0; j<nseg; j++ ) {
            s2=(EdbSegP*) seg->At(j);
            segment=(EdbSegP*)s2->Clone();
            realTrack->AddSegment(segment);
        }
        //realTrack->PrintNice();
        realTrack->SetNpl();    // Necessary otherwise these variables are not filled.
        realTrack->SetN0();     // Necessary otherwise these variables are not filled.
        trackarray->Add(realTrack);
    }
    delete tr;
    file->Close();
    delete file;
    cout << "EdbPVRQuality::GetTracksFromLinkedTracksRootFile()...done." << endl;
    return trackarray;
}





