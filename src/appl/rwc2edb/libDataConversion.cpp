// libIOConverter.h and libIOConverter.cpp
// library for win32 which converts data from the DAQ files to root using libEdb
// (was rwc2edb.cpp)
//
// Revision 1.1 Jul 08, 2003
// -> Initial revision (adapted from rwc2edb.cpp 1.7)


#include "libDataConversion.h"

//______________________________________________________________________________
// FindConfig: search for a Name and an Item inside the configurations (rwc) and
// return the value as double

double FindConfig(IO_VS_Catalog* pCat, char* ConfigName, char* ConfigItem)
{
	int CountOfConfigs = pCat->Config.CountOfConfigs;
	VS_Config* pConfigs =pCat->Config.pConfigs;

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
int AddRWC(EdbRun* run, char* rwcname, int bAddRWD)
{
	ISySalDataIO*  iIO;
   ISySalDataIO2* iIO2;
   CoInitialize(NULL);
   CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
		              IID_ISySalDataIO, (void **)&iIO);
	iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);
	UINT ExtErrorInfo;
	
	IO_VS_Catalog* pCat = 0;

	if (iIO2->Read2(NULL, (BYTE*)&pCat, &ExtErrorInfo, (UCHAR*)rwcname) != S_OK) 
		throw 0;

	int nFragments = pCat->Area.Fragments;
	cout<<"Hdr.ID :"
	    <<pCat->Hdr.ID.Part[0] <<"\t"
	    <<pCat->Hdr.ID.Part[1]<<"\t"
	    <<pCat->Hdr.ID.Part[2]<<"\t"
	    <<pCat->Hdr.ID.Part[3]<<endl;
	cout<<endl<<"Fragments: "<<nFragments<<endl;

	EdbRunHeader *Header;
	Header =  run->GetHeader();
								       // customize run flags:
                               //  eFlag[0] = 1  - UTS data
                               //  eFlag[0] = 2  - SySal data
                               //  eFlag[1] = 1  - Stage coordinates
                               //  eFlag[1] = 2  - Absolute (marks) coordinates
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

	if (pCat) CoTaskMemFree(pCat);
	pCat=0;
	iIO2->Release();
	iIO->Release();
	CoUninitialize();

// loop on rwd files
	if(bAddRWD)
	{
		for (int f = 1; f < nFragments+1; f++)
		{
			// build rwd name 
			char* rwdname=new char[strlen(rwcname)+16];
			strcpy( rwdname,rwcname );
			strncpy( rwdname + strlen(rwdname)-1, "d", 1 );
			sprintf(rwdname,"%s.%08X", rwdname, f);

			cout <<"(tot. fragm.:"<<nFragments<<")  ";
			if (! AddRWD(run, rwdname,f) ) break;
			delete[] rwdname;
		}; //end of fragments (f)
		cout <<endl;
	}
	return TRUE;
}

//______________________________________________________________________________
int AddRWD(EdbRun* run, char* rwdname, int fragID)
{
	EdbView*    edbView = run->GetView();
	EdbSegment* edbSegment = new EdbSegment(0,0,0,0,0,0,0,0);

	// ISySalDataIO variables
	ISySalDataIO*  iIO;
	ISySalDataIO2* iIO2;
	CoInitialize(NULL);
	CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
		              IID_ISySalDataIO, (void **)&iIO);
	iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);
	UINT ExtErrorInfo;
	
	IO_VS_Fragment2* pFrag = 0;
	
	if (iIO2->Read2(NULL, (BYTE*)&pFrag, &ExtErrorInfo, (UCHAR*)rwdname) 
		!= S_OK)	return FALSE;

	int v, s, t, p;  // v=view, s=side, t=track, p=point
	int tracks;		// number of tracks in the fragment
	int fclusters;	// number of clusters in the fragment 
	int vclusters;	// number of clusters in the view
	float dz	;		// z-length of the track segment
	int tr_clusters;		// number of cluster of the track


	Track2* rwdTrack;
	VS_View2* rwdView;

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
			edbViewHeader->SetAreaID(fragID);
			edbViewHeader->SetCoordXY(rwdView->X[s], rwdView->Y[s]); 
			edbViewHeader->SetCoordZ(rwdView->RelevantZs.TopExt,rwdView->RelevantZs.TopInt,
							rwdView->RelevantZs.BottomInt,rwdView->RelevantZs.BottomExt);

			if(s)	edbViewHeader->SetNframes(0,rwdView->Layers[s].Count); // s==0 top , s==1 bottom
			else	edbViewHeader->SetNframes(rwdView->Layers[s].Count,0);

			edbViewHeader->SetNsegments(rwdView->TCount[s]);
			edbViewHeader->SetViewID(v);

			for( int nlvl=0; nlvl<rwdView->Layers[s].Count; nlvl++ )
				edbView->AddFrame(nlvl,rwdView->Layers[s].pLayerInfo[nlvl].Z,
											  rwdView->Layers[s].pLayerInfo[nlvl].Clusters);


			for (t = 0; t < rwdView->TCount[s]; t++)
			{
				rwdTrack = &(rwdView->pTracks[s][t]);
				tr_clusters = rwdTrack->Grains;
				dz = rwdTrack->pGrains[0].Z - rwdTrack->pGrains[tr_clusters-1].Z;
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
			run->AddView(edbView);
			fclusters += vclusters;
		}; //end of views (v)
	};//end of sides (s)

	cout<<"Fragment:"<<fragID<<"\tmicrotracks: "
		 <<tracks<<"\tclusters: "<<fclusters<<endl;
	cout << flush;

	CoTaskMemFree(pFrag);
	pFrag = 0;	
	iIO2->Release();
	iIO->Release();
	CoUninitialize();
	return TRUE;
}

//______________________________________________________________________________
int AddMAP(EdbRun* run, char* mapname)
{
	// ISySalDataIO variables
	ISySalDataIO*  iIO;
   ISySalDataIO2* iIO2;
   CoInitialize(NULL);
   CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
		              IID_ISySalDataIO, (void **)&iIO);
	iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);
	UINT ExtErrorInfo;
	
	IO_Data *pMarks = 0;
	if( iIO->Read(0, (BYTE*)&pMarks,  &ExtErrorInfo,  (UCHAR*) mapname ) ) 
		throw 3;

	EdbMarksBox* stage = run->GetMarks()->GetStage() ;
	EdbMarksBox* abs   = run->GetMarks()->GetAbsolute();
	
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

	CoTaskMemFree(pMarks);
	pMarks=0;
	iIO2->Release();
	iIO->Release();
	CoUninitialize();
	return TRUE;
}

//______________________________________________________________________________
int AddGrainsTXT(EdbRun* run, char* txtname)
{
	printf("Testrun!\n");
	FILE* grfile;
	grfile=fopen(txtname,"r");

	int gs,gf,gTot,gn;				 //grains: view side frame Total index size
	float gX,gY,gZ,ga,gv;

		char instr[128];
		EdbView *View;
		View=run->GetView();
		View->Clear();
		float curv=0;
		int curs=1;
		int ngr=0;
		if(fgets(instr,128,grfile)!=NULL)		//get initial side and viewID
		{ 
		  sscanf(instr,"%f %d %d %f %d %d %f %f %f",&gv,&gs,&gf,&gZ,&gTot,&gn,&gX,&gY,&ga);
		}
		curs=gs;
		curv=gv;
		fseek(grfile,0,0);
        printf("Reading grains from %s..\n",txtname);
		printf("Filling view %d side %d..",int(curv),curs);
		while(fgets(instr,128,grfile)!=NULL)
		{
		  sscanf(instr,"%f %d %d %f %d %d %f %f %f",&gv,&gs,&gf,&gZ,&gTot,&gn,&gX,&gY,&ga);
		  if((gv!=curv)||(gs!=curs))			//new view found
		  {
            View->GetHeader()->SetViewID(curv);
			View->GetHeader()->SetNframes(curs,1-curs);
			View->GetHeader()->SetNclusters(ngr);
			View->GetHeader()->SetNsegments(-1);//grain view
            run->AddView(View);

			View->Clear();
			curv=gv;
			curs=gs;
			printf("%d clusters.\nFilling view %d side %d..",ngr,int(curv),curs);
			ngr=0;
		  }
          View->AddCluster(gX,gY,gZ,ga,gv,gf,gs,-2);
		  ngr++;
		}

        View->GetHeader()->SetViewID(curv);
		View->GetHeader()->SetNframes(curs,1-curs);
		View->GetHeader()->SetNclusters(ngr);
		View->GetHeader()->SetNsegments(-1);	//grain view
        run->AddView(View);						//the last view

		printf("%d clusters.\n",ngr);
		fclose(grfile);

	return TRUE;
}

//______________________________________________________________________________
int AddTLG(EdbRun* run, char* tlgname)
{
	ISySalDataIO*  iIO;
   ISySalDataIO2* iIO2;
   CoInitialize(NULL);
   CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
		              IID_ISySalDataIO, (void **)&iIO);
	iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);
	UINT ExtErrorInfo;

	IO_Data *pTracks = 0;
	if (iIO->Read(0, (BYTE *)&pTracks,  &ExtErrorInfo,  (UCHAR *)tlgname) ) throw 3;
	if (pTracks->Hdr.InfoType != (CS_SS_HEADER_BYTES | CS_SS_TRACK_SECTION) || 
									pTracks->Hdr.HeaderFormat != CS_SS_HEADER_TYPE) 
	{
		CoTaskMemFree(pTracks);
		throw 4;
	};
	Track* tlgTrack;
	int nlinked = pTracks->STks.Hdr.LCount; cout << nlinked << " tracks\n";

	EdbView       *View;
	View= run->GetView();
	EdbTrack* edbTrack = new EdbTrack(0,0,0,0,0,0);

	for (int j = 0; j < nlinked; j++)
	{
		tlgTrack = pTracks->STks.pLinked[j].pTracks[0];

		edbTrack->Set(tlgTrack->Intercept.X,tlgTrack->Intercept.Y,tlgTrack->Intercept.Z,
			  tlgTrack->Slope.X,tlgTrack->Slope.Y, 5);
		View->AddTrack(edbTrack) ;
	}

	run->AddView(View);

	CoTaskMemFree(pTracks);
	pTracks = 0;
	iIO2->Release();
	iIO->Release();
	CoUninitialize();
	return TRUE;
	return 0;
}


#include "SySalDataIO_i.c"
