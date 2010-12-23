#ifndef ROOT_EdbEmrIO
#define ROOT_EdbEmrIO
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbEmrIO                                                             //
//                                                                      //
// interface between FEDRA and EmuRec objects                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TArrayI.h"
#include "TObjArray.h"
#include "EdbPattern.h"

class EdbTrackP;
class EdbVertex;
class REMULZone;

//______________________________________________________________________________
class EdbEmrIO : public TObject {
 
 private:

 public:
  EdbEmrIO() {}
  virtual ~EdbEmrIO() {}

  void TrackEdb2Emr(EdbTrackP &tedb);  // (EdbTrackP &tedb, EmrTrack  &temr)
  void TrackEmr2Edb(EdbTrackP &tedb);  // (EmrTrack  &temr, EdbTrackP &tedb)
  void VertexEdb2Emr(EdbVertex &vedb); // (EdbVeretx &vedb, EmrVeretx &vemr)
  void VertexEmr2Edb(EdbVertex &vedb); // (EmrVeretx &vemr, EdbVeretx &vedb)
    
  void    Print();

  ClassDef(EdbEmrIO,1)  // interface between FEDRA and EmuRec objects
};

//______________________________________________________________________________
class EdbEmrFileAccess : public TObject {
 
 private:
  TObjArray eSides; // each entry is EdbPattern with the views of one side 

 public:
  EdbEmrFileAccess() {}
  virtual ~EdbEmrFileAccess() {}

  int       PlateID(int plateside) { return plateside/3; }
  int       SideID( int plateside) { return plateside%3; }
  EdbSegP  *AddEntry(int plate, int side, int zone, int view, int entry, float x, float y, float z);
  int       GetPlateEntries(int plate, int side, TArrayI &entries);
  int       FirstPlate();
  int       LastPlate();
  void      PrintStat();

  int       SideID(int plate, int side) {
    if(plate<0||plate>1000) return -1;
    if(side<0||side>2)      return -1; 
    return plate*3 + side; 
  }

  ClassDef(EdbEmrFileAccess,1)  // to manage better plane emurec file structure
};

#endif /* ROOT_EdbEmrIO */
