void check_tr()
{
  check_volume();
  check_surf();
  check_errXY();
  check_errTXTY();
}

//-----------------------------------------------------------------
check_volume()
{
  TCanvas *c = new TCanvas("volume","tracks_volume");
  c->Divide(2,2);

  c->cd(1);
  tracks->Draw("s.eY:s.eX>>h(200,200)");
  c->cd(2);
  tracks->Draw("s.eX");
  c->cd(3);
  tracks->Draw("s.eY");
  c->cd(4);
  tracks->Draw("s.eZ");
}

//-----------------------------------------------------------------
check_surf()
{
  TCanvas *c = new TCanvas("rastr","tracks_rastr");
  c->Divide(1,2);

  c->cd(1);
  tracks->Draw("s.eY:(s.eX+s.ePID*60000)>>h(500,100)");
  c->cd(2);
  tracks->Draw("s.eX:(s.eY+s.ePID*60000)>>h(500,100)");
}

//-----------------------------------------------------------------
check_errXY()
{
  TCanvas *c = new TCanvas("errXY","tracks_errXY");
  c->Divide(1,2);

  c->cd(1);
  tracks->Draw("s.eX-sf.eX:(s.eX+s.ePID*60000)>>h(500,100)");
  c->cd(2);
  tracks->Draw("s.eY-sf.eY:(s.eY+s.ePID*60000)>>h(500,100)");
}

//-----------------------------------------------------------------
check_errTXTY()
{
  TCanvas *c = new TCanvas("errTXTY","tracks_errTXTY");
  c->Divide(1,2);

  c->cd(1);
  tracks->Draw("s.eTX-sf.eTX:(s.eX+s.ePID*60000)>>h(500,100)");
  c->cd(2);
  tracks->Draw("s.eTY-sf.eTY:(s.eY+s.ePID*60000)>>h(500,100)");
}
