//-- Author :  Valeri Tioukov 8/05/2008

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  Track momentum estimation algorithms                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TF1.h"
#include "TArrayF.h"
#include "TVectorF.h"
#include "TVector.h"
#include "TGraphErrors.h"
#include "EdbLog.h"
#include "EdbPhys.h"
#include "EdbAffine.h"
#include "EdbMomentumEstimator.h"

ClassImp(EdbMomentumEstimator);

using namespace TMath;

//________________________________________________________________________________________
EdbMomentumEstimator::EdbMomentumEstimator()
{
  eF1X   = 0;
  eF1Y   = 0;
  eGX    = 0;
  eGY    = 0;
  SetParPMS_Mag();
}

//________________________________________________________________________________________
EdbMomentumEstimator::~EdbMomentumEstimator()
{
  SafeDelete(eF1X);
  SafeDelete(eF1Y);
  SafeDelete(eGX);
  SafeDelete(eGY);
}

//________________________________________________________________________________________
void EdbMomentumEstimator::SetParPMS_Mag()
{
  // set the default values for parameters used in PMS_Mag
  eX0 = 5600;
  eFlagt=0;

  eDT0  = 0.0021;
  eDT1  = 0.0054; 
  eDT2  = 0.;

 // eDTx0 =  0.00156;
  eDTx0 =  0.0021;
  eDTx1 =  0.0093;
  eDTx2 = 0.;

 // eDTy0 =  0.00156;
  eDTy0 =  0.0021;
  eDTy1 =  0.0093;
  eDTy2 = 0.;
}

//________________________________________________________________________________________
void EdbMomentumEstimator::Print()
{
  printf("EdbMomentumEstimator:\n");
  printf("eX0 = %f \n", eX0);
  printf("eDT0,  eDT1,  eDT2 = %f %f %f\n", eDT0, eDT1, eDT2);
  printf("eDTx0, eDTx1, eDTx2 = %f %f %f\n", eDTx0, eDTx1, eDTx2);
  printf("eDTy0, eDTy1, eDTy2 = %f %f %f\n", eDTy0, eDTy1, eDTy2);
}

//________________________________________________________________________________________
float EdbMomentumEstimator::PMSang(EdbTrackP tr)
{
  // Version rewised by VT 13/05/2008
  //
  // Momentum estimation by multiple scattering (Annecy implementation Oct-2007)
  //
  // Input: tr       - track
  //                   pass track by value - so we can modify the local copy
  //
  // calculate momentum in transverse and in longitudinal projections using the different 
  // measurements errors parametrisation

  int nseg = tr.N();
  if(nseg<2)   { Log(1,"PMSang","Warning! nseg<2 (%d)- impossible estimate momentum!",nseg);             return -99;}
  int npl = tr.Npl();
  if(npl<nseg) { Log(1,"PMSang","Warning! npl<nseg (%d, %d) - use track.SetCounters() first",npl,nseg);  return -99;}
  int plmax = Max( tr.GetSegmentFirst()->PID(), tr.GetSegmentLast()->PID() ) + 1;
  if(plmax<1||plmax>1000)   { Log(1,"PMSang","Warning! plmax = %d - correct the segments PID's!",plmax); return -99;}

  float xmean,ymean,zmean,txmean,tymean,wmean;
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters
  EdbAffine2D aff;
  aff.ShiftX(-xmean);
  aff.ShiftY(-ymean);
  aff.Rotate( -ATan2(tymean,txmean) );                       // rotate track to get longitudinal as tx, transverse as ty angle
  tr.Transform(aff);
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters

  int minentr  = 4;               // min number of entries in the cell to accept the cell for fitting
  int stepmax  = npl/minentr;     // max step
  int size     = stepmax+1;       // vectors size

  TVectorF dax(size), day(size);
  TArrayI  nentr(size);

  EdbSegP *s1,*s2;
  for(int ist=1; ist<=stepmax; ist++)         // cycle by the step size
    {
      for(int i1=0; i1<nseg-1; i1++)          // cycle by the first seg
	{
	  s1 = tr.GetSegment(i1);
	  for(int i2=i1+1; i2<nseg; i2++)      // cycle by the second seg
	    {
	      s2 = tr.GetSegment(i2);
	      int icell = Abs(s2->PID()-s1->PID());
	      if( icell == ist ) {
		dax[icell-1]   += ( (ATan(s2->TX())- ATan(s1->TX())) * (ATan(s2->TX())- ATan(s1->TX())) );
		day[icell-1]   += ( (ATan(s2->TY())- ATan(s1->TY())) * (ATan(s2->TY())- ATan(s1->TY())) );
		nentr[icell-1] +=1;
	      }
	    }
	}
    }
 
  float maxX =0;                                  // maximum value for the function fit
  TVector vind(size), errvind(size);
  TVector errdax(size), errday(size);
  int ist=0;                                      // use the counter for case of missing cells 
  for(int i=0; i<size; i++) 
    {
      if( nentr[i] >= minentr ) {
	vind[ist]    = i+1;                           // x-coord is defined as the number of cells
	dax[ist]     = Sqrt( dax[ist]/nentr[i] );
	day[ist]     = Sqrt( day[ist]/nentr[i] );
	errvind[ist] = 0.25;
	errdax[ist]  = dax[ist]/CellWeight(npl,i+1);
	errday[ist]  = day[ist]/CellWeight(npl,i+1);
	maxX         = vind[ist];
	ist++;
      }
    }

  float dtx = eDTx0 + eDTx1*Abs(txmean) + eDTx2*txmean*txmean;  // measurements errors parametrization
  dtx*=dtx;
  float dty = eDTy0 + eDTy1*Abs(tymean) + eDTy2*tymean*tymean;  // measurements errors parametrization
  dty*=dty;

  float Zcorr = Sqrt(1+txmean*txmean+tymean*tymean);
  float x0    = eX0/1000/Zcorr;                       // the effective rad length in [mm]
 
  SafeDelete(eF1X);
  SafeDelete(eF1Y);
  SafeDelete(eGX);
  SafeDelete(eGY);

  eF1X = MCSErrorFunction("eF1X",x0,dtx);    eF1X->SetRange(0,maxX);
  eF1X->SetParameter(0,1.);                             // starting value for momentum in GeV
  eF1Y = MCSErrorFunction("eF1Y",x0,dty);    eF1Y->SetRange(0,maxX);
  eF1Y->SetParameter(0,1.);                             // starting value for momentum in GeV

  eGX=new TGraphErrors(vind,dax,errvind,errdax);
  eGX->Fit("eF1X","MQR");
  ePx=eF1X->GetParameter(0);
  eDPx=eF1X->GetParError(0);

  eGY=new TGraphErrors(vind,day,errvind,errday);
  eGY->Fit("eF1Y","MQR");
  ePy=eF1Y->GetParameter(0);
  eDPy=eF1Y->GetParError(0);

  EstimateMomentumError( ePx, npl, txmean, ePXmin, ePXmax );
  EstimateMomentumError( ePy, npl, tymean, ePYmin, ePYmax );

  float wx = 1./eDPx/eDPx;
  float wy = 1./eDPy/eDPy;
  float p  = (ePx*wx + ePy*wy)/(wx+wy);   // TODO: check on MC the biases of different estimations

  printf("id=%6d (%2d/%2d) px=%7.2f +-%5.2f (%6.2f : %6.2f)    py=%7.2f +-%5.2f  (%6.2f : %6.2f)  pmean =%7.2f\n",
	 tr.ID(),npl,nseg,ePx,eDPx,ePXmin, ePXmax,ePy,eDPy,ePYmin, ePYmax, p);

  return p;
}

//________________________________________________________________________________________
float EdbMomentumEstimator::CellWeight(int npl, int m)
{
  // npl - number of plates, m - the cell thickness in plates
  // return the statistical weight of the cell to estimate the measurement 
  // error as dax[i]/CellWeight(npl,i+1)
  // TODO: elaborate correctly the missing segments

  //return  Sqrt(npl/m);  // the simpliest estimation no shift, no correlations

  return Sqrt( npl/m + 1./m/m*( npl*(m-1) - m*(m-1)/2.) );
}

//________________________________________________________________________________________
TF1 *EdbMomentumEstimator::MCSErrorFunction(const char *name, float x0, float dtx)
{
  //        dtx - the plane angle measurement error
  // return the function of the expected angular deviation vs range
  //
  // use the Highland-Lynch-Dahl formula for theta_rms_plane = 13.6 MeV/bcp*z*sqrt(x/x0)*(1+0.038*log(x/x0))  (PDG)
  // so the expected measured angle is sqrt( theta_rms_plane**2 + dtx**2)
  //
  // The constant term im the scattering formula is not 13.6 but 14.64, which
  // is the right reevaluated number, due to a calculation with the moliere
  // distribution. 13.6 is an approximation. See Geant3 or 4 references for more explanations.???????
  //
  // err(x) = sqrt(k*x*(1+0.038*log(x/x0))/p**2 + dtx)

  //  float k   = 14.64*14.64/x0;
  // 14.64*14.64/1000/1000 = 0.0002143296  - we need p in GeV
  // 13.6*13.6/1000/1000   = 0.0001849599  - we need p in GeV

  return new TF1(name,Form("sqrt(0.0002143296*x/%f*(1+0.038*log(x/(%f)))/([0])**2+%f)",x0,x0,dtx));
  //return new TF1(name,Form("sqrt(0.0001849599*x/%f*(1+0.038*log(x/(%f)))/([0])**2+%f)",x0,x0,dtx));
}

//________________________________________________________________________________________
void EdbMomentumEstimator::DrawPlots()
{
  // example of the plots available after PMSang
  gStyle->SetOptFit(11111);
  TCanvas *c1 = new TCanvas();
  c1->Divide(1,2);

  c1->cd(1);
  TGraphErrors *gx = new TGraphErrors(*eGX);
  gx->SetTitle("Theta vs cell (longitudinal component)");
  gx->Draw("ALPR");
  TF1 *fxmin = new TF1(*(eF1X));
  fxmin->SetLineColor(kBlue);
  fxmin->SetParameter(0,ePXmin);
  fxmin->Draw("same");
  TF1 *fxmax = new TF1(*(eF1X));
  fxmax->SetLineColor(kBlue);
  fxmax->SetParameter(0,ePXmax);
  fxmax->Draw("same");

  c1->cd(2);
  TGraphErrors *gy = new TGraphErrors(*eGY);
  gy->SetTitle("Theta vs cell (transverse component)");
  gy->Draw("ALPR");
  gy->Print();
  TF1 *fymin = new TF1(*(eF1Y));
  fymin->SetLineColor(kBlue);
  fymin->SetParameter(0,ePYmin);
  fymin->Draw("same");
  TF1 *fymax = new TF1(*(eF1Y));
  fymax->SetLineColor(kBlue);
  fymax->SetParameter(0,ePYmax);
  fymax->Draw("same");
}

//________________________________________________________________________________________
float EdbMomentumEstimator::PMS_Mag(EdbTrackP &tr)
{

  // Momentum resolution by Multiple scattering (Annecy implementation Oct-2007)
  //
  // Input: tr       - track
 
  // detheta angular dependance : 
  // To avoid angular dependance, the transverse coordinate is used for track angles more than 0.1 rad. 
  // This gives a worse resolution as the 3D angle (used with angular dependance parametrisation),
  // but gives right momentum measurement, biased in the 3D case.

  // The constant term im the scattering formula is not 13.6 but 14.64, which
  // is the right reevaluated number, due to a calculation with the moliere
  // distribution. 13.6 is an approximation. See Geant3 or 4 references for more explanations.

  float x0 = eX0/1000.;
  float k = 14.64*14.64/x0;                 //rad length in [mm]
  float x,y,z,tx,ty,w;
  FitTrackLine(tr,x,y,z,tx,ty,w);
  float theta0 = Sqrt(tx*tx+ty*ty);

  EdbSegP *s, *st;
  int nseg = tr.N();
  int npl = tr.Npl();
  if(nseg<2)   { Log(1,"PMS_Mag","Warning! nseg<2 (%d)- impossible estimate momentum!",nseg);            return 0;}
  if(npl<nseg) { Log(1,"PMS_Mag","Warning! npl<nseg (%d, %d) - use track.SetCounters() first",npl,nseg); return 0;}
  int plmax = Max( tr.GetSegmentFirst()->PID(), tr.GetSegmentLast()->PID() ) + 1;
  if(plmax<1||plmax>1000)   { Log(1,"PMS_Mag","Warning! plmax = %d - correct the segments PID's!",plmax); return 0;}

  TArrayF theta2(plmax),dtheta(plmax),theta(plmax);
  TArrayF theta2x(plmax),dthetax(plmax),thetax(plmax);
  TArrayF theta2y(plmax),dthetay(plmax),thetay(plmax);
  TArrayF thick(plmax), thickx(plmax), thicky(plmax);
  TArrayF dx(plmax);

  TArrayI sPID(plmax);
  TArrayF setx(plmax), sety(plmax);
  double   P=0, dP=0, Px=0, dPx=0, Py=0, dPy=0;
  double   slopecorx=0, slopecory=0;
  double   sigx=0, sigy=0;
  int      lastci=0, firstci=0;
  int control=-1;

  st=tr.GetSegment(0);
  for(int ci=0;ci<nseg;ci++)
    {
      s=tr.GetSegment(ci);
      if(s!=NULL)
	{
	  sPID[ci]=s->PID();
	  if (control==-1) {control=1; firstci=s->PID();}
	  slopecorx=slopecorx+s->TX();
	  slopecory=slopecory+s->TY();
	  lastci=sPID[ci];
	}
    }
  if (firstci<lastci) lastci=firstci;
    
  slopecorx=slopecorx/nseg;
  slopecory=slopecory/nseg;
  double cor=slopecorx*slopecorx+slopecory*slopecory;
  double Zeff=Sqrt(1+cor);
  float PHI=atan2( slopecorx, slopecory);
  
   for(int ci=0;ci<nseg;ci++)
    {
      s=tr.GetSegment(ci);
      if(s!=NULL)
	{
	 setx[s->PID()]=s->TY()*cos(-PHI)-s->TX()*sin(-PHI);   // longitudinal coordinate
	 sety[s->PID()]=s->TX()*cos(-PHI)+ s->TY()*sin(-PHI);  // transversal coordinate	
	}
    }
    
    float tl=st->TY()*cos(-PHI)-st->TX()*sin(-PHI); 
    float tt=st->TX()*cos(-PHI)+ st->TY()*sin(-PHI); 
  float theta0b=Sqrt(tl*tl+tt*tt);  
  float detheta=0, dty=0, dtx=0;
  detheta = eDT0  + eDT1*Abs(theta0b)+eDT2*theta0b*theta0b;   detheta*=detheta;
  dtx     = eDTx0 + eDTx1*Abs(tl) + eDTx2*tl*tl ;             dtx*=dtx;
  dty     = eDTy0 + eDTy1*Abs(tt) + eDTy2*tt*tt ;             dty*=dty;
  TF1 *f1=new TF1("f1",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/([0])**2+%f)",k,x0,detheta),0,14);  // 0-14 the fitting range (cells)
  f1->SetParameter(0,1000.);                             // strating value for momentum in MeV
  TF1 *f1x=new TF1("f1x",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/([0])**2+%f)",k,x0,dtx),0,14);  // 0-14 the fitting range (cells)
  f1x->SetParameter(0,1000.);                             // strating value for momentum in MeV
  TF1 *f1y=new TF1("f1y",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/([0])**2+%f)",k,x0,dty),0,14);  // 0-14 the fitting range (cells)
  f1y->SetParameter(0,1000.);                             // strating value for momentum in MeV



//-_-_-_-_-_-_-_-_-_  disable to large angles compared to rms

  for(int ci=0;ci<npl;ci++)
     {
     if (setx[ci]!=0)
      {
       sigx=sigx+(setx[ci]-slopecorx)*(setx[ci]-slopecorx);
       sigy=sigy+(sety[ci]-slopecory)*(sety[ci]-slopecory);
      }
     }
   sigx=sqrt(sigx/nseg);
   sigy=sqrt(sigy/nseg); 
  for(int ci=0;ci<nseg;ci++)
    {
      s=tr.GetSegment(ci);
      if(s!=NULL)
	{
	 sPID[ci]=s->PID();
	 if(fabs(s->TX()-slopecorx)>3*sigx) setx[sPID[ci]]=0;
	 if(fabs(s->TY()-slopecory)>3*sigy) sety[sPID[ci]]=0;
	}
    }    
//-_-_-_-_-_-_-_-_-_  


for(int plate=0;plate<npl;plate++)
    {
      setx[plate]=setx[plate+lastci];
      sety[plate]=sety[plate+lastci];
    }
  int Ncell=0, NcellA=0, Ncellx=0, Ncelly=0;
  
  
  for (int m=0;m<tr.Npl()-1;m++)
    {
      Ncell++;

      theta2[Ncell-1]=0;dtheta[Ncell-1]=0;theta[Ncell-1]=0;
      theta2x[Ncell-1]=0;dthetax[Ncell-1]=0;thetax[Ncell-1]=0;
      theta2y[Ncell-1]=0;dthetay[Ncell-1]=0;thetay[Ncell-1]=0;

      int dim=0, dimx=0, dimy=0;
      int nshift=Ncell-1;
      if(nshift>npl-Ncell-1){ nshift=npl-Ncell-1; }

      int nhole=0;
      for(int j=0;j<nshift+1;j++)
	{
	  double x = (npl-(j+1))/Ncell;
	  int nmes = (int)x;
	  nhole=0;
	  for (int i=0;i<nmes;i++)
	    {
	      if ( (setx[Ncell*i+j+Ncell]!=0&&setx[Ncell*i+j]!=0)&&(sety[Ncell*i+j+Ncell]!=0&&sety[Ncell*i+j]!=0))
		{
		  theta2[Ncell-1] =
		    theta2[Ncell-1] +
		    Power( (ATan(setx[Ncell*i+j+Ncell])-ATan(setx[Ncell*i+j])), 2 ) +
		    Power( (ATan(sety[Ncell*i+j+Ncell])-ATan(sety[Ncell*i+j])), 2 );
		  dim++;
		 }
		if ( (setx[Ncell*i+j+Ncell]!=0&&setx[Ncell*i+j]!=0) )
		 {
		  theta2x[Ncell-1] =
		    theta2x[Ncell-1] +
		    Power( (ATan(setx[Ncell*i+j+Ncell])-ATan(setx[Ncell*i+j])), 2 );
		  dimx++;
		 }
	        if ( (sety[Ncell*i+j+Ncell]!=0&&sety[Ncell*i+j]!=0) )
		 {
		  theta2y[Ncell-1] =
		    theta2y[Ncell-1] +
		    Power( (ATan(sety[Ncell*i+j+Ncell])-ATan(sety[Ncell*i+j])), 2 );
		  dimy++;
		 }
		
	      else nhole++;
	    }
	}//end loop on shifts

      if (dim!=0)
	{
	 NcellA++;
	  theta[NcellA-1]=Sqrt(theta2[Ncell-1]/(Zeff*2*dim));
	  double errstat=theta[NcellA-1]/Sqrt(4*dim);
	  dtheta[NcellA-1]=errstat;
	 thick[NcellA-1]=Ncell;
	}
      if (dimx!=0&&dimx!=1)
	{
	 Ncellx++;
	  thetax[Ncellx-1]=Sqrt(theta2x[Ncell-1]/(Zeff*dimx));
	  dthetax[Ncellx-1]=thetax[Ncellx-1]/Sqrt(2*dimx);
	 thickx[Ncellx-1]=Ncell;
	 if (dimx<3) dthetax[Ncellx-1]=detheta*1000/2;	  
	}
      if (dimy!=0&&dimy!=1)
	{
	 Ncelly++;
	  thetay[Ncelly-1]=Sqrt(theta2y[Ncell-1]/(Zeff*dimy));
	  dthetay[Ncelly-1]=thetay[Ncelly-1]/Sqrt(2*dimy);
	 thicky[Ncelly-1]=Ncell;
	 if (dimy<3) dthetay[Ncelly-1]=detheta*1000/2;
	}
	
   
    //  else{ P=0.5; break;}

     // thick[Ncell-1]=Ncell;
      dx[Ncell-1]=0.25;                // not important

    } //end loop on Ncell

  TGraphErrors *ef1=new TGraphErrors(npl-1,thick.GetArray(),theta.GetArray(),dx.GetArray(),dtheta.GetArray());
  ef1->Fit("f1","MRQ");
  P=f1->GetParameter(0);
  dP=f1->GetParError(0);

  P=fabs(P/1000.);

  if (P>50.||P==1) P=-99.;

  delete ef1;
  delete f1;

  TGraphErrors *ef1x=new TGraphErrors(npl-1,thickx.GetArray(),thetax.GetArray(),dx.GetArray(),dthetax.GetArray());
  ef1x->Fit("f1x","MRQ");
  Px=f1x->GetParameter(0);
  dPx=f1x->GetParError(0);

  Px=fabs(Px/1000.);

  if (Px>50.||Px==1) Px=-99.;

  delete ef1x;
  delete f1x;

  TGraphErrors *ef1y=new TGraphErrors(npl-1,thicky.GetArray(),thetay.GetArray(),dx.GetArray(),dthetay.GetArray());
  ef1y->Fit("f1y","MRQ");
  Py=f1y->GetParameter(0);
  dPy=f1y->GetParError(0);

  Py=fabs(Py/1000.);

  if (Py>50.||Py==1) Py=-99.;

  delete ef1y;
  delete f1y; 
  
//  cout<<" p=" <<P<<" px="<< Px<<" py=" <<Py<<endl;



//----------------------------
  float *DP;
  if (theta0<0.1) 
   {
    eP=P;
    DP=GetDP(P,nseg,theta0); 
    eFlagt=0;   
   }
  else
   {
      eP=Py;
      DP=GetDP(Py,nseg,Abs(ty));
      eFlagt=2;
     
   }
   if (eP>0) 
   {   
    ePmin=DP[0];
    ePmax=DP[1];
   }
    else {ePmin=-99; ePmax=-99;}
   
  if (eFlagt==0) return (float)P;
  if (eFlagt==1) return (float)Px;
  if (eFlagt==2) return (float)Py;


  return 0.;
}

//________________________________________________________________________________________
float EdbMomentumEstimator::PMS_Mag(EdbTrackP &tr,TGraphErrors *h_p,TF1 *itf,TF1 *itf_min,TF1 *itf_max)
{
  // Momentum resolution by Multiple scattering (Annecy implementation Oct-2007)
  //
  // Input: tr       - track
 
  // detheta angular dependance : 
  // To avoid angular dependance, the transverse coordinate is used for track angles more than 0.1 rad. 
  // This gives a worse resolution as the 3D angle (used with angular dependance parametrisation),
  // but gives right momentum measurement, biased in the 3D case.

  // The constant term im the scattering formula is not 13.6 but 14.64, which
  // is the right reevaluated number, due to a calculation with the moliere
  // distribution. 13.6 is an approximation. See Geant3 or 4 references for more explanations.

  float x0 = eX0/1000.;
  float k = 14.64*14.64/x0;                 //rad length in [mm]
  float x,y,z,tx,ty,w;
  FitTrackLine(tr,x,y,z,tx,ty,w);  
  float theta0 = Sqrt(tx*tx+ty*ty);
// cout<<"track "<<tr.ID()<<" -> txy3D fedra"<<tx<<" "<<ty<<" "<<theta0<<endl;


  EdbSegP *s, *st;
  int nseg = tr.N();
  int npl = tr.Npl();
  if(nseg<2)   { Log(1,"PMS_Mag","Warning! nseg<2 (%d)- impossible estimate momentum!",nseg);            return 0;}
  if(npl<nseg) { Log(1,"PMS_Mag","Warning! npl<nseg (%d, %d) - use track.SetCounters() first",npl,nseg); return 0;}
  int plmax = Max( tr.GetSegmentFirst()->PID(), tr.GetSegmentLast()->PID() ) + 1;
  if(plmax<1||plmax>1000)   { Log(1,"PMS_Mag","Warning! plmax = %d - correct the segments PID's!",plmax); return 0;}

  TArrayF theta2(plmax),dtheta(plmax),theta(plmax);
  TArrayF theta2x(plmax),dthetax(plmax),thetax(plmax);
  TArrayF theta2y(plmax),dthetay(plmax),thetay(plmax);
  TArrayF thick(plmax), thickx(plmax), thicky(plmax);
  TArrayF dx(plmax);

  TArrayI sPID(plmax);
  TArrayF setx(plmax), sety(plmax);
  double   P=0, dP=0, Px=0, dPx=0, Py=0, dPy=0;
  double   slopecorx=0, slopecory=0;
  double   sigx=0, sigy=0;
  int      lastci=0, firstci=0;
  int control=-1;

  st=tr.GetSegment(0);
  for(int ci=0;ci<nseg;ci++)
    {
      s=tr.GetSegment(ci);
      if(s!=NULL)
	{
	  sPID[ci]=s->PID();
	  if (control==-1) {control=1; firstci=s->PID();}
	  slopecorx=slopecorx+s->TX();
	  slopecory=slopecory+s->TY();
	  lastci=sPID[ci];
	}
    }    
  if (firstci<lastci) lastci=firstci; 
    
  slopecorx=slopecorx/nseg;
  slopecory=slopecory/nseg;
  double cor=slopecorx*slopecorx+slopecory*slopecory;
  double Zeff=Sqrt(1+cor);
  float PHI=atan2( slopecorx, slopecory);
 
   for(int ci=0;ci<nseg;ci++)
    {
      s=tr.GetSegment(ci);
      if(s!=NULL)
	{
	 setx[s->PID()]=s->TY()*cos(-PHI)-s->TX()*sin(-PHI);   // longitudinal coordinate
	 sety[s->PID()]=s->TX()*cos(-PHI)+ s->TY()*sin(-PHI);  // transversal coordinate	
	}
    }
    
    float tl=st->TY()*cos(-PHI)-st->TX()*sin(-PHI); 
    float tt=st->TX()*cos(-PHI)+ st->TY()*sin(-PHI); 
  float theta0b=Sqrt(tl*tl+tt*tt);  
  float detheta=0, dty=0, dtx=0;
  detheta = eDT0  + eDT1*Abs(theta0b)+eDT2*theta0b*theta0b;   detheta*=detheta;
  dtx     = eDTx0 + eDTx1*Abs(tl) + eDTx2*tl*tl ;             dtx*=dtx;
  dty     = eDTy0 + eDTy1*Abs(tt) + eDTy2*tt*tt ;             dty*=dty;
  TF1 *f1=new TF1("f1",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/([0])**2+%f)",k,x0,detheta),0,14);  // 0-14 the fitting range (cells)
  f1->SetParameter(0,1000.);                             // strating value for momentum in MeV
  TF1 *f1x=new TF1("f1x",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/([0])**2+%f)",k,x0,dtx),0,14);  // 0-14 the fitting range (cells)
  f1x->SetParameter(0,1000.);                             // strating value for momentum in MeV
  TF1 *f1y=new TF1("f1y",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/([0])**2+%f)",k,x0,dty),0,14);  // 0-14 the fitting range (cells)
  f1y->SetParameter(0,1000.);                             // strating value for momentum in MeV


//-_-_-_-_-_-_-_-_-_  disable to large angles compared to rms

  for(int ci=0;ci<npl;ci++)
     {
     if (setx[ci]!=0)
      {
       sigx=sigx+(setx[ci]-slopecorx)*(setx[ci]-slopecorx);
       sigy=sigy+(sety[ci]-slopecory)*(sety[ci]-slopecory);
      }
     }
   sigx=sqrt(sigx/nseg);
   sigy=sqrt(sigy/nseg); 
  for(int ci=0;ci<nseg;ci++)
    {
      s=tr.GetSegment(ci);
      if(s!=NULL)
	{
	 sPID[ci]=s->PID();
	 if(fabs(s->TX()-slopecorx)>3*sigx) setx[sPID[ci]]=0;
	 if(fabs(s->TY()-slopecory)>3*sigy) sety[sPID[ci]]=0;
	}
    }    
//-_-_-_-_-_-_-_-_-_  


for(int plate=0;plate<npl;plate++)
    {
      setx[plate]=setx[plate+lastci];
      sety[plate]=sety[plate+lastci];
    }
  int Ncell=0, NcellA=0, Ncellx=0, Ncelly=0;
  
  
  for (int m=0;m<tr.Npl()-1;m++)
    {
      Ncell++;

      theta2[Ncell-1]=0;dtheta[Ncell-1]=0;theta[Ncell-1]=0;
      theta2x[Ncell-1]=0;dthetax[Ncell-1]=0;thetax[Ncell-1]=0;
      theta2y[Ncell-1]=0;dthetay[Ncell-1]=0;thetay[Ncell-1]=0;

      int dim=0, dimx=0, dimy=0;
      int nshift=Ncell-1;
      if(nshift>npl-Ncell-1){ nshift=npl-Ncell-1; }

      int nhole=0;
      for(int j=0;j<nshift+1;j++)
	{
	  double x = (npl-(j+1))/Ncell;
	  int nmes = (int)x;
	  nhole=0;
	  for (int i=0;i<nmes;i++)
	    {
	      if ( (setx[Ncell*i+j+Ncell]!=0&&setx[Ncell*i+j]!=0)&&(sety[Ncell*i+j+Ncell]!=0&&sety[Ncell*i+j]!=0))
		{
		  theta2[Ncell-1] =
		    theta2[Ncell-1] +
		    Power( (ATan(setx[Ncell*i+j+Ncell])-ATan(setx[Ncell*i+j])), 2 ) +
		    Power( (ATan(sety[Ncell*i+j+Ncell])-ATan(sety[Ncell*i+j])), 2 );
		  dim++;
		 }
		if ( (setx[Ncell*i+j+Ncell]!=0&&setx[Ncell*i+j]!=0) )
		 {
		  theta2x[Ncell-1] =
		    theta2x[Ncell-1] +
		    Power( (ATan(setx[Ncell*i+j+Ncell])-ATan(setx[Ncell*i+j])), 2 );
		  dimx++;
		 }
	        if ( (sety[Ncell*i+j+Ncell]!=0&&sety[Ncell*i+j]!=0) )
		 {
		  theta2y[Ncell-1] =
		    theta2y[Ncell-1] +
		    Power( (ATan(sety[Ncell*i+j+Ncell])-ATan(sety[Ncell*i+j])), 2 );
		  dimy++;
		 }
		
	      else nhole++;
	    }
	}//end loop on shifts

      if (dim!=0)
	{
	 NcellA++;
	  theta[NcellA-1]=Sqrt(theta2[Ncell-1]/(Zeff*2*dim));
	  double errstat=theta[NcellA-1]/Sqrt(4*dim);
	  dtheta[NcellA-1]=errstat;
	 thick[NcellA-1]=Ncell;
	}
      if (dimx!=0&&dimx!=1)
	{
	 Ncellx++;
	  thetax[Ncellx-1]=Sqrt(theta2x[Ncell-1]/(Zeff*dimx));
	  dthetax[Ncellx-1]=thetax[Ncellx-1]/Sqrt(2*dimx);
	 thickx[Ncellx-1]=Ncell;
	 if (dimx<3) dthetax[Ncellx-1]=detheta*1000/2;	  
	}
      if (dimy!=0&&dimy!=1)
	{
	 Ncelly++;
	  thetay[Ncelly-1]=Sqrt(theta2y[Ncell-1]/(Zeff*dimy));
	  dthetay[Ncelly-1]=thetay[Ncelly-1]/Sqrt(2*dimy);
	 thicky[Ncelly-1]=Ncell;
	 if (dimy<3) dthetay[Ncelly-1]=detheta*1000/2;
	}
	
   
    //  else{ P=0.5; break;}

     // thick[Ncell-1]=Ncell;
      dx[Ncell-1]=0.25;                // not important

    } //end loop on Ncell

  TGraphErrors *ef1=new TGraphErrors(npl-1,thick.GetArray(),theta.GetArray(),dx.GetArray(),dtheta.GetArray());
  ef1->Fit("f1","MRQ");
  P=f1->GetParameter(0);
  dP=f1->GetParError(0);

  P=fabs(P/1000.);

  if (P>50.||P==1) P=-99.;

  delete ef1;

  TGraphErrors *ef1x=new TGraphErrors(npl-1,thickx.GetArray(),thetax.GetArray(),dx.GetArray(),dthetax.GetArray());
  ef1x->Fit("f1x","MRQ");
  Px=f1x->GetParameter(0);
  dPx=f1x->GetParError(0);

  Px=fabs(Px/1000.);

  if (Px>50.||Px==1) Px=-99.;

  delete ef1x;

  TGraphErrors *ef1y=new TGraphErrors(npl-1,thicky.GetArray(),thetay.GetArray(),dx.GetArray(),dthetay.GetArray());
  ef1y->Fit("f1y","MRQ");
  Py=f1y->GetParameter(0);
  dPy=f1y->GetParError(0);

  Py=fabs(Py/1000.);

  if (Py>50.||Py==1) Py=-99.;

  delete ef1y;
  
//  cout<<" p=" <<P<<" px="<< Px<<" py=" <<Py<<endl;



//----------------------------
  float *DP;
  if (theta0<0.1) 
   {
    eP=P;
    DP=GetDP(P,nseg,theta0); 
    eFlagt=0;   
   }
  else
   {
      eP=Py;
      DP=GetDP(Py,nseg,Abs(ty));
      eFlagt=2;
     
   }
   if (eP>0) 
   {   
    ePmin=DP[0];
    ePmax=DP[1];
   }
    else {ePmin=-99; ePmax=-99;}


   char fit_type[100];
   
	if (eFlagt==0){ 
	  if(h_p){
		  if(h_p->GetN()==npl-1){
			for(int u=0;u<npl-1;u++) {
				h_p->SetPoint(u,thick.At(u),theta.At(u)); 
				h_p->SetPointError(u,dx.At(u),dtheta.At(u));
			}
			sprintf(fit_type,"momentum 3D_evaluation");
			h_p->SetTitle(fit_type);
			};
	  };
		
	  if(itf) f1->Copy(*itf);
	  
	TF1 *fmin=new TF1("fmin",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/(%f)**2+%f)",k,x0,ePmin*1000,detheta),0,14);  // 0-14 the fitting range (cells)
    fmin->SetParameter(0,1000.);
    fmin->SetLineColor(2);
    TF1 *fmax=new TF1("fmax",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/(%f)**2+%f)",k,x0,ePmax*1000,detheta),0,14);  // 0-14 the fitting range (cells)
    fmax->SetLineColor(4);
    fmax->SetParameter(0,1000.);

	if(itf_min) fmin->Copy(*itf_min);
	if(itf_max) fmax->Copy(*itf_max);
	
	if(fmin) {delete fmin;}
    if(fmax) {delete fmax;}

	return (float)P;
	}
  if (eFlagt==1) { 
	  if(h_p){
		  if(h_p->GetN()==npl-1){
			for(int u=0;u<npl-1;u++) {
				h_p->SetPoint(u,thickx.At(u),thetax.At(u)); 
				h_p->SetPointError(u,dx.At(u),dthetax.At(u));
			}
			sprintf(fit_type,"momentum 2D_evaluation (long.)");
			h_p->SetTitle(fit_type);
			};
	  };//close if(h_p)
			
	  if(itf) f1x->Copy(*itf);

	  TF1 *fmin=new TF1("fmin",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/(%f)**2+%f)",k,x0,ePmin*1000,dtx),0,14);  // 0-14 the fitting range (cells)
      fmin->SetParameter(0,1000.);
      fmin->SetLineColor(2);
      TF1 *fmax=new TF1("fmax",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/(%f)**2+%f)",k,x0,ePmax*1000,dtx),0,14);  // 0-14 the fitting range (cells)
      fmax->SetLineColor(4);
      fmax->SetParameter(0,1000.);

	  if(itf_min) fmin->Copy(*itf_min);
	  if(itf_max) fmax->Copy(*itf_max);
	
	  if(fmin) {delete fmin;}
      if(fmax) {delete fmax;}

	  return (float)Px;
	}
  if (eFlagt==2) { 
	  if(h_p){
		  if(h_p->GetN()==npl-1){
			for(int u=0;u<npl-1;u++) {
				h_p->SetPoint(u,thicky.At(u),thetay.At(u)); 
				h_p->SetPointError(u,dx.At(u),dthetay.At(u));
			}
			sprintf(fit_type,"momentum 2D_evaluation (transv.)");
		    h_p->SetTitle(fit_type);
		  };
	  };//close if(h_p)

	  if(itf) f1y->Copy(*itf);
	 
	TF1 *fmin=new TF1("fmin",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/(%f)**2+%f)",k,x0,ePmin*1000,dty),0,14);  // 0-14 the fitting range (cells)
    fmin->SetParameter(0,1000.);
    fmin->SetLineColor(2);
    TF1 *fmax=new TF1("fmax",Form("sqrt(%f*x*(1+0.038*log(x/(%f)))/(%f)**2+%f)",k,x0,ePmax*1000,dty),0,14);  // 0-14 the fitting range (cells)
    fmax->SetLineColor(4);
    fmax->SetParameter(0,1000.); 

	if(itf_min) fmin->Copy(*itf_min);
	if(itf_max) fmax->Copy(*itf_max);

	if(fmin) {delete fmin;}
    if(fmax) {delete fmax;}
	
	return (float)Py;
	}

  
  delete f1;
  delete f1x;
  delete f1y;
  
  return 0.;
}

//______________________________________________________________________________________
void EdbMomentumEstimator::EstimateMomentumError(float P, int npl, float ang, float &pmin, float &pmax)
{
  float pinv=1./P;
  float  DP=Mat(P, npl, ang );
  float pinvmin=pinv*(1-DP*1.64);
  float pinvmax=pinv*(1+DP*1.64);
  pmin=(1./pinvmax);   //90%CL minimum momentum
  pmax=(1./pinvmin);   //90%CL maximum momentum
  if (P>10.) pmax=1000000.;
}
//______________________________________________________________________________________
float *EdbMomentumEstimator::GetDP(float P, int npl, float ang)
{
 float *p90= new float[2];
 float pinv=1./P;
 
 float  DP=Mat(P, npl, ang );
       // float DP=0.25; 
      
 float pinvmin=pinv*(1-DP*1.64);
 float pinvmax=pinv*(1+DP*1.64);

 p90[0]=(1./pinvmax);   //90%CL minimum momentum
 p90[1]=(1./pinvmin);   //90%CL maximum momentum

if (P>10.) p90[1]=1000000.;

return p90;
}
//_______________________________________________________________________________________
double EdbMomentumEstimator::Mat(float P, int npl, float ang)
{
 double DP=0.;

 TMatrixD m57(2,3);
 TMatrixD m20(2,3);
 TMatrixD m14(2,3);
 TMatrixD m10(2,3);
 TMatrixD m6(2,3);
 
 m57[0][0]=0.15;
 m57[0][1]=0.19;
 m57[0][2]=0.21;
 m57[1][0]=0.22;
 m57[1][1]=0.26;
 m57[1][2]=0.28;
 
 m20[0][0]=0.26;
 m20[0][1]=0.31;
 m20[0][2]=0.36;
 m20[1][0]=0.36;
 m20[1][1]=0.44;
 m20[1][2]=0.49;
 
 
 m14[0][0]=0.29;
 m14[0][1]=0.38;
 m14[0][2]=0.42;
 m14[1][0]=0.45;
 m14[1][1]=0.51;
 m14[1][2]=0.54;
 
 
 m10[0][0]=0.37;
 m10[0][1]=0.40;
 m10[0][2]=0.44;
 m10[1][0]=0.60;
 m10[1][1]=0.60;
 m10[1][2]=0.60;

 
 m6[0][0]=0.41;
 m6[0][1]=0.43;
 m6[0][2]=0.47;
 m6[1][0]=0.60;
 m6[1][1]=0.60;
 m6[1][2]=0.60;

  
 int i=0, j=0;
 
 if(Abs(ang)<0.1) i=0;
 if(Abs(ang)>=0.1) i=1;

 if (P<=3.) j=0;
 if (P>3.&&P<=5) j=1;
 if (P>5.) j=2;
 
 if(npl>=20) DP=m57(i,j);
 if(npl<20&&npl>=14) DP=m20(i,j);
 if(npl<14&&npl>=10) DP=m14(i,j);
 if(npl<10&&npl>=6) DP=m10(i,j);
 if(npl<6&&npl>=2) DP=m6(i,j);
 
 return DP;
}
//________________________________________________________________________________________
float EdbMomentumEstimator::P_MS(EdbTrackP &tr)
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
