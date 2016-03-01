#include <iostream>
#include "EdbLog.h"
#include "EdbDataSet.h"
// #include "EdbPVRec.h"

//
//   recset -ccd -l -ang -a -f -t5 -raw -nu
//

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        cout<< "usage: \n \trecset { -l | -a | -t | -f | ... } input_data_set_file \n\n";
        cout<< "\t\t  -ccd  - to remove ccd defects (update par/xxx.par file)\n";
        cout<< "\t\t  -l    - link up/down\n";
        cout<< "\t\t  -ang  - correct up/down angles offset and rotations\n";
        cout<< "\t\t  -a[n] - plate to plate alignment (if n=2: rigid patterns) \n";
        cout<< "\t\t  -f[n] - fine alignment based on passed-through tracks (if n=2: rigid patterns)\n";
        cout<< "\t\t  -z[n] - z-position ajustment, n is the tracks length in segs (minimum 2 is a default)\n";
        cout<< "\t\t  -t[n] - tracking (if n>1, holes insertion started - historical option - NOT recommended!)\n";
        cout<< "\t\t  -t -p[p] - tracking&propagation (p is the momentum of the particle in [GeV])\n";
        cout<< "\t\t  -nu   - suppress the update of par files\n";
        cout<< "\t\t  -w    - write the EdbPVRec object into a root file (useful for batch processing)\n";
        cout<< "\t\t  -s    - do shower-reconstruction (To Be Implemented)\n";
	cout<< "\t\t  -D[n] - Set FEDRA DebugLevel (0..4)\n";
        cout<<endl;
        return 0;
    };

    int doCCD=0, doLink=0, doAlign=0, doTrack=0, doTrackCarbonium=0,
        doFine=0, doZ=0, doAngles=0, doRaw=0, noUpdate=0, doWritePVR=0;
    int doDEBUGLEVEL=-1;

    float doPropagation=-1;
    
    char *name = argv[argc-1];

    for(int i=1; i<argc-1; i++ ) {
        char *key  = argv[i];

        if     (!strcmp(key,"-ccd"))  doCCD    =1;
        else if(!strcmp(key,"-ang"))  doAngles =1;
        else if(!strcmp(key,"-rt"))   doRaw    =1;
        else if(!strcmp(key,"-nu"))   noUpdate =1;
        else if(!strcmp(key,"-l"))    doLink   =1;
        else if(!strncmp(key,"-a",2) && strcmp(key,"-ang") ) {
            if(strlen(key)>2)
                sscanf(key+2,"%d",&doAlign);
            if(doAlign==0)                     doAlign=1;
        }
        else if(!strncmp(key,"-f",2)) {
            if(strlen(key)>2)
                sscanf(key+2,"%d",&doFine);
            if(doFine==0)                     doFine=1;
        }
        else if(!strncmp(key,"-z",2)) {
            if(strlen(key)>2)
                sscanf(key+2,"%d",&doZ);
            if(doZ<2)                     doZ=2;
        }
        else if(!strncmp(key,"-tc",3)) {
            if(strlen(key)>3)
                sscanf(key+2,"%d",&doTrackCarbonium);
            if(doTrack==0)                    doTrackCarbonium=1;
        }
        else if(!strncmp(key,"-t",2)) {
            if(strlen(key)>2)
                sscanf(key+2,"%d",&doTrack);
            if(doTrack==0)                     doTrack=1;
        }
        else if(!strncmp(key,"-p",2)) {
            if(strlen(key)>2)
                sscanf(key+2,"%f",&doPropagation);
        }
        else if(!strncmp(key,"-D",2)) {
            if(strlen(key)>2)
                sscanf(key+2,"%d",&doDEBUGLEVEL);
        }
        
        else if(!strcmp(key,"-w"))    doWritePVR=1;
    }

    printf("recset options:  %d %d %d %d %d %d %d %d %f %d %d %s\n",
           doCCD, doLink, doAlign, doTrack, doFine, doZ, doAngles, doRaw, doPropagation, noUpdate, doWritePVR, name);

    EdbDataProc proc(name);
    proc.SetNoUpdate(noUpdate);
    
    if(doDEBUGLEVEL>=0&&doDEBUGLEVEL<=4)       {
      gEDBDEBUGLEVEL=doDEBUGLEVEL;
      cout << "gEDBDEBUGLEVEL = " << gEDBDEBUGLEVEL<< endl;
    }
    if(doCCD)              {
        proc.CheckCCD();
        doCCD=0;
    }
    if(doLink)             {
        proc.Link();
        doLink=0;
    }
    if(doTrack&&doAlign)   {
        proc.AlignLinkTracks(doTrack,doAlign);
        doTrack=0;
        doAlign=0;
    }
    if(doAlign)            {
        proc.Align(doAlign);
        doAlign=0;
    }
    if(doAngles)           {
        proc.CorrectAngles();
        doAngles=0;
    }
    if(doTrack)            {
        proc.LinkTracks(doTrack, doPropagation);
        doTrack=0;
        doPropagation=-1;
    }
    if(doTrackCarbonium)   {
        proc.LinkTracksC(doTrackCarbonium, doPropagation);
        doTrackCarbonium=0;
        doPropagation=-1;
    }
    if(doFine)             {
        proc.FineAlignment(doFine);
        doFine=0;
    }
    if(doZ)                {
        proc.AjustZ(doZ);
        doZ=0;
    }
    if(doRaw)              {
        proc.LinkRawTracks(doRaw);
        doRaw=0;
    }

    if(doWritePVR)         {
	cout << "recset: Create the EdbPVRec object from the EdbDataProc object and write it into the file: ScanVolume_Ali.root " << endl;
        EdbPVRec    *ali  = proc.GetPVR();
	if (NULL==ali) {
	  ali  = new EdbPVRec();
	  proc.InitVolume(ali);
	  proc.SetPVR(ali);
	}
	else {
	 ali->Print();
	}
        
        cout << "recset: (Note that this should come at the last step of recset)." << endl;
        TFile* file = new TFile("ScanVolume_Ali.root","RECREATE");
        ali->Write();
        file->Close();
        cout << "recset: Writing EdbPVRec object into ScanVolume_Ali.root done." << endl;

        doWritePVR=0;
    }

    return 0;
}
