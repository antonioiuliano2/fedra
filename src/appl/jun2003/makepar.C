void makepar()
{
  int piece=1;
  int pl0=9;
  int pln=17;
  float dz=-300;
  float z=0;
  char *pdir="./par";

  for(int ipl=pl0; ipl<pln; ipl++ ) {
    z = dz*(ipl-pl0);
    makepar1(ipl,piece,z,pdir);
  }
}  

void makepar1(int ipl, int piece, float z, const char *pdir )
{
  char *name[80];
  sprintf(name,"%s/%2.2d_%3.3d.par",pdir,ipl,piece);
  FILE *fp=fopen(name,"w");
  fprintf(fp,"INCLUDE default.par\n");
  fprintf(fp,"ZLAYER 0 %f %f %f\n",z,0,0);
  fclose(fp); 
}
