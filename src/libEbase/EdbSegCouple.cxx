//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegCouple                                                         //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

// #include "TROOT.h"
// #include "TFile.h"
// #include "TVector3.h"
// #include "TIndexCell.h"
// #include "TArrayF.h"
// #include "TBenchmark.h"
// #include "EdbVertex.h"
// #include "EdbPVRec.h"
// #include "EdbPhys.h"
// #include "EdbMath.h"
// #include "EdbMomentumEstimator.h"
// #include "EdbLog.h"
// #include "EdbScanSet.h"
#include "EdbSegCouple.h"
// 
// #include "vt++/CMatrix.hh"
// #include "vt++/VtVector.hh"
// #include "vt++/VtDistance.hh"
// #include "smatrix/Functions.hh"
// #include "smatrix/SVector.hh"

ClassImp(EdbSegCouple)

int EdbSegCouple::egSortFlag=0;
///______________________________________________________________________________
EdbSegCouple::EdbSegCouple() 
{ 
  Set0();
}

///______________________________________________________________________________
EdbSegCouple::~EdbSegCouple() 
{ 
  SafeDelete(eS);
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
