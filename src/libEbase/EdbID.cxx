//-- Author :  Valeri Tioukov   21/03/2006
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbID                                                                //
//                                                                      //
// Scanning data identifier                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbID.h"
ClassImp(EdbID)
  
EdbID::EdbID(const char *id_string)
{
  Set(id_string);
}

bool EdbID::Set(const char *id_string)
{
  if(!id_string) return false;
  if( sscanf(id_string,"%d.%d.%d.%d",&eBrick,&ePlate,&eMajor,&eMinor)!=4 ) {Set(); return false;}
  return true;
}

char *EdbID::AsString() const
{
  char *str = new char[32];
  sprintf(str,"%d.%d.%d.%d",eBrick,ePlate,eMajor,eMinor );
  return str;
}
