//////////////////////////////////////////////////////////////////////////
//                                                                      //
// AcqOdyssey                                                           //
//                                                                      //
// Interface to MATROX ODYSSEY framegrabber								//
//	using Odyssey Native Library							            //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "AcqOdyssey.h"
#include "TROOT.h"
#include "TSystem.h"
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
      
   /* Free defaults allocations. */
   MappFreeDefault(MilApplication, MilSystem, MilDisplay, M_NULL, M_NULL);


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
	BGImageValid=false;
   if(N>MAX_NB_GRAB) N=MAX_NB_GRAB;
   float ImageScale=1;
   // Allocations.
   MappAlloc(M_DEFAULT, &MilApplication);
   MsysAlloc(M_DEF_SYSTEM_TYPE, M_DEF_SYSTEM_NUM, M_SETUP, &MilSystem);

   MdigAlloc(MilSystem, M_DEFAULT, (MIL_TEXT_PTR)DCF,
                                   M_DEFAULT, &MilDigitizer);
   MdispAlloc(MilSystem, M_DEF_DISPLAY_NUM, M_DEF_DISPLAY_FORMAT, M_DEFAULT, &MilDisplay);
   MdispControl(MilDisplay,M_THREAD_PRIORITY,31);
  MdispControl(MilDisplay,M_WINDOW_OVR_DESTRUCTIVE,M_ENABLE);
  MdispControl(MilDisplay,M_WINDOW_OVR_FLICKER,M_ENABLE);
    SizeX=MdigInquire(MilDigitizer, M_SIZE_X, M_NULL)*ImageScale;
   SizeY=MdigInquire(MilDigitizer, M_SIZE_Y, M_NULL)*ImageScale;
 
   /* Allocate display buffer. */   
   MbufAlloc2d(MilSystem,SizeX,SizeY,8L+M_UNSIGNED,M_IMAGE+M_PROC+M_DISP, &MilImageDisp);
   MbufClear(MilImageDisp, 0x0);            
   MdispSelect(MilDisplay, MilImageDisp);
     MbufAlloc2d(MilSystem,SizeX,SizeY, 8L+M_UNSIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilBGImage);
     MbufAlloc2d(MilSystem,SizeX,SizeY, 16L+M_SIGNED,M_IMAGE+M_PROC+M_ON_BOARD, &MilConvImage);
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
		ConvImage=MbufInquire(MilConvImage,M_NATIVE_ID,M_NULL);
   for (int n=0; n<N; n++)
      {
		Buffer[n]=MbufInquire(MilImage[n],M_NATIVE_ID,M_NULL);
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
	   if(MilKernel) MbufControlNeighborhood(MilKernel,M_NORMALIZATION_FACTOR,Norm);
	   bNorm=(char)Norm;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::SetFIRFKernel(char *Kern, int W, int H)
{
   MbufAlloc2d(MilSystem,H,W,8+M_SIGNED,M_KERNEL,&MilKernel);
   MbufPut(MilKernel,Kern);
 //  MbufControlNeighborhood(MilKernel,M_SATURATION,M_ENABLE);
 //  MbufControlNeighborhood(MilKernel,M_ABSOLUTE_VALUE,M_ENABLE);
 //  MbufControlNeighborhood(MilKernel,M_ABSOLUTE_VALUE,M_ENABLE);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::ShowImage(int N)
{
	MbufCopy(MilImage[N],MilImageDisp);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::ShowBGImage()
{
	MbufCopy(MilBGImage,MilImageDisp);
}
//_____________________________________________________________________________ 
EdbImage* AcqOdyssey::GetEdbImage(int N)
{
	EdbImage* im=new EdbImage();
	char *bf=(char*)malloc(SizeX*SizeY);
	MbufGet(MilImage[N],bf);
	im->AdoptImage(SizeX,SizeY,bf);
	return im;
}
//_____________________________________________________________________________ 
void  AcqOdyssey::GetImage(int N, char* buf)
{
	MbufGet(MilImage[N],buf);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::CopyImageToBG(int N)
{
	MbufCopy(MilImage[N],MilBGImage);
    BGImageValid=true;

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
void  AcqOdyssey::ShowImages(int N1, int N2, int TimeInterval, int Times)
{
	for(int i=0;i<Times;i++)
	{
        for(int N=N1;N<N2;N++)
		{
	     MbufCopy(MilImage[N],MilImageDisp);
		}
        for(int N=N2;N>N1;N--)
		{
	     MbufCopy(MilImage[N],MilImageDisp);
		}
	}
}
//_____________________________________________________________________________ 
void  AcqOdyssey::GrabImages(int N)
{
// Grabs N images, process them and return:
//if ApplyFIRF and MilKernel are set, then FIR filtering is made
// if SubtractBG flag is set - BG image is subtracted

   MdigControl(MilDigitizer, M_GRAB_MODE, M_ASYNCHRONOUS);
   printf("Recording ...\n");
long NFields;
   /* Grab the sequence. */
   for (int n=0; n<N; n++)
      {
      /* Grab one buffer at a time. */
      MdigGrab(MilDigitizer, MilImage[n]);
	  if( n>0 )
		{
			if(SubtractBG && BGImageValid) {
				 MimArith(255,MilImage[n-1],MilConvImage,M_CONST_SUB);
				 MimArith(MilConvImage,MilBGImage,MilConvImage,M_SUB);
			}
			else MimArith(255,MilImage[n-1],MilConvImage,M_CONST_SUB);

			if(ApplyFIRF && MilKernel) MimConvolve( MilConvImage,MilConvImage,MilKernel);
			MimClip(MilConvImage,MilImage[n-1],M_OUT_RANGE,Threshold,255,0,255);

		}
	  }
      
   /* Wait last grab end. */
   MdigGrabWait(MilDigitizer, M_GRAB_END);
			if(SubtractBG && BGImageValid) 
			{
				 MimArith(255,MilImage[N-1],MilConvImage,M_CONST_SUB);
				 MimArith(MilConvImage,MilBGImage,MilConvImage,M_SUB);
			}
			else MimArith(255,MilImage[N-1],MilConvImage,M_CONST_SUB);

			if(ApplyFIRF && MilKernel) MimConvolve( MilConvImage,MilConvImage,MilKernel);
			MimClip(MilConvImage,MilImage[N-1],M_OUT_RANGE,Threshold,255,0,255);
  

}
//_____________________________________________________________________________ 
void  AcqOdyssey::GrabImagesONL(int N)
{
// Grabs N images, process them and return:
//if ApplyFIRF and MilKernel are set, then FIR filtering is made
// if SubtractBG flag is set - BG image is subtracted
	// USING ODYSSEY NATIVE MODE
       MsysControl(MilSystem,M_NATIVE_MODE_ENTER,M_NULL);
    /* Get ready to grab the sequence */
		if(MilKernel) Kernel=MbufInquire(MilKernel,M_NATIVE_ID,M_NULL);
			printf("Programming grab sequence\n");
        imDigGrabSequence(GrabThread, Digitizer, Camera, BufferN, N, Control, GrabOSB);
			printf("WAiting for IM_READY\n");

        imSyncHost(SeqThread, GrabOSB, IM_READY);
    int Frame=0;
    /* Sync to each block of frames and display one of them */
       for (int n = 0; n < N; n++)
        {
			printf("Waiting for frame %d\n",n);
            Frame=imSyncGrabSequence(Thread, GrabOSB, n, IM_SEQ_WAIT);
			printf("Got frame %d. Processing\n",Frame);

        // Now we can process frame
			imIntMonadic(Thread,Buffer[n],255,ConvImage,IM_SUB_NEG,CopyOSB);
			if(SubtractBG && BGImageValid) {
				imIntDyadic(Thread,ConvImage,BGImage,ConvImage,IM_SUB,CopyOSB);
			}

			if(ApplyFIRF && MilKernel) {
				imIntConvolve(Thread,ConvImage,ConvImage,Kernel,0,CopyOSB);
				if(bNorm!=1) imIntMonadic(Thread,ConvImage,bNorm,ConvImage,IM_DIV,CopyOSB);
			}
			imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
			imIntConvert(Thread,ConvImage,Buffer[n],IM_CLIP,CopyOSB);
			imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);
        }

     /* Wait to make sure the grab OSB is reset for next time */
	//	imSyncGrabSequence(Thread, GrabOSB, N-1, IM_SEQ_HALT);
        imSyncHost(SeqThread, GrabOSB, IM_COMPLETED);


	   
	   
	   MsysControl(MilSystem,M_NATIVE_MODE_LEAVE,M_NULL);
	   	   for(int n=0;n<N;n++) MbufControl(MilImage[n],M_MODIFIED,M_DEFAULT);
	   	   MbufControl(MilImageN,M_MODIFIED,M_DEFAULT);

}

//_____________________________________________________________________________ 
void  AcqOdyssey::TracksFromNImages(int N, float TX, float TY, int Dest)
{
	for(int n=0;n<N;n++)
	{
	imBufChildMove(Thread, ShiftBuffer[n], IM_PARENT+N*TX+n*TX, IM_PARENT+SizeY*n+N*TY+n*TY, SizeX-2*N*TX, SizeY-2*N*TY);
	}
	imSyncControl(SeqThread, CopyOSB,IM_OSB_STATE,IM_WAITING);
	imIntAverage(Thread,ShiftList,0,0,Buffer[Dest],N,N,0,CopyOSB);
	imSyncHost(SeqThread, CopyOSB, IM_COMPLETED);
}
//_____________________________________________________________________________ 
void  AcqOdyssey::FindClusters(int N)
{
}