//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqTrackScanner                                                      //
//                                                                      //
// 	Algorithmic object for scanning microtracks						//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "AcqTrackScanner.h"



ClassImp(AcqTrackScanner);
//_____________________________________________________________________________ 

//_____________________________________________________________________________ 
AcqTrackScanner::AcqTrackScanner()
{
	ST=NULL; FG=NULL;
	FPS=200.;
	Layers=16;
//	Threshold=80;
	XYViewToViewSpeed=30000;
    ZBackStrokeSpeed=10000;
	ViewStepX=320.;
	ViewStepY=260.;
	Nx=1; Ny=1;


}
//_____________________________________________________________________________ 

AcqTrackScanner::~AcqTrackScanner()
{
}

//_____________________________________________________________________________ 
int AcqTrackScanner::ScanView(float Z0, float DZ, float GotoX, float GotoY, bool RawImages)
{
	// scans through the emulsion layer at Z0 with +DZ thickness
	// provides [Layers] frames for further processing
	// this is a basic function common for all scanning objects
	// If X,Y!=0 then after scan starts movement to X,Y,Z0 (next view position)

	float *Pos=new float[Layers];
	float Speed=FPS*DZ/(Layers-1);
	int res;
	int Taken;
	//FG->Threshold=Threshold;
	printf("threshold is set to %d\n",FG->Threshold);
	printf("Filling Z breakpoint position buffer\n"); 
    Speed=1.*FPS*DZ/(Layers-1);
    for(int i=0;i<Layers;i++) Pos[i]=1.*i*DZ/(Layers-1)+Z0;
 	printf("Z Started to %f..\n",Z0+DZ+2);
	ST->Z->PosMove(Z0+DZ+2,1e9,1e9);
	res=ST->Z->WaitForMoveComplete(3e3);
	printf("Z Reached initial at %f, res=%d\n",ST->Z->GetPos(),res);
	ST->Z->LoadBreakpointSequence(Pos, Layers);
	ST->Z->ArmBreakpointSequence();
	printf("Armed BP sequence and Z started to %f and \n",Z0-5);
	if(GotoX==0 || GotoY==0) ST->Z->PosMove(Z0-2,Speed,Speed*200); //old style just scan down
	else ST->ScanToZ0AndGotoXYZ(Z0-2, Speed, Speed*200, GotoX, GotoY,  XYViewToViewSpeed, XYViewToViewSpeed*10, Z0+DZ+2,  ZBackStrokeSpeed, ZBackStrokeSpeed*10, 3e3);
	printf("DZ = %f, Speed = %f,  (DZ+20.)/Speed = %f\n", DZ, Speed, (DZ+20.)/Speed);
	if(!RawImages) Taken=FG->GrabImagesONL(Layers,Pos,(DZ+20.)/Speed);
	else Taken=FG->GrabRawImagesONL(Layers,Pos,(DZ+20.)/Speed);
	res=ST->Z->WaitForMoveComplete(1e3);
//	if(GotoX!=0 && GotoY!=0) {
//	res=ST->X->WaitForMoveComplete(1e3);
//	res=ST->Y->WaitForMoveComplete(1e3);
//	}
	if(res==1 && Taken==Layers)
	printf("Z Reached final at %f, taken %d frames.\n",ST->Z->GetPos(),Taken);
	if(res==0) printf("Z movement is not complete within 1 sec! Error.\n");
	if(Taken!=Layers) printf("Expected %d layers, taken only %d! Error.\n", Layers, Taken);

    delete [] Pos;
    return Taken;

}


//_____________________________________________________________________________ 
long AcqTrackScanner::ScanAndFillView(float Z0, float DZ,  int Side, EdbView * View, bool FillImages, float GotoX, float GotoY)
{
	// scans through the emulsion layer at Z0 with +DZ thickness
	// provides [Layers] frames for further processing
	// this is a basic function common for all scanning objects
	// it uses asyncronous FG->GrabAndFillView to fill the given EdbView
	// if View==NULL then FG->eEdbView is filled
	// If X,Y!=0 then after scan starts movement to X,Y,Z0 (next view position)

	float *Pos=new float[Layers];
	float Speed=FPS*DZ/(Layers-1);
	int res;
	int Nclust=0;
	//FG->Threshold=Threshold;
	printf("Threshold is set to %d\n",FG->Threshold);
	printf("Filling Z breakpoint position buffer\n"); 
    Speed=1.*FPS*DZ/(Layers-1);
    for(int i=0;i<Layers;i++) Pos[i]=1.*i*DZ/(Layers-1)+Z0;
 	printf("Z Started to %f..\n",Z0+DZ+2);
	ST->Z->PosMove(Z0+DZ+2,1e9,1e9);
	res=ST->Z->WaitForMoveComplete(3e3);
	printf("Z Reached initial at %f, res=%d\n",ST->Z->GetPos(),res);
	ST->Z->LoadBreakpointSequence(Pos, Layers);
	ST->Z->ArmBreakpointSequence();
	printf("Armed BP sequence and Z started to %f and \n",Z0-5);
//	ST->Z->PosMove(Z0-2,Speed,Speed*200);
	if(GotoX==0 || GotoY==0) ST->Z->PosMove(Z0-2,Speed,Speed*200); //old style just scan down
	else ST->ScanToZ0AndGotoXYZ(Z0-2, Speed, Speed*200, GotoX, GotoY,  XYViewToViewSpeed, XYViewToViewSpeed*20, Z0+DZ+2,  ZBackStrokeSpeed, ZBackStrokeSpeed*10, 3e3);

//	Taken=FG->GrabImagesONL(Layers,Pos,(DZ+20.)/Speed);
    Nclust=FG->GrabAndFillView(Layers, Pos,(DZ+20.)/Speed,Side, View, FillImages);
	res=ST->Z->WaitForMoveComplete(3e3);
	if(res==1)
	printf("Z Reached final at %f, found %d clusters.\n",ST->Z->GetPos(),Nclust);
	else  printf("Z movement is not complete within 3 sec! Error.\n");

    delete [] Pos;
    return Nclust;

}

int AcqTrackScanner::FindLayer(float Z0, float DZ, int ClusterThreshold, float *z0,float *z1)
{
	// searches for emulsion layer using current Threshold and Image Processing settings
	// returns 1+2 if both surfaces are found,
	// 1 if only top surface is  found, (&z0)
	// 2 if only bottom is found        (&z1)
	// 0 if no surface found
	// uses maximum available number of layers (received from FG object)
	int retval=0;
	int res;
	int Taken=0;
	float zz0,zz1;
	int SL=Layers;
	Layers=FG->MaxNbGrab;
	printf("Find layer scans view...\n");
	Taken=ScanView(Z0,DZ);
    FG->eEdbView->Clear();
	FG->FillView(Layers,1);
    printf("Taken %d layers ..\n",Taken);

	int *b= new int[Layers];
	float *z= new float[Layers];
	EdbFrame * fr;
    for(int i=0;i<Layers;i++)
	{
		fr=(EdbFrame*)(FG->eEdbView->GetFrames()->At(i));
		b[i]=fr->GetNcl();
		z[i]=fr->GetZ();
		printf("Z=%f : %d clusters\n",z[i],b[i]);
	}
	int Nu=-1;
	int Nl=-1;
	for(int i=0;i<Layers;i++)
	{
		if(b[i]>ClusterThreshold && Nu < 0) Nu=i;
	}
	for(int i=Layers-1;i>=0;i--)
	{
		if(b[i]>ClusterThreshold && Nl < 0) Nl=i;
	}
    
	if(Nu>0 && Nu<Layers-1) {zz0=z[Nu]; retval+=1;}
	if(Nl>0 && Nl<Layers-1) {zz1=z[Nl]; retval+=2;}
	delete [] b;
	delete [] z;

 	Layers=SL;


//	if ( findsurf(&zz0, &zz1, ClusterThreshold)==0 ) return 0;
	printf("Surfaces detected %f -> %f\n",zz0,zz1);
	*z0=zz0;*z1=zz1;
	return retval;
}


int AcqTrackScanner::FastScanFragment(bool IsTop, int AreaId, bool AbsCoord, float X0, float Y0, EdbRun *run, bool RawImages)
{
	float Pos[50];
	float DZ;
	float Z0;
	int Nc=0;
	int res=0;
	int Side;
	float X,Y;
	int Vn=0;
	int vid=0;
	if(IsTop) printf("Top: "); else printf("Bottom: ");
	printf("Filling Z breakpoint position buffer\n"); 
	if(IsTop){
		Z0=ST->Z->Reference+ST->EmBase+ST->EmBotLayer;
	    DZ=ST->EmTopLayer;
		Side=1;
	}
	else{
		Z0=ST->Z->Reference;
		DZ=ST->EmBotLayer;
        Side=0;
	}
    for(int i=0;i<Layers;i++) Pos[i]=1.*i*DZ/(Layers-1)+Z0;
	ST->Z->PosMove(Z0+DZ+2,1e4,1e4);
	ST->X->PosMove(X0,1e9,1e9);
	ST->Y->PosMove(Y0,1e9,1e9);
	res=ST->Z->WaitForMoveComplete(3e3);
    res*=ST->X->WaitForMoveComplete(2e3);
	res*=ST->Z->WaitForMoveComplete(2e3);		       
	printf("XYZ Reached initial Z at %f, res=%d\n",ST->Z->GetPos(),res);
		if(!res) {printf("Position is not reached! Aborting scan.\n");return 0;}
	for(int iy=0;iy<Ny;iy++)
	for(int ix=(iy==0);ix<Nx;ix++)
	{		
       X=X0+ViewStepX*ix;
	   Y=Y0+ViewStepY*iy;
//	   ST->X->PosMove(X,1e9,1e9);
//	   ST->Y->PosMove(Y,1e9,1e9);
//      ST->Z->PosMove(Z0+DZ+2,1e4,1e4);
    run->GetView()->SetCoordXY(ST->X->GetPos(),ST->Y->GetPos());	
	Nc=ScanAndFillView(Z0,DZ,Side,run->GetView(),RawImages,X,Y);
    run->GetView()->SetCoordZ(ST->Z->Reference,ST->Z->Reference+ST->EmBotLayer,ST->Z->Reference+ST->EmBotLayer+ST->EmBase,ST->Z->Reference+ST->EmBotLayer+ST->EmTopLayer+ST->EmBase);
    run->GetView()->GetHeader()->SetViewID(vid++);
    run->GetView()->GetHeader()->SetAreaID(AreaId);
	run->GetView()->GetHeader()->SetStatus(ST->L->ReadTemperature());
	printf("dbg 0\n");
	printf("Cluster number: %d\n", Nc);
	printf("dbg 1\n");
	run->AddView();
	printf("dbg 2\n");
// run->Save();

	   res=ST->Y->WaitForMoveComplete(2e3);
	   res*=ST->X->WaitForMoveComplete(2e3);
	   res*=ST->Z->WaitForMoveComplete(2e3);

	   if(!res) {
		ST->X->PosMove(X,1e9,1e9);
		ST->Y->PosMove(Y,1e9,1e9);
		ST->Z->PosMove(Z0+DZ+2,1e4,1e4);
		res=ST->Y->WaitForMoveComplete(2e3);
		res*=ST->X->WaitForMoveComplete(2e3);
		res*=ST->Z->WaitForMoveComplete(2e3);		       
		if(!res) {printf("Position is not reached! Aborting scan.\n");return 0;}
	   }
//	   run->Save();

	}
// finish last frame
	
    run->GetView()->SetCoordXY(ST->X->GetPos(),ST->Y->GetPos());
	if(IsTop) {X=X0; Y=Y0;} //go back to the fragment start to prepare for bottom, otherwise just stop
	else {X=0;Y=0;}
	Nc=ScanAndFillView(Z0,DZ,Side,run->GetView(),RawImages,X,Y);
    run->GetView()->SetCoordZ(ST->Z->Reference,ST->Z->Reference+ST->EmBotLayer,ST->Z->Reference+ST->EmBotLayer+ST->EmBase,ST->Z->Reference+ST->EmBotLayer+ST->EmTopLayer+ST->EmBase);
    run->GetView()->GetHeader()->SetViewID(vid++);
    run->GetView()->GetHeader()->SetAreaID(AreaId);
	printf("Cluster number: %d\n", Nc);	
	run->AddView();	
// run->Save();

	   res=ST->Y->WaitForMoveComplete(2e3);
	   res*=ST->X->WaitForMoveComplete(2e3);
	   res*=ST->Z->WaitForMoveComplete(2e3);

	   if(!res) {
		ST->X->PosMove(X,1e9,1e9);
		ST->Y->PosMove(Y,1e9,1e9);
		ST->Z->PosMove(Z0+DZ+2,1e4,1e4);
		res=ST->Y->WaitForMoveComplete(2e3);
		res*=ST->X->WaitForMoveComplete(2e3);
		res*=ST->Z->WaitForMoveComplete(2e3);		       
		if(!res) {printf("Position is not reached! Aborting scan.\n");return 0;}
	   }
	   return Nc;

}

