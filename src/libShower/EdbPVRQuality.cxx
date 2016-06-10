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
    Log(2,"EdbPVRQuality::EdbPVRQuality()","Default Constructor");
    Init();
    Set0();
}

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)
{
    // Default Constructor with a EdbPVRec object.
    // (the EdbPVRec object corresponds to the collection of plates scanned with the collection
    // of basetracks (and additionally linked tracks)).
    // This constructor automatically checks the original data for background level and
    // creates a new EdbPVRec object that contains only segments that fulfill the quality
    // cut in accordance with the desired (predefined) background level.
    // If general basetrack density is lower than 20 BT/mm2 then no cleaning is done.

    Log(2,"EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)","Default Constructor with EdbPVRec object");

    Init();
    Set0();

    if (NULL == ali || ali->Npatterns()<1) {
        cout << "WARNING   EdbPVRQuality(EdbPVRec* ali)   ali is no good EdbPVRec object! Check!" << endl;
        return;
    }

    // Set ali as eAli_orig
    eAli_orig=ali;
    eIsSource=kTRUE;
    // Check EdbPVRec object for defects:
    CheckEdbPVRec();

    /// TO BE COMMENTET OUT WHEN ALL BG-Reduction-Algorithms are fully implemented:
// 		Remove_Passing(eAli_orig);
// 		Remove_DoubleBT(eAli_orig);
    /// TO BE CHECKED WHICH METHOD SHOULD BE THE DEFAULT ONE...
//     Execute_ConstantBTDensity();
//    Execute_ConstantBTDensityInAngularBins();
//    Execute_ConstantBTQuality();
    //    Execute_ConstantBTQualityInAngularBins();
//     Execute_ConstantBTX2Hat();
    //     Execute_ConstantBTX2HatInAngularBins();
//     Execute_RandomCut();
    //     Execute_RandomCutInAngularBins();

    // Create then the modified EdbPVRec object.
//     CreateEdbPVRec();
    // Finally Print Status Information
//     Print();
}

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali,  Float_t BTDensityTargetLevel)
{
    // Default Constructor with a EdbPVRec object and the desired basetrack density target level.
    // Does same as constructor EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali) but now with adjustable
    // background level.
    // If general basetrack density is lower than BTDensityTargetLevel BT/mm2 then no cleaning is done.

    Log(2,"EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali,  Float_t BTDensityTargetLevel)","Default Constructor with EdbPVRec object and given BT density level");

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
    cout << "EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)  Set BTDensityTargetLevel to (1/mm^2)= " << GetBTDensityLevel() << endl;

    // Check EdbPVRec object for defects:
    CheckEdbPVRec();

    /// TO BE COMMENTET OUT WHEN ALL BG-Reduction-Algorithms are fully implemented:
//     Remove_Passing(eAli_orig);
//     Remove_DoubleBT(eAli_orig);
    /// TO BE CHECKED WHICH METHOD SHOULD BE THE DEFAULT ONE...
    Execute_ConstantBTDensity();
    CreateEdbPVRec();
    Print();
}

//______________________________________________________________________________

EdbPVRQuality::~EdbPVRQuality()
{
    // Default Destructor
    Log(2,"EdbPVRQuality::~EdbPVRQuality()","Default Destructor");

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
    Log(2,"EdbPVRQuality::Set0","Set0");

    eAli_orig=NULL;
    eAli_modified=NULL;
    eNeedModified=kFALSE;
    eIsSource=kFALSE;
    eIsTarget=kFALSE;
    eAli_maxNpatterns=0;
    eCutMethod=0;
    for (int i=0; i<8; i++) eCutMethodIsDone[i]=kFALSE;
    eCutMethodString="ConstantBTDensity";


    // Default BT density level for which the standard cutroutine
    // will be put. This is for all tangens theta values integrated.
    eBTDensityLevel=20; // #BT/mm2
    // Default factor for equalizing the tan theta space:
    eCutTTSqueezeFactor = 0.95;

    // Default BT density level will use only segments for
    // data calculation .
    eBTDensityLevelCalcMethodMC=kFALSE;
    eBTDensityLevelCalcMethodMCConfirmationNumber=0;

    // Reset Default Geometry: 0 OperaGeometry, 1: MC Geometry
    eHistGeometry=0;

    // Reset Default Histograms
    eHistYX->Reset();
    eHistTYTX->Reset();
    eHistChi2W->Reset();
    eHistTT->Reset();

    for (int i=0; i<114; i++) {
        ePatternBTDensity_orig[i]=0;
        ePatternBTDensity_modified[i]=0;
        eCutp1[i]=0.15;
        eCutp0[i]=1.0; // Maximum Cut Value for const, BT dens
        eAgreementChi2WDistCut[i]=5.0;  // Maximum Cut Value for const, BT dens
        // Variables for the angular space also:
        for (int j=0; j<20; j++) {
            eCutTTp0[i][j]=1.00;
            eCutTTp1[i][j]=0.15;
            eXi2HatTT_m_chi2[i][j]=0;
            eXi2HatTT_s_chi2[i][j]=0;
            eXi2HatTT_m_WTilde[i][j]=0;
            eXi2HatTT_s_WTilde[i][j]=0;
        }
        eXi2Hat_m_chi2[i]=0;
        eXi2Hat_s_chi2[i]=0;
        eXi2Hat_m_WTilde[i]=0;
        eXi2Hat_s_WTilde[i]=0;
    }

    eRandomCutThreshold=1.0;

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

    Log(2,"EdbPVRQuality::Set0","Set0...done.");
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Init()
{
    // Basic Init function that creates objects in the memory which have
    // to be created only ONE time per class instance.

    Log(2,"EdbPVRQuality::Init","Init");

    eProfileBTdens_vs_PID_source = new TProfile("eProfileBTdens_vs_PID_source","eProfileBTdens_vs_PID_source",114,-0.5,113.5,0,200);
    eProfileBTdens_vs_PID_target = new TProfile("eProfileBTdens_vs_PID_target","eProfileBTdens_vs_PID_target",114,-0.5,113.5,0,200);

    eHistChi2W = new TH2F("eHistChi2W","eHistChi2W",40,0,40,90,0,3);	 // Filled with EdbSegP::Chi2(),W() value
    eHistYX = new TH2F("eHistYX","eHistYX",100,0,1,100,0,1);	 // Filled with EdbSegP::Y(),X() value
    eHistYX->SetMinimum(0);
    eHistYX->SetMaximum(150);
    eHistTYTX = new TH2F("eHistTYTX","eHistTYTX",100,-0.7,0.7,100,-0.7,0.7); 	 // Filled with EdbSegP::TY(),TX() value
    // If tangens theta binning histogram is too fine grained,
    // there is the danger of being to few entries in the histogram.
    eHistTT = new TH1F("eHistTT","eHistTT",10,0,1);  	 // Filled with EdbSegP::TT() value


    // TEMPORARY, later the Standard Geometry should default be OPERA.
    // For now we use a very large histogram that can contain both case
    // x-y ranges. This takes slightly more memory but it shouldnt matter.
    //if (eHistGeometry==0) SetHistGeometry_OPERA();
    SetHistGeometry_OPERAandMC();

    Log(2,"EdbPVRQuality::Init","Init...done.");
    return;
}


//______________________________________________________________________________
void EdbPVRQuality::SetCutMethod(Int_t CutMethod)
{
    // Set Cut Method of the background reduction:

    // 0: (default): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b
    // 1: (testing): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b  In Angular Bins

    // 2: (testing): Do cut based on a chi2-variable that compares with passing tracks (if available), i.e. cosmics.
    // 3: (testing): Do cut based on a chi2-variable that compares with passing tracks (if available), i.e. cosmics.  In Angular Bins

    // 4: (testing): Do cut based Xi2Hat relation.
    // 5: (testing): Do cut based Xi2Hat relation. In Angular Bins

    // 6: (testing): Do random cut based relation --> Just for quick test purposes....
    // 7: (testing): Do random cut based relation In Angular Bins --> Just for quick test purposes....


    Log(2,"EdbPVRQuality::SetCutMethod","SetCutMethod");

    eCutMethod=CutMethod;

    switch (eCutMethod)  {
    default:
        eCutMethodString = "ConstantBTDensity";
        break;
    case 0:
        eCutMethodString = "ConstantBTDensity";
        break;
    case 1:
        eCutMethodString = "ConstantBTDensityInAngularBins";
        break;
    case 2:
        eCutMethodString = "ConstantBTQuality";
        break;
    case 3:
        eCutMethodString = "ConstantBTQualityInAngularBins";
        break;
    case 4:
        eCutMethodString = "ConstantBTX2Hat";
        break;
    case 5:
        eCutMethodString = "ConstantBTX2HatInAngularBins";
        break;
    case 6:
        eCutMethodString = "RandomCut";
        break;
    case 7:
        eCutMethodString = "RandomCutInAngularBins";
        break;
    }


    Log(2,"EdbPVRQuality::SetCutMethod","Chosen Method (eCutMethod) =  %d", eCutMethod);

    if (CutMethod==0) cout << "// 0: (default): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b " << endl;
    if (CutMethod==1) cout << "// 1: (testing): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b 	 In Angular Bins " << endl;

    if (CutMethod==2) cout << "// 2: (testing): Do cut based on a chi2-variable that compares with passing tracks (if available), i.e. cosmics. " << endl;
    if (CutMethod==3) cout << "// 3: (testing): Do cut based on a chi2-variable that compares with passing tracks (if available), i.e. cosmics.  In Angular Bins " << endl;

    if (CutMethod==4) cout << "// 4: (testing): Do cut based Xi2Hat relation. " << endl;
    if (CutMethod==5) cout << "// 5: (testing): Do cut based Xi2Hat relation.	 In Angular Bins " << endl;
    if (CutMethod==6) cout << "// 6: (testing): Do random cut based relation --> Just for quick test purposes.... " << endl;
    if (CutMethod==7) cout << "// 7: (testing): Do random cut based relation  In Angular Bins --> Just for quick test purposes.... " << endl;

    if (CutMethod>7) {
        eCutMethod=0;
        cout << "WARNING   EdbPVRQuality::SetCutMethod  eCutMethod invalid, Set back to default eCutMethod = " << eCutMethod << endl;
    }

    Log(2,"EdbPVRQuality::SetCutMethod","SetCutMethod...done.");
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

    Log(2,"EdbPVRQuality::CheckEdbPVRec","CheckEdbPVRec");

    if (!eIsSource) {
        cout << "WARNING    EdbPVRQuality::CheckEdbPVRec  eIsSource = " << eIsSource << ". This means no source set. Return!" << endl;
        return;
    }
    // Check the patterns of the EdbPVRec:
    eAli_maxNpatterns= eAli_orig->Npatterns();

    if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
    if (eAli_maxNpatterns>114) {
        cout << "WARNING    EdbPVRQuality::CheckEdbPVRec  eAli_orig->Npatterns() = " << eAli_maxNpatterns << " is greater than possible basetrack data of two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114 !!!" << endl;
        eAli_maxNpatterns=114;
    }

    int Npat = eAli_maxNpatterns;
    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);

    // Loop over the patterns of the EdbPVRec:
    for (Int_t i=0; i<Npat; i++) {

        if (i>56) {
            cout << "WARNING    EdbPVRQuality::CheckEdbPVRec()    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING    EdbPVRQuality::CheckEdbPVRec()    Check it! " << endl;
        }
        if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::CheckEdbPVRec   Doing Pattern " << i << endl;


        eHistYX->Reset(); // important to clean the histogram
        eHistYX->SetMinimum(1); // then it can use log-scale
        eHistYX->SetMaximum(150); // Why did I write this? What if Maximum is exceeded?
        eHistChi2W->Reset(); // important to clean the histogram

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();

        EdbSegP* seg=0;
        // Loop over the segments of the pattern
        for (Int_t j=0; j<npat; j++) {
            seg=pat->GetSegment(j);
            // Very important:
            // For the data case, we assume the following:
            // Data (MCEvt<0)     will     be taken for BTdensity calculation
            // Sim (MCEvt>0)      will NOT be taken for BTdensity calculation
            // We take it ONLY and ONLY into account if it is especially wished
            // by the user!
            // Therefore (s)he needs to know how many Gauge Coupling Parameters
            // in the Standard Model exist (at least)...
            Bool_t result=kTRUE;
            if (seg->MCEvt()>0) {
                if (eBTDensityLevelCalcMethodMCConfirmationNumber==18 && eBTDensityLevelCalcMethodMC==kTRUE) {
                    result = kTRUE;
                }
                else {
                    result = kFALSE;
                }
            }

            if (gEDBDEBUGLEVEL>4)  cout << "EdbPVRQuality::CheckEdbPVRec  Doing segment " << j << " result for bool query is: " << result << endl;

            // Main decision for segment to be kept or not  (seg is of MC or data type).
            if ( kFALSE == result ) continue;

            // For the check, fill the histograms in any case:
            eHistYX->Fill(seg->Y(),seg->X());
            eHistTYTX->Fill(seg->TY(),seg->TX());
            eHistTT->Fill(TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX()));
            eHistChi2W->Fill(seg->W(),seg->Chi2());

        } // for (Int_t j=0; j<npat; j++)

        if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::CheckEdbPVRec  I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

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
        // for now we print a error message: tODO  REBIN THE YX HISTOGRA WITH 2.5x2.5 mm!!!!
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

    //---------------------------------------------------
    // This will be commented when using in batch mode...
    // And a text-output shall be written.
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
    //---------------------------------------------------

    // Reset interims variables/ histograms and delete unnecessary objects.
    eHistYX->Reset();
    eHistTYTX->Reset();
    eHistTT->Reset();
    eHistChi2W->Reset();
    delete histPatternBTDensity;

    Log(2,"EdbPVRQuality::CheckEdbPVRec","CheckEdbPVRec...done");
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
    //   Get plate, count number of basetracks in the unit area (1x1cm^2).
    //   Fill (draw if desired (like in EDA display)) histogram with the entries of the unit area.
    //   Get mean of the histogram, compare this value with the reference value.
    // The histogram covers all the area of one emulsion. (for the record: the old ORFEO MC
    // simulation gives not the same position as data does. The area of the histogramm was largely
    // increased to cover both cases).
    // It gives a good estimation of the density. Spikes in some plates, or in some zones are not
    // checked for, this is on the todo list, but maybe not so important.

    Log(2,"EdbPVRQuality::CheckEdbPVRecThetaSpace","CheckEdbPVRecThetaSpace");

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


    // Loop over the patterns of the EdbPVRec:
    for (Int_t i=0; i<Npat; i++) {

        if (i>56) {
            cout << "WARNING     EdbPVRQuality::CheckEdbPVRecThetaSpace()    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING     EdbPVRQuality::CheckEdbPVRecThetaSpace()    Check it! " << endl;
        }
        if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace    Doing Pattern " << i << endl;


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
                if (eBTDensityLevelCalcMethodMCConfirmationNumber==18 && eBTDensityLevelCalcMethodMC==kTRUE) {
                    result = kTRUE;
                }
                else {
                    result = kFALSE;
                }
            }

            if (gEDBDEBUGLEVEL>4)  cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace   Doing segment " << j << " result for bool query is: " << result << endl;

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

            // To check: Where do these hardcoded values come from?
            Double_t xxx = TMath::Sqrt((seg->Chi2()-0.8)*(seg->Chi2()-0.8)/0.2/0.2+(seg->W()-17.)*(seg->W()-17.)/2./2.);
            QualityValue_Total->Fill(xxx);

        }

        if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace   I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

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


        cout << "EdbPVRQuality::CheckEdbPVRecThetaSpace   histPatternBTDensity->GetMean() = " << histPatternBTDensity->GetMean() << endl;

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

    // delete unnecessary variables/ objects
    delete    spec2;

    cout << "TODO    // Reset interims variables/ histograms and delete unnecessary objects."  << endl;


    Log(2,"EdbPVRQuality::CheckEdbPVRecThetaSpace","CheckEdbPVRecThetaSpace...done.");
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::SetHistGeometry_OPERA()
{
    // Set the geometry of the basetrack density evaluation using OPERA case,
    // European Scanning System size conventions: x=0..125000;y=0..100000).
    // BinArea is 1mmx1mm.
    Log(3,"EdbPVRQuality::SetHistGeometry_OPERA","SetHistGeometry_OPERA");
    eHistYX->Reset();
    eHistYX->SetBins(100,0,100000,120,0,120000);
    Log(3,"EdbPVRQuality::SetHistGeometry_OPERA","SetHistGeometry_OPERA...done");
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_MC()
{
    // Set the geometry of the basetrack density evaluation using simulation case,
    // MC ORFEO size conventions: x=-xmax..0..+xmax;y=-ymax..0..ymax).
    // BinArea is 1mmx1mm
    Log(3,"EdbPVRQuality::SetHistGeometry_MC","SetHistGeometry_MC");
    eHistYX->Reset();
    eHistYX->SetBins(100,-50000,50000,100,-50000,50000);
    Log(3,"EdbPVRQuality::SetHistGeometry_MC","SetHistGeometry_MC...done");
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_OPERAandMC()
{
    // Set the geometry of the basetrack density evaluation covering MC and DATA case,
    // size conventions: x=-125000..0..+125000;y=-125000..0..125000).
    // BinArea is 1mmx1mm

    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMC","SetHistGeometry_OPERAandMC");
    eHistYX->Reset();
    eHistYX->SetBins(250,-125000,125000,250,-125000,125000);
    if (gEDBDEBUGLEVEL>2) cout << "SetHistGeometry_OPERAandMC::binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMC","SetHistGeometry_OPERAandMC...done");
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_OPERAandMCBinArea625()
{
    // Set the geometry of the basetrack density evaluation covering MC and DATA case,
    // size conventions: x=-125000..0..+125000;y=-125000..0..125000).
    // BinArea is 2.5mmx2.5mm

    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMCBinArea625","SetHistGeometry_OPERAandMCBinArea625");
    eHistYX->Reset();
    eHistYX->SetBins(100,-125000,125000,100,-125000,125000);
    if (gEDBDEBUGLEVEL>2) cout << "SetHistGeometry_OPERAandMC::binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    Log(3,"EdbPVRQuality::SetHistGeometry_OPERAandMCBinArea625","SetHistGeometry_OPERAandMCBinArea625...done");
    return;
}

//______________________________________________________________________________
void EdbPVRQuality::PrintCutType()
{
    // Print PrintCutType data for this class.
    cout << "EdbPVRQuality::Print()   " << endl;
    for (int i=0; i<8; i++) {
        cout << "EdbPVRQuality::Print()   eCutMethodIsDone["<<i<<"] " << eCutMethodIsDone[i] << endl;
    }
    if (eCutMethodIsDone[0]) PrintCutType0();
    if (eCutMethodIsDone[1]) PrintCutType1();
    if (eCutMethodIsDone[2]) PrintCutType2();
    if (eCutMethodIsDone[3]) PrintCutType3();
    if (eCutMethodIsDone[4]) PrintCutType4();
    if (eCutMethodIsDone[5]) PrintCutType5();
    if (eCutMethodIsDone[6]) PrintCutType6();
    if (eCutMethodIsDone[7]) PrintCutType7();
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Print()
{
    Log(2,"EdbPVRQuality::Print","Print");
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Print() --- SETTINGS GENERAL: ---" << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethod = " << eCutMethod << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodString = " << eCutMethodString.Data() << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[0] = " << eCutMethodIsDone[0] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[1] = " << eCutMethodIsDone[1] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[2] = " << eCutMethodIsDone[2] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[3] = " << eCutMethodIsDone[3] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[4] = " << eCutMethodIsDone[4] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[5] = " << eCutMethodIsDone[5] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutMethodIsDone[6] = " << eCutMethodIsDone[6] << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eBTDensityLevel = " << eBTDensityLevel << endl;
    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eCutTTSqueezeFactor = " << eCutTTSqueezeFactor << endl;

    cout << "EdbPVRQuality::Print() --- " << setw(40) << "eBTDensityLevelCalcMethodMC = " << eBTDensityLevelCalcMethodMC << endl;
    //cout << "EdbPVRQuality::Print() --- " << setw(40) << "eBTDensityLevelCalcMethodMCConfirmationNumber = " << eBTDensityLevelCalcMethodMCConfirmationNumber << endl;
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
    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;

    Log(2,"EdbPVRQuality::Print","Print...done.");
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
        cout << "WARNING    eAli_modified==NULL  ==>>  Take eAli_orig instead. To build eAli_modified please run CreateEdbPVRec(eAli_orig)" << endl;
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
        cout << "---      This CutType has not been done. Run  Execute_ConstantBTQuality() ---" << endl;
        cout << "---      to see the results (return now). ---" << endl;
        return;
    }

    cout << "----------void EdbPVRQuality::PrintCutType1()----------" << endl;

    cout << "Pattern || Z() || Nseg || BTDensity_orig || Chi2CutMeanChi2 || Chi2CutRMSChi2 || Chi2CutMeanW || Chi2CutRMSW || Chi2Cut[i] || BTDensity_modified ||"<< endl;

    if (NULL==eAli_modified) {
        eAli_modified=eAli_orig;
        cout << "WARNING    eAli_modified==NULL  ==>>  Take eAli_orig instead. To build eAli_modified please run CreateEdbPVRec(eAli_orig)" << endl;
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
    cout << "TODO   void EdbPVRQuality::PrintCutType2() NOT  YET IMPLEMENTED" << endl;
    return;
}


//______________________________________________________________________________


void EdbPVRQuality::PrintCutType3()
{
    cout << "TODO   void EdbPVRQuality::PrintCutType3() NOT  YET IMPLEMENTED" << endl;
    return;
}

//______________________________________________________________________________


void EdbPVRQuality::PrintCutType4()
{
    //     Prints quality cut values for each plate of the original EdbPVRec object that were
    //     applied to achieve the basetrack density level, after application the specific cut
    //     on the segments of the specific plate (=pattern).

    if (!eIsSource) return;
    if (!eCutMethodIsDone[4]) {
        cout << "---   EdbPVRQuality::PrintCutType4()----------" << endl;
        cout << "---      This CutType has not been done. Run  Execute_ConstantBTQuality() ---" << endl;
        cout << "---      to see the results (return now). ---" << endl;
        return;
    }

    cout << "----------void EdbPVRQuality::PrintCutType4()----------" << endl;

    cout << "Pattern || Z() || Nseg || BTDensity_orig || eXi2Hat_m_chi2 || eXi2Hat_s_chi2 || eXi2Hat_m_WTilde || eXi2Hat_s_WTilde || eXi2HatCut[i] || BTDensity_modified ||"<< endl;

    if (NULL==eAli_modified) {
        eAli_modified=eAli_orig;
        cout << "WARNING    eAli_modified==NULL  ==>>  Take eAli_orig instead. To build eAli_modified please run CreateEdbPVRec(eAli_orig)" << endl;
    }

    int Npat_orig = eAli_orig->Npatterns();
    for (int i=0; i<Npat_orig; i++) {
        EdbPattern* pat_orig = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npatO=pat_orig->N();
        EdbPattern* pat_modified = (EdbPattern*)eAli_modified->GetPattern(i);
        Int_t npatM=pat_modified->N();
        cout << i;
        cout << "	";
        printf("%.1f  %d  %.2f  %.2f  %.2f  %.2f  %.2f  %.2f",pat_orig->Z(),npatO, eXi2Hat_m_chi2[i] , eXi2Hat_s_chi2[i],  eXi2Hat_m_WTilde[i] , eXi2Hat_s_WTilde[i], eX2HatCut[i], ePatternBTDensity_orig[i]);
        cout << "	...	";
        printf("%.1f  %d  %.2f  %.2f  %.2f  %.2f  %.2f  %.2f",pat_orig->Z(),npatM, eXi2Hat_m_chi2[i] , eXi2Hat_s_chi2[i],  eXi2Hat_m_WTilde[i] , eXi2Hat_s_WTilde[i], eX2HatCut[i], ePatternBTDensity_modified[i]);
        cout << endl;
    }
    return;
}


//______________________________________________________________________________


void EdbPVRQuality::PrintCutType5()
{
    cout << "TODO  void EdbPVRQuality::PrintCutType5() NOT  YET IMPLEMENTED" << endl;
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::PrintCutType6()
{
    cout << "TODO  void EdbPVRQuality::PrintCutType6() NOT  YET IMPLEMENTED" << endl;
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::PrintCutType7()
{
    cout << "TODO  void EdbPVRQuality::PrintCutType6() NOT  YET IMPLEMENTED" << endl;
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Execute()
{
    // --------------------------------------------------------------------
    // Main Execution Routine.
    // Does the following things:
    //
    // a) Check EdbPVRec object of data.
    // b) Remove DoubleBasetracks
    // c) Remove passing tracks
    //   (if any stored, either in .root file or in EdbPVRec object itself)
    // d) Execute quality cuts if necessary: default is Constant BT density
    // e) Store cleaned object. Can be retrieved via GetPVR().
    // --------------------------------------------------------------------

    Log(2,"EdbPVRQuality::Execute","Execute");

    Log(2,"EdbPVRQuality::Execute","Main Execution Routine.  TODO... ");
    Log(2,"EdbPVRQuality::Execute","Does the following things:");
    Log(2,"EdbPVRQuality::Execute","  a) Check EdbPVRec object of data.");
    Log(2,"EdbPVRQuality::Execute","  b) Remove DoubleBasetracks");
    Log(2,"EdbPVRQuality::Execute","  c) Remove passing tracks (if any stored, either in");
    Log(2,"EdbPVRQuality::Execute","     the .root file or in EdbPVRec object itself)");
    Log(2,"EdbPVRQuality::Execute","  d) Execute quality cuts if necessary: default is Constant BT density");
    Log(2,"EdbPVRQuality::Execute","  e) Store cleaned object. Can be retrieved via GetPVR().");

    cout << " " << endl;
    cout << "___________________ TODO _______   AT THE MOMENT NOTHING HAPPENS YET IN THIS FUNCTION !!! ____________" << endl;
    cout << " " << endl;
    
    Log(2,"EdbPVRQuality::Execute","Execute...done.");
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Execute(Int_t CutMethod)
{
    Log(2,"EdbPVRQuality::Execute","Execute");
    Log(2,"EdbPVRQuality::Execute","Chosen CutMethod to apply = %d. Set eCutMethod accordingly.",CutMethod);

    SetCutMethod(CutMethod);

    switch (eCutMethod)  {
    default:
        Execute_ConstantBTDensity();
        break;
    case 0:
        Execute_ConstantBTDensity();
        break;
    case 1:
        Execute_ConstantBTDensityInAngularBins();
        break;
    case 2:
        Execute_ConstantBTQuality();
        break;
    case 3:
        Execute_ConstantBTQualityInAngularBins();
        break;
    case 4:
        Execute_ConstantBTX2Hat();
        break;
    case 5:
        Execute_ConstantBTX2HatInAngularBins();
        break;
    case 6:
        Execute_RandomCut();
        break;
    case 7:
        Execute_RandomCutInAngularBins();
        break;
    }

    Log(2,"EdbPVRQuality::Execute","Execute...done.");
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

    Log(2,"EdbPVRQuality::Execute_ConstantBTDensity","Execute_ConstantBTDensity");


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

    Log(2,"EdbPVRQuality::Execute_ConstantBTDensity","Execute_ConstantBTDensity...done.");
    return;
}

//______________________________________________________________________________


void EdbPVRQuality::Execute_ConstantBTQuality()
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

    Log(2,"EdbPVRQuality::Execute_ConstantBTQuality","Execute_ConstantBTQuality");

    if (!eIsSource) return;

    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Execute_ConstantBTQuality " << endl;
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

    cout << "EdbPVRQuality::Execute_ConstantBTQuality()   eAli_orig.eTracks :" << eAli_orig->eTracks << endl;

    Float_t meanChi2=0.5;
    Float_t rmsChi2=0.2;
    Float_t meanW=22;
    Float_t rmsW=4;
    Float_t agreementChi2=100;

    // No  eAli.Tracks ? Look for tracks in linked_track.root:
    if (NULL == eAli_orig->eTracks) {
        cout << "EdbPVRQuality::Execute_ConstantBTQuality()   No eAli.Tracks. Look for tracks in linked_track.root" << endl;
        TFile* trackfile = new TFile("linked_tracks.root");
        trackfile->ls();
        TTree* tracks = (TTree*)trackfile->Get("tracks");
        if (NULL == tracks) {
            cout << "EdbPVRQuality::Execute_ConstantBTQuality()   No tracks in linked_track.root file. Return, leave eAli_orig unchanged and dont do any cleaning. You might try Execute_ConstantBTDensity instead. " << endl;
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

        cout << "EdbPVRQuality::Execute_ConstantBTQuality()   Found tracks in the file! Good. Info:" << endl;
        cout << "EdbPVRQuality::Execute_ConstantBTQuality()   The long tracks in the file have an average percentage length of:" << travellenghtpercentage  <<  endl;
        cout << "EdbPVRQuality::Execute_ConstantBTQuality()   Mean(RMS) of Chi2 distribution of passing tracks: " << hChi2->GetMean() << "+-"  << hChi2->GetRMS() << endl;



        TCanvas* c1 = new TCanvas();
        c1->cd();
        tracks->Draw("s.eW>>hW(50,0,50)",cutstring);
        TH1F *hW = (TH1F*)gPad->GetPrimitive("hW");
        cout << "EdbPVRQuality::Execute_ConstantBTQuality()   Mean(RMS) of W distribution of passing tracks: " << hW->GetMean() << "+-"  << hW->GetRMS() << endl;

        meanChi2=hChi2->GetMean();
        rmsChi2=hChi2->GetRMS();
        meanW=hW->GetMean();
        rmsW=hW->GetRMS();

        // Quick check if these values are reasonable:
        if (TMath::Abs(meanChi2-0.5)>1 || TMath::Abs(meanW-22)>6) {
            cout << "WARNING   EdbPVRQuality::Execute_ConstantBTQuality()   The tracks might have a strange distribution. You are urgently requested to check these manually!!!"<<endl;
        }
        else {
            cout << "EdbPVRQuality::Execute_ConstantBTQuality()   The tracks have reasonable values."<<endl;
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
    cout << "EdbPVRQuality::Execute_ConstantBTQuality  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << endl;
    if (eAli_maxNpatterns>57) cout << " This tells us not yet if we do have one/two brick reconstruction done. A possibility could also be that the dataset was read with microtracks. Further investigation is needed! (On todo list)." << endl;
    if (eAli_maxNpatterns>114) {
        cout << "ERROR!  EdbPVRQuality::Execute_ConstantBTQuality  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",100,0,200);
    TH1F* histagreementChi2 = new TH1F("histagreementChi2","histagreementChi2",100,0,5);


    cout << "Execute_ConstantBTQuality   Loop over the patterns..." << endl;
    for (int i=0; i<eAli_maxNpatterns; i++) {
        if (i>56) {
            cout << "ERROR     EdbPVRQuality::Execute_ConstantBTQuality() Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "ERROR     EdbPVRQuality::Execute_ConstantBTQuality() Check it! " << endl;
        }

        if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantBTQuality   Doing Pattern " << i << endl;

        // Now the condition loop:
        // Loop over 30 steps agreementChi2 step 0.025
        for (int l=0; l<30; l++) {

            if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantBTQuality   Doing condition loop = " << l << endl;

            eHistYX->Reset(); // important to clean the histogram
            eHistChi2W->Reset(); // important to clean the histogram
            histPatternBTDensity->Reset(); // important to clean the histogram

            EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
            Int_t npat=pat->N();
            EdbSegP* seg=0;

            if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantBTQuality   Loop over segments of pattern " << i << ",Number of segments= " << npat <<  endl;
            for (int j=0; j<npat; j++) {
                seg=pat->GetSegment(j);

                if (gEDBDEBUGLEVEL>4) cout << "Execute_ConstantBTQuality   Doing segment= " << j << endl;
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

            if (gEDBDEBUGLEVEL>2) cout <<"Execute_ConstantBTQuality      Loop l= " << l << ":  for the eAgreementChi2WDistCut : " << eAgreementChi2WDistCut[i] <<   "  we have a dens: "  << ePatternBTDensity_modified[i] << endl;

            // Now the condition check:
            if (ePatternBTDensity_modified[i]<=eBTDensityLevel) {
                if (gEDBDEBUGLEVEL>2)  cout << "Execute_ConstantBTQuality      We reached the loop end due to good BT density level ... and break loop." << endl;
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
        cout << "Execute_ConstantBTQuality   histPatternBTDensity->GetMean()." << histPatternBTDensity->GetMean() <<  endl;
        eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

    } // of Npattern loops..
    cout << "Execute_ConstantBTQuality   Loop over the patterns...done." << endl;

    // Check if modified or original PVRec object should be returned:
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTQuality() Check if modified or original PVRec object should be returned: " << endl;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTQuality() " << eNeedModified << endl;
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

    cout << "----------void EdbPVRQuality::Execute_ConstantBTQuality() Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTQuality() If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;

    cout << "----------void EdbPVRQuality::Execute_ConstantBTQuality()....done." << endl;

    Log(2,"EdbPVRQuality::Execute_ConstantBTQuality","Execute_ConstantBTQuality...done.");
    return;
}

//___________________________________________________________________________________

void EdbPVRQuality::Execute_ConstantBTDensityInAngularBins()
{

    Log(2,"EdbPVRQuality::Execute_ConstantBTDensityInAngularBins","Execute_ConstantBTDensityInAngularBins...done.");
    Log(2,"EdbPVRQuality::Execute_ConstantBTDensityInAngularBins","Execute_ConstantBTDensityInAngularBins...done.");
    return;
}


//___________________________________________________________________________________

void EdbPVRQuality::Execute_ConstantBTX2Hat()
{
    // Execute the modified cut routines to achieve the basetrack density level,
    // after application the specific cut on the segments of the specific plate (pattern).
    // The ConstantBTX2Hat is defined by the number of BT/mm2 in the histogram.
    // cut on the variable Xi2Hat!

    Log(2,"EdbPVRQuality::Execute_ConstantBTX2Hat","Execute_ConstantBTX2Hat");

    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Execute_ConstantBTX2Hat " << endl;
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
        cout << "ERROR!  EdbPVRQuality::Execute_ConstantBTX2Hat  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }


    // Create the temporary needed histograms:
    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);
    TH1F* h_chi2 = new TH1F("h_chi2","h_chi2",100,0,2);
    TH1F* h_chi2Normalized = new TH1F("h_chi2Normalized","h_chi2Normalized",100,0,5);
    TH1F* h_WTilde = new TH1F("h_WTilde","h_WTilde",40,0,0.1);
    TH1F* h_WTildeNormalized = new TH1F("h_WTildeNormalized","h_WTildeNormalized",100,0,5);
    TH1F* h_X2 = new TH1F("h_X2","h_X2",100,0,5);

    // Loop over the patterns:
    for (int i=0; i<eAli_maxNpatterns; i++) {
        if (i>56) {
            cout << "WARNING     EdbPVRQuality::Execute_ConstantBTX2Hat()    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING     EdbPVRQuality::Execute_ConstantBTX2Hat()    Check it! " << endl;
        }
        if (gEDBDEBUGLEVEL>2) cout << "Execute_ConstantBTX2Hat   Doing Pattern " << i << endl;


        Float_t chi2Normalized=0;
        Float_t WTilde=0;
        Float_t WTildeNormalized=0;
        Float_t X2Hat=0;
        Float_t     m_chi2=0;
        Float_t s_chi2=0;
        Float_t m_WTilde=0;
        Float_t s_WTilde=0;


        // Reset of the original eX2HatCut value:
        eX2HatCut[i]=5;

        // Now the condition loop:
        // Loop over 10 steps for the eX2HatCut value:
        for (int l=0; l<20; l++) {
            eHistYX->Reset(); // important to clean the histogram
            eHistChi2W->Reset(); // important to clean the histogram
            histPatternBTDensity->Reset(); // important to clean the histogram
            eHistTYTX->Reset(); // important to clean the histogram
            eHistTT->Reset();          // important to clean the histogram


            EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
            Int_t npat=pat->N();
            EdbSegP* seg=0;
            // Loop over the segments.
            // Here do it two times, one time to get Mean and RMS of Chi2 and WTilde distribution.
            // Do this only for the loopcase l==0:
            if (l==0) {
                h_chi2->Reset();
                h_chi2Normalized->Reset();
                h_WTilde->Reset();
                h_WTildeNormalized->Reset();
                h_X2->Reset();


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

                    // Pre-Fill the Chi2() and W() histograms to get Mean and RMS values right:
                    h_chi2->Fill(seg->Chi2());
                    WTilde=1./(seg->W());
                    h_WTilde->Fill(WTilde);

                } // for (int j=0; j<npat; j++)

                // Calulation of the histograms to get Mean and RMS values right:
                m_chi2=h_chi2->GetMean();
                s_chi2=h_chi2->GetRMS();
                m_WTilde=h_WTilde->GetMean();
                s_WTilde=h_WTilde->GetRMS();


                eXi2Hat_m_chi2[i]=m_chi2;
                eXi2Hat_s_chi2[i]=s_chi2;
                eXi2Hat_m_WTilde[i]=m_WTilde;
                eXi2Hat_s_WTilde[i]=s_WTilde;

            } // if (l==0)



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

                // Calculate that Xi2Hat value, since we have now mean and rms of the distribution histograms:
                chi2Normalized=TMath::Power((seg->Chi2()-m_chi2)/s_chi2,2);
                h_chi2Normalized->Fill(chi2Normalized);
                WTilde=1./(seg->W());
                WTildeNormalized=TMath::Power((WTilde-m_WTilde)/s_WTilde,2);
                h_WTildeNormalized->Fill(WTildeNormalized);
                X2Hat=chi2Normalized + WTildeNormalized;
                h_X2->Fill(X2Hat);

                // Constant BT density cut:
                if (X2Hat>eX2HatCut[i]) continue;

                eHistYX->Fill(seg->Y(),seg->X());
                eHistChi2W->Fill(seg->W(),seg->Chi2());
                eHistTYTX->Fill(seg->TY(),seg->TX());
                eHistTT->Fill(seg->Theta());
            }


            if (gEDBDEBUGLEVEL>2) cout << "I have filled the eHistYX Histogram. Entries = " << eHistYX->GetEntries() << endl;

            int nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
            for (int k=1; k<nbins-1; k++) {
                if (eHistYX->GetBinContent(k)==0) continue;
                histPatternBTDensity->Fill(eHistYX->GetBinContent(k));
            }

            ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();
            if (gEDBDEBUGLEVEL>2) cout <<"Execute_ConstantBTX2Hat      Loop l= " << l << ":  for the eX2HatCut : " << eX2HatCut <<   "  we have a dens: "  << ePatternBTDensity_modified[i] << endl;

            // Now the condition check:
            if (ePatternBTDensity_modified[i]<=eBTDensityLevel) {
                if (gEDBDEBUGLEVEL>2)  cout << "Execute_ConstantBTX2Hat      We reached the loop end due to good BT density level ... and break loop." << endl;
                break;
            }
            else {
                // Next step, tighten cut:
                // eCutp1[i] += -0.005;
                eX2HatCut[i] += -0.2;
            }

        } // of condition loop...

        // Fill target profile histogram:
        Float_t bincontentXY=histPatternBTDensity->GetMean();
        eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

    } // of Npattern loops..

    eCutMethodIsDone[4]=kTRUE;

    // Check if modified or original PVRec object should be returned:
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTX2Hat() Check if modified or original PVRec object should be returned: " << endl;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTX2Hat() " << eNeedModified << endl;
    }


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
    eProfileBTdens_vs_PID_target->SetLineStyle(2);
    eProfileBTdens_vs_PID_target->Draw("profileZsame");
    c1->cd(6);
    eHistTT->DrawCopy("");
    c1->cd();


    histPatternBTDensity->Reset();
    eHistYX->Reset();
    eHistChi2W->Reset();

    TCanvas* c2 = new TCanvas();
    c2->Divide(3,1);
    c2->cd(1);
    h_chi2->Draw();
    c2->cd(2);
    h_WTilde->Draw();
    c2->cd(3);
    h_X2->Draw();
    c2->cd();

    eProfileBTdens_vs_PID_source_meanX=eProfileBTdens_vs_PID_source->GetMean(1);
    eProfileBTdens_vs_PID_source_meanY=eProfileBTdens_vs_PID_source->GetMean(2);
    eProfileBTdens_vs_PID_source_rmsX=eProfileBTdens_vs_PID_source->GetRMS(1);
    eProfileBTdens_vs_PID_source_rmsY=eProfileBTdens_vs_PID_source->GetRMS(2);

    eProfileBTdens_vs_PID_target_meanX=eProfileBTdens_vs_PID_target->GetMean(1);
    eProfileBTdens_vs_PID_target_meanY=eProfileBTdens_vs_PID_target->GetMean(2);
    eProfileBTdens_vs_PID_target_rmsX=eProfileBTdens_vs_PID_target->GetRMS(1);
    eProfileBTdens_vs_PID_target_rmsY=eProfileBTdens_vs_PID_target->GetRMS(2);

    delete histPatternBTDensity;

    cout << "----------void EdbPVRQuality::Execute_ConstantBTX2Hat() Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTX2Hat() If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;
    cout << "----------void EdbPVRQuality::Execute_ConstantBTX2Hat()....done." << endl;


    PrintCutType4();

    Log(2,"EdbPVRQuality::Execute_ConstantBTX2Hat","Execute_ConstantBTX2Hat...done.");
    return;
}


//___________________________________________________________________________________

void EdbPVRQuality::Execute_ConstantBTX2HatInAngularBins()
{
    cout << "TODO     void EdbPVRQuality::Execute_ConstantBTX2HatInAngularBins() TO BE DONE ! ATTENTION WARNING" << endl;
}



//___________________________________________________________________________________

void EdbPVRQuality::Execute_ConstantBTQualityInAngularBins()
{
    cout << "TODO     void EdbPVRQuality::Execute_ConstantBTQualityInAngularBins() TO BE DONE ! ATTENTION WARNING " << endl;

}

//___________________________________________________________________________________

void EdbPVRQuality::Execute_RandomCutInAngularBins()
{
    cout << "void EdbPVRQuality::Execute_RandomCutInAngularBins() TO BE DONE ! ATTENTION WARNING " << endl;
    cout << "SWTICH TO :: Execute_EqualizeTanThetaSpace_RandomCut() " << endl;
    return;
}


//___________________________________________________________________________________

void EdbPVRQuality::Execute_RandomCut()
{
    // EdbPVRQuality::Execute_RandomCut()
    // Execute the modified cut routines to achieve arbitrary basetrack density level.
    // Basically used for control curposes.
    Log(2,"EdbPVRQuality::Execute_RandomCut","Execute_RandomCut");


    for (int i=0; i<80; ++i) cout << "-";
    cout << endl;
    cout << "-";
    cout << endl;
    cout << "EdbPVRQuality::Execute_RandomCut " << endl;
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
        cout << "ERROR!  EdbPVRQuality::Execute_RandomCut  eAli_orig->Npatterns()=  " << eAli_maxNpatterns << " is greater than possible basetrack data two bricks. This class does (not yet) work with this large number of patterns. Set maximum patterns to 114!!!." << endl;
        eAli_maxNpatterns=114;
    }

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);

    // Before Starting Create the Random Generator:
    cout << "EdbPVRQuality::Execute_RandomCut    Create the Random Generator with unique Seed" << endl;
    cout << "EdbPVRQuality::Execute_RandomCut    TRandom3* RandomGenerator = new TRandom3(0);" << endl;
    TRandom3* RandomGenerator = new TRandom3(0);


    // Loop over the patterns:
    for (int i=0; i<eAli_maxNpatterns; i++) {
        if (i>56) {
            cout << "WARNING     EdbPVRQuality::Execute_RandomCut    Your EdbPVRec object has more than 57 patterns! " << endl;
            cout << "WARNING     EdbPVRQuality::Execute_RandomCut    Check it! " << endl;
        }

        cout << "EdbPVRQuality::Execute_RandomCut   Set eCutp1[i] to the initial eRandomCutThreshold = " << eRandomCutThreshold << endl;
        eCutp1[i]=eRandomCutThreshold;

        if (gEDBDEBUGLEVEL>2) cout << "Execute_RandomCut   Doing Pattern " << i << endl;

        // Now the condition loop:
        // Loop over 50 steps to determine new BG rate....s
        for (int l=0; l<50; l++) {
            eHistYX->Reset(); // important to clean the histogram
            eHistChi2W->Reset(); // important to clean the histogram
            histPatternBTDensity->Reset(); // important to clean the histogram
            eHistTYTX->Reset(); // important to clean the histogram
            eHistTT->Reset(); // important to clean the histogram

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
                    }
                    else {
                        result = kFALSE;
                    }
                }

                if (gEDBDEBUGLEVEL>4)  cout << "Doing segment " << j << " result for bool query is: " << result << endl;

                // Main decision for segment to be kept or not (seg is of MC or data type).
                if ( kFALSE == result ) continue;

                // Random CutMethod:
                if (RandomGenerator->Uniform()>eCutp1[i]) continue;

                // For the check, fill the histograms in any case:
                eHistYX->Fill(seg->Y(),seg->X());
                eHistTYTX->Fill(seg->TY(),seg->TX());
                eHistTT->Fill(TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX()));
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
                if (gEDBDEBUGLEVEL>2)  cout << "Execute_RandomCut      We reached the loop end due to random cut based condition  ... ... and break loop." << endl;
                break;
            }
            else {
                // Next step, tighten cut:
                eCutp1[i] += -0.02;
            }

        } // of condition loop...

        // Fill target profile histogram:
        Float_t bincontentXY=histPatternBTDensity->GetMean();
        eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

    } // of Npattern loops..

    eCutMethodIsDone[6]=kTRUE;

    // Check if modified or original PVRec object should be returned:
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() Check if modified or original PVRec object should be returned: " << endl;
        cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity() " << eNeedModified << endl;
    }

    /*
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
        */
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

    Log(2,"EdbPVRQuality::Execute_RandomCut","Execute_RandomCut...done.");
    return;
}

//______________________________________________________________________________
































//___________________________________________________________________________________

Int_t EdbPVRQuality::GetAngularSpaceBin(EdbSegP* seg)
{
    // Returns the number of bin for which the Basetrack corresponds to in tangens theta space.
    // Goes from 0 to 20.

    // ATTENTION and TODO :
    // Make the values in GetAngularSpaceBin() and Execute_ConstantBTDensityInAngularBins() equal !
    // done by hand... should by donesomehow automatic

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


EdbPVRec* EdbPVRQuality::CreateEdbPVRec()
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
        cout << "-----     determined in Execute_ConstantBTDensity, Execute_ConstantBTQuality" << endl;
        cout << "-----     " << endl;
        cout << "-----     WARNING: the couples structure and the tracking structure" << endl;
        cout << "-----     will be lost, this PVR object is only useful for the" << endl;
        cout << "-----     list of Segments (==ShowReco...) ... " << endl;
        cout << "-----     DO NOT USE THIS ROUTINE FOR GENERAL I/O and/or EdbPVRec operations!" << endl;
        cout << "-----     " << endl;
        cout << "CreateEdbPVRec()  Mode 0:" << eCutMethodIsDone[0] << endl;
        cout << "CreateEdbPVRec()  Mode 1:" << eCutMethodIsDone[1] << endl;
        cout << "CreateEdbPVRec()  Mode 2:" << eCutMethodIsDone[2] << endl;
        cout << "CreateEdbPVRec()  Mode 3:" << eCutMethodIsDone[3] << endl;
        cout << "CreateEdbPVRec()  Mode 4:" << eCutMethodIsDone[4] << endl;
        cout << "CreateEdbPVRec()  Mode 5:" << eCutMethodIsDone[5] << endl;
        cout << "CreateEdbPVRec()  Mode 6:" << eCutMethodIsDone[6] << endl;
        cout << "CreateEdbPVRec()  Mode 7:" << eCutMethodIsDone[7] << endl;
        cout << "-----     " << endl;
        cout << "-----     ----------------------------------------------" << endl;
    }

    // Checks
    if (NULL==eAli_orig || eIsSource==kFALSE) {
        cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    NULL==eAli_orig   || eIsSource==kFALSE    return."<<endl;
        return NULL;
    }
    // Checks
    // should be revised, such that the condition reads: "at least one cutmethod had to be done."
    Bool_t doStop=kFALSE;
    for (int i=0; i<8; i++) if (eCutMethodIsDone[i]==kTRUE) doStop=kTRUE;
    if (doStop!=kTRUE) {
        cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    No eCutMethodIsDone[..] was done. STOP and DONT create a new EdbPVR."<<endl;
        return NULL;
    }

    // Make a new PVRec object anyway
    eAli_modified = new EdbPVRec();

    // These two lines dont compile yet ... (???) ...
    // 	EdbScanCond* scancond = eAli_orig->GetScanCond();
    // 	eAli_modified->SetScanCond(*scancond);

    Float_t agreementChi2;
    Int_t angularspacebin=0;

    // Type 4:
    Float_t m_chi2,s_chi2,m_WTilde, s_WTilde;
    Float_t X2Hat,WTildeNormalized,WTilde,chi2Normalized,chi2;

    // This makes pointer copies of patterns with segments list.
    // wARNING: the couples structure and the tracking structure
    // will be lost, this PVR object is only useful for the
    // list of Segments (==ShowReco...) ...

    // Priority has the Execute_ConstantBTQuality cut.
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
            else if (eCutMethodIsDone[2]==kTRUE) {
                // Constant Quality cut
                agreementChi2=TMath::Sqrt( ( (seg->Chi2()-eAgreementChi2CutMeanChi2)/eAgreementChi2CutRMSChi2)*((seg->Chi2()-eAgreementChi2CutMeanChi2)/eAgreementChi2CutRMSChi2)  +   ((seg->W()-eAgreementChi2CutMeanW)/eAgreementChi2CutRMSW)*((seg->W()-eAgreementChi2CutMeanW)/eAgreementChi2CutRMSW) );
                if (agreementChi2>eAgreementChi2WDistCut[i]) continue;
            }
            else if (eCutMethodIsDone[3]==kTRUE) {
                // Constant Quality cut  also in angular space:
                cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    eCutMethodIsDone[2]==kTRUE BUT NOT YET IMPLEMENTED" << endl;
            }
            else if (eCutMethodIsDone[1]==kTRUE) {
                // Constant BT density cut also in angular space:
                angularspacebin = GetAngularSpaceBin(seg);
                if (seg->Chi2() >= seg->W()* eCutTTp1[i][angularspacebin] - eCutTTp0[i][angularspacebin]) continue;
            }
            else if (eCutMethodIsDone[4]==kTRUE) {
                // Constant X2Hat cut:
                m_chi2=eXi2Hat_m_chi2[i];
                s_chi2=eXi2Hat_s_chi2[i];
                m_WTilde=eXi2Hat_m_WTilde[i];
                s_WTilde=eXi2Hat_s_WTilde[i];
                // Calculate that Xi2Hat value, since we have now mean and rms of the distribution histograms:
                // Given that these histograms were not deleted...
                chi2Normalized=TMath::Power((seg->Chi2()-m_chi2)/s_chi2,2);
                WTilde=1./(seg->W());
                WTildeNormalized=TMath::Power((WTilde-m_WTilde)/s_WTilde,2);
                X2Hat=chi2Normalized + WTildeNormalized;
                if (X2Hat>eX2HatCut[i]) continue;
            }
            else if (eCutMethodIsDone[5]==kTRUE) {
                // Constant X2Hat cut also in angular space:
                angularspacebin = GetAngularSpaceBin(seg);
                m_chi2=eXi2HatTT_m_chi2[i][angularspacebin];
                s_chi2=eXi2HatTT_s_chi2[i][angularspacebin];
                m_WTilde=eXi2HatTT_m_WTilde[i][angularspacebin];
                s_WTilde=eXi2HatTT_s_WTilde[i][angularspacebin];

                // Calculate that Xi2Hat value, since we have now mean and rms of the distribution histograms:
                chi2Normalized=TMath::Power((seg->Chi2()-m_chi2)/s_chi2,2);
                WTilde=1./(seg->W());
                WTildeNormalized=TMath::Power((WTilde-m_WTilde)/s_WTilde,2);
                X2Hat=chi2Normalized + WTildeNormalized;
                if (X2Hat> eCutTTp1[i][angularspacebin]) continue;
            }
            else if (eCutMethodIsDone[6]==kTRUE) {
                // Random Uniform Cut
                if (gRandom->Uniform()<eCutp1[i]) continue;
            }
            else if (eCutMethodIsDone[7]==kTRUE) {
                // Random Uniform Cut also in angular space:
                angularspacebin = GetAngularSpaceBin(seg);
                if (gRandom->Uniform()>eCutTTp1[i][angularspacebin]) continue;
                // Nota Bene: the ">" is right here, since it is also in the
                // corresponding Execute_EqualizeTanThetaSpace_RandomCut routine.
            }
            else {
                // do nothing;
                cout << "WARNING! YOU CHOSE AN INVALID OPTION. DO NO CUT AT ALL! TAKE ALL SEGMENTS!" << endl;

                cout << " ..........................................................." << endl;
                cout << " ..........................................................." << endl;
                cout << " ..........................................................." << endl;

                cout << " ..........................................................." << endl;

                cout << "    BUT I COULD IMPLEMENT HERE THE EXCLUSION SEGMET LIST OPTION    " << endl;


                cout << " ..........................................................." << endl;

                cout << " ..........................................................." << endl;
                cout << " ..........................................................." << endl;
            }

            // Add segment:
            pt->AddSegment(*seg);
        }
        eAli_modified->AddPattern(pt);
    }

    eIsTarget=kTRUE;

    //---------------------
    cout << "EdbPVRQuality::CreateEdbPVRec()...Created new EdbPVR object at address " << eAli_modified << endl;
    cout << "EdbPVRQuality::CreateEdbPVRec()...You can also retrieve this object via   ->GetEdbPVRec(1);" << endl;
    //---------------------
    cout << "EdbPVRQuality::CreateEdbPVRec()...done." << endl;
    return eAli_modified;
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
    // Basic Help Function. Of course to be extended from time to time.

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
    cout << "-----     Constant" << endl;
    cout << "-----     BT Density Method: ..." << endl;
    cout << "-----" << endl;
    cout << "-----     Constant" << endl;
    cout << "-----     BT Quality Method: ..." << endl;
    cout << "-----" << endl;
    cout << "-----     Constant" << endl;
    cout << "-----     Xi2Hat Method: Fill a Chi2() and a Wtilde=1/W() histogram. Get Mean and Sigma" << endl;
    cout << "-----                  : of the two Histograms. Then construct a chi-square like" << endl;
    cout << "-----                  : variable: Xi2Hat=((Chi2-meanChi2)/sigmaChi2)^2         " << endl;
    cout << "-----                  : variable:       +((Wtilde-meanWtilde)/sigmaWtilde)^2   " << endl;
    cout << "-----                  : Cut is then done by reducing the Xi2Hat limit by 0.1" << endl;
    cout << "-----                  : starting from an initial value of 5.0" << endl;
    cout << "-----" << endl;
    cout << "-----  " << endl;

    cout << "-----     void EdbPVRQuality::Help()     -----" << endl;
    cout << "----------------------------------------------" << endl;
}

//___________________________________________________________________________________



TObjArray* EdbPVRQuality::FindDoubleBT(EdbPVRec* aliSource)
{
    // Find double counted basetracks.
    // More explanation in Remove_DoubleBT() function.
    // Cutvalues used are the same.
    // (See also upcoming note on BG-reduction)

    cout << "EdbPVRQuality::FindDoubleBT() " << endl;
    cout << "EdbPVRQuality::FindDoubleBT()  Check pattern volume for (fake) BT pairs." << endl;
    cout << "EdbPVRQuality::FindDoubleBT()  aliSource = " << aliSource << endl;
    cout << "EdbPVRQuality::FindDoubleBT()  eAli_orig = " << eAli_orig << endl;

    if (NULL==aliSource) {
        cout << "WARNING!   EdbPVRQuality::FindDoubleBT()  Source EdbPVRec is NULL. Try to change to object eAli_orig: " << eAli_orig << endl;
        if (NULL==eAli_orig) {
            cout << "WARNING!   EdbPVRQuality::FindDoubleBT() Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
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

    // Bool variables for the different "regions" of the FakeBT Doublets
    Bool_t IsRegion0;
    Bool_t IsRegion1;
    Bool_t IsRegion2;

    //gEDBDEBUGLEVEL=3;
    cout << "EdbPVRQuality::FindDoubleBT()   Start looping now. Attention, this might take a while!" << endl;

    if (aliSource->Npatterns()==0) return NULL;

    for (int i = 0; i <aliSource->Npatterns(); i++ ) {

        // Get Target Pattern:
        EdbPattern* pat = aliSource->GetPattern(i);
        // Helper Variable for cout purpose
        Int_t nPat=pat->N();

        if (gEDBDEBUGLEVEL>1) cout << "EdbPVRQuality::FindDoubleBT()   Looping over Ali_source->Pat()=" << i << " (PID=" << pat->PID() << ") with N= " <<  nPat << " segs. Until now double Candidates found: " << NdoubleFoundSeg << endl;

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
                // Segment Pairs can fall into three regions:
                //
                // region 0) all BT pairings within dT<->dR line
                //
                // region 1) all BT parings within a dR,dT rectangle at the origin
                //
                // region 2) all BT parings within a dR,dT rectangle at (0,6)
                //
                Float_t dR_allowed=0;
                Float_t deltaR=0;
                Float_t deltaTheta=0;
                Float_t dR_allowed_m_deltaR=0;

                // Calculate cut values:
                deltaTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                if (deltaTheta>0.1) continue;
                deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );

                // line defined by experimental values (by some arbitrary bricks and my own BG-bricks)
                // dR_allowed = 10.75/0.1*deltaTheta; // old
                dR_allowed = 14.0/0.13*deltaTheta; // better description of the line
                dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);

                // Region check of the (Fake-)BTPair
                IsRegion0=kFALSE;
                IsRegion1=kFALSE;
                IsRegion2=kFALSE;

                // Set flags for region
                if (dR_allowed_m_deltaR<0.5) IsRegion0=kTRUE;
                if (deltaR<3.0 && deltaTheta<0.01) IsRegion1=kTRUE;
                if (abs(deltaR-6.0)<2 && deltaTheta<0.01) IsRegion2=kTRUE;

                // For this function now -in contrary to Remove_DoubleBT- we
                // ONLY keep double basetrack pairings within the line
                if (IsRegion0) toKeep=kTRUE;
                // or within the small square at the origin
                if (IsRegion1) toKeep=kTRUE;
                // or within the small square at the dR value
                if (IsRegion2) toKeep=kTRUE;

                if (!toKeep) continue;

                if (gEDBDEBUGLEVEL>2) cout << "EdbPVRQuality::FindDoubleBT()   Found segment compatible close to another one: seg (" <<seg->PID() << ","<<  seg->ID() << ") is close to seg  (" << seg1->PID()<< ","<< seg1->ID()  << ")." << endl;

                if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::FindDoubleBT()   Do last check if both are MCEvt segments of different event number! " << endl;
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

    cout << "EdbPVRQuality::FindDoubleBT() Statistics: " << endl;
    cout << "EdbPVRQuality::FindDoubleBT() We found " << DoubleBTArray->GetEntries()  << " double segments too close to each other to be different basetracks." << endl;

    cout << "EdbPVRQuality::FindDoubleBT()...done." << endl;
    return DoubleBTArray;

}


//___________________________________________________________________________________


EdbPVRec* EdbPVRQuality::CreatePVRWithExcludedSegmentList(EdbPVRec* aliSource, TObjArray *SegmentArray)
{
    // Create a new volume, fill with all tracks from the old volume,
    // _except_ those which appear in the excluded segment list.
    // Quick and Dirty implementation !
    cout << "EdbPVRQuality::CreatePVRWithExcludedSegmentList() " << endl;

    EdbPVRec* aliClone = (EdbPVRec*)aliSource->Clone();

    Int_t SegmentArrayN = SegmentArray->GetEntries();
    Bool_t SegmentInSegmentArray=kFALSE;


    for (Int_t i=0; i<aliSource->Npatterns(); ++i) {
        EdbPattern* patternSource = aliSource->GetPattern(i);
        EdbPattern* patternTarget = aliClone->GetPattern(i);
        cout <<"patternTarget->GetEntries()" << patternTarget->GetN() << endl;
        patternTarget->Reset();

        patternTarget->SetID(patternSource->ID());
        patternTarget->SetPID(patternSource->PID());
        patternTarget->SetX(patternSource->X());
        patternTarget->SetY(patternSource->Y());
        patternTarget->SetZ(patternSource->Z());

        // Now every basetrack from the original pattern must be compared
        // with all basetracks from the exclusion list!
        // If the basetrack is not found in the exclusion list, then
        // it can be added in the target pattern...

        for (Int_t j=0; j<patternSource->N(); ++j) {
            EdbSegP* seg = patternSource->GetSegment(j);
            SegmentInSegmentArray=kFALSE;

            for (Int_t iSegArray=0; iSegArray<SegmentArrayN; ++iSegArray) {
                EdbSegP* segFromArray = (EdbSegP*)SegmentArray->At(iSegArray);
                if (seg->IsEqual(segFromArray)) {
                    cout << "Segment " <<  seg->ID() << " is in exclusion List:  DO NOT ADD THIS IN patternTarget" << endl;
                    SegmentInSegmentArray=kTRUE;
                }
            }
            // Add segment now:
            if(!SegmentInSegmentArray) patternTarget->AddSegment(*seg);

        }
        cout <<"patternTarget->GetEntries()" << patternTarget->GetN() << endl;

    } // for (Int_t i=0; i<aliSource->N(); ++i)

    aliClone->Print();


    cout << "EdbPVRQuality::CreatePVRWithExcludedSegmentList()...done." << endl;
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
    // aliTarget->Print();

    Bool_t seg_seg_close=kFALSE;
    EdbSegP* seg=0;
    EdbSegP* seg1=0;
    Int_t NdoubleFoundSeg=0;
    Int_t NdoubleFoundSegPatterns[114];

    // Bool variables for the different "regions" of the FakeBT Doublets
    Bool_t IsRegion0;
    Bool_t IsRegion1;
    Bool_t IsRegion2;

    //gEDBDEBUGLEVEL=3;
    cout << "EdbPVRQuality::Remove_DoubleBT()   Start looping now. Attention, this might take a while!" << endl;

    if (aliSource->Npatterns()==0) return aliSource;
    for (int i = 0; i <aliSource->Npatterns(); i++ ) {
        NdoubleFoundSegPatterns[i]=0;
    }


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
                // Here decide f.e. which segments to check...
                Bool_t toKeep=kFALSE;


                // Skip already large distances
                if (TMath::Abs(seg->X()-seg1->X())>20.1) continue;
                if (TMath::Abs(seg->Y()-seg1->Y())>20.1) continue;
                if (TMath::Abs(seg->TX()-seg1->TX())>0.1) continue;
                if (TMath::Abs(seg->TY()-seg1->TY())>0.1) continue;
                //
                // Segment Pairs can fall into three regions:
                //
                // region 0) all BT pairings within dT<->dR line
                //
                // region 1) all BT parings within a dR,dT rectangle at the origin
                //
                // region 2) all BT parings within a dR,dT rectangle at (0,6)
                //
                Float_t dR_allowed=0;
                Float_t deltaR=0;
                Float_t deltaTheta=0;
                Float_t dR_allowed_m_deltaR=0;

                deltaTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                if (deltaTheta>0.1) continue;

                deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );


                // line defined by experimental values (by some arbitrary bricks and my own BG-bricks)
                // dR_allowed = 10.75/0.1*deltaTheta; // old
                dR_allowed = 14.0/0.13*deltaTheta; // better description of the line
                dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);

                // Region check of the (Fake-)BTPair
                IsRegion0=kFALSE;
                IsRegion1=kFALSE;
                IsRegion2=kFALSE;

                // Set flags for region
                if (dR_allowed_m_deltaR<0.5) IsRegion0=kTRUE;
                if (deltaR<3.0 && deltaTheta<0.01) IsRegion1=kTRUE;
                if (abs(deltaR-6.0)<2 && deltaTheta<0.01) IsRegion2=kTRUE;

                // For this function now -in contrary to Remove_DoubleBT- we
                // ONLY keep double basetrack pairings within the line
                if (IsRegion0) toKeep=kTRUE;
                // or within the small square at the origin
                if (IsRegion1) toKeep=kTRUE;
                // or within the small square at the dR value
                if (IsRegion2) toKeep=kTRUE;

                if (!toKeep) continue;

                if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_DoubleBT()   Found incompatible segment for dR_dT line condition: " << endl;

                // if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_DoubleBT()   Do last check if both are MCEvt segments of different event number! " << endl;
                if ((seg->MCEvt()!=seg1->MCEvt())&&(seg->MCEvt()>0&&seg1->MCEvt()>0)) continue;
                // I have doubts if I shall take MC events also out, but I guess Yes, because if
                // in data these small pairings appear, then they will fall also under the
                // category "fake doublet" and then be removed, even if they are real double BT
                // from a signal.


                ++NdoubleFoundSeg;
                ++NdoubleFoundSegPatterns[i];
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

    if (gEDBDEBUGLEVEL>1) {
        cout << "EdbPVRQuality::Remove_DoubleBT() Statistics: " << endl;
        cout << "EdbPVRQuality::Remove_DoubleBT() We found " << NdoubleFoundSeg  << " double segments too close to each other to be different BTracks." << endl;
        cout << "EdbPVRQuality::Remove_DoubleBT() DoubleBT Statistics per pattern: " << endl;
        cout << setw(14) << "Pattern ID" << setw(14) << "#Segments = " << setw(14) << " #DoubleBT " << setw(14) << " Overhead(%) " << endl;

        for (int i = 0; i <aliSource->Npatterns(); i++ ) {
            EdbPattern* pat = aliSource->GetPattern(i);
            cout <<  setw(14) << pat->ID() << setw(14) <<  pat->N() << setw(14) << NdoubleFoundSegPatterns[i] << setw(14) << Double_t(NdoubleFoundSegPatterns[i])/(pat->N()+1) << endl;
        }
    }

    cout << "EdbPVRQuality::Remove_DoubleBT()...done." << endl;
    return aliTarget;
}

//___________________________________________________________________________________

EdbPVRec* EdbPVRQuality::Remove_Passing(EdbPVRec* aliSource)
{
    // Removes Passing Tracks from the EdbPVRec source object.
    // Unfortunately, there does NOT exist an easy function like
    // ->RemoveSegment from EdbPVRec object. That makes implementation complicated
    // and we have to go via the intermediate step of creating a new
    // EdbPVRec volume.

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
//             // Arrived here then the segment has no equivalent in any segments
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


//___________________________________________________________________________________


void EdbPVRQuality::PrintCutValues(Int_t CutType) {
    cout <<"-----------------------------------------------------------------------------------------------------------------------" << endl;
    cout <<"EdbPVRQuality::PrintCutValues   for CutType= " << CutType << endl;
    cout <<"-----------------------------------------------------------------------------------------------------------------------" << endl;
    if (CutType==1 || CutType==5 || CutType==7 ) {
        cout << "#plate  #tt[0.0,0.1], #tt[0.1,0.2], #tt[0.2,0.3], #tt[0.3,0.4], #tt[0.4,0.5], #tt[0.5,0.6], #tt[0.6,0.7], #tt[0.7,0.8]" << endl;
        cout <<"-----------------------------------------------------------------------------------------------------------------------" << endl;
        for (int i=0; i<eAli_maxNpatterns; i++) {
            cout << i << "      ";
            // npatterns of eali...
            for (int angspacecounter=0; angspacecounter<8; angspacecounter++) {
                printf(" %1.3f        ", eCutTTp1[i][angspacecounter]);
            }
            cout << endl;

            if (CutType==5) {
                cout << i << "      ";
                // npatterns of eali...
                for (int angspacecounter=0; angspacecounter<8; angspacecounter++) {
                    printf(" %1.3f        ", eCutTTp0[i][angspacecounter]);
                }
                cout << endl;
            }
        }
        cout <<"-----------------------------------------------------------------------------------------------------------------------" << endl;
    } // if (CutType==...)
    cout <<"EdbPVRQuality::PrintCutTypeDetailed...done." << endl;
    return;
}


//___________________________________________________________________________________

void EdbPVRQuality::Execute_EqualizeTanThetaSpace()
{
    /// TODO !!! SWITCH HERE BETWEEN THE RIGHT SWITCH !!!
//   Execute_EqualizeTanThetaSpace_ConstantBTDensity();
//   Execute_EqualizeTanThetaSpace_ConstantBTQuality();
//   Execute_EqualizeTanThetaSpace_ConstantBTX2Hat();
//   Execute_EqualizeTanThetaSpace_RandomCut();

    PrintCutValues(eCutMethod);
    return;
}

//___________________________________________________________________________________



void EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity()
{
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity" << endl;

    SetHistGeometry_OPERAandMCBinArea625();

    if (!eIsSource) return;
    if (NULL==eAli_orig) return;

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensityTT","histPatternBTDensityTT",200,0,200);
    TH1F* histPatternBTDensities[10]; // for the angular bins:
    for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
        histPatternBTDensities[angspacecounter] = new TH1F(Form("histPatternBTDensities_%d",angspacecounter),Form("histPatternBTDensities_%d",angspacecounter),200,0,200);
    }
    Double_t angularspacebinningwidth=0.1;
    Double_t angularspacebinningstart=0.00;
    Double_t angularspacebinningend=0.00;
    Double_t angularspacebinningScaleFactor=1;
    Double_t tt=0;
    Double_t ni[20];
    Double_t NumberOfFilledTTbins=0;
    TH1F* eHistTTClone = (TH1F*)eHistTT->Clone();
    Int_t BTNumberLevelAngularSpace[20];
    for (int i=0; i<1; i++) {
        BTNumberLevelAngularSpace[i]=0;
    }



    //---------------------------------------------------------------------
    // Loop over the patterns:
    for (int i=0; i<eAli_maxNpatterns; i++) {
//     for (int i=0; i<1; i++) {

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();
        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   For this (" << i << ") pattern, I have " << npat << " Edb segments to check..." << endl;

        // Reset eBTDensity levels:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
            eBTDensityLevelAngularSpace[angspacecounter]=eBTDensityLevel;
            // Maximum Cut Value for const, BT dens, also in tangens theta space
            eCutTTp0[i][angspacecounter]=1.00;
            eCutTTp1[i][angspacecounter]=0.15;
        }

        // Now do the angular space binning loop:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
            cout << endl << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   Doing now the angspacecounter loop:  angspacecounter = " << angspacecounter  << endl;

            // Calculate right end of angular space bin
            angularspacebinningstart=(Double_t)angspacecounter*angularspacebinningwidth;
            angularspacebinningend=angularspacebinningstart+angularspacebinningwidth;

            cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   Doing TT Bin from " << angularspacebinningstart << " to " << angularspacebinningend << endl;

            // Now the condition loop:
            // Loop over 20 ( or 40 ) steps a 0.15,0.145,0.14 ...  down to 0.07
            Int_t lmax=20;

            for (Int_t l=0; l<lmax; l++) {
                // Shorter Handings:
                Double_t Cutp0 = eCutTTp0[i][angspacecounter];
                Double_t Cutp1 = eCutTTp1[i][angspacecounter];

                cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   Doing now the condition loop:  l = " << l << ", the cut condition reads:  seg->Chi2() >= seg->W()* " << Cutp1 << " - " << Cutp0 << endl;

                // Important to clean the histograms:
                eHistYX->Reset(); // important to clean the histogram
                eHistYX->SetMinimum(1);
                eHistYX->SetMaximum(250); /// to be calculated accordingly !!
                eHistChi2W->Reset(); // important to clean the histogram
                histPatternBTDensity->Reset(); // important to clean the histogram
                eHistTT->Reset();  // important to clean the histogram
                eHistTTClone->Reset();  // important to clean the histogram
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
                        }
                        else {
                            result = kFALSE;
                        }
                    }
                    // Main decision for segment to be kept or not (seg is of MC or data type).
                    if ( kFALSE == result ) continue;

                    // Calculate Angle
                    tt=TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX());

                    // Fill Clone histo. This is needed for the polynomial fit.
                    eHistTTClone->Fill(tt);

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

                // Do the quadratic fit for the minimumTTBin only at the first time.
                if (l==0) {
                    eHistTTClone->Fit("pol2","0q","0q",0,0.5);
                    TF1 *myfunc = eHistTTClone->GetFunction("pol2");
                    Double_t par0 = myfunc->GetParameter(0);
                    Double_t par1 = myfunc->GetParameter(1);
                    Double_t par2 = myfunc->GetParameter(2);

                    Int_t minimumTTBin = eHistTTClone->FindBin( -par1/(2*par2));
                    Int_t nEntriesInMinimumBin = eHistTTClone->GetBinContent(minimumTTBin);
                    BTNumberLevelAngularSpace[angspacecounter] = Int_t(ni[angspacecounter]- eCutTTSqueezeFactor* Float_t(ni[angspacecounter]-nEntriesInMinimumBin));

                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   Minimum should be at TanTheta = " << -par1/(2*par2) << "  and it is in bin # " << minimumTTBin << " ; there are so many entries: " << nEntriesInMinimumBin << endl;
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   For bin " << angspacecounter << ", there are at the beginning so many segments: " << ni[angspacecounter] << endl;
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   BTNumberLevelAngularSpace[angspacecounter] = " << BTNumberLevelAngularSpace[angspacecounter] << endl;
                } // if (l==0)

                int nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
                int NemptyBins=0;
                int NFilledBins=0;
                for (int k=1; k<nbins-1; k++) {
                    if (eHistYX->GetBinContent(k)==0) {
                        ++NemptyBins;
                        continue;
                    }
                    // due to the changed areasize in eHistYX, adapt the fill weight
                    histPatternBTDensity->Fill(Double_t(eHistYX->GetBinContent(k))/ 6.25 );
                    histPatternBTDensities[angspacecounter]->Fill(eHistYX->GetBinContent(k)/ 6.25);
                    ++NFilledBins;
                }
                ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();
                Float_t ePatternBTDensityRMS_modified = histPatternBTDensity->GetRMS();

                if (gEDBDEBUGLEVEL>1) {
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   Loop l= " << l << ":  for the Cutp1 : " << Cutp1 <<   " we have #NBT: "  << ni[angspacecounter] << "+-" << sqrt(ni[angspacecounter]) <<", target number for this TT space bin = " << BTNumberLevelAngularSpace[angspacecounter] << endl;
                }

                // Now the condition check for angular space cut
                // This time, we do it in terms of Numbers Of Basetracks instead of BasetrackDensity/mm2
//                 if (ePatternBTDensity_modified[i]<=eBTDensityLevelAngularSpace[angspacecounter]) {
                if (ni[angspacecounter] <=BTNumberLevelAngularSpace[angspacecounter]) {
                    l=lmax;
                    // When we passed down _under_ the desired Number of basetrack tracks, we raise
                    // one last time the cuts, to come out at last one slight value over it....
                    if (lmax==20) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.005;
                    if (lmax==40) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.0025;

                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   We reached the loop end due to good BT density level in this angular bin and finish loop. Set l to lmax: " << l << endl;
                    // break; // no need to brake anymore since we set l to lmax....
                }
                else {
                    // Next step, tighten cut:
                    // lmax=20:
                    if (lmax==20) {
                        eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.005;
                    }
                    // l=40:
                    if (lmax==40) {
                        eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.0025;
                    }
                    if (l==lmax-1) {
                        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   ATTENTION! No more cuts to tighten up. Restore the last valid cut. Basetrack number wont go below last value!" << endl;
                        if (lmax==20) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.005;
                        if (lmax==40) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.0025;
                    }
                }

            } // of condition loop...

            // Fill target profile histogram:
            Float_t bincontentXY=histPatternBTDensity->GetMean();
            eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

            angularspacebinningstart+=angularspacebinningwidth;
        } // for (int angspacecounter=0; angspacecounter<20; angspacecounter++)

    } // of Npattern loops..
    //---------------------------------------------------------------------

    eCutMethodIsDone[1]=kTRUE;

    // Check if modified or original PVRec object should be returned:
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity    Check if modified or original PVRec object should be returned: " << endl;
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity    " << eNeedModified << endl;
    }

    delete histPatternBTDensity;
    delete eHistTTClone;

    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity   If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTDensity...done." << endl;
    return;
}








//___________________________________________________________________________________




void EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTQuality() {
    cout  << "TODO" << endl;
}



//___________________________________________________________________________________




























//___________________________________________________________________________________

void EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat() {
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat" << endl;

    SetHistGeometry_OPERAandMCBinArea625();

    if (!eIsSource) return;
    if (NULL==eAli_orig) return;

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensityTT","histPatternBTDensityTT",200,0,200);
    TH1F* histPatternBTDensities[10]; // for the angular bins:
    for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
        histPatternBTDensities[angspacecounter] = new TH1F(Form("histPatternBTDensities_%d",angspacecounter),Form("histPatternBTDensities_%d",angspacecounter),200,0,200);
    }
    Double_t angularspacebinningwidth=0.1;
    Double_t angularspacebinningstart=0.00;
    Double_t angularspacebinningend=0.00;
    Double_t angularspacebinningScaleFactor=1;
    Double_t tt=0;
    Double_t ni[20];
    Double_t NumberOfFilledTTbins=0;
    TH1F* eHistTTClone = (TH1F*)eHistTT->Clone();
    Int_t BTNumberLevelAngularSpace[20];
    for (int i=0; i<1; i++) {
        BTNumberLevelAngularSpace[i];
    }


    // Create the temporary needed histograms:
    TH1F* h_chi2 = new TH1F("h_chi2","h_chi2",100,0,2);
    TH1F* h_chi2Normalized = new TH1F("h_chi2Normalized","h_chi2Normalized",100,0,5);
    TH1F* h_WTilde = new TH1F("h_WTilde","h_WTilde",40,0,0.1);
    TH1F* h_WTildeNormalized = new TH1F("h_WTildeNormalized","h_WTildeNormalized",100,0,5);
    TH1F* h_X2 = new TH1F("h_X2","h_X2",100,0,5);

    //---------------------------------------------------------------------
    // Loop over the patterns:
    for (int i=0; i<eAli_maxNpatterns; i++) {
//     for (int i=0; i<1; i++) {

        Float_t chi2Normalized=0;
        Float_t WTilde=0;
        Float_t WTildeNormalized=0;
        Float_t X2Hat=0;
        Float_t m_chi2=0;
        Float_t s_chi2=0;
        Float_t m_WTilde=0;
        Float_t s_WTilde=0;

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();
        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   For this (" << i << ") pattern, I have " << npat << " Edb segments to check..." << endl;

        // Reset eBTDensity levels:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
            eBTDensityLevelAngularSpace[angspacecounter]=eBTDensityLevel;
            // Maximum Cut Value for ConstantBTX2Hat   , also in tangens theta space
            eCutTTp0[i][angspacecounter]=4.00; // not needed here.
            eCutTTp1[i][angspacecounter]=4.00; // -> eX2HatCut
        }


        // Now do the angular space binning loop:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
// 	  for (Int_t angspacecounter=0; angspacecounter<1; angspacecounter++) {
            cout << endl << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   Doing now the angspacecounter loop:  angspacecounter = " << angspacecounter  << endl;

            // Calculate right end of angular space bin
            angularspacebinningstart=(Double_t)angspacecounter*angularspacebinningwidth;
            angularspacebinningend=angularspacebinningstart+angularspacebinningwidth;

            cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   Doing TT Bin from " << angularspacebinningstart << " to " << angularspacebinningend << endl;

            // Now the condition loop:
            // Loop over 20 ( or 40 ) steps a 0.15,0.145,0.14 ...  down to 0.07
            Int_t lmax=20;

            for (Int_t l=0; l<lmax; l++) {
                // Shorter Handings:
                Double_t Cutp0 = eCutTTp0[i][angspacecounter];
                Double_t Cutp1 = eCutTTp1[i][angspacecounter];

                cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   Doing now the condition loop:  l = " << l << ", the cut condition reads:  Xi2Hat < " << Cutp1 << endl;

                // Important to clean the histograms:
                eHistYX->Reset(); // important to clean the histogram
                eHistYX->SetMinimum(1);
                eHistYX->SetMaximum(250); /// to be calculated accordingly !!
                eHistChi2W->Reset(); // important to clean the histogram
                histPatternBTDensity->Reset(); // important to clean the histogram
                eHistTT->Reset();  // important to clean the histogram
                eHistTTClone->Reset();  // important to clean the histogram
                histPatternBTDensities[angspacecounter]->Reset(); // important to clean the histogram

                // Clean also histograms that are used for the X2Hat variable:
                h_chi2->Reset();
                h_chi2Normalized->Reset();
                h_WTilde->Reset();
                h_WTildeNormalized->Reset();
                h_X2->Reset();

                // Reset Number of Segments Counter
                ni[angspacecounter]=0;

                // Now Get Pattern:
                EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
                Int_t npat=pat->N();
                EdbSegP* seg=0;

                // Loop over the segments.
                // Here do it two times, one time to get Mean and RMS of Chi2 and WTilde distribution.

                for (int j=0; j<npat; j++) {
                    seg=pat->GetSegment(j);
                    Bool_t result=kTRUE;
                    if (seg->MCEvt()>0) {
                        if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                            result = kTRUE;
                        }
                        else {
                            result = kFALSE;
                        }
                    }

                    // Main decision for segment to be kept or not (seg is of MC or data type).
                    if ( kFALSE == result ) continue;

                    // Calculate Angle
                    tt=TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX());

                    // Fill Clone histo. This is needed for the polynomial fit.
                    eHistTTClone->Fill(tt);

                    // Check if angle is in the desired angular space bin:
                    Int_t segttspacebin=GetAngularSpaceBin(seg);
                    if (segttspacebin!=angspacecounter)  continue;


                    // Pre-Fill the Chi2() and W() histograms to get Mean and RMS values right:
                    h_chi2->Fill(seg->Chi2());
                    WTilde=1./(seg->W());
                    h_WTilde->Fill(WTilde);


                    // Calulation of the histograms to get Mean and RMS values right:
                    m_chi2=h_chi2->GetMean();
                    s_chi2=h_chi2->GetRMS();
                    m_WTilde=h_WTilde->GetMean();
                    s_WTilde=h_WTilde->GetRMS();
                    eXi2HatTT_m_chi2[i][angspacecounter]=m_chi2;
                    eXi2HatTT_s_chi2[i][angspacecounter]=s_chi2;
                    eXi2HatTT_m_WTilde[i][angspacecounter]=m_WTilde;
                    eXi2HatTT_s_WTilde[i][angspacecounter]=s_WTilde;


                } // Loop over the segments. For the first time...

                if (l==0) {
                    cout << "Loop over the segments. For the first time... done. Information about the histograms:" << endl;
                    cout << "h_chi2:   Mean , Sigma: " << m_chi2 << "   " << s_chi2 << endl;
                    cout << "h_WTilde: Mean , Sigma: " << m_WTilde << "   " << s_WTilde << endl;
                }

                // Now that we have the mean and sigmas of the Xi2Hat distribution
                // We loop over the segments once again.
                // Second loop.
                for (int j=0; j<npat; j++) {
                    seg=pat->GetSegment(j);
                    Bool_t result=kTRUE;
                    if (seg->MCEvt()>0) {
                        if (eBTDensityLevelCalcMethodMCConfirmationNumber==18&&eBTDensityLevelCalcMethodMC==kTRUE) {
                            result = kTRUE;
                        }
                        else {
                            result = kFALSE;
                        }
                    }
                    // Main decision for segment to be kept or not (seg is of MC or data type).
                    if ( kFALSE == result ) continue;

                    // Calculate Angle
                    tt=TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX());

                    // Fill Clone histo. This is needed for the polynomial fit.
                    // Already done in the first time loop
                    // eHistTTClone->Fill(tt);

                    // Check if angle is in the desired angular space bin:
                    Int_t segttspacebin=GetAngularSpaceBin(seg);
                    if (segttspacebin!=angspacecounter)  continue;

                    // Calculate that Xi2Hat value, since we have now mean and rms of the distribution histograms:
                    chi2Normalized=TMath::Power((seg->Chi2()-m_chi2)/s_chi2,2);
                    h_chi2Normalized->Fill(chi2Normalized);
                    WTilde=1./(seg->W());
                    WTildeNormalized=TMath::Power((WTilde-m_WTilde)/s_WTilde,2);
                    h_WTildeNormalized->Fill(WTildeNormalized);
                    X2Hat=chi2Normalized + WTildeNormalized;
                    h_X2->Fill(X2Hat);


                    // Cut on the Xi2Hat Value:
                    if (X2Hat>eCutTTp1[i][angspacecounter]) continue;

                    // Increase counter for number of taken segments in tan theta bin:
                    ni[angspacecounter] += 1;
                    // Fill other histograms anyway
                    eHistYX->Fill(seg->Y(),seg->X());
                    eHistTYTX->Fill(seg->TY(),seg->TX());
                    eHistTT->Fill(tt);
                    eHistChi2W->Fill(seg->W(),seg->Chi2());
                } // Loop over the segments. For the second time. Done.







                // Do the quadratic fit for the minimumTTBin only at the first time.
                if (l==0) {
                    eHistTTClone->Fit("pol2","0q","0q",0,0.5);
                    TF1 *myfunc = eHistTTClone->GetFunction("pol2");
                    Double_t par0 = myfunc->GetParameter(0);
                    Double_t par1 = myfunc->GetParameter(1);
                    Double_t par2 = myfunc->GetParameter(2);

                    Int_t minimumTTBin = eHistTTClone->FindBin( -par1/(2*par2));
                    Int_t nEntriesInMinimumBin = eHistTTClone->GetBinContent(minimumTTBin);
                    BTNumberLevelAngularSpace[angspacecounter] = Int_t(ni[angspacecounter]- eCutTTSqueezeFactor* Float_t(ni[angspacecounter]-nEntriesInMinimumBin));

                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   Minimum should be at TanTheta = " << -par1/(2*par2) << "  and it is in bin # " << minimumTTBin << " ; there are so many entries: " << nEntriesInMinimumBin << endl;
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   For bin " << angspacecounter << ", there are at the beginning so many segments: " << ni[angspacecounter] << endl;
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   BTNumberLevelAngularSpace[angspacecounter] = " << BTNumberLevelAngularSpace[angspacecounter] << endl;
                } // if (l==0)

                int nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
                int NemptyBins=0;
                int NFilledBins=0;
                for (int k=1; k<nbins-1; k++) {
                    if (eHistYX->GetBinContent(k)==0) {
                        ++NemptyBins;
                        continue;
                    }
                    // due to the changed areasize in eHistYX, adapt the fill weight
                    histPatternBTDensity->Fill(Double_t(eHistYX->GetBinContent(k))/ 6.25 );
                    histPatternBTDensities[angspacecounter]->Fill(eHistYX->GetBinContent(k)/ 6.25);
                    ++NFilledBins;
                }
                ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();
                Float_t ePatternBTDensityRMS_modified = histPatternBTDensity->GetRMS();

                // leave also if only one entry in the histogram
                if (NFilledBins<=1) {
                    l=lmax;
                    cout << "Attention! Only one entry in the histogram" << endl;
                }

                if (gEDBDEBUGLEVEL>1) {
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   Loop l= " << l << ":  for the Cutp1 : " << Cutp1 <<   " we have #NBT: "  << ni[angspacecounter] << "+-" << sqrt(ni[angspacecounter]) <<", target number for this TT space bin = " << BTNumberLevelAngularSpace[angspacecounter] << endl;
                }

                // Now the condition check for angular space cut
                // This time, we do it in terms of Numbers Of Basetracks instead of BasetrackDensity/mm2
//                 if (ePatternBTDensity_modified[i]<=eBTDensityLevelAngularSpace[angspacecounter]) {
                if (ni[angspacecounter] <=BTNumberLevelAngularSpace[angspacecounter]) {
                    l=lmax;
                    // When we passed down _under_ the desired Number of basetrack tracks, we raise
                    // one last time the cuts, to come out at last one slight value over it....
                    if (lmax==20) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.2;
                    if (lmax==40) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.1;

                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   We reached the loop end due to good BT density level in this angular bin and finish loop. Set l to lmax: " << l << endl;
                    // break; // no need to brake anymore since we set l to lmax....
                }
                else {
                    // Next step, tighten cut:
                    // lmax=20:
                    if (lmax==20) {
                        eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.2;
                    }
                    // l=40:
                    if (lmax==40) {
                        eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.1;
                    }
                    if (l==lmax-1) {
                        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   ATTENTION! No more cuts to tighten up. Restore the last valid cut. Basetrack number wont go below last value!" << endl;
                        if (lmax==20) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.2;
                        if (lmax==40) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.1;
                    }
                }

            } // of condition loop...

            // Fill target profile histogram:
            Float_t bincontentXY=histPatternBTDensity->GetMean();
            eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

            angularspacebinningstart+=angularspacebinningwidth;
        } // for (int angspacecounter=0; angspacecounter<20; angspacecounter++)

    } // of Npattern loops..
    //---------------------------------------------------------------------

    eCutMethodIsDone[5]=kTRUE;

    // Check if modified or original PVRec object should be returned:
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat    Check if modified or original PVRec object should be returned: " << endl;
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat    " << eNeedModified << endl;
    }

    delete histPatternBTDensity;
    delete eHistTTClone;

    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat   If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_ConstantBTX2Hat...done." << endl;
    return;
}


//___________________________________________________________________________________


void EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut() {
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut" << endl;

    SetHistGeometry_OPERAandMCBinArea625();

    if (!eIsSource) return;
    if (NULL==eAli_orig) return;

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensityTT","histPatternBTDensityTT",200,0,200);
    TH1F* histPatternBTDensities[10]; // for the angular bins:
    for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
        histPatternBTDensities[angspacecounter] = new TH1F(Form("histPatternBTDensities_%d",angspacecounter),Form("histPatternBTDensities_%d",angspacecounter),200,0,200);
    }

    // Before Starting create the Random Generator:
    cout << "EdbPVRQuality::Execute_RandomCutInAngularBins   Create the Random Generator with unique Seed" << endl;
    cout << "EdbPVRQuality::Execute_RandomCutInAngularBins   TRandom3* RandomGenerator = new TRandom3(0);" << endl;
    TRandom3* RandomGenerator = new TRandom3(0);

    Double_t angularspacebinningwidth=0.1;
    Double_t angularspacebinningstart=0.00;
    Double_t angularspacebinningend=0.00;
    Double_t angularspacebinningScaleFactor=1;
    Double_t tt=0;
    Double_t ni[20];
    Double_t NumberOfFilledTTbins=0;
    TH1F* eHistTTClone = (TH1F*)eHistTT->Clone();
    Int_t BTNumberLevelAngularSpace[20];
    for (int i=0; i<1; i++) {
        BTNumberLevelAngularSpace[i];
    }



    //---------------------------------------------------------------------
    // Loop over the patterns:
    for (int i=0; i<eAli_maxNpatterns; i++) {
//     for (int i=0; i<1; i++) {

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();
        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   For this (" << i << ") pattern, I have " << npat << " Edb segments to check..." << endl;

        // Reset eBTDensity levels:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
            eBTDensityLevelAngularSpace[angspacecounter]=eBTDensityLevel;
            // Maximum Cut Value for const, BT dens, also in tangens theta space
            // Maximum Cut Value for const, BT dens, also in tangens theta space
            // Uniform from 0..1. This cutroutine uses only the parameter: "eCutTTp1"
            eCutTTp0[i][angspacecounter]=1.00;
            eCutTTp1[i][angspacecounter]=1.00;
        }

        // Now do the angular space binning loop:
        for (Int_t angspacecounter=0; angspacecounter<10; angspacecounter++) {
// 	  for (Int_t angspacecounter=0; angspacecounter<1; angspacecounter++) {
            cout << endl << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   Doing now the angspacecounter loop:  angspacecounter = " << angspacecounter  << endl;

            // Calculate right end of angular space bin
            angularspacebinningstart=(Double_t)angspacecounter*angularspacebinningwidth;
            angularspacebinningend=angularspacebinningstart+angularspacebinningwidth;

            cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   Doing TT Bin from " << angularspacebinningstart << " to " << angularspacebinningend << endl;

            // Now the condition loop:
            // Loop over 20 steps a either 5% reduction or 40 steps a 2.5% reduction
            Int_t lmax=20;
// 	    Int_t lmax=40;

            for (Int_t l=0; l<lmax; l++) {
                // Shorter Handings:
                Double_t Cutp0 = eCutTTp0[i][angspacecounter];
                Double_t Cutp1 = eCutTTp1[i][angspacecounter];

                cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   Doing now the condition loop:  l = " << l << ", the cut condition reads:  RandomGenerator->Uniform() > " << Cutp1 <<  endl;

                // Important to clean the histograms:
                eHistYX->Reset(); // important to clean the histogram
                eHistYX->SetMinimum(1);
                eHistYX->SetMaximum(250); /// to be calculated accordingly !!
                eHistChi2W->Reset(); // important to clean the histogram
                histPatternBTDensity->Reset(); // important to clean the histogram
                eHistTT->Reset();  // important to clean the histogram
                eHistTTClone->Reset();  // important to clean the histogram
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
                        }
                        else {
                            result = kFALSE;
                        }
                    }

                    // Main decision for segment to be kept or not (seg is of MC or data type).
                    if ( kFALSE == result ) continue;

                    // Calculate Angle
                    tt=TMath::Sqrt(seg->TY()*seg->TY()+seg->TX()*seg->TX());

                    // Fill Clone histo. This is needed for the polynomial fit.
                    eHistTTClone->Fill(tt);

                    // Check if angle is in the desired angular space bin:
                    Int_t segttspacebin=GetAngularSpaceBin(seg);
                    if (segttspacebin!=angspacecounter)  continue;

                    // Random CutMethod:  for angular space:
                    if (RandomGenerator->Uniform()>eCutTTp1[i][angspacecounter]) continue;


                    // Increase counter for number of taken segments in tan theta bin:
                    ni[angspacecounter] += 1;
                    // Fill other histograms anyway
                    eHistYX->Fill(seg->Y(),seg->X());
                    eHistTYTX->Fill(seg->TY(),seg->TX());
                    eHistTT->Fill(tt);
                    eHistChi2W->Fill(seg->W(),seg->Chi2());
                } // Loop over the segments.

                // Do the quadratic fit for the minimumTTBin only at the first time.
                if (l==0) {
                    eHistTTClone->Fit("pol2","0q","0q",0,0.5);
                    TF1 *myfunc = eHistTTClone->GetFunction("pol2");
                    Double_t par0 = myfunc->GetParameter(0);
                    Double_t par1 = myfunc->GetParameter(1);
                    Double_t par2 = myfunc->GetParameter(2);

                    Int_t minimumTTBin = eHistTTClone->FindBin( -par1/(2*par2));
                    Int_t nEntriesInMinimumBin = eHistTTClone->GetBinContent(minimumTTBin);
                    BTNumberLevelAngularSpace[angspacecounter] = Int_t(ni[angspacecounter]- eCutTTSqueezeFactor* Float_t(ni[angspacecounter]-nEntriesInMinimumBin));

                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   Minimum should be at TanTheta = " << -par1/(2*par2) << "  and it is in bin # " << minimumTTBin << " ; there are so many entries: " << nEntriesInMinimumBin << endl;
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   For bin " << angspacecounter << ", there are at the beginning so many segments: " << ni[angspacecounter] << endl;
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   BTNumberLevelAngularSpace[angspacecounter] = " << BTNumberLevelAngularSpace[angspacecounter] << endl;
                } // if (l==0)

                int nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();
                int NemptyBins=0;
                int NFilledBins=0;
                for (int k=1; k<nbins-1; k++) {
                    if (eHistYX->GetBinContent(k)==0) {
                        ++NemptyBins;
                        continue;
                    }
                    // due to the changed areasize in eHistYX, adapt the fill weight
                    histPatternBTDensity->Fill(Double_t(eHistYX->GetBinContent(k))/ 6.25 );
                    histPatternBTDensities[angspacecounter]->Fill(eHistYX->GetBinContent(k)/ 6.25);
                    ++NFilledBins;
                }
                ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();
                Float_t ePatternBTDensityRMS_modified = histPatternBTDensity->GetRMS();

                // leave also if only one entry in the histogram
                if (ePatternBTDensity_modified[i]<=1) l=lmax;

                if (gEDBDEBUGLEVEL>1) {
                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   Loop l= " << l << ":  for the Cutp1 : " << Cutp1 <<   " we have #NBT: "  << ni[angspacecounter] << "+-" << sqrt(ni[angspacecounter]) <<", target number for this TT space bin = " << BTNumberLevelAngularSpace[angspacecounter] << endl;
                }

                // Now the condition check for angular space cut
                // This time, we do it in terms of Numbers Of Basetracks instead of BasetrackDensity/mm2
                if (ni[angspacecounter] <=BTNumberLevelAngularSpace[angspacecounter]) {
                    l=lmax;
                    // When we passed down _under_ the desired Number of basetrack tracks, we raise
                    // one last time the cuts, to come out at last one slight value over it....
                    if (lmax==20) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.05;
                    if (lmax==40) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.025;

                    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   We reached the loop end due to good BT density level in this angular bin and finish loop. Set l to lmax: " << l << endl;
                    // break; // no need to brake anymore since we set l to lmax....
                }
                else {
                    // Next step, tighten cut:
                    // lmax=20:
                    if (lmax==20) {
                        eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.05;
                    }
                    // l=40:
                    if (lmax==40) {
                        eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] -0.025;
                    }
                    if (l==lmax-1) {
                        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   ATTENTION! No more cuts to tighten up. Restore the last valid cut. Basetrack number wont go below last value!" << endl;
                        if (lmax==20) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.05;
                        if (lmax==40) eCutTTp1[i][angspacecounter] =  eCutTTp1[i][angspacecounter] +0.025;
                    }
                }

            } // of condition loop...

            // Fill target profile histogram:
            Float_t bincontentXY=histPatternBTDensity->GetMean();
            eProfileBTdens_vs_PID_target->Fill(i,bincontentXY);

            angularspacebinningstart+=angularspacebinningwidth;
        } // for (int angspacecounter=0; angspacecounter<20; angspacecounter++)

    } // of Npattern loops..
    //---------------------------------------------------------------------

    eCutMethodIsDone[7]=kTRUE;

    // Check if modified or original PVRec object should be returned:
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut    Check if modified or original PVRec object should be returned: " << endl;
    if (eProfileBTdens_vs_PID_source_meanY>eBTDensityLevel) {
        eNeedModified=kTRUE;
        cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut    " << eNeedModified << endl;
    }

    delete histPatternBTDensity;
    delete eHistTTClone;

    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   Cuts are done and saved to obtain desired BT density. " << endl;
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut   If you want to apply the cuts now, run the  CreateEdbPVRec()  function now.  " <<   endl;
    cout << "EdbPVRQuality::Execute_EqualizeTanThetaSpace_RandomCut...done." << endl;
    return;
}


