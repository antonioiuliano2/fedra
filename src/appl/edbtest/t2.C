void testwrite()
{
  char* edbname = "prova.root";

  EdbRun *run =new EdbRun(edbname,"RECREATE");
  EdbView *edbView;
  edbView= run->GetView();

  int nviews=30;
  int nsegments =200;
  int npoints=14;

  int ntracks=0;
  int nclusters=0;
  int s=1;

  float x, y, z, tx, ty, dz;  
  int   side, puls , id;

  for (int v = 0; v < nviews; v++)
    {
      edbView->Clear();
      for (int t = 0; t < nsegments; t++)
	{
	  EdbSegment* edbSegment = new EdbSegment();
	  int id = ntracks++;
	  x=y=t;
	  edbSegment->Set(x, y, z, tx, ty, dz, side, puls , id);
	  for (int n=0; n<npoints+v; n++)
	    {
	      EdbCluster* edbCluster = new EdbCluster(t,t,50*s+n,0, 0, 0, 0, id);
	      	      edbSegment->AddElement(edbCluster);     // <----------------- ?
	      edbView->AddCluster(edbCluster);
	      nclusters++;
	    }
	  edbView->AddSegment(edbSegment);
	}
      run->AddView(edbView);
    }

 //---------------------------------------------------------- CHECK
  int nseg=0;
  for(v=0;v<nviews;v++)
    {
      EdbView* View = run->GetEntry(v);
      nseg = View->Nsegments();
      for(int j=0;j<nseg;j++)
	{
	  EdbSegment* seg = View->GetSegment(j);
	  cout <<"view: "<<v<<"   seg: "<<j<<"  ID: "<< seg->GetID()<<"\t";
	  cout<<"\t\tNel: "<<seg->GetNelements() <<endl;
	}
    }
  //----------------------------------------------------------- OK GetNelements not 0
  
  run->Close();
  delete run; //<---- close everything (or exit ...)
}

void testread()
{
  //----------------------------------------------------------CHECK
  EdbRun *run2;
  run2 =new EdbRun("prova.root","READ");
  int nviews2 = run2->GetEntries();
  
  int nseg=0;
  for(int v=0;v<nviews2;v++)
    {
      EdbView* View = run2->GetEntry(v);
      nseg = View->Nsegments();
      for(int j=0;j<nseg;j++)
	{
	  EdbSegment* seg = View->GetSegment(j);
	  cout <<"v: "<<v<<"   j: "<<j<<"  ID: "<< seg->GetID()<<"\t";
	  cout<<"\t\tNel: "<<seg->GetNelements() <<endl;
	}
    }
  //-------------------------------------------------- GetNelements return 0,> WHY???
}
