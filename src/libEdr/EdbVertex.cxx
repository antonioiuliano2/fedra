//-- Author :  Valeri Tioukov & Yury Petukhov  10.02.2004
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertex                                                            //
//                                                                      //
// Class for vertex reconstruction                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TROOT.h"
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
  if(eTrack && eFlag == 2) { eTrack->ClearVTA(this); }
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
  if (eV) {eV->clear(); SafeDelete(eV);}
  //for(int i=0; i<N();  i++) delete GetVTa(i);
  //for(int i=0; i<Nn(); i++) delete GetVTn(i);
  eVTa.Delete("slow");
  eVTn.Delete("slow");
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
  //for(int i=0; i<N();  i++) delete GetVTa(i);
  //for(int i=0; i<Nn(); i++) delete GetVTn(i);
  eVTa.Delete("slow");
  eVTn.Delete("slow");
  if (eV) {eV->clear(); SafeDelete(eV);}
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
  int nn = Nn();
  if (nn>0) eVTn.Clear("nodelete");
  for(int i=0; i<nn; i++) delete GetVTn(i);
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
  if(vta->Flag()!=2) eVTn.Add(vta);
  else               eVTa.Add(vta);
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
EdbVertex *EdbVertex::GetConnectedVertexForTrack(int it)
{
    EdbTrackP *tr = 0;
    EdbVertex *vc = 0;
    if (it<N())
    {
	if ((tr = GetTrack(it)))
	{
	    if      ((Zpos(it) == 1) && (vc = tr->VertexE()))
	    {
		return vc;
	    }
	    else if ((Zpos(it) == 0) && (vc = tr->VertexS()))
	    {
		return vc;
	    }
	}
    }
    return 0;
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
bool EdbVertex::Edb2Vt(const EdbTrackP& tr, Track& t, float X0, float m)
{
  return Edb2Vt( *((EdbSegP*)&tr), t, X0, m);
}

//________________________________________________________________________
//bool EdbVertex::Edb2Vt(const EdbSegP& tr, Track& t)
//{
//	double x1 = (double)tr.X() - eX;
//	double y1 = (double)tr.Y() - eY;
//	double z1 = (double)tr.Z() - eZ;
//	double tx1 = (double)tr.TX();
//	double ty1 = (double)tr.TY();
//	double p1 = (double)tr.P();
//
//	CMatrix cov1;             // covariance matrix for seg0
//	for(int k=0; k<5; k++) 
//    	    for(int l=0; l<5; l++) cov1(k,l) = (tr.COV())(k,l);
//
//	t.set(x1,  y1,  z1,  tx1,  ty1, p1, cov1);
//	t.propagate(0.);
//	return true;
//}
//________________________________________________________________________
bool EdbVertex::Edb2Vt(const EdbSegP& tr, Track& t, float X0, float m)
{
    double dz = eZ - tr.Z();
    double tx = (double)tr.TX();
    double ty = (double)tr.TY();
    double x  = (double)tr.X() + tx*dz - eX;
    double y  = (double)tr.Y() + ty*dz - eY;
    double z  = 0.;
    float  p  = tr.P();

    VtSymMatrix dms(4);   // multiple scattering matrix
    dms.clear();
    if ( X0 > 0. && m > 0.)
    {
	double dPb = dz*TMath::Sqrt(1.+tx*tx+ty*ty); // thickness of the Pb+emulsion cell in microns
	double teta0sq = EdbPhysics::ThetaMS2( p, m, dPb, X0 );
	dms(0,0) = teta0sq*dz*dz/3.;
	dms(1,1) = dms(0,0);
	dms(2,2) = teta0sq;
	dms(3,3) = dms(2,2);
	dms(2,0) = teta0sq*dz/2.;
	dms(3,1) = dms(2,0);
	dms(0,2) = dms(2,0);
	dms(1,3) = dms(2,0);
    }

    VtSqMatrix pred(4);        //propagation matrix for track parameters (x,y,tx,ty)
    pred.clear();
    pred(0,0) = 1.;
    pred(1,1) = 1.;
    pred(2,2) = 1.;
    pred(3,3) = 1.;
    pred(0,2) = dz;
    pred(1,3) = dz;

    VtSymMatrix cov(4);             // covariance matrix for seg0
    for(int k=0; k<4; k++) 
	for(int l=0; l<4; l++) cov(k,l) = (tr.COV())(k,l);


    VtSymMatrix covpred(4);         // covariation matrix for prediction
    covpred = pred*(cov*(pred.T()))+dms;

    CMatrix covp;             // covariance matrix for seg0
    covp.clear();
    for(int k=0; k<4; k++) 
      for(int l=0; l<4; l++) covp(k,l) = covpred(k,l);
                             covp(4,4) = (tr.COV())(4,4);

    t.set(x,  y,  z,  tx,  ty,  (double)p, covp);
    return true;
}
//________________________________________________________________________
float EdbVertex::Chi2Track(EdbTrackP *track, int zpos, float X0)
{
  // distance from track to already existed vertex

  if (!track) return 0.;
  double distchi2 = -2.;
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
      if (track->NF() <= 0) return -1.;
      if (track->P() <= 0.) track->SetP(1.);
      if (track->M() <= 0.) track->SetM(.1395);
      if (track->SP() <= 0.) track->SetErrorP(1.);
      Track *t=new Track();
      if( Edb2Vt( *seg, *t, X0, track->M() ) )
	{
	  distchi2 = -3.;
	  if (eV->valid())
	    {
	      t->rm(track->M());
	      distchi2 = eV->distance(*t);
	    } 
	}
      else
	{ 
	  printf("EdbVertex::Chi2Track - conversion to VT failed!");
	  distchi2 = -4.;
	}
      delete t;
      t=0;
    }
  return (float)distchi2;
}
//________________________________________________________________________
float EdbVertex::DistTrack(EdbTrackP *track, int zpos, float X0)
{
  // distance from track to already existed vertex

  if (!track) return 0.;
  double dist = 0.;
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
      if( Edb2Vt( *seg, *t, X0, track->M() ) )
	{
	  if (eV->valid())
	    {
	      dist = distance(*t, *eV);
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

//________________________________________________________________________
float EdbVertex::DistSeg(EdbSegP *seg, float X0)
{
  // distance from track to already existed vertex

  if (!seg) return 0.;
  double dist = 0.;
  if (eV)
    {
      Track *t=new Track();
      if( Edb2Vt( *seg, *t, X0, 0. ) )
	{
	  if (eV->valid())
	    {
	      dist = distance(*t, *eV);
	    } 
	}
      else
	{ 
	  printf("EdbVertex::DistSeg - conversion to VT failed!");
	}
      delete t;
      t=0;
    }
  return (float)dist;
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
	      dist = distance(*t, *eV);
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
  eVertex    = 0;
  eWorking   = 0;

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
  SafeDelete(eVTX);
  eVTA.Clear("nodelete");
}

//________________________________________________________________________
int EdbVertexRec::MakeV( EdbVertex &edbv )
{
  // create new VtVertex and add (two) tracks to this one
  //if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if (ePVR) if (ePVR->IsA() != EdbPVRec::Class()) ePVR = 0;
  if(!ePVR) {printf("Error: EdbVertexRec::MakeV: EdbPVRec not defined, use SetPVRec(...)\n"); return 0;}

  float X0 =  ePVR->GetScanCond()->RadX0();
  Vertex *v=edbv.V();
  if (v) {v->clear(); SafeDelete(v);}
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
    if (edbv.Edb2Vt(*seg, *t, X0, tr->M())) {
      t->rm((double)(tr->M()));
      v->push_back(*t);
    }
    else SafeDelete(t);

    if (i<50) ta[i] = t;
  }
  int retval = 0;
  if (v->ntracks() < 2) return retval;
  retval = v->findVertexVt();
  int n = edbv.N();
  if (!retval) return retval;
  if (!(v->valid())) return 0;
  for (int i=0; i<n; i++) {
    if (n<50) if (ta[i]) edbv.GetVTa(i)->SetImp(distance(*ta[i],*v));
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
      float X0 =  ePVR->GetScanCond()->RadX0();
      Track *t=new Track();
      if( edbv.Edb2Vt( *seg, *t, X0, track->M() ) )
	{
	  if (v->valid())
	    {
	      if (distance(*t, *v) > eImpMax)
	      {
		SafeDelete(t);
	      }
	      else
	      {
		  t->rm((double)(track->M()));
	          v->push_back(*t);
	      };
	      if (t)
	      {
	        if (!(v->findVertexVt()))
		{
		  v->remove_last();
		  SafeDelete(t);
		  printf("EdbVertex::AddTrack - vertex not found!");
	          v->findVertexVt();
		}
	        else if (!(v->valid()))
		{
		  v->remove_last();
		  SafeDelete(t);
		  printf("EdbVertex::AddTrack - vertex not valid!");
	          v->findVertexVt();
		}
	        else if (v->prob() < eProbMin )
		{
		  v->remove_last();
		  SafeDelete(t);
	          v->findVertexVt();
		}
	        else
		{
		  vta = new EdbVTA(track, &edbv);
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
	}
      else
	{ 
	  SafeDelete(t);
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

  //if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if (ePVR) if (ePVR->IsA() != EdbPVRec::Class()) ePVR = 0;
  if(!ePVR) {printf("Error: EdbVertexRec::FindVertex: EdbPVRec not defined, use SetPVRec(...)\n"); return 0;}

  EdbVertex *edbv = 0;
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

  if(nvtx!=nvtxt) printf("ERROR: EdbVertexRec::FindVertex():  nxtx =%d nvtxt =%d\n",nvtx,nvtxt);

  for (Int_t i = 0; i < nvtxt; i++) GetVertex(i)->SetID(i);

  if (nvtxt) eVTX->Sort(nvtxt-1);

  for (Int_t i = nvtx-1; i >= 0; i--) {
    edbv = GetVertex(i);
    if (!edbv) continue;
    edbv->SetID(i);
    edbv->ResetTracks();
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
    v[0] = (Long_t)(tr->TrackZmin(eUseSegPar)->Z()/eZbin);
    v[1] = itr;
    starts.Add(2,v); 
    v[0] = (Long_t)(tr->TrackZmax(eUseSegPar)->Z()/eZbin);
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
  float z1, z2, vestim[3];
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
      fflush(stdout);
      
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

	  imp = CheckImpact( s1, s2, zpos1, zpos2, vestim );
	  if(imp>eImpMax)                                         continue;

      	  ncombinv++;

	  //printf("imp = %f \n", imp);

	  if( !ProbVertex( tr1, tr2, zpos1, zpos2, vestim) )      continue;


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
				 int zpos1,     int zpos2,    float pv[3] )
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
  if (pv) for (int i = 0; i<3; i++) pv[i] = 0.5*(pa[i] + pb[i]);
  return EdbMath::Magnitude3( pa, pb );
}

//______________________________________________________________________________
int EdbVertexRec::ProbVertex( EdbTrackP *tr1, EdbTrackP *tr2,
			      int zpos1,      int zpos2,      float vestim[3] )
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
      SafeDelete(v2);
      return 0;
    }
  if (!(vta2 = AddTrack( *v2, tr2, zpos2)))
    {
      SafeDelete(v2);
      return 0;
    }

  //v2->EstimateVertexMath(x,y,z,d);                         //TODO?????
  //printf("xyz: %f %f %f \t d: %f\n",x,y,z,d);

  if (vestim) v2->SetXYZ(vestim[0], vestim[1], vestim[2]);

  if(!(MakeV(*v2)))
    {
      SafeDelete(vta1);
      SafeDelete(vta2);
      SafeDelete(v2);
      return 0;
    }

  if( !(v2->V()) )
    {
      SafeDelete(vta1);
      SafeDelete(vta2);
      SafeDelete(v2);
      return 0;
    }

  if( !(v2->V()->valid()) )
    {
      SafeDelete(vta1);
      SafeDelete(vta2);
      SafeDelete(v2);
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
	  SafeDelete(vta1);
	  SafeDelete(vta2);
	  SafeDelete(v2);
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
	  SafeDelete(vta1);
	  SafeDelete(vta2);
	  SafeDelete(v2);
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
	  SafeDelete(vta1);
	  SafeDelete(vta2);
	  SafeDelete(v2);
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
    SafeDelete(vta1);
    SafeDelete(vta2);
    SafeDelete(v2);
  }
  return 0;
}
//______________________________________________________________________________
EdbVertex *EdbVertexRec::ProbVertex1( EdbTrackP *tr1, EdbTrackP *tr2,
			              int zpos1,      int zpos2,      float vestim[3] )
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

  v2 = new EdbVertex();

  if (!(vta1 = AddTrack( *v2, tr1, zpos1)))
    {
      SafeDelete(v2);
      return 0;
    }
  if (!(vta2 = AddTrack( *v2, tr2, zpos2)))
    {
      SafeDelete(v2);
      return 0;
    }

  //v2->EstimateVertexMath(x,y,z,d);                         //TODO?????
  //printf("xyz: %f %f %f \t d: %f\n",x,y,z,d);

  if (vestim) v2->SetXYZ(vestim[0], vestim[1], vestim[2]);

  if(!(MakeV(*v2)))
    {
      SafeDelete(vta1);
      SafeDelete(vta2);
      SafeDelete(v2);
      return 0;
    }

  if( !(v2->V()) )
    {
      SafeDelete(vta1);
      SafeDelete(vta2);
      SafeDelete(v2);
      return 0;
    }

  if( !(v2->V()->valid()) )
    {
      SafeDelete(vta1);
      SafeDelete(vta2);
      SafeDelete(v2);
      return 0;
    }

  
  d = v2->V()->rmsDistAngle();
  x = v2->VX();
  y = v2->VY();
  z = v2->VZ();
  prob = v2->V()->prob();

  if (zpos1 == 0 && zpos2 == 1)            // ends & starts
    {
	  v2->SetFlag(1);
    }
  else if (zpos1 == 1 && zpos2 == 1)        // starts & starts
    {
	  v2->SetFlag(0);
    }
  else if (zpos1 == 0 && zpos2 == 0)        // ends & ends
    {
	  v2->SetFlag(2);
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
    return v2;
  }
  else {
    SafeDelete(vta1);
    SafeDelete(vta2);
    SafeDelete(v2);
  }
  return 0;
}
//______________________________________________________________________________
int EdbVertexRec::ProbVertexN()
{
  EdbVTA *vta = NULL, *vta1 = NULL, *vta2 = NULL;
  EdbVertex *edbv1 = NULL;
  EdbVertex *edbv2 = NULL;
  Vertex *v = 0;
  EdbTrackP *tr = 0;
  EdbTrackP *tr2 = 0;
  Int_t zpos = 0;
  int nvtx = 0;
  int nadd = 0;
  int ncombin = 0;
  int ncombinv = 0;
  bool wasadded = false;
  float dz = 0.;
  
  if (eVTX) {
    nvtx = eVTX->GetEntries();
    for (Int_t i = 0; i < nvtx; i++) {
      edbv1 = GetVertex(i);
      if (edbv1) {
	if (edbv1->N() > 2) {
	  for (Int_t j = 0; j<edbv1->N(); j++)  eVTA.Remove(edbv1->GetVTa(j));
	  for (Int_t j = 0; j<edbv1->Nn(); j++) eVTA.Remove(edbv1->GetVTn(j));
	  tr  = edbv1->GetTrack(0);
	  tr2 = edbv1->GetTrack(1);
	  edbv1->Clear();
	  vta1 = AddTrack(*edbv1, tr, edbv1->Zpos(0));
	  vta2 = AddTrack(*edbv1, tr2, edbv1->Zpos(1));
	  MakeV(*edbv1);
	  v = edbv1->V();
	  v->findVertexVt();

	  if (!eQualityMode)
	    edbv1->SetQuality(v->prob()/(v->vtx_cov_x()+v->vtx_cov_y()));
	  else if (eQualityMode == 1) {
	    Double_t rms = v->rmsDistAngle();
	    if (rms) edbv1->SetQuality((Float_t)(1./rms));
	    else edbv1->SetQuality(10.e+35);
	  }
	  else edbv1->SetQuality(1.);

	  tr->AddVTA(vta1);
	  tr2->AddVTA(vta2);
	  AddVTA(vta1);
	  AddVTA(vta2);
	}
	else {
	  if (edbv1->Flag() < 0) {
	    zpos = edbv1->Zpos(0) + edbv1->Zpos(1);
	    if (!zpos) edbv1->SetFlag(2);
	    else if (zpos == 1) edbv1->SetFlag(1);
	    else if (zpos == 2) edbv1->SetFlag(0);
	  }
	}
      }
    }
    edbv1 = 0;
  }
  else return 0;

  zpos = 0;

  nvtx = eVTX->GetEntries();
  printf("-----Merge 2-track vertex pairs to N-track vertexes-----\n");
  printf("N-track vertexes search in progress... %3d%%", 0);

  int nprint = (int)(0.05*(double)nvtx);
  if (nprint <= 0) nprint = 1;

  for (Int_t i1 = 0; i1 < nvtx; i1++) {
    wasadded = false;
    edbv1 = GetVertex(i1);
    if (!(i1%nprint)) {
      printf("\b\b\b\b%3d%%",(int)((double)i1/double(nvtx)*100.));
      fflush(stdout);
    }
    if (!edbv1) continue;
    if (edbv1->Flag() == -10) continue;
    Int_t nt1 = edbv1->N();
    bool exist = false;
    if (nt1 == 2) {
      for (Int_t ic1 = 0; ic1 < nt1; ic1++) {
	tr = edbv1->GetTrack(ic1);
	zpos = edbv1->Zpos(ic1);
	if (zpos) {
	  if (tr->VertexS()) {
	    if (nt1 < tr->VertexS()->N()) {
	      exist = true;
	      break;
	    }
	  }
	}
	else {
	  if (tr->VertexE()) {
	    if (nt1 <  tr->VertexE()->N()) {
	      exist = true;
	      break;
	    }
	  }
	}
      }

      if (exist) {
	edbv1->SetFlag(-10);
	continue;
      }
    }
    for (Int_t i2 = i1+1; i2<nvtx; i2++) {
      edbv2 = GetVertex(i2);
      if (!edbv2) continue;
      if (edbv2->Flag() == -10) continue;
      if (edbv2->N() == 2) {
	// printf(" v1 id %d, v2 id %d\n", edbv1->ID(), edbv2->ID()); 
	nt1 = edbv1->N();
	int nt2 = edbv2->N();
	int it1=0;
	int nomatch = 1;
	while (it1 < nt1 && nomatch) {
	  int it2=0;
	  tr = edbv1->GetTrack(it1);
	  while ( (it2<nt2) && nomatch) {
	    if (edbv2->GetTrack(it2) == tr && 
		edbv1->Zpos(it1) == edbv2->Zpos(it2)) {
	      ncombin++;
	      if (!it2) {
		tr2 = edbv2->GetTrack(1);
		zpos = edbv2->Zpos(1);
	      }
	      else if (it2 == 1) {
		tr2 = edbv2->GetTrack(0);
		zpos = edbv2->Zpos(0);
	      }

	      exist = false;
	      for (int ic1=0; ic1<edbv1->N(); ic1++)
		if (tr2 == edbv1->GetTrack(ic1)) exist = true;

	      if (zpos) {
		if (tr2->VertexS()) {
		  if (tr2->VertexS()->N() > edbv1->N()) exist = true;
		}
	      }
	      else {
		if (tr2->VertexE()) {
		  if (tr2->VertexE()->N() > edbv1->N()) {
		    exist = true;
		  }
		}
	      }
	      if (!exist) {
		ncombinv++;
		if (zpos) dz = edbv1->VZ() - tr2->TrackZmin(eUseSegPar)->Z();
		else      dz = tr2->TrackZmax(eUseSegPar)->Z() - edbv1->VZ();
		if(dz <= eZbin)
		  if ((vta = AddTrack(*edbv1, tr2, zpos))) {
		    nomatch = 0;
		    wasadded = true;
		    edbv2->SetFlag(-10);
		    tr2->AddVTA(vta);
		    AddVTA(vta);
		    int vfl=edbv1->Flag();
		    if      (vfl==0&&zpos==0) edbv1->SetFlag(1);
		    else if (vfl==2&&zpos==1) edbv1->SetFlag(1);
		    // printf("Add track ID %d from vertex %d to vertex %d\n",
		    //				    tr2->ID(), i2, i1);
		  }
	      }
	      else {
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

  printf("  %6d 2-track vertex pairs with common track\n", ncombin);
  printf("  %6d pairs when common track not yet attached\n  %6d N-track vertexes with Prob > %f\n",
	 ncombinv, nadd, eProbMin);
  printf("--------------------------------------------------------\n");

  for (int i1=0; (i1<nvtx); i1++) {
    edbv1 = GetVertex(i1);
    if (!edbv1) continue;
    if (edbv1->Flag() == -10) continue;
    edbv1->ResetTracks();
  }

  StatVertexN();
  return nadd;
}


//---------------------------------------------------------
void EdbVertexRec::StatVertexN()
{
  Int_t nvt = Nvtx();
  if (!nvt) return;
  TArrayI navtx(10);
  EdbVertex *v=0;
  Int_t ntv = 0;
  for (Int_t i = 0; i < nvt; i++) {
    v = GetVertex(i);
    if (!v || v->Flag() < 0) continue;
    ntv = v->N();
    if (ntv > 11) ntv = 11;
    navtx[ntv-2]++;
  }
  for (ntv = 0; ntv < 10; ntv++) {
    if (ntv < 9)
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
    v = GetVertex(iv);
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
  int ntr = 0;
  if (eEdbTracks) ntr = eEdbTracks->GetEntries();

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
int EdbVertexRec::AddSegmentToVertex(EdbSegP *s, float ImpMax, float ProbMin, float Mom)
{
    EdbVTA *vta = 0;
    EdbVertex *ePrevious = 0;

    if (eWorking == 0)
    {
	eWorking = new EdbVertex();
	int ntr = eVertex->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = AddTrack(*(eWorking), eVertex->GetTrack(i), eVertex->Zpos(i))))
	    {
		eVertex->GetTrack(i)->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	  SafeDelete(eWorking);
	  eVertex->ResetTracks();
	  printf("Can't create working copy of the vertex!\n");
	  fflush(stdout);
	  return 0;
	}

	if (!MakeV(*(eWorking)))
	{
	  SafeDelete(eWorking);
	  eVertex->ResetTracks();
	  printf("Can't create working copy of the vertex!\n");
	  fflush(stdout);
	  return 0;
	}
    }
    else
    {
	ePrevious = eWorking;
	eWorking = new EdbVertex();
	int ntr = ePrevious->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = AddTrack(*(eWorking),(ePrevious)->GetTrack(i), (ePrevious)->Zpos(i))))
	    {
		(ePrevious->GetTrack(i))->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	  SafeDelete(eWorking);
	  if (ePrevious)
	    {
	      eWorking = ePrevious;
	      eWorking->ResetTracks();
	    }
	    else
	      {
		eVertex->ResetTracks();
	      }
	    printf("Can't create working copy of the vertex!\n");
	    fflush(stdout);
	    return 0;
	}

	if (!MakeV(*(eWorking)))
	{
	  SafeDelete(eWorking);

	    if (ePrevious)
	    {
		eWorking = ePrevious;
		eWorking->ResetTracks();
	    }
	    else
	    {
		eVertex->ResetTracks();
	    }
	    printf("Can't create working copy of the vertex!\n");
	    fflush(stdout);
	    return 0;
	}
    }
    float mass = 0.139; //pion
    EdbTrackP *Tr = new EdbTrackP(s, mass);
    Tr->SetP(Mom);
    Tr->FitTrackKFS();
    float ImpMaxSave = eImpMax;
    eImpMax = ImpMax;
    float ProbMinSave = eProbMin;
    eProbMin = ProbMin;
    if ((vta = AddTrack(*(eWorking), Tr, 1)))
    {
	if ( Tr->Z() >= eWorking->VZ() ) vta->SetZpos(1);
	else vta->SetZpos(0);
	Tr->AddVTA(vta);
	EdbVertex *eW = eWorking;
	eW->SetID(eVertex->ID());
	eW->V()->rmsDistAngle();
	int trind = eEdbTracks->GetEntries();
	Tr->SetID(trind);
	eEdbTracks->Add(Tr);
	Tr->SetSegmentsTrack();
	eImpMax = ImpMaxSave;
	eProbMin = ProbMinSave;
    }
    else
    {
	printf("Track not added! May be Prob < ProbMin. Change ProbMin with 'TrackParams' button!\n");
	fflush(stdout);
	delete Tr;

	SafeDelete(eWorking);
	if (ePrevious)
	{
	    eWorking = ePrevious;
	    eWorking->ResetTracks();
	}
	else
	{
	    eVertex->ResetTracks();
	}
	eImpMax = ImpMaxSave;
	eProbMin = ProbMinSave;
	return 0;
    }
    return 1;
}
//______________________________________________________________________________
int EdbVertexRec::VertexPolish(EdbVertex *v, int refill, float RadMax, int Dpat, float ImpMax, float ProbMin, float Mom)
{
    if (refill) if (!VertexNeighbor(v, RadMax, Dpat, ImpMax)) return 0;

    EdbVTA *vta = 0;
    EdbTrackP *tn = 0;
    EdbSegP *sn = 0;
    EdbVertex *w = 0;

    int nt = v->N();
    int nn = v->Nn();
    int naddtot = 0, nmod = 0;
    int news = 0;
    if (nn)
    {
	// first of all try to propagate existing tracks with new momentum
	double p = Mom;
	int nadd = 0;
	for(int i=0; i<nt; i++)
	{
	    nadd = 0;
	    tn = v->GetTrack(i);
	    for (int ip=0; ip<2; ip++)
	    {
	     p = Mom/(ip+1);
	     tn->SetErrorP(0.2*0.2*p*p);
	     tn->SetP(p);
	     if (v->Zpos(i)) nadd += ePVR->PropagateTrack( *tn, true,  0.01, 3, 0 );
	     else            nadd += ePVR->PropagateTrack( *tn, false, 0.01, 3, 0 );
	    }
	    if (nadd) nmod++;
	    naddtot += nadd;
	}

	// then attach single segments with small impact to the vertex
	eVertex = v;
	for(int i=0; i<nn; i++)
	{
	    vta = eVertex->GetVTn(i);
            if (vta->Flag()  == 1) // neighbour segment
            {
        	sn = (EdbSegP *)vta->GetTrack();
		news += AddSegmentToVertex(sn, ImpMax, ProbMin, Mom);
            }
	}
	w = eVertex;
	if (eWorking != 0) w = eWorking;
	nt = w->N();
	p = Mom;
	nadd = 0;
	// then try to propagate one-segment tracks with new momentum
	for(int i=0; i<nt; i++)
	{
	    nadd = 0;
	    tn = w->GetTrack(i);
	    if (tn->N() > 1) continue;
	    for (int ip=0; ip<2; ip++)
	    {
	     p = Mom/(ip+1);
	     tn->SetErrorP(0.2*0.2*p*p);
	     tn->SetP(p);
	     if (w->Zpos(i)) nadd += ePVR->PropagateTrack( *tn, true,  0.01, 3, 0 );
	     else            nadd += ePVR->PropagateTrack( *tn, false, 0.01, 3, 0 );
	    }
	    if (nadd) nmod++;
	    naddtot += nadd;
	}
    }
    printf("%d single segments are attached, %d tracks are propagated (total %d segments are added).\n",
           news, nmod, naddtot);
    fflush(stdout);
    return (news+nmod);
}
//______________________________________________________________________________
void EdbVertexRec::AcceptPolish()
{
    AcceptModifiedVTX(eVertex, eWorking);
}
//______________________________________________________________________________
void EdbVertexRec::RejectPolish()
{
    CancelModifiedVTX(eVertex, eWorking);
}
//______________________________________________________________________________
int EdbVertexRec::VertexTuning(int seltype)
{
  //if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if (ePVR) if (ePVR->IsA() != EdbPVRec::Class()) ePVR = 0;
  if(!ePVR) {printf("Warning: EdbVertexRec::VertexNeighbor: EdbPVRec not defined, use SetPVRec(...)\n"); return 0;}

  int iv = 0, nntr = 0, nn = 0;
  int nvt = 0;
  int ntr = 0;
  if (eEdbTracks) ntr = eEdbTracks->GetEntries();
  if (!ntr) return 0;
  if (eVTX) nvt = eVTX->GetEntries();
  if (!nvt) return 0;

  EdbVertex *v1 = 0, *v2 = 0;
  EdbVertex *v1n = 0, *v2n = 0;
  EdbVTA *vta = 0;
  EdbTrackP *tr1 = 0, *tr2 = 0;
  int ntr1 = 0, ntr2 = 0, was = 0;
  double impa1[50] = {0.}, chia1[50] = {0.}, imp1max = 0., chi1max = 0.;
  double impa2[50] = {0.}, chia2[50] = {0.}, imp2max = 0., chi2max = 0.;
  double cri1max = 0., cri2max = 0.;
  int it1impmax = -1, it1chimax = -1, it1max = -1;
  int it2impmax = -1, it2chimax = -1, it2max = -1;
  double vchisumorig = 0., vdistsumorig = 0.;
  double v1chiorig = 0., v1distorig = 0.;
  double v2chiorig = 0., v2distorig = 0., crit = 0., critorig = 0.;

  for (iv=0; iv<nvt; iv++) {  // loop on all vertexes
    v1 = GetVertex(iv);
    if (v1)
    {
	    if (v1->Flag() < 0) continue;
	    if ((!(v1->V()))) continue;
	    ntr1 = v1->N();
	    imp1max = -1.;
	    chi1max = -1.;
	    it1max  = -1;
	    for(int it1=0; it1<v1->N() && it1<50; it1++)
	    {
		tr1 = v1->GetTrack(it1);
		impa1[it1] = v1->ImpTrack(it1);
		if (impa1[it1] > imp1max)
		{
		    imp1max = impa1[it1];
		    it1impmax = it1;
		}
		chia1[it1] = v1->Chi2Track(tr1, v1->Zpos(it1), 0.);
		if (chia1[it1] > chi1max)
		{
		    chi1max = chia1[it1];
		    it1chimax = it1;
		}
	    }
	    if (seltype == 0)
	    {
		it1max = it1impmax;
	    }
	    else
	    {
		it1max = it1chimax;
	    }
	    nntr = v1->Nn();
	    v1chiorig = v1->V()->chi2();
	    v1distorig = v1->V()->rmsDistAngle();
	    if (nntr)
	    {
	      for(int i=0; i<nntr; i++)
	      {
		vta = v1->GetVTn(i);
                if (vta->Flag() == 0)    // neighbour track
                {
//                       EdbTrackP *tn = vta->GetTrack();
                }
                else if (vta->Flag()  == 1) // neighbour segment
                {
//                       EdbSegP *sn = (EdbSegP *)vta->GetTrack();
                }
                else if (vta->Flag()  == 3) // neighbour vertex 
                {
                       v2 = (EdbVertex *)vta->GetTrack();
		       if ((!v2) || v2->Flag() == -10) continue;
		       if ((!(v2->V()))) continue;
		       ntr2 = v2->N();
		       v2chiorig = v2->V()->chi2();
		       v2distorig = v2->V()->rmsDistAngle();
		       vchisumorig = v1chiorig + v2chiorig;
		       vdistsumorig = v1distorig + v2distorig;
		       if (seltype == 0)
		       {
			    critorig = vdistsumorig;
		       }
		       else
		       {
			    critorig = vchisumorig;
		       }
		       was = 0;
		       if (ntr1==2 && ntr2>2)
		       {
			    imp2max = 0.;
			    chi2max = 0.;
			    it2max  = -1;
			    for(int it2=0; it2<ntr2 && it2<50; it2++)
			    {
				tr2 = v2->GetTrack(it2);
				impa2[it2] = v2->ImpTrack(it2);
				if (impa2[it2] > imp2max)
				{
				    imp2max = impa2[it2];
				    it2impmax = it2;
				}
				chia2[it2] = v2->Chi2Track(tr2, v2->Zpos(it2), 0.);
				if (chia2[it2] > chi2max)
				{
				    chi2max = chia2[it2];
				    it2chimax = it2;
				}
			    }
			    if (seltype == 0)
			    {
				it2max = it2impmax;
			    }
			    else
			    {
				it2max = it2chimax;
			    }
			    if (it2max < 0) continue;
			    if (v2->GetConnectedVertexForTrack(it2max)==v1) continue;
			    crit = MoveTrackToOtherVertex(v2, it2max, v1, seltype, &v2n, &v1n);
			    was = 1;
		       }
		       else if (ntr1>2 && ntr2==2)
		       {
			    imp1max = 0.;
			    chi1max = 0.;
			    it1max  = -1;
			    for(int it1=0; it1<ntr1 && it1<50; it1++)
			    {
				tr1 = v1->GetTrack(it1);
				impa1[it1] = v1->ImpTrack(it1);
				if (impa1[it1] > imp1max)
				{
				    imp1max = impa1[it1];
				    it1impmax = it1;
				}
				chia1[it1] = v1->Chi2Track(tr1, v1->Zpos(it1), 0.);
				if (chia1[it1] > chi1max)
				{
				    chi1max = chia1[it1];
				    it1chimax = it1;
				}
			    }
			    if (seltype == 0)
			    {
				it1max = it1impmax;
			    }
			    else
			    {
				it1max = it1chimax;
			    }
			    if (it1max < 0) continue;
			    if (v1->GetConnectedVertexForTrack(it1max)==v2) continue;
			    crit = MoveTrackToOtherVertex(v1, it1max, v2, seltype, &v2n, &v1n);
			    was = 1;
		       }
		       else if (ntr1>2 && ntr2>2)
		       {
			    imp2max = 0.;
			    chi2max = 0.;
			    it2max  = -1;
			    for(int it2=0; it2<ntr2 && it2<50; it2++)
			    {
				tr2 = v2->GetTrack(it2);
				impa2[it2] = v2->ImpTrack(it2);
				if (impa2[it2] > imp2max)
				{
				    imp2max = impa2[it2];
				    it2impmax = it2;
				}
				chia2[it2] = v2->Chi2Track(tr2, v2->Zpos(it2), 0.);
				if (chia2[it2] > chi2max)
				{
				    chi2max = chia2[it2];
				    it2chimax = it2;
				}
			    }
			    if (seltype == 0)
			    {
				it2max = it2impmax;
				cri1max = imp1max;
				cri2max = imp2max;
			    }
			    else
			    {
				it2max = it2chimax;
				cri1max = chi1max;
				cri2max = chi2max;
			    }
			    if (it1max < 0) continue;
			    if (it2max < 0) continue;
			    if (cri2max > cri1max)
			    {
				if (v2->GetConnectedVertexForTrack(it2max)==v1) continue;
				crit = MoveTrackToOtherVertex(v2, it2max, v1, seltype, &v2n, &v1n);
				was = 1;
			    }
			    else
			    {
				if (v1->GetConnectedVertexForTrack(it1max)==v2) continue;
				crit = MoveTrackToOtherVertex(v1, it1max, v2, seltype, &v2n, &v1n);
				was = 1;
			    } // variants of changing 
		       } // multiplicities
		       if (was)
		       {
		        if ((crit < critorig) && v1n && v2n)
		        {
			    AcceptModifiedVTX(v1,v1n);
			    AcceptModifiedVTX(v2,v2n);
			    v1 = v1n;
			    v2 = v2n;
		    	    if (v2->Flag() > -11) v2->SetFlag(-v2->Flag()-11);
			    nn++;
			    //break;
		        }
		        else
		        {
			    CancelModifiedVTX(v1,v1n);
			    CancelModifiedVTX(v2,v2n);
		        }
		       }
                } // neighbor vertex
	      } // loop on neighbor
	    } // neighbor exist
    } // good v1
  } // loop on vertexes

  nvt = eVTX->GetEntries();

  for (iv=0; iv<nvt; iv++) {
    v1 = GetVertex(iv);
    if (v1)
    {
	    if (v1->Flag()<-10)
	    {
		v1->SetFlag(-v1->Flag()-11);
	    }
    }
  }

  return nn;
} 
//______________________________________________________________________________
double EdbVertexRec::MoveTrackToOtherVertex(EdbVertex *v2, int it2max, EdbVertex *v1, int seltype,
                                            EdbVertex **v2no, EdbVertex **v1no)
{
//    printf("Rearrange vertexies %d and %d\n",v1->ID(),v2->ID());
    *v1no = 0;
    *v2no = 0;
    if (!v1 || !v2) return 0.;
    if (v2->N() < 3) return 0.;
    if (it2max >= v2->N()) return 0.;
    EdbVertex *v1n = 0, *v2n = 0;
    EdbTrackP *tr2 = 0;
    int zpos = 0;
    double dx, dy, dz, dist1, dist2, imp, vchisum, vdistsum;
    tr2 = v2->GetTrack(it2max);
    dx = v1->VX() - tr2->TrackZmin()->X();
    dy = v1->VY() - tr2->TrackZmin()->Y();
    dz = v1->VZ() - tr2->TrackZmin()->Z();
    dist1 = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
    dx = v1->VX() - tr2->TrackZmax()->X();
    dy = v1->VY() - tr2->TrackZmax()->Y();
    dz = v1->VZ() - tr2->TrackZmax()->Z();
    dist2 = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
    if (dist2 < dist1)
    {
	zpos = 0;
    }
    else
    {
	zpos = 1;
    }
    imp = v1->DistTrack(tr2, zpos);
    if (imp > 1.2*eImpMax) return 0.; 
    if ((v2n = RemoveTrackFromVertex(v2, it2max)))
    {
	*v2no = v2n;
	v1n = AddTrackToVertex(v1, tr2, zpos);
	if (!v1n)
	{
//	    CancelModifiedVTX(v2,v2n);
	    return 0.;
	}
	*v1no = v1n;
	vchisum = v1n->V()->chi2() + v2n->V()->chi2();
	vdistsum = v1n->V()->rmsDistAngle() + v2n->V()->rmsDistAngle();
	if (seltype == 0)
	{
	    return vdistsum;
	}
	else
	{
	    return vchisum;
	}
    }
    return 0.;
}
//______________________________________________________________________________
EdbVertex *EdbVertexRec::AddTrackToVertex(EdbVertex *eVertex, EdbTrackP *eTr, int zpos)
{
    EdbVTA *vta = 0;
    EdbVertex *old = 0;
    EdbVertex *eWorking = 0;
    if (!eVertex)
    {
    
	    printf("No working vertex selected!\n");
	    fflush(stdout);
	    return 0;
    }
    if (!eTr)
    {
    
	    printf("No working track selected!\n");
	    fflush(stdout);
	    return 0;
    }
    if ((old = eTr->VertexS()) && (zpos == 1))
    {
    
	    printf("Track alredy connected to a vertex by this edge!\n");
	    fflush(stdout);
	    return 0;
    }
    if ((old = eTr->VertexE()) && (zpos == 0))
    {
    
	    printf("Track alredy connected to a vertex by this edge!\n");
	    fflush(stdout);
	    return 0;
    }
    if (eWorking == 0)
    {
	eWorking = new EdbVertex();
	int ntr = eVertex->N();
	int i = 0, n = 0;
	for(i=0; i<ntr; i++)
	{
	    if ((vta = AddTrack(*(eWorking), (eVertex)->GetTrack(i), (eVertex)->Zpos(i))))
	    {
		(eVertex)->GetTrack(i)->AddVTA(vta);
		n++;
	    }
	}
	if (n < 2)
	{
	  SafeDelete(eWorking);
	  (eVertex)->ResetTracks();
	  printf("Can't create working copy of the vertex!\n");
	  fflush(stdout);
	  return 0;
	}

	if (!MakeV(*eWorking))
	{
	  SafeDelete(eWorking);
	  eVertex->ResetTracks();
	  printf("Can't create working copy of the vertex!\n");
	  fflush(stdout);
	  return 0;
	}
    }
    if ((vta = AddTrack(*eWorking, eTr, zpos)))
    {
	eTr->AddVTA(vta);
	eWorking->SetID(eVertex->ID());
    }
    else
    {
//	printf("Track not added! May be Prob < ProbMin.\n");
//	fflush(stdout);
      SafeDelete(eWorking);
      eVertex->ResetTracks();
      return 0;
    }
    return eWorking;
}
//_____________________________________________________________________________
EdbVertex *EdbVertexRec::RemoveTrackFromVertex(EdbVertex *eVertex, int itr)
{
    if (!eVertex)
    {
    
	    printf("No working vertex selected!\n");
	    fflush(stdout);
	    return 0;
    }
    EdbVTA *vta = 0;
    EdbVertex *eWorking = 0;
    int n = 0;
    int ntr = 0;
    if (eWorking == 0)
    {
	ntr = eVertex->N();
	if (ntr < 3)
	{
    
	    printf("Working vertex has 2 prongs only!\n");
	    fflush(stdout);
	    return 0;
	}
	eWorking = new EdbVertex();
	int i = 0;
	for(i=0; i<ntr; i++)
	{
	    if (i == itr)
	    {
		(eVertex->GetTrack(i))->ClearVTA(eVertex->GetVTa(i));
		continue;
	    }
	    if ((vta = AddTrack( *eWorking, eVertex->GetTrack(i), eVertex->Zpos(i))))
	    {
		(eWorking->GetTrack(n))->AddVTA(vta);
		n++;
	    }
	}
    }
    if ((n < 2)||(n == ntr))
    {
	printf("Can't create working copy of the vertex!\n");
	fflush(stdout);
	SafeDelete(eWorking);
	eVertex->ResetTracks();
	return 0;
    }

    if (MakeV(*eWorking))
    {
	EdbVertex *eW = eWorking;
	eW->ResetTracks();
	eW->SetID(eVertex->ID());
    }
    else
    {
	printf("Can't create working copy of the vertex!\n");
	fflush(stdout);
	SafeDelete(eWorking);
	eVertex->ResetTracks();
	return 0;
    }
    return eWorking;
}
//_____________________________________________________________________________
void EdbVertexRec::AcceptModifiedVTX(EdbVertex *eVertex, EdbVertex *eWorking)
{
    if (eWorking && eVertex)
    {
        EdbVertex *eW = eWorking;
	int ind = eVertex->ID();
	eW->SetID(ind);
	eW->SetQuality(eW->V()->prob()/
			   (eW->V()->vtx_cov_x()+eW->V()->vtx_cov_y()));
	int ntr = eVertex->N();
	for(int i=0; i<ntr; i++)
	{
	    eVTA.Remove(eVertex->GetVTa(i));
	}

	eW->ResetTracks();
	ntr = eW->N();
	int ifl = 0;
	for(int i=0; i<ntr; i++)
	{
		if (eW->Zpos(i)) ifl = ifl | 1; 
		else		 ifl = ifl | 2; 
	}
	ifl = 4 - ifl;
	if (ifl == 3) ifl = 0;
	if (eW->Nv()) ifl += 3;
	eW->SetFlag(ifl);
	if (eVTX) eVTX->AddAt(eW, ind);
	for(int i=0; i<ntr; i++)
	{
	    AddVTA(eW->GetVTa(i));
	}
	ntr = eVertex->Nn();
	for(int i=0; i<ntr; i++)
	{
	    eW->AddVTA(eVertex->GetVTn(i));
	}
//	if (eVertex) delete eVertex;
	eW->ResetTracks();
    }
}
//_____________________________________________________________________________
void EdbVertexRec::CancelModifiedVTX(EdbVertex *eVertex, EdbVertex *eWorking)
{
  SafeDelete(eWorking);
  if (eVertex) eVertex->ResetTracks();
}
//______________________________________________________________________________
int EdbVertexRec::VertexNeighbor(float RadMax, int Dpat, float ImpMax)
{
  //if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if (ePVR) if (ePVR->IsA() != EdbPVRec::Class()) ePVR = 0;
  if(!ePVR) {printf("Warning: EdbVertexRec::VertexNeighbor: EdbPVRec not defined, use SetPVRec(...)\n"); return 0;}

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
    v = GetVertex(iv);
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
  //if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if (ePVR) if (ePVR->IsA() != EdbPVRec::Class()) ePVR = 0;
  if(!ePVR) {printf("Warning: EdbVertexRec::VertexNeighbor: EdbPVRec not defined, use SetPVRec(...)\n"); return 0;}

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
  Float_t   Zbin = TMath::Abs((ePVR->GetPattern(1))->Z() - (ePVR->GetPattern(0))->Z());
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
		if (ve && ve->Flag() >= -99 && ve->Flag() != -10)
		{
		    dx = ve->VX() - xvert;
		    dy = ve->VY() - yvert;
		    if (TMath::Sqrt(dx*dx + dy*dy) > RadMax) continue;
		    dz = ve->VZ() - zvert;
		    if (TMath::Abs(dz) > Dpat*Zbin) continue;
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
			    ss = trv->TrackZmax();
			else
			    ss = trv->TrackZmin();
			dx = ss->X() - xvert;
			dy = ss->Y() - yvert;
			dz = ss->Z() - zvert;
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
				ve = tr->VertexS(); 
				if (ve && ve->Flag() >= -99 && ve->Flag() != -10 && ve != v)
				{
				    dx = ve->VX() - xvert;
				    dy = ve->VY() - yvert;
				    if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				    {
					dz = TMath::Abs(ve->VZ() - zvert);
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
						dx = ss->X() - xvert;
						dy = ss->Y() - yvert;
						dz = ss->Z() - zvert;
						if (TMath::Sqrt(dx*dx+dy*dy) > RadMax)    continue;
						if (TMath::Abs(dz)        > Dpat*Zbin) continue;
						dist = TMath::Sqrt(dx*dx+dy*dy+dz*dz);
						vta = v->CheckImp(trv, 10.e+10, ve->Zpos(j), dist);
    						if (vta) trv->SetMC(-trv->MCEvt()-2000, trv->MCTrack());
					    }
					}
				    }
				}
				ve = tr->VertexE(); 
				if (ve && ve->Flag() >= -99 && ve->Flag() != -10 && ve != v)
				{
				    dx = ve->VX() - xvert;
				    dy = ve->VY() - yvert;
				    if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				    {
					dz = TMath::Abs(ve->VZ() - zvert);
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
						dx = ss->X() - xvert;
						dy = ss->Y() - yvert;
						dz = ss->Z() - zvert;
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
	    ntr = v->N();
	    for(int i=0; i<ntr; i++)
	    {
		tr = v->GetTrack(i);
		tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
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
			    if (v->DistSeg((EdbSegP *)ss) > ImpMax) continue;
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
int EdbVertexRec::SegmentNeighbor(EdbSegP *s, float RadMax, int Dpat, float ImpMax, TObjArray *arrs, TObjArray *arrt, TObjArray *arrv)
{
  //if(!ePVR) ePVR = ((EdbPVRec *)(gROOT->GetListOfSpecials()->FindObject("EdbPVRec")));
  if (ePVR) if (ePVR->IsA() != EdbPVRec::Class()) ePVR = 0;
  if(!ePVR) {printf("Warning: EdbVertexRec::SegmentNeighbor: EdbPVRec not defined, use SetPVRec(...)\n"); return 0;}

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
				    if (Tdistance(*(const EdbSegP *)s, *se)           > ImpMax)    continue;
				}
				else
				{
				    if (TMath::Sqrt(distxs+distys) > RadMax)    continue;
				    if (TMath::Abs(distzs1)        > Dpat*Zbin) continue;
				    if (Tdistance(*(const EdbSegP *)s, *ss)           > ImpMax)    continue;
				}
			    }
			    if (arrt) arrt->Add(tr);
			    nn++;
//--------------------------Add vertex neighborhood
    			    tr->SetMC(-tr->MCEvt()-2000, tr->MCTrack());
			    ve = tr->VertexS(); 
			    if (ve && ve->Flag() >= -99 && ve->Flag() != -10)
			    {
				dx = ve->VX() - xseg;
				dy = ve->VY() - yseg;
				if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				{
				    dz = ve->VZ() - zseg;
				    if (TMath::Abs(dz) <= Dpat*Zbin)
				    {
					ve->SetFlag(-ve->Flag()-200);
					if (arrv) arrv->Add(ve);
					for(int j=0; j<ve->N(); j++)
					{
					    trv = ve->GetTrack(j);
					    if (trv->MCEvt() < -999) continue;
					    if (ve->Zpos(j) == 0)
					        ss = trv->TrackZmax();
					    else
					        ss = trv->TrackZmin();
					    dx = ss->X() - xseg;
					    dy = ss->Y() - yseg;
					    dz = ss->Z() - zseg;
					    if (TMath::Sqrt(dx*dx+dy*dy) > RadMax)    continue;
					    if (TMath::Abs(dz)        > Dpat*Zbin) continue;
					    //if (TDistance(*(const EdbSegP *)s, *ss)      > ImpMax)    continue;
					    if (arrt) arrt->Add(trv);
    					    trv->SetMC(-trv->MCEvt()-2000, trv->MCTrack());
					}
				    }
				}
			    }
			    ve = tr->VertexE(); 
			    if (ve && ve->Flag() >= -99 && ve->Flag() != -10)
			    {
				dx = ve->VX() - xseg;
				dy = ve->VY() - yseg;
				if (TMath::Sqrt(dx*dx + dy*dy) <= RadMax)
				{
				    dy = ve->VZ() - zseg;
				    if (TMath::Abs(dz) <= Dpat*Zbin)
				    {
					ve->SetFlag(-ve->Flag()-200);
					if (arrv) arrv->Add(ve);
					for(int j=0; j<ve->N(); j++)
					{
					    trv = ve->GetTrack(j);
					    if (trv->MCEvt() < -999) continue;
					    if (ve->Zpos(j) == 0)
					        ss = trv->TrackZmax();
					    else
					        ss = trv->TrackZmin();
					    dx = ss->X() - xseg;
					    dy = ss->Y() - yseg;
					    dz = ss->Z() - zseg;
					    if (TMath::Sqrt(dx*dx+dy*dy) > RadMax) continue;
					    if (TMath::Abs(dz)        > Dpat*Zbin) continue;
					    //if (TDistance(*(const EdbSegP *)s, *ss)      > ImpMax)    continue;
					    if (arrt) arrt->Add(trv);
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
	    //nvn = SelSegNeighbor(s, 1, RadMax, Dpat, arrs); 
	    an.Clear();
	    nvn = SelSegNeighbor(s, 1, RadMax, Dpat, &an); 
	    nvn = an.GetEntries();
	    for (int is=0; is<nvn; is++) {
		    ss = (EdbSegP*)(an.At(is));
		    if (ss)
		    {
			    if (ss != s)
			    {
				if (Tdistance(*(const EdbSegP *)s, *ss) > ImpMax) continue;
				arrs->Add((TObject *)ss);
				nn++;
			    }
		    }
	    }
//	    printf("Selected %d segments\n", nvn);
//	    nn += nvn;
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
double EdbVertexRec::TdistanceChi2(const EdbTrackP &tr1, const EdbTrackP &tr2) 
{
  EdbVertex edbv;
  Track t1;
  Track t2;
  float X0 =  ePVR->GetScanCond()->RadX0();
  if(!edbv.Edb2Vt(tr1,t1,X0,tr1.M()))  return 1.E20;
  if(!edbv.Edb2Vt(tr2,t2,X0,tr2.M()))  return 1.E20;
  return distanceChi2(t1,t2);
}

//________________________________________________________________________
double EdbVertexRec::TdistanceChi2(const EdbSegP &s1, const EdbSegP &s2, float m) 
{
  EdbVertex edbv;
  Track t1;
  Track t2;
  float X0 =  ePVR->GetScanCond()->RadX0();
  if(!edbv.Edb2Vt(s1,t1,X0,m))  return 1.E20;
  if(!edbv.Edb2Vt(s2,t2,X0,m))  return 1.E20;
  return distanceChi2(t1,t2);
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
