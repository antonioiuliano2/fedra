#ifndef ROOT_TOracleServerE2
#define ROOT_TOracleServerE2
 
#include "TOracleServer.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;
class EdbRun;

class TOracleServerE2 : public TOracleServer {

public:
  TOracleServerE2(const char *db, const char *uid, const char *pw):
    TOracleServer(db, uid, pw){}
  ~TOracleServerE2(){}

   bool        ReadZplate(int id_plate, int id_eventbrick, EdbPattern &pat);
   bool        ReadZplate_nominal(int id_plate, int id_eventbrick, EdbPattern &pat);
   Int_t       ReadBasetracksPattern(char *selection, EdbPattern &pat);
   Int_t       ReadMicrotracksPattern(int id_eventbrick, char *selection, EdbPattern &pat);

   Int_t       ReadVolume(char *id_volume, EdbPatternsVolume &vol);
   Int_t       ReadVolume(char *id_volume, EdbPatternsVolume &vol, Int_t min, Int_t max);
   Int_t       ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol);
   Int_t       ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol, Int_t min_pl, Int_t max_pl);

   Int_t       GetProcessOperationID(char *id_eventbrick, char *id);
   Int_t       GetProcessOperationID(char *id_programsettings, char *id_eventbrick, char *id_plate, char *id);

   Int_t       ReadDataSet(ULong64_t id_parent_op, int id_brick, ULong64_t path, EdbPatternsVolume &vol);
   Int_t       GetProcessType(char *IDPROCESS);
   Int_t       ReadViewsZone(ULong64_t id_zone, int side, TObjArray &edbviews);
   Int_t       ReadMicrotracksZone(Int_t id_eventbrick, ULong64_t id_zone, int side, TObjArray &edbviews);
   Int_t       ConvertMicrotracksZoneToEdb(Int_t id_eventbrick, ULong64_t id_zone, EdbRun &run);

   ClassDef(TOracleServerE2,1)  // read-only access to the OPERA scanning db (2-d version of the db)
};

#endif
