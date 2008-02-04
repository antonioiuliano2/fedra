#include "TOracleServerE2.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"

ClassImp(TOracleServerE2)

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume_NI(ULong64_t id_volume, EdbPatternsVolume &vol)
{
        char id[24];
        sprintf(id,"%lld",id_volume);
        return ReadVolume_NI(id,vol);
}
//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume_NI(ULong64_t id_volume, EdbPatternsVolume &vol, Int_t min_pl, Int_t max_pl)
{
        char id[24];
        Int_t min= min_pl;
        Int_t max= max_pl;
        sprintf(id,"%lld",id_volume);
        return ReadVolume_NI(id,vol,min,max);
}
//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume_NI(char *id_volume, EdbPatternsVolume &vol)
{
  int  nsegtot=0;
  char query[2048];
  TObjArray patterns(100);
  EdbPattern *pat=0;
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
    if( !ReadZplate_NI(pat->ID(), pat->PID(), *pat ) )
      { printf("skip plate %d %d !\n", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_zone in (select id_zone from TB_VOLUME_SLICES where id_volume=%s and id_plate=%d)", id_volume,pat->PID());
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}
//------------------------------------------------------------------------------------
Int_t  TOracleServerE2::ReadVolume_NI(char *id_volume, EdbPatternsVolume &vol, Int_t min, Int_t max)
{
  int  nsegtot=0;
  char query[2048];
  TObjArray patterns(100);
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id_plate,id_eventbrick from TB_VOLUME_SLICES where (id_volume=%s and id_plate > %d-1 and id_plate < %d+1) order by id_plate"
, id_volume,min,max);
    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
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
    if( !ReadZplate_NI(pat->ID(), pat->PID(), *pat ) )
      { printf("skip plate %d %d !\n", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_zone in (select id_zone from TB_VOLUME_SLICES where id_volume=%s and id_plate=%d)", id_volume,pat->PID());
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
bool  TOracleServerE2::ReadZplate_NI(int id_eventbrick, int id_plate, EdbPattern &pat)
{
  char *query= new char[2048];
  Float_t Z=0;

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,
            "select z from tb_plates \
             where id_eventbrick=%d and id=%d",
            id_eventbrick, id_plate);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      Z = rs->getFloat(1);
      printf("z=%f    ",Z);
    }
    delete rs;
    printf("z=%f    ",Z);
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
Int_t  TOracleServerE2::ReadBasetracksPattern(char *selection, EdbPattern &pat)
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
Int_t  TOracleServerE2::ReadMicrotracksPattern(char *selection, EdbPattern &pat)
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
Int_t TOracleServerE2::GetProcessOperationID(char *id_eventbrick, char *id)
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
Int_t TOracleServerE2::GetProcessOperationID(char *id_programsettings, char *id_eventbrick, char *id_plate, char *id)
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
