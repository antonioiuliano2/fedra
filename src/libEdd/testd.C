EdbDisplay*  ds;
void testd()
{
gStyle->SetPalette(1);
ds=new EdbDisplay("display-t",-6000.,6000.,-6000.,6000.,-5050,13000.);
float plate[3]={207.,1350.,207.};
ds->SetCuts(-50000,50000,-50000,50000);
  TTree *tree;
//  TFile *f = new TFile("linked_couples.root");
//  if (f)  tree = (TTree*)f->Get("couples");
  TFile *f = new TFile("linked_tracks.root");
  if (f)  tree = (TTree*)f->Get("tracks");
  ds->SetAffine(1,0,0,1,0,0);
//  ds->AddCouplesTree(tree,plate,0.,-1.,1.,-1.,1.);
  ds->SetNsegmin(5);
  ds->AddTracksTree(tree,plate,0.,-1.,1.,-1.,1.);
//  ds->AddCouplesTree(tree,plate,0.,-1.,-.01,-1.,-.01);
//  ds->AddCouplesTree(tree,plate,0.,.01,1.,-1.,-.01);
//  ds->AddCouplesTree(tree,plate,0.,-1.,-.01,-1.,-.01);
//  ds->AddCouplesTree(tree,plate,0.,.01,1.,.01,1.);
/*  TFile *f = new TFile("02_001.cp.root");
  if (f)  tree = (TTree*)f->Get("couples");
  ds->SetAffine();
    ds->AddCouplesTree(tree,plate,-300.);
  TFile *f = new TFile("03_001.cp.root");
  if (f)  tree = (TTree*)f->Get("couples");
  ds->SetAffine();
    ds->AddCouplesTree(tree,plate,-600.);
  TFile *f = new TFile("04_001.cp.root");
  if (f)  tree = (TTree*)f->Get("couples");
  ds->SetAffine();
  ds->AddCouplesTree(tree,plate,-1990.);
*/
  ds->Refresh();
}
