#include "TOracleServerE.h"
#include "TTree.h"
#include "EdbPattern.h"
#include "EdbAffine.h"

ClassImp(TOracleServerE)

//______________________________________________________________________________
Int_t TOracleServerE::QueryTree(char *query, TTree *tree, char *leafs)
{
  if (!IsConnected()) {
    Error("QueryTree", "not connected");
    return 0;
  }
  if (!tree) {
    Error("QueryTree", "tree is not valid");
    return 0;
  }
 
  try {
    if (!fStmt)
      fStmt = fConn->createStatement();
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    
    ResultSet *rset = fStmt->getResultSet();
    vector<MetaData> cmd = rset->getColumnListMetaData();
    const int  nlmax=cmd.size();
    printf("Number of metadata fields: %d\n", nlmax);
    if(nlmax<1) return 0;
    
    TString leaflist(nlmax*64);
    int ind=0;
    int *ifields = new int[nlmax];
    int dtype=0;
     
    for (int i = 0; i < nlmax; i++) {
      dtype = cmd[i].getInt(MetaData::ATTR_DATA_TYPE);
      printf("\nocci field type: %d \t",dtype);
      string s = cmd[i].getString(MetaData::ATTR_NAME);
      if (dtype == OCCI_SQLT_NUM )
	{
	  ifields[ind++] = i + 1;
	  if (ind > 1) leaflist+=":";
	  leaflist+= s.c_str();
	  leaflist+="/F";
	  printf("%s added", s.c_str());
	} else printf("%s skipped", s.c_str());
    }
    
    leaflist.ToLower();
    printf("\n\nleaflist=%s\n", leaflist.Data());
    if(leafs) {
      leaflist=leafs;
      printf("\n\nleaflist=%s\n", leaflist.Data());
    }
    Float_t *data = new Float_t[ind];
    tree->Branch("query", data, leaflist.Data() );
    
    while (rset->next() == 1)
      {
	for (int i = 0; i < ind; i++)
	  data[i] = rset->getFloat(ifields[i]);
	tree->Fill();
      }
    
    delete rset;
    delete[] ifields;
    delete[] data;
    return tree->GetEntries();
  } catch (SQLException &oraex)  {
    Error("TOracleServer", "QueryTree failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  return 0;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadVolume(ULong64_t id_volume, EdbPatternsVolume &vol)
{
	char id[24];
	sprintf(id,"%lld",id_volume);
	return ReadVolume(id,vol);
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadVolume(char *id_volume, EdbPatternsVolume &vol)
{
  int  nsegtot=0;
  char query[2048];
  TObjArray patterns(100);
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query,"\
    select id_plate,id_eventbrick from TB_VOLUME_SLICES where id_volume=%s order by id_plate", id_volume);
    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    int plate=0;
    int brick=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%d", &plate );
      sscanf( (rs->getString(2)).c_str(),"%d", &brick );
      pat = new EdbPattern(0.,0.,0);
      pat->SetPID(plate);
      pat->SetID(brick);
      patterns.Add( pat );
    }
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  int npat = patterns.GetEntries();
  for(int i=0; i<npat; i++) {
    pat = (EdbPattern*)patterns.At(i);
    if( !ReadZplate(pat->ID(), pat->PID(), *pat ) )
      { printf("skip plate %d %d !\n", pat->ID(),pat->PID()); continue; }
    sprintf(query,"id_zone in (select id_zone from TB_VOLUME_SLICES where id_volume=%s and id_plate=%d)", id_volume,pat->PID());
    nsegtot  += ReadBasetracksPattern(query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadDataSet(ULong64_t id_parent_op, int id_brick, EdbPatternsVolume &vol)
{
  int  nsegtot=0;
  char query[2048];
  int npat = 0;
  TObjArray patterns(100);
  ULong64_t *proc_list = new ULong64_t[100];
  EdbPattern *pat=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,"\
select to_char(op.id), op.id_plate, pl.z, op.success \
from tb_proc_operations op, tb_plates pl \
where op.id_eventbrick=%d and pl.id_eventbrick=%d and pl.id=op.id_plate and op.id_parent_operation=%lld \
order by op.id_plate",
	    id_brick, id_brick, id_parent_op);

    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();

    ULong64_t procop=0LL;
    float z=0;
    int plate=0;
    npat=0;
    while (rs->next()){
      sscanf( (rs->getString(1)).c_str(),"%lld", &procop );
      plate = rs->getInt(2);
      z     = rs->getFloat(3);
      proc_list[npat] = procop;
      npat++;
      pat = new EdbPattern(0.,0.,z);
      pat->SetPID(plate);
      patterns.Add( pat );
    }

  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadPatternsVolume; failed: (error: %s)", (oraex.getMessage()).c_str());
  }

  for(int i=0; i<npat; i++) {
    pat = (EdbPattern*)patterns.At(i);
    if( !ReadZplate(id_brick, pat->PID(), *pat ) )
      { printf("skip plate %d %d !\n", id_brick,pat->PID()); continue; }
    sprintf(query,"id_zone in (select id from tb_zones where id_processoperation=%lld and id_plate=%d)", proc_list[i],pat->PID()); 
    nsegtot  += ReadBasetracksPattern( query, *pat);
    vol.AddPattern(pat);
  }
  printf("%d segments extracted from db into PatternsVolume\n",nsegtot);

  return nsegtot;
}

//------------------------------------------------------------------------------------
bool  TOracleServerE::ReadZplate(int id_eventbrick, int id_plate, EdbPattern &pat)
{
  char *query= new char[2048];
  Float_t Z=0;
  EdbAffine2D *aff=0;
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();

    sprintf(query,
            "select z,MAPXX,MAPXY,MAPYX,MAPYY,MAPDX,MAPDY from tb_plates \
             where id_eventbrick=%d and id=%d",
            id_eventbrick, id_plate);
    fStmt->setSQL(query);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    while (rs->next()){
      Z = rs->getFloat(1);
      aff = new EdbAffine2D(rs->getFloat(2),
                            rs->getFloat(3),
                            rs->getFloat(4),
                            rs->getFloat(5),
                            rs->getFloat(6),
                            rs->getFloat(7));
    }
    delete rs;
    printf("z=%f    ",Z);
    pat.SetZ(Z);
    aff->Print();
  } catch (SQLException &oraex) {
    Error("TOracleServerE", "ReadBasetracksPattern; Affine failed: (error: %s)", (oraex.getMessage()).c_str());
    return false;
  }
  return true;
}

//------------------------------------------------------------------------------------
Int_t  TOracleServerE::ReadBasetracksPattern(char *selection, EdbPattern &pat)
{
  int ntracks=0;
  char query[2048];
  try{
    if (!fStmt)
      fStmt = fConn->createStatement();
    sprintf(query, "SELECT \
  ID_EVENTBRICK, id_zone, ID, POSX, POSY, SLOPEX, SLOPEY, GRAINS, AREASUM, PH, SIGMA \
  from tb_mipbasetracks where %s",
	    selection);
    fStmt->setSQL(query);
    fStmt->setPrefetchRowCount(2000);
    printf("\nexecute sql query: %s ...\n",query);
    fStmt->execute();
    ResultSet *rs = fStmt->getResultSet();
    EdbSegP seg;
    while (rs->next()){
      seg.Set(
	      rs->getInt(3),    //id
	      rs->getFloat(4),  //posx
	      rs->getFloat(5),  //posy
	      rs->getFloat(6),  //slopex
	      rs->getFloat(7),  //slopey
	      rs->getInt(8),    //grains
	      0                 //flag
	      );
      seg.SetZ(pat.Z());
      seg.SetDZ(300.);                          //!!!
      seg.SetVolume(rs->getInt(10));
      pat.AddSegment(seg);
      ntracks++;
    }
    delete rs;
  } catch (SQLException &oraex)  {
    Error("TOracleServerE", "ReadBasetracksPattern failed: (error: %s)", (oraex.getMessage()).c_str());
  }
  printf("%d segments are readed\n", ntracks);
  return ntracks;
}
