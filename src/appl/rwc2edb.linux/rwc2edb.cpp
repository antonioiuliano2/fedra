// rwc2edb.cpp
// linux version of rwc2edb converter
// author: Gabriele Sirri
// porting to Linux: Igor Kreslo and Nikolay Savvinov
// October 15, 2004 

#include <stdio.h>
#include <iostream>
#include <string.h>
#include "rwc2edb.h"

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
//	char* auxname = argv[3];
	bool testrun = false;
	bool txtrun = false ;
	if(rwcname[strlen(rwcname)-1]=='d') testrun=true; //if we have .rwd first 
	if(rwcname[strlen(rwcname)-1]=='t') txtrun =true;  //if we have .txt first 

        EdbRun* run;
	run = new EdbRun(edbname,"CREATE");
        AddRWC(run,  rwcname, true);
	run->Print();								  run->Close();
	return 0;
};

char*  ED_COPY_AND_DISPLACE(void* dst, void* src, int size)
{
  memcpy(dst,src,size);
  return (char*)dst+size;
}

void TRY_READ_B(FILE* HFile, int br, void* dst, int size)
{
 fread(dst,1,size,HFile);
}
int AddRWC(EdbRun* run, char* rwcname, int bAddRWD)
{
	IO_VS_Catalog* pCat = 0;
	ReadCatalog((void**)&pCat,  (char*)rwcname); 
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
	int lys;
	lys=(int)FindConfig(pCat,"Vertigo Scan","VLayers");
	Header->SetArea(pCat->Area.XViews*pCat->Area.YViews, 
						 pCat->Area.XStep,pCat->Area.YStep, lys,lys,
						 0); 
	Header->SetNareas(pCat->Area.Fragments);
	Header->SetCCD((int) FindConfig(pCat,"Objective","Width"),
						(int) FindConfig(pCat,"Objective","Height"), 
						-999,		// physical pixel size in microns along X
						-999,		// physical pixel size in microns along Y
						"",			// es. "Dalsa CAD4"
						"");		// es. "Bologna" 
	Header->SetObjective(-999,		// magnification
				(float)FindConfig(pCat,"Objective","Width" )*
				    fabs((float)FindConfig(pCat,"Objective","PixelToMicronX")),
				0,0,
				(float)FindConfig(pCat,"Objective","Height")*
				    fabs((float)FindConfig(pCat,"Objective","PixelToMicronY")),
				0,0,
				"",					// es. "Nikon CFI - oil"
				"");				// es, "50x"
	Header->SetPlate(-999,			// plate ID
				(float)FindConfig(pCat,"Vertigo Scan","VStep")*
				       (float)FindConfig(pCat,"Vertigo Scan","Shrinkage"),
				(float)FindConfig(pCat,"Vertigo Scan","BaseThickness"),
				(float)FindConfig(pCat,"Vertigo Scan","VStep")*
				       (float)FindConfig(pCat,"Vertigo Scan","Shrinkage"),
				(float)FindConfig(pCat,"Vertigo Scan","Shrinkage"),
				(float)FindConfig(pCat,"Vertigo Scan","Shrinkage"),
				"",					// es. "Test Plate"
				"");				// 


// loop on rwd files
	if(bAddRWD)
	{
		for (int f = 1; f < nFragments+1; f++)
		{
			// build rwd name 
	        	char rwdname[256], temp[256];
			sprintf(temp, "%s", rwcname);
			sprintf(temp+strlen(temp)-1, "d");
			sprintf(rwdname, "%s.%08X", temp, f);
			if (! AddRWD(run, rwdname,f) ) break;
			//			delete[] rwdname;
		}; //end of fragments (f)
		cout <<endl;
	}
	return true;
}

int AddRWD(EdbRun* run, char* rwdname, int fragID)
{
	EdbView*    edbView = run->GetView();
	EdbSegment* edbSegment = new EdbSegment(0,0,0,0,0,0,0,0);

	IO_VS_Fragment2* pFrag = 0;
 	if (ReadFragment((void**)&pFrag, (char*)rwdname) != 1)	return false;

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

	delete pFrag;
	return true;
}

int ReadFragment(void** ppData,  char* name)
 {
 IO_Header Hdr;
 FILE *F,*HFile;
 F=fopen(name,"r");
 HFile=F;
 fread(&Hdr.InfoType, 1, sizeof(Hdr.InfoType), F);
 fread(&Hdr.HeaderFormat, 1, sizeof(Hdr.HeaderFormat), F);
 
 Identifier Id;
 fread(&Id.Part[0], 1, sizeof(Id.Part[0]), F);
 fread(&Id.Part[1], 1, sizeof(Id.Part[1]), F);
 fread(&Id.Part[2], 1, sizeof(Id.Part[2]), F);
 fread(&Id.Part[3], 1, sizeof(Id.Part[3]), F);
 
	IO_VS_Fragment2 Frag;
	Frag.Hdr.Type.InfoType = Hdr.InfoType;
	Frag.Hdr.Type.HeaderFormat = VS_HEADER_TYPE;
	Frag.Hdr.ID = Id;
	int br=0;
	char *p = 0, *pData = 0;
	VS_View2 *pViews = 0;
	char *pCorr = 0;
	VS_View2::t_LayerInfo *pLayers = 0;
	int TSpace = 0;
	int GSpace = 0;
	int LSpace = 0;
	int i, j, k;
		TRY_READ_B(HFile, br, &Frag.Fragment.Index, sizeof(Frag.Fragment.Index));
		TRY_READ_B(HFile, br, &Frag.Fragment.StartView, sizeof(Frag.Fragment.StartView));
		TRY_READ_B(HFile, br, &Frag.Fragment.CountOfViews, sizeof(Frag.Fragment.CountOfViews));
		TRY_READ_B(HFile, br, &Frag.Fragment.FitCorrectionDataSize, sizeof(Frag.Fragment.FitCorrectionDataSize));
		TRY_READ_B(HFile, br, &Frag.Fragment.CodingMode, sizeof(Frag.Fragment.CodingMode));
		TRY_READ_B(HFile, br, &Frag.Reserved, sizeof(Frag.Reserved));	

		switch (Frag.Fragment.CodingMode)
		{
			case VS_COMPRESSION_NULL:
			case VS_COMPRESSION_GRAIN_SUPPRESSION:	break;
			};

		pCorr = (char *)malloc(Frag.Fragment.FitCorrectionDataSize);
		pViews = (VS_View2 *)malloc(sizeof(VS_View2) * Frag.Fragment.CountOfViews);
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
			pViews[i].pTracks[0] = pViews[i].pTracks[1] = 0;
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
		{
			TRY_READ_B(HFile, br, &pViews[i].TileX, sizeof(pViews[i].TileX));		
			TRY_READ_B(HFile, br, &pViews[i].TileY, sizeof(pViews[i].TileY));		
			TRY_READ_B(HFile, br, &pViews[i].X[0], sizeof(pViews[i].X[0]));		
			TRY_READ_B(HFile, br, &pViews[i].X[1], sizeof(pViews[i].X[1]));		
			TRY_READ_B(HFile, br, &pViews[i].Y[0], sizeof(pViews[i].Y[0]));		
			TRY_READ_B(HFile, br, &pViews[i].Y[1], sizeof(pViews[i].Y[1]));		
			TRY_READ_B(HFile, br, &pViews[i].MapX[0], sizeof(pViews[i].MapX[0]));			       	   
			TRY_READ_B(HFile, br, &pViews[i].MapX[1], sizeof(pViews[i].MapX[1]));
			TRY_READ_B(HFile, br, &pViews[i].MapY[0], sizeof(pViews[i].MapY[0]));		
			TRY_READ_B(HFile, br, &pViews[i].MapY[1], sizeof(pViews[i].MapY[1]));		
			for (int i1=0; i1<2; i1++)
				for (int i2=0; i2<2; i2++)
					for (int i3=0; i3<2; i3++)
						TRY_READ_B(HFile, br, 
								&pViews[i].ImageMat[i1][i2][i3], sizeof(pViews[i].ImageMat[i1][i2][i3]));		

			for (j = 0; j < 2; j++)	TRY_READ_B(HFile, br, &pViews[i].Layers[j].Count, sizeof(pViews[i].Layers[j].Count));		
			TRY_READ_B(HFile, br, &pViews[i].RelevantZs.TopExt, sizeof(pViews[i].RelevantZs.TopExt));		
			TRY_READ_B(HFile, br, &pViews[i].RelevantZs.TopInt, sizeof(pViews[i].RelevantZs.TopInt));		
			TRY_READ_B(HFile, br, &pViews[i].RelevantZs.BottomExt, sizeof(pViews[i].RelevantZs.BottomExt));		
			TRY_READ_B(HFile, br, &pViews[i].RelevantZs.BottomInt, sizeof(pViews[i].RelevantZs.BottomInt));		
			for (j = 0; j < 2; j++) TRY_READ_B(HFile, br, &pViews[i].Status[j], sizeof(pViews[i].Status[j]));		
			for (j = 0; j < 2; j++) TRY_READ_B(HFile, br, &pViews[i].TCount[j], sizeof(pViews[i].TCount[j]));
			LSpace += pViews[i].Layers[0].Count + pViews[i].Layers[1].Count;
			TSpace += pViews[i].TCount[0] + pViews[i].TCount[1];
			};
		pLayers = (VS_View2::t_LayerInfo *)malloc(sizeof(VS_View2::t_LayerInfo) * LSpace);
		if (Hdr.HeaderFormat == VS_HEADER_TYPE) 
		{
			for (i = 0; i < LSpace; i++)
			{
				TRY_READ_B(HFile, br, &pLayers[i].Clusters, sizeof(pLayers[i].Z));
				TRY_READ_B(HFile, br, &pLayers[i].Z, sizeof(pLayers[i].Z));
			        }
			}
		else
			for (i = 0; i < LSpace; i++)
			{
				pLayers[i].Clusters = 0;
				TRY_READ_B(HFile, br, &pLayers[i].Z, sizeof(pLayers[i].Z));
				}
		TSpace = 0;
		if (Hdr.HeaderFormat == VS_HEADER_TYPE)
		{
			for (i = 0; i < Frag.Fragment.CountOfViews; i++)
				for (j = 0; j < 2; j++)
				{
					pViews[i].pTracks[j] = (Track2 *)malloc(sizeof(Track2) * pViews[i].TCount[j]);
					for (k = 0; k < pViews[i].TCount[j]; k++)
					{
						Track2 &t = pViews[i].pTracks[j][k];
						t.Field = i;
						TRY_READ_B(HFile, br, &t.AreaSum, sizeof(t.AreaSum));
						TRY_READ_B(HFile, br, &t.Grains, sizeof(t.Grains));
						TRY_READ_B(HFile, br, &t.Intercept.X, sizeof(t.Intercept.X));
						TRY_READ_B(HFile, br, &t.Intercept.Y, sizeof(t.Intercept.Y));
						TRY_READ_B(HFile, br, &t.Intercept.Z, sizeof(t.Intercept.Z));
						TRY_READ_B(HFile, br, &t.Slope.X, sizeof(t.Slope.X));
						TRY_READ_B(HFile, br, &t.Slope.Y, sizeof(t.Slope.Y));
						TRY_READ_B(HFile, br, &t.Slope.Z, sizeof(t.Slope.Z));
						TRY_READ_B(HFile, br, &t.Sigma, sizeof(t.Sigma));
						TRY_READ_B(HFile, br, &t.FirstZ, sizeof(t.FirstZ));
						TRY_READ_B(HFile, br, &t.LastZ, sizeof(t.LastZ));
						t.Valid = true;
						TSpace++;
						GSpace += t.Grains;
						};
					};
			}
		else
		{
			for (i = 0; i < Frag.Fragment.CountOfViews; i++)
				for (j = 0; j < 2; j++)
				{
					pViews[i].pTracks[j] = (Track2 *)malloc(sizeof(Track2) * pViews[i].TCount[j]);
					for (k = 0; k < pViews[i].TCount[j]; k++)
					{
						Track2 &t = pViews[i].pTracks[j][k];
						t.Field = i;
						t.AreaSum = 0;
						static TVector Dummy;
						TRY_READ_B(HFile, br, &t.Grains, sizeof(t.Grains));
						TRY_READ_B(HFile, br, &t.Intercept.X, sizeof(t.Intercept.X));
						TRY_READ_B(HFile, br, &t.Intercept.Y, sizeof(t.Intercept.Y));
						TRY_READ_B(HFile, br, &t.Intercept.Z, sizeof(t.Intercept.Z));
						TRY_READ_B(HFile, br, &t.Slope, sizeof(t.Slope));
						TRY_READ_B(HFile, br, &t.Sigma, sizeof(t.Sigma));
						TRY_READ_B(HFile, br, &t.FirstZ, sizeof(t.FirstZ));
						TRY_READ_B(HFile, br, &t.LastZ, sizeof(t.LastZ));
						TRY_READ_B(HFile, br, &Dummy, sizeof(Dummy));
						TRY_READ_B(HFile, br, &Dummy, sizeof(Dummy));
						t.Valid = true;
						if (Frag.Fragment.FitCorrectionDataSize)
						{
							TRY_READ_B(HFile, br, pCorr, Frag.Fragment.FitCorrectionDataSize);
							};
						TSpace++;
						GSpace += t.Grains;
						};
					};
			}

		p = pData = (char *)malloc((sizeof(Frag) + sizeof(VS_View2) * Frag.Fragment.CountOfViews +  (sizeof(VS_View2::t_LayerInfo) * LSpace) + 
			 (sizeof(Track2)) * TSpace + (( (Frag.Fragment.CodingMode == VS_COMPRESSION_GRAIN_SUPPRESSION)) ? 
			0 :  (sizeof(Grain) * GSpace))));

		p= ED_COPY_AND_DISPLACE(p, &Frag, sizeof(Frag));
		((IO_VS_Fragment2 *)pData)->Fragment.pViews = (VS_View2 *)p;
		p= ED_COPY_AND_DISPLACE(p, pViews, (sizeof(VS_View2) * Frag.Fragment.CountOfViews));
		LSpace = 0;
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
			for (j = 0; j < 2; j++)
			{
				((IO_VS_Fragment2 *)pData)->Fragment.pViews[i].Layers[j].pLayerInfo = (VS_View2::t_LayerInfo *)p + LSpace;
				LSpace += ((IO_VS_Fragment2 *)pData)->Fragment.pViews[i].Layers[j].Count;
				};
		p= ED_COPY_AND_DISPLACE(p, pLayers, sizeof(VS_View2::t_LayerInfo) * LSpace);
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
			for (j = 0; j < 2; j++)
			{
				((IO_VS_Fragment2 *)pData)->Fragment.pViews[i].pTracks[j] = (Track2 *)p;
				p= ED_COPY_AND_DISPLACE(p, pViews[i].pTracks[j], sizeof(Track2) * pViews[i].TCount[j]);
				};
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
			for (j = 0; j < 2; j++)
				for (k = 0; k < pViews[i].TCount[j]; k++)
					((IO_VS_Fragment2 *)pData)->Fragment.pViews[i].pTracks[j][k].pCorrection = 0;
		
		if (!((Frag.Fragment.CodingMode == VS_COMPRESSION_GRAIN_SUPPRESSION)))
		{
			if (Hdr.HeaderFormat == VS_HEADER_TYPE)	
			{

                                for (i=0; i<GSpace; i++) {
					Grain g;
					TRY_READ_B(HFile, br, &g, sizeof(g));
					((Grain *)p + i)->Area = g.Area;
					((Grain *)p + i)->X = g.X;
					((Grain *)p + i)->Y = g.Y;
					((Grain *)p + i)->Z = g.Z;
							}	
				}
			else
				for (i = 0; i < GSpace; i++)
				{
					((Grain *)p + i)->Area = 0;
					TRY_READ_B(HFile, br, &((Grain *)p + i)->X, sizeof(((Grain *)p + i)->X));
					TRY_READ_B(HFile, br, &((Grain *)p + i)->Y, sizeof(((Grain *)p + i)->Y));
					TRY_READ_B(HFile, br, &((Grain *)p + i)->Z, sizeof(((Grain *)p + i)->Z));
					};
			GSpace = 0;
			for (i = 0; i < Frag.Fragment.CountOfViews; i++)
				for (j = 0; j < 2; j++)
					for (k = 0; k < pViews[i].TCount[j]; k++)
					{
						((IO_VS_Fragment2 *)pData)->Fragment.pViews[i].pTracks[j][k].pGrains = (Grain *)(p + sizeof(Grain) * GSpace);
						GSpace += pViews[i].pTracks[j][k].Grains;						
						};
			}
		else
		{
			for (i = 0; i < Frag.Fragment.CountOfViews; i++)
				for (j = 0; j < 2; j++)
					for (k = 0; k < pViews[i].TCount[j]; k++)
						((IO_VS_Fragment2 *)pData)->Fragment.pViews[i].pTracks[j][k].pGrains = 0;					
		};
	if (pViews) 
	{
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
			for (j = 0; j < 2; j++)
				if (pViews[i].pTracks[j]) free(pViews[i].pTracks[j]);
		free(pViews);
		};
	if (pCorr) free(pCorr);
	if (pLayers) free(pLayers);
	*ppData = (IO_VS_Fragment2 *)pData;
//	free(pData);
	fclose(F);
return 1;
 }

int ReadCatalog(void** ppData,  char* name)
 {
 IO_Header Hdr;
 FILE* F;
 int i;

 F=fopen(name,"r");
 fread(&Hdr.InfoType,1,sizeof(Hdr.InfoType),F);
 fread(&Hdr.HeaderFormat,1,sizeof(Hdr.HeaderFormat),F);
 
 	IO_VS_Catalog Cat;
	Cat.Hdr.Type = Hdr;
	char *p = 0, *pData = 0;
	VS_Config *pConfigs = 0;
	int *pFragmentIndices = 0;
	unsigned int PSpace = 0;
		for (i = 0; i < 4; i++) fread(&Cat.Hdr.ID.Part[i], 1,sizeof(Cat.Hdr.ID.Part[i]), F);
		fread( &Cat.Area.XMin, 1,sizeof(Cat.Area.XMin), F);
		fread( &Cat.Area.XMax, 1,sizeof(Cat.Area.XMax), F);
		fread( &Cat.Area.YMin, 1,sizeof(Cat.Area.YMin), F);
		fread( &Cat.Area.YMax, 1,sizeof(Cat.Area.YMax), F);
		fread( &Cat.Area.XStep, 1,sizeof(Cat.Area.XStep), F);
		fread( &Cat.Area.YStep, 1,sizeof(Cat.Area.YStep), F);
		fread( &Cat.Area.XViews, 1,sizeof(Cat.Area.XViews), F);
		fread( &Cat.Area.YViews, 1,sizeof(Cat.Area.YViews), F);
		fread( &Cat.Area.Fragments, 1,sizeof(Cat.Area.Fragments), F);
		fread( &Cat.Config.CountOfConfigs, 1,sizeof(Cat.Config.CountOfConfigs), F);
 
		pConfigs = (VS_Config *)malloc((sizeof(VS_Config) * Cat.Config.CountOfConfigs));

		for (i = 0; i < Cat.Config.CountOfConfigs; i++)	pConfigs[i].Config.pItems = 0;
		for (i = 0; i < Cat.Config.CountOfConfigs; i++)
		{
			fread( &pConfigs[i].ClassName,1, sizeof(pConfigs[i].ClassName), F);
			fread( &pConfigs[i].Config.Name,1, sizeof(pConfigs[i].Config.Name), F);
			fread( &pConfigs[i].Config.CountOfItems,1, sizeof(pConfigs[i].Config.CountOfItems), F);
			pConfigs[i].Config.pItems = (char *)malloc((SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN) * pConfigs[i].Config.CountOfItems);
			PSpace+= fread( pConfigs[i].Config.pItems,1, ((SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN) * pConfigs[i].Config.CountOfItems), F);
		};

		pFragmentIndices = (int *)malloc(sizeof(int) * Cat.Area.XViews * Cat.Area.YViews);
		fread( pFragmentIndices, 1,(sizeof(int) * Cat.Area.XViews * Cat.Area.YViews),F);
		fread( &Cat.Reserved, 1,sizeof(Cat.Reserved), F);

		p = pData = (char *)malloc((sizeof(Cat) + sizeof(VS_Config) * Cat.Config.CountOfConfigs + PSpace + sizeof(int) * Cat.Area.XViews * Cat.Area.YViews));
		
		p= ED_COPY_AND_DISPLACE(p, &Cat, sizeof(Cat));
		((IO_VS_Catalog *)pData)->Config.pConfigs = (VS_Config *)p;
		for (i = 0; i < Cat.Config.CountOfConfigs; i++)
			p= ED_COPY_AND_DISPLACE(p, &pConfigs[i], sizeof(pConfigs[i]));
		for (i = 0; i < Cat.Config.CountOfConfigs; i++)
		{
			((IO_VS_Catalog *)pData)->Config.pConfigs[i].Config.pItems = (char *)p;
			p= ED_COPY_AND_DISPLACE(p, pConfigs[i].Config.pItems, (SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN) * pConfigs[i].Config.CountOfItems);
		};
		((IO_VS_Catalog *)pData)->pFragmentIndices = (int *)p;
		p= ED_COPY_AND_DISPLACE(p, pFragmentIndices, (sizeof(int) * Cat.Area.XViews * Cat.Area.YViews));
		
	if (pFragmentIndices) free(pFragmentIndices);
	if (pConfigs) 
	{
		for (i = 0; i < Cat.Config.CountOfConfigs; i++)
			if (pConfigs[i].Config.pItems) free(pConfigs[i].Config.pItems);
		free(pConfigs);
		};
	*ppData = (IO_VS_Catalog *)pData;

 
 
 fclose(F);
 return 1;
 }

double FindConfig(IO_VS_Catalog* pCat, char* ConfigName, char* ConfigItem)
{
	int CountOfConfigs = pCat->Config.CountOfConfigs;
	VS_Config* pConfigs =pCat->Config.pConfigs;
	double dbl=0;
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
