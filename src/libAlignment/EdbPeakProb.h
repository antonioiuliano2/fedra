#ifndef ROOT_EdbPeakProb
#define ROOT_EdbPeakProb

#include <TH2F.h>

//-------------------------------------------------------------------------------------------------
class EdbPeakProb : public TObject
{
public:
  Float_t ePeakRMSmin;    // apriori limits in RMS to accept the found peak
  Float_t ePeakRMSmax;
  Int_t   ePeakNmin;      // apriori limits in the number of entries to accept the found peak
  Int_t   ePeakNmax;
  
  Float_t ePeakRMS;        // found peak parameters
  Float_t ePeakX,ePeakY;
  Float_t eProb;           // the probability the found peak is not the BG fluctuation

  TH2F *eHD;               // input histogram to be analysed

  TH1F *eHbin;             // service spectrum histo
  TH2F *eHpeak;            // service peak histo

  Int_t eVerbosity;        // 0-no any message, 1-print, 2-plot; default=2

public:
  EdbPeakProb();
  virtual ~EdbPeakProb();

  Float_t Prob();
  Float_t Probability2D(TH2F *hd, float &xpeak, float &ypeak );

  ClassDef(EdbPeakProb,1)  // prob the peak probability (2-dim only for the moment)
};

#endif /* ROOT_EdbPeakProb */
