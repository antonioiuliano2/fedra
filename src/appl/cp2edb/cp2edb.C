// cp2edb (GS - BO Dec 2004)
//--------------------------
//
//EdbSegP::eVid[0] - view entry in views tree (as before)
//EdbSegP::eVid[1] - segment entry in view (as before)
//
//EdbSegP::eAid[0] - areaID (== fragment)
//EdbSegP::eAid[1] - viewID in the area

//#include "EdbTools.h"
#include "EdbCluster.h"
#include "EdbFrame.h"
#include "EdbView.h"
#include "EdbSegment.h"
#include "EdbDataSet.h"
#include "TEventList.h"
#include "Riostream.h"
#include "stdlib.h"
#include "TCut.h"
using namespace std;

Int_t cp2edb(const char* datasetname, Int_t pieceID,const char* outfname="edb_from_cp.root", const char* options="")
{
  Bool_t addcl(false), addtr(false);
  // SCAN OPTIONS
  if (strstr(options,"C") ) addcl=true;
  if (strstr(options,"T") ) addtr=true;

  // OPEN THE DATASET
  EdbDataSet dataset(datasetname);
  Int_t nplates = dataset.N();
  cout << "The dataset has " << nplates << " plates" << endl;

  // OPEN COUPLES FILE
  EdbDataPiece       *piece = dataset.GetPiece(pieceID);
  TTree              *couples = piece->InitCouplesTree(piece->GetNameCP(), "READ");
  if(piece->Nruns() != 1) {    cout << "Raw data not found" << endl;    return 0;  }
  EdbSegCouple *cp=0;
  EdbSegP      *s=0;
  EdbSegP      *s1=0;
  EdbSegP      *s2=0;
  couples->SetBranchAddress("cp" ,&cp );		// not "cp." but "cp"
  couples->SetBranchAddress("s."  ,&s  );
  couples->SetBranchAddress("s1." ,&s1 );
  couples->SetBranchAddress("s2." ,&s2 );

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
  cout << "Start Filling... (# = 100 views)" << endl;
  cout<<"Add clusters "; if(addcl) cout<<"enabled\n"; else cout<<"disabled\n";
  Int_t nviews= run1->GetEntries();
  char str[128];
  TCut cut;
  EdbSegment* rawseg; 
  EdbSegP* segp;

  cout <<endl;
 // for(Int_t i=0; i<1000; i++) 
 // for(Int_t i=53; i<54; i++) 
  for(Int_t i=0; i<nviews; i++) 
  {
	if(i%100==0) cout << "#" << flush;
	//SET THE EVENT LIST (GET THE COUPLES WHICH HAVE AT LEAST ONE SEGMENT IN THE VIEW)
	sprintf(str,"s1.eVid[0]==%d || s2.eVid[0]==%d",i,i);
	cut= str ;
	couples->Draw(">>lst", cut && "eN1==1 && eN2==1");
	TEventList *lst = (TEventList*)gDirectory->GetList()->FindObject("lst");
	Int_t nsegments = (Int_t) lst->GetN();

	v1->Clear() ; 
	outv->Clear() ;

	v1 = run1->GetEntry(i,1,addcl,1,0,1);

	Int_t side= v1->GetNframesTop()? 1 :0 ;

	// copy view header
	EdbAffine2D const * aff = vh1->GetAffine();
		outvh->SetAffine(aff->A11(),aff->A12(),aff->A21(),aff->A22(),aff->B1(),aff->B2()) ;
		outvh->SetAreaID(vh1->GetAreaID());
		outvh->SetCoordXY(vh1->GetXview(), vh1->GetYview()); 
		outvh->SetCoordZ(vh1->GetZ1(),vh1->GetZ2(),	vh1->GetZ3(),vh1->GetZ4());
		outvh->SetNframes(vh1->GetNframesTop(),vh1->GetNframesBot());
		outvh->SetNsegments(nsegments);
		outvh->SetViewID(vh1->GetViewID());

	//copy view frames
	for(Int_t f=0; f< v1->GetNframes(); f++ )  {
		EdbFrame* frame = (EdbFrame *) (v1->GetFrames())->At(f) ;
		outv->AddFrame(frame->GetID(),frame->GetZ(),frame->GetNcl());
	}

	if (addcl) v1->AttachClustersToSegments() ;
	//cout <<"outv:"<< outv<<"\t "<< outv->GetHeader()->GetViewID()<<"\t "<<outv->Nsegments()<<"\t "<<outv->GetSegmentsAddr() <<"\t "<<outv->GetNframes()<< endl;
	for(Int_t j=0; j<nsegments; j++) {
		Int_t segID=lst->GetEntry(j);
		couples->GetEntry(segID);
		segp = side? s1: s2 ;
		rawseg =  v1->GetSegment(segp->Vid(1));
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
  cout <<endl;
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
  char filename[256]; sprintf(filename,"edb_from_cp.root");
  char options[256];
  bool printusage=(argc<3)?true:false;
  for (int i = 1; i < argc; i++)  {  // Skip argv[0] (program name)
	if (!strcmp(argv[i], "-f")) { // Process optional arguments
   	  if (i + 1 <= argc - 1) sprintf(filename,argv[++i]);
	  else printusage=true; 
	}
	else if (!strcmp(argv[i], "-c")) strcat(options,"C") ;
	else if (!strcmp(argv[i], "-t")) strcat(options,"T") ;
	else  { // Process non-optional arguments here
	  sprintf(dataset,"%s",argv[i++]) ;
	  piece=atoi(argv[i]);
	}
  }
  if(printusage) { 
    cout << "\nusage: cp2edb <data_set.def> <piece number> [options]" << endl;
	cout << "\n options: -f filename    = set output filename (default: edb_from_cp.root)" << endl;
	cout <<   "          -c             = add clusters" << endl;
	cout <<   "          -t             = add linked tracks (NOT YET IMPLEMENTED)" << endl;
    return 0;
  }
  else return cp2edb(dataset, piece, filename, options);
}
#endif
