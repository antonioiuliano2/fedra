// rwdread.cpp

#include <iostream>
using namespace std ;

#include "libDataConversion.h"

int main(int argc, char *argv[])
{
    unsigned int vid(0),tside(0),tid(0);

    if (argc < 2)
    {
        cout<< "usage: rwdread <input file (.rwd)> [view id] [track side] [track id]"<<endl;
        cout<< endl << "       use the operator> to save the output in a text file: "<<endl ;
        cout<< "       rwdread <input file (.rwd)> > output.txt"<<endl;
        return 0;
    };
    char* rwdname = argv[1];
    if (argc >2) vid = atoi(argv[2]);
    if (argc >3) tside = atoi(argv[3]);
    if (argc >4) tid = atoi(argv[4]);

   if (tside>(unsigned int)1) { cout <<"Track side can be 0 or 1"<<endl ; return false ;}

   IO_VS_Fragment2 *pFrag = 0;

   if (ReadFragment((void**)&pFrag, (char*)rwdname) != 1) return false;

	cout<<"-----------------------------------------------------------------------\n";
	cout<<"IO_VS_Fragment2" << endl;	
	cout<<"  |-- t_Hdr Hdr"<<endl;
	cout<<"  |\t  |-- IO_Header Type"<<endl;
	cout<<"  |\t  |\t  |-- BYTE InfoType     = " << pFrag->Hdr.Type.InfoType <<endl;
	cout<<"  |\t  |\t  |-- WORD HeaderFormat = " << pFrag->Hdr.Type.HeaderFormat <<endl;
	cout<<"  |\t  |-- Identifier ID"<<endl;
	cout<<"  |\t   \t  |-- DWORD Part[0] = " << pFrag->Hdr.ID.Part[0] <<endl;
	cout<<"  |\t   \t  |-- DWORD Part[1] = " << pFrag->Hdr.ID.Part[1] <<endl;
	cout<<"  |\t   \t  |-- DWORD Part[2] = " << pFrag->Hdr.ID.Part[2] <<endl;
	cout<<"  |\t   \t  |-- DWORD Part[3] = " << pFrag->Hdr.ID.Part[3] <<endl;

   VS_Fragment2* fr(& pFrag->Fragment) ;

	cout<<"  |-- VS_Fragment2 Fragment"<<endl;
	cout<<"  |\t  |-- int Index        = " << fr->Index         <<endl;
	cout<<"  |\t  |-- int StartView    = " << fr->StartView     <<endl;
	cout<<"  |\t  |-- int CountOfViews = " << fr->CountOfViews  <<endl;
	cout<<"  |\t  |-- int FitCorrectionDataSize = " << fr->FitCorrectionDataSize  <<endl;
	cout<<"  |\t  |-- int CodingMode   = " << fr->CodingMode    <<endl;

   if (vid>=(unsigned int)fr->CountOfViews ) { cout <<"View ID out of range!"<<endl ; return false ;}
	
   VS_View2* vw(& pFrag->Fragment.pViews[vid]) ;
   
   cout<<"  |\t  |-- VS_View2 *pViews" << endl;
   cout<<"  |\t   \t  |-- int TileX = "  << vw->TileX <<endl;
	cout<<"  |\t   \t  |-- int TileY = "  << vw->TileY <<endl;
	cout<<"  |\t   \t  |-- float X[0] = "	<< vw->X[0] <<endl;
	cout<<"  |\t   \t  |-- float Y[0] = "	<< vw->Y[0] <<endl;
	cout<<"  |\t   \t  |-- float X[1] = "	<< vw->X[1] <<endl;
	cout<<"  |\t   \t  |-- float Y[1] = "	<< vw->Y[1] <<endl;
   cout<<"  |\t   \t  |-- float MapX[0] = "	<< vw->MapX[0] <<endl;
   cout<<"  |\t   \t  |-- float MapY[0] = "	<< vw->MapY[0] <<endl;
	cout<<"  |\t   \t  |-- float MapX[1] = "	<< vw->MapX[1] <<endl;
	cout<<"  |\t   \t  |-- float MapY[1] = "	<< vw->MapY[1] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][0][0] = "<< vw->ImageMat[0][0][0] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][0][1] = "<< vw->ImageMat[0][0][1] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][1][0] = "<< vw->ImageMat[0][1][0] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[0][1][1] = "<< vw->ImageMat[0][1][1] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][0][0] = "<< vw->ImageMat[1][0][0] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][0][1] = "<< vw->ImageMat[1][0][1] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][1][0] = "<< vw->ImageMat[1][1][0] <<endl;
	cout<<"  |\t   \t  |-- float ImageMat[1][1][1] = "<< vw->ImageMat[1][1][1] <<endl;
   cout<<"  |\t   \t  |-- t_RelevantZs2 RelevantZs" <<endl;
   cout<<"  |\t   \t  |     |-- float TopExt    = "	<< vw->RelevantZs.TopExt    <<endl;
	cout<<"  |\t   \t  |     |-- float TopInt    = "	<< vw->RelevantZs.TopInt    <<endl;
	cout<<"  |\t   \t  |     |-- float BottomInt = "	<< vw->RelevantZs.BottomInt <<endl;
	cout<<"  |\t   \t  |     |-- float BottomExt = "	<< vw->RelevantZs.BottomExt <<endl;
   
   for(int j=0;j<2;j++)
   {
      cout<<"  |\t   \t  |-- t_Layers2 Layers["<<j<<"]" <<endl;
      cout<<"  |\t   \t  |     |-- int Count         = " << vw->Layers[j].Count <<endl;
      cout<<"  |\t   \t  |     |-- t_LayerInfo* pLayerInfo" <<endl;
      cout<<"  |\t   \t  |           |-- int Clusters     #### see below  " <<endl;
      cout<<"  |\t   \t  |           |-- float Z          #### see below  "<<endl;
   }

   cout<<"  |\t   \t  |-- unsigned char Status[0] = " << vw->Status[0] <<endl;
   cout<<"  |\t   \t  |-- unsigned char Status[1] = " << vw->Status[1] <<endl;
   cout<<"  |\t   \t  |-- int TCount[0] = " << vw->TCount[0] <<endl;
   cout<<"  |\t   \t  |-- int TCount[1] = " << vw->TCount[1] <<endl;

  if ( tid>=(unsigned int) vw->TCount[tside] ) { cout <<"track ID out of range!"<<endl ; return false ;}

   Track2* tr(& vw->pTracks[tside][tid]) ;
   cout<<"  |\t   \t  |-- Track2 *pTracks["<<tside<<"]" <<endl;
   cout<<"  |\t   \t       |-- Field       = " << tr->Field   << endl;	
   cout<<"  |\t   \t       |-- int AreaSum = " << tr->AreaSum << endl;
	cout<<"  |\t   \t       |-- int Grains  = " << tr->Grains  << endl;
	cout<<"  |\t   \t       |-- Grain *pGrains"<<endl;
	cout<<"  |\t   \t       |     |--  short Area      #### see below "<< endl; 
   cout<<"  |\t   \t       |     |--  float X         #### see below "<< endl; 
   cout<<"  |\t   \t       |     |--  float Y         #### see below "<< endl;
   cout<<"  |\t   \t       |     |--  float Z         #### see below "<< endl;

	cout<<"  |\t   \t       |-- BYTE *pCorrection = " <</* tr->pCorrection  <<*/endl;
	cout<<"  |\t   \t       |-- TVector Intercept" <<endl;
	cout<<"  |\t   \t       |     |--  float X = " << tr->Intercept.X <<endl;
	cout<<"  |\t   \t       |     |--  float Y = " << tr->Intercept.Y <<endl;
	cout<<"  |\t   \t       |     |--  float Z = " << tr->Intercept.Z <<endl;
	cout<<"  |\t   \t       |-- TVector Slope" <<endl;
	cout<<"  |\t   \t       |     |--  float X = " << tr->Slope.X <<endl;
	cout<<"  |\t   \t       |     |--  float Y = " << tr->Slope.Y <<endl;
	cout<<"  |\t   \t       |     |--  float Z = " << tr->Slope.Z <<endl;
	cout<<"  |\t   \t       |-- float Sigma   = "  << tr->Sigma   <<endl;
	cout<<"  |\t   \t       |-- float FirstZ  = "  << tr->FirstZ  <<endl;
	cout<<"  |\t   \t       |-- float LastZ   = "  << tr->LastZ   <<endl;
	cout<<"  |\t   \t       |-- boolean Valid = "  << tr->Valid   << endl;
	cout<<"  |-- unsigned char Reserved[256] = " ; 
          for(int i=0;i<256;i++) cout << pFrag->Reserved[i] ; cout<<endl;
	cout<<"-----------------------------------------------------------------------\n"<<endl;

   cout<<"\t##### clusters[0]  Z[0]\t clusters[1]  Z[1]" <<endl;
   for(int i=0;i< max(vw->Layers[0].Count,vw->Layers[1].Count);i++)
   {
         cout<<"\t\t " ;
         if (i < vw->Layers[0].Count)
            cout << vw->Layers[0].pLayerInfo[i].Clusters  <<"\t"
                 << vw->Layers[0].pLayerInfo[i].Z ;
         else cout <<"\t\t";
         cout <<"\t";
         if (i < vw->Layers[1].Count)
            cout << vw->Layers[1].pLayerInfo[i].Clusters  <<"\t"
                 << vw->Layers[1].pLayerInfo[i].Z << endl;
         else cout << endl;
   }
   cout<<"\n\t ##### area  X     Y     Z" <<endl;
   for(int i=0;i<(int) tr->Grains;i++)			
         cout<<"\t\t " 
               <<  tr->pGrains[i].Area  <<" "<<
					    tr->pGrains[i].X     <<" "<<
					    tr->pGrains[i].Y     <<" "<<
					    tr->pGrains[i].Z     << endl ;


   FreeMemory((void**)pFrag);
   delete pFrag ;

   return 0;
 }
