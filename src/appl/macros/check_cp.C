TCut cut1("pid2>-1&&eCHI2P<1.5");
TCut sameview("(s1.eAid[1]==s2.eAid[1])");
TCut diffview("(s1.eAid[1]!=s2.eAid[1])");
TCut diffarea("(s1.eAid[0]!=s2.eAid[0])");

//-----------------------------------------------------------------
void check_cp()
{
  init();
  check_all();
}

//-----------------------------------------------------------------
void check_all()
{
  check_surf();
  check_sigma();
  check_puls();
  //check_view();
  check_shrinkage();
  check_distorsion();
  correct_shrinkage();
}

//-----------------------------------------------------------------
void init()
{
  gStyle->SetOptFit(0001);
  couples->SetAlias("tx","(s2.eX-s1.eX)/(s2.eZ-s1.eZ)");
  couples->SetAlias("ty","(s2.eY-s1.eY)/(s2.eZ-s1.eZ)");  
  couples->SetAlias("t","sqrt(tx*tx+ty*ty)");
  couples->SetAlias("ts","sqrt(s.eTX*s.eTX + s.eTY*s.eTY)");
}

//-----------------------------------------------------------------
void check_surf()
{
  TCanvas *surf = new TCanvas("surf","couples_surf");
  surf->Divide(2,2);
  surf->cd(1);
  couples->Draw("s.eY:s.eX");
  surf->cd(2);
  couples->Draw("s.eTY:s.eTX");
  surf->cd(3);
  couples->Draw("eCHI2P");
  couples->Draw("eCHI2P",sameview,"same");
  couples->Draw("eCHI2P",diffview,"same");
  couples->Draw("eCHI2P",diffarea,"same");
  surf->cd(4);
  surf->GetPad(4)->SetGrid(1,1);
  couples->Project("hs" ,"s.eW:ts" , cut1,"prof");
  couples->Project("hs1","s1.eW:ts", cut1,"prof");
  couples->Project("hs2","s2.eW:ts", cut1,"prof");
  hs->Draw();
  hs1->SetLineColor(kBlue);
  hs1->Draw("same");
  hs2->SetLineColor(kRed);
  hs2->Draw("same");
  //couples->Draw("s.eTY-ty:s.eTX-tx");
}

//-----------------------------------------------------------------
void check_sigma()
{
  couples->Project("htx1", "abs(s1.eTX-tx):tx",cut1,"prof");
  couples->Project("htx2", "abs(s2.eTX-tx):tx",cut1,"prof");
  couples->Project("hty1", "abs(s1.eTY-ty):ty",cut1,"prof");
  couples->Project("hty2", "abs(s2.eTY-ty):ty",cut1,"prof");

  TCanvas *cs = new TCanvas("csig","couples_sigma");
  cs->Divide(2,1);
 
  cs->cd(1);
  cs->GetPad(1)->SetGrid(1,1);
  htx1->SetLineColor(kBlue);
  htx1->Draw();
  htx2->SetLineColor(kRed);
  htx2->Draw("same");
  
  cs->cd(2);
  cs->GetPad(2)->SetGrid(1,1);
  hty1->SetLineColor(kBlue);
  hty1->Draw();
  hty2->SetLineColor(kRed);
  hty2->Draw("same");  
}

//-----------------------------------------------------------------
void check_puls()
{
  TCanvas *cs = new TCanvas("cpuls","couples_puls");
  cs->Divide(2,2);
  cs->cd(1);
  couples->Draw("s1.eW" );
  cs->cd(2);
  couples->Draw("s2.eW" );
  cs->cd(3);
  couples->Draw("s1.eW", cut1 );
  cs->cd(4);
  couples->Draw("s2.eW", cut1 );
}

//-----------------------------------------------------------------
void check_view(TCut peak="")
{
  // check the accuracy deterioration in case when the segments are in 
  // the different views

  TCanvas *cs = new TCanvas("cview","couples_view");
  cs->Divide(2,2);
  cs->cd(1);
  couples->Draw("eCHI2P","(s1.eAid[1]==s2.eAid[1])");
  couples->Draw("eCHI2P","(s1.eAid[1]!=s2.eAid[1])","same");
  couples->Draw("eCHI2P","(s1.eAid[0]!=s2.eAid[0])","same");
  cs->cd(2);
  couples->SetMarkerStyle(20);
  couples->Draw("s.eTY:(s1.eAid[1]==s2.eAid[1])",peak,"prof");
  cs->cd(3);
  couples->Draw("s.eTX:(s1.eAid[1]==s2.eAid[1])",peak,"prof");
  cs->cd(4);
  couples->Draw("s.eTX","(s1.eAid[1]==s2.eAid[1])" && peak);
  couples->Draw("s.eTX","(s1.eAid[1]!=s2.eAid[1])" && peak,"same");
  couples->SetMarkerStyle(1);
}

//-----------------------------------------------------------------
void check_shrinkage()
{
  init();
  TCanvas *diff = new TCanvas("diff","couples_shrinkage");
  diff->Divide(2,2);

  diff->cd(1);
  couples->Draw("s1.eTX-tx:tx", cut1 );
  diff->cd(2);
  couples->Draw("s2.eTX-tx:tx", cut1 );
  diff->cd(3);
  couples->Draw("s1.eTY-ty:ty", cut1 );
  diff->cd(4);
  couples->Draw("s2.eTY-ty:ty", cut1 );
}

//-----------------------------------------------------------------
void correct_shrinkage()
{
  // this function check shrinkage and/or distance between linked planes 
  // Note: do not use s.* (linked segment parameters), because them could 
  // be different from the "base angle" calculated here directly

  init();
  TCanvas *cshr = new TCanvas("cshr","couples_shrinkage_corr");
  cshr->Divide(2,2);

  //  TCut cut1("pid2>-1&&eCHI2P<1.5");

  cshr->cd(1);   
  couples->Draw("s1.eTX-tx:tx", cut1,"prof");
  htemp->Fit("pol1","wQ","",-.4,.4);
  float p0 = htemp->GetFunction("pol1")->GetParameter(0);
  float p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side1     : p0 = %f \t p1 = %f \n",p0,p1);
  char str[160]="";
  sprintf(str,"s1.eTX*(1-(%f))-(%f)-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)",p1,p0);
  cshr->cd(3);   couples->Draw(str,"pid2>-1&&eCHI2<1.5","prof");
  htemp->Fit("pol1","wQ","",-.4,.4);

  p0 = htemp->GetFunction("pol1")->GetParameter(0);
  p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side1 corr: p0 = %f \t p1 = %f \n",p0,p1);

  cshr->cd(2);
  couples->Draw("s2.eTX-tx:tx", cut1,"prof");
  htemp->Fit("pol1","wQ","",-.4,.4);
  p0 = htemp->GetFunction("pol1")->GetParameter(0);
  p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side2     : p0 = %f \t p1 = %f \n",p0,p1);
  sprintf(str,"s2.eTX*(1-(%f))-(%f)-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)",p1,p0);
  cshr->cd(4);
  couples->Draw(str, cut1 ,"prof");
  htemp->Fit("pol1","wQ","",-.4,.4);

  p0 = htemp->GetFunction("pol1")->GetParameter(0);
  p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side2 corr: p0 = %f \t p1 = %f \n",p0,p1);
}

//-----------------------------------------------------------------
void check_distorsion()
{
  TCanvas *cs = new TCanvas("cdist","couples_distortion");
  cs->Divide(2,2);
  cs->cd(1);
  couples->Draw("s1.eTX-s.eTX:s.eX", "abs(ts)<0.15" );
  cs->cd(2);
  couples->Draw("s1.eTY-s.eTY:s.eY", "abs(ts)<0.15" );
  cs->cd(3);
  couples->Draw("s2.eTX-s.eTX:s.eX", "abs(ts)<0.15" );
  cs->cd(4);
  couples->Draw("s2.eTY-s.eTY:s.eY", "abs(ts)<0.15" );
}
