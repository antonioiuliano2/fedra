
#ifndef _BYTEMATRIX_H_
#define _BYTEMATRIX_H_

#include "TObject.h"
#include "TObjArray.h"
#include "basematrix.h"
#include "bitmatrix.h"

//=================================================================

class TByteMatrix : public TBaseMatrix {
/*private:
	long xSize;						// dimension in BYTE!
	long ySize;
	long byteSize;					// size in Byte!
	unsigned char *mas;
*/
protected:
	double Tgx,Tgy;
public:
	TByteMatrix();
	TByteMatrix(long x_size, long y_size);
	TByteMatrix(long x_size, long y_size, double tgx, double tgy);
	TByteMatrix(TBaseMatrix& T);
	virtual ~TByteMatrix();
//	void Init(long x_size, long y_size);
	void SetAngle(double tgx, double tgy);
	void GetAngle(double *tgx, double *tgy);
	int GetByte(int type, long i, long j);			//i - x, j - y
	void SetByte(long i, long j, unsigned char byte = 0xff);
	void ClearByte(long i, long j);
	void ClearAll(void);
	void Add1(long i, long j);
	int GetElement(int type, long i, long j) {return GetByte(type,i,j);}
//	void GetSize(long *x_size, long *y_size);
//	long GetSize(void) {return byteSize;}
//	const unsigned char *GetPointer(void) {return mas;}
//	TByteMatrix* operator=(TByteMatrix T);
//	TByteMatrix& operator+(TBitMatrix& Tbit1, TBitMatrix& Tbit2);
//	TByteMatrix operator+(TBitMatrix& TBit);
//	TByteMatrix operator+(TByteMatrix& TByt);
//	TByteMatrix* operator+(TBitMatrix* TBit);
//	TByteMatrix* operator+(TByteMatrix* TByt);
	TByteMatrix& operator+=(TBitMatrix& TBit);
	TByteMatrix& operator+=(TByteMatrix& TByt);
	TObjArray* Pick8(int type, int thr);					//Array of MyPoint
//	TObjArray* Pick12(int type, int thr);					//Array of MyPoint


	ClassDef(TByteMatrix,1)
};

#endif
