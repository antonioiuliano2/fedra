//-- Author :  Valeri Tioukov   6.03.2004
#include "EdbMask.h"
ClassImp(EdbMask)

void EdbMask::Print()
{
  int count=0;
  for(int i=0; i<eBuffer.GetSize(); i++) 
    if((int)(eBuffer[i])>0) { printf("%10d",i); if(++count%10==0) printf("\n"); }
  printf("\n %d/%d entries in EdbMask\n",count,eBuffer.GetSize());
}
