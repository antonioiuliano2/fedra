


#include<EdbPattern.h>
#include<EdbDataSet.h>
#include<EdbPVRec.h>



int npid = 0;
int plid[100];
double zlayer[100];
int pid[1000];

void write_lnkdef(void){
	FILE *fp = fopen("lnk.def","wt");
	fprintf(fp, "INPUT_RUNS_LIST		lnk.lst\n"
				"OUTPUT_DATA_DIR		./data/\n"
				"PARAMETERS_DIR		./par/\n");
	fclose(fp);
}

void write_lnklst(void){
	FILE *fp = fopen("lnk.lst","wt");
	for(int i=0;i<npid; i++){
		fprintf(fp, "%3d 1 ./data/* 1\n", plid[i]);
	}
	fclose(fp);
}

void write_defaultpar(void){
	FILE *fp = fopen("default.par","wt");
	fprintf(fp, "SIGMA0   0       5  5  .005  .005\n"
			"DEGRAD   0       5.\n"
			"BINS     0       4 4 4 4\n"
			"RAMP0    0       15  15\n"
			"RAMP04   0       15  15\n"
			"CHI2MAX  0	 4.5\n"
			"CHI2PMAX 0	 4.5\n"
			"RADX0    0       5810.\n");
	fclose(fp);
}

int z_not_set(int pos){
	if(zlayer[pid[pos]]>=1E10) return 1;
	return 0;
}

void write_par_files(void){
	int i;
	printf("make par directry\n");
	system("mkdir par");
	printf("write parameters files\n");
	for(i=0;i<npid;i++){
		FILE *fp = fopen(Form("par/%02d_001.par", plid[i]),"wt");
		fprintf(fp,"INCLUDE default.par\n");
		fprintf(fp,"ZLAYER 0 %.1lf 0. 0.\n", zlayer[i]);
		fprintf(fp,"AFFXY 0 1 0 0 1 0 0\n");
		fclose(fp);
	}
}

int m2track(char *filename){
	int i,j,k;

	FILE *fp = fopen(filename,"rt");
	if(NULL==fp) {
		printf("File %s error!\n", filename);
		return 0;
	}

	printf("write lnk.def\n");
	write_lnkdef();

	printf("write default.par\n");
	write_defaultpar();

	char buf[256];
	TObjArray *tracks = new TObjArray();
	for(i=0;i<100;i++) zlayer[i]=1.1E10;
	
	printf("reanding m-file : %s\n", filename);
	for(i=0;NULL!=fgets(buf,sizeof(buf),fp);i++){
		if(buf[0]=='%') {i--; continue;}
		
		if(i==0) { continue;}	// header, comment
		if(i==1) { sscanf(buf,"%d", &npid); continue;} // header, number of plates
		if(i==2) {				// header, plate numbers.
			int n=0,nn;
			
			for(j=npid-1;j>=0;j--){
			//for(j=0;j<npid;j++){
				sscanf(buf+n,"%d%n", &plid[j], &nn);
				plid[j]/=10;
				plid[j]=58-plid[j];
				pid[plid[j]]=j;
				n+=nn;
			}
			printf("write lnk.lst\n");
			write_lnklst();
			continue;
		}
		
		// i>=3 track informations.
		int itrk, nseg, pos1, pos2;
		sscanf(buf, "%d %d %d %d", &itrk, &nseg, &pos1, &pos2); // track headers
		
		EdbTrackP *t = new EdbTrackP();
		t->SetID(itrk);
		
		//printf("t %s", buf);
		EdbSegP *s=0;
		for(j=0;j<nseg;j++){	// data of segmetns.
			int pos,isg,ph;
			double ax,ay,x,y,z;
			fgets(buf,sizeof(buf),fp);
			//printf("s %s", buf);
			sscanf(buf,"%d %d %*d %d %lf %lf %lf %lf %lf",
						&pos, &isg, &ph, &ax, &ay, &x, &y, &z);
			pos/=10;
			pos=58-pos;
			if(z_not_set(pos)) zlayer[pid[pos]]=z;
			
			s = new EdbSegP(isg, x,y, ax,ay, ph/10000, 0);
			s->SetZ(z);
			s->SetPID(pid[pos]);
			s->SetPlate(pos);
			s->SetTrack(itrk);
			t->AddSegment(s);
			t->AddSegmentF( new EdbSegP(*((EdbSegP*)(s))) );
			
		}
		t->Set(itrk, s->X(), s->Y(), s->TX(), s->TY(), 0, 1);
		t->SetZ(s->Z());
		t->SetCounters();
		//if(nseg!=1) {		// ignore nseg==1 tracks.
			tracks->Add(t);
			if(tracks->GetEntries()%100 == 0) printf("%d tracks\r", tracks->GetEntries());
		//}
	}
	printf(" %d tracks\n.", tracks->GetEntries());

	printf("write tracks into linked_tracks.root\n");
	EdbDataProc::MakeTracksTree(*tracks,0,0,"linked_tracks.root");
	
	write_par_files();
	
	return tracks->GetEntries();
}


#ifndef __CINT__

int main( int argc, char *argv[] ){
	if(argc<2){
		printf("usage : m2track.exe mxx.all\n");
		return 1;
	}
	m2track(argv[1]);
	return 1 ;
}
#endif
