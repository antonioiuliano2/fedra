#include "TOracleServerE2WFB.h"
#include "EdbLog.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbSegment.h"
#include "EdbRun.h"
#include "TTimeStamp.h"

ClassImp(TOracleServerE2WFB)
ClassImp(EdbFeedback)

//------------------------------------------------------------------------------------
void TOracleServerE2WFB::Set0()
{
  eDoCommit=0;
}
 
//------------------------------------------------------------------------------------
void TOracleServerE2WFB::Print()
{
  TOracleServerE2::Print();
  printf( " eDoCommit = %d  \n eLab = %s   \n eLa = %s\n", eDoCommit, eLab.Data(), eLa.Data() );
}
 
//------------------------------------------------------------------------------------
const char *TOracleServerE2WFB::Timestamp()
{
  TTimeStamp timestamp;
  return Form("timestamp'%s'",timestamp.AsString("s"));
}
//------------------------------------------------------------------------------------
const char *TOracleServerE2WFB::Ostr(ULong64_t num)
{
    // return "null" if arg<=0;
  return num>0 ? Form("%lld",num) : Form("null");
}
//------------------------------------------------------------------------------------
const char *TOracleServerE2WFB::Ostr(Int_t num)
{
    // return "null" if arg<=0;
  return num>0 ? Form("%d",num) : Form("null");
}

//------------------------------------------------------------------------------------
Bool_t TOracleServerE2WFB::FinishTransaction()
{
  try{
    if(eDoCommit) return MyQuery("COMMIT"); 
    else          return MyQuery("ROLLBACK");
  } catch (SQLException &oraex) {
    Log(1,"TOracleServerE2WFB::FinishTransaction","failed: (error: %s)",(oraex.getMessage()).c_str());
    return 0;
  }
  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::MyQuery(const char *query)
{
  try{
    if (!fStmt)   fStmt = fConn->createStatement();
    fStmt->setSQL(query);
    Log(2,"TOracleServerE2WFB::MyQuery","execute sql query: %s ...",query);
    fStmt->execute();
  } catch (SQLException &oraex) {
    Log(1,"TOracleServerE2WFB::MyQuery","failed: (error: %s)",(oraex.getMessage()).c_str());
    eDoCommit=0;
    return 0;
  }
  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::AddEventBrick(const char *databrick)
{
  // Adds a brick into the DB
  
  if(!MyQuery(Form( "\
      INSERT INTO OPERA.PV_%s_EVENTBRICKS \
      (ID, MINX, MAXX, MINY, MAXY, MINZ, MAXZ, ID_SET, ID_BRICK, ZEROX, ZEROY, ZEROZ) VALUES (%s)", eLa.Data(),
  databrick )))  return 0;
  Log(2,"TOracleServerE2WFB:AddEventBrick","Brick added");
  return 1;
}

//------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WFB::IfEventBrick(ULong64_t id_eventbrick, const char *id_set )
{
  // Check if brick structure is in DB
  
  if(MyQuery(Form( "\
     SELECT * FROM OPERA.PV_%s_EVENTBRICKS \
     WHERE ID=%lld and ID_SET=%s", 
  eLa.Data(),  id_eventbrick,id_set))) {
    if(fStmt) {
      ResultSet *rs = fStmt->getResultSet();
      if(rs) {
        TString result;
        int nrows = PrintResultStr(result);
        if(nrows>0) {
          Log(2,"TOracleServerE2WFB::IfEventBrick","brick %lld of set %s is found in DB!",id_eventbrick,id_set);
          printf( "%s",result.Data() );
          return nrows;
        }
      }
    }
  }
  Log(1,"TOracleServerE2WFB::IfEventBrick","brick %lld of set %s is not found in DB!",id_eventbrick,id_set);
  return 0;
}

//------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WFB::IfEventRec(ULong64_t id_eventbrick)
{
  // Check if reconstruction was already published
  
  if(MyQuery(Form( "select * from vw_feedback_reconstructions where id_eventbrick=%lld",
  id_eventbrick))) {
    if(fStmt) {
      ResultSet *rs = fStmt->getResultSet();
      if(rs) {
        TString result;
        int nrows = PrintResultStr(result);
        if(nrows>0) {
          Log(2,"TOracleServerE2WFB::IfEventRec","%d reconstructions are found for brick %lld in DB!", nrows, id_eventbrick);
          printf( "%s",result.Data() );
          return nrows;
        }
      }
    }
  }
  Log(1,"TOracleServerE2WFB::IfEventBrick","brick %lld is not found in DB!",id_eventbrick);
  return 0;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::AddPlate(ULong64_t id_eventbrick, const char *dataplate)
{
  // Adds a plate into the DB
  // Table involved: TB_PLATE

  if(!MyQuery(Form( "\
      INSERT INTO OPERA.PV_%s_PLATES (ID_EVENTBRICK, ID, Z) \
      VALUES (%s, %s)", eLa.Data(),
  Ostr(id_eventbrick),
  dataplate)))   return 0;
  Log(2,"TOracleServerE2WFB::AddPlate","Plate added");
  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::AddTemplateMarkSets(const char *datamarks)
{
  // Adds a mark into the DB
  // Table involved: TB_TEMPLATEMARKSETS

  if(!MyQuery(Form( "\
      INSERT INTO OPERA.PV_%s_TEMPLATEMARKSETS\
      (ID_EVENTBRICK, ID_MARK, POSX, POSY, MARKROW, MARKCOL, SHAPE, SIDE)\
      VALUES (%s)", eLa.Data(),
  datamarks)))  return 0;
  Log(2,"TOracleServerE2WFB::AddTemplateMarkSets","TemplateMarkSets added");
  return 1;
}

//-------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WFB::AddProcessOperationBrick( 
    ULong64_t id_machine,
    ULong64_t id_programsettings, 
    ULong64_t id_requester, 
    ULong64_t id_eventbrick, 
    ULong64_t id_parent_operation,
    const char *starttime,
    const char *notes)
{
  ULong64_t id=0;
  try{
    TString s;
    s+=Form(
        "BEGIN  %s.PC_ADD_PROC_OPERATION_BRICK(%s, %s, %s, %s, %s, %s, '%s', :1); END;", eLab.Data(),
    Ostr(id_machine), 
    Ostr(id_programsettings), 
    Ostr(id_requester), 
    Ostr(id_eventbrick), 
    Ostr(id_parent_operation),
    starttime, 
    notes
           );
    
    printf("%s\n",s.Data() );
    if (!fStmt)  fStmt = fConn->createStatement();
    fStmt->setSQL( s.Data() );
    fStmt->registerOutParam(1, OCCISTRING,64);
    fStmt->execute();
    if(!(fStmt->getString(1)).c_str()) {
      Log(2,"TOracleServerE2WFB::AddProcessOperationBrick","ERROR! empty operation returned!");
      return 0;    eDoCommit=0;
    }
    Log(2,"TOracleServerE2WFB::AddProcessOperationBrick","returned string is: %s",(fStmt->getString(1)).c_str());
    sscanf( (fStmt->getString(1)).c_str(),"%lld",&id);
    Log(2,"TOracleServerE2WFB::AddProcessOperation","output: %lld",id);
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WFB", "AddProcessOperation failed: (error: %s)", (oraex.getMessage()).c_str());
    eDoCommit=0;
  }
  return id;
}

//------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WFB::AddFeedbackReconstructionTest // do not work due to some triggers
    (
    ULong64_t id_eventbrick,
    ULong64_t id_processoperation
    )
{
  ULong64_t id=0;
  try{
    TString s=Form(
        "BEGIN INSERT INTO %s.VW_FEEDBACK_RECONSTRUCTIONS_J(id_eventbrick,id_processoperation) values(%s,%s) returning id_reconstruction into :1; END;",
    eLab.Data(),
    Ostr(id_eventbrick),
    Ostr(id_processoperation)
                  );
    Log(2,"","%s",s.Data() );
    fStmt->setSQL( s.Data() );
    fStmt->registerOutParam(1, OCCISTRING,64);
    fStmt->executeUpdate();
    if(!(fStmt->getString(1)).c_str()) {
      Log(2,"TOracleServerE2WFB::AddProcessOperationBrick","ERROR! empty operation returned!");
      eDoCommit=0;
      return 0;
    }
    Log(2,"","%s", (fStmt->getString(1)).c_str() );
    sscanf( (fStmt->getString(1)).c_str(),"%lld",&id);
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WFB::AddFeedbackReconstruction", "failed: (error: %s)", (oraex.getMessage()).c_str());
    eDoCommit=0;
  }
  return id;
}

//------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WFB::AddFeedbackReconstruction
    (
    ULong64_t id_eventbrick,
    ULong64_t id_processoperation
    )
{
    if(!MyQuery( Form
        (
        "INSERT INTO %s.VW_FEEDBACK_RECONSTRUCTIONS_J(id_eventbrick,id_processoperation) values(%s,%s)", eLab.Data(),
    Ostr(id_eventbrick),
    Ostr(id_processoperation)
        ))) return 0;
    if(!MyQuery( Form
        (
        "select id_reconstruction from %s.VW_FEEDBACK_RECONSTRUCTIONS_J where  id_eventbrick=%s and id_processoperation=%s", eLab.Data(),
    Ostr(id_eventbrick),
    Ostr(id_processoperation)
        ))) return 0;
    ULong64_t id=0;
    if(fStmt) {
      ResultSet *rs = fStmt->getResultSet();
      if(rs) 
        if(rs->next()) {
          sscanf( (rs->getString(1)).c_str(),"%lld",&id);
          Log(2,"TOracleServerE2WFB::AddFeedbackReconstructionOld","added, id = %lld",id);
        }
    }
    return id;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::AddFeedbackVertex(
    ULong64_t   id_eventbrick,
    ULong64_t   id_reconstruction,
    Int_t       id_vertex,
    Float_t     posx,
    Float_t     posy,
    Float_t     posz,
    const char *isprimary,
    const char *ischarm,
    const char *istau, 
    const char *outofbrick
                                            )
{
  TString query;
  query+=Form("insert into %s.vw_feedback_vertices_j ", eLab.Data());
  query+="(id_eventbrick,id_reconstruction,id_vertex,posx,posy,posz,isprimary,ischarm,istau, outofbrick) ";
  query+=Form("values(%s, %s, %s, %12.2f, %12.2f, %12.2f, %s, %s, %s, %s)",
              Ostr(id_eventbrick),
              Ostr(id_reconstruction),
              Ostr(id_vertex),
              posx,posy,posz,isprimary,ischarm,istau, outofbrick);
  if(!MyQuery(query.Data())) { return 0;}
  Log(3,"TOracleServerE2WFB::AddFeedbackVertex","ok");
  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::AddFeedbackTrack(
        ULong64_t   id_eventbrick, 
        ULong64_t   id_reconstruction, 
        Int_t       id_track, 
        Int_t       id_upvtx, 
        Int_t       id_downvtx,
        Float_t x, 
        Float_t y, 
        Float_t z, 
        Float_t sx, 
        Float_t sy, 
        const char *manual, 
        const char *particle, 
        const char *scanback, 
        const char *darkness,
        Float_t upip, 
        Float_t downip, 
        Float_t p, 
        Float_t pmin, 
        Float_t pmax,
        const char *outofbrick,
        Int_t lastplate,
        Float_t rslopet,
        Float_t rslopel,
        Float_t rmsslopet,
        Float_t rmsslopel,
        Int_t kinkplatedown,
        Int_t kinkplateup,
        const char *decaysearch,
        ULong64_t   event
                                            )
{
  TString query;
  query+=Form("insert into %s.vw_feedback_tracks_j (",eLab.Data());
  query+="id_eventbrick, id_reconstruction, id_track, id_upvtx, id_downvtx,";
  query+="x, y, z, sx, sy, manual, particle, scanback, darkness,upip, downip, p, pmin, pmax,outofbrick,lastplate,";
  query+="rslopet,rslopel,rmsslopet,rmsslopel,kinkplatedown,kinkplateup,decaysearch,event";
  query+=Form(") values(\
      %s, %s, %s, %s, %s, \
      %12.2f, %12.2f, %12.2f, %8.5f, %8.5f, %s, %s, %s, %s, \
      %10.3f, %10.3f, %10.3f, %10.3f, %10.3f, %s, %d, %8.5f, %8.5f, %8.5f, %8.5f, \
      %s, %s, %s, %lld )",
  Ostr(id_eventbrick), 
  Ostr(id_reconstruction), 
  Ostr(id_track), 
  Ostr(id_upvtx), 
  Ostr(id_downvtx),
  x, y, z, sx, sy, 
  manual, particle, scanback, darkness,
  upip,downip, p, pmin, pmax,outofbrick,lastplate,rslopet,rslopel,rmsslopet,rmsslopel,
  Ostr(kinkplatedown),
  Ostr(kinkplateup),
  decaysearch,
  event
             );

  if(!MyQuery(query.Data())) { return 0;}
  Log(3,"TOracleServerE2WFB::AddFeedbackTrack","ok");
  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::AddFeedbackSegment(
    ULong64_t   id_eventbrick, 
    ULong64_t   id_reconstruction, 
    Int_t       id_plate,
    Int_t       id_track,
    const char *tracktype,
    Float_t posx, 
    Float_t posy, 
    Float_t posz, 
    Float_t slopex, 
    Float_t slopey, 
    Int_t grains,
    const char *trackmode
                                             )
{
  TString query;
  query+=Form("insert into %s.vw_feedback_segments_j",eLab.Data());
  query+="(id_eventbrick, id_reconstruction, id_plate,id_track,tracktype,posx, posy, posz, slopex, slopey, grains,trackmode) ";
  query+=Form("values(\
      %s, %s, %s, %s, %s,\
      %12.2f, %12.2f, %12.2f, %8.5f, %8.5f,\
      %d, %s)",
  Ostr(id_eventbrick), 
  Ostr(id_reconstruction), 
  Ostr(id_plate),
  Ostr(id_track),
  tracktype,posx, posy, posz, slopex, slopey, grains,trackmode);
  
  if(!MyQuery(query.Data())) { return 0;}
  Log(3,"TOracleServerE2WFB::AddFeedbackTrack","ok");
  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WFB::CloseFeedbackDataset(
    ULong64_t id_operation,
    const char *endtime
                                               )
{
  try{
    if (!fStmt)  fStmt = fConn->createStatement();
    fStmt->setSQL( Form(
        "begin %s.PC_SUCCESS_OPERATION( %s, %s ); end;", eLab.Data(),
    Ostr(id_operation),
    endtime
                       )
                 );
    printf("%s\n",fStmt->getSQL().c_str() );
    fStmt->execute();
    return 1;
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WFB", "CloseFeedbackDataset failed: (error: %s)", (oraex.getMessage()).c_str());
    eDoCommit=0;
  }
  return 0;
}

//------------------------------------------------------------------------------------
EdbFeedback::EdbFeedback()
{
  eDB=0;
  eEvent=0;
  eEventBrick=0;
  eProcOp=0;
  eNvtx=0;
  eNvtxLim=1000;
  eNtr=0;
  eNtrLim=5000;
  eV=0;
  eIdMachine=0;
  eIdProgramsettings=0;
  eIdRequester=0;
  eERROR=0;
}

//------------------------------------------------------------------------------------
void EdbFeedback::Print()
{
  printf("\n--------------------------------------------------------\n");
  printf("EdbFeedback: \n\
  \t eEventBrick=%lld \n\
      \t eEvent=%lld \n\
      \t eProcOp=%lld \n\
      \t eNvtx=%d  \n\
      \t eIdMachine=%lld  \n\
      \t eIdProgramsettings=%lld  \n\
      \t eIdRequester=%lld \n", 
  eEventBrick,eEvent,eProcOp,eNvtx,eIdMachine,eIdProgramsettings,eIdRequester);
  if(eDB) eDB->Print();
  printf("--------------------------------------------------------\n\n");
}

//------------------------------------------------------------------------------------
int EdbFeedback::InitDB( const char *conn, const char *user, const char *pwd )
{
  eDB = new TOracleServerE2WFB( conn, user, pwd );
  if(!eDB) return 0;
  return 1;
}

//------------------------------------------------------------------------------------
int EdbFeedback::LoadFBintoDB()
{
  if(eNtr<1)  return 0; 
  if(!eDB)    return 0;
 
  eProcOp = eDB->AddProcessOperationBrick(
      eIdMachine,      // id_machine
  eIdProgramsettings,  // id_programsettings
  eIdRequester,        // id_requester
  eEventBrick,         // id_eventbrick
  0 ,                  // id_parent_operation
  eDB->Timestamp(),    // starttime
  "Feedback");         // notes
 
  eRecID  = eDB->AddFeedbackReconstruction( eEventBrick,eProcOp );
  
  Log(2,"EdbFeedback::LoadFBintoDB","%d tracks to be added to RecID %d",eNtr, eRecID);

  for( int iv=0; iv<eNvtxLim; iv++ ) {
    fbvertex *v = eV[iv];
    if(v) { 
      const char *vprim  = v->isprim? "'Y'" : "'N'";
      const char *vcharm = v->ischarm? "'Y'" : "'N'";
      const char *vtau   = v->istau? "'Y'" : "'N'";
      const char *vout   = v->ofb? "'Y'" : "'N'";
      eDB->AddFeedbackVertex(eEventBrick, eRecID, v->idvtx, v->x, v->y, v->z, vprim,vcharm,vtau,vout);
    }
  }
  
  for(int it=0; it<eNtr; it++ ) {
    fbtrack *t = eT[it];
    if(!t) { 
      Log(1,"EdbFeedback::LoadFBintoDB","ERROR: track %d is missing!", it);
      eERROR=1;
      return 0; 
    }
 
    const char *tman = t->isman? "'Y'" : "'N'";
    const char *tparticle = PartType(t->type);
    const char *tscanback = t->scanback? "'Y'" : "'N'";
    const char *tdarkness = Darkness(t->darkness);
    const char *tofb = Ofb(t->ofb);
    const char *tdecaysearch = DecayFlag(t->flag);

    eDB->AddFeedbackTrack( eEventBrick, eRecID,t->id_track, t->id_upvtx, t->id_downvtx,
                           t->x, t->y, t->z, t->sx, t->sy,
                           tman, tparticle, tscanback, tdarkness,
                           t->upip, t->downip, t->p, t->pmin,  t->pmax,
                           tofb,
                           t->lastplate,
                           t->rslopet , t->rslopel, t->rmsslopet , t->rmsslopel,
                           t->kinkplatedown , t->kinkplateup,
                           tdecaysearch,
                           eEvent
                         );

    for(int is=0; is<t->nseg; is++ ) {
      fbsegment *s = t->segments[is];
      if(!s) { 
        Log(1,"EdbFeedback::LoadFBintoDB","ERROR: segment %d of track %d is missing!", is, it);
        eERROR=1;
        return 0; 
      }
      if(s->id_plate!=44)
        eDB->AddFeedbackSegment( eEventBrick, eRecID, 
                                 s->id_plate, t->id_track, 
                                 SegmentType(s->type),
                                 s->x, s->y, s->z, s->sx, s->sy,
                                 s->grains, 
                                 SegmentRecMode(s->irec)
                               );
    }
  }

  eDB->CloseFeedbackDataset( eProcOp, eDB->Timestamp() );

  return 1;
}

//------------------------------------------------------------------------------------
const char *EdbFeedback::SegmentRecMode(int mode)
{
  if (mode==0) return "'A'";
  if (mode==1) return "'S'";
  if (mode==2) return "'M'";
  return  "null";
}
//------------------------------------------------------------------------------------
const char *EdbFeedback::SegmentType(int type)
{
  if (type==0) return "'B'";
  if (type==1) return "'U'";
  if (type==2) return "'D'";
  return  "null";
}

//------------------------------------------------------------------------------------
const char *EdbFeedback::DecayFlag(int flag)
{
  if (flag==1) return "'PRIMARY'";
  if (flag==2) return "'E+E-'";
  if (flag==3) return "'LOWP'";
  if (flag==4) return "'SF TODO'";
  if (flag==5) return "'SF DONE'";
  return  "null";
}

//------------------------------------------------------------------------------------
const char *EdbFeedback::Ofb(int ofb)
{
  if (ofb==0) return  "'  '";
  if (ofb==1) return  "'PASSING_THROUGH'";
  if (ofb==2) return  "'EDGE_OUT'";
  return  "null";
}

//------------------------------------------------------------------------------------
const char *EdbFeedback::PartType(int partype)
{
  if (partype==1) return  "'MUON'";
  if (partype==2) return  "'CHARM'";
  if (partype==3) return  "'ELECTRON'";
  if (partype==4) return  "'E-PAIR'";
  if (partype==5) return  "'TAU'";
  return  "null";
}

//------------------------------------------------------------------------------------
const char *EdbFeedback::Darkness(int drk)
{
  if (drk==0) return  "'MIP'";
  if (drk==1) return  "'BLACK'";
  if (drk==2) return  "'GRAY'";
  return  "null";
}

//------------------------------------------------------------------------------------
int EdbFeedback::ReadFBFile( const char *file )
{
  FILE *f = fopen(file,"r");  if(!f) {
    Log(1,"EdbFeedback::ReadFBFile","ERROR! open file %s", file); 
    eERROR=1;
    return 0;
  }
  Log(2,"EdbFeedback::ReadFBFile","%s", file);
   
  eNvtx=0;
  eV = new fbvertex*[eNvtxLim]; for(int i=0; i<eNvtxLim; i++) eV[i]=0;
  
  eNtr=0;
  eT = new fbtrack*[eNtrLim]; for(int i=0; i<eNtrLim; i++) eT[i]=0;

  char str[1024];
  while(  fgets( str, sizeof(str), f) )
  {
    if( fbvertex *v = ReadVertex(str) )
    {
      if( v->idvtx<0 && v->idvtx>=eNvtxLim )
      {
        Log(1,"EdbFeedback::ReadFBFile","ERROR: vertex id %d is out of range (0-%d)!",v->idvtx,eNvtxLim); 
        eERROR=1;
        break;
      }
      if( eV[v->idvtx]!=0 ) 
      {
        Log(1,"EdbFeedback::ReadFBFile","ERROR: vertex id (%d) is duplicated!",v->idvtx);
        eERROR=1;
        break;
      }

      eV[v->idvtx] = v;
      eNvtx++;
    }
    else if( fbtrack *t = ReadTrack(str) )
    {
      if( eNtr>=eNtrLim )
      {
        Log(1,"EdbFeedback::ReadFBFile","ERROR: tracks out of limit %d!",eNtrLim);
        eERROR=1;
        break;
      }

      if( t->nseg > 0) {
        t->segments = new fbsegment*[t->nseg];
        for(int iseg=0; iseg < t->nseg; iseg++) {
          t->segments[iseg] = ReadSegment(f);
          if( !(t->segments[iseg]) ) Log(1,"EdbFeedback::ReadFBFile","ERROR: read segment (%d) of track %d!",iseg,t->id_track);
        }
      }
      eT[eNtr++]=t;
    }
    else 
    {
      Log(1,"EdbFeedback::ReadFBFile","ERROR! bad line: %s", str); 
      eERROR=1;
    }
  }
  Log(2,"EdbFeedback::ReadFBFile","%d vertices and %d tracks read from %s", eNvtx,eNtr,file );
  return eNvtx;
}

//------------------------------------------------------------------------------------
fbvertex *EdbFeedback::ReadVertex( const char *str )
{
  fbvertex *v = new fbvertex();
  int npar=sscanf(str,"%d %f %f %f %d %d %d %d %d %d",
                  &v->idvtx, &v->x, &v->y, &v->z,
                  &v->isprim, &v->ischarm, &v->istau, &v->nup, &v->ndown, &v->ofb
                 );
  if(10 != npar)  SafeDelete(v);
  return v;
}

//------------------------------------------------------------------------------------
fbtrack   *EdbFeedback::ReadTrack( const char *str )
{
  fbtrack *t = new fbtrack();
  if( 27 != sscanf(str,"%d %d %d %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %f %f %f %f %d %d %d",
      &t->id_track, &t->id_upvtx, &t->id_downvtx, 
      &t->x, &t->y, &t->z, &t->sx, &t->sy,
      &t->upip, &t->downip, 
      &t->p, &t->pmin,  &t->pmax, 
      &t->isman, &t->type , &t->scanback , &t->darkness , &t->ofb , &t->lastplate,
      &t->nseg , &t->rmsslopet , &t->rmsslopel , &t->rslopet , &t->rslopel , 
      &t->kinkplatedown , &t->kinkplateup , &t->flag
                  ))
  {
    Log(1,"EdbFeedback::ReadTrack","ERROR! bad trk line: %s", str); 
    eERROR=1;
    SafeDelete(t);
    return 0; 
  }
  return t;
}

//------------------------------------------------------------------------------------
fbsegment   *EdbFeedback::ReadSegment( FILE *f )
{
  char str[512];
  fgets( str, sizeof(str), f);
  fbsegment *s = new fbsegment();
  if( 9 != sscanf(str,"%d %f %f %f %f %f %d %d %d",
      &s->id_plate, &s->x , &s->y , &s->z , &s->sx , &s->sy , 
      &s->type , &s->irec , &s->grains
                  ))
  {
    Log(1,"EdbFeedback::ReadSegment","ERROR! bad seg line: %s", str); 
    eERROR=1;
    delete s;
    return 0; 
  }
  return s;
}

//------------------------------------------------------------------------------------
void EdbFeedback::PrintFB()
{
  for( int iv=0; iv<eNvtxLim; iv++ ) {
    fbvertex *v = eV[iv];
    if(v)    Print(v);
  }

  for(int it=0; it<eNtrLim; it++ ) {
    fbtrack *t = eT[it];
    if(t) {
      Print(t);
      for(int is=0; is<t->nseg; is++ ) {
        fbsegment *s = t->segments[is];
        if(s) Print(s);
      }
    }
  }
}

//------------------------------------------------------------------------------------
void EdbFeedback::Print(fbvertex *v)
{
  if(v) printf("%d %f %f %f %d %d %d %d %d %d\n",
  v->idvtx, v->x, v->y, v->z,
  v->isprim, v->ischarm, v->istau, v->nup, v->ndown, v->ofb
              );
}

//------------------------------------------------------------------------------------
void EdbFeedback::Print(fbtrack *t)
{
  if(t) printf("%d %d %d %f %f %f %f %f %f %f %f %f %f %d %d %d %d %d %d %d %f %f %f %f %d %d %d\n",
  t->id_track, t->id_upvtx, t->id_downvtx, 
  t->x, t->y, t->z, t->sx, t->sy,
  t->upip, t->downip, 
  t->p, t->pmin,  t->pmax, 
  t->isman, t->type , t->scanback , t->darkness , t->ofb , t->lastplate,
  t->nseg , t->rmsslopet , t->rmsslopel , t->rslopet , t->rslopel , 
  t->kinkplatedown , t->kinkplateup , t->flag
              );
}

//------------------------------------------------------------------------------------
void EdbFeedback::Print(fbsegment *s)
{
  if(s) printf("%d %f %f %f %f %f %d %d %d\n",
  s->id_plate, s->x , s->y , s->z , s->sx , s->sy,
  s->type , s->irec , s->grains
              );
}
