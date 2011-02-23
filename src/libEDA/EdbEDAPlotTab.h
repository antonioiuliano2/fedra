

#ifndef __EDA_PLOTS_H__
#define __EDA_PLOTS_H__

#include<TCanvas.h>
#include<TH1.h>
#include<TLegend.h>
#include<TProfile.h>
#include<TPaveText.h>

#include"EdbEDAUtil.h"

class EdbEDAPlotTab {
	int eEffNbins;
	double eEffTmax;
	int eEffMinSeg; // Minimum number of segment for efficiency calculation. (count except the target plate.)
	EdbMomentumEstimator eTF;
	TGNumberEntry *eMomAngleRes;
	public:
	
	double eDTReference;
	EdbEDAPlotTab():eEffNbins(7),eEffTmax(0.7), eEffMinSeg(4), eDTReference(0.07) {MakeGUI();}
	
	void SetEffNbins(int nbins, double tmax=0.7) { eEffNbins=nbins; eEffTmax = tmax;}
	void SetEffMinSeg(int nseg) { eEffMinSeg=nseg;}
	
	static TCanvas * CreateCanvas(char *plot_name){
		// --- Create an embedded canvas
		
		TCanvas *c1;
		
		if(gEve==NULL) c1 = new TCanvas();
		
		else {
			gEve->GetBrowser()->StartEmbedding(1);
			gROOT->ProcessLineFast("new TCanvas");
			c1 = (TCanvas*) gPad;
			gEve->GetBrowser()->StopEmbedding(plot_name);
		}
		
		return c1;
	}

	void MakeGUI();
	void CheckEff(EdbPVRec *pvr = NULL, TObjArray *tracks = NULL);
	void CheckAlignment(EdbPVRec *pvr = NULL);
	void CheckOverview(EdbPVRec *pvr = NULL);
	void CheckPHDAngle(EdbPVRec *pvr = NULL);
	void CheckTracks();
	void CheckSingleTrack(EdbTrackP *t);
	void CheckKinkTracks();
	TObjArray * CheckKink(EdbTrackP *);
	void MomPlot();
};


#endif // __EDA_PLOTS_H__


