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
  
}

//--------------------------------------------------------------
void getDataEdb( EdbRun *edbRun,
                 EdbPVRec *pvol,
                 TIndexCell *up, TIndexCell *down, 
		 float shrU, float shrD,
                 float uoff[2], float doff[2], float plate[3] )
{
  //  float dzd = edbRun->GetHeader()->GetPlate()->GetDown() * shrD/2.;
  float base=plate[1];
  float dzd = plate[0];
  EdbPattern *patD = new EdbPattern(0.,0., dzd );
  patD->SetID(1);

  getPatternEdb(edbRun,patD,down,shrD,doff,dzd);
  pvol->AddPattern(patD);

  //  float dzu = edbRun->GetHeader()->GetPlate()->GetUp()   * shrU/2.;
  float dzu = plate[2];
  EdbPattern *patU = new EdbPattern(0.,0., -base-dzu);
  patU->SetID(2);

  getPatternEdb(edbRun,patU,up,shrU,uoff,-dzu);
  pvol->AddPattern(patU);
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


//--------------------------------------------------------------
void getDatafromSysalFile( const char *file, 
			   EdbPatternsVolume *pvol,
			   float zpat)
{
  char str[256];

  EdbPattern *pat = new EdbPattern();
  EdbSegP    *seg = new EdbSegP();

  FILE *ff = fopen( file, "r");
  int id, pts, Tid, Tpts, Bid, Bpts; // T = top B = Bottom (refered to track segments)
  float x, y, z, ax, ay, sigma, Tx, Ty, Bx, By;

  fgets(str,255,ff); //skip 1-st string
  printf("%s\n",str);

  bool cut1,cut2,cut3;

  while( 8 == fscanf(ff,"%d %d %f %f %f %f %f %f"
		     , &id, &pts, &x, &y, &z, &ax, &ay, &sigma) ) {

    fscanf(ff," %d %d %f %f %d %d %f %f",
	   &Tid, &Tpts, &Tx, &Ty,&Bid, &Bpts, &Bx, &By );

    if( TMath::Abs(ay)>.1      ) continue;
    //if( ax > 0.05 ) continue;
    //if( TMath::Abs(ax+.22) > 0.05 ) continue;

    //cut1 = (TMath::Sqrt(ax**2+ay**2)<=0.1)&&(pts<=20);
    //if(cut1) continue;
    //cut2 = (TMath::Sqrt(ax**2+ay**2)>0.1)&&(TMath::Sqrt(ax**2+ay**2)<=0.2)&&(pts<=16);
    //cut3 = (TMath::Sqrt(ax**2+ay**2)>0.2)&&(pts>20);
    //if(cut2) continue;

    //if( !(ax<peakCut && ax>-peakCut && ay<peakCut && ay>-peakCut) ) {
    //if( ax<0.&&ay<.05&&ay>-.05 ) {
      seg->Set(id, x, y, ax, ay);
      seg->SetZ(zpat);
      seg->SetW(pts);
      pat->AddSegment( *seg );
      //}
  }

  printf("%d segments read from file %s\n", pat->N(),file);
  fclose(ff);

  pat->SetZ(zpat);
  pvol->AddPattern(pat);

  pvol->SetPatternsID();
  pvol->FillTree(pat,pat,0);
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
//        s = new EdbSegP(*s1);
//        *s += *s2;
      s->Set( ic, s1->X(), s1->Y(),
	      (s1->X()-s2->X())/(s1->Z()-s2->Z()),
	      (s1->Y()-s2->Y())/(s1->Z()-s2->Z()),
	      s1->W()+s2->W());

      tree->Fill();
      //      delete s;
    }
  }

  tree->Write();
  gDirectory->Purge();
}
