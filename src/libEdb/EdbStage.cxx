//-- Author :  Valeri Tioukov   6.06.2000

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbStage                                                             //
//                                                                      //
// Implementation of Stage class                                        //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
 
#include <TClass.h>
#include "EdbStage.h"

ClassImp(EdbCCD)
ClassImp(EdbObjective)
ClassImp(EdbStage)

//______________________________________________________________________________
EdbStage::EdbStage()
{
  eCCD            = new EdbCCD();
  eObjective      = new EdbObjective();
  eEncoders2Stage = new EdbAffine3D();
}

//______________________________________________________________________________
EdbStage::~EdbStage()
{
  if(eCCD)            delete eCCD;
  if(eObjective)      delete eObjective;
  if(eEncoders2Stage) delete eEncoders2Stage;
}

//______________________________________________________________________________
void EdbStage::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbStage.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbStage::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      R__b >> eCCD;
      R__b >> eObjective;
      R__b >> eEncoders2Stage;
      R__b.CheckByteCount(R__s, R__c, EdbStage::IsA());
      //====end of old versions
   } else {
     EdbStage::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
void EdbStage::Print( Option_t *opt ) const
{
  printf("EdbStage:\n");
  eCCD->Print();
  eObjective->Print();
  //eEncoders2Stage->Print();
}

//______________________________________________________________________________
void EdbCCD::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbCCD.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbCCD::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      R__b >> eNx;
      R__b >> eNy;
      R__b >> ePixX;
      R__b >> ePixY;
      R__b.CheckByteCount(R__s, R__c, EdbCCD::IsA());
      //====end of old versions
   } else {
     EdbCCD::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
int  EdbCCD::ReadParameters( const char *str )
{
  char name[256];
  char title[256];

  int npar = 0;
  npar = sscanf(str,"%d %d %f %f %s %s ", &eNx, &eNy, &ePixX, &ePixY, name, title );
  //  npar = sscanf(str,"%d %d %f %f", &eNx, &eNy, &ePixX, &ePixY);

  SetName(name);
  SetTitle(title);
  return npar;
}

//______________________________________________________________________________
void EdbCCD::Print( Option_t *opt ) const
{
  printf("\nEdbCCD: \t\t %s \n", GetTitle() );
  printf("%s: \t\t %d %d \t %f %f\n", GetName(),
	 eNx, eNy, ePixX, ePixY );
}
 
//______________________________________________________________________________
void EdbObjective::Streamer(TBuffer &R__b)
{
   // Stream an object of class EdbObjective.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      if (R__v > 1) {
	EdbObjective::Class()->ReadBuffer(R__b, this, R__v, R__s, R__c);
	return;
      }
      //====process old versions before automatic schema evolution
      TNamed::Streamer(R__b);
      R__b >> eMagnification;
      R__b >> eCCD2Stage;
      R__b.CheckByteCount(R__s, R__c, EdbObjective::IsA());
      //====end of old versions
   } else {
     EdbObjective::Class()->WriteBuffer(R__b,this);
   }
}

//______________________________________________________________________________
EdbObjective::EdbObjective( float mag )
{
  eMagnification = mag;
  eCCD2Stage = new EdbAffine2D();
}

//______________________________________________________________________________
EdbObjective::~EdbObjective()
{
  delete eCCD2Stage;
}

//______________________________________________________________________________
int  EdbObjective::ReadParameters( const char *str )
{
  char name[256];
  char title[256];
  int  npar = 0;

  float mag, a,b,c,d,p,q;

  npar = sscanf(str,"%f %f %f %f %f %f %f %s %s", &mag, &a, &b, &c, &d, &p, &q, name, title );

  Set(int(mag),a,b,c,d,p,q);
  SetName(name);
  SetTitle(title);

  return npar;
}

//______________________________________________________________________________
void EdbObjective::Print( Option_t *opt ) const
{
  printf("\nEdbObjective: \t\t %s \n", GetTitle() );
  printf( "%s: \t\t %f \n", GetName(),
	  eMagnification );
  eCCD2Stage->Print();
}

//______________________________________________________________________________
EdbAffine2D  *EdbStage::CCD2Stage()      const
{
  EdbAffine2D *aff = new EdbAffine2D( *(eObjective->GetCCD2Stage()) );
  return aff;
}
 
//______________________________________________________________________________
EdbAffine3D  *EdbStage::Encoders2Stage() const
{
  return 0;
}
