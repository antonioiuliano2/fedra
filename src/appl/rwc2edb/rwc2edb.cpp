// rwc2edb.cpp
// convert rwc, rwd and grains dump files to root using the libEdb library
// author: Gabriele Sirri
//______________________________________________________________________________
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
	
	if (argc < 3)
	{
		cout<< "usage:\nrwc2edb <input file (.rwc)> <output file (.root)>"<<endl;
		cout<< "rwc2edb <input file (.rwc)> <output file (.root)> <marks file (.map)>"<<endl;
		cout<< "rwc2edb <input file (.rwd)> <output file (.root)> <grains text file (.txt)>"<<endl;
		cout<< "rwc2edb <grains text file (.txt)> <output file (.root)> "<<endl;

		return 0;
	};
	char* rwcname = argv[1];
	char* edbname = argv[2];
	char* auxname = argv[3];
	bool testrun = FALSE;
	bool txtrun = FALSE ;
	if(rwcname[strlen(rwcname)-1]=='d') testrun=TRUE; //if we have .rwd first 
	if(rwcname[strlen(rwcname)-1]=='t') txtrun =TRUE;  //if we have .txt first 

   EdbRun* outrun;
	outrun = new EdbRun(edbname,"CREATE");

	if(testrun) 
	{
		// Add RWD 
		cout<<"TestRun"<<endl;
		int fragID = 0;
		AddRWD(outrun, rwcname, fragID);

		if (auxname)  // Add Clusters file (TXT) ....
		{
			char* grsname;
			grsname = _strdup(rwcname);
			strncpy( grsname + strlen(rwcname)-3, "grs", 3);
			sprintf(grsname,"%s.root",grsname);

			EdbRun* outrun2;
			outrun2 = new EdbRun(grsname,"CREATE");

			AddGrainsTXT(outrun2,auxname);
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
		AddRWC(outrun, rwcname, TRUE);
		if (auxname) AddMAP(outrun, auxname);
	}
	outrun->Print();
	outrun->Close();

	if (testrun) 
			
	return 0;
};
