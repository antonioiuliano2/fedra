
#ifndef _BASEMATRIX_H_
#define _BASEMATRIX_H_

#include "TObject.h"
#include "TObjArray.h"

//========================================================================
class MyPoint : public TObject {
public:
	long x;
	long y;
	int height;
	MyPoint(long xx, long yy, int h);
	virtual ~MyPoint();
};

//=================================================================

class TBaseMatrix : public TObject {
protected:
	long xSize;						// dimension in BIN!
	long ySize;
	long byteSize;					// size in Byte!

//	long cxSize;						// dimension in BIN!
//	long cySize;
//	long cbyteSize;					// size in Byte!

	unsigned char *mas;				//!
//	unsigned char *cmas;

	const unsigned char *GetPointer(void) {return mas;}
//	const unsigned char *GetcPointer(void) {return cmas;}

	long shiftX;
	long shiftY;

//	TObjArray *NonZeroPoints;		// Array of MyPoints - non zero bit/byte
//	long position;					// Current position of this Array;

//	const TObjArray *GetP(void) {return NonZeroPoints;}

public:
	TBaseMatrix();
	TBaseMatrix(long ix_size, long iy_size);
	TBaseMatrix(TBaseMatrix& iT);
	virtual ~TBaseMatrix();
	void Init(long ix_size, long iy_size);
	void ResetShift(void);
	void Adopt(unsigned char* mas, long rows, long columns);
//	int GetBit(long i, long j);			//i - x, j - y
//	void SetBit(long i, long j);
//	void ClearBit(long i, long j);
	long GetSize(long *ix_size = NULL, long *iy_size = NULL);
//	long GetiSize(void) {return ibyteSize;}
//	long GetcSize(long *cx_size = NULL, long *cy_size = NULL);
//	long GetcSize(void) {return cbyteSize;}
	
///////////////////////////////// 0 - init, 1 - shifted!!!

	virtual int GetElement(int type, long i, long j) = 0;
//	virtual int GetcElement(long i, long j) = 0;

	void Print(int type);
//	void cPrint(void);
/*	void MoveMatrix2Left_Up(long up, long left);
	void MoveMatrix2Right_Down(long d, long r);
	void MoveMatrix2Right_Up(long up, long r);
	void MoveMatrix2Left_Down(long d, long left);
*/	
	void MoveMatrix(long ud, long lr);		//down +;r +

	TBaseMatrix& operator=(TBaseMatrix& T);

//	int FirstPoint(int type, long *i, long *j);
//	int NextPoint(int type, long *i, long *j);



	ClassDef(TBaseMatrix,1)
};

#endif
