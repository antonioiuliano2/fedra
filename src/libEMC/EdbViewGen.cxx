//-- Author :  Valeri Tioukov   30.01.2006

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbViewGen - one microscope view simulation                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TH2F.h"
#include "TNtuple.h"

#include "TRandom.h"
#include "EdbViewGen.h"
#include "EdbCluster.h"
#include "EdbSegment.h"
#include "EdbFrame.h"
#include "EdbMath.h"

ClassImp(EdbViewGen)

//____________________________________________________________________________________
EdbViewGen::EdbViewGen()
{
  SetDef();
}

//____________________________________________________________________________________
int EdbViewGen::GenGrains(EdbView &v)
{
  TObjArray grains;
  int nseg =  v.Nsegments();
  //printf("nsegments = %d\n", nseg);
  EdbSegment   *s = 0;
  int ngrseg=0;
  int ngr;
  for(int iseg=0; iseg<nseg; iseg++) {
    s = v.GetSegment(iseg);
    ngr = GenSegGrains( *s );
    //printf("ngr = %d\n", ngr);
    for(int igr=0; igr<ngr; igr++) grains.Add(s->GetElements()->At(igr));
    ngrseg+=ngr;
  }
  //printf("ngrseg = %d\n", ngrseg);
  
  int nfog = (int)(eFogDens*(eXmax-eXmin)*(eYmax-eYmin)*(eZmax-eZmin)/1000);
  //printf("ngrfog = %d\n", nfog);
  EdbSegment sfog;
  sfog.SetPuls(nfog);
  GenFogGrains(sfog);
  for(int igr=0; igr<nfog; igr++) grains.Add(sfog.GetElements()->At(igr));

  EdbCluster *g;
  ngr = grains.GetEntries();

  // remove grains from the dead layer
  for(int i=0; i<ngr; i++) {
    g = (EdbCluster*)grains.At(i);
    if( TMath::Abs(g->GetZ()-eZdead)<eDZdead/2. ) g->SetArea(0);
  }

  for(int i=0; i<ngr; i++) {
    g = (EdbCluster*)grains.At(i);
    GenGrainClusters(v,*g);
    //v.AddCluster(new EdbCluster(*g));
  }
  
  if(sfog.GetElements()) sfog.GetElements()->Delete();
  for(int iseg=0; iseg<nseg; iseg++) 
    v.GetSegment(iseg)->GetElements()->Delete();

  return ngr;
}

//____________________________________________________________________________________
int EdbViewGen::GenGrainClusters(EdbView &v, EdbCluster &g)
{
  int ncl=0;
  if( g.GetArea()==0 ) return ncl;
  int nfr= v.GetNframes();
  float z;
  float dz = 0.5*eSz*TMath::Sqrt(g.GetArea())/3;  //ToDo
  EdbCluster *c;
  for(int i=0; i<nfr; i++) {
    z= v.GetFrame(i)->GetZ();
    if( TMath::Abs(z - g.GetZ()) < dz ) {
      c= new EdbCluster(g);
      c->SetZ(z);
      c->SetFrame(i);
      v.AddCluster(c);
      ncl++;
    }
  }
  return ncl;
}

//____________________________________________________________________________________
int EdbViewGen::GenSegGrains(EdbSegment &s)
{
 // input: segments of the view, assumed that puls of the 
 // segment is correspond to the number of grains to be generated
 // output: array of grains belong to the segment
 //  ToDo: dz only positive?

  using namespace TMath;

  // could be switched on if serve...
  //
  //   int ngr = s.GetPuls();
  //   TArrayF arr(ngr);
  //   gRandom->RndmArray(ngr, arr.GetArray());
  //   for(int i=0; i<ngr; i++)  printf("%f ",arr[i]);
  //   printf("\n");
  //   TArrayI ind(ngr);
  //   Sort( n, arr.GetArray(), ing.GetArray(), 0 );

  float x  = s.GetX0();
  float y  = s.GetY0();
  float z  = s.GetZ0();
  float dz = s.GetDz();
  float zmax=z+dz;
  float  cs = Cos(ATan(Sqrt(s.GetTx()*s.GetTx()+s.GetTy()*s.GetTy())));
  int    area;
  float  step;
  int    ncl=0;
  int    nmax=10000;
  for(int i=0; i<nmax; i++ ) {
    step = GrainPathMip()*cs;
    z += step;
    //printf("Zstep: %5.3f \n",step);
    x += s.GetTx()*step;
    y += s.GetTy()*step;
    if( z > zmax  )   break;
    if( x < eXmin )   continue;
    if( x > eXmax )   continue;
    if( y < eYmin )   continue;
    if( y > eYmax )   continue;
    area = (int)(eGrainArea); // + gRandom->Poisson(4) - 2);
   
    double sx,sy,sz, r=gRandom->Gaus(0.,0.1);  // the smearing of the physical grain center position
    gRandom->Sphere(sx,sy,sz,r);
    x+=sx;    y+=sy;    z+=sz;
    if(x<eXmin) continue;
    if(x>eXmax) continue;
    if(y<eYmin) continue;
    if(y<eYmin) continue;
    s.AddElement( new EdbCluster( x+sx, y+sy, z+sz, area, 0, 0, s.GetSide(), s.GetID()) );
    ncl++;
  }
  s.SetPuls(ncl);
  return ncl;
 }

//____________________________________________________________________________________
float EdbViewGen::GrainPathMip()
{
  //=========================================
  //
  // Some emulsion matter properties:
  // density: 2.4 g/cm^3
  //    <A>  = 18.2
  //    <Z>  = 8.9
  //     X0  = 5.5 cm
  // nuclear collision lenght: 33 cm
  //
  // <dE/dX> = 37 KeV/100 microns
  //
  // visible grain density: 30/100 microns
  // grain size: 0.6 micron
  //
  // assuming the ionization energy about 4 eV (to check!)
  // we expect about 100 ionizations/micron
  //
  //=========================================
  
  double lambda=2;    // mean path length
  int    nstep=100;  
  double prob0 = 1./nstep;   //prob of grain after single ionization
  float  grsiz=gRandom->Uniform(0.9,1.1);
  float  x=0;
  for(int j=0; j<100000; j++) {
    x+=gRandom->Exp(lambda/nstep);
    if( x>grsiz&&gRandom->Rndm()<prob0 ) {
      break;
    }
  }
  return x;
}

//____________________________________________________________________________________
int EdbViewGen::GenFogGrains(EdbSegment &sfog)
{
  float  x,y,z;
  int    area;
  int    ngr = sfog.GetPuls();
  int    n=0;

  printf("GenFogGrains: X:(%f %f)  Y:(%f %f) \n", eXmin,eXmax, eYmin,eYmax );
  while( n<ngr ) {
    x      = eXmin + gRandom->Rndm()*(eXmax-eXmin);
    y      = eYmin + gRandom->Rndm()*(eYmax-eYmin);
    z      = eZmin + gRandom->Rndm()*(eZmax-eZmin);

    area = (int)(eFogGrainArea); // + gRandom->Poisson(4) - 2);
    sfog.AddElement( new EdbCluster(x,y,z, area, 0, 0, sfog.GetSide(), -1) );
    n++;
  }
  return n;
 }
