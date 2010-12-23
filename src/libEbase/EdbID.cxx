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

void EdbID::Set(const char *id_string)
{
  if(id_string)
    if( sscanf(id_string,"%d.%d.%d.%d",&eBrick,&ePlate,&eMajor,&eMinor)!=4 ) Set();
}

char *EdbID::AsString() const
{
  char *str = new char[32];
  sprintf(str,"%d.%d.%d.%d",eBrick,ePlate,eMajor,eMinor );
  return str;
}
