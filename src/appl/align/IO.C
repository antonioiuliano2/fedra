void make_views_map( EdbRun *edbRun, TIndexCell &up, TIndexCell &down )
{
  int nentr = edbRun->GetEntries();
  printf("Make views entry map,  nentr = %d\n",nentr);

  Long_t v[2];   // areaID,entry
  EdbViewHeader *head=0;
  for(int iv=0; iv<nentr; iv++ ) {

    head = edbRun->GetEntryHeader(iv);

    v[0]=head->GetAreaID();
    v[1]=iv;

    if(head->GetNframesTop()==0) {         // fill down views
      down.Add(2,v);
    }
    else if(head->GetNframesBot()==0) {    // fill up views
      up.Add(2,v);
    }
  }

  //up->PrintStat();
  //down->PrintStat();
}

//--------------------------------------------------------------
void make_views_coord_map( EdbRun *edbRun, TIndexCell &up, TIndexCell &down )
{
  int nentr = edbRun->GetEntries();
  printf("Make views coordinate map,  nentr = %d\n",nentr);


  TIndexCell upc;
  TIndexCell downc;
  Long_t v[3];   // x,y,entry
  EdbViewHeader *head=0;

  Long_t xx=0, yy=0;
  float cx = 1000., cy = 1000.;  // 2x2 mm cells
  float dx = 400. , dy = 400.;   // 400 microns margins
  float xv,yv;
  int mx[9] = {0, 0, 0,-1,  1, -1,-1, 1, 1};
  int my[9] = {0,-1, 1, 0,  0, -1, 1,-1, 1};

  for(int iv=0; iv<nentr; iv++ ) {

    head = edbRun->GetEntryHeader(iv);
    yv = head->GetXview();
    xv = head->GetYview();
    xx = xv/cx;
    yy = yv/cx;
    v[0] = xx;
    v[1] = yy;
    v[2] = iv;

    if(head->GetNframesBot()==0) {              // fill up views
      upc.Add(3,v);
    }
    else if(head->GetNframesTop()==0) {         // fill down views
      downc.Add(3,v);                           // add center
      for( int im=1; im<5; im++ ) {             // add sides margins
	v[0] = ( xv+dx*mx[im] ) / cx;
	v[1] = ( yv+dy*my[im] ) / cy;
	if( (v[0] != xx) || (v[1] != yy) )
	    downc.Add(3,v);
      }
      for( int im=5; im<9; im++ ) {             // add angles margins
	v[0] = ( xv+dx*mx[im] ) / cx;
	v[1] = ( yv+dy*my[im] ) / cy;
	if( (v[0] != xx) && (v[1] != yy) )
	  //	  if(!downc.Find(3,v))              
	    downc.Add(3,v);
      }

    }

  }

  upc.Sort();
  downc.Sort();

  upc.SetName("x:y:entry");
  downc.SetName("x:y:entry");

  upc.PrintStat();
  downc.PrintStat();

  TIndexCell *clx=0;
  TIndexCell *cly=0;

  int areac=0;
  int nix,niy,nie;
  nix=upc.N(1);
  for(int ix=0; ix<nix; ix++) {
    clx = upc.At(ix);
    xx = clx->Value();
    niy=clx->N(1);
    for(int iy=0; iy<niy; iy++) {
      cly  = clx->At(iy);
      yy = cly->Value();
      areac++;

      nie = cly->N(1);
      for(int ie=0; ie<nie; ie++) {
	v[0]=areac;
	v[1]  = cly->At(ie)->Value();
	up->Add(2,v);
      }
      
      cly = downc.Find(xx)->Find(yy);
      nie=cly->N(1);
      for(int ie=0; ie<nie; ie++) {
	v[0] = areac;
	v[1] = cly->At(ie)->Value();
	down->Add(2,v);
      }
    }
  }

  up->Sort();
  down->Sort();

  up->PrintStat();
  down->PrintStat();
}

//--------------------------------------------------------------
void getDataEdb( EdbRun *edbRun,
                 EdbPVRec *pvol,
                 TIndexCell *up, TIndexCell *down, 
		 float shrU, float shrD,
                 float uoff[2], float doff[2], float plate[3] )
{
  float base=plate[1];

  //  float dzu = edbRun->GetHeader()->GetPlate()->GetUp()   * shrU/2.;
  float dzu = plate[2]/2.;
  EdbPattern *patU = new EdbPattern(0.,0., base+dzu);
  patU->SetID(1);

  getPatternEdb(edbRun,patU,up,shrU,uoff,dzu);
  pvol->AddPattern(patU);

  //  float dzd = edbRun->GetHeader()->GetPlate()->GetDown() * shrD/2.;
  float dzd = plate[0]/2.;
  EdbPattern *patD = new EdbPattern(0.,0., -dzd );
  patD->SetID(2);

  getPatternEdb(edbRun,patD,down,shrD,doff,-dzd);
  pvol->AddPattern(patD);
}

//--------------------------------------------------------------
void getPatternEdb( EdbRun *edbRun,
		    EdbPattern *pat,
		    TIndexCell *elist,
		    float shr, float off[2], float dz )
{
  EdbSegP    *segP = new EdbSegP();

  EdbView    *view=edbRun->GetView();
  EdbSegment *seg=0;

  float x,y,z,tx,ty;

  int nseg=0;
  int nentr = edbRun->GetEntries();
  //printf("nentr = %d\n",nentr);

  EdbViewHeader *head=0;
  int entry;

  int nsegV;
  int niu=elist->N();
  for(int iu=0; iu<niu; iu++) {
    entry = elist->At(iu)->Value();
    view = edbRun->GetEntry(entry);


    head = view->GetHeader();

    printf(" View %d : \t %d/%d ",
  	   entry,head->GetNframesTop(),head->GetNframesBot() );
    nseg=0;

    nsegV=view->Nsegments();
    for(int j=0;j<nsegV;j++) {

      seg = view->GetSegment(j);

      if( !cut_seg(seg->GetX0(), seg->GetY0(),
  		   seg->GetTx(), seg->GetTy(), 
  		   seg->GetPuls() )  )          continue;

      nseg++;
      //segP->SetDZ( seg->GetDz() );

      tx = seg->GetTx()/shr + off[0];
      ty = seg->GetTy()/shr + off[1];
      x  = (seg->GetX0()+view->GetXview()) + dz*tx;
      y  = (seg->GetY0()+view->GetYview()) + dz*ty;
      segP->Set( seg->GetID(),x,y,tx,ty);
      segP->SetZ( pat->Z() );
      segP->SetW(seg->GetPuls());
      segP->SetVid(entry,j);
      pat->AddSegment( *segP);

    }
    
    printf("\t nseg = %d  ( %d rejected ) \n", nseg, view->Nsegments()-nseg );
    
  }
   
  
}


//------------------------------------------------------------------------
void getDataCouples( const char *file_name, 
		     const char *tree_name="couples",
		     EdbPattern *pat )
{
  TTree *tree;
  EdbSegCouple    *cp = 0;
  EdbSegP         *s1 = 0;
  EdbSegP         *s2 = 0;
  EdbSegP         *s  = 0;

  printf("get couples from file: %s\n",file_name);

  TFile *f = new TFile( file_name );
  if (f)  tree = (TTree*)f->Get(tree_name);

  int nentr = tree->GetEntries();

  //tree->SetBranchStatus("*"  ,0 );
  //tree->SetBranchStatus("s.*"  ,1 );

  //tree->SetBranchAddress("cp"  , &cp );
  //tree->SetBranchAddress("s1." , &s1 );
  //tree->SetBranchAddress("s2." , &s2 );
  tree->SetBranchAddress("s."  , &s  );

  int nseg = 0;

  for(int i=0; i<nentr; i++ ) {
    tree->GetEntry(i);
    if( !cut_seg_cp( s->X(),s->Y(),s->TX(),s->TY(),s->W()) )  continue;
    s->SetZ(pat->Z());
    pat->AddSegment( *s );
    nseg++;
  }
  printf("%d (of %d) segments readed from file %s\n", nseg,nentr,file_name );
  f->Close();
}

//--------------------------------------------------------------
void getDataFile( const char *file, EdbPatternsVolume *pvol, float zpat )
{
  EdbPattern *pat = new EdbPattern();

  FILE *ff = fopen( file, "r");
  int id;
  float ax,ay,x,y;
  while( 5 == fscanf(ff,"%d %f %f %f %f", &id,&ax,&ay,&x,&y) ) {
    pat->AddSegment( id, x, y, ax, ay );
  }
  printf("%d segments read from file %s\n", pat->N(),file);
  fclose(ff);

  pat->SetZ(zpat);
  pvol->AddPattern(pat);

  pvol->SetPatternsID();
  pvol->FillTree(pat,pat,0);

}

//--------------------------------------------------------------
void getDataTreeJ( const char *file, 
		  int npl, int entries[], float dz,
		  EdbPatternsVolume *pvol )
{
  EdbPattern *pat = 0;
  EdbSegP    *segP = new EdbSegP();

  TFile  *ff    = new TFile(file);
  TTree  *tfxx = (TTree*)ff->Get("tfxx");

  NSFxxRawDB   *rawdb=0;
  NSSegRawDB   *seg=0;

  tfxx->SetBranchStatus("*",1);

  tfxx->SetBranchAddress("fxx",&rawdb);

  float zpl=0,ax=0,shr=1.;
  int   pos;

  for(int ipl=0; ipl<npl; ipl++ ) {

    tfxx->GetEntry( entries[ipl] );

    pos = (int)(rawdb->getPOS()/10);

    zpl = (36-pos)*dz;
    pat = new EdbPattern(0,0,zpl);

    printf( "**** %d (%d)  nseg=%d  run: %d   event: %d  plate = %d\n",
	    ipl,npl, rawdb->N(),rawdb->getRUN(),rawdb->getEVENT(),rawdb->getPOS() );

    int nraw=rawdb->N();
    for( int i=0; i<nraw; i++ ) {
      seg = (NSSegRawDB*)(rawdb->getSegments()->At(i));

      //if( TMath::Abs(seg->getAY()) < gAYcut ) continue;
      if( seg->getAX()*10000 == -44. ) continue;
      if( seg->getAY()*10000 == 0.   ) continue;

      //if( seg->getAX()<.01) continue;
      //if( TMath::Abs(seg->getAY()) > .1 ) continue;
      //if( seg->getPH()<65000 ) continue;

      if(rawdb->getPOS()==12) ax=seg->getAX()-.05;
      else ax=seg->getAX();

      segP->Set( seg->getISG(), 
		 seg->getX(), 
		 seg->getY(), 
		 ax*shr, 
		 seg->getAY()*shr );
      segP->SetZ(zpl);
      segP->SetW(seg->getPH());
      pat->AddSegment( *segP);
    }

    pvol->AddPattern(pat);

    pvol->SetPatternsID();
  }

  ff->Close();
}

//--------------------------------------------------------------
void getDataTree( const char *file, 
		  int npl, int entries[], float dz1, float dz2, float shr,
		  EdbPatternsVolume *pvol )
{
  EdbPattern *pat = 0;
  EdbSegP    *segP = new EdbSegP();

  TFile  *ff    = new TFile(file);
  TTree  *tfxx = (TTree*)ff->Get("tfxx");

  NSFxxRawDB   *rawdb=0;
  NSSegRawDB   *seg=0;

  tfxx->SetBranchStatus("*",1);

  tfxx->SetBranchAddress("fxx",&rawdb);

  int   pl_pos=0, surf=0;
  float zpl=0,ax=0;

  for(int ipl=0; ipl<npl; ipl++ ) {

    tfxx->GetEntry( entries[ipl] );

    if(rawdb->getPOS()==11) zpl=0;
    if(rawdb->getPOS()==12) zpl=dz2;

    pat = new EdbPattern();
    pat->SetZ(zpl);

    printf( "**** npl=%d, nseg=%d \n",npl, rawdb->N() );

    int nraw=rawdb->N();
    for( int i=0; i<nraw; i++ ) {
      seg = (NSSegRawDB*)(rawdb->getSegments()->At(i));

      //if( TMath::Abs(seg->getAY()) < gAYcut ) continue;
      //if( seg->getAX()*10000 == -44. ) continue;
      //if( seg->getAY()*10000 == 0.   ) continue;

      if( seg->getAX() > .1) continue;
      if( TMath::Abs(seg->getAY()) > .1 ) continue;
      //if( seg->getPH()<75000 ) continue;

      //if(rawdb->getPOS()==12) ax=seg->getAX()-.05;
      //else ax=seg->getAX();

      
      int aoffx=-.01;  // empiric angular offset
      int aoffy=-.02;  // empiric angular offset

      segP->Set( seg->getISG(), 
		 seg->getX(), 
		 seg->getY(), 
		 seg->getAX()*shr-.01, 
		 seg->getAY()*shr-.02 );
      segP->SetZ(zpl);
      segP->SetW(seg->getPH());
      pat->AddSegment( *segP);
    }

    pvol->AddPattern(pat);

    pvol->SetPatternsID();
    pvol->FillTree(pat,pat,0);
  }

  ff->Close();
}

///------------------------------------------------------------
TTree *inittree( char *tree_name="couples", 
		 char *file_name="aedb.root", 
		 char *mode="RECREATE" )
{
  TTree *tree;

   tree = (TTree*)gROOT->FindObject(tree_name);
   if (!tree) {
      TFile *f = new TFile(file_name,mode);
      if (f)  tree = (TTree*)f->Get(tree_name);
      if(!tree) {

	tree = new TTree(tree_name,tree_name);

	int pid1,pid2;
	EdbSegCouple *cp=0;
	EdbSegP      *s1=0;
	EdbSegP      *s2=0;
	EdbSegP      *s=0;
	
	tree->Branch("pid1",&pid1,"pid1/I");
	tree->Branch("pid2",&pid2,"pid2/I");
	tree->Branch("cp","EdbSegCouple",&cp,32000,99);
	tree->Branch("s1.","EdbSegP",&s1,32000,99);
	tree->Branch("s2.","EdbSegP",&s2,32000,99);
	tree->Branch("s." ,"EdbSegP",&s,32000,99);

      }
   }

   return tree;
}


//------------------------------------------------------------
void filltree( TTree *tree, EdbPVRec *al, int fillraw=0 )
{
  EdbPatCouple *patc=0;

  int pid1,pid2;
  EdbSegCouple *cp=0;
  EdbSegP      *s1=0;
  EdbSegP      *s2=0;
  EdbSegP      *s=0;

  tree->SetBranchAddress("pid1",&pid1);
  tree->SetBranchAddress("pid2",&pid2);
  tree->SetBranchAddress("cp"  ,&cp);
  tree->SetBranchAddress("s1." ,&s1);
  tree->SetBranchAddress("s2." ,&s2);
  tree->SetBranchAddress("s."  ,&s );

  if(fillraw) {
    // **** fill tree with raw segments ****
    EdbPattern *pat=0;
    int nic;
    int nip=al->Npatterns();
    for( int ip=0; ip<nip; ip++ ) {
      pat  = al->GetPattern(ip);
      pid1 = pat->ID();
      pid2 = -1;
      nic=pat->N();
      for( int ic=0; ic<nic; ic++ ) {
	s1 = pat->GetSegment(ic);
	tree->Fill();
      }
    }
  }

  // **** fill tree with found couples ****
  s = new EdbSegP();

  int nip=al->Ncouples();
  for( int ip=0; ip<nip; ip++ ) {
    patc = al->GetCouple(ip);
    pid1 = patc->Pat1()->ID();
    pid2 = patc->Pat2()->ID();

    int nic=patc->Ncouples();
    for( int ic=0; ic<nic; ic++ ) {
      //      printf("%d %d\n",ip,ic);
      cp = patc->GetSegCouple(ic);
      s1 = patc->Pat1()->GetSegment(cp->ID1());
      s2 = patc->Pat2()->GetSegment(cp->ID2());
      //s = new EdbSegP(*s1);
      //*s += *s2;
        s->Set( ic, s1->X(), s1->Y(),
      	      (s1->X()-s2->X())/(s1->Z()-s2->Z()),
      	      (s1->Y()-s2->Y())/(s1->Z()-s2->Z()),
      	      s1->W()+s2->W());
        s->SetZ( s1->Z() );

//        s->Set( ic, (s1->X()+s2->X())/2., (s1->Y()+s2->Y())/2.,
//  	      (s1->TX()+s2->TX())/2., (s1->TY()+s2->TY())/2.,
//        	      s1->W()+s2->W());

//        s->SetZ( (s1->Z()+s2->Z())/2. );

      tree->Fill();
      //delete s;
    }
  }

  tree->Write();
  gDirectory->Purge();
}
