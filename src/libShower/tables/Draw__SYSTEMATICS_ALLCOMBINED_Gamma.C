#include <iomanip.h>
Int_t PARTICLE_TYPE=1; // 0:Electron; 1:Gamma
Int_t TRAIN_TYPE=0; // 0:Type A; 1:Type ABCD

void Draw__SYSTEMATICS_ALLCOMBINED_Gamma(){
	gROOT->SetStyle("Pub");

// Just read the final combined table and plot the values....:
// be in directory /home/meisel/fedra_svn_XXX/src/libShower/tables
/*
meisel@lheppc87:\ w$ cat libShower_Energy_Systematics_Gamma.txt
10 0.206928 0.0957288 0.196085 0.385399 0.654593 1.08115 1.36482 1.60552 1.71841 2.1578 2 1.9 
12 0.312237 0.166424 0.168757 0.404877 0.73089 1.19248 1.57156 1.90927 2.30868 3.49286 2.2 2.3 
14 0.244039 0.243795 0.136029 0.561688 0.536846 0.672012 1.3025 1.35377 1.90329 3.9281 2.1 2.1 
16 0.28382 0.165245 0.142756 0.257738 0.487192 0.891316 1.27236 1.79092 2.03639 3.12636 2.2 1.8 
18 0.165741 0.16371 0.122923 0.225564 0.406694 0.78325 1.19181 1.72876 1.9683 3.0587 2.3 1.8 
20 0.255296 0.127867 0.137423 0.209743 0.359061 0.707776 1.06009 1.6221 1.96219 2.92038 2.4 2.3 
23 0.235603 0.144807 0.115581 0.407887 0.295895 0.477803 0.862951 0.991665 1.32228 3.57386 2.3 2 
26 0.265575 0.154625 0.144062 0.204362 0.327063 0.57382 0.903313 1.50788 1.96931 2.64586 2 1.4 
29 0.267406 0.165532 0.141513 0.190047 0.313008 0.553513 0.852301 1.39531 1.93968 2.58352 2 1.1 
32 0.264401 0.142415 0.137946 0.176253 0.328024 0.534601 0.845885 1.39317 1.86965 2.46816 2.2 1.5 
35 0.255237 0.160353 0.13366 0.187088 0.318744 0.51323 0.825485 1.37245 1.86386 2.37011 2.1 1.4 
40 0.255272 0.150665 0.125976 0.172561 0.30543 0.502208 0.810659 1.29368 1.86042 2.32437 1.9 1.6 
45 0.2436 0.153408 0.134826 0.193853 0.310406 0.530351 0.808473 1.29992 1.86067 2.32534 1.8 0.71 
*/

// // // // // // // // // // // // // // // // // // // 

Int_t dummy;
Int_t plateArray[13]={10,12,14,16,18,20,23,26,29,32,35,40,45};
Float_t dummyf;

// // // // // // // // // // // // // // // // // // // 
Double_t	E[12]  = {0.5,1.0,2.0, 4.0, 0.75, 1.5, 3.0, 6.0, 8.0, 16.0, 32.0 ,64.0};
Int_t   	E_ASCEND[12]  = {0,4,1,5,2,6,3,7,8,9,10,11};
Int_t 		NPL[13]= {10,12,14,16,18,20,23,26,29,32,35,40,45};
// 
Int_t		N_E=12;   N_E=12;
Int_t		N_NPL=13; N_NPL=13;
// // // // // // // // // // // // // // // // // // // 
TGraph* graph_MeanEnergy__vs__Energy[13]; //N_NPL
TGraph* graph_MeanEnergy__vs__Npl[12]; //N_E
TGraph* graph_RMSEnergy__vs__Energy[13]; //N_NPL
TGraph* graph_RMSEnergy__vs__Npl[12]; //N_E

TGraph* graph_MeanEnergyOverE__vs__Energy[13]; //N_NPL
TGraph* graph_MeanEnergyOverE__vs__Npl[12]; //N_E
TGraph* graph_RMSEnergyOverE__vs__Energy[13]; //N_NPL
TGraph* graph_RMSEnergyOverE__vs__Npl[12]; //N_E

TGraph* graph_MeanEnergyRelativeOverE__vs__Energy[13]; //N_NPL
TGraph* graph_MeanEnergyRelativeOverE__vs__Npl[12]; //N_E
TGraph* graph_RMSEnergyRelativeOverE__vs__Energy[13]; //N_NPL
TGraph* graph_RMSEnergyRelativeOverE__vs__Npl[12]; //N_E

TGraph* graph_MeanEnergyMinusNominalE__vs__Energy[13]; //N_NPL
TGraph* graph_MeanEnergyMinusNominalE__vs__Npl[12]; //N_E

for (int loop_E=0; loop_E<N_E; loop_E++) {
	graph_MeanEnergy__vs__Npl[loop_E] = new TGraph(N_NPL); // THIS NUMERB SHOULD BE EQUI TO N_E
	graph_RMSEnergy__vs__Npl[loop_E] = new TGraph(N_NPL);
	graph_MeanEnergyOverE__vs__Npl[loop_E] = new TGraph(N_NPL);
	graph_RMSEnergyOverE__vs__Npl[loop_E] = new TGraph(N_NPL);
	graph_MeanEnergyRelativeOverE__vs__Npl[loop_E] = new TGraph(N_NPL);
	graph_RMSEnergyRelativeOverE__vs__Npl[loop_E] = new TGraph(N_NPL);
	graph_MeanEnergyMinusNominalE__vs__Npl[loop_E] = new TGraph(N_NPL);
}
for (int loop_NPL=0; loop_NPL<N_NPL; loop_NPL++) {
	graph_MeanEnergy__vs__Energy[loop_NPL] = new TGraph(N_E);
	graph_RMSEnergy__vs__Energy[loop_NPL] = new TGraph(N_E);
	graph_MeanEnergyOverE__vs__Energy[loop_NPL] = new TGraph(N_E);
	graph_RMSEnergyOverE__vs__Energy[loop_NPL] = new TGraph(N_E);
	graph_MeanEnergyRelativeOverE__vs__Energy[loop_NPL] = new TGraph(N_E);
	graph_RMSEnergyRelativeOverE__vs__Energy[loop_NPL] = new TGraph(N_E);
	graph_MeanEnergyMinusNominalE__vs__Energy[loop_NPL] = new TGraph(N_E);
}
// Set Titles, Markers, Colors.... for the Graphs...
for (int loop_NPL=0; loop_NPL<N_NPL; loop_NPL++) {
		graph_MeanEnergy__vs__Energy[loop_NPL]->SetTitle("MeanEnergy__vs__Energy");
		graph_RMSEnergy__vs__Energy[loop_NPL]->SetTitle("RMSEnergy__vs__Energy");
		
		graph_MeanEnergyOverE__vs__Energy[loop_NPL]->SetTitle("graph_MeanEnergyOverE__vs__Energy");
		graph_RMSEnergyOverE__vs__Energy[loop_NPL]->SetTitle("graph_RMSEnergyOverE__vs__Energy");
		
		graph_MeanEnergyRelativeOverE__vs__Energy[loop_NPL]->SetTitle("graph_MeanEnergyRelativeOverE__vs__Energy");
		graph_RMSEnergyRelativeOverE__vs__Energy[loop_NPL]->SetTitle("graph_RMSEnergyRelativeOverE__vs__Energy");
		
		graph_MeanEnergyMinusNominalE__vs__Energy[loop_NPL]->SetTitle("graph_MeanEnergyMinusNominalE__vs__Energy");
}




	///----------------------------------------------------------------------------------------------------
  const char* name = "libShower_Energy_Systematics_Gamma.txt";
	pFile = fopen (name,"r");
	for (int i=0;i<13;i++) {
	loop_NPL=i;
	cout << loop_NPL << "---------------------------------" << endl;
	fscanf (pFile,"%d",&dummy);
	for (int j=0;j<12;j++) { 
		fscanf (pFile," %f ", &dummyf); cout << dummyf<< endl;
		
		graph_RMSEnergy__vs__Energy[loop_NPL]->SetPoint(j,E[E_ASCEND[j]],dummyf);
		graph_RMSEnergyRelativeOverE__vs__Energy[loop_NPL]->SetPoint(j,E[E_ASCEND[j]],dummyf/E[E_ASCEND[j]]);
		
	}
	}
	fclose (pFile);
	///----------------------------------------------------------------------------------------------------



// _SYSTEMATICS_ALLCOMBINED


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

TCanvas* Canv_RMSEnergyOverE__vs__Energy = new TCanvas("Canv_RMSEnergyOverE__vs__Energy_SYSTEMATICS_ALLCOMBINED","Canv_RMSEnergyOverE__vs__Energy_SYSTEMATICS_ALLCOMBINED",1050,750);

// NPL=20
graph_RMSEnergyRelativeOverE__vs__Energy[5]->Draw("APLS");
graph_RMSEnergyRelativeOverE__vs__Energy[5]->SetTitle("Gamma"); // For the nice title...
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetXaxis()->SetTitle("Energy [GeV]");
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetYaxis()->SetTitle("#sigma (E) / E (\% /100) Syst. Result");
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetYaxis()->SetTitleOffset(1.2);
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetXaxis()->SetTitleColor(1);
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetYaxis()->SetTitleColor(1);
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetXaxis()->CenterTitle();
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetYaxis()->CenterTitle();
// graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetYaxis()->SetRangeUser(0,0.1);
graph_RMSEnergyRelativeOverE__vs__Energy[5]->GetXaxis()->SetRangeUser(0,17); // skip 32 and 64 GeV for plot.


// NPL=10
graph_RMSEnergyRelativeOverE__vs__Energy[0]->Draw("LSsame");
graph_RMSEnergyRelativeOverE__vs__Energy[0]->SetLineStyle(2);

// NPL=32
graph_RMSEnergyRelativeOverE__vs__Energy[9]->Draw("LSsame");
graph_RMSEnergyRelativeOverE__vs__Energy[9]->SetLineStyle(5);

// NPL=40
graph_RMSEnergyRelativeOverE__vs__Energy[11]->Draw("LSsame");
graph_RMSEnergyRelativeOverE__vs__Energy[11]->SetLineStyle(3);

    
TLegend* leg_selected= new TLegend(0.65,0.55,0.95,0.85,NULL,"brNDC");
leg_selected->AddEntry(graph_RMSEnergyRelativeOverE__vs__Energy[0],"NPl=10","L");
leg_selected->AddEntry(graph_RMSEnergyRelativeOverE__vs__Energy[5],"NPl=20","L");
leg_selected->AddEntry(graph_RMSEnergyRelativeOverE__vs__Energy[9],"NPl=32","L");
leg_selected->AddEntry(graph_RMSEnergyRelativeOverE__vs__Energy[11],"NPl=40","L");
leg_selected->SetFillStyle(0);
leg_selected->SetBorderSize(0);
leg_selected->Draw();

TPaveText* pt = new TPaveText(0.8,0.93,0.98,0.98,"brNDC");
if (PARTICLE_TYPE==0) pt->AddText("Gamma");
if (PARTICLE_TYPE==1) pt->AddText("Gamma");
pt->SetFillStyle(0);
pt->SetFillColor(0);
pt->SetBorderSize(0);
pt->Draw("same");


TString canvname;
if (PARTICLE_TYPE==0) canvname=TString(Form("Canv_SYSTEMATICS_ALLCOMBINED__RMSEnergyOverE__vs__Energy__ELECTRON",0));
if (PARTICLE_TYPE==1) canvname=TString(Form("Canv_SYSTEMATICS_ALLCOMBINED__RMSEnergyOverE__vs__Energy__GAMMA",0));
Canv_RMSEnergyOverE__vs__Energy->Print(canvname+".C");
Canv_RMSEnergyOverE__vs__Energy->Print(canvname+".pdf");
Canv_RMSEnergyOverE__vs__Energy->Print(canvname+".eps");
Canv_RMSEnergyOverE__vs__Energy->Print(canvname+".png");

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

TCanvas* Canv_RMSEnergy__vs__Energy = new TCanvas("Canv_RMSEnergy__vs__Energy","Canv_RMSEnergy__vs__Energy",1050,750);

// NPL=20
graph_RMSEnergy__vs__Energy[5]->Draw("APLS");
graph_RMSEnergy__vs__Energy[5]->SetTitle("Gamma"); // For the nice title...
graph_RMSEnergy__vs__Energy[5]->GetXaxis()->SetTitle("Energy [GeV]");
graph_RMSEnergy__vs__Energy[5]->GetYaxis()->SetTitle("#sigma (E) [GeV] Syst. Result");
graph_RMSEnergy__vs__Energy[5]->GetYaxis()->SetTitleOffset(1.2);
graph_RMSEnergy__vs__Energy[5]->GetXaxis()->SetTitleColor(1);
graph_RMSEnergy__vs__Energy[5]->GetYaxis()->SetTitleColor(1);
graph_RMSEnergy__vs__Energy[5]->GetXaxis()->CenterTitle();
graph_RMSEnergy__vs__Energy[5]->GetYaxis()->CenterTitle();
// graph_RMSEnergy__vs__Energy[5]->GetYaxis()->SetRangeUser(0,0.6);
graph_RMSEnergy__vs__Energy[5]->GetXaxis()->SetRangeUser(0,17); // skip 32 and 64 GeV for plot.

// NPL=10
graph_RMSEnergy__vs__Energy[0]->Draw("LSsame");
graph_RMSEnergy__vs__Energy[0]->SetLineStyle(2);

// NPL=32
graph_RMSEnergy__vs__Energy[9]->Draw("LSsame");
graph_RMSEnergy__vs__Energy[9]->SetLineStyle(5);

// NPL=40
graph_RMSEnergy__vs__Energy[11]->Draw("LSsame");
graph_RMSEnergy__vs__Energy[11]->SetLineStyle(3);

    
TLegend* leg_selected= new TLegend(0.18,0.55,0.5,0.85,NULL,"brNDC");
leg_selected->AddEntry(graph_RMSEnergy__vs__Energy[0],"NPl=10","L");
leg_selected->AddEntry(graph_RMSEnergy__vs__Energy[5],"NPl=20","L");
leg_selected->AddEntry(graph_RMSEnergy__vs__Energy[9],"NPl=32","L");
leg_selected->AddEntry(graph_RMSEnergy__vs__Energy[11],"NPl=40","L");
leg_selected->SetFillStyle(0);
leg_selected->SetBorderSize(0);
leg_selected->Draw();

pt->Draw("same");

if (PARTICLE_TYPE==0) canvname=TString(Form("Canv_SYSTEMATICS_ALLCOMBINED__RMSEnergy__vs__Energy__ELECTRON",0));
if (PARTICLE_TYPE==1) canvname=TString(Form("Canv_SYSTEMATICS_ALLCOMBINED__RMSEnergy__vs__Energy__GAMMA",0));
Canv_RMSEnergy__vs__Energy->Print(canvname+".C");
Canv_RMSEnergy__vs__Energy->Print(canvname+".pdf");
Canv_RMSEnergy__vs__Energy->Print(canvname+".eps");
Canv_RMSEnergy__vs__Energy->Print(canvname+".png");
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 




// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 
// 
// 
// 
// 			PLOT LATEX TABLE
// 
// 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 

Double_t x,y;
TString latexname;
const char* filename;
// 
// 
// 
if (PARTICLE_TYPE==0) latexname=TString(Form("table__SYSTEMATICS_ALLCOMBINED__RMSEnergyRelativeOverE__vs__Energy__ELECTRON_ALLPLATES.tex",NULL));
if (PARTICLE_TYPE==1) latexname=TString(Form("table__SYSTEMATICS_ALLCOMBINED__RMSEnergyRelativeOverE__vs__Energy__GAMMA_ALLPLATES.tex",NULL));
cout << endl << endl << latexname << endl << endl;

filename=latexname.Data();
ofstream stream(filename);
// // // // // // // // // // // // // // // // // // // // // // // // 
for (int p=0;p<80; p++) cout << "-";
cout << endl;
for (int p=0;p<80; p++) cout << "-";
cout << endl;
cout << setw(6) << setprecision(2) << " E\Npl ";
for (int p=0;p<N_E; p++) cout << setw(6) << setprecision(2) << E[E_ASCEND[p]] << "  ";
cout << endl;
for (int o=0;o<N_NPL; o++) {
cout << setw(6) << setprecision(2) << NPL[o];
for (int p=0;p<N_E; p++) {
	graph_RMSEnergyRelativeOverE__vs__Energy[o]->GetPoint(p,x,y);
	cout << setw(6) << setprecision(2) << y << "  ";
}
cout << endl;
}
for (int p=0;p<80; p++) cout << "-";
cout << endl;
for (int p=0;p<80; p++) cout << "-";
cout << endl;
// // // // // // // // // // // // // // // // // // // // // // // // 
cout << "\\begin{tabular}{c c c c c c c c c c c c c}" << endl;
cout << "\\hline" << endl;
cout << "\\hline" << endl;
cout << " Energy/NPl & ";
for (int p=0;p<N_E-1; p++) cout << setw(6) << setprecision(2) << E[E_ASCEND[p]] << " & ";
cout << setw(6) << setprecision(2) << E[E_ASCEND[N_E-1]] << "  ";
cout << "\\\\ " << endl;
cout << "\\hline" << endl;
for (int o=0;o<N_NPL; o++) {
cout << setw(6) << setprecision(2) << NPL[o] << " ";
for (int p=0;p<N_E; p++) {
	cout << " & ";
	graph_RMSEnergyRelativeOverE__vs__Energy[o]->GetPoint(p,x,y);
	cout << setw(6) << setprecision(2) << y << " ";
}
cout << " \\\\" << endl;
}
cout << "\\hline" << endl;
cout << "\\hline" << endl;
cout << "\\end{tabular}" << endl;
// // // // // // // // // // // // // // // // // // // // // // // // // // 
// // // // // // // // // // // // // // // // // // // // // // // // 
stream << "\\begin{tabular}{c c c c c c c c c c c c c}" << endl;
stream << "\\hline" << endl;
stream << "\\hline" << endl;
stream << " Energy/NPl & ";
for (int p=0;p<N_E-1; p++) stream << setw(6) << setprecision(2) << E[E_ASCEND[p]] << " & ";
stream << setw(6) << setprecision(2) << E[E_ASCEND[N_E-1]] << "  ";
stream << "\\\\ " << endl;
stream << "\\hline" << endl;
for (int o=0;o<N_NPL; o++) {
stream << setw(6) << setprecision(2) << NPL[o] << " ";
for (int p=0;p<N_E; p++) {
	stream << " & ";
	graph_RMSEnergyRelativeOverE__vs__Energy[o]->GetPoint(p,x,y);
	stream << setw(6) << setprecision(2) << y << " ";
}
stream << " \\\\" << endl;
}
stream << "\\hline" << endl;
stream << "\\hline" << endl;
stream << "\\end{tabular}" << endl;
// // // // // // // // // // // // // // // // // // // // // // // // // // 


graph_RMSEnergyRelativeOverE__vs__Energy[0]->Print();

// return;

// 
// 
if (PARTICLE_TYPE==0) latexname=TString(Form("table__SYSTEMATICS_ALLCOMBINED__RMSEnergy__vs__Energy__ELECTRON_ALLPLATES.tex",NULL));
if (PARTICLE_TYPE==1) latexname=TString(Form("table__SYSTEMATICS_ALLCOMBINED__RMSEnergy__vs__Energy__GAMMA_ALLPLATES.tex",NULL));
cout << endl << endl << latexname << endl << endl;
filename=latexname.Data();
ofstream stream(filename);
// // // // // // // // // // // // // // // // // // // // // // // // 
for (int p=0;p<80; p++) cout << "-";
cout << endl;
for (int p=0;p<80; p++) cout << "-";
cout << endl;
cout << setw(6) << setprecision(2) << " E\Npl ";
for (int p=0;p<N_E; p++) cout << setw(6) << setprecision(2) << E[E_ASCEND[p]] << "  ";
cout << endl;
for (int o=0;o<N_NPL; o++) {
cout << setw(6) << setprecision(2) << NPL[o];
for (int p=0;p<N_E; p++) {
	graph_RMSEnergy__vs__Energy[o]->GetPoint(p,x,y);
	cout << setw(6) << setprecision(2) << y << "  ";
}
cout << endl;
}
for (int p=0;p<80; p++) cout << "-";
cout << endl;
for (int p=0;p<80; p++) cout << "-";
cout << endl;
// // // // // // // // // // // // // // // // // // // // // // // // 
cout << "\\begin{tabular}{c c c c c c c c c c c c c}" << endl;
cout << "\\hline" << endl;
cout << "\\hline" << endl;
cout << " Energy/NPl & ";
for (int p=0;p<N_E-1; p++) cout << setw(6) << setprecision(2) << E[E_ASCEND[p]] << " & ";
cout << setw(6) << setprecision(2) << E[E_ASCEND[N_E-1]] << "  ";
cout << "\\\\ " << endl;
cout << "\\hline" << endl;
for (int o=0;o<N_NPL; o++) {
cout << setw(6) << setprecision(2) << NPL[o] << " ";
for (int p=0;p<N_E; p++) {
	cout << " & ";
	graph_RMSEnergy__vs__Energy[o]->GetPoint(p,x,y);
	cout << setw(6) << setprecision(2) << y << " ";
}
cout << " \\\\" << endl;
}
cout << "\\hline" << endl;
cout << "\\hline" << endl;
cout << "\\end{tabular}" << endl;
// // // // // // // // // // // // // // // // // // // // // // // // // // 
// // // // // // // // // // // // // // // // // // // // // // // // 
stream << "\\begin{tabular}{c c c c c c c c c c c c c}" << endl;
stream << "\\hline" << endl;
stream << "\\hline" << endl;
stream << " Energy/NPl & ";
for (int p=0;p<N_E-1; p++) stream << setw(6) << setprecision(2) << E[E_ASCEND[p]] << " & ";
stream << setw(6) << setprecision(2) << E[E_ASCEND[N_E-1]] << "  ";
stream << "\\\\ " << endl;
stream << "\\hline" << endl;
for (int o=0;o<N_NPL; o++) {
stream << setw(6) << setprecision(2) << NPL[o] << " ";
for (int p=0;p<N_E; p++) {
	stream << " & ";
	graph_RMSEnergy__vs__Energy[o]->GetPoint(p,x,y);
	stream << setw(6) << setprecision(2) << y << " ";
}
stream << " \\\\" << endl;
}
stream << "\\hline" << endl;
stream << "\\hline" << endl;
stream << "\\end{tabular}" << endl;
// // // // // // // // // // // // // // // // // // // // // // // // // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // 


gSystem->Exec("cp -v table__SYSTEMATICS_ALLCOMBINED__RMSEnergyRelativeOverE__vs__Energy__GAMMA_ALLPLATES.tex  /home/meisel/opus/tables/.");
gSystem->Exec("cp -v table__SYSTEMATICS_ALLCOMBINED__RMSEnergy__vs__Energy__GAMMA_ALLPLATES.tex  /home/meisel/opus/tables/.");

gSystem->Exec("cp -v Canv_SYSTEMATICS_ALLCOMBINED__*.png /home/meisel/opus/plotsHIGHRES_png/.");
gSystem->Exec("cp -v Canv_SYSTEMATICS_ALLCOMBINED__*.eps /home/meisel/opus/plotsHIGHRES_eps/.");
}