//-- Author :  Valeri Tioukov, Michele Pozzato 3/06/2008

//////////////////////////////////////////////////////////////////////////
//                                                                      //
//  Interface from Edb to EmuRec objects                                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "EdbVertex.h"
#include "EdbEmrIO.h"

ClassImp(EdbEmrIO);

//________________________________________________________________________________________
void EdbEmrIO::Print()
{
  printf("EdbEmrIO ok\n");
}

//________________________________________________________________________________________
void EdbEmrIO::TrackEdb2Emr(EdbTrackP &tedb)
{
  // Input:  EdbTrackP tedb
  // Output: EmrTrack  temr
  tedb.Print();
}

//________________________________________________________________________________________
void EdbEmrIO::TrackEmr2Edb(EdbTrackP &tedb)
{
  // Input:  EmrTrack  temr
  // Output: EdbTrackP tedb
  tedb.Print();
}

//________________________________________________________________________________________
void EdbEmrIO::VertexEdb2Emr(EdbVertex &vedb)
{
  // Input:  EdbVertex vedb
  // Output: EmrVertex vemr
  vedb.Print();
}

//________________________________________________________________________________________
void EdbEmrIO::VertexEmr2Edb(EdbVertex &vedb)
{
  // Input:  EmrVertex vemr
  // Output: EdbVertex vedb
  vedb.Print();
}
