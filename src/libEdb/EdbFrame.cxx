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
  Set0();
}

//______________________________________________________________________________
EdbFrame::EdbFrame( int frame, int columns, int rows, char *image, float z )
{
  Set0();
  eFrameID = frame;
  eZframe  = z;
  eImage   = new EdbImage(columns,rows,image);
}

//______________________________________________________________________________
EdbFrame::EdbFrame( int frame, float z, int ncl, int npix )
{
  Set0();
  eFrameID  = frame;
  eZframe   = z;
  eNcl      = ncl;
  eNpix     = npix;
}

//______________________________________________________________________________
EdbFrame::~EdbFrame()
{
  if(eImage) delete eImage;
}

//______________________________________________________________________________
void EdbFrame::Set0()
{
  eFrameID  = 0;
  eZframe   = 0;
  eNcl      = 0;
  eNpix     = 0;
  eImage    = 0;
}

//______________________________________________________________________________
void EdbFrame::Print( Option_t *opt ) const
{
  printf("Frame: %d at z = %f\n", eFrameID,eZframe);
  if(eImage) eImage->Print();
}
