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
  
  int PLATE = 7;               // plate number of the first plate in your data directory (lowest Z)
  int MAXPLATE =56;           // total number of plates in your data directory 
  
  int piece2= 1;               // which cp to use (02_00%d.cp.root) for the next BT 
  int piece2par= 1;            // which .par to use (02_00%d.par) for the next BT 
  
  const char *def = "pi0.def";  // is needed if you want to generate the linked_tracks.root file 
  int event = 1;                // only used for MC data  
}

void link()
{
  using namespace  SHOWER_PAR;
  
  // this function read basetracks for a given simulated event and create
  // the linked_track.root file
  char cmd[256];
  /*    
  FILE *f = fopen("default.par","w");
  fprintf(f,"INCLUDE opera_emulsion.par\n");
  fprintf(f,"RCUT 0 abs(s.eTX)<1.\&\&abs(s.eTY)<1.\&\&(s.eMCEvt==%d\|\|s.eMCEvt<0)\n",event);
  fclose(f);
  */ 
  sprintf(cmd,"recset -t -p1. %s",def);
  gSystem->Exec(cmd);  
}

void  shower_tr()
{  
  using namespace  SHOWER_PAR;
  
  //link();
  
  EdbShowerRec  shower;  
  char fname[128];     
  char nme[64];
  float  a11, a12, a21, a22, bb1, bb2;
  float Zoff;
  int Zbrick [57];
  double x0[10000];
  double y0[10000];
  double z0[10000];
  double tx0[10000];
  double ty0[10000];
  int   MCid[10000];
  int   TRid[10000];
  int   id[10000];
  int   Pid[10000];
  double Esim[10000];
  int Ncand=0;
  int trid(0), nseg, n0;
  
  //SELECTION of the FIRST BASETRACK of each reconstructed tracks in  linked_tracks.root
  // 
  sprintf(fname,"linked_tracks.root");
  file1 = new TFile(fname ,"READ");
  
  EdbSegP *tr = new EdbSegP();
  EdbSegP *s = new EdbSegP();
  TClonesArray *as = 0; 
  
  TTree *tree= (TTree*)file1->Get("tracks");
  
  tree->SetBranchAddress("trid",&trid);
  tree->SetBranchAddress("nseg",&nseg);
  tree->SetBranchAddress("n0",&n0);
  
  b1 = tree->GetBranch("t.");
  b1->SetAddress(&tr);
  
  b2 = tree->GetBranch("s");
  b2->SetAddress(&as);
  
  //if (tr->TX()<0.2) continue;
  //if (tr->TY()<0.2) continue;
  //if (nseg<3) continue;  

  int nentries = int(tree->GetEntries());
  
  for(int i=0;i<nentries;i++){
    tree->GetEntry(i);
    
    int k = as->GetEntries();
    s=(EdbSegP*)(as->At(0));
    
    //filling

    x0[Ncand]  =  s->X();
    y0[Ncand]  =  s->Y();
    z0[Ncand]  = s->Z() ;
    tx0[Ncand] =  s->TX();
    ty0[Ncand] =  s->TY();
    TRid[Ncand] = s->MCEvt();//this has to be a positive number
    Esim[Ncand] = s->P();
    id[Ncand] = s->ID();
    Pid[Ncand] =    s->Z()/1290+1;    
    //cout << s->X() << " " << s->Y() << " " << s->Z() << " " << s->TX() << " "  << s->TY() << " " <<  Pid[Ncand]  << " " <<nseg <<endl;    
    Ncand++;      
  }
  
  printf("Ncand: %f\n",Ncand);    
  
  file1->Close();

  // Here call of the shower algorithm using x0,y0,z0,tx0,ty0,Pid,id,TRid,Esim as seed 
  // The results will be put in Shower.root file
  
  shower.rec(PLATE,MAXPLATE,0,Ncand,x0,y0,z0,tx0,ty0,Pid,id,TRid,Esim,piece2,piece2par,DOWN);

  gROOT->ProcessLine(".q");
}
