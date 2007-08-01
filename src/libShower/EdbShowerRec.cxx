//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbShowerRec                                                         //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "smatrix/Functions.hh"
#include "EdbTrackFitter.h"
#include "EdbShowerRec.h"


ClassImp(EdbShowerRec)
  //----------------------------------------------------------------
  EdbShowerRec::EdbShowerRec()
{
}
EdbShowerRec::~EdbShowerRec()
{
}

//-------------------------------------------------------------------
void EdbShowerRec::InitPiece(EdbDataPiece &piece, const char *cpfile, const char *parfile)
{
  piece.eFileNameCP  = cpfile;
  piece.eFileNamePar = parfile;
  piece.TakePiecePar();
  //piece.Print();
}
//-------------------------------------------------------------------
void EdbShowerRec::initproc( const char *def, int iopt,   const char *rcut="1" )
{ 
  dproc = new EdbDataProc(def);
  dproc->InitVolume(iopt, rcut);
  gAli = dproc->PVR();
}    
//-------------------------------------------------------------------
void EdbShowerRec::SaveResults()
{    fileout->cd();
 treesaveb->Write();
 fileout->Close();
 fileout->Delete();
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//void EdbShowerRec::rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par,int DOWN, float Rcut=100., float Tcut=0.05)
void EdbShowerRec::rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par,int DOWN)

{
  if (DOWN == 1) {recdown(num,MAXPLATE,DATA,Ncand,x0,y0,z0,tx0,ty0,TRid,piece2, piece2par);}
  if (DOWN == 0) {recup(num,MAXPLATE,DATA,Ncand,x0,y0,z0,tx0,ty0,TRid,piece2, piece2par);}
}
		    

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//void EdbShowerRec::recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par, float Rcut=100., float Tcut=0.05)
void EdbShowerRec::recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par)
{    
  
  // shower tree definition    

  treesaveb = new TTree("treebranch","tree of branchtrack");
  treesaveb->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb->Branch("sizeb30",&sizeb30,"sizeb30/I");
    
  treesaveb->Branch("isizeb",&isizeb,"isizeb/I");
  treesaveb->Branch("xb",xb,"xb[sizeb]/F");
  treesaveb->Branch("yb",yb,"yb[sizeb]/F");
  treesaveb->Branch("zb",zb,"zb[sizeb]/F");
  treesaveb->Branch("txb",txb,"txb[sizeb]/F");
  treesaveb->Branch("tyb",tyb,"tyb[sizeb]/F");
  treesaveb->Branch("nfilmb",nfilmb,"nfilmb[sizeb]/I");
  treesaveb->Branch("ntrace1simub",ntrace1simub,"ntrace1simu[sizeb]/I");
  treesaveb->Branch("ntrace2simub",ntrace2simub,"ntrace2simu[sizeb]/I");
  treesaveb->Branch("chi2btkb",chi2btkb,"chi2btkb[sizeb]/F");
  treesaveb->Branch("deltarb",deltarb,"deltarb[sizeb]/F");
  treesaveb->Branch("deltathetab",deltathetab,"deltathetab[sizeb]/F");
  treesaveb->Branch("deltaxb",deltaxb,"deltaxb[sizeb]/F");
  treesaveb->Branch("deltayb",deltayb,"deltayb[sizeb]/F");
  treesaveb->Branch("tagprimary",tagprimary,"tagprimary[sizeb]/F");
    
  fileout = new TFile("shower1.root","RECREATE");
    

  float Z0, SX0,SY0;
  float X0, Y0, Xss,Yss;
  float Delta, rayon,rayon2, diff, delta;
  float Xe[4], Ye[4];
  char fname2[128];
    
  char nme[64];
  float  a11, a12, a21, a22, bb1, bb2;
  float Zoff;
  double ZZ0[50];
  int ind;
  const Int_t eNSM = 10000;
  const Int_t eNTM = 1000;

  TArrayF  xbb(eNTM*eNSM);  
  TArrayF  ybb(eNTM*eNSM);  
  TArrayF  zbb(eNTM*eNSM);  
  TArrayF  txbb(eNTM*eNSM);  
  TArrayF  tybb(eNTM*eNSM); 
  //    TArrayF  deltarbb(eNTM*eNSM);
  TArrayI  sigbb(eNTM*eNSM);
  TArrayI add(eNTM*eNSM);  

  for(int i=0; i<eNSM; i++)
    {
      for(int j=0; j<eNTM; j++)  
	{
	  ind = i*eNTM+j;
	  xbb[ind];
	  ybb[ind];
	  zbb[ind];
	  txbb[ind];
	  tybb[ind];
	  //	    deltarbb[ind];
	  sigbb[ind];

	}
    }
   
  TList  *eS = new TList;
    
  for (int ii=0;ii<Ncand;ii++) 
    {    
      EdbShowerRec* shower = new EdbShowerRec();
	
      shower->SetID(ii);
      shower->SetX0(x0[ii]);
      shower->SetY0(y0[ii]);
      shower->SetZ0(z0[ii]);
      shower->SetTx(tx0[ii]);
      shower->SetTy(ty0[ii]);
      shower->SetTrID(TRid[ii]);
	
      shower->SetDeltarb(200.,0);
      shower->SetDelthetab(0.5,0);
      shower->SetPrimary(0,0);
      shower->SetNFilmb(1,0);
	
      shower->SetXb(x0[ii],0);
      shower->SetYb(y0[ii],0);
      shower->SetZb(z0[ii],0);
      shower->SetTXb(tx0[ii],0);
      shower->SetTYb(ty0[ii],0);
	
      ind = 0 +  ii*eNTM;

      xbb[ind] =  x0[ii]; 
      ybb[ind] =  y0[ii]; 
      zbb[ind] =  z0[ii]; 
      txbb[ind] =  tx0[ii]; 
      tybb[ind] =  ty0[ii]; 

      add[ii] = 1; 
      eS->Add(shower);	
    }
    
  EdbShowerRec* a=0;
  // save the Z position of the first sheet
  if(num<10) {sprintf(nme,"%s/0%d_00%d.par","par",num,piece2par);}
  else {sprintf(nme,"%s/%d_00%d.par","par",num,piece2par);}
  ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );
  //  ZZ0[0]= z0[0];
  ZZ0[0]= Zoff;

  int nntr;
  int ok;
  int max=0 ;
  int index = 0;
  double rr;

  float Rcut = 100.;
  float  Tcut = 0.05;
  //    float Rcut = 150.;
  //    float  Tcut = 0.15;


  float mini[5] ={-20000.,-20000.,-1,-1,0};
  float maxi[5] ={20000.,20000.,1,1,100};

  for (int d=num+1; d<MAXPLATE+1; d++){ 
    index++;
    printf( "Process plate number: %d/%d\n",index+1,MAXPLATE);	
	
    //	if(d<10) {sprintf(nme,"%s/0%d_00%d.par",pathpar2,d,piece2par);}
    //	else {sprintf(nme,"%s/%d_00%d.par",pathpar2,d,piece2par);}
    if(d<10) {sprintf(nme,"%s/0%d_00%d.par","par",d,piece2par);}
    else {sprintf(nme,"%s/%d_00%d.par","par",d,piece2par);}
	
    if(d<10){sprintf(fname2,"%s/0%d_00%d.cp.root","data",d,piece2);}
    else {sprintf(fname2,"%s/%d_00%d.cp.root","data",d,piece2);}
	
    EdbPVRec ali;
    EdbPattern *pat=0;
    EdbPattern *pat2=0;	
    EdbDataPiece Piece;
		
    InitPiece(Piece,fname2,nme);
    pat2 = new EdbPattern(0.,0., Piece.GetLayer(0)->Z(),100 );
    Z0 =  Piece.GetLayer(0)->Z();
    ReadPiece(Piece, *pat2);

    ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );
    Z0 =  Zoff;
    ZZ0[index] =  Zoff;

    int ii =0;
    TIter next(eS);
    while ((a=(EdbShowerRec*)next()))
      {
	if(a->GetZ0()>Z0) ii++;
	if(a->GetZ0()>Z0) continue;
	Xss =  a->GetX0() + fabs(Z0-a->GetZ0())*a->GetTx();
	Yss =  a->GetY0() + fabs(Z0-a->GetZ0())*a->GetTy();

	mini[0] = Xss-450.;
	mini[1] = Yss-450.;
	mini[2] = -0.45;
	mini[3] = -0.45;
	mini[4] = 0.;	    
	maxi[0] = Xss+450.;
	maxi[1] = Yss+450.;
	maxi[2] = 0.45;
	maxi[3] = 0.45;
	maxi[4] = 100;
	    
	pat = pat2->ExtractSubPattern(mini,maxi);	    
	ali.AddPattern(pat);

	EdbSegP *ss=0;
	    
	int incr=-1;
	
	for(int ip=0; ip<ali.Npatterns(); ip++) {incr++;};
	
	for(int i=0; i<ali.GetPattern(incr)->N(); i++)
	  {	    
	    ss = ali.GetPattern(incr)->GetSegment(i);
	    ok = 0;
	    if (DATA ==1){
	      X0 = a11*ss->X() + a12*ss->Y() + bb1;
	      Y0 = a21*ss->X() + a22*ss->Y() + bb2;

	      SX0 = a11*ss->TX()+ a12*ss->TY() ;
	      SY0 = a21*ss->TX()+a22*ss->TY();
	    }
	    else
	      {
		X0 = ss->X() ;
		Y0 = ss->Y() ;
		SX0 = ss->TX();
		SY0 = ss->TY();
	      }

	    if (fabs(SX0)>0.4) continue;
	    if (fabs(SY0)>0.4) continue;
		
	    if ( a->GetTrID() == ss->MCEvt()|| ss->MCEvt()<0) 
	      {
		Delta = 20.+   fabs(0.02*fabs(Z0-a->GetZ0())); // cone with 20 mrad 
		    
		rayon = 400.; // cylinder with 400 microns radius
		diff = sqrt((X0-Xss)*(X0-Xss)+(Y0-Yss)*(Y0-Yss));	   
		    
		if ( diff<rayon&&diff<Delta )
		  {				
		    if (index==1) max = 1;
		    if (index==2) max = 2;
		    if (index>2) max = 3;
			
		    for (int el=0;el<max;el++)
		      {
			//extrapolation in previous plate
			Xe[el] =  X0 - fabs(Z0-ZZ0[index-el-1])*SX0;
			Ye[el] =  Y0 - fabs(Z0-ZZ0[index-el-1])*SY0;
			nntr = add[ii];
			    
			for (int l2=0;l2<nntr;l2++) 
			  {
			    ind = l2 + ii*eNTM;

			    if (fabs(a->GetZb(l2)-(Z0-(el+1)*1300))<500)
			      {
				rayon2 = sqrt((Xe[el]-a->GetXb(l2))*(Xe[el]-a->GetXb(l2))+((Ye[el]-a->GetYb(l2))*(Ye[el]-a->GetYb(l2))));  
				delta = sqrt((SX0-a->GetTXb(l2))*(SX0-a->GetTXb(l2))+((SY0-a->GetTYb(l2))*(SY0-a->GetTYb(l2))));
				if (rayon2<Rcut&&delta<Tcut)
				  {

				    if (ok==0)
				      {
					ind = add[ii] + ii*eNTM;

					xbb[ind] =  X0 ;
					ybb[ind] =  Y0 ;
					zbb[ind] =  Z0 ;
					txbb[ind] =  SX0 ;
					tybb[ind] =  SY0 ;

					a->SetXb(X0,add[ii]);			
					a->SetYb(Y0,add[ii]);
					a->SetZb(Z0,add[ii]);
					a->SetTXb(SX0,add[ii]);
					a->SetTYb(SY0,add[ii]);
					a->SetDeltarb(rayon2,add[ii]);
					a->SetDelthetab(delta,add[ii]);
					//a->SetNFilmb( index+1,add[ii]);
					rr =  (Z0-a->GetZ0())/1300;
					a->SetNFilmb(int(round(rr)+1),add[ii]);
					if (ss->MCEvt()==a->GetTrID()) sigbb[ind] = 1;
					if (ss->MCEvt()<0) sigbb[ind] = 0;
					    
					ok =1;
					    
					add[ii]++;
				      } 					    
				    else if (ok==1&&rayon2<a->GetDeltarb(add[ii]-1)) 
				      {
					a->SetDeltarb(rayon2,add[ii]-1);
					a->SetDelthetab(delta,add[ii]-1);

				      }					    
				  }
			      }
			  }
		      }
		  }
	      }   
	  }
	ii++;
      }
  }
	
  int  i = 0;
  TIter next(eS);
  while ((a=(EdbShowerRec*)next()))
    {	
      //	number_eventb = a->GetID();
      number_eventb = a->GetTrID();

      sizeb = add[i];
      a->SetSize(add[i]);
      sizeb15 = 0;
      sizeb20 = 0;
      sizeb30 = 0;     
      isizeb = 0;


      for (int j=0;j<add[i]; j++) 
	{ 
	  if (a->GetZb(j)>a->GetZ0()+5000) isizeb = 1; //cross at least 4 plates
	  if (a->GetNFilmb(j)<16)  sizeb15++;
	  if (a->GetNFilmb(j)<21)  sizeb20++;
	  if (a->GetNFilmb(j)<31)  sizeb30++;	   
	     
	}
      a->SetSize15(sizeb15);
      a->SetSize20(sizeb20);
      a->SetSize30(sizeb30);

      for (int j=0;j<add[i]; j++) 
	{    
	  xb[j] = a->GetXb(j);
	  yb[j] = a->GetYb(j);
	  zb[j] = a->GetZb(j);
	  txb[j] = a->GetTXb(j);
	  tyb[j] = a->GetTYb(j);
	  deltarb[j]  =  a->GetDeltarb(j);
	  deltathetab[j] = a->GetDeltathetab(j);	 
	  nfilmb[j] = a->GetNFilmb(j);
	  tagprimary[j] = 0;

	  if (j==0) tagprimary[j] = 1; 
	  if (j==0) chi2btkb[j] = 1; 
	  if (j>0)   chi2btkb[j] = sigbb[j+i*eNTM];	    
	}
      if (isizeb == 1)treesaveb->Fill();  //cross at least 4 plates	
      i++;
    }

  SaveResults();
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//void EdbShowerRec::recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par,float Rcut=100., float Tcut=0.05)
void EdbShowerRec::recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par)

{    
  
  // shower tree definition    

  treesaveb = new TTree("treebranch","tree of branchtrack");
  treesaveb->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb->Branch("sizeb30",&sizeb30,"sizeb30/I");
    
  treesaveb->Branch("isizeb",&isizeb,"isizeb/I");
  treesaveb->Branch("xb",xb,"xb[sizeb]/F");
  treesaveb->Branch("yb",yb,"yb[sizeb]/F");
  treesaveb->Branch("zb",zb,"zb[sizeb]/F");
  treesaveb->Branch("txb",txb,"txb[sizeb]/F");
  treesaveb->Branch("tyb",tyb,"tyb[sizeb]/F");
  treesaveb->Branch("nfilmb",nfilmb,"nfilmb[sizeb]/I");
  treesaveb->Branch("ntrace1simub",ntrace1simub,"ntrace1simu[sizeb]/I");
  treesaveb->Branch("ntrace2simub",ntrace2simub,"ntrace2simu[sizeb]/I");
  treesaveb->Branch("chi2btkb",chi2btkb,"chi2btkb[sizeb]/F");
  treesaveb->Branch("deltarb",deltarb,"deltarb[sizeb]/F");
  treesaveb->Branch("deltathetab",deltathetab,"deltathetab[sizeb]/F");
  treesaveb->Branch("deltaxb",deltaxb,"deltaxb[sizeb]/F");
  treesaveb->Branch("deltayb",deltayb,"deltayb[sizeb]/F");
  treesaveb->Branch("tagprimary",tagprimary,"tagprimary[sizeb]/F");
    
  fileout = new TFile("shower1.root","RECREATE");
    

  float Z0, SX0,SY0;
  float X0, Y0, Xss,Yss;
  float Delta, rayon,rayon2, diff, delta;
  float Xe[4], Ye[4];
  char fname2[128];
    
  char nme[64];
  float  a11, a12, a21, a22, bb1, bb2;
  float Zoff;
  double ZZ0[50];
  int ind;
  const Int_t eNSM = 10000;
  const Int_t eNTM = 1000;

  TArrayF  xbb(eNTM*eNSM); 
  TArrayF  ybb(eNTM*eNSM);  
  TArrayF  zbb(eNTM*eNSM);  
  TArrayF  txbb(eNTM*eNSM);  
  TArrayF  tybb(eNTM*eNSM); 
  TArrayI  sigbb(eNTM*eNSM);
  TArrayI add(eNTM*eNSM);    
  for(int i=0; i<eNSM; i++)
    {
      for(int j=0; j<eNTM; j++)  
	{
	  ind = i*eNTM+j;
	  xbb[ind];
	  ybb[ind];
	  zbb[ind];
	  txbb[ind];
	  tybb[ind];
	  sigbb[ind];

	}
    }
 
  TList  *eS = new TList;
    
  for (int ii=0;ii<Ncand;ii++) 
    {    
      EdbShowerRec* shower = new EdbShowerRec();
	
      shower->SetID(ii);
      shower->SetX0(x0[ii]);
      shower->SetY0(y0[ii]);
      shower->SetZ0(z0[ii]);
      shower->SetTx(tx0[ii]);
      shower->SetTy(ty0[ii]);
      shower->SetTrID(TRid[ii]);
	
      shower->SetDeltarb(200.,0);
      shower->SetDelthetab(0.5,0);
      shower->SetPrimary(0,0);
      shower->SetNFilmb(1,0);
	
      shower->SetXb(x0[ii],0);
      shower->SetYb(y0[ii],0);
      shower->SetZb(z0[ii],0);
      shower->SetTXb(tx0[ii],0);
      shower->SetTYb(ty0[ii],0);

      ind = 0 +  ii*eNTM;

      xbb[ind] =  x0[ii]; 
      ybb[ind] =  y0[ii]; 
      zbb[ind] =  z0[ii]; 
      txbb[ind] =  tx0[ii]; 
      tybb[ind] =  ty0[ii]; 

      add[ii] = 1; 
      eS->Add(shower);
	
    }
    
  EdbShowerRec* a=0;
  // save the Z position of the first sheet
  if(num<10) {sprintf(nme,"%s/0%d_00%d.par","par",num,piece2par);}
  else {sprintf(nme,"%s/%d_00%d.par","par",num,piece2par);}
  ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );
  //  ZZ0[0]= z0[0];
  ZZ0[0]= Zoff;
  /*    
	for (int i=0;i<eNSM; i++)
	{
	for (int j=0;j<eNTM; j++)
	{
	ind = i*eNTM+j;
	deltarbb[ind]= 200.;
	}
	}
  */    
  int nntr;
  int ok;
  int max=0 ;
  int index = 0;
  double rr;

  float Rcut = 100.;
  float  Tcut = 0.05;

  float mini[5] ={-20000.,-20000.,-1,-1,0};
  float maxi[5] ={20000.,20000.,1,1,100};


  //    for (int d=num+1; d<MAXPLATE+1; d++){ 
  //	if (DOWN==0) break;
  for (int d=num-1; d>num-MAXPLATE; d--){
    index++;
    printf( "Process plate number: %d/%d\n",index+1,MAXPLATE);	
	
    //	if(d<10) {sprintf(nme,"%s/0%d_00%d.par",pathpar2,d,piece2par);}
    //	else {sprintf(nme,"%s/%d_00%d.par",pathpar2,d,piece2par);}
    if(d<10) {sprintf(nme,"%s/0%d_00%d.par","par",d,piece2par);}
    else {sprintf(nme,"%s/%d_00%d.par","par",d,piece2par);}
	
    if(d<10){sprintf(fname2,"%s/0%d_00%d.cp.root","data",d,piece2);}
    else {sprintf(fname2,"%s/%d_00%d.cp.root","data",d,piece2);}
	
    EdbPVRec ali;
    EdbPattern *pat=0;
    EdbPattern *pat2=0;	
    EdbDataPiece Piece;
		
    InitPiece(Piece,fname2,nme);
    pat2 = new EdbPattern(0.,0., Piece.GetLayer(0)->Z(),100 );
    Z0 =  Piece.GetLayer(0)->Z();
    ReadPiece(Piece, *pat2);

    ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );
    Z0 =  Zoff;
    ZZ0[index] =  Zoff;

    int ii =0;
    TIter next(eS);
    while ((a=(EdbShowerRec*)next()))
      {
	if(a->GetZ0()>Z0) ii++;
	if(a->GetZ0()>Z0) continue;
	Xss =  a->GetX0() + fabs(Z0-a->GetZ0())*a->GetTx();
	Yss =  a->GetY0() + fabs(Z0-a->GetZ0())*a->GetTy();

	mini[0] = Xss-450.;
	mini[1] = Yss-450.;
	mini[2] = -0.45;
	mini[3] = -0.45;
	mini[4] = 0.;	    
	maxi[0] = Xss+450.;
	maxi[1] = Yss+450.;
	maxi[2] = 0.45;
	maxi[3] = 0.45;
	maxi[4] = 100.;
	    
	pat = pat2->ExtractSubPattern(mini,maxi);	    
	ali.AddPattern(pat);

	EdbSegP *ss=0;
	    
	int incr=-1;
	
	for(int ip=0; ip<ali.Npatterns(); ip++) {incr++;};
	
	for(int i=0; i<ali.GetPattern(incr)->N(); i++)
	  {	    
	    ss = ali.GetPattern(incr)->GetSegment(i);
	    ok = 0;
	    if (DATA ==1){
	      X0 = a11*ss->X() + a12*ss->Y() + bb1;
	      Y0 = a21*ss->X() + a22*ss->Y() + bb2;

	      SX0 = a11*ss->TX()+ a12*ss->TY() ;
	      SY0 = a21*ss->TX()+a22*ss->TY();
	    }
	    else
	      {
		X0 = ss->X() ;
		Y0 = ss->Y() ;
		SX0 = ss->TX();
		SY0 = ss->TY();
	      }

	    if (fabs(SX0)>0.4) continue;
	    if (fabs(SY0)>0.4) continue;
		
	    if ( a->GetTrID() == ss->MCEvt()|| ss->MCEvt()<0) 
	      {
		Delta = 20.+   fabs(0.02*fabs(Z0-a->GetZ0())); // cone with 20 mrad 
		    
		rayon = 400.; // cylinder with 400 microns radius
		diff = sqrt((X0-Xss)*(X0-Xss)+(Y0-Yss)*(Y0-Yss));	   
		    
		if ( diff<rayon&&diff<Delta )
		  {				
		    if (index==1) max = 1;
		    if (index==2) max = 2;
		    if (index>2) max = 3;
			
		    for (int el=0;el<max;el++)
		      {
			//extrapolation in previous plate
			Xe[el] =  X0 - fabs(Z0-ZZ0[index-el-1])*SX0;
			Ye[el] =  Y0 - fabs(Z0-ZZ0[index-el-1])*SY0;
			nntr = add[ii];
			    
			for (int l2=0;l2<nntr;l2++) 
			  {
			    ind = l2 + ii*eNTM;
			    if (fabs(zbb[ind]-(Z0-(el+1)*1300))<500)
			      {
				rayon2 = sqrt((Xe[el]-xbb[ind])*(Xe[el]-xbb[ind])+((Ye[el]-ybb[ind])*(Ye[el]-ybb[ind])));  
				delta = sqrt((SX0-txbb[ind])*(SX0-txbb[ind])+((SY0-tybb[ind])*(SY0-tybb[ind])));
				if (rayon2<Rcut&&delta<Tcut)
				  {
				    if (ok==0)
				      {
					ind = add[ii] + ii*eNTM;

					xbb[ind] =  X0 ;
					ybb[ind] =  Y0 ;
					zbb[ind] =  Z0 ;
					txbb[ind] =  SX0 ;
					tybb[ind] =  SY0 ;

					a->SetXb(X0,add[ii]);			
					a->SetYb(Y0,add[ii]);
					a->SetZb(Z0,add[ii]);
					a->SetTXb(SX0,add[ii]);
					a->SetTYb(SY0,add[ii]);
					a->SetDeltarb(rayon2,add[ii]);
					a->SetDelthetab(delta,add[ii]);
					//a->SetNFilmb( index+1,add[ii]);
					rr =  (Z0-a->GetZ0())/1300;
					a->SetNFilmb(int(round(rr)+1),add[ii]);

					if (ss->MCEvt()==a->GetTrID()) sigbb[ind] = 1;
					if (ss->MCEvt()<0) sigbb[ind] = 0;
					ok =1;
					add[ii]++;
				      } 					    
				    else if (ok==1&&rayon2<a->GetDeltarb(add[ii]-1))
				      {
					a->SetDeltarb(rayon2,add[ii]-1);
					a->SetDelthetab(delta,add[ii]-1);
				      }					    
				  }
			      }
			  }
		      }
		  }
	      }   
	  }
	ii++;
      }
  }
	
  int  i = 0;
  TIter next(eS);
  while ((a=(EdbShowerRec*)next()))
    {	
      number_eventb = a->GetID();
      sizeb = add[i];
      a->SetSize(add[i]);
      sizeb15 = 0;
      sizeb20 = 0;
      sizeb30 = 0;     
      isizeb = 0;

      for (int j=0;j<add[i]; j++) 
	{ 
	  if (a->GetZb(j)>a->GetZ0()+5000) isizeb = 1; //cross at least 4 plates
	  if (a->GetNFilmb(j)<16)  sizeb15++;
	  if (a->GetNFilmb(j)<21)  sizeb20++;
	  if (a->GetNFilmb(j)<31)  sizeb30++;	   
	     
	}
      a->SetSize15(sizeb15);
      a->SetSize20(sizeb20);
      a->SetSize30(sizeb30);

      for (int j=0;j<add[i]; j++) 
	{    
	  xb[j] = a->GetXb(j);
	  yb[j] = a->GetYb(j);
	  zb[j] = a->GetZb(j);
	  txb[j] = a->GetTXb(j);
	  tyb[j] = a->GetTYb(j);
	  deltarb[j]  =  a->GetDeltarb(j);
	  deltathetab[j] = a->GetDeltathetab(j);	 
	  nfilmb[j] = a->GetNFilmb(j);
	  tagprimary[j] = 0;

	  //	    printf("txb[j]  %f\n", a->GetTXb(j));		    

	  if (j==0) tagprimary[j] = 1; 
	  if (j==0) chi2btkb[j] = 1; 
	  if (j>0)   chi2btkb[j] = sigbb[j+i*eNTM];	    
	}
      if (isizeb == 1)treesaveb->Fill();  //cross at least 4 plates	
      i++;
    }

  SaveResults();
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

//void EdbShowerRec::remove(char *shfname, char *def,float Rcut=100., float Tcut=0.05)
void EdbShowerRec::remove(char *shfname, char *def, int MAXPLATE, int piece2par)
{

  treesaveb = new TTree("treebranch","tree of branchtrack");
  treesaveb->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb->Branch("sizeb30",&sizeb30,"sizeb30/I");    
  treesaveb->Branch("isizeb",&isizeb,"isizeb/I");
  treesaveb->Branch("xb",xb,"xb[sizeb]/F");
  treesaveb->Branch("yb",yb,"yb[sizeb]/F");
  treesaveb->Branch("zb",zb,"zb[sizeb]/F");
  treesaveb->Branch("txb",txb,"txb[sizeb]/F");
  treesaveb->Branch("tyb",tyb,"tyb[sizeb]/F");
  treesaveb->Branch("nfilmb",nfilmb,"nfilmb[sizeb]/I");
  treesaveb->Branch("ntrace1simub",ntrace1simub,"ntrace1simu[sizeb]/I");
  treesaveb->Branch("ntrace2simub",ntrace2simub,"ntrace2simu[sizeb]/I");
  treesaveb->Branch("chi2btkb",chi2btkb,"chi2btkb[sizeb]/F");
  treesaveb->Branch("deltarb",deltarb,"deltarb[sizeb]/F");
  treesaveb->Branch("deltathetab",deltathetab,"deltathetab[sizeb]/F");
  treesaveb->Branch("deltaxb",deltaxb,"deltaxb[sizeb]/F");
  treesaveb->Branch("deltayb",deltayb,"deltayb[sizeb]/F");
  treesaveb->Branch("tagprimary",tagprimary,"tagprimary[sizeb]/F");
  fileout = new TFile("shower.root","RECREATE");

  //    vert(shfname,def,Rcut,Tcut);
  vert(shfname,def,MAXPLATE,piece2par);

  SaveResults();

}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
// void EdbShowerRec::vert(const char *name2,const char *def,float Rcut=100., float Tcut=0.05)
void EdbShowerRec::vert(const char *name2,const char *def, int MAXPLATE, int piece2par)
{
  double ZZ0[50];
  char nme[64];
  int ind;
  float  a11, a12, a21, a22, bb1, bb2;
  float Zoff;
  const Int_t eNSM = 10000;
  const Int_t eNTM = 1000;

  TArrayF  xbb(eNTM*eNSM); 
  TArrayF  ybb(eNTM*eNSM);  
  TArrayF  zbb(eNTM*eNSM);  
  TArrayF  txbb(eNTM*eNSM);  
  TArrayF  tybb(eNTM*eNSM); 
  TArrayF  deltarbb(eNTM*eNSM);
  TArrayF  deltatbb(eNTM*eNSM);
  TArrayI  sigbb(eNTM*eNSM);
  TArrayI  nfilmbb(eNTM*eNSM);
  TArrayI Flag(eNTM*eNSM); 
  TArrayI add(eNTM*eNSM);  
  TArrayI EvNum(eNSM);

  for(int i=0; i<eNSM; i++)
    {
      for(int j=0; j<eNTM; j++)  
	{
	  ind = i*eNTM+j;
	  xbb[ind];
	  ybb[ind];
	  zbb[ind];
	  txbb[ind];
	  tybb[ind];
	  deltarbb[ind];
	  deltatbb[ind];
	  sigbb[ind];
	  nfilmbb[ind];
	  Flag[ind];

	}
    }

  file = new TFile(name2 ,"READ");
  TTree *treebranch_e = (TTree *) file->Get("treebranch");
  treebranch_e->SetBranchAddress("number_eventb", &number_eventb);
  treebranch_e->SetBranchAddress("nfilmb", nfilmb);
  treebranch_e->SetBranchAddress("sizeb", &sizeb);   
  treebranch_e->SetBranchAddress("tagprimary", tagprimary);
  treebranch_e->SetBranchAddress("txb", txb);
  treebranch_e->SetBranchAddress("tyb", tyb);
  treebranch_e->SetBranchAddress("xb", xb);
  treebranch_e->SetBranchAddress("yb",yb);
  treebranch_e->SetBranchAddress("zb",zb);
  treebranch_e->SetBranchAddress("chi2btkb",chi2btkb);
    
  int nentries_e = treebranch_e->GetEntries(); 
  TList  *eS = new TList;

  for (Int_t ii = 0; ii<nentries_e; ii++)
    { 
      treebranch_e->GetEntry(ii);

      EdbShowerRec* shower = new EdbShowerRec();
      shower->SetID(ii);
      shower->SetX0(xb[0]);
      shower->SetY0(yb[0]);
      shower->SetZ0(zb[0]);
      shower->SetTx(txb[0]);
      shower->SetTy(tyb[0]);
      shower->SetTrID(number_eventb);
	
      shower->SetDeltarb(200.,0);
      shower->SetDelthetab(0.5,0);
      shower->SetPrimary(0,0);
      shower->SetNFilmb(1,0);
      shower->SetSize(sizeb);
	
      shower->SetXb(xb[0],0);
      shower->SetYb(yb[0],0);
      shower->SetZb(zb[0],0);
      shower->SetTXb(txb[0],0);
      shower->SetTYb(tyb[0],0); 

      for (Int_t ibtke = 1; ibtke < sizeb; ibtke++)
	{ 
	  shower->SetXb(xb[ibtke],ibtke);
	  shower->SetYb(yb[ibtke],ibtke);
	  shower->SetZb(zb[ibtke],ibtke);
	  shower->SetTXb(txb[ibtke],ibtke);
	  shower->SetTYb(tyb[ibtke],ibtke);
	  shower->SetNFilmb(nfilmb[ibtke],ibtke);
	}
      eS->Add(shower);
    }

  EdbShowerRec* a=0;
    
  //================================================================
  float Z0, Z1, TX0,TY0  ;    
  float X0, Y0, X1, Y1, Xss,Yss;
  float Delta, rayon,rayon2, diff, delta;
  float diff2, Delta2;
  float Xe[4], Ye[4];
  int  nseg;      

  for(int jj=0;jj<eNSM;jj++)
    {
      for(int ii=0;ii<eNTM;ii++)
	{
	  ind = jj*eNTM +ii;
	  Flag[ind] = 0;
	}
    }

  EdbTrackP *tr=0;
  //read the linked_tracks.root tree
  initproc(def, 100,"nseg>3"); 
    
  //printf("nseg>3&&s.eChi2<(s.eW*0.3-3.)\n");
  gAli->FillCell(30,30,0.009,0.009);

  int ntr = gAli->eTracks->GetEntries();
  printf("ntr = %d\n",ntr);

  EdbSegP *s = new EdbSegP();
  for(int i=0; i<ntr; i++) {
    tr = (EdbTrackP*)(gAli->eTracks->At(i));
    if (i%1000==0)	printf( "Processed tracks %d/%d\n",i,ntr);	
    nseg = tr->N();
    if (nseg<4) continue;  //do not take into account tracks with less than 4 segments

    s = tr->GetSegment(0);
    X1 = s->X();
    Y1 = s->Y();
    Z1 = s->Z();
    for(int l=0; l<nseg; l++) 	 
      {    
	s =	tr->GetSegment(l);
	Z0 = s->Z();
	TX0 =  s->TX();
	TY0 = s->TY();
	X0 =  s->X();
	Y0 =  s->Y();	
	// loop over the shower
	int  ii=0;
	TIter next(eS);
	while ((a=(EdbShowerRec*)next()))
	  {
	    for (Int_t ibtke = 1; ibtke < a->GetSize(); ibtke++)
	      { 
		if ( a->GetXb(ibtke)==X0&&a->GetYb(ibtke)==Y0&&a->GetZb(ibtke)==Z0 ) 
		  {
		    rayon = 400.; // cylinder with 400 microns radius
			
		    // Extrapolation of the first shower basetrack at the Z basetrack position of the first basetrack of the track
		    Xss =  a->GetX0() + (Z1-a->GetZ0())*a->GetTx();
		    Yss =  a->GetY0() + (Z1-a->GetZ0())*a->GetTy();
			
		    // check if the first  basetrack of the track is inside the cone
		    diff2 = sqrt((X1-Xss)*(X1-Xss)+(Y1-Yss)*(Y1-Yss));	   
		    Delta2 = 20. +   fabs(0.02*(Z1-a->GetZ0())); // cone with 20 mrad         
			
		    // Extrapolation of the first shower basetrack at the current Z basetrack position
		    Xss =  a->GetX0() + (Z0-a->GetZ0())*a->GetTx();
		    Yss =  a->GetY0() + (Z0-a->GetZ0())*a->GetTy();
		    // check if the current basetrack is inside the cone
		    diff = sqrt((X0-Xss)*(X0-Xss)+(Y0-Yss)*(Y0-Yss));	   
		    Delta = 20. +   fabs(0.02*(Z0-a->GetZ0())); // cone with 20 mrad 
			
		    if ( (diff<rayon&&diff<Delta)&&(diff2>Delta2||diff2>rayon) )  // (the BT must be in the cone)&&(the first BT of the track must be outside the cone)
		      {
			printf( "%f %f %f %f %d %f\n", Z1, Delta2,diff2,Z0,ii,chi2btkb[ibtke]);
			Flag[ibtke+ii*eNTM] = 1;			    
		      }
		  }
	      }
	    ii++;
	  }	    
      }	
  }    
  file->Close();
  gAli->Delete();

  // get Z value for each plate
  ZZ0[0] = 0.;
  for (Int_t iii = 1; iii<MAXPLATE; iii++)
    { 	if(iii<10) {sprintf(nme,"%s/0%d_00%d.par","par",iii,piece2par);}
    else {sprintf(nme,"%s/%d_00%d.par","par",iii,piece2par);}

    ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2);
    ZZ0[iii] = Zoff;

    }

  float zpos,rr;
  int nntr, d, max =0;
  int ok;
  float Rcut = 100.;
  float  Tcut = 0.05;
  //    float Rcut = 150.;
  //    float  Tcut = 0.15;
   
  int ige =0; 

  TIter next(eS);
  while ((a=(EdbShowerRec*)next()))
    {
      ind = 0 + ige*eNTM;
      add[ige] = 1;
      EvNum[ige] = a->GetTrID();
      xbb[ind] =  a->GetX0();
      ybb[ind] =  a->GetY0(); 
      zbb[ind] =  a->GetZ0(); 
      txbb[ind] = a->GetTx(); 
      tybb[ind] = a->GetTy();
      deltarbb[ind]= 200. ;
      deltatbb[ind]= 0.5;
	
      nfilmbb[ind] = 1;
      for ( int ibtke = 1; ibtke < a->GetSize() ; ibtke++)
	{ 	
	  ok = 0;
	  Xss =  a->GetX0() + (a->GetZb(ibtke)-a->GetZ0())*a->GetTx();
	  Yss =  a->GetY0() + (a->GetZb(ibtke)-a->GetZ0())*a->GetTy();
	    
	  Delta = 20. + fabs(0.02*(a->GetZb(ibtke)-a->GetZ0())); // cone with 20	  
	  rayon = 400.; // cylinder with 400 microns radius
	  diff = sqrt((a->GetXb(ibtke)-Xss)*(a->GetXb(ibtke)-Xss)+(a->GetYb(ibtke)-Yss)*(a->GetYb(ibtke)-Yss));
	    
	  if ( diff<rayon&&diff<Delta&&Flag[ibtke+ige*eNTM]==0 )  //BT must be in the cone and does not belong to a track which starts outside the cone volume
	    {

	      d = a->GetNFilmb(ibtke);
	      zpos = a->GetZb(ibtke);
	      if (zpos>1000&&zpos<1500.)
		{
		  max = 1;
		} else if (zpos<3000)
		  { 
		    max = 2;
		  }
	      else
		{ 
		  max = 3;
		}
	      for (int el=0;el<max;el++)
		{

		  rr =  a->GetZb(ibtke)/1300;
		  d = int(round(rr));

		  Xe[el] =  a->GetXb(ibtke) -(a->GetZb(ibtke)-ZZ0[d-el])*a->GetTXb(ibtke);
		  Ye[el] =  a->GetYb(ibtke) -(a->GetZb(ibtke)-ZZ0[d-el])*a->GetTYb(ibtke);
		  nntr = add[ige];
		   
		  for (int l2=0;l2<nntr;l2++) {			

		    if (fabs(a->GetZb(l2)-(a->GetZb(ibtke)-(el+1)*1300))<500)  
		      {

			rayon2 = sqrt((Xe[el]-a->GetXb(l2))*(Xe[el]-a->GetXb(l2))+((Ye[el]-a->GetYb(l2))*(Ye[el]-a->GetYb(l2))));  
			delta = sqrt((a->GetTXb(ibtke)-a->GetTXb(l2))*(a->GetTXb(ibtke)-a->GetTXb(l2))+((a->GetTYb(ibtke)-a->GetTYb(l2))*(a->GetTYb(ibtke)-a->GetTYb(l2))));

			  
			if (rayon2<Rcut&&delta<Tcut&&Flag[ige+l2*eNSM]==0)  // connection criteria with  a BT which does not belong to a track which starts outside the cone volume
			  {				
			    if (ok==0)
			      {
				//ind = ige + add[ige]*eNSM;
				ind = add[ige] + ige*eNTM;
				xbb[ind] =   a->GetXb(ibtke);
				ybb[ind] =   a->GetYb(ibtke);
				zbb[ind] =   a->GetZb(ibtke);
				txbb[ind] =  a->GetTXb(ibtke);
				tybb[ind] =  a->GetTYb(ibtke);
				deltarbb[ind] =  rayon2 ;
				deltatbb[ind] =  delta ;
				EvNum[ige]= a->GetTrID();
				    
				nfilmbb[ind] =  a->GetNFilmb(ibtke);		 
				    
				sigbb[ind] = (int)chi2btkb[ibtke];   //bug here?
				ok =1;
				add[ige]++;   
				    
			      }
			    else if (ok==1&&rayon2<deltarbb[(add[ige]-1)+ige*eNTM]) 
			      {
				ind = (add[ige]-1) + ige*eNTM;
				deltarbb[ind] =  rayon2 ;
				deltatbb[ind] =  delta ;  
			      }			      
			  }	          
		      }
		  } //for (int l2=0;l2<nntr;l2++) 
		} // for (int el=0;el<max;el++)
	    }//if ( diff<rayon&&diff<Delta )
	}   // for ( ibtke = 1;
      ige++;
    } // while
    
  for (int i=0;i<nentries_e; i++) 
    {
      //	number_eventb = i;
      number_eventb = EvNum[i];

      sizeb = add[i];
      sizeb15 = 0;
      sizeb20 = 0;
      sizeb30 = 0;
	
      isizeb = 0;
	
      for (int j=0;j<sizeb; j++) 
	{ 
	  ind = j + i*eNTM;
	  if (zbb[ind]>zb[0]+5000) isizeb = 1;
	  if (nfilmbb[ind]<16)  sizeb15++;
	  if (nfilmbb[ind]<21)  sizeb20++;
	  if (nfilmbb[ind]<31)  sizeb30++;	  
	  //	}      
	
	  //	for (int j=0;j<sizeb; j++) 
	  //	{    
	  xb[j] = xbb[ind];
	  yb[j] = ybb[ind];
	  zb[j] = zbb[ind];
	  txb[j] = txbb[ind];
	  tyb[j] = tybb[ind];
	  deltarb[j]  = deltarbb[ind] ;
	  deltathetab[j] = deltatbb[ind];
	    
	  nfilmb[j] = nfilmbb[ind];
	  tagprimary[j] = 0;
	  if (j==0) tagprimary[j] = 1;  
	  if (j==0) chi2btkb[j] = 1; 
	  if (j>0)  chi2btkb[j] = sigbb[ind];
	}       
      if (isizeb == 1)treesaveb->Fill();
    }
 
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
    
void EdbShowerRec::ReadAffAndZ(char *fname, Float_t *pZoffs,  Float_t *a11, Float_t *a12,  Float_t *a21, Float_t *a22, Float_t *b1, Float_t *b2)
{
  char            buf[256];
  char            key[256];
  Int_t id;
  Float_t fTranslateX,fTranslateY;

  FILE *fp=fopen(fname,"r");
  if (fp==NULL)   {
    printf("ERROR open file: %s n", fname);
    return;
  }else
    // printf( "Read affine transformation from file: %s\n", fname );
    while( (fgets(buf,256,fp)!=NULL) ) {

      for( int i=0; i<(int)strlen(buf); i++ ) 
	if( buf[i]=='#' )  {
	  buf[i]='0';                       // cut out comments starting from #
	  break;
	}
    
      if( sscanf(buf,"%s",key)!=1 ) continue;
      if      ( !strcmp(key,"INCLUDE")   )
	{
	  //        sscanf(buf+strlen(key),"%s",name);
	  //ReadAffAndZ(name,pZoffs);
	}
      else if ( !strcmp(key,"AFFXY")  )
	{
	  sscanf(buf+strlen(key),"%d %f %f %f %f %f %f",&id,a11,a12,a21,a22,b1,b2);
	}
      else if ( !strcmp(key,"ZLAYER")   )
	{
	  sscanf(buf+strlen(key),"%d %f",&id,pZoffs);
	}
      else if ( !strcmp(key,"VOLUME0")   )
	{
	  sscanf(buf+strlen(key),"%f %f",&fTranslateX,&fTranslateY);
	}
    }
  fclose(fp);	
}
//-------------------------------------------------------------------
int EdbShowerRec::ReadPiece( EdbDataPiece &piece, EdbPattern &pat )
{
  //assuming that piece was initialised correctly

  if(!piece.InitCouplesTree("READ")) return 0;
  piece.GetCPData_new( &pat,0,0,0 );
  pat.SetSegmentsZ();
  pat.Transform(    piece.GetLayer(0)->GetAffineXY()   );
  pat.TransformA(   piece.GetLayer(0)->GetAffineTXTY() );
  pat.TransformShr( piece.GetLayer(0)->Shr()  );
  return 1;
}

//-------------------------------------------------------------------
int EdbShowerRec::FindPredictions(EdbPattern &pred, const char *cpfile, const char *parfile, 
				  EdbPattern &found)
{
  // find predictions pred in couples tree of id and prepare pattern "found"
  // assumed that pred are transformed and projected into the coord system of id
  // Input:   pred          - pattern with predictions
  //         cpfile,parfile - the data piece to be processed
  // Output: found - pattern with found tracks
  //         found.txt summary file with all candidats

  EdbPVRec ali;
  EdbPattern *pat=0;
  EdbDataPiece piece;

  // predicted:
  pat = new EdbPattern(pred.X(),pred.Y(),pred.Z(),100);
  for(int i=0; i<pred.N(); i++) pat->AddSegment(*(pred.GetSegment(i)));
  pat->SetPID(0);
  ali.AddPattern(pat);

  // scanned:
  InitPiece(piece, cpfile, parfile);
  pat = new EdbPattern(0.,0., piece.GetLayer(0)->Z(),100 );
  ReadPiece(piece, *pat);
  pat->SetPID(1);
  ali.AddPattern(pat);

  EdbScanCond *cond = piece.GetCond(0);
  cond->SetChi2Mode(3);
  ali.SetScanCond( cond );
  ali.SetPatternsID();
  //ali.SetSegmentsErrors();
  ali.SetCouplesAll();
  ali.SetChi2Max(cond->Chi2PMax());

  EdbSegP *s=0;
  for(int ip=0; ip<ali.Npatterns(); ip++)
    for(int i=0; i<ali.GetPattern(ip)->N(); i++) {
      s = ali.GetPattern(ip)->GetSegment(i);
      s->SetErrors();
      cond->FillErrorsCov( s->TX(), s->TY(), s->COV() );
    }

  FILE *f = fopen("found.txt","w");

  int maxcand=100;
  Float_t *chiarr = new Float_t[maxcand];
  Int_t   *ind    = new Int_t[maxcand];
  Int_t   *count  = new Int_t[maxcand];
  Int_t   *cnsel  = new Int_t[maxcand];
  memset(chiarr,'\0',maxcand*sizeof(Float_t));
  memset(ind,'\0',maxcand*sizeof(Int_t));
  memset(count,'\0',maxcand*sizeof(Int_t));  //preselected
  memset(cnsel,'\0',maxcand*sizeof(Int_t));  //selected

  pat = ali.GetPattern(1);
  pat->FillCell(20,20,0.01,0.01);
  int nseg=0;
  TObjArray arr;
  EdbSegP *s2=0;
  EdbSegP s3;
  for(int i=0; i<pred.N(); i++) {
    s = ali.GetPattern(0)->GetSegment(i);
    arr.Clear();
    nseg = pat->FindCompliments(*s,arr,cond->BinX(),cond->BinTX());
    if(nseg>maxcand)       continue;
    count[nseg]++;
    int nsel=0;
    if(nseg>=0) {
      for(int j=0; j<nseg; j++) {
	s2 = (EdbSegP *)arr.At(j);
	s3.Copy(*s2);
	chiarr[j] = EdbTrackFitter::Chi2Seg(&s3, s);
      }
      TMath::Sort(nseg,chiarr,ind,0);
      fprintf(f,"\n%8.8d %11.2f %11.2f %8.4f %8.4f\n",
	      s->ID(),s->X(),s->Y(),s->TX(),s->TY());
      for(int j=0; j<nseg; j++) {
	s2 = (EdbSegP *)arr.At(ind[j]);
	if(chiarr[ind[j]] > cond->Chi2PMax() ) break;
	nsel=j+1;
	s2->SetMC(s->MCEvt(),s->MCTrack());

	fprintf(f,"%8d %11.2f %11.2f %8.4f %8.4f %6.2f %3.0f\n",
		j+1,s2->X(),s2->Y(),s2->TX(),s2->TY(),chiarr[ind[j]],s2->W());

      }
    }
    cnsel[nsel]++;
    if(nsel>0) found.AddSegment(*((EdbSegP *)arr.At(ind[0])));
    else       found.AddSegment(*(s));
    found.GetSegmentLast()->SetID(s->ID());         // todo!
    found.GetSegmentLast()->SetFlag(nsel);         // todo!

    printf("pred: %5d found: %3d   x,y,tx,ty = %7.0f %7.0f %7.3f %7.3f\n",
	   s->ID(),nsel, s->X(),s->Y(),s->TX(),s->TY() );
  }

  fclose(f);

  printf("Total: %d predictions, %d basetracks in scanned pattern\n",pred.N(), pat->N() );
  int sum=0;
  for(int i=0; i<maxcand; i++) 
    if(count[i]>0) {
      printf("count(%5d)= %5d\n",i, count[i] );
      sum+=count[i];
    }
  printf("sum = %d\n",sum );
  sum=0;
  for(int i=0; i<maxcand; i++) 
    if(cnsel[i]>0) {
      printf("cnsel(%5d)= %5d\n",i, cnsel[i] );
      sum+=cnsel[i];
    }
  printf("sum = %d\n",sum );
  delete [] chiarr;
  delete [] ind;
  delete [] count;
  delete [] cnsel;

  return sum-cnsel[0];
}
