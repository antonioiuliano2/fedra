#ifndef ROOT_EdbEmrIO
#define ROOT_EdbEmrIO
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbEmrIO                                                             //
//                                                                      //
// interface between FEDRA and EmuRec objects                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class EdbTrackP;
class EdbVertex;

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

  ClassDef(EdbEmrIO,1)  // interface betwwen FEDRA and EmuRec objects
};

#endif /* ROOT_EdbEmrIO */
