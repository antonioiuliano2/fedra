//-- Author :  Valeri Tioukov   22/01/2015
#include <assert.h>
#include <string.h>
#include <iostream>
#include <TRint.h>
#include <TEnv.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbCouplesTree.h"

using namespace std;

EdbScanProc sproc;
EdbScanSet *set = 0;

void DoTraceBack( EdbID idset, TEnv &cenv, EdbID *idnew=0 );
void GroupViews( EdbPattern &p, EdbCell1 &c );
EdbID GetCPID(const EdbPattern &p);
void GroupViews( EdbID &cpid, const EdbPattern &p, const EdbPattern &pp1, const EdbPattern &pp2, EdbCell1 &cv1, EdbCell1 &cv2 );
void WriteRaw( EdbRunAccess &run, EdbRun &runout, EdbCell1 &cv, int rs );

void print_help_message()
{
  cout<< "\n For a given dataset produce *.tr.raw.root files in plate \n";
  cout<<   " directories conserving volume tracks raw segments only\n";
  cout<<   " If -newset option is defined - save data into newid.raw.root files\n";
  cout<< "\nUsage: \n\t  emtrfind -set=idset [-newset=idnew -v=DEBUG] \n";
  cout<< "\nExample: \n";
  cout<< "\t  emtraceback -set=4554.0.1.1 -v=2\n";
  cout<< "\n If the data location directory if not explicitly defined\n";
  cout<< " the current directory will be assumed to be the brick directory \n";
  cout<< "\n If the parameters file (traceback.rootrc) is not presented - the default \n";
  cout<< " parameters will be used. After the execution them are saved into traceback.save.rootrc file\n";
  cout<<endl;
}

void set_default(TEnv &cenv)
{
  // default parameters for the new alignment
  cenv.SetValue("emtraceback.tracks_read_cut" , "1");
  cenv.SetValue("emtraceback.RS"              , 0);
  cenv.SetValue("emtraceback.outdir"          , ".."  );
  cenv.SetValue("emtraceback.env"             , "traceback.rootrc");
  cenv.SetValue("emtraceback.EdbDebugLevel"   ,  1    );
}


int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("emtraceback");
  set_default(cenv);
  gEDBDEBUGLEVEL     = cenv.GetValue("emtraceback.EdbDebugLevel" , 1);
  const char *env    = cenv.GetValue("emtraceback.env"            , "trfind.rootrc");
  const char *outdir = cenv.GetValue("emtraceback.outdir"         , "..");

  bool      do_set      = false;
  EdbID     *id_new=0;
  EdbID     idset,idnew;

  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];
  
    if     (!strncmp(key,"-set=",5)) 
    {
      if(strlen(key)>5) if(idset.Set(key+5))   do_set=true;
    }
    else if     (!strncmp(key,"-newset=",8)) 
    {
      if(strlen(key)>8) if(idnew.Set(key+8))   id_new = &idnew;
    }
    else if(!strncmp(key,"-v=",3))
    {
      if(strlen(key)>3) gEDBDEBUGLEVEL = atoi(key+3);
    }
  }
  
  if(!(do_set))   { print_help_message(); return 0; }

  cenv.SetValue("emtraceback.env"            , env);
  cenv.ReadFile( cenv.GetValue("emtraceback.env"   , "traceback.rootrc") ,kEnvLocal);
  cenv.SetValue("emtraceback.outdir"         , outdir);
  sproc.eProcDirClient = cenv.GetValue("emtraceback.outdir","..");
  cenv.WriteFile("traceback.save.rootrc");
  

  printf("\n----------------------------------------------------------------------------\n");
  printf("Copy raw segments of reconstructed tracks from %s to %s\n"   ,idset.AsString(), idnew.AsString());
  printf("----------------------------------------------------------------------------\n\n");

  DoTraceBack( idset, cenv, id_new );

  return 0;
}

//----------------------------------------------------------------
void DoTraceBack( EdbID idset, TEnv &cenv, EdbID *id_new )
{
  int          rs = cenv.GetValue("emtraceback.RS"        , 0);
  const char *cut = cenv.GetValue("emtraceback.tracks_read_cut"        , "1");

  set = sproc.ReadScanSet(idset);  assert(set);
  EdbPVRec ali;
  sproc.ReadTracksTree( idset,ali, cut );
  
  int npat = ali.Npatterns();
  for(int i=0; i<npat; i++)
  {
    EdbPattern *p = ali.GetPattern(i);
    EdbID cpID = GetCPID(*p);

    TString cpfile;
    sproc.MakeFileName(cpfile, cpID,"cp.root");
    EdbCouplesTree cptree;
    cptree.InitCouplesTree("couples",cpfile.Data());
    EdbPattern pp,pp1,pp2;
    cptree.GetCPData( &pp,&pp1,&pp2 );

    EdbCell1 cviews1; cviews1.InitCell(200, 50000, 0, 50000 );
    EdbCell1 cviews2; cviews2.InitCell(200, 50000, 0, 50000 );
    GroupViews( cpID,*p,pp1, pp2, cviews1, cviews2 );

    EdbRunAccess run;
    sproc.InitRunAccessNew(run, idset, cpID.ePlate);
    if(id_new) {
      EdbID idn=(*id_new); idn.ePlate=cpID.ePlate;
      sproc.MakeFileName(cpfile, idn,"raw.root");
    } else {
      sproc.MakeFileName(cpfile, cpID,"tr.raw.root");
    }
    EdbRun runout( cpfile.Data(),"RECREATE" );

    WriteRaw( run, runout, cviews1, rs );
    WriteRaw( run, runout, cviews2, rs );
    runout.Close();
  }

}

//------------------------------------------------------------------------------------------
EdbID GetCPID(const EdbPattern &p)
{
  int nseg = p.N();
  EdbID id;
  int cnt=0;
  for(int isg=0; isg<nseg; isg++)
  {
    if( id != p.GetSegment(isg)->ScanID() )
    {
      id = p.GetSegment(isg)->ScanID();
      cnt=1;
    } else {
      cnt++;
    }
  }
  Log(3,"GetCPID","cnt = %d for %s",cnt, id.AsString() );
  return id;
}

//------------------------------------------------------------------------------------------
void GroupViews( EdbID &cpid, const EdbPattern &p, const EdbPattern &pp1, const EdbPattern &pp2, EdbCell1 &c1, EdbCell1 &c2 )
{
  int nseg = p.N();
  Log(3,"GroupViews","%d segments to read ",nseg );

  //c1.Print();
  for(int isg=0; isg<nseg; isg++)
  {
    int entr = p.GetSegment(isg)->ID();
    if( !c1.AddObject( pp1.GetSegment(entr)->Vid(0),  pp1.GetSegment(entr)) )
      Log(1,"GroupViews","ERROR: can't add segment to c1 %d",
          pp1.GetSegment(entr)->Vid(0) );
    if( !c2.AddObject( pp2.GetSegment(entr)->Vid(0),  pp2.GetSegment(entr)) )
      Log(1,"GroupViews","ERROR: can't add segment to c2 %d",
        pp2.GetSegment(entr)->Vid(0) );
  }

  Log(2,"GroupViews","%d segments read from tree with %d entries",nseg, pp1.N() );
}

//-------------------------------------------------------------------------------------------------
void WriteRaw(  EdbRunAccess &run, EdbRun &runout, EdbCell1 &cv, int rs )
{
  int nv = cv.Ncell();
  for( int iv=0; iv<nv; iv++ ) {
    int nsv=cv.Bin(iv);
    if(!nsv)  continue;
    
    EdbView *vin  = run.GetRun()->GetEntry(iv);
    EdbView *vout = runout.GetView();
    vout->GetHeader()->Copy( vin->GetHeader() );
    
    for( int is=0; is<nsv; is++ ) {
      EdbSegP *sp = (EdbSegP *)(cv.GetObject(iv,is));
      
      EdbSegment *sraw = vin->GetSegment(sp->Vid(1));
      if(rs>0) run.ApplyCorrections( *vin, *sraw, rs );

      if(gEDBDEBUGLEVEL>3) {
        printf("\n");
        sp->PrintNice();
        sraw->Print();
      }

      vout->AddSegment(sraw);
      
      EdbTrack t( sp->X(), sp->Y(), sp->Z(), sp->TX(), sp->TY(),sp->DZ(), sp->ID() ); 
      vout->AddTrack(&t);  // save sp for crosscheck. Note that the last emlink should be done with all corrections off for consistency

    }
    runout.AddView(vout);
  }
}
