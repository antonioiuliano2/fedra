
//-- Author : Igor Kreslo     12.09.2003

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDisplay                                                           //
//                                                                      //
// Class to display pattern volume in 3D                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
//
//
// Usage: (script testd.C)
//
//	EdbDisplay*  ds;
//	void testd()
//	{
//	gSystem->Load("libEdd.so");
//	gStyle->SetPalette(1);
//	ds=new EdbDisplay("display-t",-6000.,6000.,-6000.,6000.,-20050,110.);
//	ds->SetCuts(-9000,9000,-9000,9000); //Coordinate cuts
//	  TTree *tree;
//	  TFile *f = new TFile("linked_tracks.root");
//	  if (f)  tree = (TTree*)f->Get("tracks");
//	  ds->SetAffine(1,0,0,1,0,0);
//	  ds->SetNsegmin(4);
//	  ds->AddTracksTree(tree,0.,-1.,1.,-1.,1.);
//	  ds->Refresh();
//	}
//
//

#include "EdbDisplay.h"
#include "EdbPVRec.h"
#include "TSlider.h"
#include "TRint.h"
#include "TStyle.h"
#include <cmath>

ClassImp(EdbDisplay);

//________________________________________________________________________
EdbDisplay::EdbDisplay(const char *title, Float_t x0, Float_t x1 , Float_t y0, Float_t y1, Float_t z0, Float_t z1)
{
      vx0=x0;vx1=x1;vy0=y0;vy1=y1;vz0=z0;vz1=z1;
      fCanvas = new TCanvas("c","EMULSION DISPLAY",800,800);
      fCanvas->ToggleEventStatus();
      fCanvas->cd();
      fCutpad=new TPad("t","cutpad",0.,0.,1.,1.);
      fCutpad->Draw();
      fCanvas->cd();
      fImagepad=new TPad("i","imagepad",0.05,0.05,1.,.95);
      fImagepad->SetFillColor(13);
      fImagepad->Draw();
      fImagepad->cd();
      fView = new TView(1);
      fView->SetRange(vx0,vy0,vz0,vx1,vy1,vz1);

      fCutpad->cd();
      cutTX0=-.9;
      cutTX1=.9;
      cutTY0=-.9;
      cutTY1=.9;
      cutX0=-100000.;
      cutX1=100000.;
      cutY0=-100000.;
      cutY1=100000.;
      eNsegmax=100;
      eNsegmin=2;
      eNpieces=0;
      eTr_Co=kFALSE; // Default - show couples
      MaxChi=kTRUE;
      (new TButton("Best Chi2","((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->MaxChi=kTRUE;((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->Refresh();",0.1,0.96,0.3,0.99))->Draw();
      (new TButton("All Chi2","((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->MaxChi=kFALSE;((EdbDisplay*)(gROOT->FindObject(\"EdbDisplay\")))->Refresh();",0.4,0.96,0.7,0.99))->Draw();
      fTXSlider = new TSlider("xslider","x",0.1,0.01,0.98,0.045);
      fTXSlider->SetObject(this);
      fTXSlider->SetRange(.5+cutTX0/2.,.5+cutTX1/2.);
      fTYSlider = new TSlider("yslider","y",0.01,0.1,0.045,0.98);
      fTYSlider->SetObject(this);
      fTYSlider->SetRange(.5+cutTY0/2.,.5+cutTY1/2.);

      colors = new TArrayI();
      DZs = new TArrayF();
      pats=new TClonesArray("EdbPattern",3,kTRUE);
      Npats=0;
      fAff=new EdbAffine2D(1,0,0,1,0,0);

}
//________________________________________________________________________
void EdbDisplay::SetAffine(Float_t a1, Float_t a2, Float_t a3, Float_t a4, Float_t ax, Float_t ay)
{
  fAff->Set(a1,a2,a3,a4,ax,ay);
}
//________________________________________________________________________
void EdbDisplay::Clears()
{

//  TObject* obj;
  TList* lst=fImagepad->GetListOfPrimitives();
  Int_t N=lst->GetSize();
  for(int e=0;e<N;e++) {lst->RemoveAt(e);}
//  TIter next(lst);
//  while (obj = next()) {lst->Remove(obj);};
//  fImagepad->GetListOfPrimitives()->Changed();
 //     fImagepad->GetListOfPrimitives()->Clear();
      fImagepad->cd();
//       TView* view = new TView(1);
     AppendPad();
 //    fImagepad->Draw();
//      view->SetRange(vx0,vy0,vz0,vx1,vy1,vz1);

/* fImagepad->Clear();
      fImagepad->cd();
     AppendPad();
*/
}
//________________________________________________________________________
void EdbDisplay::Refresh()
{
 Clears();
 EdbSegP* seg;
 Float_t x[2],y[2],z[2],DZ,x0,y0,z0;
 Float_t tx,ty;
 Int_t color;
 EdbPattern* pat;
 for(int j=0;j<Npats;j++){
  pat=(EdbPattern*)(pats->At(j));
  DZ=(*DZs)[j]/2.;
   color=(*colors)[j];
  for(int i=0; i<pat->GetN();i++){
   seg=pat->GetSegment(i);
   if(MaxChi && seg->Flag()==0) continue;
   tx=seg->TX();
   ty=seg->TY();
  if(tx>cutTX1) continue;
  if(tx<cutTX0) continue;
  if(ty>cutTY1) continue;
  if(ty<cutTY0) continue;
//  if((pat->ID()==2)&&!eTr_Co) continue;
 // if((pat->ID()==1)&&eTr_Co) continue;
   x0=seg->X();
   y0=seg->Y();
   z0=seg->Z();
//  if((*DZs)[j]<=0) DZ=(seg->W())/2.;
//   DZ=(seg->Volume())/2.;
   DZ=(seg->DZ())/2.;
   x[1]=x0+tx*DZ;
   y[1]=y0+ty*DZ;
   z[1]=z0+DZ;
   x[0]=x0-tx*DZ;
   y[0]=y0-ty*DZ;
   z[0]=z0-DZ;
   fImagepad->cd();
   pl= new TPolyLine3D(2,x,y,z);
   if(color>=0) pl->SetLineColor(color);
   else pl->SetLineColor(gStyle->GetColorPalette(int(46.*(1.-1.*seg->PID()/eNpieces))));
   pl->SetLineWidth(int(seg->W()));
   if(seg->Flag()==-1) pl->SetLineColor(kWhite);
   pl->Draw();
  }
 }
}
//________________________________________________________________________
void EdbDisplay::ExecuteEvent(Int_t event, Int_t px, Int_t py)
{
 if(px==0 && py==0 && event==kButton1Up){
  cutTX0=fTXSlider->GetMinimum()*2.-1.;
  cutTX1=fTXSlider->GetMaximum()*2.-1.;
  cutTY0=fTYSlider->GetMinimum()*2.-1.;
  cutTY1=fTYSlider->GetMaximum()*2.-1.;
  printf("Ang cuts: %f %f %f %f\n",cutTX0,cutTX1,cutTY0,cutTY1);
  Refresh();
 }
}

//________________________________________________________________________
void EdbDisplay::AddCouplesTree(TTree* tree,  Float_t Zoffs, Float_t tx0, Float_t tx1, Float_t ty0, Float_t ty1)
{
  printf("Reading tree..\n");
  EdbSegCouple    *cp = 0;
  EdbSegP         *s1 = 0;
  EdbSegP         *s2 = 0;
  EdbSegP         *s  = 0;
  Int_t           pid1,pid2;

  int nentr = int(tree->GetEntries());
  tree->SetBranchAddress("cp"  , &cp );
  tree->SetBranchAddress("s1." , &s1 );
  tree->SetBranchAddress("s2." , &s2 );
  tree->SetBranchAddress("s."  , &s  );
  tree->SetBranchAddress("pid1"  , &pid1  );
  tree->SetBranchAddress("pid2"  , &pid2  );

  int nseg = 0;
    EdbPattern* alisu      = new EdbPattern(0.,0.,0.);
    EdbPattern* alisd      = new EdbPattern(0.,0.,0.);
    EdbPattern* alic      =  new EdbPattern(0.,0.,0.);

  for(int i=0; i<nentr; i++ ) {
    tree->GetEntry(i);
    if(s->X()>cutX1) continue;
    if(s->X()<cutX0) continue;
    if(s->Y()>cutY1) continue;
    if(s->Y()<cutY0) continue;

    if(s->TX()<tx0) continue;
    if(s->TX()>tx1) continue;
    if(s->TY()<ty0) continue;
    if(s->TY()>ty1) continue;
//    s->SetZ(plate[1]/2.+Zoffs);
    s->SetZ(s->Z()+Zoffs);
    s1->SetZ(s1->Z()+Zoffs);
    s2->SetZ(s2->Z()+Zoffs);
//    s->SetW(fabs(s2->Z()-s1->Z()));
    s->SetW(1);
    s1->SetW(2);
    s2->SetW(2);
    if(cp->N1()==1 && cp->N2()==1) s->SetFlag(1); else s->SetFlag(0);
    if(cp->N1()==1 && cp->N2()==1) s1->SetFlag(1); else s1->SetFlag(0);
    if(cp->N1()==1 && cp->N2()==1) s2->SetFlag(1); else s2->SetFlag(0);
//    s1->SetFlag(1);
//    s2->SetFlag(1);
//    if(s->Flag()==1) s->SetFlag(gStyle->GetColorPalette(pid2*5+1));
     s->SetDZ(fabs(s1->Z()-s2->Z())-fabs(s1->DZ()));
//     s->SetVolume(fabs(s1->Z()-s2->Z())-plate[2]);
//     s1->SetVolume(plate[0]);
//     s2->SetVolume(plate[2]);
    alic->AddSegment( *s );
    alisu->AddSegment( *s1 );
    alisd->AddSegment( *s2 );
    nseg++;
  }

//     alic->ProjectTo(-plate[1]/2.-plate[0]/2.);
//     AddPattern(alisu,kRed,plate[0]);
//     AddPattern(alisd,kBlue,plate[2]);
     AddPattern(alic,-1);


}


//________________________________________________________________________
void EdbDisplay::AddTracksTree(TTree* tree, Float_t Zoffs, Float_t tx0, Float_t tx1, Float_t ty0, Float_t ty1)
{
  printf("Reading tree..\n");
  EdbSegP         *t = 0;
  EdbSegP         *s  = 0;
  EdbSegP         *so  = 0;
  EdbSegP         *sc  = 0;
  TClonesArray   *as = 0;
  Int_t           nseg;
  
  MaxChi=kFALSE;
  int nentr = int(tree->GetEntries());
  tree->SetBranchAddress("t" , &t );
  tree->SetBranchAddress("s"  , &as  );
  tree->SetBranchAddress("nseg"  , &nseg  );

  int ntrcks = 0;
    EdbPattern* alic      =  new EdbPattern(0.,0.,0.);
    EdbPattern* alicon      =  new EdbPattern(0.,0.,0.);
    EdbPattern* alit      =  new EdbPattern(0.,0.,0.);
//  Float_t candZ;
  for(int i=0; i<nentr; i++ ) {
    tree->GetEntry(i);
//    printf("Entry %d nseg %d\n",i,nseg);
   if(nseg<eNsegmin) continue;
   if(nseg>eNsegmax) continue;
   if(t->X()>cutX1) continue;
    if(t->X()<cutX0) continue;
    if(t->Y()>cutY1) continue;
    if(t->Y()<cutY0) continue;

    if(t->TX()<tx0) continue;
    if(t->TX()>tx1) continue;
    if(t->TY()<ty0) continue;
    if(t->TY()>ty1) continue;
    t->SetZ(t->Z()+Zoffs);
   for(int ci=0;ci<as->GetEntries();ci++)
    {
    s=(EdbSegP*)(as->At(ci));
    s->SetZ(s->Z()+Zoffs);
//    candZ=2.*fabs(s->Z()-t->Z());
//    if(candZ>t->W()) t->SetW(candZ);
//    s->SetVolume(plate[0]); //seg length for display
    s->SetW(2); //width for display
    alic->AddSegment( *s );
    //Draw connecting line
    if(ci>0) so=(EdbSegP*)(as->At(ci-1)); else so=s;
    sc=(EdbSegP*)s->Clone();
    sc->SetZ((s->Z()+so->Z())/2.);
    sc->SetX((s->X()+so->X())/2.);
    sc->SetY((s->Y()+so->Y())/2.);
    sc->SetTX((s->X()-so->X())/(s->Z()-so->Z()));
    sc->SetTY((s->Y()-so->Y())/(s->Z()-so->Z()));
//    sc->SetVolume(fabs(s->Z()-so->Z())-plate[0]);
    sc->SetDZ(fabs(s->Z()-so->Z())-fabs(s->DZ()));
    sc->SetW(1);
    alicon->AddSegment( *sc );
    if(sc->PID()>eNpieces) eNpieces=sc->PID();
    }
    alit->AddSegment( *t );
     ntrcks++;

  }
//     alic->SetID(1);
//     alit->SetID(2);
//     AddPattern(alit,kYellow,-1);
     AddPattern(alic,-1);
     AddPattern(alicon,-1);
     printf("%d tracks to display.\n",ntrcks);


}


//________________________________________________________________________
void EdbDisplay::AddPattern(EdbPattern* pat, Int_t color)
{
 Npats++;
 DZs->Set(Npats);
 colors->Set(Npats);
// (*DZs)[Npats-1]=DZ;
 (*colors)[Npats-1]=color;
 printf("Adding pattern.\n");
 new ((*pats)[Npats-1]) EdbPattern(0,0,0);
 for(int s=0;s<pat->N();s++){
    ((EdbPattern*)((*pats)[Npats-1]))->AddSegment(*(pat->GetSegment(s)));
 }
 ((EdbPattern*)((*pats)[Npats-1]))->Transform(fAff);
}

//________________________________________________________________________
void EdbDisplay::AddPatternsVolume(EdbPatternsVolume* pat, Int_t colorU, Int_t colorD)
{
 printf("Adding pattern volume.\n");
 EdbPattern* ptt;
 for(int i=0;i<pat->Npatterns();i++) {
   ptt=pat->GetPattern(i);
   if(ptt->ID()==1) AddPattern(ptt,colorU);
   else                AddPattern(ptt,colorD);
 }
}
