//#include "stdafx.h"

#include "bitmatrix.h"
#include "bytematrix.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "Riostream.h"
#include <math.h>

ClassImp(TBitMatrix);
//___________________________________________________________________

TBitMatrix::TBitMatrix():TBaseMatrix() {
//	TBaseMatrix();
/*	xSize=0;
	ySize=0;
	byteSize=0;
	mas = NULL;

	xSize=0;
	ySize=0;
	byteSize=0;
	mas = NULL;
	shiftX=0;
	position = 0;
	NonZeroPoints = new TObjArray();

	shiftY=0;
*/
}
//___________________________________________________________________

TBitMatrix::TBitMatrix(long x, long y):TBaseMatrix(x,y) {
//	TBitMatrix();
/*	xSize = x;
	ySize = y;
	long bitSize = xSize*ySize;
	byteSize = (long)(bitSize/8) + (bitSize%8)?1:0;
	mas = (unsigned char *)calloc(byteSize,sizeof(unsigned char));

	Init(x,y);
*/}

//___________________________________________________________________
TBitMatrix::TBitMatrix(long x_size, long y_size, float z):TBaseMatrix(x_size,y_size)
{
//	Init(x_size, y_size);
	Z=z;
}
//___________________________________________________________________
TBitMatrix::TBitMatrix(TBaseMatrix& T):TBaseMatrix(T)
{
/*	long xs,ys;
	T.GetSize(&xs,&ys);
	Init(xs,ys);
	*/
}

//___________________________________________________________________

TBitMatrix::~TBitMatrix() {
/*	if (imas) {
		free(imas);
		imas = NULL;
	}
	if (cmas) {
		free(cmas);
		cmas = NULL;
	}
	ixSize=0;
	iySize=0;
	ibyteSize=0;

	cxSize=0;
	cySize=0;
	cbyteSize=0;
*/}

//___________________________________________________________________

/*void TBitMatrix::Init(long x, long y) {
	if (mas) free(mas);
	xSize = x;
	ySize = y;
	long bitSize = xSize*ySize;
	byteSize = (long)(bitSize/8) + (bitSize%8)?1:0;
	mas = (unsigned char *)calloc(byteSize,sizeof(unsigned char));
//	TBitMatrix(x,y);
}
*/
//____________________________________________________________________

int TBitMatrix::GetBit(int type, long i, long j) {
	long shi, shj, shx, shy;
	if (type==0) {
		shx = 0;
		shy = 0;
	}
	else if (type==1) {
		shx = shiftX;
		shy = shiftY;
	}
	else {
		cout<<"UNKNOWN GETBIT TYPE"<<endl;
		return 0;
	}
	shi = i - shx;
	shj = j - shy;
	if ((shi>=xSize)||(shj>=ySize)||(shi<0)||(shj<0)) return 0;
	long bit = xSize * shj + shi;
//	int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
//	return (((mas[bit/8])&(0x80>>(bit%8)))>0);
	return mas[bit]>0;
}

//____________________________________________________________________

void TBitMatrix::SetBit(long i, long j) {
	long bit = xSize * j + i;
	mas[bit] =1;
/*	MyPoint *point = 0;
	int n = NonZeroPoints->GetEntries();
	for (int k=0; k<n; k++) {
		point = (MyPoint *)(NonZeroPoints->At(k));
		if ((point->x == i)&&(point->y == j)) return;
	}
	NonZeroPoints->Add(new MyPoint(i,j,0));
*/
}
//____________________________________________________________________

void TBitMatrix::SetBit(long i, long j, int area)		//area in bits!!
{
	if (area<=1) SetBit(i,j);
	else {
		int sarea = (int)(sqrt((double)area));
		long lbx = i - sarea/2;
		lbx = (lbx>=0)?lbx:0;
		long rbx = i + sarea/2;
//		long _xSize = (type==0)?xSize:xSize;
		rbx = (rbx<xSize)?rbx:(xSize-1);
		long uby = j - sarea/2;
		uby = (uby>=0)?uby:0;
//		long ySize = (type==0)?cySize:iySize;
		long dby = j + sarea/2;
		dby = (dby<ySize)?dby:(ySize-1);
		for (long jj=uby; jj<=dby; jj++) {
			for (long ii=lbx; ii<=rbx; ii++) {
				SetBit(ii,jj);
			}
		}
	}
}
//____________________________________________________________________

void TBitMatrix::ClearBit(long i, long j) {
	long bit = xSize * j + i;
//  int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
	mas[bit] =0 ;
/*	MyPoint *point = 0;
	int n = NonZeroPoints->GetEntries();
	for (int k=0; k<n; k++) {
		point = (MyPoint *)(NonZeroPoints->At(k));
		if ((point->x == i)&&(point->y == j)) {
			delete NonZeroPoints->RemoveAt(k);
			NonZeroPoints->Compress();
			break;
		}
	}
*/
}
//____________________________________________________________________

void TBitMatrix::ClearBit(long i, long j, int area)		//area in bits!!!
{
	if (area<=1) ClearBit(i,j);
	else {
		int sarea = (int)(sqrt((double)area));
		long lbx = i - sarea/2;
		lbx = (lbx>=0)?lbx:0;
		long rbx = i + sarea/2;
//		long xSize = (type==0)?cxSize:ixSize;
		rbx = (rbx<xSize)?rbx:(xSize-1);
		long uby = j - sarea/2;
		uby = (uby>=0)?uby:0;
//		long ySize = (type==0)?cySize:iySize;
		long dby = j + sarea/2;
		dby = (dby<ySize)?dby:(ySize-1);
		for (long jj=uby; jj<=dby; jj++) {
			for (long ii=lbx; ii<=rbx; ii++) {
				ClearBit(ii,jj);
			}
		}
	}
	
}
//____________________________________________________________________
/*void TBitMatrix::GetSize(long *x, long *y) {
	*x = xSize;
	*y = ySize;
}
*/
//____________________________________________________________________

/*unsigned char *Getmas(void) 
{
	return mas
}*/
//____________________________________________________________________
/*TBitMatrix& TBitMatrix::operator=(TBitMatrix& T) {
    long x,y;
	T.GetSize(&x,&y);
	Init(x,y);
	memcpy((void *)mas,(void *)T.GetPointer(),T.GetSize());
	return *this;
}
*/
//___________________________________________________________________

/*TBaseMatrix* TBitMatrix::operator+(TBitMatrix& TBit)
{
	long xthis,ythis,x2,y2;
	GetSize(0,&xthis,&ythis);
	TBit.GetSize(0,&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
	TByteMatrix* TByte = new TByteMatrix(xs,ys);
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			TByte->SetByte(0,i,j,GetBit(0,i,j)+TBit.GetBit(0,i,j));
		}
	}
	return (TBaseMatrix *)TByte;
}*/
