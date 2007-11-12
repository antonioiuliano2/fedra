#ifndef ROOT_EdbScanSet
#define ROOT_EdbScanSet

#include "TList.h"
#include "TIndexCell.h"
#include "EdbBrick.h"

//---------------------------------------------------------------------------
class EdbID : public TObject
{
 public:
  int eBrick;
  int ePlate;
  int eMajor;
  int eMinor;

  EdbID() { Set0(); }
  EdbID(int b, int p, int ma, int mi) { Set(b,p,ma,mi); }
  EdbID(int id[4]) { Set(id); }
  virtual ~EdbID() {}

  void Set0() { eBrick=0; ePlate=0; eMajor=0; eMinor=0; }
  void Set(int b, int p, int ma, int mi) { eBrick=b; ePlate=p; eMajor=ma; eMinor=mi; }
  void Set(int id[4]) { eBrick=id[0]; ePlate=id[1]; eMajor=id[2]; eMinor=id[3]; }
  void Get(int id[4]) { id[0]=eBrick; id[1]=ePlate; id[2]=eMajor; id[3]=eMinor; }

  ClassDef(EdbID,1)  // definition of the identifier of the data piece
};

//---------------------------------------------------------------------------
class EdbScanSet : public TNamed
{
 public:
  EdbBrickP eB;        // all layers of the brick defined here
  TIndexCell ePID;     // correspondance between index in eB and the plate id

  TObjArray ePC;       // Plate Couples. Each couple represented as EdbPlateP 
                       // where 2 sides in reality corresponds to 2 plates 

  TList eIDS;          // list of the identifiers to be processed

 public:
  EdbScanSet();
  virtual ~EdbScanSet(){}

  int  AssembleBrickFromPC();
  bool SetAsReferencePlate(int pid);
  int  TransformBrick(EdbAffine2D aff);
  int  ShiftBrickZ(float z);

  float GetDZP2P(int p1, int p2);
  bool GetAffP2P(int p1, int p2, EdbAffine2D &aff);
  EdbPlateP *GetPlate(int p) {if(ePID.Find(p)) return eB.GetPlate(ePID.Find(p)->At(0)->Value()); else return 0; }

  int MakeParFiles(int piece=0, const char *dir=".");

  int    ReadIDS(const char *file);
  int    WriteIDS(const char *file=0);

  void   Print();

  ClassDef(EdbScanSet,1)  // 
};

#endif /* ROOT_EdbScanSet */
