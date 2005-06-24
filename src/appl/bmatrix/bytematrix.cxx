//#include "stdafx.h"

#include "bytematrix.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "Riostream.h"


ClassImp(TByteMatrix);
//___________________________________________________________________

TByteMatrix::TByteMatrix():TBaseMatrix() {
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
	shiftY=0;
	position = 0;
	NonZeroPoints = new TObjArray();
*/
}

//___________________________________________________________________

TByteMatrix::TByteMatrix(long x, long y):TBaseMatrix(x,y) {
//	TByteMatrix();
/*	xSize = x;
	ySize = y;
	long bitSize = xSize*ySize;
	byteSize = (long)(bitSize/8) + (bitSize%8)?1:0;
	mas = (unsigned char *)calloc(byteSize,sizeof(unsigned char));

	Init(x,y);
*/}

//___________________________________________________________________
	TByteMatrix::TByteMatrix(long x_size, long y_size, double tgx, double tgy):TBaseMatrix(x_size,y_size)
{
//	Init(x_size,y_size);
	Tgx = tgx;
	Tgy = tgy;
}
//___________________________________________________________________
TByteMatrix::TByteMatrix(TBaseMatrix& T):TBaseMatrix(T)
{
/*	long xs,ys;
	T.GetSize(&xs,&ys);
	Init(xs,ys);
*/}

//___________________________________________________________________

TByteMatrix::~TByteMatrix() {
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

//____________________________________________________________________
/*void TByteMatrix::Init(long x, long y) {
	if (mas) free(mas);
	xSize = x;
	ySize = y;
//	long bitSize = xSize*ySize;
	byteSize = xSize*ySize;
	mas = (unsigned char *)calloc(byteSize,sizeof(unsigned char));
//	TByteMatrix(x,y);
}
*/
//____________________________________________________________________

void TByteMatrix::SetAngle(double tgx, double tgy)
{
	Tgx = tgx;
	Tgy = tgy;
}
//____________________________________________________________________

void TByteMatrix::GetAngle(double *tgx, double *tgy)
{
	*tgx = Tgx;
	*tgy = Tgy;
}
//____________________________________________________________________

int TByteMatrix::GetByte(int type, long i, long j) 
{
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
	long byte = xSize * shj + shi;
	return mas[byte];
}

//____________________________________________________________________

void TByteMatrix::SetByte(long i, long j, unsigned char byte) 
{
	long byt = xSize * j + i;
//  int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
	mas[byt] =byte ;
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

void TByteMatrix::ClearByte(long i, long j) 
{
	long byt = xSize * j + i;
//  int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
	mas[byt] &=0x00 ;
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
void TByteMatrix::ClearAll()
{
	memset((void *)mas,0,byteSize);
//	NonZeroPoints->Delete();
}

//____________________________________________________________________
void TByteMatrix::Add1(long i, long j)
{
	long byt = xSize * j + i;
	mas[byt]++;
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

/*void TByteMatrix::GetSize(long *x, long *y) {
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
/*TByteMatrix* TByteMatrix::operator=(TByteMatrix T) {
    long x,y;
	T.GetSize(&x,&y);
	Init(x,y);
	memcpy((void *)mas,(void *)T.Getmas(),T.GetSize());
	return this;
}
*/
//____________________________________________________________________

/*TByteMatrix& TByteMatrix::operator+(TBitMatrix& Tbit1, TBitMatrix& Tbit2)
{
	long x1,y1,x2,y2;
	Tbit1.GetSize(&x1,&y1);
	Tbit2.GetSize(&x2,&y2);
	Init(x1<x2?x1:x2,y1<y2?y1:y2);
}
*/

//_____________________________________________________________________
/*TByteMatrix TByteMatrix::operator+(TBitMatrix& TBit)
{
	long xthis,ythis,x2,y2;
	GetSize(0,&xthis,&ythis);
	TBit.GetSize(0,&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
	TByteMatrix TByte(xs,ys);
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			TByte.SetByte(0,i,j,GetByte(0,i,j)+TBit.GetBit(0,i,j));
		}
	}
	cout<<"test print"<<endl;
	TByte.Print(0);
	return TByte;
}*/

//_____________________________________________________________________
/*TByteMatrix TByteMatrix::operator+(TByteMatrix& TByt) 
{
	long xthis,ythis,x2,y2;
	GetSize(0,&xthis,&ythis);
	TByt.GetSize(0,&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
	TByteMatrix TByte(xs,ys);
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			TByte.SetByte(0,i,j,GetByte(0,i,j)+TByt.GetByte(0,i,j));
		}
	}
	cout<<"test print"<<endl;
	TByte.Print(0);
	return TByte;
}*/

//_____________________________________________________________________
/*TByteMatrix* TByteMatrix::operator+(TBitMatrix *TBit)
{
	long xthis,ythis,x2,y2;
	GetSize(0,&xthis,&ythis);
	TBit->GetSize(0,&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
	TByteMatrix* TByte = new TByteMatrix(xs,ys);
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			TByte->SetByte(0,i,j,GetByte(0,i,j)+TBit->GetBit(0,i,j));
		}
	}
	return TByte;
}*/
//_____________________________________________________________________
/*TByteMatrix* TByteMatrix::operator+(TByteMatrix *TByt)
{
	long xthis,ythis,x2,y2;
	GetSize(0,&xthis,&ythis);
	TByt->GetSize(0,&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
	TByteMatrix* TByte = new TByteMatrix(xs,ys);
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			TByte->SetByte(0,i,j,GetByte(0,i,j)+TByt->GetByte(0,i,j));
		}
	}
	return TByte;
}*/
//_____________________________________________________________________
TByteMatrix& TByteMatrix::operator+=(TBitMatrix& TBit)
{
	long xthis,ythis,x2,y2;
	GetSize(&xthis,&ythis);
	TBit.GetSize(&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
//	TByteMatrix* TByte = new TByteMatrix(xs,ys);
/*	long i,j;
	if (TBit.FirstPoint(1,&i,&j)) Add1(i,j);
	while (TBit.NextPoint(1,&i,&j)) {
		Add1(i,j);
	}
*/
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			if (TBit.GetBit(1,i,j)) Add1(i,j);
		}
	}

	return *this;
}

//_____________________________________________________________________
TByteMatrix& TByteMatrix::operator+=(TByteMatrix& TByt)
{
	long xthis,ythis,x2,y2;
	GetSize(&xthis,&ythis);
	TByt.GetSize(&x2,&y2);
	long xs = (x2<xthis)?x2:xthis;
	long ys = (y2<ythis)?y2:ythis;
//	TByteMatrix* TByte = new TByteMatrix(xs,ys);
/*	long i,j;
	if (TByt.FirstPoint(1,&i,&j)) 
		if (int b = TByt.GetByte(1,i,j)) SetByte(i,j,GetByte(0,i,j)+b);
	while (TByt.NextPoint(1,&i,&j)) {
		if (int b = TByt.GetByte(1,i,j)) SetByte(i,j,GetByte(0,i,j)+b);
	}
*/
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			if (int b = TByt.GetByte(1,i,j)) SetByte(i,j,GetByte(0,i,j)+b);
		}
	}
	return *this;
}

//_____________________________________________________________________
TObjArray* TByteMatrix::Pick8(int type, int thr)				
{
//This is a modified code from Valeri's FEDRA
	int xn[8] = { 1, -1, 0,  0, 1,  1, -1, -1 }; //pixel 3x3 suburbs
	int yn[8] = { 0,  0, 1, -1, 1, -1,  1, -1 };
	TObjArray* Ta = new TObjArray();
	long nc,nr;
	GetSize(&nc,&nr);
	int pix,pp,pix1;

//	printf("hist: %d %d\n",nc,nr);
//  int npeaks=0;
	int ic,ir,in;           // declare here to fix marazmatic eror messages of VC++
	for(ic=2; ic<nc-3; ic++) {
		for(ir=2; ir<nr-3; ir++) {

			pix = GetByte(type,ic,ir); 
			if( pix < thr)   goto NEXTPIX;
			pix1=pix+1;

			for(in=0; in<8; in++) {
				pp = GetByte(type, ic+xn[in], ir+yn[in] );
      			if( pix1 <= pp ) goto NEXTPIX;			//it was <=
				SetByte(ic,ir,pix1);
			}
//		printf("peak8(%d,%d) =  %d\n",ic,ir,pix); 
		Ta->Add(new MyPoint(ic,ir,pix));
NEXTPIX:
		continue;
    }
  }
	return Ta;
}
