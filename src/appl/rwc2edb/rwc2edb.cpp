// rwc2edb.cpp
// convert rwc, rwd and grains dump files to root using the libEdb library
// author: Gabriele Sirri
// porting to Linux: Igor Kreslo and Nikolay Savvinov (October 15, 2004)
//  
//______________________________________________________________________________
//
// Revision 1.12
// -> option -clframe: to fill the clusters.eFrame branch
// -> EdbRunHeader: set camera coordinates eFlag[2] = (1 real , 2 pixel)  
//
// Revision 1.11 
// -> integration of windows and linux versions in the same source code
//    (see libDataConversion.cpp)
// -> option -nocl: do not convert clusters
// -> add tilex, tiley in the EdbViewHeader
//
// Revision 1.9 May 11, 2004
// -> conversion TXT (Test & Configure grains) -> ROOT  without RWD was added
//    In the previous versions only TXT+RWD -> ROOT was possible
//
// Revision 1.8	Jul 08, 2003 
// -> modular structure of the source code (libDataConversion.h and libDataConversion.cpp)
// -> the header of the last view of (grains).txt file has been updated 
//
// Revision 1.7	Jul 08, 2003 
// -> testrun was missed
//
// Revision 1.6	Jul 05, 2003
// -> IO_VS_Fragment,Track,VS_View changed to IO_VS_Fragment2,Track2,VS_View2 
//    (SySalDataIO2) 
// -> stage/view coordinates instead of marks coordinates
// -> dz definition bug fixed
// -> .tlg file option removed
// -> .map file can be read 
// -> number of clusters per layer
// 
// Revision 1.5	Jul 02, 2003
// -> eNframesTop and eNframesBot are flipped
//
// Revision 1.4	Jun 27, 2003
// -> rwc2edb modified by I.Kreslo to read grains dump
//
// Revision 1.3	Jun 20, 2003
// -> cluster cycle optimized
// -> Header->SetArea(..) modified
//
// Revision 1.2	Jun 18, 2003 
// -> removed memory leak in fragment loop
//
// Revision 1.1.1.1 May 30, 2003
// -> Framework for Emulsions Data Reconstruction and Analysis
//
// Revision 1.1	May 30, 2003
// -> Initial revision 

//#include <stdio.h>
//#include <vector>

#include "libDataConversion.h"


//______________________________________________________________________________
int main(int argc, char* argv[])
{
  char rwcname[256]="\0", edbname[256]="\0", mapname[256]="\0", grsname[256]="\0";
  char options[256]="\0";
  bool addmap(false),addgrs(false);

  bool printusage=(argc<3)?true:false;
  for (int i = 1; i < argc; i++)  {  // Skip argv[0] (program name)
    if (!strcmp(argv[i], "-map")) {   // Process optional arguments
      if (i + 1 <= argc - 1) { sprintf(mapname,argv[++i]); addmap=true; }
      else printusage=true; 
    }
    else if (!strcmp(argv[i], "-grs")) { // Process optional arguments
      if (i + 1 <= argc - 1) { sprintf(grsname,argv[++i]); addgrs=true; }
      else printusage=true; 
    }
    else if (!strcmp(argv[i], "-nocl"))    strcat(options,"NOCL") ;
    else if (!strcmp(argv[i], "-asum"))    strcat(options,"SUM") ;
    else if (!strcmp(argv[i], "-clframe")) strcat(options,"CLFRAME") ;
    else  { // Process non-optional arguments here
      sprintf(rwcname,argv[i++]);
      sprintf(edbname,argv[i]);
    }
  }
  if(printusage) { 
    cout<< "usage: rwc2edb <input file (.rwc|.rwd|.txt)> <output file (.root)> [options] "<<endl;
    cout << "\n options: -nocl         = do not add the clusters" << endl;
    cout <<   "          -clframe      = fill clusters.eFrame (default empty) -> +4% file size" << endl;
    cout <<   "          -map filename = add the fiducial marks file (.map) (only mswindows)" << endl;
    cout <<   "          -grs filename = merge raw data (.rwd) and grains (.txt) " << endl;
    cout <<   "                          (if only grains: rwc2edb fname.txt fname.root)" << endl;
    cout <<   "          -asum         = encode the sum of cluster areas in the segment puls" << endl;
    cout <<   "                          (puls = (sum of clust areas)*1000 + (number of grains)" << endl;
    return 0;
  };
  
  bool testrun = false;
  bool txtrun = false ;
  if(rwcname[strlen(rwcname)-1]=='d') testrun=true;  //if we have .rwd first 
  if(rwcname[strlen(rwcname)-1]=='t') txtrun =true;  //if we have .txt first 
  
  EdbRun* outrun;
  outrun = new EdbRun(edbname,"CREATE");

  if(testrun) 
    {
      // Add RWD 
      cout<<"TestRun"<<endl;
      int fragID = 0;
      AddRWD(outrun, rwcname, fragID, options);
      
      if (addgrs)  // Add Clusters file (TXT) ....
	{
	  char grsoutname[256];
	  strncpy( grsoutname + strlen(rwcname)-3, "grs", 3);
	  sprintf( grsoutname,"%s.root",grsoutname);
	  
	  EdbRun* outrun2;
	  outrun2 = new EdbRun(grsoutname,"CREATE");
	  
	  AddGrainsTXT(outrun2,grsname);
	  outrun2->Print();
	  outrun2->Close();
	}
    }
  else if(txtrun)
    {
      // Add clusters file (TXT) ....
      AddGrainsTXT(outrun,rwcname);
    }
  else 
    {
      // Add RWC and all RWDs
      printf("options 1: %s\n", options);
      AddRWC(outrun, rwcname, true,options);
      if (addmap) AddMAP(outrun, mapname);
    }
  outrun->Print();
  outrun->Close();

  if (testrun) return 0;
}
