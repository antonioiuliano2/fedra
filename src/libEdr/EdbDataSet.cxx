///-- Author :  Valeri Tioukov   9.06.2003

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbDataSet                                                           //
//                                                                      //
// scanned raw data set and correspondent parameters                    //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TROOT.h"
#include "EdbDataSet.h"
#include "EdbSegment.h"
#include "EdbCluster.h"

ClassImp(EdbSegmentCut)
ClassImp(EdbLayer)
ClassImp(EdbDataPiece)
ClassImp(EdbDataSet)
ClassImp(EdbDataProc)

///______________________________________________________________________________
EdbSegmentCut::EdbSegmentCut(int xi, float var[10])
{
  eXI=xi; 
  for(int i=0;i<5;i++) {
    eMin[i]=var[i*2]; 
    eMax[i]=var[i*2+1];
  }
}

///______________________________________________________________________________
int EdbSegmentCut::PassCut(float var[5])
{
  if     (eXI==0)  return PassCutX(var);
  else if(eXI==1)  return PassCutI(var);
  return 0;
}

///______________________________________________________________________________
int EdbSegmentCut::PassCutX(float var[5])
{
  // exclusive cut: if var is inside cut volume - return 0

  for(int i=0; i<5; i++) {
    if(var[i]<eMin[i])  return 1;
    if(var[i]>eMax[i])  return 1;
  }
  return 0;
}

///______________________________________________________________________________
const char *EdbSegmentCut::CutLine(char *str, int i, int j) const
{
  if(eXI==0) {

    sprintf(str,
	  "  TCut x%1d%2.2d = \"!(eX0>%f&&eX0<%f && eY0>%f&&eY0<%f && eTx>%f&&eTx<%f && eTy>%f&&eTy<%f && ePuls>%f&&ePuls<%f)\";\n"
	  ,i,j,eMin[0],eMax[0], eMin[1],eMax[1], eMin[2],eMax[2], eMin[3],eMax[3], eMin[4],eMax[4]);

  } else if(eXI==1) {

    sprintf(str,
	  "  TCut i%1d%2.2d = \"(eX0>%f&&eX0<%f && eY0>%f&&eY0<%f && eTx>%f&&eTx<%f && eTy>%f&&eTy<%f && ePuls>%f&&ePuls<%f)\";\n"
	  ,i,j,eMin[0],eMax[0],eMin[1],eMax[1],eMin[2],eMax[2],eMin[3],eMax[3],eMin[4],eMax[4]);
  }
  return str;
}

///______________________________________________________________________________
int EdbSegmentCut::PassCutI(float var[5])
{
  // inclusive cut: if var is inside cut volume - return 1

  for(int i=0; i<5; i++) {
    if(var[i]<eMin[i])  return 0;
    if(var[i]>eMax[i])  return 0;
  }
  return 1;
}

///______________________________________________________________________________
void EdbSegmentCut::Print()
{
}

///==============================================================================
void EdbLayer::Print()
{
  printf("Layer:\t %d\n",eID);
  printf("ZLAYER\t %f %f %f\n",eZ,eZmin,eZmax);
  printf("SHR\t %f\n",eShr);
  printf("AFFXY\t");        eAffXY.Print();
  printf("AFFTXTY\t");      eAffTXTY.Print();
}

///==============================================================================
EdbLayer::EdbLayer()
{
  eID=0;
  eZ=eZmin=eZmax=0;
  eX=eY=eTX=eTY=0;
  eDX=eDY=kMaxInt;
  eShr = 1.;
}

///==============================================================================
EdbDataPiece::EdbDataPiece()
{
  for(int i=0; i<3; i++) eLayers[i]=0;
  Set0();
}

///______________________________________________________________________________
EdbDataPiece::EdbDataPiece(int plate, int piece, char* file, int flag)
{
  for(int i=0; i<3; i++) eLayers[i]=0;
  Set0();
  ePlate=plate;
  ePiece=piece;
  AddRunFile(file);
  eFlag=flag;
}

///______________________________________________________________________________
EdbDataPiece::~EdbDataPiece()
{
  int i;
  for(i=0; i<3; i++)  if(eLayers[i])   delete eLayers[i];
  for(i=0; i<3; i++)  if(eAreas[i])    delete eAreas[i];
  for(i=0; i<3; i++)  if(eCond[i])     delete eCond[i];
  for(i=0; i<3; i++)  if(eCuts[i])     delete eCuts[i];
  if(   eCouplesInd)                   delete eCouplesInd;

}

///______________________________________________________________________________
void EdbDataPiece::Set0()
{
  ePlate=0;
  ePiece=0;
  eFlag=0;
  eAFID=0;
  eCLUST=0;
  eCutCP[0]=-1;
  int i;
  for(i=0; i<3; i++) { 
    if(eLayers[i]) delete eLayers[i]; 
    eLayers[i]=new EdbLayer();
  }
  for(i=0; i<3; i++) eCond[i]=0;
  for(i=0; i<3; i++) eAreas[i]=0;
  for(i=0; i<3; i++) eCuts[i]=0;

  eRun    = 0;
  eCouplesTree=0;
  eCouplesInd=0;
}

///______________________________________________________________________________
int EdbDataPiece::InitCouplesInd()
{
  if(eCouplesInd) delete eCouplesInd;
  eCouplesInd = new TIndexCell();

  if(!eCouplesTree) if(!InitCouplesTree("READ")) return 0;
  EdbSegP         *s1  = 0;
  EdbSegP         *s2  = 0;
  eCouplesTree->SetBranchAddress("s1."  , &s1  );
  eCouplesTree->SetBranchAddress("s2."  , &s2  );

  Long_t v[5]; // side:aid:vid:sid:entry

  int nentr = (int)(eCouplesTree->GetEntries());

  for(int i=0; i<nentr; i++) {
    eCouplesTree->GetEntry(i);
    v[0] = 1;
    v[1] = s1->Aid(0);
    v[2] = s1->Aid(1);
    v[3] = s1->Vid(1);
    v[4] = i;
    eCouplesInd->Add(5,v);
    v[0] = 2;
    v[1] = s2->Aid(0);
    v[2] = s2->Aid(1);
    v[3] = s2->Vid(1);
    v[4] = i;
    eCouplesInd->Add(5,v);
  }
  eCouplesInd->Sort();
  return eCouplesInd->N();
}

///______________________________________________________________________________
int EdbDataPiece::GetLinkedSegEntr(int side, int aid, int vid, int sid, TArrayI &entr) const
{
  if(!eCouplesInd) return 0;
  Long_t v[4];
  v[0]=side;  v[1]=aid;  v[2]=vid;  v[3]=sid;
  TIndexCell *c = eCouplesInd->Find(4,v);
  if(!c) return 0;
  int n = c->N();
  entr.Set(n);
  for(int i=0; i<n; i++) entr.AddAt( (int)(c->At(i)->Value()), i);
  return n;
}

///______________________________________________________________________________
void EdbDataPiece::CloseRun()
{
  if(eRun) delete eRun;
  eRun=0;
}

///______________________________________________________________________________
void EdbDataPiece::Print()
{
  printf("Piece: %s\n",GetName());
  printf("%d %d \n", ePlate,ePiece);
  int i;
  for(i=0; i<eRunFiles.GetEntries(); i++)
    printf("%s\n",GetRunFile(i));
  for(i=0; i<3; i++)  if(eLayers[i])  eLayers[i]->Print();
  for(i=0; i<3; i++)  if(eCond[i])    eCond[i]->Print();
  for(i=0; i<3; i++)
    if(eCuts[i])
      for(int j=0; j<NCuts(i); j++)  GetCut(i,j)->Print();
}

///______________________________________________________________________________
void EdbDataPiece::AddRunFile( const char *name )
{
  TObjString *str = new TObjString(name);
  eRunFiles.Add(str);
}

///______________________________________________________________________________
const char *EdbDataPiece::GetRunFile( int i ) const
{
  if(eRunFiles.GetEntries()<i+1) return 0;
  return ((TObjString *)eRunFiles.At(i))->GetName();
}

///______________________________________________________________________________
const char *EdbDataPiece::MakeName()
{
  char name[8];
  sprintf(name,"%2.2d_%3.3d", ePlate,ePiece);
  SetName(name);
  return GetName();
}

///______________________________________________________________________________
const char *EdbDataPiece::MakeNameCP(const char *dir)
{
  eFileNameCP=dir;
  eFileNameCP+=MakeName();
  eFileNameCP+=".cp.root";
  return eFileNameCP.Data();
}

///______________________________________________________________________________
EdbLayer *EdbDataPiece::GetMakeLayer(int id)
{
  if(id<0) return 0;
  if(id>2) return 0;
  if(!GetLayer(id))  eLayers[id] = new EdbLayer();
  return GetLayer(id);
}

///______________________________________________________________________________
void EdbDataPiece::AddSegmentCut(int layer, int xi, float var[10])
{
  if(!eCuts[layer])  eCuts[layer] = new TObjArray();
  eCuts[layer]->Add( new EdbSegmentCut(xi,var) );
}

///______________________________________________________________________________
void EdbDataPiece::AddCutCP(float var[6])
{
  for(int i=0; i<6; i++) eCutCP[i]=var[i];
}

///______________________________________________________________________________
int EdbDataPiece::NCuts(int layer)
{
  if(!eCuts[layer])  return 0;
  return eCuts[layer]->GetEntries();
}

///______________________________________________________________________________
EdbScanCond *EdbDataPiece::GetMakeCond(int id)
{
  if(id<0) return 0;
  if(id>2) return 0;
  if(!GetCond(id))  eCond[id] = new EdbScanCond();
  return GetCond(id);
}


///______________________________________________________________________________
void EdbDataPiece::MakeNamePar(const char *dir)
{
  eFileNamePar=dir;
  eFileNamePar += MakeName();
  eFileNamePar += ".par";
}

///______________________________________________________________________________
int EdbDataPiece::TakePiecePar()
{
  return ReadPiecePar( eFileNamePar.Data() );
}

///______________________________________________________________________________
int EdbDataPiece::ReadPiecePar(const char *file)
{
  char            buf[256];
  char            key[256];
  char            name[256];

  FILE *fp=fopen(file,"r");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return(-1);
  }else
    printf( "\nRead piece parameters from file: %s\n\n", file );

  int id;
  float z,zmin,zmax,shr;
  float a11,a12,a21,a22,b1,b2;
  float x1,x2,x3,x4;
  float var[10];

  while( fgets(buf,256,fp)!=NULL ) {

    for( int i=0; i<(int)strlen(buf); i++ ) 
      if( buf[i]=='#' )  {
	buf[i]='\0';                       // cut out comments starting from #
	break;
      }
    
   if( sscanf(buf,"%s",key)!=1 )                             continue;

   if      ( !strcmp(key,"INCLUDE")   )
      {
	sscanf(buf+strlen(key),"%s",name);
	ReadPiecePar(name);
      }
   else if ( !strcmp(key,"ZLAYER")   )
      {
	sscanf(buf+strlen(key),"%d %f %f %f",&id,&z,&zmin,&zmax);
	GetMakeLayer(id)->SetZlayer(z,zmin,zmax);
      }
    else if ( !strcmp(key,"SHRINK")  )
      {
	sscanf(buf+strlen(key),"%d %f",&id,&shr);
	GetMakeLayer(id)->SetShrinkage(shr);
      }
    else if ( !strcmp(key,"AFFXY")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f %f %f",&id,&a11,&a12,&a21,&a22,&b1,&b2);
	GetMakeLayer(id)->SetAffXY(a11,a12,a21,a22,b1,b2);
      }
    else if ( !strcmp(key,"AFFTXTY")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f %f %f",&id,&a11,&a12,&a21,&a22,&b1,&b2);
	GetMakeLayer(id)->SetAffTXTY(a11,a12,a21,a22,b1,b2);
      }
    else if ( !strcmp(key,"SIGMA0")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f",&id,&x1,&x2,&x3,&x4);
	GetMakeCond(id)->SetSigma0(x1,x2,x3,x4);
      }
    else if ( !strcmp(key,"DEGRAD")  )
      {
	sscanf(buf+strlen(key),"%d %f",&id,&x1);
	GetMakeCond(id)->SetDegrad(x1);
      }
    else if ( !strcmp(key,"BINS")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f",&id,&x1,&x2,&x3,&x4);
	GetMakeCond(id)->SetBins(x1,x2,x3,x4);
      }
    else if ( !strcmp(key,"RAMP0")  )
      {
	sscanf(buf+strlen(key),"%d %f %f",&id,&x1,&x2);
	GetMakeCond(id)->SetPulsRamp0(x1,x2);
      }
    else if ( !strcmp(key,"RAMP04")  )
      {
	sscanf(buf+strlen(key),"%d %f %f",&id,&x1,&x2);
	GetMakeCond(id)->SetPulsRamp04(x1,x2);
      }
    else if ( !strcmp(key,"CHI2MAX")  )
      {
	sscanf(buf+strlen(key),"%d %f",&id,&x1);
	GetMakeCond(id)->SetChi2Max(x1);
      }
    else if ( !strcmp(key,"CHI2PMAX")  )
      {
	sscanf(buf+strlen(key),"%d %f",&id,&x1);
	GetMakeCond(id)->SetChi2PMax(x1);
      }
    else if ( !strcmp(key,"OFFSET")  )
      {
	sscanf(buf+strlen(key),"%d %f %f",&id,&x1,&x2);
	GetMakeCond(id)->SetOffset(x1,x2);
      }
    else if ( !strcmp(key,"XCUT")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f %f %f %f %f %f %f",&id,
	       var,var+1,var+2,var+3,var+4,var+5,var+6,var+7,var+8,var+9);
	AddSegmentCut(id,0,var);
      }
    else if ( !strcmp(key,"ICUT")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f %f %f %f %f %f %f",&id,
	       var,var+1,var+2,var+3,var+4,var+5,var+6,var+7,var+8,var+9);
	AddSegmentCut(id,1,var);
      }
    else if ( !strcmp(key,"CUTCP")  )
      {
	sscanf(buf+strlen(key),"%d %f %f %f %f %f %f",&id,
	       var,var+1,var+2,var+3,var+4,var+5);
	AddCutCP(var);
      }
    else if ( !strcmp(key,"AFID")  )
      {
	sscanf(buf+strlen(key),"%d",&id);
	eAFID=id;
      }
    else if ( !strcmp(key,"VOLUME0")  )
      {
	float x0=0,y0=0,z0=0,tx=0,ty=0;
	sscanf(buf+strlen(key),"%f %f %f %f %f",&x0,&y0,&z0,&tx,&ty);
	SetVolume0(x0,y0,z0,tx,ty);
      }
    else if ( !strcmp(key,"VOLUMEA")  )
      {
	float dx=0,dy=0;
	sscanf(buf+strlen(key),"%f %f",&dx,&dy);
	SetVolumeA(dx,dy);
      }
    else if ( !strcmp(key,"CLUST")  )
      {
	int icl=0;
	sscanf(buf+strlen(key),"%d",&icl);
	eCLUST=icl;
      }
  }
  fclose(fp);

  return 0;
}

///______________________________________________________________________________
void EdbDataPiece::SetVolume0(float x0, float y0, float z0, float tx, float ty)
{
  float z = GetLayer(0)->Z();
  GetLayer(0)->SetXY( x0+(z-z0)*tx, y0+(z-z0)*ty );
  GetLayer(0)->SetTXTY(tx,ty);
}

///______________________________________________________________________________
void EdbDataPiece::CorrectShrinkage(int layer, float shr)
{
  GetLayer(layer)->SetShrinkage( shr*GetLayer(layer)->Shr() );
}

///______________________________________________________________________________
int EdbDataPiece::UpdateShrPar(int layer)
{
  const char *file=eFileNamePar.Data();

  FILE *fp=fopen(file,"a");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return 0;
  }else
    printf( "\nUpdate parameters file with SHRINK %d: %s\n\n", layer, file );

  char str[64];
  sprintf(str,"SHRINK \t %d \t %f \n",layer, GetLayer(layer)->Shr() );
  fprintf(fp,"\n%s",str);

  fclose(fp);
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::UpdateAffPar(int layer, EdbAffine2D &aff)
{
  const char *file=eFileNamePar.Data();

  FILE *fp=fopen(file,"a");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return 0;
  }else
    printf( "\nUpdate parameters file with AFFXY: %s\n\n", file );

  char str[124];
  sprintf(str,"AFFXY \t %d \t %f %f %f %f %f %f\n",layer,
	 aff.A11(),aff.A12(),aff.A21(),aff.A22(),aff.B1(),aff.B2() );
  fprintf(fp,"\n%s",str);

  fclose(fp);
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::UpdateZPar(int layer, float z)
{
  const char *file=eFileNamePar.Data();

  FILE *fp=fopen(file,"a");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return 0;
  }else
    printf( "\nUpdate parameters file with ZLAYER: %s\n\n", file );

  char str[124];
  sprintf(str,"ZLAYER \t %d \t %f %f %f\n",layer,
	 z,0.,0. );
  fprintf(fp,"\n%s",str);

  fclose(fp);
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::UpdateAffTPar(int layer, EdbAffine2D &aff)
{

  EdbAffine2D *a = GetLayer(layer)->GetAffineTXTY();
  a->Transform(&aff);

  const char *file=eFileNamePar.Data();

  FILE *fp=fopen(file,"a");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return 0;
  }else
    printf( "\nUpdate parameters file with AFFTXTY: %s\n\n", file );

  char str[124];
  sprintf(str,"AFFTXTY \t %d \t %f %f %f %f %f %f\n",layer,
	 a->A11(),a->A12(),a->A21(),a->A22(),a->B1(),a->B2() );
  fprintf(fp,"\n%s",str);

  fclose(fp);
  return 1;
}

///______________________________________________________________________________
void EdbDataPiece::WriteCuts()
{
  TString file = eFileNamePar+".C";

  FILE *fp=fopen(file,"w");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file.Data());
    return;
  }else
    printf( "\nPut Cuts to file: %s\n\n", file.Data() );

  fprintf(fp,"{\n");

  char str[256];
  for(int i=0; i<3; i++)
    if(eCuts[i])
      for(int j=0; j<NCuts(i); j++)  {
	GetCut(i,j)->CutLine(str,i,j);
	fprintf(fp,"%s",str);
      }

  fprintf(fp,"}\n");
  fclose(fp);
}

///______________________________________________________________________________
int EdbDataPiece::PassCutCP(float var[6])
{
  if(eCutCP[0]<0) return 1;
  for(int i=0; i<6; i++)    if(var[i]>eCutCP[i])  return 0;
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::PassCuts(int id, float var[5])
{
  int nc = NCuts(id);
  for(int i=0; i<nc; i++)
    if( !(GetCut(id,i)->PassCut(var)) )  return 0;
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::TakeRawSegment(EdbView *view, int id, EdbSegP &segP, int side)
{
  //TODO: add transformations

  EdbSegment *seg = view->GetSegment(id);

  float var[5];
  var[0] = seg->GetX0();
  var[1] = seg->GetY0();
  var[2] = seg->GetTx();
  var[3] = seg->GetTy();
  var[4] = seg->GetPuls();

  if( !PassCuts(side,var) )     return 0;

  EdbLayer  *layer=GetLayer(side);
  if(eAFID) seg->Transform( view->GetHeader()->GetAffine() );

  float x,y,z,tx,ty,puls;
  tx   = seg->GetTx()/layer->Shr();
  ty   = seg->GetTy()/layer->Shr();
  x    = seg->GetX0() + layer->Zmin()*tx;
  y    = seg->GetY0() + layer->Zmin()*ty;
  z    = layer->Z() + layer->Zmin();
  if(eAFID==0) {
    x+=view->GetXview();
    y+=view->GetYview();
  }
  puls = seg->GetPuls();

  EdbAffine2D *aff = layer->GetAffineTXTY();
  float txx = aff->A11()*tx+aff->A12()*ty+aff->B1();
  float tyy = aff->A21()*tx+aff->A22()*ty+aff->B2();
  segP.Set( seg->GetID(),x,y,txx,tyy);
  segP.SetZ( z );
  segP.SetDZ( seg->GetDz()*layer->Shr() );
  segP.SetW( puls );
  float pix = GetRawSegmentPix(seg,view);
  segP.SetVolume( pix );

  //printf("%d\n",eCLUST);
  //  if(eCLUST) segP.SetProbability( GetRawSegmentPix(seg,view) );

  return 1;
}

///______________________________________________________________________________
float EdbDataPiece::GetRawSegmentPix( EdbSegment *seg, EdbView *view )
{
  float pix=0;
  EdbCluster *cl=0;
  int ncl = view->Nclusters();
  int iseg = seg->GetID();
  //  printf("%d %d\n",ncl,seg->GetID());
  for(int i=0; i<ncl; i++ ) {
    cl = view->GetCluster(i);
    if( cl->GetSegment()==iseg )   pix += cl->GetArea();
  }
  //  printf("%f\n",pix);
  return pix;
}

///______________________________________________________________________________
int EdbDataPiece::TakeCPSegment( EdbSegCouple &cp, EdbSegP &seg)
{
  float var[6];

  var[0] = cp.N1();
  var[1] = cp.N1tot();
  var[2] = cp.N2();
  var[3] = cp.N2tot();
  var[4] = cp.CHI2();
  var[5] = cp.CHI2P();

  if( !PassCutCP(var) )     return 0;

  var[0] = seg.X();
  var[1] = seg.Y();
  var[2] = seg.TX();
  var[3] = seg.TY();
  var[4] = seg.W();

  if( !PassCuts(0,var) )     return 0;

  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::GetCPData(EdbPVRec *ali)
{
  printf("z = %f \n", GetLayer(0)->Z());
  EdbPattern *pat = new EdbPattern( 0.,0., GetLayer(0)->Z() );
  pat->SetID(0);
  EdbSegP    segP;

  TTree *tree=eCouplesTree;
  EdbSegCouple    *cp = 0;
  //EdbSegP         *s1 = 0;
  //EdbSegP         *s2 = 0;
  EdbSegP         *s  = 0;

  tree->SetBranchAddress("cp"  , &cp  );
  tree->SetBranchAddress("s."  , &s  );

  int nseg = 0;
  int nentr = (int)(tree->GetEntries());
  printf("nentr = %d\n",nentr);
  for(int i=0; i<nentr; i++ ) {
    tree->GetEntry(i);
    if( !TakeCPSegment(*cp,*s) )      continue;
    s->SetZ(pat->Z());
    s->SetVid(ePlate*1000+ePiece,i);
    pat->AddSegment( *s );
    nseg++;
  }
  printf("%d (of %d) segments are readed\n", nseg,nentr );
  ali->AddPattern(pat);
  return nseg;
}

///______________________________________________________________________________
int EdbDataPiece::CorrectAngles(TTree *tree)
{
  EdbSegCouple    *cp = 0;
  EdbSegP         *s1 = 0;
  EdbSegP         *s2 = 0;
  EdbSegP         *s  = 0;
  tree->SetBranchAddress("cp"  , &cp  );
  tree->SetBranchAddress("s1." , &s1  );
  tree->SetBranchAddress("s2." , &s2  );
  tree->SetBranchAddress("s."  , &s   );

  EdbAffine2D *aff = new EdbAffine2D();

  int nentr = (int)(tree->GetEntries());
  float *x  = new float[nentr];
  float *y  = new float[nentr];
  float *x1 = new float[nentr];
  float *y1 = new float[nentr];
  float *x2 = new float[nentr];
  float *y2 = new float[nentr];

  int nseg = 0;
  printf("nentr = %d\n",nentr);
  for(int i=0; i<nentr; i++ ) {
    tree->GetEntry(i);

    if(cp->N1tot()>1)  continue;
    if(cp->N2tot()>1)  continue;
    if(cp->CHI2()>1.5) continue;

    x1[nseg] = s1->TX();
    y1[nseg] = s1->TY();
    x2[nseg] = s2->TX();
    y2[nseg] = s2->TY();
    x[nseg]  = (s2->X()-s1->X()) / (s2->Z()-s1->Z());
    y[nseg]  = (s2->Y()-s1->Y()) / (s2->Z()-s1->Z());
    nseg++;
  }

  aff->CalculateTurn( nseg,x1,y1,x,y );
  UpdateAffTPar(1,*aff);
  aff->CalculateTurn( nseg,x2,y2,x,y );
  UpdateAffTPar(2,*aff);

  delete[] x;
  delete[] y;
  delete[] x1;
  delete[] y1;
  delete[] x2;
  delete[] y2;

  return nseg;
}

///______________________________________________________________________________
int EdbDataPiece::CheckCCD(int maxentr)
{
  if (eRun ) delete eRun;
  eRun =  new EdbRun( GetRunFile(0),"READ" );
  EdbView    *view = eRun->GetView();
  EdbSegment *seg;

  int npeak=0;
  
  int matr[1000][1000];
  int ix,iy;

  int i,j;
  for(i=0; i<1000; i++) 
    for(j=0; j<1000; j++)
      matr[i][j]=0;

  int ncheck=0;
  int nentr = TMath::Min(maxentr,eRun->GetEntries());
  printf("nentr=%d\n",nentr);
  for (i=0; i<nentr; i++) {
    view = eRun->GetEntry(i);
    int nseg=view->Nsegments();
    for (j=0; j<nseg; j++) {
      seg = view->GetSegment(j);
      if( seg->GetTx() >  .05 )   continue;
      if( seg->GetTx() < -.05 )   continue;
      if( seg->GetTy() >  .05 )   continue;
      if( seg->GetTy() < -.05 )   continue;
      ix = (Int_t)(seg->GetX0()+500.);
      iy = (Int_t)(seg->GetY0()+500.);
      (matr[ix][iy])++;
      ncheck++;
    }
  }

  printf("ncheck=%d\n",ncheck);
  for(i=0; i<50; i++ ) {           //eliminate upto 50 CCD defects
    if(!RemoveCCDPeak(matr)) break;
    npeak++;
  }
  return npeak;
}

///______________________________________________________________________________
int EdbDataPiece::RemoveCCDPeak(int matr[1000][1000])
{
  double mean=0;
  int filled=0;
  int max=0;
  int ix=0, iy=0;
  for(int i=0; i<1000; i++) 
    for(int j=0; j<1000; j++) {
      if(matr[i][j]<=0) continue;
      filled++;
      mean+=matr[i][j];
      if(max<matr[i][j]) {
	max = matr[i][j];
	ix = i;
	iy = j;
      }
    }
  mean/=filled;
  printf("mean = %f \t max[%d,%d]=%d\n",mean,ix,iy,max);

  float vmin[5],vmax[5];
  EdbSegmentCut cut;
  if( max > 10.*mean ) {
    matr[ix][iy]=0;
    vmin[0] = ix-500.;    vmax[0] = vmin[0]+1;
    vmin[1] = iy-500.;    vmax[1] = vmin[1]+1;
    vmin[2] = -.6;        vmax[2] = .6;
    vmin[3] = -.6;        vmax[3] = .6;
    vmin[4] =   0;        vmax[4] = 17;
    cut.SetXI(0);
    cut.SetMin(vmin);
    cut.SetMax(vmax);
    UpdateSegmentCut(cut);
    return 1;
  }
  return 0;
}

///______________________________________________________________________________
int EdbDataPiece::UpdateSegmentCut(EdbSegmentCut cut)
{
  const char *file=eFileNamePar.Data();

  FILE *fp=fopen(file,"a");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return 0;
  }

  char str[124];
  if(cut.XI()==0) {
    printf( "\nUpdate parameters file with XCUT: %s\n\n", file );
    sprintf(str,"XCUT \t %d \t %f %f %f %f %f %f %f %f %f %f\n",1,
	    cut.Min(0),cut.Max(0), 
	    cut.Min(1),cut.Max(1), 
	    cut.Min(2),cut.Max(2), 
	    cut.Min(3),cut.Max(3), 
	    cut.Min(4),cut.Max(4)
	    );
    fprintf(fp,"\n%s",str);
    sprintf(str,"XCUT \t %d \t %f %f %f %f %f %f %f %f %f %f\n",2,
	    cut.Min(0),cut.Max(0), 
	    cut.Min(1),cut.Max(1), 
	    cut.Min(2),cut.Max(2), 
	    cut.Min(3),cut.Max(3), 
	    cut.Min(4),cut.Max(4)
	    );
    fprintf(fp,"\n%s",str);
  } else  if(cut.XI()==1) {
    printf( "\nUpdate parameters file with ICUT: %s\n\n", file );
    sprintf(str,"ICUT \t %d \t %f %f %f %f %f %f %f %f %f %f\n",1,
	    cut.Min(0),cut.Max(0), 
	    cut.Min(1),cut.Max(1), 
	    cut.Min(2),cut.Max(2), 
	    cut.Min(3),cut.Max(3), 
	    cut.Min(4),cut.Max(4)
	    );
    fprintf(fp,"\n%s",str);
    sprintf(str,"XCUT \t %d \t %f %f %f %f %f %f %f %f %f %f\n",2,
	    cut.Min(0),cut.Max(0), 
	    cut.Min(1),cut.Max(1), 
	    cut.Min(2),cut.Max(2), 
	    cut.Min(3),cut.Max(3), 
	    cut.Min(4),cut.Max(4)
	    );
    fprintf(fp,"\n%s",str);
  }

  fclose(fp);
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::GetRawData(EdbPVRec *ali)
{
  //TODO: irun logic

  CloseRun();
  eRun =  new EdbRun( GetRunFile(0),"READ" );
  if(!eRun) return 0;

  EdbPattern *pat1 = new EdbPattern( 0.,0., GetLayer(1)->Z() + GetLayer(0)->Z() );
  EdbPattern *pat2 = new EdbPattern( 0.,0., GetLayer(2)->Z() + GetLayer(0)->Z() );

  EdbViewHeader *head=0;
  EdbSegP        segP;
  EdbView       *view = eRun->GetView();
  int      nseg=0, nrej=0;
  int      nsegV=0;
  int      side=0;

  int nentr = eRun->GetEntries();
  for(int iv=0; iv<nentr; iv++ ) {
    head = eRun->GetEntryHeader(iv);
    if(      head->GetNframesTop()==0 ) side=2;
    else if( head->GetNframesBot()==0 ) side=1;

    if( !AcceptViewHeader(head) )  continue;


    //printf("** %d\n",eCLUST);
    if(eCLUST)       view = eRun->GetEntry(iv,1,1,1);
    else             view = eRun->GetEntry(iv);

    nsegV = view->Nsegments();
    for(int j=0;j<nsegV;j++) {
      if(!TakeRawSegment(view,j,segP,side)) {
	nrej++;
	continue;
      }
      nseg++;
      segP.SetVid(iv,j);
      segP.SetAid(view->GetAreaID(),view->GetViewID());

      if(side==1) pat1->AddSegment( segP );
      else if(side==2) pat2->AddSegment( segP );
    }

  }
  ali->AddPattern(pat1);
  ali->AddPattern(pat2);
  return nseg;
}

///______________________________________________________________________________
int EdbDataPiece::GetAreaData(EdbPVRec *ali, int aid, int side)
{
  TIndexCell *elist   = eAreas[side]->At(aid);
  if(!elist)  return 0;
  EdbPattern *pat = new EdbPattern( 0.,0., GetLayer(side)->Z() );
  pat->SetID(side);


  EdbSegP    segP;
  EdbView    *view = eRun->GetView();
  int   nseg=0, nrej=0;
  int   entry;
  int   nsegV;

  int niu=elist->N();
  for(int iu=0; iu<niu; iu++) {
    entry = elist->At(iu)->Value();
    //    view = eRun->GetEntry(entry);
    if(eCLUST)       view = eRun->GetEntry(entry,1,1,1);
    else             view = eRun->GetEntry(entry);

    nsegV = view->Nsegments();

    for(int j=0;j<nsegV;j++) {
      if(!TakeRawSegment(view,j,segP,side)) {
	nrej++;
	continue;
      }
      nseg++;
      segP.SetVid(entry,j);
      segP.SetAid(view->GetAreaID(),view->GetViewID());
      pat->AddSegment( segP);
    }
  }

  printf("Area: %d ( %d%%)  %d \t viwes: %d \t nseg: %d \t rejected: %d\n", 
	 aid,100*aid/eAreas[side]->N(1),side,niu,nseg, nrej );
  ali->AddPattern(pat);
  return nseg;
}

///______________________________________________________________________________
int EdbDataPiece::MakeLinkListArea(int irun)
{
  if (eRun ) delete eRun;
  eRun =  new EdbRun( GetRunFile(irun),"READ" );
  for(int i=0; i<3; i++) {
    if(eAreas[i])    delete eAreas[i];
    eAreas[i]= new TIndexCell();
  }

  int nentr = eRun->GetEntries();
  printf("Make views entry map,  nentr = %d\n",nentr);

  Long_t v[2];   // areaID,entry
  EdbViewHeader *head=0;

  for(int iv=0; iv<nentr; iv++ ) {
    head = eRun->GetEntryHeader(iv);
    v[0]=head->GetAreaID();
    v[1]=iv;
    if(head->GetNframesTop()==0) {         // fill down views
      eAreas[2]->Add(2,v);
    }
    else if(head->GetNframesBot()==0) {    // fill up views
       eAreas[1]->Add(2,v);
    }
  }

  eAreas[1]->Sort();
  eAreas[2]->Sort();
  return eAreas[1]->N(1);
}

///______________________________________________________________________________
int EdbDataPiece::AcceptViewHeader(const EdbViewHeader *head)
{
  EdbSegP p;
  if(eAFID==0) {
    p.SetX( head->GetXview() );
    p.SetY( head->GetYview() );
  } else {
    p.SetX( 0. );
    p.SetY( 0. );
    p.Transform( head->GetAffine() );
  }
  p.SetX( p.X() - GetLayer(0)->X() );
  p.SetY( p.Y() - GetLayer(0)->Y() );
  p.Transform( GetLayer(0)->GetAffineXY() );
  if( p.X() < -GetLayer(0)->DX() )    return 0;
  if( p.X() >  GetLayer(0)->DX() )    return 0;
  if( p.Y() < -GetLayer(0)->DY() )    return 0;
  if( p.Y() >  GetLayer(0)->DY() )    return 0;
  return 1;
}

///______________________________________________________________________________
int EdbDataPiece::InitCouplesTree(const char *mode)
{
  if( (eCouplesTree=InitCouplesTree(eFileNameCP,mode ))) return 1;
  return 0;
}

///______________________________________________________________________________
TTree *EdbDataPiece::InitCouplesTree(const char *file_name, const char *mode)
{
  const char *tree_name="couples";
  TTree *tree=0;

  if (!tree) {
    TFile *f = new TFile(file_name,mode);
    if (f)  tree = (TTree*)f->Get(tree_name);
    if(!tree) {

      f->cd();
      tree = new TTree(tree_name,tree_name);
      
      int pid1,pid2;
      float xv,yv;
      EdbSegCouple *cp=0;
      EdbSegP      *s1=0;
      EdbSegP      *s2=0;
      EdbSegP      *s=0;
      
      tree->Branch("pid1",&pid1,"pid1/I");
      tree->Branch("pid2",&pid2,"pid2/I");
      tree->Branch("xv",&xv,"xv/F");
      tree->Branch("yv",&yv,"yv/F");
      tree->Branch("cp","EdbSegCouple",&cp,32000,99);
      tree->Branch("s1.","EdbSegP",&s1,32000,99);
      tree->Branch("s2.","EdbSegP",&s2,32000,99);
      tree->Branch("s." ,"EdbSegP",&s,32000,99);
      tree->Write();
      tree->SetAutoSave(2000000);
    }
  }
  return tree;
}


///______________________________________________________________________________
int EdbDataPiece::MakeLinkListCoord(int irun)
{
  if(eRun) delete eRun;
  eRun =  new EdbRun( GetRunFile(irun),"READ" );
  for(int i=0; i<3; i++) {
    if(eAreas[i])    delete eAreas[i];
    eAreas[i]= new TIndexCell();
  }

  int nentr = eRun->GetEntries();
  printf("Make views coordinate map,  nentr = %d\n",nentr);

  TIndexCell upc;
  TIndexCell downc;
  Long_t v[3];   // x,y,entry
  EdbViewHeader *head=0;

  Long_t xx=0, yy=0;
  float cx = 2000., cy = 2000.;  // 2x2 mm cells
  float dx = 400. , dy = 400.;   // 400 microns margins
  float xv,yv;
  int mx[9] = {0, 0, 0,-1,  1, -1,-1, 1, 1};
  int my[9] = {0,-1, 1, 0,  0, -1, 1,-1, 1};

  for(int iv=0; iv<nentr; iv++ ) {

    head = eRun->GetEntryHeader(iv);
    if( !AcceptViewHeader(head) )  continue;

    yv = head->GetXview();
    xv = head->GetYview();
    xx = (Long_t)(xv/cx);
    yy = (Long_t)(yv/cx);
    v[0] = xx;
    v[1] = yy;
    v[2] = iv;

    if(head->GetNframesBot()==0) {              // fill up views
      upc.Add(3,v);
    }
    else if(head->GetNframesTop()==0) {         // fill down views
      downc.Add(3,v);                           // add center
      int im;
      for( im=1; im<5; im++ ) {             // add sides margins
        v[0] = (Long_t)(( xv+dx*mx[im] ) / cx);
        v[1] = (Long_t)(( yv+dy*my[im] ) / cy);
        if( (v[0] != xx) || (v[1] != yy) )
            downc.Add(3,v);
      }
      for( im=5; im<9; im++ ) {             // add angles margins
        v[0] = (Long_t)(( xv+dx*mx[im] ) / cx);
        v[1] = (Long_t)(( yv+dy*my[im] ) / cy);
        if( (v[0] != xx) && (v[1] != yy) )
          //      if(!downc.Find(3,v))
            downc.Add(3,v);
      }

    }

  }

  upc.Sort();
  downc.Sort();

  TIndexCell *clx=0;
  TIndexCell *cly=0;

  int areac=0;
  int nix,niy,nie;
  nix=upc.N(1);
  for(int ix=0; ix<nix; ix++) {
    clx = upc.At(ix);
    xx = clx->Value();
    niy=clx->N(1);
    for(int iy=0; iy<niy; iy++) {
      cly  = clx->At(iy);
      yy = cly->Value();
      areac++;

      nie = cly->N(1);
      int ie;
      for(ie=0; ie<nie; ie++) {
        v[0]=areac;
        v[1]  = cly->At(ie)->Value();
        eAreas[1]->Add(2,v);
      }

      cly = downc.Find(xx)->Find(yy);
      nie=cly->N(1);
      for(ie=0; ie<nie; ie++) {
        v[0] = areac;
        v[1] = cly->At(ie)->Value();
        eAreas[2]->Add(2,v);
      }
    }
  }

  eAreas[1]->Sort();
  eAreas[2]->Sort();

  return eAreas[1]->N(1);
}

///==============================================================================
EdbDataSet::EdbDataSet()
{
  Set0();
}

///------------------------------------------------------------------------------
EdbDataSet::EdbDataSet(const char *file)
{
  Set0();
  ReadDataSetDef(file);
}

///______________________________________________________________________________
EdbDataSet::~EdbDataSet()
{
  if(ePieces.GetEntries()) ePieces.Delete();
}

///______________________________________________________________________________
void EdbDataSet::Set0()
{
  eInputList  = "runs.lst";
  eAnaDir     = "./";
  eParDir     = "./";
  eDBFileName = "pieces_DB.root";
  eDBFile     = 0;
  ePieces.SetOwner();
}

///______________________________________________________________________________
int EdbDataSet::ReadDataSetDef(const char *file)
{
  char            buf[256];
  char            key[256];
  char            name[256];

  FILE *fp=fopen(file,"r");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return(-1);
  }else
    printf( "\nRead Data Set Definitions from: %s\n\n", file );


  while( fgets(buf,256,fp)!=NULL ) {
    for( int i=0; i<(int)strlen(buf); i++ ) 
      if( buf[i]=='#' )  {
	buf[i]='\0';                       // cut out comments starting from #
	break;
      }

   if( sscanf(buf,"%s",key)!=1 )                             continue;

   if      ( !strcmp(key,"OUTPUT_DATA_DIR")   )
      {
	sscanf(buf+strlen(key),"%s",name);
	eAnaDir = name;
      }
   else if ( !strcmp(key,"INPUT_RUNS_LIST")   )
      {
	sscanf(buf+strlen(key),"%s",name);
	eInputList=name;
      }
   else if ( !strcmp(key,"PARAMETERS_DIR")   )
      {
	sscanf(buf+strlen(key),"%s",name);
	eParDir=name;
      }
   else if ( !strcmp(key,"DBFILNAME")   )
      {
	sscanf(buf+strlen(key),"%s",name);
	eDBFileName=name;
      }
  }
  fclose(fp);
  GetRunList(eInputList.Data());
  return 0;
}

///______________________________________________________________________________
void EdbDataSet::PrintRunList()
{
  for(int i=0; i<ePieces.GetEntries(); i++){
    ((EdbDataPiece*)ePieces.At(i))->Print();
  }
}

///______________________________________________________________________________
void EdbDataSet::Print()
{
  EdbDataPiece *p=0;
  printf("EdbDataSet with %d pieces:\n",N());
  for(int i=0; i<N(); i++){
    p = ((EdbDataPiece*)ePieces.At(i));
    printf("%s %s\n",p->GetName(), p->GetRunFile(0));
  }
}

///______________________________________________________________________________
void EdbDataSet::WriteRunList()
{
  if(!eDBFile) eDBFile = new TFile(eDBFileName.Data(),"UPDATE");
  //else eDBFile->Cd();

  for(int i=0; i<ePieces.GetEntries(); i++){
    ((EdbDataPiece*)ePieces.At(i))->MakeName();
  }

  ePieces.Write("pieces",1);
}

///______________________________________________________________________________
EdbDataPiece *EdbDataSet::FindPiece(const char *name)
{
  const char *nn;
  EdbDataPiece *piece=0;
  for(int i=0; i<ePieces.GetEntries(); i++){
    piece = (EdbDataPiece*)ePieces.At(i);
    nn = piece->GetName();
    if(!strcmp(nn,name)) return piece;
  }
  return 0;
}

///______________________________________________________________________________
int EdbDataSet::GetRunList(const char *file)
{
  char            buf[256];
  char            filename[256];
  int             nrun=0;

  FILE *fp=fopen(file,"r");
  if (fp==NULL)   {
    printf("ERROR open file: %s \n", file);
    return(-1);
  }else
    printf( "\nRead runs list from file: %s\n\n", file );

  EdbDataPiece *piece=0;
  EdbDataPiece *pp=0;
  int plateID,pieceID,flag;

  int ntok=0;
  while( fgets(buf,256,fp)!=NULL ) {
    ntok = sscanf(buf,"%d %d %s %d",&plateID,&pieceID,filename,&flag);
    if(ntok!=4) break;
    if(flag<=0) continue;
    nrun++;
    piece = new EdbDataPiece(plateID,pieceID,filename,flag);
    piece->MakeName();
    if( (pp=FindPiece(piece->GetName())) ) {
      pp->AddRunFile( filename );
      delete piece;
    } else {
      piece->MakeNameCP(GetAnaDir());
      piece->MakeNamePar(GetParDir());
      piece->TakePiecePar();
      ePieces.Add(piece);
    }
  }
  fclose(fp);

  return nrun;
}

///==============================================================================
EdbDataProc::EdbDataProc(const char *file)
{
  eDataSet = new EdbDataSet(file);
}

///------------------------------------------------------------------------------
EdbDataProc::~EdbDataProc()
{
  if(eDataSet) delete eDataSet;
}

///------------------------------------------------------------------------------
int EdbDataProc::CheckCCD()
{
  if(!eDataSet) return 0;
  EdbDataPiece *piece;
  int ndef=0;
  int np=eDataSet->N();
  for(int i=0; i<np; i++) {
    piece = eDataSet->GetPiece(i);
    ndef = piece->CheckCCD();
    printf("piece %s: eliminated defects: %d\n",piece->GetName(),ndef); 
    piece->WriteCuts();
    piece->CloseRun();
  }
  return np;
}

///------------------------------------------------------------------------------
int EdbDataProc::Link()
{
  if(!eDataSet) return 0;
  EdbDataPiece *piece;
  int np=eDataSet->N();
  for(int i=0; i<np; i++) {
    piece = eDataSet->GetPiece(i);
    piece->TakePiecePar();
    piece->Print();
    piece->WriteCuts();
    if(piece->Flag()==1)    Link(*piece);
    piece->CloseRun();
  }
  return np;
}

///______________________________________________________________________________
void EdbDataProc::FillCouplesTree( TTree *tree, EdbPVRec *al, int fillraw )
{
  tree->GetDirectory()->cd();

  EdbPatCouple *patc=0;
  float xv = al->X();
  float yv = al->Y();

  int pid1,pid2;
  EdbSegCouple *cp=0;
  EdbSegP      *s1=0;
  EdbSegP      *s2=0;
  EdbSegP      *s=0;

  tree->SetBranchAddress("pid1",&pid1);
  tree->SetBranchAddress("pid2",&pid2);
  tree->SetBranchAddress("xv"  ,&xv);
  tree->SetBranchAddress("yv"  ,&yv);
  tree->SetBranchAddress("cp"  ,&cp);
  tree->SetBranchAddress("s1." ,&s1);
  tree->SetBranchAddress("s2." ,&s2);
  tree->SetBranchAddress("s."  ,&s );

  if(fillraw) {
    // **** fill tree with raw segments ****
    EdbPattern *pat=0;
    int nic;
    int nip=al->Npatterns();
    for( int ip=0; ip<nip; ip++ ) {
      pat  = al->GetPattern(ip);
      pid1 = pat->ID();
      pid2 = -1;
      nic=pat->N();
      for( int ic=0; ic<nic; ic++ ) {
        s1 = pat->GetSegment(ic);
        tree->Fill();
      }
    }
  }

  // **** fill tree with found couples ****
  s = new EdbSegP();

  int nip=al->Ncouples();
  for( int ip=0; ip<nip; ip++ ) {
    patc = al->GetCouple(ip);
    pid1 = patc->Pat1()->ID();
    pid2 = patc->Pat2()->ID();

    int nic=patc->Ncouples();
    for( int ic=0; ic<nic; ic++ ) {
      cp = patc->GetSegCouple(ic);
      s1 = patc->Pat1()->GetSegment(cp->ID1());
      s2 = patc->Pat2()->GetSegment(cp->ID2());


      s->Set( ic,
	      (s1->X()+s2->X())/2.,
	      (s1->Y()+s2->Y())/2.,
              (s1->X()-s2->X())/(s1->Z()-s2->Z()),
              (s1->Y()-s2->Y())/(s1->Z()-s2->Z()),
              s1->W()+s2->W()
	      );
      s->SetZ( (s2->Z()+s1->Z())/2 );
      s->SetDZ( s2->Z()-s1->Z() );
      

      //EdbSegP::LinkMT(s1,s2,s);
      s->SetVolume( s1->Volume()+s2->Volume() );

      //s->Print();
      tree->Fill();
    }
  }

}

///______________________________________________________________________________
void EdbDataProc::CloseCouplesTree(TTree *tree)
{
  tree->AutoSave();
  tree->GetCurrentFile()->Close();
  //if(tree) delete tree;
}

///______________________________________________________________________________
int EdbDataProc::CheckShrinkage(EdbPVRec *ali, int couple, float &shr1, float &shr2)
{
  EdbPatCouple  *patc = ali->GetCouple(couple);
  EdbSegCouple *sc=0;
  EdbSegP *s1=0, *s2=0;

  double dz,tx,ty,t,t1,t2,sumt1=0,sumt2=0;
  int    nsum=0;

  int  nc=patc->Ncouples();
  for( int ic=0; ic<nc; ic++ ) {
    sc = patc->GetSegCouple(ic);

    if(sc->CHI2()>1.5)  continue;

    s1 = patc->Pat1()->GetSegment(sc->ID1());
    s2 = patc->Pat2()->GetSegment(sc->ID2());

    dz = s2->Z() - s1->Z();
    tx = (s2->X() - s1->X())/dz;
    ty = (s2->Y() - s1->Y())/dz;
 
    t  = TMath::Sqrt( tx*tx + ty*ty );
    if(t<.1) continue;
    if(t>.45) continue;
    t1 = TMath::Sqrt( s1->TX()*s1->TX() + s1->TY()*s1->TY() );
    t2 = TMath::Sqrt( s2->TX()*s2->TX() + s2->TY()*s2->TY() );

    nsum++;
    sumt1 += t1/t;
    sumt2 += t2/t;
  }

  if(nsum<10)  return 0;

  shr1 = sumt1/nsum;
  shr2 = sumt2/nsum;

  return nsum;
}

///______________________________________________________________________________
int EdbDataProc::Link(EdbDataPiece &piece)
{
  EdbPVRec  *ali;

  const char *file_name=piece.GetNameCP();
  TTree *cptree=EdbDataPiece::InitCouplesTree(file_name,"RECREATE");
  EdbScanCond *cond = piece.GetCond(1);

  int ntot=0, nareas=0;

  float  shr1=1,shr2=1;
  double shrtot1=0, shrtot2=0;
  int    nshr=0,nshrtot=0;

  for( int irun=0; irun<piece.Nruns(); irun++ ) {
    nareas = piece.MakeLinkListCoord(irun);
    //nareas = piece.MakeLinkListArea(irun);
    for(int i=0; i<nareas; i++ ) {

      ali      = new EdbPVRec();
      ali->SetScanCond( cond );

      piece.GetAreaData(ali,i,1);
      piece.GetAreaData(ali,i,2);

      ali->SetSegmentsErrors();

      ali->SetCouplesAll();
      ali->SetChi2Max(cond->Chi2PMax());
      ali->Link();

      if( ShrinkCorr() ) {
	shr1 = 1;
	shr2 = 1;
	nshr = CheckShrinkage( ali,0, shr1, shr2 );
	nshrtot += nshr;
	shrtot1 += nshr*shr1;
	shrtot2 += nshr*shr2;
      }

      FillCouplesTree(cptree, ali,0);

      delete ali;
    }
    ntot+=nareas;
    shrtot1 = shrtot1/nshrtot;
    shrtot2 = shrtot2/nshrtot;
    printf("Shrinkage correction: %f %f\n", (float)shrtot1,(float)shrtot2);
    piece.CorrectShrinkage( 1, (float)shrtot1 );
    piece.CorrectShrinkage( 2, (float)shrtot2 );
    piece.UpdateShrPar(1);
    piece.UpdateShrPar(2);
  }
  CloseCouplesTree(cptree);

  return ntot;
}

///______________________________________________________________________________
void EdbDataProc::CorrectAngles()
{
  EdbDataPiece *piece;
  int npieces = eDataSet->N();
  printf("npieces = %d\n",npieces);
  if(!npieces) return;

  TTree *cptree=0;
  for(int i=0; i<npieces; i++ ) {
    piece = eDataSet->GetPiece(i);
    printf("piece: %s\n",piece->GetNameCP());
    cptree=EdbDataPiece::InitCouplesTree(piece->GetNameCP(),"READ");
    if( !cptree )  printf("no tree %d\n",i);
    piece->CorrectAngles(cptree);
  }
}

///______________________________________________________________________________
int EdbDataProc::InitVolumeRaw(EdbPVRec    *ali)
{
  EdbScanCond *cond = eDataSet->GetPiece(0)->GetCond(1);
  ali->SetScanCond( cond );
  
  EdbDataPiece *piece;
  int npieces = eDataSet->N();
  printf("npieces = %d\n",npieces);
  if(!npieces) return 0;

  for(int i=0; i<npieces; i++ ) {
    piece = eDataSet->GetPiece(i);
    piece->GetRawData(ali);
  }

  float x0 = eDataSet->GetPiece(npieces-1)->GetLayer(0)->X();
  float y0 = eDataSet->GetPiece(npieces-1)->GetLayer(0)->Y();
  ali->Centralize(x0,y0);

  for(int j=0; j<npieces; j++ ) {
    for(int ip=0; ip<2; ip++ ) {
      int i = 2*j+ip;
      ali->GetPattern(i)->SetSegmentsZ();
      ali->GetPattern(i)->Transform(    eDataSet->GetPiece(j)->GetLayer(0)->GetAffineXY()   );
      ali->GetPattern(i)->TransformA(   eDataSet->GetPiece(j)->GetLayer(0)->GetAffineTXTY() );
      ali->GetPattern(i)->TransformShr( eDataSet->GetPiece(j)->GetLayer(0)->Shr()  );
    }
  }
  ali->SetPatternsID();
  ali->SetSegmentsErrors();

  ali->SetCouplesAll();
  ali->SetChi2Max(cond->Chi2PMax());
  ali->SetOffsetsMax(cond->OffX(),cond->OffY());
  return npieces;
}

///______________________________________________________________________________
int EdbDataProc::InitVolume(EdbPVRec    *ali)
{
  EdbScanCond *cond = eDataSet->GetPiece(0)->GetCond(0);
  ali->SetScanCond( cond );
  
  EdbDataPiece *piece;
  int npieces = eDataSet->N();
  printf("npieces = %d\n",npieces);
  if(!npieces) return 0;

  TTree *cptree=0;
  int i;
  for(i=0; i<npieces; i++ ) {
    printf("\n");
    piece = eDataSet->GetPiece(i);
    cptree=EdbDataPiece::InitCouplesTree(piece->GetNameCP(),"READ");
    if( !cptree )  printf("no tree %d\n",i);
    piece->SetCouplesTree(cptree);
    piece->GetCPData(ali);
  }

  float x0 = eDataSet->GetPiece(npieces-1)->GetLayer(0)->X();
  float y0 = eDataSet->GetPiece(npieces-1)->GetLayer(0)->Y();
  //ali->Centralize();
  ali->Centralize(x0,y0);

  for(i=0; i<npieces; i++ ) {
    ali->GetPattern(i)->SetSegmentsZ();
    ali->GetPattern(i)->Transform(    eDataSet->GetPiece(i)->GetLayer(0)->GetAffineXY()   );
    ali->GetPattern(i)->TransformA(   eDataSet->GetPiece(i)->GetLayer(0)->GetAffineTXTY() );
    ali->GetPattern(i)->TransformShr( eDataSet->GetPiece(i)->GetLayer(0)->Shr()  );
  }
  ali->SetPatternsID();
  //ali->Print();
  //ali->GetScanCond()->Print();
  ali->SetSegmentsErrors();

  ali->SetCouplesAll();
  ali->SetChi2Max(cond->Chi2PMax());
  ali->SetOffsetsMax(cond->OffX(),cond->OffY());
  return npieces;
}

///______________________________________________________________________________
void EdbDataProc::Align()
{
  EdbPVRec    *ali  = new EdbPVRec();
  InitVolume(ali);

  ali->Align();
  ali->PrintAff();
  EdbAffine2D  aff;
  for(int i=0; i<ali->Npatterns(); i++) {
    ali->GetPattern(i)->GetKeep(aff);
    eDataSet->GetPiece(i)->UpdateAffPar(0,aff);
  }

}

///______________________________________________________________________________
void EdbDataProc::LinkTracks( int alg )
{
  EdbPVRec    *ali  = new EdbPVRec();
  InitVolume(ali);
  ali->Link();
  printf("link ok\n");

  if(alg==1) {
    int nhol;
    ali->FillTracksCell();
    nhol = ali->MakeHoles(1);
    //    printf("inserted %d holes forward\n",nhol);
    ali->Link();
    ali->FillTracksCell();
    nhol = ali->MakeHoles(-1);
    //    printf("inserted %d holes backward\n",nhol);
    ali->Link();
  }

  ali->FillTracksCell();

  TTree *cptree=EdbDataPiece::InitCouplesTree("linked_couples.root","RECREATE");
  FillCouplesTree(cptree, ali,0);
  CloseCouplesTree(cptree);

  ali->MakeTracksTree();
}

///______________________________________________________________________________
void EdbDataProc::LinkRawTracks( int alg )
{
  EdbPVRec    *ali  = new EdbPVRec();
  InitVolumeRaw(ali);
  ali->Link();
  printf("link ok\n");

//    if(alg==1) {
//      ali->MakeHoles();
//      ali->Link();
//      printf("link ok\n");
//    }

  ali->FillTracksCell();

  TTree *cptree=EdbDataPiece::InitCouplesTree("linked_couples.root","RECREATE");
  FillCouplesTree(cptree, ali,0);
  CloseCouplesTree(cptree);

  ali->MakeTracksTree();
}

///______________________________________________________________________________
void EdbDataProc::FineAlignment()
{
  EdbPVRec    ali;
  InitVolume(&ali);
  ali.Link();
  printf("link ok\n");
  ali.FillTracksCell();

  EdbAffine2D aff;

  int fctr=0;
  int fcMin = 49;

  int i;
  ali.SelectLongTracks(ali.Npatterns());
  ali.MakeSummaryTracks();
  for( i=0; i<ali.Npatterns(); i++ ) {
    fctr = ali.FineCorrXY(i,aff);
  }

  ali.SelectLongTracks(ali.Npatterns());
  ali.MakeSummaryTracks();
  for( i=0; i<ali.Npatterns(); i++ ) {
    fctr = ali.FineCorrXY(i,aff);
  }

  if(fctr>fcMin) {
    for( i=0; i<ali.Npatterns(); i++) {
      ali.GetPattern(i)->GetKeep(aff);
      eDataSet->GetPiece(i)->UpdateAffPar(0,aff);
    }
  }

  ali.SelectLongTracks(ali.Npatterns());
  ali.MakeSummaryTracks();
  float dz=0;
  float z = ali.GetPattern(ali.Npatterns()-1)->Z();
  for( i=ali.Npatterns()-2; i>=0; i-- ) {
    fctr = ali.FineCorrZ(i,dz);
    if(fctr<=fcMin) break;
    z -= dz;
    printf("dz = %f  z = %f\n",dz,z);
    eDataSet->GetPiece(i)->UpdateZPar(0,z);
  }

  ali.SelectLongTracks(ali.Npatterns());
  ali.MakeSummaryTracks();
  for( i=0; i<ali.Npatterns(); i++ ) {
    fctr = ali.FineCorrTXTY(i,aff);
    if(fctr<=fcMin) break;
    aff.Print();
    eDataSet->GetPiece(i)->UpdateAffTPar(0,aff);
  }

  ali.SelectLongTracks(ali.Npatterns());
  ali.MakeSummaryTracks();
  dz=0;
  z = ali.GetPattern(ali.Npatterns()-1)->Z();
  for( i=ali.Npatterns()-2; i>=0; i-- ) {
    fctr = ali.FineCorrZ(i,dz);
    if(fctr<=fcMin) break;
    z -= dz;
    printf("dz = %f  z = %f\n",dz,z);
    eDataSet->GetPiece(i)->UpdateZPar(0,z);
  }

  ali.SelectLongTracks(ali.Npatterns());
  ali.MakeSummaryTracks();
  float shr=0;
  for( i=0; i<ali.Npatterns(); i++ ) {
    fctr = ali.FineCorrShr(i,shr);
    if(fctr<=fcMin) break;
    eDataSet->GetPiece(i)->CorrectShrinkage(0,shr);
    eDataSet->GetPiece(i)->UpdateShrPar(0);
  }

}

///______________________________________________________________________________
void EdbDataProc::AlignLinkTracks(int alg)
{
  EdbPVRec    *ali  = new EdbPVRec();
  InitVolume(ali);

  ali->Align();
  ali->PrintAff();
  EdbAffine2D  aff;
  for(int i=0; i<ali->Npatterns(); i++) {
    ali->GetPattern(i)->GetKeep(aff);
    eDataSet->GetPiece(i)->UpdateAffPar(0,aff);
  }

  ali->Link();
  printf("link ok\n");

//    if(alg==1) {
//      ali->MakeHoles();
//      ali->Link();
//      printf("link ok\n");
//    } 

  ali->FillTracksCell();

  TTree *cptree=EdbDataPiece::InitCouplesTree("linked_couples.root","RECREATE");
  FillCouplesTree(cptree, ali,0);
  CloseCouplesTree(cptree);

  ali->MakeTracksTree();
}
