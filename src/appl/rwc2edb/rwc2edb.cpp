#include <stdio.h>
#include <windows.h>
#include <vector>

#include "id.h"
#include "Track.h"
#include "TVectors.h"
#include "Vectors.h"
#include "VSRawData.h"
#include "vscan_ds.h"
#include "SySalDataIO.h"
#include <iostream.h>

#include <EdbRun.h>
#include <EdbRunHeader.h>
#include <EdbView.h>
#include <EdbCluster.h> // Clusters
#include <EdbSegment.h> // Segments and Tracks

using namespace std ;

//**********************************************************//
double FindConfig(IO_VS_Catalog* pCat ,char* ConfigName, char* ConfigItem);
//**********************************************************//

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		cout<< "usage: rwc2edb <input file (.rwc)> <output file (.root)>"<<endl<<"or"<<endl;
		cout<< "usage: rwc2edb <input file (.rwd)> <output file (.root)> <input file (.txt)>"<<endl<<"or"<<endl;
		cout<< "usage: rwc2edb <input file (.rwc)> <output file (.root)> <input file (.tlg)> "<<endl;
		return 0;
	};
	char* rwcname = argv[1];
	char* edbname = argv[2];
	char* tlgname = argv[3];
	bool testrun = FALSE;
	FILE* grfile;
	
//	if(argc==4) Nfrags=atoi(tlgname); //Not the name, but number of fragments to convert
// ISySalDataIO variables
   ISySalDataIO *iIO = 0;
   CoInitialize(NULL);
   CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, IID_ISySalDataIO, (void **)&iIO);
	IO_VS_Catalog *pCat = 0;
	IO_VS_Fragment *pFrag = 0;
	Track* rwdTrack;
	UINT ExtErrorInfo;
    if(rwcname[strlen(rwcname)-1]=='d') testrun=TRUE; //if we have .rwd first
//try {

	if(testrun) {
		printf("Testrun!\n");
	 IO_VS_Catalog Cat;
	 pCat=&Cat;
     pCat->Hdr.ID.Part[0]=0;
     pCat->Hdr.ID.Part[1]=0;
     pCat->Hdr.ID.Part[2]=0;
     pCat->Hdr.ID.Part[3]=0;
	 pCat->Area.Fragments=1;
	 if(tlgname) grfile=fopen(tlgname,"r");
	}
	else 
	if (iIO->Read(NULL, (BYTE *)&pCat, &ExtErrorInfo, (UCHAR *)rwcname) != S_OK) throw 0;
	
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
                                  //    eFlag[1] = 2  - Absolute (fiducial) coordinates
	Header->SetFlag(0,2);  
	Header->SetFlag(1,FindConfig(pCat,"Flexible Sheet Map","FramesPerSecond")?2:1); 
	Header->SetLimits(pCat->Area.XMin,pCat->Area.XMax,pCat->Area.YMin,pCat->Area.YMax);
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

	EdbView *edbView;
	edbView= outrun->GetView();
	
	EdbSegment* edbSegment = new EdbSegment(0,0,0,0,0,0,0,0);
	

	int f, v, s, t, p;  //f=fragment, v=view, s=side, t=track, p=point
	int ntracks =0;
	int nclusters=0; 
	int nviews=0;
//	printf("Fragments to process %d\n",Nfrags);
//	if(Nfrags>0) tlgname=0;
//	if(Nfrags<=0) Nfrags=pCat->Area.Fragments;
	for (f = 0; f < pCat->Area.Fragments; f++)
	{
		int tracks    = 0   ; // number of tracks
		int clusters  = 0   ; // number of clusters
		// build rwd name 
		char *rwdname;
		rwdname = _strdup( rwcname );
		strncpy( rwdname + strlen(rwdname)-1, "d", 1 );
		sprintf(rwdname,"%s.%08X", rwdname, f + 1);
		
		if (iIO->Read(NULL, (BYTE *)&pFrag, &ExtErrorInfo, (UCHAR *)rwdname) != S_OK)	throw 1;

		for (s = 0; s < 2; s++)
		{
			for (v = 0; v < pFrag->Fragment.CountOfViews; v++)
			{ 
				VS_View* rwdView = &(pFrag->Fragment.pViews[v]);
				nviews++;
   				int vclusters=0;
				tracks += rwdView->TCount[s];

				int dz = (s==0?
				rwdView->RelevantZs.TopExt-rwdView->RelevantZs.TopInt:
				rwdView->RelevantZs.BottomInt-rwdView->RelevantZs.BottomExt );

				edbView->Clear();
				EdbViewHeader* edbViewHeader = edbView->GetHeader();
				edbViewHeader->SetAffine(rwdView->ImageMat[s][0][0],
												 rwdView->ImageMat[s][0][1],
												 rwdView->ImageMat[s][1][0],
												 rwdView->ImageMat[s][1][1],
												 0,0);
				edbViewHeader->SetAreaID(f);
				edbViewHeader->SetCoordXY(rwdView->MapX[s], rwdView->MapY[s]); 
				edbViewHeader->SetCoordZ(rwdView->RelevantZs.TopExt,rwdView->RelevantZs.TopInt,
								rwdView->RelevantZs.BottomInt,rwdView->RelevantZs.BottomExt);

				if(s==1)	edbViewHeader->SetNframes(0,FindConfig(pCat,"Vertigo Scan","VLayers"));
				else     edbViewHeader->SetNframes(FindConfig(pCat,"Vertigo Scan","VLayers"),0);
				edbViewHeader->SetNsegments(rwdView->TCount[s]);
				edbViewHeader->SetViewID(v);
//				edbViewHeader->SetZlevels(rwdView->Layers[s].Count,rwdView->Layers[s].pZs);
				for( int nlvl=0; nlvl<rwdView->Layers[s].Count; nlvl++ )
					edbView->AddFrame(nlvl,rwdView->Layers[s].pZs[nlvl]);

				int nclu = 0;	//number of clusters
				for (t = 0; t < rwdView->TCount[s]; t++)
				{
					rwdTrack = &(rwdView->pTracks[s][t]);
					int id = ntracks;
					
//					edbSegment->Set(rwdTrack->Intercept.X,
//									rwdTrack->Intercept.Y,
//									rwdTrack->Intercept.Z,
//									rwdTrack->Slope.X,
//									rwdTrack->Slope.Y, 
//									dz, s , rwdTrack->PointsN ,id);

					edbSegment->Set(rwdView->ImageMat[s][0][0]*rwdTrack->Intercept.X 
						            +rwdView->ImageMat[s][0][1]*rwdTrack->Intercept.Y,
										 rwdView->ImageMat[s][1][0]*rwdTrack->Intercept.X 
						            +rwdView->ImageMat[s][1][1]*rwdTrack->Intercept.Y,
										 rwdTrack->Intercept.Z,
										 rwdView->ImageMat[s][0][0]*rwdTrack->Slope.X 
						            +rwdView->ImageMat[s][0][1]*rwdTrack->Slope.Y,
										 rwdView->ImageMat[s][1][0]*rwdTrack->Slope.X 
						            +rwdView->ImageMat[s][1][1]*rwdTrack->Slope.Y,
									    dz, s , rwdTrack->PointsN ,id);

					edbSegment->SetSigma(rwdTrack->Sigma,-999);

					float xc=-100, yc=-200, zc=-300, arc=0, volc=0;
					int   ifrc=0, sidec=0;

					for ( p=0; p<rwdTrack->PointsN;p++)
					{
//						xc = rwdTrack->pPoints[p].X;
//						yc = rwdTrack->pPoints[p].Y;
						xc = rwdView->ImageMat[s][0][0]*rwdTrack->pPoints[p].X
							 +rwdView->ImageMat[s][0][1]*rwdTrack->pPoints[p].Y;
						yc = rwdView->ImageMat[s][1][0]*rwdTrack->pPoints[p].X
							 +rwdView->ImageMat[s][1][1]*rwdTrack->pPoints[p].Y;
						zc = rwdTrack->pPoints[p].Z;

						edbView->AddCluster(xc,yc,zc, arc, volc, ifrc, sidec, id);
						clusters++;
						nclu++;
					}
					edbViewHeader->SetNclusters(nclu);
					edbView->AddSegment(edbSegment) ;
					ntracks++;
				} // end of tracks (t)
				


				outrun->AddView(edbView);
			}; //end of views (v) 
		};//end of sides (s)
		cout<<"Fragment: "<<f<<"/"<<pCat->Area.Fragments<<"  microtracks: "<<tracks<<"\tclusters: "<<clusters<<endl;	
		cout << flush;
		CoTaskMemFree(pFrag);
		pFrag = 0;
	}; //end of fragments (f)


	cout <<endl;
	cout<<" Total: "<<nviews<<" views\t"<<endl<<endl;;
	if (pCat) CoTaskMemFree(pCat);
	if (pFrag) CoTaskMemFree(pFrag);

	int gs,gf,gTot,gn; //grains: view side frame Total index size
	float gX,gY,gZ,ga,gv;

	if(tlgname) if(testrun){ //read .txt grains dump file
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
        printf("Reading grains from %s..\n",tlgname);
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
	{ //read .tlg file
		// ISySalDataIO variables
		IO_Data *pTracks = 0;

		if (iIO->Read(0, (BYTE *)&pTracks,  &ExtErrorInfo,  (UCHAR *)tlgname ) ) throw 3;
		if (pTracks->Hdr.InfoType != (CS_SS_HEADER_BYTES | CS_SS_TRACK_SECTION) || 
										pTracks->Hdr.HeaderFormat != CS_SS_HEADER_TYPE) 
		{
			CoTaskMemFree(pTracks);
			throw 4;
		};

		int nlinked = pTracks->STks.Hdr.LCount; cout << nlinked << " tracks\n";

		EdbView       *View;
		View= outrun->GetView();
  		//View->SetHeader(0,0,0,0,0,50,250,300,nplan,nplan);
		EdbTrack* edbTrack = new EdbTrack(0,0,0,0,0,0);
		
		for (int j = 0; j < nlinked; j++)
		{
			edbTrack->Set(	pTracks->STks.pLinked[j].Intercept.X,
								pTracks->STks.pLinked[j].Intercept.Y,
								pTracks->STks.pLinked[j].PointsN,
								pTracks->STks.pLinked[j].Slope.X,
								pTracks->STks.pLinked[j].Slope.Y,
								pTracks->STks.pLinked[j].Sigma,
								j);

		//	edbTrack->AddElement(edbSegment);
		//	edbTrack->AddElement(edbSegment);

			View->AddTrack(edbTrack) ;
				//printf("%d\n", j);
		}
		cout<<edbTrack->GetTx()<<" "<<edbTrack->GetTy()<<" "<<edbTrack->GetNelements()<<endl;
		outrun->AddView(View);
		CoTaskMemFree(pTracks);
	}
	iIO->Release();
	CoUninitialize();

	outrun->Print();
	outrun->Close();
	return 0;

	// } catch (...) { return 0; };
};

#include "SySalDataIO_i.c"


//**********************************

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




