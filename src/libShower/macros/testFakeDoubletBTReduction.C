// // // // // // // // // // // // // // // // // // // // // // // // // //
// Purpose of this script:
// to show the behavior in the cp.root DATA that some BTs
// are infact duplicated due to scanning effect
// (zone overlapping and different sysal correction at the edges of the
//  zones)
// // // // // // // // // // // // // // // // // // // // // // // // // //

{
    TFile* f = new TFile("ScanVolume_Ali.root");
    EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");
    aliSource = ali;

// All Segment Pairs
    TH2D* h_dR_dT = new TH2D("h_dR_dT","",200,0,0.1,200,0,50);
    TH2D* h2_dR_dT = new TH2D("h2_dR_dT","",200,0,0.1,200,0,50);

// after the cut into the line area (to see if these BTs are identical, so should also be their chi2 and W values)s
    TH2D* h_dchi2_dW = new TH2D("h_dchi2_dW","",50,-10,10,100,-1,1);
    TH2D* h2_dchi2_dW = new TH2D("h2_dchi2_dW","",50,-10,10,100,-1,1);

    TCanvas* c_dR_dT = new TCanvas();
    TCanvas* c_dchi2_dW = new TCanvas();
    TCanvas* c2_dR_dT = new TCanvas();
    TCanvas* c2_dchi2_dW = new TCanvas();

    Float_t deltaR=0;
    Float_t deltaTheta=0;
    Float_t deltachi2=0;
    Float_t deltaW=0;

//      for (int i = 0; i <1; i++ ) {
    for (int i = 0; i <aliSource->Npatterns(); i++ ) {

        cout << i << endl;

        EdbPattern* pat = aliSource->GetPattern(i);
        Int_t nPat=pat->N();

        for (int j = 0; j < nPat-1; j++ ) {

            if (nPat>1000) if (j%(nPat/1000)==0) cout << "." << flush;

            seg = pat->GetSegment(j);
            if (seg->MCEvt()>0) continue;

            for (int k = j+1; k < nPat; k++ ) {

                seg1 = pat->GetSegment(k);
                if (seg1->MCEvt()>0) continue;

                // Skip already large distances
                if (TMath::Abs(seg->X()-seg1->X())>50.1) continue;
                if (TMath::Abs(seg->Y()-seg1->Y())>50.1) continue;
                if (TMath::Abs(seg->TX()-seg1->TX())>0.15) continue;
                if (TMath::Abs(seg->TY()-seg1->TY())>0.15) continue;


                deltaTheta=TMath::Sqrt( TMath::Power(seg->TX()-seg1->TX(),2)+TMath::Power(seg->TY()-seg1->TY(),2) );
                deltaR=TMath::Sqrt( TMath::Power(seg->X()-seg1->X(),2)+TMath::Power(seg->Y()-seg1->Y(),2) );
                deltachi2 = seg->Chi2()-seg1->Chi2();
                deltaW = seg->W()-seg1->W();
                
                
                // Take only interesting combinations:
                if (deltaTheta>0.15) continue;

                h_dR_dT->Fill(deltaTheta,deltaR);
                h_dchi2_dW->Fill(deltaW,deltachi2);


                // line defined by experimental values:
                // dR_allowed = 11(micron)/0.1(rad) * dTheta  // b??????
                // dR_allowed = 5(micron)/0.05(rad) * dTheta  // b138756
                dR_allowed = 5/0.05*deltaTheta;
                dR_allowed = 10.75/0.1*deltaTheta;
                dR_allowed_m_deltaR=TMath::Abs(deltaR-dR_allowed);

                // either:
                if (dR_allowed_m_deltaR>0.75) continue;             // case a: "_WithinLinearCorrelationLines"
                // or:
                // comment this  when you want so see also the very
                // next close segments (in space and angle)
//                 if (deltaR>2.0 || deltaTheta>0.02) continue;     // case b: "_WithinOriginSquare"


                h2_dR_dT->Fill(deltaTheta,deltaR);
                h2_dchi2_dW->Fill(deltaW,deltachi2);

            } // for (int k = j+1; k <pat->N(); k++ )

            // Redraw histograms for quicker updates:
            if (j%5000==0) {
                c_dR_dT->cd();
                h_dR_dT->Draw("colz");
                h_dR_dT->GetXaxis()->SetTitle("#Delta#Theta (rad)");
                h_dR_dT->GetYaxis()->SetTitle("#DeltaR (#mum)");
                gPad->Update();
                c_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT.pdf");
// // // // // // // // // //
                c_dchi2_dW->cd();
                h_dchi2_dW->DrawCopy("colz");
                gPad->Update();
                h_dchi2_dW->GetYaxis()->SetTitle("#Delta#chi^{2}");
                h_dchi2_dW->GetXaxis()->SetTitle("#DeltaW");
                gPad->Update();
                c_dchi2_dW->Print("Plot_ViewOverlap_CPData_chi2_W.pdf");
                c2_dR_dT->cd();
                h2_dR_dT->Draw("colz");
                gPad->Update();
                h2_dR_dT->GetXaxis()->SetTitle("#Delta#Theta (rad)");
                h2_dR_dT->GetYaxis()->SetTitle("#DeltaR (#mum)");
                gPad->Update();
                c2_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT_WithinLinearCorrelationLines.pdf");      // for case a
//                 c2_dR_dT->Print("Plot_ViewOverlap_CPData_dR_dT_WithinOriginSquare.pdf");                // for case b
                c2_dchi2_dW->cd();
                h2_dchi2_dW->Draw("colz");
                h2_dchi2_dW->GetYaxis()->SetTitle("#Delta#chi^{2}");
                h2_dchi2_dW->GetXaxis()->SetTitle("#DeltaW");
                gPad->Update();
                c2_dchi2_dW->Print("Plot_ViewOverlap_CPData_chi2_W_WithinLinearCorrelationLines.pdf");      // for case a
//                 c2_dchi2_dW->Print("Plot_ViewOverlap_CPData_chi2_W_WithinOriginSquare.pdf");                // for case b
            }

        }// for (int j)

    } // for (int i)


    return;
}