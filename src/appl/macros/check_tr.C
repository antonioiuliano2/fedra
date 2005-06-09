TCut trcut = "t.eFlag>=0.&&t.eProb>0.001";

void check_tr()
{
  check_volume();
  check_eff();
  check_resolution();
  check_surf();
  check_errXY();
  check_errTXTY();
}

//-----------------------------------------------------------------
void check_resolution()
{
  TCanvas *c = new TCanvas("res","tracks_resolution");
  c->Divide(2,2);

  c->cd(1);
  tracks->Draw("(s.eTX-sf.eTX)>>hstx(100,-0.1.,0.1)",trcut&&"nseg>3");
  c->cd(3);
  tracks->Draw("(s.eTY-sf.eTY)>>hsty(100,-0.1.,0.1)",trcut&&"nseg>3");

  gStyle->SetOptStat(111111);
  c->cd(2);
  tracks->Draw("(s.eX-sf.eX)>>hsx(60,-15.,15.)",trcut&&"nseg>3");
  c->cd(4);
  tracks->Draw("(s.eY-sf.eY)>>hsy(60,-15.,15.)",trcut&&"nseg>3");

  gStyle->SetOptStat(1111);
}

//-----------------------------------------------------------------
void check_eff(const char *nplmin="7")
{
  tracks->SetAlias("npl0",nplmin);

  TCanvas *c = new TCanvas("eff","tracks_eff");
  c->Divide(2,2);

  c->cd(1);
  tracks->Draw("1.*(nseg-2)/(npl-2):sqrt(t.eTY*t.eTY+t.eTX*t.eTX)>>heff(25)",trcut&&"npl>=npl0","prof");
  c->cd(2);
  tracks->Draw("n0",trcut&&"npl>=npl0");
  c->cd(3);
  tracks->Draw("t.eProb",trcut&&"npl>=npl0");
  c->cd(4);
  tracks->Draw("s.eZ", trcut );
}

//-----------------------------------------------------------------
void check_volume()
{
  TCanvas *c = new TCanvas("volume","tracks_volume");
  c->Divide(2,2);

  c->cd(1);
  tracks->Draw("s.eY:s.eX>>hxya(200,200)", trcut );
  c->cd(3);
  tracks->Draw("s.eZ", trcut );
  c->cd(2);
  tracks->Draw("s.eX", trcut );
  c->cd(4);
  tracks->Draw("s.eY", trcut );

}

//-----------------------------------------------------------------
void check_surf()
{
  TCanvas *c = new TCanvas("rastr","tracks_rastr");
  c->Divide(1,2);

  c->cd(1);
  tracks->Draw("s.eY:(s.eX+s.ePID*60000)>>hyx(500,100)", trcut );
  c->cd(2);
  tracks->Draw("s.eX:(s.eY+s.ePID*60000)>>hxy(500,100)", trcut );
}

//-----------------------------------------------------------------
void check_errXY()
{
  TCanvas *c = new TCanvas("errXY","tracks_errXY");
  c->Divide(1,2);

  c->cd(1);
  tracks->Draw("s.eX-sf.eX:(s.eX+s.ePID*60000)>>hdxx(500,100)", trcut );
  c->cd(2);
  tracks->Draw("s.eY-sf.eY:(s.eY+s.ePID*60000)>>hdyy(500,100)", trcut );
}

//-----------------------------------------------------------------
void check_errTXTY()
{
  TCanvas *c = new TCanvas("errTXTY","tracks_errTXTY");
  c->Divide(1,2);

  c->cd(1);
  tracks->Draw("s.eTX-sf.eTX:(s.eX+s.ePID*60000)>>hdtxx(500,100)", trcut );
  c->cd(2);
  tracks->Draw("s.eTY-sf.eTY:(s.eY+s.ePID*60000)>>hdtyy(500,100)", trcut );
}
