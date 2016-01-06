// // // // // // // // // // // // // // // // // // // // // // // // // //
// Purpose of this script:
// to show the behavior in the cp.root DATA that some BTs
// are infact duplicated due to scanning effect
// (zone overlapping and different sysal correction at the edges of the
//  zones)
// // // // // // // // // // // // // // // // // // // // // // // // // //

// a) RCUT 0 eN1==1&&eN2==1
// b) RCUT 0 eN1!=1||eN2!=1
// c) RCUT 0 eN1!=1&&eN2!=1

// // // // // // // // // // // // // // // // // // // // // // // // // //

{

    TObjArray* ali_array = new TObjArray();
    Int_t N_array=1; //5

    for (Int_t n_array = 0; n_array < N_array; ++n_array) {
        TFile* f = new TFile(Form("ScanVolume_Ali_%d.root",n_array));
        EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");
        ali->Print();
        ali_array->Add(ali);
    }
//     return;

    /*
         EdbPVRec* ali = new EdbPVRec();
        EdbDataProc* proc = new EdbDataProc("lnk.def");
        proc->InitVolume(ali,0);
        cout << "read_pvr.C   ::   I have read a EdbPVRec* object from basetrack cp data. " << endl;
        cout << "read_pvr.C   ::   ali = " << ali << " and it has ali->Npatterns()= " << ali->Npatterns() << endl;
        ali_array->Add(ali);
    */



// All Segment Pairs
    TH2D* h_dR_dT = new TH2D("h_dR_dT","",200,0,0.2,200,0,40);
    TH2D* h2_dR_dT = new TH2D("h2_dR_dT","",200,0,0.2,200,0,40);

// after the cut into the line area (to see if these BTs are identical, so should also be their chi2 and W values)s
    TH2D* h_dchi2_dW = new TH2D("h_dchi2_dW","",13,-6.5,6.5,100,-1,1);
    TH2D* h2_dchi2_dW = new TH2D("h2_dchi2_dW","",13,-6.5,6.5,100,-1,1);

    TH2D* h_chi2_W = new TH2D("h_chi2_W","",26,5.5,31.5,100,0,2);
    TH2D* h2_chi2_W = new TH2D("h2_chi2_W","",26,5.5,31.5,100,0,2);

    TH2D* h_TX_TY = new TH2D("h_TX_TY","",100,-0.5,0.5,100,-0.5,0.5);
    TH2D* h2_TX_TY = new TH2D("h2_TX_TY","",100,-0.5,0.5,100,-0.5,0.5);

    TH2D* h_DeltaTanTheta_Angle = new TH2D("h_DeltaTanTheta_Angle","",200,0,0.2,200,0,0.2);

    TH2D* h_X_Y = new TH2D("h_X_Y","",500,0,125000,500,0,125000);
    TH2D* h2_X_Y = new TH2D("h2_X_Y","",500,0,125000,500,0,125000);

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

    // Create Variables For ExtractSubpattern boundaries
    Float_t mini[5];
    Float_t maxi[5];
    mini[2]=-1;
    mini[3]=-1;
    mini[4]=0.0;
    maxi[2]=1;
    maxi[3]=1;
    maxi[4]=20.0;
    Float_t    halfpatternsize=100;


    for (int n_array = 0; n_array < N_array; ++n_array) {
        cout << "Doing Array " << n_array << "   ----------------------------------------" << endl;
        EdbPVRec* ali = (EdbPVRec*)ali_array->At(n_array);
        aliSource = ali;


        h_X_Y->Reset();
        h2_X_Y->Reset();
        TCanvas* c_X_Y  = new TCanvas(Form("c_X_Y_%d",n_array),Form("c_X_Y_%d",n_array));
        TCanvas* c2_X_Y  = new TCanvas(Form("c2_X_Y_%d",n_array),Form("c2_X_Y_%d",n_array));



// for (int i = 0; i <1; i++ ) {
        for (int i = 0; i <aliSource->Npatterns(); i++ ) {

            cout << "Doing Pattern " << i <<  " cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc " << endl;

            EdbPattern* pat = aliSource->GetPattern(i);
            Int_t nPat=pat->N();

            for (int j = 0; j < nPat; j++ ) {
//             for (int j = 0; j < nPat/5; j++ ) {

                if (j%1000==0) cout << j << "  out of " << nPat << endl;

                seg = pat->GetSegment(j);
//                if (seg->MCEvt()>0) continue;

                mini[0]=seg->X()-halfpatternsize;
                mini[1]=seg->Y()-halfpatternsize;
                maxi[0]=seg->X()+halfpatternsize;
                maxi[1]=seg->Y()+halfpatternsize;
                EdbPattern* singlePattern=(EdbPattern*)aliSource->GetPattern(i)->ExtractSubPattern(mini,maxi,0);
                Int_t nPatsinglePattern=singlePattern->N();

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


                    // Take only interesting combinations:
                    deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );
                    if (deltaR>40) continue;

                    deltaTanTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                    if (deltaTanTheta>0.2) continue;

                    /// ATTENTION CHECKING function "angle" of EdbSegP (which is asin really)...
                    deltaThetaTEST = TMath::ASin(seg->Angle(*seg,*seg1));


                    angleTheta =  EdbMath::Angle3( seg->TX(), seg->TY(),seg1->TX(), seg1->TY() );
                    deltaTheta = deltaTanTheta;
                    // we explicetly use here not the (correct) angle,
                    // but the Delta Tan Version, as also in the shower reconstruction, to treat it in a consistent way!
                    deltachi2 = seg->Chi2()-seg1->Chi2();
                    deltaW = seg->W()-seg1->W();


                    h_dR_dT->Fill(deltaTheta,deltaR);
                    h_dchi2_dW->Fill(deltaW,deltachi2);

                    h_chi2_W->Fill(seg->W(),seg->Chi2());
                    h_chi2_W->Fill(seg1->W(),seg1->Chi2());
                    h_TX_TY->Fill(seg->TX(),seg->TY());
                    h_DeltaTanTheta_Angle ->Fill(angleTheta,deltaTanTheta);
                    h_X_Y->Fill(seg->X(),seg->Y());


                    // line defined by experimental values:
                    // dR_allowed = 11(micron)/0.1(rad) * dTheta  // b??????
                    // dR_allowed = 5(micron)/0.05(rad) * dTheta  // b138756
//                     dR_allowed = 5/0.05*deltaTheta;
//                     dR_allowed = 10.75/0.1*deltaTheta;
                    dR_allowed = 14.0/0.13*deltaTheta;
//                     14(micron)/0.13(rad)
                    dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);


                    // either: I)
                    if (dR_allowed_m_deltaR>0.5) continue;             // case I: "_WithinLinearCorrelationLines"
                    // or: II)
                    // comment this  when you want so see also the very
                    // next close segments (in space and angle)
//                     if (deltaR>3.0 || deltaTheta>0.01) continue;     // case II: "_WithinOriginRectangle"
                    // or: both: III					// case III: "_WithinLinCorrLinesAndOriginRectangle"
//                    if (dR_allowed_m_deltaR>0.5 && (deltaR>3.0 || deltaTheta>0.01) ) continue;


                    h2_dR_dT->Fill(deltaTheta,deltaR);
                    h2_dchi2_dW->Fill(deltaW,deltachi2);

                    h2_chi2_W->Fill(seg->W(),seg->Chi2());
                    h2_chi2_W->Fill(seg1->W(),seg1->Chi2());

                    h2_TX_TY->Fill(seg->TX(),seg->TY());

                    h2_X_Y->Fill(seg->X(),seg->Y());


                } // for (int k = j+1; k <pat->N(); k++ )

                // Redraw histograms for quicker updates:
//                 if (j%10000==0 && j>0) {
                if (j==nPat-1) {
                    c_dR_dT->cd();
                    h_dR_dT->Draw("colz");
                    h_dR_dT->GetXaxis()->SetTitle("#Delta tan #theta");
                    h_dR_dT->GetYaxis()->SetTitle("#DeltaR (#mum)");
                    c_dR_dT->SetLogz();
                    gPad->Update();
                    c_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT.pdf");
// // // // // // // // // //
                    c_dchi2_dW->cd();
                    h_dchi2_dW->DrawCopy("colz");
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
                    gPad->Update();
                    c2_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT_WithinLinearCorrelationLines.pdf");      // for case I
//                  c2_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT_WithinOriginRectangle.pdf");                // for case II
//                    c2_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT_WithinLinCorrLinesAndOriginRectangle.pdf"); // for case III
                    c2_dchi2_dW->cd();
                    h2_dchi2_dW->Draw("colz");
                    h2_dchi2_dW->GetYaxis()->SetTitle("#Delta#chi^{2}");
                    h2_dchi2_dW->GetXaxis()->SetTitle("#DeltaW");
                    gPad->Update();
                    c2_dchi2_dW->Print("Plot_ViewOverlap_CPData_dchi2_dW_WithinLinearCorrelationLines.pdf");      // for case I
//                  c2_dchi2_dW->Print("Plot_ViewOverlap_CPData_dchi2_dW_WithinOriginRectangle.pdf");                // for case II
//                    c2_dchi2_dW->Print("Plot_ViewOverlap_CPData_dchi2_dW_WithinLinCorrLinesAndOriginRectangle.pdf"); // for case III
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
                    c2_chi2_W->Print("Plot_ViewOverlap_CPData_chi2_W_WithinLinearCorrelationLines.pdf");      // for case I
//                  c2_chi2_W->Print("Plot_ViewOverlap_CPData_chi2_W_WithinOriginRectangle.pdf");                // for case II
//                    c2_chi2_W->Print("Plot_ViewOverlap_CPData_chi2_W_WithinLinCorrLinesAndOriginRectangle.pdf"); // for case III


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
                    c2_TX_TY->Print("Plot_ViewOverlap_CPData_TX_TY_WithinLinearCorrelationLines.pdf");  // for case I
//                     c2_TX_TY->Print("Plot_ViewOverlap_CPData_TX_TY_WithinOriginRectangle.pdf"); // for case II
//                    c2_TX_TY->Print("Plot_ViewOverlap_CPData_TX_TY_WithinLinCorrLinesAndOriginRectangle.pdf");  // for case III


                    c_DeltaTanTheta_Angle->cd();
                    h_DeltaTanTheta_Angle->Draw("colz");
                    h_DeltaTanTheta_Angle->GetYaxis()->SetTitle("#Delta tan #theta");
                    h_DeltaTanTheta_Angle->GetXaxis()->SetTitle("angle #Theta (rad)");
                    c_DeltaTanTheta_Angle->SetLogz();
                    gPad->Update();
                    c_DeltaTanTheta_Angle->Print("Plot_ViewOverlap_CPData_DeltaTanTheta_Angle.pdf");



                    c_X_Y->cd();
                    h_X_Y->DrawCopy("colz");
                    gPad->Update();
                    c_X_Y->Print(Form("c_X_Y_%d.pdf",n_array));
                    c2_X_Y->cd();
                    h2_X_Y->DrawCopy("colz");
                    gPad->Update();
                    c2_X_Y->Print(Form("c2_X_Y_%d.pdf",n_array));

                }

                delete singlePattern;

            }// for (int j)

        } // for (int i)


    } //      for (int n_array = 0; n_array < N_array; ++n_array)


    return;
}












