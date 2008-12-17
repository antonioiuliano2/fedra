#ifndef ROOT_EdbScanSet
#define ROOT_EdbScanSet

#include "TList.h"
#include "TIndexCell.h"
#include "EdbBrick.h"

//---------------------------------------------------------------------------
class EdbID : public TObject
{
 public:

  Int_t eBrick;
  Int_t ePlate;
  Int_t eMajor;
  Int_t eMinor;

  EdbID(Int_t b=0, Int_t p=0, Int_t ma=0, Int_t mi=0) {Set(b, p, ma, mi);}
  EdbID(Int_t id[]) {Set(id);}
  virtual ~EdbID() {}

  void  Set(Int_t b = 0, Int_t p = 0, Int_t ma = 0, Int_t mi = 0) 
        {eBrick = b; ePlate = p; eMajor = ma; eMinor = mi;}
  void  Set(Int_t id[]) {eBrick=id[0];ePlate=id[1];eMajor=id[2];eMinor=id[3];}
  void  Get(Int_t id[]) {id[0]=eBrick;id[1]=ePlate;id[2]=eMajor;id[3]=eMinor;}
  Int_t GetPlate() const {return ePlate;}

  void Print() { printf("%d %d %d %d\n",eBrick,ePlate,eMajor,eMinor); }
  ClassDef(EdbID,1)  // definition of the identifier of the data piece
};

//---------------------------------------------------------------------------
class EdbScanSet : public TNamed
{
 public:

  EdbBrickP  eB;       // all layers of the brick defined here
  TIndexCell ePID;     // correspondance between index in eB and the plate id
  TObjArray  ePC;      // Plate Couples. Each couple represented as EdbPlateP 
                       // where 2 sides in reality corresponds to 2 plates 
  TList      eIDS;     // list of the identifiers to be processed

 public:

  EdbScanSet();
  virtual ~EdbScanSet(){}

  void     MakePIDList();
  void     MakeNominalSet(Int_t from_plate, Int_t to_plate, 
			  Float_t z0, Float_t dz, Int_t vma, Int_t vmi, float shr=1);
  void     Print();
  Int_t    AssembleBrickFromPC();
  Int_t    TransformBrick(EdbAffine2D aff);
  Int_t    ShiftBrickZ(Float_t z);
  Int_t    MakeParFiles(Int_t piece=0, const char *dir=".");
  Int_t    ReadIDS(const char *file);
  Int_t    WriteIDS(const char *file=0);
  Bool_t   AddID(EdbID *id, Int_t step);
  Bool_t   SetAsReferencePlate(Int_t pid);
  Bool_t   GetAffP2P(Int_t p1, Int_t p2, EdbAffine2D &aff);
  Float_t  GetDZP2P(Int_t p1, Int_t p2);
  EdbID   *FindPlateID(Int_t p);
  EdbID   *GetID(Int_t i) {return (EdbID *)(eIDS.At(i));}

  const EdbBrickP& Brick()   const {return eB;}
  EdbBrickP&       Brick()         {return eB;}

  EdbPlateP *GetPlate(Int_t p) {
    if (ePID.Find(p)) 
      return eB.GetPlate(ePID.Find(p)->At(0)->Value()); 
    else return 0; 
  }

  ClassDef(EdbScanSet,1)  // 
};

#endif /* ROOT_EdbScanSet */
