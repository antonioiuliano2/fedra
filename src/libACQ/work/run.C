//#include "vgen.C"
#include <exception>

int scanpreds(int PLATE, int MINOR)
{
int BRICK=92;

bool loadresult;
try {loadresult=PC->LoadPlate(BRICK,PLATE,0,0); printf("Load plate returned %d.\n",loadresult);}
catch (...) {printf("Exception caught in LoadPlate!\n"); return 0;}
if(!loadresult) {printf("can't load plate! Exiting.\n");  return 0;}
try { printf("Scanmarks is about to execute..\n"); loadresult=scanmarks();}
catch (...) {cout << "Exception caught in scanmarks!" << endl; return 0;}
if(!loadresult) return 0;
 char cmd[128];
// Initializing run
 try
 {
	EdbMark *xyz=new EdbMark();
	if(!FM->eFoundMarksSet) {printf("Marks are not scanned! Exiting.\n"); return 0;}
  start=atoi((gSystem->Now()).AsString());
  end=atoi((gSystem->Now()).AsString());
  sprintf(cmd,"del .\\data\\r.root");
  gSystem->Exec(cmd);
  run=new EdbRun(".\\data\\r.root","RECREATE");
  run->SetTitle("Test of prediction scan");
  run->SetComment("  Test run: online data taking with libACQ. \n Parameters are selected to fit OPERA  plates \n");
  EdbRunHeader *header = run->GetHeader();
  header->SetCCD( o->SizeX  ,       // number of pixels
		  o->SizeY  ,       // 
		  -999  ,       // ccd pixel size in microns
		  -999  ,       //  (is not important for analysis)
		  "MC1310",
		  "Mikrotron CMOS High Speed  camera" );

  header->SetObjective( 50.  ,       // magnification nominal value
			s->X->Invert*o->SizeX*o->PixelToMicronX,       // pixel size X |
			0.   ,       //              | 
			0.   ,       //              |  CCD2Stage affine  
		    s->Y->Invert*o->SizeY*o->PixelToMicronY,       // pixel size Y |   transformation:
			0.   ,       //              | pixels -> microns
			0.   ,       //              |
			"NIKON",
			"Dry Objective " );



  header->SetArea( TS->Nx*TS->Ny   ,         // number of views/area to scan
		   TS->ViewStepX ,        // X-step in microns
		   TS->ViewStepY ,        // Y-step in microns
		   TS->Layers   ,        // NframesTop
		   TS->Layers ,  // NframesBot
		   0 );     //path  

   if(run->eMarks) delete (run->eMarks);
  (run->eMarks)=FM->eFoundMarksSet;
	printf("Markset written to file:\n");
    (run->eMarks)->Print();
 


  header->SetPlate( 6   ,        // plate ID
		    s->EmTopLayer  ,        // emulsion up
		    s->EmBase ,        // base
		    s->EmBotLayer  ,        // emulsion down
		    1.0 ,        // shrinkage up
		    1.0 ,        // shrinkage down
		    "W2P6R",
		    "Welded brick2, plate 6" );
 }
 catch (...) {cout << "Exception caught in Run initialization!" << endl; return 0;}

// run initialized

int scan_zone_id=98;
int proc_zone_id=94;
bool data_ready=false;
int proctracks=0;
int dummy;

char fname[128];
sprintf(fname,".\\data\\pred%d.%d.0.%d.txt",BRICK,PLATE,MINOR);
FILE* fp=fopen(fname,"r");
if(fp==NULL) {printf("Prediction file %s can't be opened.\n",fname); return 0;}
char line[256];
int id;
float XX0,YY0,X,Y,TX,TY;
int AreaID=0;

try while(fgets(line,256,fp)!=NULL) //main loop
 {
//		if(kbhit()){getch();printf("Paused. Continue? (y/n)"); if(getchar()=='n') break;}
   sscanf(line,"%d %f %f %f %f",&id,&X,&Y,&TX,&TY);
   printf("Predicted segment N %d: %f %f %f %f\n",id,X,Y,TX,TY);
	xyz->SetX(X);
	xyz->SetY(Y);
	xyz->Transform(FM->eFoundMarksSet->Abs2Stage());
	X=xyz->X();
	Y=xyz->Y();
   printf("Stage coordinates: %f %f \n",X,Y);
   if(proctracks==0) 
	   	if(!focus(Cluster_Threshold))
	{  
		if(!finefocus(Cluster_Threshold,.4)) return 0;
		if(!finefocus(Cluster_Threshold,1)) return 0;

	}
    
   if(proctracks>0) 
   {
	if(!focus(Cluster_Threshold))
	{  
		if(!finefocus(Cluster_Threshold,.4)) {
			s->X->PosMove(X,1e9,1e9);
			s->Y->PosMove(Y,1e9,1e9);
			s->X->WaitForMoveComplete(1e4);
			s->Y->WaitForMoveComplete(1e4);
			XX0=X; YY0=Y; 
			proctracks++; 
			continue;
		}
		if(!finefocus(Cluster_Threshold,1)) {
			s->X->PosMove(X,1e9,1e9);
			s->Y->PosMove(Y,1e9,1e9);
			s->X->WaitForMoveComplete(1e4);
			s->Y->WaitForMoveComplete(1e4);
			XX0=X; YY0=Y; 
			proctracks++; 
			continue;
		}

	}
	
	 TS->Layers=Layers;
     ////////////////////////// T O P /////////
	 o->Threshold=ThrTop; 
 //    TS->FocusScanPrediction(true, AreaID, true, XX0, YY0, run, false, 0, 0);
     TS->FastScanPrediction(true, AreaID, true, XX0, YY0, run, false, 0, 0);
//	 o->ShowImage(4);
//	 AreaID++;
     ////////////////////////// BOT /////////
	 o->Threshold=ThrBot; 
//     TS->FocusScanPrediction(false, AreaID, true, XX0, YY0, run, false, X, Y);
     TS->FastScanPrediction(false, AreaID, true, XX0, YY0, run, false, X, Y);
//	 o->ShowImage(4);
	 AreaID++;
   }
  XX0=X; YY0=Y; //to remember previous 
  proctracks++;
  printf("Prepared to scan %d predictions\n",proctracks);
 } 

 catch (...) {cout << "Exception caught in main loop!" << endl; return 0;}

 // eND OF MAIN LOOP
 try {
	if(!focus(Cluster_Threshold))
	{  
		if(!finefocus(Cluster_Threshold,.4)) return 0;
		if(!finefocus(Cluster_Threshold,1)) return 0;

	}
	
	 TS->Layers=Layers;
     ////////////////////////// T O P /////////
	 o->Threshold=ThrTop; 
//     TS->FocusScanPrediction(true, AreaID, true, X, Y, run, false, 0, 0);
     TS->FastScanPrediction(true, AreaID, true, X, Y, run, false, 0, 0);
//	 o->ShowImage(4);
//	 AreaID++;
     ////////////////////////// BOT /////////
	 o->Threshold=ThrBot; 
 //    TS->FocusScanPrediction(false, AreaID, true, X, Y, run, false, 0, 0);
     TS->FastScanPrediction(false, AreaID, true, X, Y, run, false, 0, 0);
//	 o->ShowImage(4);
	 AreaID++;

run->Save();
run->Close();
delete run;
 }
  catch (...) {cout << "Exception caught in Postprocessing!" << endl; return 0;}

char rootfname[128];
 sprintf(rootfname,"data\\tst%d.%d.0.%d.root",BRICK,PLATE,MINOR); 
//else  sprintf(rootfname,"%s",ofile); 
printf("move data\\r.root %s\n",rootfname);
sprintf(cmd,"move data\\r.root %s",rootfname);
gSystem->Exec(cmd);
/*Link1(BRICK,PLATE,98,false);
 	sprintf(cmd,"mv data/%02d_098.cp.root data/%02d_%03d.cp.root ",PLATE,PLATE,MINOR);
 	printf("mv data/%02d_098.cp.root data/%02d_%03d.cp.root\n ",PLATE,PLATE,MINOR);
 	gSystem->Exec(cmd);
 	sprintf(cmd,"mv data/tst%d.%d.0.%d.root data/tst%d.%d.0.%d.root",BRICK,PLATE,98,BRICK,PLATE,MINOR);
 	printf("mv data/tst%d.%d.0.%d.root data/tst%d.%d.0.%d.root\n",BRICK,PLATE,98,BRICK,PLATE,MINOR);
 	gSystem->Exec(cmd);  
*/

fclose(fp);
start=atoi((gSystem->Now()).AsString());
printf("%d ms spent for scanning.\n",start-end);
delete xyz;
return 1;
}



void Run(bool AbsCoord=false, float X0, float Y0, float DDX, float DDY, const char* File, bool Images=false)
{
	// Scan emulsion zone with the use of faster async function TS->ScanAndFillView

 // s->L->ON();
		int res=0;
	float X,Y;
	int Vn=0;
	int vid=0;
	long start,end,cur;
	    EdbMark *xyz=new EdbMark();
		xyz->SetX(X0);
		xyz->SetY(Y0);
	if(AbsCoord && FM->eFoundMarksSet) xyz->Transform(FM->eFoundMarksSet->Abs2Stage());
	X0=xyz->X();
	Y0=xyz->Y();
	printf("Going to %f, %f\n",X0,Y0);
	s->X->PosMove(X0,1e6,1e6);
	s->Y->PosMove(Y0,1e6,1e6);
	res=s->Y->WaitForMoveComplete(1e4);
	res*=s->X->WaitForMoveComplete(1e4);
	if(!res) return;
	printf("Reached %f, %f\n",s->X->GetPos(),s->Y->GetPos());
			    start=atoi((gSystem->Now()).AsString());

//	if(!focus(Cluster_Threshold))  return;
//	finefocus(Cluster_Threshold);
		    end=atoi((gSystem->Now()).AsString());
//		printf("%d ms spent for focusing\n",end-start);

  run=new EdbRun(File,"RECREATE");
  run->SetTitle("Test of libACQ");
  run->SetComment("  Test run: online data taking with libACQ. \n Parameters are selected to fit OPERA  plates \n");
  EdbRunHeader *header = run->GetHeader();
  header->SetCCD( o->SizeX  ,       // number of pixels
		  o->SizeY  ,       // 
		  -999  ,       // ccd pixel size in microns
		  -999  ,       //  (is not important for analysis)
		  "MC1310",
		  "Mikrotron CMOS High Speed  camera" );

  header->SetObjective( 50.  ,       // magnification nominal value
			s->X->Invert*o->SizeX*o->PixelToMicronX,       // pixel size X |
			0.   ,       //              | 
			0.   ,       //              |  CCD2Stage affine  
		    s->Y->Invert*o->SizeY*o->PixelToMicronY,       // pixel size Y |   transformation:
			0.   ,       //              | pixels -> microns
			0.   ,       //              |
			"NIKON",
			"Dry Objective " );



  header->SetArea( TS->Nx*TS->Ny   ,         // number of views/area to scan
		   TS->ViewStepX ,        // X-step in microns
		   TS->ViewStepY ,        // Y-step in microns
		   TS->Layers   ,        // NframesTop
		   TS->Layers ,  // NframesBot
		   0 );     //path  

    run->eMarks=FM->eFoundMarksSet;
 


  header->SetPlate( 6   ,        // plate ID
		    s->EmTopLayer  ,        // emulsion up
		    s->EmBase ,        // base
		    s->EmBotLayer  ,        // emulsion down
		    1.0 ,        // shrinkage up
		    1.0 ,        // shrinkage down
		    "W2P6R",
		    "Welded brick2, plate 6" );

// calculate fragment division
int indFragX=0;
int indFragY=0;
int AreaID=0;
float FragStepX = TS->ViewStepX * TS->Nx;
float FragStepY = TS->ViewStepY * TS->Ny;
int NumFragsX=int(DDX/FragStepX)+1;
int NumFragsY=int(DDY/FragStepY)+1;

for( indFragY=0; indFragY<NumFragsY; indFragY++)
for( indFragX=0; indFragX<NumFragsX; indFragX++)
{
	X=X0+indFragX * FragStepX;
	Y=Y0+indFragY * FragStepY;
//finefocus(Cluster_Threshold,3);
	if(!focus(Cluster_Threshold))
	{  
		if(!finefocus(Cluster_Threshold,.4)) return;
		if(!finefocus(Cluster_Threshold,1)) return;

	}
    ////////////////////////// T O P /////////
	o->Threshold=ThrTop; 
	TS->FastScanFragment(true, AreaID, AbsCoord, X,Y, run, Images);
	////////////////////////// BOTTOM //////////////////////////
	o->Threshold=ThrBot;
	TS->FastScanFragment(false, AreaID, AbsCoord, X,Y, run, Images);
	AreaID++;

}
		start=atoi((gSystem->Now()).AsString());
       printf("%d ms spent for scanning.\n",start-end);
	   run->Save();
	run->Close();
	delete xyz;

}

