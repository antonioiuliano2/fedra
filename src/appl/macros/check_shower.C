{
  // load shower.root file
  TFile f("Shower.root");
  TTree *t = (TTree*)f.Get("treebranch");

  TCut cut  = "eProb90==1&&eProb1==1";
  TCut cut2 = "output15>=0";
  TCut cut3 = "sizeb>10";

  cout << endl;
  cout << "Plot 1 :  Number of BT per shower" << endl;
  cout << "Plot 2 :  Neural Network output with 15 plates (1:e-; 0:pion)" << endl;
  cout << "Plot 3 :  NN output versus Nb of BT (showers with 90% efficiency & 1% pion contamination)"<< endl;
  cout << "Plot 4 :  Shower (X-Y) with 90% eff & 1% pion contamination & highest Nb of BT" << endl;
  cout << "Plot 5 :  Shower (Y-Z) with 90% eff & 1% pion contamination & highest Nb of BT" << endl;
  cout << "Plot 6 :  Shower (X-Y-Z) with 90% eff & 1% pion contamination & highest Nb of BT" << endl;

  TCanvas *c = new TCanvas("check_shower","check_shower",1024,900);
  c->Divide(2,3);
  
  c->cd(1);
  t->Draw("sizeb>>NbBT",cut2);
  NbBT->SetTitle("Number of BT per shower");
  
  c->cd(2);
  t->Draw("output15>>NN15",cut2);
  NN15->SetTitle("NN ouput with 15 plates");

  c->cd(3);
  t->Draw("output15:sizeb>>NN15vsNbBT",cut3&&cut2&&cut);
  NN15vsNbBT->SetTitle("NN output (15 plates) VS Nb of BT (90% Eff & 1% Cont)");
  NN15vsNbBT->SetMarkerStyle(20);
  NN15vsNbBT->Draw();


  c->cd(4);
  
  int sizeb;
  t->SetBranchAddress("sizeb", &sizeb);  
  int nentries = t->GetEntries();
  int max=0;
  for (int i=0;i<nentries;i++)
    {
      t->GetEntry(i);

      if (sizeb>max)
	max=sizeb;
    }

  if (max<=10)
      return;

  TCut cut4;
  TString s("sizeb==");
  s += max;
  cut4=s;
  t->Draw("xb:yb>>xy",cut&&cut2&&cut3&&cut4);
  xy->SetMarkerStyle(20);
  xy->Draw();

  c->cd(5);
  t->Draw("yb:zb>>yz",cut&&cut2&&cut3&&cut4);
  yz->SetMarkerStyle(20);
  yz->Draw();

  c->cd(6);
  t->Draw("xb:yb:zb>>xyz",cut&&cut2&&cut3&&cut4);
  xyz->SetMarkerStyle(20);
  xyz->Draw();



  //  f.Close();
}
