#include <iostream>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbProcPars.h"
#include "EdbVertex.h"

using namespace std;

//----------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\n Vertex check using as input tracks ascii files\n";
  
  cout<< "\nUsage: \n\t  emvtx -fcs=tracks_file.txt [-v=DEBUG] \n";
  cout<< "\t\t  fcs    - file in CS format \n";
  cout<< "\t\t  DEBUG - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  
  cout<< "\nCS format:\n\t BRICK EVENT \n\t x y z tx ty nbt rank idtr idtrG \n\t ...\n"; 
  cout<< "\nExample: \n";
  cout<< "\t  emvtx -fcs=tracks_ev1.txt -v=2\n";
  
  cout<< "\n If the parameters file (emvtx.rootrc) is not presented - the default \n";
  cout<< " parameters are used. After the execution them will be saved into emvtx.save.rootrc\n";
  cout<<endl;
}

//-----------------------------------------------------------------------------
TEnv        cenv("emvtx");

//---------------------------------------------------------------------
EdbTrackP *make_seg_track(int id, float x, float y, float z, float tx, float ty, float p, float *vp=0)
{
  EdbSegP *s = new EdbSegP( id, x,y, tx,ty, 20, 0);
  s->SetZ(z);
  if(vp) {
    s->SetDZ(500*vp[2]);          // the length is proportional to P
    s->PropagateTo(z+s->DZ()/2);
  } else {
    s->SetDZ(200);
  }
  s->SetErrorsCOV( 10,10,100, 0.00003,0.00003, 1);
  EdbTrackP *t = new EdbTrackP(s);
  t->SetM(0.12);
  t->SetP(p);
  t->FitTrackKFS();
  return t;
}

//-----------------------------------------------------------------------------
int readCSdata(const char *fname, TObjArray &tracks)
{
  printf("open file: %s\n",fname);
  FILE *f = fopen( fname, "r" );                          if(!f) return -1;
  char    buffer[256];

  // read brick and event
  Long_t BRICK=0, EVENT=0;
  if(!fgets (buffer, 256, f))                                    return -1;
  if ( sscanf(buffer,"%ld %ld", &BRICK,&EVENT) !=2 )             return -1;
  printf("brick: %ld   event: %ld\n",BRICK,EVENT);

  // read CS tracks
  int ntr=0;
  int nbt, idtr;
  Long64_t idtrG;
  float x,y,z,tx,ty, rank;
  while( fgets (buffer, 256, f) ) {
    if ( sscanf(buffer,"%f %f %f %f %f %d %f %d %lld", &x, &y, &z, &tx, &ty, &nbt, &rank, &idtr, &idtrG) !=9 ) break;
    tracks.Add( make_seg_track( ntr++,   x, y, 0,  tx, ty, 1.5,0) );
  }
  fclose(f);
  return ntr;
}

//-----------------------------------------------------------------------------
EdbVertex *vtxCS(const char *file, TEnv &env)
{
  TObjArray tracks;
  int ntrCS = readCSdata( file, tracks );

  if(ntrCS < 0)  { printf("bad file - skip\n"); return 0; }

  if(ntrCS<2) return 0;
  
  //calc_stat(*gAliCS);


  EdbVertexRec *gEVRCS = new EdbVertexRec();
  gEVRCS->eProbMin   = 0;      // minimum acceptable probability for chi2-distance between tracks
  gEVRCS->eImpMax    = 15000;   // maximal acceptable impact parameter (preliminary check)
  gEVRCS->eUseSegPar = 0;      // use only the nearest measured segments for vertex fit
  gEVRCS->eUseMom    = 1;      // use or not track momentum for vertex calculations
  
  EdbPVRec *pvr = new EdbPVRec();
  EdbScanCond *cond = new EdbScanCond();
  pvr->SetScanCond(cond);
  gEVRCS->SetPVRec(pvr);

  EdbVertex *v = gEVRCS->Make1Vertex( tracks, 0. ); if(!v) return 0;
  v=gEVRCS->StripBadTracks( *v, 1000., 3 );         if(!v) return 0;

  //ntrCSv=v->N();
  //impMax=v->MaxImpact();
  return v;
}

//---------------------------------------------------------------------
void print_v(EdbVertex &v)
{
  int ntr = v.N();
  if(ntr<2) return;
  printf("-----------------------------------------------------------------------------------------\n");
  printf( "Vertex %d, Prongs %d, Prob %f   Position: %12.3f %12.3f %12.3f   Flag: %d\n",
          v.ID(), ntr, v.V()->prob(), v.VX(), v.VY(), v.VZ(), v.Flag() );
  printf( "    #   ID   Nseg   Mass       P       Chi2/ndf    Prob     Chi2Contrib     Impact\n");
  float maximp=0;
  int itr=-1;
  for(int i=0; i<ntr; i++) {
    EdbTrackP *tr = v.GetTrack(i);
    printf("%4d  %4d  %4d   %7.4f  %7.2f    %5.2f     %7.4f       %6.3f    %7.2f\n",
           i, tr->ID(), tr->N(), tr->M(), tr->P(),
           tr->Chi2()/tr->N(), tr->Prob(), v.V()->track_chi2(i), v.Impact(i));
    if( v.Impact(i)>maximp )  {maximp=v.Impact(i); itr=i;}
  }
  printf("maximp = %f\n",maximp);
  //for(int i=0; i<ntr; i++) v.GetTrack(i)->PrintNice();
  //  if( v.N()>3 && maximp>1000.)  print_v( *(gEVRCS->RemoveTrackFromVertex(&v, itr)) );
  //if( v.N()>3 && maximp>1000.)  print_v( *(removeTrack(v, itr)) );

  printf("-----------------------------------------------------------------------------------------\n");
}

//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }

  bool do_fcs=false;
  const char *filecs=0;
  
  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];
    if(!strncmp(key,"-fcs=",5))
      {
	if(strlen(key)>5)  { filecs = key+5;  do_fcs=true; }
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
  }
  
  cenv.ReadFile("emvtx.rootrc" ,kEnvLocal);
  
  if(do_fcs) 
  {
    EdbVertex *v = vtxCS( filecs, cenv );
    print_v(*v);
  }
   
  cenv.WriteFile("emvtx.save.rootrc");
 
  return 0;
}
