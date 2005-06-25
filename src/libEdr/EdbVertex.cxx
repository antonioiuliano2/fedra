//-- Author :  Valeri Tioukov & Yury Petukhov  10.02.2004
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertex                                                            //
//                                                                      //
// Class for vertex reconstruction                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "TRandom.h"

#include "vt++/CMatrix.hh"
#include "vt++/VtVector.hh"
#include "vt++/VtDistance.hh"
#include "smatrix/Functions.hh"
#include "smatrix/SVector.hh"

#include "EdbVertex.h"
#include "EdbPVRec.h"
#include "EdbPhys.h"
#include "EdbMath.h"
#include "TIndexCell.h"

ClassImp(EdbVTA);
ClassImp(EdbVertex);
ClassImp(EdbVertexRec);

using namespace MATRIX;
using namespace VERTEX;

//______________________________________________________________________________
EdbVTA::EdbVTA()
{
  Set0();
}

//______________________________________________________________________________
EdbVTA::EdbVTA( EdbVTA& vta )
{
  eTrack=vta.GetTrack();
  eVertex=vta.GetVertex();
  eZpos=vta.Zpos();
  eFlag=vta.Flag();
  eImp=vta.Imp();
  eDist=vta.Dist();
}

//______________________________________________________________________________
EdbVTA::EdbVTA( EdbTrackP *tr, EdbVertex *v )
{
  Set0();
  eTrack=tr;
  eVertex=v;
}

//______________________________________________________________________________
void EdbVTA::Set0()
{
  eTrack=0;
  eVertex=0;
  eZpos=0;
  eFlag=0;
  eImp=0.;
  eDist=0.;
}
//________________________________________________________________________
EdbVTA::~EdbVTA()
{
  if(eTrack) { eTrack->ClearVTA(this); }
  if(eVertex && (eFlag == 2)) { (eVertex->VTa())->Remove(this); }
  if(eVertex && (eFlag < 2)) { (eVertex->VTn())->Remove(this); }
}
//______________________________________________________________________________
void EdbVTA::AddVandT()
{
    GetTrack()->AddVTA(this);
    GetVertex()->AddVTA(this);
}
//______________________________________________________________________________
EdbVertex::EdbVertex()
{
  eID= 0;
  eV = 0;
  eX = 0.;
  eY = 0.;
  eZ = 0.;
  eFlag = 0;
  eQuality=0.;
}

//________________________________________________________________________
EdbVertex::~EdbVertex()
{
  if(eV) { eV->clear(); delete eV; eV=0; }
  for(int i=0; i<N();  i++) delete GetVTa(i);
  for(int i=0; i<Nn(); i++) delete GetVTn(i);
}

//______________________________________________________________________________
void EdbVertex::ResetTracks()
{
  EdbVTA *vta = 0;
  for (int i=0; i<N(); i++)
  {
    vta = GetVTa(i);
    if (vta) GetTrack(i)->AddVTA(vta);
  }
}

//______________________________________________________________________________
float EdbVertex::MaxAperture()
{
  float aper=0.;
  int   ntr = N();
  if(ntr<2)                      return aper;
  EdbTrackP *t1=0;
  EdbTrackP *t2=0;

  float tx=0,ty=0,a=0;
  for (int i=0; i<ntr-1; i++) {
    t1 = GetTrack(i);
    for (int j=i+1; j<ntr; j++) {
      t2 = GetTrack(j);

      tx= t1->TX() - t2->TX();
      ty= t1->TY() - t2->TY();
      a = TMath::Sqrt( tx*tx+ty*ty );
      if( a>aper) aper=a;
    }
  }
  return aper;
}

//________________________________________________________________________
void EdbVertex::Clear()
{
  for(int i=0; i<N();  i++) delete GetVTa(i);
  for(int i=0; i<Nn(); i++) delete GetVTn(i);
  eVTa.Clear();
  eVTn.Clear();
  if(eV) { eV->clear(); delete eV; eV=0; }
  eX = 0.;
  eY = 0.;
  eZ = 0.;
  eID = 0;
  eFlag = 0;
  eQuality = 0.;
}
//________________________________________________________________________
void EdbVertex::ClearNeighborhood()
{
  for(int i=0; i<Nn(); i++) delete GetVTn(i);
  if (Nn()>0) eVTn.Clear();
}
//________________________________________________________________________
int EdbVertex::Compare(const TObject *o) const
{
    /*printf("Inside compare\n");*/
    if      ( eQuality >  ((EdbVertex *)o)->eQuality )  return -1;
    else if ( eQuality == ((EdbVertex *)o)->eQuality )  return  0;
    else	      	       				return  1;
}
//________________________________________________________________________
bool EdbVertex::IsEqual(const TObject *o) const
{
    /*printf("Inside isequal\n");*/
    if   ( eID      != ((EdbVertex *)o)->eID )      return false;
    if   ( eQuality != ((EdbVertex *)o)->eQuality ) return false;
    else      	             			    return true;
}
//________________________________________________________________________
void EdbVertex::Print()
{
      printf("****** Vertex Id %d, Flag %d, quality %g, %d connected tracks ********\n",
    	     ID(), Flag(), Quality(), N());
      EdbTrackP *tr=0;
      printf("IDs ");
      for(int i=0; i<N(); i++) {
	tr = GetTrack(i);
	if(tr)  printf(" %6d", tr->ID());
      }
      printf("\n");
      printf("Z   ");
      for(int i=0; i<N(); i++) {
	tr = GetTrack(i);
	if(tr)
	{
	    if (Zpos(i)) printf(" %6.0f", (tr->TrackZmin())->Z());
	    else         printf(" %6.0f", (tr->TrackZmax())->Z());
	}
      }
      printf("\n");
      printf("Zpos");
      for(int i=0; i<N(); i++) {
	tr = GetTrack(i);
	if(tr)
	{
	    printf(" %6d", Zpos(i));
	}
      }
      printf("\n**********************************************************\n");
}
//________________________________________________________________________
void EdbVertex::AddVTA(EdbVTA *vta)
{
  if((vta->Flag()==0) || (vta->Flag()==1)) eVTn.Add(vta);
  else if(vta->Flag()==2) eVTa.Add(vta);
}
//________________________________________________________________________
int EdbVertex::Nv()
{
    EdbTrackP *tr = 0;
    EdbVertex *vc = 0;
    int nv = 0;
    for (int i=0; i<N(); i++)
    {
	if ((tr = GetTrack(i)))
	{
	    if      ((Zpos(i) == 1) && (vc = tr->VertexE())) nv++;
	    else if ((Zpos(i) == 0) && (vc = tr->VertexS())) nv++;
	}
    }
    return nv;
}

//________________________________________________________________________
EdbVertex *EdbVertex::GetConnectedVertex(int nv)
{
    EdbTrackP *tr = 0;
    EdbVertex *vc = 0;
    int n = 0;
    for (int i=0; i<N(); i++)
    {
	if ((tr = GetTrack(i)))
	{
	    if      ((Zpos(i) == 1) && (vc = tr->VertexE()))
	    {
		if (n == nv) return vc;
		n++;
	    }
	    else if ((Zpos(i) == 0) && (vc = tr->VertexS()))
	    {
		if (n == nv) return vc;
		n++;
	    }
	}
    }
    return 0;
}
//________________________________________________________________________
float EdbVertex::Impact( int i)
{
    EdbVTA *vta = 0;
    float imp = 1000000.;
    if (i < 0 || i >= N())   return imp;
    if (!(vta = GetVTa(i)))  return imp;
    return vta->Imp();
}
//________________________________________________________________________
EdbVTA *EdbVertex::CheckImp( const EdbTrackP *tr , float ImpMax, int zpos, float dist)
{
    EdbVTA *vta = 0;
    EdbTrackP *tr1 = (EdbTrackP *)tr;
    if (!tr) return vta;
    Track *t = new Track();
    Vertex *v = this->V();
    if (Edb2Vt(*tr, *t))
    {
	float imp = distance(*t,*v);
	if (imp > ImpMax) { delete t; return vta;}
	vta = new EdbVTA(tr1, this);
	vta->SetZpos(zpos);
	vta->SetFlag(0);
	vta->SetImp(imp);
	vta->SetDist(dist);
	AddVTA(vta);
    }
    delete t;
    return vta;
}
//________________________________________________________________________
bool EdbVertex::EstimateVertexMath( float& xv, float& yv, float& zv, float& d )
{
  int nt = N(); 
  if(!nt) return false;

  double tx_sum  = 0.;
  double x_sum   = 0.;
  double xw_sum  = 0.;
  double xtx_sum = 0.;
  double tx2_sum = 0.;
  double ty_sum  = 0.;
  double y_sum   = 0.;
  double yw_sum  = 0.;
  double yty_sum = 0.;
  double ty2_sum = 0.;
  const EdbTrackP   *tr = 0;
  const EdbSegP   *seg = 0;

  double x,y,tx,ty,xweight,yweight,xweight2,yweight2;

  // fill cumulants
  for( int i = 0; i < nt; i++ ) {
    
    tr = GetTrack(i);

    if (Zpos(i)) seg = tr->TrackZmin();
    else         seg = tr->TrackZmax();

    x        = seg->X();
    y        = seg->Y();
    tx       = seg->TX();
    ty       = seg->TY();
    xweight  = 1./(seg->COV())(0,0);
    yweight  = 1./(seg->COV())(1,1);
    xweight2 = xweight*xweight;
    yweight2 = yweight*yweight;

    tx_sum  += tx * xweight;
    x_sum   += x  * xweight;
    xw_sum  += xweight;
    xtx_sum += x  * tx * xweight2;
    tx2_sum += tx * tx * xweight2;

    ty_sum  += ty * yweight;
    y_sum   += y  * yweight;
    yw_sum  += yweight;
    yty_sum += y  * ty * yweight2;
    ty2_sum += ty * ty * yweight2;

  } // for track

  double det = -tx2_sum - ty2_sum + tx_sum*tx_sum/xw_sum + ty_sum*ty_sum/yw_sum;

  if(det == 0.) {
    return false;
  }

  zv = ( xtx_sum + yty_sum - tx_sum*x_sum/xw_sum - ty_sum*y_sum/yw_sum ) / det;
  xv = ( x_sum + tx_sum * zv ) / xw_sum;
  yv = ( y_sum + ty_sum * zv ) / yw_sum;


//   float zTolerance=300.;

//   for( int i = 0; i < nt; i++ ) {
//       tr = GetTrack(i);
//       if (Zpos(i)) seg = tr->TrackZmin();
//       else	   seg = tr->TrackZmax();
//       if( zv > (seg->Z() + zTolerance) )  return false;
//       if( zv < (seg->Z() - zTolerance) )  return false;
//   }

  double drx;
  double dry;
  double drz;
  double drt;
  double drms = 0.;

  for( int i = 0; i < nt; i++ ) {

    tr = GetTrack(i);

    if (Zpos(i)) seg = tr->TrackZmin();
    else	 seg = tr->TrackZmax();
    
    drx = seg->X() - xv;
    dry = seg->Y() - yv;
    drz = seg->Z() - zv;
    drt = (drx*seg->TX() + dry*seg->TY() + drz);
    drms += drx*drx + dry*dry + drz*drz -
      (drt*drt)/(1.+seg->TX()*seg->TX()+seg->TY()*seg->TY());
  }

  d = TMath::Sqrt(drms/nt);

  return true;
}

//________________________________________________________________________
bool EdbVertex::Edb2Vt(const EdbTrackP& tr, Track& t)
{
  return Edb2Vt( *((EdbSegP*)&tr), t);
}

//________________________________________________________________________
bool EdbVertex::Edb2Vt(const EdbSegP& tr, Track& t)
{
    double x1 = (double)tr.X() - eX;
    double y1 = (double)tr.Y() - eY;
    double z1 = (double)tr.Z() - eZ;
    double tx1 = (double)tr.TX();
    double ty1 = (double)tr.TY();
    double p1 = (double)tr.P();

    CMatrix cov1;             // covariance matrix for seg0
    for(int k=0; k<5; k++) 
      for(int l=0; l<5; l++) cov1(k,l) = (tr.COV())(k,l);

    t.set(x1,  y1,  z1,  tx1,  ty1, p1, cov1);
    t.propagate(0.);
    return true;
}
//________________________________________________________________________
float EdbVertex::Chi2Track(int i)
{
  // distance from track to already existed vertex

  EdbVTA *vta = GetVTa(i);
  if (!vta) return 0.;
  EdbTrackP *track = vta->GetTrack();
  if (!track) return 0.;
  double distchi2 = 0.;
  int zpos = vta->Zpos();
  EdbSegP *seg = 0;
  if   (zpos)
    {
      seg = (EdbSegP *)(track->TrackZmin());
    }
  else
    {
      seg = (EdbSegP *)(track->TrackZmax());
    }
  if (eV)
    {
      if (track->NF() <= 0) return 0.;
      Track *t=new Track();
      if( Edb2Vt( *seg, *t ) )
	{
	  if (eV->valid())
	    {
	      distchi2 = eV->distance(*t);
	    } 
	}
      else
	{ 
	  printf("EdbVertex::DistTrack - conversion to VT failed!");
	}
      delete t;
      t=0;
    }
  return (float)distchi2;
}

//________________________________________________________________________
float EdbVertex::ImpTrack(int i)
{
  // distance from track to already existed vertex

  EdbVTA *vta = GetVTa(i);
  if (!vta) return 0.;
  EdbTrackP *track = vta->GetTrack();
  if (!track) return 0.;
  double dist = 0.;
  int zpos = vta->Zpos();
  EdbSegP *seg = 0;
  if   (zpos)
    {
      seg = (EdbSegP *)(track->TrackZmin());
    }
  else
    {
      seg = (EdbSegP *)(track->TrackZmax());
    }
  if (eV)
    {
      if (track->NF() <= 0) return 0.;
      Track *t=new Track();
      if( Edb2Vt( *seg, *t ) )
	{
	  if (eV->valid())
	    {
	      dist = distance(*eV, *t);
	    } 
	}
      else
	{ 
	  printf("EdbVertex::DistTrack - conversion to VT failed!");
	}
      delete t;
      t=0;
    }
  return (float)dist;
}
//========================================================================
EdbVertexRec::EdbVertexRec()
{
  eEdbTracks = 0;
  eVTX       = 0;
  ePVR       = 0;

  eZbin       = 100.;      // microns
  eAbin       = 0.01;      // rad
  eDZmax      = 3000.;     // microns
  eProbMin    = 0.01;      // i.e 1%
  eImpMax     = 25.;       // microns
  eUseMom     = false;     // do not use it
  eUseSegPar  = false;     // use fitted track parameters

  (gROOT->GetListOfSpecials())->Add(this);
}

//________________________________________________________________________
EdbVertexRec::~EdbVertexRec()
{
  if ((gROOT->GetListOfSpecials())->FindObject(this))
  {
    (gROOT->GetListOfSpecials())->Remove(this);
  }
}

//________________________________________________________________________
int EdbVertexRec::MakeV( EdbVertex &edbv )
{
  // create new VtVertex and add (two) tracks to this one

  Vertex *v=edbv.V();
  if(v) { v->clear(); delete v; v=0; }
  v = new Vertex();
  v->use_kalman(true);               //TODO: define as parameter
  v->use_momentum(eUseMom);
  EdbTrackP *tr=0;
  EdbSegP *seg=0;
  Track *ta[50];                       //TODO!
  for (int i=0; i<50; i++) ta[i] = 0;
  for (int i=0; i<edbv.N(); i++) {
    tr = edbv.GetTrack(i);
    if (tr->NF() <= 0) return 0;        //???
    if   (edbv.Zpos(i)) seg = (EdbSegP*)tr->TrackZmin(eUseSegPar);
    else  	        seg = (EdbSegP*)tr->TrackZmax(eUseSegPar);
    Track *t=new Track();
    if( edbv.Edb2Vt( *seg, *t ) )
    {
	t->rm((double)(tr->M()));
	v->push_back(*t);
    }
    else { 
      delete t; t=0;
    }
    if (i<50) ta[i] = t;
  }
  int retval = 0;
  if (v->ntracks() < 2) return retval;
  retval = v->findVertexVt();
  int n = edbv.N();
  if (!retval) return retval;
  if (!(v->valid())) return 0;
  if ( n > 50) n = 50;
  for (int i=0; i<n; i++) {
    if (ta[i]) edbv.GetVTa(i)->SetImp(distance(*ta[i],*v));
  }
  edbv.SetV(v);
  return retval;
}

//________________________________________________________________________
EdbVTA *EdbVertexRec::AddTrack( EdbVertex &edbv, EdbTrackP *track, int zpos )
{
  // add track to already existed vertex if prob > eProbMin
  // if vertex do not exist yet - calculate medium x,y,z

  if (!track) return 0;
  int ntr = edbv.N();

  for (int i=0; i<ntr; i++) if (track == edbv.GetTrack(i)) return 0;

  EdbVTA  *vta = 0;
  EdbSegP *seg = 0;
  if (zpos)   seg = (EdbSegP*)track->TrackZmin(eUseSegPar);
  else        seg = (EdbSegP*)track->TrackZmax(eUseSegPar);

  Vertex *v=edbv.V();
  if (v)
    {
      if (track->NF() <= 0) return 0;
      Track *t=new Track();
      if( edbv.Edb2Vt( *seg, *t ) )
	{
	  if (v->valid())
	    {
	      t->rm((double)(track->M()));
	      v->push_back(*t);
	      if (!(v->findVertexVt()))
		{
		  v->remove_last();
		  delete t;
		  t=0;
		  printf("EdbVertex::AddTrack - vertex not found!");
	          v->findVertexVt();
		}
	      else if (!(v->valid()))
		{
		  v->remove_last();
		  delete t;
		  t=0;
		  printf("EdbVertex::AddTrack - vertex not valid!");
	          v->findVertexVt();
		}
	      else if (v->prob() < eProbMin )
		{
		  v->remove_last();
		  delete t;
		  t=0;
	          v->findVertexVt();
		}
	      else
		{
		  vta = new EdbVTA::EdbVTA(track, &edbv);
		  vta->SetZpos(zpos);
	          vta->SetFlag(2);
		  vta->SetImp(distance(*t, *v));
		  for (int i=0; i<ntr; i++) edbv.GetVTa(i)->SetImp(edbv.ImpTrack(i));
	          edbv.AddVTA(vta);
		  if (eQualityMode == 0)
		    edbv.SetQuality( v->prob()/(v->vtx_cov_x()+v->vtx_cov_y()) );
		  else if (eQualityMode == 1)
		  {
		    double rms=(edbv.V())->rmsDistAngle();
		    if (rms != 0.) 
			edbv.SetQuality( (float)(1./rms) );
		    else
			edbv.SetQuality( 10.e+35 );
		  }
		  else
		  {
		    edbv.SetQuality( 1. );
		  }
		}
	    }
	}
      else
	{ 
	  delete t;
	  t=0;
	  printf("EdbVertex::AddTrack - conversion to VT failed!");
	}
    }
  else
    {
      vta = new EdbVTA(track, &edbv);
      vta->SetZpos(zpos);
      vta->SetFlag(2);
      edbv.AddVTA(vta);
      if (track->NF() <= 0) return vta;
      ntr++;
      edbv.SetXYZ( ((ntr-1)*edbv.X() + seg->X())/ntr, ((ntr-1)*edbv.Y() + seg->Y())/ntr, ((ntr-1)*edbv.Z() + seg->Z())/ntr );
      //printf("EdbVertex::AddTrack: x,y,z: %f %f %f\n", edbv.X(),edbv.Y(),edbv.Z());
    }
  return vta;
}


//______________________________________________________________________________
int EdbVertexRec::FindVertex()
{
  // Note: in this function is assumed that all tracks selections are already done
  // ProbMin - minimal probability for chi2-distance between tracks

  TIndexCell starts,ends;              // "ist:entry"   "iend:entry"
  FillTracksStartEnd( starts, ends );

  printf("-----Search 2-track vertexes----------------------------\n");

  int nvtx   = 0;

  printf(" End-Begin tracks combinations:\n");
  nvtx += LoopVertex(ends  , starts,  0, 1 );

  printf(" Begin-Begin tracks combinations:\n");
  nvtx += LoopVertex(starts, starts,  1, 1 );

  printf(" End-End tracks combinations:\n");
  nvtx += LoopVertex(ends  , ends,    0, 0 );

  int nvtxt = 0;
  if (eVTX) nvtxt = eVTX->GetEntries();
  
  for (int i = 0; i < nvtxt; i++)
  {
    ((EdbVertex *)(eVTX->At(i)))->SetID(i);
  }

  if (nvtxt) eVTX->Sort(nvtxt-1);

  for (int i = 0; i < nvtxt; i++)
  {
    ((EdbVertex *)(eVTX->At(i)))->SetID(i);
  }

  printf("--------------------------------------------------------\n");

  return nvtx;
}

//______________________________________________________________________________
void EdbVertexRec::FillTracksStartEnd(TIndexCell &starts, TIndexCell &ends )
{
  // fill tracks starts and ends lookup tables "z:entry"
  // inside sorted tracks: starts - minimal Z; ends - maximal Z

  EdbTrackP *tr=0;
  int ntr  = eEdbTracks->GetEntries();
  Long_t  v[2];

  for(int itr=0; itr<ntr; itr++)   {
    tr = (EdbTrackP*)(eEdbTracks->At(itr));
    if (tr->Flag() < 0) continue;
    v[0] = (Long_t)(tr->TrackZmin()->Z()/eZbin);
    v[1] = itr;
    starts.Add(2,v); 
    v[0] = (Long_t)(tr->TrackZmax()->Z()/eZbin);
    v[1] = itr;
    ends.Add(2,v);
  }
  starts.Sort();
  ends.Sort();
}

//______________________________________________________________________________
int EdbVertexRec::LoopVertex( TIndexCell &list1, TIndexCell &list2, 
			      int zpos1, int zpos2 )
{

  // zpos1  - the directoin flag for the first  track 1-start, 0-end
  // zpos2  - the direction flag for the second track
  // in cycles is assumed that members of list1 has z <= members of list2

  printf("  Selection: dZmax=%.0f Abin=%.3f ProbMin=%f zBin=%.0f usemom=%d\n",
         eDZmax, eAbin, eProbMin, eZbin, eUseMom);

  int nvtx     = 0; 
  int ncombin  = 0;
  int ncombinv = 0;
  int ncount   = 0;

  TIndexCell *c1=0,  *c2=0;
  EdbTrackP  *tr1=0, *tr2=0;
  EdbSegP    *s1=0,  *s2=0;
  int         itr1,  itr2;

  int   nz1 = list1.GetEntries();
  int   nz2 = list2.GetEntries();
  float z1,z2;
  float dz, dtx,dty,deltaZ, imp=-1;
  float isign=1.;

  int ntot = nz1*nz2;
  printf("  2-track vertexes search in progress... %3d%%", 0);

  for(int iz1=0; iz1<nz1; iz1++)   {           // first z-group
    c1 = list1.At(iz1);

    for(int iz2=0; iz2<nz2; iz2++)   {         // second z-group
      c2 = list2.At(iz2);

      z1 = c1->Value()*eZbin;
      z2 = c2->Value()*eZbin;
      if( z2 < z1 )           continue;
      if( z2-z1 > eDZmax )    break;

      ncount++;
      printf("\b\b\b\b%3d%%",(int)((double)ncount/double(ntot)*100.));
      
      int nc1=c1->GetEntries();
      int nc2=c2->GetEntries();

      for(int ic1=0; ic1<nc1; ic1++) {      // first z-group entries

	itr1 = c1->At(ic1)->Value();
	tr1  = (EdbTrackP*)((*eEdbTracks)[itr1]);
	if(!tr1)             continue;

	if(zpos1) s1 = (EdbSegP *)tr1->TrackZmin(eUseSegPar);             // start-
	else      s1 = (EdbSegP *)tr1->TrackZmax(eUseSegPar);             // end  -

	int ic2start=0;
	if(c1==c2) ic2start=ic1+1;

	for(int ic2=ic2start; ic2<nc2; ic2++) {    // second z-group entries
      	  ncombin++;

	  itr2 = c2->At(ic2)->Value();
	  if(itr2==itr1)       continue; 
	  tr2  = (EdbTrackP*)((*eEdbTracks)[itr2]);
	  if(!tr2)             continue;

	  if(zpos2) s2 = (EdbSegP *)tr2->TrackZmin(eUseSegPar);             // - start
	  else      s2 = (EdbSegP *)tr2->TrackZmax(eUseSegPar);             // - end

	  dz     = TMath::Abs(s2->Z()-s1->Z());
	  if( dz > eDZmax )                                       continue;

	  if(zpos1!=zpos2) {              // start-end,   end-start
	    deltaZ = (dz+eZbin);
	    isign = -1.;
	  } else {                        // start-start, end-end
            deltaZ = (eDZmax-dz/2.);
	    isign = +1.;
	  }

	  dtx = TMath::Abs(s2->TX() - isign*s1->TX())+eAbin;
	  if( TMath::Abs(s2->X()-s1->X()) > dtx*deltaZ )          continue;

	  dty = TMath::Abs(s2->TY() - isign*s1->TY())+eAbin;
	  if( TMath::Abs(s2->Y()-s1->Y()) > dty*deltaZ )          continue;

	  imp = CheckImpact( s1,s2, zpos1,zpos2 );
	  if(imp>eImpMax)                                         continue;

      	  ncombinv++;

	  //printf("imp = %f \n", imp);

	  if( !ProbVertex( tr1,tr2,zpos1,zpos2) )                 continue;


	  nvtx++;
	  
	}
      }
    }
  }

  printf("\b\b\b\b%3d%%\n",100);

  printf("  %6d pairs, %d vertexes, %d with Prob > %f\n",
	    ncombin, ncombinv, nvtx, eProbMin);

  return nvtx;
}


//______________________________________________________________________________
float EdbVertexRec::CheckImpact( EdbSegP *s1,   EdbSegP *s2,
				 int zpos1,     int zpos2 )
{
  float p1[3], p2[3];  // first line
  float p3[3], p4[3];  // second line
  float pa[3], pb[3];  // shortest line
  double mua, mub;

  float dz;

  p1[0] = s1->X();
  p1[1] = s1->Y();
  p1[2] = s1->Z();
  if(zpos1) dz = -eDZmax;
  else      dz =  eDZmax;
  p2[0] = s1->X() + dz*s1->TX();
  p2[1] = s1->Y() + dz*s1->TY();
  p2[2] = s1->Z() + dz;
  
  p3[0] = s2->X();
  p3[1] = s2->Y();
  p3[2] = s2->Z();
  if(zpos2) dz = -eDZmax;
  else      dz =  eDZmax;
  p4[0] = s2->X() + dz*s2->TX();
  p4[1] = s2->Y() + dz*s2->TY();
  p4[2] = s2->Z() + dz;

  if( !EdbMath::LineLineIntersect( p1, p2, p3, p4, pa, pb, mua, mub ) )   return 10e+10;
  return EdbMath::Magnitude3( pa, pb );
}

//______________________________________________________________________________
int EdbVertexRec::ProbVertex( EdbTrackP *tr1,   EdbTrackP *tr2,
			      int zpos1,     int zpos2 )
{
  if(!tr1) return 0;
  if(!tr2) return 0;

  EdbSegP *s1=0, *s2=0;
  if(zpos1) s1 = (EdbSegP *)tr1->TrackZmin(eUseSegPar);             // start-
  else      s1 = (EdbSegP *)tr1->TrackZmax(eUseSegPar);             // end  -
  if(zpos2) s2 = (EdbSegP *)tr2->TrackZmin(eUseSegPar);             // - start
  else      s2 = (EdbSegP *)tr2->TrackZmax(eUseSegPar);             // - end 

  EdbVTA    *vta1=0;
  EdbVTA    *vta2=0;
  EdbVertex *v2=0;  // temporary vertex for couples check
  float x, y, z, d, prob;
  float zvmin=0,zvmax=0; 

  v2 = new EdbVertex();

  if (!(vta1 = AddTrack( *v2, tr1, zpos1)))
    {
      delete v2;
      v2 = 0;
      return 0;
    }
  if (!(vta2 = AddTrack( *v2, tr2, zpos2)))
    {
      delete v2;
      v2 = 0;
      return 0;
    }

  //v2->EstimateVertexMath(x,y,z,d);                         //TODO?????
  //printf("xyz: %f %f %f \t d: %f\n",x,y,z,d);

  if(!(MakeV(*v2)))
    {
      delete vta1;
      delete vta2;
      delete v2;
      v2 = 0;
      return 0;
    }

  if( !(v2->V()) )
    {
      delete vta1;
      delete vta2;
      delete v2;
      v2 = 0;
      return 0;
    }

  if( !(v2->V()->valid()) )
    {
      delete vta1;
      delete vta2;
      delete v2;
      v2 = 0;
      return 0;
    }

  
  d = v2->V()->rmsDistAngle();
  x = v2->VX();
  y = v2->VY();
  z = v2->VZ();
  prob = v2->V()->prob();

  if (zpos1 == 0 && zpos2 == 1)            // ends & starts
    {
      zvmin = TMath::Min(s1->Z() ,s2->Z()) - 0.5*eZbin;
      zvmax = TMath::Max(s1->Z() ,s2->Z()) + 0.5*eZbin;
      if (z < zvmin || z > zvmax)
	{
	  delete vta1;
	  delete vta2;
	  delete v2;
	  v2 = 0;
	  return 0;
	}
      else
	{
	  v2->SetFlag(1);
	} 
    }
  else if (zpos1 == 1 && zpos2 == 1)        // starts & starts
    {
      zvmax = TMath::Min(s1->Z() ,s2->Z()) + eZbin;
      zvmin = TMath::Max(s1->Z() ,s2->Z()) - eDZmax;
      if (z > zvmax || z < zvmin)
	{
	  delete vta1;
	  delete vta2;
	  delete v2;
	  v2 = 0;
	  return 0;
	} 
      else
	{
	  v2->SetFlag(0);
	} 
    }
  else if (zpos1 == 0 && zpos2 == 0)        // ends & ends
    {
      zvmin = TMath::Max(s1->Z() ,s2->Z());
      zvmax = TMath::Min(s1->Z() ,s2->Z()) + eDZmax + eZbin;
      if (z < zvmin || z > zvmax )
	{
	  delete vta1;
	  delete vta2;
	  delete v2;
	  v2 = 0;
	  return 0;
	} 
      else
	{
	  v2->SetFlag(2);
	} 
    }
  
  //  printf("prob = %f , eProbmin= %f\n", prob, eProbMin);

  if(prob >= eProbMin) {

    if (eQualityMode == 0)
	v2->SetQuality(v2->V()->prob()/
			    (v2->V()->vtx_cov_x()+v2->V()->vtx_cov_y()));
    else if (eQualityMode == 1)
    {
	if (d != 0.) 
	    v2->SetQuality( 1./d );
	else
	    v2->SetQuality( 10.e+35 );
    }
    else
    {
	v2->SetQuality( 1. );
    }
    tr1->AddVTA(vta1);
    tr2->AddVTA(vta2);
    AddVTA(vta1);
    AddVTA(vta2);
    AddVertex(v2);
    
    return 1;
  }
  else {
    delete vta1;
    delete vta2;
    delete v2;
    v2 = 0;
  }
  return 0;
}

//______________________________________________________________________________
int EdbVertexRec::ProbVertexN()
{
  EdbVTA *vta=0, *vta1 = 0, *vta2 = 0;
  EdbVertex *edbv1 = 0;
  EdbVertex *edbv2 = 0;
  Vertex *v = 0;
  EdbTrackP *tr = 0;
  EdbTrackP *tr2 = 0;
  int zpos = 0, zpos1 = 0, zpos2 = 0;
  int nvtx = 0;
  int nadd = 0;
  int ncombin = 0;
  int ncombinv = 0;
  bool wasadded = false;
  
  if (eVTX)
  {
    nvtx = eVTX->GetEntries();
    for (int i=0; i<nvtx; i++)
    {
	edbv1 = (EdbVertex *)(eVTX->At(i));
	if (edbv1)
	{
	    if (edbv1->N() > 2)
	    {
		for (int j=0; j<edbv1->N(); j++) eVTA.Remove(edbv1->GetVTa(j));
		for (int j=0; j<edbv1->Nn(); j++) eVTA.Remove(edbv1->GetVTn(j));
		tr  = edbv1->GetTrack(0);
		tr2 = edbv1->GetTrack(1);
		zpos1 = edbv1->Zpos(0);
		zpos2 = edbv1->Zpos(1);
		edbv1->Clear();
		vta1 = AddTrack(*edbv1, tr, zpos1);
		vta2 = AddTrack(*edbv1, tr2, zpos2);
		MakeV(*edbv1);
		v = edbv1->V();
		v->findVertexVt();
		if (eQualityMode == 0)
		    edbv1->SetQuality( v->prob()/(v->vtx_cov_x()+v->vtx_cov_y()) );
		else if (eQualityMode == 1)
		{
		    double rms=v->rmsDistAngle();
		    if (rms != 0.) 
			edbv1->SetQuality( (float)(1./rms) );
		    else
			edbv1->SetQuality( 10.e+35 );
		}
		else
		{
		    edbv1->SetQuality( 1. );
		}
		tr->AddVTA(vta1);
		tr2->AddVTA(vta2);
		AddVTA(vta1);
		AddVTA(vta2);
	    }
	    else
	    {
		if (edbv1->Flag() < 0)
		{
		    zpos1 = edbv1->Zpos(0);
		    zpos2 = edbv1->Zpos(1);
		    zpos  = zpos1 + zpos2;
		    if ( zpos == 0 )
			edbv1->SetFlag(2);
		    if ( zpos == 1 )
			edbv1->SetFlag(1);
		    if ( zpos == 2 )
			edbv1->SetFlag(0);
		}
	    }
	}
    }
    edbv1 = 0;
    zpos  = 0;
  }
  else
  {
    return 0;
  }
  nvtx = eVTX->GetEntries();
  printf("-----Merge 2-track vertex pairs to N-track vertexes-----\n");
  printf("N-track vertexes search in progress... %3d%%", 0);

  int nprint = (int)(0.05*(double)nvtx);
  if (nprint <= 0) nprint = 1;

  for (int i1=0; (i1<nvtx); i1++)
    {
	wasadded = false;
  	edbv1 = (EdbVertex *)(eVTX->At(i1));
        if (!(i1%nprint))
	{
	    printf("\b\b\b\b%3d%%",(int)((double)i1/double(nvtx)*100.));
	}
	if (!edbv1) continue;
	if (edbv1->Flag() == -10) continue;
	int  nt1 = edbv1->N();
	bool exist = false;
	if (nt1 == 2)
	{
	    for (int ic1=0; ic1<nt1; ic1++)
	    {
		tr = edbv1->GetTrack(ic1);
		zpos = edbv1->Zpos(ic1);
		if (zpos)
		{
		    if (tr->VertexS())
		    {
			if (nt1 < tr->VertexS()->N())
			{
			    exist = true;
			    break;
			}
		    }
		}
		else
		{
		    if (tr->VertexE())
		    {
			if (nt1 <  tr->VertexE()->N())
			{
			    exist = true;
			    break;
			}
		    }
		}
	    }
	    if (exist)
	    {
		edbv1->SetFlag(-10);
		continue;
	    }
	}
	for (int i2=i1+1; (i2<nvtx); i2++)
	{
  		edbv2 = (EdbVertex *)(eVTX->At(i2));
		if (!edbv2) continue;
		if (edbv2->Flag() == -10) continue;
		if (edbv2->N() == 2)
		{
//		    printf(" v1 id %d, v2 id %d\n", edbv1->ID(), edbv2->ID()); 
		    nt1 = edbv1->N();
		    int nt2 = edbv2->N();
		    int it1=0;
		    int nomatch = 1;
		    while ( (it1<nt1) && nomatch )
		    {
		      int it2=0;
		      tr = edbv1->GetTrack(it1);
		      while ( (it2<nt2) && nomatch)
		      {
			if ((edbv2->GetTrack(it2) == tr) && 
			    (edbv1->Zpos(it1) == edbv2->Zpos(it2)))
			{
			    ncombin++;
			    if      (it2 == 0) 
			    {
				tr2 = edbv2->GetTrack(1);
				zpos = edbv2->Zpos(1);
			    }
			    else if (it2 == 1)
			    {
				tr2 = edbv2->GetTrack(0);
				zpos = edbv2->Zpos(0);
			    }

			    exist = false;
			    for (int ic1=0; ic1<edbv1->N(); ic1++)
			    {
			        if (tr2 == edbv1->GetTrack(ic1)) exist = true;
			    }

			    if (zpos)
			    {
				if (tr2->VertexS())
				{
				    if (tr2->VertexS()->N() > edbv1->N())
				    {
					exist = true;
				    }
				}
			    }
			    else
			    {
				if (tr2->VertexE())
				{
				    if (tr2->VertexE()->N() > edbv1->N())
				    {
					exist = true;
				    }
				}
			    }
			    if (!exist)			    
			    {
			        ncombinv++;
				if((vta = AddTrack(*edbv1, tr2, zpos)))
				{
				    nomatch = 0;
				    wasadded = true;
				    edbv2->SetFlag(-10);
				    tr2->AddVTA(vta);
				    AddVTA(vta);
				    int vfl=edbv1->Flag();
				    if      (vfl==0&&zpos==0) edbv1->SetFlag(1);
				    else if (vfl==2&&zpos==1) edbv1->SetFlag(1);
//				    printf("Add track ID %d from vertex %d to vertex %d\n",
//				    tr2->ID(), i2, i1);
				}
			    }
			    else
			    {
			        nomatch = 0;
			    }
			    edbv2->SetFlag(-10);
			} // if one of tracks vertex 2 equal any track in vertex 1
			it2++;
		      } // tracks in vertex 2
		      it1++;
		    } // tracks in vertex 1
		} // if vertex 2 has rank 2
	} // second vertex loop
	if (wasadded) nadd++;
    }  // first vertex loop

    printf("\b\b\b\b%3d%%\n",100);

    printf("  %6d 2-track vertex pairs with common track\n",
	      ncombin);
    printf("  %6d pairs when common track not yet attached\n  %6d N-track vertexes with Prob > %f\n",
	      ncombinv, nadd, eProbMin);
    printf("--------------------------------------------------------\n");

    StatVertexN();
    return nadd;
}


//---------------------------------------------------------
void EdbVertexRec::StatVertexN()
{
  int nvt = Nvtx();
  if (!nvt) return;
  TArrayI navtx(10);
  EdbVertex *v=0;
  int ntv=0;
  for(int i=0; i<nvt; i++)    {
    v = (EdbVertex*)(eVTX->At(i));
    if (!v) continue;
    if (v->Flag() < 0) continue;
    ntv=v->N();
    if (ntv > 10) ntv = 10;
    navtx[ntv-2]++;
  }
  for ( ntv=0; ntv<10; ntv++)    {
    if ( ntv < 9 )
      printf("%5d vertexes with number of tracks  = %2d was found\n",
	     navtx[ntv], ntv+2);
    else
      printf("%5d vertexes with number of tracks >= %2d was found\n",
	     navtx[ntv], ntv+2);
  }
}


//---------------------------------------------------------
int EdbVertexRec::LinkedVertexes()
{
  int nvt = Nvtx();
  if (!nvt) return 0;

  EdbVertex *v = 0;

  int nvl = 0;
  for (int iv=0; iv<nvt; iv++) {
    v = (EdbVertex*)(eVTX->At(iv));
    if (v)
    {
	if (v->Flag() != -10)
	{
	    if (v->Nv() != 0)
	    {
			if (v->Flag() < 3) v->SetFlag(v->Flag()+3);
			nvl++;
	    }
	}
    }
  }
  return nvl;
} 

//---------------------------------------------------------
int EdbVertexRec::SelVertNeighbor( EdbVertex *v, int seltype, float RadMax, int Dpat, TObjArray *ao)
{
  EdbSegP ss; // the virtual "vertex" segment

  if (!v) return 0;

  float x = v->VX();
  float y = v->VY();
  float z = v->VZ();

  ss.SetX(x);
  ss.SetY(y);
  ss.SetZ(z);
  ss.SetTX(0.);
  ss.SetTY(0.);
  ss.SetErrors(RadMax*RadMax, RadMax*RadMax, 0., 0., 0., 0.);

  TObjArray arr(20);
  ePVR->FindComplimentsVol(ss,arr,1,1,Dpat);
  
  int nseg = arr.GetEntries();
  EdbTrackP *tr = 0;
  EdbSegP   *s  = 0;
  int trflg  = 0;
  int trind  = 0;
  int ntr = eEdbTracks->GetEntries();

  int nadd = 0;
  for (int i=0; i<nseg; i++)
  {
    s = (EdbSegP *)(arr.At(i));
    if (!s) continue;
    tr = 0;
    trind = s->Track();
    trflg = 0;
    if ( trind >= 0 && trind < ntr)
    {
	if ((tr = (EdbTrackP *)eEdbTracks->At(trind))) 
	{
	    trflg = tr->Flag();
	    if (trflg != -10 && seltype == 0 && tr->MCEvt() >= -999)
	    {
//		if (!(ao->FindObject(tr)))
//		{
		    if (ao)
		    {
			tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
			ao->Add(tr);
		    }
		    nadd++;
//		}
		continue;
	    }
	}
    } 
    if((trind < 0 || trflg == -10)&&(seltype == 1))
    {
	if (ao) ao->Add(s);
	nadd++;
    }
  }
 
  if (seltype == 0)
  {
    nseg = ao->GetEntries();
    for (int i=0; i<nseg; i++)
    {
	tr = (EdbTrackP *)(ao->At(i)); 
	if (tr && (tr->MCEvt() < -999)) tr->SetMC( -tr->MCEvt()-2000, tr->MCTrack());
    }
  }
 
  return nadd;
}

//---------------------------------------------------------
int EdbVertexRec::SelSegNeighbor( EdbSegP *sin, int seltype, float RadMax, int Dpat, TObjArray *ao)
{
  EdbSegP ss; // the virtual "vertex" segment

  if (!sin) return 0;

  ss.SetX(sin->X());
  ss.SetY(sin->Y());
  ss.SetZ(sin->Z());
  ss.SetTX(sin->TX());
  ss.SetTY(sin->TY());
  ss.SetErrors(RadMax*RadMax, RadMax*RadMax, 0., 0., 0., 0.);

  TObjArray arr(1000);
  
  ePVR->FindComplimentsVol(ss,arr,1,1,Dpat);

  int nseg = arr.GetEntries();
  EdbTrackP *tr = 0;
  EdbSegP   *s  = 0;
  int trflg  = 0;
  int trind  = 0;
  int ntr = eEdbTracks->GetEntries();

  int nadd = 0;
  for (int i=0; i<nseg; i++)
  {
    s = (EdbSegP *)(arr.At(i));
    if (!s) continue;
    tr = 0;
    trind = s->Track();
    trflg = 0;
    if ( trind >= 0 && trind < ntr)
    {
	if ((tr = (EdbTrackP *)eEdbTracks->At(trind))) 
	{
	    trflg = tr->Flag();
	    if (trflg != -10 && seltype == 0 && tr->MCEvt() >= -999)
	    {
//		if (!(ao->FindObject(tr)))
//		{
		    if (ao)
		    {
			tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
			ao->Add(tr);
		    }
		    nadd++;
//		}
		continue;
	    }
	}
    } 
    if((trind < 0 || trflg == -10)&&(seltype == 1))
    {
	if (ao) ao->Add(s);
	nadd++;
    }
  }
 
  if (seltype == 0)
  {
    nseg = ao->GetEntries();
    for (int i=0; i<nseg; i++)
    {
	tr = (EdbTrackP *)(ao->At(i)); 
	if (tr && (tr->MCEvt() < -999)) tr->SetMC( -tr->MCEvt()-2000, tr->MCTrack());
    }
  }

  return nadd;
}

//______________________________________________________________________________
int EdbVertexRec::VertexNeighbor(float RadMax, int Dpat, float ImpMax)
{
  if(!ePVR) {printf("Warning: EdbVertexRec::VertexNeighbor: EdbPVRec not defined\n"); return 0;}

  int nn   = 0, iv = 0;
//  int i = 0, nntr = 0;
  int nvt = 0;
  int ntr = 0;
  if (eVTX) nvt = eVTX->GetEntries();
  if (eEdbTracks) ntr = eEdbTracks->GetEntries();
  if (!nvt) return 0;
  if (!ntr) return 0;

  EdbVertex *v   = 0;

  for (iv=0; iv<nvt; iv++) {
    v = (EdbVertex*)(eVTX->At(iv));
    if (v)
    {
	    nn += VertexNeighbor(v, RadMax, Dpat, ImpMax);
//	    nntr = v->Nn();
//	    for(i=0; i<nntr; i++) AddVTA(v->GetVTn(i));
    }
  }

  return nn;
} 

//______________________________________________________________________________
int EdbVertexRec::VertexNeighbor(EdbVertex *v, float RadMax, int Dpat, float ImpMax)
{
  if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if(!ePVR) {printf("Warning: EdbVertexRec::VertexNeighbor: EdbPVRec not defined\n"); return 0;}

  EdbVTA    *vta = 0;
  EdbTrackP *tr  = 0;
  const EdbSegP   *ss  = 0;
  const EdbSegP   *se  = 0;
  EdbTrackP *trv = 0;
  EdbVertex *ve = 0;
  float dx = 0., dy = 0.;
  float dz = 0;
  int 	    zpos = 0;
  int 	    nn   = 0, nntr = 0;
  float     distxs, distys, distzs1, distzs, dists, dist = 0.;
  float     distxe = 0., distye = 0., distze1= 0., distze = 0., diste = 0.;
  float     xvert = 0, yvert = 0, zvert = 0;
  Float_t   Zbin = TMath::Abs(ePVR->GetPattern(1)->Z() - ePVR->GetPattern(0)->Z());
  TObjArray an(20);

  if (v->Flag() != -10)
	{
	    v->ClearNeighborhood();
	    xvert = v->VX();
	    yvert = v->VY();
	    zvert = v->VZ();
	    // Select tracks neigborhood
	    an.Clear();
	    int ntr = v->N();
	    for(int i=0; i<ntr; i++)
	    {
		tr = v->GetTrack(i);
		tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
		if (v->Zpos(i) == 1)
		    ve = tr->VertexE(); 
		else
		    ve = tr->VertexS(); 
		if (ve)
		{
		    dx = ve->VX() - v->VX();
		    dy = ve->VY() - v->VY();
		    if (TMath::Sqrt(dx*dx + dy*dy) > RadMax) continue;
		    dz = ve->VZ() - v->VZ();
		    if (dz > Dpat*Zbin) continue;
		    vta = new EdbVTA((EdbTrackP *)ve, v);
		    vta->SetFlag(3);
		    vta->SetDist(TMath::Sqrt(dx*dx+dy*dy+dz*dz));
		    v->AddVTA(vta);
		    ve->SetFlag(-ve->Flag()-200);
		    for(int j=0; j<ve->N(); j++)
		    {
			trv = ve->GetTrack(j);
			if (trv->MCEvt() < -999) continue;
			if (ve->Zpos(j) == 0)
			    ss = tr->TrackZmax();
			else
			    ss = tr->TrackZmin();
			dx = ss->X() - v->VX();
			dy = ss->Y() - v->VY();
			dz = ss->Z() - v->VZ();
			if (TMath::Sqrt(dx*dx+dy*dy) > RadMax)    continue;
			if (TMath::Abs(dz)        > Dpat*Zbin) continue;
			dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
			vta = v->CheckImp(trv, 10.e+10, ve->Zpos(j), dist);
			if (vta) trv->SetMC(-trv->MCEvt()-2000, trv->MCTrack());
		    }
		}
	    }
	    int nvn = SelVertNeighbor(v, 0, RadMax, Dpat, &an); 
	    for (int it=0; it<nvn; it++) {
		    tr = (EdbTrackP*)(an.At(it));
		    if (tr)
		    {
			    if (tr->MCEvt() < -999) continue;
			    ss = tr->TrackZmin();
			    distxs = (xvert - ss->X());
			    distxs *= distxs;
			    distys = (yvert - ss->Y());
			    distys *= distys;
			    distzs1 = (zvert - ss->Z());
			    distzs = distzs1*distzs1;
			    dists  =  distxs + distys + distzs;
			    dists  =  TMath::Sqrt(dists); 
			    se = tr->TrackZmax();
			    distxe = (xvert - se->X());
			    distxe *= distxe;
			    distye = (yvert - se->Y());
			    distye *= distye;
			    distze1 = (zvert - se->Z());
			    distze = distze1*distze1;
			    diste  =  distxe + distye + distze;
			    diste  =  TMath::Sqrt(diste);
			    if (diste < dists)
			    {
				if (TMath::Sqrt(distxe+distye) > RadMax)    continue;
				if (TMath::Abs(distze1)        > Dpat*Zbin) continue;
				dist = diste;
				zpos = 0;
			    }
			    else
			    {
				if (TMath::Sqrt(distxs+distys) > RadMax)    continue;
				if (TMath::Abs(distzs1)        > Dpat*Zbin) continue;
				dist = dists;
				zpos = 1;
			    }
			    vta = v->CheckImp(tr, ImpMax, zpos, dist);
			    if (vta)
			    {
				nn++;
				tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
				if (zpos == 0)
				    ve = tr->VertexE(); 
				else
				    ve = tr->VertexS(); 
				if (ve && ve->Flag() >= -99 && ve->Flag() != -10 && ve != v)
				{
				    dx = ve->VX() - v->VX();
				    dy = ve->VY() - v->VY();
				    if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				    {
					dz = ve->VZ() - v->VZ();
					if (dz <= Dpat*Zbin)
					{
					    ve->SetFlag(-ve->Flag()-200);
					    vta = new EdbVTA((EdbTrackP *)ve, v);
					    vta->SetFlag(3);
					    vta->SetDist(TMath::Sqrt(dx*dx+dy*dy+dz*dz));
					    v->AddVTA(vta);
					    for(int j=0; j<ve->N(); j++)
					    {
						trv = ve->GetTrack(j);
						if (trv->MCEvt() < -999) continue;
						if (ve->Zpos(j) == 0)
						    ss = trv->TrackZmax();
						else
						    ss = trv->TrackZmin();
						dx = ss->X() - v->VX();
						dy = ss->Y() - v->VY();
						dz = ss->Z() - v->VZ();
						if (TMath::Sqrt(dx*dx+dy*dy) > RadMax)    continue;
						if (TMath::Abs(dz)        > Dpat*Zbin) continue;
						dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
						vta = v->CheckImp(trv, 10.e+10, ve->Zpos(j), dist);
    						if (vta) trv->SetMC(-trv->MCEvt()-2000, trv->MCTrack());
					    }
					}
				    }
				}
			    }
		    }
	    }
	    nntr = v->Nn();
	    for(int i=0; i<nntr; i++)
	    {
		if ((vta = v->GetVTn(i)))
		{
		    if (vta->Flag() == 0) //track
		    {
			tr = vta->GetTrack();
    			if (tr->MCEvt() < -999 ) tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
		    }
		    else if (vta->Flag() == 3) //vertex
		    {
    			ve = (EdbVertex *)vta->GetTrack();
    			if (ve->Flag() < -99 ) ve->SetFlag(-ve->Flag()-200);
		    }
		}
	    }
	    // Select segments neigborhood
	    an.Clear();
	    nvn = SelVertNeighbor(v, 1, RadMax, Dpat, &an); 
	    for (int it=0; it<nvn; it++) {
		    ss = (EdbSegP*)(an.At(it));
		    if (ss)
		    {
			    distxs = (xvert - ss->X());
			    distxs *= distxs;
			    distys = (yvert - ss->Y());
			    distys *= distys;
			    distzs1 = (zvert - ss->Z());
			    distzs = distzs1*distzs1;
			    dists  =  distxs + distys + distzs;
			    dists  =  TMath::Sqrt(dists); 
//			    if (TMath::Sqrt(distxs+distys) > RadMax)    continue;
//			    if (TMath::Abs(distzs1)        > Dpat*Zbin) continue;
//			    vta = v->CheckImp((EdbTrackP *)ss, ImpMax, zpos, dists);
			    vta = new EdbVTA((EdbTrackP *)ss, v);
			    vta->SetZpos(1);
			    vta->SetFlag(1);
			    vta->SetImp(0.);
			    vta->SetDist(dists);
			    v->AddVTA(vta);
			    nn++;
		    }
	    }
  }
  return nn;
} 

//______________________________________________________________________________
int EdbVertexRec::SegmentNeighbor(EdbSegP *s, float RadMax, int Dpat, TObjArray *arrs, TObjArray *arrt, TObjArray *arrv)
{
  if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if(!ePVR) {printf("Warning: EdbVertexRec::SegmentNeighbor: EdbPVRec not defined\n"); return 0;}

  EdbTrackP *tr  = 0, *trown = 0;
  const EdbSegP   *ss  = 0;
  const EdbSegP   *se  = 0;
  int 	    nn   = 0;
  float     distxs, distys, distzs1, distzs, dists;
  float     distxe = 0., distye = 0., distze1= 0., distze = 0., diste = 0.;
  float     xseg = 0, yseg = 0, zseg = 0;
  EdbTrackP *trv = 0;
  EdbVertex *ve = 0;
  float dx = 0., dy = 0.;
  float dz = 0;
  Float_t   Zbin = TMath::Abs(ePVR->GetPattern(1)->Z() - ePVR->GetPattern(0)->Z());
  TObjArray an(200);

	    xseg = s->X();
	    yseg = s->Y();
	    zseg = s->Z();
	    an.Clear();
	    if (s->Track() >= 0)
	    {
		trown = (EdbTrackP *)ePVR->eTracks->At(s->Track()); 
    		if (trown) trown->SetMC(-trown->MCEvt()-2000, trown->MCTrack());
	    }
	    // Select tracks neigborhood
	    int nvn = SelSegNeighbor(s, 0, RadMax, Dpat, &an); 
	    if (trown) an.Add(trown);
	    nvn = an.GetEntries();
	    for (int it=0; it<nvn; it++) {
		    tr = (EdbTrackP*)(an.At(it));
		    if (tr)
		    {
			    if (tr != trown)
			    {
				ss = tr->TrackZmin();
				distxs = (xseg - ss->X());
				distxs *= distxs;
				distys = (yseg - ss->Y());
				distys *= distys;
				distzs1 = (zseg - ss->Z());
				distzs = distzs1*distzs1;
				dists  =  distxs + distys + distzs;
				dists  =  TMath::Sqrt(dists); 
				se = tr->TrackZmax();
				distxe = (xseg - se->X());
				distxe *= distxe;
				distye = (yseg - se->Y());
				distye *= distye;
				distze1 = (zseg - se->Z());
				distze = distze1*distze1;
				diste  =  distxe + distye + distze;
				diste  =  TMath::Sqrt(diste);
				if (diste < dists)
				{
				    if (TMath::Sqrt(distxe+distye) > RadMax)    continue;
				    if (TMath::Abs(distze1)        > Dpat*Zbin) continue;
				}
				else
				{
				    if (TMath::Sqrt(distxs+distys) > RadMax)    continue;
				    if (TMath::Abs(distzs1)        > Dpat*Zbin) continue;
				}
			    }
			    if (arrt) arrt->Add(tr);
			    nn++;
//--------------------------Add vertex neighborhood
    			    tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
			    ve = tr->VertexS(); 
			    if (ve && ve->Flag() >= -99 && ve->Flag() != -10)
			    {
				dx = ve->VX() - s->X();
				dy = ve->VY() - s->Y();
				if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				{
				    dz = ve->VZ() - s->Z();
				    if (dz <= Dpat*Zbin)
				    {
					ve->SetFlag(-ve->Flag()-200);
					arrv->Add(ve);
					for(int j=0; j<ve->N(); j++)
					{
					    trv = ve->GetTrack(j);
					    if (trv->MCEvt() < -999) continue;
					    if (ve->Zpos(j) == 0)
					        ss = trv->TrackZmax();
					    else
					        ss = trv->TrackZmin();
					    dx = ss->X() - s->X();
					    dy = ss->Y() - s->Y();
					    dz = ss->Z() - s->Z();
					    if (TMath::Sqrt(dx*dx+dy*dy) > RadMax)    continue;
					    if (TMath::Abs(dz)        > Dpat*Zbin) continue;
					    arrt->Add(trv);
    					    trv->SetMC(-trv->MCEvt()-2000, trv->MCTrack());
					}
				    }
				}
			    }
			    ve = tr->VertexE(); 
			    if (ve && ve->Flag() >= -99 && ve->Flag() != -10)
			    {
				dx = ve->VX() - s->X();
				dy = ve->VY() - s->Y();
				if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				{
				    dy = ve->VZ() - s->Z();
				    if (dz <= Dpat*Zbin)
				    {
					ve->SetFlag(-ve->Flag()-200);
					arrv->Add(ve);
					for(int j=0; j<ve->N(); j++)
					{
					    trv = ve->GetTrack(j);
					    if (trv->MCEvt() < -999) continue;
					    if (ve->Zpos(j) == 0)
					        ss = trv->TrackZmax();
					    else
					        ss = trv->TrackZmin();
					    dx = ss->X() - s->X();
					    dy = ss->Y() - s->Y();
					    dz = ss->Z() - s->Z();
					    if (TMath::Sqrt(dx*dx+dy*dy) > RadMax)    continue;
					    if (TMath::Abs(dz)        > Dpat*Zbin) continue;
					    arrt->Add(trv);
    					    trv->SetMC(-trv->MCEvt()-2000, trv->MCTrack());
					}
				    }
				}
			    }
		    }
	    }
//----------Restore MCEvt
	    int nv = arrv->GetEntries();
	    for (int i=0; i<nv; i++)
	    {
		ve = (EdbVertex *)arrv->At(i);
		if (ve->Flag() < -99 ) ve->SetFlag(-ve->Flag()-200);
	    }
	    int ntr = arrt->GetEntries();
	    for (int i=0; i<ntr; i++)
	    {
		tr = (EdbTrackP *)arrt->At(i);
		if (tr->MCEvt() < -999 ) tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
	    }
	    // Select segments neigborhood
	    nvn = SelSegNeighbor(s, 1, RadMax, Dpat, arrs); 
//	    printf("Selected %d segments\n", nvn);
	    nn += nvn;
  return nn;
} 

//________________________________________________________________________
TTree *EdbVertexRec::init_tracks_tree(const char *file_name, EdbTrackP *track)
{
  TTree *tracks=0;
  /*
  char *tree_name="tracks";
  TFile *f = new TFile(file_name);

  if (f)  tracks = (TTree*)f->Get(tree_name);
  if(!tracks) return 0;
  f->cd();

  EdbSegP *tr = (EdbSegP*)track;
  TClonesArray *segments  = track->S()->GetSegments();
  TClonesArray *segmentsf = track->SF()->GetSegments();

  tracks->SetBranchAddress("t.",&tr);
  tracks->SetBranchAddress("s" ,&segments);
  tracks->SetBranchAddress("sf",&segmentsf);
  */
  return tracks;
}

//________________________________________________________________________
double EdbVertexRec::Tdistance(const Track& t1, const Track& t2) {
  //
  // geometrical distance between 2 track lines in the space XYZ
  //
  const double a  = t1.tx();
  const double b  = t1.ty();
  const double c  = 1.;
  const double a1 = t2.tx();
  const double b1 = t2.ty();
  const double c1 = 1.;

  const double det = square(a*b1 - b*a1) + square(b*c1 - c*b1) + square(c*a1 - a*c1);
  const SVector<double,3> dx = t2.xvec() - t1.xvec();
  // are tracks parallel?
  if(det==0) return mag(cross(dx,t1.evec()));

  const double det2 = dx[0]*(b*c1 - c*b1) + dx[1]*(c*a1 - a*c1) + dx[2]*(a*b1 - b*a1);
  
  return fabs(det2/sqrt(det));
}

//________________________________________________________________________
double EdbVertexRec::Tdistance(const EdbSegP &s1, const EdbSegP &s2) 
{
  EdbVertex edbv;
  Track t1;
  Track t2;
  if(!edbv.Edb2Vt(s1,t1))  return 1.E20;
  if(!edbv.Edb2Vt(s2,t2))  return 1.E20;
  return Tdistance(t1,t2);

}

//________________________________________________________________________
int EdbVertexRec::BuildTracksArr(const char *file_name, int nsegMin )
{
  // read tracks from the "linked_tracks" tree and fill TObjArray

  EdbTrackP *track  = new EdbTrackP(8);
  TTree     *tracks = init_tracks_tree(file_name, track);

  EdbTrackP *track1=0;
  int ngood = 0;

  int nentr = (int)(tracks->GetEntries());

  for(int i=0; i<nentr; i++) {

    tracks->GetEntry(i);
    track1 = new EdbTrackP(*track);
    if (track1->N() < nsegMin ) continue;
    eEdbTracks->Add( (TObject *)track1 );

    ngood++;
  }

  eEdbTracks->Expand(ngood);
  return ngood;
}

//________________________________________________________________________
EdbTrackP *EdbVertexRec::GetEdbTrack(const int index)
{
  if (!eEdbTracks) return 0;
  if (index < 0)   return 0;
  if (index > eEdbTracks->GetLast()) return 0;
  return     (EdbTrackP *)eEdbTracks->At(index);
}
