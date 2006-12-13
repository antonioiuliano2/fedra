//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqOdyssey                                                           //
//                                                                      //
// Interface to MATROX ODYSSEY framegrabber								//
//	using Odyssey Native Library							            //
//
// Cable connecting DB25 "Digital IO" connector of MICOS amplifyer block
// with DB9 auxiliary digital connector of Odyssey Xpro must be present
// to provide syncronous stage-framegrabber operation.
// The pinout is as follows:
//
//  DB9 Male		DB25 Female
//  Pin Signal		Pin Signal
//  1   "Trig0"		5   "Breakpoint3"
//  6   "GND"       16  "GND"
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TCanvas.h"

#include "AcqOdyssey.h"
#include "TROOT.h"
#include "TSystem.h"
//#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TClonesArray.h"
#include "EdbImage.h"



ClassImp(AcqOdyssey);
//_____________________________________________________________________________ 

//_____________________________________________________________________________ 
AcqOdyssey::AcqOdyssey()
{
 BGImageValid=false;
 SubtractBG=false;
 ApplyFIRF=false;
 eEdbClusters=0;
 Threshold=50;
 PixelToMicronX=-0.292;
 PixelToMicronY=0.292;
 OpticalShrinkage=1.515;
 eEdbView=0;


}


//_____________________________________________________________________________ 
void AcqOdyssey::FreeResources()
{
	/* Clean up */
   /* Stop the continuous grab and free digitizer (if needed). */
   if (MilDigitizer)
      {
      MdigHalt(MilDigitizer);
      MdigFree(MilDigitizer);
	  }
   
   /* Deselect the image from the display. */
   MdispDeselect(MilDisplay, MilImageDisp);

   /* Free image. */
   MbufFree(MilImageDisp);
   for (int n=0; n<MAX_NB_GRAB; n++)
      {
		  if(MilImage[n]) MbufFree(MilImage[n]);
 	  }
 //  MbufFree(MilOverlayImage)   ;
   MbufFree(MilImageN);
   MbufFree(MilBGImage);
   MbufFree(MilGCImage);
   MbufFree(MilConvImage);
   MbufFree(MilConvImage2);
   MbufFree(MilConvImage32);
  // MbufFree(MilKernel)    ;
   MbufFree(MilGCKernel)    ;
	  MimFree(MilProjResX);
	  MimFree(MilProjResY);
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
   for (int n=0; n<MAX_NB_GRAB; n++)
      {
		  if(HostBuffer[n]) imBufFree(Thread,HostBuffer[n]);
 	  }
      if(Kernel) imBufFree(Thread,Kernel);
       MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);

      
   /* Free defaults allocations. */
   MdispFree(MilDisplay);
   MsysFree(MilSystem);
   MappFree(MilApplication);
  // MappFreeDefault(MilApplication, MilSystem, M_NULL, M_NULL, M_NULL);


}
AcqOdyssey::~AcqOdyssey()
{
/*	  
   MappFreeDefault(MilApplication, MilSystem, MilDisplay,
                                  MilDigitizer, MilImage);
*/
	FreeResources();

}
//_____________________________________________________________________________ 
bool  AcqOdyssey::Initialize(char * DCF, int N)
{
//Initialises the ODYSSEY board with default OPERA  parameters
// N defines maximum number of layers to take in one grab
	BGImageValid=false;
	GCImageValid=false;
	  eEdbClusters=new  TClonesArray("EdbCluster",5000);
	  eEdbView=new EdbView();
	  Kernel=0;

   if(N>MAX_NB_GRAB) N=MAX_NB_GRAB;
   MaxNbGrab=N;
   float ImageScale=1;
   // Allocations.
   MappAlloc(M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEF_SYSTEM_TYPE, M_DEF_SYSTEM_NUM, M_SETUP, &MilSystem);

   MdigAlloc(MilSystem, M_DEFAULT, (MIL_TEXT_PTR)DCF,
                                   M_DEFAULT, &MilDigitizer);
   MdispAlloc(MilSystem, M_DEF_DISPLAY_NUM, M_DEF_DISPLAY_FORMAT, M_DEFAULT, &MilDisplay);
 //  MdispControl(MilDisplay,M_THREAD_PRIORITY,31);
 // MdispControl(MilDisplay,M_WINDOW_OVR_DESTRUCTIVE,M_ENABLE);
  MdispControl(MilDisplay,M_WINDOW_OVR_WRITE,M_ENABLE);
 // MdispControl(MilDisplay,M_WINDOW_OVR_FLICKER,M_ENABLE);


  MdispZoom(MilDisplay,-2,-2);
    SizeX=MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)*ImageScale;
   SizeY=MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)*ImageScale;
   printf("Image size aquired from Digitizer : %d X %d\n",SizeY,SizeX);
 
   /* Allocate display buffer. */   
   MbufAlloc2d(MilSystem,SizeX,SizeY,8L+M_UNSIGNED,M_IMAGE+M_PROC+M_GRAB+M_DISP, &MilImageDisp);
   MbufClear(MilImageDisp, 0x0);            
   MdispSelect(MilDisplay, MilImageDisp);
   /* Prepare overlay buffer. */
   /***************************/

   /* Disable overlay display while preparing the data. */
   MdispControl(MilDisplay, M_OVERLAY_SHOW, M_DISABLE);
        
   /* Enable writing Overlay graphics on top of display buffer. */
   MdispControl(MilDisplay, M_OVERLAY, M_ENABLE);
        
   /* Inquire the Overlay buffer associated with the displayed buffer. */
   MdispInquire(MilDisplay, M_OVERLAY_ID, &MilOverlayImage);

   /* Inquire the current transparent color. */
   MdispInquire(MilDisplay, M_TRANSPARENT_COLOR, &TransparentColor);

   /* Clear the overlay buffer with the keying color. */
   MbufClear(MilOverlayImage, TransparentColor); 
   
   /* Enable the overlay display. */
   MdispControl(MilDisplay, M_OVERLAY_SHOW, M_ENABLE);
   /* Set graphic text to transparent background. */
   MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);

   /* Set drawing color to white. */
   MgraColor(M_DEFAULT, 255);

   
   MbufAlloc2d(MilSystem,SizeX,SizeY, 8L+M_UNSIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilBGImage);
     MbufAlloc2d(MilSystem,SizeX,SizeY, 16L+M_SIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilConvImage);
     MbufAlloc2d(MilSystem,SizeX,SizeY, 16L+M_SIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilConvImage2);
     MbufAlloc2d(MilSystem,SizeX,SizeY, 32L+M_UNSIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilConvImage32);
     MbufAlloc2d(MilSystem,SizeX,SizeY, 16L+M_UNSIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilGCImage);
 //    MbufAlloc2d(MilSystem,SizeX,SizeY, 16L+M_SIGNED,M_IMAGE+M_PROC, &MilConvImage1);


    MbufAlloc2d(MilSystem, SizeX, SizeY*N, 8L+M_UNSIGNED,M_IMAGE+M_GRAB+M_PROC+M_ON_BOARD, &MilImageN);
      MbufClear(MilImageN, 0); 
   for (int n=0; n<N; n++)
      {
      MbufChild2d(MilImageN, 0, SizeY*n, SizeX, SizeY, &MilImage[n]);
	  }
        printf("Initialized for grabbing %i frames", N);

   // Getting ONL handlers for sequence async grab
		System=MsysInquire(MilSystem,M_NATIVE_ID,M_NULL);
		Thread=MsysInquire(MilSystem,M_NATIVE_THREAD_ID,M_NULL);
		Camera=MdigInquire(MilDigitizer,M_NATIVE_CAMERA_ID,M_NULL);
		Control=MdigInquire(MilDigitizer,M_NATIVE_CONTROL_ID,M_NULL);
		Digitizer=MdigInquire(MilDigitizer,M_NATIVE_ID,M_NULL);
		BufferN=MbufInquire(MilImageN,M_NATIVE_ID,M_NULL);
		BGImage=MbufInquire(MilBGImage,M_NATIVE_ID,M_NULL);
		GCImage=MbufInquire(MilGCImage,M_NATIVE_ID,M_NULL);
		ConvImage=MbufInquire(MilConvImage,M_NATIVE_ID,M_NULL);
		ConvImage2=MbufInquire(MilConvImage2,M_NATIVE_ID,M_NULL);
		ConvImage32=MbufInquire(MilConvImage32,M_NATIVE_ID,M_NULL);
		Overlay=MbufInquire(MilOverlayImage,M_NATIVE_ID,M_NULL);
   for (int n=0; n<N; n++)
      {
		Buffer[n]=MbufInquire(MilImage[n],M_NATIVE_ID,M_NULL);
        imBufAlloc2d(Thread,SizeX,SizeY,IM_UBYTE,IM_HOST,&HostBuffer[n]);
		imBufMap(Thread,HostBuffer[n],0,0,&pHostBuffer[n],&HostBufferPitch,&HostBufferLines);
		printf("Host buffer %d mapped with pitch %d, %d lines.\n",n,HostBufferPitch,HostBufferLines);
		if(HostBufferPitch==0) {printf("Can't map buffer! Aborting.\n"); return false;}
	  }
		imThrAlloc(Digitizer, 0, &GrabThread);
		imThrAlloc(Digitizer, 0, &SeqThread);
	//	imThrAlloc(Digitizer, 0, &Thread);
		/* OSB for synchronization */
    imSyncAlloc(Thread, &GrabOSB);
    imSyncAlloc(Thread, &CopyOSB);
 	imBufAlloc1d( Thread,N,IM_LONG,IM_PROC,&ShiftList);
	for(int n=0;n<N;n++){
	imBufChild(Thread,BufferN, 0, SizeY*n, SizeX, SizeY, &ShiftBuffer[n]);
	}
	imBufPut(Thread,ShiftList,ShiftBuffer);

       MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	   MbufControl(MilImageN,M_MODIFIED,M_DEFAULT);

	  MimAllocResult(MilSystem, SizeX, M_PROJ_LIST, &MilProjResX);
	  MimAllocResult(MilSystem, SizeY, M_PROJ_LIST, &MilProjResY);

	  printf("Framegrabber initialized successfully with following parameters:\n");
	  Dump();
	return true;

}
//_____________________________________________________________________________ 
bool  AcqOdyssey::IsGrabComplete()
{
	return true;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::SetFIRFNorm(int Norm)
{
	//   if(MilKernel) MbufControlNeighborhood(MilKernel,M_NORMALIZATION_FACTOR,Norm);
	   bNorm=(char)Norm;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::SetFIRFKernel(char *Kern, int W, int H)
{
//   if(MilKernel) MbufFree(MilKernel);
//	MbufAlloc2d(MilSystem,H,W,8+M_SIGNED,M_KERNEL,&MilKernel);
//   MbufPut(MilKernel,Kern);

    MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	   imBufAlloc2d(Thread,H,W,IM_BYTE,IM_PROC,&Kernel);
	   imBufPut(Thread,Kernel,Kern);
	   imBufAllocControl(Thread,&ControlBuf);
	   imBufPutField(Thread,ControlBuf,IM_CTL_OVERSCAN, IM_REPLACE);
	   imBufPutField(Thread,ControlBuf,IM_CTL_OVERSCAN_VAL, 0);
    MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);


 //  MbufControlNeighborhood(MilKernel,M_SATURATION,M_ENABLE);
 //  MbufControlNeighborhood(MilKernel,M_ABSOLUTE_VALUE,M_ENABLE);
 //  MbufControlNeighborhood(MilKernel,M_ABSOLUTE_VALUE,M_ENABLE);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::ShowImage(int N)
{
	char ztext[64];
	sprintf(ztext,"Z=%f",ZBuffer[N]);
	MbufCopy(MilImage[N],MilImageDisp);
	DrawClear();
	DrawText(0,0,0,ztext);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::ShowBGImage()
{
	MbufCopy(MilBGImage,MilImageDisp);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::ShowGCImage()
{
				//imIntMonadic(Thread,ConvImage32,-8,ConvImage,IM_SHIFT,CopyOSB);
	MimClip(MilGCImage,MilImageDisp,M_OUT_RANGE,0,255,0,255);
//	MbufCopy(MilGCImage,MilImageDisp);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::PlotImage1D(int N, TH1F* hist)
{
			unsigned char *bf;
	long Sx,Sy;
	if(!hist)
	{
    TCanvas *c=new TCanvas();
//	c->Divide(2);
//	c->cd(1);
//	GetEdbImage(N)->GetHist2()->Draw("colz");
	c->cd();
	GetEdbImage(N)->GetHist1()->Draw();
	}
	else 
	{
	GetImageFast(N,&bf,&Sx,&Sy);
	for(int c=0;c<Sx*Sy;c++) { hist->Fill(bf[c]); }
	}
}
//_____________________________________________________________________________ 
void  AcqOdyssey::PlotImage2D(int N)
{
    TCanvas *c=new TCanvas();
	c->Divide(2);
	c->cd(1);
	GetEdbImage(N)->GetHist2()->Draw("colz");
	c->cd(2);
	GetEdbImage(N)->GetHist1()->Draw();
}
//_____________________________________________________________________________ 
void  AcqOdyssey::PlotBGImage()
{
	MbufCopy(MilBGImage,MilImageDisp);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::PlotGCImage()
{
unsigned short *bf;
bf=(unsigned short *)malloc(SizeX*SizeY*2);
GetGCImage((short*)bf);
TH1F *hist= new TH1F("GC","GC",65536,0,65536);
for(int c=0;c<SizeX*SizeY;c++) { hist->Fill(bf[c]); }
hist->Draw();

}
//_____________________________________________________________________________ 
EdbFrame* AcqOdyssey::GetEdbFrame(int N)
{
	EdbFrame *fr=new EdbFrame(N,ZBuffer[N],0,0);
	fr->SetImage(GetEdbImage(N));
	return fr;
}
//_____________________________________________________________________________ 
EdbImage* AcqOdyssey::GetEdbImage(int N)
{
	EdbImage* im=new EdbImage();
	unsigned char *bf;
	long Sx,Sy;
//	char *bf=(char*)malloc(SizeX*SizeY);
//	MbufGet(MilImage[N],bf);
	GetImageFast(N,&bf,&Sx,&Sy);
	im->AdoptImage(Sx,Sy,(char*)bf);
	return im;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::GetImage(int N, char* buf)
{
	MbufGet(MilImage[N],buf);
}

//_____________________________________________________________________________ 

void  AcqOdyssey::GetImageFast(int N, unsigned char** buf, long *Pitch, long* Lines)
{
    MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
			imSyncControl(SeqThread, CopyOSB1,IM_OSB_STATE,IM_WAITING);
    imBufCopyPCI(SeqThread,Buffer[N],HostBuffer[N],0,CopyOSB1);
			imSyncHost(SeqThread, CopyOSB1, IM_COMPLETED);
	MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	*buf=(unsigned char*)pHostBuffer[N];
	*Pitch=HostBufferPitch;
	*Lines=HostBufferLines;

}

//_____________________________________________________________________________ 
void  AcqOdyssey::CopyImageToBG(int N)
{
	MbufCopy(MilImage[N],MilBGImage);
    BGImageValid=true;

}
//_____________________________________________________________________________ 
void  AcqOdyssey::CopyImageToGC(int N, long ConstBG)
{
char GCKernel[4][4]=
{
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1},
	{1,1,1,1}
};
long ConstBG32;
long TargetConstant;
MIL_ID MilResult;
   MimAllocResult(MilSystem,1,M_EXTREME_LIST,&MilResult);
   MbufAlloc2d(MilSystem,4,4,8+M_SIGNED,M_KERNEL,&MilGCKernel);
   MbufPut(MilGCKernel,GCKernel);
   MbufControlNeighborhood(MilGCKernel,M_NORMALIZATION_FACTOR,16);
	MimClip(MilImage[N],MilGCImage,M_OUT_RANGE,0,255,0,255);
	MimConvolve(MilGCImage,MilGCImage,MilGCKernel);
	MimConvolve(MilGCImage,MilGCImage,MilGCKernel);
	MimConvolve(MilGCImage,MilGCImage,MilGCKernel);
	MimConvolve(MilGCImage,MilGCImage,MilGCKernel);
	MimConvolve(MilGCImage,MilGCImage,MilGCKernel);
	MimArith(0x00ff,MilGCImage,MilGCImage,M_CONST_SUB);
	// calculate maximum pixel PH
//	MimFindExtreme(MilGCImage,MilResult,M_MAX_VALUE);
//	MimGetResult(MilResult,M_VALUE,&MaximumPix);
//	MaximumPix=MaximumPix*256;
	ConstBG32=((ConstBG*256) & 0xffff);
	TargetConstant=0xf0ff - ConstBG32;
	MimArith(TargetConstant,MilGCImage,MilGCImage,M_CONST_DIV);
	ConstBG32=ConstBG;
	MimArith(MilGCImage,ConstBG,MilGCImage,M_ADD_CONST);
//	MimArith(MaximumPix,MilGCImage,MilGCImage,M_CONST_DIV); //normalize to MaxPix*256
//	MimConvolve(MilGCImage,MilGCImage,M_SMOOTH);
//	MimConvolve(MilGCImage,MilGCImage,M_SMOOTH);
//	MimConvolve(MilGCImage,MilGCImage,M_SMOOTH);
//	MbufCopy(MilImage[N],MilGCImage);
    GCImageValid=true;
	MbufFree(MilGCKernel);
	MimFree(MilResult);

}
//_____________________________________________________________________________ 
void  AcqOdyssey::PutImage(int N, char* buf)
{
	MbufPut(MilImage[N],buf);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::PutBGImage(short* buf)
{
	MbufPut(MilBGImage,buf);
	BGImageValid=true;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::GetBGImage(short* buf)
{
	MbufGet(MilBGImage,buf);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::PutGCImage(short* buf)
{
	MbufPut(MilGCImage,buf);
	GCImageValid=true;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::GetGCImage(short* buf)
{
	MbufGet(MilGCImage,buf);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::ShowImages(int N1, int N2, int TimeInterval, int Times)
{
// Displays images from N1 to N2 and back with a given time gap 
// Times times.

	char ztext[64];
	sprintf(ztext,"Z from %f to %f",ZBuffer[N1],ZBuffer[N2]);
//		DrawClear();
		DrawText(0,0,0,ztext);
	for(int i=0;i<Times;i++)
	{
        for(int N=N1;N<N2;N++)
		{
	     MbufCopy(MilImage[N],MilImageDisp);
		 Sleep(TimeInterval);
		}
        for(int N=N2;N>N1;N--)
		{
	     MbufCopy(MilImage[N],MilImageDisp);
		 Sleep(TimeInterval);
		}
	}
}

//_____________________________________________________________________________ 
int  AcqOdyssey::GrabImagesONL(int N, float *Zs, double Timeout)
{
// Grabs N images, process them and return: 
//if ApplyFIRF and MilKernel are set, then FIR filtering is made
// if SubtractBG flag is set - BG image is subtracted
// USING ODYSSEY NATIVE MODE
// returns  number of frames taken 
// Timeout is given in seconds, if = 0 - waits forever! (caution)
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
    /* Get ready to grab the sequence */
		//if(MilKernel) Kernel=MbufInquire(MilKernel,M_NATIVE_ID,M_NULL);
			printf("Starting Digitizer grab sequence...\n");
        imDigGrabSequence(GrabThread, Digitizer, Camera, BufferN, N, Control, GrabOSB);
			printf("Waiting for IM_READY from Digitizer...\n");
        if(Timeout>0) imSyncControl(Thread, GrabOSB,IM_OSB_TIMEOUT, Timeout);
		if(imSyncHost(SeqThread, GrabOSB, IM_READY)==IM_ERR_TIMEOUT){
			printf("Timeout happened waiting for IM_READY :-(\n");
			//TODO: 
			imSyncGrabSequence(Thread, GrabOSB, 1, IM_SEQ_ABORT);
			return 0;
		}
    int Frame=0;
    /* Sync to each block of frames and display one of them */
       for (int n = 0; n < N; n++)
        {
	//		printf("Waiting for frame %d...\n",n);
	//		if(Timeout>0) printf("...with Timeout = %f ms...\n",Timeout*1000.);
            Frame=imSyncGrabSequence(Thread, GrabOSB, n, IM_SEQ_WAIT);
			if(Frame==IM_ERR_TIMEOUT) {
				printf("Timeout happened waiting for frame %d :-(\n",n);
				//TODO: 
				imSyncGrabSequence(Thread, GrabOSB, 1, IM_SEQ_ABORT);
				return n-1;
			}
	//		printf("Got %d frames :-) Processing frame %d...\n",Frame+1,n);
        // Now we can process frame
			if(ApplyGainCorr)
			{
				// Multiply image (0-256)  by GC map (256-65535 range) into 32 bit buffer
				imIntDyadic(Thread,Buffer[n],GCImage,ConvImage32,IM_MULT,0);
				// Shift down by 8 bits - must be back in range 0 to 256
				imIntMonadic(Thread,ConvImage32,-8,ConvImage,IM_SHIFT,0);
				// negate image (255-Image)
			    imIntMonadic(Thread,ConvImage,255,ConvImage,IM_SUB_NEG,0);
			}
			else imIntMonadic(Thread,Buffer[n],255,ConvImage,IM_SUB_NEG,0); //negate only

//			if(SubtractBG && BGImageValid) {
//				imIntDyadic(Thread,ConvImage,BGImage,ConvImage,IM_SUB_SAT,CopyOSB);
//			}
			if(ApplyFIRF && Kernel) {
				imIntConvolve(Thread,ConvImage,ConvImage2,Kernel,ControlBuf,0);
				imIntMonadic(Thread,ConvImage2,bNorm,ConvImage,IM_DIV,0);
			}
			if(SubtractBG && BGImageValid) {
				imIntDyadic(Thread,ConvImage,BGImage,ConvImage,IM_SUB_SAT,0);
			}
			imIntConvert(Thread,ConvImage,Buffer[n],IM_CLIP,0);
	//		imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
			imIntClip(Thread,Buffer[n],Buffer[n],IM_LESS,Threshold,0xff,0,0xff,IM_THRESH_CONSTANT+IM_PASS_PIXEL,CopyOSB);
		//	imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);
			imSyncHost(Thread, CopyOSB, IM_COMPLETED);
			if(Zs) ZBuffer[n]=Zs[N-n-1];
//			printf("Done with frame %d. :-) \n",n);
        }

     /* Wait to make sure the grab OSB is reset for next time */
     //   imSyncHost(SeqThread, GrabOSB, IM_COMPLETED);
       imSyncHost(Thread, GrabOSB, IM_COMPLETED);
	   
	   MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	   	   for(int n=0;n<N;n++) MbufControl(MilImage[n],M_MODIFIED,M_DEFAULT);
	   	   MbufControl(MilImageN,M_MODIFIED,M_DEFAULT);
   return N;

}
//_____________________________________________________________________________ 
int  AcqOdyssey::GrabRawImagesONL(int N, float *Zs, double Timeout)
{
// test function
// Grabs N images,  and return: no processing is done
// USING ODYSSEY NATIVE MODE
// returns  number of frames taken 
// Timeout is given in seconds, if = 0 - waits forever! (caution)
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
    /* Get ready to grab the sequence */
		//if(MilKernel) Kernel=MbufInquire(MilKernel,M_NATIVE_ID,M_NULL);
			printf("Starting Digitizer grab sequence...\n");
        imDigGrabSequence(GrabThread, Digitizer, Camera, BufferN, N, Control, GrabOSB);
			printf("Waiting for IM_READY from Digitizer...\n");
        if(Timeout>0) imSyncControl(Thread, GrabOSB,IM_OSB_TIMEOUT, Timeout);
		if(imSyncHost(SeqThread, GrabOSB, IM_READY)==IM_ERR_TIMEOUT){
			printf("Timeout happened waiting for IM_READY :-(\n");
			//TODO: 
			imSyncGrabSequence(Thread, GrabOSB, 1, IM_SEQ_ABORT);
			return 0;
		}
    int Frame=0;
    /* Sync to each block of frames and display one of them */
       for (int n = 0; n < N; n++)
        {
	//		printf("Waiting for frame %d...\n",n);
	//		if(Timeout>0) printf("...with Timeout = %f ms...\n",Timeout*1000.);
            Frame=imSyncGrabSequence(Thread, GrabOSB, n, IM_SEQ_WAIT);
			if(Frame==IM_ERR_TIMEOUT) {
				printf("Timeout happened waiting for frame %d :-(\n",n);
				//TODO: 
				imSyncGrabSequence(Thread, GrabOSB, 1, IM_SEQ_ABORT);
				return n-1;
			}
	//		printf("Got %d frames :-) Processing frame %d...\n",Frame+1,n);
        // Now we can process frame
			//imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
			imIntClip(Thread,Buffer[n],Buffer[n],IM_LESS,Threshold,0xff,0,0xff,IM_THRESH_CONSTANT+IM_PASS_PIXEL,CopyOSB);
			//imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);
			imSyncHost(Thread, CopyOSB, IM_COMPLETED);
			if(Zs) ZBuffer[n]=Zs[N-n-1];
//			printf("Done with frame %d. :-) \n",n);
        }

     /* Wait to make sure the grab OSB is reset for next time */
       // imSyncHost(SeqThread, GrabOSB, IM_COMPLETED);
      imSyncHost(Thread, GrabOSB, IM_COMPLETED);
	   
	   MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	   	   for(int n=0;n<N;n++) MbufControl(MilImage[n],M_MODIFIED,M_DEFAULT);
	   	   MbufControl(MilImageN,M_MODIFIED,M_DEFAULT);
   return N;

}

//_____________________________________________________________________________ 
 long AcqOdyssey::GrabAndFillView(int N, float *Zs, double Timeout, int Side, EdbView * View, bool FillImages)
 {
// Grabs N images, process them, clusterise by Fire-In-Steep procedure, and fill the EdbView: 
//if ApplyFIRF and MilKernel are set, then FIR filtering is made
// if SubtractBG flag is set - BG image is subtracted
// USING ODYSSEY NATIVE MODE
// if pointer to View is not given, result is saved into eEdbView;
// Cluster coordinates are returned in microns with respect to image center!!!
// returns total number of clusters found.
// Timeout is given in seconds, if = 0 - waits forever! (caution)
// This method is asyncronous combination of GrabImagesONL, Clusterise and FillView...

	    long TNumber=0;
    long Number=0;
    long PixNumber=0;
	EdbView *V;
	TClonesArray * Cl;
	if(View) V=View; else V=eEdbView; 
	V->Clear();
	Cl=V->GetClusters();
	int NtakenFrames=0;
//	V->SetNframes(N*Side,N*(1-Side));
	V->SetNframes(NtakenFrames*Side,NtakenFrames*(1-Side));

      MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
    /* Get ready to grab the sequence */
		//if(MilKernel) Kernel=MbufInquire(MilKernel,M_NATIVE_ID,M_NULL);
			printf("Starting Digitizer grab sequence...\n");
        imDigGrabSequence(GrabThread, Digitizer, Camera, BufferN, N, Control, GrabOSB);
			printf("Waiting for IM_READY from Digitizer...\n");
        if(Timeout>0) imSyncControl(Thread, GrabOSB,IM_OSB_TIMEOUT, Timeout);
        if(Timeout>0) imSyncControl(Thread, CopyOSB,IM_OSB_TIMEOUT, Timeout*10);
		if(imSyncHost(SeqThread, GrabOSB, IM_READY)==IM_ERR_TIMEOUT){
			printf("Timeout happened waiting for IM_READY :-(\n");
			//TODO: 
			imSyncGrabSequence(Thread, GrabOSB, 1, IM_SEQ_ABORT);
			return 0;
		}
    int Frame=0;
    /* Sync to each block of frames and display one of them */
       for (int n = 0; n < N; n++)
        {
	//		printf("Waiting for frame %d...\n",n);
	//		if(Timeout>0) printf("...with Timeout = %f ms...\n",Timeout*1000.);
            Frame=imSyncGrabSequence(Thread, GrabOSB, n, IM_SEQ_WAIT);
			if(Frame==IM_ERR_TIMEOUT) {
				printf("Timeout happened waiting for frame %d :-(\n",n);
				//TODO: 
				imSyncGrabSequence(Thread, GrabOSB, n, IM_SEQ_ABORT);
				return 0;
			}
	//		printf("Got %d frames :-) Processing frame %d...\n",Frame+1,n);
        // Now we can process frame
			if(ApplyGainCorr)
			{
				// Multiply image (0-256)  by GC map (256-65535 range) into 32 bit buffer
				imIntDyadic(Thread,Buffer[n],GCImage,ConvImage32,IM_MULT,0);
				// Shift down by 8 bits - must be back in range 0 to 256
				imIntMonadic(Thread,ConvImage32,-8,ConvImage,IM_SHIFT,0);
				// negate image (255-Image)
			    imIntMonadic(Thread,ConvImage,255,ConvImage,IM_SUB_NEG,0);
			}
			else imIntMonadic(Thread,Buffer[n],255,ConvImage,IM_SUB_NEG,0); //negate only

//			if(SubtractBG && BGImageValid) {
//				imIntDyadic(Thread,ConvImage,BGImage,ConvImage,IM_SUB_SAT,CopyOSB);
//			}
			if(ApplyFIRF && Kernel) {
				imIntConvolve(Thread,ConvImage,ConvImage2,Kernel,ControlBuf,0);
				imIntMonadic(Thread,ConvImage2,bNorm,ConvImage,IM_DIV,0);
			}
			if(SubtractBG && BGImageValid) {
				imIntDyadic(Thread,ConvImage,BGImage,ConvImage,IM_SUB_SAT,0);
			}
			imIntConvert(Thread,ConvImage,Buffer[n],IM_CLIP,0);
	//		imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
			imIntClip(Thread,Buffer[n],Buffer[n],IM_LESS,Threshold,0xff,0,0xff,IM_THRESH_CONSTANT+IM_PASS_PIXEL,CopyOSB);
// AT this moment current image processing is charged to thread Thread and we can clusterize the previous image
// and fill the EdbView....
			if(n>0) {
				Number=Clusterize(n-1,Cl,Side);
	            V->AddFrame(n-1,ZBuffer[n-1],Number,0);
	            if(FillImages) ((EdbFrame*)(V->GetFrames()->At(n-1)))->SetImage(GetEdbImage(n-1));
	            TNumber+=Number;
				NtakenFrames++;
				V->SetNframes(NtakenFrames*Side,NtakenFrames*(1-Side));

			}
// Wait till current image processing is complete and go on with next..
		//	imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);
		if(imSyncHost(Thread, CopyOSB, IM_COMPLETED)==IM_ERR_TIMEOUT){
			printf("Timeout happened waiting for IM_COMPLETED :-(\n");
			//TODO: 
			imSyncGrabSequence(Thread, GrabOSB, 1, IM_SEQ_ABORT);
			return 0;
		}
			
			if(Zs) ZBuffer[n]=Zs[N-n-1];
//			printf("Done with frame %d. :-) \n",n);
        }
// AT this moment last image is processed and we can clusterize it
// and fill the EdbView....
				Number=Clusterize(N-1,Cl,Side);
	            V->AddFrame(N-1,ZBuffer[N-1],Number,0);
	            if(FillImages) ((EdbFrame*)(V->GetFrames()->At(N-1)))->SetImage(GetEdbImage(N-1));
	            TNumber+=Number;
				NtakenFrames++;
				V->SetNframes(NtakenFrames*Side,NtakenFrames*(1-Side));
// Finished with the last..

     /* Wait to make sure the grab OSB is reset for next time */
      //  imSyncHost(SeqThread, GrabOSB, IM_COMPLETED);
      imSyncHost(Thread, GrabOSB, IM_COMPLETED);
	   
	   MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	   	   for(int n=0;n<N;n++) MbufControl(MilImage[n],M_MODIFIED,M_DEFAULT);
	   	   MbufControl(MilImageN,M_MODIFIED,M_DEFAULT);
   return TNumber;


 }

//_____________________________________________________________________________ 
void  AcqOdyssey::TracksFromNImages(int N,  int DX, int DY, float TX, float TY, int Dest)
{
	// Calculate average of shifted by TX,TY stack of images a la UTS
	// TX, TY are frame shifts in pixels (frame x w.r.t. frame x-1) 
	// in the centered ROI of the size DY x DX pixels 
	// Puts the result into Dest Mil buffer
	// Sync
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	if(DX>SizeX-2*N*TX) DX=SizeX-2*N*TX;
	if(DY>SizeY-2*N*TY) DY=SizeY-2*N*TY;
	int OFX,OFY;
	OFX=IM_PARENT+(SizeX-DX)/2;
	OFY=IM_PARENT+(SizeY-DY)/2;
	for(int n=0;n<N;n++)
	{
	imBufChildMove(Thread, ShiftBuffer[n], OFX+n*TX, OFY+SizeY*n+n*TY, DX, DY);
	}
	imBufChildMove(Thread, ShiftBuffer[Dest], OFX, OFY+SizeY*Dest, DX, DY);

	imBufClear(Thread,Buffer[Dest],0,CopyOSB); //clear destination buffer
//	imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
	imIntAverage(Thread, ShiftList, 0, 0, ShiftBuffer[Dest], N, N, 0,CopyOSB);
	if(ApplyFIRF && Kernel)
	{
			imIntConvolve(Thread,Buffer[Dest],ConvImage,Kernel,0,CopyOSB);
			if(bNorm!=1) imIntMonadic(Thread,ConvImage,bNorm,ConvImage,IM_DIV,CopyOSB);
			imIntConvert(Thread,ConvImage,Buffer[Dest],IM_CLIP,CopyOSB);

	}
//	imIntClip(Thread,ShiftBuffer[Dest],ShiftBuffer[Dest],IM_OUT_RANGE,Threshold,255,0,255,IM_PASS_PIXEL,CopyOSB);
	imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
	imIntClip(Thread,Buffer[Dest],Buffer[Dest],IM_LESS,Threshold,0xff,0,0xff,IM_THRESH_CONSTANT+IM_PASS_CONSTANT,CopyOSB);
	imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);
	       MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);

}

//_____________________________________________________________________________ 
void  AcqOdyssey::DrawTrack(int N, float X, float Y, float TX, float TY)
{
	// Draw track on the display overlay according to number of layers N
	// TX, TY are frame shifts in pixels (frame x w.r.t. frame x-1) 
	// DZ is calculated automatically from Image number (from 0 to N-1)
	int Ix0,Ix,Iy0,Iy;
		Ix0=int(X/PixelToMicronX+SizeX/2.);
		Iy0=int(Y/PixelToMicronY+SizeY/2.);
	//	Ix=int((X+N*TX)/PixelToMicronX+SizeX/2.);
	//	Iy=int((Y+N*TY)/PixelToMicronY+SizeY/2.);
 
	MgraLine(M_DEFAULT, MilOverlayImage, Ix0,Iy0, Ix0+N*TX, Iy0+N*TY);
}
void  AcqOdyssey::DrawText(int Layer, int X, int Y, char *text)
{
     MgraText(M_DEFAULT, MilOverlayImage, X, Y,    MIL_TEXT(text));

}

void AcqOdyssey::DrawCircles(float RadPitch) //pitches are in microns!
{
	//Draws measuring circles into overlay 
	// the radius pitches are given in microns
	int rX,rY;
	float PixPitchX=RadPitch/fabs(PixelToMicronX);
	float PixPitchY=RadPitch/fabs(PixelToMicronY);
	for( int d=1; d*PixPitchX*2<SizeX; d++)
	{
		rX=int(d*PixPitchX);
 		rY=int(d*PixPitchY);
      MgraArc(M_DEFAULT, MilOverlayImage, SizeX/2,SizeY/2,rX,rY,0,360);
 	}

}

void AcqOdyssey::DrawGrid(float MajorPitch, float MinorPitch) //pitches are in microns!
{
	//Draws measuring grid into overlay 
	// the pitches are given in microns
	int X,Y;
	float PixPitchX=MajorPitch/fabs(PixelToMicronX);
	float PixPitchY=MajorPitch/fabs(PixelToMicronY);
	for( int dX=0; dX*PixPitchX*2<SizeX; dX++)
	{
		X=int(SizeX/2.+dX*PixPitchX);
       MgraLine(M_DEFAULT, MilOverlayImage, X,0, X, SizeY-1);
		X=int(SizeX/2.-dX*PixPitchX);
       MgraLine(M_DEFAULT, MilOverlayImage, X,0, X, SizeY-1);
	}
	for( int dY=0; dY*PixPitchY*2<SizeY; dY++)
	{
		Y=int(SizeY/2.+dY*PixPitchY);
       MgraLine(M_DEFAULT, MilOverlayImage, 0,Y, SizeX-1, Y);
		Y=int(SizeY/2.-dY*PixPitchY);
       MgraLine(M_DEFAULT, MilOverlayImage, 0,Y, SizeX-1, Y);
	}
	PixPitchX=MinorPitch/fabs(PixelToMicronX);
	PixPitchY=MinorPitch/fabs(PixelToMicronY);
	for( int dX=0; dX*PixPitchX*2<SizeX; dX++)
	{
		X=int(SizeX/2.+dX*PixPitchX);
       MgraLine(M_DEFAULT, MilOverlayImage, X,SizeY/2-10, X, SizeY/2+10);
		X=int(SizeX/2.-dX*PixPitchX);
       MgraLine(M_DEFAULT, MilOverlayImage, X,SizeY/2-10, X, SizeY/2+10);
	}
	for( int dY=0; dY*PixPitchY*2<SizeY; dY++)
	{
		Y=int(SizeY/2.+dY*PixPitchY);
       MgraLine(M_DEFAULT, MilOverlayImage, SizeX/2-10,Y, SizeX/2+10, Y);
		Y=int(SizeY/2.-dY*PixPitchY);
       MgraLine(M_DEFAULT, MilOverlayImage, SizeX/2-10,Y, SizeX/2+10, Y);
	}
}

void AcqOdyssey::DrawClusters(TClonesArray * Clusters)
{
	//Draws circles around each cluster 
	// with a diameter equal to cluster diameter;
	TClonesArray * arr;
	EdbCluster * cl;
	int r,x,y;
	if(!Clusters) arr=eEdbClusters;
	if(!arr) return;
	for(int i=0;i<arr->GetEntries();i++)
	{
		cl=(EdbCluster*)(arr->At(i));
		r=int(2.*sqrt(cl->GetArea())/3.14);
		x=int(cl->GetX()/PixelToMicronX+SizeX/2.);
		y=int(cl->GetY()/PixelToMicronY+SizeY/2.);
        MgraArc(M_DEFAULT, MilOverlayImage, x,y,r,r,0,360);
	}
}


void  AcqOdyssey::DrawClear()
{
   // Clear the overlay buffer with the keying color.
   MbufClear(MilOverlayImage, TransparentColor); 
}


long AcqOdyssey::FillView(int N, int Side, EdbView * View, bool FillImages)
{
	// Finds clusters in a given images sequence (from 0 to N) and fills the EdbView structure
	// if pointer to View is not given, result is saved into eEdbView;
	// Cluster coordinates are returned in microns with respect to image center!!!
	// returns total number of clusters found.
	printf("Filling View..."); fflush(stdout);
    long TNumber=0;
    long Number=0;
    long PixNumber=0;
	EdbView *V;
	TClonesArray * Cl;
	if(View) V=View; else V=eEdbView; 
	V->Clear();
	Cl=V->GetClusters();
	  V->SetNframes(N*Side,N*(1-Side));
    for(int n=0;n<N;n++)
	{
      Number=FindClusters(n,Cl,Side);
	  V->AddFrame(n,ZBuffer[n],Number,0);
	  if(FillImages)
	  {
		  ((EdbFrame*)(V->GetFrames()->At(n)))->SetImage(GetEdbImage(n));
	  }

	  TNumber+=Number;
	}
	printf("Done.\n"); fflush(stdout);
	return TNumber;
}


//____________________________________________________________________________

//int AcqOdyssey::BurnPix( unsigned char *buf, int ic, int ir, int xSize, int ySize, EdbClustP *cl )
int BurnPix( unsigned char *buf, int ic, int ir, int xSize, int ySize, EdbCluster *cl )
{
  
  int pix=(int)(buf[ic+ir*xSize]);
  if( pix==0 )   return 0;
  if(cl->GetArea()>5000) return 1;

  cl->AddPixelSum(ic,ir,pix);

  buf[ic+ir*xSize]=0;
  pix = 1;
  
  int xn[4] = { 1, -1, 0,  0 };    //pixel closest suburbs
  int yn[4] = { 0,  0, 1, -1 };
  int pp;
  int i,j;

  for(int in=0; in<4; in++) {
    i = ic+xn[in];
    j = ir+yn[in];
	if(i<0 || i>=xSize) continue;
	if(j<0 || j>=ySize) continue;
    pp = (int)( buf[i+j*xSize]);
    if( pp ==0 ) continue;
    pix += BurnPix(buf,i,j,xSize,ySize,cl);
 //   BurnPix(buf,i,j,xSize,ySize,cl);
  }
  return pix;
}



long  AcqOdyssey::Clusterize(int N, TClonesArray * Clusters, int Side)
{
  long nc ;
  long nr ;
  unsigned char *buf;
  GetImageFast(N,&buf,&nc,&nr);
  return Clusterize(N,buf,nc,nr,Clusters,Side);
}

long AcqOdyssey::Clusterize(int N, unsigned char *buf, int nc,int nr, TClonesArray * Clusters, int Side) 
{
// c++ remake of  good old "Fire in steppe" by Valery Tiukov ;-)
   
  int pix;
  int wcl;
  long   ncl=0;
  int Border=8;
  			float x,y,z,a,v;
			int s,seg;
//  printf("Clusterize Image %d: %d %d....",N,nc,nr);
 // fflush(stdout);

  EdbCluster *cl=new EdbCluster();
  if(!Clusters) Clusters=eEdbClusters;
//  Clusters->Clear();

int ir,ic,l;
for(int i=0;i<nc*nr;i++)
{
	if(buf[i]==0) continue;
    ir=int(i/nc);
	if(ir<Border || ir>nr-Border) continue;
	ic=i-ir*nc;
	if(ic<Border || ic>nc-Border) continue;
      cl->Set0();
      wcl = BurnPix( buf, ic,ir, nc,nr, cl);
	  cl->Normalize();
      z=ZBuffer[N];
	  x=(cl->X()-SizeX/2.)*PixelToMicronX;
	  y=(cl->Y()-SizeY/2.)*PixelToMicronY;
	  a=cl->GetArea();
				if(a<=1) continue;
	  v=cl->GetVolume();
                l = Clusters->GetLast()+1;
      new((*Clusters)[l])  EdbCluster( x,y,z, a,v,N,Side,-1 );	  
     ncl++;

}
  delete cl;
 // printf("found %d clusters.\n",ncl);
 // fflush(stdout);

  return ncl;
}



//_____________________________________________________________________________ 
long  AcqOdyssey::FindClusters(int N, TClonesArray * Clusters, int Side)
{
	// Finds clusters in a given image
	// if pointer to Clusters is not given, result is saved into eEdbClusters;
	// Cluster coordinates are returned in microns with respect to image center!!!
	// returns number of clusters found.
	long Result;
	long FeatList;
	long Number;
	EdbCluster * cl;
           IM_BLOB_GROUP5_ST *Group5=0;  /* Results */
           IM_BLOB_GROUP1_ST *Group1=0;  /* Results */
	 MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
    int Border=8;
	int DX=SizeX-2*Border;
	int DY=SizeY-2*Border;
	int OFX,OFY;
	OFX=IM_PARENT+Border;
	OFY=IM_PARENT+Border+SizeY*N;
	imBufChildMove(Thread, ShiftBuffer[N], OFX, OFY, DX, DY);
	        /* Allocate a blob feature list and result buffer */
            imBlobAllocFeatureList(Thread, &FeatList);
            imBlobAllocResult(Thread, &Result);
            imBlobSelectFeature(Thread, FeatList, IM_BLOB_NO_FEATURES, IM_DEFAULT);
            imBlobSelectFeature(Thread, FeatList, IM_BLOB_AREA, IM_DEFAULT);
           imBlobSelectFeature(Thread, FeatList, IM_BLOB_SUM_PIXEL, IM_DEFAULT);
    //       imBlobSelectFeature(Thread, FeatList, IM_BLOB_MAX_PIXEL, IM_DEFAULT);
           imBlobSelectFeature(Thread, FeatList, IM_BLOB_CENTER_OF_GRAVITY, IM_DEFAULT);

        /* Increase speed by not saving runs */
            imBlobControl(Thread, Result, IM_BLOB_SAVE_RUNS, IM_DISABLE);

        /* Calculate selected features */
 //           imBlobCalculate(Thread, Buffer[N], Buffer[N], FeatList, Result, IM_CLEAR, 0);
           imBlobCalculate(Thread, ShiftBuffer[N], ShiftBuffer[N], FeatList, Result, IM_CLEAR, 0);

        /* Get the number of blobs */
            imBlobGetNumber(Thread, Result, &Number);
         /* Allocate enough memory for the results */
            Group5 = (IM_BLOB_GROUP5_ST *) malloc(Number * sizeof(IM_BLOB_GROUP5_ST));
            Group1 = (IM_BLOB_GROUP1_ST *) malloc(Number * sizeof(IM_BLOB_GROUP1_ST));
        /* Get the results */
            imBlobGetResult(Thread, Result, IM_BLOB_GROUP5, IM_DEFAULT, Group5);
            imBlobGetResult(Thread, Result, IM_BLOB_GROUP1, IM_DEFAULT, Group1);
			if(!Clusters) Clusters=eEdbClusters;
	//		Clusters->Clear();
			int i=0;
			float x,y,z,a,v;
			int s,seg;
			int NumberReturned=0;
            if(Clusters) for(int cn=0;cn<Number;cn++)
			{
/*				cl=new EdbCluster();
				cl->SetArea(Group1[cn].area); 
				cl->SetVolume(Group5[cn].sum_pixel);
				cl->SetFrame(N); 
				cl->SetSide(0) ;    
				cl->SetSegment(0);
				cl->SetX((Group1[cn].center_of_gravity_x-SizeX/2.)*PixelToMicronX);
				cl->SetY((Group1[cn].center_of_gravity_y-SizeY/2.)*PixelToMicronY);
				cl->SetZ(ZBuffer[N]);
				*/
				a=Group1[cn].area; 
				if(a<=1) continue;
				v=Group5[cn].sum_pixel;
				x=(Group1[cn].center_of_gravity_x+Border-SizeX/2.)*PixelToMicronX;
				y=(Group1[cn].center_of_gravity_y+Border-SizeY/2.)*PixelToMicronY;
				z=ZBuffer[N];
                i = Clusters->GetLast()+1;
                new((*Clusters)[i++])  EdbCluster( x,y,z, a,v,N,Side,-1 );
				NumberReturned++;
//				Clusters->Add(cl);
			}

 //	imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
 //          imBlobFill(Thread,Result,Overlay,M_ALL_BLOBS,223,CopyOSB);
//	imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);

			if(Group1) { delete Group1; Group1=NULL;}
			if(Group5) { delete Group5; Group5=NULL;}
            imBlobFree(Thread, FeatList);
           imBlobFree(Thread, Result);
	 MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
     return NumberReturned;

}
//_____________________________________________________________________________ 
EdbClustersBox*  AcqOdyssey::GetEdbClustersBox(int N1, int N2)
{
//	EdbClustersBox* cb=new EdbClustersBox();
	return NULL;
}

int AcqOdyssey::GrabOneImage(float Z, double Timeout)
{
   // Sets trigger input of Timer1 from output of Timer2, which runs continuously
   // with 50Hz rep. rate (see DCF file)
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	   imCamControl(GrabThread,Camera,IM_DIG_EXP_SOURCE,IM_TIMER2);
// here we grab one image
       if(GrabAndFillView(1, &Z, Timeout, 0, eEdbView, true)==0) return 0;
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	   imCamControl(GrabThread,Camera,IM_DIG_EXP_SOURCE,IM_DEFAULT);
	   MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
//	   StopLiveGrab();

	return 1;
}

void  AcqOdyssey::StartLiveGrab()
{
   // Sets exposure from output of Timer2, which runs continuously
   // with 50Hz rep. rate (see DCF file)
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	   imCamControl(GrabThread,Camera,IM_DIG_EXP_SOURCE,IM_TIMER2);
	   MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	   MdigGrabContinuous(MilDigitizer,MilImageDisp);

}
void  AcqOdyssey::StopLiveGrab()
{
   // Sets trigger input of Timer1 from HW input 0 on DB9 connector, which 
	// is triggered by BreakPoint output from the stage controller
	// This configuration must be default one saved in DCF file
	// This function waits for last frame to be completed, so
	// if it must be called only when digitizer is in LiveGrab mode!!
	   MdigHalt(MilDigitizer);
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	   imCamControl(GrabThread,Camera,IM_DIG_EXP_SOURCE,IM_DEFAULT);
       MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	

}


  void AcqOdyssey::XProjectImage(int N, long *buf)
  {
	  // Projects image to horisontal axis and fill the linear buffer buf
	  MimProject(MilImage[N],MilProjResX,M_0_DEGREE);
	  MimGetResult1d(MilProjResX,0,SizeX,M_VALUE,buf);

  }
  void AcqOdyssey::YProjectImage(int N, long *buf)
  {
	  // Projects image to horisontal axis and fill the linear buffer buf
	  MimProject(MilImage[N],MilProjResY,M_90_DEGREE);
	  MimGetResult1d(MilProjResY,0,SizeY,M_VALUE,buf);

  }

  int AcqOdyssey::VFindEdge(int N, int Thresh, int MinWidth, int MaxWidth)
  {
	  // Fing horisontal emulsion edge in image sequence 0-N
	long *buf;
	buf=new long[SizeX];
	long MaxDeriv[MAX_NB_GRAB];
	long MinDeriv[MAX_NB_GRAB];
	long MaxDerivX[MAX_NB_GRAB];
	long MinDerivX[MAX_NB_GRAB];
	long MaxDer,MinDer,MaxDerX,MinDerX;
	int Delta=30;
	for(int n=0;n<N;n++)
	{
     for(int i=0;i<SizeX;i++) buf[i]=0;
     XProjectImage(n,buf);
	 MaxDeriv[n]=0; MinDeriv[n]=0;
	 MaxDerivX[n]=0; MinDerivX[n]=0;
     for(int i=Delta;i<SizeX-2*Delta;i++)
	 {
		 if(MaxDeriv[n]<buf[i]-buf[i+Delta]){ MaxDeriv[n]=buf[i]-buf[i+Delta]; MaxDerivX[n]=i+Delta/2;}
		 if(MinDeriv[n]>buf[i]-buf[i+Delta]) {MinDeriv[n]=buf[i]-buf[i+Delta]; MinDerivX[n]=i+Delta/2;}
	 }
	}
    MaxDer=0;MinDer=0;
	for(int n=0;n<N;n++)
	{
		if(MaxDer<MaxDeriv[n]) {MaxDer=MaxDeriv[n]; MaxDerX=MaxDerivX[n];}
		if(MinDer>MinDeriv[n]) {MinDer=MinDeriv[n]; MinDerX=MinDerivX[n];}
	}
	printf("Max derivative %d detected at X= %d\n",MaxDer,MaxDerX);
	printf("Min derivative %d detected at X= %d\n",MinDer,MinDerX);
	  delete [] buf;

	if(MinDer>-Thresh || MaxDer<Thresh || MaxDerX-MinDerX<MinWidth || MaxDerX-MinDerX>MaxWidth) {printf("No edge detected\n");  return -1;}
	else printf("Edge detected at %f\n",(MaxDerX+MinDerX)/2.);

	  return (MaxDerX+MinDerX)/2.;
  }
  int AcqOdyssey::HFindEdge(int N, int Thresh, int MinWidth, int MaxWidth)
  {
	  // Fing horisontal emulsion edge in image sequence 0-N
	long *buf;
	buf=new long[SizeY];
	long MaxDeriv[MAX_NB_GRAB];
	long MinDeriv[MAX_NB_GRAB];
	long MaxDerivY[MAX_NB_GRAB];
	long MinDerivY[MAX_NB_GRAB];
	long MaxDer,MinDer,MaxDerY,MinDerY;
	int Delta=30;
	for(int n=0;n<N;n++)
	{
     for(int i=0;i<SizeY;i++) buf[i]=0;
     YProjectImage(n,buf);
	 MaxDeriv[n]=0; MinDeriv[n]=0;
	 MaxDerivY[n]=0; MinDerivY[n]=0;
     for(int i=Delta;i<SizeY-2*Delta;i++)
	 {
		 if(MaxDeriv[n]<buf[i]-buf[i+Delta]){ MaxDeriv[n]=buf[i]-buf[i+Delta]; MaxDerivY[n]=i+Delta/2;}
		 if(MinDeriv[n]>buf[i]-buf[i+Delta]) {MinDeriv[n]=buf[i]-buf[i+Delta]; MinDerivY[n]=i+Delta/2;}
	 }
	}
    MaxDer=0;MinDer=0;
	for(int n=0;n<N;n++)
	{
		if(MaxDer<MaxDeriv[n]) {MaxDer=MaxDeriv[n]; MaxDerY=MaxDerivY[n];}
		if(MinDer>MinDeriv[n]) {MinDer=MinDeriv[n]; MinDerY=MinDerivY[n];}
	}
	printf("Max derivative %d detected at Y= %d\n",MaxDer,MaxDerY);
	printf("Min derivative %d detected at Y= %d\n",MinDer,MinDerY);
	  delete [] buf;

	if(MinDer>-Thresh || MaxDer<Thresh || MaxDerY-MinDerY<MinWidth || MaxDerY-MinDerY>MaxWidth) {printf("No edge detected\n");  return -1;}
	else printf("Edge detected at %f\n",(MaxDerY+MinDerY)/2.);

	  return (MaxDerY+MinDerY)/2.;
  }
  void AcqOdyssey::UARTSendString(const char *s, int N)
  {
	  long adc=(long)s;
	 MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
	 imDigControl(Thread,Digitizer,IM_UART_BAUD_RATE+IM_CHANNEL_0,9600);
	 imDigControl(Thread,Digitizer,IM_UART_WRITE_LENGTH+IM_CHANNEL_0,1);
	 for(int i=0;i<N;i++){
	 imDigControl(Thread,Digitizer,IM_UART_WRITE_STRING+IM_CHANNEL_0,adc+i);
	 }
	 MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);

  }
