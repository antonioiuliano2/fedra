// tlg2couples.cpp
// ---------------
// 2007-07-10 G.Sirri (INFN BO)
// gabriele.sirri@bo.infn.it, tel +29 051 2095228/2095095
//
// Convert SySal LinkedZone files (*.tlg) to Fedra couples (*.cp.root)
//
// SySal reference file: Scanning2 SySal project
// namespace: SySal2.Scanning.Plate.IO.OPERA.LinkedZone
// method:    LinkedZone(System.IO.Stream str)
//
// Note:
// - valid only for File::Format = 5 or 6 
// - microtracks not yet included
//
// 2008-09-01 L.S.Esposito
// Read multiSession TLG : File::Format = 7

#ifndef __CINT__
#include "Riostream.h"
#include "TFile.h"
#include "EdbPVRec.h"
#endif

TTree *couples ;
Int_t Pid1=0,Pid2=0;
Float_t Xv=0,Yv=0;
EdbSegCouple *cp = 0 ;
EdbSegP      *s1 = 0 ;
EdbSegP      *s2 = 0 ;
EdbSegP      *s  = 0 ;

TClonesArray   segtop("EdbSegP") ;
TClonesArray   segbot("EdbSegP") ;

enum en_side { TOP , BOT } ;

// List of ROOT data types                                   
// -----------------------                                    
// Char_t     - Signed Character 1 byte    (char)
// Double_t   - Double 8 bytes             (double)
// Float_t    - Float 4 bytes              (float)
// Int_t      - Signed integer 4 bytes     (int)
// UInt_t     - Unsigned integer 4 bytes   (unsigned int)
// UShort_t   - UShort_t Integer 2 bytes   (unsigned short)

#include <iostream>
using namespace std;
//___________________________________________________________________________//
namespace File
{
	namespace  Info     { enum { Track = 1, BaseTrack = 2, Field = 3 } ; } // byte
	namespace  Section  { enum { Data = 0x20, Header = 0x40          } ; } // byte 
	namespace  Format   { enum { Old  = 0x08,                              // ushort
		Old2 = 0x02,                   
		NoExtents = 0x01, 
		Normal = 0x03, 
		NormalWithIndex = 0x04, 
		NormalDouble_tWithIndex = 0x05, 
		Detailed = 0x06,
		MultiSection = 0x07} ;}
}
//___________________________________________________________________________//
namespace Header
{
    const Char_t SectionTag = 0x01;

    Char_t   infotype     ;
	UShort_t headerformat ;

	Double_t Top_TopZ       ;
	Double_t Top_BottomZ    ;
	Double_t Bottom_TopZ    ;
	Double_t Bottom_BottomZ ;
	namespace Identifier { Int_t    Part0, Part1, Part2 , Part3 ;}
	namespace Center     { Double_t X, Y; } 
	namespace Extents    { Double_t MinX, MaxX, MinY, MaxY ; }
	namespace Transform  { Double_t MXX, MXY, MYX, MYY, TX, TY, RX, RY ; }

	Int_t TopViewsLength       ;  
	Int_t BottomViewsLength    ;

	Int_t TopTracksLength ;
	Int_t BottomTracksLength ;
	Int_t TracksLength ;
}
//___________________________________________________________________________//
namespace View
{
	Int_t      id        ;
	Double_t   topz      ;
	Double_t   bottomz   ;
	Double_t   px        ;
	Double_t   py        ;
}
//___________________________________________________________________________//
namespace MIPEmulsionTrack
{
	UInt_t     Field    ;   // The field of view where the track has been found.
	UInt_t     AreaSum  ;   // Sum of the areas of all grains.
	UInt_t     Count    ;   // The number of grains in the track.
	Double_t   X        ;   // 3D position of a poInt_t on the track trajectory
	Double_t   Y        ;   // 3D position of a poInt_t on the track trajectory
	Double_t   Z        ;   // 3D position of a poInt_t on the track trajectory
	Double_t   SX       ;   // 3D slope of the track trajectory.
	Double_t   SY       ;   // 3D slope of the track trajectory.
	Double_t   SZ       ;   // 3D slope of the track trajectory.
	Double_t   Sigma    ;   // Quality of the track, usually expressed as alignment residuals or angular agreement.
	Double_t   TopZ     ;   // Z of the top grain. 
	Double_t   BottomZ  ;   // Z of the bottom grain.
	//Int_t    id       ;   //for MIPIndexesEmulsiontrack: Id of the track, e.g. its sequential number in the emulsion layer
	Int_t      viewid   ;   // id of the view this M.i.p. track belongs to.
}
//___________________________________________________________________________//
namespace MIPBaseTrack
{
	UInt_t     id       ;    
	UInt_t     AreaSum  ;    
	UInt_t     Count    ;    
	Double_t   X        ;    
	Double_t   Y        ;    
	Double_t   Z        ;    
	Double_t   SX       ;    
	Double_t   SY       ;    
	Double_t   SZ       ;    
	Double_t   Sigma    ;    
	Int_t      TopId    ;   // Id of the track, e.g. its sequential number in the scanning zone. Top track connected to the base track.
	Int_t      BottomId ;   // Id of the track, e.g. its sequential number in the scanning zone. Bottom track connected to the base track.
}
//___________________________________________________________________________//
namespace TrackIndexEntry
{
	// Stores information about the origin of an emulsion track in the Raw Data (RWD) files.
	Int_t     Fragment ;  // The index of the fragment to which the original raw microtrack belongs.
	Int_t     View     ;  // The number of the view in the fragment to which the original raw microtrack belongs.
	Int_t     Track    ;  // The number of the track in the view to which the original raw microtrack belongs.
}


//___________________________________________________________________________//
void print_Header()
{

	using namespace Header ;
	cout << "Header"<< endl;
	cout << (Int_t) infotype << endl;
	cout << headerformat << endl;

	using namespace Header::Identifier;
	cout << Part0 <<" "<< Part1 <<" "<< Part2 <<" "<< Part3 << endl;
	using namespace Header::Center;
	cout << X <<" "<< Y << endl;
	using namespace Header::Extents ;
	cout << MinX <<" "<< MaxX <<" "<< MinY <<" "<< MaxY << endl; 
	using namespace Header::Transform ;
	cout << MXX  <<" "<< MXY  <<" "<< MYX  <<" "<< MYY  << endl; 
	cout << TX   <<" "<< TY   <<" "<< RX   <<" "<< RY   << endl; 

	using namespace Header;
	cout << TopViewsLength  <<" "<< BottomViewsLength  << endl ;
	cout << TopTracksLength <<" "<< BottomTracksLength <<" "<< TracksLength << endl;   
	cout << Top_TopZ <<" "<< Top_BottomZ <<" "<< Bottom_TopZ <<" "<< Bottom_BottomZ << endl; 

}
//___________________________________________________________________________//
void print_View(int side, int i)
{
	using namespace View ;
	//if (i<2 || i>848)
	cout << "View " << side <<" "<< id <<" "<< topz <<" "<< bottomz <<" "<< px <<" "<< py << endl;
} ;
//___________________________________________________________________________//
void print_MIPEmulsionTrack(int s, int i)
{
	using namespace MIPEmulsionTrack ;
	cout << "MIPEmulsionTrack " << s      <<" "<< i       <<" " ;
	cout << Field  <<" "<< AreaSum <<" "<< Count    <<" "
		<< X      <<" "<< Y       <<" "<< Z        <<" "
		<< SX     <<" "<< SY      <<" "<< SZ       <<" "
		<< Sigma  <<" "<< TopZ    <<" "<< BottomZ  <<" "<< viewid << endl;
} ;
//___________________________________________________________________________//
void print_MIPBaseTrack(int i)
{
	using namespace MIPBaseTrack ;
	cout << "MIPBaseTrack " << i       <<" "<< id      <<" " ;
	cout << AreaSum <<" "<< Count   <<" "
		<< X       <<" "<< Y       <<" "<< Z        <<" "
		<< SX      <<" "<< SY      <<" "<< SZ      <<" " 
		<< Sigma   <<" "<< TopId   <<" "<< BottomId  << endl;
} ;
//___________________________________________________________________________//
void printrackIndexEntry(int s, int i )
{
	using namespace TrackIndexEntry ;
	cout << "TrackIndexEntry " << s <<" "<< i <<" "<< TrackIndexEntry::Fragment <<" "
		<< TrackIndexEntry::View <<" "<< TrackIndexEntry::Track << endl;
} ;


//___________________________________________________________________________//
void Add_MIPEmulsionTrack( en_side side, int i  )
{
	using namespace MIPEmulsionTrack ;

	if ( side == TOP) {
		new( segtop[i] ) EdbSegP( i , X, Y, SX, SY, Count ) ;
		((EdbSegP*) segtop[i])->SetZ( Z );
		//((EdbSegP*) segtop[i])->SetZ( Header::Top_BottomZ );
		((EdbSegP*) segtop[i])->SetChi2( Sigma );
		((EdbSegP*) segtop[i])->SetVolume( AreaSum ) ;

		((EdbSegP*) segtop[i])->SetAid ( -91 , viewid );
		((EdbSegP*) segtop[i])->SetFlag( -91 );
		((EdbSegP*) segtop[i])->SetDZ( TopZ-BottomZ );
		((EdbSegP*) segtop[i])->SetDZem( 0 );
	}

	if ( side == BOT) {
		new( segbot[i] ) EdbSegP( i , X, Y, SX, SY, Count ) ;
		((EdbSegP*) segbot[i])->SetZ( Z );
		//(EdbSegP*) segtop[i])->SetZ( Header::Bottom_TopZ ); 
		((EdbSegP*) segbot[i])->SetChi2( Sigma );
		((EdbSegP*) segbot[i])->SetVolume( AreaSum ) ;

		((EdbSegP*) segbot[i])->SetAid ( -92 , viewid );
		((EdbSegP*) segbot[i])->SetFlag( -92 );
		((EdbSegP*) segbot[i])->SetDZ( TopZ-BottomZ );
		((EdbSegP*) segbot[i])->SetDZem( 0 );
	}
}

//___________________________________________________________________________//
void Add_MIPBaseTrack(int i, float project_dz= -105.)
{
	using namespace MIPBaseTrack ;

	EdbSegP* t = (EdbSegP*) segtop[ TopId ] ;

	EdbSegP* b = (EdbSegP*) segbot[ BottomId ] ;

	float x_ = X + SX*( project_dz ) ;
	float y_ = Y + SY*( project_dz ) ;

	float z_ = Z + project_dz ;

	s = new EdbSegP( i , x_, y_, SX, SY, Count ) ;
	s->SetChi2( Sigma ) ;
	s->SetVolume( AreaSum ) ;
	s->SetZ( z_ ) ;

	s->SetAid ( -90 , -90 );
	s->SetFlag( -90  );
	s->SetDZ(  b->Z() - t->Z()  );
	s->SetDZem( 0 ) ;

	Pid1=1;
	Pid2=2;
	Xv=0;
	Yv=0;
	cp= new EdbSegCouple( TopId, BottomId );
	cp->SetN1(1);
	cp->SetN2(1);
	cp->SetN1tot(1);
	cp->SetN2tot(1);
	cp->SetCHI2P( Sigma ) ;

	s1 = new EdbSegP( *t );

	s2 = new EdbSegP( *b );

	couples->Fill();
}

//___________________________________________________________________________//
void tlg2couples( const char* input_file , 
				 char* output_file = 0 ,
				 float project_dz= -105. ) 
{
	if (!output_file) {
		output_file = new char[256];
		sprintf(output_file, "%s", input_file );
		char* ext = strrchr(output_file,'.') ;
		sprintf(ext,".cp.root\0");
	}

	// CREATING OUTPUT FILE
	TFile *o = new TFile( output_file, "RECREATE");

	couples = new TTree("couples","couples");

	couples->Branch( "pid1" , &Pid1 , "pid1/I" ) ;
	couples->Branch( "pid2" , &Pid2 , "pid2/I" ) ;
	couples->Branch( "xv"   , &Xv   , "xv/F"   ) ;
	couples->Branch( "yv"   , &Yv   , "yv/F"   ) ;

	couples->Branch( "cp"   , "EdbSegCouple" , &cp ,32000 ,99 ) ;
	couples->Branch( "s1."  , "EdbSegP"      , &s1 ,32000 ,99 ) ;
	couples->Branch( "s2."  , "EdbSegP"      , &s2 ,32000 ,99 ) ;
	couples->Branch( "s."   , "EdbSegP"      , &s  ,32000 ,99 ) ;


	// CREATE TEMPORARY ARRAY to store top and Bottom

	//base = new TClonesArray("EdbSegP", 100) ;
	if ( segtop.GetEntriesFast() ) segtop.Delete() ;
	if ( segbot.GetEntriesFast() ) segbot.Delete() ;

	// create dummy variables to read unused field 
	Char_t         dummych ;
	UShort_t       dummyus ;
	UInt_t         dummyui ;
	Float_t        dummyf  ;
	Double_t       dummyd  ;

	// Initialize the transformation
	Header::Transform::MXX = 1 ;
	Header::Transform::MXY = 0 ;
	Header::Transform::MYX = 0 ;
	Header::Transform::MYY = 1 ;
	Header::Transform::TX  = 0 ;
	Header::Transform::TY  = 0 ;
	Header::Transform::RX  = 0 ;
	Header::Transform::RY  = 0 ;

	// Read File
	//fstream infile(input_file, ios::in | ios::binary);
	fstream infile(input_file, ios::in | ios::binary);

	using namespace Header;
	infile.read( (Char_t*) &infotype              , sizeof(Char_t) );
	infile.read( (Char_t*) &headerformat          , sizeof(UShort_t) );

	if (infotype == ((Char_t) File::Info::Track | (Char_t)File::Section::Header))
		switch (headerformat)
	{
		case (UShort_t) File::Format::MultiSection:				break;
		case (UShort_t) File::Format::Detailed :                break;
		case (UShort_t) File::Format::NormalDouble_tWithIndex : break;
		case (UShort_t) File::Format::NormalWithIndex :         break;
		case (UShort_t) File::Format::Normal :                  break;
		case (UShort_t) File::Format::NoExtents :               break;	
		case (UShort_t) File::Format::Old :                     break;
		case (UShort_t) File::Format::Old2 :                    break;

		default: { cout << "Unknown format\n"; return ; }
	};

	if (headerformat == (UShort_t)File::Format::MultiSection)
	{
		infile.read( (Char_t*) &dummych       , sizeof(Char_t)           );
		if (dummych != SectionTag) 
		{
			cout << "The first section in a TLG file must contain tracks!" << endl;
			return;
		}
		infile.read( (Char_t*) &dummyd       , sizeof(Double_t)            );
	}

	int i;

	Header::Transform::MXX = Header::Transform::MYY = 1.0;
	Header::Transform::MXY = Header::Transform::MYX = 0.0;
	Header::Transform::TX = Header::Transform::TY = 0.0;
	Header::Transform::RX = Header::Transform::RY= 0.0;

	if (headerformat == (UShort_t) File::Format::Old)
	{
		infile.read( (Char_t*) &Identifier::Part3       , sizeof(Int_t)            );
		Identifier::Part0 = Identifier::Part1 = Identifier::Part2 = 0;
	}
	else
	{
		infile.read( (Char_t*) &Identifier::Part0       , sizeof(Int_t)            );
		infile.read( (Char_t*) &Identifier::Part1       , sizeof(Int_t)            );
		infile.read( (Char_t*) &Identifier::Part2       , sizeof(Int_t)            );
		infile.read( (Char_t*) &Identifier::Part3       , sizeof(Int_t)            );
	}

	if (headerformat == (UShort_t) File::Format::Detailed || headerformat == (UShort_t)File::Format::MultiSection)
	{
		infile.read( (Char_t*) &Center::X             , sizeof(Double_t)         );
		infile.read( (Char_t*) &Center::Y             , sizeof(Double_t)         );
		infile.read( (Char_t*) &Extents::MinX         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Extents::MaxX         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Extents::MinX         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Extents::MaxY         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::MXX        , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::MXY        , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::MYX        , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::MYY        , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::TX         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::TY         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::RX         , sizeof(Double_t)         );
		infile.read( (Char_t*) &Transform::RY         , sizeof(Double_t)         );


		infile.read( (Char_t*) &TopViewsLength        , sizeof(Int_t)            ); // n. of View
		infile.read( (Char_t*) &BottomViewsLength     , sizeof(Int_t)            ); // n. of View
		infile.read( (Char_t*) &Top_TopZ              , sizeof(Double_t)         );  // topext  
		infile.read( (Char_t*) &Top_BottomZ           , sizeof(Double_t)         );  // topInt_t  
		infile.read( (Char_t*) &Bottom_TopZ           , sizeof(Double_t)         );  // bottomInt_t
		infile.read( (Char_t*) &Bottom_BottomZ        , sizeof(Double_t)         );  // bottomext

		using namespace View ;
		for (int s=0; s<2; s++ ) {
			int nviews = ( s==0 ? TopViewsLength : BottomViewsLength );
			for (int i=0; i<nviews; i++ ) {
				infile.read( (Char_t*) &id        , sizeof(Int_t)            );
				infile.read( (Char_t*) &px        , sizeof(Double_t)         );         
				infile.read( (Char_t*) &py        , sizeof(Double_t)         );         
				infile.read( (Char_t*) &topz      , sizeof(Double_t)         );         
				infile.read( (Char_t*) &bottomz   , sizeof(Double_t)         );            
				// print_View( s, i ) ;
			}
		}

		infile.read( (Char_t*) &TopTracksLength       , sizeof(Int_t)            );
		infile.read( (Char_t*) &BottomTracksLength    , sizeof(Int_t)            );
		infile.read( (Char_t*) &TracksLength          , sizeof(Int_t)            );

		print_Header();

		using namespace MIPEmulsionTrack ;
		for (int s=0; s<2; s++ ) {
			en_side side = ( s==0? TOP : BOT ) ;
			int ntracks = ( s==0 ? TopTracksLength : BottomTracksLength );
			for (int i=0; i<ntracks; i++ ) {
				// 84 bytes
				infile.read( (Char_t*) &Field                 , sizeof(UInt_t)   );
				infile.read( (Char_t*) &AreaSum               , sizeof(UInt_t)   );   
				infile.read( (Char_t*) &Count                 , sizeof(UInt_t)   );   
				infile.read( (Char_t*) &X                     , sizeof(Double_t)         );         
				infile.read( (Char_t*) &Y                     , sizeof(Double_t)         );         
				infile.read( (Char_t*) &Z                     , sizeof(Double_t)         );         
				infile.read( (Char_t*) &SX                    , sizeof(Double_t)         );         
				infile.read( (Char_t*) &SY                    , sizeof(Double_t)         );         
				infile.read( (Char_t*) &SZ                    , sizeof(Double_t)         );         
				infile.read( (Char_t*) &Sigma                 , sizeof(Double_t)         );         
				infile.read( (Char_t*) &TopZ                  , sizeof(Double_t)         );         
				infile.read( (Char_t*) &BottomZ               , sizeof(Double_t)         );            
				infile.read( (Char_t*) &viewid                , sizeof(Int_t)            );            

				// print_MIPEmulsionTrack( s, i ) ;
				Add_MIPEmulsionTrack ( side, i ) ;
			}
		}

		using namespace MIPBaseTrack ;
		for (int i=0; i<TracksLength; i++ ) {
			// .. bytes
			infile.read( (Char_t*) &MIPBaseTrack::AreaSum    , sizeof(UInt_t)   );   
			infile.read( (Char_t*) &MIPBaseTrack::Count      , sizeof(UInt_t)   );   
			infile.read( (Char_t*) &MIPBaseTrack::X          , sizeof(Double_t)         );   
			infile.read( (Char_t*) &MIPBaseTrack::Y          , sizeof(Double_t)         );         
			infile.read( (Char_t*) &MIPBaseTrack::Z          , sizeof(Double_t)         );         
			infile.read( (Char_t*) &MIPBaseTrack::SX         , sizeof(Double_t)         );         
			infile.read( (Char_t*) &MIPBaseTrack::SY         , sizeof(Double_t)         );         
			infile.read( (Char_t*) &MIPBaseTrack::SZ         , sizeof(Double_t)         );         
			infile.read( (Char_t*) &MIPBaseTrack::Sigma      , sizeof(Double_t)         );         
			infile.read( (Char_t*) &MIPBaseTrack::TopId      , sizeof(Int_t)            );            
			infile.read( (Char_t*) &MIPBaseTrack::BottomId   , sizeof(Int_t)            );      

			// print_MIPBaseTrack( i ) ;
			Add_MIPBaseTrack( i , project_dz) ;
		} 

		using namespace TrackIndexEntry;
		for (int s=0; s<2; s++ ) {
			int ntracks = ( s==0 ? TopTracksLength : BottomTracksLength );
			for (int i=0; i<ntracks; i++ ) 
			{
				infile.read( (Char_t*) &TrackIndexEntry::Fragment  , sizeof(Int_t)            );            
				infile.read( (Char_t*) &TrackIndexEntry::View      , sizeof(Int_t)            );            
				infile.read( (Char_t*) &TrackIndexEntry::Track     , sizeof(Int_t)            );            

				// printrackIndexEntry( s , i ) ;
			}
		}
	}
	else if (headerformat == (UShort_t) File::Format::NormalDouble_tWithIndex )
	{
		using namespace Header;
		infile.read( (Char_t*) &Center::X           , sizeof(Double_t)  );
		infile.read( (Char_t*) &Center::Y           , sizeof(Double_t)  );
		infile.read( (Char_t*) &Extents::MinX       , sizeof(Double_t)  );
		infile.read( (Char_t*) &Extents::MaxX       , sizeof(Double_t)  );
		infile.read( (Char_t*) &Extents::MinX       , sizeof(Double_t)  );
		infile.read( (Char_t*) &Extents::MaxY       , sizeof(Double_t)  );
		infile.read( (Char_t*) &dummyf              , sizeof(Float_t)   ); // for format compliance

		// 16 bytes
		infile.read( (Char_t*) &TopTracksLength     , sizeof(Int_t)     ); // n. of MIPIndexedEmulsionTrack
		infile.read( (Char_t*) &BottomTracksLength  , sizeof(Int_t)     ); // n. of MIPIndexedEmulsionTrack
		infile.read( (Char_t*) &TracksLength        , sizeof(Int_t)     ); // n. of MIPBaseTrack
		infile.read( (Char_t*) &dummyui             , sizeof(UInt_t)    ); // count of fields

		// 34 bytes
		infile.read( (Char_t*) &Top_TopZ            , sizeof(Double_t)  );  // topext  
		infile.read( (Char_t*) &Top_BottomZ         , sizeof(Double_t)  );  // topInt_t  
		infile.read( (Char_t*) &Bottom_TopZ         , sizeof(Double_t)  );  // bottomInt_t
		infile.read( (Char_t*) &Bottom_BottomZ      , sizeof(Double_t)  );  // bottomext
		infile.read( (Char_t*) &dummych             , sizeof(Char_t)    );  // skip fields
		infile.read( (Char_t*) &dummych             , sizeof(Char_t)    );  // skip fields

		print_Header();

		using namespace MIPEmulsionTrack ;
		for (int s=0; s<2; s++ ) {
			en_side side = ( s==0? TOP : BOT ) ;
			int ntracks = ( s==0 ? TopTracksLength : BottomTracksLength );
			for (int i=0; i<ntracks; i++ ) {
				// 84 bytes
				infile.read( (Char_t*) &Field          , sizeof(UInt_t)    );
				infile.read( (Char_t*) &AreaSum        , sizeof(UInt_t)    );   
				infile.read( (Char_t*) &Count          , sizeof(UInt_t)    );   
				infile.read( (Char_t*) &X              , sizeof(Double_t)  );         
				infile.read( (Char_t*) &Y              , sizeof(Double_t)  );         
				infile.read( (Char_t*) &Z              , sizeof(Double_t)  );         
				infile.read( (Char_t*) &SX             , sizeof(Double_t)  );         
				infile.read( (Char_t*) &SY             , sizeof(Double_t)  );         
				infile.read( (Char_t*) &SZ             , sizeof(Double_t)  );         
				infile.read( (Char_t*) &Sigma          , sizeof(Double_t)  );         
				infile.read( (Char_t*) &TopZ           , sizeof(Double_t)  );         
				infile.read( (Char_t*) &BottomZ        , sizeof(Double_t)  );            

				//print_MIPEmulsionTrack( s, i ) ;
				Add_MIPEmulsionTrack ( side , i );
			}
		}

		using namespace MIPBaseTrack ;
		for (int i=0; i<TracksLength; i++ ) {
			// .. bytes
			infile.read( (Char_t*) &MIPBaseTrack::AreaSum   , sizeof(UInt_t)    );   
			infile.read( (Char_t*) &MIPBaseTrack::Count     , sizeof(UInt_t)    );   
			infile.read( (Char_t*) &MIPBaseTrack::X         , sizeof(Double_t)  );   
			infile.read( (Char_t*) &MIPBaseTrack::Y         , sizeof(Double_t)  );         
			infile.read( (Char_t*) &MIPBaseTrack::Z         , sizeof(Double_t)  );         
			infile.read( (Char_t*) &MIPBaseTrack::SX        , sizeof(Double_t)  );         
			infile.read( (Char_t*) &MIPBaseTrack::SY        , sizeof(Double_t)  );         
			infile.read( (Char_t*) &MIPBaseTrack::SZ        , sizeof(Double_t)  );         
			infile.read( (Char_t*) &MIPBaseTrack::Sigma     , sizeof(Double_t)  ); 
			if (headerformat == (UShort_t)File::Format::NoExtents) {
				// Int_tercept errors and slope errors
				for (int j=0; j<6; j++)
					infile.read( (Char_t*) &dummyd      , sizeof(Double_t)  );   
			}
			infile.read( (Char_t*) &MIPBaseTrack::TopId     , sizeof(Int_t)     );            
			infile.read( (Char_t*) &MIPBaseTrack::BottomId  , sizeof(Int_t)     );      

			//print_MIPBaseTrack( i ) ;
			Add_MIPBaseTrack( i , project_dz ) ;
		} 

		using namespace TrackIndexEntry ;
		for (int s=0; s<2; s++ ) {
			int ntracks = ( s==0 ? TopTracksLength : BottomTracksLength );
			for (int i=0; i<ntracks; i++ ) 
			{
				infile.read( (Char_t*) &TrackIndexEntry::Fragment  , sizeof(Int_t)   );            
				infile.read( (Char_t*) &TrackIndexEntry::View      , sizeof(Int_t)   );            
				infile.read( (Char_t*) &TrackIndexEntry::Track     , sizeof(Int_t)   );            

				//printrackIndexEntry( s , i ) ;
			}
		}
	}
	else
	{

		/*
		if (headerformat == (ushort)File.Format.Normal || headerformat == (ushort)File.Format.NormalWithIndex)
		{
		m_Extents.MinX = r.ReadSingle();
		m_Extents.MaxX = r.ReadSingle(); 
		m_Extents.MinY = r.ReadSingle();
		m_Extents.MaxY = r.ReadSingle(); 
		r.ReadSingle(); //skip a meaningless Float_t...
		}
		else
		{
		r.ReadSingle(); r.ReadSingle(); 
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); 
		m_Extents.MinX = m_Extents.MaxX = m_Center.X;
		m_Extents.MinY = m_Extents.MaxY = m_Center.Y;
		}

		View topview = new View();
		View bottomview = new View();
		MIPIndexedEmulsionTrack [] toptracks = new MIPIndexedEmulsionTrack[r.ReadUInt_t32()];
		MIPIndexedEmulsionTrack [] bottomtracks = new MIPIndexedEmulsionTrack[r.ReadUInt_t32()];
		m_Tracks = new MIPBaseTrack[r.ReadUInt_t32()];

		i = r.ReadInt_t32(); // count of fields

		Double_t topext, topInt_t, bottomInt_t, bottomext;
		topext = r.ReadSingle();
		topInt_t = r.ReadSingle();
		bottomInt_t = r.ReadSingle();
		bottomext = r.ReadSingle();

		r.ReadBytes(2 * i); // skip fields

		MIPIndexedEmulsionTrack [] tkarr = toptracks;
		View vw = topview;
		do
		{
		if (headerformat == (ushort)File.Format.Normal || headerformat == (ushort)File.Format.NormalWithIndex)
		{
		for (i = 0; i < tkarr.Length; i++)
		{
		MIPEmulsionTrackInfo info = new MIPEmulsionTrackInfo();
		info.Field = r.ReadUInt_t32();
		info.AreaSum = r.ReadUInt_t32();
		info.Count = (ushort)r.ReadUInt_t32();
		info.Int_tercept.X = r.ReadSingle();
		info.Int_tercept.Y = r.ReadSingle();
		info.Int_tercept.Z = r.ReadSingle();
		info.Slope.X = r.ReadSingle();
		info.Slope.Y = r.ReadSingle();
		info.Slope.Z = r.ReadSingle();
		info.Sigma = r.ReadSingle();
		info.TopZ = r.ReadSingle();
		info.BottomZ = r.ReadSingle();
		tkarr[i] = new MIPIndexedEmulsionTrack(info, i, vw);
		}
		}
		else
		{
		if (headerformat == (ushort)File.Format.NoExtents)
		for (i = 0; i < tkarr.Length; i++)
		{
		MIPEmulsionTrackInfo info = new MIPEmulsionTrackInfo();
		info.AreaSum = 0;
		info.Field = r.ReadUInt_t32();
		info.Count = (ushort)r.ReadUInt_t32();
		info.Int_tercept.X = r.ReadSingle();
		info.Int_tercept.Y = r.ReadSingle();
		info.Int_tercept.Z = r.ReadSingle();
		info.Slope.X = r.ReadSingle();
		info.Slope.Y = r.ReadSingle();
		info.Slope.Z = r.ReadSingle();
		info.Sigma = r.ReadSingle();
		info.TopZ = r.ReadSingle();
		info.BottomZ = r.ReadSingle();
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); // Int_tercept errors
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); // slope errors
		tkarr[i] = new MIPIndexedEmulsionTrack(info, i, vw);
		}
		else
		for (i = 0; i < tkarr.Length; i++)
		{											
		MIPEmulsionTrackInfo info = new MIPEmulsionTrackInfo();
		info.AreaSum = 0;
		info.Field = r.ReadUInt_t32();
		info.Count = (ushort)r.ReadUInt_t32();
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle();
		info.Int_tercept.X = r.ReadSingle();
		info.Int_tercept.Y = r.ReadSingle();
		info.Int_tercept.Z = r.ReadSingle();
		info.Slope.X = r.ReadSingle();
		info.Slope.Y = r.ReadSingle();
		info.Slope.Z = r.ReadSingle();
		info.Sigma = r.ReadSingle();
		info.TopZ = r.ReadSingle();
		info.BottomZ = r.ReadSingle();
		tkarr[i] = new MIPIndexedEmulsionTrack(info, i, vw);
		}
		}
		tkarr = (tkarr == toptracks) ? bottomtracks : null;
		vw = (vw == topview) ? bottomview : null;
		}
		while (tkarr != null);

		m_Top = new Side(topext, topInt_t);
		m_Bottom = new Side(bottomInt_t, bottomext);
		((Side)m_Top).SetViews(new View[1] { topview });
		((Side)m_Bottom).SetViews(new View[1] { bottomview });
		((Side)m_Top).SetTracks(toptracks);
		((Side)m_Bottom).SetTracks(bottomtracks);
		topview.m_Side = (Side)m_Top;
		bottomview.m_Side = (Side)m_Bottom;
		topview.m_Tracks = toptracks;
		bottomview.m_Tracks = bottomtracks;
		topview.m_Id = 0;
		bottomview.m_Id = 0;
		topview.m_Position.X = bottomview.m_Position.X = m_Center.X;
		topview.m_Position.Y = bottomview.m_Position.Y = m_Center.Y;
		topview.m_TopZ = m_Top.TopZ;
		topview.m_BottomZ = m_Top.BottomZ;
		bottomview.m_TopZ = m_Bottom.TopZ;
		bottomview.m_BottomZ = m_Bottom.BottomZ;

		if (headerformat == (ushort)File.Format.Normal || headerformat == (ushort)File.Format.NormalWithIndex)
		{
		for (i = 0; i < m_Tracks.Length; i++)
		{
		MIPEmulsionTrackInfo info = new MIPEmulsionTrackInfo();
		info.AreaSum = (ushort)r.ReadUInt_t32();
		info.Count = (ushort)r.ReadUInt_t32();
		info.Int_tercept.X = r.ReadSingle();
		info.Int_tercept.Y = r.ReadSingle();
		info.Int_tercept.Z = r.ReadSingle();
		info.Slope.X = r.ReadSingle();
		info.Slope.Y = r.ReadSingle();
		info.Slope.Z = r.ReadSingle();
		info.Sigma = r.ReadSingle();
		if (headerformat == (ushort)File.Format.NoExtents)
		{
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); // Int_tercept errors
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); // slope errors
		}
		MIPIndexedEmulsionTrack toptk = (MIPIndexedEmulsionTrack)m_Top.m_Tracks[r.ReadUInt_t32()];
		MIPIndexedEmulsionTrack bottomtk = (MIPIndexedEmulsionTrack)m_Bottom.m_Tracks[r.ReadUInt_t32()];
		info.TopZ = MIPIndexedEmulsionTrack.GetInfo(toptk).TopZ;
		info.BottomZ = MIPIndexedEmulsionTrack.GetInfo(bottomtk).BottomZ;
		m_Tracks[i] = new MIPBaseTrack(info, toptk, bottomtk, i);
		}
		}
		else
		{
		if (headerformat == (ushort)File.Format.NoExtents)
		{
		for (i = 0; i < m_Tracks.Length; i++)
		{
		MIPEmulsionTrackInfo info = new MIPEmulsionTrackInfo();
		info.AreaSum = 0;
		info.Count = (ushort)r.ReadUInt_t32();
		info.Int_tercept.X = r.ReadSingle();
		info.Int_tercept.Y = r.ReadSingle();
		info.Int_tercept.Z = r.ReadSingle();
		info.Slope.X = r.ReadSingle();
		info.Slope.Y = r.ReadSingle();
		info.Slope.Z = r.ReadSingle();
		info.Sigma = r.ReadSingle();
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); // Int_tercept errors
		r.ReadSingle(); r.ReadSingle(); r.ReadSingle(); // slope errors
		MIPIndexedEmulsionTrack toptk = (MIPIndexedEmulsionTrack)m_Top.m_Tracks[r.ReadUInt_t32()];
		MIPIndexedEmulsionTrack bottomtk = (MIPIndexedEmulsionTrack)m_Bottom.m_Tracks[r.ReadUInt_t32()];
		info.TopZ = MIPIndexedEmulsionTrack.GetInfo(toptk).TopZ;
		info.BottomZ = MIPIndexedEmulsionTrack.GetInfo(bottomtk).BottomZ;										
		m_Tracks[i] = new MIPBaseTrack(info, toptk, bottomtk, i);
		}
		}
		else
		{
		for (i = 0; i < m_Tracks.Length; i++)
		{
		MIPEmulsionTrackInfo info = new MIPEmulsionTrackInfo();
		info.Count = (ushort)r.ReadUInt_t32();
		info.Int_tercept.X = r.ReadSingle();
		info.Int_tercept.Y = r.ReadSingle();
		info.Int_tercept.Z = r.ReadSingle();
		info.Slope.X = r.ReadSingle();
		info.Slope.Y = r.ReadSingle();
		info.Slope.Z = r.ReadSingle();
		info.Sigma = r.ReadSingle();										
		MIPIndexedEmulsionTrack toptk = (MIPIndexedEmulsionTrack)m_Top.m_Tracks[r.ReadUInt_t32()];
		MIPIndexedEmulsionTrack bottomtk = (MIPIndexedEmulsionTrack)m_Bottom.m_Tracks[r.ReadUInt_t32()];
		info.AreaSum = MIPIndexedEmulsionTrack.GetInfo(toptk).AreaSum + MIPIndexedEmulsionTrack.GetInfo(bottomtk).AreaSum;
		info.TopZ = MIPIndexedEmulsionTrack.GetInfo(toptk).TopZ;
		info.BottomZ = MIPIndexedEmulsionTrack.GetInfo(bottomtk).BottomZ;										
		m_Tracks[i] = new MIPBaseTrack(info, toptk, bottomtk, i);
		}
		}
		}
		if (headerformat == (ushort)File.Format.NormalWithIndex)
		{
		tkarr = (MIPIndexedEmulsionTrack [])m_Top.m_Tracks;
		while (true)
		{
		for (i = 0; i < tkarr.Length; i++)
		{
		tkarr[i].m_OriginalRawData.Fragment = r.ReadInt_t32();
		tkarr[i].m_OriginalRawData.View = r.ReadInt_t32();
		tkarr[i].m_OriginalRawData.Track = r.ReadInt_t32();
		}
		if (tkarr == (MIPIndexedEmulsionTrack [])m_Top.m_Tracks) tkarr = (MIPIndexedEmulsionTrack [])m_Bottom.m_Tracks; else break;
		}
		}
		*/
	}

	infile.close() ;

	o->Write(); 
	o->Close();

}
//___________________________________________________________________________//
#ifndef __CINT__
int main( int argc, char *argv[] )
{
	if(argc==2)       
		tlg2couples( argv[1],       0, -105. );
	else if(argc==3)  
		tlg2couples( argv[1], argv[2], -105. );
	else if(argc==4) {
		float project_dz ;
		sscanf( argv[3], "%f", &project_dz );
		tlg2couples( argv[1], argv[2], project_dz );
	} else {
		cout << "tlg2couples.exe <input_file> [output_file] [project_dz (default= -105.)]\n";
		return 0 ;
	}

	return 1 ;
}
#endif

