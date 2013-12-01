#ifndef ROOT_EdbScanClientPav
#define ROOT_EdbScanClientPav

#include "EdbScanClient.h"
#include "MessageManager.h"
#include "MessageReader.h"

class EdbScanClientPav:public EdbScanClientCommon
{public: 
	EdbScanClientPav():m_pathLib("OpBTPath.pth"), m_pathName("Volume_Scan"), m_predPathName("Prediction_Scan"), m_scanningStarted(false)
	{
		m_mm.CreateMessage();
	};
  virtual ~EdbScanClientPav(){};

	virtual int    UnloadPlate();//N/A
	virtual int    LoadPlate(int BRICK, int PLATE, const char *mapext, int nAttempts=1);
  virtual void   SetParameter(const char* Object, const char* Parameter, const char* Value);//convert for sysal names?
  virtual void   SetClusterThresholds(int TOP, int BOT);//- default used
  virtual void   SetOdysseyThresholds(int itop, int ibottom, int size, int TOP, int BOT);//+
  virtual int    SetFragmentSize(int X, int Y);//converted to whole region for cont.mode

  virtual int    ScanAreas(ScanType st, int id[4], EdbPattern &areas, EdbRun *run, const char* options="");
  virtual void   AsyncScanAreaS( int id1, int id2, int id3, int id4,	
						 float x1, float x2, float y1, float y2,	const char *fname);//N/A, not used ever
  virtual void   AsyncScanPreloadAreaS( int id1, int id2, int id3, int id4,	
								 float x1, float x2, float y1, float y2,	const char *fname, 
								 float x1n, float x2n, float y1n, float y2n);
  virtual bool   ScanFromPrediction(int id1, int id2, int id3, int id4, float dx, float dy);

  virtual void   AsyncStartScan();
  virtual int    AsyncWaitForScanResult();//+
	virtual void   Print();//s

	void SetPathLib(const char* lib_){ m_pathLib=lib_; };
	void SetPathName(const char* name_){ m_pathName=name_; };

	MessageManager m_mm;
	MessageReader m_mr;

		uint32 m_insize;
		std::vector<uint8> m_inbuf;
		std::string m_pathLib;
		std::string m_pathName;
		std::string m_predPathName;
		bool m_scanningStarted;

  ClassDef(EdbScanClientPav,1)  // remote scanning for PAVICOM
};


#endif /* ROOT_EdbScanClient */
