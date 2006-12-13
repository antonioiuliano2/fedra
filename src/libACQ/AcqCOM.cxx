//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqCOM                                                      //
//                                                                      //
// 	Wraping of MFC's CSerial						//
//																		//
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "serialport.h"

#include "AcqCOM.h"
CSerialPort port;

ClassImp(AcqCOM);
//_____________________________________________________________________________ 

//_____________________________________________________________________________ 
AcqCOM::AcqCOM()
{
}
//_____________________________________________________________________________ 

AcqCOM::~AcqCOM()
{
}

  //_____________________________________________________________________________ 
int AcqCOM::Open(int prt, int baud)
{
	COMMCONFIG config;
    CSerialPort::GetDefaultConfig(prt, config);
    port.Open(prt, baud, CSerialPort::NoParity, 8, CSerialPort::OneStopBit, CSerialPort::NoFlowControl);
/*
    HANDLE hPort = port.Detach();
    port.Attach(hPort);


	DWORD dwModemStatus;
    port.GetModemStatus(dwModemStatus);

    DCB dcb;
    port.GetState(dcb);

    dcb.BaudRate = baud;
    port.SetState(dcb);    

    DWORD dwErrors;                      
    port.ClearError(dwErrors);

    port.SetBreak();
    port.ClearBreak();

    COMSTAT stat;
    port.GetStatus(stat);
*/
    COMMTIMEOUTS timeouts;
    port.GetTimeouts(timeouts);

 //   port.Setup(10000, 10000);

timeouts.ReadIntervalTimeout = 40; 
timeouts.ReadTotalTimeoutMultiplier = 10;
timeouts.ReadTotalTimeoutConstant = 100;
timeouts.WriteTotalTimeoutMultiplier = 10;
timeouts.WriteTotalTimeoutConstant = 100;
    port.SetTimeouts(timeouts);

//    port.Set0WriteTimeout();
 //   port.Set0ReadTimeout();

 /*   char sBuf[] = "BCD:01";
	char iBuf[256];
	char cr=13;
	int readb;
    port.Write(sBuf, strlen(sBuf));
    port.Write(&cr, 1);
	readb=port.Read(iBuf,256);
    port.Flush();
*/
    DWORD dwErrors;                      
    port.ClearError(dwErrors);
//	  port.TerminateOutstandingWrites();
//  port.TerminateOutstandingReads();
//  port.ClearWriteBuffer();
 // port.ClearReadBuffer();

    port.Flush();
	return 1;
}
  //_____________________________________________________________________________ 
void AcqCOM::Close()
{
		if(port.IsOpen())
	{
   port.Flush();
   port.Close();
	}

}
  //_____________________________________________________________________________ 
void AcqCOM::Flush()
{
	port.Flush();
}


  //_____________________________________________________________________________ 
void AcqCOM::CleanAll()
{
    DWORD dwErrors;                      
    port.ClearError(dwErrors);
  port.TerminateOutstandingWrites();
  port.TerminateOutstandingReads();
  port.ClearWriteBuffer();
  port.ClearReadBuffer();
}

  //_____________________________________________________________________________ 
int AcqCOM::SendString(char * string, int N)
{
	string[N]=0;
//	printf("Com send : %s\n",string);
	return port.Write(string, N);
}
  //_____________________________________________________________________________ 
int AcqCOM::RecvString(char * string, int N) //Non-Blocking call!
{
	int res;
	res=port.Read(string, N);
	string[res]=0;
//	printf("Com received : %s\n",string);
	return res;
}