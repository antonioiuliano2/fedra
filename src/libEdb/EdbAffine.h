#ifndef ROOT_EdbAffine
#define ROOT_EdbAffine

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbAffine                                                            //
//                                                                      //
// Affine transformation algoriths                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"

class EdbPointsBox2D;

//______________________________________________________________________________
class EdbAffine2D : public TObject{

  //  x1 = A11*x + A12*y + B1
  //  y1 = A21*x + A22*y + B2

 private:
  Float_t  eA11, eA12;   //
  Float_t  eA21, eA22;   //
  Float_t  eB1,  eB2;    //

 public:
  EdbAffine2D() { Reset(); }
  EdbAffine2D( float a11, float a12, 
	       float a21, float a22, 
	       float b1,  float b2   ): 
    eA11(a11), eA12(a12), eA21(a21), eA22(a22), eB1(b1), eB2(b2) {}
  virtual ~EdbAffine2D() {}

  void Set( float a11, float a12, 
	    float a21, float a22, 
	    float b1,  float b2   )
    { eA11=a11; eA12=a12; eA21=a21; eA22=a22; eB1=b1; eB2=b2; }

  Float_t   A11() const { return eA11; }
  Float_t   A12() const { return eA12; }
  Float_t   A21() const { return eA21; }
  Float_t   A22() const { return eA22; }
  Float_t   B1()  const { return eB1;   }
  Float_t   B2()  const { return eB2;   }

  //EdbAffine2D     operator*( const EdbAffine2D& rhs ) const;

  void     Transform( const EdbAffine2D *a );

  void Reset();
  void Invert();
  void SetAxisX( float a, float b ) { eA11 = a; eB1 = b; }
  void SetAxisY( float a, float b ) { eA22 = a; eB2 = b; }

  void ZoomX( float k )  { eA11*=k; eA12*=k; }
  void ZoomY( float k )  { eA21*=k; eA22*=k; }
  void Zoom ( float k )  { ZoomX(k); ZoomY(k); }
  
  void ShiftX(float d)  { eB1+=d; }
  void ShiftY(float d)  { eB2+=d; }

  void Rotate( float angle );

  Int_t     Calculate( int n, float *x0, float *y0, float *x1, float *y1 );
  Int_t     Calculate( EdbPointsBox2D *b1, EdbPointsBox2D *b2 );
  Double_t  Phi(Double_t x, Double_t y) const;

  void Print(Option_t *opt="") const;

  ClassDef(EdbAffine2D,1)  // 2D affine parameters  
};

//______________________________________________________________________________
class EdbAffine3D : public TObject {

  //  x1 = A11*x + A12*y + A13*z + B1
  //  y1 = A21*x + A22*y + A23*z + B2
  //  z1 = A31*x + A32*y + A33*z + B3

 private:
  Float_t  eA11, eA12, eA13;   //
  Float_t  eA21, eA22, eA23;   //
  Float_t  eA31, eA32, eA33;   //
  Float_t  eB1,  eB2,  eB3;    //

 public:
  EdbAffine3D() { Reset(); }
  virtual ~EdbAffine3D() {}

  void Set( float a11, float a12, float a13, 
	    float a21, float a22, float a23, 
	    float a31, float a32, float a33, 
	    float b1,  float b2,  float b3  )
    { eA11=a11; eA12=a12; eA13=a13; 
      eA21=a21; eA22=a22; eA23=a23; 
      eA31=a31; eA32=a32; eA33=a33; 
      eB1=b1;   eB2=b2;   eB3=b3;   }

  void Reset();

  Float_t   A11() const { return eA11; }
  Float_t   A12() const { return eA12; }
  Float_t   A13() const { return eA13; }
  Float_t   A21() const { return eA21; }
  Float_t   A22() const { return eA22; }
  Float_t   A23() const { return eA23; }
  Float_t   A31() const { return eA31; }
  Float_t   A32() const { return eA32; }
  Float_t   A33() const { return eA33; }
  Float_t   B1()  const { return eB1;  }
  Float_t   B2()  const { return eB2;  }
  Float_t   B3()  const { return eB3;  }

  void SetAxisX( float a, float b ) { eA11 = a; eB1 = b; }
  void SetAxisY( float a, float b ) { eA22 = a; eB2 = b; }
  void SetAxisZ( float a, float b ) { eA33 = a; eB3 = b; }

  void SetSlantXZ( float s ) { eA13=s; }
  void SetSlantYZ( float s ) { eA23=s; }

  void ZoomX( float k )  { eA11*=k; eA12*=k; eA31*=k; }
  void ZoomY( float k )  { eA21*=k; eA22*=k; eA23*=k; }
  void ZoomZ( float k )  { eA31*=k; eA32*=k; eA33*=k; }
  void Zoom ( float k )  { ZoomX(k); ZoomY(k); ZoomZ(k); }

  virtual void ShiftX(float d)  { eB1+=d; }
  virtual void ShiftY(float d)  { eB2+=d; }
  virtual void ShiftZ(float d)  { eB3+=d; }

  void Print(Option_t *opt="") const;

  ClassDef(EdbAffine3D,1)  // 3D affine parameters
};

#endif /* ROOT_EdbAffine */
