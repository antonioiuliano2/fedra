//#include <ctype.h>

int even(const int n)
{
 	return !(n&1);
}

void setSegment(const char *file, const char *pred_file = 0, const int NumAcq = 20, const int NumOfViews = 0, const int firstView = 0)
{
	char 	NewRootFile[255],
		TextFile[255],
		TextFileTop[255],
		TextFileBot[255];
//		buf[255];
//	strncpy(NewRootFile,file,strlen(file)-5);
//	Cpy(NewRootFile,file,strlen(file)-5);
	sprintf(NewRootFile,"%s",file);
	sprintf(NewRootFile+strlen(file)-5,"%s\0","+seg.root");
	cout<<strlen(NewRootFile)<<endl;
//	cout<<NewRootFile<<endl;
//	Append(NewRootFile,"+seg.root");
//        strcat(NewRootFile,"+seg.root");
	cout<<NewRootFile<<endl;
	sprintf(TextFile,"%s",file);
	sprintf(TextFile+strlen(file)-5,"%s\0",".txt");

	sprintf(TextFileTop,"%s",file);
	sprintf(TextFileTop+strlen(file)-5,"%s\0","_top.txt");

	sprintf(TextFileBot,"%s",file);
	sprintf(TextFileBot+strlen(file)-5,"%s\0","_bot.txt");

//	Cpy(TextFile,file,strlen(file)-5);
//	Append(TextFile,".txt");
	cout<<TextFile<<endl;
//	strcat(strncpy(NewRootFile,file,strlen(file)-5),"+seg.root");
//	strcat(strncpy(TextFile,file,strlen(file)-5),".txt");
	EdbRun *run = new EdbRun(file,"READ");
	EdbMarksSet* mark = run->GetMarks();
	EdbRun *rnew = new EdbRun(*run,NewRootFile);		//"test_Jun_2004_5.64+seg.root");
	rnew->Save();
	FILE *f=fopen(TextFile,"w");		//("test_Jun_2004_5.64.txt","w");
	FILE *ftop=fopen(TextFileTop,"w");
	FILE *fbot=fopen(TextFileBot,"w");
	FILE *fpred=NULL;
	int npr = 0;
	int nv;
	if (pred_file) fpred = fopen(pred_file,"rt");
	if (fpred) {
		if (!fscanf(fpred,"%d",&npr)) npr = 0;
	}
	printf("npr=%d\n",npr);
	int nEntries = run->GetEntries();
	if (npr) nv = npr*2*NumAcq;
	else nv = nEntries;
	if (nv>nEntries) nv = nEntries;
	if ((NumOfViews>0)&&(NumOfViews+firstView<nv)) nv = NumOfViews+firstView;
	EdbSegment *s = 0;
	EdbCluster *c = 0;
//	TClonesArray *cls = 0;
	EdbView* v=0;
	fprintf(f,"view,sID,X,Y,Z,Tx,Ty,puls,side\n");
	fprintf(ftop,"view,sID,X,Y,Z,Tx,Ty,puls,side\n");
	fprintf(fbot,"view,sID,X,Y,Z,Tx,Ty,puls,side\n");
	float Xpred[2], Ypred[2], tgXpr[2], tgYpr[2];
	int cPred = -1;
	int ner=0;
	for (int j=firstView; j<nv; j++) {
		v = run->GetEntry(j,1,1,0,0,1);
////////////////////////////////////////
//	float S=0.147;
	float Sx=0.1486;
	float Sy=0.1488;
	int pulsthres = 10;
	float accept = 1.;

	float tgxmin = -0.050;
	float tgxmax = 0.050;
	float tgymin = -0.050;
	float tgymax = 0.050;
	float tgstep = 0.005;

	float xmin = -15.;
	float xmax = 15.;
	float ymin = -15.;
	float ymax = 15.;

	float xpr, ypr, tgxpr, tgypr;
	int idpr,pulspr;

	if (fpred) for(;cPred<j/(2*NumAcq);cPred++)  {
		ner = 0;
		char b[2];
//		cPred = j/(2*NumAcq);
		fscanf(fpred,"%f %f %f %f %d %d",&xpr,&ypr,&tgxpr,&tgypr,&idpr,&pulspr);
//		fread((void *)buf,sizeof(char),255,fpred);
		for(int l=0;l<2;l++) {
		if (fscanf(fpred,"%s %f %f %f %f %d %d", b,&xpr,&ypr,&tgxpr,&tgypr,&idpr,&pulspr)==7) {
			cout<<b[0]<<" "<<xpr<<" "<<ypr<<endl;
//			ner = 1;
			if (toupper(b[0])=='T') {
				Xpred[0] = xpr;
				Ypred[0] = ypr;
				tgXpr[0] = tgxpr;
				tgYpr[0] = tgypr;
				ner=1;
			}
			else if (toupper(b[0])=='B') {
				Xpred[1] = xpr;
				Ypred[1] = ypr;
				tgXpr[1] = tgxpr;
				tgYpr[1] = tgypr;
				ner=1;
			}
			else printf("<<<<<<<<<< UNKNOWN PREDICTION FILE FORMAT! >>>>>>>>>>>>>>\n");
		}
		else 
			printf("<<<<<<<<<< UNKNOWN PREDICTION FILE FORMAT >>>>>>>>>>>>>>\n");	
		}
	}
	if (ner) {
		float xv,yv;
		if (v->GetNframesTop()) {
			a2v(Xpred[0],Ypred[0],mark,v,&xv,&yv);

			tgxmin = tgXpr[0]-0.050;
			tgxmax = tgXpr[0]+0.050;
			tgymin = tgYpr[0]-0.050;
			tgymax = tgYpr[0]+0.050;


			xmin = xv-30.;
			xmax = xv+30.;
			ymin = yv-30.;
			ymax = yv+30.;
		}
		else {
			a2v(Xpred[1],Ypred[1],mark,v,&xv,&yv);

			tgxmin = tgXpr[1]-0.050;
			tgxmax = tgXpr[1]+0.050;
			tgymin = tgYpr[1]-0.050;
			tgymax = tgYpr[1]+0.050;


			xmin = xv-30.;
			xmax = xv+30.;
			ymin = yv-30.;
			ymax = yv+30.;
		}
	}
	cout<<xmin<<" "<<xmax<<" "<<ymin<<" "<<ymax<<" "<<tgxmin<<" "<<tgxmax<<" "<<tgymin<<" "<<tgymax<<endl<<endl;

///////////////////////////////////////////

//////////////////  MAIN SEGMENTATION PROCEDURE ////////////////////////////////////////////////
		TBinTracking *tr = new TBinTracking(tgxmin,tgxmax,tgymin,tgymax,tgstep);
//		TBinTracking *tr = new TBinTracking(tgxmin,tgxmax,tgymin,tgymax,tgstep,0.1,0.1,1.6);
//		ConfigData *Cfg;
//		EstValue(Cfg);
		EdbView* vnew = tr->AdoptSegment(v,Sx,Sy,xmin,xmax,ymin,ymax,1.7*Sx,pulsthres,accept,1,1,1);		//0.3);
//inside comcluster rep
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
	if (fpred) fclose(fpred);
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