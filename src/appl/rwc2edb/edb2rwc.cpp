// edb2rwc.cpp
// convert edb Root files to raw data using sysaldatio library
// author: Gabriele Sirri
//
// note: only ms windows


#include "TCut.h"
#include "TEventList.h"
#include "Riostream.h"
#include "EdbFrame.h"

#include "libDataConversion.h"

//__________________________________________________________BACK CONVERSION

int DumpRWD(EdbRun* run, char* rwcname, int fragID, IO_VS_Catalog* pCat)
{

	ISySalDataIO*  iIO=0;
	ISySalDataIO2* iIO2=0;
   UINT ExtErrorInfo; 
	CoInitialize(NULL);
	CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
						IID_ISySalDataIO, (void **)&iIO);
	iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);

   IO_VS_Fragment2 Frag2;

	//SET THE EVENT LISTS
   char str[128];
   TCut cut_t,cut_b;
   TTree* tree = (TTree* ) gDirectory->Get("Views");
	sprintf(str,"headers.eAreaID==%d && headers.eNframesTop",fragID);
	cut_t= str ;
	tree->Draw(">>lst_t", cut_t);
   TEventList *lst_t = (TEventList*)gDirectory->GetList()->FindObject("lst_t");
	sprintf(str,"headers.eAreaID==%d && headers.eNframesBot",fragID);
	cut_b= str ;
	tree->Draw(">>lst_b", cut_b);
	TEventList *lst_b = (TEventList*)gDirectory->GetList()->FindObject("lst_b");

	int nviews   =  lst_t->GetN();
//  cout << "nviews (top): " << nviews << " (bot): " << lst_b->GetN() << endl;

	if(pCat) Frag2.Hdr.ID = pCat->Hdr.ID;                                             // <--------- 
   else Frag2.Hdr.ID.Part[0] =Frag2.Hdr.ID.Part[1] =Frag2.Hdr.ID.Part[2] =Frag2.Hdr.ID.Part[3] = 0 ;
	Frag2.Hdr.Type.InfoType = VS_HEADER_BYTES | VS_FRAGMENT_SECTION;
	Frag2.Hdr.Type.HeaderFormat = VS_HEADER_TYPE;
	//for (i = 0; i < sizeof(Frag2.Reserved); i++) Frag2.Reserved[i] = i;
	Frag2.Fragment.CountOfViews = nviews;
	//	Frag2.Fragment.CodingMode = (rand() % 2) ? VS_COMPRESSION_NULL : VS_COMPRESSION_GRAIN_SUPPRESSION;
	Frag2.Fragment.CodingMode = VS_COMPRESSION_NULL ;
	Frag2.Fragment.FitCorrectionDataSize = 0;
	Frag2.Fragment.StartView = fragID * nviews;
	Frag2.Fragment.Index = fragID;
	Frag2.Fragment.pViews = new VS_View2[nviews];

   EdbView* eView = run->GetView();
   EdbViewHeader* eViewHeader = eView->GetHeader();
   EdbSegment* eSeg=0;

   Grain* rwdGrain;
   Track2* rwdTrack;
	VS_View2* rwdView;

   int totsegments(0), totclusters(0);

	for (int v = 0; v < nviews; v++)
	{
      eView->Clear();
      run->GetEntry(lst_t->GetEntry(v));
    
      rwdView = &(Frag2.Fragment.pViews[v]);

		rwdView->TileX = -1;                                                 // <--------------
		rwdView->TileY = -1;                                                 // <--------------
      rwdView->RelevantZs.TopExt = eViewHeader->GetZ1();
		rwdView->RelevantZs.TopInt = eViewHeader->GetZ2();
		rwdView->RelevantZs.BottomInt = eViewHeader->GetZ3();
		rwdView->RelevantZs.BottomExt = eViewHeader->GetZ4();

		for (int s = 0; s < 2; s++)
		{
         eView->Clear();
         s==0? run->GetEntry(lst_t->GetEntry(v),1,1,1,1,1) :
               run->GetEntry(lst_b->GetEntry(v),1,1,1,1,1) ;
         eView->AttachClustersToSegments() ;

         rwdView->X[s] = eViewHeader->GetXview();
			rwdView->Y[s] = eViewHeader->GetYview();
         rwdView->MapX[s] = eViewHeader->GetAffine()->B1();
			rwdView->MapY[s] = eViewHeader->GetAffine()->B2();
         rwdView->ImageMat[s][0][0] = eViewHeader->GetAffine()->A11() ;
         rwdView->ImageMat[s][0][1] = eViewHeader->GetAffine()->A12() ;
         rwdView->ImageMat[s][1][0] = eViewHeader->GetAffine()->A21() ;
         rwdView->ImageMat[s][1][1] = eViewHeader->GetAffine()->A22() ;

         int nframes = eView->GetNframes() ;
         rwdView->Layers[s].Count = nframes;
         rwdView->Layers[s].pLayerInfo = new VS_View2::t_LayerInfo[nframes];

         for (int f = 0; f < nframes; f++)
			{
  		      EdbFrame* eFrame = (EdbFrame*) (eView->GetFrames())->At(f) ;
            rwdView->Layers[s].pLayerInfo[f].Clusters = eFrame->GetNcl();
            rwdView->Layers[s].pLayerInfo[f].Z = eFrame->GetZ();
			}

         rwdView->Status[s]  = 0;
         int nsegments = eView->Nsegments() ;
         rwdView->TCount[s]  = nsegments;
			rwdView->pTracks[s] = new Track2[nsegments] ;

         for (int t = 0; t < nsegments; t++)
			{
            totsegments++ ;
				rwdTrack = &(rwdView->pTracks[s][t]);
            eSeg = eView->GetSegment(t);
            rwdTrack->Field = v;
            rwdTrack->Grains = eSeg->GetPuls();
            rwdTrack->Intercept.X = eSeg->GetX0();
				rwdTrack->Intercept.Y = eSeg->GetY0();
				rwdTrack->Intercept.Z = eSeg->GetZ0();
            rwdTrack->Slope.X = eSeg->GetTx();
				rwdTrack->Slope.Y = eSeg->GetTy();
				rwdTrack->Slope.Z = 1.f;
            rwdTrack->Sigma = eSeg->GetSigmaX();
				rwdTrack->Valid = true;

            EdbCluster *eCluster=0;
            TObjArray *eClusterArray = eSeg->GetElements();
            int ncl = 0;
            double zmin,zmax;
            if(eClusterArray) ncl = eClusterArray->GetLast()+1;

				rwdTrack->AreaSum = 0;
				rwdTrack->pGrains = new Grain[ncl];
            for (int p = 0; p < ncl; p++)
				{
               totclusters++;
	            eCluster = (EdbCluster*)eClusterArray->At(p);
   				rwdGrain = &(rwdTrack->pGrains[p]);

               rwdGrain->Area = eCluster->GetArea();
					rwdTrack->AreaSum += rwdGrain->Area ;
					rwdGrain->X = eCluster->GetX();
					rwdGrain->Y = eCluster->GetY();
               int z = eCluster->GetZ() ;
					rwdGrain->Z = z;
               if(p==0) zmin=zmax= z;
               else
               {
                  if(z>zmax) zmax = z;
                  if(z<zmin) zmin = z;
               }
		   		rwdTrack->LastZ  = zmin ;
		   		rwdTrack->FirstZ = zmax ;
				};
			   /*if (Frag2.Fragment.FitCorrectionDataSize)
			   {
				      T.pCorrection = new BYTE[Frag2.Fragment.FitCorrectionDataSize];
				      for (p = 0; p < Frag2.Fragment.FitCorrectionDataSize; T.pCorrection[p++] = rand() % 256);
				      }
		      else*/ 
			   rwdTrack->pCorrection = 0;
			};
		};
	};

	// build rwd name 
	char rwdname[256], temp[256];
	sprintf(temp, "%s", rwcname);
	sprintf(temp+strlen(temp)-1, "d");
	sprintf(rwdname, "%s.%08X", temp, fragID);

   cout << rwdname << "  segments: "<< totsegments << "\tclusters: "<< totclusters << endl;
	if (iIO2->Write2(0, (BYTE *)&Frag2, &ExtErrorInfo, (UCHAR *)rwdname) != S_OK)
		cout << "Error Writing"<< endl;

   iIO2->Release();
   iIO->Release();
   return true;
}

//______________________________________________________________________________
int DumpRWC(EdbRun* run, char* rwcname)
{
   // Retrieve the catalog file from the comment field of the run header
   /*
   TString rwc = run->GetHeader()->GetComment();
   if (rwc.Length()>2000)
   {
      ofstream out;
      out.open(rwcname,ios::out | ios::binary);
      out << rwc ;
      out.close();
   }
   else 
   */
   {
      cout << "Cannot extract catalog information from the root file !" << endl;
      return 0;
   }
   return 1;
}

//______________________________________________________________________________
int BackConversion(EdbRun* run, char* rwcname) 
{
   IO_VS_Catalog* pCat = 0;
   DumpRWC(run,rwcname) ;

   if( ReadCatalog((void**)&pCat, (char*)rwcname) != 1) 
   {
      cout << "Cannot read catalog --> the Fragment.Hdr.ID field will be saved as {0,0,0,0}" << endl;
   }
 
   TTree* tree = (TTree* ) gDirectory->Get("Views");  
   int nfragm = tree->GetMaximum("eAreaID");
   for(int i=0; i < nfragm; i++) DumpRWD( run, rwcname, i+1, pCat) ;

   return true;
}

//______________________________________________________________________________
int main(int argc, char* argv[])
{
  char rwcname[256], edbname[256] ;

  bool printusage=(argc<3)?true:false;
  for (int i = 1; i < argc; i++)  {  // Skip argv[0] (program name)
	{ // Process non-optional arguments here
      sprintf(edbname,argv[i++]);
      sprintf(rwcname,argv[i]);
	}
  }
  if(printusage) { 
		cout<< "usage: edb2rwc <input file (.root)> <output file (.rwc|.rwd|.txt)> "<<endl;
		return 0;
	};

	EdbRun* outrun;
	outrun = new EdbRun(edbname);

   BackConversion(outrun,rwcname);

	outrun->Close();

	return 0;
};

 