//-- Author :  Valeri Tioukov  8.10.2005

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TF1.h"
#include "TArrayF.h"
#include "TGraphErrors.h"

#include "EdbPhys.h"
#include "EdbTrackFitter.h"

ClassImp(EdbTrackFitter);

using namespace TMath;

//______________________________________________________________________________
EdbTrackFitter::EdbTrackFitter()
{
  SetDefaultBrick();
}

//______________________________________________________________________________
void EdbTrackFitter::SetDefaultBrick()
{
  eX0            = EdbPhysics::kX0_Pb();
  eTPb           = 1000./1300.;
  ePcut          = 0.050;
  eM             = 0.13957;
  eDE_correction = false;

  eNsegMax=60;
}

//______________________________________________________________________________
void EdbTrackFitter::Print()
{
  printf("EdbTrackFitter seetings:\n");
  printf("eNsegMax = %d\n",eNsegMax);
  printf("eX0      = %f\n",eX0);
  printf("eM       = %f\n",eM);
  printf("\n");
}

//______________________________________________________________________________
int EdbTrackFitter::FitTrackLine(EdbTrackP &tr)
{
  // track fit by averaging of segments parameters
  
  int nseg=tr.N();
  float x=0,y=0,z=0,tx=0,ty=0,w=0;
  EdbSegP *seg=0;
  for(int i=0; i<nseg; i++) {
    seg = tr.GetSegment(i);
    x  += seg->X();
    y  += seg->Y();
    z  += seg->Z();
    tx += seg->TX();
    ty += seg->TY();
    w  += seg->W();
  }
  x  /= nseg;
  y  /= nseg;
  z  /= nseg;
  tx /= nseg;
  ty /= nseg;
  tr.Set(tr.ID(),x,y,tx,ty,w,tr.Flag());
  tr.SetZ(z);
  return nseg;
}

//________________________________________________________________________________________
float EdbTrackFitter::PMS_Mag(EdbTrackP &tr, float detheta)
{
  // Momentum resolution by Multiple scattering (Annecy implementation Sep-2006)
  // 
  // Input: tr      - track
  //        detheta - average basetrack Theta angle resolution. Must be evaluated before.
  //        x0 - the radiation length of the media [mm]. The deafault is lead.

  // TODO: take into account the angular dependance of detheta

  // The constant term im the scattering formula is not 13.6 but 14.64, which 
  // is the right reevaluated number, due to a calculation with the moliere 
  // distribution. 13.6 is an approximation. See Geant3 or 4 references for more explanations.

  float x0 = eX0/1000.;                 //rad length in [mm]
  detheta = (detheta*Sqrt(2.)/1000.);
  detheta *= detheta;
  float k = 14.64*14.64/x0;                              // approx 38.3
  TF1 *f1=new TF1("f1",Form("sqrt(%f*x/([0])**2+%f)",k,detheta),0,14);  // 0-14 the fitting range (cells)
  f1->SetParameter(0,4000.);                             // strating value for momentum in MeV

  EdbSegP *s;
  int nseg = tr.N();
  int npl = tr.Npl();

  TArrayF theta2(eNsegMax),dtheta(eNsegMax),theta(eNsegMax);
  TArrayF thick(eNsegMax);
  TArrayF dx(eNsegMax);
  TArrayI sPID(eNsegMax);
  TArrayF setx(eNsegMax), sety(eNsegMax);
  double   P=0, dP=0;
  double   slopecorx=0, slopecory=0;
  int      lastci=0;
  
  for(int ci=0;ci<nseg;ci++){
    s=tr.GetSegment(ci);
    if(s!=NULL){
      sPID[ci]=s->PID(); 
      lastci=sPID[ci];
      setx[sPID[ci]]=s->TX();
      sety[sPID[ci]]=s->TY();
      slopecorx=slopecorx+s->TX();
      slopecory=slopecory+s->TY();    
    }
  }
  slopecorx=slopecorx/nseg;
  slopecory=slopecory/nseg;
  double cor=Sqrt(slopecorx*slopecorx+slopecory*slopecory);
  double Zeff=(cor*cor/Sqrt(1+cor*cor));

  for(int plate=0;plate<npl;plate++){
    setx[plate]=setx[plate+lastci];
    sety[plate]=sety[plate+lastci];
  }	
  
  int Ncell=0;
  for (int m=0;m<npl-1;m++) {
    theta2[m]=0;dtheta[m]=0;theta[m]=0;
    Ncell++;
    int dim=0;
    int nshift=Ncell-1;
    if(nshift>npl-Ncell-1){ nshift=npl-Ncell-1; }
    
    for(int j=0;j<nshift+1;j++){	     	
      double x = (npl-(j+1))/Ncell;
      int nmes = (int)x;
      int nhole=0;
      for (int i=0;i<nmes;i++){		   
	if ( (setx[Ncell*i+j+Ncell]!=0&&setx[Ncell*i+j]!=0)&&(sety[Ncell*i+j+Ncell]!=0&&sety[Ncell*i+j]!=0))
	  {
	    theta2[m] = 
	      theta2[m] + 
	      Power( (ATan(setx[Ncell*i+j+Ncell])-ATan(setx[Ncell*i+j])), 2 ) +
	      Power( (ATan(sety[Ncell*i+j+Ncell])-ATan(sety[Ncell*i+j])), 2 );
	    dim++;
	  }
	else nhole++; 	  	 
      }
     }//end loop on shifts            
    
    if (dim!=0){
      theta[m]=sqrt(theta2[m]/(2*dim));
      double errstat=theta[m]/Sqrt(2*dim);
      dtheta[m]=errstat;
    }
    else{ P=0.5; break;}
   
    thick[m]=Ncell*(Zeff+1);
    dx[m]=0.25;                // not important
    
  } //end loop on Ncell 

  TGraphErrors *ef1=new TGraphErrors(npl-1,thick.GetArray(),theta.GetArray(),dx.GetArray(),dtheta.GetArray());
  ef1->Fit("f1","MQ");
  P=f1->GetParameter(0);
  dP=f1->GetParError(0);
  P=fabs(P/1000.);

  delete ef1;
  delete f1;
  return (float)P;
}

//________________________________________________________________________________________
float EdbTrackFitter::P_MS(EdbTrackP &tr)
{
  // momentum estimation by multiple scattering (first version)

  int	 stepmax = 1;
  int    nms = 0;
  double tms = 0.;
  int ist = 0;

  float m;  // the mass of the particle
  eM<0? m = tr.M(): m=eM;

  EdbSegP *s1=0,*s2=0;

  double dx,dy,dz,ds;
  double dtx,dty,dts,fact,ax1,ax2,ay1,ay2,dax1,dax2,day1,day2;

  int nseg = tr.N(), i1 = 0, i2 = 0;

  for (ist=1; ist<=stepmax; ist++) {     // step size

      for (i1=0; i1<(nseg-ist); i1++) {       // for each step just once

	i2 = i1+ist;

	s1 = tr.GetSegment(i1);
	s2 = tr.GetSegment(i2);
	
	dx = s2->X()-s1->X();
	dy = s2->Y()-s1->Y();
	dz = s2->Z()-s1->Z();
	ds = Sqrt(dx*dx+dy*dy+dz*dz);
	
	ax1 = ATan(s1->TX());
	ax2 = ATan(s2->TX());
	ay1 = ATan(s1->TY());
	ay2 = ATan(s2->TY());
	dax1 = s1->STX();
	dax2 = s2->STX();
	day1 = s1->STY();
	day2 = s2->STY();
	dtx = (ax2-ax1);
	dty = (ay2-ay1);
	dts = dtx*dtx+dty*dty;
	fact = 1.+0.038*Log(ds/eX0);
	dts = (dts-dax1-dax2-day1-day2)/ds/fact/fact;
//	if (dts < 0.) dts = 0.;
	tms += dts;
	nms++;
      }
  }

  if(tms<=0) { 
	printf("P_MS: BAD estimation for track %d: tms=%g  nms=%d\n",tr.ID(),tms,nms);
	return 10;   // with correct parameters setting this problem is usually happend for hard tracks >=10 GeV
  }
  double pbeta = 0., pbeta2 = 0.;
  pbeta = Sqrt((double)nms/tms/eX0)*0.01923;
  pbeta2 = pbeta*pbeta;
  double p = 0.5*(pbeta2 + Sqrt(pbeta2*pbeta2 + 4.*pbeta2*m*m));
  if (p <= 0.)
    p = 0.;
  else
    p = Sqrt(p);
  
  if (eDE_correction)
  {
    double dtot = 0., eTPb = 1000./1300., e = 0., tkin = 0.;
    s1 = tr.GetSegment(0);
    s2 = tr.GetSegment(nseg-1);

    dx = s2->X()-s1->X();
    dy = s2->Y()-s1->Y();
    dz = s2->Z()-s1->Z();
    
    dtot = Sqrt(dx*dx+dy*dy+dz*dz)*eTPb;

    double DE = EdbPhysics::DeAveragePb(p, m, dtot);
    tkin = Sqrt(p*p + m*m) - m;

    if (tkin < DE)
    {
	tkin = 0.5*DE;
	e = tkin + m;
	p = Sqrt(e*e - m*m);
	DE = EdbPhysics::DeAveragePb(p, m, dtot);
    }
    tkin = tkin + 0.5*DE;
    e = tkin + m;
    p = Sqrt(e*e - m*m);
  }
  
  return (float)p;
}
