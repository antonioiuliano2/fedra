#include "TOracleServerE2W.h"
#include "EdbLog.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbSegment.h"

ClassImp(TOracleServerE2W)

//-------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddProcessOperation(char *id_machine, 
					   char *id_programsettings, 
					   char *id_requester, 
					   char *id_parent_operation, 
					   char *id_eventbrick, 
					   char *id_plate, 
					   char *driverlevel, 
					   char *id_calibration, 
					   char *starttime, 
					   char *finishtime,
					   char *success)
{
  // Adds a process operation into the DB
  // Table involved: TB_PROC_OPERATIONS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_PROC_OPERATIONS (ID_MACHINE, ID_PROGRAMSETTINGS, ID_REQUESTER, ID_PARENT_OPERATION, ID_EVENTBRICK, ID_PLATE, DRIVERLEVEL, ID_CALIBRATION_OPERATION, STARTTIME, FINISHTIME, SUCCESS) \
 VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)", 
	    id_machine, id_programsettings, id_requester, id_parent_operation, id_eventbrick, id_plate, driverlevel, id_calibration, starttime, finishtime, success);
    
    fStmt->setSQL(query);
    Log(2,"AddProcessOperation","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddProcessOperation","Process operation added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddProcessOperation; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddEventBricks(char *databrick)
{
  // Adds a brick into the DB
  // Table involved: TB_EVENTBRICKS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
     INSERT INTO OPERA.TB_EVENTBRICKS (MINX, MAXX, MINY, MAXY, MINZ, MAXZ, ID_SET, ID_BRICK, ZEROX, ZEROY, ZEROZ) \
     VALUES (%s)", databrick);

    fStmt->setSQL(query);
    Log(2,"AddEventBricks","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);

    Log(2,"AddEventBricks","Brick added");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddEventBricks; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddBrick_Set(
				      char *id, 
				      char *idrange_min, 
				      char *idrange_max, 
				      char *id_partition)
{
  // Adds a brick-set into the DB
  // Procedure involved: PC_ADD_BRICK_SET (it fills TB_BRICK_SETS)
  // Details: CALL query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    /*sprintf(query,"\
 INSERT INTO OPERA.TB_BRICK_SETS (ID, IDRANGE_MIN, IDRANGE_MAX, ID_PARTITION) \
 VALUES (%s, %s, %s, %s)", */
      sprintf(query,"CALL PC_ADD_BRICK_SET(%s, %s, %s, %s)",
 	    id, idrange_min, idrange_max, id_partition);

    fStmt->setSQL(query);
    Log(2,"AddBrick_Set","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);

    Log(2,"AddBrick_Set","Brick_Set added, partition created");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddBrick_Set; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddBrick_Space(
					char *id_brick, 
					char *id_set)
{
  // Adds a brick-space into the DB
  // Procedure involved: PC_ADD_BRICK_SPACE
  // Details: CALL query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"CALL PC_ADD_BRICK_SPACE(%s, %s)",
 	    id_brick, id_set);

    fStmt->setSQL(query);
    Log(2,"AddBrick_Space","execute sql query: %s ...",query);
    fStmt->executeUpdate();
    Query(commit);

    Log(2,"AddBrick_Space","Brick_Space mapped");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddBrick_Space; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddPlate(char *id_eventbrick,
				 char *dataplate
				 )
{
  // Adds a plate into the DB
  // Table involved: TB_PLATE
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_PLATES (ID_EVENTBRICK, ID, Z) \
 VALUES (%s, %s)", id_eventbrick, dataplate);

    fStmt->setSQL(query);
    Log(2,"AddPlate","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddPlate","Plate added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddPlate; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddPlateCalibration(char *id_eventbrick,
					    char *id_process_operation,
					    char *datacalibration)	    

{
  // Adds a plate calibration into the DB
  // Table involved: TB_PLATE_CALIBRATIONS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_PLATE_CALIBRATIONS (ID_EVENTBRICK, ID_PROCESSOPERATION, ID_PLATE, Z, MAPXX, MAPXY, MAPYX, MAPYY, MAPDX, MAPDY) \
 VALUES (%s, %s, %s)", id_eventbrick, id_process_operation, datacalibration);

    fStmt->setSQL(query);
    Log(2,"AddPlateCalibration","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddPlateCalibration","PlateCalibration added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddPlateCalibration; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddZone(char *data)
{
  // Adds a zone into the DB
  // Table involved: TB_ZONES
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
    INSERT INTO OPERA.TB_ZONES (ID_EVENTBRICK, ID_PLATE, ID_PROCESSOPERATION, MINX, MAXX, MINY, MAXY, RAWDATAPATH, STARTTIME, ENDTIME, SERIES,\
    TXX, TXY, TYX, TYY, TDX, TDY)VALUES (%s)",data);

    fStmt->setSQL(query);
    Log(2,"AddZone","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddZone","Zone added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddZone; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddBasetracks(EdbPattern &pat, char *id_eventbrick, char *id_zone)
{
  // Adds a basetrack into the DB
  // Table involved: TB_MIPBASETRACKS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    for(int i=0; i<pat.N(); i++)
      {
	EdbSegP *s = pat.GetSegment(i);
	sprintf(query,"\
  INSERT INTO OPERA.TB_MIPBASETRACKS (ID_EVENTBRICK, ID_ZONE, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA, ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
  VALUES (%s, %s, %.2f, %.2f, %.2f, %.2f, %d, %d, %d, %d, %d, %d, %d, %d)",
		id_eventbrick, id_zone, s->X(), s->Y(), s->TX(), s->TY(), (int)s->W(), (int)s->Volume(), 0, 0, 0, 0, 0, 0);
	fStmt->setSQL(query);
      }

    Query(commit);
    Log(2,"AddBaseTracks","%d basetracks added\n",pat.N());
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddBaseTracks; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddView(char *dataView)
{
  // Adds a view into the DB
  // Table involved: TB_VIEWS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_VIEWS (ID_EVENTBRICK, ID_ZONE, SIDE, ID, DOWNZ, UPZ, POSX, POSY) \
 VALUES (%s)", dataView);

    fStmt->setSQL(query);
    Log(2,"AddView","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddView","View added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddView; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddMicroTrack(char *datamicro)
{
  // Adds a microtrack into the DB
  // Table involved: TB_MIPMICROTRACKS
  // Details: INSERT query. The COMMIT query is not done to improve the writing speed
  // please remember to apply a commit at the end of your script
  // or to execute a method which runs the commit query

  char query[2048];
//  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_MIPMICROTRACKS(ID_EVENTBRICK, ID_ZONE, SIDE, ID, ID_VIEW, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA) \
 VALUES (%s)", datamicro);

    fStmt->setSQL(query);
    Log(3,"AddMicroTrack","execute sql query: %s ...",query);
    fStmt->execute();
//    Query(commit);
    Log(3,"AddMicroTrack","MicroTrack added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddMicroTrack; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddBaseTrack(char *database)
{
  // Adds a basetrack into the DB
  // Table involved: TB_MIPBASETRACKS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_MIPBASETRACKS(ID_EVENTBRICK, ID_ZONE, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA, ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
 VALUES (%s)", database);

    fStmt->setSQL(query);
    Log(3,"AddBaseTrack","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(3,"AddBaseTrack","BaseTrack added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddBaseTrack; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddScanbackPath(char *datapath)
{
  // Adds a scanback path into the DB
  // Table involved: TB_SCANBACK_PATHS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
    INSERT INTO OPERA.TB_SCANBACK_PATHS (ID_EVENTBRICK, ID_PROCESSOPERATION, PATH, ID_START_PLATE, ID_FORK_PATH, ID_CANCEL_PLATE)VALUES (%s)",datapath);

    fStmt->setSQL(query);
    Log(2,"AddScanbackPath","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddScanbackPath","ScanbackPath added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddScanbackPath; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddScanbackPrediction(char *dataprediction)
{
  // Adds a scanback prediction into the DB
  // Table involved: TB_SCANBACK_PREDICTIONS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
    INSERT INTO OPERA.TB_SCANBACK_PREDICTIONS (ID_EVENTBRICK, ID_PATH, ID_PLATE,POSX,POSY,SLOPEX,SLOPEY,\
    POSTOL1,POSTOL2,SLOPETOL1,SLOPETOL2,FRAME,ID_ZONE,ID_CANDIDATE,DAMAGED,ISMANUAL)VALUES (%s)", dataprediction);

    fStmt->setSQL(query);
    Log(2,"AddScanbackPrediction","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddScanbackPrediction","ScanbackPrediction added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddScanbackPrediction; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddTemplateMarkSets(char *datamarks)
{
  // Adds a mark into the DB
  // Table involved: TB_TEMPLATEMARKSETS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
    INSERT INTO OPERA.TB_TEMPLATEMARKSETS\
    (ID_EVENTBRICK, ID_MARK, POSX, POSY, MARKROW, MARKCOL, SHAPE, SIDE)\
    VALUES (%s)", datamarks);

    fStmt->setSQL(query);
    Log(2,"AddTemplateMarkSets","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddTemplateMarkSets","TemplateMarkSets added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddTemplateMarkSets; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddPlateCalibration(char *id_eventbrick, char *id_process_operation, EdbPlateP *plate)
{
  // Adds a plate calibration into the DB
  // Table involved: TB_PLATE_CALIBRATIONS
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    EdbAffine2D *a = plate->GetAffineXY();

    sprintf(query,"\
      INSERT INTO OPERA.TB_PLATE_CALIBRATIONS \
      (ID_EVENTBRICK, ID_PROCESSOPERATION, ID_PLATE, Z, MAPXX, MAPXY, MAPYX, MAPYY, MAPDX, MAPDY) \
      VALUES (%s, %s, %d, %f, %f, %f, %f, %f, %f, %f)", 
	    id_eventbrick, id_process_operation, 
	    plate->ID(), plate->Z(), a->A11(), a->A12(), a->A21(), a->A22(), a->B1(), a->B2());

    fStmt->setSQL(query);
    Log(2,"AddPlateCalibration","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddPlateCalibration","PlateCalibration added");  
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddPlateCalibration; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddView(EdbView *view, int id_view, char *id_eventbrick, char *id_calibzone)
{
  // Adds a view and all its microtracks from an EdbView object into the DB
  // Tables involved: TB_VIEWS through AddView(...) and TB_MIPMICROTRACKS through AddMicroTrack(...)
  // Details: no queries directly executed

  if(!view) {
    Log(1,"AddView","Error! view=0");
    return 1;
  }
    
  int side;
  if(view->GetNframesTop()==0) side=2;  // 2 - bottom
  else side=1;                          // 1 - top
  
  EdbSegP sview(0,0,0,0,0);
  sview.Transform(view->GetHeader()->GetAffine());
  float xview = sview.X();
  float yview = sview.Y();
  
  int AreaID = view->GetAreaID();
  int ViewID = view->GetViewID();
  int nsegV  = view->Nsegments();
  
  Log(2,"AddView","Area %d, View %d is in the side %d and contains %d segments",AreaID,ViewID,side,nsegV);
  Log(2,"AddView","POSX %f, POSY %f",xview,yview);
  Log(2,"AddView","%f %f %f %f",view->GetHeader()->GetZ1(),view->GetHeader()->GetZ2(),
      view->GetHeader()->GetZ3(),view->GetHeader()->GetZ4());
  
  char datacalibview[2048];
  if (side==1)
    sprintf(datacalibview,"%s, %s, %d, %d, %f, %f, %f, %f", id_eventbrick, id_calibzone, side, id_view,
	    view->GetHeader()->GetZ1(),view->GetHeader()->GetZ2(),xview,yview );
  else
    sprintf(datacalibview,"%s, %s, %d, %d, %f, %f, %f, %f", id_eventbrick, id_calibzone, side, id_view,
	    view->GetHeader()->GetZ3(),view->GetHeader()->GetZ4(),xview,yview );
  
  AddView(datacalibview);
  
  // For each segment
  
  EdbSegment *seg;
  char datamicro[2048];
  int id_microtrack;
  
  for(int isegment=0;isegment<nsegV;isegment++) {
    
    seg = view->GetSegment(isegment);
    seg->Transform(view->GetHeader()->GetAffine());
    id_microtrack = id_view*10000 + isegment;
    sprintf(datamicro,"%s, %s ,%d, %d, %d, %f, %f, %f, %f, %d, %d, NULL, %f", id_eventbrick, id_calibzone, side, id_microtrack, id_view, seg->GetX0(), seg->GetY0(), seg->GetTx(), seg->GetTy(), seg->GetPuls(), seg->GetVolume(), seg->GetSigmaX());
    AddMicroTrack(datamicro);
    
  }

  Log(2,"AddView","View added: %d microtracks added",nsegV);

  return 0;
}



int TOracleServerE2W::AddBasetracks(TTree *tree, char *id_eventbrick, char *id_zone)
{
  // Adds a set of basetracks from a TTree (cp.root file) into the DB,
  // assuming that the corresponding microtracks are already added into the DB
  // Table involved: TB_MIPBASETRACKS through AddBaseTrack(...)
  // Details: no queries directly executed

  if(!tree)  return(0);

  EdbSegP    segP;

  EdbSegP         *s1 = 0;
  EdbSegP         *s2 = 0;
  EdbSegP         *s  = 0;

  TBranch *b_s=0, *b_s1=0, *b_s2=0;
  b_s  = tree->GetBranch("s.");
  b_s1 = tree->GetBranch("s1.");
  b_s2 = tree->GetBranch("s2.");
  b_s->SetAddress(  &s   );
  b_s1->SetAddress( &s1  );
  b_s2->SetAddress( &s2  );

  int nentr = (int)(tree->GetEntries());

  char data[2048];

  for(int i=0; i<nentr; i++ ) {

    b_s->GetEntry(i);
    b_s1->GetEntry(i);
    b_s2->GetEntry(i);

    int id1 = s1->Vid(0)*10000 + s1->ID();
    int id2 = s2->Vid(0)*10000 + s2->ID();

    sprintf(data,"%s, %s, %d, %2f, %2f, %2f, %2f, %2f, %2f, NULL, %2f, %d, %d, %d, %d", 
	    id_eventbrick, id_zone, i, s->X(), s->Y(), s->TX(), s->TY(), s->W(), s->Volume(), s->Chi2(),
	    1, id1, 2, id2);

    AddBaseTrack(data);
  }

  Log(2,"AddBasetracks","Basetracks added: %d basetracks added assuming microtracks previously added",nentr);

  return nentr;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::DeleteBrick(char *id_eventbrick)
{
  // Delete all informations related to a brick from the DB
  // Tables involved: ... a lot, look at the code...
  // Details: DELETE queries and then a COMMIT query

  char query[11][2048];
  char commit[11]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query[0], "delete from Tb_Scanback_Predictions where id_eventbrick=%s",id_eventbrick);
    sprintf(query[1], "delete from Tb_Scanback_Paths       where id_eventbrick=%s",id_eventbrick);
    sprintf(query[2], "delete from Tb_MipBasetracks        where id_eventbrick=%s",id_eventbrick);
    sprintf(query[3], "delete from Tb_MipMicrotracks       where id_eventbrick=%s",id_eventbrick);
    sprintf(query[4], "delete from Tb_Views                where id_eventbrick=%s",id_eventbrick);
    sprintf(query[5], "delete from Tb_Plate_calibrations   where id_eventbrick=%s",id_eventbrick);
    sprintf(query[6], "delete from Tb_Zones                where id_eventbrick=%s",id_eventbrick);
    sprintf(query[7], "delete from Tb_Proc_operations      where id_eventbrick=%s",id_eventbrick);
    sprintf(query[8], "delete from Tb_Plates               where id_eventbrick=%s",id_eventbrick);
    sprintf(query[9], "delete from Tb_TemplateMarkSets     where id_eventbrick=%s",id_eventbrick);
    sprintf(query[10],"delete from Tb_Eventbricks          where id=%s",id_eventbrick);

    for (int i=0;i<11;i++) {
	fStmt->setSQL(query[i]);
	Log(2,"DeleteBrick","execute sql query: %s ...",query);
	fStmt->execute();
    }
    Query(commit);

    Log(2,"DeleteBrick","Brick deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "DeleteBrick; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::DeleteBrickSpace(char *id_brick)
{
  // Delete a brick space related to a brick from the DB
  // Procedure involved: PC_REMOVE_BRICK_SPACE
  // Details: CALL query and then a COMMIT query

  char query[2048];
  char commit[11]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"call pc_remove_brick_space(%s,'OPERA NA BK01')",id_brick);

    fStmt->setSQL(query);
    Log(2,"DeleteBrickSpace","execute sql query: %s ...",query);
    fStmt->execute();

    Query(commit);

    Log(2,"DeleteBrickSpace","BrickSpace deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "DeleteBrickSpace; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::DeleteOperation(char *id_brick, char *id_process_operation)
{
  // Delete all informations related to a process operation of a brick from the DB
  // Tables involved: ... a lot, look at the code...
  // Details: DELETE queries and then a COMMIT query

  char query[9][2048];
  char commit[11]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query[0], "delete from Tb_Scanback_Predictions where (id_path) in (select id from tb_scanback_paths where id_processoperation=%s) and id_eventbrick=%s",id_process_operation,id_brick);    
    sprintf(query[1], "delete from Tb_Scanback_Paths where id_processoperation=%s and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[2], "delete from Tb_MipBasetracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s)) and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[3], "delete from Tb_MipMicrotracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s)) and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[4], "delete from tb_views where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s))",id_process_operation);
    sprintf(query[5], "delete from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s)",id_process_operation);
    sprintf(query[6], "delete from tb_plate_calibrations where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s)",id_process_operation);
    sprintf(query[7], "delete from tb_proc_operations where id_parent_operation=%s",id_process_operation);
    sprintf(query[8], "delete from tb_proc_operations where id=%s",id_process_operation);

    for (int i=0;i<9;i++) {
	fStmt->setSQL(query[i]);
	Log(2,"DeleteOperation","execute sql query: %s ...",query);
	fStmt->execute();
    }
    Query(commit);

    Log(2,"DeleteOperation","Operation deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "DeleteOperation; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}
