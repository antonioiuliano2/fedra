#ifndef _BITVIEW_H_
#define _BITVIEW_H_

#include "TObject.h"
#include "TObjArray.h"
#include "bitmatrix.h"
#include "basematrix.h"
#include "bytematrix.h"
#include "EdbView.h"
#include "EdbCluster.h"
#include "EdbFrame.h"
#include "TH2.h"

class track : public TObject {
public:
	double tgx;
	double tgy;
	double x;
	double y;
	double z;
	int height;
	track();
	track(double tx, double ty, double xx, double yy, double zz, int h);
	virtual ~track();
};

class TBitView : public TObject {
protected:
	TObjArray* TBitArray;
//	int NumberOfLayers;
	float Z1,Z2,Z3,Z4;
	float delta;
public:
	float ZBase;
	float bitSize;						//size of bit in microns
	TBitView();
	TBitView(float z, float bs);
	virtual ~TBitView();
	float GetZ1(void) {return Z1;}
	float GetZ2(void) {return Z2;}
	float GetZ3(void) {return Z3;}
	float GetZ4(void) {return Z4;}
	void AddNewLayer(TBitMatrix* TLayer);
	TBitMatrix* GetLayer(int n);

	TBitMatrix* operator[](int n) {return GetLayer(n);}
	int GetNumberOfLayers(void) {return TBitArray->GetEntries();}
//	void SetZ(float z) {ZBase = z;}
//	float GetZBase(void) {return ZBase;}
//	void SetBitSize(float bs) {bitSize = bs;}
//	float GetBitSize(void) {return bitSize;}

	void FillBitView(EdbView* view, float sx, float sy, float Xmin, float Xmax, float Ymin, float Ymax,int inside = 1, int rep = 1);	//in View coordinates
	// s - conversion factor mkm/pix for x & y
	// inside - 1 counting only INSIDE emulsion clusters; - 0 counting all clusters, not depence its base relative position  
	// rep - take to account each "rep" frame. NOT 0!

	ClassDef(TBitView,1)
};

//________________________________________________________________________________
class TBinTracking : public TObject {
protected:
	TObjArray* TByteArray;
//	TObjArray* TTrackArray;
//	int NumberOfElements;
public:
	double Tgxmin, Tgxmax, Tgymin, Tgymax, Tgstep;
	float ZBase;
/////////// added at 10.11.2004 ///////////
	double dX, dY, dZ;

	TBinTracking();
	TBinTracking(double tgxmin, double tgxmax, double tgymin, double tgymax, double tgstep);
	TBinTracking(double tgxmin, double tgxmax, double tgymin, double tgymax, double tgstep, double dx, double dy, double dz);
	virtual ~TBinTracking();
	TByteMatrix* GetElement(int n);
	TByteMatrix* GetElement(double tgx, double tgy);
	TByteMatrix* operator[](int n) {return GetElement(n);}
	int GetNelements(void) {return TByteArray->GetEntries();}
//	void SetZ(float z) {ZBase = z);
	void FillByteArray(TBitView *Tv);
	TObjArray* FindBinTracks(int thr);				// Array of tracks !!!
	void ImproveTracksArray(TObjArray *tracks, int thr, float accept);		// Improve Array of tracks	(now testing)
	EdbView* AdoptSegment(EdbView* view, float sx, float sy, float Xmin, float Xmax, float Ymin, float Ymax, float bs,
		int thr, float accept, int inside = 1, int excludeCommonClusters = 0, int rep = 1);
	// view with segments !!!!!!!!
	// s - conversion factor in mkm/pix for x & y
	// Xmin - Xmax; Ymin - Ymax : window
	// bs - binSize (mkm per bin)
	// thr - threshold (min number of clusters belong segment)
	// accept - mkm max spread of clusters around segment
	// inside - 1 counting only INSIDE emulsion clusters; - 0 counting all clusters, not depence its base relative position
	// excludeCommonClusters - 1 if segments have common clusters, one of them will be excluded.
	// rep - take to account each "rep" frame. NOT 0!
	TH2F* Histo(double tgx, double tgy);

	ClassDef(TBinTracking,1)

};

//________________________________________________________________________________
#endif
