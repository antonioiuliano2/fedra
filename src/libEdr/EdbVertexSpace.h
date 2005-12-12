#ifndef ROOT_EdbVertexSpace
#define ROOT_EdbVertexSpace

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbVertexSpace                                                       //
//                                                                      //
// Class for fast vertex check                                          //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TObject.h>

//_________________________________________________________________________
class EdbVertexSpace: public TObject {

 private:

  // Volume limits:
  Float_t eVmin[3];  // x,y,z of min point
  Float_t eVmax[3];  // x,y,z of max point

  Float_t eBin[3];   // xyz bin sizes

  Int_t   eNcell[3]; // number of cells in each dimension

  TObjArray *eCells;

 public:
  EdbVertexSpace( float vmin[3], float vmax[3], float bin[3] );
  virtual ~EdbVertexSpace() { if(eCells) delete eCells; }

  void AddTrackLine( float x1[3], float x2[3], int id);

  void Cycle();

  ClassDef(EdbVertexSpace,1)  // vertex-track association
};
#endif /* ROOT_EdbVertexSpace */
