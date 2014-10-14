//-- Author :  Valeri Tioukov 8/05/2008

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  Track momentum estimation algorithms                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TString.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TF1.h"
#include "TArrayF.h"
#include "TVectorF.h"
#include "TVector.h"
#include "TGraph2D.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "EdbLog.h"
#include "EdbPhys.h"
#include "EdbAffine.h"
#include "EdbMomentumEstimator.h"

ClassImp(EdbMomentumEstimator);

using namespace TMath;

//________________________________________________________________________________________
EdbMomentumEstimator::EdbMomentumEstimator()
{
  eAlg   = 0; // default algorithm
  eF1    = 0;
  eF1X   = 0;
  eF1Y   = 0;
  eG     = 0;
  eGX    = 0;
  eGY    = 0;
  eGA    = 0;
  eGAX   = 0;
  eGAY   = 0;
  eVerbose=0;
  eMinEntr = 1;
  SetParPMS_Mag();
}

//________________________________________________________________________________________
EdbMomentumEstimator::~EdbMomentumEstimator()
{
  SafeDelete(eF1);
  SafeDelete(eF1X);
  SafeDelete(eF1Y);
  SafeDelete(eG);
  SafeDelete(eGX);
  SafeDelete(eGY);
}

//________________________________________________________________________________________
void EdbMomentumEstimator::Set0()
{
  eStatus=-1;
  ePx=ePy=-99;
  eDPx=eDPy=-99;
  ePXmin=ePXmax=ePYmin=ePYmax=-99;
  eP=eDP=ePmin=ePmax = -99;
}

//________________________________________________________________________________________
void EdbMomentumEstimator::SetParPMS_Mag()
{
  // set the default values for parameters used in PMS_Mag
  eX0 = 5600;

  eDT0  = 0.0021;
  eDT1  = 0.0054; 
  eDT2  = 0.;

  eDTx0 =  0.0021;
  eDTx1 =  0.0093;
  eDTx2 = 0.;

  eDTy0 =  0.0021;
  eDTy1 =  0.;     // transversal error do not depends on the angle
  eDTy2 = 0.;
}

//________________________________________________________________________________________
void EdbMomentumEstimator::Print()
{
  printf("\nEdbMomentumEstimator:\n");
  printf("Algorithm: %s\n", AlgStr(eAlg).Data());
  printf("eX0 = %f \n", eX0);
  printf("eDT0,  eDT1,  eDT2 = %f %f %f\n", eDT0, eDT1, eDT2);
  printf("eDTx0, eDTx1, eDTx2 = %f %f %f\n", eDTx0, eDTx1, eDTx2);
  printf("eDTy0, eDTy1, eDTy2 = %f %f %f\n", eDTy0, eDTy1, eDTy2);
}

//________________________________________________________________________________________
TString EdbMomentumEstimator::AlgStr(int alg)
{
  char *str = new char[256];
  switch(alg) {
    case 0:
      sprintf(str,"%d (PMSang)        Version rewised by VT 13/05/2008 (see PMSang_base) and further modified by Magali at the end of 2008", alg); break;
    case 1:
      sprintf(str,"%d (PMSang_base)   Version rewised by VT 13/05/2008",alg); break;
    case 2:
      sprintf(str,"%d (PMSang_base_A) Version revised by Andrea Russo 13/03/2009 based on PMSang_base() by VT",alg); break;
    case 3:
      sprintf(str,"%d (PMScoordinate) Momentum estimation by coordinate method A.Russo 2010",alg); break;
  }
  TString s(str);
  return s;
}

//________________________________________________________________________________________
float EdbMomentumEstimator::PMS(EdbTrackP &tr)
{
  // according to eAlg the algorithm will be selected to estimate the momentum
  
  eTrack.Clear();
  eTrack.Copy(tr);

  Set0();

  if(eAlg==0) 
    {
      return PMSang(eTrack);
    }

  if(eAlg==1) 
    {
      eStatus = PMSang_base(eTrack);
      if(eStatus>0) return eP;
      else return -100.;     // todo
    }

  if(eAlg==2) 
    {
      eStatus = PMSang_base_A(eTrack);
      if(eStatus>0) return eP;
      else return -100.;     // todo
    }

  if(eAlg==3) 
    {
      return PMScoordinate(eTrack);
    }

  return -100;
}

//________________________________________________________________________________________
float EdbMomentumEstimator::PMSang(EdbTrackP &tr)
{
  // Version rewised by VT 13/05/2008 (see PMSang_base) and further modified by Magali at the end of 2008
  //
  // Momentum estimation by multiple scattering (Annecy implementation Oct-2007)
  //
  // Input: tr  - can be modified by the function
  //
  // calculate momentum in transverse and in longitudinal projections using the different 
  // measurements errors parametrisation
   
  int nseg = tr.N();
  int npl=tr.Npl();
  if(nseg<2)   { Log(1,"PMSang","Warning! nseg<2 (%d)- impossible estimate momentum!",nseg);             return -99;}
  if(npl<nseg) { Log(1,"PMSang","Warning! npl<nseg (%d, %d) - use track.SetCounters() first",npl,nseg);  return -99;}
  int plmax = Max( tr.GetSegmentFirst()->PID(), tr.GetSegmentLast()->PID() ) + 1;
  if(plmax<1||plmax>1000)   { Log(1,"PMSang","Warning! plmax = %d - correct the segments PID's!",plmax); return -99;}

 
  float xmean,ymean,zmean,txmean,tymean,wmean;
  float xmean0,ymean0,zmean0,txmean0,tymean0,wmean0;
  FitTrackLine(tr,xmean0,ymean0,zmean0,txmean0,tymean0,wmean0);    // calculate mean track parameters
  float tmean=Sqrt(txmean0*txmean0+ tymean0*tymean0);
  
  EdbSegP *aas;
  float sigmax=0, sigmay=0;
  for (int i =0;i<tr.N();i++)
    {
      aas=tr.GetSegment(i); 
      sigmax+=(txmean0-aas->TX())*(txmean0-aas->TX());   
      sigmay+=(tymean0-aas->TY())*(tymean0-aas->TY());   
    } 
  sigmax= Sqrt(sigmax/tr.N());
  sigmay= Sqrt(sigmay/tr.N());
  for (int i =0;i<tr.N();i++)
    {
      aas=tr.GetSegment(i);
      if(Abs(aas->TX()-txmean0)>3*sigmax||Abs(aas->TY()-tymean0)>3*sigmay) { 
      aas->Set(aas->ID(),aas->X(),aas->Y(),0.,0.,aas->W(),aas->Flag());}
    } 

  FitTrackLine(tr,xmean0,ymean0,zmean0,txmean0,tymean0,wmean0);

  //  EdbAffine2D aff;
  //  aff.ShiftX(-xmean0);
  //  aff.ShiftY(-ymean0);
  //  aff.Rotate( -ATan2(txmean0,tymean0) );   // rotate track to get longitudinal as tx, transverse as ty angle
  //  tr.Transform(aff);
  float PHI=atan2(txmean0,tymean0);
  for (int i =0;i<tr.N();i++)
    {
      aas=tr.GetSegment(i);
      float slx=aas->TY()*cos(-PHI)-aas->TX()*sin(-PHI);
      float sly=aas->TX()*cos(-PHI)+ aas->TY()*sin(-PHI);
      aas->Set(aas->ID(),aas->X(),aas->Y(),slx,sly,aas->W(),aas->Flag());
    }
  
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters
  



  // -- start calcul --

  int minentr  = eMinEntr;               // min number of entries in the cell to accept the cell for fitting
  int stepmax  = npl-1; //npl/minentr;     // max step
  const int size     = stepmax+1;       // vectors size

  TVectorF da(size), dax(size), day(size);
  TArrayI  nentr(size), nentrx(size), nentry(size);

  EdbSegP *s1,*s2;
  for(int ist=1; ist<=stepmax; ist++)         // cycle by the step size
    {
      for(int i1=0; i1<nseg-1; i1++)          // cycle by the first seg
	{
	  s1 = tr.GetSegment(i1);
	  if(!s1) continue;
	  for(int i2=i1+1; i2<nseg; i2++)      // cycle by the second seg
	    {
	      s2 = tr.GetSegment(i2);
	      if(!s2) continue;
	      int icell = Abs(s2->PID()-s1->PID());
	      if( icell == ist ) 
		{
		  if (s2->TX()!=0&&s1->TX()!=0)
		    { 
		      dax[icell-1]   += ( (ATan(s2->TX())- ATan(s1->TX())) * (ATan(s2->TX())- ATan(s1->TX())) );
		      nentrx[icell-1]+=1;
		    }
		  if (s2->TY()!=0&&s1->TY()!=0)
		    { 
		      day[icell-1]   += ( (ATan(s2->TY())- ATan(s1->TY())) * (ATan(s2->TY())- ATan(s1->TY())) );
		      nentry[icell-1]+=1;		   
		    }
		  if (s2->TX()!=0&&s1->TX()!=0&&s2->TY()!=0&&s1->TY()!=0)
		    {
		      da[icell-1]   += (( (ATan(s2->TX())- ATan(s1->TX())) * (ATan(s2->TX())- ATan(s1->TX())) ) 
					+ ( (ATan(s2->TY())-ATan(s1->TY())) * (ATan(s2->TY())- ATan(s1->TY())) ));
		      nentr[icell-1] +=1;
		    }
		}
	    }
	}
    }
 
  float Zcorr = Sqrt(1+txmean0*txmean0+tymean0*tymean0);  // correction due to non-zero track angle and crossed lead thickness

  int maxX =0, maxY=0, max3D=0;                                  // maximum value for the function fit
  TVectorF vindx(size), errvindx(size),vindy(size), errvindy(size),vind3d(size), errvind3d(size);
  TVectorF errda(size), errdax(size), errday(size);
  int ist=0,  ist1=0, ist2=0;                          // use the counter for case of missing cells 
  for(int i=0; i<size; i++) 
    {
      if( nentrx[i] >= minentr && Abs(dax[i])<0.1) 
	{
	  vindx[ist]    = i+1;                            // x-coord is defined as the number of cells
	  errvindx[ist] = .25;
	  dax[ist]    = Sqrt( dax[i]/(nentrx[i]*Zcorr) );
	  errdax[ist] = dax[ist]/Sqrt(2*nentrx[i]);//CellWeight(npl,i+1);    //   Sqrt(npl/vind[i]);
	  ist++;
	  maxX=i+1;
	}
      if( nentry[i] >= minentr && Abs(day[i])<0.1) 
	{
	  vindy[ist1]    = i+1;                            // x-coord is defined as the number of cells
	  errvindy[ist1] = .25;
	  day[ist1]    = Sqrt( day[i]/(nentry[i]*Zcorr) );
	  errday[ist1] = day[ist1]/Sqrt(2*nentry[i]);//CellWeight(npl,i+1);
	  ist1++;
	  maxY=i+1;
	}      
      if( nentr[i] >= minentr/2 && Abs(da[i])<0.1 ) 
	{
	  vind3d[ist2]    = i+1;                            // x-coord is defined as the number of cells
	  errvind3d[ist2] = .25;
	  da[ist2]    = Sqrt( da[i]/(2*nentr[i]*Zcorr) );
	  errda[ist2] = da[ist2]/Sqrt(4*nentr[i]);//CellWeight(npl,i+1));	
	  ist2++;
	  max3D=i+1;
	}
    }

  float dt = eDT0 + eDT1*Abs(tmean) + eDT2*tmean*tmean;  // measurements errors parametrization
  dt*=dt;
  float dtx = eDTx0 + eDTx1*Abs(txmean) + eDTx2*txmean*txmean;  // measurements errors parametrization
  dtx*=dtx;
  float dty = eDTy0 + eDTy1*Abs(tymean) + eDTy2*tymean*tymean;  // measurements errors parametrization
  dty*=dty;
  
  float x0    = eX0/1000;      
  float chi2_3D =0;
  float chi2_T =0;
  float chi2_L =0;
  
  SafeDelete(eF1);
  SafeDelete(eF1X);
  SafeDelete(eF1Y);
  SafeDelete(eG);
  SafeDelete(eGX);
  SafeDelete(eGY);

  eF1X = MCSErrorFunction("eF1X",x0,dtx);    eF1X->SetRange(0,Min(14,maxX));
  eF1X->SetParameter(0,2000.);                             // starting value for momentum in GeV
  eF1Y = MCSErrorFunction("eF1Y",x0,dty);    eF1Y->SetRange(0,Min(14,maxY));
  eF1Y->SetParameter(0,2000.);                             // starting value for momentum in GeV
  eF1 = MCSErrorFunction("eF1",x0,dt);     eF1->SetRange(0,Min(14,max3D));
  eF1->SetParameter(0,2000.);                             // starting value for momentum in GeV

  if (max3D>0)
    {
      eG=new TGraphErrors(vind3d,da,errvind3d,errda);
      eG->Fit("eF1","QR");
      eP=1./1000.*Abs(eF1->GetParameter(0));
      eDP=1./1000.*eF1->GetParError(0);
      if (eP>20||eP<0||eP==2) eP=-99;
      EstimateMomentumError( eP, npl, tymean, ePmin, ePmax );
      chi2_3D = eF1->GetChisquare()/eF1->GetNDF();
      if (eVerbose) printf("P3D=%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] ; chi2_3D %6.2f\n", eP, ePmin, ePmax,chi2_3D);
    }
  if (maxX>0)
    {
      eGX=new TGraphErrors(vindx,dax,errvindx,errdax);
      eGX->Fit("eF1X","QR");
      ePx=1./1000.*Abs(eF1X->GetParameter(0));
      eDPx=1./1000.*eF1X->GetParError(0);
      if (ePx>20||ePx<0||ePx==2) ePx=-99;
      EstimateMomentumError( ePx, npl, txmean, ePXmin, ePXmax );
      chi2_L = eF1X->GetChisquare()/eF1X->GetNDF();
      if (eVerbose) printf("PL=%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] ; chi2_L %6.2f \n",ePx,ePXmin, ePXmax,chi2_L);
    }
  if (maxY>0)
    {
      eGY=new TGraphErrors(vindy,day,errvindy,errday);
      eGY->Fit("eF1Y","QR");
      ePy=1./1000.*Abs(eF1Y->GetParameter(0));
      eDPy=1./1000.*eF1Y->GetParError(0);
      if (ePy>20||ePy<0||ePy==2) ePy=-99;
      EstimateMomentumError( ePy, npl, tmean, ePYmin, ePYmax ); 
      chi2_T = eF1Y->GetChisquare()/eF1Y->GetNDF();
      if (eVerbose) printf("PT=%7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] ; chi2_T %6.2f\n", ePy, ePYmin, ePYmax,chi2_T);
    }

  float ptrue=eP;
  if (tmean>0.200&&chi2_T<chi2_3D) 
  {
     ptrue = ePy;
     if (eVerbose) printf(" For this track the evolution of the Transverse projection gives the most accurate estimate of the momentum %7.2f GeV ; 90%%C.L. range = [%6.2f : %6.2f] ; chi2_T /DoF %6.2f\n", ePy, ePYmin, ePYmax,chi2_T);
}

  //-----------------------------TO TEST with MC studies------------------------------------
  //  float wx = 1./eDPx/eDPx;
  //  float wy = 1./eDPy/eDPy;
  //  float ptest  = (ePx*wx + ePy*wy)/(wx+wy);  
  //----------------------------------------------------------------------------------------

  return ptrue;
}

//___________________________________________________________________________________________________


float EdbMomentumEstimator::PMScoordinate(EdbTrackP &tr)
{
  // Momentum estimation by coordinate method
  //
  // April 2010
  
  int nseg = tr.N();
  int npl  = tr.Npl();
  
 
  //float xmean,ymean,zmean,txmean,tymean,wmean;
  float xmean0,ymean0,zmean0,txmean0,tymean0,wmean0;
  FitTrackLine(tr,xmean0,ymean0,zmean0,txmean0,tymean0,wmean0);    // calculate mean track parameters
  float tmean=Sqrt(txmean0*txmean0+ tymean0*tymean0);

  float ang = 0.;

  /*
  for (int i =0;i<tr.N();i++)
    {
      aas=tr.GetSegment(i);
      float slx=aas->TY()*cos(-PHI)-aas->TX()*sin(-PHI);
      float sly=aas->TX()*cos(-PHI)+ aas->TY()*sin(-PHI);
      aas->Set(aas->ID(),aas->X(),aas->Y(),slx,sly,aas->W(),aas->Flag());
    }
  
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters
  */

  

  //int minentr  = eMinEntr;               // min number of entries in the cell, should not be set smaller than 5
  int nr1,nr2;

  float dx1,dx2,dy1,dy2,DX1,DY1,DX2,DY2,appx,appy;

  TVectorF da(npl), dax(npl), day(npl);
  TArrayI  nentr(npl);

  for(int i=0;i<npl;i++)
    {
      da[i]    = 0;
      dax[i]   = 0;
      day[i]   = 0;
      nentr[i] = 0;
    }

  EdbSegP *s1=0,*s2=0,*s3=0;


  for(int i =0;i<=nseg-3;i++)                   // cycle by the first  seg
    {
      s1 = tr.GetSegment(i);
      for(int j =i+1;j<=nseg-2;j++)             // cycle by the second seg
	{
	  s2 = tr.GetSegment(j);
	  for(int k =j+1;k<=nseg-1;k++)         // cycle by the third  seg
	    {
	      s3 = tr.GetSegment(k);
	      
	      nr1 = TMath::Abs(s1->PID()-s2->PID());
	      nr2 = TMath::Abs(s2->PID()-s3->PID());
	      if(nr1!=nr2) continue;            // continue if (s1,s2) and (s2,s3) correspond to different cells

	      dx1 = s2->X()-s1->X();
	      dx2 = s3->X()-s2->X();
	      dy1 = s2->Y()-s1->Y();
	      dy2 = s3->Y()-s2->Y();
	      
	      ang = 0; // ang is the rotation angle to get trasverse and longitudinal coordinates; not yet implemented 
	      DX1 = cos(ang)*dx1+sin(ang)*dy1;
	      DY1 = cos(ang)*dy1-sin(ang)*dx1;
	      
	      DX2 = cos(ang)*dx2+sin(ang)*dy2;
	      DY2 = cos(ang)*dy2-sin(ang)*dx2;

  	      appx = (  DX1 * ((s2->Z()-s3->Z())/(s1->Z()-s2->Z())) - DX2  ) * (  DX1 * ((s2->Z()-s3->Z())/(s1->Z()-s2->Z())) - DX2  );
	      appy = (  DY1 * ((s2->Z()-s3->Z())/(s1->Z()-s2->Z())) - DY2  ) * (  DY1 * ((s2->Z()-s3->Z())/(s1->Z()-s2->Z())) - DY2  );


	      dax[nr1] += appx;
	      day[nr1] += appy;
	      //da[nr1] += (  (((s2->X()-s1->X())*(s2->Z()-s3->Z())/(s1->Z()-s2->Z()))-(s3->X()-s2->X()))**2 + (((y[j]-y[i])*(z[j]-z[k])/(z[i]-z[j]))-(y[k]-y[j]))**2 ) /2. ; 
	      da[nr1] += (appx + appy)/2.; 
	      nentr[nr1] += 1;

	    }//end cycle 3rd seg

	}//end cycle 2nd seg

    }//end cycle 1st seg
  
  
  for(int i=0;i<npl;i++)
    {
      if(nentr[i]>0)
	{
	  dax[i] = sqrt(dax[i]/nentr[i]);
	  day[i] = sqrt(day[i]/nentr[i]);
	  da[i]  = sqrt(da[i]/nentr[i]);
	}
    }
  

  eG  = new TGraphErrors();
  eGX = new TGraphErrors();
  eGY = new TGraphErrors();


  int cont = 0;
  for(int i=0;i<npl;i++)
    {
      if(nentr[i]>eMinEntr)
	{
	  eGX->SetPoint(cont,i*1300,dax[i]);
	  eGX->SetPointError(cont,0,dax[i]/Sqrt(nentr[i]));
	  eGY->SetPoint(cont,i*1300,day[i]);
	  eGY->SetPointError(cont,0,day[i]/Sqrt(nentr[i]));
	  eG->SetPoint(cont,i*1300,da[i]);
	  eG->SetPointError(cont,0,da[i]/Sqrt(nentr[i]));
	  cont++;
	}
    }
  

  /*
  SafeDelete(eF1);
  SafeDelete(eF1X);
  SafeDelete(eF1Y);
  SafeDelete(eG);
  SafeDelete(eGX);
  SafeDelete(eGY);
  */

  eF1X = MCSCoordErrorFunction("eF1X",tmean,eX0);
  //eF1X->SetRange(0,Min(57,maxX));
  eF1X->SetParLimits(0,0.0001,100);
  eF1X->SetParLimits(1,0.0001,100);
  eF1X->SetParameter(0,5);                             // starting value for momentum in GeV
  eF1X->SetParameter(1,10);                              // starting value for coordinate error
  
  eF1Y = MCSCoordErrorFunction("eF1Y",tmean,eX0); 
  //eF1Y->SetRange(0,Min(57,maxY));
  eF1Y->SetParLimits(0,0.0001,100);
  eF1Y->SetParLimits(1,0.0001,100);
  eF1Y->SetParameter(0,5);                             // starting value for momentum in GeV
  eF1Y->SetParameter(1,10);                              // starting value for coordinate error
  
  eF1 = MCSCoordErrorFunction("eF1",tmean,eX0);
  //eF1->SetRange(0,Min(57,max3D));
  eF1->SetParLimits(0,0.0001,100);
  eF1->SetParLimits(1,0.0001,100);
  eF1->SetParameter(0,5);                             // starting value for momentum in GeV
  eF1->SetParameter(1,10);                              // starting value for coordinate error  


  eG->Fit("eF1");
  eGX->Fit("eF1X");
  eGY->Fit("eF1Y");


  eP  = 1./sqrt(eF1->GetParameter(0));
  ePx = 1./sqrt(eF1X->GetParameter(0));
  ePy = 1./sqrt(eF1Y->GetParameter(0));
  
  return eP;
}



//________________________________________________________________________________________
float EdbMomentumEstimator::CellWeight(int npl, int m)
{
  //--------------------------- TO BE IMPLEMENTED-----------------------------------------

  // npl - number of plates, m - the cell thickness in plates
  // return the statistical weight of the cell

  //return  Sqrt(npl/m);  // the simpliest estimation no shift, no correlations

  return 2*Sqrt( npl/m + 1./m/m*( npl*(m-1) - m*(m-1)/2.) );
  // return 1;
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


  return new TF1(name,Form("sqrt(214.3296*x/%f*((1+0.038*log(x/(%f)))**2)/([0])**2+%e)",x0,x0,dtx));

  //P is returned in MeV by this function for more convinience, but given in GeV as output.
}

//________________________________________________________________________________________
TF1 *EdbMomentumEstimator::MCSCoordErrorFunction(const char *name, float tmean,float x0)
{
  // return the function of the expected position deviation as function of range
  //
  // use the Highland-Lynch-Dahl formula for theta_rms_plane = 13.6 MeV/bcp*z*sqrt(x/x0)  (PDG)
  // so the expected measured position deviation is (1/sqrt(3))*theta_rms_plane + measurement error
  // log term in HLD formula is neglected
  
  
  return new TF1(name,Form("sqrt(([1])**2+(2./3)*((x*sqrt(1+%f**2))**3)*(0.0136**2)*[0]/%f)",tmean,x0));
  
  //P is returned in GeV
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
  if (P>1000.) pmax=10000000.;
}

//______________________________________________________________________________________
double EdbMomentumEstimator::Mat(float P, int npl, float ang)
{
  // These parametrisations at low and large angles are parametrised with MC
  // See Magali's thesis for more informations
  double DP=0.;
// new parameterisition as published
	if(Abs(ang)<0.2)   DP = ((0.397+0.019*P)/Sqrt(npl) + (0.176+0.042*P) + (-0.014-0.003*P)*Sqrt(npl));
	if(Abs(ang)>=0.2)  DP = ((1.400-0.022*P)/Sqrt(npl) + (-0.040+0.051*P) + (0.003-0.004*P)*Sqrt(npl));
  if (DP>0.80) DP=0.80;
  return DP;
}

//________________________________________________________________________________________
void EdbMomentumEstimator::DrawPlots(TCanvas *c1)
{
  // example of the plots available after PMSang
  gStyle->SetOptFit(11111);
  if(c1==NULL) c1 = new TCanvas("tf","track momentum estimation",800,600);
  c1->Divide(3,2);

  if(eAlg<2||eAlg==3)
    {
      if(eGX) {
	c1->cd(1);
	TGraphErrors *gx = new TGraphErrors(*eGX);

	if(eAlg<2)
	  {
	    gx->Draw("ALPR");
	    gx->SetTitle("Theta vs cell (transverse component)");
	    TF1 *fxmin = new TF1(*(eF1X));
	    fxmin->SetLineColor(kBlue);
	    fxmin->SetParameter(0,ePXmin);
	    fxmin->Draw("same");
	    TF1 *fxmax = new TF1(*(eF1X));
	    fxmax->SetLineColor(kBlue);
	    fxmax->SetParameter(0,ePXmax);
	    fxmax->Draw("same");
	  }
	else
	  {
	    gx->Draw("A*");
	    gx->SetTitle("Position vs cell (X component)");
	  }
	
	
      }
      
      if(eGY) {
	c1->cd(2);
	TGraphErrors *gy = new TGraphErrors(*eGY);

	if(eAlg<2)
	  {
	    gy->Draw("ALPR");
	    gy->SetTitle("Theta vs cell (longitudinal component)");
	    TF1 *fymin = new TF1(*(eF1Y));
	    fymin->SetLineColor(kBlue);
	    fymin->SetParameter(0,ePYmin);
	    fymin->Draw("same");
	    TF1 *fymax = new TF1(*(eF1Y));
	    fymax->SetLineColor(kBlue);
	    fymax->SetParameter(0,ePYmax);
	    fymax->Draw("same");
	  }
	else
	  {
	    gy->Draw("A*");
	    gy->SetTitle("Position vs cell (Y component)");
	  }
	
	
      }
      
      if(eG) {
	c1->cd(3);
	TGraphErrors *g = new TGraphErrors(*eG);

	if(eAlg<2)
	  {
	    g->Draw("ALPR");
	    g->SetTitle("Theta vs cell (3D)");
	    //g->Print();
	    TF1 *fmin = new TF1(*(eF1));
	    fmin->SetLineColor(kBlue);
	    fmin->SetParameter(0,ePmin);
	    fmin->Draw("same");
	    TF1 *fmax = new TF1(*(eF1));
	    fmax->SetLineColor(kBlue);
	    fmax->SetParameter(0,ePmax);
	    fmax->Draw("same");
	  }
	else
	  {
	    g->Draw("A*");
	    g->SetTitle("Position vs cell (3D)");
	  }
 
	
      }
    }

  if(eAlg==2)
    {
      if(eGAX) {
	c1->cd(1);
	TGraphAsymmErrors *gx = new TGraphAsymmErrors(*eGAX);
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
      }
      
      if(eGAY) {
	c1->cd(2);
	TGraphAsymmErrors *gy = new TGraphAsymmErrors(*eGAY);
	gy->SetTitle("Theta vs cell (transverse component)");
	gy->Draw("ALPR");
	TF1 *fymin = new TF1(*(eF1Y));
	fymin->SetLineColor(kBlue);
	fymin->SetParameter(0,ePYmin);
	fymin->Draw("same");
	TF1 *fymax = new TF1(*(eF1Y));
	fymax->SetLineColor(kBlue);
	fymax->SetParameter(0,ePYmax);
	fymax->Draw("same");
      }
      
      if(eGA) {
	c1->cd(3);
	TGraphAsymmErrors *g = new TGraphAsymmErrors(*eGA);
	g->SetTitle("Theta vs cell (3D)");
	g->Draw("ALPR");
	//g->Print();
	TF1 *fmin = new TF1(*(eF1));
	fmin->SetLineColor(kBlue);
	fmin->SetParameter(0,ePmin);
	fmin->Draw("same");
	TF1 *fmax = new TF1(*(eF1));
	fmax->SetLineColor(kBlue);
	fmax->SetParameter(0,ePmax);
	fmax->Draw("same");
      }
    }


  
      int nseg = eTrack.N();
      if(nseg) {
	c1->cd(4);
    TGraph2D *gxyz = new TGraph2D(nseg);
    for(int i=0; i<nseg; i++) 
      gxyz->SetPoint(i, eTrack.GetSegment(i)->X(), eTrack.GetSegment(i)->Y(), eTrack.GetSegment(i)->Z()); 
    gxyz->SetName("gxyz");
    gxyz->SetTitle("track: X vs Y vs Z");
    gxyz->SetMarkerStyle(21);
    gxyz->Draw("P");

    TGraphErrors *gtx = new TGraphErrors(nseg);
    for(int i=0; i<nseg; i++) 
      gtx->SetPoint(i, eTrack.GetSegment(i)->PID(), eTrack.GetSegment(i)->TX()); 
    TGraphErrors *gty = new TGraphErrors(nseg);
    for(int i=0; i<nseg; i++) 
      gty->SetPoint(i, eTrack.GetSegment(i)->PID(), eTrack.GetSegment(i)->TY()); 


    TVirtualPad *vp;
    vp = c1->cd(5);
    vp->SetGrid();
    gtx->SetLineColor(kBlue);
    gtx->SetMarkerStyle(24);
    gtx->Draw("ALP");

    vp = c1->cd(6);
    vp->SetGrid();
    gty->SetMarkerStyle(24);
    gty->SetLineColor(kRed);
    gty->Draw("ALP");
      }

}

//________________________________________________________________________________________
int EdbMomentumEstimator::PMSang_base(EdbTrackP &tr)
{
  // Version rewised by VT 13/05/2008
  //
  // Momentum estimation by multiple scattering (Annecy algorithm Oct-2007)
  //
  // Input: tr  - can be modified by the function
  //
  // calculate momentum in transverse and in longitudinal projections using the different 
  // measurements errors parametrisation
  // return value:  -99 - estimation impossible; 
  //                  0 - fit is not successful; 
  //                  1 - only one momentum component is fitted well
  //                  2 - both components are successfully fitted
  // "base" is stay for the original version - to be tested in comparison to the "final" version

  int nseg = tr.N();
  if(nseg<2)   { Log(1,"PMSang_base","Warning! nseg<2 (%d)- impossible estimate momentum!",nseg);             return -99;}
  int npl = tr.Npl();
  if(npl<nseg) { Log(1,"PMSang_base","Warning! npl<nseg (%d, %d) - use track.SetCounters() first",npl,nseg);  return -99;}
  int plmax = Max( tr.GetSegmentFirst()->PID(), tr.GetSegmentLast()->PID() ) + 1;
  if(plmax<1||plmax>1000)   { Log(1,"PMSang_base","Warning! plmax = %d - correct the segments PID's!",plmax); return -99;}
  Log(3,"PMSang_base","estimate track with %d segments %d plates",tr.N(), tr.Npl());

  float xmean,ymean,zmean,txmean,tymean,wmean;
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters
  EdbAffine2D aff;
  aff.ShiftX(-xmean);
  aff.ShiftY(-ymean);
  aff.Rotate( -ATan2(tymean,txmean) );                       // rotate track to get longitudinal as tx, transverse as ty angle
  tr.Transform(aff);
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters

  int minentr  = eMinEntr;        // min number of entries in the cell to accept the cell for fitting
  int stepmax  = npl/minentr;     // max step
  int size     = stepmax+1;       // vectors size

  TVectorF dax(size), day(size);
  TArrayI  nentr(size);

  Log(3,"PMSang_base","stepmax = %d",stepmax);

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
  
  const char *fitopt = "MQR"; //MQR

  bool statFitPX = false, statFitPY  = false;
  float initP = 1., minP=0., maxP=100.;                             // starting value for momentum in GeV

  eF1X = MCSErrorFunction_base("eF1X",x0,dtx);    eF1X->SetRange(0,maxX);
  eF1X->SetParameter(0, initP);
  //  eF1X->SetParameter(1, 0.002);
  eF1X->SetParLimits(0, minP, maxP);
  eGX=new TGraphErrors(vind,dax,errvind,errdax);
  eGX->Fit("eF1X",fitopt);
  ePx=eF1X->GetParameter(0);
  if( Abs(ePx-initP)<0.00001 ) {
    eF1X->SetParameter(0, 2*initP);
    eGX->Fit("eF1X",fitopt);
    ePx=eF1X->GetParameter(0);
    if( Abs(ePx - 2*initP)>0.00001 ) statFitPX=true;
  }
  else  statFitPX=true;

  if(statFitPX) eDPx=eF1X->GetParError(0);
  else { eDPx =-99; ePx = -99; }

  eF1Y = MCSErrorFunction_base("eF1Y",x0,dty);    eF1Y->SetRange(0,maxX);
  eF1Y->SetParameter(0,initP);
  eF1Y->SetParLimits(0, minP, maxP);
  eGY=new TGraphErrors(vind,day,errvind,errday);
  eGY->Fit("eF1Y",fitopt);
  ePy=eF1Y->GetParameter(0);
  if( Abs(ePy-initP)<0.00001 ) {
    eF1Y->SetParameter(0, 2*initP);
    eGY->Fit("eF1Y",fitopt);
    ePy=eF1Y->GetParameter(0);
    if( Abs(ePy - 2*initP)>0.00001 ) statFitPY=true;
  }
  else  statFitPY=true;
  if(statFitPY) eDPy=eF1Y->GetParError(0);
  else { eDPy =-99; ePy = -99; }

  EstimateMomentumError( ePx, npl, txmean, ePXmin, ePXmax );
  EstimateMomentumError( ePy, npl, tymean, ePYmin, ePYmax );

  float wx = statFitPX? 1./eDPx/eDPx : 0;
  float wy = statFitPY? 1./eDPy/eDPy : 0;
  if(statFitPX||statFitPY) {
    eP  = (ePx*wx + ePy*wy)/(wx+wy);   // TODO: check on MC the biases of different estimations
    eDP = Sqrt(1./(wx+wy));
  } else {eP = -1; eDP=-1; }

  if(gEDBDEBUGLEVEL>1)
    printf("id=%6d (%2d/%2d) px=%7.2f +-%5.2f (%6.2f : %6.2f)    py=%7.2f +-%5.2f  (%6.2f : %6.2f)  pmean =%7.2f  %d %d\n",
	   tr.ID(),npl,nseg,ePx,eDPx,ePXmin, ePXmax,ePy,eDPy,ePYmin, ePYmax, eP, statFitPX, statFitPY);
  
  return statFitPX+statFitPY;
}

//________________________________________________________________________________________
TF1 *EdbMomentumEstimator::MCSErrorFunction_base(const char *name, float x0, float dtx)
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

  return new TF1(name,Form("sqrt(0.0002143296*x/%f*((1+0.038*log(x/(%f)))**2)/([0])**2+%f)",x0,x0,dtx));
  //return new TF1(name,Form("sqrt(0.0001849599*x/%f*(1+0.038*log(x/(%f)))/([0])**2+%f)",x0,x0,dtx));
}

//________________________________________________________________________________________
float EdbMomentumEstimator::P_MS(EdbTrackP &tr)
{
  // momentum estimation by multiple scattering (first test version (VT))

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

//____________________________________________________________________________________
int EdbMomentumEstimator::PMSang_base_A(EdbTrackP &tr)
{
  // Version revised by Andrea Russo 13/03/2009 based on PMSang_base() by VT
  //
  // include asymmetrical errors in scattering VS ncell graphs, based on ChiSquare distribution
  //
  // improved check on correctness of fit result, based on chisquare cut (FitProbability > 0.05)

  int nseg = tr.N();
  if(nseg<2)   { Log(1,"PMSang_base","Warning! nseg<2 (%d)- impossible estimate momentum!",nseg);             return -99;}
  int npl = tr.Npl();
  if(npl<nseg) { Log(1,"PMSang_base","Warning! npl<nseg (%d, %d) - use track.SetCounters() first",npl,nseg);  return -99;}
  int plmax = Max( tr.GetSegmentFirst()->PID(), tr.GetSegmentLast()->PID() ) + 1;
  if(plmax<1||plmax>1000)   { Log(1,"PMSang_base","Warning! plmax = %d - correct the segments PID's!",plmax); return -99;}
  Log(3,"PMSang_base","estimate track with %d segments %d plates",tr.N(), tr.Npl());

  float xmean,ymean,zmean,txmean,tymean,wmean;
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters
  EdbAffine2D aff;
  aff.ShiftX(-xmean);
  aff.ShiftY(-ymean);
  aff.Rotate( -ATan2(tymean,txmean) );                       // rotate track to get longitudinal as tx, transverse as ty angle
  tr.Transform(aff);
  FitTrackLine(tr,xmean,ymean,zmean,txmean,tymean,wmean);    // calculate mean track parameters

  int minentr  = eMinEntr;        // min number of entries in the cell to accept the cell for fitting
  int stepmax  = npl/minentr;     // max step
  int size     = stepmax+1;       // vectors size

  TVectorF dax(size), day(size);
  TArrayI  nentr(size);

  Log(3,"PMSang_base_A","stepmax = %d",stepmax);


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
  
  TVector errdaxL(size), errdayL(size);           // L stands for low, lower error bar
  TVector errdaxH(size), errdayH(size);           // H stands for high, hogher error bar
  int ist=0;                                      // use the counter for case of missing cells 
  for(int i=0; i<size; i++) 
    {
      if( nentr[i] >= minentr ) {
	float ndf = CellWeight(npl,i+1);           // CellWeight is interpreted as the ndf in the determination of dax and day

	vind[ist]    = i+1;                           // x-coord is defined as the number of cells
	dax[ist]     = Sqrt( dax[ist]/nentr[i] );
	day[ist]     = Sqrt( day[ist]/nentr[i] );

	errvind[ist] = 0.25;

	//errdax[ist]  = dax[ist]/CellWeight(npl,i+1);
	//errday[ist]  = day[ist]/CellWeight(npl,i+1);

	errdaxL[ist]  = dax[ist] - dax[ist]/(Sqrt(TMath::ChisquareQuantile(0.84,ndf)/ndf));
	errdaxH[ist]  = dax[ist]/(Sqrt(TMath::ChisquareQuantile(0.16,ndf)/ndf)) - dax[ist];

	errdayL[ist]  = day[ist] - day[ist]/(Sqrt(TMath::ChisquareQuantile(0.84,CellWeight(npl,i+1))/ndf));
	errdayH[ist]  = day[ist]/(Sqrt(TMath::ChisquareQuantile(0.16,ndf)/ndf)) - day[ist];

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
  SafeDelete(eGAX);
  SafeDelete(eGAY);

  bool statFitPX = false, statFitPY  = false;
  float initP = 1., minP=0., maxP=100.;                             // starting value for momentum in GeV

  eF1X = MCSErrorFunction_base("eF1X",x0,dtx);    eF1X->SetRange(0,maxX);
  eF1X->SetParameter(0, initP);
  eF1X->SetParameter(1, 0.003);
  eF1X->SetParLimits(0, minP, maxP);
  
  eGAX=new TGraphAsymmErrors(vind,dax,errvind,errvind,errdaxL,errdaxH);
  
  const char *fitopt = "MQR"; //MQR
  eGAX->Fit("eF1X",fitopt);
  ePx=eF1X->GetParameter(0);
  
  /*
  if( Abs(ePx-initP)<0.00001 ) {
    eF1X->SetParameter(0, 2*initP);
    eGAX->Fit("eF1X",fitopt);
    ePx=eF1X->GetParameter(0);
    if( Abs(ePx - 2*initP)>0.00001 ) statFitPX=true;
  }
  else  statFitPX=true;
  */
  
  if(eF1X->GetChisquare() > TMath::ChisquareQuantile(0.05,eF1X->GetNDF())) //checking if fit procedure gives a reasonable result (i.e., p>0.05)
    statFitPX=false;
  else
    statFitPX=true;
  
  //statFitPX=true;


  if(statFitPX) eDPx=eF1X->GetParError(0);
  else { eDPx =-99; ePx = -99; }

  eF1Y = MCSErrorFunction_base("eF1Y",x0,dty);    eF1Y->SetRange(0,maxX);
  eF1Y->SetParameter(0,initP);
  eF1Y->SetParLimits(0, minP, maxP);

  eGAY=new TGraphAsymmErrors(vind,day,errvind,errvind,errdayL,errdayH);
  
  eGAY->Fit("eF1Y", fitopt);
  ePy=eF1Y->GetParameter(0);
  /*
  if( Abs(ePy-initP)<0.00001 ) {
    eF1Y->SetParameter(0, 2*initP);
    eGAY->Fit("eF1Y",fitopt);
    ePy=eF1Y->GetParameter(0);
    if( Abs(ePy - 2*initP)>0.00001 ) statFitPY=true;
  }
  else  statFitPY=true;
  */

  if(eF1Y->GetChisquare()> TMath::ChisquareQuantile(0.05,eF1Y->GetNDF())) //checking if fit procedure gives a reasonable result (i.e., p>0.05)
    statFitPY=false;
  else
    statFitPY=true;
  
  //statFitPY=true;

  if(statFitPY) eDPy=eF1Y->GetParError(0);
  else { eDPy =-99; ePy = -99; }

  EstimateMomentumError( ePx, npl, txmean, ePXmin, ePXmax );
  EstimateMomentumError( ePy, npl, tymean, ePYmin, ePYmax );

  float wx = statFitPX? 1./eDPx/eDPx : 0;
  float wy = statFitPY? 1./eDPy/eDPy : 0;
  if(statFitPX||statFitPY) {
    eP  = (ePx*wx + ePy*wy)/(wx+wy);   // TODO: check on MC the biases of different estimations
    eDP = Sqrt(1./(wx+wy));
  } else {eP = -1; eDP=-1; }

  if(gEDBDEBUGLEVEL>1)
    printf("id=%6d (%2d/%2d) px=%7.2f +-%5.2f (%6.2f : %6.2f)    py=%7.2f +-%5.2f  (%6.2f : %6.2f)  pmean =%7.2f  %d %d\n",
	   tr.ID(),npl,nseg,ePx,eDPx,ePXmin, ePXmax,ePy,eDPy,ePYmin, ePYmax, eP, statFitPX, statFitPY);
  
  
  return statFitPX+statFitPY;
}
