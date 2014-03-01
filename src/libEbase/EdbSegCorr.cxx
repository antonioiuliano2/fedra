#include "EdbSegCorr.h"

ClassImp(EdbSegCorr)

//---------------------------------------------------------------------
void EdbSegCorr::Print()
{
  printf("EdbSegCorr (dx:dy:dz:dtx:dty:shr:phy): %9.3f %9.3f %11.3f %9.5f %9.5f %9.5f %9.5f\n"
	 ,eV[0],eV[1],eV[2],eV[3],eV[4],eV[5],eV[6]);
}

///---------------------------------------------------------------------
void EdbSegCorr::ApplyCorrections(EdbSegP &s)
{
  if(eApplyCorr) {
    float x=X(s), y=Y(s), tx=TX(s), ty=TY(s);
    s.SetX(x); s.SetY(y); s.SetTX(tx); s.SetTY(ty);
  }
}
