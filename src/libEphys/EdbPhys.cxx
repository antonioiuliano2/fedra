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

//______________________________________________________________________________

const float EdbPhysics::kX0_Pb   =   5600.; // pure Pb [microns]
const float EdbPhysics::kX0_Em   = 286000.; // pure Emulsion (?)
const float EdbPhysics::kX0_Cell =   5810.; // Pb+emulsion mixture [microns]

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
    { return ThetaMS2(p,mass,dx, kX0_Pb); }

static TRandom *r=0;
//________________________________________________________________________
double EdbPhysics::DeLandauPb(float pf, float massf, float xmicrons)
{
    if (!r)
    {
	r=new TRandom();
    }
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
	lambda=r->Landau(0.,1.);
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
