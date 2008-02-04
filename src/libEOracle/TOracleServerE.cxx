#include "TOracleServerE.h"
#include "EdbPattern.h"
#include "EdbAffine.h"

ClassImp(TOracleServerE)

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol)
{
	char id[24];
	sprintf(id,"%lld",id_volume);
	return ReadVolume(id,vol);
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadCalibration(int id_brick, EdbPatternsVolume &vol)
{
  int  nsegtot=0;
  char query[2048];
  int  npat=0;
  ULong64_t *proc_list = new ULong64_t[100];
  TObjArray patterns(100);
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id,to_char(calibration) from  tb_plates \
    where id_eventbrick = %d and calibration is not null order by id desc", id_brick);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    int plate=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%d", &plate );
      ULong64_t procop=0LL;
      sscanf( (rs->getString(2)).c_str(),"%lld", &procop );
      proc_list[npat] = procop;
      npat++;
      pat = new EdbPattern(0.,0.,0);
      pat->SetPID(plate);
      pat->SetID(id_brick);
      patterns.Add( pat );
    }
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  npat = patterns.GetEntries();
  for(int i=0; i<npat; i++) {
    pat = (EdbPattern*)patterns.At(i);
    if( !ReadZplate(pat->ID(), pat->PID(), *pat ) )
      { printf("skip plate %d %d !\n", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_zone in (select id from tb_zones where id_processoperation=%lld)", proc_list[i]);
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadVolume(char *id_volume, EdbPatternsVolume &vol)
{
  int  nsegtot=0;
  char query[2048];
  TObjArray patterns(100);
  EdbPattern *pat=0;
  int brick=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id_plate,id_eventbrick from TB_VOLUME_SLICES where id_volume=%s order by id_plate", id_volume);
    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    int plate=0;
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
      { printf("skip plate %d %d !\n", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_zone in (select id_zone from TB_VOLUME_SLICES where id_volume=%s and id_plate=%d and id_eventbrick=%d) and id_eventbrick=%d", id_volume,pat->PID(),brick,brick);
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadDataSet(ULong64_t id_parent_op, int id_brick, EdbPatternsVolume &vol)
{
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
from tb_proc_operations op, tb_plates pl \
where op.id_eventbrick=%d and pl.id_eventbrick=%d and pl.id=op.id_plate and op.id_parent_operation=%lld \
order by op.id_plate",
	    id_brick, id_brick, id_parent_op);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();

    ULong64_t procop=0LL;
    float z=0;
    int plate=0;
    npat=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%lld", &procop );
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
      { printf("skip plate %d %d !\n", id_brick,pat->PID()); continue; }
    sprintf(query,"id_zone in (select id from tb_zones where id_processoperation=%lld and id_plate=%d)", proc_list[i],pat->PID()); 
    nsegtot  += ReadBasetracksPattern( query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
bool  TOracleServerE::ReadZplate(int id_eventbrick, int id_plate, EdbPattern &pat)
{
  char *query= new char[2048];
  Float_t Z=0;
  EdbAffine2D *aff=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,
            "select z,MAPXX,MAPXY,MAPYX,MAPYY,MAPDX,MAPDY from tb_plates \
             where id_eventbrick=%d and id=%d",
            id_eventbrick, id_plate);
    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
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
    printf("z=%f    ",Z);
    pat.SetZ(Z);
    aff->Print();
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadBasetracksPattern; Affine failed: (error: %s)", (oraex.getMessage()).c_str());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadBasetracksPattern(char *selection, EdbPattern &pat)
{
  int ntracks=0;
  char query[2048];
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query, "SELECT \
  ID_EVENTBRICK, id_zone, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA \
  from tb_mipbasetracks where %s",
	    selection);
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    printf("\nexecute sql query: %s ...\n",query);
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
  printf("%d segments are readed\n", ntracks);
  return ntracks;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadMicrotracksPattern(char *selection, EdbPattern &pat)
{
  int ntracks=0;
  char query[2048];
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query, "SELECT \
  ID_EVENTBRICK, id_zone, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA \
  from TB_MIPMICROTRACKS where %s",
	    selection);
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    printf("\nexecute sql query: %s ...\n",query);
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
      seg.SetDZ(45.);                          //!!! a kind of hack
      seg.SetVolume(rs->getInt(10));
      seg.SetVid(pat.PID(),0);          // keep in a segment also plate ID (by Ale)
      pat.AddSegment(seg);
      ntracks++;
    }
    delete rs;
  } catch (SQLException &oraex)  {
    Error("TOracleServerE", "ReadBasetracksPattern failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  printf("%d segments are readed\n", ntracks);
  return ntracks;
}



//------------------------------------------------------------------------------------
Int_t TOracleServerE::GetProcessOperationID(char *id_eventbrick, char *id)
{
  char *query= new char[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"select ID from tb_proc_operations where id_eventbrick=%s",id_eventbrick);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;
    //    printf("id=%s\n",id);

  } catch (SQLException &oraex) {
    Error("TOracleServerE", "GetProcessOperationID; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}


//------------------------------------------------------------------------------------
Int_t TOracleServerE::GetProcessOperationID(char *id_programsettings, char *id_eventbrick, char *id_plate, char *id)
{
  char *query= new char[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
 select ID from tb_proc_operations \
 where id_programsettings=%s and id_eventbrick=%s and id_plate=%s",
	    id_programsettings,id_eventbrick,id_plate);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      strcpy(id,rs->getString(1).c_str());
    }
    delete rs;
    //    printf("id=%s\n",id);

  } catch (SQLException &oraex) {
    Error("TOracleServerE", "GetProcessOperationID; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}




//------------------------------------------------------------------------------------
Int_t  TOracleServerE::AddBrick(char *id, 
				char *minx, 
				char *maxx, 
				char *miny, 
				char *maxy, 
				char *minz, 
				char *maxz)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
 INSERT INTO OPERA.TB_EVENTBRICKS (ID, MINX, MAXX, MINY, MAXY, MINZ, MAXZ) \
 VALUES (%s, %s, %s, %s, %s, %s, %s)", 
 	    id, minx, maxx, miny, maxy, minz, maxz);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    //    Query(query);
    Query(commit);

    printf("Brick added\n");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE", "AddBrick; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE::AddPlate(char *data)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_PLATES (ID_EVENTBRICK, ID, Z, MAPXX, MAPXY, MAPYX, MAPYY, MAPDX, MAPDY, ISDAMAGED) \
 VALUES (%s)",data);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    //    Query(query);
    Query(commit);
    printf("Plate added\n");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "AddPlate; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE::UpdatePlate(char *id_eventbrick, char *id_plate, char *calibration)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"UPDATE OPERA.TB_PLATES SET CALIBRATION=%s WHERE ID_EVENTBRICK=%s AND ID=%s",calibration,id_eventbrick,id_plate);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    //    Query(query);
    Query(commit);
    printf("Plate updated\n");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "UpdatePlate; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE::AddZone(char *data)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_ZONES (ID_EVENTBRICK, ID_PLATE, ID_PROCESSOPERATION, MINX, MAXX, MINY, MAXY, RAWDATAPATH, STARTTIME, ENDTIME, BASETHICKNESS, SERIES) \
 VALUES (%s)",data);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    //    Query(query);
    Query(commit);
    printf("Zone added\n");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "AddZone; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE::AddZone(EdbPattern &pat, char *data1, char *data2)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    for(int i=0; i<pat.N(); i++)
      {
	EdbSegP *s = pat.GetSegment(i);
	sprintf(query,"\
 INSERT INTO OPERA.TB_ZONES (ID_EVENTBRICK, ID_PLATE, ID_PROCESSOPERATION, MINX, MAXX, MINY, MAXY, RAWDATAPATH, STARTTIME, ENDTIME, BASETHICKNESS) \
 VALUES (%s, %.2f, %.2f, %.2f, %.2f, %s)",
		data1,s->X()-s->SX(),s->X()+s->SX(),s->Y()-s->SY(),s->Y()+s->SY(),data2);
	fStmt->setSQL(query);
	//	printf("\nexecute sql query: %s ...\n",query);
	//	fStmt->execute();
      }

    Query(commit);
    printf("%d zones added\n",pat.N());
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "AddZone; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE::AddBasetracks(EdbPattern &pat, char *id_eventbrick, char *id_zone)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    for(int i=0; i<pat.N(); i++)
      {
	EdbSegP *s = pat.GetSegment(i);
	sprintf(query,"\
  INSERT INTO OPERA.TB_MIPBASETRACKS (ID_EVENTBRICK, ID_ZONE, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA, ID_SOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
  VALUES (%s, %s, %.2f, %.2f, %.2f, %.2f, %d, %d, %d, %d, %d, %d, %d, %d)",
		id_eventbrick, id_zone, s->X(), s->Y(), s->TX(), s->TY(), 
		(int)s->W(), (int)s->Volume(), 0, 0, 0, 0, 0, 0);
	fStmt->setSQL(query);
      }

    Query(commit);
    printf("%d zones added\n",pat.N());
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "AddZone; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



Int_t  TOracleServerE::AddProcessOperation(char *id_machine, 
					   char *id_programsettings, 
					   char *id_requester, 
					   char *id_parent_operation, 
					   char *id_eventbrick, 
					   char *id_plate, 
					   char *driverlevel, 
					   char *templatemarks, 
					   char *starttime, 
					   char *finishtime,
					   char *success)
{
  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_PROC_OPERATIONS (ID_MACHINE, ID_PROGRAMSETTINGS, ID_REQUESTER, ID_PARENT_OPERATION, ID_EVENTBRICK, ID_PLATE, DRIVERLEVEL, TEMPLATEMARKS, STARTTIME, FINISHTIME, SUCCESS) \
 VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)", 
	    id_machine, id_programsettings, id_requester, id_parent_operation, id_eventbrick, id_plate, driverlevel, templatemarks, starttime, finishtime, success);
    
    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    //    Query(query);
    Query(commit);
    printf("Process operation added\n");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "AddProcessOperation; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


