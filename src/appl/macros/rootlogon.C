void rootlogon()
{
  // project lib path should be setted in LD_LIBRARY_PATH

  loadlib( "libvt"   , "CMatrix" );
  loadlib( "libEdb"  , "EdbView" );

  loadlib( "libEmath", "EdbMath" );
  loadlib( "libEphys", "EdbPhysics" );
  loadlib( "libEGA"  , "EdbGA" );           //optional
  loadlib( "libEdr"  , "EdbPattern" );
  loadlib( "libEIO"  , "EdbRunAccess" );

  loadlib( "libEdd"  , "EdbDisplay" );     //optional
  loadlib( "libEMC"  , "EdbPVGen" );       //optional

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
