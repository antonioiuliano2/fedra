///==================================================================
///
///   mc2raw: convert data to raw.root format read data from EdbDataStore file (root file with array of vertices) 
///   
///
///==================================================================
#include <TROOT.h> 
#include <TObjArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TEnv.h>
#include <TF1.h>
#include <TRandom3.h>

#include <EdbVertex.h> 
#include <EdbDataStore.h> 
#include <EdbPattern.h> 
#include <EdbLog.h> 
//--------------------------------------------------------
void Help(){
  printf(" -------------------------------------------------\n");
  printf(" usage:\n\t mc2raw [options] BrickID input.root\n");
  printf(" Convert simulation data from file [input.root] to FEDRA brick structure b0[BrickID].\n");
  printf(" Options: \n") ;
  printf(" \t-h           \t - display this help message");
  printf(" \t-n0=NEV_START\t - starting event number. Default=0\n");
  printf(" \t-n=NEV_TODO  \t - number of events to process. Default=0\n");
  printf(" \t-v=VLEVEL    \t - set Fedra verbosity level. Default=0\n");
  printf(" \t-a           \t - Add to existing brick. I.e. set file access mode to \'APPEND\'. Default=\'RECREATE\'\n");
  printf(" \t-env=ENV_FILE\t - set ENV file to read. Default=\'mceff.rootrc\'\n");
  printf(" \t-noaff       \t - Do not modify AFF files\n");
  printf(" Note: you can use modifiers like \'k\' and \'m\', they will be substituted with \'000\' and \'000000\' correspondingly\n");
  printf("---------------------------------------------------\n");
  printf(" Usage example:\n mc2raw -a -n0=2m -n=200k 90001 /data/sim5m.root\n");
  printf(" will read file \"/data/sim5m.root\", take events [2000000,2200000] and and APPEND them to brick directory \"b090001\"\n");
  printf("---------------------------------------------------\n");
}
///====================================================================
long NEV0=0,NEV1=0,NBG=0;
int BRICK_ID=0;
const char* fname=0;
const char* opt="RECREATE";
const char* envfname="mceff.rootrc";
bool noaff=false;

const char* bg_fnm=0;
char* bg_hnm1="bg_TXTY";
char* bg_hnm2="bg_WT";
TH2F *bg_Ang=0, *bg_Puls=0;
bool bg_BaseTrk=false;  ///if true - generate BG basetracks (i.e. corellated microtracks). Ottherwise - ucorellated.

//TEnv EffEnv("mceff");
//--------------------------------------------------------
void set_default(TEnv& cenv){
  cenv.SetValue("addbg.FileName", "BG_mtk_Strom.root");
  cenv.SetValue("addbg.HistName", "bg_TXTY bg_WT");
  cenv.SetValue("addbg.BaseTrk", 0);
  cenv.SetValue("mceff.ApplyEff", 0);
  cenv.SetValue("mceff.EffAlg","pol0");
  cenv.SetValue("mceff.EffPar","0.98 0.98 0.98 0.98");
  cenv.SetValue("mceff.TanMax",1.0);
  cenv.SetValue("mceff.ApplySmearing", 0);
  cenv.SetValue("fedra.smear.Sigma0", "1 1 0.013 0.013");
  cenv.SetValue("fedra.smear.PulsRamp0","6 6");
  cenv.SetValue("fedra.smear.PulsRamp04","6 6");
  cenv.SetValue("fedra.smear.Degrad", 5);
}

//--------------------------------------------------------
int readNum(char* st){
  ///read number with letters: k=10^3; m=10^6
  TString res="";
  int len=strlen(st);
  for(int n=0;n<len;++n){
    if(st[n]>='0' && st[n]<='0'+9){res+=st[n]; continue;}
    switch(st[n]){
      case 'k':
      case 'K': res+="000"; break;
      case 'm':
      case 'M': res+="000000"; break;
    }
  }
  printf("%s ---> %s\n",st,res.Data());
  return atoi(res.Data());
}
//--------------------------------------------------------
void read_args(int argc, char** argv){
  gEDBDEBUGLEVEL=0;
  if(argc<3){Help(); exit(0);}
  for(int n=1;n<argc-2;++n){
    if(strncmp(argv[n],"-h",2)==0){
      Help(); exit(0);
    }
    if(strncmp(argv[n],"-n0=",4)==0){
      NEV0=readNum(argv[n]+4);
      continue;
      }
    if(strncmp(argv[n],"-n=",3)==0){
      NEV1=NEV0+readNum(argv[n]+3);
      continue;
      }
    if(strncmp(argv[n],"-nB=",4)==0){
      NBG=readNum(argv[n]+4);
      continue;
      }
    if(strncmp(argv[n],"-env=",5)==0){
      envfname=argv[n]+5;
      continue;
      }
    if(strncmp(argv[n],"-a",2)==0){
      opt="UPDATE";
      continue;
      }
    if(strncmp(argv[n],"-noaff",6)==0){
      noaff=true;
      continue;
      }
    if(strncmp(argv[n],"-v=",3)==0){
      gEDBDEBUGLEVEL=atoi(argv[n]+3);
      continue;
      }
  }
  BRICK_ID=atoi(argv[argc-2]);
  fname=argv[argc-1];
  
  printf("Fname=\"%s\"\n BrickID=%06d\n,Taking events in range [%ld, %ld].\n",fname,BRICK_ID,NEV0,NEV1);
}

//--------------------------------------------------------
//--------------------------------------------------------
TF1* InitEfficiency(TEnv &cenv){
  cenv.Print();
  int ApplyEff=cenv.GetValue("mceff.ApplyEff",1);
  if(!ApplyEff)return 0;
  const char* EffAlg=cenv.GetValue("mceff.EffAlg","pol0");
  const char* EffPar=cenv.GetValue("mceff.EffPar","1.00");
  float tanmax=cenv.GetValue("mceff.TanMax",1.0);
  printf("--- initializing efficiency algorithm \'%s\'\n ---",EffAlg);

  TF1* eff=new TF1("mc_eff",Form("%s(0)",EffAlg),0,tanmax);
  eff->SetTitle("Microtrack efficiency; tan#theta; survival probability");
  int n=0;
  const char* nxt=EffPar;
  float par=0;
  do{
    //printf("next line(%d)=\'%s\'\n",n,nxt);
    par=atof(nxt);
    nxt=strchr(nxt+1,' ');
    eff->SetParameter(n,par);
    printf("Par#%d = %2.4f\n",n,par);
    n++;
  }while(nxt); 
  printf(" -------------done--------------\n");
///-----------plot probability-------------  
  TCanvas c;
  eff->Draw("l");
  eff->SetMaximum(1.);
  eff->SetMinimum(0.);
  c.SetGrid();
  c.Print("eff.png");
  return eff;
}

//--------------------------------------------------------
EdbScanCond* InitSmearing(TEnv &cenv){
   int ApplySmear=cenv.GetValue("mceff.ApplySmearing",1);
   if(ApplySmear==0)return 0;
   EdbScanCond* smear=new EdbScanCond;
   float s0[4];
   float p0[2], p04[2];
   float degrad;
   const char* line=cenv.GetValue("fedra.smear.Sigma0","1 1 0.13 0.13");
   sscanf(line,"%f %f %f %f",s0,s0+1,s0+2,s0+3);
   line=cenv.GetValue("fedra.smear.PulsRamp0","6 6");
   sscanf(line,"%f %f",p0,p0+1);
   line=cenv.GetValue("fedra.smear.PulsRamp04","6 6");
   sscanf(line,"%f %f",p04,p04+1);
   degrad=cenv.GetValue("fedra.smear.Degrad",5.0);
   
   smear->SetSigma0(s0[0],s0[1],s0[2],s0[3]);
   smear->SetPulsRamp0 (p0[0],  p0[1]);
   smear->SetPulsRamp04(p04[0],p04[1]);
   smear->SetDegrad(degrad);
   smear->Print();
   return smear;
}
//--------------------------------------------------------
void InitBGHist(TEnv &cenv){
   bg_fnm=cenv.GetValue("addbg.FileName",(const char*)0); 
   if(bg_fnm==0 || NBG==0)return; 
   const char* line=cenv.GetValue("addbg.HistName","bg_TXTY bg_WT"); 
   printf("line=%s\n",line); 
   char* h1=new char[10]; 
   char* h2=new char[10]; 
   sscanf(line,"%s %s",h1,h2); 
   bg_hnm1=h1; bg_hnm2=h2; 
   printf("hstnms=%s %s\n",bg_hnm1,bg_hnm2); 
   TFile f(bg_fnm,"READ"); 
   TH2F *h1p, *h2p; 
   f.GetObject(bg_hnm1,h1p); 
   f.GetObject(bg_hnm2,h2p);
   bg_BaseTrk=cenv.GetValue("addbg.BaseTrk",0); 
   gROOT->cd();
   bg_Ang =(TH2F*)gROOT->CloneObject(h1p);
   bg_Puls=(TH2F*)gROOT->CloneObject(h2p);
   f.Close();
   
   ///normalize histograms for simulation
   ///angular histogram is used for generation of TX:TY, so we need just to make its maximum=1
   float maxval=0,val;
   maxval=bg_Ang->GetBinContent(bg_Ang->GetMaximumBin());
   //printf("max_val Ang=%2.4f\n",maxval);
   bg_Ang->Scale(1./maxval);
   
   for(int nbx=0; nbx< bg_Puls->GetNbinsX();++nbx){
     maxval=0;
     for(int nby=0; nby< bg_Puls->GetNbinsY();++nby){
       val=bg_Puls->GetBinContent(nbx,nby);
       if(val>maxval)maxval=val;
       }
     //printf("Xbin#%d(%2.4f) maxval = %2.5f\n",nbx,bg_Puls->GetXaxis()->GetBinCenter(nbx),maxval);
     if(maxval==0)continue;
     for(int nby=0; nby< bg_Puls->GetNbinsY();++nby){
       val=bg_Puls->GetBinContent(nbx,nby);
       bg_Puls->SetBinContent(nbx,nby,val/maxval);
       }
     }
   }
//--------------------------------------------------------
void WriteLog(const char* logfnm,TF1* efff,EdbScanCond* smr,EdbDataStore* DS){
  FILE* f=fopen(logfnm,(strcmp(opt,"UPDATE"))?"w":"a");
  time_t rawtime;
  struct tm * timeinfo;
  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  fprintf(f,"======= mc2raw run at ======\n %s\n",asctime(timeinfo));
  fprintf(f,"%s to b%06d\n",opt,BRICK_ID);
  if(NEV1>NEV0)
    fprintf(f," * READ INPUT : Read %ld events [%ld-%ld] from file \'%s\'\n",NEV1-NEV0,NEV0,NEV1,fname);
  if(NBG && bg_Ang && bg_Puls){
    fprintf(f," * GENERATE BG: %ld mtk/layer (%2.4f mtk/mm2). PDFs \'%s %s\' from file \'%s\'\n",NBG,NBG/(12.0e3),bg_hnm1,bg_hnm2,bg_fnm);
    fprintf(f,"Background: uncorellated %s\n",bg_BaseTrk?"BASE tracks":"Microtracks");
    fprintf(f," Angular distribution: %2.0f entries. [TX=%6.3f (RMS=%6.3f), TY=%6.3f (RMS=%6.3f)\n",
    bg_Ang->GetEntries(),bg_Ang->GetMean(1),bg_Ang->GetRMS(1),bg_Ang->GetMean(2),bg_Ang->GetRMS(2));
    fprintf(f," Pulse distribution: %2.0f entries. [Angle=%6.3f (RMS=%6.3f), Puls=%6.3f (RMS=%6.3f)\n",
    bg_Puls->GetEntries(),bg_Puls->GetMean(1),bg_Puls->GetRMS(1),bg_Puls->GetMean(2),bg_Puls->GetRMS(2));
  }
  if(efff){
    fprintf(f," * * Efficiency (%2.4f<Tan(theta)<%2.4f):\n", efff->GetXmin(),efff->GetXmax()); 
    const int N_pt=11;
    double x0=efff->GetXmin();
    double dx=(efff->GetXmax()-efff->GetXmin())/(N_pt-1);
    fprintf(f,"----------------");
    for(int n=0;n<N_pt;++n)fprintf(f,"---------");
    fprintf(f,"\n");
    fprintf(f,"Tan(theta):\t|");
    for(int n=0;n<N_pt;++n)fprintf(f," % 7.2f|",x0+n*dx);
    fprintf(f,"\n");
    fprintf(f,"Efficinency(%%):\t|");
    for(int n=0;n<N_pt;++n) fprintf(f," % 7.2f|",efff->Eval(x0+n*dx)*100.);
    fprintf(f,"\n");
    fprintf(f,"----------------");
    for(int n=0;n<N_pt;++n)fprintf(f,"---------");
    fprintf(f,"\n");
  }else fprintf(f," * * No inefficiency was applied\n");
  if(smr){
   fprintf(f," * * Smearing applied on microtrack level:\n");
   fprintf(f," Sigma0[TX,TY] = %7.4f %7.4f\t",smr->SigmaTX(0),smr->SigmaTY(0));
   fprintf(f," Sigma1[TX,TY] = %7.4f %7.4f\n",smr->SigmaTX(1),smr->SigmaTY(1));
   fprintf(f," Sigma0[X,Y] = %7.4f %7.4f\t",smr->SigmaX(0),smr->SigmaY(0));
   fprintf(f," Sigma1[X,Y] = %7.4f %7.4f\n",smr->SigmaX(1),smr->SigmaY(1));
  }else fprintf(f," * * No smearing was applied\n");
  fprintf(f," -------------Result (full eff)--------------------\n");
  for(int np=0; np< DS->eRawPV.Npatterns(); ++np){
    EdbPattern* pat=DS->GetRawPat(np);
    fprintf(f," - Plate#%d side%d: %d microtracks\n",pat->Plate(),pat->Side(),pat->N());
  }
  fprintf(f," =======================================\n");
  fclose(f);
/*     TCanvas c("c_bg","c_bg",800,400);
   c.Divide(2,1);
   c.cd(1);   bg_Ang->Draw("colz");
   c.cd(2);   bg_Puls->Draw("colz");
   printf("BG - uncorellated %s\n",bg_BaseTrk?"BASE tracks":"Microtracks");
   printf("%ld mtk/layer (%2.4f mtk/mm2). PDFs \'%s %s\' from file \'%s\'\n",NBG,NBG/(12.0e3),bg_hnm1,bg_hnm2,bg_fnm);
   c.Print("bg1.png");
    printf(" Angular distribution: %2.0f entries. [TX=%6.3f (RMS=%6.3f), TY=%6.3f (RMS=%6.3f)\n",
    bg_Ang->GetEntries(),bg_Ang->GetMean(1),bg_Ang->GetRMS(1),bg_Ang->GetMean(2),bg_Ang->GetRMS(2));
    printf(" Pulse distribution: %2.0f entries. [Angle=%6.3f (RMS=%6.3f), Puls=%6.3f (RMS=%6.3f)\n",
    bg_Puls->GetEntries(),bg_Puls->GetMean(1),bg_Puls->GetRMS(1),bg_Puls->GetMean(2),bg_Puls->GetRMS(2));*/
}

//--------------------------------------------------------
void GenerateBG(EdbDataStore *DS, bool correl=false){
  long Ntot=0;
  for(int Npl=0; Npl< DS->Nplt(); ++Npl){
      if(bg_BaseTrk) Ntot+=DS->Gen_mtk_BG(NBG,Npl,0,bg_Ang,bg_Puls);
      else{
        Ntot+=DS->Gen_mtk_BG(NBG,Npl,1,bg_Ang,bg_Puls);
        Ntot+=DS->Gen_mtk_BG(NBG,Npl,2,bg_Ang,bg_Puls);
        }
    }
    printf("Generated total of %ld bg segments\n",Ntot);
}
//--------------------------------------------------------

int main(int argc, char** argv){
  /// Init random
  if(gRandom==0)gRandom=new TRandom3(0);
  gRandom->SetSeed(0);
  printf("Random seed=%d\n",gRandom->GetSeed());
  /// Init arguments and environment
  read_args(argc,argv);
  TEnv cenv("mceff_env");
  set_default(cenv);
  cenv.ReadFile(envfname,kEnvAll);
  /// Set efficiency function
  TF1* mtk_eff=InitEfficiency(cenv);
  /// Set smearing conditions
  EdbScanCond* mtk_smear=InitSmearing(cenv);
  InitBGHist(cenv);
  
  TFile* F=new TFile(fname,"READ");
  
  // setup EdbDataStore object
  EdbDataStore DS;
  DS.eBrick=(EdbBrickP*)F->Get("Brick");
  DS.Restore_PatFromGeom();
  if(NBG>0){
    /// ====  generate background! ====
    GenerateBG(&DS);
  }
  if(NEV1>NEV0){
    /// ====   read segments from tree!   ====
    TTree* tree=(TTree*)F->Get("FluSim");
    TObjArray* vtx=new TObjArray();
    printf("tree has %lld entries\n",tree->GetEntries());
    tree->GetBranch("Vtx")->SetAddress(&vtx);
    cenv.WriteFile("mceff.save.rootrc");
    ///finished prepare
    if(NEV1>tree->GetEntries())NEV1=tree->GetEntries();
    for(int nEv=NEV0; nEv<NEV1; ++nEv){  
      tree->GetEntry(nEv);
      DS.LoadMCVertices(vtx);
      DS.Restore_TrxFromVtx();
      DS.Restore_SegFromTrx(0,0,100);
      if(nEv%10000==0){
         printf("Evt#%d of %lld\n",nEv,tree->GetEntries());
       }
       DS.ClearTracks();
       DS.ClearVTX();
       DS.ClearSeg();
    }
  }
  if(mtk_eff)DS.DoEfficiency(0,mtk_eff);
  if(mtk_smear)DS.DoSmearing(0,mtk_smear);
  DS.SaveToRaw(".",BRICK_ID,opt,!noaff);
  WriteLog(Form("./b%06d/b%06d.mc.log",BRICK_ID,BRICK_ID),mtk_eff,mtk_smear, &DS);
  cenv.WriteFile(Form("./b%06d/mceff.rootrc",BRICK_ID));
  return 0;
}
