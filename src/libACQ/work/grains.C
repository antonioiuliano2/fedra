#include "init.C"
#include "run.C"
grains()
{
 float X0,Y0, DDX=500, DDY=500;
 char line[256];
 float Zmid;
	int res;
	hwinit();
    homeall();
    X0=s->X->Reference+125000/2.;
    Y0=s->Y->Reference+100000/2.;
    Zmid=s->Z->Reference+s->EmBotLayer+s->EmBase/2.;
	s->Z->PosMove(Zmid,1e6,1e6);
	s->Z->WaitForMoveComplete(1e4);
	xy(X0,Y0);
//	printf("Please set the focus to the middle of the base, then type 'q'..\n");
//	monitor();
    tunelight();
	takebggchere(); //take background and equalization map
	/// Configure Odyssey for taking image with processing

	o->ApplyGainCorr=true;
//	o->ApplyGainCorr=false;
	o->ApplyFIRF=true;
	o->Threshold=ThrTop;
	o->SubtractBG=true;
 if(!finefocus(400,.4)) { return 0;};
 if(!finefocus(400,2)) {return 0;};
    Zmid=s->Z->Reference+s->EmBotLayer+s->EmBase/2.;
	s->Z->PosMove(Zmid,1e6,1e6);
	s->Z->WaitForMoveComplete(1e4);
 tunelight();
	takebggchere(); //take background and equalization map
	/// Configure Odyssey for taking image with processing
	o->ApplyGainCorr=true;
//	o->ApplyGainCorr=false;
	o->ApplyFIRF=true;
	o->Threshold=ThrTop;
	o->SubtractBG=true;
 if(!finefocus(400,2)) {return 0;};
 printf("************************* Tuning complete. taking data **************************\n");
 date=new TDatime();
 int id0,id1,id2,id3;
 if(!PC->GetLoadedId(&id0,&id1,&id2,&id3)) {printf("No plate is loaded! Load the plate and re-start the application.\n"); return 0;};
// sprintf(line,"dat%dem%d.%d.%d.%d",date->GetDate(),id0,id1,id2,id3);
 sprintf(line,"z:\\Qcheck\\dat%dem%d.%d.%d.%d",date->GetDate(),id0,id1,id2,id3);
 gSystem->mkdir(line);
// sprintf(line,"cp dat.root z:\\Qcheck\\dat%dem%d.%d.%d.%d\\",date->GetDate(),id0,id1,id2,id3);
 gSystem->cd(line);
 Run(false, X0, Y0, DDX, DDY, "dat.root", false);
 Run(false, X0-20000, Y0-20000, DDX, DDY, "dat1.root", false);
 Run(false, X0-20000, Y0+20000, DDX, DDY, "dat2.root", false);
 Run(false, X0+20000, Y0-20000, DDX, DDY, "dat3.root", false);
Run(false, X0+20000, Y0+20000, DDX, DDY, "dat4.root", false);
 printf("************************* Analysing data **************************\n");

// printf("making GA object in %s\n",gSystem->pwd());
 //EdbGA ga("dat.root",6,6,1);
// ga.SetRun("dat.root");
/* printf("making GA analysis\n");
 ga.CheckViewGrains();
 c->cd(1);
 grains->Draw("ncl","ncl<48","");
 c->cd(2);
 grains->Draw("z0","ncl<48","");
 c->SaveAs("grains.gif");
*/
 file=new TFile("dat.root","READ");
 TTree* Views=(TTree*)file->Get("Views"); 
 c=new TCanvas();
 c->Divide(2,1);
 c->cd(1);
 Views->Draw("clusters.eFrame>>ht","eSide==1"); //TOP
 ht->Scale(1./Views->GetEntries());
 ht->Draw();
 c->cd(2);
 Views->Draw("clusters.eFrame>>ht1","eSide==0"); //BOT
 ht1->Scale(1./Views->GetEntries());
 ht1->Draw();
 c->SaveAs("clust.gif");

 file=new TFile("dat1.root","READ");
 TTree* Views=(TTree*)file->Get("Views"); 
 c=new TCanvas();
 c->Divide(2,1);
 c->cd(1);
 Views->Draw("clusters.eFrame>>ht","eSide==1"); //TOP
 ht->Scale(1./Views->GetEntries());
 ht->Draw();
 c->cd(2);
 Views->Draw("clusters.eFrame>>ht1","eSide==0"); //BOT
 ht1->Scale(1./Views->GetEntries());
 ht1->Draw();
 c->SaveAs("clust1.gif");

 file=new TFile("dat2.root","READ");
 TTree* Views=(TTree*)file->Get("Views"); 
 c=new TCanvas();
 c->Divide(2,1);
 c->cd(1);
 Views->Draw("clusters.eFrame>>ht","eSide==1"); //TOP
 ht->Scale(1./Views->GetEntries());
 ht->Draw();
 c->cd(2);
 Views->Draw("clusters.eFrame>>ht1","eSide==0"); //BOT
 ht1->Scale(1./Views->GetEntries());
 ht1->Draw();
 c->SaveAs("clust2.gif");

 file=new TFile("dat3.root","READ");
 TTree* Views=(TTree*)file->Get("Views"); 
 c=new TCanvas();
 c->Divide(2,1);
 c->cd(1);
 Views->Draw("clusters.eFrame>>ht","eSide==1"); //TOP
 ht->Scale(1./Views->GetEntries());
 ht->Draw();
 c->cd(2);
 Views->Draw("clusters.eFrame>>ht1","eSide==0"); //BOT
 ht1->Scale(1./Views->GetEntries());
 ht1->Draw();
 c->SaveAs("clust3.gif");
 
 file=new TFile("dat4.root","READ");
 TTree* Views=(TTree*)file->Get("Views"); 
 c=new TCanvas();
 c->Divide(2,1);
 c->cd(1);
 Views->Draw("clusters.eFrame>>ht","eSide==1"); //TOP
 ht->Scale(1./Views->GetEntries());
 ht->Draw();
 c->cd(2);
 Views->Draw("clusters.eFrame>>ht1","eSide==0"); //BOT
 ht1->Scale(1./Views->GetEntries());
 ht1->Draw();
 c->SaveAs("clust4.gif");
 
 return 1;
}