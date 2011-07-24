{
EdbPVRec* ali = new EdbPVRec();
EdbDataProc* proc = new EdbDataProc("lnk.def");
proc->InitVolume(ali,0);
cout << "read_pvr.C   ::   I have read a EdbPVRec* object from basetrack cp data. " << endl;
cout << "read_pvr.C   ::   ali = " << ali << " and it has ali->Npatterns()= " << ali->Npatterns() << endl;
return;
}