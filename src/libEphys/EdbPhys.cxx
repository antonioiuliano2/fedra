//-- Author :  Valeri Tioukov & Yury Petukhov  10.02.2004
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPhys                                                              //
//                                                                      //
// Class for Emulsion Physics                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "EdbPhys.h"

ClassImp(EdbPhysics);

//Pb constants 

namespace Pb_CONST {
  static double const geu = 0.577215;
  static double const geu1 = 0.422785;
  static double const me = 0.000511;            // Gev
  static double const me_kg = 9.10938188E-31;   // kg
  static double const mpi = 0.13957;            // Gev
  static double const ro = 11.35;               // g/cm**3 
  static double const Z = 82.;
  static double const A = 207.2;
  static double const Nav = 6.02214199E+23;      // 1/mol
  static double const I = 844.408;               // eV
  static double const Nel = 2.705E+24;           // 1/cm**3
  static double const h = 4.1357E-15;            // eV*s
  static double const D = 0.000153537;           // (GeV * cm**2)/s
  static double const ec = 1.602176462E-19;      // Coulombs
  static double const vp = 1.5577E+11;           // 1/s
  static double const C = -28.1723;
  static double const Xa = 6.1164;
  static double const X0 = 7.6842;
  static double const X1 = 3.0;
  static double const mm = 2.0;
  static double const aa = -0.3291;
}

//______________________________________________________________________________
double EdbPhysics::ThetaMS2( float p, float mass, float dx, float X0 )
{
  // calculate the square of multiple scattering angle theta (in one projection)
  // after the distance dx [microns] in the media with radiation length X0

  if (p < 0.0000001) return 2.;
  double k = 0.0136*0.0136;    // [GeV]
  double p2 = p*p;
  double p4=p2*p2;
  return  TMath::Abs(k*(mass*mass+p2)*dx/p4/X0);
}

//________________________________________________________________________
double EdbPhysics::ThetaPb2( float p, float mass, float dx ) 
    { return ThetaMS2(p,mass,dx, kX0_Pb()); }

//static TRandom *r=0;
//static TRandom *r=gRandom;
//________________________________________________________________________
double EdbPhysics::DeLandauPb(float pf, float massf, float xmicrons)
{
  using namespace Pb_CONST;
  //if (!r)
  //{
  //r=new TRandom();
  //}
    if (xmicrons <= 0.) return 0.;
    if (pf       <= 0.) return 0.;
    if (massf    <= 0.) return 0.;
    double p=(double)pf;
    double mass=(double)massf;
    double x=(double)xmicrons/10000.;
    double e=TMath::Sqrt(p*p+mass*mass);
    double beta=p/e;
    double gamma=e/mass;
    double emax=(2.*me*beta*beta*gamma*gamma)/
		(1.+2.*gamma*me/mass+(me/mass)*(me/mass));
    double eta=gamma*beta;
    double eta2=eta*eta;
    double eta4=eta2*eta2;
    double eta6=eta2*eta4;
    double X=TMath::Log(gamma*gamma*beta*beta)/4.606;

    double dzeta=153.4*Z*ro*x/(beta*beta*A)/1000000.; // GeV
    double k=dzeta/emax;
    double delta;
    if 	    (X < X0) delta = 0;
    else if (X < X1) delta = 4.606*X+C+aa*TMath::Power((X1-X),mm);
    else	     delta = 4.606*X+C;
    double Ce=(0.42237/eta2+0.0304/eta4-0.00038/eta6)*1.E-6*I*I +
	      (3.85800/eta2-0.1668/eta4+0.00158/eta6)*1.E-9*I*I*I;

    double de_aver=x*ro*(D*Z/(A*beta*beta))*
	           (TMath::Log(2.*me*beta*beta*gamma*gamma*emax/(I*I*1.E-18))-
		   2.*beta*beta-delta-2.*Ce/Z);
		 
    double lambda_aver=-geu1-beta*beta-TMath::Log(k);
    double lambda_max=0.60715+1.1934*lambda_aver+(0.67794+0.052382*lambda_aver)
		*TMath::Exp(0.94753+0.74442*lambda_aver);
    double lambda=0.;
    double de;
    for (int i=0;i<100000;i++)
    {
	lambda=gRandom->Landau(0.,1.);
	if (lambda<lambda_max)
	{
	    de = de_aver + dzeta*(lambda-geu1+beta*beta+TMath::Log(dzeta/emax));
	    return de;
	}
    }
    return 0.;
}

//________________________________________________________________________
double EdbPhysics::DeAveragePb(float pf, float massf, float xmicrons)
{
  using namespace Pb_CONST;
    if (xmicrons <= 0.) return 0.;
    if (pf       <= 0.) return 0.;
    if (massf    <= 0.) return 0.;
    double p=(double)pf;
    double mass=(double)massf;
    double x=(double)xmicrons/10000.;
    double e=TMath::Sqrt(p*p+mass*mass);
    double beta=p/e;
    double gamma=e/mass;
    double emax=(2.*me*beta*beta*gamma*gamma)/
		(1.+2.*gamma*me/mass+(me/mass)*(me/mass));
    double eta=gamma*beta;
    double eta2=eta*eta;
    double eta4=eta2*eta2;
    double eta6=eta2*eta4;
    double X=TMath::Log(gamma*gamma*beta*beta)/4.606;

    double delta;
    if 	    (X < X0) delta = 0;
    else if (X < X1) delta = 4.606*X+C+aa*TMath::Power((X1-X),mm);
    else	     delta = 4.606*X+C;
    double Ce=(0.42237/eta2+0.0304/eta4-0.00038/eta6)*1.E-6*I*I +
	      (3.85800/eta2-0.1668/eta4+0.00158/eta6)*1.E-9*I*I*I;

    double de_aver=x*ro*(D*Z/(A*beta*beta))*
	           (TMath::Log(2.*me*beta*beta*gamma*gamma*emax/(I*I*1.E-18))-
		   2.*beta*beta-delta-2.*Ce/Z);
		 
    return de_aver;
}

//	Kinematic parameters

double eP=0.;
double eMass=0.;
double eE=0.;
double eBeta=0.;
double eGamma=0.;
double eEmax=0.;
double eEta=0.;
double eEta2=0.;
double eEta4=0.;
double eEta6=0.;
double eX=0.;
double eDelta=0.;
double eCe=0.;
double eDe_aver_fact=0.;



//________________________________________________________________________
void EdbPhysics::DeAveragePbFastSet(float pf, float massf)
{
  using namespace Pb_CONST;
    if (pf       <= 0.) return;
    if (massf    <= 0.) return;
    eP=(double)pf;
    eMass=(double)massf;
    eE=TMath::Sqrt(eP*eP+eMass*eMass);
    eBeta=eP/eE;
    eGamma=eE/eMass;
    eEmax=(2.*me*eBeta*eBeta*eGamma*eGamma)/
		(1.+2.*eGamma*me/eMass+(me/eMass)*(me/eMass));
    eEta=eGamma*eBeta;
    eEta2=eEta*eEta;
    eEta4=eEta2*eEta2;
    eEta6=eEta2*eEta4;
    eX=TMath::Log(eGamma*eGamma*eBeta*eBeta)/4.606;

    if 	    (eX < X0) eDelta = 0;
    else if (eX < X1) eDelta = 4.606*eX+C+aa*TMath::Power((X1-eX),mm);
    else	      eDelta = 4.606*eX+C;
    eCe=(0.42237/eEta2+0.0304/eEta4-0.00038/eEta6)*1.E-6*I*I +
	(3.85800/eEta2-0.1668/eEta4+0.00158/eEta6)*1.E-9*I*I*I;

    eDe_aver_fact=ro*(D*Z/(A*eBeta*eBeta))*
	           (TMath::Log(2.*me*eBeta*eBeta*eGamma*eGamma*eEmax/(I*I*1.E-18))-
		   2.*eBeta*eBeta-eDelta-2.*eCe/Z);
		 
    return;
}

//________________________________________________________________________
double EdbPhysics::DeAveragePbFast(float pf, float massf, float xmicrons)
{
  using namespace Pb_CONST;
    if (pf       <= 0.) return 0.;
    if (massf    <= 0.) return 0.;
    if (xmicrons <= 0.) return 0.;
    double x=(double)xmicrons/10000.;
    double de_aver=x*eDe_aver_fact;
    return de_aver;
}
