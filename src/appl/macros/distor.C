distor(char *fnam)
{
  TH2F* h1=new TH2F("h1","h1",100,-.1,.1,100,-.1,.1);
  ctop=new TCanvas();
// cbot=new TCanvas();
  TFile *f = TFile::Open(fnam);
  tree=(TTree*)f->Get("couples");

  int nentr = int(tree->GetEntries());

  float xoff = -61000.;
  float yoff =  58000.;

  EdbSegP        *s2 = 0,*s=0,*s1=0;
  EdbSegCouple   *cp = 0;
  TClonesArray   *as = 0;
  TClonesArray   *as1 = 0;
  TClonesArray   *as2 = 0;
  Int_t           nseg;
  Float_t  dx_top,dy_top,dx_bot,dy_bot;
  tree->SetBranchAddress("s."  , &s  );
  tree->SetBranchAddress("s1."  , &s1  );
  tree->SetBranchAddress("s2."  , &s2  );
  tree->SetBranchAddress("cp"  , &cp  );
  for(int i=0;i<nentr;i++)
   {
     tree->GetEntry(i);
     if(fabs(s->TY())<.1 && fabs(s->TX())<.1)
     if(cp->N1()==1)
     if(cp->N2()==1)
     if(cp->CHI2P()<1.5)
     {
     dx_top=s->TX()-s1->TX();
     dx_bot=s->TX()-s2->TX();
     dy_top=s->TY()-s1->TY();
     dy_bot=s->TY()-s2->TY();
     addarrow(ctop,s->X()-xoff,s->Y()-yoff,dx_top,dy_top,kRed);
     addarrow(ctop,s->X()-xoff,s->Y()-yoff,dx_bot,dy_bot,kBlue);
  //  addarrow(ctop,s->X(),s->Y(),dx_bot-dx_top,dy_bot-dy_top,kBlue);
//    h1->Fill(dx_top,dx_bot);
     }
     addarrow(ctop,-11600.,-11600.,.1,0.,kBlack);
//     h1->Draw();
  }

}

void addarrow(TCanvas* c, Float_t x, Float_t y, Float_t dx, Float_t dy,int color)
{
 c->cd();
 Float_t ascale=1.; // full scale in radians
 Float_t cscale=24000.; // full scale in microns
 x+=12000.;
 y+=12000.;
 x/=cscale;
 y/=cscale;
 TArrow *a=new TArrow(x,y,x+dx/ascale,y+dy/ascale,0.002);
 a->SetLineColor(color);
 a->Draw();
}
