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

class TH2F;
class TH1F;

//______________________________________________________________________________
class EdbFrame : public TObject {
 
private:

  Int_t       eFrameID;           // frame identifier
  Float_t     eZframe;            // Z-coordinate of the frame
  Int_t       eNcl;               // total number of clusters found in the frame
  Int_t       eNpix;              // total number of nonzero pixels found in the frame
  EdbImage    *eImage;            // CCD image

public:
  EdbFrame();
  EdbFrame(int frame, float z=0, int ncl=0, int npix=0 );
  EdbFrame(int frame, int columns, int rows, char *image, float z=0 );
  virtual ~EdbFrame();

  void    Set0();
  void    SetID(int id)               {eFrameID=id;}
  void    SetZ(float z)               {eZframe=z;}
  void    SetImage(EdbImage *image)   {eImage=image;}

  int       GetID()     const { return eFrameID; }
  float     GetZ()      const { return eZframe;  }
  int       GetNcl()    const { return eNcl;  }
  int       GetNpix()   const { return eNpix;  }
  EdbImage *GetImage()  const { return eImage;   }

  void    Print( Option_t *opt="" ) const;
  char   *GetBuffer()         const { return eImage->GetBuffer(); }
  char    Pixel(int r, int c) const { return eImage->Pixel(r,c);  }
  TH2F   *GetHist2(int flip=0) const { return eImage->GetHist2(flip); }
  TH1F   *GetHist1() const { return eImage->GetHist1(); }

  ClassDef(EdbFrame,4)  // Scanning Frame: image+positin information
};

#endif /* ROOT_EdbFrame */
