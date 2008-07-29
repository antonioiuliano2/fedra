#include "TOracleServerE2W.h"
#include "EdbLog.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbSegment.h"
#include "EdbRun.h"

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
					       char *success,
					       char *notes)
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
 INSERT INTO OPERA.TB_PROC_OPERATIONS (ID_MACHINE, ID_PROGRAMSETTINGS, ID_REQUESTER, ID_PARENT_OPERATION, ID_EVENTBRICK, ID_PLATE, DRIVERLEVEL, ID_CALIBRATION_OPERATION, STARTTIME, FINISHTIME, SUCCESS, NOTES) \
 VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, '%s')",
	    id_machine, id_programsettings, id_requester, id_parent_operation, id_eventbrick, id_plate, driverlevel, id_calibration, starttime, finishtime, success, notes);
    
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
Int_t  TOracleServerE2W::AddBaseTracks(EdbPattern &pat, char *id_eventbrick, char *id_zone)
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
Int_t  TOracleServerE2W::AddScanbackPath(char *id_eventbrick, char *id_header_operation, int id_path, int id_start_plate)
{
  // Adds a scanback path into the DB and connects it with the CS prediction
  // Table involved: TB_SCANBACK_PATHS, TB_B_CSCANDS_SBPATHS
  // Details: a SELECT and two INSERT queries and then a COMMIT query
  
  char query[2048];
  char commit[10]="commit";
  
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
    INSERT INTO OPERA.TB_SCANBACK_PATHS (ID_EVENTBRICK, ID_PROCESSOPERATION, PATH, ID_START_PLATE, ID_FORK_PATH, ID_CANCEL_PLATE) VALUES (%s, %s, %d, %d, NULL, NULL)",
	    id_eventbrick, id_header_operation, id_path, id_start_plate);
    
    fStmt->setSQL(query);
    Log(2,"AddScanbackPath","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddScanbackPath","TB_SCANBACK_PATHS added");
    
    sprintf(query,"select id_cs_eventbrick,idcand from vw_local_cs_candidates where id_cs_eventbrick in (select id from tb_eventbricks where id_brick in (select id_brick from tb_eventbricks where id=%s)) and id_plate=2 and cand=%d",
	    id_eventbrick, id_path);

    fStmt->setSQL(query);
    Log(2,"AddScanbackPath","execute sql query: %s ...",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    char id_cs_eventbrick[20],id_candidate[20];
    while (rs->next()){
      strcpy(id_cs_eventbrick,rs->getString(1).c_str());
      strcpy(id_candidate,rs->getString(2).c_str());
    }
    delete rs;


    sprintf(query,"\
    INSERT INTO OPERA.TB_B_CSCANDS_SBPATHS (ID_CS_EVENTBRICK, ID_CANDIDATE, ID_EVENTBRICK, ID_SCANBACK_PROCOPID, PATH) VALUES (%s, %s, %s, %s, %d)",
	    id_cs_eventbrick, id_candidate, id_eventbrick, id_header_operation, id_path);

    fStmt->setSQL(query);
    Log(2,"AddScanbackPath","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddScanbackPath","TB_B_CSCANDS_SBPATHS added");
    
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
Int_t  TOracleServerE2W::AddView(EdbView *view, int id_view, char *id_eventbrick, char *id_zone, bool usebuffer)
{
  // Adds a view and all its microtracks from an EdbView object into the DB
  // Tables involved: TB_VIEWS through AddView(...) and TB_MIPMICROTRACKS through AddMicroTrack(...)
  // Details: no queries directly executed

  if(!view) {
    Log(1,"AddView","Error! view=0");
    return 1;
  }
    
  int side;
//   if(view->GetNframesTop()==0) side=1;  // 1 - bottom
//   else side=2;                          // 2 - top

  // Warning! inverted definition to make it compatible with the DB
  if(view->GetNframesTop()==0) side=2;
  else side=1;


  EdbSegP sview(0,0,0,0,0);
  sview.Transform(view->GetHeader()->GetAffine());
  float xview = sview.X();
  float yview = sview.Y();
  
  int AreaID = view->GetAreaID();
  int ViewID = view->GetViewID();
  int nsegV  = view->Nsegments();
  
  Log(2,"AddView","Area %d, View %d is in the (DB) side %d and contains %d segments",AreaID,ViewID,side,nsegV);
  Log(2,"AddView","POSX %f, POSY %f",xview,yview);
  Log(2,"AddView","%f %f %f %f",view->GetHeader()->GetZ1(),view->GetHeader()->GetZ2(),
      view->GetHeader()->GetZ3(),view->GetHeader()->GetZ4());
  
  char dataview[2048];
  if (side==1)
    sprintf(dataview,"%s, %s, %d, %d, %f, %f, %f, %f", id_eventbrick, id_zone, side, id_view,
 	    view->GetHeader()->GetZ1(),view->GetHeader()->GetZ2(),xview,yview );
  else
    sprintf(dataview,"%s, %s, %d, %d, %f, %f, %f, %f", id_eventbrick, id_zone, side, id_view,
 	    view->GetHeader()->GetZ4(),view->GetHeader()->GetZ3(),xview,yview );

//   if (side==1)
//     sprintf(dataview,"%s, %s, %d, %d, %f, %f, %f, %f", id_eventbrick, id_zone, side, id_view,
// 	    view->GetHeader()->GetZ3(),view->GetHeader()->GetZ4(),xview,yview );
//   else
//     sprintf(dataview,"%s, %s, %d, %d, %f, %f, %f, %f", id_eventbrick, id_zone, side, id_view,
// 	    view->GetHeader()->GetZ1(),view->GetHeader()->GetZ2(),xview,yview );
  
  AddView(dataview);

  if (nsegV<=0) return(-1);

  if (!usebuffer) {
  
    EdbSegment *seg;
    char datamicro[2048];
    int id_microtrack;
    for(int isegment=0;isegment<nsegV;isegment++) {
      seg = view->GetSegment(isegment);
      seg->Transform(view->GetHeader()->GetAffine());
      //      id_microtrack = id_view*10000 + isegment;
      id_microtrack = id_view*100000 + side*10000 + isegment;
      sprintf(datamicro,"%s, %s ,%d, %d, %d, %f, %f, %f, %f, %d, %d, NULL, %f", id_eventbrick, id_zone, side, id_microtrack, id_view, seg->GetX0(), seg->GetY0(), seg->GetTx(), seg->GetTy(), seg->GetPuls(), seg->GetVolume(), seg->GetSigmaX());
      AddMicroTrack(datamicro);
    }

    Log(2,"AddView","View added (without buffering): %d microtracks added",nsegV);

  } else {

    try{
      if (!fStmt)
	fStmt = fConn->createStatement();
      
      char query[2048];
      sprintf(query,"\
 INSERT INTO OPERA.TB_MIPMICROTRACKS					\
 (ID_EVENTBRICK, ID_ZONE, SIDE, ID, ID_VIEW, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, SIGMA) \
 VALUES (:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12)");
      fStmt->setSQL(query);
      
      char ID_EVENTBRICK[10000][50],ID_ZONE[10000][50];
      int SIDE[10000],ID[10000],ID_VIEW[10000];
      float POSX[10000],POSY[10000],SLOPEX[10000],SLOPEY[10000];
      int GRAINS[10000],AREASUM[10000];
      float SIGMA[10000];
      ub2 SINT[10000],SFLOAT[10000],SID_EVENTBRICK[10000],SID_ZONE[10000];
      
      if (nsegV>10000) {
	Log(1,"AddViewWithBuffer","Error! Number of segments (%d) in the view is greater than 10000",nsegV);
	exit(1);
      }
      
      EdbSegment *seg;
      for(int isegment=0;isegment<nsegV;isegment++) {
	seg = view->GetSegment(isegment);
	seg->Transform(view->GetHeader()->GetAffine());
	sprintf(ID_EVENTBRICK[isegment],"%s%c",id_eventbrick,0);  //  1
	sprintf(ID_ZONE[isegment],"%s%c",id_zone,0);              //  2
	SIDE[isegment]=side;                   //  3
	//	ID[isegment]=id_view*10000 + isegment; //  4
	ID[isegment]=id_view*100000 + side*10000 + isegment; // 4
	ID_VIEW[isegment]=id_view;             //  5
	POSX[isegment]=seg->GetX0();           //  6
	POSY[isegment]=seg->GetY0();           //  7
	SLOPEX[isegment]=seg->GetTx();         //  8
	SLOPEY[isegment]=seg->GetTy();         //  9
	GRAINS[isegment]=seg->GetPuls();       // 10
	AREASUM[isegment]=seg->GetVolume();    // 11
	SIGMA[isegment]=seg->GetSigmaX();      // 13
	
	SID_EVENTBRICK[isegment]=strlen(ID_EVENTBRICK[isegment])+1;
	SID_ZONE[isegment]=strlen(ID_ZONE[isegment])+1;
	SINT[isegment]=sizeof(int);
	SFLOAT[isegment]=sizeof(float);
      }
      
      fStmt->setDataBuffer( 1, ID_EVENTBRICK, OCCI_SQLT_STR, sizeof(ID_EVENTBRICK[0]), (unsigned short *) &SID_EVENTBRICK);
      fStmt->setDataBuffer( 2, ID_ZONE, OCCI_SQLT_STR, sizeof(ID_ZONE[0]), (unsigned short *) &SID_ZONE);
      fStmt->setDataBuffer( 3, SIDE,    OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
      fStmt->setDataBuffer( 4, ID,      OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
      fStmt->setDataBuffer( 5, ID_VIEW, OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
      fStmt->setDataBuffer( 6, POSX,    OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
      fStmt->setDataBuffer( 7, POSY,    OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
      fStmt->setDataBuffer( 8, SLOPEX,  OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
      fStmt->setDataBuffer( 9, SLOPEY,  OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
      fStmt->setDataBuffer(10, GRAINS,  OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
      fStmt->setDataBuffer(11, AREASUM, OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
      fStmt->setDataBuffer(12, SIGMA,   OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
      fStmt->executeArrayUpdate(nsegV);
      
      char commit[10]="commit";
      Query(commit);
      
    } catch (SQLException &oraex) {
      Error("TOracleServerE2W", "AddView; failed: (error: %s)", (oraex.getMessage()).c_str());
    }

    Log(2,"AddView","View added (with buffering): %d microtracks added",nsegV);
     
  }

  return 0;
}


Int_t TOracleServerE2W::AddViews(EdbRun *run, char *id_eventbrick, char *id_zone, bool usebuffer)
{
  // Adds all views stored in a EdbRun object (== raw.root file) and all their microtracks into the DB
  // Tables involved: TB_VIEWS through AddView(EdbView,...)
  // Details: no queries directly executed

  EdbView *view = run->GetView();
  int nviews = run->GetEntries();

  if (!nviews) return 0;

  eNviewsPerArea = 0;
  view = run->GetEntry(0);
  int idfirstarea = view->GetAreaID();

  // count number of views per area
  for(int iview=0; iview<nviews; iview++) {
    view = run->GetEntry(iview);
    if (view->GetAreaID()==idfirstarea) eNviewsPerArea++;
  }
  // divide by 2 because we have 2 views per side
  eNviewsPerArea/=2;

  //  printf("eNviewsPerArea %d\n",eNviewsPerArea);

  for(int iview=0; iview<nviews; iview++) {
    view = run->GetEntry(iview);
    if (gEDBDEBUGLEVEL>=1) { printf("Adding view %d/%d\r",iview+1,nviews); fflush(stdout); }
    //    AddView(view, iview, id_eventbrick, id_zone, usebuffer);
    int id_view = (view->GetAreaID()-1)*eNviewsPerArea+view->GetViewID();
    //    printf("id_view %d %d %d %d\n",view->GetAreaID(),eNviewsPerArea,view->GetViewID(),id_view);
    AddView(view, id_view, id_eventbrick, id_zone, usebuffer);
  }
  if (gEDBDEBUGLEVEL>=1) printf("\n");
  
  return 0;
}

Int_t TOracleServerE2W::AddBaseTracks(TTree *tree, char *id_eventbrick, char *id_zone, bool usebuffer)
{
  // Adds a set of basetracks from a TTree (cp.root file) into the DB,
  // assuming that the corresponding microtracks are already added into the DB
  // Table involved: TB_MIPBASETRACKS through AddBaseTrack(...)
  // Details: no queries directly executed

  if(!tree)  return(0);

  EdbSegP *s1=0, *s2=0, *s=0;
  TBranch *b_s=0, *b_s1=0, *b_s2=0;
  b_s  = tree->GetBranch("s.");
  b_s1 = tree->GetBranch("s1.");
  b_s2 = tree->GetBranch("s2.");
  b_s->SetAddress(  &s   );
//   b_s1->SetAddress( &s1  );
//   b_s2->SetAddress( &s2  );

  // Warning! inverted definition to make it compatible with the DB
  b_s1->SetAddress( &s2  );
  b_s2->SetAddress( &s1  );

  int nentr = (int)(tree->GetEntries());

  if (!usebuffer) {
    
    char data[2048];
    for(int i=0; i<nentr; i++ ) {
      b_s->GetEntry(i);
      b_s1->GetEntry(i);
      b_s2->GetEntry(i);
      //       int id_up   = s1->Vid(0)*10000 + s1->ID();
      //       int id_down = s2->Vid(0)*10000 + s2->ID();
      int id_view_up   = (s1->Aid(0)-1)*eNviewsPerArea+s1->Aid(1);
      int id_view_down = (s2->Aid(0)-1)*eNviewsPerArea+s2->Aid(1);
      int id_up   = id_view_up  *100000 + 10000*2 + s1->ID();
      int id_down = id_view_down*100000 + 10000*1 + s2->ID();

      sprintf(data,"%s, %s, %d, %2f, %2f, %2f, %2f, %2f, %2f, NULL, %2f, %d, %d, %d, %d", 
	      id_eventbrick, id_zone, i, s->X(), s->Y(), s->TX(), s->TY(), s->W(), s->Volume(), s->Chi2(),
	      1, id_down, 2, id_up);
      AddBaseTrack(data);
    }

    Log(2,"AddBaseTracks","Basetracks added (without buffering): %d basetracks added assuming microtracks previously added",nentr);

  } else {
    
    try{
      if (!fStmt)
	fStmt = fConn->createStatement();
      
      char ID_EVENTBRICK[10000][50],ID_ZONE[10000][50];
      int ID[10000];
      float POSX[10000],POSY[10000],SLOPEX[10000],SLOPEY[10000];
      int GRAINS[10000],AREASUM[10000];
      float SIGMA[10000];
      int ID_DOWNSIDE[10000],ID_DOWNTRACK[10000],ID_UPSIDE[10000],ID_UPTRACK[10000];
      ub2 SINT[10000],SFLOAT[10000],SID_EVENTBRICK[10000],SID_ZONE[10000];
      
//       if (nentr>10000) {
// 	Log(1,"AddBaseTracksWithBuffer","Error! Number of segments (%d) is greater than 10000",nentr);
// 	exit(1);
//       }

      int nstep = nentr/10000+1;

      for(int istep=0; istep<nstep; istep++ ) {
	
	char query[2048];
	sprintf(query,"\
 INSERT INTO OPERA.TB_MIPBASETRACKS					\
 (ID_EVENTBRICK, ID_ZONE, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, SIGMA, ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
 VALUES (:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12, :13, :14)");
	fStmt->setSQL(query);

	int ibt=0;

	for(int ibasetrack=10000*istep; (ibasetrack<nentr && ibasetrack<10000*(istep+1)); ibasetrack++ ) {
	  
	  b_s->GetEntry(ibasetrack);
	  b_s1->GetEntry(ibasetrack);
	  b_s2->GetEntry(ibasetrack);
	  
// 	  int id_up   = s1->Vid(0)*10000 + s1->ID();
// 	  int id_down = s2->Vid(0)*10000 + s2->ID();

	  int id_view_up   = (s1->Aid(0)-1)*eNviewsPerArea+s1->Aid(1);
	  int id_view_down = (s2->Aid(0)-1)*eNviewsPerArea+s2->Aid(1);
	  int id_up   = id_view_up  *100000 + 10000*2 + s1->ID();
	  int id_down = id_view_down*100000 + 10000*1 + s2->ID();
	  
	  sprintf(ID_EVENTBRICK[ibt],"%s%c",id_eventbrick,0);  //  1
	  sprintf(ID_ZONE[ibt],"%s%c",id_zone,0);              //  2
	  ID[ibt]=ibasetrack;               //  3
	  POSX[ibt]=s->X();                 //  4
	  POSY[ibt]=s->Y();                 //  5
	  SLOPEX[ibt]=s->TX();              //  6
	  SLOPEY[ibt]=s->TY();              //  7
	  GRAINS[ibt]=(int)s->W();          //  8
	  AREASUM[ibt]=(int)s->Volume();    //  9
	  SIGMA[ibt]=s->Chi2();             // 10
	  ID_DOWNSIDE[ibt]=1;               // 11
	  ID_DOWNTRACK[ibt]=id_down;        // 12
	  ID_UPSIDE[ibt]=2;                 // 13
	  ID_UPTRACK[ibt]=id_up;            // 14
	  
	  SID_EVENTBRICK[ibt]=strlen(ID_EVENTBRICK[ibt])+1;
	  SID_ZONE[ibt]=strlen(ID_ZONE[ibt])+1;
	  SINT[ibt]=sizeof(int);
	  SFLOAT[ibt]=sizeof(float);
	  
	  if (gEDBDEBUGLEVEL>=1) {
	    printf("buffer %d/%d: filling basetrack buffers... %2.0f%%\r",istep+1,nstep,100.*ibasetrack/nentr);
	    fflush(stdout);
	  }
	  ibt++;
	}
	if (gEDBDEBUGLEVEL>=1) printf("\n");
	
	fStmt->setDataBuffer( 1, ID_EVENTBRICK, OCCI_SQLT_STR, sizeof(ID_EVENTBRICK[0]), (unsigned short *) &SID_EVENTBRICK);
	fStmt->setDataBuffer( 2, ID_ZONE, OCCI_SQLT_STR, sizeof(ID_ZONE[0]), (unsigned short *) &SID_ZONE);
	fStmt->setDataBuffer( 3, ID,      OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->setDataBuffer( 4, POSX,    OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
	fStmt->setDataBuffer( 5, POSY,    OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
	fStmt->setDataBuffer( 6, SLOPEX,  OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
	fStmt->setDataBuffer( 7, SLOPEY,  OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
	fStmt->setDataBuffer( 8, GRAINS,  OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->setDataBuffer( 9, AREASUM, OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->setDataBuffer(10, SIGMA,   OCCIFLOAT, sizeof(float), (unsigned short *) &SFLOAT);
	fStmt->setDataBuffer(11, ID_DOWNSIDE,  OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->setDataBuffer(12, ID_DOWNTRACK, OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->setDataBuffer(13, ID_UPSIDE,    OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->setDataBuffer(14, ID_UPTRACK,   OCCIINT,   sizeof(int),   (unsigned short *) &SINT);
	fStmt->executeArrayUpdate(ibt);
	
	char commit[10]="commit";
	Query(commit);
      
      } // istep

    } catch (SQLException &oraex) {
      Error("TOracleServerE2W", "AddBaseTracksWithBuffer; failed: (error: %s)", (oraex.getMessage()).c_str());
    }
    
    Log(2,"AddBaseTracksWithBuffer","Basetracks added (with buffering): %d basetracks added assuming microtracks previously added",nentr);
    
  }

  return nentr;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddVolume(char *id_eventbrick, 
				   char *id_process_operation, 
				   int ivolume)
{
  // Adds a volume into the DB
  // Table involved: TB_VOLUMES
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_VOLUMES (ID_EVENTBRICK, ID_PROCESSOPERATION, VOLUME) \
 VALUES (%s, %s, %d)", id_eventbrick, id_process_operation, ivolume);

    fStmt->setSQL(query);
    Log(2,"AddVolume","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddVolume","Volume added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddVolumeSlice(char *datavolumeslice)
{
  // Adds a volume slice into the DB
  // Table involved: TB_VOLUME_SLICES
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_VOLUME_SLICES \
 (ID_EVENTBRICK, ID_VOLUME, ID_PLATE, MINX, MINY, MAXX, MAXY, ID_ZONE, DAMAGED) \
 VALUES (%s)", datavolumeslice);

    fStmt->setSQL(query);
    Log(2,"AddVolumeSlice","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddVolumeSlice","Volume slice added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddVolumeSlice; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddBSBpathsVolumes(char *databsbpathsvolumes)
{
  // Adds a connection between brick, sbpath and volume into the DB
  // Table involved: TB_B_SBPATHS_VOLUMES
  // Details: INSERT query and then a COMMIT query

  char query[2048];
  char commit[10]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_B_SBPATHS_VOLUMES \
 (ID_EVENTBRICK, ID_SCANBACK_PROCOPID, PATH, ID_VOLUMESCAN_PROCOPID, VOLUME, ID_PLATE) \
 VALUES (%s)", databsbpathsvolumes);

    fStmt->setSQL(query);
    Log(2,"AddBSBpathsVolumes","execute sql query: %s ...",query);
    fStmt->execute();
    Query(commit);
    Log(2,"AddBSBpathsVolumes","BSBpathsVolumes added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "AddBSBpathsVolumes; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
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
	Log(2,"DeleteBrick","execute sql query: %s ...",query[i]);
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

  char query[12][2048];
  char commit[11]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query[0], "delete from Tb_Scanback_Predictions where (id_path) in (select id from tb_scanback_paths where id_processoperation=%s) and id_eventbrick=%s",id_process_operation,id_brick);    
    sprintf(query[1], "delete from Tb_Scanback_Paths where id_processoperation=%s and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[2], "delete from Tb_MipBasetracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s) and id_eventbrick=%s",id_process_operation,id_brick,id_brick);
    sprintf(query[3], "delete from Tb_MipMicrotracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s) and id_eventbrick=%s",id_process_operation,id_brick,id_brick);
    sprintf(query[4], "delete from tb_views where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s) and id_eventbrick=%s",id_process_operation,id_brick,id_brick);
    sprintf(query[5], "delete from tb_b_sbpaths_volumes where id_eventbrick=%s and id_volumescan_procopid=%s",id_brick,id_process_operation);
    sprintf(query[6], "delete from tb_volume_slices where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s))",id_process_operation);
    sprintf(query[7], "delete from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[8], "delete from tb_volumes where id_processoperation=%s",id_process_operation);
    sprintf(query[9], "delete from tb_plate_calibrations where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s)",id_process_operation);
    sprintf(query[10],"delete from tb_proc_operations where id_parent_operation=%s",id_process_operation);
    sprintf(query[11],"delete from tb_proc_operations where id=%s",id_process_operation);

    for (int i=0;i<12;i++) {
	fStmt->setSQL(query[i]);
	Log(2,"DeleteOperation","execute sql query: %s ...",query[i]);
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


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::DeletePlateOperation(char *id_brick, char *id_process_operation, char *id_plate)
{
  // Delete all informations related to a plate of a process operation of a brick from the DB
  // Tables involved: ... a lot, look at the code...
  // Details: DELETE queries and then a COMMIT query

  char query[7][2048];
  char commit[11]="commit";

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query[0], "delete from Tb_Scanback_Predictions where (id_path) in (select id from tb_scanback_paths where id_processoperation=%s) and id_eventbrick=%s and id_plate=%s",id_process_operation,id_brick,id_plate);
    sprintf(query[1], "delete from Tb_MipBasetracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s and id_plate=%s)) and id_eventbrick=%s",id_process_operation,id_plate,id_brick);
    sprintf(query[2], "delete from Tb_MipMicrotracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s and id_plate=%s)) and id_eventbrick=%s",id_process_operation,id_plate,id_brick);
    sprintf(query[3], "delete from tb_views where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s and id_plate=%s))",id_process_operation,id_plate);
    sprintf(query[4], "delete from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s and id_plate=%s)",id_process_operation,id_plate);
    sprintf(query[5], "delete from tb_plate_calibrations where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s and id_plate=%s)",id_process_operation,id_plate);
    sprintf(query[6], "delete from tb_proc_operations where id_parent_operation=%s and id_plate=%s",id_process_operation,id_plate);

    for (int i=0;i<7;i++) {
	fStmt->setSQL(query[i]);
	Log(2,"DeletePlateOperation","execute sql query: %s ...",query[i]);
	fStmt->execute();
    }
    Query(commit);

    Log(2,"DeletePlateOperation","Plate operation deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2W", "DeletePlateOperation; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}
