{
    gSystem->Load("libvt++-1-35.so");
    gROOT->ProcessLine(".L gen.C");
    gROOT->ProcessLine(".L vt_mc.C");
    gROOT->ProcessLine(".L VtHists.C");
    gROOT->ProcessLine("vt_mc_init()");
}
