{
 // project lib path should be setted in LD_LIBRARY_PATH

  if (!TClassTable::GetDict("CMatrix")) { 
    if(gSystem->Load("libvt")==0)
      printf("libvt     \tloaded \n");
    else 
      printf("libvt do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("TIndexCell")) { 
    if(gSystem->Load("libEmath")==0)
      printf("libEmath \tloaded \n");
    else 
      printf("libEmath do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbPhysics")) { 
    if(gSystem->Load("libEphys")==0)
      printf("libEphys \tloaded \n");
    else 
      printf("libEphys do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbView")) { 
    if(gSystem->Load("libEdb")==0) 
      printf("libEdb   \tloaded \n");
    else 
      printf("libEdb do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbPattern")) { 
    if(gSystem->Load("libEdr")==0) 
      printf("libEdr   \tloaded \n");
    else 
      printf("libEdr do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbDisplay")) { 
    if(gSystem->Load("libEdd")==0) 
      printf("libEdd   \tloaded \n");
    else 
      printf("libEdd do NOT loaded!\n");
  }

  if (!TClassTable::GetDict("EdbPVGen")) { 
    if(gSystem->Load("libEMC")==0) 
      printf("libEMC   \tloaded \n");
    else 
      printf("libEMC do NOT loaded!\n");
  }

}
