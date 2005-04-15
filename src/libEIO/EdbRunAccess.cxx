//-- Author :  Valeri Tioukov   15.11.2004

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunAccess                                                         //
//                                                                      //
// OPERA Run Access helper class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "Riostream.h"
#include "TSystem.h"
#include "EdbMath.h"
#include "EdbSegment.h"
#include "EdbCluster.h"
#include "EdbRunAccess.h"

ClassImp(EdbRunAccess)

///_________________________________________________________________________
EdbRunAccess::EdbRunAccess()
{
  Set0();
}

///_________________________________________________________________________
EdbRunAccess::EdbRunAccess(const char *fname)
{
  Set0();
  eRunFileName=fname;
}

///_________________________________________________________________________
EdbRunAccess::EdbRunAccess(EdbRun *r)
{
  Set0();
  eRun=r;
}

///_________________________________________________________________________
void EdbRunAccess::Set0()
{
  eRun=0;
  for(int i=0; i<3; i++) {
    eVP[i]=0;
    eLayers[i]=0;
  }
  eAFID=0;
  eCLUST=0;
  eFirstArea = 9999999;
  eLastArea  = 0;
  eNareas=0;
  eXmin=eXmax=eYmin=eYmax=0;
}

///_________________________________________________________________________
bool EdbRunAccess::InitRun()
{
  if(!eRun)
    if( gSystem->AccessPathName(eRunFileName.Data(), kFileExists) ) {
      printf("ERROR open file: %s\n",eRunFileName.Data());
      return false;
    }
  eRun=new EdbRun(eRunFileName.Data());
  if(!eRun) return false;
  if(!(eRun->GetTree())) return false;
  for(int i=0; i<3; i++) if(eLayers[i]==0) GetMakeLayer(i);

  if(FillVP()<1) return false;
  
  //PrintStat();

  eXstep=400;  //TODO
  eYstep=400;
  return true;
}

///______________________________________________________________________________
EdbLayer *EdbRunAccess::GetMakeLayer(int id)
{
  if(id<0) return 0;  if(id>2) return 0;
  if(!GetLayer(id))  eLayers[id] = new EdbLayer();
  return GetLayer(id);
}

///_________________________________________________________________________
int EdbRunAccess::GetViewsXY(int ud, TArrayI &entr, float x, float y)
{
  return 0;
}

///_________________________________________________________________________
int EdbRunAccess::GetVolumeArea(EdbPatternsVolume &vol, int area)
{
  //

  int maxA=2000;            //TODO
  TArrayI entr1(maxA);
  float xmin,xmax,ymin,ymax;
  int n1 = GetViewsArea( 1, entr1, area, xmin,xmax,ymin,ymax);
  TArrayI entr2(maxA);
  xmin -= eXstep+eXstep/10.;
  xmax += eXstep+eXstep/10.;
  ymin -= eYstep+eYstep/10.;
  ymax += eYstep+eYstep/10.;
  int n2 = GetViewsArea( 2, entr2, xmin,xmax,ymin,ymax );
  //  int n2 = GetViewsAreaMarg( 2, entr2, area, eXstep+eXstep/10.,eYstep+eYstep/10. );

  //printf("n1=%d n2=%d\n",n1,n2);

  TArrayI eall(n1+n2);
  int ic=0;
  for(int i=0; i<n1; i++)    eall[ic++]=entr1[i];
  for(int i=0; i<n2; i++)    eall[ic++]=entr2[i];

  TArrayI ind(ic);
  TArrayI srt(ic);
  TMath::Sort(ic,eall.GetArray(),ind.GetArray(),0);
  for (int i=0; i<ic; i++)    srt[i] = eall[ind[i]];

  int nrej=-1;
  int nseg = GetVolumeData(vol, ic, srt, nrej);

  printf("Area:%3d (%3d\%%)  views:%4d/%4d   %6d +%6d segments are accepted; %6d - rejected\n",
  	 area, 100*area/eNareas, n1,n2, 
	 vol.GetPattern(0)->N(),
	 vol.GetPattern(1)->N(),
	 nrej );
  if(nseg != vol.GetPattern(0)->N()+vol.GetPattern(1)->N()) 
    printf("WARNING!!! nseg\n");

  return ic;
}

///_________________________________________________________________________
int EdbRunAccess::GetVolumeData(EdbPatternsVolume &vol, 
				int nviews, 
				TArrayI &srt, 
				int &nrej)
{
  // get raw segments as a volume for the given "Area"

  EdbSegP    segP;
  EdbView    *view = eRun->GetView();
  int   side;
  int   nseg=0;
  nrej=0;
  int   entry;
  int   nsegV;
  
  for(int iu=0; iu<nviews; iu++) {
    entry = srt[iu];
    if(eCLUST)       {
      view = eRun->GetEntry(entry,1,1,1);
      view->AttachClustersToSegments();
    }
    else view = eRun->GetEntry(entry);

    nsegV = view->Nsegments();
    side = ViewSide(view);

    if(side<1||side>2) continue;

    for(int j=0;j<nsegV;j++) {
      if(!AcceptRawSegment(view,j,segP,side)) {
	nrej++;
	continue;
      }
      nseg++;
      segP.SetVid(entry,j);
      segP.SetAid(view->GetAreaID(),view->GetViewID());

      vol.GetPattern(side-1)->AddSegment( segP);
    }
  }

  //printf("nseg: %d \t rejected: %d\n", nseg, nrej );

  vol.GetPattern(0)->SetSegmentsZ();
  vol.GetPattern(1)->SetSegmentsZ();
  return nseg;
}

///_________________________________________________________________________
int EdbRunAccess::GetViewsArea(int ud, TArrayI &entr, int area,
			       float &xmin, float &xmax, float &ymin, float &ymax )
{
  // get all views of "area" for the side "ud"; fill entr with the entries, 
  // calculate area limits

  int nv=0;
  if(ud<0) return nv;  if(ud>2) return nv;

  EdbPattern *pat=GetVP(ud);
  if(!pat)                             return nv;
  xmin=eXmax;  xmax=eXmin;
  ymin=eYmax;  ymax=eYmin;
  EdbSegP *seg=0;
  for( int i=0; i<pat->N(); i++ ) {
    seg = pat->GetSegment(i);
    if( seg->Aid(0) != area ) continue;
    if(xmin>seg->X()) xmin=seg->X();
    if(xmax<seg->X()) xmax=seg->X();
    if(ymin>seg->Y()) ymin=seg->Y();
    if(ymax<seg->Y()) ymax=seg->Y();
    entr[nv++]=seg->ID();
  }  
  return nv;
}

///_________________________________________________________________________
int EdbRunAccess::GetViewsArea(int ud, TArrayI &entr, 
			       float xmin, float xmax, float ymin, float ymax )
{
  // get all views in a given limits

  int nv=0;
  if(ud<0) return nv;  if(ud>2) return nv;

  EdbPattern *pat=GetVP(ud);
  if(!pat)                             return nv;
  EdbSegP *seg=0;
  for( int i=0; i<pat->N(); i++ ) {
    seg = pat->GetSegment(i);
    if(seg->X()<xmin)     continue;
    if(seg->X()>xmax)     continue;
    if(seg->Y()<ymin)     continue;
    if(seg->Y()>ymax)     continue;
    entr[nv++]=seg->ID();
  }
  return nv;
}

///_________________________________________________________________________
int EdbRunAccess::GetViewsAreaMarg(int ud, TArrayI &entr, int area, 
				   float xmarg, float ymarg)
{
  // return area views with margins
  // ud: 0-base, 1-down, 2-up         //TODO - to check this convention!

  int nv=0;
  if(ud<0 || ud>2) return nv;
  EdbPattern *pat=GetVP(ud);
  if(!pat)  return nv;

  EdbPattern ptmp;
  EdbSegP *seg=0;
  int N=pat->N();
  for( int i=0; i<N; i++ ) {
    seg = pat->GetSegment(i);
    if( seg->Aid(0) == area )  ptmp.AddSegment( *seg );
  }
  
  float xmin = ptmp.Xmin();
  float xmax = ptmp.Xmax();
  float ymin = ptmp.Ymin();
  float ymax = ptmp.Ymax();

  for( int i=0; i<N; i++ ) {
    seg = pat->GetSegment(i);
    if( seg->X() < xmin-xmarg ) continue;
    if( seg->X() > xmax+xmarg ) continue;
    if( seg->Y() < ymin-ymarg ) continue;
    if( seg->Y() > ymax+ymarg ) continue;
    entr[nv++] = seg->ID();
  }

  return nv;
}

///_________________________________________________________________________
void EdbRunAccess::PrintStat()
{
  if(!eRun) return;

  printf("\nSome run statistics:\n");
  printf(  "--------------------\n");
  int nviews=eRun->GetEntries();
  eNareas=eRun->GetHeader()->GetNareas();
  printf("entries    : %d\n", nviews );
  printf("areas      : %d   in the range: ( %d : %d )\n", 
	 eNareas, eFirstArea,eLastArea );
  int var=0;
  if(eNareas>0) {
    var =  nviews/eNareas;
    printf("views/area : %d  (%d/side)\n", var, var/2 );
  }
  if(var*eNareas != nviews) printf("\t WARNING: areas are not equal\n");

  for(int ud=0; ud<3; ud++) {
    EdbPattern *pat=GetVP(ud);
	int nempty=0;
	if(pat) {
      printf( "side:%d  xmin,xmax  = %f %f   ymin,ymax = %f %f\n", 
	      ud,
	      pat->Xmin(), 
	      pat->Xmax(),
	      pat->Ymin(),
	      pat->Ymax()
	      );
		nempty = CheckEmptyViews(*pat);
		if(nempty>0) printf("\t WARNING: %d empty views in layer %d\n",nempty,ud);
	}
  }
  printf("\n");
}

///_________________________________________________________________________
void EdbRunAccess::CheckRunLine()
{
  if(!eRun) return;

  cerr << endl << eRunFileName.Data();
  int nviews=eRun->GetEntries();
  cerr<<"\tv: "<< nviews;
  eNareas=0;
  if(eRun->GetHeader()) {
	eNareas =eRun->GetHeader()->GetNareas();
  } else 
	  cerr <<"  RUN HEADER is missing!";

  cerr<<"  a: "<<eNareas;
  int var=0;
  if(eNareas>0) {
    var =  nviews/eNareas/2;
	cerr<<"   v/a/s: "<<var;
  }
  cerr<<endl;

  if(eNareas>0) 
	  if(var*2*eNareas != nviews) {
		  cerr<<"\tWARNING: areas and views numbers are mismatching!\n";
		  for(int ud=0; ud<3; ud++) {
			EdbPattern *pat=GetVP(ud);
			if(pat) {
				int nv = pat->N();
				cerr<<"\t\t views("<<ud<<"): "<<nv;
				if(nv>0) {
					cerr<<"  Areas in range: ";
					cerr<<pat->GetSegment(0)->Aid(0)<<" : "<<pat->GetSegment(nv-1)->Aid(0);
					cerr<<"    Views in range: ";
					cerr<<pat->GetSegment(0)->Aid(1)<<" : "<<pat->GetSegment(nv-1)->Aid(1);
				}
				cerr<<endl;
			}
		  }
	  }

  for(int ud=0; ud<3; ud++) {
    EdbPattern *pat=GetVP(ud);
	int nempty=0;
	if(pat) {
 		nempty = CheckEmptyViews(*pat);
		if(nempty>0) cerr<<"\tWARNING: "<<nempty<<" empty views in layer "<<ud<<endl;
	}
  }
}

///_________________________________________________________________________
int EdbRunAccess::CheckEmptyViews(EdbPattern &pat)
{
	int nempty=0;
	EdbSegP *s=0;
	for(int i=0; i<pat.N(); i++) {
		s = pat.GetSegment(i);
		if( s->Vid(0)>0 )    continue;
		if( s->Vid(1)>0 )    continue;
		nempty++;
	}
	return nempty;
}

///_________________________________________________________________________
int EdbRunAccess::FillVP()
{
  // fill patterns up/down with dummy segments with center of view coordinates 
  // to profit of the pattern tools for the fast views selection
  // the convention is: 
  // segp->ID() - tree entry number
  // segp->X() - coordinates in the external (brick) RS (after transformation)
  // segp->Y() - /
  // segp->Aid(areaID,viewID);


  if(!eRun) { printf("ERROR: run is not opened\n"); return 0; }
  EdbView        *view = eRun->GetView();
  EdbViewHeader  *head = view->GetHeader();

  int nentr = eRun->GetEntries();
  if( eVP[1] ) delete  eVP[1];
  if( eVP[2] ) delete  eVP[2];
  eVP[1] = new EdbPattern( 0.,0.,   0., nentr);
  eVP[2] = new EdbPattern( 0.,0., 200., nentr);  //TODO

  EdbSegP segP;

  eFirstArea = 9999999;
  eLastArea  = 0;

  int side;
  int nseg,ncl;
  for(int iv=0; iv<nentr; iv++ ) {
    view = eRun->GetEntry(iv,1,0,0,0,0);

	nseg = head->GetNsegments();
	ncl  = head->GetNclusters();
    if(eAFID) {
      segP.Set( iv,0,0, 0,0,ncl,nseg);
      segP.Transform( head->GetAffine() );
    } else {
      segP.Set( iv,view->GetXview(),view->GetYview(), 0,0,ncl,nseg);
    }
    if( view->GetAreaID() < eFirstArea )  eFirstArea= view->GetAreaID();
    if( view->GetAreaID() > eLastArea )   eLastArea = view->GetAreaID();

    segP.SetAid( view->GetAreaID()    , view->GetViewID() );
	segP.SetVid( head->GetNframesTop(), head->GetNframesBot() );

    side = ViewSide(view);
    if(side<1||side>2) continue;
    eVP[side]->AddSegment(segP);
  }

  eVP[1]->Transform(eLayers[1]->GetAffineXY());  //TODO a nujno-li eto zdes'?
  eVP[2]->Transform(eLayers[2]->GetAffineXY());

  eXmin = eVP[1]->Xmin();
  eXmax = eVP[1]->Xmax();
  eYmin = eVP[1]->Ymin();
  eYmax = eVP[1]->Ymax();

  printf("fillVP: %d entries\n", nentr);
  return nentr;
}

///______________________________________________________________________________
bool EdbRunAccess::AcceptRawSegment(EdbView *view, int id, EdbSegP &segP, int side)
{
  EdbSegment *seg = view->GetSegment(id);

  if( !PassCuts(side,*seg) )     return false;

  float pix, chi2;
  if(eCLUST) {
    pix = GetRawSegmentPix(seg);
    segP.SetVolume( pix );
    chi2 = CalculateSegmentChi2( seg,
				 GetCond(1)->SigmaXgr(),  //TODO: side logic
				 GetCond(1)->SigmaYgr(), 
				 GetCond(1)->SigmaZgr());
    if(chi2>GetCutGR())  return false;
    segP.SetChi2( chi2 );
  }

  EdbLayer  *layer=GetLayer(side);
  if(eAFID) seg->Transform( view->GetHeader()->GetAffine() );

  float x,y,z,tx,ty,puls;
  tx   = seg->GetTx()/layer->Shr();
  ty   = seg->GetTy()/layer->Shr();
  x    = seg->GetX0() + layer->Zmin()*tx;
  y    = seg->GetY0() + layer->Zmin()*ty;
  z    = layer->Z() + layer->Zmin();
  if(eAFID==0) {
    x+=view->GetXview();
    y+=view->GetYview();
  }
  puls = seg->GetPuls();

  EdbAffine2D *aff = layer->GetAffineTXTY();
  float txx = aff->A11()*tx+aff->A12()*ty+aff->B1();
  float tyy = aff->A21()*tx+aff->A22()*ty+aff->B2();
  segP.Set( seg->GetID(),x,y,txx,tyy,puls,0);
  segP.SetZ( z );
  segP.SetDZ( seg->GetDz()*layer->Shr() );
  segP.SetW( puls );

  return true;
}

///______________________________________________________________________________
bool  EdbRunAccess::PassCuts(int id, EdbSegment &seg)
{
  float var[5];
  var[0] = seg.GetX0();
  var[1] = seg.GetY0();
  var[2] = seg.GetTx();
  var[3] = seg.GetTy();
  var[4] = seg.GetPuls();

  int nc = NCuts(id);
  for(int i=0; i<nc; i++)
    if( !(GetCut(id,i)->PassCut(var)) )  return false;
  return true;
}

///______________________________________________________________________________
float  EdbRunAccess::CalculateSegmentChi2( EdbSegment *seg, float sx, float sy, float sz )
{
  //assumed that clusters are attached to segments
  double chi2=0;
  EdbCluster *cl=0;
  TObjArray *clusters = seg->GetElements();
  if(!clusters) return 0;
  int ncl = clusters->GetLast()+1;
  if(ncl<=0)     return 0;

  float xyz1[3], xyz2[3];             // segment line parametrized as 2 points
  float xyz[3];
  bool inside=true;

  xyz1[0] = seg->GetX0() /sx;
  xyz1[1] = seg->GetY0() /sy;
  xyz1[2] = seg->GetZ0() /sz;
  xyz2[0] = (seg->GetX0() + seg->GetDz()*seg->GetTx()) /sx;
  xyz2[1] = (seg->GetY0() + seg->GetDz()*seg->GetTy()) /sy;
  xyz2[2] = (seg->GetZ0() + seg->GetDz())              /sz;

  double d;
  for(int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)clusters->At(i);
    xyz[0] = cl->GetX()/sx;
    xyz[1] = cl->GetY()/sy;
    xyz[2] = cl->GetZ()/sz;
    d = EdbMath::DistancePointLine3(xyz,xyz1,xyz2, inside);
    chi2 += d*d;
  }

  return TMath::Sqrt(chi2/ncl);
}

///______________________________________________________________________________
float  EdbRunAccess::GetRawSegmentPix( EdbSegment *seg )
{
  //assumed that clusters are attached to segments
  float pix=0;
  EdbCluster *cl=0;
  TObjArray *clusters = seg->GetElements();
  if(!clusters) return 0;
  int ncl = clusters->GetLast()+1;
  for(int i=0; i<ncl; i++ ) {
    cl = (EdbCluster*)clusters->At(i);
    pix += cl->GetArea();
  }
  return pix;
}

///______________________________________________________________________________
void EdbRunAccess::AddSegmentCut(int layer, int xi, float var[10])
{
  if(!eCuts[layer])  eCuts[layer] = new TObjArray();
  eCuts[layer]->Add( new EdbSegmentCut(xi,var) );
}

///______________________________________________________________________________
void EdbRunAccess::AddSegmentCut(int layer, int xi, float min[5], float max[5])
{
  if(!eCuts[layer])  eCuts[layer] = new TObjArray();
  EdbSegmentCut *cut=new EdbSegmentCut();
  cut->SetXI(xi);
  cut->SetMin(min);
  cut->SetMax(max);
  eCuts[layer]->Add( cut );
}

