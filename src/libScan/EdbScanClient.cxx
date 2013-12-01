//-- Author :  Mykhailo Vladymyrov   18/07/2013
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbScanClien                                                         //
//                                                                      //
// remote scanning library main class. Works as a frontend for one of   //
// two scan clients implementations, redirecting all calls to it.       //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TSystem.h"
#include "EdbScanClient.h"
#include "EdbScanClientPav.h"
#include "EdbScanClientSysal.h"
#include "libDataConversion.h"

ClassImp(EdbScanClient)
//----------------------------------------------------------------
EdbScanClient::EdbScanClient(ScanClientType type_): eServerCreatesRootFile(true)
{
	switch(type_){
		 case scanClientSySal:
			 m_implementation = std::auto_ptr<EdbScanClientCommon> (new EdbScanClientSysal());
			 eServerCreatesRootFile = false;
			 break;

		 case scanClientPavicom:
		 case scanClientLasso:
			 m_implementation = std::auto_ptr<EdbScanClientCommon> (new EdbScanClientPav());
			 break;
	};

};

//----------------------------------------------------------------
int EdbScanClient::InitializeSocket(){
	return m_implementation->InitializeSocket();
};

//----------------------------------------------------------------
int EdbScanClient::ScanPreloadAreaS( int id1, int id2, int id3, int id4,
						float x1, float x2, float y1, float y2,	const char *fname, 
						float x1n, float x2n, float y1n, float y2n){
	return m_implementation->ScanPreloadAreaS(id1, id2, id3, id4, x1, x2, y1, y2,	fname, x1n, x2n, y1n, y2n);
};

//----------------------------------------------------------------
int EdbScanClient::ScanAreas(ScanType st, int id[4], EdbPattern &areas, EdbRun *run, const char* options){
	return m_implementation->ScanAreas(st, id, areas, run, options);
};

//----------------------------------------------------------------
int EdbScanClient::AddRWC_(EdbRun* run, char* rwcname, int bAddRWD, const char* options){
	return m_implementation->AddRWC_(run, rwcname, bAddRWD, options);
}

//----------------------------------------------------------------
Short_t EdbScanClient::ShortBrick(Int_t brick){ return m_implementation->ShortBrick(brick);};

//----------------------------------------------------------------
int EdbScanClient::ConvertAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options){
	return m_implementation->ConvertAreas(id, areas, run, options);
};

//----------------------------------------------------------------
void EdbScanClient::Print()
{
	m_implementation->Print();
}

//----------------------------------------------------------------
int EdbScanClient::UnloadPlate()
{
	return m_implementation->UnloadPlate();
}

//----------------------------------------------------------------
int EdbScanClient::LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts)
{
	return m_implementation->LoadPlate(BRICK, PLATE, mapext, nAttempts);
}

//----------------------------------------------------------------
void EdbScanClient::SetParameter(const char* Object, const char* Parameter, const char* Value)
{
	m_implementation->SetParameter(Object, Parameter, Value);
}

//----------------------------------------------------------------
void  EdbScanClient::SetClusterThresholds(int TOP, int BOT)
{
	m_implementation->SetClusterThresholds(TOP, BOT);
}

//----------------------------------------------------------------
void  EdbScanClient::SetOdysseyThresholds(int itop, int ibottom, int size, int TOP, int BOT)
{
	m_implementation->SetOdysseyThresholds(itop, ibottom, size, TOP, BOT);
}

//----------------------------------------------------------------
int EdbScanClient::SetFragmentSize(int X, int Y)
{
	return m_implementation->SetFragmentSize(X, Y);
}

//----------------------------------------------------------------
void EdbScanClient::AsyncScanAreaS(  int id1, int id2, int id3, int id4, 
								float x1, float x2, float y1, float y2, 
								const char *fname)
{
	m_implementation->AsyncScanAreaS(id1, id2, id3, id4, x1, x2, y1, y2, fname);
}

//----------------------------------------------------------------
void EdbScanClient::AsyncScanPreloadAreaS(  int id1, int id2, int id3, int id4, 
					    float x1, float x2, float y1, float y2, const char *fname,
					    float x1n, float x2n, float y1n, float y2n)
{
	
	//scanf("%d", &tmp);
	m_implementation->AsyncScanPreloadAreaS(id1, id2, id3, id4, x1, x2, y1, y2, fname, x1n, x2n, y1n, y2n);
}

//----------------------------------------------------------------
int EdbScanClient::AsyncWaitForScanResult()
{
	return m_implementation->AsyncWaitForScanResult();
}

