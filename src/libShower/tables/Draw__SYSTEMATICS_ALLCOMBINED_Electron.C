#include <iomanip.h>
Int_t PARTICLE_TYPE=0; // 0:Electron; 1:Gamma
Int_t TRAIN_TYPE=0; // 0:Type A; 1:Type ABCD

void Draw__SYSTEMATICS_ALLCOMBINED_Electron(){
	gROOT->SetStyle("Pub");

// Just read the final combined table and plot the values....:
// be in directory /home/meisel/fedra_svn_XXX/src/libShower/tables
/*
meisel@lheppc87:\ w$ cat libShower_Energy_Systematics_Electron.txt
10 0.487087 0.268673 0.251139 0.392944 0.578185 0.938788 1.1368 1.38043 1.52797 2.1963 1.6 1.8 
12 0.613404 0.326983 0.236053 0.399411 0.743169 1.1208 1.53704 1.74958 2.14625 3.3622 1.9 2 
14 0.436089 0.255157 0.182932 0.306581 0.592368 0.998098 1.4699 1.75716 2.17982 3.35874 1.7 1.6 
16 0.352786 0.190076 0.180189 0.262015 0.481161 0.884421 1.28573 1.73485 2.02485 2.99823 1.9 1.7 
18 0.303112 0.180964 0.162963 0.212751 0.387164 0.765904 1.17881 1.71155 1.96478 3.0471 1.9 1.6 
20 0.272523 0.135122 0.154926 0.204074 0.330928 0.654695 1.06094 1.58079 1.94743 3.01188 2 1.4 
23 0.244422 0.157274 0.146185 0.18524 0.285288 0.564008 0.947304 1.49379 1.89982 2.89843 2.3 1.8 
26 0.253945 0.166703 0.160981 0.205353 0.270174 0.498645 0.854425 1.45537 1.84654 2.75398 1.8 1.1 
29 0.27287 0.162324 0.16428 0.179237 0.262875 0.456495 0.786588 1.34062 1.88284 2.67105 2 1.2 
32 0.2726 0.163444 0.159981 0.187163 0.264464 0.450378 0.771966 1.3237 1.85959 2.48608 2.2 1.8 
35 0.26658 0.149793 0.164912 0.191953 0.254888 0.442164 0.71815 1.3068 1.84922 2.40437 2.2 1.5 
40 0.261101 0.155412 0.161152 0.171272 0.249347 0.417975 0.717652 1.26137 1.78077 2.32406 2.3 1.2 
45 0.242131 0.136081 0.151822 0.174786 0.258639 0.420054 0.696882 1.23248 1.70424 2.22336 2.1 1.5
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
  const char* name = "libShower_Energy_Systematics_Electron.txt";
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
graph_RMSEnergyRelativeOverE__vs__Energy[5]->SetTitle("Electron"); // For the nice title...
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
if (PARTICLE_TYPE==0) pt->AddText("Electron");
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
graph_RMSEnergy__vs__Energy[5]->SetTitle("Electron"); // For the nice title...
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

gSystem->Exec("cp -v table__SYSTEMATICS_ALLCOMBINED__RMSEnergyRelativeOverE__vs__Energy__ELECTRON_ALLPLATES.tex  /home/meisel/opus/tables/.");
gSystem->Exec("cp -v table__SYSTEMATICS_ALLCOMBINED__RMSEnergy__vs__Energy__ELECTRON_ALLPLATES.tex  /home/meisel/opus/tables/.");


gSystem->Exec("cp -v Canv_SYSTEMATICS_ALLCOMBINED__*.png /home/meisel/opus/plotsHIGHRES_png/.");
gSystem->Exec("cp -v Canv_SYSTEMATICS_ALLCOMBINED__*.eps /home/meisel/opus/plotsHIGHRES_eps/.");
}