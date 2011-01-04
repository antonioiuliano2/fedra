{

	Bool_t LINEARERRORADDION=kTRUE;
// 	Bool_t LINEARERRORADDION=kFALSE;

	Int_t plate;
	Int_t dummy;
	Int_t plateArray[13]={10,12,14,16,18,20,23,26,29,32,35,40,45};
	Float_t dummyf;
	Int_t plate_i;
	Float_t sigmasysoutput;
	Float_t sigmasysinput[12][13];
  Float_t sigmasysoutput_tot[12][13];
	
	for (int i=0;i<13;i++) {
		for (int j=0;j<12;j++) { 
			sigmasysoutput_tot[j][i]=0;
		}
	}



	///----------------------------------------------------------------------------------------------------
  const char* name = "libShower_Energy_Systematics_EFFICIENCY_Electron.txt";
	pFile = fopen (name,"r");
	for (int i=0;i<13;i++) {
	plate_i=i;
	cout << plate_i << "---------------------------------" << endl;
	fscanf (pFile,"%d",&dummy);
	for (int j=0;j<12;j++) { 
		fscanf (pFile," %f ", &dummyf); cout << dummyf<< endl; sigmasysinput[j][plate_i]=dummyf; 
				if (!LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf*dummyf;
		if (LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf;
	}
	}
	fclose (pFile);
	///----------------------------------------------------------------------------------------------------

	///----------------------------------------------------------------------------------------------------
  const char* name = "libShower_Energy_Systematics_PLATEMISMATCH_Electron.txt";
	pFile = fopen (name,"r");
	for (int i=0;i<13;i++) {
	plate_i=i;
	cout << plate_i << "---------------------------------" << endl;
	fscanf (pFile,"%d",&dummy);
	for (int j=0;j<10;j++) { 
		fscanf (pFile," %f ", &dummyf); cout << dummyf<< endl; sigmasysinput[j][plate_i]=dummyf; 
				if (!LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf*dummyf;
		if (LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf;
	}
	}
	fclose (pFile);
	///----------------------------------------------------------------------------------------------------
	///----------------------------------------------------------------------------------------------------
  const char* name = "libShower_Energy_Systematics_IDMISMATCH_Electron.txt";
	pFile = fopen (name,"r");
	for (int i=0;i<13;i++) {
	plate_i=i;
	cout << plate_i << "---------------------------------" << endl;
	fscanf (pFile,"%d",&dummy);
	for (int j=0;j<10;j++) { 
		fscanf (pFile," %f ", &dummyf); cout << dummyf<< endl; sigmasysinput[j][plate_i]=dummyf; 
				if (!LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf*dummyf;
		if (LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf;
	}
	}
	fclose (pFile);
	///----------------------------------------------------------------------------------------------------
	///----------------------------------------------------------------------------------------------------
  const char* name = "libShower_Energy_Systematics_ANNMETHOD_Electron.txt";
	pFile = fopen (name,"r");
	for (int i=0;i<13;i++) {
	plate_i=i;
	cout << plate_i << "---------------------------------" << endl;
	fscanf (pFile,"%d",&dummy);
	for (int j=0;j<10;j++) { 
		fscanf (pFile," %f ", &dummyf); cout << dummyf<< endl; sigmasysinput[j][plate_i]=dummyf; 
				if (!LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf*dummyf;
		if (LINEARERRORADDION) sigmasysoutput_tot[j][plate_i]+=dummyf;
	}
	}
	fclose (pFile);
	///----------------------------------------------------------------------------------------------------

	

	

	
	for (int i=0;i<13;i++) {
		cout << plateArray[i] << " ";
		for (int j=0;j<12;j++) { 
			if (!LINEARERRORADDION) dummyf=TMath::Sqrt(sigmasysoutput_tot[j][i]);
			if (LINEARERRORADDION) dummyf=sigmasysoutput_tot[j][i];
			sigmasysoutput_tot[j][i]=dummyf;
			cout << sigmasysoutput_tot[j][i] <<" ";
		}
		cout << endl;
	}
}