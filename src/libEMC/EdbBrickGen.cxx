//-- Author :  Valeri Tioukov   03.01.2005

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbBrickGen - OPERA simulation                                       //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TRandom.h"
#include "EdbBrickGen.h"

ClassImp(EdbBeamGen)
ClassImp(EdbBrickGen)

//____________________________________________________________________________________
EdbBeamGen::EdbBeamGen()
{
  eLimits=0;
  eMass=0.124;

  eX0=0; eSigmaX=1;
  eY0=0; eSigmaY=1;
  eZ0=0; eSigmaZ=1;

  eTX0=0; eSigmaTX=1;
  eTY0=0; eSigmaTY=1;

  eP0=4.; eSigmaP=1;
}

//____________________________________________________________________________________
EdbTrackP *EdbBeamGen::NextTrack(int id)
{
  float x,y,tx,ty;
  EdbTrackP *tr = new EdbTrackP();

  int maxcycle=10000;
  int i;
  for(i=0; i<maxcycle; i++) {
    gRandom->Rannor(x,y); 
    x = eX0 + eSigmaX*x; 
    y = eY0 + eSigmaY*y; 
    if(!eLimits)               break;
    if(eLimits->IsInside(x,y)) break;
  }
  if(i>maxcycle-2) printf("WARNING: EdbBeamGen::NextTrack - infinit cycle\n");

  for(i=0; i<maxcycle; i++) {
    gRandom->Rannor(tx,ty); 
    tx = eTX0 + eSigmaTX*tx; 
    ty = eTY0 + eSigmaTY*ty; 
    if(tx*tx+ty*ty<1.)    break;
  }  
  if(i>maxcycle-2) printf("WARNING: EdbBeamGen::NextTrack - infinit cycle\n");

  tr->Set(id, x, y, tx, ty, 1, 0);

  tr->SetZ(eZ0);
  tr->SetP(eP0);
  tr->SetM(eMass);
  return tr;
}


//====================================================================================
EdbBrickGen::EdbBrickGen()
{
  eBrick=0;
  ePVG=0;
}

//____________________________________________________________________________________
EdbBrickGen::~EdbBrickGen()
{
  if(eBrick) delete eBrick;
  if(ePVG)   delete ePVG;
}

//____________________________________________________________________________________
void EdbBrickGen::Generate()
{
  GenerateOperaBrick();
  GeneratePatternsVolumeBT();
}

//____________________________________________________________________________________
void EdbBrickGen::Print()
{
  if(eBrick) eBrick->Print();
  if(ePVG)   ePVG->Print();
}

//____________________________________________________________________________________
void EdbBrickGen::GenerateOperaBrick( int   npl,
				      float dx, float dy,
				      float x0, float y0, float z0
				      )
{
  if(eBrick) delete eBrick;
  eBrick = new EdbBrickP();
  GenerateOperaBrick( *eBrick, npl, dx, dy, x0, y0, z0 );
}

//____________________________________________________________________________________
void EdbBrickGen::GenerateOperaBrick( EdbBrickP &br,
				      int   npl,
				      float dx, float dy,
				      float x0, float y0, float z0
				      )
{
  float zspacer=1000;
  float zbase=210, zu=45., zd=45.;

  float dzpl  = zbase+zu+zd;
  float zstep = zspacer+dzpl;

  float z = z0;
  EdbPlateP *pl=0;
  for(int i=0; i<npl; i++) {
    pl = new EdbPlateP();
    pl->SetZlayer( z , z-dzpl/2. , z+dzpl/2. );
    z += zstep;
    br.AddPlate(pl);
  }
  br.SetPlatesLayout(zbase,zu,zd);
  br.SetXY(x0,y0);
  br.SetDXDY(dx,dy);

  br.SetZlayer( br.GetPlate(0)->Z(), 
		br.GetPlate(0)->Zmin(), 
		br.GetPlate(br.Npl()-1)->Zmax());

}

//____________________________________________________________________________________
void EdbBrickGen::GeneratePatternsVolumeBT()
{
  // generate patterns corresponding to basetracks (1 pattern/plate)

  if(!ePVG)  ePVG = new EdbPVGen();
  EdbPatternsVolume *v = new EdbPatternsVolume();
  GeneratePatternsVolumeBT(*v);
  ePVG->SetVolume(v);
}

//____________________________________________________________________________________
void EdbBrickGen::GeneratePatternsVolumeBT(EdbPatternsVolume &v)
{
  int npl = eBrick->Npl();
  EdbPattern *pat =0;
  for(int i=0; i<npl; i++) {
    pat = new EdbPattern( 0,0, eBrick->GetPlate(i)->Z() );
    pat->SetPID(i);
    v.AddPattern(pat);
  }
  v.SetPatternsID();
}
