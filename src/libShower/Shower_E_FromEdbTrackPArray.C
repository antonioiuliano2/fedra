{

	// Instantate ShowerAlgorithmEnergy Class
	EdbShowerAlgESimple* ShowerAlgEnergyInstance = new EdbShowerAlgESimple();
	
	// Run Shower Energy Algorith on all shower/tracks
	ShowerAlgEnergyInstance->DoRun(RecoShowerArray);
	
	// All tracks contain now in P() the estimated energy.
}