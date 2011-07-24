{
if (NULL==ali) return;
TFile* file = new TFile("ScanVolume_Ali.root","RECREATE");
ali->Write();
file->Close();
return;
}

