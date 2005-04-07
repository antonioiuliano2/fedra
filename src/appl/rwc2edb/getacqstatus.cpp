//	getacqstatus.exe
//	Gabriele Sirri (4/4/05, based on a previous work of Enrico Billi)
//
//	Open the catalog file.rwc and return
//      -1 : catalog not found
//       1 : acquisition terminated
//       0 : other cases
//
//	usage: getacqstatus <file.rwc> 

#include "libDataConversion.h"
#include <io.h>

//______________________________________________________________________________
int NFragments(char* rwcname)
{
   IO_VS_Catalog* pCat = 0;
   if( ReadCatalog((void**)&pCat, (char*)rwcname) != 1) return 0; 
	int nFragments = pCat->Area.Fragments;
   FreeMemory((void**)pCat);
	delete pCat;
	return nFragments;
}
//______________________________________________________________________________
void GetRWDName(char* rwdname, char* rwcname, int idx)
{
	strcpy(rwdname,rwcname);
	strncpy(rwdname + strlen(rwdname)-1, "d", 1);
	sprintf(rwdname,"%s.%08X", rwdname, idx);
}
//______________________________________________________________________________
int find_file(char* filename)
{
   struct _finddata_t   c_file;
   long                 hFile;
   int status;
   if((hFile = _findfirst(filename, &c_file)) == -1L )  status= 0; //not found
   if(_findfirst(filename, &c_file) == -1L )  
      status= 0; //not found
	else 
      status= 1; //found
   _findclose(hFile);
   return status;
}
//______________________________________________________________________________
int main(int argc, char *argv[])
{
	if (argc != 2)
	{		
		printf("\nusage: getacqstatus <file.rwc>\n");
		printf("         open the catalog file.rwc and return\n");
		printf("         -1 : catalog not found\n");
		printf("          1 : acquisition terminated\n");
		printf("          0 : other cases\n");
      return -1 ;
	}

   #define MAXLENGTH 1024
	char rwcname[MAXLENGTH], rwdname[MAXLENGTH];
	strcpy(rwcname, argv[1]);

	int nFragments = NFragments(rwcname);
   if (nFragments==0) return -1;
	GetRWDName(rwdname, rwcname, nFragments);
   //printf("%d\n",find_file(rwdname) );
   return find_file(rwdname);
}

