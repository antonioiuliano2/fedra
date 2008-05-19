#include <map>
#include "EdbLog.h"
#include "TOracleRow.h"
#include "TOracleServerE2.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbRun.h"
#include "EdbView.h"
#include "EdbSegment.h"
#include "EdbScanProc.h"

ClassImp(TOracleServerE2)


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadDataSet(ULong64_t id_parent_op, int id_brick, ULong64_t path, EdbPatternsVolume &vol)
{
  // read all basetracks patterns corresponsing to the given parent process for the given brick and given path ("series" in tb_zones table)
  // TO CHECK: this set of selection parameters guarantee the consistence and uniqueness of data? (all data of calibration/scanback/totalscan, etc)
  int  nsegtot=0;
  char query[2048];
  int npat = 0;
  TObjArray patterns(100);
  ULong64_t *proc_list = new ULong64_t[100];
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
 select to_char(op.id), op.id_plate, pl.z, op.success \
 from tb_proc_operations%s op, tb_plates%s pl \
 where op.id_eventbrick=%d and pl.id_eventbrick=%d and pl.id=op.id_plate and op.id_parent_operation=%lld \
 order by op.id_plate", eRTS.Data(), eRTS.Data(),
	    id_brick, id_brick, id_parent_op);

    fStmt->setSQL(query);
    Log(2,"ReadDataSet","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();

    ULong64_t procop=0LL;
    float z=0;
    int plate=0;
    npat=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%lld", &procop );
      if(strncmp(rs->getString(4).c_str(),"Y",1)) continue;
      plate = rs->getInt(2);
      z     = rs->getFloat(3);
      proc_list[npat] = procop;
      npat++;
      pat = new EdbPattern(0.,0.,z);
      pat->SetPID(plate);
      patterns.Add( pat );
    }

  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  for(int i=0; i<npat; i++) {
    pat = (EdbPattern*)patterns.At(i);
    if( !ReadZplate(id_brick, pat->PID(), *pat ) )
      { Log(1,"ReadDataSet","skip plate %d %d !", id_brick,pat->PID()); continue; }
    sprintf(query,"id_zone in (select id from tb_zones%s where id_processoperation=%lld and id_plate=%d and series=%lld)",eRTS.Data(),
	    proc_list[i],pat->PID(), path);
    nsegtot  += ReadBasetracksPattern( query, *pat);
    vol.AddPattern(pat);
  }
  Log(2,"ReadDataSet","%d segments extracted from db into PatternsVolume",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol)
{
        char id[24];
        sprintf(id,"%lld",id_volume);
        return ReadVolume(id,vol);
}
//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol, Int_t min_pl, Int_t max_pl)
{
  char id[24];
  Int_t min= min_pl;
  Int_t max= max_pl;
  sprintf(id,"%lld",id_volume);
  return ReadVolume(id,vol,min,max);
}
//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume(char *id_volume, EdbPatternsVolume &vol)
{
  int  nsegtot=0;
  char query[2048];
  TObjArray patterns(100);
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id_plate,id_eventbrick from TB_VOLUME_SLICES%s where id_volume=%s order by id_plate",eRTS.Data(), id_volume);
    fStmt->setSQL(query);
    Log(2,"ReadVolume","\nexecute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    if(!rs) return 0;
    int plate=0;
    int brick=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%d", &plate );
      sscanf( (rs->getString(2)).c_str(),"%d", &brick );
      pat = new EdbPattern(0.,0.,0);
      pat->SetPID(plate);
      pat->SetID(brick);
      patterns.Add( pat );
    }
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  int npat = patterns.GetEntries();
  for(int i=0; i<npat; i++) {
    pat = (EdbPattern*)patterns.At(i);
    //if( !ReadZplate(pat->ID(), pat->PID(), *pat ) )
    if( !ReadZplate_nominal(pat->ID(), pat->PID(), *pat ) )
      { Log(1,"ReadVolume","skip plate %d %d !", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_eventbrick=%d and id_zone in (select id_zone from TB_VOLUME_SLICES%s where id_volume=%s and id_plate=%d)",pat->ID(), eRTS.Data(), id_volume,pat->PID());
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  Log(2,"ReadVolume","%d segments extracted from db into PatternsVolume",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume(char *id_volume, EdbPatternsVolume &vol, Int_t min, Int_t max)
{
  int  nsegtot=0;
  char query[2048];
  TObjArray patterns(100);
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id_plate,id_eventbrick from TB_VOLUME_SLICES%s where (id_volume=%s and id_plate > %d-1 and id_plate < %d+1) order by id_plate", 
	    eRTS.Data(),
	    id_volume,min,max);
    fStmt->setSQL(query);
    Log(2,"ReadVolume","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    int plate=0;
    int brick=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%d", &plate );
      sscanf( (rs->getString(2)).c_str(),"%d", &brick );
      pat = new EdbPattern(0.,0.,0);
      pat->SetPID(plate);
      pat->SetID(brick);
      patterns.Add( pat );
    }
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  int npat = patterns.GetEntries();
  for(int i=0; i<npat; i++) {
    pat = (EdbPattern*)patterns.At(i);
    if( !ReadZplate(pat->ID(), pat->PID(), *pat ) )
      { Log(1,"ReadVolume","skip plate %d %d !", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_zone in (select id_zone from TB_VOLUME_SLICES%s where id_volume=%s and id_plate=%d)",eRTS.Data(),
	    id_volume,pat->PID());
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  Log(2,"ReadVolume","%d segments extracted from db into PatternsVolume",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
bool  TOracleServerE2::ReadZplate_nominal(int id_eventbrick, int id_plate, EdbPattern &pat)
{
  // read nominal z of the plate from the tb_plates table

  char *query= new char[2048];
  Float_t Z=0;

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,
            "select pl.z-br.minz from tb_plates%s pl, tb_eventbricks%s br\
             where pl.id_eventbrick=%d and pl.id=%d and br.id=pl.id_eventbrick",eRTS.Data(),eRTS.Data(),
            id_eventbrick, id_plate);

    fStmt->setSQL(query);
    Log(2,"ReadZplate_nominal","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      Z = rs->getFloat(1);
    }
    delete rs;
    pat.SetZ(Z);

  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadBasetracksPattern; Affine failed: (error: %s)", (oraex.getMessage()).c_str());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------------
bool  TOracleServerE2::ReadZplate(int id_eventbrick, int id_plate, EdbPattern &pat)
{
  // read z of the plate after calibration from the tb_plate_calibrations table

  char *query= new char[2048];
  Float_t Z=0;
  EdbAffine2D *aff=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,
            "select z,MAPXX,MAPXY,MAPYX,MAPYY,MAPDX,MAPDY from tb_plate_calibrations%s \
             where id_eventbrick=%d and id_plate=%d", eRTS.Data(),
            id_eventbrick, id_plate);
    fStmt->setSQL(query);
    Log(2,"ReadZplate","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      Z = rs->getFloat(1);
      aff = new EdbAffine2D(rs->getFloat(2),
                            rs->getFloat(3),
                            rs->getFloat(4),
                            rs->getFloat(5),
                            rs->getFloat(6),
                            rs->getFloat(7));
    }
    delete rs;
    pat.SetZ(Z);
    aff->Print();
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadBasetracksPattern; Affine failed: (error: %s)", (oraex.getMessage()).c_str());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadBasetracksPattern(char *selection, EdbPattern &pat)
{
  // read basetracks from the tb_mipbasetracks table using the given selection string

  int ntracks=0;
  char query[2048];
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query, "SELECT \
  ID_EVENTBRICK, id_zone, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA \
  from tb_mipbasetracks%s where %s",eRTS.Data(),
	    selection);
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    Log(2,"ReadBasetracksPattern","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    EdbSegP seg;
    while (rs->next()){
      seg.Set(
	      rs->getInt(3),    //id
	      rs->getFloat(4),  //posx
	      rs->getFloat(5),  //posy
	      rs->getFloat(6),  //slopex
	      rs->getFloat(7),  //slopey
	      rs->getInt(8),    //grains
	      0                 //flag
	      );
      seg.SetZ(pat.Z());
      seg.SetDZ(300.);                          //!!! a kind of hack
      seg.SetVolume(rs->getInt(10));
      seg.SetVid(pat.PID(),0);          // keep in a segment also plate ID (by Ale)
      pat.AddSegment(seg);
      ntracks++;
    }
    delete rs;
  } catch (SQLException &oraex)  {
    Error("TOracleServerE", "ReadBasetracksPattern failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  Log(2,"ReadBasetracksPattern","%d segments are read\n", ntracks);
  return ntracks;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadViewsZone(ULong64_t id_zone, int side, TObjArray &edbviews)
{
  // read TB_VIEWS for the given zone and fill the array of edbviews with the correct headers information

  int  nviews=0;
  char query[2048];
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query, "SELECT \
  id_eventbrick, side, id, downz, upz, posx, posy \
  from TB_VIEWS%s where id_zone=%lld and side=%d order by id", eRTS.Data(),
	    id_zone,side);
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    Log(2,"ReadViewsZone","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    EdbView *view=0;
    while (rs->next()) {
      view = new EdbView();
      view->GetHeader()->SetAreaID(id_zone%10000000);
      view->GetHeader()->SetViewID(rs->getInt(3));
      if     (side==1)	{
	view->SetNframes(0,15);  //bottom side
	view->GetHeader()->SetCoordZ( 0,0, rs->getFloat(5), rs->getFloat(4) );  //to check it!
      }
      else if(side==2) {
	view->SetNframes(15,0);  //top side
	view->GetHeader()->SetCoordZ( rs->getFloat(5), rs->getFloat(4), 0,0 );  //to check it!
      }
      view->GetHeader()->SetCoordXY( rs->getFloat(6), rs->getFloat(7) );

      edbviews.Add(view);
      nviews++;
    }
    delete rs;
  } catch (SQLException &oraex)  {
    Error("TOracleServerE2", "ReadViews is failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  Log(2,"ReadViewsZone","%d views are read\n", nviews);
  return nviews;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ConvertMicrotracksZoneToEdb(Int_t id_eventbrick, ULong64_t id_zone, EdbRun &run)
{
  // read possibly full information for all views and microtracks for the given zone and fill the run file with the raw data (rwd like)

  int nviews=0;
  TObjArray edbviews1;
  int n1= ReadViewsZone(id_zone,1,edbviews1);
  if(n1>0) {
    nviews += n1;  
    ReadMicrotracksZone( id_eventbrick, id_zone, 1, edbviews1 );
    for(int i=0; i<edbviews1.GetEntries(); i++)    run.AddView( (EdbView *)(edbviews1.At(i)) );
  }

  TObjArray edbviews2;
  int n2 = ReadViewsZone(id_zone,2,edbviews2);
  if(n2>0) {
    nviews += n2;
    ReadMicrotracksZone( id_eventbrick, id_zone, 2, edbviews2 );
    for(int i=0; i<edbviews2.GetEntries(); i++)    run.AddView( (EdbView *)(edbviews2.At(i)) );
  }
  run.Save();
  return nviews;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadMicrotracksZone(Int_t id_eventbrick, ULong64_t id_zone, int side, TObjArray &edbviews)
{
  // read all microtracks for the one side of the one zone

  char selection[1024];
  sprintf(selection, " id_zone=%lld and side=%d", id_zone, side );
  EdbPattern pat;
  int nseg = ReadMicrotracksPattern( id_eventbrick, selection, pat);
  
  EdbView     *v=0;
  EdbSegment   sv;
  EdbSegP    *sp=0;
  float z=0;
  for(int i=0; i<nseg; i++ ) {
    sp = pat.GetSegment(i);
    v = (EdbView*)edbviews.At(sp->Aid(1)-1);   // assume that views are in the order!
    if(v->GetNframesTop()>0) z = v->GetZ2(); 
    else                     z = v->GetZ3();
    sv.Set( sp->X()-v->GetXview(),
	    sp->Y()-v->GetYview(),
	    z,sp->TX(),
	    sp->TY(), 
	    sp->DZ(),         // dz - not exist the real one??
	    side, 
	    int(sp->W()), 
	    0 );              // id - where to get it???
    sv.SetSigma(sp->SZ(),0);
    v->AddSegment(&sv);
  }

  return 0;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadMicrotracksPattern(int id_eventbrick, char *selection, EdbPattern &pat)
{
  // read all microtracks from TB_MIPMICROTRACKS table for the given selection

  int ntracks=0;
  char query[2048];
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query, "SELECT \
  ID_EVENTBRICK, id_zone, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA, SIDE, ID_VIEW\
  from TB_MIPMICROTRACKS%s where id_eventbrick=%d and %s",eRTS.Data(),
	    id_eventbrick, selection);
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    Log(2,"ReadMicrotracksPattern","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    EdbSegP seg;
    while (rs->next()){
      seg.Set(
	      rs->getInt(3),    //id
	      rs->getFloat(4),  //posx
	      rs->getFloat(5),  //posy
	      rs->getFloat(6),  //slopex
	      rs->getFloat(7),  //slopey
	      rs->getInt(8),    //grains
	      0                 //flag
	      );
      seg.SetSZ(rs->getFloat(11)); // put sigma here!
      seg.SetZ(pat.Z());
      seg.SetDZ(45.);                          //!!! a kind of hack
      seg.SetVolume(rs->getInt(9));     //areasum
      seg.SetVid(pat.PID(),0);          // keep in a segment also plate ID (by Ale)
      seg.SetAid(0,rs->getInt(13));     // area id, view id
      pat.AddSegment(seg);
      ntracks++;
    }
    delete rs;
  } catch (SQLException &oraex)  {
    Error("TOracleServerE2", "ReadMicrotracksPattern failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  Log(2,"ReadMicrotracksPattern","%d segments are read", ntracks);
  return ntracks;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ConvertMicrotracksVolumeToEdb(ULong64_t id_volume, const char *outdir)
{
  int  nviewtot=0;
  char query[2048];
  std::map<int,ULong64_t> pl_zones;
  int brick =-999, plate=0;
  ULong64_t zone=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id_eventbrick,id_plate,id_zone from TB_VOLUME_SLICES%s where \
    (id_volume=%lld) order by id_plate",
	    eRTS.Data(),
	    id_volume);
    fStmt->setSQL(query);
    Log(2,"ConvertMicrotracksVolumeToEdb","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%d", &brick );
      sscanf( (rs->getString(2)).c_str(),"%d", &plate );
      sscanf( (rs->getString(3)).c_str(),"%lld", &zone );
      pl_zones[plate]=zone;
    }
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  EdbScanProc sproc;
  sproc.eProcDirClient=outdir;
  int id[4];
  int plate0 =-999;
  EdbRun *run=0;
  map<int,ULong64_t>::const_iterator iter;
  for (iter=pl_zones.begin(); iter != pl_zones.end(); ++iter) {
    plate= iter->first;
    zone = iter->second;
    Log(2,"ConvertMicrotracksVolumeToEdb","for plate: %d read zone: %lld", plate,zone);
    id[0] = brick; id[1]=plate; id[2]=0; id[3]=0;
    TString str; sproc.MakeFileName(str,id,"raw.root");
    sproc.CheckProcDir(id);
    if(plate!=plate0) {
      plate0=plate;
      run = new EdbRun(str.Data(),"RECREATE");
    } else run = new EdbRun(str.Data(),"UPDATE");
    nviewtot+=ConvertMicrotracksZoneToEdb(brick,zone,*run);
    if(run) run->Close();      SafeDelete(run);
  }
  Log(2,"ConvertMicrotracksVolumeToEdb","%d views is extracted from db for the volume %lld",nviewtot,id_volume);
  return nviewtot;
}

//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetProcessOperationID(char *id_eventbrick, char *id_programsettings, char *id)
{
  // Get the last process operation ID related to a given brick and program setting

  char *query= new char[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID from tb_proc_operations%s where id_eventbrick=%s and id_programsettings=%s"
	    ,eRTS.Data(), id_eventbrick, id_programsettings);

    fStmt->setSQL(query);
    Log(2,"GetProcessOperationID","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;
 
  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetProcessOperationID; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}

//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetProcessOperationID(char *id_eventbrick, char *id_programsettings, char *id_plate, char *id)
{
  // Get the last process operation ID related to a given brick, plate and program setting

  char *query= new char[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
 select ID from tb_proc_operations%s \
 where id_programsettings=%s and id_eventbrick=%s and id_plate=%s",eRTS.Data(),
	    id_programsettings,id_eventbrick,id_plate);

    fStmt->setSQL(query);
    Log(2,"GetProcessOperationID","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetProcessOperationID; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}



//------------------------------------------------------------------------------------
Int_t TOracleServerE2::DumpProcessOperations(char *id_eventbrick,char *id_programsettings)
{
  // Dump on the screen the list of process operation IDs related to a given brick and program setting

  char *query= new char[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID,to_char(starttime) from tb_proc_operations%s where id_eventbrick=%s and id_programsettings=%s", 
	    eRTS.Data(), id_eventbrick, id_programsettings);

    fStmt->setSQL(query);
    Log(2,"DumpProcessOperations","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      printf("Operation: %s, Start time: %s\n",rs->getString(1).c_str(),rs->getString(2).c_str());
    }
    delete rs;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetProcessOperationID; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}


//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetId_EventBrick(char *id_brick,char *id_set, char *id)
{
  // Get the brick ID related to a given brick name and brick-set

  char *query= new char[2048];
   try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID from tb_eventbricks%s where id_brick=%s and id_set=%s ",
	    eRTS.Data(), id_brick, id_set);

    fStmt->setSQL(query);
    Log(2,"GetId_EventBrick","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetId_Eventbrick; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}


//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetId_Zone(char *id_eventbrick,char *id_plate, char *id_process_operation, char *series, char* id)
{
  // Get the zone ID related to a given brick, plate, process operation and series

  char *query= new char[2048];
   try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID from tb_zones%s where id_eventbrick=%s and id_plate=%s and id_processoperation=%s and series=%s", 
	    eRTS.Data(), id_eventbrick, id_plate, id_process_operation, series);

    fStmt->setSQL(query);
    Log(2,"GetId_Zone","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetId_Zone; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}



//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetId_ScanbackPath(char *id_eventbrick, char *id_process_operation, int path, char *id)
{
  // Get the scanback path ID related to a given brick, process operation and path

  char *query= new char[2048];
   try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID from TB_SCANBACK_PATHS%s where id_eventbrick=%s and ID_PROCESSOPERATION=%s and PATH=%d", 
	    eRTS.Data(), id_eventbrick, id_process_operation, path);

    fStmt->setSQL(query);
    Log(2,"GetId_ScanbackPath","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetId_ScanbackPath; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}


//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetId_Volume(char *id_eventbrick, char *id_process_operation, int ivolume, char *id)
{
  // Get the volume ID related to a given brick, process operation and progressive number of the volume

  char *query= new char[2048];
   try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID from TB_VOLUMES%s where id_eventbrick=%s and ID_PROCESSOPERATION=%s and VOLUME=%d", 
	    eRTS.Data(), id_eventbrick, id_process_operation, ivolume);

    fStmt->setSQL(query);
    Log(2,"GetId_Volume","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2", "GetId_Volume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}



//------------------------------------------------------------------------------------
Int_t TOracleServerE2::GetProcessType(char *IDPROCESS)
{
  // GL: strange function; probably can be useful as an example of the complex query

  char *query= new char[2048];
  int scanback=-1;
  int SBFLAG=-99,TSFLAG=-99;

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
select SBFLAG, TSFLAG FROM (select decode(count(*),0,0,1) as SBFLAG from tb_scanback_paths \
where (id_eventbrick, id_processoperation) in (select id_eventbrick, id from tb_proc_operations \
where id = %s)),(select decode(count(*),0,0,1) as TSFLAG from tb_volumes \
where (id_eventbrick, id_processoperation) \
in (select id_eventbrick, id from tb_proc_operations where id = %s))",
	    IDPROCESS,IDPROCESS);

    fStmt->setSQL(query);
    Log(2,"GetProcessType","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();

    if(!rs->next())  return scanback;

    SBFLAG = rs->getInt(1);
    TSFLAG = rs->getInt(2);

    if (SBFLAG == 0 && TSFLAG == 0)
      scanback = -1;
    else if (SBFLAG == 1)
      scanback = 1;
    else if (TSFLAG == 1)
      scanback = 0;

    delete rs;
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "GetProcessType; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return scanback;
}
