#include "TOracleServerE2WX.h"
#include "EdbLog.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"
#include "EdbSegment.h"
#include "EdbRun.h"
#include "EdbRunTracking.h"
#include "TTimeStamp.h"

using namespace TMath;

ClassImp(EdbScan2DB)
ClassImp(TOracleServerE2WX)


//------------------------------------------------------------------------------------
void TOracleServerE2WX::Set0()
{
  eTestLoad=kMaxInt;
  eDoCommit=0;
  //eNviewsPerArea=0;
}
 
//------------------------------------------------------------------------------------
void TOracleServerE2WX::Print()
{
  TOracleServerE2::Print();
  printf( " eDoCommit  = %d  \n", eDoCommit );
  printf( " eTestLoad  = %d  \n", eTestLoad );
  if(eTestLoad) printf( " eNTestLoad = %d  \n", eNTestLoad );
}
 
//------------------------------------------------------------------------------------
const char *TOracleServerE2WX::Timestamp()
{
  TTimeStamp timestamp;
  return Form("timestamp'%s'",timestamp.AsString("s"));
}

//------------------------------------------------------------------------------------
void TOracleServerE2WX::FinishTransaction()
{
  if(eTestLoad) return; // do nothing!
  try{
    if(eDoCommit) MyQuery("COMMIT"); 
    else          MyQuery("ROLLBACK");
  } catch (SQLException &oraex) {
    Log(1,"TOracleServerE2WX::FinishTransaction","failed: (error: %s)",
        (oraex.getMessage()).c_str());
  }
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::MyQuery(const char *query)
{
  try{
    if (!fStmt)   fStmt = fConn->createStatement();
    fStmt->setSQL(query);
    Log(2,"TOracleServerE2WX::MyQuery","%s",query);
    fStmt->execute();
  } catch (SQLException &oraex) {
    Log(1,"TOracleServerE2WX::MyQuery","failed: (error: %s)",(oraex.getMessage()).c_str());
    eDoCommit=0;
    return 0;
  }
  return 1;
}

//------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WX::MyQueryInsertReturning( const char *query, const char *var )
{
  ULong64_t  id=0;
  try{
    if (!fStmt)   fStmt = fConn->createStatement();
    TString s=Form("BEGIN %s returning %s into :1; END;",query,var);
    Log(2,"TOracleServerE2WX::MyQueryInsertReturning","%s",s.Data());
    fStmt->setSQL(s.Data());
    fStmt->registerOutParam(1, OCCISTRING,64);
    fStmt->executeUpdate();
    if(!(fStmt->getString(1)).c_str()) {
      Log(2,"TOracleServerE2WX::MyQueryInsertReturning","ERROR! empty operation returned!");
      eDoCommit=0;
      return 0;
    }
    Log(2,"","%s", (fStmt->getString(1)).c_str() );
    sscanf( (fStmt->getString(1)).c_str(),"%lld",&id);
  } catch (SQLException &oraex) {
    Log(1,"TOracleServerE2WX::MyQuery","failed: (error: %s)",(oraex.getMessage()).c_str());
    eDoCommit=0;
    return 0;
  }
  return id;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddBaseTrack( const char *data )
{
  return MyQuery( Form( "\
      INSERT INTO OPERA.TB_MIPBASETRACKS(\
      ID_EVENTBRICK, ID_ZONE, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, SIGMA,\
      ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
      VALUES (%s)", data) );
}
//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddMicroTrack(char *datamicro)
{
  return MyQuery( Form( "\
      INSERT INTO OPERA.TB_MIPMICROTRACKS(ID_EVENTBRICK, ID_ZONE, SIDE, ID, ID_VIEW, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, SIGMA) \
            VALUES (%s)", datamicro
                      ));
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddScanbackPrediction(char *dataprediction)
{
  return MyQuery( Form( "\
      INSERT INTO OPERA.TB_SCANBACK_PREDICTIONS(\
      ID_EVENTBRICK,ID_PATH,ID_PLATE,POSX,POSY,SLOPEX,SLOPEY,\
      POSTOL1,POSTOL2,SLOPETOL1,SLOPETOL2,\
      FRAME,ID_ZONE,ID_CANDIDATE,DAMAGED,ISMANUAL)VALUES (%s)"
      ,dataprediction
                      ));
}

//-------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WX::AddProcessOperation(
    ULong64_t  id_machine,
    ULong64_t  id_programsettings, 
    ULong64_t  id_requester, 
    ULong64_t  id_parent_operation,
    ULong64_t  id_eventbrick, 
    Int_t      id_plate,
    Int_t      driverlevel,
    ULong64_t  id_calibration,
    const char *starttime,
    const char *finishtime,
    const char *success,
    const char *notes       )
{
  // Adds a process operation into the DB
  // Table involved: TB_PROC_OPERATIONS
  
  return MyQueryInsertReturning(
      Form(
        "INSERT INTO OPERA.TB_PROC_OPERATIONS ( \
        ID_MACHINE, ID_PROGRAMSETTINGS, ID_REQUESTER, \
        ID_PARENT_OPERATION, ID_EVENTBRICK, ID_PLATE, DRIVERLEVEL, ID_CALIBRATION_OPERATION, \
        STARTTIME, FINISHTIME, SUCCESS, NOTES) \
        VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, '%s' )",
    Ostr(id_machine),
    Ostr(id_programsettings),
    Ostr(id_requester),
    Ostr(id_parent_operation),
    Ostr(id_eventbrick),
    Ostr(id_plate),
    Ostr(driverlevel),
    Ostr(id_calibration),
    starttime,
    finishtime,
    success,
    notes
          ), "ID" );
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddEventBrick(const char *databrick)
{
  // Adds a brick into the DB
  // Table involved: TB_EVENTBRICKS

  return MyQuery(
      Form( "\
      INSERT INTO OPERA.TB_EVENTBRICKS \
      (ID, MINX, MAXX, MINY, MAXY, MINZ, MAXZ, ID_SET, ID_BRICK, ZEROX, ZEROY, ZEROZ) VALUES (%s)", 
  databrick
          )   );
}

//------------------------------------------------------------------------------------
ULong64_t TOracleServerE2WX::IfEventBrick(ULong64_t id_eventbrick, const char *id_set )
{
  // Check if brick structure is in DB
  ULong64_t id = 0, id_brick = 0;
  if(MyQuery(Form( "\
     SELECT ID, ID_BRICK FROM OPERA.TB_EVENTBRICKS \
     WHERE ID=%lld and ID_SET=%s",  id_eventbrick,id_set))) 
  {
    if(fStmt) {
      ResultSet *rs = fStmt->getResultSet();
      if(rs) {
        while (rs->next()){
          sscanf( (rs->getString(1)).c_str(),"%lld",&id);
          sscanf( (rs->getString(2)).c_str(),"%lld",&id_brick);
        }
        delete rs;
      }
    }
  }
  if(id)  Log(1,"TOracleServerE2WFB::IfEventBrick","brick ID=%lld of set=%s is found in DB with ID_BRICK=%lld",id_eventbrick,id_set,id_brick);
  else    Log(1,"TOracleServerE2WFB::IfEventBrick","brick ID=%lld of set=%s is not found in DB!",id_eventbrick,id_set);
  return id;
}

/*
//------------------------------------------------------------------------------------
Int_t TOracleServerE2WX::IfEventBrick(ULong64_t id_eventbrick, const char *id_set )
{
  // Check if brick structure is in DB
  
  if(MyQuery(Form( "\
     SELECT * FROM OPERA.TB_EVENTBRICKS \
     WHERE ID=%lld and ID_SET=%s",  id_eventbrick,id_set))) 
  {
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
*/

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddPlate(ULong64_t id_eventbrick, const char *dataplate)
{
  // Adds a plate into the DB
  // Table involved: TB_PLATE

  return MyQuery(Form( "\
      INSERT INTO OPERA.TB_PLATES (ID_EVENTBRICK, ID, Z) \
      VALUES (%s, %s)",  Ostr(id_eventbrick),  dataplate));
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddPlateCalibration( ULong64_t id_eventbrick, ULong64_t id_process_operation, EdbPlateP *plate)
{
  // Adds a plate calibration into the DB
  // Table involved: TB_PLATE_CALIBRATIONS
  EdbAffine2D *a = plate->GetAffineXY();
  if(a) 
  {
    if(!MyQuery(Form( "\
        INSERT INTO OPERA.TB_PLATE_CALIBRATIONS \
        (ID_EVENTBRICK, ID_PROCESSOPERATION, ID_PLATE, Z, MAPXX, MAPXY, MAPYX, MAPYY, MAPDX, MAPDY) \
        VALUES (%s, %s, %d, %.2f, %f, %f, %f, %f, %f, %f)", 
    Ostr(id_eventbrick),
    Ostr(id_process_operation),
    plate->ID(), plate->Z(),
    a->A11(), a->A12(), a->A21(), a->A22(), a->B1(), a->B2() )))    return 0;
  
    Log(3,"TOracleServerE2WX:AddPlateCalibration","ok");
    return 1;
  }
  return 0;
}

//------------------------------------------------------------------------------------
Int_t TOracleServerE2WX::AddViews(EdbRunTracking &rt, ULong64_t id_eventbrick, ULong64_t id_zone, bool usebuffer)
{
  // Adds all views stored in a EdbRun object (== raw.root file) and all their microtracks into the DB
  // Tables involved: TB_VIEWS through AddView(EdbView,...)
  // Details: no queries directly executed

  EdbRun *r = rt.GetRun();       if (!r)      return 0;
  int nviews = r->GetEntries();  if (!nviews) return 0;
  int nvpa=rt.GetNviewsPerArea();

  if(eTestLoad) nviews = Min(nviews,eNTestLoad);   // load only few views
  for(int iview=0; iview<nviews; iview++) {
    EdbView *view = r->GetEntry(iview);
    if(view) 
    {
      Log(2,"TOracleServerE2WX::AddView","%d/%d",iview+1,nviews);
      
      int id_view = (view->GetAreaID())*nvpa+view->GetViewID();
      AddView(view, id_view, id_eventbrick, id_zone, usebuffer);
      
      ///int id_view = iview;
      ///AddView(view, id_view, id_eventbrick, id_zone, usebuffer);
    
    }
    else Log(1,"TOracleServerE2WX::AddView","Error! view=0");
  }
  return 0;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddView(EdbView *view, int id_view, ULong64_t id_eventbrick, ULong64_t id_zone, bool usebuffer)
{
  // Adds a view and all its microtracks from an EdbView object into the DB
  // Tables involved: TB_VIEWS through AddView(...) and TB_MIPMICROTRACKS through AddMicroTrack(...)
  // Details: no queries directly executed

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
  if(eTestLoad) nsegV = Min(nsegV,eNTestLoad);   // load only few segments
  Log(2,"TOracleServerE2WX::AddView","Area %d, View %d is in the (DB) side %d and contains %d segments",AreaID,ViewID,side,nsegV);
  
  float z1,z2;
  if (side==1) { z1 = view->GetHeader()->GetZ1(); z2 = view->GetHeader()->GetZ2(); }
  else         { z1 = view->GetHeader()->GetZ4(); z2 = view->GetHeader()->GetZ3(); }

  MyQuery(
      Form(
      "INSERT INTO OPERA.TB_VIEWS (ID_EVENTBRICK, ID_ZONE, SIDE, ID, DOWNZ, UPZ, POSX, POSY) VALUES (%s, %s, %d, %d, %.2f, %.2f, %.2f, %.2f)",
  Ostr(id_eventbrick),
  Ostr(id_zone),
  side,
  id_view,
  z1,
  z2,
  xview,
  yview
          ));
  
  if (nsegV<=0) return(-1);
  if (!usebuffer) 
  {
    EdbSegment *seg;
    int id_microtrack;
    
    //if(nsegV>10) nsegV =10; //!!!!!!!!!!!!!!
    
    for(int isegment=0;isegment<nsegV;isegment++) {
      seg = view->GetSegment(isegment);
      seg->Transform(view->GetHeader()->GetAffine());
      id_microtrack = id_view*100000 + side*10000 + isegment;
      ///id_microtrack = id_view*100000+isegment;
      MyQuery( Form("\
          INSERT INTO OPERA.TB_MIPMICROTRACKS(ID_EVENTBRICK, ID_ZONE, SIDE, ID, ID_VIEW, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, SIGMA) \
          VALUES (%s, %s ,%d, %d, %d, %f, %f, %f, %f, %d, %d, %f)",
      Ostr(id_eventbrick), 
      Ostr(id_zone), 
      side, id_microtrack, id_view, seg->GetX0(), seg->GetY0(),
      seg->GetTx(), seg->GetTy(), seg->GetPuls(), seg->GetVolume(), seg->GetSigmaX()
                   ));
    }

    Log(2,"TOracleServerE2WX::AddView","View added (without buffering): %d microtracks added",nsegV);

  } else {

    try{
      if (!fStmt) fStmt = fConn->createStatement();
      
      char query[2048];
      sprintf(query,"\
          INSERT INTO OPERA.TB_MIPMICROTRACKS          \
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
        sprintf(ID_EVENTBRICK[isegment],"%s%c",Ostr(id_eventbrick),0);  //  1
        sprintf(ID_ZONE[isegment],"%s%c",Ostr(id_zone),0);              //  2
        SIDE[isegment]=side;                   //  3
        ///ID[isegment]=id_view*100000 + isegment; //  4
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
    } catch (SQLException &oraex) {
      Error("TOracleServerE2WX", "AddView; failed: (error: %s)", (oraex.getMessage()).c_str());
    }
    Log(2,"AddView","View added (with buffering): %d microtracks added",nsegV);
  }

  return 0;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddBaseTracks(EdbPattern &pat, ULong64_t id_eventbrick, ULong64_t id_zone)
{
  // Adds a basetrack into the DB
  // Table involved: TB_MIPBASETRACKS
  Int_t nseg = pat.N();
  if(eTestLoad) nseg = Min(nseg,eNTestLoad);   // load only few segments
  for(int i=0; i<nseg; i++)
  {
    EdbSegP *s = pat.GetSegment(i);
    if(s) {
      MyQuery(Form("\
          INSERT INTO OPERA.TB_MIPBASETRACKS (\
          ID_EVENTBRICK, ID_ZONE, POSX, POSY, SLOPEX, SLOPEY, GRAINS,\
          AREASUM, PH, SIGMA, ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
          VALUES (%s, %s, %.2f, %.2f, %.2f, %.2f, %d, %d, %d, %d, %d, %d, %d, %d)",
      Ostr(id_eventbrick),
      Ostr(id_zone), 
      s->X(), s->Y(), s->TX(), s->TY(), (int)s->W(), (int)s->Volume(),
      0, 0, 0, 0, 0, 0)
             );
    }
  }
  return pat.N();
}

//------------------------------------------------------------------------------------
Int_t TOracleServerE2WX::AddBaseTracks(TTree *tree, ULong64_t id_eventbrick, ULong64_t id_zone, Int_t nvpa, bool usebuffer)
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
  /// Warning! inverted definition to make it compatible with the DB
  b_s1->SetAddress( &s2  );
  b_s2->SetAddress( &s1  );

  int nentr = (int)(tree->GetEntries());
  if(eTestLoad) nentr = Min(nentr,eNTestLoad);   // load only few segments

  if (!usebuffer) {
    for(int i=0; i<nentr; i++ ) {
      b_s->GetEntry(i);
      b_s1->GetEntry(i);
      b_s2->GetEntry(i);

      ///Int_t id_up   = s1->Vid(0)*100000 + s1->Vid(1);
      ///Int_t id_down = s2->Vid(0)*100000 + s2->Vid(1);
      //!!! the above convention used by VT should always work but probably some service is lost (to check)
      //!!! the below convention used by L.Scotto but it's rely on eNviewsPerArea which not always defined correctly
      //printf("nvpa=%d  s1->Aid(0)=%d  s1->Aid(1)=%d  s1->Vid(1)=%d \n", nvpa, s1->Aid(0),s1->Aid(1), s1->Vid(1)%100000 );
      int id_view_up   = (s1->Aid(0))*nvpa+s1->Aid(1);
      int id_view_down = (s2->Aid(0))*nvpa+s2->Aid(1);
      int id_up   = id_view_up  *100000 + 10000*2 + s1->Vid(1)%100000;
      int id_down = id_view_down*100000 + 10000*1 + s2->Vid(1)%100000;

      MyQuery( Form("\
          INSERT INTO OPERA.TB_MIPBASETRACKS(\
          ID_EVENTBRICK, ID_ZONE, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM,\
          SIGMA, ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
          VALUES \
          (%s, %s, %d, %2f, %2f, %2f, %2f, %2f, %2f, %2f, %d, %d, %d, %d)",
      Ostr(id_eventbrick), 
      Ostr(id_zone), 
      i, s->X(), s->Y(), s->TX(), s->TY(), s->W(), s->Volume(), s->Chi2(),
      1, id_down, 2, id_up
                   ));
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

      int nstep = nentr/10000+1;
      for(int istep=0; istep<nstep; istep++ ) {
  
        char query[2048];
        sprintf(query,"\
            INSERT INTO OPERA.TB_MIPBASETRACKS         \
                (ID_EVENTBRICK, ID_ZONE, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, SIGMA, ID_DOWNSIDE, ID_DOWNTRACK, ID_UPSIDE, ID_UPTRACK) \
                VALUES (:1, :2, :3, :4, :5, :6, :7, :8, :9, :10, :11, :12, :13, :14)");
        fStmt->setSQL(query);

        int ibt=0;

        for(int ibasetrack=10000*istep; (ibasetrack<nentr && ibasetrack<10000*(istep+1)); ibasetrack++ ) {
    
          b_s->GetEntry(ibasetrack);
          b_s1->GetEntry(ibasetrack);
          b_s2->GetEntry(ibasetrack);
    
          ///Int_t id_up   = s1->Vid(0)*100000 + s1->Vid(1)%100000;
          ///Int_t id_down = s2->Vid(0)*100000 + s2->Vid(1)%100000;
          
          int id_view_up   = (s1->Aid(0))*nvpa+s1->Aid(1);
          int id_view_down = (s2->Aid(0))*nvpa+s2->Aid(1);
          int id_up   = id_view_up  *100000 + 10000*2 + s1->Vid(1)%100000;
          int id_down = id_view_down*100000 + 10000*1 + s2->Vid(1)%100000;
    
          sprintf(ID_EVENTBRICK[ibt],"%s%c",Ostr(id_eventbrick),0);  //  1
          sprintf(ID_ZONE[ibt],"%s%c",Ostr(id_zone),0);              //  2
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
          ibt++;
        }
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
      } // istep

    } catch (SQLException &oraex) {
      Error("TOracleServerE2WX", "AddBaseTracksWithBuffer; failed: (error: %s)", (oraex.getMessage()).c_str());
    }
    
    Log(2,"AddBaseTracksWithBuffer","Basetracks added (with buffering): %d basetracks added assuming microtracks previously added",nentr);
    
  }

  return nentr;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddScanbackPath( ULong64_t id_eventbrick, ULong64_t id_header_operation, int id_path, int id_start_plate, int skipCSconnection)
{
  // Adds a scanback path into the DB and connects it with the CS prediction
  // Table involved: TB_SCANBACK_PATHS, TB_B_CSCANDS_SBPATHS
  
  MyQuery( Form("\
      INSERT INTO OPERA.TB_SCANBACK_PATHS (ID_EVENTBRICK, ID_PROCESSOPERATION, PATH, ID_START_PLATE, ID_FORK_PATH, ID_CANCEL_PLATE) VALUES (%s, %s, %d, %d, NULL, NULL)",
  Ostr(id_eventbrick), 
  Ostr(id_header_operation), 
  id_path, 
  id_start_plate
               ));

  if (!skipCSconnection) {
    char query[2048];
    try{
      if (!fStmt) fStmt = fConn->createStatement();
        
      sprintf(query,"select id_cs_eventbrick,idcand from vw_local_cs_candidates where id_cs_eventbrick in (select id from tb_eventbricks where id_brick in (select id_brick from tb_eventbricks where id=%s)) and cand=%d",
              Ostr(id_eventbrick), id_path%10000);
      
      int id_path_copy = id_path/10000;
      int icopy=0;

      fStmt->setSQL(query);
      Log(2,"AddScanbackPath","execute sql query: %s ...",query);
      fStmt->execute();
      ResultSet *rs = fStmt->getResultSet();
      char id_cs_eventbrick[20],id_candidate[20];
      while (rs->next()&&icopy<=id_path_copy){
        strcpy(id_cs_eventbrick,rs->getString(1).c_str());
        strcpy(id_candidate,rs->getString(2).c_str());
        icopy++;
      }
      delete rs;
      
      
      sprintf(query,"\
          INSERT INTO OPERA.TB_B_CSCANDS_SBPATHS (ID_CS_EVENTBRICK, ID_CANDIDATE, ID_EVENTBRICK, ID_SCANBACK_PROCOPID, PATH) VALUES (%s, %s, %s, %s, %d)",
          id_cs_eventbrick, id_candidate, Ostr(id_eventbrick), Ostr(id_header_operation), id_path);
      
      fStmt->setSQL(query);
      Log(2,"AddScanbackPath","execute sql query: %s ...",query);
      fStmt->execute();

      Log(2,"AddScanbackPath","TB_B_CSCANDS_SBPATHS added");
    
    } catch (SQLException &oraex) {
      Error("TOracleServerE2WX", "AddScanbackPath; failed: (error: %s)", (oraex.getMessage()).c_str());
    }
  } // end !skipCSconnection

  return 0;
}

/*
//------------------------------------------------------------------------------------
void  TOracleServerE2WX::AddZone(char *data)
{
    MyQuery(Form("\
        INSERT INTO OPERA.TB_ZONES (ID_EVENTBRICK, ID_PLATE, ID_PROCESSOPERATION, MINX, MAXX, MINY, MAXY, RAWDATAPATH, STARTTIME, ENDTIME, SERIES,\
            TXX, TXY, TYX, TYY, TDX, TDY)VALUES (%s)",data);
}

*/

//------------------------------------------------------------------------------------
ULong64_t  TOracleServerE2WX::AddVolume( ULong64_t id_eventbrick,
                                         ULong64_t id_process_operation,
                                         int       ivolume )
{
  return MyQueryInsertReturning(
      Form("\
      INSERT INTO OPERA.TB_VOLUMES (ID_EVENTBRICK, ID_PROCESSOPERATION, VOLUME) \
      VALUES (%s, %s, %d)",
  Ostr(id_eventbrick),
  Ostr(id_process_operation),
  ivolume
          ), "ID" );
}















































//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddBrick_Set(
				      char *id, 
				      char *idrange_min, 
				      char *idrange_max, 
				      char *id_partition)
{
  // Adds a brick-set into the DB
  // Procedure involved: PC_ADD_BRICK_SET (it fills TB_BRICK_SETS)

  char query[2048];
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

    Log(2,"AddBrick_Set","Brick_Set added, partition created");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "AddBrick_Set; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddBrick_Space(
					char *id_brick, 
					char *id_set)
{
  // Adds a brick-space into the DB
  // Procedure involved: PC_ADD_BRICK_SPACE

  char query[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"CALL PC_ADD_BRICK_SPACE(%s, %s)",
 	    id_brick, id_set);

    fStmt->setSQL(query);
    Log(2,"AddBrick_Space","execute sql query: %s ...",query);
    fStmt->executeUpdate();

    Log(2,"AddBrick_Space","Brick_Space mapped");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "AddBrick_Space; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddPlateCalibration(char *id_eventbrick,
					    char *id_process_operation,
					    char *datacalibration)	    

{
  // Adds a plate calibration into the DB
  // Table involved: TB_PLATE_CALIBRATIONS

  char query[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    
    sprintf(query,"\
 INSERT INTO OPERA.TB_PLATE_CALIBRATIONS (ID_EVENTBRICK, ID_PROCESSOPERATION, ID_PLATE, Z, MAPXX, MAPXY, MAPYX, MAPYY, MAPDX, MAPDY) \
 VALUES (%s, %s, %s)", id_eventbrick, id_process_operation, datacalibration);

    fStmt->setSQL(query);
    Log(2,"AddPlateCalibration","execute sql query: %s ...",query);
    fStmt->execute();
    Log(2,"AddPlateCalibration","PlateCalibration added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "AddPlateCalibration; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}





//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddTemplateMarkSets(char *datamarks)
{
  // Adds a mark into the DB
  // Table involved: TB_TEMPLATEMARKSETS

  char query[2048];

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
    Log(2,"AddTemplateMarkSets","TemplateMarkSets added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "AddTemplateMarkSets; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}



//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddVolumeSlice(char *datavolumeslice)
{
  // Adds a volume slice into the DB
  // Table involved: TB_VOLUME_SLICES

  char query[2048];

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
    Log(2,"AddVolumeSlice","Volume slice added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "AddVolumeSlice; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::AddBSBpathsVolumes(char *databsbpathsvolumes)
{
  // Adds a connection between brick, sbpath and volume into the DB
  // Table involved: TB_B_SBPATHS_VOLUMES

  char query[2048];

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
    Log(2,"AddBSBpathsVolumes","BSBpathsVolumes added");
    
  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "AddBSBpathsVolumes; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 0;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::DeleteBrick(char *id_eventbrick)
{
  // Delete all informations related to a brick from the DB
  // Tables involved: ... a lot, look at the code...

  char query[16][2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query[0], "delete from Tb_B_SBPATHS_VOLUMES    where id_eventbrick=%s",id_eventbrick);
    sprintf(query[1], "delete from Tb_Volume_Slices        where id_eventbrick=%s",id_eventbrick);
    sprintf(query[2], "delete from Tb_Volumes              where id_eventbrick=%s",id_eventbrick);
    sprintf(query[3], "delete from Tb_Scanback_Predictions where id_eventbrick=%s",id_eventbrick);
    sprintf(query[4], "delete from Tb_B_CSCANDS_SBPATHS    where id_eventbrick=%s",id_eventbrick);
    sprintf(query[5], "delete from Tb_Scanback_Paths       where id_eventbrick=%s",id_eventbrick);
    sprintf(query[6], "delete from Tb_MipBasetracks        where id_eventbrick=%s",id_eventbrick);
    sprintf(query[7], "delete from Tb_MipMicrotracks       where id_eventbrick=%s",id_eventbrick);
    sprintf(query[8], "delete from Tb_Views                where id_eventbrick=%s",id_eventbrick);
    sprintf(query[9], "delete from Tb_Zones                where id_eventbrick=%s",id_eventbrick);
    sprintf(query[10],"delete from tb_proc_operations where id_eventbrick=%s and id_calibration_operation in (select id_processoperation from tb_plate_calibrations where id_eventbrick=%s)",id_eventbrick,id_eventbrick);
    sprintf(query[11],"delete from Tb_Plate_calibrations   where id_eventbrick=%s",id_eventbrick);
    sprintf(query[12],"delete from Tb_Proc_operations      where id_eventbrick=%s",id_eventbrick);
    sprintf(query[13],"delete from Tb_Plates               where id_eventbrick=%s",id_eventbrick);
    sprintf(query[14],"delete from Tb_TemplateMarkSets     where id_eventbrick=%s",id_eventbrick);
    sprintf(query[15],"delete from Tb_Eventbricks          where id=%s",id_eventbrick);

    for (int i=0;i<16;i++) {
	fStmt->setSQL(query[i]);
	Log(2,"DeleteBrick","execute sql query: %s ...",query[i]);
	fStmt->execute();
    }

    Log(2,"DeleteBrick","Brick deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "DeleteBrick; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::DeleteBrickSpace(char *id_brick)
{
  // Delete a brick space related to a brick from the DB
  // Procedure involved: PC_REMOVE_BRICK_SPACE

  char query[2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"call pc_remove_brick_space(%s,'OPERA NA BK01')",id_brick);

    fStmt->setSQL(query);
    Log(2,"DeleteBrickSpace","execute sql query: %s ...",query);
    fStmt->execute();

    Log(2,"DeleteBrickSpace","BrickSpace deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "DeleteBrickSpace; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::DeleteOperation(char *id_brick, char *id_process_operation)
{
  // Delete all informations related to a process operation of a brick from the DB
  // Tables involved: ... a lot, look at the code...
  // Details: DELETE queries

  char query[13][2048];

  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query[0], "delete from Tb_Scanback_Predictions where (id_path) in (select id from tb_scanback_paths where id_processoperation=%s) and id_eventbrick=%s",id_process_operation,id_brick);    
    sprintf(query[1], "delete from Tb_B_CSCANDS_SBPATHS where id_scanback_procopid=%s and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[2], "delete from Tb_Scanback_Paths where id_processoperation=%s and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[3], "delete from Tb_MipBasetracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s) and id_eventbrick=%s",id_process_operation,id_brick,id_brick);
    sprintf(query[4], "delete from Tb_MipMicrotracks where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s) and id_eventbrick=%s",id_process_operation,id_brick,id_brick);
    sprintf(query[5], "delete from tb_views where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s) and id_eventbrick=%s",id_process_operation,id_brick,id_brick);
    sprintf(query[6], "delete from tb_b_sbpaths_volumes where id_eventbrick=%s and id_volumescan_procopid=%s",id_brick,id_process_operation);
    sprintf(query[7], "delete from tb_volume_slices where (id_zone) in (select id from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s))",id_process_operation);
    sprintf(query[8], "delete from tb_zones where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s) and id_eventbrick=%s",id_process_operation,id_brick);
    sprintf(query[9], "delete from tb_volumes where id_processoperation=%s",id_process_operation);
    sprintf(query[10], "delete from tb_plate_calibrations where (id_processoperation) in (select id from tb_proc_operations where id_parent_operation=%s)",id_process_operation);
    sprintf(query[11],"delete from tb_proc_operations where id_parent_operation=%s",id_process_operation);
    sprintf(query[12],"delete from tb_proc_operations where id=%s",id_process_operation);

    for (int i=0;i<13;i++) {
	fStmt->setSQL(query[i]);
	Log(2,"DeleteOperation","execute sql query: %s ...",query[i]);
	fStmt->execute();
    }

    Log(2,"DeleteOperation","Operation deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "DeleteOperation; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}


//------------------------------------------------------------------------------------
Int_t  TOracleServerE2WX::DeletePlateOperation(char *id_brick, char *id_process_operation, char *id_plate)
{
  // Delete all informations related to a plate of a process operation of a brick from the DB
  // Tables involved: ... a lot, look at the code...
  // Details: DELETE queries

  char query[7][2048];

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

    Log(2,"DeletePlateOperation","Plate operation deleted");
    return 0;

  } catch (SQLException &oraex) {
    Error("TOracleServerE2WX", "DeletePlateOperation; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  return 1;
}




//------------------------------------------------------------------------------------
EdbScan2DB::EdbScan2DB()
{
  eDB=0;
  eBrick=0;
  eEvent=0;
  eEventBrick=0;
  eIsBlackCS=0;
  eIdMachine=0;
  eIdRequester=0;
  eHeaderOperation=0;
  eCalibrationOperation=0;
  ePredictionOperation=0;
  eVolumeOperation=0;
  eHeaderProgramsettings=0;
  eCalibrationProgramsettings=0;
  ePredictionProgramsettings=0;
  eVolumeProgramsettings=0;
  eX_marks=0;
  eWriteRawCalibrationData = 0;
  eERROR=0;
}

//------------------------------------------------------------------------------------
void EdbScan2DB::Print()
{
  printf("\n=============== EdbScan2DB settings ================\n");
  printf("eBrick                      = %lld\n", eBrick);
  printf("eEvent                      = %lld\n", eEvent);
  printf("eEventBrick                 = %lld\n",eEventBrick);
  printf("eID_SET                     = %s\n",eID_SET.Data());
  printf("eIdMachine                  = %lld\n", eIdMachine);
  printf("eIdRequester                = %lld\n",eIdRequester );
  printf("eHeaderProgramsettings      = %lld\n", eHeaderProgramsettings);
  printf("eCalibrationProgramsettings = %lld\n",eCalibrationProgramsettings );
  printf("ePredictionProgramsettings  = %lld\n",ePredictionProgramsettings );
  printf("eVolumeProgramsettings      = %lld\n",eVolumeProgramsettings );
  printf("eHeaderOperation            = %lld\n",eHeaderOperation );
  printf("eCalibrationOperation       = %lld\n",eCalibrationOperation );
  printf("ePredictionOperation        = %lld\n",ePredictionOperation );
  printf("eVolumeOperation            = %lld\n",eVolumeOperation );
  printf("eIsBlackCS                  = %d\n", eIsBlackCS);
  if(eDB) {
    printf("=========== Database settings ================\n");
    eDB->Print();
  }
  printf("================================================\n\n");
}

//------------------------------------------------------------------------------------
int EdbScan2DB::InitDB( const char *conn, const char *user, const char *pwd )
{
  eDB = new TOracleServerE2WX( conn, user, pwd );
  if(!eDB) return 0;
  eDB->SetTransactionRW();
  return 1;
}

//------------------------------------------------------------------------------------
void EdbScan2DB::AddHeaderOperation()
{
  eHeaderOperation =  eDB->AddProcessOperation(
      eIdMachine,
  eHeaderProgramsettings,
  eIdRequester,
  0,                       // id_parent_operation
  eEventBrick,
  0,                       // id_plate
  2,                       // driverlevel
  0,                       // Id_plate_calibrations : Null on Header Operation
  eDB->Timestamp(),    // starttime
  eDB->Timestamp(),    // finishtime
  "'Y'",
  0                        // notes
                                              );
}

//------------------------------------------------------------------------------------
ULong64_t EdbScan2DB::LoadCalibrationZone( EdbScanProc &sproc, EdbID id )
{
  EdbPattern pred;
  sproc.ReadPred(pred,id,-1);
  EdbSegP *spred = pred.GetSegment(0);
  if(!spred) return 0;
  
  return eDB->MyQueryInsertReturning(
      Form("INSERT INTO OPERA.TB_ZONES (ID_EVENTBRICK, ID_PLATE, ID_PROCESSOPERATION, \
      MINX, MAXX, MINY, MAXY, RAWDATAPATH, STARTTIME, ENDTIME, SERIES,\
      TXX, TXY, TYX, TYY, TDX, TDY) VALUES \
      (%s, %s, %s, %.2f, %.2f, %.2f, %.2f, %s, %s, %s, %s, 1, 0, 0, 1, 0, 0)",
          eDB->Ostr(eEventBrick),
          eDB->Ostr(id.ePlate),
          eDB->Ostr(eCalibrationOperation),
          spred->X() - spred->SX(),
          spred->X() + spred->SX(),
          spred->Y() - spred->SY(),
          spred->Y() + spred->SY(),
          "'Local RawdataPath'",
          eDB->Timestamp(),
          eDB->Timestamp(),
          eDB->Ostr(id.eMajor)  // or minor?
          ), "ID");
}

//------------------------------------------------------------------------------------
ULong64_t EdbScan2DB::LoadZone( EdbSegP &s, int plate, ULong64_t operation, ULong64_t series, const char *cmt )
{
  return eDB->MyQueryInsertReturning(
      Form("INSERT INTO OPERA.TB_ZONES (ID_EVENTBRICK, ID_PLATE, ID_PROCESSOPERATION, \
      MINX, MAXX, MINY, MAXY, RAWDATAPATH, STARTTIME, ENDTIME, SERIES,\
      TXX, TXY, TYX, TYY, TDX, TDY) VALUES \
      (%s, %s, %s, %.2f, %.2f, %.2f, %.2f, %s, %s, %s, %s, 1, 0, 0, 1, 0, 0)",
  eDB->Ostr(eEventBrick),
  eDB->Ostr(plate),
  eDB->Ostr(operation),
  s.X() - s.SX(),
  s.X() + s.SX(),
  s.Y() - s.SY(),
  s.Y() + s.SY(),
  cmt,
  eDB->Timestamp(),
  eDB->Timestamp(),
  eDB->Ostr(series)
          ), "ID");
}

//------------------------------------------------------------------------------------
void EdbScan2DB::LoadPrediction( EdbScanProc &sproc, EdbID edbid )
{
  Log( 1,"EdbScan2DB::LoadPrediction","Load dataset: %s",edbid.AsString() );
  EdbScanSet *ss = sproc.ReadScanSet(edbid);

  EdbID *idstart = ss->GetID(0);
  TString str;
  sproc.MakeFileName(str,*idstart,"sbt.root");
  EdbRunTracking rtpath;
  rtpath.ePredictionScan=true;
  TTree *t = rtpath.InitSBtree(str.Data(),"READ");
  
  int nsbt = (int) t->GetEntries();
  for (int ipath=0;ipath<nsbt;ipath++) {                             // scanback paths of most downstream plate  (could be connected to CS
    rtpath.GetSBtreeEntry(ipath, *t);
    eDB->AddScanbackPath( eEventBrick, eHeaderOperation,
                          rtpath.ePred.ID(),idstart->ePlate,eIsBlackCS );
  }
  rtpath.CloseSBtree(t);
  
  
  for (int ip=0; ip<ss->eIDS.GetEntries(); ip++)
  {
    EdbID *id = ss->GetID(ip);
    EdbPlateP *plate = ss->GetPlate(id->ePlate);

    ULong64_t id_calibration_operation = 
        eDB->GetProcessOperationID (
        eEventBrick,
    eHeaderOperation,
    eCalibrationProgramsettings, 
    id->ePlate
                                   );

    ePredictionOperation =  eDB->AddProcessOperation(  // Adding prediction operation
        eIdMachine,
    ePredictionProgramsettings,
    eIdRequester,
    eHeaderOperation,       // id_parent_operation
    eEventBrick,
    plate->ID(),            // id_plate
    1,                      // driverlevel
    id_calibration_operation,  // calibration operation : NULL for Header Operation
    eDB->Timestamp(),       // starttime
    eDB->Timestamp(),       // finishtime
    "'Y'",
    "Prediction area"          // notes
                                                     );
    LoadSBData(sproc,*id);
  }
}

//------------------------------------------------------------------------------------
void EdbScan2DB::LoadSBData( EdbScanProc &sproc, EdbID id )
{
  //======== load scamback data for one plate ==============//
  
  TString rstr;
  sproc.MakeFileName( rstr, id, "sbt.root" );
  EdbRunTracking rtsb;    rtsb.ePredictionScan=true;
  TTree *tsbt = rtsb.InitSBtree(rstr.Data(),"READ");
  int nsbt = (int) tsbt->GetEntries();

  int id_basetrack=1;
  for (int ipath=0;ipath<nsbt;ipath++)
  {
    //=================== For each path ================//
    rtsb.GetSBtreeEntry( ipath, *tsbt);
    ULong64_t id_path = eDB->GetId_ScanbackPath( eEventBrick, eHeaderOperation, rtsb.ePred.ID() );
    if(!id_path) { 
      eDB->AddScanbackPath( eEventBrick, eHeaderOperation,rtsb.ePred.ID(),id.ePlate, 1 );
      id_path = eDB->GetId_ScanbackPath( eEventBrick, eHeaderOperation, rtsb.ePred.ID() );
    }
    //char *SERIES = id_path;
    // SERIES = 101,102 or 103 (intercalibration with 3 zones)
    // SERIES = 0              (intercalibration with a single zone)
    // SERIES = PATH_ID        (zone coming from a scan-back)
    // SERIES = ???            (zone coming from a total-scan)
    /*********** Adding one path-predicition zone ***********/
    ULong64_t id_pred_zone = LoadZone( rtsb.ePred, id.ePlate, ePredictionOperation, 
                                       id_path, "'Local RawdataPath'");
    
    //========= Adding views and their microtracks ==========//
    EdbRunTracking rt;
    rt.ePredictionScan=true;
    sproc.InitRunAccess( rt, rtsb.eIdf );
    eDB->AddViews(rt, eEventBrick, id_pred_zone, true);
    int nvpa = rt.GetNviewsPerArea();
    EdbRun  *run  = rt.GetRun();
        
    int id_view_up   = 0;
    int id_view_down = 0;
    int id_up        = 0;
    int id_down      = 0;
    if (rtsb.eStatus==0) // in case of basetrack found
    {
      id_view_down = (rtsb.eS1.Aid(0))*nvpa+rtsb.eS1.Aid(1);
      id_view_up   = (rtsb.eS2.Aid(0))*nvpa+rtsb.eS2.Aid(1);
      id_down = id_view_down*100000 + 10000*1 + rtsb.eS1.Vid(1)%100000;
      id_up   = id_view_up  *100000 + 10000*2 + rtsb.eS2.Vid(1)%100000;
      Log(2,"EdbScan2DB::LoadSBData","Add BT (with top mt and bottom mt)");
    }
    else if (rtsb.eStatus==1) // in case of microtrack bottom found
    {
      id_view_down = (rtsb.eS1.Aid(0))*nvpa+rtsb.eS1.Aid(1);
      id_view_up   = (rtsb.eS1.Aid(0))*nvpa+rtsb.eS1.Aid(1);
      id_down = id_view_down*100000 + 10000*1 + rtsb.eS1.Vid(1)%100000;
      id_up   = id_view_up  *100000 + 10000*2 + rtsb.eS1.Vid(1)%100000 + 20000;

      EdbView *view = run->GetEntry(rtsb.eS1.Vid(0));
      float dz = view->GetHeader()->GetZ2()-view->GetHeader()->GetZ3();

        // adding fake mt top
      eDB->AddMicroTrack( Form(
          "%s, %s ,%d, %d, %d, %2f, %2f, %2f, %2f, %2f, %2f, %2f",
      eDB->Ostr(eEventBrick),
      eDB->Ostr(id_pred_zone),
      2, id_up, id_view_up,
      rtsb.eS1.X() - dz*rtsb.eS1.TX(), rtsb.eS1.Y() - dz*rtsb.eS1.TY(),
      rtsb.eS1.TX(), rtsb.eS1.TY(), 
      0, 0, -1
                             ));
      Log(2,"EdbScan2DB::LoadSBData","Fake MT top added (and BT with MT bottom real and MT top fake)");
    }
    else if (rtsb.eStatus==2) // in case of microtrack top found
    {
      id_view_down = (rtsb.eS2.Aid(0))*nvpa+rtsb.eS2.Aid(1);
      id_view_up   = (rtsb.eS2.Aid(0))*nvpa+rtsb.eS2.Aid(1);
      id_down = id_view_down*100000 + 10000*1 + rtsb.eS2.Vid(1)%100000 + 20000;
      id_up   = id_view_up  *100000 + 10000*2 + rtsb.eS2.Vid(1)%100000;

      EdbView *view = run->GetEntry(rtsb.eS2.Vid(0));
      float dz = view->GetHeader()->GetZ2()-view->GetHeader()->GetZ3();

        // adding fake mt bottom
      eDB->AddMicroTrack( Form(
          "%s, %s ,%d, %d, %d, %2f, %2f, %2f, %2f, %2f, %2f, %2f",
      eDB->Ostr(eEventBrick),
      eDB->Ostr(id_pred_zone),
      1, id_down, id_view_down,
      rtsb.eS2.X() - dz*rtsb.eS2.TX(), 
      rtsb.eS2.Y() - dz*rtsb.eS2.TY(),
      rtsb.eS2.TX(), 
      rtsb.eS2.TY(),
      0, 0, -1
                              ));
      Log(2,"EdbScan2DB::LoadSBData","Fake MT top added (and BT with MT bottom real and MT top fake)");
    }
    if(rtsb.eStatus==0||rtsb.eStatus==1||rtsb.eStatus==2)
    {
      eDB->AddBaseTrack( Form(
          "%s, %s ,%d, %2f, %2f, %2f, %2f, %2f, %2f, %2f, %d, %d, %d, %d",
      eDB->Ostr(eEventBrick),
      eDB->Ostr(id_pred_zone),
      id_basetrack,
      rtsb.eNext.X(), rtsb.eNext.Y(),
      rtsb.eNext.TX(),rtsb.eNext.TY(),
      rtsb.eNext.W(), rtsb.eNext.Volume(), rtsb.eNext.Chi2(),
      1, id_down, 2, id_up
                             ));
      eDB->AddScanbackPrediction( Form(
          "%s, %s ,%d, %f, %f, %f, %f, NULL, NULL, NULL, NULL, NULL, %s, %d,'N', %d",
      eDB->Ostr(eEventBrick),
      eDB->Ostr(id_path),
      id.ePlate,
      rtsb.ePred.X(), 
      rtsb.ePred.Y(), 
      rtsb.ePred.TX(), 
      rtsb.ePred.TY(),
      eDB->Ostr(id_pred_zone),
      id_basetrack,
      0
                                      ));
      
      id_basetrack++;
    }
  }
  rtsb.CloseSBtree(tsbt);
}

//------------------------------------------------------------------------------------
void EdbScan2DB::LoadCalibration( EdbScanProc &sproc, EdbID edbid )
{
  Log( 2,"EdbScan2DB::LoadCalibration","Load dataset: %s",edbid.AsString() );
  if(!eDB)    return;
  
  EdbScanSet *ss = sproc.ReadScanSetGlobal(edbid, eX_marks);
  if(ss) 
  {
    int n = ss->eIDS.GetEntries();
    for(int i=0; i<n; i++) 
    {
      EdbID *id  = ss->GetID(i);
      EdbPlateP *plate = ss->GetPlate(id->ePlate);
      if(plate) 
      {
        eCalibrationOperation =  eDB->AddProcessOperation(  // Adding calibration operation
            eIdMachine,
        eCalibrationProgramsettings,
        eIdRequester,
        eHeaderOperation,    // id_parent_operation
        eEventBrick,
        id->ePlate,          // id_plate
        1,                   // driverlevel
        0,                   // calibration operation : NULL for Header Operation
        eDB->Timestamp(),    // starttime
        eDB->Timestamp(),    // finishtime
        "'Y'",
        "Calibration area"          // notes
                                                     );
        eDB->AddPlateCalibration(eEventBrick, eCalibrationOperation, plate);  // load Aff

        if(eWriteRawCalibrationData)  {
          ULong64_t id_calib_zone = LoadCalibrationZone( sproc, *id );
          //
          //========== Adding views and their microtracks ==========//
          EdbRunTracking rt;
          rt.ePredictionScan=true;
          sproc.InitRunAccess( rt, *id );
          eDB->AddViews(rt, eEventBrick, id_calib_zone, true);
          int nvpa = rt.GetNviewsPerArea();
          //
          //========= Adding basetracks assuming that microtracks already added ==========//
          EdbDataPiece dp;
          sproc.MakeFileName(dp.eFileNameCP,*id,"cp.root");
          dp.InitCouplesTree();
          eDB->AddBaseTracks(dp.eCouplesTree, eEventBrick, id_calib_zone, nvpa, true);
          dp.CloseCPData();
        }
      }
    }
  }
}

//------------------------------------------------------------------------------------
void EdbScan2DB::LoadVolume( EdbScanProc &sproc, EdbID edbid )
{
  Log( 2,"EdbScan2DB::LoadVolume","Load dataset: %s",edbid.AsString() );
  if(!eDB)    return;
  
  EdbScanSet *ss = sproc.ReadScanSet(edbid);
  if(ss) 
  {
    ULong64_t id_volume = eDB->AddVolume(eEventBrick, eHeaderOperation, edbid.eMajor );
    
    int n = ss->eIDS.GetEntries();
    for(int i=0; i<n; i++) {
      EdbID *id  = ss->GetID(i);
      EdbPlateP *plate = ss->GetPlate(id->ePlate);
    
      if(plate) {
 
        eVolumeOperation =  eDB->AddProcessOperation(  // Adding volume operation
            eIdMachine,
        eVolumeProgramsettings,
        eIdRequester,
        eHeaderOperation,    // id_parent_operation
        eEventBrick,
        id->ePlate,          // id_plate
        1,                   // driverlevel
        0,                   // calibration operation : NULL for Header Operation
        eDB->Timestamp(),    // starttime
        eDB->Timestamp(),    // finishtime
        "'Y'",
        "Volume area"          // notes
                                                    );
        EdbPattern pred;
        sproc.ReadPred( pred, *id, -1 );
        EdbSegP *spred = pred.GetSegment(0);
        ULong64_t id_volume_zone = LoadZone( *spred, id->ePlate, eVolumeOperation, id_volume, "'Local RawdataPath'");

        //============= Loading views and their microtracks ==========//
        EdbRunTracking rt;
        rt.ePredictionScan=true;
        sproc.InitRunAccess( rt, *id );
        eDB->AddViews(rt, eEventBrick, id_volume_zone, true);
        int nvpa = rt.GetNviewsPerArea();
          
        //========= Adding basetracks assuming that microtracks already added ==========//
        EdbDataPiece dp;
        sproc.MakeFileName(dp.eFileNameCP,*id,"cp.root");
        dp.InitCouplesTree();
        eDB->AddBaseTracks(dp.eCouplesTree, eEventBrick, id_volume_zone,nvpa, true);
        dp.CloseCPData();
        
        //=========== Adding volume slice ==========//
        eDB->MyQuery(Form("\
            INSERT INTO OPERA.TB_VOLUME_SLICES \
            (ID_EVENTBRICK, ID_VOLUME, ID_PLATE, MINX, MINY, MAXX, MAXY, ID_ZONE, DAMAGED) \
            VALUES (%lld, %s, %d, %.2f, %.2f, %.2f, %.2f, %s, %s)",
                eEventBrick,
                eDB->Ostr(id_volume),
                id->ePlate,
                spred->X() - spred->SX(),
                spred->X() + spred->SX(), 
                spred->Y() - spred->SY(),
                spred->Y() + spred->SY(), 
                eDB->Ostr(id_volume_zone),
                "'N'"                     //'N' - for "not damaged"
                    ));

      }
    }
  }
}

//------------------------------------------------------------------------------------
void EdbScan2DB::AddBrick( EdbScanProc &sproc )
{
  Log(2,"EdbScan2DB::AddBrick","%d",eEventBrick);
  if(eDB->IfEventBrick( eEventBrick, eID_SET.Data()))
  {
    Log(2,"EdbScan2DB::AddBrick","%d is already in DB, do nothing",eEventBrick);
    return;
  }

  //========== Loading X ray mark sets in global RS to get ZEROX,ZEROY ===========//
  EdbMarksSet msXG;
  sproc.ReadMarksSet(msXG,eBrick,"map.XG",'_','S');
  float ZEROX = msXG.eXmin;
  float ZEROY = msXG.eYmin;

  //========== Loading Optical ray mark sets in local (and corrected) RS to get brick dimension ==========//
  int X_MARKS = eX_marks;

  EdbMarksSet msOL;
  if (X_MARKS) sproc.ReadMarksSet(msOL,eBrick,"map.LL",'_','L');
  else sproc.ReadMarksSet(msOL,eBrick,"map.OL",'_','S');
  float MINX = msOL.eXmin + ZEROX;
  float MAXX = msOL.eXmax + ZEROX;
  float MINY = msOL.eYmin + ZEROY;
  float MAXY = msOL.eYmax + ZEROY;
  float MINZ = -72800.;
  float MAXZ = 0.;
  float ZEROZ = MAXZ;
  const char *BS_ID = eID_SET.Data();

  char databrick[500];
  sprintf(databrick,"%d, %12.2f, %12.2f, %12.2f, %12.2f, %12.2f, %12.2f, %s, %d, %12.2f, %12.2f, %12.2f",
          eEventBrick, MINX, MAXX, MINY, MAXY, MINZ, MAXZ, BS_ID, eBrick, ZEROX, ZEROY, ZEROZ);
  if( eDB->AddEventBrick(databrick) )
  {
    //========= Adding plates =========//
    TString filename=Form("%s/b%06d/b%06d.geometry",sproc.eProcDirClient.Data(),eBrick,eBrick);
    FILE *fp=0;
    if ((fp=fopen( filename.Data(), "r" ))==NULL) {
      Log(1,"AddBrick","ERROR! Cannot open file %s",filename); return;
    }
    while (!feof(fp))
    {
      int iplate;
      float z;
      if (fscanf(fp,"%d %f",&iplate,&z)!=2) continue;
      char dataplate[50];
      sprintf(dataplate,"%d, %12.2f",iplate, z);
      eDB->AddPlate( eEventBrick, dataplate);
    }
  }
  else eERROR++;
}