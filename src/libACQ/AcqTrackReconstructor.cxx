//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqTrackReconstructor                                                      //
//                                                                      //
// 	Algorithmic object for scanning microtracks						//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "AcqTrackReconstructor.h"



ClassImp(AcqTrackReconstructor);
//_____________________________________________________________________________ 

//_____________________________________________________________________________ 
AcqTrackReconstructor::AcqTrackReconstructor()
{

}
//_____________________________________________________________________________ 

AcqTrackReconstructor::~AcqTrackReconstructor()
{
}
	  int NCellX,NCellY,NCellZ;
      TObjArray *Volume;
	  int MinRating; //minimum rating to keep the chain


int Propagate(int *Rating,  int iX, int iY,int iZ)
{
	int ind;
	TObjArray *farr;
	bool Neighbours=false;
	EdbCluster* fcl;
	int res;
	(*Rating)++; //increase rating
				  int cZ=iZ+1; //next layer
				  if(cZ>=NCellZ)
					  if ((*Rating)<MinRating) {(*Rating)--; return 0;} //last layer, not a valid chain
					  else {(*Rating)--; return 1;}
                  for(int cX=iX-1;cX<=iX+1;cX++) for(int cY=iY-1;cY<=iY+1;cY++)
				  {//loop on all neighbour cells in the next layer
				    if(cX<0 || cY<0 || cX>=NCellX || cY>=NCellY) continue; //out of bounds 
					ind=cZ*NCellX*NCellY+cY*NCellX+cX;
					farr=(TObjArray*)(Volume->At(ind)); //get array of clusters in the cell
					if(farr->GetEntries()>0 && !Neighbours) { Neighbours=true;} 
					for(int j=0;j<farr->GetEntries();j++) //loop on clusters
					{
						// here we have a neighbour cluster fcl
						fcl=(EdbCluster*)(farr->At(j));
						if(fcl->GetSegment() < (*Rating) ) fcl->SetSegment(*Rating); //set rating
						res=Propagate( Rating, cX,cY,cZ); //propagate rating to it's neighbours
			if((*Rating)>0) printf("Propagate(%d, %d,%d,%d) returned %d\n",(*Rating),iX,iY,iZ, res);

                        if(res) fcl->SetSegment(*Rating); //update rating
				    }
				  }
				  if(!Neighbours && (*Rating)<MinRating) {(*Rating)--; return 0;} //not a valid chain
(*Rating)--;
return 1;
}


//_____________________________________________________________________________ 
  int AcqTrackReconstructor::RecRating(EdbView* View)
  {
	  // reconstruction based on rating propagation
	  int iX,iY,iZ,ind;
	  float SizeX=360.,SizeY=300.; //view size in microns
	  float DX,DY;
	  float DZ=40.;// emulsion thickness in real space
	  float MaxT=.7; //max tangent of track angle
	  MinRating=10; //minimum rating to keep the chain
	  int CurRating=0; // current chain rating during propagation
	  NCellZ=View->GetNframes();
	  DX=DZ*MaxT;
	  DY=DZ*MaxT;
	  NCellX=int(SizeX/DX);
	  NCellY=int(SizeY/DY);
	  Volume=new TObjArray(NCellX*NCellY*NCellZ);
	  for( ind=0;ind<NCellX*NCellY*NCellZ;ind++)   Volume->AddAt(new  TObjArray(10),ind);
      TObjArray *arr,*farr;
	  EdbCluster *cl, *fcl;
	  try {
		  //Building index  table
	  for(int i=0;i<View->Nclusters();i++)
	  {
		  cl=View->GetCluster(i);
		  cl->SetSegment(0);
		  iX=int((cl->X()+SizeX/2.)/DX);
		  iY=int((cl->Y()+SizeY/2.)/DY);
		  iZ=cl->GetFrame();
		  ind=iZ*NCellX*NCellY+iY*NCellX+iX;
		  if(ind>=NCellX*NCellY*NCellZ) continue;
          ((TObjArray*)(Volume->At(ind)))->AddLast(cl);
	  }
         // now all clusters in 9 neighbour cells to the given one are within MaxT angle
	  //Cluster->eSegment is used as chain rating variable
      for(int Lyr=0;Lyr<NCellZ-MinRating+1;Lyr++)  //loop on first layers
	  {   iZ=Lyr;
          for(iX=2;iX<NCellX-2;iX++) for(iY=2;iY<NCellY-2;iY++) //loop on all cells in layer
		  {
		      ind=iZ*NCellX*NCellY+iY*NCellX+iX;
			  arr=(TObjArray*)(Volume->At(ind)); //get array of clusters in the cell
			  for(int i=0;i<arr->GetEntries();i++) //loop on clusters
			  {
				  cl=(EdbCluster*)(arr->At(i));    // looking for beginning of the chain
				  if(cl->GetSegment()>0) continue; // cluster already a member of the chain - go to next
				  // cluster can be a start of the chain. iterate now.
				  if(!Propagate(&CurRating, iX,iY,iZ)) continue; //no valid chain found, skip the cluster
                  cl->SetSegment(CurRating); 

			  }
		  }
	  }



	  }
	  catch(...) {printf("Exception! ind=%d, max=%d.\n",ind,NCellX*NCellY*NCellZ); }
	  for( ind=0;ind<NCellX*NCellY*NCellZ;ind++)   delete (Volume->At(ind));
	  delete Volume;

	  return 0;
  }
//_____________________________________________________________________________ 
  int AcqTrackReconstructor::RecNetscan(EdbView* View)
  {
  // reconstruction a la Netscan (slow)
	  return 0;
  }
//_____________________________________________________________________________ 
  int AcqTrackReconstructor::RecValeri(EdbView* View)
  {
  // reconstruction a la Valeri Tiukov (4D histogramming)
	  int iX,iY,iZ,ind;
	  float SizeX=360.,SizeY=300.; //view size in microns
	  float DX,DY;
	  float DZ=40.;// emulsion thickness in real space
	  float MaxT=.7; //max tangent of track angle
	  MinRating=10; //minimum rating to keep the chain
	  int CurRating=0; // current chain rating during propagation
	  NCellZ=View->GetNframes();
	  DX=DZ*MaxT;
	  DY=DZ*MaxT;
	  NCellX=int(SizeX/DX);
	  NCellY=int(SizeY/DY);
	  Volume=new TObjArray(NCellX*NCellY*NCellZ);
	  for( ind=0;ind<NCellX*NCellY*NCellZ;ind++)   Volume->AddAt(new  TObjArray(10),ind);
      TObjArray *arr,*farr;
	  EdbCluster *cl, *fcl;
	  try {
		  //Building index  table
	  for(int i=0;i<View->Nclusters();i++)
	  {
		  cl=View->GetCluster(i);
		  cl->SetSegment(0);
		  iX=int((cl->X()+SizeX/2.)/DX);
		  iY=int((cl->Y()+SizeY/2.)/DY);
		  iZ=cl->GetFrame();
		  ind=iZ*NCellX*NCellY+iY*NCellX+iX;
		  if(ind>=NCellX*NCellY*NCellZ) continue;
          ((TObjArray*)(Volume->At(ind)))->AddLast(cl);
	  }
	  }
	  catch(...) {printf("Exception! ind=%d, max=%d.\n",ind,NCellX*NCellY*NCellZ); }
	  for( ind=0;ind<NCellX*NCellY*NCellZ;ind++)   delete (Volume->At(ind));
	  delete Volume;
      return 0;
  }
//_____________________________________________________________________________ 
  int AcqTrackReconstructor::RecSySal(EdbView* View)
  {
  // reconstruction a la SySal
	  return 0;
  }
