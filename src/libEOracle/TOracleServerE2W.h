#ifndef ROOT_TOracleServerE2W
#define ROOT_TOracleServerE2W
 
#include "TOracleServerE2.h"
#include "EdbView.h"
#include "EdbBrick.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;

class TOracleServerE2W : public TOracleServerE2 {

public:
  TOracleServerE2W(const char *db, const char *uid, const char *pw):
    TOracleServerE2(db, uid, pw){}
    ~TOracleServerE2W(){}

    Int_t       AddEventBricks(char *databrick);
    Int_t       AddBrick_Set(char *id, char *idrange_min, char *idrange_max, char *id_partition);
    Int_t       AddBrick_Space(char *id_brick, char *id_set);
    Int_t       AddPlate(char *id_eventbrick, char *dataplate);
    Int_t       AddPlateCalibration(char *id_eventbrick, char *id_process_operation, char *datacalibration);
    Int_t       AddZone(char *data);
    Int_t       AddProcessOperation(char *id_machine, char *id_programsettings, char *id_requester, char *id_parent_operation, 
				    char *id_eventbrick, char *id_plate, char *driverlevel, char *templatemarks, 
				    char *starttime, char *finishtime, char *success);
    Int_t       AddView(char *dataView);
    Int_t       AddBasetracks(EdbPattern &pat, char *id_eventbrick, char *id_zone);                     
    Int_t       AddMicroTrack(char *datamicro);
    Int_t       AddBaseTrack(char *database);
    Int_t       AddScanbackPath(char *datapath);
    Int_t       AddScanbackPrediction(char *dataprediciton);
    Int_t       AddTemplateMarkSets(char *datamarks);
    
    Int_t       AddPlateCalibration(char *id_eventbrick, char *id_process_operation, EdbPlateP *plate);
    Int_t       AddView(EdbView *view, int id_view, char *id_eventbrick, char *id_calibzone);
    Int_t       AddBasetracks(TTree *tree, char *id_eventbrick, char *id_zone);
    
    Int_t       DeleteBrick(char *id_eventbrick);
    Int_t       DeleteBrickSpace(char *id_brick);
    Int_t       DeleteOperation(char *id_brick, char *id_process_operation);

    ClassDef(TOracleServerE2W,1)  // Write enabled access to the OPERA db 
};

#endif
