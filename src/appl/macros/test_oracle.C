//-----------------------------------------------------------------
//
//  Several examples for access to ORACLE data from FEDRA
//  VT: 06-Dec-05
//
//-----------------------------------------------------------------

TOracleServerE *db=0;
EdbPVRec *ali=0;

//-----------------------------------------------------------------
void test_oracle()
{
  //get_aff();                    // minimal example of sql query to tree conversion
  //get_sb_hist();                // more elaborated example (selections may depends on lab)
  get_volume("8000000000900207"); // this function read total-scan volume into EdbPVRec object and save it
}

//-----------------------------------------------------------------
int init()
{
  db = (TOracleServerE *)TSQLServer::Connect("oracle://operasoft:1521/dbtest","opera","neutrino");
  printf("Server info: %s\n", db->ServerInfo());
  if(!db) return 0;
  return 1;
}

//-----------------------------------------------------------------
void get_aff()
{ 
  // minimal example of db query to tree conversion

  if(!db) if(!init()) return;
  TTree *t = new TTree("t","t");
  int n =db->QueryTree( "select * from tb_plates order by id",t);
  printf("%d entries\n",n);
  t->Draw("mapdx:z","calibration>0","*");
  delete db;
  db=0;
}

//-----------------------------------------------------------------
int get_sb_hist(char *id_proc="8000000001131121")
{
  // more elaborated example of db query to tree conversion

  if(!db) if(!init()) return;

  TFile *f=new TFile("sbh.root","RECREATE");
  TTree *t = new TTree("t","t");

  char *leaflist = new char[512];
  leaflist = "track/f:grains/f:fpx/f:fpy/f:fsx/f:fsy/f:ppx/f:ppy/f:psx/f:psy/f:z/f:plate/f:path/f:proc/f:brick/f";

  char *query = new char[2048];
  sprintf(query,"\
select id_track,grains,fpx,fpy,fsx,fsy,ppx,ppy,psx,psy,z,id_plate,path,id_processoperation-8000000000000000,id_eventbrick \ 
from vw_scanback_history \ 
where id_eventbrick=7 and ID_PROCESSOPERATION=%s and id_plate=1 and path in \
(select path from vw_scanback_history \
where id_eventbrick=7 and ID_PROCESSOPERATION=%s \
and id_plate=7 and abs(fpy-ppy)<50 and abs(fpx-ppx)<50) \
and ppy>25000 and ppy<75000 ", 
id_proc,id_proc);

  int n =db->QueryTree(query,t,leaflist);
  
  printf("%d entries\n",n);
  t->Write();
  t->Draw("ppx-fpx","grains>0");
  f->Close();
  delete db;  db=0;
}

//-----------------------------------------------------------------
get_volume(char *idproc)
{
  // example of the access to the scanned volume

  if(!db) if(!init()) return;

  ali  = new EdbPVRec();

  EdbPatternsVolume *v = (EdbPatternsVolume*)ali;

  int n = db->ReadVolume(idproc, *v);
  v->Print();
  TFile f("vol.root","RECREATE");
  ali->Write("ali");
  f.Close();
  delete db;  db=0;
}
                                                                                                                                                                
