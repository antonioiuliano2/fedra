//--------------------------------------------------------
// Example of chain-to-run subselection. 
// Some typical cases when it can be useful: 
// - run file splitted in more pieces due to 2Gb size limit
// - 2 or more intercalibration zones to be merged
// - prediction zones to be merged
// VT: 27/01/06
//--------------------------------------------------------

void chain2run( TChain &c, char *rout="r.root" )
{
  // input - chain of files(runs) - output is one run

  EdbRun r( rout, "RECREATE" );
  r.GetFile()->SetCompressionLevel(9);
  EdbView *v = r.GetView();

  c.SetBranchStatus("clusters",  0  );
  c.SetBranchAddress("frames",   v->GetFramesAddr()   );
  c.SetBranchAddress("headers",  v->GetHeaderAddr()   );
  c.SetBranchAddress("segments", v->GetSegmentsAddr() );

  int n=c.GetEntries();
  printf("entries=%d\n",n);
  for(int i=0; i<n; i++) {
    c.GetEntry(i);

    if( !GoodEvent(v) ) continue;      // selections
    v->GetHeader()->SetStatus( c.GetTreeNumber() );      // keep memory about each tree (if needed)
    r.AddView(v);
    if( !(i%1000) ) printf("%d (%d\%)\n",i,100*i/n);
  }
  r.Close();
}

//--------------------------------------------------------
bool GoodEvent(EdbView *v)
{
  // do all selections here

  if( v->Nsegments() < 0) return 0;
  TObjArray *segs = v->GetSegments();
  int n = v->Nsegments();
  for(int i=0; i<n; i++) {
    s = v->GetSegment(i);
    if( s->GetPuls()<8 )    
      { segs->RemoveAt(i); continue; }
    if( TMath::Sqrt(s->GetTx()*s->GetTx() + s->GetTy()*s->GetTy()) >0.5 ) 
      { segs->RemoveAt(i); continue;}
  }
  segs->Compress();

  return true;
}
