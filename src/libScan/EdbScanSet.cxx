//-- Author :  Valeri Tioukov   21/03/2006
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanSet                                                           //
//                                                                      //
// Scanning data assembler                                              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbScanSet.h"

ClassImp(EdbScanSet)
//----------------------------------------------------------------
EdbScanSet::EdbScanSet()
{

}

//----------------------------------------------------------------
int EdbScanSet::MakeParFiles(int piece, const char *dir)
{
  // prepare par files for the dataset for "recset-like" processing

  TString name;
  printf("MakeParFiles for brick %d with %d plates:\n", eB.ID(), eB.Npl());
  EdbPlateP *p=0;
  for(int i=0; i<eB.Npl(); i++) {
    p = eB.GetPlate(i);
    name.Form("%s/%2.2d_%3.3d.par",dir,p->ID(),piece);
    printf("%s\n",name.Data());
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
  eB.Clear();
  EdbPlateP *plate=0;

  EdbAffine2D aff;
  int npc = ePC.GetEntries();
  EdbPlateP *pc=0;                    //the couple of plates
  for(int i=0; i<npc; i++) {
    pc = (EdbPlateP *)(ePC.At(i));
    if(i==0) {                       //the first plate is the reference
      plate = new EdbPlateP();
      plate->SetID(pc->GetLayer(1)->ID());
      plate->SetZlayer(0,-150,150);
      eB.AddPlate(plate);
    }
    plate = new EdbPlateP();
    plate->SetID(pc->GetLayer(2)->ID());
    float z = eB.GetPlate(eB.Npl()-1)->Z() - pc->GetLayer(1)->Z();
    plate->SetZlayer(z,z-150,z+150);

    aff.Reset();
    aff.Transform( pc->GetLayer(1)->GetAffineXY() );
    aff.Invert();
    aff.Transform( eB.GetPlate(eB.Npl()-1)->GetAffineXY() );
    plate->GetAffineXY()->Transform(&aff);

    eB.AddPlate(plate);    
  }

  //ePID.Clear();
  Long_t v[2];
  for(int i=0; i<eB.Npl(); i++) {
    v[0]= eB.GetPlate(i)->ID();
    v[1]= i;
    ePID.Add(2,v);
  }
  //ePID.Sort();

  return eB.Npl();
}

//----------------------------------------------------------------
bool EdbScanSet::GetAffP2P(int p1, int p2, EdbAffine2D &aff)
{
  // input:  p1 - id of the "from-plate"
  //         p2 - id of the "to-plate"
  // output: aff - the transformation
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
}
