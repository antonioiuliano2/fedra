
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbShowerRec                                                         //
//                                                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbShowerRec.h"


ClassImp(EdbShowerRec)
//----------------------------------------------------------------
    EdbShowerRec::EdbShowerRec()
{
  eShowerTreeIsDone=kFALSE;
  eDoEnergyCalculation=kFALSE;
  eEnergyIsDone=kFALSE;   
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
void EdbShowerRec::rec(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par,int DOWN)
{
    //if (DOWN == 1) {recdown(num,MAXPLATE,DATA,Ncand,x0,y0,z0,tx0,ty0,TRid,piece2, piece2par,Rcut,Tcut);}
  if (DOWN == 1) {recdown(num,MAXPLATE,DATA,Ncand,x0,y0,z0,tx0,ty0,plate0,id0,TRid,Esim,piece2, piece2par);}
  if (DOWN == 0) {recup(num,MAXPLATE,DATA,Ncand,x0,y0,z0,tx0,ty0,plate0,id0,TRid,Esim,piece2, piece2par);}
}
		    

//-------------------------------------------------------------------
//-------------------------------------------------------------------
//void EdbShowerRec::recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par, float Rcut=100., float Tcut=0.05)
 void EdbShowerRec::recdown(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim,int piece2, int piece2par)
{    
  
// shower tree definition    

  treesaveb = new TTree("treebranch","tree of branchtrack");
  treesaveb->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb->Branch("sizeb30",&sizeb30,"sizeb30/I");
  treesaveb->Branch("E_MC",&E_MC,"E_MC/F");
    
  treesaveb->Branch("isizeb",&isizeb,"isizeb/I");
  treesaveb->Branch("showerID",&showerID,"showerID/I");
  treesaveb->Branch("idb",idb,"idb[sizeb]/I");
  treesaveb->Branch("plateb",plateb,"plateb[sizeb]/I");
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
  double ZZ0[60];
  int ind;
  const Int_t eNSM = 10000;
  const Int_t eNTM = 1000;

  TArrayF  xbb(eNTM*eNSM);  
  TArrayF  ybb(eNTM*eNSM);  
  TArrayF  zbb(eNTM*eNSM);  
  TArrayF  txbb(eNTM*eNSM);  
  TArrayF  tybb(eNTM*eNSM); 
  TArrayF  chi2bb(eNTM*eNSM);
  TArrayI  sigbb(eNTM*eNSM);
  TArrayI  wbb(eNTM*eNSM); 
  TArrayI  add(eNTM*eNSM);  

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
      chi2bb[ind];
      wbb[ind];
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
    shower->SetIDb(id0[ii],0);
    shower->SetPlateb(plate0[ii],0);	
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
  if(num<10)
  {
    if(piece2par<10)
    {sprintf(nme,"%s/0%d_00%d.par","par",num,piece2par);}
    else
    { sprintf(nme,"%s/0%d_0%d.par","par",num,piece2par);}
  }
  else 
  {
    if(piece2par<10)
    { sprintf(nme,"%s/%d_00%d.par","par",num,piece2par);}
    else
    { sprintf(nme,"%s/%d_0%d.par","par",num,piece2par);}
  }
  ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );
    //  ZZ0[0]= z0[0];
  ZZ0[0]= Zoff;

  int nntr;
  int ok;
  int max=0 ;
  int index = 0;
  double rr;

//    float Rcut = 180.;
//    float  Tcut = 0.18;
  float Rcut = 150.;
  float  Tcut = 0.15;

  float mini[5] ={-20000.,-20000.,-1,-1,0};
  float maxi[5] ={20000.,20000.,1,1,100};

  for (int d=num+1; d<MAXPLATE+1; d++){ 
    index++;
    printf( "Process plate number: %d/%d\n",index+1,MAXPLATE);	
	
    if(d<10) 
    {
      if(piece2par<10)
      {
        sprintf(nme,"%s/0%d_00%d.par","par",d,piece2par);
        sprintf(fname2,"%s/0%d_00%d.cp.root","data",d,piece2);
      }
      else
      {
        sprintf(nme,"%s/0%d_0%d.par","par",d,piece2par);
        sprintf(fname2,"%s/0%d_0%d.cp.root","data",d,piece2);
      }
    }
    else 
    {
      if(piece2par<10)
      {	 
        sprintf(nme,"%s/%d_00%d.par","par",d,piece2par);
        sprintf(fname2,"%s/%d_00%d.cp.root","data",d,piece2);
      }
      else
      {
        sprintf(nme,"%s/%d_0%d.par","par",d,piece2par);
        sprintf(fname2,"%s/%d_0%d.cp.root","data",d,piece2);		
      }
    }
	
	//	if(d<10){sprintf(fname2,"%s/0%d_00%d.cp.root","data",d,piece2);}
	//else {sprintf(fname2,"%s/%d_00%d.cp.root","data",d,piece2);}
	
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

			
      mini[0] = Xss-850.;
      mini[1] = Yss-850.;
      mini[2] = -0.5;
      mini[3] = -0.5;
      mini[4] = 0.;	    
      maxi[0] = Xss+850.;
      maxi[1] = Yss+850.;
      maxi[2] = 0.5;
      maxi[3] = 0.5;
      maxi[4] = 100;
	    
      pat = pat2->ExtractSubPattern(mini,maxi);
      ali.AddPattern(pat);
			//ali.Print();

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
          rayon = 800.; // cylinder with 800 microns radius
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
                      a->SetIDb(ss->ID(),add[ii]);
                      a->SetPlateb(d,add[ii]);
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
                      chi2bb[ind] = ss->Chi2();
                      wbb[ind] = int(ss->W());
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
  int evtid = 0;
  TIter next(eS);
  while ((a=(EdbShowerRec*)next()))
  {	
    number_eventb = a->GetTrID();
    sizeb = add[i];
    a->SetSize(add[i]);
    sizeb15 = 0;
    sizeb20 = 0;
    sizeb30 = 0;     
    isizeb = 0;
    E_MC = Esim[i];

    for (int j=0;j<add[i]; j++) 
    { 
      if ((a->GetZb(j)>a->GetZ0()+5000)&&isizeb==0) isizeb = 1; //cross at least 4 plates	  
      if (a->GetNFilmb(j)<16)  sizeb15++;
      if (a->GetNFilmb(j)<21)  sizeb20++;
      if (a->GetNFilmb(j)<31)  sizeb30++;	   
      if (a->GetNFilmb(j)==15)  isizeb=15;
      if (a->GetNFilmb(j)==20)  isizeb=20;
      if (a->GetNFilmb(j)==30)  isizeb=30;
      if (a->GetNFilmb(j)==50)  isizeb=50;	     
    }
    a->SetSize15(sizeb15);
    a->SetSize20(sizeb20);
    a->SetSize30(sizeb30);

    for (int j=0;j<add[i]; j++) 
    { 
      idb[j] = a->GetIDb(j);
      plateb[j] = a->GetPlateb(j);
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
      if (j==0) chi2btkb[j] = 0; 
      if (j>0)   chi2btkb[j] = chi2bb[j+i*eNTM];	    
      if (j==0) ntrace1simub[j]= 1; 
      if (j>0)  ntrace1simub[j] = sigbb[j+i*eNTM];	    
      if (j==0) ntrace2simub[j]= 32; 
      if (j>0)  ntrace2simub[j] = wbb[j+i*eNTM];
    }
    if (isizeb > 0)//cross at least 4 plates
    {
      showerID = evtid;   
      treesaveb->Fill(); 
      evtid++;   
    }
    i++;
  }
	
  SaveResults();
  NeuralNet();
  CalculateEnergyValues();
	
  strcpy(cmd,"rm  shower1.root");
  gSystem->Exec(cmd);
  strcpy(cmd,"rm  Shower2.root");
  gSystem->Exec(cmd);

  return;
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//void EdbShowerRec::recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *TRid, int piece2, int piece2par,float Rcut=100., float Tcut=0.05)
void EdbShowerRec::recup(int num,int MAXPLATE,  int DATA, int Ncand, double *x0, double *y0, double *z0, double *tx0, double *ty0, int *plate0, int *id0, int *TRid, double *Esim, int piece2, int piece2par)
{    
  
// shower tree definition    

  treesaveb = new TTree("treebranch","tree of branchtrack");
  treesaveb->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb->Branch("sizeb30",&sizeb30,"sizeb30/I");
  treesaveb->Branch("E_MC",&E_MC,"E_MC/F");
  treesaveb->Branch("idb",idb,"idb[sizeb]/I");
  treesaveb->Branch("plateb",plateb,"plateb[sizeb]/I");    
  treesaveb->Branch("showerID",&showerID,"showerID/I");
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
  double ZZ0[57];
  int ind;
  const Int_t eNSM = 10000;
  const Int_t eNTM = 1000;

  TArrayF  xbb(eNTM*eNSM); 
  TArrayF  ybb(eNTM*eNSM);  
  TArrayF  zbb(eNTM*eNSM);  
  TArrayF  txbb(eNTM*eNSM);  
  TArrayF  tybb(eNTM*eNSM); 
  TArrayI  sigbb(eNTM*eNSM);
  TArrayF  chi2bb(eNTM*eNSM);
  TArrayI  wbb(eNTM*eNSM); 
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
      wbb[ind];
      chi2bb[ind];
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
    shower->SetIDb(id0[ii],0);
    shower->SetPlateb(plate0[ii],0);	
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
  if(num<10)
  {
    if(piece2par<10)
    {sprintf(nme,"%s/0%d_00%d.par","par",num,piece2par);}
    else
    { sprintf(nme,"%s/0%d_0%d.par","par",num,piece2par);}
  }
  else 
  {
    if(piece2par<10)
    { sprintf(nme,"%s/%d_00%d.par","par",num,piece2par);}
    else
    { sprintf(nme,"%s/%d_0%d.par","par",num,piece2par);}
  }

  ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );
    //  ZZ0[0]= z0[0];
  ZZ0[0]= Zoff;

  int nntr;
  int ok;
  int max=0 ;
  int index = 0;
  double rr;

  float Rcut = 150.;
  float  Tcut = 0.15;

  float mini[5] ={-20000.,-20000.,-1,-1,0};
  float maxi[5] ={20000.,20000.,1,1,100};

//    for (int d=num+1; d<MAXPLATE+1; d++){ 
//	if (DOWN==0) break;
  for (int d=num-1; d>num-MAXPLATE; d--){
    index++;
    printf( "Process plate number: %d/%d\n",index+1,MAXPLATE);	
	
	//if(d<10) {sprintf(nme,"%s/0%d_00%d.par","par",d,piece2par);}
	//else {sprintf(nme,"%s/%d_00%d.par","par",d,piece2par);}
	
    if(d<10){sprintf(fname2,"%s/0%d_00%d.cp.root","data",d,piece2);}
    else {sprintf(fname2,"%s/%d_00%d.cp.root","data",d,piece2);}
	
    if(d<10) 
    {
      if(piece2par<10)
      {
        sprintf(nme,"%s/0%d_00%d.par","par",d,piece2par);
        sprintf(fname2,"%s/0%d_00%d.cp.root","data",d,piece2);
      }
      else
      {
        sprintf(nme,"%s/0%d_0%d.par","par",d,piece2par);
        sprintf(fname2,"%s/0%d_0%d.cp.root","data",d,piece2);
      }
    }
    else 
    {
      if(piece2par<10)
      {	 
        sprintf(nme,"%s/%d_00%d.par","par",d,piece2par);
        sprintf(fname2,"%s/%d_00%d.cp.root","data",d,piece2);
      }
      else
      {
        sprintf(nme,"%s/%d_0%d.par","par",d,piece2par);
        sprintf(fname2,"%s/%d_0%d.cp.root","data",d,piece2);		
      }
    }
	
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

      mini[0] = Xss-850.;
      mini[1] = Yss-850.;
      mini[2] = -0.5;
      mini[3] = -0.5;
      mini[4] = 0.;	    
      maxi[0] = Xss+850.;
      maxi[1] = Yss+850.;
      maxi[2] = 0.5;
      maxi[3] = 0.5;
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
          rayon = 800.; // cylinder with 800 microns radius
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

                      a->SetIDb(ss->ID(),add[ii]);
                      a->SetPlateb(d,add[ii]);
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
                      chi2bb[ind] = ss->Chi2();
                      wbb[ind] = int(ss->W());
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
  int evtid = 0;
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
      if ((a->GetZb(j)>a->GetZ0()+5000)&&isizeb==0) isizeb = 1; //cross at least 4 plates
      if (a->GetNFilmb(j)<16)  sizeb15++;
      if (a->GetNFilmb(j)<21)  sizeb20++;
      if (a->GetNFilmb(j)<31)  sizeb30++;	   
      if (a->GetNFilmb(j)==15)  isizeb=15;
      if (a->GetNFilmb(j)==20)  isizeb=20;
      if (a->GetNFilmb(j)==30)  isizeb=30;
      if (a->GetNFilmb(j)==50)  isizeb=50;	     	     
    }
    a->SetSize15(sizeb15);
    a->SetSize20(sizeb20);
    a->SetSize30(sizeb30);

    for (int j=0;j<add[i]; j++) 
    { 
      idb[j] = a->GetIDb(j);
      plateb[j] = a->GetPlateb(j);
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
      if (j==0) chi2btkb[j] = 0; 
      if (j>0)   chi2btkb[j] = chi2bb[j+i*eNTM];	
      if (j==0) ntrace1simub[j]= 1; 
      if (j>0)  ntrace1simub[j] = sigbb[j+i*eNTM];    
      if (j==0) ntrace2simub[j]= 32; 
      if (j>0)  ntrace2simub[j] = wbb[j+i*eNTM];
    }
    if (isizeb >0) //cross at least 4 plates
    {
	  // number_eventb = evtid;   
      showerID = evtid;   
      treesaveb->Fill(); 
      evtid++;  
    }
    i++;
  }

  SaveResults();
  NeuralNet();
  strcpy(cmd,"rm  shower1.root");
  gSystem->Exec(cmd);
  strcpy(cmd,"rm  Shower2.root");
  gSystem->Exec(cmd);
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
  treesaveb->Branch("E_MC",&E_MC,"E_MC/F");
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
  double ZZ0[57];
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
    
  int nentries_e = int(treebranch_e->GetEntries()); 
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
            rayon = 800.; // cylinder with 400 microns radius
			
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
  {
    if(iii<10)
    {
      if(piece2par<10)
      {sprintf(nme,"%s/0%d_00%d.par","par",iii,piece2par);}
      else
      { sprintf(nme,"%s/0%d_0%d.par","par",iii,piece2par);}
    }
    else 
    {
      if(piece2par<10)
      { sprintf(nme,"%s/%d_00%d.par","par",iii,piece2par);}
      else
      { sprintf(nme,"%s/%d_0%d.par","par",iii,piece2par);}
    }
    // 	if(iii<10) {sprintf(nme,"%s/0%d_00%d.par","par",iii,piece2par);}
    //	else {sprintf(nme,"%s/%d_00%d.par","par",iii,piece2par);}

    ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2);
    ZZ0[iii] = Zoff;

  }

  float zpos,rr;
  int nntr, d, max =0;
  int ok;
//    float Rcut = 100.;
//    float  Tcut = 0.05;
  float Rcut = 150.;
  float  Tcut = 0.15;
   
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
      rayon = 800.; // cylinder with 400 microns radius
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
				    
                  sigbb[ind] = int(chi2btkb[ibtke]);
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
//	    if (j==0) chi2btkb[j] = 1; 
//	    if (j>0)  chi2btkb[j] = sigbb[ind];
      if (j==0) ntrace1simub[j]= 1; 
      if (j>0)  ntrace1simub[j] = sigbb[j+i*eNTM];
    }       
    if (isizeb > 0)treesaveb->Fill();
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
    while( fgets(buf,256,fp)!=NULL ) {

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

void EdbShowerRec::NeuralNet() {

  float offset;  // Z position of last (first) plate in the data set (usually = 0.)
  offset = 0.; 

  //definition of treebranch
  treesaveb3 = new TTree("treebranch","tree of branchtrack");
  treesaveb3->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb3->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb3->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb3->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb3->Branch("sizeb30",&sizeb30,"sizeb30/I");
  treesaveb3->Branch("output15",&output15,"output15/F");
  treesaveb3->Branch("output20",&output20,"output20/F");
  treesaveb3->Branch("output30",&output30,"outpu30/F");
  treesaveb3->Branch("output50",&output50,"outpu50/F");
  treesaveb3->Branch("eProb90",&eProb90,"eProb90/I");
  treesaveb3->Branch("eProb1",&eProb1,"eProb1/I");
  treesaveb3->Branch("E_MC",&E_MC,"E_MC/F");
  treesaveb3->Branch("idb",idb,"idb[sizeb]/I");
  treesaveb3->Branch("plateb",plateb,"plateb[sizeb]/I");
  treesaveb3->Branch("showerID",&showerID,"showerID/I");
  treesaveb3->Branch("isizeb",&isizeb,"isizeb/I");
  treesaveb3->Branch("xb",xb,"xb[sizeb]/F");
  treesaveb3->Branch("yb",yb,"yb[sizeb]/F");
  treesaveb3->Branch("zb",zb,"zb[sizeb]/F");
  treesaveb3->Branch("txb",txb,"txb[sizeb]/F");
  treesaveb3->Branch("tyb",tyb,"tyb[sizeb]/F");
  treesaveb3->Branch("nfilmb",nfilmb,"nfilmb[sizeb]/I");
  treesaveb3->Branch("ntrace1simub",ntrace1simub,"ntrace1simu[sizeb]/I");
  treesaveb3->Branch("ntrace2simub",ntrace2simub,"ntrace2simu[sizeb]/I");
  treesaveb3->Branch("chi2btkb",chi2btkb,"chi2btkb[sizeb]/F");
  treesaveb3->Branch("deltarb",deltarb,"deltarb[sizeb]/F");
  treesaveb3->Branch("deltathetab",deltathetab,"deltathetab[sizeb]/F");
  treesaveb3->Branch("deltaxb",deltaxb,"deltaxb[sizeb]/F");
  treesaveb3->Branch("deltayb",deltayb,"deltayb[sizeb]/F");
  treesaveb3->Branch("tagprimary",tagprimary,"tagprimary[sizeb]/F");
 
  treesaveb3->Branch("Energy",&EnergyCorrectedb,"EnergyCorrectedb/F");
  treesaveb3->Branch("EnergyUnCorrected",&EnergyUnCorrectedb,"EnergyUnCorrectedb/F");
  treesaveb3->Branch("EnergySigma",&EnergySigmaCorrectedb,"EnergySigmaCorrectedb/F");
  treesaveb3->Branch("EnergySigmaUnCorrected",&EnergySigmaUnCorrectedb,"EnergySigmaUnCorrectedb/F");

 //ouput file
  fileout3 = new TFile("Shower2.root","RECREATE"); 

  char fname_e[128];
  Float_t Rcut30, Tcut30;
  Float_t Rcut20, Tcut20;
  Float_t Rcut15, Tcut15;
  Float_t Rcut50, Tcut50;

  int flag15;
  int flag20;
  int flag30;
  int flag50;

  Double_t params[53];

  Float_t ntrke50;
  Float_t ntrke30;
  Float_t ntrke20;
  Float_t ntrke15;

  TTree *varia = new TTree("MonteCarlo", "Filtered Monte Carlo Events");
  varia->Branch("bin3", &bin3, "bin3/F");
  varia->Branch("bin4", &bin4, "bin4/F");
  varia->Branch("bin5", &bin5, "bin5/F");
  varia->Branch("bin6", &bin6, "bin6/F");
  varia->Branch("bin7", &bin7, "bin7/F");
  varia->Branch("bin8", &bin8, "bin8/F");
  varia->Branch("bin9", &bin9, "bin9/F");
  varia->Branch("bin10", &bin10, "bin10/F");
  varia->Branch("bin11", &bin11, "bin11/F");
  varia->Branch("bin12", &bin12, "bin12/F");
  varia->Branch("bin13", &bin13, "bin13/F");
  varia->Branch("bin14", &bin14, "bin14/F");
  varia->Branch("bin15", &bin15, "bin15/F");
  varia->Branch("bin16", &bin16, "bin16/F");
  varia->Branch("bin17", &bin17, "bin17/F");
  varia->Branch("bin18", &bin18, "bin18/F");
  varia->Branch("bin19", &bin19, "bin19/F");
  varia->Branch("bin20", &bin20, "bin20/F");  
  varia->Branch("bin21", &bin21, "bin21/F");  
  varia->Branch("bin22", &bin22, "bin22/F");  
  varia->Branch("bin23", &bin23, "bin23/F");  
  varia->Branch("bin24", &bin24, "bin24/F");  
  varia->Branch("bin25", &bin25, "bin25/F");  
  varia->Branch("bin26", &bin26, "bin26/F");
  varia->Branch("bin27", &bin27, "bin27/F");
  varia->Branch("bin28", &bin28, "bin28/F");
  varia->Branch("bin29", &bin29, "bin29/F");
  varia->Branch("bin30", &bin30, "bin30/F");  
  varia->Branch("bin31", &bin31, "bin31/F");  
  varia->Branch("bin32", &bin32, "bin32/F");  
  varia->Branch("bin33", &bin33, "bin33/F");  
  varia->Branch("bin34", &bin34, "bin34/F");  
  varia->Branch("bin35", &bin35, "bin35/F");  
  varia->Branch("bin41", &bin41, "bin41/F");
  varia->Branch("bin42", &bin42, "bin42/F");
  varia->Branch("bin43", &bin43, "bin43/F");
  varia->Branch("bin44", &bin44, "bin44/F");
  varia->Branch("bin45", &bin45, "bin45/F");
  varia->Branch("bin46", &bin46, "bin46/F");
  varia->Branch("bin47", &bin47, "bin47/F");
  varia->Branch("bin48", &bin48, "bin48/F");
  varia->Branch("bin49", &bin49, "bin49/F");
  varia->Branch("bin50", &bin50, "bin50/F");  
  varia->Branch("bin51", &bin51, "bin51/F");  
  varia->Branch("bin52", &bin52, "bin52/F");  
  varia->Branch("bin53", &bin53, "bin53/F");  
  varia->Branch("bin54", &bin54, "bin54/F");  
  varia->Branch("bin55", &bin55, "bin55/F");  
  varia->Branch("bin56", &bin56, "bin56/F");
  varia->Branch("bin57", &bin57, "bin57/F");
  varia->Branch("bin58", &bin58, "bin58/F");
  varia->Branch("bin59", &bin59, "bin59/F");
  varia->Branch("bin60", &bin60, "bin60/F");  
  varia->Branch("type",  &type,  "type/I");
    
  TMultiLayerPerceptron *mlp1 = new TMultiLayerPerceptron("@bin3,@bin4,@bin5,@bin6,@bin7,@bin8,@bin9,@bin10,@bin11,@bin12,@bin13,@bin14,@bin15,@bin16,@bin17,@bin18,@bin19,@bin20,@bin31,@bin32,@bin33,@bin34,@bin35:63:21:type",varia,"bin31>0","bin31>0");
    
  strcpy(cmd,"ln -fs  $FEDRA_ROOT/src/libShower/weights_20.txt .");
  gSystem->Exec(cmd);
  mlp1->LoadWeights("weights_20.txt");
    
  TMultiLayerPerceptron *mlp2 = new TMultiLayerPerceptron("@bin3,@bin4,@bin5,@bin6,@bin7,@bin8,@bin9,@bin10,@bin11,@bin12,@bin13,@bin14,@bin15,@bin31,@bin32,@bin33,@bin34,@bin35:63:21:type",varia,"bin31>0","bin31>0");
  strcpy(cmd,"ln -fs  $FEDRA_ROOT/src/libShower/weights_15.txt .");
  gSystem->Exec(cmd);
  mlp2->LoadWeights("weights_15.txt");
    
  TMultiLayerPerceptron *mlp3 = new TMultiLayerPerceptron("@bin3,@bin4,@bin5,@bin6,@bin7,@bin8,@bin9,@bin10,@bin11,@bin12,@bin13,@bin14,@bin15,@bin16,@bin17,@bin18,@bin19,@bin20,@bin21,@bin22,@bin23,@bin24,@bin25,@bin26,@bin27,@bin28,@bin29,@bin30,@bin31,@bin32,@bin33,@bin34,@bin35:63:21:type",varia,"bin31>0","bin31>0");
  strcpy(cmd,"ln -fs  $FEDRA_ROOT/src/libShower/weights_30.txt .");
  gSystem->Exec(cmd);
  mlp3->LoadWeights("weights_30.txt");
    
  TMultiLayerPerceptron *mlp4 = new TMultiLayerPerceptron("@bin3,@bin4,@bin5,@bin6,@bin7,@bin8,@bin9,@bin10,@bin11,@bin12,@bin13,@bin14,@bin15,@bin16,@bin17,@bin18,@bin19,@bin20,@bin21,@bin22,@bin23,@bin24,@bin25,@bin26,@bin27,@bin28,@bin29,@bin30,@bin41,@bin42,@bin43,@bin44,@bin45,@bin46,@bin47,@bin48,@bin49,@bin50,@bin51,@bin52,@bin53,@bin54,@bin55,@bin56,@bin57,@bin58,@bin59,@bin60,@bin31,@bin32,@bin33,@bin34,@bin35:63:21:type",varia,"bin31>0","bin31>0");
  strcpy(cmd,"ln -fs  $FEDRA_ROOT/src/libShower/weights_50.txt .");
  gSystem->Exec(cmd);
  mlp4->LoadWeights("weights_50.txt");
    
  TH1F *histogl_e = new TH1F("longitudinal profile","longitudinal profile",50,0,50);
    
  TH1F *historcut30 = new TH1F("Delta r 30"," ",50,0,200);
  TH1F *histotcut30 = new TH1F("Delta theta 30"," ",50,0,0.2);
  TH1F *historcut50 = new TH1F("Delta r 50"," ",50,0,200);
  TH1F *histotcut50 = new TH1F("Delta theta 50"," ",50,0,0.2);
    
  TH1F *historcut20 = new TH1F("Delta r 20 "," ",50,0,200);
  TH1F *histotcut20 = new TH1F("Delta theta 20"," ",50,0,0.2);
  TH1F *historcut15 = new TH1F("Delta r 15"," ",50,0,200);
  TH1F *histotcut15 = new TH1F("Delta theta 15"," ",50,0,0.2);

  float val30,val20,val15,val50;

  sprintf(fname_e,"shower1.root");  //input shower tree file 

  TFile *file_e2 = new TFile(fname_e ,"READ");
  TTree *treebranch_e = (TTree *) file_e2->Get("treebranch");
  treebranch_e->SetBranchAddress("nfilmb", nfilmb);
  treebranch_e->SetBranchAddress("sizeb", &sizeb); 
  treebranch_e->SetBranchAddress("sizeb15", &sizeb15); 
  treebranch_e->SetBranchAddress("sizeb20", &sizeb20); 
  treebranch_e->SetBranchAddress("sizeb30", &sizeb30);     
  treebranch_e->SetBranchAddress("isizeb", &isizeb); 
  treebranch_e->SetBranchAddress("chi2btkb", &chi2btkb); 
  treebranch_e->SetBranchAddress("tagprimary", tagprimary);   
  treebranch_e->SetBranchAddress("txb", txb);
  treebranch_e->SetBranchAddress("tyb", tyb);
  treebranch_e->SetBranchAddress("xb", xb);
  treebranch_e->SetBranchAddress("yb", yb);
  treebranch_e->SetBranchAddress("zb", zb);
  treebranch_e->SetBranchAddress("showerID", &showerID);
  treebranch_e->SetBranchAddress("plateb", plateb);
  treebranch_e->SetBranchAddress("idb", idb);
  treebranch_e->SetBranchAddress("deltarb", deltarb);
  treebranch_e->SetBranchAddress("deltathetab", deltathetab);
  treebranch_e->SetBranchAddress("number_eventb", &number_eventb);   
  treebranch_e->SetBranchAddress("ntrace1simub", &ntrace1simub);
  treebranch_e->SetBranchAddress("ntrace2simub", &ntrace2simub);
  treebranch_e->SetBranchAddress("E_MC", &E_MC);
	
	
  for (Int_t ik = 0; ik < treebranch_e->GetEntries(); ik++) 
  {
    treebranch_e->GetEntry(ik);
    histogl_e->Reset();
    historcut15->Reset();
    histotcut15->Reset();
    historcut20->Reset();
    histotcut20->Reset();
    historcut30->Reset();
    histotcut30->Reset();
    historcut50->Reset();
    histotcut50->Reset();

    output50 = -10;
    output30 = -10;
    output20 = -10;
    output15 = -10;

    if (isizeb>0){
      ntrke50 = 0;
      Rcut50 = 0.;
      Tcut50 = 0.;
      ntrke30 = 0;
      Rcut30 = 0.;
      Tcut30 = 0.;
      ntrke20 = 0;
      Rcut20 = 0.;
      Tcut20 = 0.;
      ntrke15 = 0;
      Rcut15 = 0.;
      Tcut15 = 0.;
      flag15 = 0;
      flag20 = 0;
      flag30 = 0;
      flag50 = 0;

      if (isizeb==15)flag15=1;
      if (isizeb==20)flag20=1;
      if (isizeb==30)flag30=1;
      if (isizeb==50)flag50=1;
			

      for (Int_t ibtke = 0; ibtke < sizeb; ibtke++){ // loop on btk
		
        histogl_e->Fill(nfilmb[ibtke]);
        if (nfilmb[ibtke]<51)ntrke50++;
        if (nfilmb[ibtke]<31)ntrke30++;
        if (nfilmb[ibtke]<21)ntrke20++;
        if (nfilmb[ibtke]<16)ntrke15++;	
		
        if (ibtke>0&&nfilmb[ibtke]<51)
        {
          Rcut50 += deltarb[ibtke];
          Tcut50 += deltathetab[ibtke];
          historcut50->Fill(deltarb[ibtke]);
          histotcut50->Fill(deltathetab[ibtke]);
        }		
        if (ibtke>0&&nfilmb[ibtke]<31)
        {
          Rcut30 += deltarb[ibtke];
          Tcut30 += deltathetab[ibtke];
          historcut30->Fill(deltarb[ibtke]);
          histotcut30->Fill(deltathetab[ibtke]);
        }
        if (ibtke>0&&nfilmb[ibtke]<21)
        {
          Rcut20 += deltarb[ibtke];
          Tcut20 += deltathetab[ibtke];
          historcut20->Fill(deltarb[ibtke]);
          histotcut20->Fill(deltathetab[ibtke]);
        }
        if (ibtke>0&&nfilmb[ibtke]<16)
        {
          Rcut15 += deltarb[ibtke];
          Tcut15 += deltathetab[ibtke];
          historcut15->Fill(deltarb[ibtke]);
          histotcut15->Fill(deltathetab[ibtke]);
        }		           
      }
	    
      bin1 = histogl_e->GetBinContent(1);		
      bin2 = histogl_e->GetBinContent(2);
      bin3 = histogl_e->GetBinContent(3);
      bin4 = histogl_e->GetBinContent(4);
      bin5 = histogl_e->GetBinContent(5);
      bin6 = histogl_e->GetBinContent(6);
      bin7 = histogl_e->GetBinContent(7);
      bin8 = histogl_e->GetBinContent(8);
      bin9 = histogl_e->GetBinContent(9);
      bin10 = histogl_e->GetBinContent(10);
      bin11 = histogl_e->GetBinContent(11);
      bin12 = histogl_e->GetBinContent(12);
      bin13 = histogl_e->GetBinContent(13);
      bin14 = histogl_e->GetBinContent(14);
      bin15 = histogl_e->GetBinContent(15);
      bin16 = histogl_e->GetBinContent(16);
      bin17 = histogl_e->GetBinContent(17);
      bin18 = histogl_e->GetBinContent(18);
      bin19 = histogl_e->GetBinContent(19);
      bin20 = histogl_e->GetBinContent(20);
      bin21 = histogl_e->GetBinContent(21);
      bin22 = histogl_e->GetBinContent(22);
      bin23 = histogl_e->GetBinContent(23);
      bin24 = histogl_e->GetBinContent(24);
      bin25 = histogl_e->GetBinContent(25);
      bin26 = histogl_e->GetBinContent(26);
      bin27 = histogl_e->GetBinContent(27);
      bin28 = histogl_e->GetBinContent(28);
      bin29 = histogl_e->GetBinContent(29);
      bin30 = histogl_e->GetBinContent(30);
      bin41 = histogl_e->GetBinContent(31);
      bin42 = histogl_e->GetBinContent(32);
      bin43 = histogl_e->GetBinContent(33);
      bin44 = histogl_e->GetBinContent(34);
      bin45 = histogl_e->GetBinContent(35);
      bin46 = histogl_e->GetBinContent(36);
      bin47 = histogl_e->GetBinContent(37);
      bin48 = histogl_e->GetBinContent(38);
      bin49 = histogl_e->GetBinContent(39);
      bin50 = histogl_e->GetBinContent(40);
      bin51 = histogl_e->GetBinContent(41);
      bin52 = histogl_e->GetBinContent(42);
      bin53 = histogl_e->GetBinContent(43);
      bin54 = histogl_e->GetBinContent(44);
      bin55 = histogl_e->GetBinContent(45);
      bin56 = histogl_e->GetBinContent(46);
      bin57 = histogl_e->GetBinContent(47);
      bin58 = histogl_e->GetBinContent(48);
      bin59 = histogl_e->GetBinContent(49);
      bin60 = histogl_e->GetBinContent(50);
// input variables for 20 sheets	    
      bin31 = ntrke20;
      bin32 = Rcut20/ntrke20;
      bin33 = Tcut20/ntrke20;
      bin34 =  historcut20->GetRMS();
      bin35 =  histotcut20->GetRMS();
	    
      params[0] = bin3;
      params[1] = bin4;
      params[2] = bin5;
      params[3] = bin6;
      params[4] = bin7;
      params[5] = bin8;
      params[6] = bin9;
      params[7] = bin10;
      params[8] = bin11;
      params[9] = bin12;
      params[10] = bin13;
      params[11] = bin14;
      params[12] = bin15;
      params[13] = bin16;
      params[14] = bin17;
      params[15] = bin18;
      params[16] = bin19;
      params[17] = bin20;
      params[18] = bin31;
      params[19] = bin32;
      params[20] = bin33;
      params[21] = bin34;
      params[22] = bin35;

      val20=mlp1->Evaluate(0, params);

// input variables for 30 sheets
      bin31 = ntrke30;
      bin32 = Rcut30/ntrke30;
      bin33 = Tcut30/ntrke30;
      bin34 =  historcut30->GetRMS();
      bin35 =  histotcut30->GetRMS();

      params[18] = bin21;
      params[19] = bin22;
      params[20] = bin23;
      params[21] = bin24;
      params[22] = bin25;
      params[23] = bin26;
      params[24] = bin27;
      params[25] = bin28;
      params[26] = bin29;
      params[27] = bin30;
      params[28] = bin31;
      params[29] = bin32;
      params[30] = bin33;
      params[31] = bin34;
      params[32] = bin35;
      val30 = mlp3->Evaluate(0, params);

// input variables for 50 sheets
      bin31 = ntrke50;
      bin32 = Rcut50/ntrke50;
      bin33 = Tcut50/ntrke50;
      bin34 =  historcut50->GetRMS();
      bin35 =  histotcut50->GetRMS();

      params[28] = bin41;
      params[29] = bin42;
      params[30] = bin43;
      params[31] = bin44;
      params[32] = bin45;
      params[33] = bin46;
      params[34] = bin47;
      params[35] = bin48;
      params[36] = bin49;
      params[37] = bin50;
      params[38] = bin51;
      params[39] = bin52;
      params[40] = bin53;
      params[41] = bin54;
      params[42] = bin55;
      params[43] = bin56;
      params[44] = bin57;
      params[45] = bin58;
      params[46] = bin59;
      params[47] = bin60;

      params[48] = bin31;
      params[49] = bin32;
      params[50] = bin33;
      params[51] = bin34;
      params[52] = bin35;
      val50 = mlp4->Evaluate(0, params);

// input variables for 15 sheets

      bin31 = ntrke15;
      bin32 = Rcut15/ntrke15;
      bin33 = Tcut15/ntrke15;
      bin34 =  historcut15->GetRMS();
      bin35 =  histotcut15->GetRMS();
      params[13] = bin31;
      params[14] = bin32;
      params[15] = bin33;
      params[16] = bin34;
      params[17] = bin35;

      val15=mlp2->Evaluate(0, params);

// save final output values

      output50= val50;
      output30= val30;
      output20= val20;
      output15= val15;
      eProb90=0;
      eProb1=0;
	    
      if (output15>0.38) eProb90=1;
      if (flag20==1&&output20>0.37) eProb90=1;
      if (flag30==1&&output30>0.36) eProb90=1;
      if (flag50==1&&output50>0.34)  eProb90=1;

      if (output15>0.78) eProb1=1;
      if (flag20==1&&output20>0.73) eProb1=1;
      if (flag30==1&&output30>0.7) eProb1=1;
      if (flag50==1&&output50>0.73) eProb1=1;
	    /*
      if (output15>0.345) eProb90=1;
      if (output15>0.345&&output20>0.355) eProb90=1;
      if (output15>0.345&&output20>0.355&&output30>0.385) eProb90=1;
      if (output15>0.345&&output20>0.355&&output30>0.385&&output50>0.37) eProb90=1;

      if (output15>0.64) eProb1=1;
      if (output15>0.64&&output20>0.65) eProb1=1;
      if (output15>0.64&&output20>0.65&&output30>0.64) eProb1=1;
      if (output15>0.64&&output20>0.65&&output30>0.64&output50>0.645) eProb1=1;
      */
      if (eProb90==1)
      {
        cout<<  " output30: " << val30  <<  " output20: " << val20 <<  "   output15: " << val15 << endl;
      }	  
      treesaveb3->Fill();
			//cout << "Do treesaveb3->Fill();"<<endl;
    }	
  }  
  file_e2->Close(); 
    
  fileout3->cd();
  treesaveb3->Write();
  fileout3->Close();       
}

void EdbShowerRec::Shower2Tracks() 
{

  EdbSegP *seg;
  // EdbSegP *s1=0;
  EdbTrackP *track2;
  EdbPVRec *ali;

  ali = new EdbPVRec();
  // dproc = new EdbDataProc();

  EdbPattern *pat=0;

  char fname_e[128];

  float x,y,tx,ty,z;
  int w2;
  //  int plate[10000];

  sprintf(fname_e,"Shower.root");  //input shower tree file 
  
  TFile *file_e2 = new TFile(fname_e ,"READ");
  TTree *treebranch_e = (TTree *) file_e2->Get("treebranch");
  treebranch_e->SetBranchAddress("nfilmb", nfilmb);   // max(nfilmb)=npl
  treebranch_e->SetBranchAddress("sizeb", &sizeb);   // nseg
  treebranch_e->SetBranchAddress("isizeb", &isizeb); 
  treebranch_e->SetBranchAddress("chi2btkb", &chi2btkb); 
  treebranch_e->SetBranchAddress("txb", txb);
  treebranch_e->SetBranchAddress("tyb", tyb);
  treebranch_e->SetBranchAddress("xb", xb);
  treebranch_e->SetBranchAddress("yb", yb);
  treebranch_e->SetBranchAddress("zb", zb);
  treebranch_e->SetBranchAddress("number_eventb", &number_eventb);   
  treebranch_e->SetBranchAddress("ntrace2simub", &ntrace2simub); // grain
  treebranch_e->SetBranchAddress("eProb1", &eProb1); 
  treebranch_e->SetBranchAddress("eProb90", &eProb90); 
  int nentries =  treebranch_e->GetEntries();

  for (int i=0;i<nentries;i++)
  {
    track2 = new EdbTrackP();
    treebranch_e->GetEntry(i);



    if (eProb1==0||eProb90==0) continue; // remove non electron!!!

    for (int j=0;j<sizeb;j++)
    {

      x=xb[j];
      y=yb[j];
      tx=txb[j];
      ty=tyb[j];
      z=zb[j];
	  // plate[j] = abs(57+(int)(z/1300.));
      w2=ntrace2simub[j];

      seg = new EdbSegP();
      seg->Set(j, x, y, tx, ty, w2, 0);
      seg->SetZ(z);

	  //	  seg->SetFlag(0);
      seg->SetDZ(300.);
	  //seg->SetChi2(chi2btkb[j]);
	  // 	  seg->SetPID(plate[j]);
	  // seg->SetPID(j); // DISPLAY PROBLEM IF RIGHT PID IS USED !!!!!!!!!!!!!!
      pat = ali->GetPattern( seg->PID() );
      if(!pat) { 
	    // 	    printf("WARNING: no pattern with pid %d: creating new one!\n",seg->PID()); 
        pat = new EdbPattern( 0., 0., seg->Z() );
 	    //pat->SetID(seg->PID());
        ali->AddPatternAt(pat,seg->PID());
        ali->AddPatternAt(pat,j);
      }
      pat->AddSegment(*seg);
      track2->AddSegment(seg);
    }    
    track2->SetSegmentsTrack(i);
      //track2->SetPID(plate[0]);
    track2->SetID(i);
    track2->SetNpl(nfilmb[sizeb-1]);
    track2->SetProb(1.);
    track2->SetFlag(10);
      // track2->SetCounters();
    track2->FitTrack();
    ali->AddTrack(track2);
  //     track2->PrintNice();
  }



  const char *file="test.root";

  if(!ali) return ;
  TObjArray *trarr = ali->eTracks;
  if(!trarr) return ;
  float xv=ali->X();
  float yv=ali->Y();

  printf("write tracks into %s ... \n",file);
  TFile fil(file,"RECREATE");
  TTree *tracks= new TTree("tracks","tracks");

  EdbTrackP    *track = new EdbTrackP();
  EdbSegP      *tr = (EdbSegP*)track;
  TClonesArray *segments  = new TClonesArray("EdbSegP");
  TClonesArray *segmentsf = new TClonesArray("EdbSegP");

  int   nseg,trid,npl,n0;
  float w=0.;

  tracks->Branch("trid",&trid,"trid/I");
  tracks->Branch("nseg",&nseg,"nseg/I");
  tracks->Branch("npl",&npl,"npl/I");
  tracks->Branch("n0",&n0,"n0/I");
  tracks->Branch("xv",&xv,"xv/F");
  tracks->Branch("yv",&yv,"yv/F");
  tracks->Branch("w",&w,"w/F");
  tracks->Branch("t.","EdbSegP",&tr,32000,99);
  tracks->Branch("s", &segments);
  tracks->Branch("sf",&segmentsf);

  int ntr = trarr->GetEntriesFast();
  for(int itr=0; itr<ntr; itr++) {
    track = (EdbTrackP*)(trarr->At(itr));

    trid = track->ID();
    nseg = track->N();
    npl  = track->Npl();
    n0   = track->N0();

    tr = (EdbSegP*)track;

    segments->Clear("C");
    segmentsf->Clear("C");
    nseg = track->N();
    w    = track->Wgrains();
    EdbSegP *s=0,*sf=0;
    for(int is=0; is<nseg; is++) {
      s = track->GetSegment(is);
      if(s) new((*segments)[is])  EdbSegP( *s );
      sf = track->GetSegment(is);
      if(sf) new((*segmentsf)[is])  EdbSegP( *sf );
    }

    track->SetVid( 0, tracks->GetEntries() );  // put track counter in t.eVid[1]
    tracks->Fill();
    track->Clear();
  }

  tracks->Write();
  fil.Close();
  printf("%d tracks are written \n",ntr);
  
}


//-------------------------------------------------------------------
int EdbShowerRec::AddInitiatorBT(EdbSegP* seg)
{
  // Check if eInitiatorBTs array exits, otherwise create.
  if (!eInitiatorBTIsCreated) {
    eInitiatorBTs=new TObjArray(9999);
    eInitiatorBTIsCreated=kTRUE;
  }
  // Add the segment to the eInitiatorBTs array
  eInitiatorBTs->Add(seg);
  
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::AddInitiatorBT(seg) added. Now entries in eInitiatorBTs: ";
  if (gEDBDEBUGLEVEL>2) cout << eInitiatorBTs->GetEntries() << endl;
  
  eInitiatorBTIsFilled=kTRUE;
  return 1;
}
//-------------------------------------------------------------------




//-------------------------------------------------------------------
void EdbShowerRec::PrintInitiatorBTs()
{
  // Print the eInitiatorBTs nice...
  if (!eInitiatorBTIsFilled) return;
  
  EdbSegP* seg; 
  for (int i=0; i< eInitiatorBTs->GetEntries(); ++i) {
    seg=(EdbSegP*) eInitiatorBTs->At(i);
    //seg->PrintNice();
  }
  return;
}
//-------------------------------------------------------------------







//-------------------------------------------------------------------
void EdbShowerRec::CalculateEnergyValues()
{
//   if (!gROOT->GetClass("TMultiLayerPerceptron")) {
//     cout << "Need to load libMLP seperately"<<endl;
//     gSystem->Load("libMLP");
//   } 
  
  gEDBDEBUGLEVEL=3;
	
  if (gEDBDEBUGLEVEL>2) cout << "------------------------------------------------------"<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::CalculateEnergyValues()"<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "------------------------------------------------------"<<endl;
  

  ///--------------------------------------------
	///---CreateANNTree:
	///--------------------------------------------
  Energy_CreateANNTree();
	///--------------------------------------------
  
  ///--------------------------------------------
	///---CreateEnergyResolutionFitFunction:
	///--------------------------------------------
  Energy_CreateEnergyResolutionFitFunction();
  ///--------------------------------------------
  
	///--------------------------------------------
	///---Extract the desired profile
	///--------------------------------------------
  Energy_ExtractShowerParametrisationProfile();
	///--------------------------------------------
  
  return; 
}




//-------------------------------------------------------------------
void EdbShowerRec::LoadShowerFile(TString ShowerFileName="Shower2.root")
{
  if (gEDBDEBUGLEVEL>2) cout << "------------------------------------------------------"<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::LoadShowerFile()"<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "------------------------------------------------------"<<endl;
  
  FileReconstructedShowerTree = new TFile("Shower2.root");
  if (FileReconstructedShowerTree==0)  { 
    cout << "ERROR: FileReconstructedShowerTree==NULL. ABORTING."<<endl;
    return;
  }
  
  if (gEDBDEBUGLEVEL>2) FileReconstructedShowerTree->ls();
  if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::LoadShowerFile() done."<<endl;
  return;
}


//-------------------------------------------------------------------
void EdbShowerRec::Energy_ExtractShowerParametrisationProfile()
{
  if (gEDBDEBUGLEVEL>2) cout << "------------------------------------------------------"<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile()"<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "------------------------------------------------------"<<endl;
  
  treesaveb3 = new TTree("treebranch","tree of branchtrack");
  treesaveb3->Branch("number_eventb",&number_eventb,"number_eventb/I");
  treesaveb3->Branch("sizeb",&sizeb,"sizeb/I");
  treesaveb3->Branch("sizeb15",&sizeb15,"sizeb15/I");
  treesaveb3->Branch("sizeb20",&sizeb20,"sizeb20/I");
  treesaveb3->Branch("sizeb30",&sizeb30,"sizeb30/I");
  treesaveb3->Branch("output15",&output15,"output15/F");
  treesaveb3->Branch("output20",&output20,"output20/F");
  treesaveb3->Branch("output30",&output30,"outpu30/F");
  treesaveb3->Branch("output50",&output50,"outpu50/F");
  treesaveb3->Branch("eProb90",&eProb90,"eProb90/I");
  treesaveb3->Branch("eProb1",&eProb1,"eProb1/I");
  treesaveb3->Branch("E_MC",&E_MC,"E_MC/F");
  treesaveb3->Branch("idb",idb,"idb[sizeb]/I");
  treesaveb3->Branch("plateb",plateb,"plateb[sizeb]/I");
  treesaveb3->Branch("showerID",&showerID,"showerID/I");
  treesaveb3->Branch("isizeb",&isizeb,"isizeb/I");
  treesaveb3->Branch("xb",xb,"xb[sizeb]/F");
  treesaveb3->Branch("yb",yb,"yb[sizeb]/F");
  treesaveb3->Branch("zb",zb,"zb[sizeb]/F");
  treesaveb3->Branch("txb",txb,"txb[sizeb]/F");
  treesaveb3->Branch("tyb",tyb,"tyb[sizeb]/F");
  treesaveb3->Branch("nfilmb",nfilmb,"nfilmb[sizeb]/I");
  treesaveb3->Branch("ntrace1simub",ntrace1simub,"ntrace1simu[sizeb]/I");
  treesaveb3->Branch("ntrace2simub",ntrace2simub,"ntrace2simu[sizeb]/I");
  treesaveb3->Branch("chi2btkb",chi2btkb,"chi2btkb[sizeb]/F");
  treesaveb3->Branch("deltarb",deltarb,"deltarb[sizeb]/F");
  treesaveb3->Branch("deltathetab",deltathetab,"deltathetab[sizeb]/F");
  treesaveb3->Branch("deltaxb",deltaxb,"deltaxb[sizeb]/F");
  treesaveb3->Branch("deltayb",deltayb,"deltayb[sizeb]/F");
  treesaveb3->Branch("tagprimary",tagprimary,"tagprimary[sizeb]/F");
 
  treesaveb3->Branch("Energy",&EnergyCorrectedb,"EnergyCorrectedb/F");
  treesaveb3->Branch("EnergyUnCorrected",&EnergyUnCorrectedb,"EnergyUnCorrectedb/F");
  treesaveb3->Branch("EnergySigma",&EnergySigmaCorrectedb,"EnergySigmaCorrectedb/F");
  treesaveb3->Branch("EnergySigmaUnCorrected",&EnergySigmaUnCorrectedb,"EnergySigmaUnCorrectedb/F");

 //ouput file
  fileout3 = new TFile("Shower.root","RECREATE"); 

  // -------------------------------------------------------------
  //  If no showerTree was created we have to load the ShowerTree
  //  most probably from an already reconstructed file.
  //  Float_t energy_shot_particle;
  if (!eShowerTreeIsDone) LoadShowerFile();
  // Get Treebranch:
  // Either from shower file (then it has to be casted) or from the
  // reconstruction, then its automatically there.
  if (!eShowerTreeIsDone) { 
    treesaveb = (TTree*)(FileReconstructedShowerTree->Get("treebranch"));
    treesaveb->SetBranchAddress("nfilmb", nfilmb);
    treesaveb->SetBranchAddress("sizeb", &sizeb); 
    treesaveb->SetBranchAddress("sizeb15", &sizeb15); 
    treesaveb->SetBranchAddress("sizeb20", &sizeb20); 
    treesaveb->SetBranchAddress("sizeb30", &sizeb30);
    treesaveb->SetBranchAddress("output15", &output15); 
    treesaveb->SetBranchAddress("output20", &output20); 
    treesaveb->SetBranchAddress("output30", &output30);     
    treesaveb->SetBranchAddress("isizeb", &isizeb); 
    treesaveb->SetBranchAddress("chi2btkb", &chi2btkb); 
    treesaveb->SetBranchAddress("tagprimary", tagprimary);   
    treesaveb->SetBranchAddress("txb", txb);
    treesaveb->SetBranchAddress("tyb", tyb);
    treesaveb->SetBranchAddress("xb", xb);
    treesaveb->SetBranchAddress("yb", yb);
    treesaveb->SetBranchAddress("zb", zb);
    treesaveb->SetBranchAddress("showerID", &showerID);
    treesaveb->SetBranchAddress("plateb", plateb);
    treesaveb->SetBranchAddress("idb", idb);
    treesaveb->SetBranchAddress("deltarb", deltarb);
    treesaveb->SetBranchAddress("deltathetab", deltathetab);
    treesaveb->SetBranchAddress("number_eventb", &number_eventb);   
    treesaveb->SetBranchAddress("ntrace1simub", &ntrace1simub);
    treesaveb->SetBranchAddress("ntrace2simub", &ntrace2simub);
    treesaveb->SetBranchAddress("E_MC", &E_MC);
    treesaveb->SetBranchAddress("Energy", &EnergyCorrectedb);
    treesaveb->SetBranchAddress("EnergyUnCorrected", &EnergyUnCorrectedb);
    treesaveb->SetBranchAddress("EnergySigma", &EnergySigmaCorrectedb);
    treesaveb->SetBranchAddress("EnergySigmaUnCorrected", &EnergySigmaUnCorrectedb);
  }
  else {; /* treesaveb already declared and filled. */ }
  // -------------------------------------------------------------  
  
  
  // Just local variables which are neeeded once for the shower parametrisation...  
  Float_t Dr[57];
  Float_t X0[57];
  Float_t Y0[57];
  Float_t TX0=0. ,TY0=0.;
  Float_t theta[57];
  Float_t dist;
  Int_t nentries_e;
  Int_t nentries_withisizeb=0;
  Int_t NumberOfPlates;
  Int_t sizeb_for_plates;
  Float_t BT_deltaR_mean;
  Float_t BT_deltaR_rms;
  Float_t BT_deltaT_mean;
  Float_t BT_deltaT_rms;
  Int_t longprofile[57]; // Depens on how much sheets;
  Int_t numberofilms;
  Float_t EnergyMC;
  TString histname;
  TH1D *histo_nbtk;
  TH1D *histo_longprofile;
  TH1D* histo_deltaR;
  TH1D* histo_deltaT;
  // Just local variables which are neeeded once for the shower parametrisation...END  
    
  // Create ShowerProfileTree ...
	
  if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile(): Create ShowerProfileTree ..."<<endl;
  ShowerParametrisationTree = new TTree("ShowerProfileTree","ShowerProfileTree");

  ShowerParametrisationTree->Branch("sizeb_for_plates",&sizeb_for_plates,"sizeb_for_plates/I");
  ShowerParametrisationTree->Branch("BT_deltaR_mean",&BT_deltaR_mean,"BT_deltaR_mean/F");
  ShowerParametrisationTree->Branch("BT_deltaR_rms",&BT_deltaR_rms,"BT_deltaR_rms/F");
  ShowerParametrisationTree->Branch("BT_deltaT_mean",&BT_deltaT_mean,"BT_deltaT_mean/F");
  ShowerParametrisationTree->Branch("BT_deltaT_rms",&BT_deltaT_rms,"BT_deltaT_rms/F");
  ShowerParametrisationTree->Branch("longprofile",longprofile,"longprofile[58]/I");
  ShowerParametrisationTree->Branch("numberofilms",&numberofilms,"numberofilms/I");
  ShowerParametrisationTree->Branch("EnergyMC",&E_MC,"EnergyMC/F");
 
  ShowerParametrisationTree->Branch("Energy",&EnergyCorrectedb,"EnergyCorrectedb/F");
  ShowerParametrisationTree->Branch("EnergyUnCorrected",&EnergyUnCorrectedb,"EnergyUnCorrectedb/F");
  ShowerParametrisationTree->Branch("EnergySigma",&EnergySigmaCorrectedb,"EnergySigmaCorrectedb/F");
  ShowerParametrisationTree->Branch("EnergySigmaUnCorrected",&EnergySigmaUnCorrectedb,"EnergySigmaUnCorrectedb/F");

	//=C= -----------------------------------------------------	
  histname="histo_nbtk_av";
  histo_nbtk_av		= new TH1D(histname,"Avergage basetrack numbers",21,0.0,210.0);
  histname="histo_longprofile_av";
  histo_longprofile_av 	= new TH1D(histname,"Basetracks per emulsion number",56,0.0,56.0);
  histname="histo_deltaR_mean";
  histo_deltaR_mean  		= new TH1D(histname,"Mean #deltar of all BTs in one shower",61,0.0,61.0);
  histname="histo_deltaT_mean";
  histo_deltaT_mean 			= new TH1D(histname,"Mean #delta#theta of all BTs in one shower",50,0.0,0.05);	
  histname="histo_deltaR_rms";
  histo_deltaR_rms  			= new TH1D(histname,"RMS #deltar of all BTs in one shower",61,0.0,61.0);
  histname="histo_deltaT_rms";
  histo_deltaT_rms 			= new TH1D(histname,"RMS #delta#theta of all BTs in one shower",50,0.0,0.025);	
	//=C= These Histos are for each Shower Entrie filled new
  histname="histo_nbtk";
  histo_nbtk  						= new TH1D(histname,"Basetracks in the shower",250,0.0,250.0);
  histname="histo_longprofile";
  histo_longprofile  		= new TH1D(histname,"Basetracks per emulsion number",56,0.0,56.0);
  histname="histo_deltaR";
  histo_deltaR  					= new TH1D(histname,"Mean #deltar of all BTs in Shower",100,0.0,100.0);
  histname="histo_deltaT";
  histo_deltaT 					= new TH1D(histname,"Mean #delta#theta of all BTs in Shower",100,0.0,0.05);	
	//=C= -----------------------------------------------------
  eHistEnergyNominal      = new TH1F("eHistEnergyNominal","eHistEnergyNominal",100,0,20);
  eHistEnergyCorrectedb   = new TH1F("eHistEnergyCorrectedb","eHistEnergyCorrectedb",100,0,20);
  eHistEnergyUnCorrectedb = new TH1F("eHistEnergyUnCorrectedb","eHistEnergyUnCorrectedb",100,0,20);
  //=C= -----------------------------------------------------
  histo_nbtk_av->Reset();
  histo_nbtk->Reset();
  histo_longprofile_av->Reset();
  histo_deltaR_mean->Reset();
  histo_deltaT_mean->Reset();
  histo_deltaR_rms->Reset();
  histo_deltaT_rms->Reset();
  //=C= -----------------------------------------------------
  
  //=C= Again Define some Variables used for calculating:
  nentries_e = (Int_t) treesaveb -> GetEntries();
  for(Int_t j = 0; j<57; ++j){ Dr[j] = 0.03*j*1300. +20.0;}
  Float_t zmax=0;
  Float_t zmin=0;

  //=C= -----------------------------------------------------
  Double_t params[70];
  Double_t val;
  Double_t val_corr;
  Double_t sigma_val_corr;
  Double_t sigma_val;
  
  Int_t NrOfANNInputNeurons[]={11,16,21,26,31,36,41,46};
  for (Int_t i=0; i<8; ++i) NrOfANNInputNeurons[i]=NrOfANNInputNeurons[i]+1+4;
	//=C= -----------------------------------------------------
  
  cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile(): Loop over reconstructed showers (=" << nentries_e << " total)."<< endl;
  //=C= -----------------------------------------------------
  //=C= Loop over reconstructed Showers (Entries):
  for (Int_t ige = 0; ige < nentries_e; ++ige) {
    
    /// DEBUG -----------------------------------
    //    if (ige>5&&ige<nentries_e-5) continue;
    //    if (ige>900) continue;
    //    if (ige<nentries_e-900) continue;
    /// DEBUG -----------------------------------
    
    if (ige%50==0) cout<<"===   Doing Entry "<< ige << endl;
    treesaveb->GetEntry(ige);
    if (gEDBDEBUGLEVEL>3) treesaveb->Show(ige);  /// DEBUG OPTION
		
		// GetEnergyMC:
    EnergyMC=E_MC;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() === EnergyMC= "<< EnergyMC << endl;
		
    // Search for local zmax and zmin in the whole shower event to
	  // guess from that the number of plates...
    zmax=0; zmin=0; // Reinit zmax, min;
    for (Int_t j=0;j<sizeb;++j) {
      if (zb[j]>zmax) zmax=zb[j];
      if (zb[j]<zmin) zmin=zb[j];
    }
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() === zmax= "<< zmax << endl;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() === zmin= "<< zmin << endl;
	
	  // Calculate the plates.
    Int_t pl=int(zmax-zmin)/1300+1;
    NumberOfPlates=pl;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() === pl= "<< pl << endl;
    
	  // Make sure that its at least 1 or more...
    NumberOfPlates=max(NumberOfPlates,1);
    NumberOfPlates=min(NumberOfPlates,57);
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  NumberOfPlates= "<< NumberOfPlates << endl;
	  
    
    histo_longprofile		   ->Reset();
    histo_deltaR					 ->Reset();
    histo_deltaT					 ->Reset(); 
    for (int id=0; id<57; id++) {
      theta[id]= 0;
      X0[id] = id*1300.*txb[0] +  xb[0];
      Y0[id] = id*1300.*tyb[0] +  yb[0];
      longprofile[id]=-1;
    }
    if (tagprimary[0]==1) {
      TX0 = txb[0];
      TY0 = tyb[0];
    }
    ++nentries_withisizeb;
		
    sizeb_for_plates=0;
    numberofilms=NumberOfPlates;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  numberofilms= " << numberofilms <<endl;
    
		//=C= -----------------------------------------------------
		//=C= loop over the basetracks in the shower (boucle sur les btk)
    for (Int_t ibtke = 0; ibtke < sizeb; ibtke++) {
      dist = sqrt((xb[ibtke]- X0[nfilmb[ibtke]-1])*(xb[ibtke]- X0[nfilmb[ibtke]-1])+(yb[ibtke]- Y0[nfilmb[ibtke]-1])*(yb[ibtke]- Y0[nfilmb[ibtke]-1]));			
			// inside the cone
      if (dist<Dr[nfilmb[ibtke]-1]&&nfilmb[ibtke]<=numberofilms) { 
        
        histo_longprofile					->Fill(nfilmb[ibtke]);
        histo_longprofile_av			->Fill(nfilmb[ibtke]);
        theta[nfilmb[ibtke]]+= (TX0-txb[ibtke])*(TX0-txb[ibtke])+(TY0-tyb[ibtke])*(TY0-tyb[ibtke]);
				
        if (nfilmb[ibtke]<=numberofilms) {++sizeb_for_plates;}
        if (ibtke>0&&nfilmb[ibtke]<=numberofilms) {
          histo_deltaR						->Fill(deltarb[ibtke]);
          histo_deltaT						->Fill(deltathetab[ibtke]);
        }
      }
    }//=C= END OF loop over the basetracks in the shower
		//=C= -----------------------------------------------------
		
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  ige sizeb_for_plates sizeb" << ige << "  " << sizeb_for_plates <<"   " <<  sizeb << endl;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  histo_deltaR->GetMean() histo_deltaT->GetMean()" << histo_deltaR->GetMean() << "  " << histo_deltaT->GetMean() <<"   " << endl;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  histo_deltaR->GetRMS() histo_deltaT->GetRMS()" << histo_deltaR->GetRMS() << "  " << histo_deltaT->GetRMS() <<"   " << endl;
    
		//=C= Fill NumberBT Hist----Just temporary....for all showers:
    histo_nbtk										->Fill(sizeb_for_plates);
    histo_nbtk_av									->Fill(sizeb_for_plates);
		//=C= Fill dR,dT Mean and RMS Histos for all showers:
    histo_deltaR_mean							->Fill(histo_deltaR->GetMean());
    histo_deltaT_mean							->Fill(histo_deltaT->GetMean());
    histo_deltaR_rms							->Fill(histo_deltaR->GetRMS());
    histo_deltaT_rms							->Fill(histo_deltaT->GetRMS());
    
		//=C= -----------------------------------------------------
		// "Variable" 0 describes the (MonteCarlo) energy of the particle
		// energy_shot_particle;  (if there)
		// Variable 1 corresponds to number of Basetracks in the Shower.
		// sizeb;
		// Variable 2,3,4,5 corresponds to dR, mean,rms; dT, mean,rms; 
    BT_deltaR_mean=histo_deltaR->GetMean();
    BT_deltaR_rms=histo_deltaR->GetRMS();
    BT_deltaT_mean=histo_deltaT->GetMean();
    BT_deltaT_rms=histo_deltaT->GetRMS();		
		// Variables longprofile[...] corresponds to BinEntries Of the longitudialProfile (averaged):
		// Take Care!! BinContent(0) is the UnderflowBin! so it has to go from 0 (plate1) to nbfilm+1 (f.e.plate11->nbfilm+1=12) !
    for (Int_t i=1; i<=numberofilms+1; ++i) {
      longprofile[i-1] = int(histo_longprofile->GetBinContent(i));		
    }
    //=C= ----------------------------------------------------- 
    
    
    //=C= -----------------------------------------------------
    //=C= --- Here we differentiate which Platetype we have.
    //=C= --- Can be for each Shower different!
    Int_t type=0;
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  NumberOfPlates    " <<  NumberOfPlates << endl;
    if (NumberOfPlates<16) {type=0;}
    if (NumberOfPlates>=16 && NumberOfPlates<21) {type=1;}
    if (NumberOfPlates>=21 && NumberOfPlates<26) {type=2;}
    if (NumberOfPlates>=26 && NumberOfPlates<31) {type=3;}
    if (NumberOfPlates>=31 && NumberOfPlates<36) {type=4;}
    if (NumberOfPlates>=36 && NumberOfPlates<41) {type=5;}
    if (NumberOfPlates>=41 && NumberOfPlates<46) {type=6;}
    if (NumberOfPlates>=46) {type=7;}
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  Type   " << type << endl;
    //=C= -----------------------------------------------------
        
    
    //=C= -----------------------------------------------------
		//=C= Fill the ANN Inputvariables 
		//=C= For the specific Sample
		//=C= -----------------------------------------------------
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  " <<  ige;
    if (gEDBDEBUGLEVEL>2) cout << "     type=  " << type << endl;
		// Reset All  inANN  Values to Zero:  
    for (Int_t j=0; j<70; j++) { inANN[j]=0; }
		
		// Variable 0 corresponds to the "type" i.e. to the Energy of the Shower.
    inANN[0]= EnergyMC;
		// Variable 1 corresponds to number of Basetracks in the Shower.
    inANN[1]= sizeb_for_plates;
		// Variable 2,3,4,5 corresponds to dR, mean,rms; dT, mean,rms; 
    inANN[2]= BT_deltaR_mean;
    inANN[3]= BT_deltaR_rms;
    inANN[4]= BT_deltaT_mean;
    inANN[5]= BT_deltaT_rms;
		// Variable 6... (end) to the longitudinale profile...
    for (Int_t j=0; j<numberofilms; j++) inANN[6+j]=longprofile[j+1];
    //=C= -----------------------------------------------------
    
    
    //---------------Parameters as   param array suited for ANN to evaluate: ---------------
    // Reset All Values to Zero:  
    for (Int_t j=0; j<70; j++) { params[j]=0; }
    //=C=	NeuralNetworkLayout: check to which layout this specific shower belongs:
    for (Int_t j=1; j<=NrOfANNInputNeurons[type]; ++j) {
      params[j-1]=inANN[j];
      if (gEDBDEBUGLEVEL>3) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  j  params[j-1]=inANN[j]   " << j<< "  " << params[j-1] << "  " << inANN[j] << endl; ///DEBUG OPTION
    }
    
    
    //--- Main Core Part: Evaluate function of Neural Network, specific to the
    //---                 ANN which is usefull for the shower length!
    //------------------------------------------
    TMultiLayerPerceptron* mlp= (TMultiLayerPerceptron*)eEnergyANN->At(type);
    val=mlp->Evaluate(0,params);
    val_corr=Energy_CorrectEnergy(val, type);
    //------------------------------------------
    sigma_val=Energy_CalcSigma(val, type);
    sigma_val_corr=Energy_CalcSigma(val_corr, type);
    //------------------------------------------
    
    //------------------------------------------ 
    EnergyCorrectedb=val_corr;
    EnergyUnCorrectedb=val;
    EnergySigmaCorrectedb=sigma_val_corr;
    EnergySigmaUnCorrectedb=sigma_val;
    EnergyMC=E_MC;
    //------------------------------------------
    
    if (gEDBDEBUGLEVEL>3) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() === " << mlp->GetStructure()<<endl; ///DEBUG OPTION
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  EnergyUnCorrectedb, EnergyUnCorrectedb,EnergyMC:    " <<  EnergyUnCorrectedb << "    ==  " << EnergyCorrectedb << "      " <<  EnergyMC << endl; ///DEBUG OPTION
    if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_ExtractShowerParametrisationProfile() ===  EnergySigmaCorrectedb, EnergySigmaUnCorrectedb:    " <<  EnergySigmaUnCorrectedb << "    ==  " << EnergySigmaCorrectedb << "      " << endl; ///DEBUG OPTION   
    
    
    eHistEnergyUnCorrectedb->Fill(val);
    eHistEnergyCorrectedb->Fill(val_corr);
    eHistEnergyNominal->Fill( inANN[0]/1000. );
    
    ShowerParametrisationTree->Fill();
    treesaveb3->Fill();
    //--------------------------------------------------------------------
  }  //=C= END of Loop over reconstructed Showers (Entries):
  cout << "Loop over reconstructed Showers finished."<< endl;
  
  
  // Scale histo_longprofile_av to number of entries:
  histo_longprofile_av->Scale(1.0/nentries_withisizeb);
	
	// Write to File:
  fileout3->cd();
  treesaveb3->Write();
  fileout3->Close();         
  
  eEnergyIsDone=kTRUE;
  return;
}




//-------------------------------------------------------------------
Float_t EdbShowerRec::Energy_CorrectEnergy(Float_t MeasuredEnergy, Int_t NPlatesNr)
{
	//=C= -----------------------------------------------------
	//=C= Correct the given ANN  Energy linear according
	//=C= to the formula
	//=C= E_corr = 1/p1 * ( E_meas -p0 )
	//=C= p1,p0 are different for the plate numbers.
	//=C= -----------------------------------------------------
  Float_t p0[8];
  Float_t p1[8];
  Float_t CorrEnergy;
	// New Values
  p0[0]=1.01; p1[0]=2.83;  // 11 Plates
  p0[1]=1.07; p1[1]=1.48;  // 16 Plates
  p0[2]=0.97; p1[2]=1.03;  // 21 Plates
  p0[3]=0.98; p1[3]=0.89;  // 26 Plates
  p0[4]=0.92; p1[4]=1.05;  // 31 Plates
  p0[5]=0.90; p1[5]=1.03;  // 36 Plates
  p0[6]=0.88; p1[6]=1.07;  // 41 Plates
  p0[7]=0.87; p1[7]=1.10;  // 46 Plates
  if (gEDBDEBUGLEVEL>2) cout << "void EdbShowerRec::Energy_CorrectEnergy()                   === NPlatesNr	p0[NPlatesNr] " << NPlatesNr << "  " << p0[NPlatesNr] << endl;
  CorrEnergy= 1.0/p1[NPlatesNr] * ( MeasuredEnergy-p0[NPlatesNr] );
	
  return CorrEnergy;
}





//-------------------------------------------------------------------
void EdbShowerRec::Energy_CreateEnergyResolutionFitFunction()
{
  eEnergyResolutionFitFunction_All = new TF1("EnergyResolutionFitFunction_All","TMath::Sqrt([0]*[0]/sqrt(x)/sqrt(x)+[2]*[2]+[1]*[1]/x/x)",0.9,48.0);
  eEnergyResolutionFitFunction_All->SetParameter(0,0.5);	//Initialize with A=50%
  eEnergyResolutionFitFunction_All->SetParameter(1,0.2);	//Initialize with B=20%
  eEnergyResolutionFitFunction_All->SetParameter(2,0.1);	//Initialize with C=10%
  eEnergyResolutionFitFunction_All->SetParName(0,"ResolutionFactor A");
  eEnergyResolutionFitFunction_All->SetParName(1,"ResolutionFactor B");
  eEnergyResolutionFitFunction_All->SetParName(2,"ResolutionFactor C");
  eEnergyResolutionFitFunction_All->SetLineColor(2);	
  eEnergyResolutionFitFunction_All->SetParLimits(0,0,2);
  eEnergyResolutionFitFunction_All->SetParLimits(1,0,2);
  eEnergyResolutionFitFunction_All->SetParLimits(2,0,2);
  
  return;
}


//-------------------------------------------------------------------
Float_t EdbShowerRec::Energy_CalcSigma(Float_t Energy, Int_t type)
{
	//=C= -----------------------------------------------------
	//=C= -----------------------------------------------------
	// To understand where these values come from, please look in 
	// the note of energy measurement by FWM. to be found on the opera doc server.
  Float_t SigmaParametrizationArray[8][3]={ { 1.27,0.0,0.28}, {0.79,0.0,0.26}, {0.48,0.0,0.23}, {0.50,0.0,0.15} ,{0.49,0.0,0.10} ,{0.50,0.17,0.04}, {0.49,0.24,0.00},{0.45,0.37,0.03} };
		
  eEnergyResolutionFitFunction_All->SetParameter(0,SigmaParametrizationArray[type][0]);
  eEnergyResolutionFitFunction_All->SetParameter(1,SigmaParametrizationArray[type][1]);
  eEnergyResolutionFitFunction_All->SetParameter(2,SigmaParametrizationArray[type][2]);
  
  Float_t CalcSigma = eEnergyResolutionFitFunction_All->Eval(Energy);
  return CalcSigma;
}




//-------------------------------------------------------------------
void EdbShowerRec::Energy_CreateANNTree()
{
  if (gEDBDEBUGLEVEL>2) cout  << "------------------------------------------"<<endl;
  if (gEDBDEBUGLEVEL>2) cout  << "void EdbShowerRec::Energy_CreateANNTree()"<<endl;
  if (gEDBDEBUGLEVEL>2) cout  << "------------------------------------------"<<endl;
  
	//=C= Load MLP Root Class:
// 	if (!gROOT->GetClass("TMultiLayerPerceptron")) {
// 		gSystem->Load("libMLP");
// 	}
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Energy_CreateANNTree() === Load(libMLP) done." << endl;
	
	//=C= -----------------------------------------------------
	//=C= 
	//=C= -----------------------------------------------------
	//=C= Explanation: ReconstructedPlatesArray[ReconstructedPlatesNr]: 
  //=C= for BTs per each EmulsionNr (11,16,21..56)
	//=C= Explanation: 4: dR, dT: mean and rms.
	//=C= Explanation: 1: number of Basetracks
	//=C= Explanation: [0]: type=energy of the Shower
	//=C= For the FJ Profile we have 1+4+(11,16,21=NumberOfPlates)... InputNeurons:
  //=C= -----------------------------------------------------
  //=C= We Create different instances of ANNs for different shower lengths:
  //=C= -----------------------------------------------------
/// DEBUG // 	inANN = new Double_t[70];
  ANNInterimTreeInputvariables = new TTree("ANNInterimTreeInputvariables","ANNInterimTreeInputvariables");
  ANNInterimTreeInputvariables  ->  Branch("inANN", inANN, "inANN[70]/D");
	//=C= -----------------------------------------------------
	
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Energy_CreateANNTree() === CreateANNTree()  is done."<<endl;
  
  
  Int_t	ReconstructedPlatesArray[]  ={11,16,21,26,31,36,41,46};
  Int_t NrOfANNInputNeurons[]       ={11,16,21,26,31,36,41,46};
  eEnergyANN = new TObjArray(9);
  
  //=C= -----------------------------------------------------
  //=C=	This ANN Layout has the following form:
  //=C=	inputNeurons 						: 		hiddenLayer1 					: 	hiddenLayer2 					:		type
  //=C=	inANN[1],...,inANN[x]		: 		NrOfANNInputNeurons+1 :  	NrOfANNInputNeurons		:		inANN[0]
  //=C= -----------------------------------------------------

  for (Int_t i=0; i<8; ++i) {
    NrOfANNInputNeurons[i]=NrOfANNInputNeurons[i]+1+4;
    TString layout="";
    //=C= -----------------------------------------------------
    for (Int_t j=1; j<NrOfANNInputNeurons[i]; ++j) layout += "@inANN["+TString(Form("%d",j))+"],";
    layout += "@inANN["+TString(Form("%d",NrOfANNInputNeurons[i]))+"]:"+TString(Form("%d",NrOfANNInputNeurons[i]+1))+":"+TString(Form("%d",NrOfANNInputNeurons[i]));
    layout+=":inANN[0]/1000"; //if (Geant4) ([E]=MeV) //else layout+=":inANN[0]"; (Geant3) ([E]=GeV)
    //=C= -----------------------------------------------------
//     eEnergyANN[i]	
    TMultiLayerPerceptron* eEANN = 	new TMultiLayerPerceptron(layout,ANNInterimTreeInputvariables,"","(Entry$)%2");
    
    //=C= -----------------------------------------------------
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Energy_CreateANNTree() === ANN_MLP->GetStructure() : " << eEANN->GetStructure() << endl ;
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Energy_CreateANNTree() === LoadANNWeightFile():"<<endl;
    TString weightfilestring=TString(Form("EnergyMeasurement_WeightFile_%d_Plates.txt",ReconstructedPlatesArray[i]));
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Energy_CreateANNTree() === weightfilestring=  " << weightfilestring << endl;

    sprintf(cmd,"ln -fs  $FEDRA_ROOT/src/libShower/EnergyMeasurement_WeightFile_%d_Plates.txt .",ReconstructedPlatesArray[i]);
    //    strcpy(cmd,"ln -fs  $FEDRA_ROOT/src/libShower/weights_15.txt .");
    gSystem->Exec(cmd);

    eEANN->LoadWeights(weightfilestring);
    eEnergyANN->Add(eEANN);
  }
	//=C= -----------------------------------------------------
  
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowerRec::Energy_CreateANNTree() === is done."<<endl;
  return;
}
