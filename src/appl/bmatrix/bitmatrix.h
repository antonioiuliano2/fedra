// CHANGED!!! EACH ELEMENT NOW BYTE!!!!!!!!!!!! BUT INTERPRETED AS BIT!!!!!!
// NOW IT'S OBSOLETE!!!!!!!!
//16.06.2004 now these version is correct!


#ifndef _BITMATRIX_H_
#define _BITMATRIX_H_

#include "TObject.h"
#include "basematrix.h"
//#include "bytematrix.h"

class TBitMatrix : public TBaseMatrix {
/*private:
	long xSize;						// dimension in BIN!
	long ySize;
	long byteSize;					// size in Byte!
	unsigned char *mas;
*/
protected:
	float Z;
public:
	TBitMatrix();
	TBitMatrix(long x_size, long y_size);
	TBitMatrix(long x_size, long y_size, float z);
	TBitMatrix(TBaseMatrix& T);
	virtual ~TBitMatrix();
//	void Init(long x_size, long y_size);
	int GetBit(int type, long i, long j);			//i - x, j - y	type=0 - init mas 1 - shifted mas allways!
//	int GetBit(int type, long i, long j);			//i - x, j - y
	void SetBit(long i, long j);
	void SetBit(long i, long j, int area);	//area in bins!!!!!!
	void ClearBit(long i, long j);
	void ClearBit(long i, long j, int area);	//area in bins!!!!!!
	int GetElement(int type, long i, long j) {return GetBit(type,i,j);}
	void SetZ(float z) {Z=z;}
	float GetZ(void) {return Z;}
//	int GetcElement(long i, long j) {return GetcBit(i,j);}
//	void GetSize(long *x_size, long *y_size);
//	long GetSize(void) {return byteSize;}
//	const unsigned char *GetPointer(void) {return mas;}
//	TBitMatrix& operator=(TBitMatrix& T);
//	TBaseMatrix* operator+(TBitMatrix& TBit);

	ClassDef(TBitMatrix,1)
};

#endif
