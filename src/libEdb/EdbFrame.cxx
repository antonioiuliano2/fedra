//-- Author :  Valeri Tioukov   7.04.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbFrame                                                             //
//                                                                      //
// Implementation of Frame class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_EdbFrame
#include "EdbFrame.h"
#endif

#ifndef ROOT_TH2
#include "TH2.h"
#endif

ClassImp(EdbFrame)

//______________________________________________________________________________
EdbFrame::EdbFrame()
{
  eFrameID  = 0;
  eImage    = 0;
  eZframe   = 0;
}

//______________________________________________________________________________
EdbFrame::~EdbFrame()
{
  if(eImage) delete eImage;
}

//______________________________________________________________________________
EdbFrame::EdbFrame( int frame, int columns, int rows, char *image, float z )
{
  eFrameID = frame;
  eZframe  = z;
  eImage   = new EdbImage(columns,rows,image);
}

//______________________________________________________________________________
EdbFrame::EdbFrame( int frame, float z )
{
  eFrameID  = frame;
  eZframe   = z;
  eImage    = 0;
}

//______________________________________________________________________________
void EdbFrame::Print( Option_t *opt ) const
{
  printf("Frame: %d at z = %f\n", eFrameID,eZframe);
  if(eImage) eImage->Print();
}
