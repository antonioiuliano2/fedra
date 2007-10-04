#ifndef ROOT_EdbAlignment
#define ROOT_EdbAlignment

#include "EdbLayer.h"

class EdbAffine2D; 
class TArrayF;

//______________________________________________________________________________
class EdbAlignment : public TObject {
 
 public:

  //EdbScanCond eScanCond;   // segments (scanning) properties

 public:
  EdbAlignment(){}
  ~EdbAlignment(){}

  static float DVAR( const TObjArray &segarr1, const TObjArray &segarr2, int ivar );
  static float DX(   const TObjArray &segarr1, const TObjArray &segarr2 ) { return DVAR(segarr1,segarr2,1); }
  static float DY(   const TObjArray &segarr1, const TObjArray &segarr2 ) { return DVAR(segarr1,segarr2,2); }
  static float DTX(  const TObjArray &segarr1, const TObjArray &segarr2 ) { return DVAR(segarr1,segarr2,3); }
  static float DTY(  const TObjArray &segarr1, const TObjArray &segarr2 ) { return DVAR(segarr1,segarr2,4); }

  static int   CalculateM( const TObjArray &segarr1, const TObjArray &segarr2, EdbAffine2D &aff, int flag=0 );
  static int   Calculate( const TObjArray &segarr1, const TObjArray &segarr2, EdbAffine2D &aff );
  static int   Calculate( TArrayF &X1, TArrayF &Y1, TArrayF &X2, TArrayF &Y2, EdbAffine2D &aff );
  static void  MakeAff(Double_t X0, Double_t Y0, Double_t dX, Double_t dY, Double_t dPhi, EdbAffine2D &aff);

  ClassDef(EdbAlignment,1)  // alignment class
};
#endif /* ROOT_EdbAlignment */
