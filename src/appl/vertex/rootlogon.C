{
 // project lib path should be setted in LD_LIBRARY_PATH

  if (!TClassTable::GetDict("CMatrix")) { 
    if(gSystem->Load("libvt.so")==0)
      printf("libvt.so \tloaded \n");
    else 
      printf("libvt.so do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("TIndexCell")) { 
    if(gSystem->Load("libEmath.so")==0)
      printf("libEmath.so \tloaded \n");
    else 
      printf("libEmath.so do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbView")) { 
    if(gSystem->Load("libEdb.so")==0) 
      printf("libEdb.so \tloaded \n");
    else 
      printf("libEdb.so do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbPattern")) { 
    if(gSystem->Load("libEdr.so")==0) 
      printf("libEdr.so \tloaded \n");
    else 
      printf("libEdr.so do NOT loaded!\n");
  }

}
