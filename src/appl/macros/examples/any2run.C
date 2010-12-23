// VT: 26/03/2009
//---------------------------------------------------------------
// Minimal working example of the Views tree writing starting from scratch:
// Can be used for the converters development of for direct 
// data generation
//
// usage: root -b -q any2run.C
//        root -l run.root
//        root [1] Views.Draw("eTy:eTx")
//---------------------------------------------------------------

using namespace TMath;

void any2run( char *file = "run.root" )
{
  EdbRun r(file,"RECREATE");     // create new run file
  EdbView *view  = r.GetView();  // use this view as a container

  int nx=10, ny=10;   // generate 1 area of 10x10 views
  int ic=0;
  for(int ix=0; ix<nx; ix++) {
    for(int iy=0; iy<ny; iy++) {
      view->Clear();
      view->SetAreaID( 0 ); 
      view->SetCoordXY( ix*300, iy*300 );   // center of the view is here
      view->SetNframes(16,0);               // ntop, nbot

      fillViewWithSegments( *view );
//       fillViewWithClusters( *view );
//       fillViewWithFrames( *view );
//       fillViewWithImages( *view );

      r.AddView(view);
    }
  }

  r.Close();
}

//--------------------------------------------------------------
void fillViewWithSegments(EdbView &v)
{
  // generate segments inside 300x300 microns area in a local view coordinates

  EdbSegment s;
  for(int i=0; i<100; i++) {

    int   id = i;         // seg id
    float x = 300*gRandom->Rndm()-150;
    float y = 300*gRandom->Rndm()-150;
    float z = 0;
    float tx = gRandom->Gaus(0,0.3);
    float ty = gRandom->Gaus(0,0.3);
    float dz = 45;
    int side = 0;         // 0/1 for top/bot
    int puls = Min(gRandom->Poisson(10),16);

    s.Set( x, y, z, tx, ty, dz, side, puls, id);

    v.AddSegment(s);
  }

}
