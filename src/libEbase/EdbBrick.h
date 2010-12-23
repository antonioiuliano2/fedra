#ifndef ROOT_EdbBrick
#define ROOT_EdbBrick

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbBrick - OPERA Brick structure definition                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TObjArray.h"
#include "EdbLayer.h"

//______________________________________________________________________________
class EdbPlateP : public EdbLayer {

 private:
  TObjArray eLayers;      // 0-base, 1-up, 2-down

 public:
  EdbPlateP();
  EdbPlateP(EdbPlateP &pp): EdbLayer(pp) { for(int i=0; i<3; i++) eLayers.Add(new EdbLayer(*(pp.GetLayer(i)))); }

  ~EdbPlateP(){eLayers.Delete();}

  void      Copy(EdbPlateP &p);
  void      SetPlateLayout(float z0, float z1, float z2);
  void      SetOperaLayout() { SetPlateLayout(210,44,44); }
  void      PrintPlateLayout();
  EdbLayer *GetLayer(int i) {return (i < 0) ? 0 : (EdbLayer*)eLayers.At(i);}
  void      SetDXDY(float dx, float dy);
  void      TransformSidesIntoPlateRS();
  void      Print();

  ClassDef(EdbPlateP,1)  // OPERA Plate structure definition
};

//______________________________________________________________________________
class EdbBrickP : public EdbLayer {

 private:
  TObjArray ePlates;
  TObjArray eSpacers;

 public:
  EdbBrickP();
  ~EdbBrickP(){ePlates.Delete(); eSpacers.Delete();}

  void Copy(EdbBrickP &b);
  void SetPlatesLayout(float z0, float z1, float z2);
  void SetDXDY(float dx, float dy);
  int Npl() const {return ePlates.GetEntries();}
  void AddPlate(EdbPlateP *pl) { ePlates.Add(pl); }
  EdbPlateP *GetPlate(int i) {return (EdbPlateP*)ePlates.At(i);}
  void Clear() { ePlates.Clear(); eSpacers.Clear(); }
  void Print();
  void PrintSides();
  void RemovePlate(int pid);
  void ResetAffXY();
  void ResetAffTXTY();
  void ResetAff() {ResetAffXY(); ResetAffTXTY();}

  ClassDef(EdbBrickP,1)  // OPERA Brick structure definition
};


#endif /* ROOT_EdbBrick */

