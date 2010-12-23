//-- Author :  Alessandra Pastore   17.12.2009

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRunParent                                                       //
//                                                                      //
// 
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbRunParent.h"
using namespace TMath;

ClassImp(EdbRunParent)

///_________________________________________________________________________
EdbRunParent::EdbRunParent()
{
  eIPmax=0;
}
///_________________________________________________________________________
EdbRunParent::~EdbRunParent()
{
}

//----------------------------------------------------------------------------------------
int EdbRunParent::FindComplimentsIP( EdbSegP &s, EdbPattern &pat, TObjArray &found )
{
  //  Input: s   - prediction segment
  //         par - pattern with segments after preliminary selection
  // Output: found - list of the selected segments from pat in agreement with s (ip<eIPmax)
  //         found sorted by increasing ip

  
  return 0;
}

//----------------------------------------------------------------------------------------
int  EdbRunParent::FindParentCandidates( EdbSegP &s, EdbPattern &fndbt, EdbPattern &fnds1, EdbPattern &fnds2 )
{
  // Input: s   - prediction segment
  // Output: fndbt - found basetracks
  //         fnds1 - found microtracks side 1
  //         fnds2 - found microtracks side 2

  return 0;
}
