#ifndef ROOT_EdbFrame
#define ROOT_EdbFrame
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbFrame                                                             //
//                                                                      //
// Scanning Frame: image+positin information                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
 
#ifndef ROOT_EdbImage
#include "EdbImage.h"
#endif

class TH2S;

//______________________________________________________________________________
class EdbFrame : public TObject {
 
private:

  Int_t       eFrameID;           // frame identifier
  Float_t     eZframe;            // Z-coordinate of the frame
  EdbImage    *eImage;            // CCD image

public:
  EdbFrame();
  EdbFrame(int frame, float z=0 );
  EdbFrame(int frame, int columns, int rows, char *image, float z=0 );
  virtual ~EdbFrame();

  void    SetID(int id)               {eFrameID=id;}
  void    SetZ(float z)               {eZframe=z;}
  void    SetImage(EdbImage *image)   {eImage=image;}

  int       GetID()     const { return eFrameID; }
  float     GetZ()      const { return eZframe;  }
  EdbImage *GetImage()  const { return eImage;   }

  void    Print( Option_t *opt="" ) const;
  char   *GetBuffer()         const { return eImage->GetBuffer(); }
  char    Pixel(int r, int c) const { return eImage->Pixel(r,c);  }
  TH2S   *GetHist2(int flip=0) const { return eImage->GetHist2(flip); }
  TH1S   *GetHist1() const { return eImage->GetHist1(); }

  ClassDef(EdbFrame,2)  // Scanning Frame: image+positin information
};

#endif /* ROOT_EdbFrame */
