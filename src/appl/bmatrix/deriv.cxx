
#include "deriv.h"
#include "Riostream.h"


ClassImp(Deriv);

//______________________________________________________________________

  float Deriv::derivate(EdbFrame *fpred,EdbFrame *f,EdbFrame *fsuc) {

	float d;
	int ncl_pred, ncl, ncl_suc;
	float z_pred, z, z_suc;
	z = f->GetZ();
	ncl = f->GetNcl();
	if (fpred==NULL) {
		ncl_suc = fsuc->GetNcl();
		z_suc = f->GetZ();
		if (z_suc==z) d=0.;
		else d = (ncl_suc-ncl)/(z_suc-z);
	}
	else if (fsuc==NULL) {
		ncl_pred = fpred->GetNcl();
		z_pred = fpred->GetZ();
		if (z_pred==z) d=0.;
		else d = (ncl-ncl_pred)/(z-z_pred);
	}
	else {
		ncl_suc = fsuc->GetNcl();
		ncl_pred = fpred->GetNcl();
		z_suc = fsuc->GetZ();
		z_pred = fpred->GetZ();
		if (z_suc==z_pred) d=0.;
		else d = (ncl_suc-ncl_pred)/(z_suc-z_pred);
	}
	eDp = d;
	eZp = z;
	return eDp;
}

//_______________________________________________________________________

