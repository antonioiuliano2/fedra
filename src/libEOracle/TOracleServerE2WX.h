#ifndef ROOT_TOracleServerE2WX
#define ROOT_TOracleServerE2WX
 
#include "TOracleServerE2.h"
#include "EdbView.h"
#include "EdbBrick.h"
#include "EdbScanProc.h"
#include "EdbRunTracking.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;

class TOracleServerE2WX : public TOracleServerE2 {

  private:
    Int_t      eNTestLoad;
    Bool_t     eTestLoad;   // if test load do not perform intermediate transactions
    Bool_t     eDoCommit;   // true - commit current transaction (default is false)
  public:
    Int_t      eNviewsPerArea;
    TString    eLab;       //! as "NAPOLI"
    TString    eLa;        //! as "NA"

  public:
  TOracleServerE2WX(const char *db, const char *uid, const char *pw):
    TOracleServerE2(db, uid, pw){eNviewsPerArea=0;}
    ~TOracleServerE2WX(){}

    void       SetTestLoad(int n) {eTestLoad=true; eDoCommit=0; eNTestLoad=n;}
    void       SetCommit(bool i) {eTestLoad=false; eDoCommit=i; eNTestLoad=kMaxInt;}
    Bool_t      TestLoad() {return eTestLoad;}
    Int_t       AddBrick_Set(char *id, char *idrange_min, char *idrange_max, char *id_partition);
    Int_t       AddBrick_Space(char *id_brick, char *id_set);
    Int_t       AddMicroTrack(char *datamicro);
    Int_t       AddScanbackPrediction(char *dataprediciton);
    Int_t       AddTemplateMarkSets(char *datamarks);
 
    Int_t       AddBSBpathsVolumes(char *databsbpathsvolumes);
    
    Int_t       DeleteBrick(char *id_eventbrick);
    Int_t       DeleteBrickSpace(char *id_brick);
    Int_t       DeleteOperation(char *id_brick, char *id_process_operation);
    Int_t       DeletePlateOperation(char *id_brick, char *id_process_operation, char *id_plate);

    Int_t       NviewsPerArea() {return eNviewsPerArea;};
    
    //------ common service functions (to be moved into parent class) -------------
    const char *Timestamp();
    
    Int_t     MyQuery(const char *sqlquery);
    ULong64_t MyQueryInsertReturning( const char *query, const char *var );
    Int_t     SetTransactionRW() {return MyQuery("SET TRANSACTION READ WRITE");}
    void      FinishTransaction();
    Int_t     ROLLBACK() {return MyQuery("ROLLBACK");}
    //------------------------------
    void Set0();
    void Print();

    ULong64_t   AddVolume( ULong64_t id_eventbrick,ULong64_t id_process_operation, int ivolume );
    Int_t       AddView(EdbView *view, int id_view, ULong64_t id_eventbrick, ULong64_t id_zone, bool usebuffer=true);
    Int_t       AddViews(EdbRunTracking &rt, ULong64_t id_eventbrick, ULong64_t id_zone, bool usebuffer=true);
    Int_t       AddPlate(ULong64_t id_eventbrick, const char *dataplate);
    Int_t       AddEventBrick(const char *str);
    ULong64_t   IfEventBrick( ULong64_t id_eventbrick, const char *id_set );
    
    Int_t       AddBaseTracks(TTree     *tree, ULong64_t id_eventbrick, ULong64_t id_zone, Int_t nvpa, bool usebuffer);
    Int_t       AddBaseTracks(EdbPattern &pat, ULong64_t id_eventbrick, ULong64_t id_zone);
    Int_t       AddPlateCalibration( ULong64_t id_eventbrick, ULong64_t id_process_operation, EdbPlateP *plate);
//    void       AddZone(const char *data);

    
    Int_t  AddBaseTrack(const char *data);
    
    ULong64_t  AddProcessOperation(
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
    const char *notes       );
    
    Int_t  AddScanbackPath( ULong64_t id_eventbrick, ULong64_t id_header_operation, int id_path, int id_start_plate, int skipCSconnection);

    ClassDef(TOracleServerE2WX,1)  // Write enabled access to the OPERA db 
};

//------------------------------------------------------------------------------------
class EdbScan2DB : public TObject {
  
  public:

    TOracleServerE2WX *eDB;
    
    ULong64_t eBrick;
    ULong64_t eEventBrick;
    ULong64_t eEvent;
    Int_t     eIsBlackCS;
    Int_t     eIDPATH;                    // -1 - do not connect path and volume (table TB_B_SBPATHS_VOLUMES)
    TString   eID_SET;                     // as "'OPERA NA SET  01'"
    ULong64_t eIdMachine;                  // as 6000000000010002
    ULong64_t eIdRequester;                // as 6000000000100375
    ULong64_t eHeaderProgramsettings;      // as 6000000000700006
    ULong64_t eCalibrationProgramsettings; // as 6000000000700004
    ULong64_t ePredictionProgramsettings;  // as 6000000000700005
    ULong64_t eVolumeProgramsettings;      // as 6000000000100374
    ULong64_t eFeedbackProgramsettings;    // as 6000000001371016
    ULong64_t eCalibrationHeaderOperation; //
    ULong64_t ePredictionHeaderOperation;  //
    ULong64_t eVolumeHeaderOperation;      //

    Int_t     eX_marks;  // 2=in case lateral X-rays marks, followed by a calibration scan;
                         // 1=in case lateral X-rays marks only;
                         // 0=calibration scan only
    Bool_t   eWriteRawCalibrationData;
    
//    Int_t    eNviewsPerArea;
   
    Int_t     eERROR;   // 0 by default, 1 if error happened
  
  public:
    EdbScan2DB();
    ~EdbScan2DB(){}
    
    void Print();
    int       InitDB(const char *conn, const char *user, const char *pwd);
    void      LoadCalibration( EdbScanProc &sproc, EdbID edbid );
    void      LoadPrediction( EdbScanProc &sproc, EdbID edbid );
    void      LoadVolume( EdbScanProc &sproc, EdbID edbid, EdbID idstop );
    ULong64_t LoadCalibrationZone( EdbScanProc &sproc, EdbID edbid, ULong64_t operation);
    void      LoadSBData( EdbScanProc &sproc, EdbID id, ULong64_t operation);
    ULong64_t LoadZone( EdbSegP &s, int plate, ULong64_t operation, ULong64_t series, const char *cmt );
    void      AddBrick( EdbScanProc &sproc );
    ULong64_t AddHeaderOperation(const char *note);
    void      DumpListOfHeaderOperations();
    ClassDef(EdbScan2DB,1)  // scan to oracle db loading class
};

#endif
