TCut cut1("pid2>-1&&eCHI2P<1.5");
TCut sameview("(s1.eAid[1]==s2.eAid[1])");
TCut diffview("(s1.eAid[1]!=s2.eAid[1])");
TCut diffarea("(s1.eAid[0]!=s2.eAid[0])");

//-----------------------------------------------------------------
void check_cp()
{
  printf("s1: Red  line\n");
  printf("s2: Blue line\n");
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
  //correct_shrinkage();
}

//-----------------------------------------------------------------
void init()
{
  gStyle->SetOptFit(0001);
  couples->SetAlias("tx","(s2.eX-s1.eX)/(s2.eZ-s1.eZ)");          // baseline angle tx
  couples->SetAlias("ty","(s2.eY-s1.eY)/(s2.eZ-s1.eZ)");          // baseline angle ty
  couples->SetAlias("t","sqrt(tx*tx+ty*ty)");                     // baseline theta
  couples->SetAlias("ts","sqrt(s.eTX*s.eTX + s.eTY*s.eTY)");      // basetrack theta

  couples->SetAlias("t1","sqrt(s1.eTX*s1.eTX + s1.eTY*s1.eTY)");  // theta of the s1
  couples->SetAlias("t2","sqrt(s2.eTX*s2.eTX + s2.eTY*s2.eTY)");  // theta of the s2

  couples->SetAlias("phi","atan(s.eTY/s.eTX)");                   // phi of the s
  couples->SetAlias("phi1","atan(s1.eTY/s1.eTX)");                // phi of the s1
  couples->SetAlias("phi2","atan(s2.eTY/s2.eTX)");                // phi of the s2

  couples->SetAlias("dsx1","s1.eTX-s.eTX");
  couples->SetAlias("dsy1","s1.eTY-s.eTY");
  couples->SetAlias("ds1","sqrt(dsx1*dsx1+dsy1*dsy1)");                 // absoulte angular diff    s1-s
  couples->SetAlias("dsx2","s2.eTX-s.eTX");
  couples->SetAlias("dsy2","s2.eTY-s.eTY");
  couples->SetAlias("ds2","sqrt(dsx2*dsx2+dsy2*dsy2)");                 // absoulte angular diff    s2-s

  couples->SetAlias("dst1","(s1.eTX*s.eTY-s1.eTY*s.eTX)/ts");           // transverse slope diff      s1-s
  couples->SetAlias("dsl1","sqrt(ds1*ds1-dst1*dst1)");                  // longitudinal slope diff    s1-s
  couples->SetAlias("dst2","(s2.eTX*s.eTY-s2.eTY*s.eTX)/ts");           // transverse slope diff      s2-s
  couples->SetAlias("dsl2","sqrt(ds2*ds2-dst2*dst2)");                  // longitudinal slope diff    s2-s
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
  hs1->SetLineColor(kRed);
  hs1->Draw("same");
  hs2->SetLineColor(kBlue);
  hs2->Draw("same");
  //couples->Draw("s.eTY-ty:s.eTX-tx");
  gStyle->SetOptStat("nemr");
 
}

//-----------------------------------------------------------------
void check_sigma()
{

  couples->Project("htx1(20,-1.,1.,40,0.,0.1)", "abs(s1.eTX-tx):tx",cut1,"prof");
  couples->Project("htx2(20,-1.,1.,40,0.,0.1)", "abs(s2.eTX-tx):tx",cut1,"prof");
  couples->Project("hty1(20,-1.,1.,40,0.,0.1)", "abs(s1.eTY-ty):ty",cut1,"prof");
  couples->Project("hty2(20,-1.,1.,40,0.,0.1)", "abs(s2.eTY-ty):ty",cut1,"prof");

  TCanvas *cs = new TCanvas("csig","couples_sigma");
  cs->Divide(2,2);

  cs->cd(1);
  cs->GetPad(1)->SetGrid(1,1);
  htx1->SetLineColor(kRed);
  htx1->Draw();
  htx2->SetLineColor(kBlue);
  htx2->Draw("same");
  
  cs->cd(2);
  cs->GetPad(2)->SetGrid(1,1);
  hty1->SetLineColor(kRed);
  hty1->Draw();
  hty2->SetLineColor(kBlue);
  hty2->Draw("same");

  couples->Project("htt1(10,0.,1.,40,0.,0.1)", "abs(dst1):ts",cut1,"prof");
  couples->Project("htt2(10,0.,1.,40,0.,0.1)", "abs(dst2):ts",cut1,"prof");
  couples->Project("htl1(10,0.,1.,40,0.,0.1)", "dsl1:ts",cut1,"prof");
  couples->Project("htl2(10,0.,1.,40,0.,0.1)", "dsl2:ts",cut1,"prof");

  cs->cd(3);
  cs->GetPad(3)->SetGrid(1,1);
  htt1->SetLineColor(kRed);
  htt1->Draw();
  htt2->SetLineColor(kBlue);
  htt2->Draw("same");

  cs->cd(4);
  cs->GetPad(4)->SetGrid(1,1);
  htl1->SetLineColor(kRed);
  htl1->Draw();
  htl2->SetLineColor(kBlue);
  htl2->Draw("same");
  gStyle->SetOptStat("ne");
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
  gStyle->SetOptStat("nemr");
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
  gStyle->SetOptStat("nemr");
}

//-----------------------------------------------------------------
void check_shrinkage()
{
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
  gStyle->SetOptStat("nemr");
}

//-----------------------------------------------------------------
void correct_shrinkage()
{
  // this function check shrinkage and/or distance between linked planes 
  // Note: do not use s.* (linked segment parameters), because them could 
  // be different from the "base angle" calculated here directly

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
  gStyle->SetOptStat("nemr");
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
  gStyle->SetOptStat("nemr");
}
