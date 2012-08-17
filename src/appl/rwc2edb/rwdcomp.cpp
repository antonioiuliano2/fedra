// rwdcomp.cpp

#include <iostream>
using namespace std ;

#include "libDataConversion.h"

int main(int argc, char *argv[])
{
    unsigned int vid(0),tside(0),tid(0);

    if (argc < 3)
    {
        cout<< "usage: rwdcomp <input file1 (.rwd)> <input file2 (.rwd)> [view id] [track side] [track id]"<<endl;
        cout<< endl << "       use the operator> to save the output in a text file: "<<endl ;
        cout<< "       rwdcomp <input file1 (.rwd)> <input file2 (.rwd)> > output.txt"<<endl;
        return 0;
    };
    char* rwdname1 = argv[1];
    char* rwdname2 = argv[2];
    if (argc >3) vid = atoi(argv[3]);
    if (argc >4) tside = atoi(argv[4]);
    if (argc >5) tid = atoi(argv[5]);

   if (tside>(unsigned int)1) { cout <<"Track side can be 0 or 1"<<endl ; return false ;}

   IO_VS_Fragment2 *pFrag1 = 0;
   IO_VS_Fragment2 *pFrag2 = 0;

   if (ReadFragment((void**)&pFrag1, (char*)rwdname1) != 1) return false;
   if (ReadFragment((void**)&pFrag2, (char*)rwdname2) != 1) return false;

	cout<<"-----------------------------------------------------------------------\n";
	cout<<"IO_VS_Fragment2" << endl;	
	cout<<"  |-- t_Hdr Hdr"<<endl;
	cout<<"  |\t  |-- IO_Header Type"<<endl;
	cout<<"  |\t  |\t  |-- BYTE InfoType     = " 
      << memcmp(&pFrag1->Hdr.Type.InfoType     , &pFrag2->Hdr.Type.InfoType     ,sizeof(pFrag1->Hdr.Type.InfoType)     ) <<endl;
	cout<<"  |\t  |\t  |-- WORD HeaderFormat = "							
      << memcmp(&pFrag1->Hdr.Type.HeaderFormat , &pFrag2->Hdr.Type.HeaderFormat ,sizeof(pFrag1->Hdr.Type.HeaderFormat) ) <<endl;
	cout<<"  |\t  |-- Identifier ID"<<endl;
	cout<<"  |\t   \t  |-- DWORD Part[0] = " << memcmp(&pFrag1->Hdr.ID.Part[0] , &pFrag2->Hdr.ID.Part[0] ,sizeof(pFrag1->Hdr.ID.Part[0])) <<endl;
	cout<<"  |\t   \t  |-- DWORD Part[1] = " << memcmp(&pFrag1->Hdr.ID.Part[1] , &pFrag2->Hdr.ID.Part[1] ,sizeof(pFrag1->Hdr.ID.Part[1])) <<endl;
	cout<<"  |\t   \t  |-- DWORD Part[2] = " << memcmp(&pFrag1->Hdr.ID.Part[2] , &pFrag2->Hdr.ID.Part[2] ,sizeof(pFrag1->Hdr.ID.Part[2])) <<endl;
	cout<<"  |\t   \t  |-- DWORD Part[3] = " << memcmp(&pFrag1->Hdr.ID.Part[3] , &pFrag2->Hdr.ID.Part[3] ,sizeof(pFrag1->Hdr.ID.Part[3])) <<endl;

   VS_Fragment2* fr1(& pFrag1->Fragment),*fr2(& pFrag2->Fragment) ;

	cout<<"  |-- VS_Fragment2 Fragment"<<endl;
	cout<<"  |\t  |-- int Index        = " << memcmp(&fr1->Index        , &fr2->Index        ,sizeof(fr1->Index       ) ) <<endl;
	cout<<"  |\t  |-- int StartView    = " << memcmp(&fr1->StartView    , &fr2->StartView    ,sizeof(fr1->StartView   ) ) <<endl;
	cout<<"  |\t  |-- int CountOfViews = " << memcmp(&fr1->CountOfViews , &fr2->CountOfViews ,sizeof(fr1->CountOfViews) ) <<endl;
	cout<<"  |\t  |-- int FitCorrectionDataSize = " << memcmp(&fr1->FitCorrectionDataSize , &fr2->FitCorrectionDataSize ,sizeof(fr1->FitCorrectionDataSize)) <<endl;
	cout<<"  |\t  |-- int CodingMode   = " << memcmp(&fr1->CodingMode   , &fr2->CodingMode   ,sizeof(fr1->CodingMode) ) <<endl;

   if (vid>=(unsigned int)fr1->CountOfViews ) { cout <<"View ID out of range!"<<endl ; return false ;}
	
   VS_View2* vw1(& pFrag1->Fragment.pViews[vid]),* vw2(& pFrag2->Fragment.pViews[vid]) ;
   
   cout<<"  |\t  |-- VS_View2 *pViews" << endl;
   cout<<"  |\t   \t  |-- int TileX = "   << memcmp(&vw1->TileX , &vw2->TileX ,sizeof(vw1->TileX) ) <<endl;
	cout<<"  |\t   \t  |-- int TileY = "   << memcmp(&vw1->TileY , &vw2->TileY ,sizeof(vw1->TileY) ) <<endl;
	cout<<"  |\t   \t  |-- float X[0] = "	<< memcmp(&vw1->X[0]  , &vw2->X[0]  ,sizeof(vw1->X[0] ) ) <<endl;
	cout<<"  |\t   \t  |-- float Y[0] = "	<< memcmp(&vw1->Y[0]  , &vw2->Y[0]  ,sizeof(vw1->Y[0] ) ) <<endl;
	cout<<"  |\t   \t  |-- float X[1] = "	<< memcmp(&vw1->X[1]  , &vw2->X[1]  ,sizeof(vw1->X[1] ) ) <<endl;
	cout<<"  |\t   \t  |-- float Y[1] = "	<< memcmp(&vw1->Y[1]  , &vw2->Y[1]  ,sizeof(vw1->Y[1] ) ) <<endl;
   cout<<"  |\t   \t  |-- float MapX[0] = "	<< memcmp(&vw1->MapX[0] , &vw2->MapX[0] ,sizeof(vw1->MapX[0]) ) <<endl;
   cout<<"  |\t   \t  |-- float MapY[0] = "	<< memcmp(&vw1->MapY[0] , &vw2->MapY[0] ,sizeof(vw1->MapY[0]) ) <<endl;
	cout<<"  |\t   \t  |-- float MapX[1] = "	<< memcmp(&vw1->MapX[1] , &vw2->MapX[1] ,sizeof(vw1->MapX[1]) ) <<endl;
	cout<<"  |\t   \t  |-- float MapY[1] = "	<< memcmp(&vw1->MapY[1] , &vw2->MapY[1] ,sizeof(vw1->MapY[1]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][0][0] = "<< memcmp(&vw1->ImageMat[0][0][0] , &vw2->ImageMat[0][0][0] ,sizeof(vw1->ImageMat[0][0][0]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][0][1] = "<< memcmp(&vw1->ImageMat[0][0][0] , &vw2->ImageMat[0][0][0] ,sizeof(vw1->ImageMat[0][0][0]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][1][0] = "<< memcmp(&vw1->ImageMat[0][1][0] , &vw2->ImageMat[0][1][0] ,sizeof(vw1->ImageMat[0][1][0]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][1][1] = "<< memcmp(&vw1->ImageMat[0][1][1] , &vw2->ImageMat[0][1][1] ,sizeof(vw1->ImageMat[0][1][1]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][0][0] = "<< memcmp(&vw1->ImageMat[1][0][0] , &vw2->ImageMat[1][0][0] ,sizeof(vw1->ImageMat[1][0][0]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][1][0] = "<< memcmp(&vw1->ImageMat[1][1][0] , &vw2->ImageMat[1][1][0] ,sizeof(vw1->ImageMat[1][1][0]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][0][1] = "<< memcmp(&vw1->ImageMat[1][0][1] , &vw2->ImageMat[1][0][1] ,sizeof(vw1->ImageMat[1][0][1]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][1][0] = "<< memcmp(&vw1->ImageMat[1][1][0] , &vw2->ImageMat[1][1][0] ,sizeof(vw1->ImageMat[1][1][0]) ) <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][1][1] = "<< memcmp(&vw1->ImageMat[1][1][1] , &vw2->ImageMat[1][1][1] ,sizeof(vw1->ImageMat[1][1][1]) ) <<endl;
   cout<<"  |\t   \t  |-- t_RelevantZs2 RelevantZs" <<endl;
   cout<<"  |\t   \t  |     |-- float TopExt    = "  << memcmp(&vw1->RelevantZs.TopExt    , &vw2->RelevantZs.TopExt    ,sizeof(vw1->RelevantZs.TopExt   ) ) <<endl;
	cout<<"  |\t   \t  |     |-- float TopInt    = "  << memcmp(&vw1->RelevantZs.TopInt    , &vw2->RelevantZs.TopInt    ,sizeof(vw1->RelevantZs.TopInt   ) ) <<endl;
	cout<<"  |\t   \t  |     |-- float BottomInt = "  << memcmp(&vw1->RelevantZs.BottomInt , &vw2->RelevantZs.BottomInt ,sizeof(vw1->RelevantZs.BottomInt) ) <<endl;
	cout<<"  |\t   \t  |     |-- float BottomExt = "  << memcmp(&vw1->RelevantZs.BottomExt , &vw2->RelevantZs.BottomExt ,sizeof(vw1->RelevantZs.BottomExt) ) <<endl;
   
   for(int j=0;j<2;j++)
   {
      cout<<"  |\t   \t  |-- t_Layers2 Layers["<<j<<"]" <<endl;
      cout<<"  |\t   \t  |     |-- int Count       = " << memcmp(&vw1->Layers[j].Count , &vw2->Layers[j].Count ,sizeof(vw1->Layers[j].Count) ) <<endl;
      cout<<"  |\t   \t  |     |-- t_LayerInfo* pLayerInfo" <<endl;
      cout<<"  |\t   \t  |           |-- int Clusters     #### see below  " <<endl;
      cout<<"  |\t   \t  |           |-- float Z          #### see below  "<<endl;
   }

   cout<<"  |\t   \t  |-- unsigned char Status[0] = " << memcmp(&vw1->Status[0] , &vw2->Status[0] ,sizeof(vw1->Status[0]) ) <<endl;
   cout<<"  |\t   \t  |-- unsigned char Status[1] = " << memcmp(&vw1->Status[1] , &vw2->Status[1] ,sizeof(vw1->Status[1]) ) <<endl;
   cout<<"  |\t   \t  |-- int TCount[0] = "           << memcmp(&vw1->TCount[0] , &vw2->TCount[0] ,sizeof(vw1->TCount[0]) ) <<endl;
   cout<<"  |\t   \t  |-- int TCount[1] = "           << memcmp(&vw1->TCount[1] , &vw2->TCount[1] ,sizeof(vw1->TCount[1]) ) <<endl;

  if (tid>=(unsigned int)pFrag1->Fragment.pViews[vid].TCount[tside] ) { cout <<"track ID out of range!"<<endl ; return false ;}

   Track2* tr1(& pFrag1->Fragment.pViews[vid].pTracks[tside][tid]),* tr2(& pFrag2->Fragment.pViews[vid].pTracks[tside][tid]) ;
   cout<<"  |\t   \t  |-- Track2 *pTracks["<<tside<<"]" <<endl;
   cout<<"  |\t   \t       |-- Field       = " << memcmp(&tr1->Field   , &tr2->Field   ,sizeof(tr1->Field  ) ) << endl;	
   cout<<"  |\t   \t       |-- int AreaSum = " << memcmp(&tr1->AreaSum , &tr2->AreaSum ,sizeof(tr1->AreaSum) ) << endl;
	cout<<"  |\t   \t       |-- int Grains  = " << memcmp(&tr1->Grains  , &tr2->Grains  ,sizeof(tr1->Grains ) ) << endl;
	cout<<"  |\t   \t       |-- Grain *pGrains"<<endl;
	cout<<"  |\t   \t       |     |--  short Area      #### see below "<< endl; 
   cout<<"  |\t   \t       |     |--  float X         #### see below "<< endl; 
   cout<<"  |\t   \t       |     |--  float Y         #### see below "<< endl;
   cout<<"  |\t   \t       |     |--  float Z         #### see below "<< endl;

	cout<<"  |\t   \t       |-- BYTE *pCorrection = " << memcmp(&tr1->pCorrection , &tr2->pCorrection ,sizeof(tr1->pCorrection) ) <<endl;
	cout<<"  |\t   \t       |-- TVector Intercept" <<endl;
	cout<<"  |\t   \t       |     |--  float X = " << memcmp(&tr1->Intercept.X , &tr2->Intercept.X ,sizeof(tr1->Intercept.X) ) <<endl;
	cout<<"  |\t   \t       |     |--  float Y = " << memcmp(&tr1->Intercept.Y , &tr2->Intercept.Y ,sizeof(tr1->Intercept.Y) ) <<endl;
	cout<<"  |\t   \t       |     |--  float Z = " << memcmp(&tr1->Intercept.Z , &tr2->Intercept.Z ,sizeof(tr1->Intercept.Z) ) <<endl;
	cout<<"  |\t   \t       |-- TVector Slope" <<endl;
	cout<<"  |\t   \t       |     |--  float X = " << memcmp(&tr1->Slope.X , &tr2->Slope.X ,sizeof(tr1->Slope.X) ) <<endl;
	cout<<"  |\t   \t       |     |--  float Y = " << memcmp(&tr1->Slope.Y , &tr2->Slope.Y ,sizeof(tr1->Slope.Y) ) <<endl;
	cout<<"  |\t   \t       |     |--  float Z = " << memcmp(&tr1->Slope.Z , &tr2->Slope.Z ,sizeof(tr1->Slope.Z) ) <<endl;
	cout<<"  |\t   \t       |-- float Sigma   = "  << memcmp(&tr1->Sigma   , &tr2->Sigma   ,sizeof(tr1->Sigma  ) )<<endl;
	cout<<"  |\t   \t       |-- float FirstZ  = "  << memcmp(&tr1->FirstZ  , &tr2->FirstZ  ,sizeof(tr1->FirstZ ) )<<endl;
	cout<<"  |\t   \t       |-- float LastZ   = "  << memcmp(&tr1->LastZ   , &tr2->LastZ   ,sizeof(tr1->LastZ  ) )<<endl;
	cout<<"  |\t   \t       |-- boolean Valid = "  << memcmp(&tr1->Valid   , &tr2->Valid   ,sizeof(tr1->Valid  ) )<< endl;
	cout<<"  |-- unsigned char Reserved[256] = " ; 
          for(int i=0;i<256;i++) cout << memcmp(& pFrag1->Reserved[i] , &pFrag2->Reserved[i] , sizeof(pFrag1->Reserved[i]) ) ; cout<<endl;
	cout<<"-----------------------------------------------------------------------\n"<<endl;

   cout<<"\t\t\t##### clusters[0]  Z[0]\t clusters[1]  Z[1]" <<endl;
   for(int i=0;i< max(vw1->Layers[0].Count,vw1->Layers[1].Count);i++)
   {
         cout<<"\t\t\t\t " ;
         if (i < vw1->Layers[0].Count)
            cout   << memcmp(&vw1->Layers[0].pLayerInfo[i].Clusters , &vw2->Layers[0].pLayerInfo[i].Clusters ,sizeof(vw1->Layers[0].pLayerInfo[i].Clusters) ) 
            <<"\t" << memcmp(&vw1->Layers[0].pLayerInfo[i].Z        , &vw2->Layers[0].pLayerInfo[i].Z        ,sizeof(vw1->Layers[0].pLayerInfo[i].Z       ) ) ;
         else cout <<"\t\t\t";
         cout <<"\t";
         if (i < vw1->Layers[1].Count)
            cout  << memcmp(&vw1->Layers[1].pLayerInfo[i].Clusters , &vw2->Layers[1].pLayerInfo[i].Clusters ,sizeof(vw1->Layers[1].pLayerInfo[i].Clusters) ) 
            <<"\t"<< memcmp(&vw1->Layers[1].pLayerInfo[i].Z        , &vw2->Layers[1].pLayerInfo[i].Z        ,sizeof(vw1->Layers[1].pLayerInfo[i].Z       ) ) <<endl;
         else cout << endl;
   }
   cout<<"\n\t\t\t ##### area  X     Y     Z" <<endl;
   for(int i=0;i<(int) tr1->Grains;i++)
   {
      tr1->pGrains[i].Area = tr2->pGrains[i].Area = 25 ;
         cout<<"\t\t\t\t\t " 
               << memcmp(&tr1->pGrains[i].Area , &tr2->pGrains[i].Area , sizeof(tr1->pGrains[i].Area)) <<" "<<
					   memcmp(&tr1->pGrains[i].X    , &tr2->pGrains[i].X    , sizeof(tr1->pGrains[i].X) ) <<" "<<
					   memcmp(&tr1->pGrains[i].Y    , &tr2->pGrains[i].Y    , sizeof(tr1->pGrains[i].Y) ) <<" "<<
					   memcmp(&tr1->pGrains[i].Z    , &tr2->pGrains[i].Z    , sizeof(tr1->pGrains[i].Z) ) << endl ;
   }
/* 
   cout<<"\n\t\t\t ##### area  X     Y     Z" <<endl;
      for(int i=0;i<(int) tr1->Grains;i++)	
      {
         printf("\t%d\t%d - %02X %02X\t%d - %02X %02X\n",i,
                sizeof(tr1->pGrains[i].Area)   , 
                ( (tr1->pGrains[i].Area)&0xFF00 ),
                ( (tr1->pGrains[i].Area)&0x00FF ),
                sizeof(tr2->pGrains[i].Area)   , 
                ( (tr2->pGrains[i].Area)&0xFF00 ),
                ( (tr2->pGrains[i].Area)&0x00FF ) );
      }
 */



//��������������������������������������
   int nlayers = 0;
   int ntracks = 0;
   int ngrains = 0;
   int nviews  = pFrag1->Fragment.CountOfViews;
   for (int i = 0; i < nviews; i++)
	{
		nlayers +=   pFrag1->Fragment.pViews[i].Layers[0].Count 
                 + pFrag1->Fragment.pViews[i].Layers[1].Count;
		ntracks +=   pFrag1->Fragment.pViews[i].TCount[0] 
                 + pFrag1->Fragment.pViews[i].TCount[1];
		for (int j = 0; j < 2; j++)
         for (int k = 0; k < pFrag1->Fragment.pViews[i].TCount[j]; k++)
            ngrains += pFrag1->Fragment.pViews[i].pTracks[j][k].Grains ;
    };
   
   int HdrSize = sizeof(pFrag1->Hdr);
   int FragSize =  sizeof(pFrag1->Fragment.Index)
                 + sizeof(pFrag1->Fragment.StartView)
                 + sizeof(pFrag1->Fragment.CountOfViews)
                 + sizeof(pFrag1->Fragment.FitCorrectionDataSize)
                 + sizeof(pFrag1->Fragment.CodingMode)
                 + sizeof(pFrag1->Reserved) ;
   VS_View2* pView = & pFrag1->Fragment.pViews[0] ;
   int ViewSize =  sizeof(pView->TileX)
                 + sizeof(pView->TileY)
                 + sizeof(pView->X[0])
                 + sizeof(pView->X[1])		
			        + sizeof(pView->Y[0])		
			        + sizeof(pView->Y[1])		
			        + sizeof(pView->MapX[0])			       	   
			        + sizeof(pView->MapX[1])
			        + sizeof(pView->MapY[0])		
			        + sizeof(pView->MapY[1])
                 + 2*2*2* sizeof(pView->ImageMat[0][0][0])
                 + 2* sizeof(pView->Layers[0].Count)
                 + sizeof(pView->RelevantZs.TopExt)
                 + sizeof(pView->RelevantZs.TopInt)	
                 + sizeof(pView->RelevantZs.BottomExt)		
                 + sizeof(pView->RelevantZs.BottomInt)
                 + 2* sizeof(pView->Status[0])	
                 + 2* sizeof(pView->TCount[0]) ;
   ViewSize *= nviews;
   
   VS_View2::t_LayerInfo* pLayInf =  pView->Layers[0].pLayerInfo ;
   int LayInfSize =  sizeof(pLayInf->Clusters)
                  + sizeof(pLayInf->Z) ;
   LayInfSize *= nlayers;

   Track2* t =  pView->pTracks[0] ;
   int TrackSize =  sizeof(t->AreaSum)
                  + sizeof(t->Grains)
                  + sizeof(t->Intercept.X)
                  + sizeof(t->Intercept.Y)
                  + sizeof(t->Intercept.Z)
                  + sizeof(t->Slope.X)
                  + sizeof(t->Slope.Y)
                  + sizeof(t->Slope.Z)
                  + sizeof(t->Sigma)
                  + sizeof(t->FirstZ)
                  + sizeof(t->LastZ)
               //   + sizeof(t->Valid) 
                  ;
   TrackSize *= ntracks;

   int GrainSize = sizeof(Grain) ;
   GrainSize *= ngrains ;

   int TotalSize = 0;
   printf("\t\tstart \t end  \t  bytes\n");
   printf("Header Size: %7X %7X %9d\n",       TotalSize,TotalSize-1+HdrSize,   HdrSize)  ; TotalSize += HdrSize;
   printf("Frag Size:   %7X %7X %9d\n",       TotalSize,TotalSize-1+FragSize, FragSize)  ; TotalSize += FragSize;
   printf("Views Size:  %7X %7X %9d\t| %7d views  * %4d B \n", TotalSize,TotalSize-1+ViewSize,  ViewSize, nviews,    ViewSize/nviews) ; TotalSize += ViewSize;
   printf("LayInf Size: %7X %7X %9d\t| %7d layers * %4d B \n", TotalSize,TotalSize-1+LayInfSize,LayInfSize,nlayers,LayInfSize/nlayers); TotalSize += LayInfSize;
   printf("Tracks Size: %7X %7X %9d\t| %7d tracks * %4d B \n", TotalSize,TotalSize-1+TrackSize, TrackSize,ntracks,  TrackSize/ntracks); TotalSize += TrackSize;
   printf("Grain Size:  %7X %7X %9d\t| %7d grain  * %4d B \n", TotalSize,TotalSize-1+GrainSize, GrainSize,ngrains,  GrainSize/ngrains); TotalSize += GrainSize;
 
   printf("            \t\t      -------\n");
   printf("Total Size: \t\t      %8d bytes\n\n\n",TotalSize);

   cout << "Grain::Area is unsigned short (2 BYTES) but in the Grain structure it takes 4 BYTES both in memory and in file" << endl;
   cout << "size of Grain::Area + Grain::X + Grain::Y + Grain::Z = " <<
      sizeof(t->pGrains[0].Area) + sizeof(t->pGrains[0].X) +sizeof(t->pGrains[0].Y) + sizeof(t->pGrains[0].Z) << endl;
   cout << "size of Grain structure = " << sizeof(Grain)<< endl;
   
   FreeMemory((void**)pFrag1);
   delete pFrag1 ;
   delete pFrag2 ;
   
   return 0;
 }

