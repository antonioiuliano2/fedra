void make_views_map( EdbRun *edbRun, TIndexCell &up, TIndexCell &down )
{
  int nentr = edbRun->GetEntries();
  printf("Make views entry map,  nentr = %d\n",nentr);

  Long_t v[2];
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
  printf("nentr = %d\n",nentr);

  EdbViewHeader *head=0;
  int entry;

  for(int iu=0; iu<elist->N(); iu++) {
    entry = elist->At(iu)->Value();
    view = edbRun->GetEntry(entry);


    head = view->GetHeader();

    printf(" View %d (%5.1f\% ): \t %d/%d ",
  	   entry,100.*entry/nentr,head->GetNframesTop(),head->GetNframesBot() );
    nseg=0;

    
    for(int j=0;j<view->Nsegments();j++) {

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

    for( int i=0; i<rawdb->N(); i++ ) {
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

    for( int i=0; i<rawdb->N(); i++ ) {
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
    for( int ip=0; ip<al->Npatterns(); ip++ ) {
      pat  = al->GetPattern(ip);
      pid1 = pat->ID();
      pid2 = -1;

      for( int ic=0; ic<pat->N(); ic++ ) {
	s1 = pat->GetSegment(ic);
	tree->Fill();
      }
    }
  }

  // **** fill tree with found couples ****
  s = new EdbSegP();

  for( int ip=0; ip<al->Ncouples(); ip++ ) {
    patc = al->GetCouple(ip);
    pid1 = patc->Pat1()->ID();
    pid2 = patc->Pat2()->ID();

    for( int ic=0; ic<patc->Ncouples(); ic++ ) {
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
