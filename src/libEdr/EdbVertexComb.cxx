//-- Author :  Valeri Tioukov  27.11.2011
 
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertexComb                                                        //
//                                                                      //
//   combinatorial selection of the best tracks/vertex combinations     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TArrayF.h"
#include "EdbVertexComb.h"
#include "EdbCombGen.h"
#include "EdbPVRec.h"
#include "EdbLog.h"

using namespace TMath;

ClassImp(EdbVertexComb);
ClassImp(EdbTopology);

//===================================================================================
Float_t EdbTopology::Probability()
{
  float prob=1;
  int n=Nvtx();               if(!n) return 0;
  for(int i=0; i<n; i++)
  {
    EdbVertex *v = GetVertex(i);
    prob *= v->V()->prob();
  }
  return prob;
}

//________________________________________________________________________
Float_t EdbTopology::DZ()
{
  float dz=0;
  int n=Nvtx();               if(n<2) return dz;
  float zmin,zmax;
  zmin=zmax=GetVertex(0)->VZ();
  for(int i=1; i<n; i++)
  {
    float z = GetVertex(i)->VZ();
    if(z>zmax) zmax=z;
    if(z<zmin) zmin=z;
  }
  return zmax-zmin;
}

//________________________________________________________________________
bool EdbTopology::IsEqual(EdbTopology &t)
{
  if(       Nvtx() != t.Nvtx()       ) return false;
  if(Probability() != t.Probability()) return false;
  if(         DZ() != t.DZ()         ) return false;
  return true;
}

//________________________________________________________________________
void EdbTopology::OrderVtxByZ()
{
  int nt = Nvtx();  
  TArrayF rating(nt);  TArrayI ind(nt);
  for(int i=0; i<nt; i++)     rating[i] = GetVertex(i)->VZ();
  Sort( nt, rating.GetArray(), ind.GetArray(), 1 );
  TObjArray tmparr(nt);
  for(int i=0; i<nt; i++)   tmparr.Add( GetVertex(ind[i]) );
  eVertices.Clear();  eVertices.AddAll( &tmparr );
}

//-----------------------------------------------------------------------------
void EdbTopology::PrintTracks()
{
  printf("---------------------------- Single Tracks --------------------------------------------------------\n");
  printf("  idtr         X            Y            Z      TX       TY         P    idpl\n");
  printf("--------------------------------------------------------------------------------------------------\n");
  int ntr = eTracks.GetEntriesFast();
  for(int i=0; i<ntr; i++) {
    EdbTrackP *t = GetTrack(i);
    printf("%5d %12.2f %12.2f %12.2f %8.4f %8.4f %8.2f %4d\n"
        ,t->ID(), t->X(), t->Y(), t->Z(), t->TX(), t->TY(), t->P(), t->Plate() );
  }
  printf("--------------------------------------------------------------------------------------------------\n");
}

//________________________________________________________________________
char *EdbTopology::TracksStr()
{
  char *str = new char[1000];   memset(str,'\0',1000);
  char *cstr=str;     cstr+=75; memset(str,' ',75);
  sprintf(cstr,"   (");  cstr+=4;
  for(int j=0; j<Ntr(); j++) { sprintf(cstr," %6d", GetTrack(j)->ID() ); cstr+=7; }
  sprintf(cstr," )");  cstr+=2;
  return str;
}

//________________________________________________________________________
char *EdbTopology::VertexStr(EdbVertex &v)
{
  char *str = new char[1000];   memset(str,'\0',1000);
  char *cstr=str;
  sprintf(cstr,"%5d  %9.7f %9.1f %9.1f %9.1f %9.3f %9.4f %9.6f",
          v.ID(),v.V()->prob(), v.VX(), v.VY(), v.VZ(), v.MaxImpact(), v.MaxAperture(), v.Volume()/100/100/100 );
  cstr+=75;
  sprintf(cstr,"   (");  cstr+=4;
  for(int j=0; j<v.N(); j++) { sprintf(cstr," %6d", v.GetTrack(j)->ID() ); cstr+=7; }
  sprintf(cstr," )");  cstr+=2;
  return str;
}

//________________________________________________________________________
void EdbTopology::Print()
{
  int nvtx = eVertices.GetEntriesFast();
  printf("\nEdbTopology: prob = %10.8f    dz= %10.2f\n",Probability(), DZ());
  printf("  id     Prob         X          Y        Z      MaxImp   MaxApert    Vol[(0.1mm)³]    Tracks\n");
  for(int i=0; i< nvtx; i++) 
  {
    EdbVertex *v = GetVertex(i);
    printf( "%s\n", VertexStr(*v) );
  }
  printf( "%s\n", TracksStr() );
}

//===================================================================================
EdbVertexComb::EdbVertexComb( TObjArray &tracks )
{
  Set0();
  eTracks = tracks;
}

//________________________________________________________________________
EdbVertexComb::~EdbVertexComb()
{
}

//-----------------------------------------------------------------------------
void EdbVertexComb::Set0()
{
  eRecursionMax   = 1;
  eRecursion      = 0;
  eZ0             = 0;
  eProbMinV       = 0.001;     // min probability to accept the vertex for topology calculation
  eNProngMinV     = 2;         // min prongs to accept the vertex for topology calculation
}

//-----------------------------------------------------------------------------
void EdbVertexComb::CopyPar(EdbVertexComb &comb)
{
  eRecursionMax   = comb.eRecursionMax;
  eRecursion      = comb.eRecursion;
  eCond           = comb.eCond;
  eVPar           = comb.eVPar;
  eZ0             = comb.eZ0;
  eProbMinV       = comb.eProbMinV;
  eNProngMinV     = comb.eNProngMinV;
}

//-----------------------------------------------------------------------------
void EdbVertexComb::PrintTeoricalCombinations(Int_t n)
{
  // Print only the first level (1 vertex combinations)
  Long_t total=0;
  for(int i=1; i<=n; i++) {
    Long_t ncomb = (Long_t)(Factorial(n)/Factorial(n-i)/Factorial(i));
    printf("%8d of %8d = %16ld \n", i,n,ncomb);
    total +=ncomb;
  }
  printf("total = %ld\n",total);
}

//-----------------------------------------------------------------------------
void EdbVertexComb::SetTracksErrors(TObjArray &tracks, EdbScanCond &cond)
{
  int n = tracks.GetEntriesFast();
  for(int i=0; i<n; i++) {
     EdbTrackP *t = (EdbTrackP*)tracks.At(i);
     int nseg = t->N();
     for(int j=0; j<nseg; j++) {
       EdbSegP   *s = t->GetSegment(j);
       s->SetErrors0();
       cond.FillErrorsCov( s->TX(),s->TY(), s->COV() );
     }
     t->FitTrackKFS();
  }
}

//______________________________________________________________________________
int EdbVertexComb::FindTopologies()
{
  FormVertices();
  eTopologies.Clear();
  SelectTopologies(eTopologies);
  SortTopologies(eTopologies);
  ClearDoublets(eTopologies);
  int n = eTopologies.GetEntriesFast();
  Log(2,"EdbVertexComb::FindTopologies"," %d input tracks, upto %d vtx to search  -> %d topologies found", eTracks.GetEntriesFast(), eRecursionMax, n);
  return n;
}

//______________________________________________________________________________
void EdbVertexComb::PrintTopologies()
{
  int n = eTopologies.GetEntriesFast();
  PrintTracks();
  for(int i=0; i<n; i++) GetTopology(i)->Print();
}

//______________________________________________________________________________
void EdbVertexComb::FormVertices()
{
  if( eRecursion >= eRecursionMax )      return;
  int ntr = eTracks.GetEntriesFast();
  if(ntr<eNProngMinV) return;
  if(ntr<2)           return;
  //if(nprongMin<2)   return 0;

  //float distMax=100000;
  for( int nitems=eNProngMinV; nitems <= ntr; nitems++ )
  {
    EdbCombGen comber(eTracks,nitems);
    TObjArray selected, other;
    while( comber.NextCombination(selected,other) ) {
       EdbVertex *v = CheckVTX(selected);
       if( v->V()->prob() < eProbMinV )  continue;
       //if( v->MinDist()   > distMax  ) continue;
       eVertices.Add(v);
       if(eRecursion<eRecursionMax) {
         EdbVertexComb *combo = new EdbVertexComb(other);
         combo->CopyPar(*this);
         combo->eRecursion++;
         eOther.Add(combo);
       }
    }
  }

  int nvtx = Nvtx();
  //printf("eRecursion = %d   nvtx = %d\n",eRecursion, nvtx);
  
  for(int i=0; i<nvtx; i++)   {
      EdbVertexComb *combo = (EdbVertexComb*)(eOther.At(i));
      if(combo) combo->FormVertices();
  }
}

//-----------------------------------------------------------------------------
void EdbVertexComb::PrintTracks()
{
  printf("\n---------------------------- Input Tracks --------------------------------------------------------\n");
  printf("  idtr         X            Y            Z      TX       TY         P    idpl\n");
  printf("--------------------------------------------------------------------------------------------------\n");
  int ntr = eTracks.GetEntriesFast();
  for(int i=0; i<ntr; i++) {
    EdbTrackP *t = (EdbTrackP*)(eTracks.At(i));
    printf("%5d %12.2f %12.2f %12.2f %8.4f %8.4f %8.2f %4d\n"
        ,t->ID(), t->X(), t->Y(), t->Z(), t->TX(), t->TY(), t->P(), t->Plate() );
  }
  printf("--------------------------------------------------------------------------------------------------\n");
}

//-----------------------------------------------------------------------------
void EdbVertexComb::ClearDoublets(TObjArray &topoarr)
{
  int nremove=1;
  do {
    nremove=0;
    int nt = topoarr.GetEntriesFast(); if(!nt) return;
    for(int i=nt-1; i>0; i--) {
      EdbTopology *t  = (EdbTopology*)topoarr.At(i);
      EdbTopology *tn = (EdbTopology*)topoarr.At(i-1);
      if( t->IsEqual(*tn) ) { topoarr.RemoveAt(i); nremove++; }
    }
    topoarr.Compress();
  } while(nremove);
}

//-----------------------------------------------------------------------------
void EdbVertexComb::SortTopologies(TObjArray &topoarr)
{
  int nt = topoarr.GetEntriesFast();
  TArrayF rating(nt);  TArrayI ind(nt);
  for(int i=0; i<nt; i++)     rating[i] =((EdbTopology*)topoarr.At(i))->Probability();
  Sort( nt, rating.GetArray(), ind.GetArray(), 1 );
  TObjArray tmparr(nt);
  for(int i=0; i<nt; i++)   tmparr.Add( topoarr.At(ind[i]) );
  topoarr.Clear();  topoarr.AddAll( &tmparr );
  
  for(int i=0; i<nt; i++)   ((EdbTopology*)topoarr.At(i))->OrderVtxByZ();
}

//-----------------------------------------------------------------------------
void EdbVertexComb::SelectTopologies(TObjArray &topoarr)
{
  int nvtx = eVertices.GetEntriesFast();
  Log(3,"EdbVertexComb::SelectTopologies","nvtx = %d",nvtx);
  if(!nvtx) return;
  for(int i=0; i<nvtx; i++)
  {
    EdbVertex *v = GetVertex(i);
    //if(!IsAcceptable(*v))  continue;
    
    TObjArray newarr;
    int nnew=0;
    EdbVertexComb *combo = GetVertexComb(i);
    if(combo) {
      combo->SelectTopologies(newarr);
      nnew=newarr.GetEntriesFast();
    }
    if(!nnew) { 
      EdbTopology *topa = new EdbTopology();
      topa->AddSingleTracks(combo->eTracks);
      newarr.Add( topa ); 
      nnew+=1;
    }
    for(int j=0; j< nnew; j++) {
       EdbTopology *topa = (EdbTopology *)(newarr.At(j));
       topa->AddVertex( v );
       topoarr.Add( topa );
    }
  }
}

//-----------------------------------------------------------------------------
bool EdbVertexComb::IsAcceptable( EdbVertex &v )
{
  if( v.V()->prob() > 0.001 ) return true;
  return false;
}

//-----------------------------------------------------------------------------
EdbVertex *EdbVertexComb::CheckVTX(TObjArray &tracks)
{
  if(tracks.GetEntriesFast() < 2 ) return 0;
  EdbVertexRec evr(eVPar);
  EdbPVRec *pvr = new EdbPVRec();
  pvr->SetScanCond( new EdbScanCond(eCond) );
  evr.SetPVRec(pvr);
  return evr.Make1Vertex( tracks, eZ0 );
}


