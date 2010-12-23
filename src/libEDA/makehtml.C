makehtml(){
	gSystem->Load("libEDA");
	
	THtml h;
	h.SetOutputDir("./htmldoc/");
	h.SetInputDir("./");
	h.MakeClass("EdbEDA");
	h.MakeClass("EdbEDAUtil");
	h.MakeClass("EdbEDACamera");
	h.MakeClass("EdbEDAIO");
	h.MakeClass("EdbEDABrickData");
	h.MakeClass("EdbEDASelection");

	h.MakeClass("EdbEDAArea");
	h.MakeClass("EdbEDAAreaSet");
	h.MakeClass("EdbEDATrackComment");
	h.MakeClass("EdbEDATrackSelection");
	h.MakeClass("EdbEDATrackSet");
	h.MakeClass("EdbEDAVertexSet");
	h.MakeClass("EdbEDAExtentionSet");
	h.MakeClass("EdbEDAListTab");
	h.MakeClass("EdbEDAMainTab");
	h.MakeClass("EdbEDATrackSetTab");
	h.MakeClass("EdbEDAVertexTab");
	h.MakeClass("EdbEDADecaySearch");
	h.MakeClass("EdbEDADecaySearchTab");
	h.MakeClass("EdbEDAOperationTab");
	h.MakeClass("EdbEDAMCTab");
	h.MakeClass("EdbEDAPlotTab");
	h.MakeClass("EdbEDAPredTab");
	h.MakeClass("EdbEDAShowerTab");



	h.MakeIndex();
}