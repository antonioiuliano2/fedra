//-- Author :  Valeri Tioukov   18/12/2011
// read tracks tree, analyse it and produce the report

#include <string.h>
#include <iostream>
#include <TROOT.h>
#include <TCanvas.h>
#include <TEnv.h>
#include <TMath.h>
#include <TRint.h>
#include "EdbLog.h"
#include "EdbScanProc.h"
#include "EdbAlignmentV.h"
#include "EdbCorrectionMapper.h"

using namespace std;
using namespace TMath;

bool MakeCorrectionMap(EdbPVRec &ali, TEnv &cenv);

//----------------------------------------------------------------------------------------
void print_help_message()
{
  cout<< "\nUsage: \n\t  trackan -set=ID [-corraff -divide=NXxNY -o=DATA_DIRECTORY -v=DEBUG] \n";
  cout<< "\t  trackan -file=File [-o=DATA_DIRECTORY -v=DEBUG] \n\n";
  cout<< "\t\t -corraff -  apply corrections to the aff-files\n";
  cout<<endl;
}

//----------------------------------------------------------------------------------------
void set_default(TEnv &cenv)
{
  // default parameters for the new linking
  
  cenv.SetValue("trackan.outdir"          , "..");
  cenv.SetValue("trackan.NCPmin"          , "50");
  cenv.SetValue("trackan.EdbDebugLevel"   , 1);
}

bool      do_set      = false;
bool      do_file     = false;
bool      do_corraff  = false;
EdbID idset;
EdbScanProc sproc;

//----------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
  if (argc < 2)   { print_help_message();  return 0; }
  
  TEnv cenv("trackanenv");
  set_default(cenv);
  gEDBDEBUGLEVEL        = cenv.GetValue("trackan.EdbDebugLevel" , 1);
  const char *outdir    = cenv.GetValue("trackan.outdir"        , "..");

  const char *name=0;
  
  for(int i=1; i<argc; i++ ) {
    char *key  = argv[i];
    if(!strncmp(key,"-set=",5))
     {
	if(strlen(key)>5)	if(idset.Set(key+5))   do_set=true;
     }
    if(!strncmp(key,"-file=",6))
     {
	if(strlen(key)>6) { name=(key+6);   do_file=true;}
     }
    if(!strncmp(key,"-divide=",8))
     {
	if(strlen(key)>8) {
	  int nx=0,ny=0; 
	  if(2 == sscanf(key+8,"%dx%d",&nx,&ny) ) {
	    cenv.SetValue("trackan.NX" , nx);
	    cenv.SetValue("trackan.NY" , ny);
	  }
	}
     }
    if(!strncmp(key,"-corraff",8))
     {
	do_corraff=true;
     }
    else if(!strncmp(key,"-v=",3))
      {
	if(strlen(key)>3)	gEDBDEBUGLEVEL = atoi(key+3);
      }
  }
  
  sproc.eProcDirClient=outdir;

  EdbPVRec ali;
  if(do_set)       sproc.ReadTracksTree( idset,ali,"1" );
  else if(do_file) sproc.ReadTracksTree( name ,ali,"1" );
  ali.Print();
  
  MakeCorrectionMap(ali,cenv);
  
  return 1;
}

//------------------------------------------------------------------------------
bool MakeCorrectionMap(EdbPVRec &ali, TEnv &cenv)
{
  float xmin = ali.Xmin();
  float xmax = ali.Xmax();
  float ymin = ali.Ymin();
  float ymax = ali.Ymax();
  
  EdbPattern *patMin = ali.GetPatternZLowestHighest(1);   // get pattern with lowest z
  EdbPattern *patMax = ali.GetPatternZLowestHighest(0);   // get pattern with highest z
  
  bool direction = 0;    // 1-increaseZ, 2-decreaseZ
  int idstart,idend, step;
  if(direction) {
    idstart = patMin->ID();
    idend   = patMax->ID();
  }
  else {
    idstart = patMax->ID();
    idend   = patMin->ID(); 
  }
  step    = idstart<idend?1:-1;
  
  int ref_pl =  ali.GetPattern(idend)->ScanID().ePlate;

  int npat = ali.Npatterns();
  if(npat<2)  return 0; 
  if(Abs(idend-idstart)+1 != npat) return 0;
  int idmap[npat]; int j=idstart;
  for(int i=0; i<npat; i++) { 
    idmap[i]=j; j+=step; 
  }
  
  EdbScanSet *ss =0;
  if(do_set) ss=sproc.ReadScanSet(idset);

  int NX = cenv.GetValue("trackan.NX" , 0);
  int NY = cenv.GetValue("trackan.NY" , 0);
  
  int NCPMIN = cenv.GetValue("trackan.NCPmin" , 50);

  EdbCorrectionMapper cmtmp[npat-1];
  EdbCorrectionMapper cm[npat-1];

  for(int i=0; i<npat-1; i++) {
    EdbPattern *p1 = ali.GetPattern(idmap[i]);
    EdbPattern *p2 = ali.GetPattern(idmap[i+1]);
    cm[i].eID1 = p1->ScanID();
    cm[i].eID2 = p2->ScanID();
    cm[i].eZ1  = p1->Z();
    cm[i].eZ2  = p2->Z();
    cm[i].eNcpMin = NCPMIN;
    
    if(NX*NY) cm[i].InitMap( NX,xmin, xmax, NY, ymin,ymax);
    if(NX*NY) cmtmp[i].InitMap( NX,xmin, xmax, NY, ymin,ymax);
    
    cm[i].eHdty_ty.InitH2( 50,-2,2, 50,-0.1,0.1 );
    cm[i].eHshr.InitH1( 100, 0.8, 1.2 );

    cm[i].eHdxy.InitH2( 61,-60.5,60.5, 61,-60.5,60.5 );
    cm[i].eHdtxy.InitH2( 41,-0.105,0.105, 41,-0.105,0.105 );
    cm[i].eHdz.InitH1( 50,-100,100 );
    
    cm[i].eHxy1.InitH2(100,xmin, xmax, 100, ymin,ymax);
    cm[i].eHtxty1.InitH2(100,-2, 2, 100, -2,2);
    cm[i].eHxy2.InitH2(100,xmin, xmax, 100, ymin,ymax);
    cm[i].eHtxty2.InitH2(100,-2, 2, 100, -2,2);
  }


  int ntr  = ali.Ntracks();

  for(int i=0; i<ntr; i++) {
    EdbTrackP *t = ali.GetTrack(i);
    int nseg = t->N();
    if(nseg<2) continue;
    
    for(int j=0; j<nseg-1; j++) {
      EdbSegP *s1,*s2;
      if(direction) {
        s1 = t->GetSegment(j);
        s2 = t->GetSegment(j+1);
      }
      else {
        s2 = t->GetSegment(j);
        s1 = t->GetSegment(j+1);
      }
      if( Abs(s2->PID()-s1->PID())!=1 )  continue;

      cm[idmap[s1->PID()]].Fill(*s1,*s2);                // fill general hists
      cmtmp[idmap[s1->PID()]].AddSegCouple(s1,s2);
    }
  }
  
  //EnrichPoorBins:
  for(int i=0; i<npat-1; i++) {
    int nbin = cmtmp[i].eMapAl.Ncell();
    for(int j=0; j<nbin; j++) {
      EdbCorrectionBin *bin  = cm[i].GetBin(j);
      EdbCorrectionBin *bint = cmtmp[i].GetBin(j);

      cm[i].AddBin(*bint); bin->AddBin(*bint);             // TODO enrichment algorithm is disabled now
/*      if(bint->eAl.Ncp() >= NCPMIN)      { cm[i].AddBin(*bint); bin->AddBin(*bint); }
      else      {
        cm[i].AddBin(*bint); 
        TObjArray arr;
        int ne = cmtmp[i].eMapAl.SelectObjectsCJ(j,1,arr);
        for(int ie=0; ie<ne; ie++) {
          EdbCorrectionBin *bine = (EdbCorrectionBin *)(arr.At(ie));
          bin->AddBin(*bine);
        }
      }*/
    }
    if(nbin<1) cm[i].AddBin( cmtmp[i] );
  }

  for(int i=0; i<npat-1; i++) {
    cm[i].SetCorrDZ();
    cm[i].CalculateCorrections();
    cm[i].MakeCorrectionsTable();
  }
  

  TString name;
  sproc.MakeFileName(name,idset,"trk.an.root",false);
  Log(2,"MakeCorrectionMap","%s",name.Data());
  TFile f(name,"RECREATE");
  
  if(ss) {
    for(int i=0; i<npat-1; i++) {
	ss->eReferencePlate=ref_pl;
	
	if(NX*NY) cm[i].UpdateLayerWithLocalCorr( cm[i].eLayer );
        ss->UpdateBrickWithP2P( cm[i].eLayer, cm[i].eID1.ePlate, cm[i].eID2.ePlate );
        
        gROOT->SetBatch();
        TCanvas *csum = cm[i].DrawSum(Form("%d",i));
        csum->Write();
        TCanvas *cdiff = cm[i].DrawMap(cm[i].eMap, Form("diff_%d",i));
        cdiff->Write();
        TCanvas *cabs = cm[i].DrawMap( ss->GetPlate(cm[i].eID1.ePlate)->Map(), Form("%d",i));
        cabs->Write();

    }
  }
  f.Close();

  if(do_corraff) if(ss) sproc.WriteScanSet(idset,*ss);

  return 1;
}
