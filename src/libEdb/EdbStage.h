#ifndef ROOT_EdbStage
#define ROOT_EdbStage

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbStage                                                             //
//                                                                      //
// Stage and microscop parameters                                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef ROOT_TNamed
#include "TNamed.h"
#endif

#ifndef ROOT_EdbAffine
#include "EdbAffine.h"
#endif

//______________________________________________________________________________
class EdbCCD : public TNamed {

private:
  Int_t      eNx;      // number of pixels along X (number of columns)
  Int_t      eNy;      // number of pixels along Y (number of rows)
  Float_t    ePixX;    // physical pixel size in microns along X
  Float_t    ePixY;    // physical pixel size in microns along Y

public:
  EdbCCD( float px=0, float py=0, int nx=0, int ny=0 ): TNamed(),
    eNx(nx), eNy(ny), ePixX(px), ePixY(py) {}
  virtual ~EdbCCD() {}

  void Set(int nx=0, int ny=0, float px=0, float py=0)
    { eNx=nx; eNy=ny; ePixX=px; ePixY=py; }

  Int_t     GetNx()    const { return eNx;    }
  Int_t     GetNy()    const { return eNy;    }
  Float_t   GetPixX()  const { return ePixX;  }
  Float_t   GetPixY()  const { return ePixY;  }

  int  ReadParameters( const char *str );

  void Print(Option_t *opt="") const;

  ClassDef(EdbCCD,1)  // CCD camera specification
};

//______________________________________________________________________________
class EdbObjective : public TNamed {

 private:
  Float_t        eMagnification;

  EdbAffine2D    *eCCD2Stage;           // from CCD pixels to stage microns

 public:
  EdbObjective( float mag=0 );
  virtual ~EdbObjective();

  void Set( float mag, 
	    float a, float b, float c, float d, float p, float q )
    { eMagnification=mag; 
      eCCD2Stage->Set(a,b,c,d,p,q); }

  void    SetMagnification(float mag) { eMagnification=mag; }

  void    SetCCD2Stage( float a, float b, float c, float d, float p, float q ) 
    { eCCD2Stage->Set(a,b,c,d,p,q); }

  Float_t       GetMagnification() const { return eMagnification; }
  EdbAffine2D  *GetCCD2Stage()     const { return eCCD2Stage; }

  Float_t       PixelX()   const { return TMath::Abs(eCCD2Stage->A11()); }
  Float_t       PixelY()   const { return TMath::Abs(eCCD2Stage->A22()); }

  int  ReadParameters( const char *str );
  void Print(Option_t *opt="") const;

  ClassDef(EdbObjective,1)  // objective affine parameters
};


//______________________________________________________________________________
class EdbStage : public TNamed {

 private:

  EdbCCD         *eCCD;              // CDD matrix physical parameters
  EdbObjective   *eObjective;        // Objective parameters

  EdbAffine3D    *eEncoders2Stage;   // XYZ encoders to Stage affine transformation parameters

 public:
  EdbStage();
  virtual ~EdbStage();

  EdbAffine2D  *CCD2Stage()      const;  // create new object with copied affine parameters
  EdbAffine3D  *Encoders2Stage() const;  // create new object with copied affine parameters

  Float_t       PixelX()   const { return eObjective->PixelX(); }
  Float_t       PixelY()   const { return eObjective->PixelY(); }

  EdbCCD       *GetCCD()              const { return eCCD; }
  EdbObjective *GetObjective()        const { return eObjective; }
  EdbAffine3D  *GetEncoders2Stage()   const { return eEncoders2Stage; }

  void Print(Option_t *opt="") const;

  ClassDef(EdbStage,1)  // stage parameters
};

#endif /* ROOT_EdbStage */
