//-- Author :  Valeri Tioukov   15.11.2004

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunAccess                                                         //
//                                                                      //
// OPERA Run Access helper class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbRunAccess.h"
#include "TSystem.h"

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

  for(int i=0; i<3; i++) if(eLayers[i]==0) GetMakeLayer(i);

  if(FillVP()<1) return false;
  
  PrintStat();

  eXstep=350;  //TODO
  eYstep=350;
  return true;
}

///______________________________________________________________________________
EdbLayer *EdbRunAccess::GetMakeLayer(int id)
{
  if(id<0) return 0;
  if(id>2) return 0;
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
  int maxA=5000;            //TODO
  TArrayI entr1(maxA);
  int n1 = GetViewsArea( 1, entr1, area );
  TArrayI entr2(maxA);
  int n2 = GetViewsAreaMarg( 2, entr2, area, eXstep+eXstep/5.,eYstep+eYstep/5. );

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

  printf("Area:%3d (%3d\%%)  views:%4d    %6d +%6d segments are accepted; %6d - rejected\n",
  	 area, 100*area/eNareas, ic, 
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
int EdbRunAccess::GetViewsArea(int ud, TArrayI &entr, int area)
{
  // ud: 0-base, 1-down, 2-up         //TODO - to check this convention!

  int nv=0;
  if(ud>-1) 
    if(ud<3) {
      EdbPattern *pat=GetVP(ud);
      if(pat) 
	for( int i=0; i<pat->N(); i++ ) 
	  if( pat->GetSegment(i)->Aid(0) == area ) 
	    entr[nv++]=pat->GetSegment(i)->ID();
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
  if(var*eNareas != nviews) printf("WARNING: areas are not equal\n");

  for(int ud=0; ud<3; ud++) {
    EdbPattern *pat=GetVP(ud);
    if(pat)
      printf( "side:%d  xmin,xmax  = %f %f   ymin,ymax = %f %f\n", 
	      ud,
	      pat->Xmin(), 
	      pat->Xmax(),
	      pat->Ymin(),
	      pat->Ymax()
	      );
  }
  printf("\n");
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
  for(int iv=0; iv<nentr; iv++ ) {
    view = eRun->GetEntry(iv,1,0,0,0,0);

    if(eAFID) {
      segP.Set( iv,0,0, 0,0,0,0);
      segP.Transform( head->GetAffine() );
    } else {
      segP.Set( iv,view->GetXview(),view->GetYview(), 0,0,0,0);
    }

    if( view->GetAreaID() < eFirstArea )  eFirstArea= view->GetAreaID();
    if( view->GetAreaID() > eLastArea )   eLastArea = view->GetAreaID();

    segP.SetAid( view->GetAreaID(), view->GetViewID() );

    side = ViewSide(view);
    if(side<1||side>2) continue;
    eVP[side]->AddSegment(segP);
  }

  eVP[1]->Transform(eLayers[1]->GetAffineXY());  //TODO a nujno-li eto zdes'?
  eVP[2]->Transform(eLayers[2]->GetAffineXY());

  return nentr;
}
