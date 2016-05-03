// // // // // // // // // // // // // // // // // // // // // // // // // //
// Purpose of this script:
// to show the behavior in the cp.root DATA that some BTs
// are infact duplicated due to scanning effect
// (zone overlapping and different sysal correction at the edges of the
//  zones)
// // // // // // // // // // // // // // // // // // // // // // // // // //

// CUTTYPE_0) RCUT 0 eN1==1&&eN2==1
// (no cuttype 1)
// CUTTYPE_2) RCUT 0 eN1*eN2>1

// default.par cut files are taken from the original default.par files as
// they were in the original brick directory...

// SELECTTYPE_999   // case: "NoCutsOnPairs"
// SELECTTYPE_0   // case: "_WithinLinearCorrelationLine"
// SELECTTYPE_1   // case: "_WithinOriginRectangle"
// SELECTTYPE_2   // case: "_WithinOriginRectangle2"
// SELECTTYPE_3   // case: "_Selecttype_1_or_2_or_3"


// // // // // // // // // // // // // // // // // // // // // // // // // //
Int_t CUTTYPE = 0;
Int_t SELECTTYPE = 3;
// // // // // // // // // // // // // // // // // // // // // // // // // //
    

void ReadRunFakeDoubletBGReductionPlots(){

    // Quick runthrough to check data integrity and so on...
    Bool_t QUICK = 0;
    // Read PVR object either from a root file (0) or from lnk.def data proc (default)
    Bool_t READTYPE = 1;

    TObjArray* ali_array = new TObjArray();
    // maximal number of root files:
    // (to be adapted in case of special needs)
    Int_t N_array=12; 
    if (CUTTYPE==2) N_array=3;
    
    
    Int_t StartArray=0;

    EdbPVRec* ali;

    if (READTYPE==0) {
        for (Int_t n_array = StartArray; n_array < N_array; ++n_array) {
	    cout << "n_array = " << n_array << endl;
            // This time, read from the pre-filtered file:
            TFile* f = new TFile(Form("ScanVolume_Ali_%d_CUTTYPE_%d_SELECTTYPE_%d.root",n_array,CUTTYPE,999));
	    ali = new EdbPVRec();
            ali = (EdbPVRec*)f->Get("EdbPVRec");
            ali->Print();
            ali_array->Add(ali);
        }
    } else {
        ali = new EdbPVRec();
        EdbDataProc* proc = new EdbDataProc("lnk.def");
        proc->InitVolume(ali,0);
        cout << "read_pvr.C   ::   I have read a EdbPVRec* object from basetrack cp data. " << endl;
        cout << "read_pvr.C   ::   ali = " << ali << " and it has ali->Npatterns()= " << ali->Npatterns() << endl;
        ali_array->Add(ali);
    }
    
//cout << "return HERE" << endl;
//return;

    // All Segment Pairs
    TH2D* h_dR_dT = new TH2D("h_dR_dT","",200,0,0.2,200,0,40);
    TH2D* h2_dR_dT = new TH2D("h2_dR_dT","",200,0,0.2,200,0,40);
    // after the cut into the line area (to see if these BTs are identical,
    // so should also be their chi2 and W values)s
    TH2D* h_dchi2_dW = new TH2D("h_dchi2_dW","",13,-6.5,6.5,100,-1,1);
    TH2D* h2_dchi2_dW = new TH2D("h2_dchi2_dW","",13,-6.5,6.5,100,-1,1);

    TH2D* h_chi2_W = new TH2D("h_chi2_W","",26,5.5,31.5,100,0,2);
    TH2D* h2_chi2_W = new TH2D("h2_chi2_W","",26,5.5,31.5,100,0,2);

    TH2D* h_TX_TY = new TH2D("h_TX_TY","",100,-0.5,0.5,100,-0.5,0.5);
    TH2D* h2_TX_TY = new TH2D("h2_TX_TY","",100,-0.5,0.5,100,-0.5,0.5);

    TH2D* h_DeltaTanTheta_Angle = new TH2D("h_DeltaTanTheta_Angle","",200,0,0.2,200,0,0.2);

    TH2D* h_X_Y = new TH2D("h_X_Y","",1250,0,125000,1000,0,100000);
    TH2D* h2_X_Y = new TH2D("h2_X_Y","",1250,0,125000,1000,0,100000);
    
    TH2D* h_X_Y = new TH2D("h_X_Y","",2*1250,0,125000,2*1000,0,100000);
    TH2D* h2_X_Y = new TH2D("h2_X_Y","",2*1250,0,125000,2*1000,0,100000);
    
    TH1D* h2_X_Y_ProjX = new TH1D("h2_X_Y_ProjX","",2*1250,0,125000);
    
    
    TCanvas* c_dR_dT = new TCanvas();
    TCanvas* c_dchi2_dW = new TCanvas();
    TCanvas* c2_dR_dT = new TCanvas();
    TCanvas* c2_dchi2_dW = new TCanvas();
    TCanvas* c_chi2_W = new TCanvas();
    TCanvas* c2_chi2_W = new TCanvas();
    TCanvas* c_TX_TY  = new TCanvas("c_TX_TY","c_TX_TY",2);
    TCanvas* c2_TX_TY = new TCanvas("c2_TX_TY","c2_TX_TY",2);
    TCanvas* c_DeltaTanTheta_Angle  = new TCanvas("c_DeltaTanTheta_Angle","c_DeltaTanTheta_Angle");

    Float_t deltaR=0;
    Float_t deltaTheta=0;
    Float_t deltachi2=0;
    Float_t deltaW=0;
    Float_t deltaTanTheta=0;
    Float_t angleTheta=0;
    
    // Bool Variables for Region
    Bool_t IsRegion0;
    Bool_t IsRegion1;
    Bool_t IsRegion2;

    // Create Variables For ExtractSubpattern boundaries
    Float_t mini[5];
    Float_t maxi[5];
    mini[2]=-1;
    mini[3]=-1;
    mini[4]=0.0;
    maxi[2]=1;
    maxi[3]=1;
    maxi[4]=32.0;
    Float_t    halfpatternsize=40;
    
    Double_t deltaThetaTEST;
    Double_t dR_allowed;
    Double_t dR_allowed_m_deltaR;

    int n_array = 0;

    TCanvas* c_X_Y  = new TCanvas(Form("c_X_Y_%d",n_array),Form("c_X_Y_%d",n_array));
    TCanvas* c2_X_Y  = new TCanvas(Form("c2_X_Y_%d",n_array),Form("c2_X_Y_%d",n_array));


    for (n_array = 0; n_array < N_array; ++n_array) {
//     for (n_array = 0; n_array < 1; ++n_array) {    
        
        cout << "Doing Array " << n_array << "   ----------------------------------------" << endl;
        EdbPVRec* ali = (EdbPVRec*)ali_array->At(n_array);
        aliSource = ali;
	
	//////////////////
	aliNEW = (EdbPVRec*)ali->Clone();
	aliNEW->Print();
	Int_t npataliNEW = aliNEW->Npatterns();
	for (int ctr_i = 0; ctr_i< npataliNEW; ++ctr_i ) aliNEW->GetPattern(ctr_i)->Reset();
	//////////////////

        /*
        h_X_Y->Reset();
        h2_X_Y->Reset();
        TCanvas* c_X_Y  = new TCanvas(Form("c_X_Y_%d",n_array),Form("c_X_Y_%d",n_array));
        TCanvas* c2_X_Y  = new TCanvas(Form("c2_X_Y_%d",n_array),Form("c2_X_Y_%d",n_array));
        */

        for (int i = 0; i <aliSource->Npatterns(); i++ ) {

            cout << "Doing Pattern " << i << endl;

            EdbPattern* pat = aliSource->GetPattern(i);
            Int_t nPat=pat->N();
	    
	    EdbPattern* patClone = aliNEW->GetPattern(i);
	    Int_t nPatClone=patClone->N();

            for (int j = 0; j < nPat; j++ ) {

                if (j%25000==0) cout << j << "  out of " << nPat << endl;

                seg = pat->GetSegment(j);
//                if (seg->MCEvt()>0) continue;

                mini[0]=seg->X()-halfpatternsize;
                mini[1]=seg->Y()-halfpatternsize;
                maxi[0]=seg->X()+halfpatternsize;
                maxi[1]=seg->Y()+halfpatternsize;
                EdbPattern* singlePattern=(EdbPattern*)aliSource->GetPattern(i)->ExtractSubPattern(mini,maxi,0);
                Int_t nPatsinglePattern=singlePattern->N();
                /*
                		cout <<"------------------------"<< endl;
                		cout << j << "  out of " << nPat << endl;
                		cout << nPatsinglePattern << "  nPatsinglePattern  entries for this subpattern" << endl;
                		singlePattern->Print();
                		cout <<"------------------------"<< endl;
                */
                for (int k = 0; k < nPatsinglePattern; k++ ) {

                    EdbSegP* seg1 = singlePattern->GetSegment(k);

//                    if (seg1->MCEvt()>0) continue;
                    // if (seg==seg1) continue; // doesnt work, since the new subpattern has new adresses
                    // for the segments, so rather compare segments with the function EdbSegP::IsEqual
                    if (seg->IsEqual(seg1)) continue;

                    // Skip already large distances
                    if (TMath::Abs(seg->X()-seg1->X())>40.1) continue;
                    if (TMath::Abs(seg->Y()-seg1->Y())>40.1) continue;
                    if (TMath::Abs(seg->TX()-seg1->TX())>0.21) continue;
                    if (TMath::Abs(seg->TY()-seg1->TY())>0.21) continue;
		    
		    // To avoid double filling (seg-seg1 and seg1-seg)
		    // we restrict to seg->ID() < seg1->ID()
		    if (seg->ID() > seg1->ID()) continue;


                    // Take only interesting combinations:
                    deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );
                    if (deltaR>40) continue;

                    deltaTanTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                    if (deltaTanTheta>0.2) continue;

                    /// AATTENTION CHECKING function "angle" of EdbSegP (which is asin really)...
                    deltaThetaTEST = TMath::ASin(seg->Angle(*seg,*seg1));


                    angleTheta =  EdbMath::Angle3( seg->TX(), seg->TY(),seg1->TX(), seg1->TY() );
                    deltaTheta = deltaTanTheta;
                    // we explicitly use here not the (correct) angle,
                    // but the Delta Tan Version, as also in the shower reconstruction, to treat it in a consistent way!
                    deltachi2 = seg->Chi2()-seg1->Chi2();
                    deltaW = seg->W()-seg1->W();


		    // Fill Histograms (only for first segment, 
		    // where value of just one segment is needed)
                    h_dR_dT->Fill(deltaTheta,deltaR);
                    h_dchi2_dW->Fill(deltaW,deltachi2);
                    h_chi2_W->Fill(seg->W(),seg->Chi2());
                    h_TX_TY->Fill(seg->TX(),seg->TY());
                    h_DeltaTanTheta_Angle ->Fill(angleTheta,deltaTanTheta);
                    h_X_Y->Fill(seg->X(),seg->Y());
                    
                    
                    
                    // Region check of the (Fake-)BTPair
                    IsRegion0=kFALSE;
                    IsRegion1=kFALSE;
                    IsRegion2=kFALSE;

                    // Linear Correlation Line by experimental values:
                    // dR_allowed = 11(micron)/0.1(rad) * dTheta  // b??????
                    // dR_allowed = 5(micron)/0.05(rad) * dTheta  // b138756
//                     dR_allowed = 5/0.05*deltaTheta;
//                     dR_allowed = 10.75/0.1*deltaTheta;
                    dR_allowed = 14.0/0.13*deltaTheta;
//                     14(micron)/0.13(rad)
                    dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);
                    
                    // Set flags for region
                    if (dR_allowed_m_deltaR<0.5) IsRegion0=kTRUE;
                    if (deltaR<3.0 && deltaTheta<0.01) IsRegion1=kTRUE;
                    if (abs(deltaR-6.0)<2 && deltaTheta<0.01) IsRegion2=kTRUE;  
                    


		    //==================================================
		    if (SELECTTYPE==999) ;  // dont impose extra cut...
		    
                    if (SELECTTYPE==0)   if (!IsRegion0) continue;
		    
                    if (SELECTTYPE==1)   if (!IsRegion1) continue;
                    
                    if (SELECTTYPE==2)   if (!IsRegion2) continue;
		    
		    if (SELECTTYPE==3)   if ( !(IsRegion0 || IsRegion1 || IsRegion2) ) continue;
		    //==================================================


		    // Fill Histograms (only for first segment, 
		    // where value of just one segment is needed)
                    h2_dR_dT->Fill(deltaTheta,deltaR);
                    h2_dchi2_dW->Fill(deltaW,deltachi2);
                    h2_chi2_W->Fill(seg->W(),seg->Chi2());
                    h2_TX_TY->Fill(seg->TX(),seg->TY());
                    h2_X_Y->Fill(seg->X(),seg->Y());


		    //cout << " For j = " << j << " we have a segment pair, which is close! k=" << k << endl;
		    //seg->PrintNice();
		    //seg1->PrintNice();
		    	    
		    //==================================================
		    patClone->AddSegmentNoDuplicate(*seg); // Add this first.
		    patClone->AddSegmentNoDuplicate(*seg1);
		    //==================================================


                } // for (int k = j+1; k <pat->N(); k++ )
                

                delete singlePattern;

            }// for (int j)
            

            if (QUICK) i+=aliSource->Npatterns()-1;
        } // for (int i)

        
// // // // // // // // // // // // // // // // // // // // // // // // // // // //         
    TFile* f = new TFile(Form("ScanVolume_Ali_%d_CUTTYPE_%d_SELECTTYPE_%d.root",n_array,CUTTYPE,SELECTTYPE),"RECREATE");
    aliNEW -> Write();    
    aliNEW->Print();
// // // // // // // // // // // // // // // // // // // // // // // // // // // // 
    

    } //      for (int n_array = 0; n_array < N_array; ++n_array)

    
    
    
          
                // Draw histograms
                if (1==1) {
                    c_dR_dT->cd();
                    h_dR_dT->Draw("colz");
                    h_dR_dT->GetXaxis()->SetTitle("#Delta tan #theta");
                    h_dR_dT->GetYaxis()->SetTitle("#DeltaR (#mum)");
                    c_dR_dT->SetLogz();
                    gPad->Update();
                    c_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT.pdf");

                    c_dchi2_dW->cd();
                    h_dchi2_dW->Draw("colz");
                    gPad->Update();
                    h_dchi2_dW->GetYaxis()->SetTitle("#Delta#chi^{2}");
                    h_dchi2_dW->GetXaxis()->SetTitle("#DeltaW");
                    gPad->Update();
                    c_dchi2_dW->Print("Plot_ViewOverlap_CPData_dchi2_dW.pdf");
		    
                    c2_dR_dT->cd();
                    h2_dR_dT->Draw("colz");
                    gPad->Update();
                    h2_dR_dT->GetXaxis()->SetTitle("#Delta tan #theta");
                    h2_dR_dT->GetYaxis()->SetTitle("#DeltaR (#mum)");
                    c2_dR_dT->SetLogz();
                    gPad->Update();
                    c2_dR_dT->Print(Form("Plot_ViewOverlap_CPData_dR_dT_SELECTTYPE_%d.pdf",SELECTTYPE));
		    
                    c2_dchi2_dW->cd();
                    h2_dchi2_dW->Draw("colz");
                    h2_dchi2_dW->GetYaxis()->SetTitle("#Delta#chi^{2}");
                    h2_dchi2_dW->GetXaxis()->SetTitle("#DeltaW");
                    gPad->Update();
                    c2_dchi2_dW->Print(Form("Plot_ViewOverlap_CPData_dchi2_dW_SELECTTYPE_%d.pdf",SELECTTYPE));
		    
                    c_chi2_W->cd();
                    h_chi2_W->Draw("colz");
                    h_chi2_W->GetYaxis()->SetTitle("#chi^{2}");
                    h_chi2_W->GetXaxis()->SetTitle("W");
                    gPad->Update();
                    c_chi2_W->Print("Plot_ViewOverlap_CPData_chi2_W.pdf");

                    c2_chi2_W->cd();
                    h2_chi2_W->Draw("colz");
                    h2_chi2_W->GetYaxis()->SetTitle("#chi^{2}");
                    h2_chi2_W->GetXaxis()->SetTitle("W");
                    gPad->Update();
		    c2_chi2_W->Print(Form("Plot_ViewOverlap_CPData_chi2_W_SELECTTYPE_%d.pdf",SELECTTYPE));

                    c_TX_TY->cd();
                    h_TX_TY->Draw("colz");
                    h_TX_TY->GetYaxis()->SetTitle("tan #theta_{Y}");
                    h_TX_TY->GetXaxis()->SetTitle("tan #theta_{X}");
                    c_TX_TY->SetLogz();
                    gPad->Update();
                    c_TX_TY->Print("Plot_ViewOverlap_CPData_TX_TY.pdf");

                    c2_TX_TY->cd();
                    h2_TX_TY->Draw("colz");
                    h2_TX_TY->GetYaxis()->SetTitle("tan #theta_{Y}");
                    h2_TX_TY->GetXaxis()->SetTitle("tan #theta_{X}");
                    c2_TX_TY->SetLogz();
                    gPad->Update();
		    c2_TX_TY->Print(Form("Plot_ViewOverlap_CPData_TX_TY_SELECTTYPE_%d.pdf",SELECTTYPE));

		    
                    c_DeltaTanTheta_Angle->cd();
                    h_DeltaTanTheta_Angle->Draw("colz");
                    h_DeltaTanTheta_Angle->GetYaxis()->SetTitle("#Delta tan #theta");
                    h_DeltaTanTheta_Angle->GetXaxis()->SetTitle("angle #Theta (rad)");
                    c_DeltaTanTheta_Angle->SetLogz();
                    gPad->Update();
                    c_DeltaTanTheta_Angle->Print("Plot_ViewOverlap_CPData_DeltaTanTheta_Angle.pdf");


                    c_X_Y->cd();
                    h_X_Y->Draw("colz");
                    h_X_Y->GetXaxis()->SetTitle("X (#mum)");
                    h_X_Y->GetYaxis()->SetTitle("Y (#mum)");
                    c_X_Y->SetLogz();
                    gPad->Update();
                    TPaletteAxis *palette = (TPaletteAxis*)h_X_Y->GetListOfFunctions()->FindObject("palette");
                    palette->SetY1NDC(0.2);
                    palette->SetX2NDC(0.93);
                    gPad->Update();
                    c_X_Y->Print("Plot_ViewOverlap_CPData_X_Y.pdf");

                    c2_X_Y->cd();
                    h2_X_Y->Draw("colz");
                    h2_X_Y->GetXaxis()->SetTitle("X (#mum)");
                    h2_X_Y->GetYaxis()->SetTitle("Y (#mum)");
                    c2_X_Y->SetLogz();
                    gPad->Update();
                    TPaletteAxis *palette = (TPaletteAxis*)h2_X_Y->GetListOfFunctions()->FindObject("palette");
                    palette->SetY1NDC(0.2);
                    palette->SetX2NDC(0.93);
                    gPad->Update();
		    c2_X_Y->Print(Form("Plot_ViewOverlap_CPData_X_Y_SELECTTYPE_%d.pdf",SELECTTYPE));
		    
                } // Redraw Histos

                
	    
    TFile* f = new TFile(Form("HistosFakeDoubleBT_CUTTYPE_%d_SELECTTYPE_%d.root",CUTTYPE,SELECTTYPE),"RECREATE");
    h_dR_dT -> Write();
    h2_dR_dT-> Write();
    h_dchi2_dW -> Write();
    h2_dchi2_dW -> Write();
    h_chi2_W -> Write();
    h2_chi2_W -> Write();
    h_TX_TY -> Write();
    h2_TX_TY-> Write();
    h_DeltaTanTheta_Angle -> Write();
    h_X_Y -> Write();
    h2_X_Y -> Write();
    f->Close();

    return;
}