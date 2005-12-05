void rootlogon()
{
  // project lib path should be setted in LD_LIBRARY_PATH

  loadlib( "libvt"   , "CMatrix" );
  loadlib( "libEdb"  , "EdbView" );
  loadlib( "libEmath", "EdbMath" );
  loadlib( "libEphys", "EdbPhysics" );
  loadlib( "libEdr"  , "EdbPattern" );
  loadlib( "libEIO"  , "EdbRunAccess" );

  loadlib( "libEGA"  , "EdbGA" );          //optional
  loadlib( "libEdd"  , "EdbDisplay" );     //optional
  loadlib( "libEMC"  , "EdbPVGen" );       //optional

  loadlibEOracle();                                //optional

  //loadlib( "EmrPoint", "libEmr" );
}

//----------------------------------------------------------
void loadlib(const char *lib, const char *key)
{
  if (!TClassTable::GetDict(key)) { 
    if(gSystem->Load(lib)) printf("%s do NOT loaded!\n",lib);
    //else                   printf("%s   \tloaded \n",lib);
  }  
}

//----------------------------------------------------------
void loadlibEOracle()
{
  if (!TClassTable::GetDict("TOracleServerE")) {
    if(gSystem->Load("libclntsh")==0) {
      if(gSystem->Load("libocci")==0)
        if(gSystem->Load("libEOracle")==0)
	  printf("libEOracle   \tloaded (Linux)\n");
    } else {
      if(gSystem->Load("oraocci10")==0)
        if(gSystem->Load("libEOracle")==0)
	  printf("libEOracle   \tloaded (Win32)\n");
    } else 
      printf("libEOracle do NOT loaded!\n");
  }
}

