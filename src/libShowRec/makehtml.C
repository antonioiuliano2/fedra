makehtml(){
	gSystem->Load("libShowRec");
	
	THtml h;
	h.SetOutputDir("./htmldoc/");
	h.SetInputDir("./");
	h.MakeClass("EdbShowRec");
	h.MakeClass("EdbShowerP");
	h.MakeClass("EdbShowPVRQuality");
	
// 	h.MakeClass("EdbShowerAlg");
// 	h.MakeClass("EdbShowerAlgESimple");
// 	h.MakeClass("EdbShowerAlgIDSimple");
// 	h.MakeClass("EdbShowerAlg_GS");
	
	
	h.MakeIndex();
}