#include "TMultiLayerPerceptron.h"
{
  char cmd[100];
  strcpy(cmd,"cp  Shower.root Shower2.root");
  gSystem->Exec(cmd);  

  gEDBDEBUGLEVEL=2;
  ee= new EdbShowerRec();  
  ee->CalculateEnergyValues();
  
  
  //-----------------------------------------------
  TCanvas* c1=new TCanvas("c1","c1",1200,400);
  c1->Divide(3,1);
  c1->cd(1);
  TH1F* h1= ee->GetHistEnergyUnCorrectedb();
  h1->Draw();
  c1->cd(2);
  TH1F* h2= ee->GetHistEnergyCorrectedb();
  h2->Draw();
  c1->cd(3); 
  TH1F* h3= ee->GetHistEnergyNominal();
  h3->Draw();
  //-----------------------------------------------
    
  TCanvas* c2=new TCanvas();
  c2->Divide(3,2);
  c2->cd(1);
  TH1D* gh1= ee->GetHistLongProfile();
  gh1->Draw();
  c2->cd(4);
  TH1D* gh2= ee->GetHistNBaseTracks();
  gh2->Draw();
  c2->cd(2);
  TH1D* gh3= ee->GetHistdeltaR_mean();
  gh3->Draw();
  c2->cd(3);
  TH1D* gh4= ee->GetHistdeltaR_rms();
  gh4->Draw();
  c2->cd(5);
  TH1D* gh5= ee->GetHistdeltaT_mean();
  gh5->Draw();
  c2->cd(6);
  TH1D* gh6= ee->GetHistdeltaT_rms();
  gh6->Draw();
  //-----------------------------------------------  
  
  
  return;
  
  
  
  
  
  
  
  
  
  
  
  
  ss= new EdbSegP(1,12.1,45.4,0.2,0.14,32,1);
  ss->PrintNice(); 
  
  
  ee->AddInitiatorBT(ss);
  ss= new EdbSegP(1,122.1,245.4,0.264,0.24,32,1);
  
  ee->AddInitiatorBT(ss);
  ss= new EdbSegP(1,1322.1,2345.4,0.23,0.4,32,1);
  
  ee->AddInitiatorBT(ss);
  ss= new EdbSegP(1,1322.1,2345.4,0.23,0.4,32,1);
  
  ee->PrintInitiatorBTs();
  
}
