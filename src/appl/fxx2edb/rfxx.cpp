#include "EdbRun.h"
#include "EdbView.h"
#include "EdbSegment.h"

int readFXX( char *file, char *froot, int tob, char *stat="RECREATE");

//---------------------------------------------------------------------------
int main()
{
  readFXX("/mnt/utsdbs01_m/data/OPERA/TEST23/UP/&F01"  ,"fxx_08.23.root",1);
  readFXX("/mnt/utsdbs01_m/data/OPERA/TEST23/DOWN/&F01","fxx_08.23.root",2,"UPDATE");

//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST24/UP/&F01"  ,"fxx_09.24.root",1);
//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST24/DOWN/&F01","fxx_09.24.root",2,"UPDATE");

//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST25/UP/&F01"  ,"fxx_10.25.root",1);
//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST25/DOWN/&F01","fxx_10.25.root",2,"UPDATE");

//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST26/UP/&F01"  ,"fxx_11.26.root",1);
//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST26/DOWN/&F01","fxx_11.26.root",2,"UPDATE");

//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST27/UP/&F01"  ,"fxx_10.27.root",1);
//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST27/DOWN/&F01","fxx_10.27.root",2,"UPDATE");

//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST29/UP/&F01"  ,"fxx_10.29.root",1);
//    readFXX("/mnt/utsdbs01_m/data/OPERA/TEST29/DOWN/&F01","fxx_10.29.root",2,"UPDATE");

  return 0;
}

//---------------------------------------------------------------------------
int readFXX( char *file, char *froot, int tob, char *stat)
{
  EdbRun        edbRun(froot,stat);
  EdbView       *edbView = edbRun.GetView();
  EdbViewHeader *header  = edbView->GetHeader();

  int     i,n, n_fields;
  int     npre;
  int     runevent,run,event,pos;
  int     stack_,module,tb,plate,pver;
  int     stage;
  int     re,s,m,t,p,nn;
  int     n1,num,f1,f2,f3;
  long    scanmode;

  int     ph;
  double  ax,ay,x,y,dx,dy;
  double  gap,emulsion,base,shrink;
  double  dr;
  double  axc,ayc,xc,yc,axs,ays,xs,ys;
  double  xmin,xmax,ymin,ymax;

  char str[256];

  FILE *f_fxx = fopen( file, "r");

  fgets(str,sizeof(str),f_fxx);
  fgets(str,sizeof(str),f_fxx);
  fgets(str,sizeof(str),f_fxx);

  EdbSegment* edbSegment = new EdbSegment();

  int nviews=0;
  while(fgets(str,sizeof(str),f_fxx)) {

    if( sscanf(str,"%d %d %ld %lf %lf %lf %lf %d %lf %d %d %d",
	       &n1,&runevent,&scanmode,&axc,&ayc,&xc,&yc,&num,&dr,&f1,&f2,&f3) != 12 ) {
      printf("unexpected format in &f (view line): %s\n",str);
      break;
    }
    edbView->Clear();

    header->SetAreaID(runevent);
    header->SetCoordXY(xc,yc);
    if(tob==1)      header->SetNframes(16, 0);
    if(tob==2)      header->SetNframes( 0,16);
    header->SetNsegments(num);

    for(i=0;i<num;++i){                   // read &f segments lines
      if( !fgets(str,sizeof(str),f_fxx) ) break;
      if( sscanf(str,"%d %d %d %lf %lf %lf %lf %lf %lf",
		 &nn,&re,&ph,&ax,&ay,&x,&y,&dx,&dy) != 9) {
	printf("unexpected format in &f (seg line): %s\n",str);
	break;
      }

      edbSegment->Set(x-xc, y-yc, dx, ax, ay, dy-dx, tob, ph);
      edbView->AddSegment(edbSegment);

    }
    nviews++;
    edbRun.AddView(edbView);
    printf("view %d \t Area = %d\n",nviews,runevent);
  }

  fclose(f_fxx);
  edbRun.Close();

  return 0;
}
