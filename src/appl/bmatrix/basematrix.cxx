//#include "stdafx.h"

#include "basematrix.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include "Riostream.h"

ClassImp(TBaseMatrix);
//void TBaseMatrix::Init(long x, long y);

MyPoint::MyPoint(long xx, long yy, int h)
{
	x=xx;
	y=yy;
	height = h;
}
//___________________________________________________________________
MyPoint::~MyPoint()
{
}
//===================================================================
//___________________________________________________________________

TBaseMatrix::TBaseMatrix() 
{
	xSize=0;
	ySize=0;
	byteSize=0;
	mas = NULL;
	shiftX=0;
	shiftY=0;
//	position = 0;
//	NonZeroPoints = new TObjArray();

/*	cxSize=0;
	cySize=0;
	cbyteSize=0;
	cmas = NULL;
*/
}

TBaseMatrix::TBaseMatrix(long ix_size, long iy_size)
{
	Init(ix_size,iy_size);
}

//___________________________________________________________________
TBaseMatrix::TBaseMatrix(TBaseMatrix& iT)
{
	long ixs,iys;
	iT.GetSize(&ixs,&iys);
	Init(ixs,iys);
//	*this = T;
}

//___________________________________________________________________
/*TBaseMatrix::TBaseMatrix(long x, long y) {
//	TBaseMatrix();
	xSize = x;
	ySize = y;
	long bitSize = xSize*ySize;
	byteSize = (long)(bitSize/8) + (bitSize%8)?1:0;
	mas = (unsigned char *)calloc(byteSize,sizeof(unsigned char));
}
*/
//___________________________________________________________________

TBaseMatrix::~TBaseMatrix() {
	if (mas) {
		free(mas);
		mas = NULL;
	}
/*	if (cmas) {
		free(cmas);
		cmas = NULL;
	}
*/	xSize=0;
	ySize=0;
	byteSize=0;
	shiftX=0;
	shiftY=0;
/*	if (NonZeroPoints) {
		NonZeroPoints->Delete();
		delete NonZeroPoints;
	}
*/
/*	cxSize=0;
	cySize=0;
	cbyteSize=0;
*/
}

//__________________________________________________________________
/*const unsigned char *TBaseMatrix::GetPointer()
{
	if (type==1) return imas;
	else if (type==0) return cmas;
	else {
		cout<<"UNKNOWN GETPOINTER TYPE"<<endl;
		return NULL;
	}
}
*/
//__________________________________________________________________
void TBaseMatrix::Init(long x, long y) {
	if (mas) free(mas);
//	if (cmas) free(cmas);
	xSize = x;
	ySize = y;
//	long bitSize = xSize*ySize;
	byteSize = xSize*ySize;
	mas = (unsigned char *)calloc(byteSize,sizeof(unsigned char));
//	cmas = (unsigned char *)calloc(cbyteSize,sizeof(unsigned char));
	shiftX=0;
	shiftY=0;
/*	if (NonZeroPoints) {
		NonZeroPoints->Delete();
		delete NonZeroPoints;
	}
	NonZeroPoints = new TObjArray();
	position = 0;
*/
//	TByteMatrix(x,y);
}

//___________________________________________________________________
void TBaseMatrix::ResetShift()
{
/*	if (cmas) free(cmas);
	cxSize = ixSize;
	cySize = iySize;
	cbyteSize = ibyteSize;
	cmas = (unsigned char *)calloc(cbyteSize,sizeof(unsigned char));
	memcpy((void *)cmas,(void *)imas,ibyteSize);
*/
	shiftX=0;
	shiftY=0;
}

//___________________________________________________________________
void TBaseMatrix::Adopt(unsigned char *m, long rows, long columns)
{
	Init(columns,rows);
	memcpy((void *)mas,(void *)m,rows*columns);
//	Synchronize();
}
//___________________________________________________________________
/*void TBaseMatrix::Init(long x, long y) 
{

}
*/
/*void TBaseMatrix::Init(long x, long y) {
	if (mas) free(mas);
	TBaseMatrix(x,y);
}
*/
//____________________________________________________________________

/*int TBaseMatrix::GetBit(long i, long j) {
	long bit = xSize * j + i;
//	int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
	return (((mas[bit/8])&(0x80>>(bit%8)))>0);
}
*/
//____________________________________________________________________

/*void TBaseMatrix::SetBit(long i, long j) {
	long bit = xSize * j + i;
//  int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
	mas[bit/8] |= 0x80>>(bit%8);
}
*/
//____________________________________________________________________

/*void TBaseMatrix::ClearBit(long i, long j) {
	long bit = xSize * j + i;
//  int byte = (int)(xSize/8)*j + (int)(i/8);
//  int bit_byte = i%8;
//  unsigned char l1 = 1;
	mas[bit/8] &= ~(0x80>>(bit%8));
}
*/
//____________________________________________________________________
long TBaseMatrix::GetSize(long *x, long *y)
{
	if (x) *x = xSize;
	if (y) *y = ySize;
	return byteSize;
}

//____________________________________________________________________
/*long TBaseMatrix::GetcSize(long *cx, long *cy)
{
	if (cx) *cx = cxSize;
	if (cy) *cy = cySize;
	return cbyteSize;
}
*/
//____________________________________________________________________
//____________________________________________________________________
void TBaseMatrix::Print(int type)
{
	long xs,ys;
	GetSize(&xs,&ys);
	if (type==0) {cout<<"=========initial mas================"<<endl;}
	else if (type == 1) {cout<<"=========current mas================"<<endl;}
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			cout<<GetElement(type,i,j)<<" ";
		}
		cout<<endl;
	}
	cout<<"===================================="<<endl;
}

//____________________________________________________________________
/*void TBaseMatrix::cPrint()
{
	long cxs,cys;
	GetcSize(&cxs,&cys);
	cout<<"=========current mas================"<<endl;
	for (long j=0; j<iys; j++) {
		for (long i=0; i<ixs; i++) {
			cout<<GetcElement(i,j)<<" ";
		}
		cout<<endl;
	}
	cout<<"===================================="<<endl;
}
*/
/*unsigned char *Getmas(void) 
{
	return mas
}*/
//____________________________________________________________________
/*
void TBaseMatrix::MoveMatrix2Left_Up(long up, long left)
{
	if ((up==0)&&(left==0)) {
		Synchronize();
	}
	else if ((up<iySize)&&(left<ixSize)) {
		for (long j=0; j<iySize-up; j++) {
//			for (long i=0; i<ixSize-left; i++) {
			memcpy((void *)(&cmas[j*ixSize]), (void *)(&imas[(up+j)*ixSize+left]),ixSize-left);
			if (left) memset((void *)(&cmas[j*ixSize+(ixSize-left)]),0,left);
		}
	//		for (j=iySize-up; j<iySize; j++) {
		if (up) memset((void *)(&cmas[(iySize-up)*ixSize]),0,up*ixSize);
	}
	else memset((void *)cmas,0,ibyteSize);
}

//____________________________________________________________________
void TBaseMatrix::MoveMatrix2Right_Down(long d, long r)
{
	if ((d==0)&&(r==0)) {
		Synchronize();
	}
	else if ((d<iySize)&&(r<ixSize)) {
		if (d) memset((void *)cmas,0,d*ixSize);
		for (long j=d; j<iySize; j++) {
			if (r) memset((void *)(&cmas[j*ixSize]),0,r);
			memcpy((void *)(&cmas[j*ixSize+r]), (void *)(&imas[(j-d)*ixSize]),ixSize-r);
		}
//		for (j=iySize-up; j<iySize; j++) {
	}
	else memset((void *)cmas,0,ibyteSize);
}

//____________________________________________________________________
void TBaseMatrix::MoveMatrix2Left_Down(long d, long left)
{
	if ((d==0)&&(left==0)) {
		Synchronize();
	}
	else if ((d<iySize)&&(left<ixSize)) {
		if (d) memset((void *)cmas,0,d*ixSize);
		for (long j=d; j<iySize; j++) {
			memcpy((void *)(&cmas[j*ixSize]), (void *)(&imas[(j-d)*ixSize+left]),ixSize-left);
			if (left) memset((void *)(&cmas[j*ixSize+(ixSize-left)]),0,left);
		}
//		for (j=iySize-up; j<iySize; j++) {
	}
	else memset((void *)cmas,0,ibyteSize);
}

//____________________________________________________________________
void TBaseMatrix::MoveMatrix2Right_Up(long up, long r)
{
	if ((up==0)&&(r==0)) {
		Synchronize();
	}
	else if ((up<iySize)&&(r<ixSize)) {
		for (long j=0; j<iySize-up; j++) {
			if (r) memset((void *)(&cmas[j*ixSize]),0,r);
			memcpy((void *)(&cmas[j*ixSize+r]), (void *)(&imas[(up+j)*ixSize]),ixSize-r);
		}
	//		for (j=iySize-up; j<iySize; j++) {
		if (up) memset((void *)(&cmas[(iySize-up)*ixSize]),0,up*ixSize);
	}
	else memset((void *)cmas,0,ibyteSize);
}

//____________________________________________________________________
*/
void TBaseMatrix::MoveMatrix(long ud, long lr)
{
/*	if ((ud==0)&&(lr==0)) Synchronize();
	else if ((ud>=0)&&(lr>=0)) MoveMatrix2Right_Down(ud,lr);
	else if ((ud>=0)&&(lr<0)) MoveMatrix2Left_Down(ud,-lr);
	else if ((ud<0)&&(lr>=0)) MoveMatrix2Right_Up(-ud,lr);
	else if ((ud<0)&&(lr<0)) MoveMatrix2Left_Up(-ud,-lr);
*/
	shiftX=lr;
	shiftY=ud;
}

//____________________________________________________________________
TBaseMatrix& TBaseMatrix::operator=(TBaseMatrix& T) {
    long x,y;
	T.GetSize(&x,&y);
	Init(x,y);
	memcpy((void *)mas,(void *)T.GetPointer(),T.GetSize());
/*	if (NonZeroPoints) {
		NonZeroPoints->Delete();
		delete NonZeroPoints;
	}
	NonZeroPoints = new TObjArray(*T.GetP());	*/
//	Synchronize();
	return *this;
}

//____________________________________________________________________
/*
int TBaseMatrix::FirstPoint(int type, long *i, long *j)
{
//	long shi,shj;
//	position = 0;
	int n = NonZeroPoints->GetEntries();
	if (!n) return false;
	MyPoint *point = 0;
	for (position = 0; position<n; position++) {
		point = (MyPoint *)(NonZeroPoints->At(position));
		long shi,shj;
		if (type==0) {
			shi = point->x;
			shj = point->y;
		}
		else if (type ==1) {
			shi = point->x + shiftX;
			shj = point->y + shiftY;
		}
		if ((shi>=xSize)||(shj>=ySize)||(shi<0)||(shj<0)) {
			if (position==n-1) return false;
			else continue;
		}
		else {
			*i = shi;
			*j = shj;
			return true;
		}
	}
}

//___________________________________________________________________

int TBaseMatrix::NextPoint(int type, long *i, long *j)
{
//	long shi,shj;
	position++;
	int n = NonZeroPoints->GetEntries();
	if ((!n)||(position>=n)) return false;
	MyPoint *point = 0;
	for (; position<n; position++) {
		point = (MyPoint *)(NonZeroPoints->At(position));
		long shi,shj;
		if (type==0) {
			shi = point->x;
			shj = point->y;
		}
		else if (type ==1) {
			shi = point->x + shiftX;
			shj = point->y + shiftY;
		}
		if ((shi>=xSize)||(shj>=ySize)||(shi<0)||(shj<0)) {
			if (position==n-1) return false;
			else continue;
		}
		else {
			*i = shi;
			*j = shj;
			return true;
		}
	}
}
*/