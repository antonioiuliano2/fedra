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

using namespace TMath;

ClassImp(EdbVertexComb);

//______________________________________________________________________________
EdbVertexComb::EdbVertexComb( TObjArray &tracks )
{
  eTracks = tracks;
}

//________________________________________________________________________
EdbVertexComb::~EdbVertexComb()
{
}

//-----------------------------------------------------------------------------
void EdbVertexComb::SetTracksErrors(TObjArray &tracks, EdbScanCond &cond)
{
  int n = tracks.GetEntries();
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
float EdbVertexComb::SelectSortVertices( int nprongMin, float probMin )
{
  int ntr = eTracks.GetEntries();
  if(ntr<nprongMin) return 0;
  if(ntr<2)         return 0;
  if(nprongMin<2)   return 0;

  printf("probmin = %g\n",probMin);
  float distMax=100000;
  TObjArray vertices;
  for( int nitems=nprongMin; nitems <= ntr; nitems++ )
  {
    EdbCombGen comber(eTracks,nitems);
    TObjArray selected, other;
    while( comber.NextCombination(selected,other) ) {
       EdbVertex *v = CheckVTX(selected);
       if( v->V()->prob() < probMin ) continue;
       if( v->MinDist()   > distMax  ) continue;
       vertices.Add(v);
    }
  }

  int nvtx = vertices.GetEntries();
  TArrayF rating(nvtx);
  TArrayI ind(nvtx);
  for(int i=0; i<nvtx; i++) {
    EdbVertex *v = (EdbVertex*)vertices.At(i);
    rating[i]= v->V()->prob() * v->N();               // TODO select best criterium
  }
  Sort( nvtx, rating.GetArray(), ind.GetArray(), 1 );
  
  for(int i=0; i<nvtx; i++)   eVertices.Add( (EdbVertex*)vertices.At(ind[i]) );
  
  return nvtx;
}

//-----------------------------------------------------------------------------
void EdbVertexComb::PrintSelectedVTX()
{
  printf("\n-------------------- EdbVertexComb::PrintSelectedVTX --------------------------------------\n\n");
  for(int i=0; i<eTracks.GetEntries(); i++) ((EdbTrackP*)eTracks.At(i))->PrintNice();

  printf("\n---------------------------- Vertex list --------------------------------------------------------\n");
  printf(" Rating  Prob         X          Y        Z      MaxImp   MaxApert    Vol[(0.1mm)³]    Tracks\n");
  printf("-------------------------------------------------------------------------------------------------\n");
  int nvtx = eVertices.GetEntries();
  for(int i=0; i< nvtx; i++) 
    {
      EdbVertex *v = (EdbVertex*)(eVertices.At(i));
      printf("%5d  %9.7f %9.1f %9.1f %9.1f %9.1f %9.4f %9.1f", 
        i,v->V()->prob(), v->VX(), v->VY(), v->VZ(), v->MaxImpact(), v->MaxAperture(), v->Volume()/100/100/100 );
      printf("\t(");
      for(int i=0; i<v->N(); i++) printf("%6d", v->GetTrack(i)->ID() );
      printf(" )\n");
    }
    printf("-------------------------------------------------------------------------------------------\n");

  printf("Vertexing parameters: eUseKalman = %d  eUseMom = %d\n", eVPar.eUseKalman, eVPar.eUseMom );

  printf("\n The measurements errors assigned to tracks at last measured point:\n");
  printf("  sigmaX0  sigmaY0  sigmaTX0  sigmaTY0   Degrad\n");
  printf("%8.3f %8.3f  %8.4f  %8.4f %8.2f\n",
    eCond.SigmaX(0),eCond.SigmaY(0),eCond.SigmaTX(0),eCond.SigmaTY(0),eCond.Degrad() );
  printf("The errors extrapolated to the vertex point taking into account MCS with Rad length of the media  X0 = %10.2f [microns]\n",eCond.RadX0());

}


//-----------------------------------------------------------------------------
EdbVertex *EdbVertexComb::CheckVTX(TObjArray &tracks)
{
  if(tracks.GetEntries() < 2 ) return 0;
  EdbVertexRec evr(eVPar);
  EdbPVRec *pvr = new EdbPVRec();
  pvr->SetScanCond( new EdbScanCond(eCond) );
  evr.SetPVRec(pvr);
  return evr.Make1Vertex( tracks, eZ0 );
}


