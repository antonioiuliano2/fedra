//-- Author :  Valeri Tioukov   16.03.2003
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPVGen - Patterns Volume Generator                                 //
//                                                                      //
//  collection of agorithms                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TRandom.h"
#include "TMath.h"
#include "EdbPVGen.h"
#include "EdbPhys.h"
#include "EdbVertex.h"

ClassImp(EdbPVGen)

//______________________________________________________________________________________
EdbPVGen::EdbPVGen()
{
  eTracks = 0;
  eVTX    = 0;
  eScanCond = 0;
}

///______________________________________________________________________________
EdbPVGen::~EdbPVGen()
{
  if(eTracks)         delete eTracks;
  if(eVTX)            delete eVTX;
}

///______________________________________________________________________________
void EdbPVGen::GenerateBeam( int n, float xx[4], float sxx[4], float lim[4], float z0, int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  Float_t x0,y0,x,y,z,tx,ty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = new EdbSegP();

  int j=0;
  while(j<n) {

    x0 = xx[0] + sxx[0] * gRandom->Gaus();
    y0 = xx[1] + sxx[1] * gRandom->Gaus();
    if(x0<lim[0])                      continue; 
    if(y0<lim[1])                      continue; 
    if(x0>lim[2])                      continue; 
    if(y0>lim[3])                      continue; 
    j++;
    tx = xx[2] + sxx[2] * gRandom->Gaus();
    ty = xx[3] + sxx[3] * gRandom->Gaus();      // starting point (at z0)

    for (int i=0; i<pv->Npatterns(); i++ ) {

      pat = pv->GetPattern(i);
      z = pat->Z();

      x = x0 + tx*(z-z0);
      y = y0 + ty*(z-z0);

      seg->Set(j, x, y, tx, ty,1,0);
      seg->SetZ(z);
      seg->SetFlag(flag);
      pat->AddSegment( *seg );

    }
  }
}

//______________________________________________________________________________________
void EdbPVGen::GenerateBackground( int n, float xmin[4], float xmax[4], int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  Float_t x,y,z,tx,ty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = new EdbSegP();

  for( int j=0; j<n; j++ ) {

    for (int i=0; i<pv->Npatterns(); i++ ) {

      pat = pv->GetPattern(i);
      z = pat->Z();

      x  = xmin[0] + (xmax[0]-xmin[0]) * gRandom->Rndm();
      y  = xmin[1] + (xmax[1]-xmin[1]) * gRandom->Rndm();
      tx = xmin[2] + (xmax[2]-xmin[2]) * gRandom->Rndm();
      ty = xmin[3] + (xmax[3]-xmin[3]) * gRandom->Rndm();

      seg->Set(j, x, y, tx, ty,1,0);
      seg->SetZ(z);
      seg->SetFlag(flag);
      pat->AddSegment( *seg );

    }
  }
}

//______________________________________________________________________________
void EdbPVGen::GeneratePulsPoisson( float mean, float a, 
				    float wmin, float wmax, int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;
  float w;

  for( int i=0; i<pv->Npatterns(); i++ ) {
    pat = pv->GetPattern(i);

    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);
      if(flag>0)  if( seg->Flag() != flag)  continue;

    REGEN:
      w = a * gRandom->Poisson(mean);
      if(wmin>0) if(w<wmin) goto REGEN;
      if(wmax>0) if(w>wmax) w = wmax;

      seg->SetW( w );
    }
  }
}

//_________________________________________________________________________________________
void EdbPVGen::GeneratePulsGaus( float amp, float mean, float sigma, 
				 float wmin, float wmax, int flag )
{
  EdbPatternsVolume *pv = GetVolume();

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;
  float w;

  for( int i=0; i<pv->Npatterns(); i++ ) {
    pat = pv->GetPattern(i);

    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);
      if(flag>0)  if( seg->Flag() != flag)  continue;
      
    REGEN:
      w = (int)(amp * gRandom->Gaus(mean,sigma));
      if(wmin>0) if(w<wmin) goto REGEN;
      if(wmax>0) if(w>wmax) w = wmax;

      seg->SetW( w );
    }
  }
}

//______________________________________________________________________________
void EdbPVGen::SmearSegments()
{
  // smearing with parameters defined by ScanCond

  EdbPatternsVolume *pv = GetVolume();
  EdbScanCond *scan = eScanCond;

  Float_t x,y,z,tx,ty;
  Float_t sx,sy,sz,stx,sty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;

  for( int i=0; i<pv->Npatterns(); i++ ) {
    pat = pv->GetPattern(i);

    for( int j=0; j<pat->N(); j++ ) {
      seg = pat->GetSegment(j);

      sx  = scan->SigmaX(seg->TX());
      sy  = scan->SigmaY(seg->TY());
      sz  = 0.;
      stx = scan->SigmaTX(seg->TX());
      sty = scan->SigmaTY(seg->TY());

      x  = gRandom->Gaus(seg->X(),   sx);
      y  = gRandom->Gaus(seg->Y(),   sy);
      z  = gRandom->Gaus(seg->Z(),   sz);
      tx = gRandom->Gaus(seg->TX(), stx);
      ty = gRandom->Gaus(seg->TY(), sty);

      seg->Set( seg->ID(), x,y,tx,ty,seg->W(),seg->Flag());
      seg->SetZ( z );
      //seg->SetErrors( sx,sy,sz,stx,sty );

    }
  }
}

//______________________________________________________________________________
void EdbPVGen::SmearPatterns(float shift, float rot)
{
  EdbPatternsVolume *pv = GetVolume();
  EdbAffine2D a;

  for( int i=0; i<pv->Npatterns()-1; i++ ) {

    a.Rotate( gRandom->Gaus(0, rot)   );
    a.ShiftX( gRandom->Gaus(0, shift) );
    a.ShiftY( gRandom->Gaus(0, shift) );

    pv->GetPattern(i)->Transform(&a);
  }

}
//______________________________________________________________________________
void EdbPVGen::TrackMC( float zlim[2], float lim[4], 
			EdbTrackP &tr, int eloss_flag, float PGap)
{
  // Segments generation for single MC track with taking into account MS and 
  //  XY errors correllation

  // Input parameters:
  //                   zlim  - Z limits ( if zlim[0] > zlim[1] then direction of
  //			       tracking is opposit Z-axis )  
  //                   lim   - XY limits
  //		       sigma - measuring sigma (x,y,tx,ty)
  //                   tr    - segments generated according to track parameters
  //
  // Output:  fill associated volume with segments
  //          tr - add generated segments to the tr

  bool exact_angles = false;   // exact new angles calculation after MS  
  bool x_y_correlation = true; // take into account x-y measurement correlation
  Float_t eTPb = 1000./1300.;
  Float_t x0  =tr.X();
  Float_t y0  =tr.Y();
  Float_t z0  =tr.Z();
  Float_t tx0 =tr.TX();
  Float_t ty0 =tr.TY();
  Float_t p0  =tr.P();
  Float_t m   =tr.M();
  Float_t sx=eScanCond->SigmaX(0.), sy=eScanCond->SigmaY(0.);
  Float_t stx=eScanCond->SigmaTX(0.), sty=eScanCond->SigmaTY(0.);
  Float_t sxd=sx, syd=sy, stxd=stx, styd=sty;
  Float_t x,y,z,tx,ty, xs, ys, txs, tys;
  Float_t dz, dzm, cost, r1, r2, teta0, zold, dzPb;
  Float_t dx = 0., dy = 0., dtx = 0., dty = 0.;
  Float_t dxs = 0., dys = 0., dtxs = 0., dtys = 0.;
  Double_t Phi,CPhi,SPhi;
  Float_t X0 = eScanCond->RadX0();
  EdbPattern *pat = 0;
  int segnum = 0;

  if ( x0 < lim[0] ) return; 
  if ( y0 < lim[1] ) return; 
  if ( x0 > lim[2] ) return; 
  if ( y0 > lim[3] ) return;

  EdbSegP    *seg = new EdbSegP();

  Float_t p = p0, pa = p0, pn, de = 0., DE = 0.;
  Float_t e = TMath::Sqrt((double)p*(double)p+(double)m*(double)m);
  x = x0;
  y = y0;
  tx = tx0;
  ty = ty0;
  zold = z0;
  int npat = ePVolume->Npatterns();
  int k = 0, dir = 0;
  for (int kc=0; kc<npat; kc++ ) {
    if (zlim[0] > zlim[1])
    {
	k = npat - 1 - kc;
	dir = -1;
    }
    else
    {
	k = kc;
	dir = +1;
    }
    pat = ePVolume->GetPattern(k);
    z = pat->Z();
    segnum = pat->N();

    if ( dir > 0 ) if ( z < zlim[0] ) { continue; }
    if ( dir > 0 ) if ( z > zlim[1] ) { continue; }
    if ( dir < 0 ) if ( z > zlim[0] ) { continue; }
    if ( dir < 0 ) if ( z < zlim[1] ) { continue; }
    if ( dir > 0 ) if ( z < zold ) { continue; }
    if ( dir < 0 ) if ( z >= zold ) { continue; }
    if ( gRandom->Rndm() < (double)PGap) { continue; }

    dz = z - zold;
    cost = TMath::Sqrt((double)1.+(double)tx*(double)tx+(double)ty*(double)ty);
    if (cost >= 10.) break;
    dzm = dz*cost;
    dzPb = eTPb*dzm;
    if (eloss_flag == 1)
    {
	de = EdbPhysics::DeAveragePb(p, m, TMath::Abs(dzPb));
	if ( de < 0.) de = 0;
	e  = e - de;
	if (e < m) e = m;
	pn = TMath::Sqrt((double)e*(double)e - (double)m*(double)m);
	pa = 0.5*(p+pn);
	p  = pn;
	DE += de;
	
    }
    else if (eloss_flag == 2)
    {
	de = EdbPhysics::DeLandauPb(p, m, TMath::Abs(dzPb));
	if ( de < 0.) de = 0;
	e  = e - de;
	if (e < m) e = m;
	pn = TMath::Sqrt((double)e*(double)e - (double)m*(double)m);
	pa = 0.5*(p+pn);
	p  = pn;
	DE += de;
    }
    if (kc)
      {
	teta0 = EdbPhysics::ThetaMS2( pa, m, dzm, X0);
	teta0 = TMath::Sqrt(teta0);
	do  { r1 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
	do  { r2 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
	dx = (0.5*r1+0.866025*r2)*dzm*teta0/1.73205;
	dtx = r2*teta0;
	do  { r1 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
	do  { r2 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
	dy = (0.5*r1+0.866025*r2)*dzm*teta0/1.73205;
	dty = r2*teta0;
      }
    else
      {
        teta0 = 0.;
	dx = 0.;
	dy = 0.;
	dtx = 0.;
	dty = 0.;
      }
    x = x + tx*dz + dx;
    y = y + ty*dz + dy;
    if (exact_angles)
    {
	tx = TMath::Tan(TMath::ATan(tx) + dtx);
	ty = TMath::Tan(TMath::ATan(ty) + dty);
    }
    else
    {
	tx = tx + dtx;
	ty = ty + dty;
    }
    zold = z;
    if ( x < lim[0] ) break; 
    if ( y < lim[1] ) break; 
    if ( x > lim[2] ) break; 
    if ( y > lim[3] ) break;
    do  { r1 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
    sxd = eScanCond->SigmaX(tx);
    dxs = sxd*r1;
    do  { r1 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
    if (!x_y_correlation) syd = eScanCond->SigmaY(ty);
    else		  syd = sy;
    dys = syd*r1;
    do  { r1 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
    stxd = eScanCond->SigmaTX(tx);
    dtxs = stxd*r1;
    do  { r1 = gRandom->Gaus();} while (TMath::Abs(r1) > 50.);
    if (!x_y_correlation) styd = eScanCond->SigmaTY(ty);
    else		  styd = sty;
    dtys = styd*r1;
    if (x_y_correlation)
    {
	Phi = -TMath::ATan2(ty,tx);          // azimuthal angle of the track
	CPhi = TMath::Cos(Phi);
	SPhi = TMath::Sin(Phi);
	xs = x + dxs*CPhi - dys*SPhi;        // smearing of track parameters with corellation 
	ys = y + dxs*SPhi + dys*CPhi;        // along the track projection to XY plane 
	txs = tx + dtxs*CPhi - dtys*SPhi;    // (regression line)
	tys = ty + dtxs*SPhi + dtys*CPhi;
    }
    else
    {
	xs = x + dxs;        // smearing of track parameters with corellation 
	ys = y + dys;        // along the track projection to XY plane 
	txs = tx + dtxs;    // (regression line)
	tys = ty + dtys;
    }
    seg->Set(segnum++, xs, ys, txs, tys, 25., tr.ID());
    seg->SetP(pa);
    seg->SetZ(z);
    seg->SetDZ(300.);
    seg->SetPID(k);
    if (x_y_correlation)
	seg->SetErrorsCOV(sxd*sxd, syd*syd, 0., stxd*stxd, styd*styd, 0.);
    else
	seg->SetErrors(sxd*sxd, syd*syd, 0., stxd*stxd, styd*styd, 0.);

    tr.AddSegment( pat->AddSegment(*seg) );
    if (p <= 0.050) break;
    if (teta0 >= 1.00) break;
  }
  tr.SetDE(DE);
  tr.SetCounters();
  tr.SetSegmentsTrack();
  delete seg;
}
//______________________________________________________________________________
void EdbPVGen::GenerateUncorrelatedSegments(int nb, float lim[4],
					    float TetaMax, int flag )
{
  // generation of uncorrelated segments background

  float x, y, z, tx, ty, costmin = TMath::Cos(TetaMax);
  float cost, phi, ex, ey, ez, sint;
  int segnum = 0;
  EdbSegP *s=0;
  EdbPattern *pat = 0;
  EdbPatternsVolume *pv = GetVolume();
  s = new EdbSegP();

  Float_t sx=eScanCond->SigmaX(0.), sy=eScanCond->SigmaY(0.);
  Float_t stx=eScanCond->SigmaTX(0.), sty=eScanCond->SigmaTY(0.);
 
  for(int np=0; np<pv->Npatterns(); np++)
  {
    pat = pv->GetPattern(np);
    z = pat->Z();
    segnum = pat->N();
    for(int i=0; i<nb; i++) {

	x = lim[0] + (lim[2] - lim[0])*gRandom->Rndm();
	y = lim[1] + (lim[3] - lim[1])*gRandom->Rndm();

	cost = costmin + (1. - costmin)*gRandom->Rndm();
	sint = TMath::Sqrt(1. - cost*cost);
	phi  = TMath::TwoPi()*gRandom->Rndm();
	ex = sint*TMath::Cos(phi);
	ey = sint*TMath::Sin(phi);
	ez = cost;
	tx = ex/ez;
	ty = ey/ez;
	s->Set(segnum++, x, y, tx, ty, 1, flag);
	s->SetZ(z);
	s->SetP(4.);
	s->SetPID(np);
	s->SetDZ(290.);
	tx = TMath::Sqrt(tx*tx+ty*ty);
	sx=eScanCond->SigmaX(tx);
	sy=eScanCond->SigmaY(0.);
	stx=eScanCond->SigmaTX(tx);
	sty=eScanCond->SigmaTY(0.);
	s->SetErrorsCOV(sx*sx, sy*sy, 0., stx*stx, sty*sty, 0.);

        pat->AddSegment( *s );
    }
  }
  delete s;
}

//------------------------------------------------------------
void EdbPVGen::GenerateBackgroundTracks(int nb, float vlim[4], float lim[4],
					float plim[2], float TetaMax,
					float ProbGap, int eloss_flag )
{
  // event generation
  float x, y, z, costmin = TMath::Cos(TetaMax);
  float cost, sint, phi, tx, ty, ex, ey, ez, p;
  float zlim[2];
  EdbTrackP *trb = 0;
  EdbPatternsVolume *pv = GetVolume();

  zlim[0] = (pv->GetPattern(0))->Z();
  int np  = pv->Npatterns();
  zlim[1] = (pv->GetPattern(np-1))->Z();

  int id = 0;
  if (eTracks) id = eTracks->GetEntries();

  for(int i=0; i<nb; i++) {

	x = vlim[0] + (vlim[2] - vlim[0])*gRandom->Rndm();
	y = vlim[1] + (vlim[3] - vlim[1])*gRandom->Rndm();
	z = zlim[0];

	cost = costmin + (1. - costmin)*gRandom->Rndm();
	sint = TMath::Sqrt(1. - cost*cost);
	phi  = TMath::TwoPi()*gRandom->Rndm();
	ex = sint*TMath::Cos(phi);
	ey = sint*TMath::Sin(phi);
	ez = cost;
	tx = ex/ez;
	ty = ey/ez;
	trb = new EdbTrackP();
	trb->Set(id++, x, y, tx, ty, 1, 0);
	trb->SetZ(z);
	trb->SetPID(0);
	p = plim[0] + (plim[1] - plim[0])*gRandom->Rndm();
	trb->SetP(p);
	trb->SetM(0.139);

	// generation of background track segments

	TrackMC( zlim, lim, *trb, eloss_flag, ProbGap );
//	delete trb;
	AddTrack(trb);
  }

}

//______________________________________________________________________________
void EdbPVGen::GeneratePhaseSpaceEvents( int nv, TGenPhaseSpace *pDecay,
					 float vzlim[2],   float vlim[4],
					 float lim[4],     float ProbGap,
					 int eloss_flag,   int *charges )
{
  // Phase Space event generation
  float x;
  float y;
  float z;
  float zlimt[2];
  EdbVertex *vedb = 0;
  EdbTrackP *tr = 0; 
  Double_t    weight, tx, ty;
  TLorentzVector *pi = 0;
  int numt = 0, nt = 0, ntrgood = 0;
  double pxs,pys,pzs,es, p2;
  float mp, pp;

  if (eTracks) numt = eTracks->GetEntries();

  int nvmod = nv;

  for(int iv=0; iv<nvmod; iv++) {
    vedb = new EdbVertex();
    x = vlim[0] + (vlim[2] - vlim[0])*gRandom->Rndm();
    y = vlim[1] + (vlim[3] - vlim[1])*gRandom->Rndm();
    z = vzlim[0] + (vzlim[1] - vzlim[0])*gRandom->Rndm();
    vedb->SetXYZ(x, y, z);

    weight = pDecay->Generate();

    nt = pDecay->GetNt();
    pxs = 0.;
    pys = 0.;
    pzs = 0.;
    es = 0.;
    ntrgood = 0;

    for(int ip=0; ip<nt; ip++) {
	pi = pDecay->GetDecay(ip);
	pxs += pi->Px();
	pys += pi->Py();
	pzs += pi->Pz();
	es  += pi->E();
	if (!charges[ip]) continue;
	tr = new EdbTrackP();
	tx = pi->Px()/pi->Pz();
	ty = pi->Py()/pi->Pz();
	tr->Set(numt++, x, y, (float)tx, (float)ty, 1, iv+1);
	tr->SetZ(z);
	tr->SetP(pi->P());
	tr->SetM(pi->M());

	// generation of track segments for secondary particles

	if (pi->Pz() >= 0.)
	{
	    zlimt[0] = z;
	    zlimt[1] = 200000.;
	}
	else
	{
	    zlimt[0] = z;
	    zlimt[1] = -1000.;
	}
	TrackMC( zlimt, lim, *tr, eloss_flag, ProbGap);
	if (tr->N() > 0)
	{
	    if (pi->Pz() < 0.)
		vedb->AddTrack(tr, 0);
	    else
		vedb->AddTrack(tr, 1);
	    AddTrack(tr);
	    ntrgood++;
	}
	else
	{
	    delete tr;
	    tr = 0;
	}
    }

    // generate primary track
    if (z > 300. && charges[nt])
    {
	p2 = pxs*pxs + pys*pys + pzs*pzs;
	pp = TMath::Sqrt(p2);
	mp = TMath::Sqrt(es*es - p2); 
	tr = new EdbTrackP();
	tr->Set(numt++, x, y, 0., 0., 1, iv+1);
	tr->SetZ(0);
	tr->SetP(pp);
//	tr->SetP(1000000.);
	tr->SetM(mp);

	// generation of track segments for primary particle

	zlimt[0] = z;
	zlimt[1] = -1000.;
	TrackMC( zlimt, lim, *tr, 0, ProbGap );
	if (tr->N() > 0)
	{
	    vedb->AddTrack(tr, 0);
	    AddTrack(tr);
	    ntrgood++;
	}
	else
	{
	    delete tr;
	    tr = 0;
	}
    }

    if (ntrgood == 0)
    {
	delete vedb;
	vedb = 0;
	nvmod++;
    }
    else
    if (ntrgood == 1)
    {
	delete vedb;
	vedb = 0;
	if (tr) tr->SetFlag(0);
	nvmod++;
    }
    else
    {
	AddVertex(vedb);
    }
  }
}
//______________________________________________________________________________
int EdbPVGen::MakeTracksMC(int nsegments, TObjArray *tracks)
{
  // use MC data and form tracks array

  if (!eTracks) return 0;
  if (!tracks) return 0;
  tracks->Delete();
  tracks->Clear();

  printf("make tracks from MC information...\n");

  int         nseg, ntr=0, ntrg = 0, n0 = 0;
  EdbSegP    *seg = 0;
  EdbTrackP  *track = 0, *trackg = 0;

  ntrg=eTracks->GetEntries();
  for(int it=0; it<ntrg; it++) {

    trackg = (EdbTrackP *)(eTracks->At(it));
    if (!trackg) continue;
    nseg = trackg->N();
    if( nseg < nsegments ) continue;
    track = new EdbTrackP(nseg);

    track->SetNpl( TMath::Abs(trackg->GetSegmentLast()->PID() -
			      trackg->GetSegmentFirst()->PID()) + 1 );

    n0=0;
    for(int is=0; is<nseg; is++) {
      seg = trackg->GetSegment(is);
      if(seg->Flag()<0) n0++;
      track->AddSegment(seg);
    }
    track->SetN0(n0);
    track->SetID(it);
    tracks->Add(track);
    ntr++;
  }

  printf("%d tracks with >= %d segments are selected\n",ntr, nsegments);
  return ntr;
}

//______________________________________________________________________________
