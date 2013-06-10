void rootlogon()
{
  // fedra lib path should be setted in LD_LIBRARY_PATH
  //gEDBDEBUGLEVEL=1;
  printf("\nLoad FEDRA libs: \n");
  if( strncmp(gSystem->GetName(),"Unix",4)==0 )
     loadlib( "libvt"   , "CMatrix" );
  loadlib( "libEphys", "EdbPhysics" );
  loadlib( "libEdb"  , "EdbView" );
  loadlib( "libEmath", "EdbMath" );
  loadlib( "libEbase", "EdbLayer" );
  loadlib( "libEdr"  , "EdbPattern" );
  loadlib( "libEIO"  , "EdbRunAccess" );

  loadlib( "libEGA"  , "EdbGA" );                 // optional
  loadlib( "libGeom" , "TGeoVolume" );            // required for VMC
  loadlib( "libGui"  , "TGNumberEntry" );         // required only in batch mode
  loadlib( "libEdd"  , "EdbDisplay" );            // optional
  if(gSystem->Load("libDataConversion"))          // optional
    printf("libDataConversion is NOT loaded!\n"); 
  loadlib( "libAlignment", "EdbPositionAlignment" );   // optional
  loadlib( "libScan"  , "EdbScanProc" );          // optional
  loadlib( "libEMC"  , "EdbPVGen" );              // optional
  loadlib( "libShower", "EdbShowerRec" );         // optional, beta-version
  loadlibEOracle();                               // optional
  loadlib( "libEmr", "EdbEmrFileAccess" );        // optional

  loadlib( "libEve", "TEveVector" );              // optional required for EDA
  loadlib( "libEDA",  "EdbEDA");                  // optional
  
  loadlib( "libShowRec", "EdbShowRec" );         // optional, ALPHA-version (replacement for libshower soon)
  printf("\n\n");

  gStyle->SetPalette(1);
}

//----------------------------------------------------------
void loadlib(const char *lib, const char *key)
{
  if (!TClassTable::GetDict(key)) { 
    if(gSystem->Load(lib)) printf("\n ERROR: \n%s is NOT loaded!",lib);
    else printf(" %s",lib);
  }
}

//----------------------------------------------------------
void loadlibEOracle()
{
  if (TClassTable::GetDict("TOracleServerE")) return;

  if( strncmp(gSystem->GetName(),"Unix",4)==0 ) {
    if( (gSystem->Load("libclntsh")==0) && 
	(gSystem->Load("libocci")==0) &&
        (gSystem->Load("libEOracle")==0) )
      printf(" libEOracle(Unux)"); return;
  } else {
    if( (gSystem->Load("oraocci10")==0) && 
	(gSystem->Load("libEOracle")==0) )
      printf(" libEOracle(Win32)"); return;
  }
  printf("\nERROR: \n libEOracle is NOT loaded!\n");
}

