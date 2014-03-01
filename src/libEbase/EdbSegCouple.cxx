//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegCouple                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbSegCouple.h"

ClassImp(EdbSegCouple)

int EdbSegCouple::egSortFlag=0;
///______________________________________________________________________________
EdbSegCouple::EdbSegCouple()
{ 
  Set0();
}

///______________________________________________________________________________
EdbSegCouple::EdbSegCouple( const EdbSegCouple &sc )
{
  eID1=sc.eID1;
  eID2=sc.eID2;
  eN1=sc.eN1;
  eN2=sc.eN2;
  eN1tot=sc.eN1tot;
  eN2tot=sc.eN2tot;
  eCHI2=sc.eCHI2;
  eCHI2P=sc.eCHI2P;
  if(sc.eS)  eS  = new EdbSegP( *(sc.eS) );
  if(sc.eS1) eS1 = new EdbSegP( *(sc.eS1) );
  if(sc.eS2) eS2 = new EdbSegP( *(sc.eS2) );
  eIsOwner=sc.eIsOwner;
}

///______________________________________________________________________________
EdbSegCouple::~EdbSegCouple() 
{ 
  SafeDelete(eS);
  if(eIsOwner) SafeDelete(eS1);
  if(eIsOwner) SafeDelete(eS2);
}

///______________________________________________________________________________
void EdbSegCouple::Set0() 
{ 
  eID1=0;
  eID2=0;
  eN1=0;
  eN2=0;
  eN1tot=0;
  eN2tot=0;
  eCHI2=0;
  eCHI2P=0;
  eS=0;
  eS1=0;
  eS2=0;
  eIsOwner=0;
}

///______________________________________________________________________________
void EdbSegCouple::SetSortFlag(int s) 
{ 
  egSortFlag=s; 
}

///______________________________________________________________________________
void EdbSegCouple::Print()
{
  printf("%f \t %f \t %d \t%d  \t\t %d \t %d \n",CHI2(),CHI2P(),ID1(),N1(),ID2(),N2() );
}

///______________________________________________________________________________
int EdbSegCouple::Compare( const TObject *obj ) const
{
  const EdbSegCouple *seg = (EdbSegCouple *)obj;

  double f1=0, f2=0;

  if(SortFlag()==0) {
    f1=CHI2P();
    f2=seg->CHI2P();
  } else {
    f1 =      N1() +      N2()*100000  +      CHI2P()/1000.;
    f2 = seg->N1() + seg->N2()*100000  + seg->CHI2P()/1000.;
  }

  if (f1>f2)
    return 1;
  else if (f1<f2)
    return -1;
  else
    return 0;
 
}
