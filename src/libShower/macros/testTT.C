{
TFile* f = new TFile("ScanVolume_Ali.root");
EdbPVRec* ali = (EdbPVRec*)f->Get("EdbPVRec");
ali->Print();

p = new EdbPVRQuality(ali);
p->Execute_ConstantBTDensityInAngularBins();
p->CreateEdbPVRec();
ali2 = p->GetEdbPVRec(1);



q = new EdbPVRQuality(ali2);

}