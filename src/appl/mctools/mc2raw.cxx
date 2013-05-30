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
  printf(" Note: you can use modifiers like \'k\' and \'m\', they will be substituted with \'000\' and \'000000\' correspondingly\n");
  printf("---------------------------------------------------\n");
  printf(" Usage example:\n mc2raw -a -n0=2m -n=200k 90001 /data/sim5m.root\n");
  printf(" will read file \"/data/sim5m.root\", take events [2000000,2200000] and and APPEND them to brick directory \"b090001\"\n");
  printf("---------------------------------------------------\n");
}
///====================================================================
long NEV0=0,NEV1=0;
int BRICK_ID=0;
const char* fname=0;
const char* opt="RECREATE";
const char* envfname="mceff.rootrc";
//TEnv EffEnv("mceff");
//--------------------------------------------------------
void set_default(TEnv& cenv){
  cenv.SetValue("mceff.EffAlg","pol0");
  cenv.SetValue("mceff.EffPar","0.98 0.98 0.98 0.98");
  cenv.SetValue("mceff.TanMax","1.0");
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
    if(strncmp(argv[n],"-env=",5)==0){
      envfname=argv[n]+5;
      continue;
      }
    if(strncmp(argv[n],"-a",2)==0){
      opt="UPDATE";
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

int main(int argc, char** argv){
  /// Init arguments and environment
  read_args(argc,argv);
  TEnv cenv("mceff_env");
  set_default(cenv);
  cenv.ReadFile(envfname,kEnvAll);
  cenv.WriteFile("mceff.save.rootrc");
  /// Set efficiency function
  TF1* mtk_eff=InitEfficiency(cenv);
  /// Set smearing conditions
  EdbScanCond* mtk_smear=InitSmearing(cenv);

  
  TFile* F=new TFile(fname,"READ");
  
  // setup EdbDataStore object
  EdbDataStore DS;
  DS.eBrick=(EdbBrickP*)F->Get("Brick");
  DS.Restore_PatFromGeom();
  TTree* tree=(TTree*)F->Get("FluSim");
  TObjArray* vtx=new TObjArray();
  printf("tree has %lld entries\n",tree->GetEntries());
  tree->GetBranch("Vtx")->SetAddress(&vtx);
  ///finished prepare
  if(NEV1>tree->GetEntries())NEV1=tree->GetEntries();
  for(int nEv=NEV0; nEv<NEV1; ++nEv){  
    tree->GetEntry(nEv);
  //Or better like this
  DS.LoadMCVertices(vtx);
  DS.Restore_TrxFromVtx();
  DS.Restore_SegFromTrx(0,0,100);
    
//     DS.PrintBrief();
    if(nEv%10000==0){
      printf("Evt#%d of %lld\n",nEv,tree->GetEntries());
      //DS.PrintPatterns();
    }
    //DumpPatterns(&DS);
    //DS.FillErrorsCOV();
    DS.ClearTracks();
    DS.ClearVTX();
    DS.ClearSeg();
  }
  DS.DoEfficiency(0,mtk_eff);
  DS.DoSmearing(0,mtk_smear);
  DS.SaveToRaw(".",BRICK_ID,opt);
  return 0;
}
