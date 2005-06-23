//-------------------------------------------------------------------
void makepar(int piece=1, int pl0=1, int pln=57, float z0=0, float dz=1300)
{
  float z;
  char *pdir="par";

  for(int ipl=pl0; ipl<=pln; ipl++ ) {
    z = z0 + dz*(ipl-pl0);
    makepar1(ipl,piece,z,pdir);
  }
}

//-------------------------------------------------------------------
void makepar1(int ipl, int piece, float z, const char *pdir )
{
  char *name[80];
  sprintf(name,"%s/%2.2d_%3.3d.par",pdir,ipl,piece);
  FILE *fp=fopen(name,"w");
  fprintf(fp,"INCLUDE default.par\n");
  fprintf(fp,"ZLAYER 0 %f %f %f\n",z,0,0);
  fclose(fp); 
}

//-------------------------------------------------------------------
