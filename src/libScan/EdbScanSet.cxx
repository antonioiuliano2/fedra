//-- Author :  Valeri Tioukov   21/03/2006
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanSet                                                           //
//                                                                      //
// Scanning data assembler                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbLog.h"
#include "EdbScanSet.h"

ClassImp(EdbID)
ClassImp(EdbScanSet)
//----------------------------------------------------------------
EdbScanSet::EdbScanSet()
{

}

//----------------------------------------------------------------
void EdbScanSet::MakeNominalSet( int from_plate, int to_plate, float z0, float dz, int vmi, int vma )
{
  // assumed that brick eB.ID is already set

  int step= (from_plate>to_plate)?-1:1;
  int p=from_plate;
  float z=z0;
  do {
    EdbPlateP *plate = new EdbPlateP();
    plate->SetID(p);
    plate->SetZlayer(z,z-150,z+150);
    eB.AddPlate(plate);
    eIDS.Add(new EdbID(eB.ID(),p,vmi,vma));
    p += step;
    z += dz;
  } while( p!=(to_plate+step) );
  MakePIDList();
  Print();
}

//----------------------------------------------------------------
int EdbScanSet::MakeParFiles(int piece, const char *dir)
{
  // prepare par files for the dataset for "recset-like" processing

  TString name;
  Log(2,"MakeParFiles","for brick %d with %d plates:\n", eB.ID(), eB.Npl());
  EdbPlateP *p=0;
  for(int i=0; i<eB.Npl(); i++) {
    p = eB.GetPlate(i);
    name.Form("%s/%2.2d_%3.3d.par",dir,p->ID(),piece);
    Log(3,"MakeParFiles","%s\n",name.Data());
    FILE *f = fopen(name.Data(),"w");
    EdbAffine2D *a=p->GetAffineXY();
    fprintf(f,"INCLUDE default.par\n");
    fprintf(f,"ZLAYER 0  %15.3f 0. 0.\n", p->Z());
    fprintf(f,"AFFXY  0  %15.6f %9.6f %9.6f %9.6f %15.6f %15.6f\n",
	    a->A11(),a->A12(),a->A21(),a->A22(),a->B1(),a->B2());
    fclose(f);
  }
  return eB.Npl();
}

//----------------------------------------------------------------
int EdbScanSet::AssembleBrickFromPC()
{
  ePID.Delete();
  eB.Clear();
  EdbPlateP *plate=0;

  float dz0=214,dz1=45,dz2=45;  //TODO!
  EdbAffine2D aff;
  int npc = ePC.GetEntries();
  EdbPlateP *pc=0;                    //the couple of plates
  float z;
  for(int i=-1; i<npc; i++) {
    plate = new EdbPlateP();
    
    if(i==-1) {
      pc = (EdbPlateP *)(ePC.At(0));
      plate->SetID(pc->GetLayer(1)->ID());
      z=0; 
    }
    else {
      pc = (EdbPlateP *)(ePC.At(i));
      plate->SetID(pc->GetLayer(2)->ID());
      z = eB.GetPlate(eB.Npl()-1)->Z() - pc->GetLayer(1)->Z();
      aff.Reset();
      aff.Transform( pc->GetLayer(1)->GetAffineXY() );
      aff.Invert();
      aff.Transform( eB.GetPlate(eB.Npl()-1)->GetAffineXY() );
      plate->GetAffineXY()->Transform(&aff);
    }
    plate->SetZlayer(z, z - dz0/2 + dz1, z+dz0/2+dz2);                
    plate->GetLayer(0)->SetZlayer(0,-dz0/2,dz0/2);       // internal plate coord
    plate->GetLayer(2)->SetZlayer(-dz0/2,-dz0/2-dz2,-dz0/2);
    plate->GetLayer(1)->SetZlayer( dz0/2, dz0/2, dz0/2+dz1);

    eB.AddPlate(plate);    
  }
  MakePIDList();

  return eB.Npl();
}

//----------------------------------------------------------------
void EdbScanSet::MakePIDList()
{
  ePID.Clear();
  Long_t v[2];
  for(int i=0; i<eB.Npl(); i++) {
    v[0]= eB.GetPlate(i)->ID();
    v[1]= i;
    ePID.Add(2,v);
  }
  ePID.Sort();
}

//----------------------------------------------------------------
bool EdbScanSet::SetAsReferencePlate(int pid)
{
  EdbPlateP *p = GetPlate(pid);
  if(!p) return false;
  float  z = p->Z();
  EdbAffine2D aff( *(p->GetAffineXY()) );
  aff.Invert();
  TransformBrick(aff);
  ShiftBrickZ(-z);
  return true;
}

//----------------------------------------------------------------
int EdbScanSet::TransformBrick(EdbAffine2D aff)
{
  int npl=eB.Npl();
  if(npl>0)
    for(int i=0; i<npl; i++)  eB.GetPlate(i)->GetAffineXY()->Transform(aff);
  return npl;
}

//----------------------------------------------------------------
int EdbScanSet::ShiftBrickZ(float z)
{
  int npl=eB.Npl();
  if(npl>0)
    for(int i=0; i<npl; i++)
      eB.GetPlate(i)->SetZlayer( eB.GetPlate(i)->Z() + z, eB.GetPlate(i)->Zmin()+z, eB.GetPlate(i)->Zmax()+z );
  return npl;
}

//----------------------------------------------------------------
bool EdbScanSet::GetAffP2P(int p1, int p2, EdbAffine2D &aff)
{
  // input:  p1 - id of the "from-plate"
  //         p2 - id of the "to-plate"
  // output: aff - the transformation - applied to "from" segment became 'to"-segment
  int ip1,ip2;
  if(ePID.Find(p1)) ip1 = ePID.Find(p1)->At(0)->Value();  else return false;
  if(ePID.Find(p2)) ip2 = ePID.Find(p2)->At(0)->Value();  else return false;
  aff.Reset();
  aff.Transform( eB.GetPlate(ip2)->GetAffineXY() );
  aff.Invert();
  aff.Transform( eB.GetPlate(ip1)->GetAffineXY() );
  return true;
}

//----------------------------------------------------------------
float EdbScanSet::GetDZP2P(int p1, int p2)
{
  // input:  p1 - id of the "from-plate"
  //         p2 - id of the "to-plate"
  // return: dz - segment of from-plate propagated to dz become in to-plate 
  int ip1,ip2;
  if(ePID.Find(p1)) ip1 = ePID.Find(p1)->At(0)->Value();  else return false;
  if(ePID.Find(p2)) ip2 = ePID.Find(p2)->At(0)->Value();  else return false;
  return  eB.GetPlate(ip2)->Z() - eB.GetPlate(ip1)->Z();
}

//----------------------------------------------------------------
void EdbScanSet::Print()
{
  EdbPlateP *p=0;
  printf("EdbScanSet:\n");
//   int npc = ePC.GetEntries();
//   printf("%d couples\n",npc);
//   for(int i=0; i<npc; i++) {
//     p = (EdbPlateP *)(ePC.At(i));
//     p->GetLayer(1)->Print();
//     p->GetLayer(2)->Print();
//  }
  printf("Brick %d with %d plates:\n", eB.ID(), eB.Npl());
  for(int i=0; i<eB.Npl(); i++) { 
    p = eB.GetPlate(i);
    EdbAffine2D *a=p->GetAffineXY();
    printf("%3d  %12.2f       %9.6f %9.6f %9.6f %9.6f %15.6f %15.6f\n",
	   p->ID(), p->Z(), a->A11(),a->A12(),a->A21(),a->A22(),a->B1(),a->B2());
  }
  printf("for this brick %d identifiers are defined:\n", eIDS.GetEntries());
  for(int i=0; i<eIDS.GetEntries(); i++)  ((EdbID*)eIDS.At(i))->Print();
}

//----------------------------------------------------------------
int EdbScanSet::ReadIDS(const char *file)
{
  char line[256];
  FILE *f = fopen (file, "rt");
  int b,p,mi,ma;
  int cnt=0;
  while(fgets(line, 256, f) != NULL)
    {
      if( sscanf(line, "%d, %d, %d, %d", &b, &p, &mi, &ma) != 4 )  break;
      eIDS.Add(new EdbID(b,p,mi,ma));
      cnt++;
    }
  fclose(f);
  return cnt;
}

//----------------------------------------------------------------
int EdbScanSet::WriteIDS(const char *file)
{
  FILE *f = 0;
  if(file) f = fopen (file, "n");
  EdbID *id;
  for( int i=0; i<eIDS.GetEntries(); i++ ) {
    id = (EdbID *)eIDS.At(i);
    if(file) fprintf(f,"%d, %d, %d, %d\n", id->eBrick, id->ePlate, id->eMajor, id->eMinor );
    else      printf("%d, %d, %d, %d\n", id->eBrick, id->ePlate, id->eMajor, id->eMinor );
  }

  if(file) fclose(f);
  return eIDS.GetEntries();
}

//----------------------------------------------------------------
EdbID *EdbScanSet::FindPlateID(int plate)
{
  EdbID *id;
  for( int i=0; i<eIDS.GetEntries(); i++ ) {
    id = (EdbID *)eIDS.At(i);
    if(id->ePlate ==plate ) return id;
  }
  return 0;
}
