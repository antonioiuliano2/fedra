//---------------------------------------------------------------------------------------------------
//                 Algorithm for Electromagnetic shower reconstruction, 
//                         identification  and energy measurement
//                                  (june 2008)
//---------------------------------------------------------------------------------------------------
// FIRST: - Fill properly "namespace SHOWER_PAR"
//        - Comment line 48 if you have already a linked_tracks.root 
//---------------------------------------------------------------------------------------------------
// OUTPUT: - shower tree in Shower.root
//         - contains the reconstructed showers with the electron probability and the energy estimation 
//---------------------------------------------------------------------------------------------------
#include "TMultiLayerPerceptron.h"

namespace SHOWER_PAR
{
    int DOWN = 1; // this has to be put to 0 if you order the plates in upstream direction 

    int PLATE = 22;               // plate number of the first plate in your data directory (lowest Z)
    int MAXPLATE =56;           // total number of plates in your data directory 

    int piece2= 1;               // which cp to use (02_00%d.cp.root) for the next BT 
    int piece2par= 1;            // which .par to use (02_00%d.par) for the next BT 

    int DATA = 0;                // REAL DATA OPTION - DATA = 1 for real data DATA = 0 for MC
                                 // to use the affine trans. (real data) or not (MC) 
}
void  shower_btr()
{    
  using namespace  SHOWER_PAR;
  
  EdbShowerRec  shower;
  
  char fname[128];     
  char nme[64];
  float  a11, a12, a21, a22, bb1, bb2;
  float Zoff;
  
  double x0[10000];
  double y0[10000];
  double z0[10000];
  double tx0[10000];
  double ty0[10000];
  int   MCid[10000];
  int   TRid[10000];
  double Esim[10000];
  int   id[10000];
  int   Pid[10000];  
  int Ncand=0;
  int trid(0), nseg, n0;
  
  //SELECTION of the FIRST BASETRACK FOR THE SHOWER RECONSTRUCTION   
  // 
  // Here as an exemple we select BT from the first plate of the brick "data/01_001.cp.root"    
  //
  // this part can be modified as you want
  
  sprintf(fname,"data/01_001.cp.root");
  
  file1 = new TFile(fname ,"READ");
  
  sprintf(nme,"par/01_001.par");    // READ AFFINE TRANSFORMATION    
  shower.ReadAffAndZ(nme,&Zoff,&a11,&a12,&a21,&a22,&bb1,&bb2 );

// Get the values for shower reconstruction from cp file   
    TTree *tree = (TTree*)file1->Get("couples");       
    EdbSegP *s  = 0;
    EdbSegCouple *cp;
    
    s = new EdbSegP();
    tree->SetBranchAddress("s.", &s);
    tree->SetBranchAddress("cp", &cp);
    int nentries = int(tree->GetEntries());

    for(int i=0;i<nentries;i++){
      tree->GetEntry(i);
      //================================================================
      // CUTS for the first basetrack selection to fill the x0, y0, z0, tx0, ty0
      // here you can add the cuts you want  	
      //	if((fabs(s->TX()-0.15)>0.02&&fabs(s->TY()-0.15)>0.02)) continue;
      //  if((fabs(s->TX())>0.05&&fabs(s->TY())>0.05)) continue;
      if (s->MCEvt()!=1)continue;	
      //if (s->MCEvt()==-999)continue;
      //if((fabs(s->TX())>0.1||fabs(s->TY())>0.1)) continue;
      //if(s->W()<25) continue;
      //if(s->Z()<24000) continue;
      //filling
      
      if (DATA==1)
	{  
	  x0[Ncand]  =  a11*s->X() + a12*s->Y() + bb1;
	  y0[Ncand]  =  a21*s->X() + a22*s->Y() + bb2;
	  z0[Ncand]  =  Zoff;
	  tx0[Ncand] =  a11*s->TX()+ a12*s->TY() ;
	  ty0[Ncand] =  a21*s->TX()+ a22*s->TY();
	  TRid[Ncand] = s->MCEvt();  //this has to be a positive number
	  Esim[Ncand] = s->P();
	  id[Ncand] = s->ID();
	  Pid[Ncand] =    Zoff/1290+1; 
	  //cout <<s->X()<<" "<<s->Y()<<" "<<Zoff<<" "<<s->TX()<<" "<<s->TY()<<" "<<s->W()<<" "<<Ncand <<endl;
	}
      else
	{	  
	  x0[Ncand]  =  s->X();
	  y0[Ncand]  =  s->Y();
	  z0[Ncand]  =  Zoff;
	  tx0[Ncand] =  s->TX();
	  ty0[Ncand] =  s->TY();
	  TRid[Ncand] = s->MCEvt();//this has to be a positive number
	  Esim[Ncand] = s->P();
	  id[Ncand] = s->ID();
	  Pid[Ncand] =    Zoff/1290+1; 
	  //cout <<s->X()<<" "<<s->Y()<<" "<<Zoff<<" "<<s->TX()<<" "<<s->TY()<<" "<<s->W()<<" "<<Ncand <<endl;
	}
      Ncand++;      
    }
    printf("Ncand: %f\n",Ncand);
    file1->Close();
    
    // Here call of the shower algorithm using x0,y0,z0,tx0,ty0,Pid,id,TRid,Esim as seed 
    // The results will be put in Shower.root file
   
    shower.rec(PLATE,MAXPLATE,0,Ncand,x0,y0,z0,tx0,ty0,Pid,id,TRid,Esim,piece2,piece2par,DOWN);   
   
}
