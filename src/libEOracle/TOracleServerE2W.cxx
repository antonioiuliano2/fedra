#include "TOracleServerE2W.h"
#include "EdbPattern.h"
#include "EdbAffine.h"

ClassImp(TOracleServerE2W)

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2W::AddBrick(char *id, 
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
Int_t  TOracleServerE2W::AddPlate(char *data)
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
Int_t  TOracleServerE2W::UpdatePlate(char *id_eventbrick, char *id_plate, char *calibration)
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
Int_t  TOracleServerE2W::AddZone(char *data)
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
Int_t  TOracleServerE2W::AddZone(EdbPattern &pat, char *data1, char *data2)
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
Int_t  TOracleServerE2W::AddBasetracks(EdbPattern &pat, char *id_eventbrick, char *id_zone)
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



Int_t  TOracleServerE2W::AddProcessOperation(char *id_machine, 
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


