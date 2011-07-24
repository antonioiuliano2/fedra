#include "EdbShowAlgClean.h"
#include "EdbShowAlg.h"
using namespace std;

ClassImp(EdbShowAlgClean)

//______________________________________________________________________________

EdbShowAlgClean::EdbShowAlgClean()
{
	Init();
	Set0();
}

//______________________________________________________________________________

EdbShowAlgClean::EdbShowAlgClean(TObjArray* Source)
{
	Init();
	Set0();
	eSource=Source;
}
//______________________________________________________________________________

EdbShowAlgClean::EdbShowAlgClean(TObjArray* Source,TObjArray* Dirt)
{
	Init();
	Set0();
	eSource=Source;
	eDirt=Dirt;
}
//______________________________________________________________________________

EdbShowAlgClean::EdbShowAlgClean(TObjArray* Source,TObjArray* Dirt,TObjArray* Target)
{
	Init();
	Set0();
	eSource=Source;
	eDirt=Dirt;
	eTarget=Target;
}

//______________________________________________________________________________

EdbShowAlgClean::~EdbShowAlgClean()
{
  // Default Destructor
	cout << "EdbShowAlgClean::~EdbShowAlgClean()"<<endl;
}

//______________________________________________________________________________

void EdbShowAlgClean::Print()
{
  cout << " EdbShowAlgClean::Print()."<<endl;
  cout << " EdbShowAlgClean::Print().Possible options:  SetSource(TObjArray*)"<<endl;
  cout << " EdbShowAlgClean::Print().Possible options:  SetDirt(TObjArray*)"<<endl;
  cout << " EdbShowAlgClean::Print().Possible options:  SetTarget(TObjArray*)"<<endl;
  cout << " EdbShowAlgClean::Print().Possible options:  CheckClassType()"<<endl;
  cout << " EdbShowAlgClean::Print().Possible options:  Execute()"<<endl;
  cout << " EdbShowAlgClean::Print().Possible options:   SimpleRemoveDoubleBTViewOverlap(EdbShowerP* shower)"<<endl;
	cout << " EdbShowAlgClean::Print().Possible options:   SimpleRemoveBGSegmentsFromShower(EdbShowerP* shower)"<<endl;
	cout << " EdbShowAlgClean::Print().Possible options:   SimpleRemovePassingDirtFromSource(EdbShowerP* shower, EdbTrackP* track)"<<endl;
	cout << " EdbShowAlgClean::Print():"<<endl;
	cout << " EdbShowAlgClean::Print()     Source Is Set? "<<    eIsSet[0] << endl;
	cout << " EdbShowAlgClean::Print()     Dirt Is Set?   "<<    eIsSet[1] << endl;
	cout << " EdbShowAlgClean::Print()     Target Is Set? "<<    eIsSet[2] << endl;

	cout << " EdbShowAlgClean::Print()     Source ClassType? "<<    eSourceClassType << endl;
	cout << " EdbShowAlgClean::Print()     Dirt ClassType?   "<<    eDirtClassType << endl;
	cout << " EdbShowAlgClean::Print()     Target ClassType? "<<    eTargetClassType << endl;

	cout << " EdbShowAlgClean::Print()     Source Entries? "  <<    eSource->GetEntries() << endl;
	cout << " EdbShowAlgClean::Print()     Dirt   Entries?  " <<    eDirt->GetEntries() << endl;
	cout << " EdbShowAlgClean::Print()     Target Entries? "  <<    eTarget->GetEntries() << endl;
	
  cout << " EdbShowAlgClean::Print()...done."<<endl;
}

//______________________________________________________________________________

void EdbShowAlgClean::Help()
{
  cout << " EdbShowAlgClean::Help()."<<endl;
}

//______________________________________________________________________________

void EdbShowAlgClean::Init() 
{
  eSource=new TObjArray();
  eDirt=new TObjArray();
  eTarget=new TObjArray();
  eCleaningType=new TArrayI(10);
  cout << " EdbShowAlgClean::Init()...done."<<endl;
}

//______________________________________________________________________________

void EdbShowAlgClean::Set0()
{
  eSource->Clear();
  eDirt->Clear();
  eTarget->Clear();
  for (int i=0;i<3;++i) eIsSet[i]=0;
  for (int i=0;i<3;++i) eCleaningType->SetAt(i,0);
  eSourceClassType=0;eDirtClassType=0;eTargetClassType=0;
  cout << " EdbShowAlgClean::Set0()...done."<<endl;
}


//______________________________________________________________________________

void EdbShowAlgClean::CheckClassType() 
{
  cout << " EdbShowAlgClean::CheckClassType()"<<endl;
  
  TString eSourceClassName="";
  TString eDirtClassName="";
  
  cout << "Now checking eSource= " << eSource << endl;
  
  if (!eIsSet[0] || eSource->GetEntries()==0) return;
  eSourceClassName=eSource->At(0)->ClassName();
  if (eSource->At(0)->ClassName()=="EdbShowerP") eSourceClassType=0;
  if (eSource->At(0)->ClassName()=="EdbTrackP") eSourceClassType=1;
  if (eSource->At(0)->ClassName()=="EdbSegP") eSourceClassType=2;
  
  cout << "eSourceClassType= " << eSourceClassType << endl;
  cout << "eSourceClassName= " << eSourceClassName << endl;
  
  cout << "Now checking eDirt= " << eDirt << endl;
  
  if (!eIsSet[1] || eDirt->GetEntries()==0) return;
  eDirtClassName=eDirt->At(0)->ClassName();
  if (eDirt->At(0)->ClassName()=="EdbShowerP") eDirtClassType=0;
  if (eDirt->At(0)->ClassName()=="EdbTrackP") eDirtClassType=1;
  if (eDirt->At(0)->ClassName()=="EdbSegP") eDirtClassType=2;
  
  cout << "eDirtClassType= " << eDirtClassType << endl;
  cout << "eDirtClassName= " << eDirtClassName << endl;
  
  cout << " EdbShowAlgClean::CheckClassType()...done."<<endl;
}


//______________________________________________________________________________

void EdbShowAlgClean::CheckClassTypeEntries() 
{
  cout << " EdbShowAlgClean::CheckClassTypeEntries()"<<endl;
  
  if (!eIsSet[0] || eSource->GetEntries()==0) return;
  Int_t eSourceClassTypeEntries=eSource->GetEntries();
  cout << "eSourceClassTypeEntries= " << eSourceClassTypeEntries << endl;
  
  if (!eIsSet[1] || eDirt->GetEntries()==0) return;
  Int_t eDirtClassTypeEntries=eDirt->GetEntries();
  cout << "eDirtClassTypeEntries= " << eDirtClassTypeEntries << endl;
  
  cout << " EdbShowAlgClean::CheckClassTypeEntries()...done."<<endl;
}


//______________________________________________________________________________

void EdbShowAlgClean::SetSource(TObjArray* Source)
{
  cout << " EdbShowAlgClean::SetSource()... NO CHEKCING YET FOR CORRECT CLASS TYPE.... (still to be done)"<<endl;
  eSource=Source;
	eSourceClassType=0; 
  eIsSet[0]=kTRUE;
  return;
}
void EdbShowAlgClean::SetSource(EdbShowerP* SourceShower)
{
  cout << " EdbShowAlgClean::SetSource(SourceShower)... NO CHEKCING YET FOR CORRECT CLASS TYPE.... (still to be done)"<<endl;
	if (TString(SourceShower->ClassName())!=TString("EdbShowerP")) {
		cout << "EdbShowAlgClean::SetSource(SourceShower)...    WRONG CLASS TYPE! Return." << endl;
		return;
	}
	eSource->Clear();
  eSource->Add(SourceShower);
	eSourceClassType=0; 
  eIsSet[0]=kTRUE;
  return;
}
void EdbShowAlgClean::SetDirt(TObjArray* Dirt)
{
  cout << " EdbShowAlgClean::SetDirt()... NO CHEKCING YET FOR CORRECT CLASS TYPE.... (still to be done)"<<endl;
  eDirt=Dirt;
	eDirtClassType=1;
  eIsSet[1]=kTRUE;
  return;
}
void EdbShowAlgClean::SetDirt(EdbTrackP* DirtTrack)
{
  cout << " EdbShowAlgClean::SetDirt()... NO CHEKCING YET FOR CORRECT CLASS TYPE.... (still to be done)"<<endl;
	if (TString(DirtTrack->ClassName())!=TString("EdbTrackP")) {
		cout << "EdbShowAlgClean::SetDirt(DirtTrack)...    WRONG CLASS TYPE! Return." << endl;
		return;
	}
  eDirt->Clear();
	eDirt->Add(DirtTrack);
	eDirtClassType=1;
  eIsSet[1]=kTRUE;
  return;
}
void EdbShowAlgClean::SetTarget(TObjArray* Target)
{
  cout << " EdbShowAlgClean::SetTarget()... NO CHEKCING YET FOR CORRECT CLASS TYPE.... (still to be done)"<<endl;
  eTarget=Target; 
  eIsSet[2]=kTRUE;
  return;
}

//______________________________________________________________________________

void EdbShowAlgClean::Execute()
{
  
  // In this function all the interface methods to the different cleaning types
  // are called, according to the flag value in eCleaningType.
  
  // Possible/Supported cleaning type methods will be:
  
  // a) Remove passing Dirt from Source:
  // a) All Objects in Dirt which start _before_ the Source
  // a) Source Objects are then removed from the specific Source object.
  // a) Example: Source=EdbShowerP, Dirt=TObjArray filled with EdbTrackP.
  // a) Example: Source=EdbShowerP, Dirt=EdbShowerP.
  
  
  // b) Remove stopping Dirt From Source:
  // -----
  /// Two options, to be separated later...
  /// a)	Remove Track Segments if the track start is behind/same as first segment.
  /// b)	Remove Track Segments if the track start is outside the cone that is defined by the first shower BT;
  
  //if (eCleaningType->At(0)==1) RemovePassingDirtFromSource();
  //if (eCleaningType->At(1)==1) RemoveStoppingDirtFromSource(); 
  
  
  
}


//______________________________________________________________________________


Bool_t EdbShowAlgClean::CompareSegmentsByPosition(EdbSegP* s1,EdbSegP* s2, Int_t type=0)
{
  // Cuts for "same segement" flag
  Float_t positioncut=0.1;
  Float_t anglecut=0.005;
  // Cuts for the view Overlap
  if (type==1) {
    // These Cutvalues have beed adapted by running over our Backgrounds.
    // A peak from View Overlap is seen as for these values:
    positioncut=2;anglecut=0.01; // Cuts for the view Overlap
  }
  if (type==99) {
    // DEBUG values
    positioncut=100;anglecut=0.1;
  }
  if (TMath::Abs(s1->X()-s2->X())>positioncut) return kFALSE;
  if (TMath::Abs(s1->Y()-s2->Y())>positioncut) return kFALSE;
  if (TMath::Abs(s1->Z()-s2->Z())>positioncut) return kFALSE;
  if (TMath::Abs(s1->TX()-s2->TX())>anglecut) return kFALSE;
  if (TMath::Abs(s1->TY()-s2->TY())>anglecut) return kFALSE;
  return kTRUE;
}


//______________________________________________________________________________
  

void EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() 
{
  cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()" << endl;
  cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()    Using source : TObjArray of EdbShowerP..." << endl;
  cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()    Using dirt :   TObjArray of EdbTrackP..." << endl;

	// Short helper instance.
	EdbShowAlg fEdbShowAlg;
	
  TObjArray* src = GetSource();
  TObjArray* drt = GetDirt();
  
  Int_t Nbefore=0;Int_t Nafter=0;
  Int_t Nsrc=src->GetEntries();
  Int_t Ndrt=drt->GetEntries();
  if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() " << Nsrc << endl;
  if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() " << Ndrt << endl; 
// return;  
  Int_t NshowersChanged=0;
  Int_t NsrcChanged=0;
	Int_t NsrcStep=Nsrc/20;
	if (NsrcStep<=2) NsrcStep=1;

	EdbSegP* showerstart=0;
  
 for (int i=0; i<src->GetEntries(); ++i) {
    EdbShowerP* sh = (EdbShowerP*)src->At(i);
		EdbSegP* showerstart=sh->GetSegment(0);
    Nbefore=sh->N();
    NshowersChanged=0;
		int debug_cnt_0=0;
		int debug_cnt_1=0;
		int debug_cnt_2=0;
		int debug_cnt_3=0;
		Float_t shLastZ=sh->GetSegmentLastZ();
		Float_t shFirstZ=sh->GetSegmentFirstZ();
		Float_t trLastZ=0;Float_t trFirstZ=0;

    for (int j=0; j<drt->GetEntries(); ++j) {
			if (gEDBDEBUGLEVEL>3)
				if (j%5==0) cout << "2% more done... j= "<< j << endl;

      EdbTrackP* tr = (EdbTrackP*)drt->At(j);
      trFirstZ=tr->Z();
			trLastZ=tr->GetSegment(tr->N()-1)->Z();
			debug_cnt_0++;
			
      // Simple Cut statements here to tell which tracks should NOT be removed:
      // These two should anyway be Not in the shower:

			if (gEDBDEBUGLEVEL>3) cout << "XXX:Cut0  sh:(i,Z0,Z1) "<< i <<" "<<  shFirstZ << " "<<  shLastZ << " tr (j,Z0,Z1) "  << j << " "<<  trFirstZ << " "<<  trLastZ  << " Cut0." << endl;

			

			// Dont check two by two tracks?
			if (tr->N()<3) continue;
			
      // Tracks starting after the shower ending:
      if (trFirstZ>shLastZ) continue;
      debug_cnt_1++;
			if (gEDBDEBUGLEVEL>3) cout << "XXX:XXX:Cut1  sh:(i,Z0,Z1) "<< i <<" "<<  shFirstZ << " "<<  shLastZ << " tr (j,Z0,Z1) "  << j << " "<<  trFirstZ << " "<<  trLastZ  << " Cut1." << endl;


      
      // Tracks ending before the shower starting
      if (trLastZ<shFirstZ) continue;
      debug_cnt_2++;
      if (gEDBDEBUGLEVEL>3) cout << "XXX:XXX:XXX:Cut2  sh:(i,Z0,Z1) "<< i <<" "<<  shFirstZ << " "<<  shLastZ << " tr (j,Z0,Z1) "  << j << " "<<  trFirstZ << " "<<  trLastZ  << " Cut2." << endl;

			
      
      // Tracks starting within the shower Cone should also NOT be removed.
			if (gEDBDEBUGLEVEL>3) cout << "Is this track within the cone w.r.t. the 1st segment of the shower???" << endl;
			if (gEDBDEBUGLEVEL>3) cout << "IsInConeTube(EdbSegP* sTest, EdbSegP* sStart, Double_t CylinderRadius, Double_t ConeAngle)"<<endl;
			Bool_t  inConeTube = fEdbShowAlg.IsInConeTube(tr->GetSegment(0), showerstart, 1000 , 0.75);
			if (inConeTube) continue;
			debug_cnt_3++;
			
			if (gEDBDEBUGLEVEL>3) cout << "XXX:XXX:XXX:XXX:Cut3  sh:(i,Z0,Z1) "<< i <<" "<<  shFirstZ << " "<<  shLastZ << " tr (j,Z0,Z1) "  << j << " "<<  trFirstZ << " "<<  trLastZ  << " Cut3." << endl;
			debug_cnt_3++;

			// Additional Safety Cut, for non removing tracks from different MC than shower.
			// Normaly this shouldnt be necessary since the Reconstruction itsself shouldn
			// avoid the MC-Mixing anyway.
			if (sh->MCEvt()>0 && tr->MCEvt()>0 && (sh->MCEvt()-tr->MCEvt()!=0)) continue;
			//cout << tr->MCEvt() << "   " << sh->MCEvt() << endl;

			
      NshowersChanged=SimpleRemoveTrackSegmentsFromShower(sh,tr);
      if (NshowersChanged>0) cout << "SimpleRemoveTrackSegmentsFromShower("<<sh->ID()<<","<<tr->ID()<<") :      YES.     Removed" << NshowersChanged << " segments." << endl;
    
    } // for (int j=0; j<drt->GetEntries(); ++j) 
    Nafter=sh->N();

		// Now here we print message, in case it was changed:
    if (Nbefore!=Nafter){
		 cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() Shower has been modified! Show details: " << endl;
     cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() Nbefore: " << Nbefore << endl; 
     cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() Nafter: " << Nafter << endl;
		 cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower() NRemoved: " << Nafter-Nbefore << endl;
     NsrcChanged++;
    }

		cout << debug_cnt_0 << endl;
		cout << debug_cnt_1 << endl;
		cout << debug_cnt_2 << endl;
		cout << debug_cnt_3 << endl;
  
  } //for (int i=0; i<src->GetEntries(); ++i) 
  
  cout << "RunStatistics:  src->GetEntries() :  " <<  src->GetEntries() << "   src changed: " << NsrcChanged << endl;

/// DEBUG
	gEDBDEBUGLEVEL=2;
  return;
}

//______________________________________________________________________________


Int_t EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower(EdbShowerP* shower, EdbTrackP* track) 
{
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower(EdbShowerP* shower, EdbTrackP* track)" << endl;
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()  Returns nseg of track if it was removed, else 0." << endl;
  
  if (gEDBDEBUGLEVEL>2) shower->PrintNice();
  if (gEDBDEBUGLEVEL>2) track ->PrintNice();
  
  TObjArray* InterimStorageArray = new TObjArray();
  Int_t IsTrackRemoved=0;
  
  // Remove _ALL_ track segments from the track which are in the shower.
  // Comparison can be either done on adresses or on positions.
  // Addresses might be unsafe, if track-segment doesnt come from shower-segment
  // source. So it might be better to compare on positions....
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()   Old shower->N() = " << shower->N() << endl;
  
  for (int n_dirt=0; n_dirt<track->N(); ++n_dirt) {
    if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()    Checking trackseg " << n_dirt <<" now" << endl;
    EdbSegP* trackseg=track->GetSegment(n_dirt);
    
    for (int n_source=0; n_source<shower->N(); ++n_source) {
      if (gEDBDEBUGLEVEL>3)  cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()     Checking showerseg " << n_source <<" now" << endl;
      EdbSegP* showerseg=shower->GetSegment(n_source);
    
      if (!CompareSegmentsByPosition(trackseg,showerseg)) continue;
      if (gEDBDEBUGLEVEL>3) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()     Possible Same Segments found (for source n="<< n_source << ") and dirt-n=" << n_dirt<< ")."<< endl;
      
      InterimStorageArray->Add(showerseg); // this shall be removed later.
      ++IsTrackRemoved;
      
      // Now a tricky part: if we remove the BT inside this loop, we collaps because the Shower->N() doesnt correspond 
      // to the original one anymore.
      // So instead we have to add this to a temporary array, and then remove it outside the loop.
			// shower->RemoveSegment(trackseg);
			// cout << "new shower->N() = " << shower->N() << endl;
			// shower->PrintSegments();
      // Update:: In ROOT the TObjArray::Remove does not automatically compress the array when you remove one object,
      // so in principle you can Remove it without InterimStorageArray and no need of indexing everytime new.
      // Though, now we keep it as it is.

      } //for (int n_source=0;    
    } //for (int n_dirt=0; 
    
    if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()     Loop Done."<<endl;
  
    // Here we can check if we want remove more segments than there are still left...
    // This is NOT a check whether the first shower basetrack is unchanged... 
    // This would be to implemnt somewhere else, actually one wants to keep always
    // the first shower segment anyway...
    if (shower->N()<=InterimStorageArray->GetEntries()) {
      cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()... WARNING. I shall remove more tracks from the shower than the shower has segments. I wont do this: dont remove the track." << endl;
      delete InterimStorageArray;
      if (gEDBDEBUGLEVEL>1) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower(EdbShowerP* shower, EdbTrackP* track)...Done." << endl;
      return IsTrackRemoved;
    }
    
    if (InterimStorageArray->GetEntries()==0) {
      if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()... INFORMATION. No segments from the track have been removed;" << endl;
      delete InterimStorageArray;
      if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower(EdbShowerP* shower, EdbTrackP* track)...Done." << endl;
      return IsTrackRemoved;
    }
    
  
    // Now is the right time to remove the segments from the shower:
    int nent=InterimStorageArray->GetEntries();
    for (int n=0; n<nent; ++n) {
       EdbSegP* showerseg=(EdbSegP*)InterimStorageArray->At(n);
       // No EdbShowerP* Update for all, except the last one
       shower->RemoveSegment(showerseg,kTRUE);
    }
    shower->Update();
    
    // Clear InterimStorageArray: 
    InterimStorageArray->Clear();
  
    if (gEDBDEBUGLEVEL>2) {
      cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower()   New shower->N() = " << shower->N() << endl;
      shower->PrintSegments();
    }
  
  delete InterimStorageArray;
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveTrackSegmentsFromShower(EdbShowerP* shower, EdbTrackP* track)...Done." << endl;
  return IsTrackRemoved;
}

//______________________________________________________________________________

void EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap() 
{
  // Using source : TObjArray of EdbShowerP...
  cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()" << endl;
  cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()    Using source : TObjArray of EdbShowerP..." << endl;
  
  TObjArray* src = GetSource();
  
  for (int i=0; i<src->GetEntries(); ++i) {
    EdbShowerP* sh = (EdbShowerP*)src->At(i);
    SimpleRemoveDoubleBTViewOverlap(sh);
  }
  return;
}

//______________________________________________________________________________

Int_t EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap(EdbShowerP* shower) 
{
  /// Looks for DoubleBT which are very close to each other (in TX,TY,X,Y)
  /// This is due to Sysal ViewCorrection of the objective view, since the
  /// correction function f(x,y)_view is not symmetric w.r.t f(x-300,y-300)
  /// 300=viewsize of the objective camera.

  // Cutvalues: deltaX,Y<5micron
  // Cutvalues: deltaTX,TY<5micron
  // Cutvalues: deltaZ==0 (same plate! only there the ViewOverlap occurs, of course...)
  // (plots and cutvalues justified in the thesis...).
  
  /// EdbTrackP::Remove is NOT CORRECTLY WORKING IN current fedra versions,
  /// BUG FIXING WAS DONE BY VALERI in svn rev1000
  /// (at the moment I use 9xx so it is not yet corrected for this function here!!!

  cout << "void EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap(EdbShowerP* shower)"  << endl;
  shower->PrintSegments();
  
  EdbSegP* s1=0;
  EdbSegP* s2=0;
  Int_t n1=shower->N();
  cout << "void EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap(EdbShowerP* shower)   Checking now N(N-1)/2 combinations: "  << n1*(n1-1)/2 << endl;
  TObjArray* InterimStorageArray = new TObjArray(n1);
  Bool_t IsShowerChanged=kFALSE;
  Int_t IsTrackRemoved=0;
  
  for (int i=0; i<n1-1; ++i) {
    s1=shower->GetSegment(i);
    for (int j=i+1; j<n1; ++j) {
      s2=shower->GetSegment(j);
   
      //cout << "Checking Shower Segment Combination: " << i << " " << j << "  : CompareSegmentsByPosition(EdbSegP* s1,EdbSegP* s2) " <<  CompareSegmentsByPosition(s1,s2) << endl;
      
//if (!CompareSegmentsByPosition(s1,s2,99)) continue; /// DEBBUG PURPOSE ONLY
      if (!CompareSegmentsByPosition(s1,s2,1)) continue;
	
      cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()      Checking Shower Segment Combination: " << i << " " << j << "  : Could be two close segments caused by view overlap:"<< endl;
      s1->PrintNice();
      s2->PrintNice();
      // If s1 has the worser Chi2 we add into the array which is to be taken out afterwards,...
      if (s1->Chi2()>s2->Chi2()) {
	InterimStorageArray->Add(s1);
	if (gEDBDEBUGLEVEL>2)  cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()      s1 has worse chi2, therefore added into InterimStorageArray" << endl;
      }
      
      IsShowerChanged=kTRUE;
      IsTrackRemoved=1;
    } // of (int j=i+1; j<shower->N(); ++j)
  } // of (int i=0; i<shower->N(); ++i)


  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()     Loop Done."<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()      InterimStorageArray->GetEntries() "<< InterimStorageArray->GetEntries() << endl;
  
  if (!IsShowerChanged) {
   cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()     No change for shower. Return leaving shower unchanged."<<endl;
   delete InterimStorageArray;
   return IsTrackRemoved; 
  }
  
   // Now is the right time to remove the segments from the shower:
    int nent=InterimStorageArray->GetEntries();
    for (int n=0; n<nent; ++n) {
       EdbSegP* showerseg=(EdbSegP*)InterimStorageArray->At(n);
//        showerseg->PrintNice();
       // No EdbShowerP* Update for all, except the last one
       shower->RemoveSegment(showerseg,kTRUE);
    }
    shower->Update();
    
    // Clear InterimStorageArray: 
    InterimStorageArray->Clear();

    delete InterimStorageArray;
  
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveDoubleBTViewOverlap()   New shower->N() = " << shower->N() << endl;
  if (gEDBDEBUGLEVEL>2) shower->PrintSegments();
  return IsTrackRemoved;
}

//______________________________________________________________________________

void EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower()
{

return;
}

//______________________________________________________________________________

Int_t EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower(EdbShowerP* shower)
{

	/// Cheating cleaning alg for test purposes:
  /// Simply remove BG basetracks from the shower:
	/// Remove also MCEvt basetracks which are not shower MC

	cout << "void EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower(EdbShowerP* shower)"  << endl;
  //shower->PrintSegments();

  EdbSegP* s1=0;
  Int_t  n1=shower->N();
  Int_t  IsTrackRemoved=0;
	Int_t  showerMC=shower->MCEvt();
	Bool_t IsShowerChanged=kFALSE;
	TObjArray* InterimStorageArray = new TObjArray(n1);

	cout << showerMC << endl;
  for (int i=0; i<n1; ++i) {
    s1=shower->GetSegment(i);

		if (s1->MCEvt()==showerMC) continue;

    cout << "EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower()      Segment at: " << i << " has different MCEvt(" << s1->MCEvt()<< ")  (shower->MCEvt="<<showerMC <<"). Remove it." << endl;
      //s1->PrintNice();

			InterimStorageArray->Add(s1);

      IsShowerChanged=kTRUE;
      IsTrackRemoved=1;
  } // of (int i=0; i<shower->N(); ++i)


  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower()     Loop Done."<<endl;
  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower()     InterimStorageArray->GetEntries() "<< InterimStorageArray->GetEntries() << endl;

  if (!IsShowerChanged) {
   cout << "EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower()     No change for shower. Return leaving shower unchanged."<<endl;
   delete InterimStorageArray;
   return IsTrackRemoved;
  }

   // Now is the right time to remove the segments from the shower:
    int nent=InterimStorageArray->GetEntries();
    for (int n=0; n<nent; ++n) {
       EdbSegP* showerseg=(EdbSegP*)InterimStorageArray->At(n);
//        showerseg->PrintNice();
       // No EdbShowerP* Update for all, except the last one
       shower->RemoveSegment(showerseg,kFALSE);
    }
    shower->Update();

    // Clear InterimStorageArray:
    InterimStorageArray->Clear();

    delete InterimStorageArray;

  if (gEDBDEBUGLEVEL>2) cout << "EdbShowAlgClean::SimpleRemoveBGSegmentsFromShower()   New shower->N() = " << shower->N() << endl;
  if (gEDBDEBUGLEVEL>2) shower->PrintSegments();
  return IsTrackRemoved;

return 0;
}

//______________________________________________________________________________

void EdbShowAlgClean::RemovePassingDirtFromSource() 
{
  /// THE DYNAMIC CASTING DOEST WORK YET SOMEHOW!!!
  
  // Find from Dirt array the objects which are also present in the source object
  
  EdbSegP* source=0;
  
  // Loop over Source objects:
  for (int n_source=0; n_source<eSource->GetEntriesFast(); ++n_source) {
    
    // Helping Trick To Identifiy the right class of the object:
    cout << eSource->At(n_source)->ClassName() << endl;
    
    if (eSourceClassType==0) source=(EdbShowerP*)eSource->At(n_source);
    if (eSourceClassType==1) source=(EdbTrackP*)eSource->At(n_source);
    if (eSourceClassType==2) source=(EdbSegP*)eSource->At(n_source);
    
    source->PrintNice();
    
    TObject* dirt=0;
    // Loop over Dirt objects:
    for (int n_dirt=0; n_dirt<eDirt->GetEntriesFast(); ++n_dirt) {  
      
      if (eDirtClassType==0) dirt=(EdbShowerP*)eDirt->At(n_dirt);
      if (eDirtClassType==1) dirt=(EdbTrackP*)eDirt->At(n_dirt);
      if (eDirtClassType==2) dirt=(EdbSegP*)eDirt->At(n_dirt);
      
      dirt->Print();
      
    }// Loop over Dirt objects:
    
  }// Loop over Source objects
  
 return; 
}

//______________________________________________________________________________

void EdbShowAlgClean::RemoveStoppingDirtFromSource() 
{
 return; 
}