TH1F *hp0, *hp1, *hp2, *hp3, *hp4, *hp5, *hp6, *hp7, *hp8;

extern Vertex *v;

void BookHists(void)
{
        hp0 = new TH1F("Hist_Vt_Dx","Vertex X error",200,(Axis_t)-100,(Axis_t)100);
        hp1 = new TH1F("Hist_Vt_Dy","Vertex Y error",200,(Axis_t)-100,(Axis_t)100);
        hp2 = new TH1F("Hist_Vt_Dz","Vertex Z error",200,(Axis_t)-1000,(Axis_t)1000);
        hp3 = new TH1F("Hist_Vt_Sx","Vertex X sigma",200,(Axis_t)-100,(Axis_t)100);
        hp4 = new TH1F("Hist_Vt_Sy","Vertex Y sigma",200,(Axis_t)-100,(Axis_t)100);
        hp5 = new TH1F("Hist_Vt_Sz","Vertex Z sigma",200,(Axis_t)-1000,(Axis_t)1000);
        hp6 = new TH1F("Hist_Vt_Chi2","Vertex Chi-square/D.F.",50,(Axis_t)0,(Axis_t)5);
        hp7 = new TH1F("Hist_Vt_Prob","Vertex Chi-square Probability",100,(Axis_t)0,(Axis_t)1);
        hp8 = new TH1F("Hist_Vt_Ntra","Vertex Ntracks",20,(Axis_t)0,(Axis_t)20);
        return;
}
int DrawHists(void)
{
    char name[128], cname[128], pname[128];
    sprintf(name, "Vertex histograms");
    sprintf(cname, "Canvas_vertex_histograms");
    sprintf(pname, "Pad_vertex_histograms");
    TCanvas *c1 = new TCanvas(cname, name, 10, 10, 1000, 750);
    c1->cd();
    TPad *pad1 = new TPad(pname, name, 0.01, 0.01, 0.97, 0.97, 0);
    pad1->Draw();
    pad1->Divide(3,3);
    TPad *pad1_1 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_1")));
    pad1_1->cd();
    hp0->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_2 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_2")));
    pad1_2->cd();
    hp1->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_3 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_3")));
    pad1_3->cd();
    hp2->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_4 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_4")));
    pad1_4->cd();
    hp3->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_5 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_5")));
    pad1_5->cd();
    hp4->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_6 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_6")));
    pad1_6->cd();
    hp5->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_7 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_7")));
    pad1_7->cd();
    hp6->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_8 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_8")));
    pad1_8->cd();
    hp7->Draw();
    sprintf(pname, "Pad_vertex_histograms");
    TPad *pad1_9 = (TPad *)(pad1->GetPrimitive(strcat(pname,"_9")));
    pad1_9->cd();
    hp8->Draw();
    return 0;
}
void FillHists(void)
{
/*	cout << "Vertex XYZ is "
	     << v->vx()
	     << ", "
	     << v->vy()
	     << ", "
	     << v->vz()
	     << ", Chi2 is "
	     << v->chi2()
	     << ", Ntracks is "
	     << v->ntracks() << endl; */
//	hp0->Fill(5.);
	if (v)
	{
	    hp0->Fill((v->vx())-vx_mc);
	    hp1->Fill((v->vy())-vy_mc);
	    hp2->Fill((v->vz())-vz_mc);
	    hp3->Fill(sqrt(v->vtx_cov_x()));
	    hp4->Fill(sqrt(v->vtx_cov_y()));
	    hp5->Fill(sqrt(v->vtx_cov_z()));
	    hp6->Fill(v->ndf() > 0 ? v->chi2()/v->ndf() : 0);
	    hp7->Fill(v->prob());
	    hp8->Fill(v->ntracks());
	}
	return;
}
