makehtml(){
	gSystem->Load("libShower");
	
	THtml h;
	h.SetOutputDir("./htmldoc/");
	h.SetInputDir("./");
	h.MakeClass("EdbShowerRec");
	h.MakeClass("EdbPVRQuality");
	h.MakeClass("EdbShowerAlg");
	h.MakeClass("EdbShowerAlgESimple");
	h.MakeClass("EdbShowerAlgIDSimple");
	h.MakeClass("EdbShowerAlg_GS");
	
	
	h.MakeIndex();
}