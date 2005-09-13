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
  //loadlibEoi();                            //optional

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
void loadlibEoi()
{
  if (!TClassTable::GetDict("EoiIO")) {
    if( (gSystem->Load("libclntsh")==0) &&
        (gSystem->Load("libocci")==0) &&
        (gSystem->Load("libEoi")==0) )
      printf("libEoi   \tloaded \n");
    else
      printf("libEoi do NOT loaded!\n");
  }
}

