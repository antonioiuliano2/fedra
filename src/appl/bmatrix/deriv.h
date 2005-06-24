#ifndef _DERIV_H
#define _DERIV_H

#include "TObject.h"
#include "EdbFrame.h"

class Deriv : public TObject {

  private:
	float eZp;
	float eDp;
	

  public:
	Deriv() {}
//	Deriv(Deriv *d) {eZp=d->eZp;eDp=d->eDp;}
	virtual ~Deriv() {}

	float derivate(EdbFrame *fpred,EdbFrame *f,EdbFrame *fsuc);
	void GetDerivate(float *Z, float *D) {*Z = eZp; *D = eDp;}
	float GetZ() {return eZp;}
	float GetD() {return eDp;}

	ClassDef(Deriv,1)
};

#endif
