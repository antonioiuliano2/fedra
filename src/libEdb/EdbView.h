#ifndef ROOT_EdbView
#define ROOT_EdbView

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbView                                                              //
//                                                                      //
// Base scanning data object: entry into Run tree                       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TClonesArray.h"
#include "TObjArray.h"
#include "TList.h"
#include "TPolyMarker3D.h"
#include "TArrayF.h"


#include "EdbAffine.h"
#include "EdbCluster.h"
//#include "EdbSegment.h"

class EdbSegment;
class EdbTrack;
class EdbFrame;

//______________________________________________________________________________
class EdbViewHeader : public TObject {

private:

  Int_t    eViewID;   // View ID in the Area
  Int_t    eAreaID;   // Area ID in Run (prediction id)

  Float_t  eXview;    // stage coord, top left corner
  Float_t  eYview;    // 

  Float_t  eZ1;    //
  Float_t  eZ2;    // emulsion surfaces in absolute stage coordinates
  Float_t  eZ3;    // measured once per view
  Float_t  eZ4;    //

  Int_t    eNframesTop;  // top    | number of frames in the view (0,1,2...16...)
  Int_t    eNframesBot;  // bottom | 

  Int_t   eTime;         // System time since last view saving in msec

  Int_t   eNclusters;    // number of clusters saved in the view
  Int_t   eNsegments;    // number of segments saved in the view

  EdbAffine2D   eAff;    // affine transformation for the view 
                         // (make sence for SySal-converted data only)

  TArrayF  *eZlevels;    //! z of each taken view (frame) (obsolete!)

  Int_t   eCol;          // the position of the view in the scanned area, measured in views, 
  Int_t   eRow;          // starting from the reference angle (typically up-left)

  Int_t   eStatus;       // View scanning status
public:
  EdbViewHeader();
  virtual ~EdbViewHeader();

  void     Set0();

  void     SetAffine( float a11, float a12, 
		      float a21, float a22, 
		      float b1,  float b2  )
    { eAff.Set(a11,a12,a21,a22,b1,b2); }

  EdbAffine2D const *GetAffine() const { return &eAff; }

  void     SetZlevels(Int_t n, const Float_t *array) { 
    //dummy
    //if(!eZlevels) eZlevels=new TArrayF(n); 
    //eZlevels->Set(n,array);
  }

  TArrayF *GetZlevels() const {return 0;} //eZlevels;}

  Int_t    GetTime()              { return eTime; }
  void     SetTime( Int_t time )  { eTime = time; }

  void     SetNclusters(int nclu) { eNclusters=nclu; }
  void     SetNsegments(int nseg) { eNsegments=nseg; }
  Int_t    GetNclusters() const { return eNclusters; }
  Int_t    GetNsegments() const { return eNsegments; }

  Int_t    GetViewID()      const { return eViewID; }
  Int_t    GetAreaID()      const { return eAreaID; }

  Float_t  GetXview()       const { return eXview;  }
  Float_t  GetYview()       const { return eYview;  }

  void     SetViewID( int id )  { eViewID = id; }
  void     SetAreaID( int id )  { eAreaID = id; }
  void     SetCoordXY(float x, float y)  { eXview = x; eYview=y; }
  void     SetCoordZ(float z1, float z2, float z3, float z4)  
    { eZ1=z1; eZ2=z2; eZ3=z3; eZ4=z4; }
  void     SetNframes( int top, int bot )  { eNframesTop=top; eNframesBot=bot; }
  void     SetColRow( int col, int row) { eCol=col; eRow=row; }
  void     SetStatus( int st) { eStatus=st; }

  Float_t  GetZ1()          const { return eZ1;  }
  Float_t  GetZ2()          const { return eZ2;  }
  Float_t  GetZ3()          const { return eZ3;  }
  Float_t  GetZ4()          const { return eZ4;  }

  Float_t  GetZ0top()       const { return eZ1;  }
  Float_t  GetZ0bot()       const { return eZ3;  }
  Float_t  GetDZtop()       const { return eZ2-eZ1;  }
  Float_t  GetDZbot()       const { return eZ4-eZ3;  }

  Int_t    GetNframesTop()  const { return eNframesTop;  }
  Int_t    GetNframesBot()  const { return eNframesBot;  }

  Int_t    GetCol()         const { return eCol; }
  Int_t    GetRow()         const { return eRow; }
  Int_t    GetStatus()      const { return eStatus; }
  void     Print() const;

  ClassDef(EdbViewHeader,3)  // view identification
};

//______________________________________________________________________________
class EdbView : public TObject {

private:

  EdbViewHeader    *eHeader;      // View header

  TClonesArray     *eClusters;    // array of Clusters
  TClonesArray     *eSegments;    // array of Segments
  TClonesArray     *eTracks;      // array of Tracks
  TClonesArray     *eFrames;      // array of Frames (images)

  Long_t  eLastSystemTime;        // system time when view was saved

public:
  EdbView();
  virtual ~EdbView();

  void Clear();

  void   Transform( const EdbAffine2D *aff );

  void GenerateClustersFog( float density );
  void GenerateClustersSegment( EdbSegment *segment, int n0=25, float sigma=.1 );
  void GenerateClustersTrack( EdbTrack *track, int n0=25, float sigma=.1 );
  void Print( Option_t *opt=0 ) const;
  void PrintClusters( Option_t *opt=0 ) const;

  EdbViewHeader *GetHeader()   const { return eHeader;   }
  TClonesArray  *GetClusters() const { return eClusters; }
  TClonesArray  *GetSegments() const { return eSegments; }
  TClonesArray  *GetTracks()   const { return eTracks;   }
  TClonesArray  *GetFrames()   const { return eFrames;   }

  void          *GetHeaderAddr()    { return &eHeader;   }
  void          *GetClustersAddr()  { return &eClusters; }
  void          *GetSegmentsAddr()  { return &eSegments; }
  void          *GetTracksAddr()    { return &eTracks;   }
  void          *GetFramesAddr()    { return &eFrames;   }

  int          AttachClustersToSegments();
  int          AttachClustersToSegmentsFast();
  int          AttachClustersToSegmentsSlow();

  TObjArray     *GetSegmentsClusters() const;

  void     SetAreaID( int id )  { GetHeader()->SetAreaID(id); }
  void     SetNframes( int top, int bot)  { GetHeader()->SetNframes(top,bot); }
  void     SetCoordXY(float x, float y) { GetHeader()->SetCoordXY(x,y); }
  void     SetCoordZ(float z1, float z2, float z3, float z4) 
    { GetHeader()->SetCoordZ(z1,z2,z3,z4); }


  Int_t    GetViewID()      const { return GetHeader()->GetViewID(); }
  Int_t    GetAreaID()      const { return GetHeader()->GetAreaID(); }

  Float_t  GetXview()       const { return GetHeader()->GetXview();  }
  Float_t  GetYview()       const { return GetHeader()->GetYview();  }

  Float_t  GetZ1()          const { return GetHeader()->GetZ1();  }
  Float_t  GetZ2()          const { return GetHeader()->GetZ2();  }
  Float_t  GetZ3()          const { return GetHeader()->GetZ3();  }
  Float_t  GetZ4()          const { return GetHeader()->GetZ4();  }

  Float_t  GetZ0top()       const { return GetHeader()->GetZ0top();  }
  Float_t  GetZ0bot()       const { return GetHeader()->GetZ0bot();  }
  Float_t  GetDZtop()       const { return GetHeader()->GetDZtop();  }
  Float_t  GetDZbot()       const { return GetHeader()->GetDZbot();  }

  Int_t    GetNframes()     const { return GetNframesTop()+GetNframesBot(); }
  Int_t    GetNframesTop()  const { return GetHeader()->GetNframesTop();    }
  Int_t    GetNframesBot()  const { return GetHeader()->GetNframesBot();    }

  Int_t    GetTime()        const { return GetHeader()->GetTime();    }
  Long_t   GetLastSystemTime()        const { return eLastSystemTime; }
  void     SetLastSystemTime(Long_t time)  { eLastSystemTime=time; }

  TList       *GetClustersFrame( int frame ) const;
  Int_t        Nclusters()       const { return eClusters->GetLast()+1; }
  Int_t        Nsegments()       const { return eSegments->GetLast()+1; }
  Int_t        Ntracks()         const { return eTracks->GetLast()+1; }
  EdbCluster  *GetCluster(int i) const { return (EdbCluster*)eClusters->At(i); }
  EdbSegment  *GetSegment(int i) const { return (EdbSegment*)eSegments->At(i); }
  EdbTrack    *GetTrack(int i)   const { return (EdbTrack*)eTracks->At(i); }
  EdbFrame    *GetFrame(int i)   const { return (EdbFrame*)eFrames->At(i); }

  EdbCluster  *AddCluster( EdbCluster *c ) 
    {return (EdbCluster*)(new((*eClusters)[eClusters->GetLast()+1])  EdbCluster( *c )); }
  EdbCluster  *AddCluster( float x,  float y,  float z,  
			   float a,  float v, int f, int s, int seg=-1) 
    {return (EdbCluster*)(new((*eClusters)[eClusters->GetLast()+1])  EdbCluster(x,y,z, a,v,f,s,seg)); }

  void         AddSegment( EdbSegment *s   );
  void         AddTrack(   EdbTrack   *t   );
  void         AddFrame(   int id, float z, int ncl=0, int npix=0 );
  void         AddFrame(   EdbFrame *frame );

  void         GenerateFrames( int n=32 );
  //TH2S        *GetHist2() const;

  void         DeleteClustersFog();

  TPolyMarker3D  *DrawClustersFog(Option_t *opt=0) const;
  TPolyMarker3D  *DrawClustersSegments(Option_t *opt=0) const;

  void Draw(Option_t *option="");

  Int_t   ReadView( char *fname );

  float Xmin() const { return 0; }  // View coordinates are in pixels
  float Ymin() const { return 0; }  //  starting from 0
  float Xmax() const;
  float Ymax() const;
  float Zmin() const {return TMath::Min( GetZ1(), GetZ4() );}
  float Zmax() const {return TMath::Max( GetZ1(), GetZ4() );}

  ClassDef(EdbView,2)  // Base scanning data object: entry into Run tree
};

#endif /* ROOT_EdbView */



