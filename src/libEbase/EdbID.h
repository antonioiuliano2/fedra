#ifndef ROOT_EdbID
#define ROOT_EdbID
#include "TObject.h"

//---------------------------------------------------------------------------
class EdbID : public TObject
{
 public:

  Int_t eBrick;
  Int_t ePlate;
  Int_t eMajor;
  Int_t eMinor;

  EdbID(Int_t b=0, Int_t p=0, Int_t ma=0, Int_t mi=0) {Set(b, p, ma, mi);}
  EdbID(const char *id_string);
  EdbID(Int_t id[]) {Set(id);}
  virtual ~EdbID() {}

  bool operator==(const EdbID &id) { return eBrick==id.eBrick&&ePlate==id.ePlate&&eMajor==id.eMajor&&eMinor==id.eMinor; }
  bool operator!=(const EdbID &id) { return eBrick!=id.eBrick||ePlate!=id.ePlate||eMajor!=id.eMajor||eMinor!=id.eMinor; }
  bool  Set(const char *id_string);
  void  Set(Int_t b = 0, Int_t p = 0, Int_t ma = 0, Int_t mi = 0) 
        {eBrick = b; ePlate = p; eMajor = ma; eMinor = mi;}
  void  Set(Int_t id[]) {eBrick=id[0];ePlate=id[1];eMajor=id[2];eMinor=id[3];}
  void  Get(Int_t id[]) {id[0]=eBrick;id[1]=ePlate;id[2]=eMajor;id[3]=eMinor;}
  Int_t GetPlate() const {return ePlate;}
  char *AsString() const;

  void Print() { printf("%d %d %d %d\n",eBrick,ePlate,eMajor,eMinor); }
  ClassDef(EdbID,1)  // definition of the identifier of the data piece
};

#endif /* ROOT_EdbID */
