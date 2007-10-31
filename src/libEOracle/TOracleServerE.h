#ifndef ROOT_TOracleServerE
#define ROOT_TOracleServerE
 
#include "TOracleServer.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;

class TOracleServerE : public TOracleServer {

public:
  TOracleServerE(const char *db, const char *uid, const char *pw):
    TOracleServer(db, uid, pw){}
  ~TOracleServerE(){}

   Int_t       QueryTree(char *query, TTree *tree, char *leafs=0);

   Int_t       ReadVolume(char *id_volume, EdbPatternsVolume &vol);
   Int_t       ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol);
   Int_t       ReadDataSet(ULong64_t id_parent_op, int id_brick, EdbPatternsVolume &vol);
   bool        ReadZplate(int id_plate, int id_eventbrick, EdbPattern &pat);
   Int_t       ReadBasetracksPattern(char *selection, EdbPattern &pat);
   Int_t       ReadMicrotracksPattern(char *selection, EdbPattern &pat);
   Int_t       ReadCalibration(int id_brick, EdbPatternsVolume &vol);

   Int_t       ReadVolume_NI(char *id_volume, EdbPatternsVolume &vol);
   Int_t       ReadVolume_NI(char *id_volume, EdbPatternsVolume &vol, Int_t min, Int_t max);
   Int_t       ReadVolume_NI(ULong64_t id_volume, EdbPatternsVolume &vol);
   Int_t       ReadVolume_NI(ULong64_t id_volume, EdbPatternsVolume &vol, Int_t min_pl, Int_t max_pl);
   bool        ReadZplate_NI(int id_plate, int id_eventbrick, EdbPattern &pat);

   Int_t       GetProcessOperationID(char *id_eventbrick, char *id);
   Int_t       GetProcessOperationID(char *id_programsettings, char *id_eventbrick, char *id_plate, char *id);

   Int_t       AddBrick(char *id, char *minx, char *maxx, char *miny, char *maxy, char *minz, char *maxz);
   //   Int_t       AddPlate(char *id_eventbrick, char *id, char *z, char *mapxx, char *mapxy, char *mapyx, char *mapyy, char *mapdx, char *mapdy, char *isdamaged);
   Int_t       AddPlate(char *data);
   Int_t       UpdatePlate(char *id_eventbrick, char *id_plate, char *calibration);
   Int_t       AddZone(char *data);
   Int_t       AddZone(EdbPattern &pat, char *data1, char *data2);
   Int_t       AddBasetracks(EdbPattern &pat, char *id_eventbrick, char *id_zone);
   Int_t       AddProcessOperation(char *id_machine, char *id_programsettings, char *id_requester, char *id_parent_operation, char *id_eventbrick, char *id_plate, char *driverlevel, char *templatemarks, char *starttime, char *finishtime, char *success);

   ClassDef(TOracleServerE,1)  // Connection to Oracle server
};

#endif
