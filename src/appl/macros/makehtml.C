makehtml(){
 THtml h;
 h.SetProductName("FEDRA");
 h.SetOutputDir("./htmldocs");
 TString fsrc=gSystem->ExpandPathName("$(FEDRA_ROOT)/src");
 void *fdir = gSystem->OpenDirectory(fsrc);
 TString input;
 const char *sd=0;
 while( (sd=gSystem->GetDirEntry(fdir)) ) { if(!strncmp(sd,"lib",3)) {input += fsrc; input += "/"; input+=sd; input+=":";} }
 //printf("%s\n",input.Data());
 h.SetInputDir(input);
 h.SetAuthorTag("//-- Author :");
 h.MakeAll();
}
