#ifndef ROOT_EdbCluster
#define ROOT_EdbCluster

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCluster                                                           //
//                                                                      //
// cluster (spot of pixels)                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TClonesArray.h"

#ifndef ROOT_EdbVirtual
#include "EdbVirtual.h"
#endif

//______________________________________________________________________________
class EdbCluster : public TObject, public EdbPoint3D {

private:

  Float_t    eX;       // cluster coordinates in pixels(?)
  Float_t    eY;       //
  Float_t    eZ;       //
  Float_t    eArea;    // Cluster area: number of pixels
  Float_t    eVolume;  // Cluster volume - in case of greyscale pixels
  Int_t      eFrame;   // frame index
  Int_t      eSide;    // emulsion side index
  Int_t      eSegment; // segment id to be attached (-1 if no segment)

public:
   EdbCluster() { }
   EdbCluster( float x,  float y,  float z,  
	       float a,  float v, int f, int s, int seg=-1 ) :
    eX(x), eY(y), eZ(z), 
     eArea(a),eVolume(v), eFrame(f), eSide(s), eSegment(seg) {}
   virtual ~EdbCluster() { }

   void Print( Option_t *opt=0 ) const;

   Float_t  GetX()         const  { return eX      ;}
   Float_t  GetY()         const  { return eY      ;}
   Float_t  GetZ()         const  { return eZ      ;}
   Float_t  GetArea()      const  { return eArea   ;}
   Float_t  GetVolume()    const  { return eVolume ;}
   Int_t    GetFrame()     const  { return eFrame  ;}
   Int_t    GetSide()      const  { return eSide   ;}
   Int_t    GetSegment()   const  { return eSegment;}
   void     SetSegment(int seg)   { eSegment=seg   ;}

   //mandatory virtual functions
   
   virtual Float_t    X()          const { return eX; }
   virtual Float_t    Y()          const { return eY; }
   virtual Float_t    Z()          const { return eZ; }
   virtual void    SetX( float x )       { eX=x; }
   virtual void    SetY( float y )       { eY=y; }
   virtual void    SetZ( float z )       { eZ=z; }

   //other functions
   void Draw(Option_t *opt="") const;

   void TT() const { Test(); }

   ClassDef(EdbCluster,1)  // single cluster
};

//______________________________________________________________________________
class EdbClustersBox : public TObject, public EdbPointsBox3D {

 private:

  TClonesArray *eClusters;  // collection of clusters

 public: 
  EdbClustersBox();
  EdbClustersBox( int n );
  virtual ~EdbClustersBox();

  //members access functions
  void          AddCluster( EdbCluster *c );
  void          AddCluster( float x,  float y,  float z,
			    float a,  float v, int f, int s, int seg );

  Int_t         GetN()             const;
  EdbCluster    *GetCluster(int i) const;
  TClonesArray  *GetClusters()     const { return eClusters; }

  // mandatory virtual functions:
  Int_t         N()       const { return GetN(); }
  EdbPoint     *At(int i)       { return (EdbPoint*)GetCluster(i); }

  //  void  Draw(int style=23, int   col=4, float size=1. ){}

  // other finctions
  // void       Print( Option_t *opt="") const;

  ClassDef(EdbClustersBox,1)  // collection of clusters
};


#endif /* ROOT_EdbCluster */

