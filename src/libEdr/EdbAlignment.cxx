/////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbAlignment                                                         //
//                                                                      //
//   all algorithms responsible for the patterns alignment              //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TArrayF.h"
#include "EdbPattern.h"
#include "EdbAlignment.h"
using namespace TMath;

ClassImp(EdbAlignment)

//---------------------------------------------------------------------------------------------
float EdbAlignment::DVAR(const TObjArray &segarr1, const TObjArray &segarr2, int ivar )
{
  int n = Min( segarr1.GetEntries(), segarr2.GetEntries() );
  Double_t dx = 0;
  for(int i=0; i<n; i++) {
    switch(ivar)
      {
      case 1:  dx += ((EdbSegP *)segarr2.At(i))->X() - ((EdbSegP *)segarr1.At(i))->X(); break;
      case 2:  dx += ((EdbSegP *)segarr2.At(i))->Y() - ((EdbSegP *)segarr1.At(i))->Y(); break;
      case 3:  dx += ((EdbSegP *)segarr2.At(i))->TX() - ((EdbSegP *)segarr1.At(i))->TX(); break;
      case 4:  dx += ((EdbSegP *)segarr2.At(i))->TY() - ((EdbSegP *)segarr1.At(i))->TY(); break;
      }
  }
  dx /= n;
  return (float)dx;
}

//---------------------------------------------------------------------------------------------
int  EdbAlignment::CalculateM( const TObjArray &segarr1, const TObjArray &segarr2, EdbAffine2D &aff, int flag )
{
  // interface to the matrix procedure
  int n = Min( segarr1.GetEntries(), segarr2.GetEntries() );
  if(n<1) return 0;
  TArrayF X1(n), Y1(n), X2(n), Y2(n);
  for(int i=0; i<n; i++) {
    X1[i] = ((EdbSegP *)segarr1.At(i))->X();
    Y1[i] = ((EdbSegP *)segarr1.At(i))->Y();
    X2[i] = ((EdbSegP *)segarr2.At(i))->X();
    Y2[i] = ((EdbSegP *)segarr2.At(i))->Y();
  }
  return aff.Calculate( n, X1.fArray,Y1.fArray, X2.fArray,Y2.fArray , flag);
}

//---------------------------------------------------------------------------------------------
int  EdbAlignment::Calculate( const TObjArray &segarr1, const TObjArray &segarr2, EdbAffine2D &aff )
{
  // aff applied to pattern 1 gives pattern 2
  int n = Min( segarr1.GetEntries(), segarr2.GetEntries() );
  if(n<1) return 0;
  TArrayF X1(n), Y1(n), X2(n), Y2(n);
  for(int i=0; i<n; i++) {
    X1[i] = ((EdbSegP *)segarr1.At(i))->X();
    Y1[i] = ((EdbSegP *)segarr1.At(i))->Y();
    X2[i] = ((EdbSegP *)segarr2.At(i))->X();
    Y2[i] = ((EdbSegP *)segarr2.At(i))->Y();
  }
  return Calculate( X1,Y1, X2,Y2, aff );
}

//---------------------------------------------------------------------------------------------
int  EdbAlignment::Calculate( TArrayF &X1, TArrayF &Y1, TArrayF &X2, TArrayF &Y2,  EdbAffine2D &aff )
{
  // this function do not use the matrix inversion for the precision reasons
  // aff applied to pattern 1 gives pattern 2

  int n = Min( X1.GetSize(), X2.GetSize() );
  if(n<1) return 0;

  Double_t dX = 0, dY=0, dPhi=0;
  Double_t X0 = 0, Y0=0;
  
  for(int i=0; i<n; i++) {
    dX += X2[i] - X1[i];
    dY += Y2[i] - Y1[i];
    X0 += X1[i];
    Y0 += Y1[i];
  }
  dX /= n;  dY /= n;  X0 /= n;  Y0 /= n;

  if(n<2) return n;
  
  Double_t x1 = 0, y1=0, x2=0, y2=0;
  Double_t R=0, r=0;
  for(int i=0; i<n; i++) {
    x1 = X1[i] -X0;
    y1 = Y1[i] -Y0;
    x2 = X2[i] -X0 -dX;
    y2 = Y2[i] -Y0 -dY;

    r = Sqrt(x1*x1+y1*y1);
    dPhi += (ATan2(-y2,-x2) - ATan2(-y1,-x1)) * r;
    R += r;
  }
  dPhi /= R;
  
  MakeAff(X0,Y0, dX,dY, dPhi, aff);

  printf("X0,Y0 = %f %f  dX,dY= %f %f dPhi = %f\n",X0,Y0,dX,dY,dPhi);
  return n;
}

//---------------------------------------------------------------------------------------------
void  EdbAlignment::MakeAff(Double_t X0, Double_t Y0, Double_t dX, Double_t dY, Double_t dPhi, EdbAffine2D &aff)
{
  aff.Reset();
  aff.Rotate(dPhi);
  Double_t x1new = X0*Cos(dPhi)-Y0*Sin(dPhi);
  Double_t y1new = X0*Sin(dPhi)+Y0*Cos(dPhi);
  aff.ShiftX(X0-x1new+dX);
  aff.ShiftY(Y0-y1new+dY);
}
