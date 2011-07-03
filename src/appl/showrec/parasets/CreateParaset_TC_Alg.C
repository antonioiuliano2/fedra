{
	
	//=C= ------------------------------------------------------------------------------------------------------
	//=C= FOR SHORT AND FAST RECONSTRUCTION STUDIES: NOT CONTAINING SO MUCH PARAMETERSET (instead of 5k only 128) 
	//=C= COMPARED TO   CreateParameterSetFile.C   
	//=C= ------------------------------------------------------------------------------------------------------
	
	TFile *PARAMETERSET_DEFINITIONFILE_SHORT = new TFile("PARAMETERSET_DEFINITIONFILE_LONG_TC_ALG.root","RECREATE");

  //=C= Values valid for ShowerReco_Algorithm_6 = TC Algo: "Track (into) Cone"
  

	Double_t	CUT_SHOWERFOLLOWERBT_DR_MAX;								// Distance to the next follower BT
	Double_t	CUT_SHOWERFOLLOWERBT_DTAN_MAX;							// Angle to the next follower BT
	Double_t	CUT_ZYLINDER_R_MAX;												// Radius when cone becomes a cylinder
	Double_t	CUT_ZYLINDER_ANGLE_MAX;								// Opening Angle of the cone to collect all BTs	

//	Double_t CUT_SHOWERFOLLOWERBT_DR_MAX_ARRAY={};
//	Double_t CUT_SHOWERFOLLOWERBT_DTAN_MAX_ARRAY={};
//	Double_t CUT_ZYLINDER_R_MAX_ARRAY={};
//	Double_t CUT_ZYLINDER_ANGLE_MAX_ARRAY={};

  Double_t  CUT_TRACKATTACH_DISTMIN;                // Minimum Distance Track BT To one of the by the shower.
  Double_t  CUT_TRACKATTACH_DTAN_MAX;              // Max DTheta (single angle) Track BT to one of the by the shower.
  Int_t  CUT_TRACKATTACH_NTRACKSEG;                // N of the track BTs to try  to attach [0..nseg]
  
  
  
  

	
	ParaSet = new TTree("ParaSet_Variables","ParaSet_Variables");
	ParaSet->Branch("CUT_SHOWERFOLLOWERBT_DR_MAX",&CUT_SHOWERFOLLOWERBT_DR_MAX,"CUT_SHOWERFOLLOWERBT_DR_MAX/D");
	ParaSet->Branch("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&CUT_SHOWERFOLLOWERBT_DTAN_MAX,"CUT_SHOWERFOLLOWERBT_DTAN_MAX/D");
	ParaSet->Branch("CUT_ZYLINDER_R_MAX",&CUT_ZYLINDER_R_MAX,"CUT_ZYLINDER_R_MAX/D");
	ParaSet->Branch("CUT_ZYLINDER_ANGLE_MAX",&CUT_ZYLINDER_ANGLE_MAX,"CUT_ZYLINDER_ANGLE_MAX/D");	
	
  ParaSet -> Branch("CUT_TRACKATTACH_DISTMIN",&CUT_TRACKATTACH_DISTMIN,"CUT_TRACKATTACH_DISTMIN/D");
  ParaSet -> Branch("CUT_TRACKATTACH_DTAN_MAX",&CUT_TRACKATTACH_DTAN_MAX,"CUT_TRACKATTACH_DTAN_MAX/D");
  ParaSet -> Branch("CUT_TRACKATTACH_NTRACKSEG",&CUT_TRACKATTACH_NTRACKSEG,"CUT_TRACKATTACH_NTRACKSEG/I");
  
  
	Int_t ParaSetNr=0;
	ofstream outstream;
	TString OutputFile="PARAMETERSET_DEFINITIONFILE_LONG_TC_ALG.txt";
	outstream.open(OutputFile);
	outstream << " ParaSetNr  CUT_ZYLINDER_R_MAX CUT_ZYLINDER_ANGLE_MAX CUT_SHOWERFOLLOWERBT_DR_MAX CUT_SHOWERFOLLOWERBT_DTAN_MAX  CUT_TRACKATTACH_DISTMIN  CUT_TRACKATTACH_DTAN_MAX CUT_TRACKATTACH_NTRACKSEG "<< endl;
	
	
	//-------------------------------------------------------------
	// 
	//	Compability Mode to OI ALG: using not the linked_tracks should give same
	//	ALGORITHM as OI
	//
	CUT_TRACKATTACH_DISTMIN=0;
         CUT_TRACKATTACH_DTAN_MAX=0;
         CUT_TRACKATTACH_NTRACKSEG=0;
	    
	    
	    CUT_ZYLINDER_R_MAX=700;
            CUT_ZYLINDER_ANGLE_MAX=0.025;
            CUT_SHOWERFOLLOWERBT_DR_MAX=150;
            CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.13;
            ParaSet->Fill();
            outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_TRACKATTACH_DISTMIN << "  " << CUT_TRACKATTACH_DTAN_MAX << "  " << CUT_TRACKATTACH_NTRACKSEG << endl;
            ++ParaSetNr;
	    
	    
	    CUT_ZYLINDER_R_MAX=700;
            CUT_ZYLINDER_ANGLE_MAX=0.025;
            CUT_SHOWERFOLLOWERBT_DR_MAX=150;
            CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.11;
	    ParaSet->Fill();
            outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_TRACKATTACH_DISTMIN << "  " << CUT_TRACKATTACH_DTAN_MAX << "  " << CUT_TRACKATTACH_NTRACKSEG << endl;
            ++ParaSetNr;
	    CUT_ZYLINDER_R_MAX=900;
            CUT_ZYLINDER_ANGLE_MAX=0.04;
            CUT_SHOWERFOLLOWERBT_DR_MAX=150;
            CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.15;
	    ParaSet->Fill();
            outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_TRACKATTACH_DISTMIN << "  " << CUT_TRACKATTACH_DTAN_MAX << "  " << CUT_TRACKATTACH_NTRACKSEG << endl;
            ++ParaSetNr;
        //-------------------------------------------------------------    
	
	
	
	
	
	
	
	
// 				dr_max=120.0;dt_max=0.12;coneangle=0.020;tubedist=700.0;
	///===========================================================================

//   1213    700  0.025  150  0.13
          for (Int_t i5=0; i5 <=5; ++i5) {
            for (Int_t i6=0; i6 <=5; ++i6) {
              for (Int_t i7=0; i7 <=5; ++i7) {

						CUT_ZYLINDER_R_MAX=700;
						CUT_ZYLINDER_ANGLE_MAX=0.025;
						CUT_SHOWERFOLLOWERBT_DR_MAX=150;
						CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.13;
            
            CUT_TRACKATTACH_DISTMIN=200+i5*50;
            CUT_TRACKATTACH_DTAN_MAX=60+i6*20;
            CUT_TRACKATTACH_NTRACKSEG=0+i7;

						ParaSet->Fill();
						outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_TRACKATTACH_DISTMIN << "  " << CUT_TRACKATTACH_DTAN_MAX << "  " << CUT_TRACKATTACH_NTRACKSEG << endl;
						++ParaSetNr;
						
              }
            }
          }
      
	///===========================================================================

          
          
          
    ///===========================================================================

//   1211    700  0.025  150  0.11
          
          for (Int_t i5=0; i5 <=5; ++i5) {
            for (Int_t i6=0; i6 <=5; ++i6) {
              for (Int_t i7=0; i7 <=5; ++i7) {

            CUT_ZYLINDER_R_MAX=700;
            CUT_ZYLINDER_ANGLE_MAX=0.025;
            CUT_SHOWERFOLLOWERBT_DR_MAX=150;
            CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.11;
            
            CUT_TRACKATTACH_DISTMIN=200+i5*50;
            CUT_TRACKATTACH_DTAN_MAX=60+i6*20;
            CUT_TRACKATTACH_NTRACKSEG=0+i7;

            ParaSet->Fill();
            outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_TRACKATTACH_DISTMIN << "  " << CUT_TRACKATTACH_DTAN_MAX << "  " << CUT_TRACKATTACH_NTRACKSEG << endl;
            ++ParaSetNr;
            
              }
            }
          }
      
  ///===========================================================================       
          
           
    ///===========================================================================

//     3455    900  0.04  150  0.15        
          for (Int_t i5=0; i5 <=5; ++i5) {
            for (Int_t i6=0; i6 <=5; ++i6) {
              for (Int_t i7=0; i7 <=5; ++i7) {

            CUT_ZYLINDER_R_MAX=900;
            CUT_ZYLINDER_ANGLE_MAX=0.04;
            CUT_SHOWERFOLLOWERBT_DR_MAX=150;
            CUT_SHOWERFOLLOWERBT_DTAN_MAX=0.15;
            
            CUT_TRACKATTACH_DISTMIN=200+i5*50;
            CUT_TRACKATTACH_DTAN_MAX=60+i6*20;
            CUT_TRACKATTACH_NTRACKSEG=0+i7;

            ParaSet->Fill();
            outstream << "  " << ParaSetNr << "  " << "  " << CUT_ZYLINDER_R_MAX << "  " << CUT_ZYLINDER_ANGLE_MAX << "  " << CUT_SHOWERFOLLOWERBT_DR_MAX << "  " << CUT_SHOWERFOLLOWERBT_DTAN_MAX << "  " << CUT_TRACKATTACH_DISTMIN << "  " << CUT_TRACKATTACH_DTAN_MAX << "  " << CUT_TRACKATTACH_NTRACKSEG << endl;
            ++ParaSetNr;
            
              }
            }
          }
      
  ///===========================================================================              
          
          
          
          
          
	ParaSet->Print();
	PARAMETERSET_DEFINITIONFILE_SHORT->cd();
	ParaSet->Write();
	//PARAMETERSET_DEFINITIONFILE_SHORT->Write();
	return;
}
