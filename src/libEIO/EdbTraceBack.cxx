//-- Author :  Valeri Tioukov   22.06.2005

//////////////////////////////////////////////////////////////////////////
// 
// EdbTraceBack
//
// define address&counters logic here
//
// for raw segments (in the run tree) the address is defined by:
// ~~~~~~~~~~~~~~~~
// viewEntry   - entry of the view in the run tree
// inViewEntry - index of the segment in the view segments array
//
// the segment also belong to the area (fragment): view.eAreaID
// and to the view in the current area:            view.eViewID
// in the view segment has the identifier          segraw.eID
// segraw.eID normally is equal to inViewEntry
//
// For the segments in the couples tree:
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// s1,s2 - are the transformed copies of raw segments (microtracks) 
// For them:
// s1.eVid[0] == viewEntry
// s1.eVid[1] == inViewEntry
// s1.eAid[0] == view.eAreaID
// s1.eAid[1] == view.eViewID
// s1/s2 eVid - are enough to find the correspondent raw segment 
//              in the raw run tree
//
// s - is the basetrack in the couples tree
//
// s.eVid[0] - contains the information about plate and piece, to extract 
//             it one should use the correspondent functions of EdbTraceBack
// s.eVid[1] - is the entry number of the basetrack in the couples tree
//
// in the linked_tracks tree s.eVid are enough to find the basetrack 
// in the couples tree and then eventually to find the raw segments
//
// in the linked_tracks tree
// t.eVid[1] - is the entry number of the current track in the tracks tree
//
//////////////////////////////////////////////////////////////////////////
#include "EdbTraceBack.h"

ClassImp(EdbTraceBack)

//-----------------------------------------------------------------------
void  EdbTraceBack::SetBaseTrackVid( EdbSegP &s, int plate, int piece, int entry ) 
{
  s.SetVid( plate*1000+piece, entry); 
}

//-----------------------------------------------------------------------
void EdbTraceBack::FromBaseTrackVid( EdbSegP &s, int &plate, int &piece, int &entry )
{ 
  entry = s.Vid(1); 
  plate = s.Vid(0)/1000;
  piece = s.Vid(0)%1000;
}
