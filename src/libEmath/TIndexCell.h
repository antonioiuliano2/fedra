#ifndef ROOT_TIndexCell
#define ROOT_TIndexCell

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TIndexCell                                                           //
//                                                                      //
//  sort collection with attributes                                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TString.h"
#include "TObjArray.h"



//__________________________________________________________________________
class TIndexCell : public TObject { 

  friend class TIndexCellIter;
  friend class TIndexCellIterV;

 private:

  Long_t       fValue;  // hash value

  TObjArray *fList;   // list of TIndexCell's

 public:
  TIndexCell():fValue(0) { fList=0; }
  TIndexCell(Long_t p1):fValue(p1) { fList=0; }
  TIndexCell(const TIndexCell &c);
  ~TIndexCell();

  Int_t  ComparePatterns( Int_t nind, TIndexCell *cin, Int_t srtip=0 );
  Int_t  ComparePatterns( Int_t nind, Long_t vdiff[], TIndexCell *cin, Int_t strip=0 );

  void   Shift( Long_t vshift ) { SetValue( Value()+vshift ); }
  void   Shift( Int_t n, Long_t vshift[] );

  void   Delete();

  void   Drop();
  void   Drop(int i);
  int    DropCouples(int level);

  //void   DropFirst( int level );
  //void   DropLast(  int level );
  void   DropButFirst( int level );
  void   DropButLast(  int level );

  Int_t  Nlevels() const;
  Int_t  N() const;
  Int_t  N(Int_t level) const;

  Int_t  MinN(Int_t level, Int_t vind[]) const;
  Int_t  MaxN(Int_t level, Int_t vind[]) const;
  Long_t MinV(Int_t level, Int_t vind[]) const;
  Long_t MaxV(Int_t level, Int_t vind[]) const;

  Int_t  MinN(Int_t level) const;
  Int_t  MaxN(Int_t level) const;
  Long_t MinV(Int_t level) const;
  Long_t MaxV(Int_t level) const;

  Int_t Add( Int_t narg, Long_t varg[] );
  Int_t Add( Long_t p1 );

  TIndexCell *FindAdd( Long_t p1 );
  TIndexCell *Find( Int_t narg, Long_t varg[] ) const;
  TIndexCell *Find( Long_t p1 ) const;
  TIndexCell const *At( Int_t narg, Int_t vind[] ) const;
  TIndexCell *At( Int_t idx) const
  { if(fList) return (TIndexCell*)(fList->At(idx));
      else return 0; }

  TObjArray *List() const { return fList; }
  Long_t    Value() const { return fValue; }
  Int_t     GetValues(Int_t level, Int_t vind[], Long_t val[] ) const;

  Int_t      GetEntries() const 
    { if(fList) return fList->GetEntriesFast();
      else return 0; }

  void   SetName(const char *varlist);
  void   SetValue( Long_t p1) { fValue=p1; }
  void   Print(Option_t *) const;
  void   PrintStat()       const;
  void   PrintPopulation(int level)       const;
  void   Sort(Int_t upto = kMaxInt);
  void   Purge(int level);

  ULong_t Hash() const { return fValue; }
  Bool_t  IsEqual(const TObject *obj) const 
    { return fValue == ((TIndexCell*)obj)->Value(); }
  Bool_t  IsSortable() const { return kTRUE; }
  Int_t   Compare(const TObject *obj) const 
    { if (fValue > ((TIndexCell*)obj)->Value())
    return 1;
  else if (fValue < ((TIndexCell*)obj)->Value())
    return -1;
  else
    return 0; }

  ClassDef(TIndexCell,1)  // sort collection with attributes
};

//__________________________________________________________________________
class TIndexCellIter : public TObject { 

private:

   const TIndexCell  *fCell;     //cell being iterated
   Int_t             *fVind;     //current position in array
   Int_t              fLevel0;   //level to be iterated on
   Int_t              fLevel;    //current level
   Int_t              fPass;     //number of cells iterated
   Bool_t            fDirection; //iteration direction

   TIndexCellIter() : fCell(0) { }

public:
   TIndexCellIter(const TIndexCell *cell, 
		  int level, 
		  Bool_t dir = kIterForward);
   //TIndexCellIter(const TIndexCellIter &iter);
   ~TIndexCellIter();
   //TIterator     &operator=(const TIterator &rhs);
   //TObjArrayIter &operator=(const TObjArrayIter &rhs);

   const TIndexCell *GetCell() const { return fCell; }
   TIndexCell  const     *Next();
   void              Reset();
   int               LevelDown();
   void Test();

   ClassDef(TIndexCellIter,0)  // IndexCell iterator
};

//__________________________________________________________________________
class TIndexCellIterV : public TObject { 

private:

   const TIndexCell  *fCell;      // cell being iterated
   Int_t              fLevel0;    // level to be iterated on
   Long_t            *fVcent;     // central subcell
   Long_t            *fVdiff;     // volume to be iterated around Vcent

   Int_t             *fVind;     //current position in array
   Long_t            *fVval;     //values corresponding to Vind
   Int_t              fLevel;    //current level
   Int_t              fPass;     //number of cells iterated
   Bool_t            fDirection; //iteration direction

   TIndexCellIterV() : fCell(0) { }

public:

   TIndexCellIterV(const TIndexCell *cell, 
		   int level, 
		   long vcent[],
		   long vdiff[],
		   Bool_t dir = kIterForward);

   ~TIndexCellIterV() { }

   const TIndexCell *GetCell() const { return fCell; }
   TIndexCell  const     *Next();
   TIndexCell  const     *NextAll();
   void              Reset();
   int               LevelDown();
   void Test();

   ClassDef(TIndexCellIterV,0)  // IndexCell volume iterator
};

#endif /* TIndexCell */
