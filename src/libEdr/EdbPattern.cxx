//-- Author :  Valeri Tioukov   19.05.2002
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbPattern                                                           //
//                                                                      //
// Segments pattern                                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TIndexCell.h"
#include "EdbAffine.h"
#include "EdbPattern.h"

ClassImp(EdbSegP)
ClassImp(EdbSegmentsBox)
ClassImp(EdbTrackP)
ClassImp(EdbPattern)
ClassImp(EdbPatternsVolume)

//______________________________________________________________________________
EdbSegP& EdbSegP::operator += (EdbSegP const& s) 
{
  float dz = s.Z() - Z();

  float tx,ty,stx,sty;   //base angle
  float w,w1,w2;

  if (dz == 0 ) {                     // base vector is mean of angles
    w1 = 1./STX()/STX();
    w2 = 1./s.STX()/s.STX();
    tx = (w1*TX()+ w2*s.TX())/(w1+w2);
    stx = TMath::Sqrt(1./(w1+w2));

    w1 = 1./(STY()*STY());
    w2 = 1./(s.STY()*s.STY());
    ty = (w1*TY()+ w2*s.TY())/(w1+w2);
    sty = TMath::Sqrt(1./(w1+w2));

  } else {        // base vector as 2-points vector

    tx = (s.X() - X()) / dz;
    ty = (s.Y() - Y()) / dz;
    stx = TMath::Sqrt( (SX()*SX() + s.SX()*s.SX())/(dz*dz) );
    sty = TMath::Sqrt( (SY()*SY() + s.SY()*s.SY())/(dz*dz) );

    if(stx>0&&sty>0) {              // correction of base angle to segments angles
      w  = 1./(stx*stx);
      w1 = 1./(STX()*STX());
      w2 = 1./(s.STX()*s.STX());
      tx = (tx*w + TX()*w1 + s.TX()*w2)/(w+w1+w2);
      stx = TMath::Sqrt(1./(w+w1+w2));

      w  = 1./(sty*sty);
      w1 = 1./(STY()*STY());
      w2 = 1./(s.STY()*s.STY());
      ty = (ty*w + TY()*w1 + s.TY()*w2)/(w+w1+w2);
      sty = TMath::Sqrt(1./(w+w1+w2));
    }
  }

  // extrapolate s1 to s2->Z() and calculate mean x,y

  float x1  = X() + dz*tx;
  float y1  = Y() + dz*ty;
  float wx1 = 1./( SX()*SX() + stx*dz*stx*dz );
  float wy1 = 1./( SY()*SY() + sty*dz*sty*dz );

  float wx2 = 1./s.SX()/s.SX();
  float wy2 = 1./s.SY()/s.SY();

  float x = (x1*wx1 + s.X()*wx2)/(wx1+wx2);
  float y = (y1*wy1 + s.Y()*wy2)/(wy1+wy2);
  float sx = TMath::Sqrt(1./(wx1+wx2));
  float sy = TMath::Sqrt(1./(wy1+wy2));
  float sz = s.SZ();

  Set(s.ID(),x,y,tx,ty,W()+s.W(),s.Flag());
  SetZ(s.Z());
  SetErrors(sx,sy,sz,stx,sty);

  return *this;
}

//______________________________________________________________________________
double EdbSegP::Chi2( EdbSegP &s ) const
{
  //calculated at the s.Z();

  double dz = s.Z()-Z();
  double x1  = X() + dz * TX();
  double y1  = Y() + dz * TY();

  double sx = TMath::Sqrt( SX()*SX() + dz*STX()*dz*STX() );
  double sy = TMath::Sqrt( SY()*SY() + dz*STY()*dz*STY() );

  double stx = TMath::Sqrt( STX()*STX() + s.STX()*s.STX() );
  double sty = TMath::Sqrt( STY()*STY() + s.STY()*s.STY() );

  double dx  = (s.X()-x1)/sx;
  double dy  = (s.Y()-y1)/sy;
  double dtx = (s.TX()-TX())/stx;
  double dty = (s.TY()-TY())/sty;

  return TMath::Sqrt(dx*dx + dy*dy + dtx*dtx + dty*dty)/2.;
}

//______________________________________________________________________________
float EdbSegP::Chi2A( EdbSegP &s ) const
{
  // ignore the position errors here;

  float dz  = s.Z()-Z();
  float tx  = (s.X()-X())/dz;
  float ty  = (s.Y()-Y())/dz;

  float dtx1 = (TX()-tx)*(TX()-tx)/STX()/STX();
  float dty1 = (TY()-ty)*(TY()-ty)/STY()/STY();
  float dtx2 = (s.TX()-tx)*(s.TX()-tx)/s.STX()/s.STX();
  float dty2 = (s.TY()-ty)*(s.TY()-ty)/s.STY()/s.STY();

  return TMath::Sqrt(dtx1+dty1+dtx2+dty2)/2.;
}

//______________________________________________________________________________
float EdbSegP::Chi2Aprob( EdbSegP &s ) const
{
  // ignore the position errors here;
  // use segments probablility information

  float dz  = s.Z()-Z();
  float tx  = (s.X()-X())/dz;
  float ty  = (s.Y()-Y())/dz;

  float dtx1 = (TX()-tx)*(TX()-tx)/STX()/STX()/Prob();
  float dty1 = (TY()-ty)*(TY()-ty)/STY()/STY()/Prob();
  float dtx2 = (s.TX()-tx)*(s.TX()-tx)/s.STX()/s.STX()/s.Prob();
  float dty2 = (s.TY()-ty)*(s.TY()-ty)/s.STY()/s.STY()/s.Prob();

  return TMath::Sqrt(dtx1+dty1+dtx2+dty2)/2.;
}

//______________________________________________________________________________
void EdbSegP::PropagateTo( float z ) 
{
  float dz = z-Z();

  eX  = X() + TX()*dz;
  eY  = Y() + TY()*dz;
  eZ  = z;
  eSX = TMath::Sqrt( SX()*SX() + STX()*dz*STX()*dz );
  eSY = TMath::Sqrt( SY()*SY() + STY()*dz*STY()*dz );
}

//______________________________________________________________________________
float EdbSegP::ProbLink( EdbSegP &s1, EdbSegP &s2 )
{
  // return probability of the correct link in case Up/Down - specifics is 
  // that the position errors are neglected)

  double dz = s2.Z() - s1.Z();

  double tx = (s2.X() - s1.X()) / dz;
  double ty = (s2.Y() - s1.Y()) / dz;

  double dtx1 = (s1.TX()-tx)/s1.STX();
  double dty1 = (s1.TY()-ty)/s1.STY();
  double dtx2 = (s2.TX()-tx)/s2.STX();
  double dty2 = (s2.TY()-ty)/s2.STY();

  double chi2 = TMath::Sqrt(dtx1*dtx1 + dty1*dty1 + dtx2*dtx2 + dty2*dty2);
  double p3 = TMath::Prob(chi2,4);

  return s1.Prob()*s2.Prob()*p3;
}

//______________________________________________________________________________
void EdbSegP::MergeTo( EdbSegP &s ) 
{
  // create linked segment at Z of s2 
  // TODO - navesti nauku covariantnuiu 
  
  PropagateTo( s.Z() );

  float wx1,wx2, wy1,wy2;
  float wtx1,wtx2, wty1,wty2;

  wx1 = 1/SX()/SX();
  wx2 = 1/s.SX()/s.SX();
  wy1 = 1/SY()/SY();
  wy2 = 1/s.SY()/s.SY();
  wtx1 = 1/STX()/STX();
  wtx2 = 1/s.STX()/s.STX();
  wty1 = 1/STY()/STY();
  wty2 = 1/s.STY()/s.STY();

  eX = (X()*wx1 + s.X()*wx2)/(wx1+wx2);
  eY = (Y()*wy1 + s.Y()*wy2)/(wy1+wy2);
  eSX = TMath::Sqrt( 1./(wx1+wx2) );
  eSY = TMath::Sqrt( 1./(wy1+wy2) );

  eTX = (TX()*wtx1 + s.TX()*wtx2)/(wtx1+wtx2);
  eTY = (TY()*wty1 + s.TY()*wty2)/(wty1+wty2);
  eSTX = TMath::Sqrt( 1./(wtx1+wtx2) );
  eSTY = TMath::Sqrt( 1./(wty1+wty2) );

  eZ = s.Z();
  eSZ = TMath::Sqrt(( SZ()*SZ() + s.SZ()*s.SZ())/2);

  eW = W()+s.W();

  eID   = s.ID();
  ePID  = s.PID();
  eFlag = s.Flag();
}

//______________________________________________________________________________
void EdbSegP::Print(Option_t *opt) const
{
  printf("EdbSegP: %d  %f %f %f  %f %f  %f  %d\n", ID(),X(),Y(),Z(),TX(),TY(),W(),Flag() );
} 

//______________________________________________________________________________
EdbSegmentsBox::EdbSegmentsBox()
{
  eSegments = new TClonesArray("EdbSegP");
  Set0();

}
 
//______________________________________________________________________________
EdbSegmentsBox::EdbSegmentsBox(float x0, float y0, float z0)
{
  eSegments = new TClonesArray("EdbSegP");
  eX=x0;  eY=y0;  eZ=z0;
  eDZkeep=0;
}
 
//______________________________________________________________________________
EdbSegmentsBox::~EdbSegmentsBox( )
{
  if(eSegments) delete eSegments;
}
 
//______________________________________________________________________________
void EdbSegmentsBox::Set0()
{
  eX=0;  eY=0;  eZ=0;
  eDZkeep=0;
}
 
//______________________________________________________________________________
void EdbSegmentsBox::AddSegment(int id, float x, float y, float tx, float ty, 
			    float w, int flag)
{
  new((*eSegments)[N()])  EdbSegP( id,x,y,tx,ty,w,flag );
}
 
//______________________________________________________________________________
void EdbSegmentsBox::AddSegment( EdbSegP &s )
{
  new((*eSegments)[N()])  EdbSegP( s );
}
 
//______________________________________________________________________________
void EdbSegmentsBox::AddSegment( EdbSegP &s1, EdbSegP &s2 )
{
  EdbSegP *s = new((*eSegments)[N()])  EdbSegP( s1 );
  s->MergeTo(s2);
}
 
//______________________________________________________________________________
void EdbSegmentsBox::Reset()
{
  Set0();
  if(eSegments) eSegments->Clear();
}
 
//______________________________________________________________________________
float EdbSegmentsBox::DiffAff(EdbAffine2D *aff)
{
  EdbSegmentsBox p,p1;
  p.AddSegment(0,Xmin(),Ymin(),0.,0.);
  p.AddSegment(0,Xmax(),Ymin(),0.,0.);
  p.AddSegment(0,Xmin(),Ymax(),0.,0.);
  p.AddSegment(0,Xmax(),Ymax(),0.,0.);

  p1.AddSegment(0,Xmin(),Ymin(),0.,0.);
  p1.AddSegment(0,Xmax(),Ymin(),0.,0.);
  p1.AddSegment(0,Xmin(),Ymax(),0.,0.);
  p1.AddSegment(0,Xmax(),Ymax(),0.,0.);

  p1.Transform(aff);
  return p.Diff(p1);
}
 
//______________________________________________________________________________
float EdbSegmentsBox::Diff(EdbSegmentsBox &p)
{
  // return the mean difference beteween pattern elements
  int nseg = TMath::Min( N(), p.N() );
  if(nseg<1) return 0;

  EdbSegP *s1=0, *s2=0;

  float dx=0, dy=0;
  double sdx=0;
  for(int i=0; i<nseg; i++ ) {
    s1 =   GetSegment(i);
    s2 = p.GetSegment(i);
    dx = s2->X() - s1->X();
    dy = s2->Y() - s1->Y();
    sdx += TMath::Sqrt( dx*dx + dy*dy);
  }
  return sdx/nseg;
}
 
//______________________________________________________________________________
void EdbSegmentsBox::ProjectTo(const float dz)
{
  eZ += dz;  eDZkeep += dz;
 
 EdbSegP *p;
  for(int i=0; i<N(); i++ ) {
    p = GetSegment(i);
    p->SetX( p->X() + p->TX()*dz );
    p->SetY( p->Y() + p->TY()*dz );
  }
}
 
//______________________________________________________________________________
int EdbSegmentsBox::CalculateXY( EdbSegmentsBox *pat, EdbAffine2D *aff )
{
  int n=N();
  if( n>pat->N() )  n=pat->N();
  if(n<2) return 0;

  aff->Calculate(this,pat);
  return 1;
}

//______________________________________________________________________________
int EdbSegmentsBox::CalculateAXAY( EdbSegmentsBox *pat, EdbAffine2D *aff )
{
  int n=N();
  if( n>pat->N() )  n=pat->N();
  if(n<2) return 0;

  float *ax1 = new float[n];
  float *ay1 = new float[n];
  float *ax2 = new float[n];
  float *ay2 = new float[n];

  EdbSegP *p1,*p2;
  for(int i=0; i<n; i++ ) {
    p1 = GetSegment(i);
    p2 = pat->GetSegment(i);
    ax1[i] = p1->TX();
    ay1[i] = p1->TY();
    ax2[i] = p2->TX();
    ay2[i] = p2->TY();
  }
  aff->Calculate(n,ax1,ay1,ax2,ay2);

  delete ax1;
  delete ay1;
  delete ax2;
  delete ay2;

  return 1;
}

//______________________________________________________________________________
void EdbSegmentsBox::TransformA( EdbAffine2D *aff )
{
  EdbSegP *p;
  float tx,ty;

  for(int i=0; i<N(); i++ ) {
    p = GetSegment(i);

    tx = aff->A11()*p->TX() + aff->A12()*p->TY() + aff->B1();
    ty = aff->A21()*p->TX() + aff->A22()*p->TY() + aff->B2();
    p->SetTX(tx);
    p->SetTY(ty);
  }
}

//______________________________________________________________________________
void EdbSegmentsBox::TransformARot( EdbAffine2D *aff )
{
  // apply to the angles only rotation members of transformation

  EdbSegP *p;
  float tx,ty;

  for(int i=0; i<N(); i++ ) {
    p = GetSegment(i);

    tx = aff->A11()*p->TX() + aff->A12()*p->TY();
    ty = aff->A21()*p->TX() + aff->A22()*p->TY();
    p->SetTX(tx);
    p->SetTY(ty);
  }
}

//______________________________________________________________________________
void EdbSegmentsBox::Print(Option_t *opt) const
{
  printf("EdbSegmentsBox: %d segments\n", GetN() );
  for(int i=0; i<GetN(); i++) GetSegment(i)->Print();
} 

//______________________________________________________________________________
//______________________________________________________________________________
EdbTrackP::EdbTrackP()
{
  eID = 0;
  eVid = 0;
}
 
//______________________________________________________________________________
EdbTrackP::~EdbTrackP()
{

}

//______________________________________________________________________________
void EdbTrackP::Copy(EdbTrackP &tr)
{
  Reset();
  eID = tr.ID();
  for(int i=0; i<tr.N(); i++)
    AddSegment(*tr.GetSegment(i));
}

//______________________________________________________________________________
//______________________________________________________________________________
EdbPattern::EdbPattern()
{
  eCell     = new TIndexCell();
  Set0();
}

//______________________________________________________________________________
EdbPattern::EdbPattern(float x0, float y0, float z0) : EdbSegmentsBox(x0,y0,z0) 
{
  eCell     = new TIndexCell();
  Set0();
}
 
//______________________________________________________________________________
EdbPattern::~EdbPattern( )
{
  if(eCell)     delete eCell;
}

//______________________________________________________________________________
void EdbPattern::Set0()
{
  eID = 0;
}

//______________________________________________________________________________
void EdbPattern::Reset()
{
  ((EdbSegmentsBox *)this)->Reset();
  Set0();
  if(eCell)     eCell->Drop();
}
 
////////////////////////////////////////////////////////////////////////////////
//
//   EdbPatternsVolume
//
////////////////////////////////////////////////////////////////////////////////


//______________________________________________________________________________
EdbPatternsVolume::EdbPatternsVolume()
{
  ePatterns   = new TObjArray();
  eTracksCell = 0;
  eTracks     = new TObjArray();
  eSV  = 0;
  Set0();
}

//______________________________________________________________________________
EdbPatternsVolume::EdbPatternsVolume(EdbPatternsVolume &pvol)
{
  ePatterns   = new TObjArray();
  eTracksCell = 0;
  eTracks     = new TObjArray();
  eSV  = 0;
  Set0();

  pvol.PassProperties(*this);
  for(int j=0; j<Npatterns(); j++) {
    for(int i=0; i<GetPattern(j)->N(); i++ ) {
      pvol.GetPattern(j)->AddSegment( *(GetPattern(j)->GetSegment(i)) );
    }
  }
}

//______________________________________________________________________________
EdbPatternsVolume::~EdbPatternsVolume()
{
  if(ePatterns) {
    ePatterns->Delete();
    delete ePatterns;
  }
}

//______________________________________________________________________________
void EdbPatternsVolume::Set0()
{

}
 
//______________________________________________________________________________
int EdbPatternsVolume::DropCouples()
{
  int count=0;
  for(int i=0; i<Npatterns(); i++ )
    count += GetPattern(i)->Cell()->DropCouples(4);
  if(count) printf("%d couples are dropped in volume cells\n",count);
  return count;
}
 
//______________________________________________________________________________
void EdbPatternsVolume::SetPatternsID()
{
  for(int i=0; i<Npatterns(); i++ )
    GetPattern(i)->SetID(i);
}
 
//______________________________________________________________________________
void EdbPatternsVolume::Transform( EdbAffine2D *aff )
{
  for(int i=0; i<Npatterns(); i++ )  {
    GetPattern(i)->Transform(aff);
    GetPattern(i)->TransformARot(aff);
  }
}
 
//______________________________________________________________________________
void EdbPatternsVolume::Centralize()
{
  // find geometrical center (XY) of all patterns and set it as the center of 
  // coordinates  to simplify transformations
  // To be used before any operations on patterns

  float xc=0;
  float yc=0;
  for(int i=0; i<Npatterns(); i++ ) {
    xc += GetPattern(i)->Xmax() + GetPattern(i)->Xmin();
    yc += GetPattern(i)->Ymax() + GetPattern(i)->Ymin();
  }
  xc = xc/Npatterns()/2;
  yc = yc/Npatterns()/2;

  eX = xc;  eY=yc;

  Shift(-xc,-yc);
  for(int i=0; i<Npatterns(); i++ ) 
    GetPattern(i)->SetKeep(1,0,0,1,0,0);
}

//______________________________________________________________________________
void EdbPatternsVolume::PrintAff() const
{
  EdbAffine2D a;
  for(int i=0; i<Npatterns(); i++ ) {
    GetPattern(i)->GetKeep(a);
    printf(" %d ",i); a.Print();
  }
}

//______________________________________________________________________________
void EdbPatternsVolume::PrintStat( Option_t *opt="") const
{
  int npat = Npatterns();
  printf("\nVolume statistics for %d patterns\n",npat);

  float dx,dy;
  EdbPattern *pat=0;
  printf("pat# \t segments \t dX \t\tdY \t meanDist \n");
  for(int i=0; i<Npatterns(); i++ ) {
    pat = GetPattern(i);
    dx = pat->Xmax() - pat->Xmin();
    dy = pat->Ymax()- pat->Ymin();
    printf(" %d\t %d\t %10.2f \t %10.2f \t %10.4f \n", 
	   i, pat->GetN(),dx,dy, TMath::Sqrt(dx*dy/pat->GetN()) );
  }

  for(int i=0; i<Npatterns(); i++ ) {
    pat = GetPattern(i);
    pat->Cell()->PrintStat();
  }
}
 
//______________________________________________________________________________
void EdbPatternsVolume::PrintStat(EdbPattern &pat) const
{
} 

//______________________________________________________________________________
void EdbPatternsVolume::AddPattern( EdbPattern *pat )
{
  ePatterns->Add(pat);
}

//______________________________________________________________________________
EdbPattern *EdbPatternsVolume::GetPattern( int id ) const
{
  if(Npatterns()>id) return (EdbPattern*)ePatterns->At(id);
  else return 0;
}


//______________________________________________________________________________
void EdbPatternsVolume::PassProperties( EdbPatternsVolume &pvol )
{
  pvol.SetXYZ(eX,eY,eZ);
  EdbAffine2D a;
  EdbPattern *p=0;

  for(int i=0; i<Npatterns(); i++ ) {
    p = GetPattern(i);
    p->GetKeep(a);
    EdbPattern *psel = new EdbPattern( p->X(),p->Y(),p->Z() );
    psel->SetKeep( a.A11(), a.A12(), a.A21(), a.A22(), a.B1(),a.B2() );
    pvol.AddPattern(psel);
  }
  pvol.SetPatternsID();
}


//______________________________________________________________________________
void EdbPatternsVolume::Shift( float x, float y )
{
  EdbAffine2D aff;
  aff.ShiftX(x);
  aff.ShiftY(y);

  for(int i=0; i<Npatterns(); i++)
    GetPattern(i)->Transform(&aff);
}

//______________________________________________________________________________
void EdbPatternsVolume::DropCell()
{
  for(int i=0; i<Npatterns(); i++ ) GetPattern(i)->Cell()->Drop();
}
