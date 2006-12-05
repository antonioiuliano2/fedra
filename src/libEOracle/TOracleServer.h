// @(#)root/physics:$Name: not supported by cvs2svn $:$Id: TOracleServer.h,v 1.2 2006-12-05 13:01:44 valeri Exp $
// Author: Yan Liu and Shaowen Wang   23/11/04

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
#include "TSQLServer.h"
#endif

#if !defined(__CINT__)
#ifndef R__WIN32
#include <sys/time.h>
#endif
#include <occi.h>
using namespace std;
using namespace oracle::occi;
#else
class Environment;
class Connection;
class Statement;
#endif

class TOracleServer : public TSQLServer {

  friend class TOracleServerE;

private:
   Environment  *fEnv;    // environment of Oracle access
   Connection   *fConn;   // connection to Oracle server
#if ROOT_VERSION_CODE < ROOT_VERSION(5,11,0)
   Statement    *fStmt;   // resusable statement object
#else
#if !defined(__CINT__)
   oracle::occi::Statement    *fStmt;   // resusable statement object
#else
   Statement    *fStmt;   // resusable statement object
#endif
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

   ClassDef(TOracleServer,0)  // Connection to Oracle server
};

#endif
