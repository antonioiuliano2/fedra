//-- Author :  Valeri Tioukov   27/01/2011
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbCouplesTree                                                       //
//                                                                      //
// service class for the couples tree access                            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TFile.h"
#include "EdbLog.h"
#include "EdbCouplesTree.h"

ClassImp(EdbCouplesTree)

//---------------------------------------------------------------------
EdbCouplesTree::EdbCouplesTree()
{
  eS1=eS2=eS=0;
  eCP=0;
  eXv=eYv=0;
  ePid1=ePid2=0;
  //eDZ1=eDZ2=eChi2=0;
  //eFlag=0;
}

//---------------------------------------------------------------------
void EdbCouplesTree::Print()
{
  Log(1,"EdbCouplesTree::Print","s1 s2 s cp:");
  if(eS1) eS1->PrintNice();
  if(eS2) eS2->PrintNice();
  if(eS)  eS->PrintNice();
  if(eCP) eCP->Print();
}

//---------------------------------------------------------------------
bool EdbCouplesTree::InitCouplesTree(const char *name, const char *fname, Option_t *mode)
{
  Log(3,"EdbCouplesTree::InitCouplesTree","%s from file %s for %s",name,fname,mode);
  if(fname) {                                      // if fname==0 - assumed that file is already opened 
    TFile *f = new TFile(fname, mode);
    if (!f->IsOpen()) return 0;
    f->cd();
  }
  if( (strcmp(mode,"READ")==0)||(strcmp(mode,"UPDATE")==0)) {
    eTree = (TTree*)gDirectory->Get(name);
    if(!eTree) {Log(1,"EdbCouplesTree::InitCouplesTree","ERROR: can not open tree %s",name); return 0;}
    
    eTree->SetBranchAddress("pid1",  &ePid1);
    eTree->SetBranchAddress("pid2",  &ePid2);
    eTree->SetBranchAddress("xv"  ,  &eXv);
    eTree->SetBranchAddress("yv"  ,  &eYv);
    eTree->SetBranchAddress("cp"  ,  &eCP);
    eTree->SetBranchAddress("s1." ,  &eS1);
    eTree->SetBranchAddress("s2." ,  &eS2);
    eTree->SetBranchAddress("s."  ,  &eS);
  } else {
    eTree = new TTree(name, "couples tree (s1:s2:s)");
    eTree->SetMaxTreeSize(15000000000LL);   //set 15 Gb file size limit)

    eTree->Branch("pid1",  &ePid1,"pid1/I");
    eTree->Branch("pid2",  &ePid2,"pid2/I");
    eTree->Branch("xv"  ,  &eXv,"xv/F");
    eTree->Branch("yv"  ,  &eYv,"yv/F");
    eTree->Branch("cp"  ,  &eCP,32000,99);
    eTree->Branch("s1." ,  &eS1,32000,99);
    eTree->Branch("s2." ,  &eS2,32000,99);
    eTree->Branch("s."  ,  &eS,32000,99);
  }

  return 1;
}

/*
//---------------------------------------------------------------------
bool EdbCouplesTree::InitTreeNew(const char *name)
{
  // assumed that the correct file is already opened
  
  eTree = new TTree(name,"couples tree (s1:s2:s)");
  
  eTree->Branch("cp",   "EdbSegCouple",&eCP,32000,99);
  eTree->Branch("s1.",  "EdbSegP",&eS1,32000,99);
  eTree->Branch("s2.",  "EdbSegP",&eS2,32000,99);
  eTree->Branch("s." ,  "EdbSegP",&eS ,32000,99);
  eTree->Branch("dz1",  &eDZ1,"dz1/F");
  eTree->Branch("dz2",  &eDZ2,"dz2/F");
  eTree->Branch("flag", &eFlag,"flag/I");
  eTree->Branch("chi2", &eChi2,"chi2/F");

  eTree->SetAlias("x1","s1.eX+dz1*s1.eTX");
  eTree->SetAlias("x2","s2.eX+dz2*s2.eTX");
  eTree->SetAlias("y1","s1.eY+dz1*s1.eTY");
  eTree->SetAlias("y2","s2.eY+dz2*s2.eTY");
  
eTree->SetAlias( "tx1c" , Form("s1.eTX/(%f)+(%f)",ePC1.eShr,ePC1.eDTX) );
  eTree->SetAlias( "ty1c" , Form("s1.eTY/(%f)+(%f)",ePC1.eShr,ePC1.eDTY) );
  eTree->SetAlias( "tx2c" , Form("s2.eTX/(%f)+(%f)",ePC2.eShr,ePC2.eDTX) );
  eTree->SetAlias( "ty2c" , Form("s2.eTY/(%f)+(%f)",ePC2.eShr,ePC2.eDTY) );

  eTree->SetAlias("x1c" ,  Form("(%f) + s1.eX + tx1c*(%f)",ePC1.eDX,ePC1.eDZ) );
  eTree->SetAlias("y1c" ,  Form("(%f) + s1.eY + ty1c*(%f)",ePC1.eDY,ePC1.eDZ) );
  eTree->SetAlias("x2c" ,  Form("(%f) + s2.eX + tx2c*(%f)",ePC2.eDX,ePC2.eDZ) );
  eTree->SetAlias("y2c" ,  Form("(%f) + s2.eY + ty2c*(%f)",ePC2.eDY,ePC2.eDZ) );

  eTree->SetAlias("dx" , "x2c-x1c" );
  eTree->SetAlias("dy" , "y2c-y1c" );
  eTree->SetAlias("dtx" , "tx2c-tx1c" );
  eTree->SetAlias("dty" , "ty2c-ty1c" );

  eTree->SetAlias("dr" , "sqrt(dx*dx+dy*dy)" );
  eTree->SetAlias("dt" , "sqrt(dtx*dtx+dty*dty)" );
  return 1;
}
*/

//---------------------------------------------------------------------
bool EdbCouplesTree::WriteTree()
{
  if(!eTree)    return 0;
  eTree->Write();
  SafeDelete(eTree);
  return 1;
}

//---------------------------------------------------------------------
Int_t EdbCouplesTree::Fill( EdbSegP *s1, EdbSegP *s2, EdbSegP *s, EdbSegCouple *cp, float xv, float yv, int pid1, int pid2)
{
  eS1=s1;    eS2=s2;  eS=s; eCP=cp; 
  eXv=xv; eYv=yv; ePid1=pid1; ePid2=pid2;
   //eDZ1 = dz1; eDZ2 = dz2; eFlag=flag;
  return Fill();
}

//---------------------------------------------------------------------
Int_t EdbCouplesTree::Fill()
{
  return eTree->Fill();
}
