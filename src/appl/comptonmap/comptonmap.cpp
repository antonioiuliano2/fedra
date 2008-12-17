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

//void testcompt(EdbPattern &p1, EdbPattern &p2, int brick, TEnv *cenv);
//void test(EdbPattern &p1, EdbPattern &p2, float x0, float y0, int ix, int iy, TEnv *cenv);
void get_patterns(EdbID id1, EdbID id2, TEnv *cenv, EdbPattern &p1, EdbPattern &p2);
void get_patterns_tlg(TEnv *cenv, EdbPattern &p1, EdbPattern &p2);
void add_pattern(EdbPattern &p, EdbPattern &padd, 
				 float wmin=0, float thetamax=1., float xmin=0, float xmax=1000000, float ymin=0, float ymax=1000000);
void get_patterns_tlglist1(const char *listfile, TEnv *cenv, EdbPattern *ptop, EdbPattern *pbot, EdbPattern *pbase );

EdbScanProc   *sproc=0;

//--------------------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\nExample: \n";
  cout<< "\t  comptonmap -listA=tlg1.lst -listB=tlg2.lst -outmap=map.out -par=comptonpar0.par -v2 \n";
  cout<< "\t  comptonmap -A3103130.1.0.1 -B3103130.2.0.1 -oD:/work/valeri/newCS \n";
  cout<< "\nThe data location directory if not explicitly defined will be taken from .rootrc as: \n";
  cout<< "\t  comptonmap.outdir:      /scratch/BRICKS \n";
  cout<< "\t  comptonmap.EdbDebugLevel:      1\n";
  cout<<endl;
}

//-------------------------------------------------------------------------------
void AlignMap(TEnv *cenv)
{
  EdbPattern p1b,p2t;  // read full patterns
  get_patterns_tlglist1(cenv->GetValue("comptonmap.tlgA",""), cenv, 0, &p1b, 0);
  get_patterns_tlglist1(cenv->GetValue("comptonmap.tlgB",""), cenv, &p2t, 0, 0);
  printf("test 2 patterns with  %d and %d  segments\n",p1b.N(), p2t.N());

  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "RECREATE");
  amap.eEnv = cenv;  // pass parameters here
  amap.ePat1 = &p1b;
  amap.ePat2 = &p2t;
  amap.AlignMap();
  amap.SaveAll();
  amap.SaveMap(cenv->GetValue("comptonmap.out" , "out.map"));
  amap.ExtractMapFromTree();
  amap.SaveAll();
}

//-------------------------------------------------------------------------------
void ApplyMap(TEnv *cenv)
{
  EdbPattern p1t,p1b, p1bas, p2t,p2b, p2bas;  // read full patterns
  get_patterns_tlglist1(cenv->GetValue("comptonmap.tlgA",""), cenv, &p1t, &p1b, &p1bas);
  printf("read %d %d %d (top:bot:base) segments for plate 1\n",p1t.N(),p1b.N(),p1bas.N());
  get_patterns_tlglist1(cenv->GetValue("comptonmap.tlgB",""), cenv, &p2t, &p2b, &p2bas);
  printf("read %d %d %d (top:bot:base) segments for plate 2\n",p2t.N(),p2b.N(),p2bas.N());

  EdbAlignmentMap amap(Form("%s.root", cenv->GetValue("comptonmap.out" , "out.map")), "READ");
  amap.eEnv = cenv;  // pass parameters here
  amap.ExtractMapFromTree();


  int doAppl = cenv->GetValue("comptonmap.apply.ApplyCorr", 1 );
  int iside  = cenv->GetValue("comptonmap.apply.TestSide", 0 );
  if(doAppl) {
    if(iside) amap.ApplyMap(p1b,p2t);
    else      amap.ApplyMap(p1bas,p2bas);
  }

  EdbAlignmentMap amap2("atest.root", "RECREATE");

  EdbPositionAlignment abase;
  abase.eXcell   = abase.eYcell   = 100;
  abase.eNZ1step = cenv->GetValue("comptonmap.apply.NZ1step",  100);
  abase.eNZ2step = cenv->GetValue("comptonmap.apply.NZ2step",  100);
  abase.eZ1from  = cenv->GetValue("comptonmap.apply.Z1from", -400);
  abase.eZ1to    = cenv->GetValue("comptonmap.apply.Z1to",    400);
  abase.eZ2from  = cenv->GetValue("comptonmap.apply.Z2from", -400);
  abase.eZ2to    = cenv->GetValue("comptonmap.apply.Z2to",    400);
  abase.eBinX    = abase.eBinY    = cenv->GetValue("comptonmap.apply.BinXY",    7);;
  abase.eDTXmax  = abase.eDTYmax  = cenv->GetValue("comptonmap.apply.DTMax", 0.01 );

  if(iside)  abase.FillArrays(p1b, p2t);
  else       abase.FillArrays(p1bas, p2bas);
  abase.FillCombinations();
  abase.ePC1.PrintStat();
  abase.ePC2.PrintStat();
  abase.ActivatePosTree();
  abase.Align();
  abase.PrintSummary();

  amap2.FillMapTree(abase,0);

}

//--------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }

  TEnv *cenv = new TEnv("comptonenv");
  cenv->ReadFile("comptonmap.rootrc",kEnvAll);

  bool      do_ida      = false;
  bool      do_idb      = false;
  Int_t     brick=0, plate=0, major=0, minor=0;
  //Int_t     npre=0,  nfull=0;
  EdbID     idA,idB;
  bool      do_tlga = false;
  bool      do_tlgb = false;
  bool      do_tlglista = false;
  bool      do_tlglistb = false;

  bool      do_apply_map = false;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];

    if     (!strncmp(key,"-A",2)) 
      {
	if(strlen(key)>2)	sscanf(key+2,"%d.%d.%d.%d",&brick,&plate,&major,&minor);
	idA.Set(brick,plate,major,minor);
	do_ida=true;
      }
    else if(!strncmp(key,"-B",2)) 
      {
	if(strlen(key)>2)	sscanf(key+2,"%d.%d.%d.%d",&brick,&plate,&major,&minor);
	idB.Set(brick,plate,major,minor);
	do_idb=true;
      }
    else if(!strncmp(key,"-par=",5)) 
      {
	cenv->ReadFile(key+5,kEnvAll);
      }
    else if(!strncmp(key,"-outmap=",8)) 
      {
	if(strlen(key)>8)	cenv->SetValue("comptonmap.out",key+8);
      }
    else if(!strncmp(key,"-brick=",7)) 
      {
	if(strlen(key)>7)	cenv->SetValue("comptonmap.brick",atoi(key+7));
      }
    else if(!strncmp(key,"-tlgA=",6)) 
      {
	do_tlga=true;
	if(strlen(key)>6)	cenv->SetValue("comptonmap.tlgA",key+6);
      }
    else if(!strncmp(key,"-tlgB=",6)) 
      {
	do_tlgb=true;
	if(strlen(key)>6)	cenv->SetValue("comptonmap.tlgB",key+6);
      }
    else if(!strncmp(key,"-listA=",7)) 
      {
	do_tlglista=true;
	if(strlen(key)>7)	cenv->SetValue("comptonmap.tlgA",key+7);
      }
    else if(!strncmp(key,"-listB=",7)) 
      {
	do_tlglistb=true;
	if(strlen(key)>7)	cenv->SetValue("comptonmap.tlgB",key+7);
      }
    else if(!strncmp(key,"-o",2)) 
      {
	if(strlen(key)>2)	cenv->SetValue("comptonmap.outdir",key+2);
      }
    else if(!strncmp(key,"-apply",6)) 
      {
	do_apply_map = true;
	//	if(strlen(key)>5)	cenv->SetValue("comptonmap.ApplyMapFile",key+5);
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
      printf("make compton map for  %d.%d.%d.%d and  %d.%d.%d.%d\n"
	     ,idA.eBrick,idA.ePlate, idA.eMajor,idA.eMinor
	     ,idB.eBrick,idB.ePlate, idB.eMajor,idB.eMinor
	     );
      printf("----------------------------------------------------------------------------\n\n");

      EdbPattern p1,p2;
      get_patterns(idA, idB, cenv, p1, p2);
      //testcompt(p1,p2, idA.eBrick, cenv);
    }
  else if(do_tlga&&do_tlgb)
    {
      //int brick = cenv->GetValue("comptonmap.brick" , 0);
      EdbPattern p1,p2;
      get_patterns_tlg(cenv, p1, p2);
      printf("test 2 patterns with  %d and %d  segments\n",p1.N(), p2.N());
      //testcompt(p1,p2, brick, cenv);
    }
  else if(do_tlglista&&do_tlglistb)
    {
//       EdbPattern p1t,p1b, p1bas, p2t,p2b, p2bas;  // read full patterns
//       get_patterns_tlglist1(cenv->GetValue("comptonmap.tlgA",""), cenv, p1t, p1b, p1bas);
//       get_patterns_tlglist1(cenv->GetValue("comptonmap.tlgB",""), cenv, p2t, p2b, p2bas);
      
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


      if(do_apply_map) ApplyMap(cenv);
      else             AlignMap(cenv);

    }
  else	{ print_help_message(); return 0; }

  return 1;
}

bool SINGLEZONE=true;
FILE *file=0;
TNtuple *mapnt   =0;
TNtuple *peaknt_ =0;

//------------------------------------------------------------------------
void get_patterns_tlglist1(const char *listfile, TEnv *cenv, EdbPattern *ptop, EdbPattern *pbot, EdbPattern *pbase )
{
  char *str = new char[512];

  float wminTop     = cenv->GetValue("comptonmap.MinGrainsTop" ,8.);
  float wminBot     = cenv->GetValue("comptonmap.MinGrainsBot" ,8.);
  float wminBase    = cenv->GetValue("comptonmap.MinGrainsBase",16);

  float thetamax = cenv->GetValue("comptonmap.ThetaMax", 1.);
  float xmin = cenv->GetValue("comptonmap.Xmin", -1000000.);
  float xmax = cenv->GetValue("comptonmap.Xmax",  1000000.);
  float ymin = cenv->GetValue("comptonmap.Ymin", -1000000.);
  float ymax = cenv->GetValue("comptonmap.Ymax",  1000000.);

  ifstream in1; in1.open(listfile);	if(!in1.good()) return;

  while(1) {
    in1.getline( (str),512 );
    if(!in1.good()) break;		if(in1.eof()) break;

    EdbPattern pt, pb, pbas;
    tlg2patterns( str, pt, pb, pbas);

    int ntr=0, nbr=0, nbasr=0;

    if(ptop) {
      int n0t=ptop->N();
      add_pattern(*ptop,    pt, wminTop , thetamax, xmin, xmax, ymin, ymax );
      ntr = ptop->N() - n0t;
    }
   if(pbot) {
      int n0b=pbot->N();
      add_pattern(*pbot,    pb, wminBot , thetamax, xmin, xmax, ymin, ymax );
      nbr = pbot->N() - n0b;
   }
   if(pbase) {
     int n0bas=pbase->N();
     add_pattern(*pbase, pbas, wminBase, thetamax, xmin, xmax, ymin, ymax );
     nbasr = pbase->N() - n0bas;
   }
   Log(2,"get_patterns_tlglist", "read %d:%d:%d  (top:bot:base) segments from file: \"%s\"",
       ntr,nbr,nbasr ,str );
  }
}

//------------------------------------------------------------------------
void get_patterns_tlg(TEnv *cenv, EdbPattern &p1, EdbPattern &p2)
{
  EdbPattern ptop1, pbot1, base1;
  tlg2patterns( cenv->GetValue("comptonmap.tlgA",""), ptop1, pbot1,base1);
  EdbPattern ptop2, pbot2, base2;
  tlg2patterns( cenv->GetValue("comptonmap.tlgB",""), ptop2, pbot2,base2);
  add_pattern(p1, pbot1);
  add_pattern(p2, ptop2);
}

//------------------------------------------------------------------------
void add_pattern(EdbPattern &p, EdbPattern &padd, float wmin, float thetamax, float xmin, float xmax, float ymin, float ymax)
{
  EdbSegP *s=0;
  for(int i=0; i<padd.N(); i++) {
    s = padd.GetSegment(i);
    if(s->W() < wmin)         continue;
    if(s->Theta() > thetamax) continue;
    if(s->X() < xmin)         continue;
    if(s->X() > xmax)         continue;
    if(s->Y() < ymin)         continue;
    if(s->Y() > ymax)         continue;
    p.AddSegment( *s );
  }
}

//------------------------------------------------------------------------
void get_patterns(EdbID id1, EdbID id2, TEnv *cenv, EdbPattern &p1, EdbPattern &p2)
{
  float x           = cenv->GetValue("comptonmap.x0"   , 40000.);
  float y           = cenv->GetValue("comptonmap.y0"   , 40000.);
  float size        = cenv->GetValue("comptonmap.size" , 5000.);
  float pulsmin     = cenv->GetValue("comptonmap.pulsmin" , 9);
  int   side1       = cenv->GetValue("comptonmap.side1" , 2);
  int   side2       = cenv->GetValue("comptonmap.side2" , 1);
  int   useExternal = cenv->GetValue("comptonmap.useExternal" , 0);

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
  ra2.AddSegmentCut(side2, 1, min, max);

  EdbSegP s(0, x, y, 0,0);

  int n1= ra1.GetPatternXY(s, side1, p1, size);
  int n2= ra2.GetPatternXY(s, side2, p2, size);
  p1.SetID(1);
  p2.SetID(2);
  printf("read segments: n1=%d  n2=%d \n",n1,n2);
}
