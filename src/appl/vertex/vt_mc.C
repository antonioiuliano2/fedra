#include <iostream.h>

VtVector *p;
CMatrix *c;
Vertex *v;

Track *trlist[100];
int NTrack = 0;
Vertex *vtlist[100];
int NVertex = 0;
float vx_mc = 0., vy_mc = 0., vz_mc = 0.;

/*--------------------------------------------------------------------------*/
void vt_mc_init(void)
{
//    gDebug = 3;
//    gROOT->ProcessLine(".debug Vertex");
      BookHists();
      cout << "Initialized!" << endl;
}
/*--------------------------------------------------------------------------*/
void vt_mc_find(void)
{
    if (NTrack <= 1) return;
    v = 0;
    v = new Vertex(*trlist[0], *trlist[1]);
    int i = 2;
    while (i<NTrack) { v->push_back(*trlist[i]); i++;}
    v->findVertexVt();
    if (v->valid())
    {
//	cout << "Valid vertex! " << endl;
/*	cout << "Vertex XYZ is "
	     << v->vx()
	     << ", "
	     << v->vy()
	     << ", "
	     << v->vz()
	     << ", Chi2 is "
	     << v->chi2()
	     << ", Ntracks is "
	     << v->ntracks() << endl; */
    }
    else
    {
	cout << "InValid vertex!" << endl;
    };
//    delete v;
//    v = 0;
    for (int i = 0; i < NTrack; i++);
    {
	delete trlist[i];
	trlist[i] = 0;
    }
//    cout << "End of Vt++ test." << endl;
//    gROOT->Reset();
}
/*--------------------------------------------------------------------------*/
void VertexMC()
{
  // smearing with parameters defined by ScanCond

  EdbPatternsVolume *pv = ali;
  EdbScanCond *scan = scanCond_mc;

  Float_t x,y,z,tx,ty;
  Float_t sx,sy,sz,stx,sty;

  EdbPattern *pat = 0;
  EdbSegP    *seg = 0;

  NTrack = 0;
  
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
/*    
      cout << "x = "
           <<  x
           << ", "
           << "y = "
           <<  y
           << ", "
           << "z = "
           <<  z
           << ", "
           << "tx = "
           <<  tx
           << ", "
           << "ty = "
           <<  ty
           << endl;    */

      p = new VtVector( x,  y,  z,  tx,  ty, 1.);
      c = new CMatrix();
      c->set_x(sx*sx);   //Sx
      c->set_y(sy*sy);   //Sy
      c->set_tx(stx*stx);  //Stx
      c->set_ty(sty*sty);  //Sty
      c->set_p(0.1*0.1);   //Sp
      trlist[NTrack++] = new Track(*p,*c);
      delete p;
      p = 0;
      delete c;
      c = 0;
      if ( NTrack == 6 )
      {
        vt_mc_find();
	FillHists();
	NTrack = 0;
      } 
    }
  }
}
