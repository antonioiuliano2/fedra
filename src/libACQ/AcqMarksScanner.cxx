//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqMarksScanner                                                      //
//                                                                      //
// 	Algorithmic object for scanning Fiducial Marks						//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "AcqMarksScanner.h"
#include "math.h"


ClassImp(AcqMarksScanner);
//_____________________________________________________________________________ 

//_____________________________________________________________________________ 
AcqMarksScanner::AcqMarksScanner()
{
	ST=NULL; FG=NULL;
	eMarksSet=new EdbMarksSet();
	eFoundMarksSet=new EdbMarksSet();
	Side=-1;
    IsValid=0;
	FPS=200.;
	Layers=5;
	Threshold=190;
	MarkDiameter=80.0;
	MarkDiameterTolerance=30.0;
	ViewsToScan=100; //10x10 views -> 3x3mm search area
	XYScanSpeed=10000; //  [microns/s]

}
//_____________________________________________________________________________ 

AcqMarksScanner::~AcqMarksScanner()
{
	delete eMarksSet;
	delete eFoundMarksSet;
}
//_____________________________________________________________________________ 

bool AcqMarksScanner::SetMapext(char * mapext)
{
	// Reads the standart SySal mapext string, parse it and
	// fills the eMarkSet object
	const char separators[] = "\r\n;";
	char *replica = new char[strlen(mapext) + 1];
//	IO_MarkMap *pMap = 0;
		strcpy(replica, mapext);
		char key[10];
		char *line = strtok(replica, separators);
		int marks = 0;
		int mark=0;
		int Id;
		int d;
		printf("%s\n",line);
		if (sscanf(line, "%s %d %d %d %d", &key, &Id, &Id, &Id, &Id) != 5) return false; 
		line = strtok(0, separators);
		printf("%s\n",line);
		float tminx, tminy, tmaxx, tmaxy;
		if (sscanf(line, "%d %f %f %f %f", &marks, &tminx, &tminy, &tmaxx, &tmaxy) != 5) return false;
		float nomx,nomy,stx,sty;
		while ((mark < marks) && (line = strtok(0, separators)))
		{
		printf("%s\n",line);
	if (sscanf(line, "%d %f %f %f %f %d %d %d", &Id, &nomx, &nomy, &stx, &sty, &d, &d, &Side) != 8) return false;

	eMarksSet->GetStage()->AddMark(Id,(ST->X->Reference)+stx,(ST->Y->Reference)+sty);
    eMarksSet->GetAbsolute()->AddMark(Id,nomx,nomy);
  	mark++;
		}
	return true;
}
//_____________________________________________________________________________ 

bool AcqMarksScanner::GotoMark(int Id)
{
	//Moves stage to a given mark
	// the coordinates are taken from the MarkSet

	float X,Y,Z;
	int res;
	if(Id<0 || Id>=eMarksSet->GetStage()->N()) 
	{
		printf("Mark Id %d  is out of range [0,%d]",Id,eMarksSet->GetStage()->N()-1);
		return false;
	}
	if(!ST) 
	{
		printf("No pointer to stage!\n");
		return false;
	}
	X=eMarksSet->GetStage()->At(Id)->X();
	Y=eMarksSet->GetStage()->At(Id)->Y();
	Z=ST->Z->Reference;
	if(Side==1) Z+=ST->EmTopLayer+ST->EmBotLayer+ST->EmBase;
	printf("Going to Mark %d at (%f,%f,%f)..",Id,X,Y,Z);	
	fflush(stdout);
	ST->X->PosMove(X,1e7,1e7);
	ST->Y->PosMove(Y,1e7,1e7);
	res=ST->X->WaitForMoveComplete(1e4);
	res*=ST->Y->WaitForMoveComplete(1e4);
	ST->Z->PosMove(Z,1e6,1e6);
	res*=ST->Z->WaitForMoveComplete(3e3);
	if(res) { printf("reached.\n");	return true; }
	else {printf("didn't reach! Error.\n");	return false; }

}


//_____________________________________________________________________________ 
int AcqMarksScanner::ScanView(float Z0, float DZ)
{
	// scans through the emulsion layer at Z0 with +DZ thickness
	// provides [Layers] frames for further processing
	// this is a basic function common for all scanning objects

	float *Pos=new float[Layers];
	float Speed=FPS*DZ/(Layers-1);
	int res;
	int Taken;
	bool SaveFIRF,SaveBGSUB,SaveGCORR;
	int SaveTHRES;
	SaveFIRF=FG->ApplyFIRF;
	FG->ApplyFIRF=false;
	SaveBGSUB=FG->SubtractBG;
	FG->SubtractBG=false;
	SaveGCORR=FG->ApplyGainCorr;
	FG->ApplyGainCorr=false;
	SaveTHRES=FG->Threshold;
	FG->Threshold=Threshold;
	printf("Threshold is set to %d\n",FG->Threshold);
	printf("Filling Z breakpoint position buffer\n"); 
    Speed=1.*FPS*DZ/(Layers-1);
    for(int i=0;i<Layers;i++) Pos[i]=1.*i*DZ/(Layers-1)+Z0;
    for(int i=0;i<1;i++)
	{
	printf("Z Started to %f..\n",Z0+DZ+5);
	ST->Z->PosMove(Z0+DZ+5,1e4,1e4);
	res=ST->Z->WaitForMoveComplete(3e3);
	printf("Z Reached initial at %f, res=%d\n",ST->Z->GetPos(),res);
	ST->Z->LoadBreakpointSequence(Pos, Layers);
	ST->Z->ArmBreakpointSequence();
	printf("Armed BP sequence and Z started to %f and \n",Z0-5);
	ST->Z->PosMove(Z0-5,Speed,50000);
	Taken=FG->GrabImagesONL(Layers,Pos,(DZ+20.)/Speed);
	res=ST->Z->WaitForMoveComplete(3e3);
	printf("Z Reached final at %f, res=%d\n, taken %d frames.\n",ST->Z->GetPos(),res,Taken);
 	}
    //restore FG settings
    FG->ApplyFIRF=SaveFIRF;
	FG->SubtractBG=SaveBGSUB;
	FG->ApplyGainCorr=SaveGCORR;
	FG->Threshold=SaveTHRES;

    delete [] Pos;
    return Taken;

}

//_____________________________________________________________________________ 
bool AcqMarksScanner::SearchMark(float *X, float *Y, float *Z)
{
	// Looks for the biggest cluster in the pile of images scanned by ScanView
	// and checks it's dimensions. If they match the mark size
	// returns true and its center coordinates in microns w.r.t view center
	// otherwise returns false

	if(Side==0) ScanView(ST->Z->Reference-ST->EmBotLayer/2.,ST->EmBotLayer);
	else if(Side==1) ScanView(ST->Z->Reference+ST->EmBotLayer+ST->EmBase-ST->EmTopLayer/2.,ST->EmTopLayer);
	else { printf("Side is not defined, probably mapext is not set.\n");return false;}
//	ScanView(Side*ST->NominalEmBase + ST->Z->Reference-2*ST->NominalEmLayer,4*ST->NominalEmLayer);
//	ScanView(Side*ST->NominalEmBase + ST->Z->Reference-2*ST->NominalEmLayer,4*ST->NominalEmLayer);
	int MaxArea=0;
	int Area;
	int Nf=0;
	float MaxX,MaxY,MaxZ;
	float PixToMic=FG->PixelToMicronX;
	float MarkDiam;
	EdbCluster *Cluster;
	for(int n=0;n<Layers;n++){
		FG->eEdbClusters->Clear();
		Nf=FG->FindClusters(n,FG->eEdbClusters);
//	printf("Found %d clusters in frame %d\n", Nf,n);
//	FG->ShowImage(n);
	int NCL=FG->eEdbClusters->GetEntries();
	for(int cl=0;cl<NCL;cl++)
	 {
      Cluster=(EdbCluster*)( FG->eEdbClusters->At(cl));
	  Area=Cluster->GetArea();
	  MarkDiam=abs(sqrt(Area/3.1415)*2.*PixToMic); //in microns
	  if(MarkDiam>MarkDiameter-MarkDiameterTolerance)
	  if(MarkDiam<MarkDiameter+MarkDiameterTolerance)
	  if(Area>MaxArea) { MaxArea=Area; MaxX=Cluster->X();MaxY=Cluster->Y();MaxZ=Cluster->Z();}
	 }

	}
      MarkDiam=abs(sqrt(MaxArea/3.1415)*2.*PixToMic); //in microns
	  if(MarkDiam>MarkDiameter-MarkDiameterTolerance && MarkDiam<MarkDiameter+MarkDiameterTolerance)
	  {
    printf("Best Mark with  diameter %f microns at %f,%f,%f\n",MarkDiam,MaxX,MaxY,MaxZ);
	if(X && Y && Z) {*X=MaxX; *Y=MaxY; *Z=MaxZ;}

   	return true;
	  }
	  else return false;
}
//_____________________________________________________________________________ 
bool AcqMarksScanner::SearchMarkFast(float *X, float *Y, float *Z)
{
	// Looks for the biggest cluster in the one image taken at right level (focusing must be done before!)
	// and checks it's dimensions. If they match the mark size
	// returns true and its center coordinates in microns w.r.t view center
	// otherwise returns false
	int res;
	bool SaveFIRF,SaveBGSUB,SaveGCORR;
	int SaveTHRES;

	if(Side==0) ST->Z->PosMove(ST->Z->Reference,1e9,1e9);
	else if(Side==1) ST->Z->PosMove(ST->Z->Reference+ST->EmBotLayer+ST->EmBase+ST->EmTopLayer,1e9,1e9);
	else { printf("Side is not defined, probably mapext is not set.\n");return false;}
	ST->Z->WaitForMoveComplete(2e3);
	// save FG settings
	SaveFIRF=FG->ApplyFIRF;
	FG->ApplyFIRF=false;
	SaveBGSUB=FG->SubtractBG;
	FG->SubtractBG=false;
	SaveGCORR=FG->ApplyGainCorr;
	FG->ApplyGainCorr=false;
	SaveTHRES=FG->Threshold;
	FG->Threshold=Threshold;
	printf("Threshold is set to %d\n",FG->Threshold);

	res=FG->GrabOneImage(ST->Z->GetPos(),5);

    //restore FG settings
    FG->ApplyFIRF=SaveFIRF;
	FG->SubtractBG=SaveBGSUB;
	FG->ApplyGainCorr=SaveGCORR;
	FG->Threshold=SaveTHRES;


	if(!res) return false;
	int BestDelta=1e3;
	int DeltaDiam;
	int BestDiam;
	int Area;
	int Nf=0;
	float MaxX,MaxY,MaxZ;
	float PixToMic=FG->PixelToMicronX;
	float MarkDiam;
	EdbCluster *Cluster;
		FG->eEdbClusters->Clear();
		Nf=FG->FindClusters(0,FG->eEdbClusters);
	printf("Found %d clusters in frame %d\n", Nf,0);
//	FG->ShowImage(n);
	int NCL=FG->eEdbClusters->GetEntries();
	for(int cl=0;cl<NCL;cl++)
	 {
      Cluster=(EdbCluster*)( FG->eEdbClusters->At(cl));
	  Area=Cluster->GetArea();
	  MarkDiam=abs(sqrt(Area/3.1415)*2.*PixToMic); //in microns
	  DeltaDiam=abs(MarkDiam-MarkDiameter);
	  if(DeltaDiam < MarkDiameterTolerance)
	  if(DeltaDiam < BestDelta) { BestDelta=DeltaDiam; BestDiam=MarkDiam; MaxX=Cluster->X();MaxY=Cluster->Y();MaxZ=Cluster->Z();}
	 }
      MarkDiam=BestDiam; //in microns
	  if(MarkDiam>MarkDiameter-MarkDiameterTolerance && MarkDiam<MarkDiameter+MarkDiameterTolerance)
	  {
    printf("Best Mark with  diameter %f microns at %f,%f,%f\n",MarkDiam,MaxX,MaxY,MaxZ);
	if(X && Y && Z) {*X=MaxX; *Y=MaxY; *Z=MaxZ;}

   	return true;
	  }
	  else return false;
}

//_____________________________________________________________________________ 
bool AcqMarksScanner::ScanMark(float *X, float *Y, float *Z)
{
	// scans the spiral path around current point and looks for the mark.
	// if found returns true and it's global positions in microns

    int *ix = new int[ViewsToScan];
    int *iy = new int[ViewsToScan];
	int res=0;
	bool Found=false;
	float PixToMic=FG->PixelToMicronX;
	float Posx,Posy,Posz;
	float VPosx,VPosy,VPosz;
	float GPosx,GPosy,GPosz;
	float CenterX=ST->X->GetPos();
	float CenterY=ST->Y->GetPos();
	StepX=fabs((FG->SizeX)*PixToMic)-2*MarkDiameter;
	StepY=fabs((FG->SizeY)*PixToMic)-2*MarkDiameter;
	printf("Search path is generated with StepX=%f and StepY=%f\n",StepX,StepY);
    EdbArea *ear=new EdbArea(); //only used to generate path
	ear->MakeSpiralPath(ViewsToScan,ix,iy);
    for(int v=0;v<ViewsToScan;v++)
	{
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') break;}
		Posx=CenterX+StepX*ix[v];
		Posy=CenterY+StepY*iy[v];
		res=0;
		while(!res)
		{
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') break;}
		res=1;
		ST->X->PosMove(Posx,XYScanSpeed,XYScanSpeed*100);
		ST->Y->PosMove(Posy,XYScanSpeed,XYScanSpeed*100);
		res*=ST->X->WaitForMoveComplete(2e3);
		res*=ST->Y->WaitForMoveComplete(2e3);
		}
		if(!res){ printf("Move is not complete within timeout of 2sec! \n"); break;}
		printf("Searching at (%f,%f).\n",Posx,Posy);

		if(SearchMarkFast(&VPosx,&VPosy,&VPosz)) 
		{ 
			Found=true; 
            
			//if(ST->X->Invert) GPosx=Posx-VPosx; else GPosx=Posx+VPosx;
            //if(ST->Y->Invert) GPosy=Posy-VPosy; else GPosy=Posy+VPosy;
			GPosx=Posx+VPosx;
            GPosy=Posy+VPosy;

			res=0;
		    while(!res)
		    {
		       if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') break;}
		       res=1;
			   ST->X->PosMove(GPosx,XYScanSpeed,XYScanSpeed*100);
			   ST->Y->PosMove(GPosy,XYScanSpeed,XYScanSpeed*100);
			   res*=ST->X->WaitForMoveComplete(2e3);
			   res*=ST->Y->WaitForMoveComplete(2e3);
		    }
			if(!res){ printf("Move is not complete within timeout of 2sec! \n"); break;}
			GPosx=ST->X->GetPos();
			GPosy=ST->Y->GetPos();
		     printf("Searching at (%f,%f).\n",GPosx,GPosy);
			Found=SearchMarkFast(&VPosx,&VPosy,&VPosz); if(!Found) continue;
//			if(ST->X->Invert) GPosx=GPosx-VPosx; else GPosx=GPosx+VPosx;
//            if(ST->Y->Invert) GPosy=GPosy-VPosy; else GPosy=GPosy+VPosy;           
			GPosx=GPosx+VPosx;
            GPosy=GPosy+VPosy;    

			res=0;
		    while(!res)
		    {
		       if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') break;}
		       res=1;
			   ST->X->PosMove(GPosx,XYScanSpeed,XYScanSpeed*100);
			   ST->Y->PosMove(GPosy,XYScanSpeed,XYScanSpeed*100);
			   res*=ST->X->WaitForMoveComplete(2e3);
			   res*=ST->Y->WaitForMoveComplete(2e3);
		    }
			if(!res){ printf("Move is not complete within timeout of 2sec! \n"); break;}
			GPosx=ST->X->GetPos();
			GPosy=ST->Y->GetPos();
		     printf("Searching at (%f,%f).\n",GPosx,GPosy);
			Found=SearchMarkFast(&VPosx,&VPosy,&VPosz); if(!Found) continue;
//			if(ST->X->Invert) GPosx=GPosx-VPosx; else GPosx=GPosx+VPosx;
//            if(ST->Y->Invert) GPosy=GPosy-VPosy; else GPosy=GPosy+VPosy;           
			GPosx=GPosx+VPosx;
            GPosy=GPosy+VPosy;           


			GPosz=VPosz;
		    FG->ShowImage(0);
			if(!X || !Y || !Z) break;
            *X=GPosx;
            *Y=GPosy;
			*Z=GPosz;
			break;
		}
		FG->ShowImage(0);
	}
    if(Found) printf("Mark found at global (%f,%f,%f)\n",GPosx,GPosy,GPosz);
    delete ear;
	delete [] ix;
	delete [] iy;
	return Found;
}

//_____________________________________________________________________________ 
bool AcqMarksScanner::ScanMarks()
{
	// scans for all marks in a given MarkSet
/*
	float X,Y,Z;
	float Xe,Ye,Ze;
	float Dx,Dy;
	int Id=0;
    Xe=eMarksSet->GetStage()->At(Id)->X();
	Ye=eMarksSet->GetStage()->At(Id)->Y();
	while(!GotoMark(Id)){		
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return false;}
	};
	if(!ScanMark(&X,&Y,&Z)) { printf("First mark is not found! Exiting.\n"); return false; }
    eMarksSet->GetStage()->At(Id)->SetX(X);
	eMarksSet->GetStage()->At(Id)->SetY(Y);
	eMarksSet->GetStage()->At(Id)->SetZ(Z);
    Dx=X-Xe; Dy=Y-Ye; //displacement actual w.r.t. expected mark position
	// calc 2nd mark expected
	Id++;
    Xe=eMarksSet->GetStage()->At(Id)->X();
	Ye=eMarksSet->GetStage()->At(Id)->Y();
	Xe+=Dx; Ye+=Dy;
    eMarksSet->GetStage()->At(Id)->SetX(Xe);
	eMarksSet->GetStage()->At(Id)->SetY(Ye);
	while(!GotoMark(Id)){		
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return false;}
	};
	if(!ScanMark(&X,&Y,&Z)) { printf("Second mark is not found! Exiting.\n"); return false; }
    eMarksSet->GetStage()->At(Id)->SetX(X);
	eMarksSet->GetStage()->At(Id)->SetY(Y);
	eMarksSet->GetStage()->At(Id)->SetZ(Z);
  //  Dx=X-Xe; Dy=Y-Ye; //displacement actual w.r.t. expected mark position
	// calc 3nd mark expected
	Id++;
    Xe=eMarksSet->GetStage()->At(Id)->X();
	Ye=eMarksSet->GetStage()->At(Id)->Y();
	Xe+=Dx; Ye+=Dy;
    eMarksSet->GetStage()->At(Id)->SetX(Xe);
	eMarksSet->GetStage()->At(Id)->SetY(Ye);
	while(!GotoMark(Id)){		
		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') return false;}
	};
	if(!ScanMark(&X,&Y,&Z)) { printf("Second mark is not found! Exiting.\n"); return false; }

*/

	return true;

}
