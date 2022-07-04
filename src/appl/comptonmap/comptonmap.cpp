//-- Author :  Valeri Tioukov   1/09/2008

#include <string.h>
#include <iostream>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbPositionAlignment.h"
#include "EdbAlignmentMap.h"
#include "tlg2pattern.C"

using namespace std;
using namespace TMath;

void get_run_patterns(const char *runfile, TEnv *cenv, EdbPattern *ptop, EdbPattern *pbot);
//void get_patterns(EdbID id1, EdbID id2, TEnv *cenv, EdbPattern &p1, EdbPattern &p2);
void add_pattern(EdbPattern &p, EdbPattern &padd, 
				 float wmin, float thetamax, float xmin, float xmax, float ymin, float ymax, float maxDens=2);
void get_patterns_tlglist1(const char *listfile, TEnv *cenv, EdbPattern *ptop, EdbPattern *pbot, EdbPattern *pbase );
void get_patterns_Jap(const char *listfile, TEnv *cenv, EdbPattern &p);
void get_patterns_Jap2(const char *listfile, TEnv *cenv, EdbPattern &p);
void AlignMap(TEnv *cenv);
void AlignRunMap(TEnv *cenv);
void AlignMapJap(TEnv *cenv);
void LinkJap2(TEnv *cenv);
void ApplyMap(TEnv *cenv);
void ApplyTestMap(TEnv *cenv);

EdbScanProc   *sproc=0;

//--------------------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\nExample: \n";
  cout<< "\t  comptonmap -listA=tlg1.lst -listB=tlg2.lst -outmap=map.out -par=comptonmap.rootrc -v2 \n";
  cout<< "\t  comptonmap -Arun1.root -Brun2.root \n";
  cout<< "\nThe data location directory if not explicitly defined will be taken from comptonmap.rootrc as: \n";
  cout<< "\t  comptonmap.outdir:      /scratch/BRICKS \n";
  cout<< "\t  comptonmap.EdbDebugLevel:      1\n";
  cout<< "\nAdditional options: \n";
  cout<< "\t -apply: apply map to the basetracks\n";
  cout<< "\t -applytest: apply map to the same layers used for map production to test the result\n";
  cout<< "\t -link:  link raw data \n";
  cout<<endl;
}

//--------------------------------------------------------------------------------------------------
void set_default(TEnv &cenv)
{
  // default parameters for the comptom map search
  //tlglist case
  cenv.SetValue("comptonmap.MinGrainsTop" , 8.);
  cenv.SetValue("comptonmap.MinGrainsBot" , 8.);
  cenv.SetValue("comptonmap.MinGrainsBase",16);
  cenv.SetValue("comptonmap.ThetaMax",      1.);
  cenv.SetValue("comptonmap.Xmin",   -1000000.);
  cenv.SetValue("comptonmap.Xmax",    1000000.);
  cenv.SetValue("comptonmap.Ymin",   -1000000.);
  cenv.SetValue("comptonmap.Ymax",    1000000.);
  cenv.SetValue("comptonmap.DensityMax",    2.);
  
  //run case
  cenv.SetValue("comptonmap.x0"      ,  40000.);
  cenv.SetValue("comptonmap.y0"      ,  40000.);
  cenv.SetValue("comptonmap.size"    , 500000.);
  cenv.SetValue("comptonmap.pulsmin" ,      8 );
  cenv.SetValue("comptonmap.side1"   ,      1 );    // usually top    of the plate
  cenv.SetValue("comptonmap.side2"   ,      2 );    // usually bottom of the plate
}

//--------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }

  TEnv *cenv = new TEnv("comptonenv");
  set_default(*cenv);
  cenv->ReadFile("comptonmap.rootrc",kEnvLocal);

  bool      do_ida      = false;
  bool      do_idb      = false;
  //EdbID     idA,idB;

  bool      do_tlglista = false;
  bool      do_tlglistb = false;

  bool      do_align_jap = false;
  bool      do_align_jap2 = false;

  bool      do_apply_map = false;
  bool      do_apply_test_map = false;
  bool      do_link = false;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-A",2)) 
      {
	//	if(strlen(key)>2)	sscanf(key+2,"%d.%d.%d.%d", &idA.eBrick, &idA.ePlate, &idA.eMajor, &idA.eMinor );
	if(strlen(key)>2)	cenv->SetValue("comptonmap.runA",key+2);
	do_ida=true;
      }
    else if(!strncmp(key,"-B",2)) 
      {
	//	if(strlen(key)>2)	sscanf(key+2,"%d.%d.%d.%d", &idB.eBrick, &idB.ePlate, &idB.eMajor, &idB.eMinor );
	if(strlen(key)>2)	cenv->SetValue("comptonmap.runB",key+2);
	do_idb=true;
      }
    else if(!strncmp(key,"-par=",5)) 
      {
	cenv->ReadFile(key+5,kEnvLocal);
      }
    else if(!strncmp(key,"-outmap=",8)) 
      {
	if(strlen(key)>8)	cenv->SetValue("comptonmap.out",key+8);
      }
    else if(!strncmp(key,"-jap2",5))
      {
	do_align_jap2=true;
      }
    else if(!strncmp(key,"-jap",4)) 
      {
	do_align_jap=true;
      }
    else if(!strncmp(key,"-listA=",7)) 
      {
	do_tlglista=true;
	if(strlen(key)>7)	cenv->SetValue("comptonmap.listA",key+7);
      }
    else if(!strncmp(key,"-listB=",7)) 
      {
	do_tlglistb=true;
	if(strlen(key)>7)	cenv->SetValue("comptonmap.listB",key+7);
      }
    else if(!strncmp(key,"-o",2)) 
      {
	if(strlen(key)>2)	cenv->SetValue("comptonmap.outdir",key+2);
      }
    else if(!strncmp(key,"-applytest",10)) 
      {
	do_apply_test_map = true;
      }
    else if(!strncmp(key,"-apply",6)) 
      {
	do_apply_map = true;
	//	if(strlen(key)>5)	cenv->SetValue("comptonmap.ApplyMapFile",key+5);
      }
    else if(!strncmp(key,"-link=",6)) 
      {
	do_link = true;
	if(strlen(key)>6)	cenv->SetValue("comptonmap.link.runfile",key+6);
      }
    else if(!strncmp(key,"-v",2))
      {
	if(strlen(key)>2)	cenv->SetValue("comptonmap.EdbDebugLevel",atoi(key+2));
      }
  }

  cenv->Print();
  gEDBDEBUGLEVEL        = cenv->GetValue("comptonmap.EdbDebugLevel" , 1);

  if(do_ida&&do_idb)
    {
      printf("\n----------------------------------------------------------------------------\n");
      printf("make compton map for runs %s and  %s\n", 
	     cenv->GetValue("comptonmap.runA" , ""),
	     cenv->GetValue("comptonmap.runB" , "")
	     );
      printf("----------------------------------------------------------------------------\n\n");

      AlignRunMap(cenv);
    }
  else if(do_link)
    {

      const char *runfile = cenv->GetValue("comptonmap.link.runfile","run.root");
      EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.link.out" , "link_test")), "RECREATE");
      amap.eEnv = cenv;
      EdbPlateP p;
      float DZ   = cenv->GetValue("fedra.link.DZbase",214.);
      p.SetPlateLayout(DZ,45,45);
      amap.Link(runfile,p);

//       EdbPattern p1,p2;
//       const char *runfile = cenv->GetValue("comptonmap.link.runfile","run.root");
//       get_run_patterns( runfile , cenv, &p1, &p2);

//       p1.SetZ(0);    p1.SetSegmentsZ();
//       p2.SetZ(DZ);   p2.SetSegmentsZ();      

//       Log(0,"LinkPlate", "with %d and %d segments (dz = %6.1f)", p1.N(), p2.N(), DZ );

//       EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.link.out" , "link_test")), "RECREATE");
//       amap.eEnv = cenv;
//       amap.ePat1 = &p1;
//       amap.ePat2 = &p2;
//       amap.Link();
//       cenv->Print();
//       amap.SaveAll();

    }
  else if(do_tlglista&&do_tlglistb)
    {
//       EdbPattern p1t,p1b, p1bas, p2t,p2b, p2bas;  // read full patterns
//       get_patterns_tlglist1(cenv->GetValue("comptonmap.listA",""), cenv, p1t, p1b, p1bas);
//       get_patterns_tlglist1(cenv->GetValue("comptonmap.listB",""), cenv, p2t, p2b, p2bas);
      
//       TFile f("pattmp.root","RECREATE");   // temporary file for the fast access
//       printf("save %d %d %d (top:bot:base) segments for plate 1\n",p1t.N(),p1b.N(),p1bas.N());
//       p1t.Write("p1t");
//       p1b.Write("p1b");
//       p1bas.Write("p1bas");
//       printf("save %d %d %d (top:bot:base) segments for plate 2\n",p2t.N(),p2b.N(),p2bas.N());
//       p2t.Write("p2t");
//       p2b.Write("p2b");
//       p2bas.Write("p2bas");
//       f.Close();

//       TFile f("pattmp.root","READ");
//       EdbPattern *p1 = (EdbPattern*)(f.Get("p1b"));
//       EdbPattern *p2 = (EdbPattern*)(f.Get("p2t"));
//       printf("test 2 patterns with  %d and %d  segments\n",p1->N(), p2->N());
//       EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")));
//       amap.eEnv = cenv;  // pass parameters here
//       amap.ePat1 = p1;
//       amap.ePat2 = p2;
//       amap.DoAll();
//       amap.SaveAll();

      // NOTE: in the postree flag=0 - all comb; flag=1 - peak selected com

      if     (do_apply_map) ApplyMap(cenv);
      else if(do_apply_test_map) ApplyTestMap(cenv);
//       else if(do_link_map)  
// 	{
// 	  EdbPattern p1t,p1b, p1bas, p2t,p2b, p2bas;  // read full patterns
// 	  get_patterns_tlglist1(cenv->GetValue("comptonmap.listA",""), cenv, &p1t, &p1b, 0);
// 	  printf("read %d %d %d (top:bot:base) segments for plate 1\n",p1t.N(),p1b.N(),p1bas.N());
// 	  //Link1(cenv, p1b, p1t);
// 	}
      else if(do_align_jap)  AlignMapJap(cenv);
      else if(do_align_jap2)  LinkJap2(cenv);
      else                   AlignMap(cenv);

    }
  else	{ print_help_message(); return 0; }
  
  cenv->WriteFile("comptonmap.save.rootrc");
  
  return 1;
}

//-------------------------------------------------------------------------------
void AlignRunMap(TEnv *cenv)
{
  // read from root raw files sides p1b and p2t and find the compton map

  EdbPattern p1b(0,0,0,2000000),p2t(0,0,0,2000000);  // read full patterns
  get_run_patterns(cenv->GetValue("comptonmap.runA",""), cenv, 0, &p1b);
  get_run_patterns(cenv->GetValue("comptonmap.runB",""), cenv, &p2t, 0);
  printf("test 2 patterns with  %d and %d  segments\n",p1b.N(), p2t.N());

  p1b.ProjectTo(-45.);  // project to nominal contact surface position to simplify the alignment
  p2t.ProjectTo( 45.);
  
  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "RECREATE");
  amap.eEnv = cenv;  // pass parameters here
  amap.ePat1 = &p1b;
  amap.ePat2 = &p2t;
  amap.AlignMap();
  //amap.SaveAll();
  amap.SaveMap(cenv->GetValue("comptonmap.out" , "out.map"));
  //amap.ExtractMapFromTree();
  amap.SaveAll();
}

//-------------------------------------------------------------------------------
void AlignMap(TEnv *cenv)
{
  // read from tlglists the sides p1b and p2t and find the compton map

  EdbPattern p1b(0,0,0,2000000),p2t(0,0,0,2000000);  // read full patterns
  get_patterns_tlglist1(cenv->GetValue("comptonmap.listA",""), cenv, 0, &p1b, 0);
  get_patterns_tlglist1(cenv->GetValue("comptonmap.listB",""), cenv, &p2t, 0, 0);
  printf("test 2 patterns with  %d and %d  segments\n",p1b.N(), p2t.N());

  p1b.ProjectTo(-45.);  // project to nominal contact surface position to simplify the alignment
  p2t.ProjectTo( 45.);
  
  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "RECREATE");
  amap.eEnv = cenv;  // pass parameters here
  amap.ePat1 = &p1b;
  amap.ePat2 = &p2t;
  amap.AlignMap();
  //amap.SaveAll();
  amap.SaveMap(cenv->GetValue("comptonmap.out" , "out.map"));
  //amap.ExtractMapFromTree();
  amap.SaveAll();
}

//-------------------------------------------------------------------------------
void AlignMapJap(TEnv *cenv)
{
  // read from tlglists the sides p1b and p2t and find the compton map

  EdbPattern p1b(0,0,0,200000),p2t(0,0,0,200000);  // read full patterns
  get_patterns_Jap(cenv->GetValue("comptonmap.listA",""), cenv, p1b);
  get_patterns_Jap(cenv->GetValue("comptonmap.listB",""), cenv, p2t);
  printf("test 2 Jap patterns with  %d and %d  segments\n",p1b.N(), p2t.N());

  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "RECREATE");
  amap.eEnv = cenv;  // pass parameters here
  amap.ePat1 = &p1b;
  amap.ePat2 = &p2t;
  amap.AlignMap();
  //amap.SaveAll();
  amap.SaveMap(cenv->GetValue("comptonmap.out" , "out.map"));
  //amap.ExtractMapFromTree();
  amap.SaveAll();
}

//-------------------------------------------------------------------------------
void LinkJap2(TEnv *cenv)
{
  // read 2 sides from nagoya files and link them

  EdbPattern p1b(0,0,0,200000),p2t(0,0,0,200000);  // read full patterns
  get_patterns_Jap2(cenv->GetValue("comptonmap.listA",""), cenv, p1b);
  get_patterns_Jap2(cenv->GetValue("comptonmap.listB",""), cenv, p2t);
  printf("test 2 Jap patterns with  %d and %d  segments\n",p1b.N(), p2t.N());

  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "link_test")), "RECREATE");
  amap.eEnv = cenv;  // pass parameters here
  amap.ePat1 = &p1b;
  amap.ePat2 = &p2t;
  amap.Link();
  amap.SaveAll();
}

//-------------------------------------------------------------------------------
void ApplyTestMap(TEnv *cenv)
{
  // read the same layers used for the map production and do the global alignment 
  // to see the difference with what was before

  EdbPattern p1t,p1b, p1bas, p2t,p2b, p2bas;  // read full patterns
  get_patterns_tlglist1(cenv->GetValue("comptonmap.listA",""), cenv, 0, &p1b, 0);
  printf("read %d %d %d (top:bot:base) segments for plate 1\n",p1t.N(),p1b.N(),p1bas.N());
  get_patterns_tlglist1(cenv->GetValue("comptonmap.listB",""), cenv, &p2t, 0, 0);
  printf("read %d %d %d (top:bot:base) segments for plate 2\n",p2t.N(),p2b.N(),p2bas.N());

  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "READ");
  amap.eEnv = cenv;  // pass parameters here
  amap.ExtractMapFromTree();

  amap.ApplyMap(p1b);
  //amap.ApplyMap(p1b,p2t);

  EdbAlignmentMap amap2("atest.root", "RECREATE");

  EdbPositionAlignment abase;
  abase.eXcell   = abase.eYcell   = 100;
  abase.eNZ1step = cenv->GetValue("comptonmap.apply.NZ1step",  100);
  abase.eNZ2step = cenv->GetValue("comptonmap.apply.NZ2step",  100);
  abase.eZ1from  = cenv->GetValue("comptonmap.apply.Z1from", -400);
  abase.eZ1to    = cenv->GetValue("comptonmap.apply.Z1to",    400);
  abase.eZ2from  = cenv->GetValue("comptonmap.apply.Z2from", -400);
  abase.eZ2to    = cenv->GetValue("comptonmap.apply.Z2to",    400);
  abase.eBinX    = abase.eBinY    = cenv->GetValue("comptonmap.apply.BinXY",    7);
  abase.eDTXmax  = abase.eDTYmax  = cenv->GetValue("comptonmap.apply.DTMax", 0.01 );

  abase.FillArrays(p1b, p2t);

  abase.FillCombinations();
  abase.ePC1.PrintStat();
  abase.ePC2.PrintStat();
  abase.ActivatePosTree();
  abase.Align();
  abase.PrintSummary();

  amap2.FillMapTree(abase,0);
}

//-------------------------------------------------------------------------------
void ApplyMap(TEnv *cenv)
{
  // read basetracks from tlglists and apply the compton map

  EdbPattern p1t,p1b, p1bas, p2t,p2b, p2bas;  // read full patterns
  get_patterns_tlglist1(cenv->GetValue("comptonmap.listA",""), cenv, 0, 0, &p1bas);
  printf("read %d %d %d (top:bot:base) segments for plate 1\n",p1t.N(),p1b.N(),p1bas.N());
  get_patterns_tlglist1(cenv->GetValue("comptonmap.listB",""), cenv, 0, 0, &p2bas);
  printf("read %d %d %d (top:bot:base) segments for plate 2\n",p2t.N(),p2b.N(),p2bas.N());

  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "READ");
  amap.eEnv = cenv;  // pass parameters here
  amap.ExtractMapFromTree();

  p1bas.ProjectTo(-150.);
  p2bas.ProjectTo( 150.);

  amap.ApplyMap(p1bas);
  //amap.ApplyMap(p1bas,p2bas);

  EdbAlignmentMap amap2("atestBase.root", "RECREATE");

  EdbPositionAlignment abase;
  abase.eXcell   = abase.eYcell   = 100;
  abase.eNZ1step = cenv->GetValue("comptonmap.apply.NZ1step",  100);
  abase.eNZ2step = cenv->GetValue("comptonmap.apply.NZ2step",  100);
  abase.eZ1from  = cenv->GetValue("comptonmap.apply.Z1from", -400);
  abase.eZ1to    = cenv->GetValue("comptonmap.apply.Z1to",    400);
  abase.eZ2from  = cenv->GetValue("comptonmap.apply.Z2from", -400);
  abase.eZ2to    = cenv->GetValue("comptonmap.apply.Z2to",    400);
  abase.eBinX    = abase.eBinY    = cenv->GetValue("comptonmap.apply.BinXY",    7);
  abase.eDTXmax  = abase.eDTYmax  = cenv->GetValue("comptonmap.apply.DTMax", 0.01 );

  abase.FillArrays(p1bas, p2bas);
  abase.DoubletsFilterOut(false);
  abase.FillCombinations();
  abase.ePC1.PrintStat();
  abase.ePC2.PrintStat();
  abase.ActivatePosTree();
  abase.Align();
  abase.PrintSummary();

  amap2.FillMapTree(abase,0);
}

//------------------------------------------------------------------------
void get_patterns_tlglist1(const char *listfile, TEnv *cenv, EdbPattern *ptop, EdbPattern *pbot, EdbPattern *pbase )
{
  // read data from the list of tlgfiles, 
  // apply cuts defined by  wminTop, wminBot,  wminBase, thetamax, xmin, xmax, ymin, ymax

  char *str = new char[512];

  float wminTop    = cenv->GetValue("comptonmap.MinGrainsTop" , 8.);
  float wminBot    = cenv->GetValue("comptonmap.MinGrainsBot" , 8.);
  float wminBase   = cenv->GetValue("comptonmap.MinGrainsBase",16);
  float thetamax   = cenv->GetValue("comptonmap.ThetaMax",      1.);
  float xmin       = cenv->GetValue("comptonmap.Xmin",   -1000000.);
  float xmax       = cenv->GetValue("comptonmap.Xmax",    1000000.);
  float ymin       = cenv->GetValue("comptonmap.Ymin",   -1000000.);
  float ymax       = cenv->GetValue("comptonmap.Ymax",    1000000.);
  float maxDens    = cenv->GetValue("comptonmap.DensityMax",    2.) + 0.2;

  ifstream in1; in1.open(listfile);	if(!in1.good()) return;

  EdbPattern pt(0,0,0,10000), pb(0,0,0,10000), pbas(0,0,0,10000);
  while(1) {
    in1.getline( (str),512 );
    if(!in1.good()) break;		if(in1.eof()) break;

	pt.Reset();	pb.Reset();	pbas.Reset();
    tlg2patterns( str, pt, pb, pbas);

    int ntr=0, nbr=0, nbasr=0;
    if(ptop) {
      int n0t=ptop->N();
      add_pattern(*ptop,    pt, wminTop , thetamax, xmin, xmax, ymin, ymax, maxDens );
      ntr = ptop->N() - n0t;
    }
   if(pbot) {
      int n0b=pbot->N();
      add_pattern(*pbot,    pb, wminBot , thetamax, xmin, xmax, ymin, ymax, maxDens );
      nbr = pbot->N() - n0b;
   }
   if(pbase) {
     int n0bas=pbase->N();
     add_pattern(*pbase, pbas, wminBase, thetamax, xmin, xmax, ymin, ymax, maxDens );
     nbasr = pbase->N() - n0bas;
   }
   Log(2,"get_patterns_tlglist", "read %d:%d:%d  (top:bot:base) segments from file: \"%s\"",
       ntr,nbr,nbasr ,str );
  }
}

//------------------------------------------------------------------------
void get_patterns_Jap(const char *listfile, TEnv *cenv, EdbPattern &p )
{
  // read data from the japanese compton sample
  // apply cuts defined by  wminTop, wminBot,  wminBase, thetamax, xmin, xmax, ymin, ymax

  float wmin       = cenv->GetValue("comptonmap.MinGrainsTop" , 8.);
  float thetamax   = cenv->GetValue("comptonmap.ThetaMax",      1.);
  float xmin       = cenv->GetValue("comptonmap.Xmin",   -1000000.);
  float xmax       = cenv->GetValue("comptonmap.Xmax",    1000000.);
  float ymin       = cenv->GetValue("comptonmap.Ymin",   -1000000.);
  float ymax       = cenv->GetValue("comptonmap.Ymax",    1000000.);
  float maxDens    = cenv->GetValue("comptonmap.DensityMax",    2.) + 0.2;

  printf("listfile = %s\n",listfile);
  FILE *f = fopen( listfile, "r");
  if(!f) return;
  char *str = new char[512];
  for(int i=0; i<4; i++) {fgets(str,512,f); printf("%s\n",str);}

  EdbPattern ptmp(0,0,0,10000);
  EdbSegP *s=0;
  int    id, id1, ipuls;
  float  tx, ty, x,y, vx, vy;
  while(1) {
    if( fscanf(f,"%d %d %d %f %f %f %f %f %f",&id, &id1, &ipuls, &tx, &ty, &x,&y, &vx, &vy) != 9) break;
    s = ptmp.AddSegment(id, x,y,tx,ty, ipuls/10000 );
  }
  add_pattern(p,    ptmp, wmin , thetamax, xmin, xmax, ymin, ymax, maxDens );
   Log(2,"get_patterns_tlglist", "read %d segments from file: \"%s\"",
       p.N() ,listfile );
}

//------------------------------------------------------------------------
void get_patterns_Jap2(const char *listfile, TEnv *cenv, EdbPattern &p )
{
  // read data from the japanese event sample
  // apply cuts defined by  wminTop, wminBot,  wminBase, thetamax, xmin, xmax, ymin, ymax

  float wmin       = cenv->GetValue("comptonmap.MinGrainsTop" , 8.);
  float thetamax   = cenv->GetValue("comptonmap.ThetaMax",      1.);
  float xmin       = cenv->GetValue("comptonmap.Xmin",   -1000000.);
  float xmax       = cenv->GetValue("comptonmap.Xmax",    1000000.);
  float ymin       = cenv->GetValue("comptonmap.Ymin",   -1000000.);
  float ymax       = cenv->GetValue("comptonmap.Ymax",    1000000.);
  float maxDens    = cenv->GetValue("comptonmap.DensityMax",    2.) + 0.2;

  printf("listfile = %s\n",listfile);
  FILE *f = fopen( listfile, "r");  if(!f) return;

  EdbPattern ptmp(0,0,0,10000);
  EdbSegP *s=0;
  int    idpl, id, id1, id2, ipuls, flag;
  float  tx, ty, x,y,z, z1,z2, vx, vy;
  while(1) {
    if( fscanf(f,"%d %d %d %d %d %f %f %f %f %f %f %f %f %f %d",
	       &idpl, &id, &id1, &id2, &ipuls, &tx, &ty, &x,&y,&z, &z1,&z2, &vx, &vy, &flag) != 15) break;
    s = ptmp.AddSegment(id1, x,y,tx,ty, ipuls/10000 );
    s->SetZ(z);
    s->SetDZ(Abs(z2-z1));
    //if(!(id1%1000)) { printf("id1 = %d\n",id1); s->PrintNice(); }
  }

  add_pattern(p,    ptmp, wmin , thetamax, xmin, xmax, ymin, ymax, maxDens );
  Log(2,"get_patterns_tlglist", "read %d (of %d) segments from file: \"%s\"",
      p.N(), ptmp.N() ,listfile );
}

//------------------------------------------------------------------------
void add_pattern( EdbPattern &p, EdbPattern &padd, 
				  float wmin, float thetamax, float xmin, float xmax, float ymin, float ymax, float maxDens)
{
  // add pattern padd to p with the requested selections

  float min[2] = { kMaxInt, kMaxInt};
  float max[2] = { kMinInt, kMinInt};
  int n = padd.N();
  TObjArray a(n);
  EdbSegP *s=0;
  for(int i=0; i<n; i++) {
    s = padd.GetSegment(i);
    if(s->W() < wmin)         continue;
    if(s->Theta() > thetamax) continue;
    if(s->X() < xmin)         continue;
    if(s->X() > xmax)         continue;
    if(s->Y() < ymin)         continue;
    if(s->Y() > ymax)         continue;
	s->SetFlag(0);
	a.Add( s );

	if(s->X() < min[0]) min[0]=s->X();
	if(s->Y() < min[1]) min[1]=s->Y();
	if(s->X() > max[0]) max[0]=s->X();
	if(s->Y() > max[1]) max[1]=s->Y();
  }
  int nmax = (int)(maxDens*(max[0]-min[0])*(max[1]-min[1]) /100/100);

  TObjArray arr(n);
  EdbPositionAlignment::SelectBestComptons( a, arr, nmax);
  int nsel = arr.GetEntriesFast();
  for(int i=0; i<nsel; i++)  {
	s = (EdbSegP *)arr.At(i);
	p.AddSegment( *s );
  }
}

/*
//------------------------------------------------------------------------
void get_patterns(EdbID id1, EdbID id2, TEnv *cenv, EdbPattern &p1, EdbPattern &p2)
{
  // read patterns from the raw files in the root format

  float x           = cenv->GetValue("comptonmap.x0"   , 40000.);
  float y           = cenv->GetValue("comptonmap.y0"   , 40000.);
  float size        = cenv->GetValue("comptonmap.size" , 500000.);
  float pulsmin     = cenv->GetValue("comptonmap.pulsmin" , 8);
  int   side1       = cenv->GetValue("comptonmap.side1" , 2);
  int   side2       = cenv->GetValue("comptonmap.side2" , 1);
  int   useExternal = cenv->GetValue("comptonmap.useExternal" , 0);
  cenv->Print();

  sproc = new EdbScanProc();
  //sproc->eProcDirClient = "d:\\work\\valeri\\newCS";
  sproc->eProcDirClient = cenv->GetValue("comptonmap.outdir"   , "./");

  EdbRunAccess ra1,ra2;
  int id14[4]; id1.Get(id14);
  int id24[4]; id2.Get(id24);
  sproc->InitRunAccess(ra1,id14);
  sproc->InitRunAccess(ra2,id24);

  ra1.eAFID=1;  ra2.eAFID=1;
  if(useExternal) {
    ra1.eCLUST=1;   ra1.eUseExternalSurface=1;
    ra2.eCLUST=1;   ra2.eUseExternalSurface=1;
  }

   float min[5] = {-500,-500,-1.,-1., pulsmin};
   float max[5] = { 500, 500, 1., 1., 50};
   ra1.AddSegmentCut(side1, 1, min, max);
   ra1.AddSegmentCut(side2, 1, min, max);
//   ra2.AddSegmentCut(side1, 1, min, max);
//   ra2.AddSegmentCut(side2, 1, min, max);

//   // exclude low angles for linking test:
//   float minX[5] = {-500,-500,-.02,-.02, pulsmin};
//   float maxX[5] = { 500, 500, .02, .02, 50};
//   ra1.AddSegmentCut(side1, 0, minX, maxX);
//   ra1.AddSegmentCut(side2, 0, minX, maxX);
//   ra2.AddSegmentCut(side1, 0, minX, maxX);
//   ra2.AddSegmentCut(side2, 0, minX, maxX);

  EdbSegP s(0, x, y, 0,0);

  int n1= ra1.GetPatternXY(s, side1, p1, size);
  int n2= ra1.GetPatternXY(s, side2, p2, size);
  //  int n2= ra2.GetPatternXY(s, side2, p2, size);
  p1.SetID(1);
  p2.SetID(2);
  printf("read segments: n1=%d  n2=%d \n",n1,n2);
}
*/

//------------------------------------------------------------------------
void get_run_patterns(const char *runfile, TEnv *cenv, EdbPattern *p1, EdbPattern *p2)
{
  // read patterns from the raw files in the root format

  float x           = cenv->GetValue("comptonmap.x0"      , 40000.);
  float y           = cenv->GetValue("comptonmap.y0"      , 40000.);
  float size        = cenv->GetValue("comptonmap.size"    , 500000.);
  float pulsmin     = cenv->GetValue("comptonmap.pulsmin" , 8);
  int   side1       = cenv->GetValue("comptonmap.side1"   , 1);    // usually top    of the plate
  int   side2       = cenv->GetValue("comptonmap.side2"   , 2);    // usually bottom of the plate

  Log(2,"get_run_patterns","read file: %s",runfile);

  EdbRunAccess ra;
  ra.InitRun(runfile);
  ra.GetLayer(2)->SetZlayer(   0,   0,   0 );
  ra.GetLayer(1)->SetZlayer( 214,   0,   0 );
  ra.GetLayer(1)->SetShrinkage( 1. );
  ra.GetLayer(2)->SetShrinkage( 1. );
  ra.eAFID=1;

  float min[5] = {-500,-500,-1.,-1., pulsmin};
  float max[5] = { 500, 500, 1., 1., 50};
  ra.AddSegmentCut(side1, 1, min, max);
  ra.AddSegmentCut(side2, 1, min, max);

  EdbSegP s(0, x, y, 0,0);

  int n1=0, n2=0;
  if(p1) { n1= ra.GetPatternXY(s, side1, *p1, size);   p1->SetID(1); }
  if(p2) { n2= ra.GetPatternXY(s, side2, *p2, size);   p2->SetID(2); }

  printf("read segments: n1=%d  n2=%d \n",n1,n2);
}

