// rwcread.cpp

#include <iostream>
using namespace std ;

#include "libDataConversion.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout<< "usage: rwcread <input file (.rwc)> "<<endl;
        return 0;
    };

    char* rwcname = argv[1];

   IO_VS_Catalog *pCat = 0;

 	if (ReadCatalog((void**)&pCat, (char*)rwcname) != 1) return false;

    cout<<"-----------------------------------------------------------------------\n";
	 cout<<"t_Hdr Hdr\tIO_Header Type  BYTE InfoType = "	<<pCat->Hdr.Type.InfoType<<endl;
	 cout<<"\t\t\t\tWORD HeaderFormat = "							<<pCat->Hdr.Type.HeaderFormat <<endl;
	 cout<<"\t\tIdentifier ID\tDWORD Part[0] = "<<pCat->Hdr.ID.Part[0] <<endl;
	 cout<<"\t\t\t\tDWORD Part[1] = "					<<pCat->Hdr.ID.Part[1] <<endl;
	 cout<<"\t\t\t\tDWORD Part[2] = "					<<pCat->Hdr.ID.Part[2] <<endl;
	 cout<<"\t\t\t\tDWORD Part[3] = "					<<pCat->Hdr.ID.Part[3] <<endl;
	 cout<<"-----------------------------------------------------------------------\n";
	 cout<<"t_Area Area\tfloat XMin = "  <<pCat->Area.XMin <<endl;
	 cout<<"\t\tfloat XMax = "		<<pCat->Area.XMax <<endl;
	 cout<<"\t\tfloat YMin = "		<<pCat->Area.YMin <<endl;
	 cout<<"\t\tfloat YMax = "		<<pCat->Area.YMax <<endl;
	 cout<<"\t\tfloat XStep = "	<<pCat->Area.XStep <<endl;
	 cout<<"\t\tfloat YStep = "	<<pCat->Area.YStep <<endl;
	 cout<<"\t\tfloat XViews = "  <<pCat->Area.XViews <<endl;
	 cout<<"\t\tfloat YViews = "  <<pCat->Area.YViews <<endl;
	 cout<<"\t\tfloat Fragments = "  <<pCat->Area.Fragments <<endl;
	 cout<<"-----------------------------------------------------------------------\n";
	 cout<<"t_Config Config\tint CountOfConfigs = "	<<	pCat->Config.CountOfConfigs << endl;
	 cout<<"\t\tVS_Config *pConfigs   char ClassName[64] = "	<< pCat->Config.pConfigs[0].ClassName << endl;	
	 cout<<"\t\t\t\t      SySalConfig Config  char Name[64] = "	<< pCat->Config.pConfigs[0].Config.Name << endl;
	 cout<<"\t\t\t\t                          long CountOfItems = "	<< pCat->Config.pConfigs[0].Config.CountOfItems << endl;
	 cout<<"\t\t\t\t                          char *pItems = "	<< pCat->Config.pConfigs[0].Config.pItems[0] << endl;
	 cout<<"-----------------------------------------------------------------------\n";
	 cout<<"int *pFragmentIndices = "	<<	pCat->pFragmentIndices[0] << endl;
	 cout<<"-----------------------------------------------------------------------\n";
	 cout<<"unsigned char Reserved[256] = " ; for(int i=0;i<256;i++) cout << pCat->Reserved[i]; cout<<endl;
	 cout<<"-----------------------------------------------------------------------\n";



	 cout<<endl<<endl<<endl<<"List of Configs"<<endl;
	 for (int j=0;j<pCat->Config.CountOfConfigs;j++)
//	 int j=0;
	 {
		 cout<<"-------------------------------------------\npConfigs["<<j<<"]"<<endl;
		 cout<<"  ClassName = "<<pCat->Config.pConfigs[j].ClassName<<endl;
		 cout<<"  Config.Name = "<<pCat->Config.pConfigs[j].Config.Name<<endl;
		 cout<<"  Config.CountOfItems = "<<pCat->Config.pConfigs[j].Config.CountOfItems
			  <<"\n   pItems =\n";
		 for(int k=0;k<pCat->Config.pConfigs[j].Config.CountOfItems;k++) 
		 {
				for(int l=0;l<39;l++) cout <<pCat->Config.pConfigs[j].Config.pItems[k*128+l];
				cout << ":\t  "; 
				for(int l=0;l<40;l++) cout <<pCat->Config.pConfigs[j].Config.pItems[k*128+64+l];
				cout << endl;
		 }
//		 for(int k=0;k<256;k++)
//			 cout << k<< ":\t"<<(int) pCat->Config.pConfigs[j].Config.pItems[k] <<endl;

		 cout<<endl;
	 }


	 cout<<endl<<endl<<"List of Fragment Indices"<<endl;
	 for(int j=0;j<pCat->Area.Fragments;j++) cout << pCat->pFragmentIndices[j] << " " ;
	 cout<< endl<<endl;

    FreeMemory((void**)pCat);

   delete pCat ;

};
