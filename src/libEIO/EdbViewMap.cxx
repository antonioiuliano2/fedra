//-- Author :  Valeri Tioukov   05/03/2015

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewMap                                                          //
//                                                                      //
// view coord corrections                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbViewMap.h"
#include "EdbLog.h"
#include "TEventList.h"
#include "TCut.h"

using namespace TMath;

ClassImp(EdbViewMap);

//-----------------------------------------------------------------------
EdbViewMap::EdbViewMap()
{
  eNcp  =0;
  eALcp =0;
  eNvh  =0;
  eAHT  =0;
  eAlgorithm=0;
  eHX0odd=0;
  eHX0even=0;
  eHY0odd=0;
  eHY0even=0;
  eHX1odd=0;
  eHX1even=0;
  eHY1odd=0;
  eHY1even=0;
}

//-----------------------------------------------------------------------
EdbViewMap::~EdbViewMap()
{
  if(eALcp) delete[] eALcp;
}

//-----------------------------------------------------------------------
void EdbViewMap::InitAR()
{
  int narea=0;
  for(int i=0; i<eNvh; i++) 
  { 
    EdbViewHeader *vh = GetHeader(i);
    if( vh) if(vh->GetAreaID()>narea) narea=vh->GetAreaID();
  }
  narea++;
  for(int iside=0; iside<2; iside++) {
    eAR.side[iside].narea = narea;
    eAR.side[iside].area = new AArea*[narea];
    for(int iarea=0; iarea<narea; iarea++) {
      eAR.side[iside].area[iarea] = new AArea;
      eAR.side[iside].area[iarea]->nview=0;
      //eAR.side[iside].area[iarea]->nalv=0;
    }
  }
  printf("%d areas\n", narea);
  
  for(int i=0; i<eNvh; i++) 
  { 
    EdbViewHeader *vh = GetHeader(i);
    //eAR.side[ViewSide(*vh)].area[vh->GetAreaID()]->nview++;
    if( eAR.side[ViewSide(*vh)].area[vh->GetAreaID()]->nview < vh->GetViewID())
      eAR.side[ViewSide(*vh)].area[vh->GetAreaID()]->nview = vh->GetViewID();
  }

  for(int iside=0; iside<2; iside++)
    for(int iarea=0; iarea<narea; iarea++) 
    {
      eAR.side[iside].area[iarea]->nview += (eNpvh +1);     // keep space for pinned views at the end of the buffer
      eAR.side[iside].area[iarea]->ilast = 0;
      printf( "%d %d %d\n", iside, iarea, eAR.side[iside].area[iarea]->nview );
      eAR.side[iside].area[iarea]->view = new AView*[eAR.side[iside].area[iarea]->nview];
      for( int iv=0; iv<eAR.side[iside].area[iarea]->nview; iv++ ) 
      {
        AView *v = eAR.side[iside].area[iarea]->view[iv] = new AView;
        v->header=0;
        v->quality=1;
        v->nalv=0;
        v->isCorrected=0;
      }
    }
}

//-----------------------------------------------------------------------
void EdbViewMap::FillAR()
{
  for(int i=0; i<eNvh; i++) 
  {
    EdbViewHeader *vh = GetHeader(i);
    int side=ViewSide(*vh);
    int area=vh->GetAreaID();
    int view = vh->GetViewID();
    //printf("%d %d (%d) %d (%d)\n",side,area, eAR.side[side].narea ,view, eAR.side[side].area[area]->nview );
    eAR.side[side].area[area]->view[view]->header=vh;
    if( view > eAR.side[side].area[area]->ilast) eAR.side[side].area[area]->ilast=view;
  }
  Log(2,"EdbViewMap::FillARun","with %d headers",eNvh );
  
  for(int i=0; i<eNpvh; i++)   //pinned
  {
    EdbViewHeader *vh = GetPinHeader(i);
    int side=ViewSide(*vh);
    int area=vh->GetAreaID();
    int oldid = vh->GetViewID();
    int newid = (eAR.side[side].area[area]->ilast++);
    printf("oldid %d newid %d\n",oldid,newid);
    eAR.side[side].area[area]->view[newid]->header=vh;
    eAR.side[side].area[area]->view[ newid ]->isCorrected=true;
    eAR.side[side].area[area]->view[ newid ]->quality=1;
    vh->SetViewID(newid);                         // update header with new id
    for(int ia=0; ia<eNcp; ia++) 
    {
      AlignmentParView *apv = eALcp[ia];
      if( apv->side1==side && apv->area1==area  ) 
      {
        if( apv->view1==oldid) apv->view1=newid;   // update APV vith newid
        if( apv->view2==oldid) apv->view2=newid;
      }
    }
  }
  Log(2,"EdbViewMap::FillARun","with %d pinned headers",eNpvh );
}

//-----------------------------------------------------------------------
void EdbViewMap::ReadViewsHeaders(const char *file, TCut &cut)
{
  // read headers from runfile, fill eViewHeaders
  EdbRun r(file);
  EdbView    *view = r.GetView();
  r.GetTree()->Draw(">>lst", cut );
  TEventList *lst = (TEventList*)(gDirectory->GetList()->FindObject("lst"));
  if(!lst) {Log(1,"EdbViewMap::ReadViewsHeaders","ERROR!: events list (lst) did not found!"); return;}
  int n = lst->GetN();
  for(int i=0; i<n; i++)
  {
    view = r.GetEntry(lst->GetEntry(i),1,0,0,0);
    if(view) 
      if(view->GetHeader()) 
        eViewHeaders.Add( new EdbViewHeader( *(view->GetHeader()) ) );
  }
  eNvh = n;
  r.Close();
  Log(2,"EdbViewMap::ReadViewsHeaders","Read %d headers",eNvh);
}

//-----------------------------------------------------------------------
void EdbViewMap::ReadPinViewsHeaders(const char *file)
{
  EdbRun r(file);
  TTree *t = r.GetPinViews();
  if(t) {
    EdbViewHeader *h=new EdbViewHeader();
    t->SetBranchAddress("headers",&h);
    int n = t->GetEntries();
    for(int i=0; i<n; i++)
    {
      t->GetEntry(i);
      if(h) ePinViewHeaders.Add( new EdbViewHeader( *h ) );
    }
    eNpvh = n;
  }
  r.Close();
  Log(2,"EdbViewMap::ReadPinViewsHeaders","Read %d headers",eNpvh);
}

//-----------------------------------------------------------------------
bool EdbViewMap::IsBug0( AView &v1, AView &v2 )
{
  // protection for bug in LASSO 11/06/2015
  if( Abs( v1.header->GetXview() - v2.header->GetXview()) > 1000. || 
     Abs( v1.header->GetYview() - v2.header->GetYview()) > 1000. ) return 1;
  return 0;
}

//-----------------------------------------------------------------------
void EdbViewMap::ReadViewAlign(const char *file)
{
  eNcp=0;
  TFile f(file);
  TTree *t = (TTree*)(f.Get("ViewAlign"));
  AlignmentParView      v;
  t->SetBranchAddress("alpar",&v);
  int n = t->GetEntries();
  eALcp  = new AlignmentParView*[2*n];  // doubled space for inverse align
  for(int i=0; i<n; i++) 
  {
    t->GetEntry(i);
    eALcp[i] = new AlignmentParView(v);
    //eNcp++;
  }
  eNcp=n;
  f.Close();
  Log(2,"EdbViewMap::ReadViewAlign","Read %d aligned views couples",eNcp);
}
//-----------------------------------------------------------------------
void EdbViewMap::AddInverseAlign()
{
  for(int i=0; i<eNcp; i++)
  {
    AlignmentParView *v=eALcp[i];
    AlignmentParView *vi = eALcp[i+eNcp] = new AlignmentParView(*v);
    vi->view1 = v->view2;
    vi->area1 = v->area2;
    vi->side1 = v->side2;
    vi->view2 = v->view1;
    vi->area2 = v->area1;
    vi->side2 = v->side1;
    vi->dx = -v->dx;
    vi->dy = -v->dy;
    vi->dz = -v->dz;
  }
  eNcp *= 2;
}

//-----------------------------------------------------------------------
void EdbViewMap::CheckViewFrameAlignQuality(const char *file)
{
  // tag view as bad if there are notaligned frames inside
  TFile f(file);
  TTree *t = (TTree*)(f.Get("FrameAlign"));
  AlignmentParFrame      fr;
  t->SetBranchAddress("frpar",&fr);
  int n = t->GetEntries();
  printf("%d entries in FrameAlign \n",n);
  int view=-1, area=-1, side=-1;
  int changeflag=0;
  int flag=0;
  for(int i=0; i<n; i++)
  {
    t->GetEntry(i);
    if( fr.view != view || area != fr.area || side != fr.side) {   // new view started here
      if(changeflag>2) GetAView(side,area,view)->quality /= 100;
      view = fr.view; area = fr.area; side = fr.side;
      flag = 0;
      changeflag=0;
    }
    if(fr.flag!=flag) { flag=fr.flag; changeflag++; }
  }
  if(changeflag>2) GetAView(side,area,view)->quality /= 100;

  f.Close();
  Log(2,"EdbViewMap::CheckViewFrameAlignQuality","Read %d frame alignments",n);
}

//-----------------------------------------------------------------------
AView *EdbViewMap::GetAView(EdbViewHeader &h) 
{ 
  return GetAView( ViewSide(h), h.GetAreaID(), h.GetViewID() ); 
}
AView *EdbViewMap::GetAView(int side, int area, int view)
{
  if( side<0||side>1) return 0;
  if( area<0||area>=eAR.side[side].narea) return 0;
  if( view<0||view>=eAR.side[side].area[area]->nview) return 0;
  return eAR.side[side].area[area]->view[view];
}

//-----------------------------------------------------------------------
void EdbViewMap::FillALcp()
{
  for(int i=0; i<eNcp; i++) {
    AlignmentParView *apv = eALcp[i];
    if(!apv) continue;
    AView *v1 = GetAView( apv->side1, apv->area1, apv->view1 );
    if(!v1) continue;
    AView *v2 = GetAView( apv->side2, apv->area2, apv->view2 );
    if(!v2) continue;
    
    float quality=1;
    if( IsBug0(*v1,*v2) ) continue;
    //printf("%d %d %d -   %d %d %d\n",apv->side1, apv->area1, apv->view1, apv->side2, apv->area2, apv->view2);

    if(v1) {
      if(v1->nalv>15) { Log(1,"EdbViewMap::FillALcp","ERROR: nalv out of limits: %d",v1->nalv); continue;}
      v1->alv[v1->nalv] = new AApv;
      v1->alv[v1->nalv]->apv = apv;
      v1->alv[v1->nalv]->quality=quality;
      v1->nalv++;
    }
    /*
    if(v2) {
      if(v2->nalv>15) { Log(1,"EdbViewMap::FillALcp","ERROR: nalv out of limits: %d",v2->nalv); continue;}
      v2->alv[v2->nalv] = new AApv;
      v2->alv[v2->nalv]->apv = apv;
      v2->alv[v2->nalv]->quality=quality;
      v2->nalv++;
    }
    */
  }
  Log(3,"FillALcp","filled with %d cp",eNcp);
}

//-----------------------------------------------------------------------
float EdbViewMap::Quality(AApv &apv)
{// the bigger the better
  
  float q = 0;
  if(apv.apv->view1>=0&&apv.apv->view2>=0)
  {
    AView *v1 = GetAView( apv.apv->side1, apv.apv->area1, apv.apv->view1 );
    AView *v2 = GetAView( apv.apv->side2, apv.apv->area2, apv.apv->view2 );
    if(v1&&v2)    q = apv.apv->nsg * apv.quality * v1->quality * v2->quality;
  }
  return q;
}

//-----------------------------------------------------------------------
void EdbViewMap::Transform( AView &v, const AlignmentParView &apv )
{
  v.aff.ShiftX(apv.dx);
  v.aff.ShiftY(apv.dy);
}

//-----------------------------------------------------------------------
void EdbViewMap::CheckView(int side, int area, int idview)
{
  AView *v = eAR.side[side].area[area]->view[idview];
  CheckView(v);
}
//-----------------------------------------------------------------------
void EdbViewMap::CheckView(AView *v)
{
  v->header->Print();
  float corr = ViewCorrectability(*v);
  printf("%d corrected: %d nalv= %d correctability: %f \n", v->header->GetViewID(), v->isCorrected, v->nalv, corr);
  
  for(int i=0; i<v->nalv; i++) 
  {
    AlignmentParView  *apv = v->alv[i]->apv;
    if(apv->view1== v->header->GetViewID()|| apv->view2==v->header->GetViewID())
      printf("%d %d %d\n",apv->view1,apv->view2,apv->nsg);
  }
}

//-----------------------------------------------------------------------
void EdbViewMap::DoCorrectionBestNeib( AView &v )
{
  // bestNeigbours algorithm
  // convention for apv: view1+offset => view2
  Log(3,"DoCorrectionBestNeib","%d alv",v.nalv);
  AView *vcbest=0;
  AApv *apv0=0;
  float wmax=0;
  for( int inb=0; inb < v.nalv; inb++ )
    {
      AApv *apv = v.alv[inb];
      AView *vc=GetAView( apv->apv->side2, apv->apv->area2, apv->apv->view2);
      //printf("i=%d\n",inb);
      if(!vc) continue;
      if(vc->isCorrected)
      {
        float w=Quality(*apv);
        //printf("w=%f\n",w);
        if( w > wmax) { wmax=w; vcbest=vc; apv0=apv;}
      }
    }
    if(vcbest&&apv0) {
      v.aff.Transform( vcbest->aff );

      Transform( v, *(apv0->apv) );
      //printf( "%d %d %d ", apv0->apv->side1, apv0->apv->area1, v.header->GetViewID()); v.aff.Print();
      float w=Quality(*apv0);
      v.isCorrected=true;
      FillAHT( apv0->apv, v.header, vcbest->header, &(v.aff), w, eAHT, "AHT" );
    }
}

//-----------------------------------------------------------------------
void EdbViewMap::DoCorrectionAllNeib( AView &v )
{
  // allNeigbours algorithm
  // convention for apv: view1+offset => view2
  
  // TODO: insert sigma approach
  
  int ncorr=0;
  EdbAffine2D aff[10];   // corrections obtained from different neighbours
  float weight[10];
  for( int inb=0; inb < v.nalv; inb++ )
  {
    AApv *apv = v.alv[inb];
    AView *vc=GetAView( apv->apv->side2, apv->apv->area2, apv->apv->view2);
    if(!vc) continue;
    if(vc->isCorrected)
    {
      aff[ncorr].Set( vc->aff );
      aff[ncorr].ShiftX(apv->apv->dx);
      aff[ncorr].ShiftY(apv->apv->dy);
      weight[ncorr] = Quality(*apv);
      //printf("%5d %5d %9.1f ",apv->apv->view1, apv->apv->view2, weight[ncorr]); aff[ncorr].Print();
      FillAHT( apv->apv, v.header, vc->header, &(aff[ncorr]), weight[ncorr], eAHT, "AHT" );
      ncorr++;
    }
  }
    
  float xmean=0, ymean=0;
  float wmean=0;
  for( int i=0; i < ncorr; i++ )
  {
    xmean += aff[i].B1()*weight[i];
    ymean += aff[i].B2()*weight[i];
    wmean += weight[i];
  }
  xmean /= wmean;
  ymean /= wmean;
  v.aff.ShiftX(xmean);
  v.aff.ShiftY(ymean);
  v.isCorrected=true;
}

//-----------------------------------------------------------------------
float EdbViewMap::ViewCorrectability(AView &v)
{
  if     (eAlgorithm==0) return ViewCorrectability1(v);
  else if(eAlgorithm==1) return ViewCorrectabilityAll(v);
  return 0;
}

//-----------------------------------------------------------------------
float EdbViewMap::ViewCorrectability1(AView &v)
{
  float correctability=0;
  if(!v.isCorrected) 
  {
    for(int ia=0; ia<v.nalv; ia++ ) 
    {
      AApv *apv=v.alv[ia];
      if(!apv) Log(1,"EdbViewMap::ViewCorrectability1","ERROR! apv = zero pointer");
      if(apv->apv->view2<0)  continue;
      AView *v = GetAView( apv->apv->side2, apv->apv->area2, apv->apv->view2);
      if(!v) { Log(1,"EdbViewMap::ViewCorrectability","ERROR! v(%d,%d,%d) = zero pointer",apv->apv->side2, apv->apv->area2, apv->apv->view2); continue;}
      if( v->isCorrected )  if(Quality(*apv)>correctability) correctability=Quality(*apv);
    }
  }
  return correctability;
}

//-----------------------------------------------------------------------
float EdbViewMap::ViewCorrectabilityAll(AView &v)
{
  float correctability=0;
  if(!v.isCorrected) 
  {
    for(int ia=0; ia<v.nalv; ia++ ) 
    {
      AApv *apv=v.alv[ia];
      if(!apv) Log(1,"EdbViewMap::ViewCorrectabilityAll","ERROR! apv = zero pointer");
      if(apv->apv->view2<0)  continue;
      AView *v = GetAView( apv->apv->side2, apv->apv->area2, apv->apv->view2);
      if(!v) { Log(1,"EdbViewMap::ViewCorrectability","ERROR! v(%d,%d,%d) = zero pointer",apv->apv->side2, apv->apv->area2, apv->apv->view2); continue;}
      if( v->isCorrected )  correctability+=Quality(*apv);
    }
  }
  return correctability;
}

//-----------------------------------------------------------------------
AView *EdbViewMap::SelectBestNotCorrected(AArea &area)
{ 
  AView *vbest=0;
  float maxcorr=0;
  for(int i=0; i<area.nview; i++)
  {
    AView *v=area.view[i];
    if(!v) Log(1,"EdbViewMap::SelectBestNotCorrected","ERROR; zero pointer");
    float corr = ViewCorrectability(*v);
    if( corr > maxcorr) 
    {
      //printf("%d nalv=%d correatability=%f\n",i, v->nalv, corr);
      vbest=v;
      maxcorr = corr;
    }
  
  }
  return vbest;
}

//-----------------------------------------------------------------------
void EdbViewMap::DoCorrection1(AArea &area)
{
  Log(3,"DoCorrection","area with %d views, alg=%d",area.nview,eAlgorithm);
  for(int i=0; i<area.nview; i++) 
  {
    AView *v = SelectBestNotCorrected(area);
    if(v) 
    {
      if     ( eAlgorithm==0 )   DoCorrectionBestNeib(*v);
      else if( eAlgorithm==1 )   DoCorrectionAllNeib(*v);
    }
  }
  Log(3,"DoCorrection","ok");
}

//-----------------------------------------------------------------------
void EdbViewMap::DoCorrection()
{
  for( int iside=0; iside<2; iside++)
    for( int iarea=0; iarea<eAR.side[iside].narea; iarea++)
        DoCorrection1( *(eAR.side[iside].area[iarea]) );
}

//-----------------------------------------------------------------------
void EdbViewMap::CorrectToStage()
{
  //for each area eliminate scaling&rotation in respect to the stage coord
  for( int side=0; side<2; side++)
    for( int area=0; area<eAR.side[side].narea; area++)
    {
      AArea *a= eAR.side[side].area[area];
      TArrayF xst(a->nview);
      TArrayF yst(a->nview);
      TArrayF x(a->nview);
      TArrayF y(a->nview);
      int cnt=0;
      for(int i=0; i<a->nview; i++)
      {
         AView *v = GetAView(side,area,i);
         if(!v) continue;
         if(!v->header) continue;
         xst[cnt]= v->header->GetXview();
         yst[cnt]= v->header->GetYview();
         v->aff.ShiftX( xst[cnt] );
         v->aff.ShiftY( yst[cnt] );
         x[cnt]  = v->aff.B1();
         y[cnt]  = v->aff.B2();
         cnt++;
      }
      EdbAffine2D aff;
      aff.Calculate(cnt,x.GetArray(),y.GetArray(),xst.GetArray(),yst.GetArray());
      aff.Print();
      for(int i=0; i<a->nview; i++)
      {
        if(GetAView(side,area,i)) GetAView(side,area,i)->aff.Transform(aff);
      }
    }
}

//-----------------------------------------------------------------------
void EdbViewMap::ConvertRun(const char *fin, const char *fout)
{
  EdbRun rin(fin);
  EdbView *v = rin.GetView();
  EdbRun rout(fout,"RECREATE");

  int n=rin.GetEntries();
  printf("entries=%d\n",n);

  for(int i=0; i<n; i++) 
  {
    rin.GetEntry(i,1,0,1,1,1);
    //rin.GetEntry(i,1,0,0,0,0);

    EdbViewHeader *h = v->GetHeader();
    AView *av=GetAView(*h);
    if(av) {
      EdbAffine2D a( av->aff );
      h->SetAffine( a.A11(),a.A12(),a.A21(),a.A22(),a.B1(),a.B2() );
    }
    rout.AddView(v);
  }
  
  if(eAHT) eAHT->Write();
  
  //FillAHTapv();
  //if(eAHTapv) eAHTapv->Write();
  
  rout.Close();
  //rin.Close();
}

//-----------------------------------------------------------------------
void EdbViewMap::SaveCorrToRun(const char *fin)
{
  EdbRun rin(fin);
  EdbView *v = rin.GetView();
  int n=rin.GetEntries();
  printf("entries=%d\n",n);
  TObjArray viewcorr(n);
  
  for(int i=0; i<n; i++) 
  {
    rin.GetEntry(i,1,0,0,0,0);
    EdbViewHeader *h = v->GetHeader();
    viewcorr.Add( new EdbAffine2D(GetAView(*h)->aff) );
  }
  int k = strlen(fin);
  TString s(fin,k-4); s+="va.root";
  TFile  rout( s.Data(),"RECREATE" );
  
  viewcorr.Write("viewcorr",1);
  if(eAHT)     eAHT->Write();
  rout.Close();
  //rin.Close();
}

//-----------------------------------------------------------------------
void EdbViewMap::FillAHT( AlignmentParView *apv, EdbViewHeader *vn, EdbViewHeader *vc, EdbAffine2D *aff, float w, TTree *&aht, const char *name )
{
  if(!aht) {
    aht=new TTree( name , "Aligned Headers");
    aht->Branch("alpar",apv,"view1/I:view2/I:area1/I:area2/I:side1/I:side2/I:dx/F:dy/F:dz/F:n1tot/I:n2tot/I:n1/I:n2/I:nsg/I:nbg/I:flag/I");
    aht->Branch("vn",vn);
    aht->Branch("vc",vc);
    aht->Branch("aff",aff);
    aht->Branch("w",&w,"w/F");
  } else {
    aht->SetBranchAddress("alpar",&(apv->view1) );
    aht->SetBranchAddress("vn",&vn);
    aht->SetBranchAddress("vc",&vc);
    aht->SetBranchAddress("aff",&aff);
    aht->SetBranchAddress("w",&w);
  }
  aht->Fill();
}

//-----------------------------------------------------------------------
EdbViewHeader *EdbViewMap::GetViewHeader(AlignmentParView &apv, int fs)
{
  EdbViewHeader *h=0;
  int side=-1;
  int vid =-1;
  int area = -1;
  if(fs==1) {
    side = apv.side1;
    area = apv.area1;
    vid  = apv.view1;
  } else if(fs==2) {
    side = apv.side2;
    area = apv.area2;
    vid  = apv.view2;
  }
  else return h;
  
  for(int i=0; i<eNvh; i++)
  {
    h = GetHeader(i);
    if(ViewSide(*h) == side)
      if( h->GetViewID()==vid )
        if(h->GetAreaID()==area)
          return h;
  }
  return 0;
}

//-----------------------------------------------------------------------
void EdbViewMap::MakeAHTnocorr()
{
  TTree *tree=0;
  EdbAffine2D *aff = new EdbAffine2D();
  for(int i=0; i<eNcp; i++)
  {
    AlignmentParView *apv=eALcp[i];
    EdbViewHeader *v1 = GetViewHeader(*apv,1);
    EdbViewHeader *v2 = GetViewHeader(*apv,2);
    if(v1&&v2)  FillAHT(apv, v1, v2, aff, 0, tree, "AHTnocorr");
  }

  if     (eAlgorithm==2) CorrectLines( *tree );
  else if(eAlgorithm==3) CorrectCols( *tree );
  
  TFile  rout( "aht.root","RECREATE" );
  tree->Write();
  rout.Close();

}

//-----------------------------------------------------------------------
void EdbViewMap::CorrectLines(TTree &tree)
{
  tree.SetAlias("xm","(vc.eXview+vn.eXview)/2.");
  tree.SetAlias("ym","(vc.eYview+vn.eYview)/2.");

  TCut cutx("cutx","flag!=0 && abs(vn.eXview-vc.eXview)<100");
  TCut cuty("cuty","flag!=0 && abs(vn.eYview-vc.eYview)<100");

  tree.Draw( "dx:ym:xm>>hx0even(30,0,120000,20,0,100000)", "!(vn.eRow%2)&&side1==0" && cutx ,"prof colz");
  tree.Draw( "dx:ym:xm>>hx0odd(30,0,120000,20,0,100000)",  "(vn.eRow%2)&&side1==0" && cutx ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy0even(30,0,120000,20,0,100000)", "!(vn.eRow%2)&&side1==0" && cuty ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy0odd(30,0,120000,20,0,100000)",  "(vn.eRow%2)&&side1==0" && cuty ,"prof colz");
  tree.Draw( "dx:ym:xm>>hx1even(30,0,120000,20,0,100000)", "!(vn.eRow%2)&&side1==1" && cutx ,"prof colz");
  tree.Draw( "dx:ym:xm>>hx1odd(30,0,120000,20,0,100000)",  "(vn.eRow%2)&&side1==1" && cutx ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy1even(30,0,120000,20,0,100000)", "!(vn.eRow%2)&&side1==1" && cuty ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy1odd(30,0,120000,20,0,100000)",  "(vn.eRow%2)&&side1==1" && cuty ,"prof colz");
  eHX0odd  = (TH2D*)((TH2D*)gDirectory->Get("hx0odd"))->Clone("X0odd");   // positive
  eHX0even = (TH2D*)((TH2D*)gDirectory->Get("hx0even"))->Clone("X0even"); // negative
  eHY0odd  = (TH2D*)((TH2D*)gDirectory->Get("hy0odd"))->Clone("Y0odd");   // positive
  eHY0even = (TH2D*)((TH2D*)gDirectory->Get("hy0even"))->Clone("Y0even"); // negative
  eHX1odd  = (TH2D*)((TH2D*)gDirectory->Get("hx1odd"))->Clone("X1odd");   // positive
  eHX1even = (TH2D*)((TH2D*)gDirectory->Get("hx1even"))->Clone("X1even"); // negative
  eHY1odd  = (TH2D*)((TH2D*)gDirectory->Get("hy1odd"))->Clone("Y1odd");   // positive
  eHY1even = (TH2D*)((TH2D*)gDirectory->Get("hy1even"))->Clone("Y1even"); // negative

  /*
  printf("eHXodd integral: %f eHXeven integral: %f  \n",
         eHXodd->Integral(),
         eHXeven->Integral());
  TH2D *hxdiff = (TH2D *)(eHXodd->Clone("hxdiff"));
  hxdiff->Add(eHXeven, 1);
  printf("hdiff integral: %f  \n",hxdiff->Integral());
  printf("eHYodd integral: %f eHYeven integral: %f  \n",
         eHYodd->Integral(),
         eHYeven->Integral());
  TH2D *hydiff = (TH2D *)(eHYodd->Clone("hydiff"));
  hydiff->Add(eHYeven, 1);
  printf("hydiff integral: %f  \n",hydiff->Integral());
  */
}

//-----------------------------------------------------------------------
void EdbViewMap::CorrectCols(TTree &tree)
{
  tree.SetAlias("xm","(vc.eXview+vn.eXview)/2.");
  tree.SetAlias("ym","(vc.eYview+vn.eYview)/2.");

  TCut cutx("cutx","flag!=0 && abs(vn.eXview-vc.eXview)<100");
  TCut cuty("cuty","flag!=0 && abs(vn.eYview-vc.eYview)<100");

  tree.Draw( "dx:ym:xm>>hx0even(30,0,120000,20,0,100000)", "!(vn.eCol%2)&&side1==0" && cutx ,"prof colz");
  tree.Draw( "dx:ym:xm>>hx0odd(30,0,120000,20,0,100000)",  "(vn.eCol%2)&&side1==0" && cutx ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy0even(30,0,120000,20,0,100000)", "!(vn.eCol%2)&&side1==0" && cuty ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy0odd(30,0,120000,20,0,100000)",  "(vn.eCol%2)&&side1==0" && cuty ,"prof colz");
  tree.Draw( "dx:ym:xm>>hx1even(30,0,120000,20,0,100000)", "!(vn.eCol%2)&&side1==1" && cutx ,"prof colz");
  tree.Draw( "dx:ym:xm>>hx1odd(30,0,120000,20,0,100000)",  "(vn.eCol%2)&&side1==1" && cutx ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy1even(30,0,120000,20,0,100000)", "!(vn.eCol%2)&&side1==1" && cuty ,"prof colz");
  tree.Draw( "dy:ym:xm>>hy1odd(30,0,120000,20,0,100000)",  "(vn.eCol%2)&&side1==1" && cuty ,"prof colz");
  eHX0odd  = (TH2D*)((TH2D*)gDirectory->Get("hx0odd"))->Clone("X0odd");   // positive
  eHX0even = (TH2D*)((TH2D*)gDirectory->Get("hx0even"))->Clone("X0even"); // negative
  eHY0odd  = (TH2D*)((TH2D*)gDirectory->Get("hy0odd"))->Clone("Y0odd");   // positive
  eHY0even = (TH2D*)((TH2D*)gDirectory->Get("hy0even"))->Clone("Y0even"); // negative
  eHX1odd  = (TH2D*)((TH2D*)gDirectory->Get("hx1odd"))->Clone("X1odd");   // positive
  eHX1even = (TH2D*)((TH2D*)gDirectory->Get("hx1even"))->Clone("X1even"); // negative
  eHY1odd  = (TH2D*)((TH2D*)gDirectory->Get("hy1odd"))->Clone("Y1odd");   // positive
  eHY1even = (TH2D*)((TH2D*)gDirectory->Get("hy1even"))->Clone("Y1even"); // negative
}

//-----------------------------------------------------------------------
void EdbViewMap::SaveLinesCorrToRun(const char *fin)
{
  EdbRun rin(fin);
  EdbView *v = rin.GetView();
  int n=rin.GetEntries();
  printf("entries=%d\n",n);
  TObjArray viewcorr(n);
  
  for(int i=0; i<n; i++) 
  {
    rin.GetEntry(i,1,0,0,0,0);
    EdbViewHeader *h = v->GetHeader();
    float x = h->GetXview();
    float y = h->GetYview();
    EdbAffine2D *aff = new EdbAffine2D(*h->GetAffine());
    
    if(h->GetRow()%2) //odd
    {
      if(ViewSide(*h)) {
        aff->ShiftX( 0.5*eHX1odd->Interpolate(x,y) );
        aff->ShiftY( -0.5*eHY1odd->Interpolate(x,y) );
      }
      else {
        aff->ShiftX(  0.5*eHX0odd->Interpolate(x,y) );
        aff->ShiftY(  -0.5*eHY0odd->Interpolate(x,y) );
      }
    }
    else if(!(h->GetRow()%2)) //even
    {
      if(ViewSide(*h)) {
        aff->ShiftX( 0.5*eHX1even->Interpolate(x,y) );
        aff->ShiftY( -0.5*eHY1even->Interpolate(x,y) );
      }
      else {
        aff->ShiftX( 0.5*eHX0even->Interpolate(x,y) );
        aff->ShiftY( -0.5*eHY0even->Interpolate(x,y) );
      }
    }
    viewcorr.Add( new EdbAffine2D(*aff) );
  }
  int k = strlen(fin);
  TString s(fin,k-4); s+="va.root";
  TFile  rout( s.Data(),"RECREATE" );
  
  viewcorr.Write("viewcorr",1);
  if(eAHT)     eAHT->Write();
  eHX0odd->Write();
  eHY0odd->Write();
  eHX0even->Write();
  eHY0even->Write();
  eHX1odd->Write();
  eHY1odd->Write();
  eHX1even->Write();
  eHY1even->Write();
  rout.Close();
  //rin.Close();
}

//-----------------------------------------------------------------------
void EdbViewMap::SaveColsCorrToRun(const char *fin)
{
  EdbRun rin(fin);
  EdbView *v = rin.GetView();
  int n=rin.GetEntries();
  printf("entries=%d\n",n);
  TObjArray viewcorr(n);
  
  for(int i=0; i<n; i++) 
  {
    rin.GetEntry(i,1,0,0,0,0);
    EdbViewHeader *h = v->GetHeader();
    float x = h->GetXview();
    float y = h->GetYview();
    EdbAffine2D *aff = new EdbAffine2D(*h->GetAffine());
    
    if(h->GetCol()%2) //odd
    {
      if(ViewSide(*h)) {
        aff->ShiftX( 0.5*eHX1odd->Interpolate(x,y) );
        aff->ShiftY( -0.5*eHY1odd->Interpolate(x,y) );
      }
      else {
        aff->ShiftX(  0.5*eHX0odd->Interpolate(x,y) );
        aff->ShiftY(  -0.5*eHY0odd->Interpolate(x,y) );
      }
    }
    else if(!(h->GetCol()%2)) //even
    {
      if(ViewSide(*h)) {
        aff->ShiftX( 0.5*eHX1even->Interpolate(x,y) );
        aff->ShiftY( -0.5*eHY1even->Interpolate(x,y) );
      }
      else {
        aff->ShiftX( 0.5*eHX0even->Interpolate(x,y) );
        aff->ShiftY( -0.5*eHY0even->Interpolate(x,y) );
      }
    }
    viewcorr.Add( new EdbAffine2D(*aff) );
  }
  int k = strlen(fin);
  TString s(fin,k-4); s+="va.root";
  TFile  rout( s.Data(),"RECREATE" );
  
  viewcorr.Write("viewcorr",1);
  if(eAHT)     eAHT->Write();
  eHX0odd->Write();
  eHY0odd->Write();
  eHX0even->Write();
  eHY0even->Write();
  eHX1odd->Write();
  eHY1odd->Write();
  eHX1even->Write();
  eHY1even->Write();
  rout.Close();
  //rin.Close();
}

//-----------------------------------------------------------------------
void EdbViewMap::t(const char *fin, const char *fout, int algorithm, const char *c )
{
  TCut cut("cut",c);
  EdbViewMap vm;
  vm.ReadViewsHeaders(fin, cut);    // read headers from runfile, fill eViewHeaders
  vm.ReadPinViewsHeaders(fin); // read headers from runfile, fill ePinViewHeaders
  vm.ReadViewAlign(fin);       // read ViewAlign from runfile, fill eALcp
  vm.eAlgorithm=algorithm;
  
  if(algorithm==2) {
    vm.MakeAHTnocorr();          // create tree with alignment parameters and views headers
    vm.SaveLinesCorrToRun(fin);   // correct hysteresis only
  }
  else if(algorithm==3) {
    vm.MakeAHTnocorr();          // create tree with alignment parameters and views headers
    vm.SaveColsCorrToRun(fin);   // correct hysteresis only (colomns)
  }
  else {
    vm.AddInverseAlign();
  
    vm.InitAR();               // init eAR structure
    vm.FillAR();               // fill eAR structure (assumed that view entry==h.GetViewID())
    vm.CheckViewFrameAlignQuality(fin);
  
    vm.FillALcp();                        //fill neighbouring
  //vm.CheckView(0,0,225);
  //vm.CheckView(0,0,112);
  
    vm.DoCorrection();
    vm.CorrectToStage();
  //vm.ConvertRun(fin,fout);
    vm.SaveCorrToRun(fin);
  }
}
