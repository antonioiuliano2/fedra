#include <iostream>
#include "TRint.h"
#include "TStyle.h"
#include "TArrayL64.h"
#include "TMath.h"
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbProcPars.h"
#include "EdbVertex.h"
#include "EdbDisplay.h"
#include "EdbCombGen.h"

using namespace std;
using namespace TMath;

//-----------------------------------------------------------------------------
TEnv        cenv("emvtx");
EdbScanCond gCond;
EdbVertexRec *gEVRCS=0;

//----------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\n Vertex check using as input tracks ascii files\n";
  
  cout<< "\nUsage: \n\t  emvtx -fcs=tracks_file.txt [-display -v=DEBUG] \n";
  cout<< "\t\t  fcs    - file in CS format \n";
  cout<< "\t\t  fbt    - file with basetracks in \"Bari format\" \n";
  cout<< "\t\t  display- start interactive event display \n";
  cout<< "\t\t  DEBUG - verbosity level: 0-print nothing, 1-errors only, 2-normal, 3-print all messages\n";
  
  cout<< "\nCS format:\n\t BRICK EVENT \n\t x y z tx ty nbt rank idtr idtrG \n\t ...\n"; 
  cout<< "\nExample: \n";
  cout<< "\t  emvtx -fcs=tracks_ev1.txt -v=2\n";
  
  cout<< "\n If the parameters file (emvtx.rootrc) is not presented - the default \n";
  cout<< " parameters are used. After the execution them will be saved into emvtx.save.rootrc\n";
  cout<<endl;
}

//---------------------------------------------------------------------
void set_default(TEnv &cenv)
{
  // default parameters for the new alignment
  cenv.SetValue("emvtx.CS.minRank",   0 );
  cenv.SetValue("emvtx.CS.maxImp", 1000);
}

//---------------------------------------------------------------------
void vd( const char *dsname,  EdbVertexRec *evr )
{
  // draw vertexes with multiplicity>=trmin, and aperture >= amin
  int   trmin=2;
  float amin=.0;

  TObjArray *varr = new TObjArray();
  TObjArray *tarr = new TObjArray();

  EdbVertex *v=0;
  EdbTrackP *t=0;

  int nv = evr->Nvtx();
  printf("nv=%d\n",nv);
  if(nv<1) return;

  ///for(int i=0; i<nv; i++) {
  for(int i=nv-1; i<nv; i++) {
      v = (EdbVertex *)(evr->eVTX->At(i));
    //if(v->Flag()<0)         continue;
    //if( v->N()<trmin )                       continue;
    //if( v->N()<3 && v->MaxAperture()<amin )  continue;
    varr->Add(v);
    for(int j=0; j<v->N(); j++) tarr->Add( v->GetTrack(j) );
  }

//   TObjArray *tsegG = new TObjArray();
//   for(int i=0; i<gAliV->Ntracks(); i++) {
//     EdbSegG *sg = new EdbSegG( *( gAliV->GetTrack(i) ) );
//     sg->SetLineColor(kRed);
//     tsegG->Add(sg);
//   }

  gStyle->SetPalette(1);

  EdbDisplay *ds = EdbDisplay::EdbDisplayExist(dsname);
  if(!ds)  ds=new EdbDisplay(dsname,-10000.,10000.,-10000.,10000.,-10000., 10000.);
  ds->SetVerRec(evr);
  ds->SetArrTr( tarr );
  printf("%d tracks to display\n", tarr->GetEntries() );
  ds->SetArrV( varr );
  printf("%d vertex to display\n", varr->GetEntries() );
  //ds->SetArrSegG( tsegG );
  //printf("%d primary tracks to display\n", tsegG->GetEntries() );
  ds->SetDrawTracks(14);
  ds->SetDrawVertex(1);
//   //ds->SetView(90,180,90);

  ds->GuessRange(10000,2000,30000);
  ds->SetStyle(1);
  ds->Draw();

//  float s[3] = {0,0,0 };
  //float e[3] = {Vmc[0],Vmc[1],Vmc[2]+600};
  //ds->DrawRef(Vmc,e);
}

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
int readCSdata( const char *fname, TObjArray &tracks, TArrayL64 &idArr, int minRank )
{
//
  printf("open file (expect CS format): %s\n",fname);
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
    if(rank<minRank) continue;
    idArr[ntr] = idtrG;
    tracks.Add( make_seg_track( idtr,   x, y, 0,  tx, ty, 1.5,0) );
    ntr++;
  }
  fclose(f);
  return ntr;
}

//-----------------------------------------------------------------------------
int readBTdata(const char *fname, TObjArray &tracks, TObjArray &aux_tr )
{
  printf("open file (expect Bari format): %s\n",fname);
  FILE *f = fopen( fname, "r" );                          if(!f) return -1;
  char    buffer[256];

  // read brick and event
  Long_t BRICK=0, NTR=0;
  if(!fgets (buffer, 256, f))                                    return -1;
  if ( sscanf(buffer,"%ld %ld", &BRICK,&NTR) !=2 )               return -1;
  printf("BRICK: %ld   NTR: %ld\n",BRICK,NTR);

  // read CS tracks
  int ntr=0;
  int idtr, plate;
  float x,y,z,tx,ty, p;
  if(gEDBDEBUGLEVEL>1) printf(" idtr         X            Y            Z      TX       TY         P    idpl\n");
  while( fgets (buffer, 256, f) ) {
    if ( sscanf(buffer,"%d %f %f %f %f %f %f %d", &idtr, &x, &y, &z, &tx, &ty, &p, &plate) !=8 ) break;
    if(idtr>=0) tracks.Add( make_seg_track( idtr,   x, y, z,  tx, ty, p,0) );
    else        aux_tr.Add( make_seg_track( idtr,   x, y, z,  tx, ty, p,0) );
    if(gEDBDEBUGLEVEL>1) printf("%5d %12.2f %12.2f %12.2f %8.4f %8.4f %8.2f %4d\n"
                                  ,idtr, x, y, z, tx, ty, p, plate );
    ntr++;
  }
  if(NTR!=ntr) Log(1,"readVTXdata","Warning! expected number of tracks is different: %d %d", NTR, ntr);
  fclose(f);
  return ntr;
}

//-----------------------------------------------------------------------------
void MakeScanCondBT(EdbScanCond &cond)
{
  cond.SetSigma0( 1., 1., 0.002, 0.002);
  cond.SetDegrad( 5. );
  cond.SetBins(3, 3, 3, 3);
  cond.SetPulsRamp0(  12., 18. );
  cond.SetPulsRamp04( 12., 18. );
  cond.SetChi2Max( 6.5 );
  cond.SetChi2PMax( 6.5 );
  cond.SetChi2Mode( 3 );
  cond.SetRadX0( 5810. );
  cond.SetName("OPERA_basetrack");
}

//-----------------------------------------------------------------------------
void MakeScanCondCS(EdbScanCond &cond)
{
  cond.SetSigma0( 10., 10., 0.006, 0.006);
  cond.SetDegrad( 5. );
  cond.SetBins(3, 3, 3, 3);
  cond.SetPulsRamp0(  12., 18. );
  cond.SetPulsRamp04( 12., 18. );
  cond.SetChi2Max( 6.5 );
  cond.SetChi2PMax( 6.5 );
  cond.SetChi2Mode( 3 );
  cond.SetRadX0( 5810. );
  cond.SetName("OPERA_CS_prediction");
}

//-----------------------------------------------------------------------------
void SetTracksErrors(TObjArray &tracks, EdbScanCond &cond)
{
  int n = tracks.GetEntries();
  for(int i=0; i<n; i++) {
     EdbTrackP *t = (EdbTrackP*)tracks.At(i);
     int nseg = t->N();
     for(int j=0; j<nseg; j++) {
       EdbSegP   *s = t->GetSegment(j);
       s->SetErrors0();
       cond.FillErrorsCov( s->TX(),s->TY(), s->COV() );
     }
     t->FitTrackKFS();
  }
}

//-----------------------------------------------------------------------------
Long64_t FindID(EdbTrackP *t, TObjArray &tracks, TArrayL64 &idArr)
{
  int n = tracks.GetEntries();
  for(int i=0; i<n; i++) {
    if(t == tracks.At(i)) return idArr[i];
  }
  return -1;
}

//-----------------------------------------------------------------------------
EdbVertex *vtxCS(const char *file, TEnv &env)
{
  EdbVertex *v=0;
  TObjArray tracks;
  TArrayL64 idArr(100);
  int   minRank = env.GetValue("emvtx.CS.minRank",   0  );
  float maxImp  = env.GetValue("emvtx.CS.maxImp", 1000. );
  int ntrCS = readCSdata( file, tracks, idArr, minRank );
  EdbScanCond cond;
  MakeScanCondCS(cond);
  SetTracksErrors(tracks,cond);
  EdbPVRec *pvr = new EdbPVRec();

  if(ntrCS < 0)  { printf("bad file - skip\n"); goto OUTPUT; }

  if(ntrCS<2) goto OUTPUT;

  gEVRCS = new EdbVertexRec();
  gEVRCS->eProbMin   = 0;      // minimum acceptable probability for chi2-distance between tracks
  gEVRCS->eImpMax    = 15000;   // maximal acceptable impact parameter (preliminary check)
  gEVRCS->eUseSegPar = 0;      // use only the nearest measured segments for vertex fit
  gEVRCS->eUseMom    = 1;      // use or not track momentum for vertex calculations
  
  
  pvr->SetScanCond( new EdbScanCond(cond) );
  gEVRCS->SetPVRec(pvr);

  v = gEVRCS->Make1Vertex( tracks, 0. );
  if(v) v=gEVRCS->StripBadTracks( *v, maxImp, 3 );
  
OUTPUT:
  
  FILE *f = fopen( Form("%s.vtx",file), "w" );
  
  int ntrv = v? v->N() : 0;
  fprintf(f,"%5d\n",ntrCS);
  if(ntrv) {
    fprintf(f,"%5d %10.2f  %10.2f    %12.2f %12.2f %12.2f\n", 
    ntrv, v->MaxImpact(), v->V()->prob(),
    v->VX(), v->VY(), v->VZ() );
    fprintf(f,"\t\t\t\t  %10.2f   %10.2f   %10.2f\n", v->V()->vxerr(), v->V()->vyerr(), v->V()->vzerr() );
    for(int i=0; i<v->N(); i++) {
      EdbTrackP *t = v->GetTrack(i);
      fprintf(f,"%5d  %12.2f   %llu\n", t->ID(), v->Impact(i), FindID(t,tracks,idArr) );
    }
  }
  fclose(f);
  
  return v;
}

//-----------------------------------------------------------------------------
EdbVertex *vtxBT1(TObjArray &tracks, TEnv &env)
{
  int ntr = tracks.GetEntries();   if(ntr<2) return 0;
  
  EdbScanCond cond;
  MakeScanCondBT(cond);
  SetTracksErrors(tracks,cond);
  
  SafeDelete(gEVRCS);
  gEVRCS = new EdbVertexRec();
  gEVRCS->eProbMin   = 0;      // minimum acceptable probability for chi2-distance between tracks
  gEVRCS->eImpMax    = 150;   // maximal acceptable impact parameter (preliminary check)
  gEVRCS->eUseSegPar = 0;      // use only the nearest measured segments for vertex fit
  gEVRCS->eUseMom    = 1;      // use or not track momentum for vertex calculations
  
  EdbPVRec *pvr = new EdbPVRec();
  pvr->SetScanCond( new EdbScanCond(cond) );
  gEVRCS->SetPVRec(pvr);

  EdbVertex *v = gEVRCS->Make1Vertex( tracks, -6000. ); if(!v) return 0;
  //v=gEVRCS->StripBadTracks( *v, 200., 3 );          if(!v) return 0;
  return v;
}

//-----------------------------------------------------------------------------
void vtxBTcomb(const char *file, TEnv &env)
{
  TObjArray tracks;
  TObjArray aux_tr;
  int ntr = readBTdata( file, tracks, aux_tr );
  if(ntr < 0)  { printf("bad file - skip\n"); return; }
  if(ntr<2) return;
 
  TObjArray vertices1, vertices2;

  int ic=0;
  for( int nitems=ntr; nitems >= ntr/2; nitems-- )
  {
     EdbCombGen comber(tracks,nitems);
     TObjArray selected, other;
     while( comber.NextCombination(selected,other) ) {
       EdbVertex *v1 = vtxBT1(selected, env);
       vertices1.Add(v1);
       EdbVertex *v2 = vtxBT1(other, env);
       vertices2.Add(v2);
       ic++;
     }
   }
  
  TArrayF probarr(ic);
  TArrayI ind(ic);
  for(int i=0; i<ic; i++) {
    EdbVertex *v1 = (EdbVertex*)vertices1.At(i);
    EdbVertex *v2 = (EdbVertex*)vertices2.At(i);
    float prob = v1->V()->prob();
    if( v2 ) prob *= v2->V()->prob();
    probarr[i]= prob;
  }
  Sort(ic,probarr.GetArray(),ind.GetArray(),1);
  
  // remove duplications (possible in case of equal groups)
  for(int i=0; i<ic-1; i++) 
  {
    int i1 = ind[i];
    int i2 = ind[i+1];
    if(i1<0) continue;
    if(i2<0) continue;
    if( Abs( probarr[i1]-probarr[i2] ) < 0.00000001 )  ind[i+1]=-1;
  }
  
  //printf("\n    #   prob1         Z1    prob2         Z2           set1                                  set2\n");
  printf("\n    #   prob1          X1        Y1       Z1    prob2           X2        Y2       Z2           set1                                  set2\n");
  printf("-----------------------------------------------------------------------------------------------------------------------------------------------\n");
  float prob_pred=1, prob_lim=0.001;
  for(int i=0; i<ic; i++) 
  {
    if(ind[i]<0)   continue;
    if(prob_pred>prob_lim && probarr[ind[i]] < prob_lim)
       printf("      ----- below the probability product is less then %f ------\n",prob_lim );
    prob_pred=probarr[ind[i]];
    EdbVertex *vv1 = (EdbVertex*)vertices1.At(ind[i]);
    EdbVertex *vv2 = (EdbVertex*)vertices2.At(ind[i]);
    EdbVertex *v1=0, *v2=0; 
    if(!vv2) v1 = vv1;
    else {                                                    // the primary is first
      if( vv1->VZ() < vv2->VZ() ) {v1 = vv1; v2 = vv2; }
      else                        {v2 = vv1; v1 = vv2; }
    }
    printf("%5d",i);
    printf("  %9.7f %9.1f %9.1f %9.1f", v1->V()->prob(), v1->VX(), v1->VY(), v1->VZ());
    if(v2) printf("  %9.7f %9.1f %9.1f %9.1f",v2->V()->prob(), v2->VX(), v2->VY(), v2->VZ());
    else   printf("                                      ");
    printf("\t(");
    for(int i=0; i<v1->N(); i++) printf("%6d", v1->GetTrack(i)->ID() );
    for(int i=0; i<ntr-v1->N(); i++) printf("      " );
    printf(") & (");
    if(v2) for(int i=0; i<v2->N(); i++) printf("%6d", v2->GetTrack(i)->ID() );
    printf(")\n");
  }
       
  printf("\n---------------- The most probable combinations --------------------\n" );
  for(int i=0; i<ic; i++)  
  {
    if(ind[i]<0)   continue;
    if(probarr[ind[i]] < 0.01) continue;
    
    printf("\n----------- the probability product is %f :----------\n", probarr[ind[i]]);

    EdbVertex *vv1 = (EdbVertex*)vertices1.At(ind[i]);  vv1->Print();
    EdbVertex *vv2 = (EdbVertex*)vertices2.At(ind[i]);  vv2->Print();
    
    int naux = aux_tr.GetEntries();
    for(int j=0; j<naux; j++) 
    {
      EdbTrackP *t = (EdbTrackP*)(aux_tr.At(j));
      printf("Impact (v1, %d) = %f %f\n", t->ID(), vv1->CheckImp(t), vv1->CheckImpGeom(t) );
      printf("Impact (v2, %d) = %f %f\n", t->ID(), vv2->CheckImp(t), vv2->CheckImpGeom(t) );
    }
  }
  
  const EdbScanCond *cond = gEVRCS->ePVR->GetScanCond();
  printf("\n The measurements errors assigned to tracks at last measured point:\n");
  printf("  sigmaX0  sigmaY0  sigmaTX0  sigmaTY0   Degrad\n");
  printf("%8.3f %8.3f  %8.4f  %8.4f %8.2f\n",
    cond->SigmaX(0),cond->SigmaY(0),cond->SigmaTX(0),cond->SigmaTY(0),cond->Degrad() );
  printf("The errors extrapolated to the vertex point taking into account CMS with Rad length of the media  X0 = %10.2f [microns]\n",cond->RadX0());
}


//-----------------------------------------------------------------------------
EdbVertex *vtxBT(const char *file, TEnv &env)
{
  TObjArray tracks, aux_tr;
  int ntrBT = readBTdata( file, tracks, aux_tr );
  if(ntrBT < 0)  { printf("bad file - skip\n"); return 0; }
  EdbScanCond cond;
  MakeScanCondBT(cond);
  SetTracksErrors(tracks,cond);

  return vtxBT1(tracks, env);
}


//-----------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }

  bool        do_fcs     = false;
  bool        do_fbt     = false;
  bool        do_display = false;
  const char *filecs=0;
  const char *filebt=0;
  
  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];
    if(!strncmp(key,"-fcs=",5))
      {
	if(strlen(key)>5)  { filecs = key+5;  do_fcs=true; }
      }
    else if(!strncmp(key,"-fbt=",5))
      {
	if(strlen(key)>5)  { filebt = key+5;  do_fbt=true; }
      }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
    else if(!strncmp(key,"-display",8))
      {
        do_display=true;
      }
  }
  
  set_default(cenv);
  cenv.ReadFile("emvtx.rootrc" ,kEnvLocal);
  
  if(do_fcs) 
  {
    EdbVertex *v = vtxCS( filecs, cenv );
    v->Print();
  }
   
  if(do_fbt) 
  {
    //EdbVertex *v = vtxBT( filebt, cenv );
    //v->Print();
    vtxBTcomb( filebt, cenv );
  }
   
  cenv.WriteFile("emvtx.save.rootrc");
 
  if(do_display)
  {
      int argc2=1;
      char *argv2[]={"-l"};
      TRint app("APP",&argc2, argv2);

      vd("display",gEVRCS);
      app.Run();
  }

  
  return 0;
}
