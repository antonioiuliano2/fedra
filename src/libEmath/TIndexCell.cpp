//-- Author :  Valeri Tioukov   4-05-2002

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TIndexCell                                                           //
//                                                                      //
// sort collection with attributes                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
/*
  Convension about levels of cell ic: 
  0 - value of ic
  1 - list of ic
  2 - sublists of list of ic...

  Example of usage:

*/

#include "TIndexCell.h"

ClassImp(TIndexCell)
ClassImp(TIndexCellIter)
ClassImp(TIndexCellIterV)

//____________________________________________________________________________
TIndexCell::TIndexCell(const TIndexCell &c)
{ 
  // Copy constructor - do real copy of all objects
  fValue = c.Value();
  if(c.List()) {
    int ncc = c.GetEntries();
    fList = new TObjArray(ncc);
    for(int i=0; i<ncc; i++)  {
      fList->Add(new TIndexCell( *(c.At(i))));
      fList->SetName((c.List())->GetName());
    }
  }
  else fList=0;
}

//____________________________________________________________________________
TIndexCell::~TIndexCell()
{ 
  Delete();
}

//____________________________________________________________________________
void TIndexCell::Delete()
{ 
  if(fList) {
    int ncc = GetEntries();
    for(int i=0; i<ncc; i++ )  {
      At(i)->Delete();
    }
    fList->Delete();
    delete fList;
    fList=0;
  }
}

//____________________________________________________________________________
void TIndexCell::Shift( Int_t level, Long_t vshift[] )
{ 
  // Shift al values of level i on the vshift[i]

  int ncc = GetEntries();
  if(level==1)  
    for(int i=0;i<ncc;i++) At(i)->Shift(vshift[0]);
  else if( level>1 ) {
    if(!fList)         return;
    for(int i=0; i<ncc; i++ )  {
      At(i)->Shift(vshift[0]); 
      At(i)->Shift(level-1,vshift+1);
    }
  }
}

//____________________________________________________________________________
Int_t TIndexCell::ComparePatterns( Int_t level, Long_t vdiff[], 
				    TIndexCell *cin, Int_t strip )
{ 
  // Compare 2 TIndexCells of the same structure and find number of common 
  // cells at the given level and if(strip): drop all the rest (for current pattern)
  //
  // vdiff - is the vector of values (levels) to be taken into account of 
  //         the length level
  // vdiff[i] =  0 - require value of i-th level being the same (exact comparison)
  //          >0   - abs(value2-value1) <= vdiff[i]

  Int_t npat=0;
  if(!cin)      return npat;
  if(!fList)    return npat;
  if(level<1)   return npat;
  Int_t entries = GetEntries();
  if(entries<1) return npat;

  Int_t npat0;
  TIndexCell *c1=0;
  TIndexCell *at=0;

  npat0=npat=0;

  for( int i=entries-1; i>-1; i-- ) {
    at = At(i);
    npat0 = npat;
    if( vdiff[0] == 0 ) {
      c1 = cin->Find(at->Value());
      if( c1 ) {
	if(level==1)     {npat++;}
	else if(level>1) {npat += at->ComparePatterns(level-1,vdiff+1,c1,strip);}
      }
    }
    else if(vdiff[0] > 0 ) {
      for( Long_t j=-vdiff[0]; j<=vdiff[0]; j++ ) {
	c1 = cin->Find( at->Value()+j );
	if( c1 ) {
	  if(level==1)     npat++;
	  else if(level>1) {
	    npat += at->ComparePatterns(level-1,vdiff+1,c1,0);
	  }
	}
      }
    }
    //printf("level=%d   i=%d, entries=%d, npat=%d, npat0=%d \n", 
    //  level,i,entries,npat,npat0 );
    if(strip) { 
      if(npat<=npat0) { fList->RemoveAt(i); at->Drop(); delete at; Sort(); }
    }
  }

  if(strip) {
    fList->Compress();
    Sort();
  }

  return npat;
}

//____________________________________________________________________________
Int_t TIndexCell::ComparePatterns( Int_t level, TIndexCell *cin, Int_t strip )
{ 
  Long_t *vdiff = new Long_t[level];
  for( int i=0; i<level; i++ ) vdiff[i]=0;
  Int_t cp =  ComparePatterns( level, vdiff, cin, strip );
  delete[] vdiff;
  return cp;
}

//____________________________________________________________________________
//  Int_t TIndexCell::ComparePatterns( Int_t level, TIndexCell *cin, Int_t strip )
//  { 
//    // Compare 2 TIndexCells of the same structure, find number of common 
//    // cells at the given level and if(strip): drop all the rest (for current pattern)

//    Int_t npat=0;
//    if(!cin)    return npat;
//    if(!fList)  return npat;
//    TIndexCell *at=0;
//    TIndexCell *c1 =0;
//    int entries = GetEntries();

//    for(int i=0;i<entries;i++) {
//      at = At(i);
//      c1 = cin->Find(at->Value());
//      if( c1 ) {
//        if(level==1)       npat++;
//        else if( level>1 ) npat += at->ComparePatterns(level-1,c1,strip);
//      }
//      else if(strip) { Drop(i); i--; entries--; }
//    }
//    return npat;
//  }

//____________________________________________________________________________
void TIndexCell::DropButFirst(int level)
{ 
  // drop all elements except of first one in all cells on the level

  if(!fList) return;
  int ncc=GetEntries();
  if(level==0) {
    if(ncc<2) return;
    for(int i=ncc-1; i>0; i--)  Drop(i);
  } else if(level>0) {
    for(int i=0; i<ncc; i++)  At(i)->DropButFirst(level-1);
  }
}

//____________________________________________________________________________
void TIndexCell::DropButLast(int level)
{ 
  // drop all elements except of last one in all cells on the level

  if(!fList) return;
  int ncc=GetEntries();
  if(level==0) {
    if(ncc<2) return;
    for( int i=ncc-2; i>=0; i-- )  Drop(i);
  } else if(level>0) {
    for(int i=0; i<ncc; i++)  At(i)->DropButLast(level-1);
  }
}

//____________________________________________________________________________
int TIndexCell::DropCouples(int level)
{ 
  // drop all overoccupated cells on the level
  int count=0;
  if(!fList) return count;
  int ncc=GetEntries();
  if(level==0) {
    for(int i=ncc-1; i>-1; i--)  
      if(ncc>1) { Drop(i); count++;}
  } else if(level>0) {
    for(int i=0; i<ncc; i++)  count+=At(i)->DropCouples(level-1);
  }
  return count;
}

//____________________________________________________________________________
void TIndexCell::Drop(int i)
{ 
  // drop i-th cell on the highest level

  if(!fList) return;
  TIndexCell *at = At(i);
  if(at) {
    at->Delete();
    fList->RemoveAt(i);
    delete at; 
    Sort();
  }
}

//____________________________________________________________________________
void TIndexCell::Drop()
{ 
  fValue=0;
  Delete();
}

//____________________________________________________________________________
void TIndexCell::PrintPopulation( int level ) const
{
  TIndexCellIter itr(this,level);
  const TIndexCell *c=0;
  TIndexCell *cn=0;
  TIndexCell hist;
  while( (c=itr.Next()) ) {
    cn = hist.FindAdd((Long_t)(c->N(1)));
    if(!cn->At(0)) cn->Add(0);
    cn->At(0)->SetValue(cn->At(0)->Value()+1); 
  }
  hist.Sort();
  hist.SetName("N:entries");
  printf("Population on the level %d\n",level);
  hist.Print("");
}

//____________________________________________________________________________
void TIndexCell::PrintStat() const
{ 
  printf( "Ncell:   %d\n", N() );
  printf( "Nlevels: %d   %s \n", Nlevels(), List()->GetName() );

  float phaseVol=1;
  int range=0;
  printf("level\t entries\t range\t occupancy\t minPop\t maxPop\n");
  int nlvl=Nlevels();
  for(int i=1; i<nlvl; i++ ) {
    range = MaxV(i)-MinV(i)+1;
    phaseVol *= range;
    printf(" %d \t %d \t\t %d \t %f \%% \t %d \t %d\n",
	   i,N(i),range,100.*N(i)/phaseVol, MinN(i),MaxN(i) );
  }
  printf(" %d \t %d \t\t %d \t %f \%%\n",
	 Nlevels(),N(Nlevels()),1,100.*N(Nlevels())/phaseVol );
}

//____________________________________________________________________________
void TIndexCell::Print(Option_t *opt ) const 
{ 
  //printf(" %s = ", (this->->List()->GetName());
  printf("%ld\n", fValue);

  if(!fList) return;
  int ncc=GetEntries();
  for(int i=0; i<ncc; i++) {
    printf(" %s = ",fList->GetName());
    ((TIndexCell*)fList->At(i))->Print("");
  }
}

//____________________________________________________________________________
void TIndexCell::Purge(int level)
{
  // Drop branches of length < level
  if( !fList )   return;
  if( level< 1 ) return;

  int entries = GetEntries();
  if( entries<1 ) Drop();
  for(int i=entries-1; i>-1; i--)  {
    //if( At(i)->Nlevels() < level-1 ) { At(i)->Drop();  fList->RemoveAt(i); }
    //??
    if( At(i)->Nlevels()+1 < level-1 ) { At(i)->Drop();  fList->RemoveAt(i); }
    else At(i)->Purge(level-1);
  }
  if(GetEntries()<1)   Drop();
  if(fList) {
    fList->Compress();
    Sort();
  }
}

//____________________________________________________________________________
Int_t TIndexCell::Nlevels() const
{
  // return maximal depth of nested arrays
  int n=0,k=0;
  if(!fList) return 0;
  else {
    int ncc = GetEntries();
    for(int i=0; i<ncc; i++)  {
      k = At(i)->Nlevels();
      if(k>n) n=k;
    }
  }
  return n+1;
}

//____________________________________________________________________________
Int_t TIndexCell::N() const
{
  // return total number of basic cells (last level where fList=0)
  int n=0;
  int ncc = GetEntries();
  if(!fList) n++;
  else for(int i=0; i<ncc; i++)  n += At(i)->N();
  return n;
}

//____________________________________________________________________________
Int_t TIndexCell::N( int level ) const
{
  // return total number of cells (nodes) on given level
  int n=0;
  if(!fList)         return 0;
  int ncc = GetEntries();
  if(level==1)       return ncc;
  else if(level>1) {
    for(int i=0; i<ncc; i++)  n += At(i)->N(level-1);
  }
  return n;
}

//____________________________________________________________________________
Long_t TIndexCell::MinV( Int_t level ) const
{
  // return mimimal value on the given level
  Int_t *vind = new Int_t[level];
  Long_t min = MinV(level,vind);
  delete[] vind;
  return min;
}

//____________________________________________________________________________
Long_t TIndexCell::MinV( Int_t level, Int_t vind[] ) const
{
  // return mimimal value on the given level and vector of indexes 

  if(level==0)      return fValue;
  Long_t   n=kMaxInt, k=kMaxInt;
  if(fList) {
    if(level>0) {
      int ncc = GetEntries();
      for(int i=0; i<ncc; i++) {
	k=At(i)->MinV(level-1,vind+1);
	if(n > k) {
	  n=k;
	  vind[0] = i;
	}
      }
    }
  }
  return n;
}

//____________________________________________________________________________
Long_t TIndexCell::MaxV( Int_t level  ) const
{
  // return maximal value on the given level
  Int_t *vind = new Int_t[level];
  Long_t max = MaxV(level,vind);
  delete[] vind;
  return max;
}

//____________________________________________________________________________
Long_t TIndexCell::MaxV( Int_t level, Int_t vind[]  ) const
{
  // return maximal value on the given level and cell address

  if(level==0)      return fValue;
  Long_t   n=-kMaxInt, k=-kMaxInt;
  if(fList) {
    if(level>0) {
      int ncc = GetEntries();
      for(int i=0; i<ncc; i++) {
	k=At(i)->MaxV(level-1,vind+1);
	if(n < k) {
	  n=k;
	  vind[0]=i;
	}
      }
    }
  }
  return n;
}

//____________________________________________________________________________
Int_t TIndexCell::MinN( Int_t level ) const
{
  // return population of the mimimal populated cell on the given level
  Int_t *vind = new Int_t[level];
  Int_t min = MinN(level,vind);
  delete[] vind;
  return min;
}

//____________________________________________________________________________
Int_t TIndexCell::MinN( Int_t level, Int_t vind[] ) const
{
  // return population of the mimimal populated cell on the given level
  // and cell address

  //TODO: check this routine

  if(level==0)  
    if(fList) return GetEntries();
  Int_t n=kMaxInt, k=kMaxInt;
  if(fList) {
    if(level>0) {
      int nentr=GetEntries();
      for(int i=0; i<nentr; i++)  {
	k=At(i)->MinN(level-1,vind+1);
	if(n > k) {
	  n=k;
	  vind[0]=i;
	}
      }
    }
  }
  return n;
}

//____________________________________________________________________________
Int_t TIndexCell::MaxN( Int_t level ) const
{
  // return population of the maximal populated cell on the given level
  Int_t *vind = new Int_t[level];
  Int_t max = MaxN(level,vind);
  delete[] vind;
  return max;
}

//____________________________________________________________________________
Int_t TIndexCell::MaxN( Int_t level, Int_t vind[] ) const
{
  // return population of the maximal populated cell on the given level 
  // and cell address

  //TODO: check this routine

  if(level==0)
    if(fList) return GetEntries();
  Int_t n=-999999999, k=-999999999;
  if(fList) {
    if(level>0) {
      int nentr=GetEntries();
      for(int i=0; i<nentr; i++)  {
	k=At(i)->MaxN(level-1,vind+1);
	if(n < k) {
	  n=k;
	  vind[0] = i;
	}
      }
    }
  }
  return n;
}

//____________________________________________________________________________
Int_t TIndexCell::GetValues( Int_t nind, Int_t vind[], Long_t val[] ) const
{
  // return vector of values correspondent to indexes on the given level

  TIndexCell const *b;
  for( int lev=1; lev<=nind; lev++ ) {
    b=At(lev,vind);
    if(!b) return 0;
    val[lev-1]=b->Value();
  }
  return 1;
}

//____________________________________________________________________________
TIndexCell const *TIndexCell::At( Int_t nind, Int_t vind[] ) const
{
  // return cell located at vind[narg] address

  TIndexCell *b=0;
  //  TIndexCell *th=this;

  if(nind>0) {
    b = At(vind[0]);
    if(nind==1) return b;
    if(b) {
      nind--;
      return b->At(nind,vind+1);
    }
  }
  else if(nind==0)  return this;
  return 0;
}

//____________________________________________________________________________
void TIndexCell::Sort(Int_t upto) 
{
  if(fList) {
    fList->Sort();
    int ncc = GetEntries();
    for(int i=0; i<ncc; i++) At(i)->Sort();
  }
}

//____________________________________________________________________________
void TIndexCell::SetName(const char *varlist)
{
  if(!fList) return;

  fList->SetName(varlist);

  Int_t nch = strlen(varlist);
  if (nch <= 1) return;

  const char *newvar=0;
  int i;
  for (i=1;i<nch;i++) {
    if (varlist[i] == ':') {
      if(i+1<nch)  newvar=&varlist[i+1];
      break;
    }
  }

  if(!newvar) return;

  int ncc = GetEntries();
  for(i=0; i<ncc; i++) 
    ((TIndexCell*)fList->At(i))->SetName(newvar);
}

//____________________________________________________________________________
TIndexCell *TIndexCell::FindAdd( Long_t p1 )
{
  TIndexCell *b=0;

  if(!fList)   fList = new TObjArray();
  else {
    b = Find(p1);
    if( b )                 return b;
  }
  b= new TIndexCell(p1);
  fList->Add(b);
  return b;
}

//____________________________________________________________________________
Int_t TIndexCell::Add( Long_t p1 )
{
  if(!fList)     fList = new TObjArray();
  else if( Find(p1) )
      printf("WARNING: TIndexCell::Add(%ld) - object is not unique!\n",p1);

  TIndexCell *b= new TIndexCell(p1);
  fList->Add(b);
  return 1;
}

//____________________________________________________________________________
Int_t TIndexCell::Add( Int_t narg, Long_t varg[] )
{
  if(narg==1) return Add( varg[0] );
  else {
    TIndexCell *b=FindAdd(varg[0]);
    narg--;
    return b->Add( narg, varg+1 );
  }
}

//____________________________________________________________________________
TIndexCell *TIndexCell::Find( Int_t narg, Long_t varg[] ) const
{
  if(narg==1) return Find( varg[0] );
  else {
    TIndexCell *b=Find(varg[0]);
    if(b) {
      narg--;
      return b->Find( narg, varg+1 );
    }
  }
  return 0;
}

//____________________________________________________________________________
TIndexCell *TIndexCell::Find( Long_t p1 ) const
{
  if(!fList) return 0;
  TIndexCell a(p1);
  if(!fList->IsSorted())
    return (TIndexCell*)(fList->FindObject(&a));
  Int_t i = fList->BinarySearch(&a);
  if(i>=0) return At(i);
  return 0;
}

//______________________________________________________________________________
TIndexCellIter::TIndexCellIter(const TIndexCell *cell, int level, Bool_t dir)
{
   // Create cell iterator. By default the iteration direction
   // is kIterForward. To go backward use kIterBackward.

   fPass = 0;
   fCell     = cell;
   fDirection = dir;
   if( level > fCell->Nlevels() ) level=0;
   if( level < 1 ) level=0;
   fLevel = fLevel0 = level-1;
   if(fLevel0 >= 0) {
     fVind = new int[ fLevel0+1 ];
     Reset();
   }
}

//______________________________________________________________________________
TIndexCell const *TIndexCellIter::Next()
{
   // Return next object in cell. Returns 0 when no more objects in cell.

  if( fLevel0 <0 )   return 0;

 START:

  if( fLevel != fLevel0 )   return 0;

  int nent = fCell->At(fLevel,fVind)->GetEntries();
  //printf("Next: nent= %d  fPass= %d\n",nent,fPass);

  if( fVind[fLevel] < nent-1 ) {
    fVind[fLevel]++;
    fPass++;
    return  fCell->At(fLevel+1,fVind);
  } else {
    if( LevelDown() ) goto START;
  }
  return 0;
}

//______________________________________________________________________________
int TIndexCellIter::LevelDown()
{
  if(fLevel<1) return 0;
  fVind[fLevel] = 0;
  fLevel--;
  int nent = fCell->At(fLevel,fVind)->GetEntries();
  //printf("LevelDown: level = %d, nent= %d  fVind= %d\n",fLevel, nent,fVind[fLevel]);
  if( fVind[fLevel] < nent-1 ) {
    fVind[fLevel]++;
    fLevel=fLevel0;
    fVind[fLevel0]=-1;
    return 1;
  } else return LevelDown();
  return 0;
}

//______________________________________________________________________________
void TIndexCellIter::Reset()
{
   // Reset array iterator.

  if (fDirection == kIterForward) {
    fLevel = fLevel0;
    for(int i=0; i<fLevel0; i++ ) fVind[i]=0;
    fVind[fLevel0]=-1;
  }
  else    {}
}

//______________________________________________________________________________
void TIndexCellIter::Test()
{
  int count=0;
  while( Next() ) count++;
  printf("count = %d\n",count);
}


//================================================================================

//______________________________________________________________________________
TIndexCellIterV::TIndexCellIterV( const TIndexCell *cell, 
				  int level, 
				  long vcent[],
				  long vdiff[],
				  Bool_t dir )
{
   // Create cell iterator. By default the iteration direction
   // is kIterForward. To go backward use kIterBackward.

   fPass = 0;
   fCell     = cell;
   fDirection = dir;
   if( level > fCell->Nlevels() ) level=0;
   if( level < 1 ) level=0;
   fLevel = fLevel0 = level;

   if(fLevel0 >= 0) {
     fVind  = new Int_t[  fLevel0 ];
     fVval  = new Long_t[ fLevel0 ];
     Reset();
   }

   fVcent = new Long_t[level];
   fVdiff = new Long_t[level];

   for(int i=0; i<level; i++) {
     fVcent[i] = vcent[i];
     fVdiff[i] = vdiff[i];
   }
}

//______________________________________________________________________________
void TIndexCellIterV::Reset()
{
   // Reset array iterator.
  if (fDirection == kIterForward) {
    fLevel = fLevel0;
    for(int i=0; i<fLevel0; i++ ) fVind[i]=0;
    fVind[fLevel0-1]=-1;
  }
  else    {}
}

//______________________________________________________________________________
TIndexCell const *TIndexCellIterV::Next()
{
  // Return next object in the defined volume. 
  // Returns 0 when no more objects in the cell.

  // TODO: inefficient cycle ! put here find...

  TIndexCell const *b=0;

  while( (b=NextAll()) ) {
    fCell->GetValues( fLevel0, fVind, fVval );
    //printf("flevel0: %d  fVind[0]: %d, fVval[1]: %ld \n",fLevel0,  fVind[0], fVval[1]);
    for(int lev=0; lev<fLevel0; lev++) {
      if( TMath::Abs( fVval[lev]-fVcent[lev] ) > fVdiff[lev] ) break;
      if( lev == fLevel0-1 ) return b;
    }
  }
  return 0;
}

//______________________________________________________________________________
TIndexCell const *TIndexCellIterV::NextAll()
{
   // Return next object in cell. Returns 0 when no more objects in cell.

  if( fLevel0 <0 )   return 0;

 START:

  if( fLevel != fLevel0 )   return 0;

  int nent = fCell->At(fLevel-1,fVind)->GetEntries();
  //printf("Next: nent= %d  fPass= %d\n",nent,fPass);

  if( fVind[fLevel-1] < nent-1 ) {
    fVind[fLevel-1]++;
    fPass++;
    return  fCell->At(fLevel,fVind);
  } else {
    if( LevelDown() ) goto START;
  }
  return 0;
}

//______________________________________________________________________________
int TIndexCellIterV::LevelDown()
{
  if(fLevel<2) return 0;
  fLevel--;
  fVind[fLevel] = 0;
  int nent = fCell->At(fLevel-1,fVind)->GetEntries();
  //printf("LevelDown: level = %d, nent= %d  fVind= %d\n",fLevel, nent,fVind[fLevel-1]);
  if( fVind[fLevel-1] < nent-1 ) {
    fVind[fLevel-1]++;
    fLevel=fLevel0;
    fVind[fLevel0-1]=-1;
    return 1;
  } else return LevelDown();
  return 0;
}

//______________________________________________________________________________
void TIndexCellIterV::Test()
{
  int count=0;
  while( NextAll() ) count++;
  printf("count = %d\n",count);
}
