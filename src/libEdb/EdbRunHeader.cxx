//-- Author :  Valeri Tioukov   10.04.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRun                                                               //
//                                                                      //
// Implementation of Scanning Run class                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include <TSystem.h>
#include <TClass.h>
#include "EdbRunHeader.h"

ClassImp(EdbRunHeader)
ClassImp(EdbCamera)
ClassImp(EdbPlate)

//______________________________________________________________________________
EdbRunHeader::EdbRunHeader( int n )
{
  eRunID       = n;
  for(int i=0;i<10;i++) eFlag[i]=0;
  eStartTime.Set();
  eFinishTime.Set();
  eArea        = new EdbArea();
  ePlate       = new EdbPlate();
  eStage       = new EdbStage();
  eSystemStartTime = gSystem->Now();
  eCPU = 0;
  eNareas=0;
  eXmin=0; eXmax=0;
  eYmin=0; eYmax=0;
}

//______________________________________________________________________________
void EdbRunHeader::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbRunHeader.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbRunHeader::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      TNamed::Streamer(R__b);
      R__b >> eRunID;
      R__b.ReadStaticArray((int*)eFlag);
      eStartTime.Streamer(R__b);
      eFinishTime.Streamer(R__b);
      R__b >> eSystemStartTime;
      R__b >> eCPU;
      eComment.Streamer(R__b);
      R__b >> eArea;
      R__b >> ePlate;
      R__b >> eStage;
      R__b >> eXmin;
      R__b >> eXmax;
      R__b >> eYmin;
      R__b >> eYmax;
      R__b >> eNareas;
      R__b.CheckByteCount(R__s, R__c, EdbRunHeader::IsA());
      //====end of old versions
   } else {
     EdbRunHeader::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbRunHeader::Print()
{
  printf("\n--------------------------------------------------------------------\n");
  printf("Run Title:      \t %s\n", GetTitle() );
  printf("Run Identifier: \t %d\n", eRunID);
  if(eStage)    eStage->Print();
  if(ePlate)    ePlate->Print();
  printf("\n");
  printf("Run started  at:\t "); eStartTime.Print();
  printf("Run finished at:\t "); eFinishTime.Print();
  int rtime = eFinishTime.Get()-eStartTime.Get();
  //Int_t rtime = ((ULong_t)(gSystem->Now()) - (ULong_t)GetSystemStartTime()); ???
  printf("Total real time:\t %d \n", rtime );
  printf("Total CPU time: \t %.3f \n",eCPU );
  printf("\n");
  if(eComment)  printf( "Run Comment: \t %s\n",eComment.Data() );
  printf("--------------------------------------------------------------------\n");
}

//______________________________________________________________________________
void EdbCamera::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbCamera.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbCamera::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      R__b >> eWidth;
      R__b >> eHeight;
      R__b >> eRows;
      R__b >> eColumns;
      R__b.CheckByteCount(R__s, R__c, EdbCamera::IsA());
      //====end of old versions
   } else {
     EdbCamera::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbCamera::SetCamera(float w, float h, int r, int c)
{
  if(w!=0) eWidth   = w;
  if(h!=0) eHeight  = h;
  if(r!=0) eRows    = r;
  if(c!=0) eColumns = c;
}

//______________________________________________________________________________
void EdbCamera::Print() const
{
  printf("\nEdbCamera: \t\t %s \n", GetTitle() );
  printf("%s \t\t\t %d %d    %d %d\n", GetName(),
	 (int)eWidth, (int)eHeight,  eRows, eColumns);
}

//______________________________________________________________________________
void EdbPlate::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbPlate.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbPlate::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      R__b >> eID;
      R__b >> eUp;
      R__b >> eBase;
      R__b >> eDown;
      R__b >> eShrinkageU;
      R__b >> eShrinkageD;
      R__b.CheckByteCount(R__s, R__c, EdbPlate::IsA());
      //====end of old versions
   } else {
     EdbPlate::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbPlate::SetEmulsion( float thickness )
{
  //
  // To set the nominal thickness of both emulsion sides if they are the same
  //
  eUp   = thickness;
  eDown = thickness;
}

//______________________________________________________________________________
Float_t EdbPlate::GetEmulsion() const
{
  //
  // Returns the nominal thickness of both emulsion sides if they are the same
  // if not - return 0
  //

  if( eUp == eDown ) return eUp;
  else {
    printf(
     "WARNING: EdbPlate::GetEmulsion(): Up and Down has different thickness!");
    return 0;
  }
}

//______________________________________________________________________________
int  EdbPlate::ReadParameters( const char *str )
{
  char name[256];
  char title[256];
  int  npar = 0;

  int   id;
  float u,b,d,su,sd;

  npar = sscanf(str,"%d %f %f %f %f %f %s %s", &id, &u, &b, &d, &su, &sd, name, title );

  Set( id,u,b,d,su,sd );
  SetName(name);
  SetTitle(title);

  return npar;
}

//______________________________________________________________________________
void EdbPlate::Print() const
{
  printf("\nEdbPlate: \t\t %s \n", GetTitle() );
  printf("%s: \t\t\t %d   %d/%d/%d   %f %f \n", GetName(),
	 eID, (int)eUp, (int)eBase, (int)eDown, eShrinkageU, eShrinkageD);
}

//______________________________________________________________________________
void EdbPlate::Set(int id, 
		   float u, float b, float d, 
		   float su, float sd )
{
  eID          = id;
  eUp          = u;
  eBase        = b;
  eDown        = d;
  eShrinkageU  = su;
  eShrinkageD  = sd;
}
