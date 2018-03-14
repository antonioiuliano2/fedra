//-- Author :  Valeri Tioukov   7.04.2000
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbFrame                                                             //
//                                                                      //
// Implementation of Frame class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TBuffer.h>
#include <TH2.h>
#include <TClass.h>
#include "EdbFrame.h"

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
void EdbFrame::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbFrame.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 3) {
	EdbFrame::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TObject::Streamer(R__b);
      R__b >> eFrameID;
      R__b >> eZframe;
      R__b >> eNcl;
      R__b >> eNpix;
      R__b >> eImage;
      R__b.CheckByteCount(R__s, R__c, EdbFrame::IsA());
      //====end of old versions
   } else {
     EdbFrame::Class()->WriteBuffer(R__b,this);
   }
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
