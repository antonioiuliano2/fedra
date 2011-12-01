// tracks2edb (GS - BO Jun 2005)
//------------------------------
//
//tracks:
//EdbSegP::eID     - couples entry in the couples tree
//
//couples:
//EdbSegP::eVid[0] - view entry in views tree (as before)
//EdbSegP::eVid[1] - segment entry in view (as before)
//
//EdbSegP::eAid[0] - areaID (== fragment)
//EdbSegP::eAid[1] - viewID in the area

//#include "EdbTools.h"
#ifndef __CINT__
#include "EdbCluster.h"
#include "EdbFrame.h"
#include "EdbView.h"
#include "EdbSegment.h"
#include "EdbDataSet.h"
#include "TEventList.h"
#include "Riostream.h"
#include "stdlib.h"
#include "TCut.h"
#include "TH1F.h"

using namespace std;
#endif

TEventList* GetCPListFromTracks(const char* fname, Int_t pid, const char* cutstr="1",Int_t* retpiece_id=0,Int_t* retpiece_piece=0 )
{
  // OPEN TRACKS FILE
  cout << "   ... open the volume tracks file " << fname << endl;
  TFile* trfile = new TFile(fname);
  TTree* tracks = (TTree*) gDirectory->Get("tracks");
  Int_t   nseg=0;
  EdbSegP *trk=0;
  TClonesArray *seg  = new TClonesArray("EdbSegP", 60);
  tracks->SetBranchAddress("nseg", &nseg);
  tracks->SetBranchAddress("t.", &trk);
  tracks->SetBranchAddress("s",  &seg);
  cout << "   ... number of volume tracks: " << tracks->GetEntries() << endl;
   
  // SET THE TRACKS EVENTLIST
  char str[128];
  sprintf(str,"s.ePID == %d && %s",pid,cutstr);
  TCut cut;
  cut= str ;
  tracks->Draw(">>trlst",cut);
  TEventList *trlst = (TEventList*)gDirectory->GetList()->FindObject("trlst");
  Int_t ntracks = (Int_t) trlst->GetN();
   cout << "   ... number of volume tracks in the piece "<<pid <<" : " <<ntracks;
   if (cutstr[0]=='1') cout<<endl; else cout << " /\""<< cutstr <<"\"" << endl;

  // CREATE THE EVENTLIST FOR THE COUPLES TREE
  TEventList* cplst = new TEventList("cplst","cplst",ntracks);
  EdbSegP *s1=0;
  for(Int_t i=0; i<ntracks; i++) {
		Int_t trID = trlst->GetEntry(i);
		tracks->GetEntry(trID);
      for(Int_t j=0 ; j<nseg; j++)
      {
         s1 = (EdbSegP*)(seg->At(j));
         if (s1->PID() == pid) break; 
      }
      Int_t cpID = s1->Vid(1);
      // cout <<"i:"<<i<<" PID: " << s1->PID() <<" z: "<< s1->Z()<< " trID: "<< trID << " cpID:" << cpID << endl;
      cplst->Enter(cpID);
  }
  Int_t piece_id    = (int) s1->Vid(0)/1000 ; 
  Int_t piece_piece = s1->Vid(0)%(((int) s1->Vid(0)/1000)*1000) ;
  cout << "   ... plate id " << piece_id <<" "<< piece_piece <<"   z : " << s1->Z() << endl;

  if(retpiece_id)    *retpiece_id    = piece_id ;
  if(retpiece_piece) *retpiece_piece = piece_piece ;
  trfile->Close();
 return cplst;
}



//___________________________________________________________________

Int_t tracks2edb(const char* datasetname, 
                 Int_t pieceID, 
                 const char* outfname="edb_from_cp.root", 
                 const char* options="",
                 const char* cutstr_cp="1",
                 const char* tracksfname="", 
                 const char* cutstr_tr="1")
{
  Bool_t addcl(true), usevoltracks(false), writebck(false);
  // SCAN OPTIONS
  if (strstr(options,"NOCL") ) addcl=false;
  if (strstr(options,"TR") ) usevoltracks=true;
  if (strstr(options,"BCK") ) writebck=true;

  // OPEN THE DATASET
  EdbDataSet dataset(datasetname);
  Int_t nplates = dataset.N();
  cout << "The dataset has " << nplates << " plates" << endl;

  // OPEN COUPLES FILE
  cout << "Open the plate n. " << pieceID << endl;
  EdbDataPiece       *piece = dataset.GetPiece(pieceID);
  TTree              *couples = piece->InitCouplesTree(piece->GetNameCP(), "READ");
  if(piece->Nruns() != 1) {    cout << "Raw data not found" << endl;    return 0;  }
  Float_t piece_z = piece->GetLayer(0)->Z() ;
  Int_t   piece_id = piece->Plate() ; 
  Int_t   piece_piece = atoi( piece->MakeName()+3 );
  cout << " plate id "<<piece_id <<" " << piece_piece<<"  z: " << piece_z << endl;
  cout << " - Number of couples: " << couples->GetEntries() << endl;

  EdbSegCouple *cp=0;
  EdbSegP      *s=0;
  EdbSegP      *s1=0;
  EdbSegP      *s2=0;
  couples->SetBranchAddress("cp" ,&cp  );		// not "cp." but "cp"
  couples->SetBranchAddress("s."  ,&s  );
  couples->SetBranchAddress("s1." ,&s1 );
  couples->SetBranchAddress("s2." ,&s2 );
  couples->Draw(">>y");

  // GET THE COUPLES EVENTLIST FROM THE TRACKS
  if(usevoltracks)
  {
   Int_t retpiece_id, retpiece_piece;
   TEventList* cplist = GetCPListFromTracks(tracksfname, pieceID, cutstr_tr, &retpiece_id , &retpiece_piece ) ;
   if (retpiece_id != piece_id || retpiece_piece != piece_piece ) 
   {
      cout << " ERROR: the piece ID or the piece number do not corresponds...." << endl;
      return -1 ;
   }
   cout << " - Number of couples (in volume tracks): " << cplist->GetN() ;
   if (cutstr_tr[0]=='1') cout<<endl; else cout << " /\""<< cutstr_tr <<"\"" << endl;
   couples->SetEventList(cplist);
  }

  // Show the number of selected couples
  TCut cut_cp;
  cut_cp = cutstr_cp ;
  Int_t selcp = couples->Draw(">>dum", cut_cp);
  cout << " - Number of selected couples: " << selcp;
  if (cutstr_cp[0]!='1') cout << " /\""<< cutstr_cp <<"\"";
  if (usevoltracks)      cout << " (belonging to volume tracks)";
  if (cutstr_tr[0]=='1') cout << endl; else cout << " /\""<< cutstr_tr <<"\"" << endl;
   // return -1;

  //INVERT COUPLES
  const EdbAffine2D  *Aff    = piece->GetLayer(0)->GetAffineXY();
  EdbAffine2D  *InvAff = (EdbAffine2D*) Aff->Clone();
  InvAff->Invert();

  //  OPEN RAW DATA FILE AND CREATE A CONTAINER FOR ITS VIEWS
  EdbRun* run1 = new EdbRun(piece->GetRunFile(0),"READ");
  cout << run1->GetFile()->GetName() << endl;
  cout << dataset.GetPiece(pieceID)->GetNameCP() << endl;
  EdbView* v1=0;
  v1=run1->GetView();
  EdbViewHeader* vh1   = v1->GetHeader();

  // OPEN OUTPUT DATA FILE AND CREATE A CONTAINER FOR ITS VIEWS
  EdbRun* outrun = new EdbRun(outfname,"RECREATE");
  EdbView* outv=0;
  outv=outrun->GetView();
  EdbViewHeader* outvh = outv->GetHeader();

  // SET THE RUN HEADER
  EdbRunHeader* rh1 = run1->GetHeader() ;
  EdbRunHeader* outrh = outrun->GetHeader() ;
  *outrh = *rh1 ;

  // START FILLING VIEWS...
  cout<<"Add clusters "; if(addcl) cout<<"enabled\n"; else cout<<"disabled\n";  cout <<endl;
  printf("Start Filling... ... %3d%%", 0) ;
  Int_t nviews= run1->GetEntries();
  char str[128];
  TCut cut;
  EdbSegment* rawseg; 
  EdbSegP* segp;
  
  Int_t totsegments=0;
  //for(Int_t i=0; i<1; i++) 
  for(Int_t i=0; i<nviews; i++) 
  {
	if(i%100==0) printf("\b\b\b\b%3d%%",(int)((double)i/double(nviews)*100.));

  	v1->Clear() ; 
	outv->Clear() ;
	v1 = run1->GetEntry(i,1,addcl,1,0,1);
	Int_t side= v1->GetNframesTop()? 1 :0 ;

	//SET THE EVENT LIST (GET THE COUPLES WHICH HAVE AT LEAST ONE SEGMENT IN THE VIEW)
	sprintf(str,"s1.eVid[0]==%d || s2.eVid[0]==%d",i,i);
	cut= str ;
	//couples->Draw(">>lst",cut_cp && cut && "eN1==1 && eN2==1");
	couples->Draw(">>lst",cut_cp && cut );
	TEventList *lst = (TEventList*)gDirectory->GetList()->FindObject("lst");

   Long_t nlinked = lst->GetN();         //cout<<"nlinked: "<<nlinked<<endl;
   Long_t ntotal  = v1->Nsegments();     //cout<<"ntotal : "<<ntotal<<endl;
   Long_t nselected;

   // CREATE THE LIST OF LINKED SEGMENTS
   TIndexCell* listselected = new TIndexCell();;
   TIndexCell* listlinked   = new TIndexCell();
   Long_t segID;
   for(int j=0;j<nlinked;j++)  {
		couples->GetEntry(lst->GetEntry(j));
		segp = side? s1: s2 ;
      segID = segp->Vid(1); //cout <<segID<< " " ;
      //listlinked->Add(segID);
      listlinked->FindAdd(segID);    //if FindAdd is used nlinked must be re-evaluated (*)
   }
   if(listlinked) nlinked = listlinked->GetEntries(); // (*) nlinked is re-eveluated to remove the duplicated segment
   if(listlinked) listlinked->Sort(); //cout<<"view: "<<i<<" ";for(int l=0;l<nlinked;l++)cout <<" "<<listlinked->At(l)->Value();cout<<endl;
   
   // CREATE THE LIST OF SELECTED SEGMENTS
   if(writebck) {
      // COMPLEMENTARY LIST 
      Int_t iLink=0; 
      for(Int_t j=0;j<ntotal;j++) {
        // in the next line, nlinked must be evaluated first otherwise if nlinked is 0 the program crashes
        if ( nlinked && j == listlinked->At(iLink)->Value() ) { 
            if(iLink < nlinked-1) iLink++;
            continue;
         } else {
            segID = (Long_t) j ; //cout <<segID[0]<< " " ;
            listselected->Add(segID);
         }
      }
      nselected = listselected->GetEntries();
   } else {
      listselected = listlinked;
      nselected = nlinked ;
   }
   //cout <<"nselected: "<<nselected<<endl;
   //for(int l=0;l<nselected;l++)cout <<" "<<listselected->At(l)->Value();cout<<endl;
   totsegments += nselected;

	// copy view header
	EdbAffine2D const * aff = vh1->GetAffine();
		outvh->SetAffine(aff->A11(),aff->A12(),aff->A21(),aff->A22(),aff->B1(),aff->B2()) ;
		outvh->SetAreaID(vh1->GetAreaID());
		outvh->SetCoordXY(vh1->GetXview(), vh1->GetYview()); 
		outvh->SetCoordZ(vh1->GetZ1(),vh1->GetZ2(),	vh1->GetZ3(),vh1->GetZ4());
		outvh->SetNframes(vh1->GetNframesTop(),vh1->GetNframesBot());
		outvh->SetNsegments(nselected);
		outvh->SetViewID(vh1->GetViewID());

	//copy view frames
	for(Int_t f=0; f< v1->GetNframes(); f++ )  {
		EdbFrame* frame = (EdbFrame *) (v1->GetFrames())->At(f) ;
		outv->AddFrame(frame->GetID(),frame->GetZ(),frame->GetNcl());
	}

	if (addcl) v1->AttachClustersToSegments() ;
   // ADD SEGMENTS
   for(Int_t j=0; j<nselected; j++) {
		//Int_t segID=lst->GetEntry(j);
		//couples->GetEntry(segID);
		//segp = side? s1: s2 ;
      rawseg =  v1->GetSegment(listselected->At(j)->Value());  //cout <<listselected->At(j)->Value()<<"\t";
		//rawseg =  v1->GetSegment(segp->Vid(1));                  cout <<segp->Vid(1)<<" "<<rawseg<<endl;
		outv->AddSegment(rawseg);
		if (addcl) {
			EdbCluster *cl=0;
			TObjArray *clusters = rawseg->GetElements();
			if(!clusters) continue;
			int ncl = clusters->GetLast()+1;
			for(Int_t k=0; k<ncl; k++ ) {
				cl = (EdbCluster*)clusters->At(k);
				outv->AddCluster(cl) ;			
			}
		}
	}
	outrun->AddView(outv); 
 }
  printf("\b\b\b\b%3d%%\n",100);  
  cout << " - Number of selected segments: " << totsegments <<endl <<endl;
  // outrun->Print();
  outrun->Close();
  run1->Close();
  return 0;
}


//___________________________________________________________________

#ifndef __CINT__
int main(int argc, char* argv[])
{
  char dataset[256];
  Int_t piece=-1; 
  char outfilename[256];      sprintf(outfilename,"edb_from_tracks.root");
  char tracksfilename[256];   sprintf(tracksfilename,"linked_tracks.root");
  char cutstr_cp[256];           sprintf(cutstr_cp,"1");
  char cutstr_tr[256];           sprintf(cutstr_tr,"1");
  char options[256];
  bool printusage=(argc<3)?true:false;
  //bool writebck(false);
  for (int i = 1; i < argc; i++)  {  // Skip argv[0] (program name)
	if (!strcmp(argv[i], "-f")) { // Process optional arguments
   	  if (i + 1 <= argc - 1) sprintf(outfilename,argv[++i]);
	  else printusage=true; 
	}
	else if (!strcmp(argv[i], "-cutcp")) { // Process optional arguments
    if (i + 1 <= argc - 1) sprintf(cutstr_cp,argv[++i]);
	  else printusage=true; 
	}
	else if (!strcmp(argv[i], "-cuttr")) { // Process optional arguments
    if (i + 1 <= argc - 1) sprintf(cutstr_tr,argv[++i]);
	  else printusage=true; 
	}
	else if (!strcmp(argv[i], "-tr")) { // Process optional arguments
     if (i + 1 <= argc - 1) {  sprintf(tracksfilename,argv[++i]); strcat(options,"TR"); } 
	  else printusage=true; 
	}
	else if (!strcmp(argv[i], "-nocl")) strcat(options,"NOCL") ;
	else if (!strcmp(argv[i], "-bck"))  strcat(options,"BCK") ;
   else  { // Process non-optional arguments here
      if( piece == -1 ) { 
         sprintf(dataset,"%s",argv[i++]) ;
         piece=atoi(argv[i]);
      }
	}
  }
  if(printusage) { 
   cout << "\nusage: tracks2edb <data_set.def> <piece number> [options]" << endl;
	cout << "\n Load the data_set and open the <piece number> raw data file (edb format)." << endl;
	cout << " Then create a new edb raw data file only with the micro-tracks which belong to" << endl;
	cout << " the couples. Optionally it is possible to cut the couples or to select" << endl;
	cout << " couples which belongs to volume tracks (with/without cuts on the vol. tracks)." << endl;
	cout << " options: -f filename         = set output filename (def: edb_from_tracks.root)" << endl;
	cout <<   "          -nocl               = don't add clusters" << endl;
   cout <<   "          -cutcp \"cutstr\"     = cut the couples  with \"cutstr\"" << endl;
   cout <<   "                                example -cutcp \"eCHI2P>0.4*(.eW-12)\" " << endl;
   cout <<   "          -tr linked_tracks.root = only couples which belongs to vol. tracks" << endl;
   cout <<   "          -cuttr \"cutstr\"     = cut the linked tracks with \"cutstr\" " << endl;
   cout <<   "                                example -cuttr \"nseg>=3\" " << endl;
   cout <<   "          -bck                  create the complementary file" << endl;
    return 0;
  }
  else tracks2edb(dataset, piece, outfilename, options, cutstr_cp,tracksfilename,cutstr_tr); 
}
#endif

