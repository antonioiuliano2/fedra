#ifndef ROOT_EdbSegment
#define ROOT_EdbSegment
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbSegment                                                           //
//                                                                      //
// segment of the track                                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
 
#include "TArrayI.h"
#include "TObjArray.h"
 
class EdbAffine2D;

//______________________________________________________________________________
class EdbSeg3D : public TObject {
 
private:

  Float_t    eX0;         // |_coordinates of the segment's initial point
  Float_t    eY0;         // | in the SAME FOR ALL SEGMENTS stage coordinate system
  Float_t    eZ0;         // |
  Float_t    eTx;         // tanX: deltaX/deltaZ
  Float_t    eTy;         // tanY: deltaY/deltaZ

  Float_t    eDz;         // length of the segment along Z with sign

public:
  EdbSeg3D() { }
  EdbSeg3D( float x,  float y,  float z,  float tx,  float ty, float dz=0 ):
    eX0(x), eY0(y), eZ0(z), eTx(tx), eTy(ty), eDz(dz) {}
  virtual ~EdbSeg3D() { }

  virtual Float_t    GetX0() const { return eX0; }
  virtual Float_t    GetY0() const { return eY0; }
  virtual Float_t    GetZ0() const { return eZ0; }
  virtual Float_t    GetTx() const { return eTx; }
  virtual Float_t    GetTy() const { return eTy; }
  virtual Float_t    GetDz() const { return eDz; }

  virtual void    SetTx( float tx ) { eTx = tx; }
  virtual void    SetTy( float ty ) { eTy = ty; }
  virtual void    SetDz( float dz ) { eDz = dz; }

  virtual void       Set(float x,  float y,  float z,  float tx,  float ty, float dz=0);

  virtual void       Transform( const EdbAffine2D *aff );

  //  virtual void       Print( Option_t *opt=0) const;
  
  ClassDef(EdbSeg3D,1)  // base class with geometrical data for segments
};

//______________________________________________________________________________
class EdbSegment : public EdbSeg3D {

private:

  Int_t      eSide;       // side of the segment location (0-up, 1-down)
  Int_t      ePuls;       // puls height (number of grains)
  Int_t      eID;         // segment identifier
  Float_t    eSigmaX;     // dispersion parameter of grains around track line
  Float_t    eSigmaY;     // dispersion parameter of grains around track line

  TObjArray *eElements;   //! array of clusters (transient!)

public:
  EdbSegment();
  EdbSegment( float x,  float y,  float z,  float tx,  float ty, float dz=0, 
	      int side=0, int puls=0, int id=0 );
  virtual ~EdbSegment();

  void       Set(float x,  float y,  float z,  float tx,  float ty, 
		 float dz=0, int side=0, int puls=0, int id=0);

  void       SetSigma(float sx, float sy) { eSigmaX=sx; eSigmaY=sy; }
  float      GetSigmaX() const { return eSigmaX; }
  float      GetSigmaY() const { return eSigmaY; }

  Int_t      GetSide()   const { return eSide; }
  Int_t      GetPuls()   const { return ePuls; }
  Int_t      GetID()     const { return eID; }

  void       SetSide(int side)   { eSide = side; }
  void       SetPuls(int puls)   { ePuls = puls; }
  void       SetID(int id)       { eID   = id; }

  Int_t      GetNelements() const { if(eElements) return eElements->GetLast()+1;
                                    else          return 0; }

  TObjArray  *GetElements() const { return eElements;  }

  void       AddElement( TObject *element );

  void       Print( Option_t *opt=0) const;
  
  ClassDef(EdbSegment,2)  // segment of the track
};

//______________________________________________________________________________
class EdbTrack : public EdbSeg3D {

 private:

  Int_t       eID;         // Track identifier

  TObjArray  *eElements;   //! array of segments (transient!)

 public:
  EdbTrack();
  EdbTrack( float x,  float y,  float z,  float tx,  float ty, float dz=0, int id=0 );
  virtual ~EdbTrack();

  void       Set(float x,  float y,  float z,  float tx,  float ty, 
		 float dz, int id=0);

  Int_t      GetID()   const { return eID; }

  Int_t      GetNelements() const { if(eElements) return eElements->GetLast()+1;
                                     else return 0; }
  TObjArray  *GetElements() const { return eElements;  }

  void       AddElement( TObject *element );

  void       Print( Option_t *opt=0) const;
  
  ClassDef(EdbTrack,2)  // Track linked from segments
};

#endif /* ROOT_EdbSegment */
