// @(#)root/physics:$Name: fedra-v2-04-00 $:$Id: TOracleServer.h,v 1.5 2007/08/31 12:14:00 valeri Exp $
// Author: Yan Liu and Shaowen Wang   23/11/04
// Modified and adopted to OPERA by Valeri Tioukov 

/*************************************************************************
 * Copyright (C) 1995-2005, Rene Brun and Fons Rademakers.               *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/

#ifndef ROOT_TOracleServer
#define ROOT_TOracleServer

#ifndef ROOT_TSQLServer
#   include "TSQLServer.h"
#endif

#if !defined(__CINT__)
#   ifndef R__WIN32
#      include <sys/time.h>
#   endif
#   include <occi.h>
#   include "RVersion.h"
	using namespace std;
	using namespace oracle::occi;
#else
	class Environment;
	class Connection;
	class Statement;
#endif

class TTree;

class TOracleServer : public TSQLServer {

  friend class TOracleServerE;
  friend class TOracleServerE2;
  friend class TOracleServerE2W;

private:
   Environment  *fEnv;    // environment of Oracle access
   Connection   *fConn;   // connection to Oracle server

#ifndef R__WIN32
#   if !defined(__CINT__)
#      if ROOT_VERSION_CODE >= ROOT_VERSION(5,11,0)
          oracle::occi::Statement *fStmt; // reusable statement object
#      else
          Statement    *fStmt;   // reusable statement object
#      endif
#   else
       Statement    *fStmt;   // reusable statement object
#   endif
#else
#   if ROOT_VERSION_CODE < ROOT_VERSION(5,11,0)
		Statement    *fStmt;   // reusable statement object
#	else
#		if !defined(__CINT__)
			oracle::occi::Statement *fStmt; // reusable statement object
#		else
			Statement    *fStmt;   // reusable statement object
#		endif
#	endif
#endif

public:
   TOracleServer(const char *db, const char *uid, const char *pw);
   ~TOracleServer();

   void        Close(Option_t *opt="");
   TSQLResult *Query(const char *sql);
   Int_t       SelectDataBase(const char *dbname);
   TSQLResult *GetDataBases(const char *wild = 0);
   TSQLResult *GetTables(const char *dbname, const char *wild = 0);
   TSQLResult *GetColumns(const char *dbname, const char *table, const char *wild = 0);
   Int_t       CreateDataBase(const char *dbname);
   Int_t       DropDataBase(const char *dbname);
   Int_t       Reload();
   Int_t       Shutdown();
   const char *ServerInfo();

   // new functions in respect to basic root class (VT)
   Int_t       QueryTree(char *query, TTree *tree, char *leafs=0);

   ClassDef(TOracleServer,0)  // Connection to Oracle server
};

#endif
