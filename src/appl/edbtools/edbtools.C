// edbtools (GS - BO May 2005)
//----------------------------
//
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

Int_t RemoveViews(const char* infile, const char* outfname, 
                       const char* cutstr, const char* options="")
{
  Bool_t addcl(true);
  char openoption[256];
  // SCAN OPTIONS
  if (strstr(options,"NOCL") ) addcl=false;
  
  if (strstr(options,"UPDATE") ) sprintf(openoption,"UPDATE");
  else  sprintf(openoption,"RECREATE");
  
  //  OPEN RAW DATA FILE AND CREATE A CONTAINER FOR ITS VIEWS
   EdbRun* run1 = new EdbRun(infile,"READ");
   EdbView* v1=0;
   v1=run1->GetView();
   EdbViewHeader* vh1   = v1->GetHeader();
   TTree* tree = (TTree* ) gDirectory->Get("Views");

  // OPEN OUTPUT DATA FILE AND CREATE A CONTAINER FOR ITS VIEWS
  EdbRun* outrun = new EdbRun(outfname,openoption);
  EdbView* outv=0;
  outv=outrun->GetView();
  EdbViewHeader* outvh = outv->GetHeader();

  // SET THE RUN HEADER
   EdbRunHeader* rh1 = run1->GetHeader() ;
   EdbRunHeader* outrh = outrun->GetHeader() ;
   *outrh = *rh1 ;

   //SET THE EVENT LIST (GET THE COUPLES WHICH HAVE AT LEAST ONE SEGMENT IN THE VIEW)
   TCut cut;
   cut= cutstr ;
   cut.Print();
   tree->Draw(">>lst", cut);
   TEventList *lst = (TEventList*)gDirectory->GetList()->FindObject("lst");
   Int_t nviews = (Int_t) lst->GetN();

   cout <<"nviews: "<<nviews <<endl;
   cout <<endl;
	
   // START FILLING VIEWS...
   printf("Start Filling... ... %3d%%", 0) ;

   *v1 = *outv ; // otherwise the first entry is empty !!!!!!!!!!!!!!!!!!!!!!!!!!

   EdbSegment* rawseg; 
   for(Int_t i=0; i<nviews; i++) 
   {
	   if(i%100==0) printf("\b\b\b\b%3d%%",(int)((double)i/double(nviews)*100.));
	   //v1->Clear() ; 
	   //outv->Clear() ;

	   v1 = run1->GetEntry(lst->GetEntry(i),1,addcl,1,0,1);
	   //Int_t side= v1->GetNframesTop()? 1 :0 ;

      /*
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
      */
      *v1 = *outv ;
   outrun->AddView(outv);
   }
  printf("\b\b\b\b%3d%%\n",100);  
   // outrun->Print();
   run1->Close();
  outrun->Close();
  return 0;
}


//___________________________________________________________________

#ifndef __CINT__

int main(int argc, char* argv[])
{
  char fname[9][256] ;
  char* outname;
  char cutstr[256];
  char options[256];
  sprintf(cutstr,"") ;

  int nfiles = 0;

  bool printusage=(argc<3)?true:false;
  for (int i = 1; i < argc; i++)  {  // Skip argv[0] (program name)
	if (!strcmp(argv[i], "-cutv")) {   // Process optional arguments
      if (i + 1 <= argc - 1) sprintf(cutstr,argv[++i]);
	   else printusage=true; 
   }
	else if (!strcmp(argv[i], "-nocl"))    strcat(options,"NOCL") ;
	else if (!strcmp(argv[i], "-bot"))     sprintf(cutstr,"eNframesBot") ;
	else if (!strcmp(argv[i], "-top"))     sprintf(cutstr,"eNframesTop") ;
	else  { // Process non-optional arguments here
      sprintf(fname[nfiles++],argv[i]);
	}
  }
  if(printusage) { 
     cout << endl << " usage: edbtools <file1> <file2> .... <outfile> [options] " << endl << endl;
     cout << " merge file1 file2 (up to 8) [if only one input file copy file1 to outfile]" << endl;
     cout << " options:   -top            = extract top views" << endl;
	  cout << "            -bot            = extract bottom views" << endl;
	  cout << "            -nocl           = do not add the clusters" << endl;
	  cout << "            -cutv \"cutview\" = apply the cut \"cutview\" to the views" << endl;
     cout << "                                example -cutv \"eAreaID==10\"  [extract the fragment 10]" << endl;
	  return 0;
	};

  outname = fname[nfiles-1] ;

  int n_input = nfiles-1 ;
  cout << " input file(s) : " ;
            for(int j=0;j<n_input;j++) cout<< fname[j] <<" " ;
            cout <<endl;
  cout << " output file   : " << outname << endl;
  cout << " cut views     : " << cutstr << endl;
  cout << " options       : " << options << endl;


  RemoveViews(fname[0], outname, cutstr ,options);
  strcat(options,"UPDATE") ;  
  for(int j=1;j<n_input;j++) RemoveViews(fname[j], outname, cutstr ,options);

	return 0;
}
#endif
