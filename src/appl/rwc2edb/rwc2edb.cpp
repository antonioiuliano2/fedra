// rwc2edb 
// convert rwc and rwd files to root using the libEdb library
// ----------------------------------------------------------
//
// Revision 1.6 Jul  3,2003
// -> IO_VS_Fragment,Track,VS_View changed to IO_VS_Fragment2,Track2,VS_View2 (SySalDataIO2) 
// -> stage/view coordinates instead of marks coordinates
// -> dz definition bug fixed
// -> .tlg option removed
// -> .map file can be read 
// -> number of clusters per layer
// 
// Revision 1.5 Jul  2,2003
// -> eNframesTop and eNframesBot are flipped
//
// Revision 1.4 Jun 27, 2003
// -> rwc2edb modified by I.Kreslo to read grains dump
//
// Revision 1.3 Jun 20, 2003
// -> cluster cycle optimized
// -> Header->SetArea(..) modified
//
// Revision 1.2 Jun 18, 2003 
// -> removed memory leak in fragment loop
//
// Revision 1.1.1.1 May 30, 2003
// -> Framework for Emulsions Data Reconstruction and Analysis
//
// Revision 1.1 May 30, 2003
// -> Initial revision 


//#include <stdio.h>
//#include <vector>
#include <windows.h>
#include <iostream>
#include <math.h>
#include <fstream>

//#include "id.h"
//#include "Track.h"
//#include "TVectors.h"
//#include "Vectors.h"
//#include "VSRawData.h"
#include "vscan_ds.h"
#include "SySalDataIO.h"

#include <EdbRun.h>
#include <EdbRunHeader.h>
#include <EdbView.h>
#include <EdbCluster.h>
#include <EdbSegment.h>

using namespace std ;

//______________________________________________________________________________

// FindConfig: search for a Name and an Item inside the configurations and
// return the value as double
double FindConfig(IO_VS_Catalog* pCat, char* ConfigName, char* ConfigItem)
{
	int CountOfConfigs = pCat->Config.CountOfConfigs;
	VS_Config *pConfigs =pCat->Config.pConfigs;

	double dbl;
	for (int j=0;j<CountOfConfigs;j++)
	{
		if(! strcmp(pConfigs[j].Config.Name, ConfigName) ) 
			for(int k=0;k<pConfigs[j].Config.CountOfItems;k++) 
			{
				if(! strcmp(&pConfigs[j].Config.pItems[k*128],ConfigItem) )
				{
					char* stopchar;
					dbl = strtod(&pConfigs[j].Config.pItems[k*128+64],&stopchar);
					break;
				}
			}
	}
	return dbl;
}
//______________________________________________________________________________

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cout<< "usage: rwc2edb <input file (.rwc)> <output file (.root)>"<<endl<<"or"<<endl;
		cout<< "usage: rwc2edb <input file (.rwc)> <output file (.root)> <marks file (.map)>"<<endl<<"or"<<endl;
		cout<< "usage: rwc2edb <input file (.rwd)> <output file (.root)> <input file (.txt)>"<<endl;
		return 0;
	};
	char* rwcname = argv[1];
	char* edbname = argv[2];
	char* mapname = argv[3];
	bool testrun = FALSE;
	FILE* grfile;

	// ISySalDataIO variables
	ISySalDataIO  *iIO;
   ISySalDataIO2 *iIO2;
   CoInitialize(NULL);
   CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
		              IID_ISySalDataIO, (void **)&iIO);
	iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);
	UINT ExtErrorInfo;
	
	IO_VS_Catalog *pCat = 0;
	IO_VS_Fragment2 *pFrag = 0;
	Track2* rwdTrack;
	VS_View2* rwdView;

	if(rwcname[strlen(rwcname)-1]=='d') testrun=TRUE; //if we have .rwd first
	
//try {
	if(testrun) 
	{
	  printf("Testrun!\n");
	  IO_VS_Catalog Cat;
	  pCat=&Cat;
     pCat->Hdr.ID.Part[0]=0;
     pCat->Hdr.ID.Part[1]=0;
     pCat->Hdr.ID.Part[2]=0;
     pCat->Hdr.ID.Part[3]=0;
	  pCat->Area.Fragments=1;
	  if(mapname) grfile=fopen(mapname,"r");
	}
	else 
	{
		if (iIO2->Read2(NULL, (BYTE *)&pCat, &ExtErrorInfo, (UCHAR *)rwcname) != S_OK) 
		throw 0;
	}
	
	cout<<"Hdr.ID :"<<pCat->Hdr.ID.Part[0]<<"\t"<<pCat->Hdr.ID.Part[1]<<"\t"
						 <<pCat->Hdr.ID.Part[1]<<"\t"<<pCat->Hdr.ID.Part[3]<<endl;
	cout<<endl<<"Fragments: "<<pCat->Area.Fragments<<endl;

//Edb variables
	EdbRun *outrun;
	outrun =new EdbRun(edbname,"CREATE");
//	outrun->SetTitle("first acq");
//	outrun->SetComment("First");

	EdbRunHeader *Header;
	Header =  outrun->GetHeader();
								          //  customize run flags:
                                  //    eFlag[0] = 1  - UTS data
                                  //    eFlag[0] = 2  - SySal data
                                  //    eFlag[1] = 1  - Stage coordinates
                                  //    eFlag[1] = 2  - Absolute (marks) coordinates
	Header->SetFlag(0,2);  
	Header->SetFlag(1,1);
	Header->SetLimits(pCat->Area.XMin,pCat->Area.XMax,
		               pCat->Area.YMin,pCat->Area.YMax);
	// fiducial coordinates
	Header->SetArea(pCat->Area.XViews*pCat->Area.YViews, 
						 pCat->Area.XStep,pCat->Area.YStep, 
						 FindConfig(pCat,"Vertigo Scan","VLayers"),
						 FindConfig(pCat,"Vertigo Scan","VLayers"),
						 0);

	Header->SetNareas(pCat->Area.Fragments);
	Header->SetCCD(FindConfig(pCat,"Objective","Width"),
						FindConfig(pCat,"Objective","Height"),
						-999,						// physical pixel size in microns along X
						-999,						// physical pixel size in microns along Y
						"",						// es. "Dalsa CAD4"
						"");						// es. "Bologna"
	Header->SetObjective(-999,				// magnification
		                  FindConfig(pCat,"Objective","Width" )*fabs(FindConfig(pCat,"Objective","PixelToMicronX")),0,
								0,FindConfig(pCat,"Objective","Height")*fabs(FindConfig(pCat,"Objective","PixelToMicronY")),
								0,0,
								"",				// es. "Nikon CFI - oil"
								"");				// es, "50x"
	Header->SetPlate(-999,					// plate ID
						  FindConfig(pCat,"Vertigo Scan","VStep")*FindConfig(pCat,"Vertigo Scan","Shrinkage"),
						  FindConfig(pCat,"Vertigo Scan","BaseThickness"),
						  FindConfig(pCat,"Vertigo Scan","VStep")*FindConfig(pCat,"Vertigo Scan","Shrinkage"),
						  FindConfig(pCat,"Vertigo Scan","Shrinkage"),
						  FindConfig(pCat,"Vertigo Scan","Shrinkage"),
						  "",						// es. "Test Plate"
						  "");					//


	EdbView *edbView = outrun->GetView();
	EdbSegment* edbSegment = new EdbSegment(0,0,0,0,0,0,0,0);

	int VLayers = FindConfig(pCat,"Vertigo Scan","VLayers");
	float* pZlevels   = new float[256];
	float* pZclusters = new float[256];

	int f, v, s, t, p, l;  //f=fragment, v=view, s=side, t=track, p=point, l=layer
	int tracks;		// number of tracks in the fragment
	int fclusters;	// number of clusters in the fragment 
	int vclusters;	// number of clusters in the view
	float dz	;		// z-length of the track segment
	int tr_clusters;		// number of cluster of the track

	for (f = 1; f < pCat->Area.Fragments+1; f++)
	{
		// build rwd name 
		char *rwdname;
		rwdname = _strdup( rwcname );
		strncpy( rwdname + strlen(rwdname)-1, "d", 1 );
		sprintf(rwdname,"%s.%08X", rwdname, f);
		printf("open rwd file: %s\n",rwdname);
		
		if (iIO2->Read2(NULL, (BYTE *)&pFrag, &ExtErrorInfo, (UCHAR *)rwdname) != S_OK)	throw 1;

		tracks = 0;
		fclusters = 0;
		for (s = 0; s < 2; s++)
		{
			for (v = 0; v < pFrag->Fragment.CountOfViews; v++)
			{ 
				rwdView = &(pFrag->Fragment.pViews[v]);
   			vclusters=0;
				tracks += rwdView->TCount[s];

				edbView->Clear();
				EdbViewHeader* edbViewHeader = edbView->GetHeader();
				edbViewHeader->SetAffine(rwdView->ImageMat[s][0][0],
												 rwdView->ImageMat[s][0][1],
												 rwdView->ImageMat[s][1][0],
												 rwdView->ImageMat[s][1][1],
												 rwdView->MapX[s], 
												 rwdView->MapY[s]); 
				edbViewHeader->SetAreaID(f);
				edbViewHeader->SetCoordXY(rwdView->X[s], rwdView->Y[s]); 
				edbViewHeader->SetCoordZ(rwdView->RelevantZs.TopExt,rwdView->RelevantZs.TopInt,
								rwdView->RelevantZs.BottomInt,rwdView->RelevantZs.BottomExt);

				if(s)	edbViewHeader->SetNframes(0,VLayers); // s==0 top , s==1 bottom
				else	edbViewHeader->SetNframes(VLayers,0);

				edbViewHeader->SetNsegments(rwdView->TCount[s]);
				edbViewHeader->SetViewID(v);


/*				// Z LEVELS
//          edbViewHeader->SetZlevels(rwdView->Layers[s].Count,
//												  rwdView->Layers[s].pZs);  
            for( int nlvl=0; nlvl<rwdView->Layers[s].Count; nlvl++ )  
                            edbView->AddFrame(nlvl,rwdView->Layers[s].pZs[nlvl]);  
				for(l=0;l<VLayers;l++)
				     { pZlevels[l] = rwdView->Layers[s].pLayerInfo[l].Z;}
				edbViewHeader->SetZlevels(VLayers,pZlevels);
//				edbViewHeader->SetZclusters(VLayers,pZclusters);		 //NEEDS MODIFICATION OF EDB ?SetZclusters()?
*/
				for( int nlvl=0; nlvl<rwdView->Layers[s].Count; nlvl++ )
					edbView->AddFrame(nlvl,rwdView->Layers[s].pLayerInfo[nlvl].Z,
										   rwdView->Layers[s].pLayerInfo[nlvl].Clusters);


				for (t = 0; t < rwdView->TCount[s]; t++)
				{
					rwdTrack = &(rwdView->pTracks[s][t]);
					tr_clusters = rwdTrack->Grains;
					dz = rwdTrack->pGrains[0].Z - rwdTrack->pGrains[tr_clusters].Z;
					edbSegment->Set(rwdTrack->Intercept.X,
										 rwdTrack->Intercept.Y,
										 rwdTrack->Intercept.Z,
										 rwdTrack->Slope.X,
										 rwdTrack->Slope.Y, 
									    dz, s , tr_clusters, t);
					edbSegment->SetSigma(rwdTrack->Sigma,-999);

					for ( p=0; p<tr_clusters;p++)
					{
						edbView->AddCluster(rwdTrack->pGrains[p].X,
										        rwdTrack->pGrains[p].Y,
												  rwdTrack->pGrains[p].Z,
												  rwdTrack->pGrains[p].Area,
												  0,0,s,t);										
						}
						
					edbViewHeader->SetNclusters(vclusters);
					edbView->AddSegment(edbSegment) ;
					vclusters += tr_clusters;
				} // end of tracks (t)
				outrun->AddView(edbView);
				fclusters += vclusters;
			}; //end of views (v)
		};//end of sides (s)

		cout<<"Fragment: "<<f<<"/"<<pCat->Area.Fragments<<"  microtracks: "
			 <<tracks<<"\tclusters: "<<fclusters<<endl;
		cout << flush;

		CoTaskMemFree(pFrag);
		pFrag = 0;

	}; //end of fragments (f)

	cout <<endl;
	if (pCat) CoTaskMemFree(pCat);
	if (pFrag) CoTaskMemFree(pFrag);


if(mapname)
{
	int gs,gf,gTot,gn; //grains: view side frame Total index size
	float gX,gY,gZ,ga,gv;

	if(testrun){ //read .txt grains dump file
		char instr[128];
		EdbView *View;
		View=outrun->GetView();
		View->Clear();
		float curv=0;
		int curs=1;
		int ngr=0;
		if(fgets(instr,128,grfile)!=NULL){ //get initial side and viewID
		  sscanf(instr,"%f %d %d %f %d %d %f %f %f",&gv,&gs,&gf,&gZ,&gTot,&gn,&gX,&gY,&ga);
		}
		curs=gs;
		curv=gv;
		fseek(grfile,0,0);
        printf("Reading grains from %s..\n",mapname);
		printf("Filling view %d side %d..",int(curv),curs);
		while(fgets(instr,128,grfile)!=NULL){
		  sscanf(instr,"%f %d %d %f %d %d %f %f %f",&gv,&gs,&gf,&gZ,&gTot,&gn,&gX,&gY,&ga);
		  if((gv!=curv)||(gs!=curs)){ //new view found
            View->GetHeader()->SetViewID(curv);
			View->GetHeader()->SetNframes(curs,1-curs);
			View->GetHeader()->SetNclusters(ngr);
			View->GetHeader()->SetNsegments(-1); //grain view

            outrun->AddView(View);
			View->Clear();
			curv=gv;
			curs=gs;
			printf("%d clusters.\nFilling view %d side %d..",ngr,int(curv),curs);
			ngr=0;
		  }
          View->AddCluster(gX,gY,gZ,ga,gv,gf,gs,-2);
		  ngr++;
		}
        outrun->AddView(View); //the last view
		printf("%d clusters.\n",ngr);
		fclose(grfile);

	}
	else
	{ //read .map file

		IO_Data *pMarks = 0;
		if( iIO->Read(0, (BYTE*)&pMarks,  &ExtErrorInfo,  (UCHAR *)mapname ) ) 
			throw 3;

		EdbMarksBox* stage = outrun->GetMarks()->GetStage() ;
		EdbMarksBox* abs   = outrun->GetMarks()->GetAbsolute();
		
		int nmarks = pMarks->MkMap.Map.Count ;
		for(int i=0;i<nmarks;i++)
		{
			float AbsX = pMarks->MkMap.Map.pMarks[i].Nominal.X;
			float AbsY = pMarks->MkMap.Map.pMarks[i].Nominal.Y;
			abs->AddMark(i,AbsX,AbsY);
			float StageX = pMarks->MkMap.Map.pMarks[i].Stage.X;
			float StageY = pMarks->MkMap.Map.pMarks[i].Stage.Y;
			stage->AddMark(i,StageX,StageY);
		}
	}
}

	iIO2->Release();
	iIO->Release();
	CoUninitialize();

	printf("\n*******************************\n");
	outrun->Print();
	outrun->Close();
	return 0;

	// } catch (...) { return 0; };
};

#include "SySalDataIO_i.c"
