#ifndef ROOT_TOracleServerE
#define ROOT_TOracleServerE
 
#include "TOracleServer.h"

class TTree;
class EdbPattern;
class EdbPatternsVolume;

class TOracleServerE : public TOracleServer {

public:
  TOracleServerE(const char *db, const char *uid, const char *pw):
    TOracleServer(db, uid, pw){}
  ~TOracleServerE(){}

   Int_t       QueryTree(char *query, TTree *tree, char *leafs=0);

   Int_t       ReadVolume(long long id_volume, EdbPatternsVolume &vol);
   Int_t       ReadDataSet(long long id_parent_op, int id_brick, EdbPatternsVolume &vol);
   bool        ReadZplate(int id_plate, int id_eventbrick, EdbPattern &pat);
   Int_t       ReadBasetracksPattern(char *selection, EdbPattern &pat);

   ClassDef(TOracleServerE,0)  // Connection to Oracle server
};

#endif
