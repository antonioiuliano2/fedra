//-- Author :  Valeri Tioukov   13.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbAffine                                                            //
//                                                                      //
// Affine transformations                                               //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <TMath.h>
#include <TClass.h>
#include "EdbAffine.h"
#include "EdbVirtual.h"

ClassImp(EdbAffine2D)
ClassImp(EdbAffine3D)

inline Double_t sqr(Double_t x) {return (x*x);};

//______________________________________________________________________________
void EdbAffine2D::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbAffine2D.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbAffine2D::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      R__b >> eA11;
      R__b >> eA12;
      R__b >> eA21;
      R__b >> eA22;
      R__b >> eB1;
      R__b >> eB2;
      R__b.CheckByteCount(R__s, R__c, EdbAffine2D::IsA());
      //====end of old versions
   } else {
     EdbAffine2D::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbAffine2D::Print( Option_t *opt ) const
{
  printf("EdbAffine2D: \t\t %f %f %f %f \t %f %f\n", 
	 eA11, eA12, eA21, eA22, eB1, eB2 );
}
 
//______________________________________________________________________________
void EdbAffine2D::Reset()
{
  // set to self-transformation:

  eA11=1;  eA12=0;
  eA21=0;  eA22=1;
  eB1=0;   eB2=0;
}
 
//______________________________________________________________________________
//EdbAffine2D
//EdbAffine2D::operator*( const EdbAffine2D& rhs ) const
//{
//  return EdbAffine2D
//    ( A11()*rhs.A11() + A21()*rhs.A12(), A11()*rhs.A21() + A21()*rhs.A22(),
//      A12()*rhs.A11() + A22()*rhs.A12(), A12()*rhs.A21() + A22()*rhs.A22(),
//      A11()*rhs.B1()  + A21()*B2() + B1(),
//      A12()*rhs.B1()  + A22()*B2() + B2() );
//}
 
//______________________________________________________________________________
void EdbAffine2D::Transform( const EdbAffine2D *a  ) 
{
  Set( 
      a->A11()*A11() + a->A12()*A21(), a->A11()*A12() + a->A12()*A22(),
      a->A21()*A11() + a->A22()*A21(), a->A21()*A12() + a->A22()*A22(),
      a->A11()*B1() + a->A12()*B2() + a->B1(),
      a->A21()*B1() + a->A22()*B2() + a->B2()   );
}

//______________________________________________________________________________
void EdbAffine2D::Invert()
{
  Double_t  d;
  Float_t   a11,a12,a21,a22,b1,b2;

  d = eA11*eA22 - eA12*eA21;
  if( d==0. ) printf("EdbAffine2D::Inverse: Error: Determinant = 0\n");
  
  a11 =  eA22/d;
  a12 = -eA12/d;
  b1  = (eA12*eB2 - eA22*eB1)/d;

  a21 = -eA21/d;
  a22 =  eA11/d;
  b2  =  (eA21*eB1 - eA11*eB2)/d;

  Set( a11, a12, a21, a22, b1, b2 );
}

//______________________________________________________________________________
Int_t EdbAffine2D::Calculate(int n, float *x0, float *y0, float *x1, float *y1, int flag)
{
  // Calculate affine transformation for 2 patterns
  // if flag==2 : do not permit scaling of patterns

  if(flag==2) return CalculateTurn( n, x0,y0, x1,y1 );
  else        return CalculateFull( n, x0,y0, x1,y1 );
}

//______________________________________________________________________________
Int_t EdbAffine2D::CalculateFull(int n, float *x0, float *y0, float *x1, float *y1)
{
  //
  // Calculate affine transformation for 2 patterns.
  // b1 == b0->Transform(this);
  //

  Double_t a,b,c,d,p,q;
  a=b=c=d=p=q=0;

  if(n<2)    return 0; 

  else if(n==2){

    Double_t    dx0 = x0[1] - x0[0];
    Double_t    dy0 = y0[1] - y0[0];
    Double_t    dx1 = x1[1] - x1[0];
    Double_t    dy1 = y1[1] - y1[0];

    Double_t    s = TMath::Sqrt(dx1*dx1+dy1*dy1)/TMath::Sqrt(dx0*dx0+dy0*dy0);
    Double_t    th = Phi(dx1,dy1)-Phi(dx0,dy0);

    a =  s*TMath::Cos(th);
    b = -s*TMath::Sin(th);
    c =  s*TMath::Sin(th);
    d =  s*TMath::Cos(th);
    p =  x1[0] - a*x0[0] - b*y0[0];
    q =  y1[0] - c*x0[0] - d*y0[0];

  }
  else {

    Double_t  
      sx0   =0,
      sy0   =0,
      sx1   =0,  
      sy1   =0,
      sx0x0 =0,  
      sy0y0 =0,  
      sx0y0 =0,  
      sx0x1 =0,  
      sx0y1 =0,  
      sx1y0 =0,  
      sy0y1 =0,
      r     =0;

    for(int i=0; i<n; i++) {
      r = 1./(i+1);
      sx0   = sx0   *i*r + x0[i]         *r;
      sy0   = sy0   *i*r + y0[i]         *r;
      sx1   = sx1   *i*r + x1[i]         *r;
      sy1   = sy1   *i*r + y1[i]         *r;
      sx0x0 = sx0x0 *i*r + x0[i] * x0[i] *r;
      sy0y0 = sy0y0 *i*r + y0[i] * y0[i] *r;
      sx0y0 = sx0y0 *i*r + x0[i] * y0[i] *r;
      sx0x1 = sx0x1 *i*r + x0[i] * x1[i] *r;
      sx0y1 = sx0y1 *i*r + x0[i] * y1[i] *r;
      sx1y0 = sx1y0 *i*r + x1[i] * y0[i] *r;
      sy0y1 = sy0y1 *i*r + y0[i] * y1[i] *r;
    }
    
    Double_t    sp0 = sx0x1 - sx0*sx1;
    Double_t    sp1 = sx1y0 - sx1*sy0;
    Double_t    sq0 = sx0y1 - sx0*sy1;
    Double_t    sq1 = sy0y1 - sy0*sy1;

    //        printf(" sp0,sp1, sq0,sq1: %f %f \t %f %f\n", sp0,sp1, sq0,sq1);
 
    Double_t    sar = sx0x0 - sx0*sx0;
    Double_t    sbr = sx0y0 - sx0*sy0;
    Double_t    scr = sbr;
    Double_t    sdr = sy0y0 - sy0*sy0;
    Double_t    det = sar*sdr-sbr*scr;

    //        printf(" sar,sbr,scr,sdr, det: %f %f %f %f \t %f\n", sar,sbr,scr,sdr, det);

    if ( det == 0 )                           return 0;

    Double_t    sa =  sdr/det;
    Double_t    sb = -sbr/det;
    Double_t    sc = -scr/det;
    Double_t    sd =  sar/det;

    //        printf(" sa,sb,sc,sd: %f %f %f %f \n\n", sa,sb,sc,sd);

    a = sa*sp0+sb*sp1;
    b = sc*sp0+sd*sp1;
    c = sa*sq0+sb*sq1;
    d = sc*sq0+sd*sq1;
    p = sx1-a*sx0-b*sy0;
    q = sy1-c*sx0-d*sy0;
  }

  printf("Aff2D   ( %6d ) : %9.6f %9.6f %9.6f %9.6f %12.6f %12.6f \n", n, a,b,c,d,p,q);

  Set(a,b,c,d,p,q);

  return  1;
}

//______________________________________________________________________________
Int_t EdbAffine2D::Calculate( EdbPointsBox2D *b0, EdbPointsBox2D *b1 )
{
  //
  // Calculate affine transformation for 2 patterns.
  // b1 == b0->Transform(this);
  //

  int n0 = b0->N();
  int n1 = b1->N();
  int n = TMath::Min(n0,n1);

  Double_t a=0;
  Double_t b=0;
  Double_t c=0;
  Double_t d=0;
  Double_t p=0;
  Double_t q=0;

  if(n<2)    return 0; 

  else if(n==2){

    Double_t    dx0 = b0->At(1)->X() - b0->At(0)->X();
    Double_t    dy0 = b0->At(1)->Y() - b0->At(0)->Y();
    Double_t    dx1 = b1->At(1)->X() - b1->At(0)->X();
    Double_t    dy1 = b1->At(1)->Y() - b1->At(0)->Y();

    Double_t    s = TMath::Sqrt(dx1*dx1+dy1*dy1)/TMath::Sqrt(dx0*dx0+dy0*dy0);
    Double_t    th = Phi(dx1,dy1)-Phi(dx0,dy0);

    a =  s*TMath::Cos(th);
    b = -s*TMath::Sin(th);
    c =  s*TMath::Sin(th);
    d =  s*TMath::Cos(th);
    p =  b1->At(0)->X() - a*b0->At(0)->X() - b*b0->At(0)->Y();
    q =  b1->At(0)->Y() - c*b0->At(0)->X() - d*b0->At(0)->Y();

  }
  else {

    Double_t  
      sx0   =0,  
      sy0   =0,
      sx1   =0,  
      sy1   =0,
      sx0x0 =0,  
      sy0y0 =0,  
      sx0y0 =0,  
      sx0x1 =0,  
      sx0y1 =0,  
      sx1y0 =0,  
      sy0y1 =0,
      r     =0;

    for(int i=0; i<n; i++) {
      r = 1./(i+1);
      sx0   = sx0   *i*r + b0->At(i)->X()                  *r;
      sy0   = sy0   *i*r + b0->At(i)->Y()                  *r;
      sx1   = sx1   *i*r + b1->At(i)->X()                  *r;
      sy1   = sy1   *i*r + b1->At(i)->Y()                  *r;
      sx0x0 = sx0x0 *i*r + b0->At(i)->X() * b0->At(i)->X() *r;
      sy0y0 = sy0y0 *i*r + b0->At(i)->Y() * b0->At(i)->Y() *r;
      sx0y0 = sx0y0 *i*r + b0->At(i)->X() * b0->At(i)->Y() *r;
      sx0x1 = sx0x1 *i*r + b0->At(i)->X() * b1->At(i)->X() *r;
      sx0y1 = sx0y1 *i*r + b0->At(i)->X() * b1->At(i)->Y() *r;
      sx1y0 = sx1y0 *i*r + b1->At(i)->X() * b0->At(i)->Y() *r;
      sy0y1 = sy0y1 *i*r + b0->At(i)->Y() * b1->At(i)->Y() *r;
    }
    
    Double_t    sp0 = sx0x1 - sx0*sx1;
    Double_t    sp1 = sx1y0 - sx1*sy0;
    Double_t    sq0 = sx0y1 - sx0*sy1;
    Double_t    sq1 = sy0y1 - sy0*sy1;

    //    printf("%f %f \t %f %f\n", sp0,sp1, sq0,sq1);
 
    Double_t    sar = sx0x0 - sx0*sx0;
    Double_t    sbr = sx0y0 - sx0*sy0;
    Double_t    scr = sbr;
    Double_t    sdr = sy0y0 - sy0*sy0;
    Double_t    det = sar*sdr-sbr*scr;

    //    printf("%f %f %f %f \t %f\n", sar,sbr,scr,sdr, det);

    if ( det == 0 )                           return 0;

    Double_t    sa =  sdr/det;
    Double_t    sb = -sbr/det;
    Double_t    sc = -scr/det;
    Double_t    sd =  sar/det;

    a = sa*sp0+sb*sp1;
    b = sc*sp0+sd*sp1;
    c = sa*sq0+sb*sq1;
    d = sc*sq0+sd*sq1;
    p = sx1-a*sx0-b*sy0;
    q = sy1-c*sx0-d*sy0;
  }

  printf("Aff2Dbox( %6d ) : %9.6f %9.6f %9.6f %9.6f %12.6f %12.6f \n", n, a,b,c,d,p,q);

  Set(a,b,c,d,p,q);

  return  1;
}

/*
//______________________________________________________________________________
void EdbAffine2D::GetRot(Double_t a, Double_t phi, Double_t dx, Double_t dy) const 
{ 
  // eA11, eA12, eA21, eA22, eB1, eB2
  a = TMath::Sqrt(TMath::Abs(eA11*eA22 - eA12*eA21));
  dx = B1/a;
  dy = B1/a;

}
*/
//______________________________________________________________________________

Double_t EdbAffine2D::Phi(Double_t x, Double_t y) const 
{ 
  // phi() is defined in [0,TWOPI]
  return TMath::Pi()+TMath::ATan2(-y,-x);
}

//______________________________________________________________________________
void EdbAffine2D::Rotate( float angle )
{
  EdbAffine2D aff( 
		  TMath::Cos(angle), -TMath::Sin(angle),
		  TMath::Sin(angle),  TMath::Cos(angle),
		  0, 0 );

  Transform( &aff );
}
 
//______________________________________________________________________________
void EdbAffine3D::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbAffine3D.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbAffine3D::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      R__b >> eA11;
      R__b >> eA12;
      R__b >> eA13;
      R__b >> eA21;
      R__b >> eA22;
      R__b >> eA23;
      R__b >> eA31;
      R__b >> eA32;
      R__b >> eA33;
      R__b >> eB1;
      R__b >> eB2;
      R__b >> eB3;
      R__b.CheckByteCount(R__s, R__c, EdbAffine3D::IsA());
      //====end of old versions
   } else {
     EdbAffine3D::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbAffine3D::Print( Option_t *opt ) const
{
  printf("EdbAffine3D: %f %f %f\n %f %f %f\n %f %f %f\n %f %f %f\n",
	 eA11, eA12, eA13,
	 eA21, eA22, eA23,
	 eA31, eA32, eA33, 
	 eB1,  eB2,  eB3    );
}
 
//______________________________________________________________________________
void EdbAffine3D::Reset()
{
  // set to self transformation:

  eA11=1;  eA12=0; eA13=0;
  eA21=0;  eA22=1; eA23=0;
  eA31=0;  eA32=0; eA33=1;
  eB1=0;   eB2=0;  eB3=0;
}

//______________________________________________________________________________ 
// insert 08.10.2003 by Stas Blokhin ///////////////////////////////////////////
Int_t EdbAffine2D::CalculateTurn(int n, float *x0, float *y0, float *x1, float *y1)
{
  //
  // Calculate affine transformation for 2 patterns: shift and rotation only
  // b1 == b0->Transform(this);
  //

  Double_t a,b,c,d,p,q;
  a=b=c=d=p=q=0;
  Double_t	X = 0.0;
  Double_t	Y = 0.0;
  Double_t	X_ = 0.0;
  Double_t	Y_ = 0.0;
  Double_t	f = 0.0;
  Double_t	e = 0.0;
  Double_t	teta1,teta2,a1,a2,b1,b2,F1=0,F2=0;
  Int_t i;
		

	if (n==0){
		return 0;
	}
	else if(n==1){
		a = 1.0;
		b = 0.0;
		c = 0.0;
		d = 1.0;
		p = x1[0] - x0[0];
		q = y1[0] - y0[0];
	}
	else {
		
		for (i=0;i<n;i++) {
			X += x0[i];
			Y += y0[i];
			X_ += x1[i];
			Y_ += y1[i];
		}
		X /= (Double_t)n;
		Y /= (Double_t)n;
		X_ /= (Double_t)n;
		Y_ /= (Double_t)n;

		for (i=0;i<n;i++) {
			f += x1[i] * (x0[i] - X) + y1[i] * (y0[i] - Y);
			d += x1[i] * (Y - y0[i]) + y1[i] * (x0[i] - X);
			e += x1[i] * Y_ - y1[i] * X_;
		}
		
		teta1 = TMath::ASin((e*f-d*TMath::Sqrt(sqr(f)+sqr(d)-sqr(e)))/(sqr(f)+sqr(d)));
		teta2 = TMath::Pi() - TMath::ASin((e*f+d*TMath::Sqrt(sqr(f)+sqr(d)-sqr(e)))/(sqr(f)+sqr(d)));

		a1 = X + Y_*TMath::Sin(teta1) - X_*TMath::Cos(teta1);
		b1 = Y - Y_*TMath::Cos(teta1) - X_*TMath::Sin(teta1);

		a2 = X + Y_*TMath::Sin(teta2) - X_*TMath::Cos(teta2);
		b2 = Y - Y_*TMath::Cos(teta2) - X_*TMath::Sin(teta2);

		for (i=0;i<n;i++) {
			F1 += sqr(x0[i] - x1[i]*TMath::Cos(teta1) + y1[i]*TMath::Sin(teta1) - a1)	\
				+ sqr(y0[i] - x1[i]*TMath::Sin(teta1) - y1[i]*TMath::Cos(teta1) - b1);

			F2 += sqr(x0[i] - x1[i]*TMath::Cos(teta2) + y1[i]*TMath::Sin(teta2) - a2)	\
				+ sqr(y0[i] - x1[i]*TMath::Sin(teta2) - y1[i]*TMath::Cos(teta2) - b2);
		}

		if (F1<F2) {
			a = TMath::Cos(teta1);
			b = TMath::Sin(teta1);
			c = -TMath::Sin(teta1);
			d = TMath::Cos(teta1);
			p = -a1*TMath::Cos(teta1) - b1*TMath::Sin(teta1);
			q = a1*TMath::Sin(teta1) - b1*TMath::Cos(teta1);
		}
		else {
			a = TMath::Cos(teta2);
			b = TMath::Sin(teta2);
			c = -TMath::Sin(teta2);
			d = TMath::Cos(teta2);
			p = -a2*TMath::Cos(teta2) - b2*TMath::Sin(teta2);
			q = a2*TMath::Sin(teta2) - b2*TMath::Cos(teta2);
		}

	}
  printf("Aff2D   ( %6d ) : %9.6f %9.6f %9.6f %9.6f %12.6f %12.6f \n", n, a,b,c,d,p,q);

  Set(a,b,c,d,p,q);

  return  1;
}

//______________________________________________________________________________
// insert 08.10.2003 ///////////////////////////////////////////////////////////
