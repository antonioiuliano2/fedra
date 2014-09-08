#ifndef ROOT_TOracleServerE2
#define ROOT_TOracleServerE2
 
#include "TOracleServer.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;
class EdbRun;
class EdbTrackP;
class EdbMarksSet;

class TOracleServerE2 : public TOracleServer {

public:
  TString eRTS; // "Remote Tables Suffix" for example "@opita"
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

   void        PrintBrickInfo(Long_t id_eventbrick, int level );
   void        PrintBrickInfoFull(Long_t id_eventbrick, int level );

   Int_t       GetProcessOperationID(char *id_eventbrick, char *id_programsettings, char *id);
   Int_t       GetProcessOperationID(char *id_eventbrick, char *id_parent_operation, char *id_programsettings, char *id_plate, char *id);
   Int_t       DumpEventsID(char *id_eventbrick);
   Int_t       DumpBrickVolumesID(char *id_eventbrick);
   Int_t       DumpProcessOperations(char *id_eventbrick,char *id_programsettings);
   Int_t       DumpProcessOperations(char *id_eventbrick, Int_t driverlevel);
   Int_t       GetId_EventBrick (const char *id_brick, const char*id_set, char *id);
   Int_t       GetId_Zone(char *id_eventbrick,char *id_plate, char *id_process_operation, char *series, char* id);
   Int_t       GetId_ScanbackPath(char *id_eventbrick, char *id_process_operation, int path, char *id);
   Int_t       GetId_Volume(char *id_eventbrick, char *id_process_operation, int ivolume, char *id);
   Int_t       GetProcessType(char *IDPROCESS);

   Int_t       ReadScanbackPath(Int_t id_eventbrick, Int_t path, EdbTrackP &t);
   Int_t       ReadDataSet(ULong64_t id_parent_op, int id_brick, ULong64_t path, EdbPatternsVolume &vol);
   Int_t       ReadViewsZone(ULong64_t id_zone, int side, TObjArray &edbviews);
   Int_t       ReadMicrotracksZone(Int_t id_eventbrick, ULong64_t id_zone, int side, TObjArray &edbviews);
   Int_t       ConvertMicrotracksZoneToEdb(Int_t id_eventbrick, ULong64_t id_zone, EdbRun &run);
   
   Int_t       ConvertMicrotracksDataSetToEdb(const char *query, const char *outdir, int major=0, int minor=0, bool structure_only=false );
   Int_t       ConvertMicrotracksVolumeToEdb(ULong64_t id_volume, const char *outdir, int major=0, int minor=0, bool structure_only=false );
   Int_t       ConvertMicrotracksProcessToEdb(ULong64_t processoperation, const char *outdir, int major=0, int minor=0, bool structure_only=false );
   Int_t       ConvertMicrotracksParentProcessToEdb(ULong64_t parentprocessoperation, const char *outdir, int major=0, int minor=0, bool structure_only=false );
   
   Int_t       ConvertScanbackPathToEdb(Int_t id_eventbrick, Int_t path, const char *outdir, int major, int minor);

   Int_t       ReadTemplateMarks(Int_t id_brick, EdbMarksSet &ms);
   Int_t       ReadBrickOffset(Int_t id_brick, EdbMarksSet &ms);
   Int_t       ReadCSPredictions(Int_t id_brick, EdbPattern &pred);
   Int_t       ReadCSPredictions2(Int_t id_brick, EdbPattern &pred);
   Int_t       ReadCSPredictions_remote_v2(Int_t id_brick, EdbPattern &pred, int csid);
   Int_t       ReadVetoTracks(Int_t id_brick, EdbPattern &pred);

   ClassDef(TOracleServerE2,1)  // read-only access to the OPERA scanning db (2-d version of the db)
};

#endif
