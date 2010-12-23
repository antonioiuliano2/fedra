void eda(){

	// Parameters for EDA
	char lnkdef[50]="lnk.def";
	char datatype = 100;
	char rcut[200] = "1";
	
	int ibrick = 0;  // Brick ID.

	int sbrun = -1;  // run number of Scanback
	int sfrun = -1;  // run number of Scanforth
	int use_microtrack = kFALSE; // read microtrack for SB and SF or not.

	int dovertex = 1;  // Do vertexing
	int drawtrack = 1; // Draw TS tracks
	char listfile[256] = ""; // list file name. if it's given, read it as default.
	char textfile[256] = ""; // text file name. if it's given, read it as MN.

	char filteredtext[256]  = ""; // Filtered text filename.
	if(strlen(filteredtext)>1) { datatype=-1; dovertex=0;}
	
	
	// Start EDA
	
	EdbEDA *EDA=new EdbEDA(lnkdef,datatype,rcut);

	if(strlen(filteredtext)>1) {
		EdbPVRec *pvr = EDA->ReadFilteredText(filteredtext);
		EDA->GetAreaSet()->SetAreas(pvr);
	}

	if(dovertex) EDA->GetTrackSet("TS")->DoVertexing(NULL,3);
	EDA->GetTrackSet("TS")->SetDraw(drawtrack);
	if(strlen(listfile)>1) EDA->GetTrackSet("TS")->ReadListFile(listfile);
	
	EDA->SetBrick(ibrick);
	if(sbrun>0)  EDA->GetTrackSet("SB")->ReadPredictionScan(ibrick, 1,sbrun, use_microtrack); 
	if(sfrun>0)  EDA->GetTrackSet("SF")->ReadPredictionScan(ibrick, 1,sfrun, use_microtrack); 

	if(strlen(textfile)>1) EDA->GetTrackSet("MN")->ReadTextTracks(textfile);

	EDA->Run();
}
