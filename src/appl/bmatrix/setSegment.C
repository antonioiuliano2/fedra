//#include <ctype.h>

int even(const int n)
{
 	return !(n&1);
}

void setSegment(const char *file)
{
	char 	NewRootFile[255],
		TextFile[255],
		TextFileTop[255],
		TextFileBot[255];
	sprintf(NewRootFile,"%s",file);
	sprintf(NewRootFile+strlen(file)-5,"%s\0","+seg.root");
	cout<<strlen(NewRootFile)<<endl;
	cout<<NewRootFile<<endl;
	sprintf(TextFile,"%s",file);
	sprintf(TextFile+strlen(file)-5,"%s\0",".txt");

	sprintf(TextFileTop,"%s",file);
	sprintf(TextFileTop+strlen(file)-5,"%s\0","_top.txt");

	sprintf(TextFileBot,"%s",file);
	sprintf(TextFileBot+strlen(file)-5,"%s\0","_bot.txt");

	cout<<TextFile<<endl;
	EdbRun *run = new EdbRun(file,"READ");
	EdbMarksSet* mark = run->GetMarks();
	EdbRun *rnew = new EdbRun(*run,NewRootFile);		//"test_Jun_2004_5.64+seg.root");
	rnew->Save();
	FILE *f=fopen(TextFile,"w");		//("test_Jun_2004_5.64.txt","w");
	FILE *ftop=fopen(TextFileTop,"w");
	FILE *fbot=fopen(TextFileBot,"w");
	FILE *fpred=NULL;

	int nEntries = run->GetEntries();
	EdbSegment *s = 0;
	EdbCluster *c = 0;
	EdbView* v=0;
	fprintf(f,"view,sID,X,Y,Z,Tx,Ty,puls,side\n");
	fprintf(ftop,"view,sID,X,Y,Z,Tx,Ty,puls,side\n");
	fprintf(fbot,"view,sID,X,Y,Z,Tx,Ty,puls,side\n");
	for (int j=firstView; j<nEntries; j++) {
		v = run->GetEntry(j,1,1,0,0,1);
////////////////////////////////////////
	float S=0.147;
	int pulsthres = 10;
	float accept = 5.;

	float tgxmin = -0.050;
	float tgxmax = 0.050;
	float tgymin = -0.050;
	float tgymax = 0.050;
	float tgstep = 0.005;

	float xmin = -15.;
	float xmax = 15.;
	float ymin = -15.;
	float ymax = 15.;

	cout<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<tgxmin<<" "<<tgxmax<<" "<<tgymin<<" "<<tgymax<<endl<<endl;

///////////////////////////////////////////

//////////////////  MAIN SEGMENTATION PROCEDURE ////////////////////////////////////////////////
//		TBinTracking *tr = new TBinTracking(tgxmin,tgxmax,tgymin,tgymax,tgstep);
		TBinTracking *tr = new TBinTracking(tgxmin,tgxmax,tgymin,tgymax,tgstep,0.1,0.1,1.6);
		EdbView* vnew = tr->AdoptSegment(v,S,xmin,xmax,ymin,ymax,1.7*S,pulsthres,accept,0,1);		//0.3);
/////////////////////////////////////////////////////////////////////////////////////////////////
		int nseg = vnew->Nsegments();
//		int nclust = vnew->Nclusters();
		printf("View number=%d, nseg=%d, xView=%f, yView=%f \n",j,nseg,v->GetXview(),v->GetYview());
		fprintf(f,"View number=%d, nseg=%d \n",j,nseg);
		for (int i=0; i<nseg; i++) {
			s = vnew->GetSegment(i);
			s->Print();
			fprintf(f,"%d , %d , %f , %f , %f , %f , %f , %d , %d\n",j,s->GetID(),s->GetX0(),s->GetY0(),s->GetZ0(),s->GetTx(),s->GetTy(),s->GetPuls(),s->GetSide());
			if (even(j)) fprintf(ftop,"%d , %d , %f , %f , %f , %f , %f , %d , %d\n",j,s->GetID(),s->GetX0(),s->GetY0(),s->GetZ0(),s->GetTx(),s->GetTy(),s->GetPuls(),s->GetSide());
			else fprintf(fbot,"%d , %d , %f , %f , %f , %f , %f , %d , %d\n",j,s->GetID(),s->GetX0(),s->GetY0(),s->GetZ0(),s->GetTx(),s->GetTy(),s->GetPuls(),s->GetSide());
			int nclust =s->GetNelements();
			int nn=1;
			for (int ic=0; ic<nclust; ic++) {
				c = (EdbCluster *)(s->GetElements()->At(ic));
//				if (c->GetSegment()==s->GetID()) {
				printf("<<<<<<<< %d: X=%f, Y=%f, Z=%f\n", ic+1, c->GetX(), c->GetY(), c->GetZ());
				fprintf(f,"<<<<<<<< %d: X=%f, Y=%f, Z=%f\n", ic+1, c->GetX(), c->GetY(), c->GetZ());
				nn++;
//				}
			}
			printf("\n");
			fprintf(f,"\n");
//			_getch();
		}

//		run->Close();

//		delete vnew;
		rnew->AddView(vnew);
		rnew->Save();
		delete tr;
	}
	fclose(f);
	fclose(ftop);
	fclose(fbot);
	rnew->Close();
//	run->Close();

//	return 0;
}

//______________________________________________________________________________

void a2v(float xa, float ya, EdbMarksSet *mark, EdbView *v, float *xv, float *yv)
{
//  EdbRun r(file);

//	 EdbMarksSet* mark = r->GetMarks();
//mark->Print();

float	 a11 = (mark->Abs2Stage()->A11() );	//Perform Stage to Absolute coordinates transform
float	 a12 = (mark->Abs2Stage()->A12() );	//Abs2Stage is a pointer to EdbAffine2D    
float	 a21 = (mark->Abs2Stage()->A21() ); 
float	 a22 = (mark->Abs2Stage()->A22() );
float	 b1  = (mark->Abs2Stage()->B1() );
float	 b2  = (mark->Abs2Stage()->B2() ); 

float xs = a11*xa+a12*ya + b1;
float ys = a21*xa+a22*ya + b2;

//EdbView *v = r.GetEntry(0,1,1,1,1,1);
EdbViewHeader *vh = v->GetHeader();
float xV = vh->GetXview();
float yV = vh->GetYview();

 *xv = xs - xV;
 *yv = ys - yV;

printf("xa=%f, ya=%f, xv=%f , yv=%f \n",xa,ya,*xv,*yv);
}
