//-- Author :  Valeri Tioukov   10.04.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbRun                                                               //
//                                                                      //
// Implementation of Scanning Run class                                 //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "TSystem.h"

#ifndef ROOT_EdbRunHeader
#include "EdbRunHeader.h"
#endif

ClassImp(EdbRunHeader)
ClassImp(EdbCamera)
ClassImp(EdbPlate)

//______________________________________________________________________________
EdbRunHeader::EdbRunHeader( int n )
{
  eRunID       = n;
  eStartTime.Set();
  eFinishTime.Set();
  // eComment     = 0;

  eArea        = new EdbArea();
  ePlate       = new EdbPlate();
  eStage       = new EdbStage();

  eSystemStartTime = gSystem->Now();
  eCPU = 0;
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
