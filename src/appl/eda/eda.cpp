#include<stdlib.h>
#include<string>
#include<EdbEDA.h>
#include<EdbLog.h>

void print_usage(){
	printf("Usage : eda.exe [-option] (value)\n");
	printf(
		"  opt  value\n"
		"  -f   filename  Read file (any).\n"
		"  -l   filename  Set Link def filename\n"
		"  -d   datatype  Set datatype for InitVolume\n"
		"                 0:cp, 10:cpfull, 100:tracks, 100: tracks+cp\n"
		"  -c   selection Set cut for InitVolume\n"
		"  -b   ibrick    Set Brick ID\n"
		"  -sb  sbrun     Read ScanBack tracks\n"
		"  -sbm sbrun     Read ScanBack, using microtrack\n"
		"  -sf  sfrun     Read ScanForth tracks\n"
		"  -sfm sfrun     Read ScanForth, using microtrack\n"
		"  -v             Do Vertexing\n"
		"  -n             Show nothing in initial view.\n"
		"  -lst filename  Read Track List file.\n"
		"  -t   filename  Read Text track.\n"
		" need space between option and value\n");

}


int main(int argc, char *argv[]){

	char lnkdef[50]="lnk.def";
	char datatype = 100;
	char rcut[200] = "1";
	
	int sbrun = -1;
	int sfrun = -1;
	int ibrick = 0;
	int use_microtrack = 0;
	int dovertex = 0;
	int drawtrack = 1;
	char listfile[256] = "";
	char textfile[256] = "";
	
	for(int i=0;i<argc;i++){
		std::string s(argv[i]);
		cout << s << endl;
		if(!s.find("-h")) {print_usage(); return 0;}
		if(!s.find("-l")&&s.find("-lst")==string::npos) sprintf(lnkdef,argv[++i]);
		if(!s.find("-d")) datatype = atoi(argv[++i]);
		if(!s.find("-c")) sprintf(rcut,argv[++i]);
		if(!s.find("-b")) ibrick = atoi(argv[++i]);
		if(!s.find("-sb"))  sbrun = atoi(argv[++i]);
		if(!s.find("-sf"))  sfrun = atoi(argv[++i]);
		if(!s.find("-sbm")||!s.find("-sbm")) use_microtrack = 1;
		if(!s.find("-v"))   dovertex=1;
		if(!s.find("-n"))   drawtrack=0;
		if(!s.find("-lst")) sprintf(listfile,argv[++i]);
		if(!s.find("-t"))   sprintf(textfile,argv[++i]);
		if(!s.find("-f"))   sprintf(lnkdef,argv[++i]);
	}
	
	int argc2=1;
	char *argv2[]={"-l"};
	TRint app("EDA",&argc2, argv2);

	if(gSystem->AccessPathName(lnkdef, kReadPermission))     {
	  Log(1,"\neda","ERROR: can not open input file: %s !!!\n",lnkdef);
	  print_usage();
	  return 0;
	}
	EdbEDA *EDA=new EdbEDA(lnkdef,datatype,rcut);

	if(dovertex) EDA->GetTrackSet("TS")->DoVertexing(NULL,3);
	EDA->GetTrackSet("TS")->SetDraw(drawtrack);
	if(strlen(listfile)>1) EDA->GetTrackSet("TS")->ReadListFile(listfile);
	
	EDA->SetBrick(ibrick);
	if(sbrun>0)  EDA->GetTrackSet("SB")->ReadPredictionScan(ibrick, 1,sbrun, use_microtrack); 
	if(sfrun>0)  EDA->GetTrackSet("SF")->ReadPredictionScan(ibrick, 1,sfrun, use_microtrack); 

	if(strlen(textfile)>1) EDA->GetTrackSet("MN")->ReadTextTracks(textfile);


	EDA->Run();
	
	app.Run();
	return 0;
}
