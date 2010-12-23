{

	if (!track) return;

	// Instantate ShowerAlgorithmEnergy Class
	EdbShowerAlgESimple* ShowerAlgEnergyInstance = new EdbShowerAlgESimple();
	
	// Run Shower Energy Algorith on all shower/tracks
	ShowerAlgEnergyInstance->DoRun(track);
	
	// The track contains now in P() the estimated energy.
	track->PrintNice();
}