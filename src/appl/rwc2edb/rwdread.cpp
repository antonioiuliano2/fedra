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
	cout<<"t_Hdr Hdr\tIO_Header Type  BYTE InfoType = "	<<pFrag->Hdr.Type.InfoType<<endl;
	cout<<"\t\t\t\tWORD HeaderFormat = "							<<pFrag->Hdr.Type.HeaderFormat <<endl;
	cout<<"\t\tIdentifier ID\tDWORD Part[0] = "<<pFrag->Hdr.ID.Part[0] <<endl;
	cout<<"\t\t\t\tDWORD Part[1] = "					<<pFrag->Hdr.ID.Part[1] <<endl;
	cout<<"\t\t\t\tDWORD Part[2] = "					<<pFrag->Hdr.ID.Part[2] <<endl;
	cout<<"\t\t\t\tDWORD Part[3] = "					<<pFrag->Hdr.ID.Part[3] <<endl;
	cout<<"-----------------------------------------------------------------------\n";

	cout<<"VS_Fragment2 Fragment\tint Index = "<<pFrag->Fragment.Index<<endl;
	cout<<"\t\tint StartView = "					<<pFrag->Fragment.StartView<<endl;
	cout<<"\t\tint CountOfViews = "				<<pFrag->Fragment.CountOfViews<<endl;
	cout<<"\t\tint FitCorrectionDataSize = "	<<pFrag->Fragment.FitCorrectionDataSize<<endl;
	cout<<"\t\tint CodingMode = "					<<pFrag->Fragment.CodingMode<<endl;

   if (vid>=(unsigned int)pFrag->Fragment.CountOfViews ) { cout <<"View ID out of range!"<<endl ; return false ;}
	cout<<"\t\tVS_View2 *pViews\tint TileX = "	<<pFrag->Fragment.pViews[vid].TileX<<endl;
	cout<<"\t\t\t\tint TileY = "	<<pFrag->Fragment.pViews[vid].TileY<<endl;
	cout<<"\t\t\t\tfloat X[0] = "	<<pFrag->Fragment.pViews[vid].X[0]<<endl;
	cout<<"\t\t\t\tfloat Y[0] = "	<<pFrag->Fragment.pViews[vid].Y[0]<<endl;
	cout<<"\t\t\t\tfloat X[1] = "	<<pFrag->Fragment.pViews[vid].X[1]<<endl;
	cout<<"\t\t\t\tfloat Y[1] = "	<<pFrag->Fragment.pViews[vid].Y[1]<<endl;
   cout<<"\t\t\t\tfloat MapX[0] = "	<<pFrag->Fragment.pViews[vid].MapX[0]<<endl;
   cout<<"\t\t\t\tfloat MapY[0] = "	<<pFrag->Fragment.pViews[vid].MapY[0]<<endl;
	cout<<"\t\t\t\tfloat MapX[1] = "	<<pFrag->Fragment.pViews[vid].MapX[1]<<endl;
	cout<<"\t\t\t\tfloat MapY[1] = "	<<pFrag->Fragment.pViews[vid].MapY[1]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[0][0][0] = "<<pFrag->Fragment.pViews[vid].ImageMat[0][0][0]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[0][0][1] = "<<pFrag->Fragment.pViews[vid].ImageMat[0][0][1]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[0][1][0] = "<<pFrag->Fragment.pViews[vid].ImageMat[0][1][0]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[0][1][1] = "<<pFrag->Fragment.pViews[vid].ImageMat[0][1][1]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[1][0][0] = "<<pFrag->Fragment.pViews[vid].ImageMat[1][0][0]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[1][0][1] = "<<pFrag->Fragment.pViews[vid].ImageMat[1][0][1]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[1][1][0] = "<<pFrag->Fragment.pViews[vid].ImageMat[1][1][0]<<endl;
	cout<<"\t\t\t\tfloat ImageMat[1][1][1] = "<<pFrag->Fragment.pViews[vid].ImageMat[1][1][1]<<endl;
	cout<<"\t\t\t\tRelevantZs\tfloat TopExt    = "	<<pFrag->Fragment.pViews[vid].RelevantZs.TopExt<<endl;
	cout<<"\t\t\t\t\t\tfloat TopInt    = "	<<pFrag->Fragment.pViews[vid].RelevantZs.TopInt<<endl;
	cout<<"\t\t\t\t\t\tfloat BottomInt = "	<<pFrag->Fragment.pViews[vid].RelevantZs.BottomInt<<endl;
	cout<<"\t\t\t\t\t\tfloat BottomExt = "	<<pFrag->Fragment.pViews[vid].RelevantZs.BottomExt<<endl;

   cout<<"\t\t\t\tt_Layers2 Layers[0]\tint Count = "	<<pFrag->Fragment.pViews[vid].Layers[0].Count<<endl;
   cout<<"\t\t\t\t\t\t\tt_LayerInfo* pLayerInfo\tint Clusters , float Z ="<<endl;
   for(int i=0;i<pFrag->Fragment.pViews[vid].Layers[0].Count;i++)
      cout<<"\t\t\t\t\t\t\t " <<pFrag->Fragment.pViews[vid].Layers[0].pLayerInfo[i].Clusters
          <<"\t"<< pFrag->Fragment.pViews[vid].Layers[0].pLayerInfo[i].Z <<endl;

   cout<<"\t\t\t\tt_Layers2 Layers[0]\tint Count = "	<<pFrag->Fragment.pViews[vid].Layers[1].Count<<endl;
   cout<<"\t\t\t\t\t\t\tt_LayerInfo* pLayerInfo\tint Clusters , float Z ="<<endl;
   for(int i=0;i<pFrag->Fragment.pViews[vid].Layers[1].Count;i++)
      cout<<"\t\t\t\t\t\t\t " <<pFrag->Fragment.pViews[vid].Layers[1].pLayerInfo[i].Clusters
          <<"\t"<< pFrag->Fragment.pViews[vid].Layers[1].pLayerInfo[i].Z <<endl;

   cout<<"\t\t\t\tunsigned char Status[0] = " << pFrag->Fragment.pViews[vid].Status[0]<<endl;
	cout<<"\t\t\t\tunsigned char Status[1] = " << pFrag->Fragment.pViews[vid].Status[1]<<endl;
	cout<<"\t\t\t\tint TCount[0] = " << pFrag->Fragment.pViews[vid].TCount[0]<<endl;
	cout<<"\t\t\t\tint TCount[1] = " << pFrag->Fragment.pViews[vid].TCount[1]<<endl;

  if (tid>=(unsigned int)pFrag->Fragment.pViews[vid].TCount[tside] ) { cout <<"track ID out of range!"<<endl ; return false ;}

   cout<<"\t\t\t\tTrack2 *pTracks["<<tside<<"]  int Field = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].Field << endl;
	cout<<"\t\t\t\t\t\t   int AreaSum = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].AreaSum << endl;
	cout<<"\t\t\t\t\t\t   int Grains = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].Grains << endl;
	cout<<"\t\t\t\t\t\t   Grain *pGrains short Area float X,Y,Z = " << 
                        pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[0].Area <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[0].X <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[0].Y <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[0].Z << endl ;
   for(int i=1;i<(int)pFrag->Fragment.pViews[vid].pTracks[tside][tid].Grains;i++)			
				cout<<"\t   "<< pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[i].Area <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[i].X <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[i].Y <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].pGrains[i].Z << endl ;
	cout<<"\t\t\t\t\t\t   BYTE *pCorrection = " /*<< pFrag->Fragment.pViews[vid].pTracks[tside][tid].pCorrection */<<endl;
	cout<<"\t\t\t\t\t\t   TVector Intercept float X,Y,Z = " << 
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].Intercept.X <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].Intercept.Y <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].Intercept.Z << endl ;
	cout<<"\t\t\t\t\t\t   TVector Slope float X,Y,Z = " << 
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].Slope.X <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].Slope.Y <<" "<<
								pFrag->Fragment.pViews[vid].pTracks[tside][tid].Slope.Z << endl ;
	cout<<"\t\t\t\t\t\t   float Sigma = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].Sigma <<endl;
	cout<<"\t\t\t\t\t\t   float FirstZ = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].FirstZ <<endl;
	cout<<"\t\t\t\t\t\t   float LastZ = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].LastZ <<endl;
	cout<<"\t\t\t\t\t\t   boolean Valid = " << pFrag->Fragment.pViews[vid].pTracks[tside][tid].Valid << endl;
	cout<<"-----------------------------------------------------------------------\n";
	cout<<"unsigned char Reserved[256] = " ; for(int i=0;i<256;i++) cout << pFrag->Reserved[i]; cout<<endl;
	cout<<"-----------------------------------------------------------------------\n";

   FreeMemory((void**)pFrag);
   delete pFrag ;

   return 0;
 };


 