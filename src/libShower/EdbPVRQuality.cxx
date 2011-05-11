#include "EdbPVRQuality.h"
using namespace std;

// Base class for checks of the scanned data, especially for shower reconstruction.
// The observed basetrack density can vary from brick to brick with a large
// fluctuation.
// The shower algorithm in its standard implementation is sensitive to the
// level of tracks per unit area. In case there are too many of them, purity shower
// reco goes down and gives not reliable results anymore.
// The EdbPVRQuality class basically equals the number of basetracks per unit area
// to a maximum upper value (if necessary) by adapting the quality cut for a single
// basetrack.

ClassImp(EdbPVRQuality)

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality()
{
    // Default Constructor
    cout << "EdbPVRQuality::EdbPVRQuality()   Default Constructor"<<endl;
    Set0();
    Init();
    Help();
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
    Set0();
    Init();
    Help();

    if (NULL == ali || ali->Npatterns()<1) {
        cout << "WARNING   EdbPVRQuality(EdbPVRec* ali)   ali is no good EdbPVRec object! Check!" << endl;
        return;
    }

    // Set ali as eAli_orig
    eAli_orig=ali;
    eIsSource=kTRUE;

    CheckEdbPVRec();
    Execute_ConstantBTDensity();
    CreateEdbPVRec();
}

//______________________________________________________________________________

EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali,  Float_t BTDensityTargetLevel)
{
    // Default Constructor with a EdbPVRec object and the desired basetrack density target level.
    // Does same as constructor EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali) but now with adjustable
    // background level.
    // If general basetrack density is lower than BTDensityTargetLevel BT/mm2 then no cleaning is done.
    cout << "EdbPVRQuality::EdbPVRQuality(EdbPVRec* ali)   Constructor (does automatically all in one...)"<<endl;
    Set0();
    Init();
    Help();

    if (NULL == ali || ali->Npatterns()<1) {
        cout << "WARNING   EdbPVRQuality(EdbPVRec* ali)   ali is no good EdbPVRec object! Check!" << endl;
        return;
    }

    // Set ali as eAli_orig
    eAli_orig=ali;
    eIsSource=kTRUE;

    // Set BTDensityTargetLevel
    SetBTDensityLevel(BTDensityTargetLevel);
    cout << " GetBTDensityLevel() " << GetBTDensityLevel() << endl;

    CheckEdbPVRec();
    Execute_ConstantBTDensity();
    CreateEdbPVRec();
}

//______________________________________________________________________________

EdbPVRQuality::~EdbPVRQuality()
{
    // Default Destructor
    cout << "EdbPVRQuality::~EdbPVRQuality()"<<endl;
    delete 		eHistChi2W;
    delete 		eHistYX;
    delete 		eProfileBTdens_vs_PID;
}

//______________________________________________________________________________

void EdbPVRQuality::Set0()
{
    // Reset Values
    eAli_orig=NULL;
    eAli_modified=NULL;
    eIsSource=kFALSE;
    for (int i=0; i<2; i++) eCutMethodIsDone[i]=kFALSE;

    // Default BT density level for which the standard cutroutine
    // will be put:
    eBTDensityLevel=20; // #BT/mm2

    // Reset Default Geometry: 0 OperaGeometry, 1: MC Geometry
    eHistGeometry=0;

    eHistChi2W = new TH2F("eHistChi2W","eHistChi2W",40,0,40,90,0,3);
    eHistYX = new TH2F("eHistYX","eHistYX",100,0,1,100,0,1);

    eHistYX->Reset();
    eHistChi2W->Reset();

    for (int i=0; i<57; i++) {
        ePatternBTDensity_orig[i]=0;
        ePatternBTDensity_modified[i]=0;
        eCutp1[i]=0.15;
        eCutp0[i]=1.0; // Maximum Cut Value for const, BT dens
        eagreementChi2WDistCut[i]=3.0;  // Maximum Cut Value for const, BT dens
    }

    eProfileBTdens_vs_PID = new TProfile("eProfileBTdens_vs_PID","eProfileBTdens_vs_PID",57,0,57,0,100);
    cout << "eProfileBTdens_vs_PID->GetBinWidth(1)" << eProfileBTdens_vs_PID->GetBinWidth(1) << endl;

    // Default values for cosmics, taken from a brick data:
    // (I cant remember which one, I hope it doesnt matter).
    eagreementChi2CutMeanChi2=1.0;
    eagreementChi2CutRMSChi2=0.3;
    eagreementChi2CutMeanW=23;
    eagreementChi2CutRMSW=3;
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Init()
{
    /// TEMPORARY, later the Standard Geometry should default be OPERA.
    /// For now we use a very large histogram that can contain both case
    /// x-y ranges. This takes slightly more memory but it shouldnt matter.
    if (eHistGeometry==0) SetHistGeometry_OPERA();

    cout << "EdbPVRQuality::Init()   /// TEMPORARY  SetHistGeometry_OPERAandMC " <<  endl;
    SetHistGeometry_OPERAandMC();
    return;
}


//______________________________________________________________________________
void EdbPVRQuality::SetCutMethod(Int_t CutMethod)
{
    // Set Cut Method of the background reduction:
    // 0: (default): Do cut based on the linear relation: seg.Chi2()<seg.eW*a-b
    // 1: (testing): Do cut based on a chi2-variable that compares with passing tracks
    // (if available), i.e. cosmics.
    eCutMethod=CutMethod;
    cout << "EdbPVRQuality::SetCutMethod  eCutMethod=  " << eCutMethod << endl;
    if (CutMethod>1) {
        eCutMethod=0;
        cout << "WARNING   EdbPVRQuality::SetCutMethod  eCutMethod invalid, Set back to default eCutMethod=  " << eCutMethod << endl;
    }
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
    if (!eIsSource) {
        cout << "EdbPVRQuality::CheckEdbPVRec  eIsSource=  " << eIsSource << ". This means no source set. Return!" << endl;
        return;
    }
    // Check the patterns of the EdbPVRec:
    int Npat = eAli_orig->Npatterns();
    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);

    // Loop over the patterns of the EdbPVRec:
    for (Int_t i=0; i<Npat; i++) {
        eHistYX->Reset(); // important to clean the histogram
        eHistChi2W->Reset(); // important to clean the histogram

        EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npat=pat->N();

        EdbSegP* seg=0;
        // Loop over the segments of the pattern
        for (Int_t j=0; j<npat; j++) {
            seg=(EdbSegP*)pat->At(j);
            // Very important:
            // For the data case, we assume the following:
            // Data (MCEvt==-999) will     be taken for BTdensity calculation
            // Sim (MCEvt>0)      will NOT be taken for BTdensity calculation
            if (seg->MCEvt() > 0) continue;

            // For the check, fill the histograms in any case:
            eHistYX->Fill(seg->Y(),seg->X());
            eHistChi2W->Fill(seg->W(),seg->Chi2());
        }

        histPatternBTDensity->Reset();
        Int_t nbins=eHistYX->GetNbinsX()*eHistYX->GetNbinsY();

        // Search for empty bins, because they can spoil the overall calulation
        // of the mean value.
        Int_t nemptybinsXY=0;
        Int_t bincontentXY=0;
        for (int k=1; k<nbins-1; k++) {
            if (eHistYX->GetBinContent(k)==0) {
                ++nemptybinsXY;
                continue;
            }
            bincontentXY=eHistYX->GetBinContent(k);
            histPatternBTDensity->Fill(bincontentXY);
            eProfileBTdens_vs_PID->Fill(i,bincontentXY);
        }

        // failsafe warning in case that there are many bins with zero content.
        // for now we print a error message:
        CheckFilledXYSize();

        // Save the density in the variable.
        ePatternBTDensity_orig[i]=histPatternBTDensity->GetMean();
        histPatternBTDensity->Reset();
    }

    eHistYX->Reset();
    eHistChi2W->Reset();

    Print();
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
    cout << "SetHistGeometry_OPERA :binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
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
    cout << "SetHistGeometry_MC :binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::SetHistGeometry_OPERAandMC()
{
    // Set the geometry of the basetrack density evaluation covering MC and DATA case,
    // size conventions: x=-125000..0..+125000;y=-125000..0..125000).
    // BinArea is 1mmx1mm
    eHistYX->Reset();
    eHistYX->SetBins(250,-125000,125000,250,-125000,125000);
    cout << "SetHistGeometry_OPERAandMC :binwidth (micron)= " << eHistYX->GetBinWidth(1) << endl;
    return;
}
//______________________________________________________________________________
void EdbPVRQuality::Print()
{
    // Print either cut type values if cut method 1 or if cut method 2 had been chosen.
    cout << "EdbPVRQuality::Print()   " << endl;
    cout << "EdbPVRQuality::Print()   eCutMethodIsDone[0] " << eCutMethodIsDone[0] << endl;
    cout << "EdbPVRQuality::Print()   eCutMethodIsDone[1] " << eCutMethodIsDone[1] << endl;
    if (eCutMethodIsDone[0]) PrintCutType0();
    if (eCutMethodIsDone[1]) PrintCutType1();
    return;
}


//______________________________________________________________________________
void EdbPVRQuality::PrintCutType0()
{
    //     Prints quality cut values for each plate of the original EdbPVRec object that were
    //     applied to achieve the basetrack density level, after application the specific cut
    //     on the segments of the specific plate (=pattern).

    if (!eIsSource) return;

    cout << "----------void EdbPVRQuality::PrintCutType0()----------" << endl;
    cout << "Pattern || Z() || Nseg || eCutp0[i] || eCutp1[i] || BTDensity_orig ||...|| Nseg_modified || BTDensity_modified ||"<< endl;

    if (NULL==eAli_modified) {
        eAli_modified=eAli_orig;
        cout << "WARNING eAli_modified==NULL  ==>>  Take eAli_orig instead. To calculate eAli_modified please run (not supported yet...)" << endl;
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
        printf("%.1f  %d  %.3f  %.3f  %.1f",pat_orig->Z(),npatO, eCutp0[i], eCutp1[i] , ePatternBTDensity_orig[i]);
        cout << "	...	";
        printf("%.1f  %d  %.3f  %.3f  %.1f",pat_modified->Z(),npatM,  eCutp0[i] ,eCutp1[i],  ePatternBTDensity_modified[i]);
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
    cout << "----------void EdbPVRQuality::PrintCutType1()----------" << endl;

    cout << "Pattern || Z() || Nseg || BTDensity_orig || Chi2CutMeanChi2 || Chi2CutRMSChi2 || Chi2CutMeanW || Chi2CutRMSW || Chi2Cut[i] || BTDensity_modified ||"<< endl;

    if (NULL==eAli_modified) {
        eAli_modified=eAli_orig;
        cout << "WARNING eAli_modified==NULL  ==>>  Take eAli_orig instead. To calculate eAli_modified please run (not supported yet...)" << endl;
    }

    int Npat_orig = eAli_orig->Npatterns();
    for (int i=0; i<Npat_orig; i++) {
        EdbPattern* pat_orig = (EdbPattern*)eAli_orig->GetPattern(i);
        Int_t npatO=pat_orig->N();
        EdbPattern* pat_modified = (EdbPattern*)eAli_modified->GetPattern(i);
        Int_t npatM=pat_modified->N();
        cout << i;
        cout << "	";
        printf("%.1f  %d  %.2f  %.2f  %.2f  %.2f  %.2f  %.1f",pat_orig->Z(),npatO, eagreementChi2CutMeanChi2 , eagreementChi2CutRMSChi2,  eagreementChi2CutMeanW , eagreementChi2CutRMSW, eagreementChi2WDistCut[i], ePatternBTDensity_orig[i]);
        cout << "	...	";
        printf("%.1f  %d  %.2f  %.2f  %.2f  %.2f  %.2f  %.1f",pat_modified->Z(),npatM, eagreementChi2CutMeanChi2 , eagreementChi2CutRMSChi2,  eagreementChi2CutMeanW , eagreementChi2CutRMSW, eagreementChi2WDistCut[i], ePatternBTDensity_modified[i]);
        cout << endl;
    }
    return;
}

//______________________________________________________________________________

void EdbPVRQuality::Execute_ConstantBTDensity()
{
    // Execute the modified cut routines to achieve the basetrack density level, after application the specific cut on the segments of the specific plate (pattern).
    // The Constant BT Density is defined by the number of BT/mm2 in the histogram.

    if (!eIsSource) return;
    if (NULL==eAli_orig) return;

    int Npat = eAli_orig->Npatterns();
    cout << "----------void EdbPVRQuality::Execute_ConstantBTDensity()   Npat=" << Npat << endl;

    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",200,0,200);

    // Loop over the patterns:
    for (int i=0; i<Npat; i++) {
        if (i>56) {
            cout << "ERROR     EdbPVRQuality::Execute_ConstantBTDensity() Your EdbPVRec object has more than 57 plates! " << endl;
            cout << "ERROR     EdbPVRQuality::Execute_ConstantBTDensity() Check it! " << endl;
            break;
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
                seg=(EdbSegP*)pat->At(j);

                // Very important:
                // For the data case, we assume the following:
                // Data (MCEvt==-999) will be taken for BTdens calculation
                // Sim (MCEvt>0) will not be taken for BTdens calculation
                if (seg->MCEvt() > 0) continue;

                // Constant BT density cut:
                if (seg->Chi2() >= seg->W()* eCutp1[i] - eCutp0[i]) continue;

                eHistYX->Fill(seg->Y(),seg->X());
                eHistChi2W->Fill(seg->W(),seg->Chi2());
            }


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

    } // of Npattern loops..

    eCutMethodIsDone[0]=kTRUE;

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
    eProfileBTdens_vs_PID->Draw("profileZ");
    c1->cd();
    histPatternBTDensity->Reset();
    eHistYX->Reset();
    eHistChi2W->Reset();

    delete histPatternBTDensity;
    return;
}

//______________________________________________________________________________


void EdbPVRQuality::Execute_ConstantQuality()
{
    // The cut method that compares the passing muon tracks with all scanned segments.
    // This may help to improve, since it takes into account the actual segment quality,
    // which varies from scan to scan anyway.
    // Works for tracks passing the volume to extract their mean chi2/W
    // If eAli->Tracks is there we take them from there.
    // If not, we try if there is a file linked_tracks.root, and we take tracks from there.
    // If that doesnt work either, nothing is done.

    if (!eIsSource) return;
    cout << "----------void EdbPVRQuality::Execute_ConstantQuality()----------" << endl;
    cout << "----------    Works for tracks passing the volume to extract their mean chi2/W " << endl;
    cout << "----------    If eAli->Tracks is there we take them from there." << endl;
    cout << "----------    If not, we try if there is a file linked_tracks.root " << endl;
    cout << "----------    we take tracks from there. " << endl;
    cout << "----------    If that doesnt work either, nothing is done." << endl;

    cout << "EdbPVRQuality::Execute_ConstantQuality()   eAli_orig.eTracks :" << eAli_orig->eTracks << endl;

    Float_t meanChi2=0.5;
    Float_t rmsChi2=0.2;
    Float_t meanW=22;
    Float_t rmsW=4;
    Float_t agreementChi2=100;

    // No  eAli.Tracks ? Look for tracks in linked_track.root:
    if (NULL == eAli_orig->eTracks) {
        TFile* trackfile = new TFile("linked_tracks.root");
        trackfile->ls();
        TTree* tracks = (TTree*)trackfile->Get("tracks");
        // 		TH1F* h1;
        tracks->Draw("nseg>>h(60,0,60)","","");
        TH1F *h1 = (TH1F*)gPad->GetPrimitive("h");

        // Short implementation of getting the last bin filled:
        int lastfilledbin=0;
        for (int k=1; k<h1->GetNbinsX()-1; k++) {
            if (h1->GetBinContent(k)>0) lastfilledbin=k;
        }

        TString cutstring = TString(Form("nseg>=%d",int(h1->GetBinCenter(lastfilledbin-3)) ));
        tracks->Draw("s.eChi2>>hChi2(100,0,2)",cutstring);
        TH1F *hChi2 = (TH1F*)gPad->GetPrimitive("hChi2");
        cout << hChi2->GetMean() << " " << hChi2->GetRMS() << endl;

        TCanvas* c1 = new TCanvas();
        c1->cd();
        tracks->Draw("s.eW>>hW(50,0,50)",cutstring);
        TH1F *hW = (TH1F*)gPad->GetPrimitive("hW");
        cout << hW->GetMean() << " " << hW->GetRMS() << endl;

        meanChi2=hChi2->GetMean();
        rmsChi2=hChi2->GetRMS();
        meanW=hW->GetMean();
        rmsW=hW->GetRMS();

        // since the values for the passing tracks are
        // calculated for the whole volume, we assume that the cutvalues
        // are valid for all plates.
        eagreementChi2CutMeanChi2=meanChi2;
        eagreementChi2CutRMSChi2=rmsChi2;
        eagreementChi2CutMeanW=meanW;
        eagreementChi2CutRMSW=rmsW;
    }

    /// ______  now same code as in the function Execute_ConstantBTDensity  ___________________

    int Npat = eAli_orig->Npatterns();
    TH1F* histPatternBTDensity = new TH1F("histPatternBTDensity","histPatternBTDensity",100,0,200);
    TH1F* histagreementChi2 = new TH1F("histagreementChi2","histagreementChi2",100,0,5);

    for (int i=0; i<Npat; i++) {
        if (i>56) {
            cout << "ERROR     EdbPVRQuality::Execute_ConstantBTDensity() Your EdbPVRec object has more than 57 plates! " << endl;
            cout << "ERROR     EdbPVRQuality::Execute_ConstantBTDensity() Check it! " << endl;
            break;
        }

        cout << "Execute_ConstantQuality   Doing Pattern " << i << endl;

        // Now the condition loop:
        // Loop over 30 steps agreementChi2 step 0.05
        for (int l=0; l<30; l++) {

            eHistYX->Reset(); // important to clean the histogram
            eHistChi2W->Reset(); // important to clean the histogram
            histPatternBTDensity->Reset(); // important to clean the histogram

            EdbPattern* pat = (EdbPattern*)eAli_orig->GetPattern(i);
            Int_t npat=pat->N();
            EdbSegP* seg=0;
            for (int j=0; j<npat; j++) {
                seg=(EdbSegP*)pat->At(j);

                // Very important:
                // For the data case, we assume the following:
                // Data (MCEvt==-999) will be taken for BTdens calculation
                // Sim (MCEvt>0) will not be taken for BTdens calculation
                if (seg->MCEvt() > 0) continue;

                // Change here to the quality with values obtained from the tracks.
                // Constant BT quality cut:
                agreementChi2=TMath::Sqrt( ( (seg->Chi2()-meanChi2)/rmsChi2)*((seg->Chi2()-meanChi2)/rmsChi2)  +   ((seg->W()-meanW)/rmsW)*((seg->W()-meanW)/rmsW) );

                histagreementChi2->Fill(agreementChi2);

                if (agreementChi2>eagreementChi2WDistCut[i]) continue;

                eHistYX->Fill(seg->Y(),seg->X());
                eHistChi2W->Fill(seg->W(),seg->Chi2());
            }

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
            // for now we print a error message:
            CheckFilledXYSize();

            ePatternBTDensity_modified[i]=histPatternBTDensity->GetMean();
            cout <<"Execute_ConstantBTDensity      Loop l= " << l << ":  for the eagreementChi2WDistCut : " << eagreementChi2WDistCut[i] <<   "  we have a dens: "  << ePatternBTDensity_modified[i] << endl;

            // Now the condition check:
            if (ePatternBTDensity_modified[i]<=eBTDensityLevel) {
                if (gEDBDEBUGLEVEL>2)  cout << "Execute_ConstantBTDensity      We reached the loop end due to good BT density level ... and break loop." << endl;
                // But dont forget to set values:
                eCutDistChi2[i]=meanChi2;
                eCutDistW[i]=meanW;
                eagreementChi2CutMeanChi2=meanChi2;
                eagreementChi2CutRMSChi2=rmsChi2;
                eagreementChi2CutMeanW=meanW;
                eagreementChi2CutRMSW=rmsW;
                break;
            }
            else {
                // Next step, tighten cut:
                eagreementChi2WDistCut[i]+=  -0.05;
            }

        } // of condition loop...

    } // of Npattern loops..

    eCutMethodIsDone[1]=kTRUE;

    histPatternBTDensity->Reset();
    eHistYX->Reset();
    eHistChi2W->Reset();
    delete histPatternBTDensity;
    return;
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
    Float_t agreementChi2=TMath::Sqrt( ( (seg->Chi2()-eagreementChi2CutMeanChi2)/eagreementChi2CutRMSChi2)*((seg->Chi2()-eagreementChi2CutMeanChi2)/eagreementChi2CutRMSChi2)  +   ((seg->W()-eagreementChi2CutMeanW)/eagreementChi2CutRMSW)*((seg->W()-eagreementChi2CutMeanW)/eagreementChi2CutRMSW) );
    if (agreementChi2>eagreementChi2WDistCut[PatternAtNr]) return kFALSE;

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
        cout << "-----     " << endl;
        cout << "-----     ----------------------------------------------" << endl;
    }

    // Checks
    if (NULL==eAli_orig || eIsSource==kFALSE) {
        cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    NULL==eAli_orig   || eIsSource==kFALSE   return."<<endl;
        return;
    }
    // Checks
    if (eCutMethodIsDone[0]==kFALSE && eCutMethodIsDone[1]==kFALSE) {
        cout << "WARNING!   EdbPVRQuality::CreateEdbPVRec    eCutMethodIsDone[0]==kFALSE && eCutMethodIsDone[1]==kFALSE   return."<<endl;
        return;
    }

    // Make a new PVRec object anyway
    eAli_modified = new EdbPVRec();

    // These two lines dont compile yet ... (???) ...
    // 	EdbScanCond* scancond = eAli_orig->GetScanCond();
    // 	eAli_modified->SetScanCond(*scancond);

    Float_t agreementChi2;

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
                agreementChi2=TMath::Sqrt( ( (seg->Chi2()-eagreementChi2CutMeanChi2)/eagreementChi2CutRMSChi2)*((seg->Chi2()-eagreementChi2CutMeanChi2)/eagreementChi2CutRMSChi2)  +   ((seg->W()-eagreementChi2CutMeanW)/eagreementChi2CutRMSW)*((seg->W()-eagreementChi2CutMeanW)/eagreementChi2CutRMSW) );
                if (agreementChi2>eagreementChi2WDistCut[i]) continue;
            }
            else {
                // do nothing;
            }

            // Add segment:
            pt->AddSegment(*seg);
        }
        eAli_modified->AddPattern(pt);
    }
    //---------------------
    eAli_orig->Print();
    eAli_modified->Print();
    cout << "-----     void EdbPVRQuality::CreateEdbPVRec()...done." << endl;
    return;
}




//___________________________________________________________________________________

void EdbPVRQuality::CheckFilledXYSize()
{
    // Check the bins filled in the actual pattern.
    // The histogram of the XY distribution is analysed.
    // A warning is given if more than 10 percent of
    // the bins are empty.
    // In this case one should look closer at the specific
    // plate distribution.

    if (gEDBDEBUGLEVEL>2)   cout << "-----     void EdbPVRQuality::CheckFilledXYSize() return maximum/minimum entries of  eHistYX    -----" << endl;
    Int_t nbx,nby=0;
    nbx=eHistYX->GetNbinsX();
    nby=eHistYX->GetNbinsY();

    Int_t n1x= FindFirstBinAbove(eHistYX,0,1);   // Int_t FindFirstBinAbove(Double_t threshold = 0, Int_t axis = 1) const
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
    Int_t NonEmptyBins=0;
    Int_t nBins=0;
    for (Int_t i=n1x; i<n2x; ++i) {
        for (Int_t j=n1y; j<n2y; ++j) {
            ++nBins;
            if (eHistYX->GetBinContent(i,j)==0) continue;
            if (eHistYX->GetBinContent(i,j)==0) continue;
            ++NonEmptyBins;
        }
    }
    Float_t FractionOfEmptyBins=1-(Float_t(NonEmptyBins)/Float_t(nBins));
    if (FractionOfEmptyBins>0.1) cout << "WARNING: void EdbPVRQuality::CheckFilledXYSize() FractionOfEmptyBins = " << FractionOfEmptyBins << endl;

    if (gEDBDEBUGLEVEL>2) {
        cout << "----      NonEmptyBins bins in der covered area:  = " << NonEmptyBins << endl;
        cout << "----      nBins totale bins in der covered area:  = " << nBins << endl;
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
    return;
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
    cout << "-----     void EdbPVRQuality::Help()     -----" << endl;
    cout << "----------------------------------------------" << endl;
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
    cout << "-----     void EdbPVRQuality::Remove_DoubleBT()" << endl;
    cout << "-----     void EdbPVRQuality::Take source EdbPVRec from " << aliSource << endl;

    EdbPVRec* eAli_source=aliSource;

    if (NULL==aliSource) {
        cout << "-----     void EdbPVRQuality::Source EdbPVRec is NULL. Change to object eAli_orig: " << eAli_orig << endl;
        eAli_source=eAli_orig;
    }

    if (NULL==eAli_orig) {
        cout << "-----     void EdbPVRQuality::Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
        return NULL;
    }

    // Make a new PVRec object anyway
    EdbPVRec* eAli_target = new EdbPVRec();
    eAli_target->Print();

    Bool_t seg_seg_close=kFALSE;
    EdbSegP* seg=0;
    EdbSegP* seg1=0;
    Int_t NdoubleFoundSeg=0;

    for (int i = 0; i <eAli_source->Npatterns(); i++ ) {
        if (gEDBDEBUGLEVEL>2) cout << "Looping over eAli_source->Pat()=" << i << endl;

        EdbPattern* pat = eAli_source->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());

        for (int j = 0; j <pat->N()-1; j++ ) {
            seg = pat->GetSegment(j);
            seg_seg_close=kFALSE;
            for (int k = j+1; k <pat->N(); k++ ) {
                if (seg_seg_close) continue;

                if (gEDBDEBUGLEVEL>3) cout << "Looping over eTracks for segment pair nr=" << j << "," << k << endl;
                seg1 = pat->GetSegment(k);

                // Here decide f.e. which segments to check...
                if (TMath::Abs(seg->X()-seg1->X())>2.1) continue;
                if (TMath::Abs(seg->Y()-seg1->Y())>2.1) continue;
                if (TMath::Abs(seg->TX()-seg1->TX())>0.01) continue;
                if (TMath::Abs(seg->TY()-seg1->TY())>0.01) continue;
                if (gEDBDEBUGLEVEL>3) cout << "EdbPVRQuality::Remove_DoubleBT()   Found compatible segment!! " << endl;
                ++NdoubleFoundSeg;
                seg_seg_close=kTRUE;
                //                 if (seg_seg_close) break;
            }
            if (seg_seg_close) continue;

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "// Add segment:" << endl;
            pt->AddSegment(*seg);
        }
        if (gEDBDEBUGLEVEL>2) cout << "// Add AddPattern:" << endl;
        eAli_target->AddPattern(pt);
    }

    if (gEDBDEBUGLEVEL>1) eAli_source->Print();
    if (gEDBDEBUGLEVEL>1) eAli_target->Print();

    cout << "-----     void EdbPVRQuality::Remove_DoubleBT()...Statistics: We found " << NdoubleFoundSeg  << " double segments too close to each other to be different BTracks." << endl;
    cout << "-----     void EdbPVRQuality::Remove_DoubleBT()...done." << endl;
    return eAli_target;
}
//___________________________________________________________________________________



EdbPVRec* EdbPVRQuality::Remove_Passing(EdbPVRec* aliSource)
{
    // Removes Passing Tracks from the EdbPVRec source object.
    // Still TODO: Take (as in Execute_ConstantQuality) tracks from linked_tracks
    // file.
    // Unfortunately, there does Not Exist an easy function like
    // ->RemoveSegment from EdbPVRec object. That makes implementation complicated.

    // Quick and Dirty implementation !
    EdbPVRec* eAli_source=aliSource;

    if (NULL==aliSource) {
        cout << "-----     void EdbPVRQuality::Source EdbPVRec is NULL. Change to object eAli_orig: " << eAli_orig << endl;
        eAli_source=eAli_orig;
    }

    if (NULL==eAli_orig) {
        cout << "-----     void EdbPVRQuality::Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
        return NULL;
    }

    TObjArray* Tracks=eAli_source->eTracks;
    Int_t TracksN=eAli_source->eTracks->GetEntries();
    EdbTrackP* track;
    EdbSegP* trackseg;


    // Make a new PVRec object anyway
    EdbPVRec* eAli_target = new EdbPVRec();
    eAli_target->Print();

    Bool_t seg_in_eTracks=kFALSE;

    for (int i = 0; i <eAli_target->Npatterns(); i++ ) {
        if (gEDBDEBUGLEVEL>2) cout << "Looping over eAli_target->Pat()=" << i << endl;

        EdbPattern* pat = eAli_target->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());

        for (int j = 0; j <pat->N(); j++ ) {
            EdbSegP* seg = pat->GetSegment(j);
            //seg->PrintNice();

            if (gEDBDEBUGLEVEL>3) cout << "Looping over eTracks for segment nr= " << j << endl;
            for (int ll = 0; ll<TracksN; ll++ ) {
                track=(EdbTrackP*)Tracks->At(ll);
                //track->PrintNice();
                Int_t TrackSegN=track->N();

                // Here decide f.e. which tracks to check...
                // On cosmics it would be nice that f.e. Npl()>=Npatterns-4 ...
                // if ....()....
                // Since addresses of objects can vary, its better to compare them
                // by absolute positions.

                for (int kk = 0; kk<TrackSegN; kk++ ) {
                    trackseg=track->GetSegment(kk);
                    if (TMath::Abs(seg->Z()-trackseg->Z())>10.1) continue;
                    if (TMath::Abs(seg->X()-trackseg->X())>5.1) continue;
                    if (TMath::Abs(seg->Y()-trackseg->Y())>5.1) continue;
                    if (TMath::Abs(seg->TX()-trackseg->TX())>0.05) continue;
                    if (TMath::Abs(seg->TY()-trackseg->TY())>0.05) continue;
                    //cout << "Found compatible segment!! " << endl;
                    seg_in_eTracks=kTRUE;
                }
                if (seg_in_eTracks) break;
            }
            if (seg_in_eTracks) continue;
            seg_in_eTracks=kFALSE;

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "// Add segment:" << endl;
            pt->AddSegment(*seg);
        }
        if (gEDBDEBUGLEVEL>2) cout << "// Add Pattern:" << endl;
        eAli_target->AddPattern(pt);
    }

    if (gEDBDEBUGLEVEL>2) eAli_source->Print();
    if (gEDBDEBUGLEVEL>2) eAli_target->Print();

    cout << "-----     void EdbPVRQuality::Remove_Passing()...done." << endl;
    return eAli_target;
}

//___________________________________________________________________________________

void EdbPVRQuality::Remove_TrackArray(TObjArray* trackArray)
{
    // Remove a whole track array (Array of EdbTrackP type).

    cout << "-----     void EdbPVRQuality::Remove_TrackArray(TObjArray* trackArray)...." << endl;

    // Quick and Dirty implementation!
    EdbPVRec* eAli_source=NULL;
    EdbPVRec* aliSource=NULL;

    if (NULL==aliSource) {
        cout << "-----     void EdbPVRQuality::Source EdbPVRec is NULL. Change to object eAli_orig: " << eAli_orig << endl;
        eAli_source=eAli_orig;
    }

    if (NULL==eAli_orig) {
        cout << "-----     void EdbPVRQuality::Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
        return;
    }

    TObjArray* Tracks=trackArray;
    Int_t TracksN=trackArray->GetEntries();
    EdbTrackP* track;
    EdbSegP* trackseg;


    // Make a new PVRec object anyway
    EdbPVRec* eAli_target = new EdbPVRec();
    eAli_target->Print();

    Bool_t seg_in_eTracks=kFALSE;

    for (int i = 0; i <eAli_target->Npatterns(); i++ ) {
        if (gEDBDEBUGLEVEL>2) cout << "Looping over eAli_target->Pat()=" << i << endl;

        EdbPattern* pat = eAli_target->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());

        for (int j = 0; j <pat->N(); j++ ) {
            EdbSegP* seg = pat->GetSegment(j);
            seg->PrintNice();

            if (gEDBDEBUGLEVEL>3) cout << "Looping over eTracks for segment nr=" << j << endl;
            for (int ll = 0; ll<TracksN; ll++ ) {
                track=(EdbTrackP*)Tracks->At(ll);
                //track->PrintNice();
                Int_t TrackSegN=track->N();

                // Here decide f.e. which tracks to check...
                // Since addresses of objects can vary, its better to compare them
                // by absolute positions.

                for (int kk = 0; kk<TrackSegN; kk++ ) {
                    trackseg=track->GetSegment(kk);
                    if (TMath::Abs(seg->Z()-trackseg->Z())>10.1) continue;
                    if (TMath::Abs(seg->X()-trackseg->X())>5.1) continue;
                    if (TMath::Abs(seg->Y()-trackseg->Y())>5.1) continue;
                    if (TMath::Abs(seg->TX()-trackseg->TX())>0.05) continue;
                    if (TMath::Abs(seg->TY()-trackseg->TY())>0.05) continue;
                    //cout << "Found compatible segment!! " << endl;
                    seg_in_eTracks=kTRUE;
                }
                if (seg_in_eTracks) break;
            }
            if (seg_in_eTracks) continue;
            seg_in_eTracks=kFALSE;

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "// Add segment:" << endl;
            pt->AddSegment(*seg);
        }
        if (gEDBDEBUGLEVEL>2) cout << "// Add Pattern:" << endl;
        eAli_modified->AddPattern(pt);
    }

    if (gEDBDEBUGLEVEL>2) eAli_source->Print();
    if (gEDBDEBUGLEVEL>2) eAli_target->Print();

    cout << "-----     void EdbPVRQuality::Remove_TrackArray(TObjArray* trackArray)...done." << endl;
    return;
}

//___________________________________________________________________________________


//___________________________________________________________________________________

void EdbPVRQuality::Remove_SegmentArray(TObjArray* segArray) {

    // Remove a whole segment array (Array of EdbSegP type).

    cout << "-----     void EdbPVRQuality::Remove_SegmentArray(TObjArray* segArray)...." << endl;

    // Quick and Dirty implementation !
    EdbPVRec* eAli_source=NULL;
    EdbPVRec* aliSource=NULL;

    if (NULL==aliSource) {
        cout << "-----     void EdbPVRQuality::Source EdbPVRec is NULL. Change to object eAli_orig: " << eAli_orig << endl;
        eAli_source=eAli_orig;
    }

    if (NULL==eAli_orig) {
        cout << "-----     void EdbPVRQuality::Also eAli_orig EdbPVRec is NULL. Do nothing and return NULL pointer!" << endl;
        return;
    }

    TObjArray* Tracks=segArray;
    Int_t TracksN=segArray->GetEntries();
    EdbSegP* trackseg;

    // Make a new PVRec object anyway
    EdbPVRec* eAli_target = new EdbPVRec();
    eAli_target->Print();

    Bool_t seg_in_eTracks=kFALSE;

    for (int i = 0; i <eAli_target->Npatterns(); i++ ) {
        if (gEDBDEBUGLEVEL>2) cout << "Looping over eAli_target->Pat()=" << i << endl;

        EdbPattern* pat = eAli_target->GetPattern(i);
        EdbPattern* pt= new EdbPattern();
        // SetPattern Values to the parent patterns:
        pt->SetID(pat->ID());
        pt->SetPID(pat->PID());
        pt->SetZ(pat->Z());

        for (int j = 0; j <pat->N(); j++ ) {
            EdbSegP* seg = pat->GetSegment(j);
            seg->PrintNice();

            if (gEDBDEBUGLEVEL>3) cout << "Looping over eTracks for segment nr=" << j << endl;
            for (int ll = 0; ll<TracksN; ll++ ) {
                trackseg=(EdbSegP*)Tracks->At(ll);
                // Here decide f.e. which EdbSegP to check...
                // Since addresses of objects can vary, its better to compare them
                // by absolute positions.
                if (TMath::Abs(seg->Z()-trackseg->Z())>10.1) continue;
                if (TMath::Abs(seg->X()-trackseg->X())>5.1) continue;
                if (TMath::Abs(seg->Y()-trackseg->Y())>5.1) continue;
                if (TMath::Abs(seg->TX()-trackseg->TX())>0.05) continue;
                if (TMath::Abs(seg->TY()-trackseg->TY())>0.05) continue;
                //cout << "Found compatible segment!! " << endl;
                seg_in_eTracks=kTRUE;
            }
            if (seg_in_eTracks) continue;
            seg_in_eTracks=kFALSE;

            // Add segment:
            if (gEDBDEBUGLEVEL>3) cout << "// Add segment:" << endl;
            pt->AddSegment(*seg);
        }
        if (gEDBDEBUGLEVEL>2) cout << "// Add AddPattern:" << endl;
        eAli_modified->AddPattern(pt);
    }

    if (gEDBDEBUGLEVEL>2) eAli_source->Print();
    if (gEDBDEBUGLEVEL>2) eAli_target->Print();

    cout << "-----     void EdbPVRQuality::Remove_SegmentArray(TObjArray* segArray)...done." << endl;
    return;
}

//___________________________________________________________________________________


void EdbPVRQuality::Remove_Track(EdbTrackP* track) {
    TObjArray* trackArray= new TObjArray();
    trackArray->Add(track);
    Remove_TrackArray(trackArray);
    delete trackArray;
}

//___________________________________________________________________________________

void EdbPVRQuality::Remove_Segment(EdbSegP* seg) {
    TObjArray* segArray= new TObjArray();
    segArray->Add(seg);
    Remove_SegmentArray(segArray);
    delete segArray;
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