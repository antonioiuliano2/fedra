//////////////////////////////////////////////////////////////////////////////////////
//
// EdbRun test suite
//
// VT 11-04-2000
//
//////////////////////////////////////////////////////////////////////////////////////

//____________________________________________________________________________________
void w( int rid=2, int nviews=2 )
{
  // test of writing into run file 
  // Parameters:  rid    - run identifier
  //              nviews - number of views to be written
  //

  EdbRun *run = new EdbRun(rid,"RECREATE");

  float width   = 120, height  = 110;     // microns
  int   colomns = 512, rows    = 512;     // pixels

  run->GetHeader()->SetCCD( width, height, rows, colomns );

  int    plateID=111;
  float  up=100, base=850, down=100;
  float  shrU=2, shrD=2; 

  run->GetHeader()->SetPlate( plateID, up,base,down, shrU,shrD );

  float z1 = 0, z2=100, z3=800, z4=900; 
  //run->GetPlateSet()->Set( z1,z2,z3,z4 );

  run->Print();

  EdbView       *view=run->GetView();
  EdbViewHeader *vh = view->GetHeader();

  for( int ivc=0;  ivc<nviews; ivc++) {           // views cycle

    Int_t    iv   = ivc;   // View ID in the Area
    Int_t    ia   = 20;    // Area ID in Run (prediction id)
    Float_t  xf   = 30;    // stage coord, top left corner
    Float_t  yf   = 40;    // 
    Float_t  z1   = 0;     //
    Float_t  z2   = 100;   //
    Float_t  z3   = 800;   //
    Float_t  z4   = 900;   //
    Int_t    nft  = 3;     // top    | number of frames in the view (0,1,2...16...)
    Int_t    nfb  = 3;     // botoom | 
    Float_t  *levels = new Float_t[nft+nfb];

    vh->SetViewID( iv );
    vh->SetAreaID( ia );
    vh->SetCoordXY(xf,yf);
    vh->SetCoordZ(z1,z2,z3,z4);
    vh->SetNframes(nft,nfb);
    vh->SetAffine(1,0,0,1,0,0);

    for( int ifr=0; ifr<nft+nfb; ifr++ ) {        // frames cycle

      for( int icl=0; icl<3; icl++ ) {            // clusters cycle

	float x=ivc*1000, y=2, z=3;       // cluster coord
	float ar  =40;             // cluster area
	float vol =50;             // cluster volume
	int   side=0;              // emulsion side (top=0, bottom=1) ?
	if( ifr>nft-1 ) side=1;

	view->AddCluster(x,y,z, ar, vol, ifr, side);

      }                                           // end of clusters cycle

        levels[ifr] = 10.*ifr+ivc;
	view->AddFrame( ifr, levels[ifr] );
     
    }                                             // end of frames cycle

    vh->SetZlevels(nft+nfb,levels);

    run->AddView(view);

  }                                               // end of views cycle

  run->Close();
}

//____________________________________________________________________________________
void wRan( int rid=2, int nareas=2, int nviews=2 )
{
  // test of writing into run file random generated clusters 
  // Parameters:  rid    - run identifier
  //              nviews - number of views to write
  //

  //  TStopwatch timer;
  //printf("Starting timer\n");
  //timer.Start();

  EdbRun *run = new EdbRun(rid,"RECREATE");

  run->SetTitle("Test row data");
  run->SetComment("I will not say that women have no  character; \n\t\t rather, they have a new one every day. \n \t\t\t\t -- Heine ");

  float width   = 150, height  = 150;     // microns
  int   colomns = 512, rows    = 512;     // pixels

  run->GetHeader()->SetCCD( width, height, rows, colomns );

  int    plateID=111;
  float  up=100, base=200, down=100;
  float  shrU=2, shrD=2;

  run->GetHeader()->SetPlate( plateID, up,base,down, shrU,shrD );

  float z1 = 0, z2=100, z3=300, z4=400;
  //run->GetPlateSet()->Set( z1,z2,z3,z4 );

  //run->GetHeader()->Write("RunHeader");
  run->Print();

  EdbCluster    *cluster=0;
  EdbSegment    *seg=new EdbSegment();

  EdbView       *view=run->GetView();
  EdbViewHeader *vh = view->GetHeader();

  for( int iac=0;   iac<nareas; iac++) {             // areas cycle
    printf("Process area: %d  (%d)\n", iac, nareas );

    for( int ivc=0;  ivc<nviews; ivc++) {           // views cycle

      Int_t    iv   = ivc;   // View ID in the Area
      Int_t    ia   = iac;   // Area ID in Run (prediction id)
      Float_t  xf   = 0;     // stage coord, top left corner
      Float_t  yf   = 0;     // 
      Int_t    nft  = 16;    // top    | number of frames in the view (0,1,2...16...)
      Int_t    nfb  = 16;    // botoom | 

      vh->SetViewID( iv );
      vh->SetAreaID( ia );
      vh->SetCoordXY(xf,yf);
      vh->SetCoordZ(z1,z2,z3,z4);
      vh->SetNframes(nft,nfb);
      vh->SetAffine(1,0,0,1,0,0);

      //      view->GenerateClustersFog(.1);

      float x0=0, y0=0, z0=0;
      float ax=0, ay=0;

      for( int itr=0; itr<350; itr++ ) {
	x0 = gRandom->Rndm()*512;
	y0 = gRandom->Rndm()*512;
	z0 = gRandom->Rndm()*400;
	ax = gRandom->Gaus( 0, 0.1 );
	ay = gRandom->Gaus( 0, 0.1 );
	x0 = x0 - ax*(z0-z1);
	y0 = y0 - ay*(z0-z1);

	seg->Set( x0,y0,  z1, ax, ay, 40, 1, 16, itr );
	view->AddSegment(seg);
	view->GenerateClustersSegment( seg, 25., 0.2 );
      }

      run->AddView(view);
      view->Clear();

    }
  }                                               // end of views cycle

  run->Close();
  //  timer.Print();
  //  printf("Time at the end of job = %f seconds\n",timer.CpuTime());
}

//____________________________________________________________________________________
void r(int rid=2)
{
  // test of reading from RUN rid

  EdbRun *run = new EdbRun(rid);
  
  int  nviews = run->GetEntries();

  EdbView    *view    =0;
  TList      *clFrame =0;
  EdbCluster *cluster =0;
  TObjArray  *frames  =0;         // array of images

  for( int iv=0; iv<nviews; iv++ ) {      // views cycle

    view = run->GetEntry(iv);
    view->Print();

    int nframes = view->GetNframesTop() + view->GetNframesBot();

    for( int ifr=0; ifr<nframes; ifr++) {          // frames cycle

      frames = view->GetFrames();
      //      if(frames) printf("Number of Frames = %d\n", frames->GetEntries() );

      clFrame = view->GetClustersFrame(ifr);

      int nclFr = clFrame->Capacity();

      for( int icl=0; icl<nclFr; icl++) {       // clusters cycle

	cluster = (EdbCluster*)(clFrame->At(icl));
	//	cluster->Print();

	float x = cluster->GetX();

      }                                         // end of clusters cycle

    }                                           // end of frames cycle

  }                                             // end of views cycle

}

//____________________________________________________________________________________
void read_clusters_fast(int rid=2)
{
  // test of clusters reading from RUN rid

  EdbRun *run = new EdbRun(rid);
  int  nviews = run->GetEntries();

  EdbView        *view     =0;
  EdbCluster     *cluster  =0;

  for( int iv=0; iv<nviews; iv++ ) {      // views cycle

    view = run->GetEntry(iv);
    view->Print();

    int ncl = view->Nclusters();

    int frame = 0;

    for( int icl=0; icl<ncl; icl++) {
      cluster = view->GetCluster(icl);
      cluster->Print();
      frame = cluster->GetFrame();
    }
  }
}


t(int n=1000)
{
  TH1F *hist = new TH1F("hist","hist",100,0,99);

  for(int i=0; i<n; i++)  hist->Fill( gRandom->Integer(100) );
  hist->Draw();
}
