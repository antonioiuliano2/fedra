TCut signal("1");
TCut cut1("pid2>-1&&eCHI2P<1.5");
TCut sameview("(s1.eAid[1]==s2.eAid[1])");
TCut diffview("(s1.eAid[1]!=s2.eAid[1])");
TCut diffarea("(s1.eAid[0]!=s2.eAid[0])");

//-----------------------------------------------------------------
void check_cp(char *signal_cut="1")
{
  signal = signal_cut;
  printf("Check couples with the general selection as: %s\n",signal_cut);
  printf("s1: Red  line\n");
  printf("s2: Blue line\n");
  printf("functions: check_surf, check_sigma, check_shrinkage, check_distortion, check_view, correct_shrinkage\n");
  init();
  check_all();
}

//-----------------------------------------------------------------
void check_all()
{
  check_surf();
  //check_sigma();
  //check_view();
  //check_shrinkage();
  //check_distorsion();
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
  printf("check_surf with the cut: %s \n", signal.GetTitle() );
  TCanvas *surf = new TCanvas("surf","couples_surf",600,800);
  surf->Divide(2,3);
  surf->cd(1);  couples->Draw("s.eY:s.eX",signal);
  surf->cd(2);  couples->Draw("s.eTY:s.eTX",signal);
  surf->cd(5); {
    couples->Draw("eCHI2P", signal );
    couples->SetLineColor(kGreen);
    couples->Draw("eCHI2P", signal && sameview ,"same");
    couples->SetLineColor(kRed);
    couples->Draw("eCHI2P", signal && diffview,"same");
    couples->SetLineColor(6);
    couples->Draw("eCHI2P", signal && diffarea,"same");
  }
  couples->SetLineColor(1);
  surf->cd(3); {
    couples->SetLineColor(kBlue);
    couples->Draw("s1.eW>>hw1(15,5,20)", signal );
    couples->SetLineColor(kRed);
    couples->Draw("s2.eW", signal,"same");
  } 
  couples->SetLineColor(1);
  
  surf->cd(4); couples->Draw("s.eW>>hw(25,10,35)", signal );
  surf->cd(6);  couples->Draw("eCHI2P:s.eW>>hchiw(25,10,35,30,0,3.)", signal,"colZ");
}

//-----------------------------------------------------------------
void check_sigma()
{
  printf("check_sigma with the cut: %s \n", signal.GetTitle() );

  couples->Project("htx1(20,-1.,1.,40,0.,0.1)", "abs(s1.eTX-tx):tx",signal,"prof");
  couples->Project("htx2(20,-1.,1.,40,0.,0.1)", "abs(s2.eTX-tx):tx",signal,"prof");
  couples->Project("hty1(20,-1.,1.,40,0.,0.1)", "abs(s1.eTY-ty):ty",signal,"prof");
  couples->Project("hty2(20,-1.,1.,40,0.,0.1)", "abs(s2.eTY-ty):ty",signal,"prof");

  TCanvas *cs = new TCanvas("csig","couples_sigma");
  cs->Divide(2,3);

  cs->cd(1);{
    cs->GetPad(1)->SetGrid(1,1);
    htx1->SetLineColor(kRed);
    htx1->Draw();
    htx2->SetLineColor(kBlue);
    htx2->Draw("same");
  }
  cs->cd(2);{
    cs->GetPad(2)->SetGrid(1,1);
    hty1->SetLineColor(kRed);
    hty1->Draw();
    hty2->SetLineColor(kBlue);
    hty2->Draw("same");
  }
  couples->Project("htt1(10,0.,1.,40,0.,0.1)", "abs(dst1):ts",signal,"prof");
  couples->Project("htt2(10,0.,1.,40,0.,0.1)", "abs(dst2):ts",signal,"prof");
  couples->Project("htl1(10,0.,1.,40,0.,0.1)", "dsl1:ts",signal,"prof");
  couples->Project("htl2(10,0.,1.,40,0.,0.1)", "dsl2:ts",signal,"prof");

  cs->cd(3); {
    cs->GetPad(3)->SetGrid(1,1);
    htt1->SetLineColor(kRed);
    htt1->Draw();
    htt2->SetLineColor(kBlue);
    htt2->Draw("same");
  }
  cs->cd(4);{
    cs->GetPad(4)->SetGrid(1,1);
    htl1->SetLineColor(kRed);
    htl1->Draw();
    htl2->SetLineColor(kBlue);
    htl2->Draw("same");
    gStyle->SetOptStat("ne");
  }

  surf->cd(5); {
    surf->GetPad(4)->SetGrid(1,1);
    couples->Project("hs" ,"s.eW:ts" , signal, "prof");
    couples->Project("hs1","s1.eW:ts", signal, "prof");
    couples->Project("hs2","s2.eW:ts", signal, "prof");
    hs->Draw();
    hs1->SetLineColor(kRed);
    hs1->Draw("same");
    hs2->SetLineColor(kBlue);
    hs2->Draw("same");
    gStyle->SetOptStat("nemr");
  }
  surf->cd(6); {
    couples->SetLineColor(kBlue);
    couples->Draw("eN1tot:ts", signal, "prof");
    couples->SetLineColor(kRed);
    couples->Draw("eN2tot:ts", signal, "prof same");
  } 
  couples->SetLineColor(1);


}

//-----------------------------------------------------------------
void check_view()
{
  // check the accuracy deterioration in case when the segments are in 
  // the different views

  printf("check_view with the cut: %s \n", signal.GetTitle() );

  TCanvas *cs = new TCanvas("cview","couples_view");
  cs->Divide(2,2);
  cs->cd(1);
  couples->Draw("eCHI2P", signal&&sameview);
  couples->Draw("eCHI2P", signal&&diffview,"same");
  couples->Draw("eCHI2P", signal&&diffarea,"same");
  cs->cd(2);
  couples->SetMarkerStyle(20);
  couples->Draw("s.eTY:samevew",signal,"prof");
  cs->cd(3);
  couples->Draw("s.eTX:sameview",signal,"prof");
  cs->cd(4);
  couples->Draw("s.eTX>>htxv(100)",signal&&sameview);
  couples->Draw("s.eTX",signal&&diffview,"same");
  couples->SetMarkerStyle(1);
  gStyle->SetOptStat("nemr");
}

//-----------------------------------------------------------------
void check_shrinkage()
{
  printf("check_shrinkage with the cut: %s \n", signal.GetTitle() );
  TCanvas *diff = new TCanvas("diff","couples_shrinkage");
  diff->Divide(2,2);

  diff->cd(1);
  couples->Draw("s1.eTX-tx:tx", signal );
  diff->cd(2);
  couples->Draw("s2.eTX-tx:tx", signal );
  diff->cd(3);
  couples->Draw("s1.eTY-ty:ty", signal );
  diff->cd(4);
  couples->Draw("s2.eTY-ty:ty", signal );
  gStyle->SetOptStat("nemr");
}

//-----------------------------------------------------------------
void correct_shrinkage()
{
  // this function check shrinkage and/or distance between linked planes 
  // Note: do not use s.* (linked segment parameters), because them could 
  // be different from the "base angle" calculated here directly

  printf("correct_shrinkage with the cut: %s \n", signal.GetTitle() );
  TCanvas *cshr = new TCanvas("cshr","couples_shrinkage_corr");
  cshr->Divide(2,2);

  //  TCut cut1("pid2>-1&&eCHI2P<1.5");

  cshr->cd(1);   {
    couples->Draw("s1.eTX-tx:tx", signal,"prof");
    htemp->Fit("pol1","wQ","",-.4,.4);
  }
  float p0 = htemp->GetFunction("pol1")->GetParameter(0);
  float p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side1     : p0 = %f \t p1 = %f \n",p0,p1);
  char str[160]="";
  sprintf(str,"s1.eTX*(1-(%f))-(%f)-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)",p1,p0);
  cshr->cd(3);   {
    couples->Draw(str,signal,"prof");
    htemp->Fit("pol1","wQ","",-.4,.4);
  } 
  p0 = htemp->GetFunction("pol1")->GetParameter(0);
  p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side1 corr: p0 = %f \t p1 = %f \n",p0,p1);

  cshr->cd(2);{
    couples->Draw("s2.eTX-tx:tx", signal,"prof");
    htemp->Fit("pol1","wQ","",-.4,.4);
  }
  p0 = htemp->GetFunction("pol1")->GetParameter(0);
  p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side2     : p0 = %f \t p1 = %f \n",p0,p1);
  sprintf(str,"s2.eTX*(1-(%f))-(%f)-(s2.eX-s1.eX)/(s2.eZ-s1.eZ):(s2.eX-s1.eX)/(s2.eZ-s1.eZ)",p1,p0);
  cshr->cd(4); {
    couples->Draw(str, signal ,"prof");
    htemp->Fit("pol1","wQ","",-.4,.4);
  }
  p0 = htemp->GetFunction("pol1")->GetParameter(0);
  p1 = htemp->GetFunction("pol1")->GetParameter(1);
  printf("side2 corr: p0 = %f \t p1 = %f \n",p0,p1);
  gStyle->SetOptStat("nemr");
}

//-----------------------------------------------------------------
void check_distorsion()
{
  printf("check_distortion with the cut: %s \n", signal.GetTitle() );
  TCanvas *cs = new TCanvas("cdist","couples_distortion");
  cs->Divide(2,2);
  cs->cd(1);
  couples->Draw("s1.eTX-s.eTX:s.eX", signal );
  cs->cd(2);
  couples->Draw("s1.eTY-s.eTY:s.eY", signal );
  cs->cd(3);
  couples->Draw("s2.eTX-s.eTX:s.eX", signal );
  cs->cd(4);
  couples->Draw("s2.eTY-s.eTY:s.eY", signal );
  gStyle->SetOptStat("nemr");
}
