#ifndef ROOT_TOracleServerE2W
#define ROOT_TOracleServerE2W
 
#include "TOracleServerE2.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;

class TOracleServerE2W : public TOracleServerE2 {

public:
  TOracleServerE2W(const char *db, const char *uid, const char *pw):
    TOracleServerE2(db, uid, pw){}
  ~TOracleServerE2W(){}

   Int_t       AddBrick(char *id, char *minx, char *maxx, char *miny, char *maxy, char *minz, char *maxz);
   //   Int_t       AddPlate(char *id_eventbrick, char *id, char *z, char *mapxx, char *mapxy, char *mapyx, char *mapyy, char *mapdx, char *mapdy, char *isdamaged);
   Int_t       AddPlate(char *data);
   Int_t       UpdatePlate(char *id_eventbrick, char *id_plate, char *calibration);
   Int_t       AddZone(char *data);
   Int_t       AddZone(EdbPattern &pat, char *data1, char *data2);
   Int_t       AddBasetracks(EdbPattern &pat, char *id_eventbrick, char *id_zone);
   Int_t       AddProcessOperation(char *id_machine, char *id_programsettings, char *id_requester, char *id_parent_operation, 
				   char *id_eventbrick, char *id_plate, char *driverlevel, char *templatemarks, 
				   char *starttime, char *finishtime, char *success);

   ClassDef(TOracleServerE2W,1)  // Write enabled access to the OPERA db 
};

#endif
