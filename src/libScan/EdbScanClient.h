#ifndef ROOT_EdbScanClient
#define ROOT_EdbScanClient

#include <memory>
#include "TSocket.h"
#include "EdbRun.h"
#include "EdbPattern.h"
#include "EdbScanClientBase.h"


class EdbScanClient:public EdbScanClientBase
{
public: 
	enum ScanClientType {scanClientSySal, scanClientPavicom, scanClientLasso};
	
	EdbScanClient(ScanClientType type_ = scanClientSySal);
  ~EdbScanClient(){};
  


	int    InitializeSocket();
	int    ScanPreloadAreaS( int id1, int id2, int id3, int id4,
							float x1, float x2, float y1, float y2,	const char *fname, 
							float x1n, float x2n, float y1n, float y2n);
	int   ScanAreas(int id[4], EdbPattern &areas, EdbRun *run, const char* options="");
	int     AddRWC_(EdbRun* run, char* rwcname, int bAddRWD=true, const char* options="");
	Short_t  ShortBrick(Int_t brick);
	int   ConvertAreas(int id[4], EdbPattern &areas, EdbRun &run, const char* options="");

	virtual int    UnloadPlate();
	virtual int    LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts=1);
  virtual void   SetParameter(const char* Object, const char* Parameter, const char* Value);

  virtual void   SetClusterThresholds(int TOP, int BOT);
  virtual void   SetOdysseyThresholds(int itop, int ibottom, int size, int TOP, int BOT);
  virtual int    SetFragmentSize(int X, int Y);
  virtual void   AsyncScanAreaS( int id1, int id2, int id3, int id4,	
						 float x1, float x2, float y1, float y2,	const char *fname);
  virtual void   AsyncScanPreloadAreaS( int id1, int id2, int id3, int id4,	
								 float x1, float x2, float y1, float y2,	const char *fname, 
								 float x1n, float x2n, float y1n, float y2n);
  virtual int    AsyncWaitForScanResult();
	virtual void   Print();

public:
	const char * GetCmd(){return m_implementation->GetCmd(); };
	void SetCmd(const char* cmd_){
		m_implementation->SetCmd(cmd_);
	};

	const char * GetMess(){return m_implementation->GetMess();};
	void SetMess(const char* mess_){
		m_implementation->SetMess(mess_);
	};

	const char * GetMess1(){return m_implementation->GetMess1(); };
	void SetMess1(const char* mess_){
		m_implementation->SetMess1(mess_);
	};

	float GetNXView(){return m_implementation->GetNXView();};
	void SetNXview(float nxview_){m_implementation->SetNXview(nxview_);};

	float GetNYView(){return m_implementation->GetNYView();};
	void SetNYview(float nyview_){m_implementation->SetNYview(nyview_);};

	float GetXstep(){return m_implementation->GetXstep();};
	void SetXstep(float xstep_){m_implementation->SetXstep(xstep_);};

	float GetYstep(){return m_implementation->GetYstep();};
	void SetYstep(float ystep_){m_implementation->SetYstep(ystep_);};

	int GetMaxAttempts(){return m_implementation->GetMaxAttempts();};
	void SetMaxAttempts(int attempts_){m_implementation->SetMaxAttempts(attempts_);};

	int GetMaxFails(){return m_implementation->GetMaxFails();};
	void SetMaxFails(int fails_){m_implementation->SetMaxFails(fails_);};

	int GetPort(){return m_implementation->GetPort();};
	void SetPort(int port_){m_implementation->SetPort(port_);};

	const char * GetServer(){return m_implementation->GetServer(); };
	void SetServer(const char* server_){
		m_implementation->SetServer(server_);
	};

	const char * GetRawDirServer(){return m_implementation->GetRawDirServer(); };
	void SetRawDirServer(const char* rawdirserver_){
		m_implementation->SetRawDirServer(rawdirserver_);
	};

	const char * GetRawDirClient(){return m_implementation->GetRawDirClient(); };
	void SetRawDirClient(const char* rawdirclient_){
		m_implementation->SetRawDirClient(rawdirclient_);
	};

	bool ServerCreatesRootFile(){return eServerCreatesRootFile;};
private:
	std::auto_ptr<EdbScanClientCommon> m_implementation;
	bool eServerCreatesRootFile;

  ClassDef(EdbScanClient,1) 
};


#endif /* ROOT_EdbScanClient */
