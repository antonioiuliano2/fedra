//#include "stdafx.h"

#include "bitview.h"
#include <math.h>
#include "EdbSegment.h"
#include "Riostream.h"
#include "TMath.h"
#include "deriv.h"

/////////////// do not need /////////////
//#include <conio.h>
////////////////////////////////////

ClassImp(TBitView);
ClassImp(TBinTracking);
//________________________________________________________________________
//========================================================================
inline int In(float X, float Y, float Xmin, float Xmax, float Ymin, float Ymax)
{
	if ((X>=Xmin)&&(X<Xmax)&&(Y>=Ymin)&&(Y<Ymax)) return 1;
	else return 0;
}
//________________________________________________________________________

inline float sqr(float x) {return x*x;}
//________________________________________________________________________

int CommonClusters(EdbSegment *seg0, EdbSegment *seg1)
{
	EdbCluster	*c0 = 0,
				*c1 = 0;
	int numCl0 = seg0->GetNelements(),
		numCl1 = seg1->GetNelements();
	for (int cc0=0; cc0<numCl0; cc0++) {
		c0 = (EdbCluster *)(seg0->GetElements()->At(cc0));
		for (int cc1=0; cc1<numCl1; cc1++) {
			c1 = (EdbCluster *)(seg1->GetElements()->At(cc1));
			if (c0==c1) return true;
		}
	}
	return false;
}
//________________________________________________________________________

void TrackAngle(track tr, float *teta, float *phi)
{
	float tgx = tr.tgx;
	float tgy = tr.tgy;

	float tgteta = TMath::Sqrt(tgx*tgx+tgy*tgy);
	*teta = TMath::ATan(tgteta);
	float tgphi;

	if (tgx!=0.0) {
		tgphi=(tgy)/(tgx);
  //  printf("tgphi=%lf, \n ",tgphi);
		if (tgphi>=0) {
			if (tgx>0) {
				*phi=TMath::ATan(tgphi);
			}
			else {
				*phi=TMath::ATan(tgphi)+TMath::Pi();
			}
		}
		else {
			if (tgx>0) {
				*phi=2*TMath::Pi()-TMath::ATan(-tgphi);
			}
			else {
				*phi=TMath::Pi()-TMath::ATan(-tgphi);
			}
		}
	}
	else {
		if (tgy>0) {
			*phi=TMath::Pi()/2.0;
		}
		else if (tgy<0) {
			*phi=3.0*TMath::Pi()/2.0;
		}
		else {
			*phi=0.0;
		}
    //   printf("phi=%lf ,a=%lf, c=%lf\n", phi,a,c);
  }
}
//________________________________________________________________________
float Distance(float xp,float yp,float zp,float x0, float y0, float z0, float teta,float phi)
{
  float r2,cos2,sin2,pr;
  float x = xp-x0;
  float y = yp-y0;
  float z = zp-z0;

  r2=x*x+y*y+z*z;
  pr=x*TMath::Sin(teta)*TMath::Cos(phi)+y*TMath::Sin(teta)*TMath::Sin(phi)+z*TMath::Cos(teta);
  if (r2==0.0) {return (0.0);}
  cos2=pr*pr/r2;
  sin2=1-cos2;

//    printf("r2=%lf \n",r2*sin2);

  return TMath::Sqrt(r2*sin2);
}
//_______________________________________________________________________
float Distance(float xp, float yp, float zp, track tr)
{
	float xt = tr.x;
	float yt = tr.y;
	float zt = tr.z;
	float teta, phi;

	TrackAngle(tr,&teta,&phi);
	return Distance(xp,yp,zp,xt,yt,zt,teta,phi);
}
//_______________________________________________________________________

float DistanceInNormalizeSpace(float xp, float yp, float zp, track tr, double dx, double dy, double dz)
{
	float xpNorm = xp/dx;
	float ypNorm = yp/dy;
	float zpNorm = zp/dz;
	track trNorm;
	trNorm.x = tr.x/dx;
	trNorm.y = tr.y/dy;
	trNorm.z = tr.z/dz;
	trNorm.tgx = tr.tgx * dz/dx;
	trNorm.tgy = tr.tgy * dz/dy;
	return Distance(xpNorm,ypNorm,zpNorm,trNorm);
}
//_______________________________________________________________________
/*
float DistanceInNormalizeSpace(float xp,float yp,float zp,float x0, float y0, float z0, float teta,float phi, double dx, double dy, double dz)
{
	float xpNorm = xp/dx;
	float ypNorm = yp/dy;
	float zpNorm = zp/dz;
	float x0Norm = x0/dx;
	float y0Norm = y0/dy;
	float z0Norm = z0/dz;

	track trNorm;
	trNorm.x = tr.x/dx;
	trNorm.y = tr.y/dy;
	trNorm.z = tr.z/dz;
	trNorm.tgx = tr.tgx * dz/dx;
	trNorm.tgy = tr.tgy * dz/dy;
	return Distance(xpNorm,ypNorm,zpNorm,trNorm);
}
*/
//_____________________________________________________________________________________

int SatCondAccept(track *tr, float xc, float yc, float zc, double dx, double dy, double dz, float accept)
{
/*	float xt = tr->x;
	float yt = tr->y;
	float zt = tr->z;
	float tgx = tr->tgx;
	float tgy = tr->tgy;

	float x = xt + (zc-zt)*tgx;
	float y = yt + (zc-zt)*tgy;
	
	float teta, phi;
	TrackAngle(*tr,&teta,&phi);
*/
/*	if (Distance(xc,yc,zc,xt,yt,zt,teta,phi)<=accept) return true;
	else return false;
*/
	if (DistanceInNormalizeSpace(xc,yc,zc,*tr,dx,dy,dz)<=accept) return true;
	else return false;

/*	if (sqrt((x-xc)*(x-xc)+(y-yc)*(y-yc))<=accept) return true;
	else return false;
	*/
}
//___________________________________________________________________________
//___________________________________________________________________________

int MY_LFIT( float *X, float *Y, int L, int KEY, float &A, float &B,float &erA, float &erB, float &s)
{

  A=0; B=0; erA=0; erB=0; s=0;

  //     CALCULTE SUMS
  if(L<2)  return false;

  double Ex=0.;
  double Ey=0.;
  double Sxx=0.;
  double Syy=0.;
  double Syx=0.;
  int    ICNT=0;
  double SSE;
  int j;

  for(j=0; j<L; j++ ){
    if( Y[j]==0. && KEY==0 )  continue;
    Ex+=X[j];
    Ey+=Y[j];
    ICNT++;
  }

  if(ICNT<3)     return false;

  Ex=Ex/(double)ICNT;
  Ey=Ey/(double)ICNT;

  for(j=0; j<L; j++ ){
    if( Y[j]==0. && KEY==0 )  continue;
    Syx+=(Y[j]-Ey)*(X[j]-Ex);
    Sxx+=(X[j]-Ex)*(X[j]-Ex);
    Syy+=(Y[j]-Ey)*(Y[j]-Ey);
 }

  if(Sxx==0.) return false;


  A=Syx/Sxx;
  B=Ey-A*Ex;

  SSE=Syy-A*Syx;

  s=sqrt(SSE/(double)(ICNT-2));
  
  erA=s/sqrt(Sxx);
  erB=s*sqrt(1./(double)ICNT+Ex*Ex/Sxx);

  //  printf("MYTEST!!!!!!!!!!! A= %lf , erA= %lf , B= %lf , erB= %lf \n",A,erA,B,erB);

 
  return true;
}

//________________________________________________________________________

TClonesArray* Smoothing(EdbView *v)
{
	TClonesArray *t = new TClonesArray("EdbFrame");
	EdbFrame *fpp=0,*fpred=0,*f=0,*fsucc=0,*fss=0;
	int nFrame = v->GetNframes();
	for (int i=0; i<nFrame; i++) {
		int Nclpp,Nclp,Ncl,Ncls,Nclss;
		if (i>1) {
			fpp = (EdbFrame *)(v->GetFrames()->At(i-2));
			Nclpp = fpp->GetNcl();
		}
		else Nclpp = 0;
		if (i>0) {
			fpred = (EdbFrame *)(v->GetFrames()->At(i-1));
			Nclp = fpred->GetNcl();
		}
		else Nclp = 0;
		f = (EdbFrame *)(v->GetFrames()->At(i));
		Ncl = f->GetNcl();
		if (i<nFrame-1) {
			fsucc = (EdbFrame *)(v->GetFrames()->At(i+1));
			Ncls = fsucc->GetNcl();
		}
		else Ncls = 0;
		if (i<nFrame-2) {
			fss = (EdbFrame *)(v->GetFrames()->At(i+2));
			Nclss = fss->GetNcl();
		}
		else Nclss = 0;
		int ncl = (Nclpp+2*Nclp+3*Ncl+2*Ncls+Nclss)/9;
		new((*t)[i]) EdbFrame(f->GetID(),f->GetZ(),ncl,f->GetNpix());
//		h2->Fill(f->GetZ(),ncl);
//		t->Add(new EdbFrame(f->GetID(),f->GetZ(),ncl,f->GetNpix()));
	}
	return t;
}
//________________________________________________________________________

float min(TClonesArray *t) {

int num = t->GetEntries();
//cout<<num<<endl;
float d,z;
float dmin = ((Deriv *)(t->At(0)))->GetD();
float zmin = ((Deriv *)(t->At(0)))->GetZ();
//cout<<" "<<zmin<<" "<<dmin<<endl;
for (int i=1; i<num; i++) {
    d=((Deriv *)(t->At(i)))->GetD();
    z=((Deriv *)(t->At(i)))->GetZ();
//    cout<<" "<<z<<" "<<d<<" "<<endl;
    if (d<dmin) {
	zmin=z;
	dmin=d;
    }
}
//cout<<endl;
return zmin;
}

//________________________________________________________________
float max(TClonesArray *t) {

int num = t->GetEntries();
float d,z;
float dmax = ((Deriv *)(t->At(0)))->GetD();
float zmax = ((Deriv *)(t->At(0)))->GetZ();
for (int i=1; i<num; i++) {
    d=((Deriv *)(t->At(i)))->GetD();
    z=((Deriv *)(t->At(i)))->GetZ();
    if (d>dmax) {
	zmax=z;
	dmax=d;
    }
}
return zmax;
}
//________________________________________________________________________

void PreciseBasePosition(float zmin, float zmax, TClonesArray *dc, float &zup, float &zdown)
{
	TH1F *h1min = new TH1F("h1min","h1min",20,zmin-5.,zmin+5.);
	TH1F *h1max = new TH1F("h1max","h1max",20,zmax-5.,zmax+5.);
	float z,d;
	Deriv *D=0;
	int N=dc->GetEntries();
	for (int i=0; i<N; i++) {
		D=(Deriv *)(dc->At(i));
		z=D->GetZ();
		d=D->GetD();
		if (TMath::Abs(z-zmin)<5.) h1min->Fill(z,d);
		if (TMath::Abs(z-zmax)<5.) h1max->Fill(z,d);
	}
//	h1min->Fit("gaus","Q0");
//	h1max->Fit("gaus","Q0");
//	TF1 *fmin = h1min->GetFunction("gaus");
//	TF1 *fmax = h1max->GetFunction("gaus");
//	error->Fill(fmin->GetParameter(2));
//	error->Fill(fmax->GetParameter(2));
//	zdown = fmax->GetParameter(1);
//	zup = fmin->GetParameter(1);
	zdown = h1max->GetMean();
	zup = h1min->GetMean();
//	error->Fill(h1max->GetRMS());
//	error->Fill(h1min->GetRMS());
	delete h1min;
	delete h1max;
}
//________________________________________________________________________
//========================================================================
track::track()
{

}
//________________________________________________________________________
track::track(double tx, double ty, double xx, double yy, double zz, int h)
{
	tgx=tx;
	tgy=ty;
	x=xx;
	y=yy;
	z=zz;
	height = h;
}
//________________________________________________________________________
track::~track()
{
}
//________________________________________________________________________
//========================================================================
//________________________________________________________________________
TBitView::TBitView()
{
	ZBase=0.;
	delta = 1.6;
//	NumberOfLayers=0;
	TBitArray = new TObjArray();
}

//________________________________________________________________________
TBitView::TBitView(float z, float bs)
{
	ZBase = z;
	delta = 1.6;
//	NumberOfLayers = 0;
	bitSize = bs;
	TBitArray = new TObjArray();
}
//________________________________________________________________________
TBitView::~TBitView()
{
	if (TBitArray) {
		TBitArray->Delete();
		delete TBitArray;
	}
//	NumberOfLayers=0;
}
//________________________________________________________________________
void TBitView::AddNewLayer(TBitMatrix *TLayer)
{
	TBitArray->Add(TLayer);
	//new((*TBitArray)[TBitArray->GetEntries]) TBitMatrix(*TLayer);
//	NumberOfLayers = TBitArray.GetEntries();
}
//________________________________________________________________________
TBitMatrix* TBitView::GetLayer(int n)
{
	if (n<TBitArray->GetEntries()) return (TBitMatrix *)TBitArray->At(n);
	else return NULL;
}
//________________________________________________________________________
void TBitView::FillBitView(EdbView *v, float sx, float sy, float Xmin, float Xmax, float Ymin, float Ymax, int inside, int rep)
{
	int i;
	long xs = (long)((Xmax-Xmin)/bitSize +1);
	long ys = (long)((Ymax-Ymin)/bitSize +1);
// added at 31.01.2005 //////////////////////////////////////////////
	{
		EdbFrame *f=0;
		EdbFrame *fpred=0;
		EdbFrame *fsuc=0;
		TClonesArray *dc = new TClonesArray("Deriv");
		Deriv *d=new Deriv();
		float zmin,zmax;

		TClonesArray* t = Smoothing(v);
		int nFrame = t->GetEntries();
//    printf(" View: %d of %d   \n",j+jj,nEntries);
		for (int ifr=0; ifr<nFrame; ifr++) {
//	printf(" Field:%d of %d;   ",ifr,nFrame);
			f = (EdbFrame *)(t->At(ifr));
			if (ifr==0) fpred=NULL;
			else fpred = (EdbFrame *)(t->At(ifr-1));
			if (ifr==nFrame-1) fsuc=NULL;
			else fsuc = (EdbFrame *)(t->At(ifr+1));
//	d = new Deriv();
//	printf(" deriv=%f\n",
			d->derivate(fpred,f,fsuc);
//		d2->Fill(d->GetZ(),d->GetD());
//        tree->Fill();
//	dc->Add(d);
//	fc->Add(f);
//	delete d;
			new((*dc)[ifr]) Deriv(*d);
//		new((*fc)[ifr]) EdbFrame(*f);
    	}
		zmin=min(dc);
		zmax=max(dc);
		if (v->GetNframesTop()) {
			PreciseBasePosition(zmin,zmax,dc,Z1,Z2);
			if ((Z1-Z2)<30||(Z1-Z2)>60) {
				printf("!!!!!!!! dztop=%f, z1=%f, z2=%f\n",Z1-Z2,Z1,Z2);
				Z1 = v->GetZ1();
				Z2 = v->GetZ2();
			}
		}
		else {
			PreciseBasePosition(zmin,zmax,dc,Z3,Z4);
			if ((Z3-Z4)<30||(Z3-Z4)>60) {
				printf("!!!!!!!! dzbot=%f, z3=%f, z4=%f\n",Z3-Z4,Z3,Z4);
				Z3 = v->GetZ3();
				Z4 = v->GetZ4();
			}
		}
//	printf("zmin=%f, zmax=%f, dif=%f \n",zmin,zmax,zmax-zmin);
//    tree->Fill();
		dc->Delete();
		t->Delete();
		delete d;
		delete dc;
		delete t;
	}
/////////////////////////////////////////////////////////////////////////////// 31.01.2005
	TClonesArray* frames = v->GetFrames();
	//	TClonesArray* clusters = v->GetClusters();
	EdbFrame* f = 0;
	EdbCluster* c = 0;
	int nframes = v->GetNframes();
	int nclusters = v->Nclusters();
//	int NumberOfLayers = nframes;
	float zmin = ((EdbFrame *)(frames->At(0)))->GetZ();
	float zmax = zmin;
	for (i=0; i<nframes; i=i+rep) {
		f = (EdbFrame *)(frames->At(i));
		if (inside)
			if (!(f->GetZ()>((v->GetNframesTop())?(Z2+delta):(Z4+delta))&&
				f->GetZ()<((v->GetNframesTop())?(Z1-delta):(Z3-delta)))) continue;
		if (zmin>f->GetZ()) zmin = f->GetZ();
		if (zmax<f->GetZ()) zmax = f->GetZ();
        TBitMatrix* TBit = new TBitMatrix(xs,ys,f->GetZ());
		TBitArray->Add(TBit);
//		delete TBit;
	}
	cout<<GetNumberOfLayers()<<endl;
	for (i=0; i<nclusters; i++) {
		int j;
		int flag = false;
		c = v->GetCluster(i);
		float xc = c->GetX();
		float yc = c->GetY();
		if (In(xc,yc,Xmin,Xmax,Ymin,Ymax)) {
			for (j=0; j<GetNumberOfLayers(); j++) {
//				cout<<c->GetZ()<<" "<<GetLayer(j)->GetZ()<<endl;
				if (c->GetZ()==GetLayer(j)->GetZ()) {
					flag = true;
					break;
				}
			}
			if (!flag) continue;
			long xi = (long)((xc-Xmin)/bitSize);
			long yi = (long)((yc-Ymin)/bitSize);
			float area = c->GetArea();
			GetLayer(j)->SetBit(xi,yi,(int)(area*sx*sy/(bitSize*bitSize)));
		}
	}
/////////////////////// not needed //////////////////
/*	cout<<"layers= "<<NumberOfLayers<<endl;
	for (long ij=0; ij<NumberOfLayers; ij++) {
//	cout<<c->GetZ()<<" "<<GetLayer(j)->GetZ()<<endl;
//	if (c->GetZ()==GetLayer(j)->GetZ()) break;
		cout<<"layer= "<<ij<<endl;
		GetLayer(ij)->Print(1);
 		_getch();
	}
*///////////////////////////////////
	ZBase = (v->GetNframesTop())?(Z2):(Z3);
/*	if (c->GetSide()) ZBase = v->GetZ3();
	else ZBase = v->GetZ2();
	*/
}
//________________________________________________________________________
//========================================================================
//========================================================================

TBinTracking::TBinTracking()
{
	TByteArray = new TObjArray();
	dX = 1.0;
	dY = 1.0;
	dZ = 1.0;
//	TTrackArray = NULL;
}
//________________________________________________________________________

TBinTracking::TBinTracking(double tgxmin, double tgxmax, double tgymin, double tgymax, double tgstep)
{
	Tgxmin = tgxmin;
	Tgxmax = tgxmax;
	Tgymin = tgymin;
	Tgymax = tgymax;
	Tgstep = tgstep;
	TByteArray = new TObjArray();
	dX = 1.0;
	dY = 1.0;
	dZ = 1.0;
//	TTrackArray = NULL;
}
//________________________________________________________________________

TBinTracking::TBinTracking(double tgxmin, double tgxmax, double tgymin, double tgymax, double tgstep, double dx, double dy, double dz)
{
	Tgxmin = tgxmin;
	Tgxmax = tgxmax;
	Tgymin = tgymin;
	Tgymax = tgymax;
	Tgstep = tgstep;
	TByteArray = new TObjArray();
	dX = dx;
	dY = dy;
	dZ = dz;
}
//________________________________________________________________________

TBinTracking::~TBinTracking()
{
	if (TByteArray) {
		TByteArray->Delete();
		delete TByteArray;
	}
/*	if (TTrackArray) {
		TTrackArray->Delete();
		delete TTrackArray;
	}*/
}
//________________________________________________________________________

TByteMatrix* TBinTracking::GetElement(int n)
{
	if (n<TByteArray->GetEntries()) return (TByteMatrix *)TByteArray->At(n);
	else return NULL;
}
//________________________________________________________________________

TByteMatrix* TBinTracking::GetElement(double tgx, double tgy)
{
	if ((tgx>=Tgxmin)&&(tgx<Tgxmax)&&(tgy>=Tgymin)&&(tgy<Tgymax)) {
		TByteMatrix *TByte = 0;
		double Tgx, Tgy;
		for (long i=0; i<GetNelements(); i++) {
			TByte = GetElement(i);
			TByte->GetAngle(&Tgx,&Tgy);
			if ((abs(Tgx-tgx)<=Tgstep)&&(abs(Tgy-tgy)<=Tgstep)) return TByte;
		}
	}
	return NULL;
}
//________________________________________________________________________

void TBinTracking::FillByteArray(TBitView *Tv)
{
	ZBase = Tv->ZBase;
	long tgxi = (long)((Tgxmax-Tgxmin)/Tgstep +1);
	long tgyi = (long)((Tgymax-Tgymin)/Tgstep +1);
	long xs, ys;
	TBitMatrix *TBit = 0;
	Tv->GetLayer(0)->GetSize(&xs,&ys);
	for (long j=0; j<tgyi; j++) {
		for (long i=0; i<tgxi; i++) {
			double tgx = Tgxmin + (i+0.5)*Tgstep;
			double tgy = Tgymin + (j+0.5)*Tgstep;
			TByteMatrix *TByte = new TByteMatrix(xs,ys,tgx,tgy);
			for (long k=0; k<Tv->GetNumberOfLayers(); k++) {
				TBit = Tv->GetLayer(k);
				long shiftX = (long)((TBit->GetZ()-ZBase)*tgx/Tv->bitSize);
				long shiftY = (long)((TBit->GetZ()-ZBase)*tgy/Tv->bitSize);
				TBit->MoveMatrix(-shiftY,-shiftX);
				*TByte += *TBit;
			}
//			TByte->Print(0);
//			_getch();
			TByteArray->Add(TByte);
//			delete TByte;
		}
	}
}
//________________________________________________________________________

TObjArray* TBinTracking::FindBinTracks(int thr)
{
	TByteMatrix* TByte = 0;
	TObjArray* Tob = new TObjArray();
	for (long i=0; i<GetNelements(); i++) {
		TByte = GetElement(i);
		double tgx, tgy;
		TByte->GetAngle(&tgx, &tgy);
		TObjArray* points = TByte->Pick8(0,thr);
		MyPoint* point = 0;
        for (long ii=0; ii<points->GetEntries(); ii++) {
			point = (MyPoint *)(points->At(ii));
			Tob->Add(new track(tgx, tgy, point->x, point->y, ZBase,point->height));
////////////////// not needed ////////////////////
//			TByte->Print(0);
/////////////////////////////////////////////////
		}
		delete points;
	}
	return Tob;
}
//________________________________________________________________________
void TBinTracking::ImproveTracksArray(TObjArray *tracks, int thr, float accept)
{
	track* tr = 0;
	track* tr1 = 0;

	int xn[8] = { 1, -1, 0,  0, 1,  1, -1, -1 }; //pixel 3x3 suburbs
	int yn[8] = { 0,  0, 1, -1, 1, -1,  1, -1 };

	long nc = (long)((Tgymax-Tgymin)/Tgstep);
	long nr = (long)((Tgxmax-Tgxmin)/Tgstep);
	
	int pic=0,pic1=0;

//	printf("hist: %d %d\n",nc,nr);
//  int npeaks=0;
	int ic,ir,in;           // declare here to fix marazmatic eror messages of VC++
	for(ic=0; ic<nc; ic++) {
		for(ir=0; ir<nr; ir++) {
			float tgx0 = Tgxmin + ir*Tgstep;
			float tgx1 = Tgxmin + (ir+1)*Tgstep;
			float tgy0 = Tgymin + ic*Tgstep;
			float tgy1 = Tgymin + (ic+1)*Tgstep;
			int nmax = -1;
			for (int i=0; i<tracks->GetEntries(); i++) {
			  if ( (tr = (track *)(tracks->At(i))) )
				if (In(tr->tgx,tr->tgy,tgx0,tgx1,tgy0,tgy1)) {
					nmax = i;
					pic = tr->height;
					pic1 = pic+1;
					break;
				}
			}
			if (nmax==-1) continue;
			if (pic<thr) {
				delete (track *)(tracks->RemoveAt(nmax));
				tracks->Compress();
				continue;
			}
//			tr->height = pic1;
//			pic = GetByte(type,ic,ir); 
//			if( pix < thr)   goto NEXT;
//			pix1=pix+1;
			int n = 0;
			int nn[8];
			for(in=0; in<8; in++) {
				int icc = ic+xn[in];
				int irr = ir+yn[in];
				tgx0 = Tgxmin + irr*Tgstep;
				tgx1 = Tgxmin + (irr+1)*Tgstep;
				tgy0 = Tgymin + icc*Tgstep;
				tgy1 = Tgymin + (icc+1)*Tgstep;
				for (int i=0; i<tracks->GetEntries(); i++) {
					if ((tr1 = (track *)(tracks->At(i))) == tr) continue;
					if (In(tr1->tgx,tr1->tgy,tgx0,tgx1,tgy0,tgy1)&&
						(fabs(tr->x-tr1->x)<accept&&fabs(tr->y-tr1->y)<accept)) {
						if (pic1<=tr1->height) goto NEXT;
						nn[n] = i;
						n++;
					}
				}
			}
			if (n) {
//printf("         %d \n",n);
				for (int i=0; i<n; i++) delete (track *)(tracks->RemoveAt(nn[i]));
				tr->height = pic1;
				tracks->Compress();
			}
//				pp = GetByte(type, ic+xn[in], ir+yn[in] );
//      			if( pix1 <= pp ) goto NEXT;
//				SetByte(type,ic,ir,pix1);
//		printf("peak8(%d,%d) =  %d\n",ic,ir,pix); 
//		Ta->Add(new MyPoint(ic,ir,pix));
NEXT:
		continue;
    }
  }
}
//________________________________________________________________________

EdbView* TBinTracking::AdoptSegment(EdbView* view, float cfx, float cfy, float Xmin, float Xmax, float Ymin, float Ymax, float bs,
		int thr, float accept, int inside, int excludeCommonClusters, int rep)
{
	EdbView *vmod =	view;		
//	EdbView *vmod = new EdbView(*view);
///////////////////////
//	TClonesArray *SAr = new TClonesArray("EdbSegment");
	TObjArray *SAr = new TObjArray();
	TBitView* Tv = new TBitView();
	Tv->bitSize = bs;
	Tv->FillBitView(vmod,cfx,cfy,Xmin,Xmax,Ymin,Ymax,inside,rep);
	FillByteArray(Tv);
	TObjArray* tracks = FindBinTracks(thr);
/////////////////// testing ////////////////////
	ImproveTracksArray(tracks,thr,bs);		// was accept instead of bs
////////////////////////////////////////////////
	float X[200], Y[200] ,Z[200];
	int ntracks = tracks->GetEntries();
	track* tr = 0;
	EdbCluster* cl = 0;
	int nclusters = vmod->Nclusters();
	int sid = 0;
	int cyclelimit = 10;
//	cout<<"tracks="<<ntracks<<endl;
	for (long t=0; t<ntracks; t++) {
		tr = (track *)(tracks->At(t));
		tr->x = tr->x * bs + Xmin;
		tr->y = tr->y * bs + Ymin;
		EdbSegment* s = new EdbSegment();
		float resinit = 0;
		int pulsinit = 0;
		int cyc = 0;
CYCLE:
		cyc++;
		int n = 0;
/////////////////////// added at 23.09.2004 //////////////////////////////
/*		float teta,phi;
		TrackAngle(*tr,&teta,&phi);
*/////////////////////////////////////////////////////////////////////////
		for (long i=0; i<nclusters; i++) {
			int flag = false;
			cl = vmod->GetCluster(i);
			float xc = cl->GetX();
			float yc = cl->GetY();
			float zc = cl->GetZ();
			if (In(xc,yc,Xmin,Xmax,Ymin,Ymax)&&SatCondAccept(tr,xc,yc,zc,dX,dY,dZ,accept)) {
/*				(!(inside)||
				((zc>(view->GetNframesTop()?Tv->GetZ2():Tv->GetZ4()))
				&&(zc<(view->GetNframesTop()?Tv->GetZ1():Tv->GetZ3()))))) {
*/
				//printf("distance=%f\n",Distance(cl->GetX(),	cl->GetY(),cl->GetZ(),*tr));
				//if (rep!=1) {
				if (inside) {
					for (int j=0; j<Tv->GetNumberOfLayers(); j++) {
//				cout<<c->GetZ()<<" "<<GetLayer(j)->GetZ()<<endl;
						if (zc==Tv->GetLayer(j)->GetZ()) {
							flag = true;
							break;
						}
					}
					if (!flag) continue;
				}
/////////////////////// added at 23.09.2004 //////////////////////////////
				if (int last = s->GetNelements()) {
					EdbCluster *clsucc = ((EdbCluster *)(s->GetElements()->Last()));
					if (zc==clsucc->GetZ()) {
						if (Distance(clsucc->GetX(),
									clsucc->GetY(),
									clsucc->GetZ(),
									*tr) <= Distance(xc,yc,zc,*tr)) continue;
						else {
							(*(s->GetElements()))[last-1] = cl;
							X[n-1] = xc;
							Y[n-1] = yc;
							Z[n-1] = zc;
							continue;
						}
					}
				}
///////////////////////////////////////////////////////////////////////////
				s->AddElement(cl);
				X[n] = xc;
				Y[n] = yc;
				Z[n] = zc;
				n++;
			}
		}
		float Ax,Bx,erAx,erBx,sx,Ay,By,erAy,erBy,sy;;
		int flagx = MY_LFIT(Z,X,n,1,Ax,Bx,erAx,erBx,sx);
		int flagy = MY_LFIT(Z,Y,n,1,Ay,By,erAy,erBy,sy);
		if (flagx&&flagy) {
			int puls = s->GetNelements();
			int side = ((EdbCluster *)(s->GetElements()->At(0)))->GetSide();
			float maxz = ((EdbCluster *)(s->GetElements()->At(0)))->GetZ();
			float minz = maxz;
			for (long j=0; j<puls; j++) {
				cl = (EdbCluster *)(s->GetElements()->At(j));
				if (minz>=cl->GetZ()) minz = cl->GetZ();
				if (maxz<cl->GetZ()) maxz = cl->GetZ();
			}
			float res = sqrt((sx*sx)+(sy*sy));
/////////
			if (((pulsinit!=puls)||(res!=resinit))&&cyc<cyclelimit) {
				pulsinit = puls;
				resinit = res;
				float zl =	ZBase;			//side?maxz:minz;
				tr->tgx = Ax;
				tr->tgy = Ay;
				tr->z = zl;
				tr->x = Ax*zl+Bx;
				tr->y = Ay*zl+By;
				s->GetElements()->Clear();
				goto CYCLE;
			}
//////////
//			cout<<"cycle="<<cyc<<endl;
			float dz = maxz-minz;
			s->Set(tr->x,tr->y,tr->z,Ax,Ay,dz,side,puls,sid);
//////////// not needed ////////////////////////
/*			for (int nclust=0; nclust<puls; nclust++) {
				cl = (EdbCluster *)(s->GetElements()->At(nclust));
				printf("< clnum=%d ", nclust);
				cl->Print();
			}
*////////////////////////////////////////////////
//			new ((*SAr)[sid]) EdbSegment(*s);
			SAr->Add(s);
			sid++;
		}
//		delete s;
	}
	delete Tv;
	delete tracks;

//////////////// analizing of the same Segments //////////////////////
	if (sid==0) return vmod;			//SAr;
//	int newsid=sid;
	EdbSegment* seg0 = 0;
	EdbSegment* seg = 0;
	for (int s0=0; s0<sid-1; s0++) {
//REPEAT:
		seg0 = (EdbSegment *)(SAr->At(s0));
		int newid=seg0->GetID();
		int numofdel = 0;
		int rep = 0;

		for (int s=s0+1; s<sid; s++) {
			seg = (EdbSegment *)(SAr->At(s));
/*			if ((seg->GetX0()==seg0->GetX0())&&(seg->GetY0()==seg0->GetY0())&&
				(seg->GetZ0()==seg0->GetZ0())&&
				(seg->GetTx()==seg0->GetTx())&&(seg->GetTy()==seg0->GetTy())&&
				(seg->GetDz()==seg0->GetDz())&&(seg->GetPuls()==seg0->GetPuls())) {
*/
			if (((fabs(seg->GetX0()-seg0->GetX0())<bs&&fabs(seg->GetY0()-seg0->GetY0())<bs)&&
//				(seg->GetZ0()==seg0->GetZ0())&&
				abs(seg->GetTx()-seg0->GetTx())<Tgstep&&abs(seg->GetTy()-seg0->GetTy())<Tgstep)
//				&&(seg->GetDz()==seg0->GetDz())&&(seg->GetPuls()==seg0->GetPuls())
// added at 13.11.2004 ///////////////////////////////
				||(excludeCommonClusters
				&&CommonClusters(seg0,seg))
/////////////////////////////////////////////////////
				)
			{
////////////////// added at 12.11.2004 ///////////////////
				if (seg->GetPuls()>seg0->GetPuls()) {
					(*(SAr))[s] = seg0;
					(*(SAr))[s0] = seg;
					((EdbSegment *)(SAr->At(s0)))->SetID(seg0->GetID());
					seg0 = seg;
					rep=1;
				}
/////////////////////////////////////////////////////////
				delete (SAr->RemoveAt(s));
				numofdel++;
//				sid--;
			}
			else {
				newid++;
				seg->SetID(newid);
//				seg0 = seg;
			}
		}
		SAr->Compress();
//		printf("sid=%d, del=%d\n",sid,numofdel);
		sid -= numofdel;
		if (rep) s0--;
	}
////////////////////////////////////////////////////////////////////
/////////////////// modifided view ////////////////////////////////

	vmod->GetHeader()->SetNsegments(sid);
//	printf("sid=%d\n",sid);
//	EdbSegment* seg = 0;
	for (int ss=0; ss<sid; ss++) {
		seg = (EdbSegment *)(SAr->At(ss));
		EdbSegment *segnew = new EdbSegment(
			seg->GetX0(),
			seg->GetY0(),
			seg->GetZ0(),
			seg->GetTx(),
			seg->GetTy(),
			seg->GetDz(),
			seg->GetSide(),
			seg->GetPuls(),
			seg->GetID());
		vmod->AddSegment(segnew);
		delete segnew;
		seg0 = ((EdbSegment *)(vmod->GetSegments()->At(ss)));
//		cout<<seg->GetNelements()<<" "<<seg->GetID()<<endl;
		for (int cc=0; cc<seg->GetNelements(); cc++) {
			cl = (EdbCluster *)(seg->GetElements()->At(cc));
			seg0->AddElement(cl);
			cl->SetSegment(seg->GetID());
		}
//		vmod->AddSegment(seg);
	}
    SAr->Delete();
	delete SAr;
///////////////////////////////////////////////////////////////////
	return vmod;
}
//________________________________________________________________________

TH2F* TBinTracking::Histo(double tgx,double tgy)
{
	TByteMatrix *tbyte = 0;
	if ((tbyte = GetElement(tgx,tgy)) == NULL) return 0;
	long xs, ys;
	tbyte->GetSize(&xs,&ys);
	printf("xs=%ld, ys=%ld\n",xs,ys);
	TH2F* h2 = new TH2F("h2","h2",xs,0.,xs,ys,0., ys);
	for (long j=0; j<ys; j++) {
		for (long i=0; i<xs; i++) {
			h2->Fill(i+.5,j+.5,tbyte->GetByte(0,i,j));
		}
	}
	return h2;
}
