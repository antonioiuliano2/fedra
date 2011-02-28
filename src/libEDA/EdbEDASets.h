#ifndef __EDA_SETS_H__
#define __EDA_SETS_H__

#include"EdbEDAUtil.h"
using namespace EdbEDAUtil;

class EdbEDAArea : public TObject {
	private :
	int ePlate;
	double eXmin;
	double eXmax;
	double eYmin;
	double eYmax;
	double eZ;
	bool eDraw;
	
	public:
	
	EdbEDAArea():eXmin(1e10), eXmax(-1e10), eYmin(1e10), eYmax(-1e10), eDraw(0){}
	EdbEDAArea(int ipl, double xmin, double xmax, double ymin, double ymax, double z){
		ePlate = ipl; eXmin = xmin; eXmax = xmax; eYmin = ymin; eYmax = ymax; eZ = z; eDraw=1;}
	
	void SetAreaMinMax (int ipl, double xmin, double xmax, double ymin, double ymax, double z){
		ePlate = ipl; eXmin = xmin; eXmax = xmax; eYmin = ymin; eYmax = ymax; eZ = z; eDraw=1;}
	void SetAreaXYRadius (int ipl, double x, double y, double dx, double dy, double z){
		ePlate = ipl; eXmin = x-dx; eXmax = x+dx; eYmin = y-dy; eYmax = y+dy; eZ = z; eDraw=1;}
	
	int    Plate() {return ePlate;}
	double Xmin () {return eXmin ;}
	double Xmax () {return eXmax ;}
	double Ymin () {return eYmin ;}
	double Ymax () {return eYmax ;}
	double X() { return (Xmin()+Xmax())/2.0;}
	double Y() { return (Ymin()+Ymax())/2.0;}
	double Z    () {return eZ ;}
	double RX() { return fabs( Xmin()-X() ); }
	double RY() { return fabs( Ymin()-Y() ); }
	
	void SetPlate(int ipl)  { ePlate=ipl;}
	void SetXmin (double v) { eXmin = v; eDraw=1;}
	void SetXmax (double v) { eXmax = v; eDraw=1;}
	void SetYmin (double v) { eYmin = v; eDraw=1;}
	void SetYmax (double v) { eYmax = v; eDraw=1;}
	void SetZ    (double v) { eZ    = v; }
	void SetDraw (bool b)   { eDraw = b;}
	bool GetDraw () { return eDraw;}
	
	void Print() { 
		printf("ePlate = %3d, eXmin = %8.1lf, eXmax = %8.1lf, eYmin = %8.1lf, eYmax = %8.1lf, eZ = %8.1lf\n",
				ePlate, eXmin, eXmax, eYmin, eYmax, eZ);
	}
	ClassDef(EdbEDAArea,0) // class for scanning area data.
};

class EdbEDAAreaSet : public TObject{
	
	TObjArray *eAreas;
	int eLineColor;
	int eLineWidth;
	int eTextSize;
	int eTextColor;
	TEveCompound * eCompound;
	bool eDraw;  // Draw control
	bool eDrawG; // Global Draw control by Main tab.
	
	//TArray eIndex; // to do. IPL->Area conversion index.
	
	public :
	
	EdbEDAAreaSet(){Init();};
	EdbEDAAreaSet(EdbPVRec *pvr){
		Init();
		SetAreas(pvr);
	}
	EdbEDAAreaSet(EdbDataSet *dset, double x, double y, double dx, double dy){
		Init();
		SetAreas(dset,x,y,dx,dy);
	}
	
	void SetAreas(EdbPVRec *pvr);
	void SetAreas(EdbScanSet *ss);
	
	void SetAreas(EdbDataSet *dset, double x, double y, double dx, double dy){
		if(dset==NULL) return;
		for(int i=0;i<dset->N(); i++){
			EdbDataPiece *piece = dset->GetPiece(i);
			if(NULL==piece) continue;
			EdbLayer* l = piece->GetLayer(0);
			EdbEDAArea *area = new EdbEDAArea( piece->Plate(), x-dx, x+dx, y-dy, y+dy, l->Z());
			eAreas->Add(area);
		}
	}
	
	void SetAreas(TObjArray *tracks);
	
	void Init (){
		eAreas = new TObjArray();
		eLineColor = kGray+2;
		eLineWidth = 1;
		eTextSize  = 13;
		eTextColor = kGray;
		eCompound  = NULL;
		eDraw      = kFALSE;
		eDrawG     = kTRUE;
	}
	void SetDraw(bool draw)  { eDraw = draw;}
	void SetDrawG(bool draw) { eDrawG = draw;}
	

	void AddArea(int ipl, double xmin, double xmax, double ymin, double ymax, double z){
		EdbEDAArea *area = new EdbEDAArea(ipl, xmin, xmax, ymin, ymax, z);
		eAreas->Add(area);
	}
	void AddArea(EdbEDAArea *area) { eAreas->Add(area);}	
	
	void Print(){
		printf("EdbEDAAreaSet eLineColor = %d, eLineWidth = %d, eTextSize = %d, eTextColor = %d nAreas = %d\n",
				eLineColor, eLineWidth, eTextSize, eTextColor, eAreas->GetEntries());
		for(int i=0;i<eAreas->GetEntries();i++) ((EdbEDAArea*)eAreas->At(i))->Print();
	}
	
	TObjArray *GetAreas () {return eAreas;}

	void Draw(int redraw=0);
	void UnDraw(int redraw = 0){
		TEveEventManager *ev = gEve->GetCurrentEvent();
		TEveElement *el = ev->FindChild("Scanning Areas");
		if(el) el->Destroy();
		eCompound=NULL;
		if(redraw) gEve->Redraw3D();
	}
	
	int N() { return eAreas->GetEntries();}
	EdbEDAArea * GetArea(int i) { if(0<=i&&i<N()) return (EdbEDAArea *) eAreas->At(i); else return NULL;}
	EdbEDAArea * GetAreaIPL(int ipl) { for(int i=0; i<N(); i++) if(GetArea(i)->Plate()==ipl) return GetArea(i); return NULL;}
	
	void Clear() { eAreas->Clear();}
	
	
	ClassDef(EdbEDAAreaSet,0) // class for scanning area draw.
};

class EdbEDAVertexSet:public TObject{
	
	private:
	
	public:
	
	int    eVertexColor;
	int    eVertexColorSelected;
	int    eVertexMarkerStyle;
	
	TObjArray *eVertices;     // array of vertexs to be drawn
	TObjArray *eVerticesBase; // array of vertexs, base of vertex search
	TObjArray *eVerticesPrevious;     // previous array of vertexs
	
	
	EdbEDAVertexSet(){ Init();}
	
	void Init(){
		eVertices         = new TObjArray;
		eVerticesBase     = new TObjArray;
		eVerticesPrevious = new TObjArray;
		SetVertexAttribute();
	}
	void SetVertexAttribute(
		int    vertex_color  =  kOrange, 
		int    vertex_color_selected = kRed,
		int    vertex_marker_style = 4){
		// set vertex attribute. 
		eVertexColor          = vertex_color;
		eVertexColorSelected  = vertex_color_selected;
		eVertexMarkerStyle    = vertex_marker_style;
	}
	void SetColorMode(int mode) { SetVertexAttribute(); eVertexColor = mode==kBLACKWHITE ? kBlack: kOrange;}
	
	void ResetVertexAttribute(void) { SetVertexAttribute(); }
	
	int  GetVertexColor         (void) {return eVertexColor;}
	int  GetVertexColorSelected (void) {return eVertexColorSelected;}
	int  GetVertexMarkerStyle   (void) {return eVertexMarkerStyle;}

	void AddVertex(EdbVertex *v) { 
//		if(eVerticesBase->FindObject(v)==NULL) eVerticesBase->Add(v);
//		if(eVertices->FindObject(v)==NULL)     eVertices->Add(v); 
		eVerticesBase->Add(v);
		eVertices->Add(v); 
	}
	void AddVertices(TObjArray *vertices){ if(NULL==vertices) return; for(int i=0;i<vertices->GetEntries();i++) AddVertex((EdbVertex *)vertices->At(i));}

	void SetVertex(EdbVertex *v) { if(eVertices->FindObject(v)==NULL) eVertices->Add(v);}
	void SetVertices(TObjArray *vertices){ 
		for(int i=0;i<vertices->GetEntries();i++) SetVertex((EdbVertex *)vertices->At(i));}
	void SetVerticesBase(TObjArray *vertices){ *eVerticesBase = *vertices;}
	void SetVerticesAll(void)          { *eVertices = *eVerticesBase;}
	void SetVerticesNtrk(int ntrkcut=3){
		ClearVertices();
		for(int i=0; i<NBase(); i++){
			EdbVertex *v = GetVertexBase(i);
			if(v->N()<ntrkcut) continue;
			SetVertex(v);
		}
	}
	void RestorePrevious(void) { *eVertices = *eVerticesPrevious;}
	void StorePrevious(void)   { *eVerticesPrevious = *eVertices;}
	
	
	void RemoveVertex(EdbVertex *v){ eVertices->Remove(v); eVertices->Sort();}
	void RemoveVertices(TObjArray *vertices) { for(int i=0;i<vertices->GetEntries();i++) RemoveVertex((EdbVertex *) vertices->At(i));}


	
	void ClearVertices() { eVertices->Clear();}
	void Clear() { eVertices->Clear(); eVerticesBase->Clear();}
	int N() { return eVertices->GetEntries();}
	EdbVertex * GetVertex(int i) { return (EdbVertex *) eVertices->At(i);}
	TObjArray * GetVertices() { return eVertices;}
	
	int NBase() { return eVerticesBase->GetEntries();}
	EdbVertex * GetVertexBase(int i) { return (EdbVertex *) eVerticesBase->At(i);}
	TObjArray * GetVerticesBase() { return eVerticesBase;}
	
	void DrawSingleVertex(EdbVertex *v);
	void Draw(int redraw = 0);
	void UnDraw();
	
	ClassDef(EdbEDAVertexSet,0) // Data set for vertices
};

class EdbEDAExtentionSet: public TObject{
	
	int eEnable;
	int eColor;
	double eZ;
	TEveCompound * eCompound;
	public :
	
	EdbEDAExtentionSet():eEnable(0),eColor(kGray),eZ(0.0),eCompound(NULL){};
	void Draw(int redraw=0, int fix = 0);
	void UnDraw(int redraw=0);
	void DrawResolution(int redraw =0);
	
	void Enable() { eEnable=1;}
	void Disable() { eEnable=0;}
	void SetZ(double z) { eZ=z;}
	double GetZ()   { return eZ;}
	
	ClassDef(EdbEDAExtentionSet,0) // class for draw extention
};


#endif //__EDA_SETS_H__
