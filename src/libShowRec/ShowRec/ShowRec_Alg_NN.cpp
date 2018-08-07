//-------------------------------------------------------------------------------------------

void Create_NN_Alg_Histograms()
{
    var_NN__BG__SpatialDist_TestBT_To_InBT = new TH1F("var_NN__BG__SpatialDist_TestBT_To_InBT","var_NN__BG__SpatialDist_TestBT_To_InBT",1000,0,60000);
    var_NN__SG__SpatialDist_TestBT_To_InBT = new TH1F("var_NN__SG__SpatialDist_TestBT_To_InBT","var_NN__SG__SpatialDist_TestBT_To_InBT",1000,0,60000);
    var_NN__ALL__SpatialDist_TestBT_To_InBT = new TH1F("var_NN__ALL__SpatialDist_TestBT_To_InBT","var_NN__ALL__SpatialDist_TestBT_To_InBT",1000,0,60000);

    var_NN__BG__zDiff_TestBT_To_InBT = new TH1F("var_NN__BG__zDiff_TestBT_To_InBT","var_NN__BG__zDiff_TestBT_To_InBT",1000,0,60000);
    var_NN__SG__zDiff_TestBT_To_InBT = new TH1F("var_NN__SG__zDiff_TestBT_To_InBT","var_NN__SG__zDiff_TestBT_To_InBT",1000,0,60000);
    var_NN__ALL__zDiff_TestBT_To_InBT = new TH1F("var_NN__ALL__zDiff_TestBT_To_InBT","var_NN__ALL__zDiff_TestBT_To_InBT",1000,0,60000);


    var_NN__SG__dT_InBT_To_TestBT = new TH1F("var_NN__SG__dT_InBT_To_TestBT","var_NN__SG__dT_InBT_To_TestBT",100,0,1);
    var_NN__BG__dT_InBT_To_TestBT = new TH1F("var_NN__BG__dT_InBT_To_TestBT","var_NN__BG__dT_InBT_To_TestBT",100,0,1);
    var_NN__ALL__dT_InBT_To_TestBT = new TH1F("var_NN__ALL__dT_InBT_To_TestBT","var_NN__ALL__dT_InBT_To_TestBT",100,0,1);

    var_NN__SG__dR_InBT_To_TestBT = new TH1F("var_NN__SG__dR_InBT_To_TestBT","var_NN__SG__dR_InBT_To_TestBT",100,0,2000);
    var_NN__BG__dR_InBT_To_TestBT = new TH1F("var_NN__BG__dR_InBT_To_TestBT","var_NN__BG__dR_InBT_To_TestBT",100,0,2000);
    var_NN__ALL__dR_InBT_To_TestBT = new TH1F("var_NN__ALL__dR_InBT_To_TestBT","var_NN__ALL__dR_InBT_To_TestBT",100,0,2000);

    var_NN__SG__dR_TestBT_To_InBT = new TH1F("var_NN__SG__dR_TestBT_To_InBT","var_NN__SG__dR_TestBT_To_InBT",100,0,2000);
    var_NN__BG__dR_TestBT_To_InBT = new TH1F("var_NN__BG__dR_TestBT_To_InBT","var_NN__BG__dR_TestBT_To_InBT",100,0,2000);
    var_NN__ALL__dR_TestBT_To_InBT = new TH1F("var_NN__ALL__dR_TestBT_To_InBT","var_NN__ALL__dR_TestBT_To_InBT",100,0,2000);



    var_NN__SG__nseg_TestBT_To2BeforePlate = new TH1F("var_NN__SG__nseg_TestBT_To2BeforePlate","var_NN__SG__nseg_TestBT_To2BeforePlate",10,0,10);
    var_NN__BG__nseg_TestBT_To2BeforePlate = new TH1F("var_NN__BG__nseg_TestBT_To2BeforePlate","var_NN__BG__nseg_TestBT_To2BeforePlate",10,0,10);
    var_NN__ALL__nseg_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__nseg_TestBT_To2BeforePlate","var_NN__ALL__nseg_TestBT_To2BeforePlate",10,0,10);

    var_NN__BG__nseg_TestBT_ToBeforePlate = new TH1F("var_NN__BG__nseg_TestBT_ToBeforePlate","var_NN__BG__nseg_TestBT_ToBeforePlate",10,0,10);
    var_NN__SG__nseg_TestBT_ToBeforePlate = new TH1F("var_NN__SG__nseg_TestBT_ToBeforePlate","var_NN__SG__nseg_TestBT_ToBeforePlate",10,0,10);
    var_NN__ALL__nseg_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__nseg_TestBT_ToBeforePlate","var_NN__ALL__nseg_TestBT_ToBeforePlate",10,0,10);

    var_NN__BG__nseg_TestBT_ToSamePlate = new TH1F("var_NN__BG__nseg_TestBT_ToSamePlate","var_NN__BG__nseg_TestBT_ToSamePlate",10,0,10);
    var_NN__SG__nseg_TestBT_ToSamePlate = new TH1F("var_NN__SG__nseg_TestBT_ToSamePlate","var_NN__SG__nseg_TestBT_ToSamePlate",10,0,10);
    var_NN__ALL__nseg_TestBT_ToSamePlate = new TH1F("var_NN__ALL__nseg_TestBT_ToSamePlate","var_NN__ALL__nseg_TestBT_ToSamePlate",10,0,10);

    var_NN__BG__nseg_TestBT_To2AfterPlate = new TH1F("var_NN__BG__nseg_TestBT_To2AfterPlate","var_NN__BG__nseg_TestBT_To2AfterPlate",10,0,10);
    var_NN__SG__nseg_TestBT_To2AfterPlate = new TH1F("var_NN__SG__nseg_TestBT_To2AfterPlate","var_NN__SG__nseg_TestBT_To2AfterPlate",10,0,10);
    var_NN__ALL__nseg_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__nseg_TestBT_To2AfterPlate","var_NN__ALL__nseg_TestBT_To2AfterPlate",10,0,10);

    var_NN__BG__nseg_TestBT_ToAfterPlate = new TH1F("var_NN__BG__nseg_TestBT_ToAfterPlate","var_NN__BG__nseg_TestBT_ToAfterPlate",10,0,10);
    var_NN__SG__nseg_TestBT_ToAfterPlate = new TH1F("var_NN__SG__nseg_TestBT_ToAfterPlate","var_NN__SG__nseg_TestBT_ToAfterPlate",10,0,10);
    var_NN__ALL__nseg_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__nseg_TestBT_ToAfterPlate","var_NN__ALL__nseg_TestBT_ToAfterPlate",10,0,10);



    var_NN__BG__mean_dT_TestBT_ToSamePlate = new TH1F("var_NN__BG__mean_dT_TestBT_ToSamePlate","var_NN__BG__mean_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__SG__mean_dT_TestBT_ToSamePlate = new TH1F("var_NN__SG__mean_dT_TestBT_ToSamePlate","var_NN__SG__mean_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_ToSamePlate = new TH1F("var_NN__ALL__mean_dT_TestBT_ToSamePlate","var_NN__ALL__mean_dT_TestBT_ToSamePlate",100,0,0.2);

    var_NN__BG__mean_dR_TestBT_ToSamePlate = new TH1F("var_NN__BG__mean_dR_TestBT_ToSamePlate","var_NN__BG__mean_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__SG__mean_dR_TestBT_ToSamePlate = new TH1F("var_NN__SG__mean_dR_TestBT_ToSamePlate","var_NN__SG__mean_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_ToSamePlate = new TH1F("var_NN__ALL__mean_dR_TestBT_ToSamePlate","var_NN__ALL__mean_dR_TestBT_ToSamePlate",100,0,800);


    var_NN__SG__mean_dR_TestBT_ToAfterPlate = new TH1F("var_NN__SG__mean_dR_TestBT_ToAfterPlate","var_NN__SG__mean_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_ToAfterPlate = new TH1F("var_NN__BG__mean_dR_TestBT_ToAfterPlate","var_NN__BG__mean_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__mean_dR_TestBT_ToAfterPlate","var_NN__ALL__mean_dR_TestBT_ToAfterPlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_ToAfterPlate = new TH1F("var_NN__SG__mean_dT_TestBT_ToAfterPlate","var_NN__SG__mean_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_ToAfterPlate = new TH1F("var_NN__BG__mean_dT_TestBT_ToAfterPlate","var_NN__BG__mean_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__mean_dT_TestBT_ToAfterPlate","var_NN__ALL__mean_dT_TestBT_ToAfterPlate",100,0,0.2);


    var_NN__SG__mean_dR_TestBT_To2AfterPlate = new TH1F("var_NN__SG__mean_dR_TestBT_To2AfterPlate","var_NN__SG__mean_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_To2AfterPlate = new TH1F("var_NN__BG__mean_dR_TestBT_To2AfterPlate","var_NN__BG__mean_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__mean_dR_TestBT_To2AfterPlate","var_NN__ALL__mean_dR_TestBT_To2AfterPlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_To2AfterPlate = new TH1F("var_NN__SG__mean_dT_TestBT_To2AfterPlate","var_NN__SG__mean_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_To2AfterPlate = new TH1F("var_NN__BG__mean_dT_TestBT_To2AfterPlate","var_NN__BG__mean_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__mean_dT_TestBT_To2AfterPlate","var_NN__ALL__mean_dT_TestBT_To2AfterPlate",100,0,0.2);

    var_NN__SG__mean_dR_TestBT_ToBeforePlate = new TH1F("var_NN__SG__mean_dR_TestBT_ToBeforePlate","var_NN__SG__mean_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_ToBeforePlate = new TH1F("var_NN__BG__mean_dR_TestBT_ToBeforePlate","var_NN__BG__mean_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__mean_dR_TestBT_ToBeforePlate","var_NN__ALL__mean_dR_TestBT_ToBeforePlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_ToBeforePlate = new TH1F("var_NN__SG__mean_dT_TestBT_ToBeforePlate","var_NN__SG__mean_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_ToBeforePlate = new TH1F("var_NN__BG__mean_dT_TestBT_ToBeforePlate","var_NN__BG__mean_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__mean_dT_TestBT_ToBeforePlate","var_NN__ALL__mean_dT_TestBT_ToBeforePlate",100,0,0.2);


    var_NN__SG__mean_dR_TestBT_To2BeforePlate = new TH1F("var_NN__SG__mean_dR_TestBT_To2BeforePlate","var_NN__SG__mean_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__BG__mean_dR_TestBT_To2BeforePlate = new TH1F("var_NN__BG__mean_dR_TestBT_To2BeforePlate","var_NN__BG__mean_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__ALL__mean_dR_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__mean_dR_TestBT_To2BeforePlate","var_NN__ALL__mean_dR_TestBT_To2BeforePlate",100,0,800);

    var_NN__SG__mean_dT_TestBT_To2BeforePlate = new TH1F("var_NN__SG__mean_dT_TestBT_To2BeforePlate","var_NN__SG__mean_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__BG__mean_dT_TestBT_To2BeforePlate = new TH1F("var_NN__BG__mean_dT_TestBT_To2BeforePlate","var_NN__BG__mean_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__ALL__mean_dT_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__mean_dT_TestBT_To2BeforePlate","var_NN__ALL__mean_dT_TestBT_To2BeforePlate",100,0,0.2);

    //---------

    var_NN__BG__min_dT_TestBT_ToSamePlate = new TH1F("var_NN__BG__min_dT_TestBT_ToSamePlate","var_NN__BG__min_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__SG__min_dT_TestBT_ToSamePlate = new TH1F("var_NN__SG__min_dT_TestBT_ToSamePlate","var_NN__SG__min_dT_TestBT_ToSamePlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_ToSamePlate = new TH1F("var_NN__ALL__min_dT_TestBT_ToSamePlate","var_NN__ALL__min_dT_TestBT_ToSamePlate",100,0,0.2);

    var_NN__BG__min_dR_TestBT_ToSamePlate = new TH1F("var_NN__BG__min_dR_TestBT_ToSamePlate","var_NN__BG__min_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__SG__min_dR_TestBT_ToSamePlate = new TH1F("var_NN__SG__min_dR_TestBT_ToSamePlate","var_NN__SG__min_dR_TestBT_ToSamePlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_ToSamePlate = new TH1F("var_NN__ALL__min_dR_TestBT_ToSamePlate","var_NN__ALL__min_dR_TestBT_ToSamePlate",100,0,800);


    var_NN__SG__min_dR_TestBT_ToAfterPlate = new TH1F("var_NN__SG__min_dR_TestBT_ToAfterPlate","var_NN__SG__min_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__BG__min_dR_TestBT_ToAfterPlate = new TH1F("var_NN__BG__min_dR_TestBT_ToAfterPlate","var_NN__BG__min_dR_TestBT_ToAfterPlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__min_dR_TestBT_ToAfterPlate","var_NN__ALL__min_dR_TestBT_ToAfterPlate",100,0,800);

    var_NN__SG__min_dT_TestBT_ToAfterPlate = new TH1F("var_NN__SG__min_dT_TestBT_ToAfterPlate","var_NN__SG__min_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_ToAfterPlate = new TH1F("var_NN__BG__min_dT_TestBT_ToAfterPlate","var_NN__BG__min_dT_TestBT_ToAfterPlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_ToAfterPlate = new TH1F("var_NN__ALL__min_dT_TestBT_ToAfterPlate","var_NN__ALL__min_dT_TestBT_ToAfterPlate",100,0,0.2);


    var_NN__SG__min_dR_TestBT_To2AfterPlate = new TH1F("var_NN__SG__min_dR_TestBT_To2AfterPlate","var_NN__SG__min_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__BG__min_dR_TestBT_To2AfterPlate = new TH1F("var_NN__BG__min_dR_TestBT_To2AfterPlate","var_NN__BG__min_dR_TestBT_To2AfterPlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__min_dR_TestBT_To2AfterPlate","var_NN__ALL__min_dR_TestBT_To2AfterPlate",100,0,800);

    var_NN__SG__min_dT_TestBT_To2AfterPlate = new TH1F("var_NN__SG__min_dT_TestBT_To2AfterPlate","var_NN__SG__min_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_To2AfterPlate = new TH1F("var_NN__BG__min_dT_TestBT_To2AfterPlate","var_NN__BG__min_dT_TestBT_To2AfterPlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_To2AfterPlate = new TH1F("var_NN__ALL__min_dT_TestBT_To2AfterPlate","var_NN__ALL__min_dT_TestBT_To2AfterPlate",100,0,0.2);

    var_NN__SG__min_dR_TestBT_ToBeforePlate = new TH1F("var_NN__SG__min_dR_TestBT_ToBeforePlate","var_NN__SG__min_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__BG__min_dR_TestBT_ToBeforePlate = new TH1F("var_NN__BG__min_dR_TestBT_ToBeforePlate","var_NN__BG__min_dR_TestBT_ToBeforePlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__min_dR_TestBT_ToBeforePlate","var_NN__ALL__min_dR_TestBT_ToBeforePlate",100,0,800);

    var_NN__SG__min_dT_TestBT_ToBeforePlate = new TH1F("var_NN__SG__min_dT_TestBT_ToBeforePlate","var_NN__SG__min_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_ToBeforePlate = new TH1F("var_NN__BG__min_dT_TestBT_ToBeforePlate","var_NN__BG__min_dT_TestBT_ToBeforePlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_ToBeforePlate = new TH1F("var_NN__ALL__min_dT_TestBT_ToBeforePlate","var_NN__ALL__min_dT_TestBT_ToBeforePlate",100,0,0.2);


    var_NN__SG__min_dR_TestBT_To2BeforePlate = new TH1F("var_NN__SG__min_dR_TestBT_To2BeforePlate","var_NN__SG__min_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__BG__min_dR_TestBT_To2BeforePlate = new TH1F("var_NN__BG__min_dR_TestBT_To2BeforePlate","var_NN__BG__min_dR_TestBT_To2BeforePlate",100,0,800);
    var_NN__ALL__min_dR_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__min_dR_TestBT_To2BeforePlate","var_NN__ALL__min_dR_TestBT_To2BeforePlate",100,0,800);

    var_NN__SG__min_dT_TestBT_To2BeforePlate = new TH1F("var_NN__SG__min_dT_TestBT_To2BeforePlate","var_NN__SG__min_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__BG__min_dT_TestBT_To2BeforePlate = new TH1F("var_NN__BG__min_dT_TestBT_To2BeforePlate","var_NN__BG__min_dT_TestBT_To2BeforePlate",100,0,0.2);
    var_NN__ALL__min_dT_TestBT_To2BeforePlate = new TH1F("var_NN__ALL__min_dT_TestBT_To2BeforePlate","var_NN__ALL__min_dT_TestBT_To2BeforePlate",100,0,0.2);

    return;
}

//-------------------------------------------------------------------------------------------

void Delete_NN_Alg_Histograms()
{
  delete var_NN__BG__SpatialDist_TestBT_To_InBT;
delete var_NN__SG__SpatialDist_TestBT_To_InBT;
delete var_NN__ALL__SpatialDist_TestBT_To_InBT;
delete var_NN__BG__zDiff_TestBT_To_InBT;
delete var_NN__SG__zDiff_TestBT_To_InBT;
delete var_NN__ALL__zDiff_TestBT_To_InBT;
// -------------------------------
delete var_NN__BG__dT_InBT_To_TestBT;
delete var_NN__SG__dT_InBT_To_TestBT;
delete var_NN__ALL__dT_InBT_To_TestBT; 
delete var_NN__SG__dR_InBT_To_TestBT;
delete var_NN__BG__dR_InBT_To_TestBT;
delete var_NN__ALL__dR_InBT_To_TestBT; 
delete var_NN__SG__dR_TestBT_To_InBT;
delete var_NN__BG__dR_TestBT_To_InBT;
delete var_NN__ALL__dR_TestBT_To_InBT; 
// -------------------------------
delete var_NN__SG__nseg_TestBT_ToBeforePlate;
delete var_NN__BG__nseg_TestBT_ToBeforePlate;
delete var_NN__ALL__nseg_TestBT_ToBeforePlate;
delete var_NN__SG__nseg_TestBT_To2BeforePlate;
delete var_NN__BG__nseg_TestBT_To2BeforePlate;
delete var_NN__ALL__nseg_TestBT_To2BeforePlate;
delete var_NN__BG__nseg_TestBT_ToSamePlate;
delete var_NN__SG__nseg_TestBT_ToSamePlate;
delete var_NN__ALL__nseg_TestBT_ToSamePlate;
delete var_NN__SG__nseg_TestBT_ToAfterPlate;
delete var_NN__BG__nseg_TestBT_ToAfterPlate;
delete var_NN__ALL__nseg_TestBT_ToAfterPlate;
delete var_NN__SG__nseg_TestBT_To2AfterPlate;
delete var_NN__BG__nseg_TestBT_To2AfterPlate;
delete var_NN__ALL__nseg_TestBT_To2AfterPlate;
delete var_NN__SG__mean_dR_TestBT_ToBeforePlate;
delete var_NN__BG__mean_dR_TestBT_ToBeforePlate;
delete var_NN__ALL__mean_dR_TestBT_ToBeforePlate;
delete var_NN__SG__mean_dR_TestBT_To2BeforePlate;
delete var_NN__BG__mean_dR_TestBT_To2BeforePlate;
delete var_NN__ALL__mean_dR_TestBT_To2BeforePlate;
delete var_NN__SG__mean_dT_TestBT_ToBeforePlate;
delete var_NN__BG__mean_dT_TestBT_ToBeforePlate;
delete var_NN__ALL__mean_dT_TestBT_ToBeforePlate;
delete var_NN__SG__mean_dT_TestBT_To2BeforePlate;
delete var_NN__BG__mean_dT_TestBT_To2BeforePlate;
delete var_NN__ALL__mean_dT_TestBT_To2BeforePlate;
delete var_NN__SG__mean_dR_TestBT_ToSamePlate;
delete var_NN__BG__mean_dR_TestBT_ToSamePlate;
delete var_NN__ALL__mean_dR_TestBT_ToSamePlate;
delete var_NN__SG__mean_dT_TestBT_ToSamePlate;
delete var_NN__BG__mean_dT_TestBT_ToSamePlate;
delete var_NN__ALL__mean_dT_TestBT_ToSamePlate;
delete var_NN__SG__mean_dR_TestBT_ToAfterPlate;
delete var_NN__BG__mean_dR_TestBT_ToAfterPlate;
delete var_NN__ALL__mean_dR_TestBT_ToAfterPlate;
delete var_NN__SG__mean_dT_TestBT_ToAfterPlate;
delete var_NN__BG__mean_dT_TestBT_ToAfterPlate;
delete var_NN__ALL__mean_dT_TestBT_ToAfterPlate;
delete var_NN__SG__mean_dR_TestBT_To2AfterPlate;
delete var_NN__BG__mean_dR_TestBT_To2AfterPlate;
delete var_NN__ALL__mean_dR_TestBT_To2AfterPlate;
delete var_NN__SG__mean_dT_TestBT_To2AfterPlate;
delete var_NN__BG__mean_dT_TestBT_To2AfterPlate;
delete var_NN__ALL__mean_dT_TestBT_To2AfterPlate;
delete var_NN__SG__min_dR_TestBT_ToBeforePlate;
delete var_NN__BG__min_dR_TestBT_ToBeforePlate;
delete var_NN__ALL__min_dR_TestBT_ToBeforePlate;
delete var_NN__SG__min_dR_TestBT_To2BeforePlate;
delete var_NN__BG__min_dR_TestBT_To2BeforePlate;
delete var_NN__ALL__min_dR_TestBT_To2BeforePlate;
delete var_NN__SG__min_dT_TestBT_ToBeforePlate;
delete var_NN__BG__min_dT_TestBT_ToBeforePlate;
delete var_NN__ALL__min_dT_TestBT_ToBeforePlate;
delete var_NN__SG__min_dT_TestBT_To2BeforePlate;
delete var_NN__BG__min_dT_TestBT_To2BeforePlate;
delete var_NN__ALL__min_dT_TestBT_To2BeforePlate;
delete var_NN__SG__min_dR_TestBT_ToSamePlate;
delete var_NN__BG__min_dR_TestBT_ToSamePlate;
delete var_NN__ALL__min_dR_TestBT_ToSamePlate;
delete var_NN__SG__min_dT_TestBT_ToSamePlate;
delete var_NN__BG__min_dT_TestBT_ToSamePlate;
delete var_NN__ALL__min_dT_TestBT_ToSamePlate;
delete var_NN__SG__min_dR_TestBT_ToAfterPlate;
delete var_NN__BG__min_dR_TestBT_ToAfterPlate;
delete var_NN__ALL__min_dR_TestBT_ToAfterPlate;
delete var_NN__SG__min_dT_TestBT_ToAfterPlate;
delete var_NN__BG__min_dT_TestBT_ToAfterPlate;
delete var_NN__ALL__min_dT_TestBT_ToAfterPlate;
delete var_NN__SG__min_dR_TestBT_To2AfterPlate;
delete var_NN__BG__min_dR_TestBT_To2AfterPlate;
delete var_NN__ALL__min_dR_TestBT_To2AfterPlate;
delete var_NN__SG__min_dT_TestBT_To2AfterPlate;
delete var_NN__BG__min_dT_TestBT_To2AfterPlate;
delete var_NN__ALL__min_dT_TestBT_To2AfterPlate;  
}

//-------------------------------------------------------------------------------------------

void Get_NN_ALG_MLP(TTree* simu, Int_t parasetnr)
{
    if (!gROOT->GetClass("TMultiLayerPerceptron")) {
        gSystem->Load("libMLP");
    }

//-----------------------------------------------------------------------------
//-
//-
//-    WE USE FOR NN_ALG and the PARAMETERSETS For it the following conventions:
//-
//-    NCUTSteps =   20;
//-    MLP Structure:
//-
//-    a)  Paraset [0..20[
//-           first five variables.
//-    b)  Paraset [20..40[
//-           first ten variables.
//-    c)  Paraset [40..60[
//-           all (30) variables.
//-    d)  ....(something else)
//-
//-    As Trainingssample we use EXP2500 Events, Padi 199 ... 196 with b11845 BG
//-    additionally as BG samples b99999GENERICBG  and b99998GENERICBG
//-    (both with no signal...)
//-
//-
//-    Take care thet the added Traingssample has about the same traingssampledata for BG and SG
//-
//-----------------------------------------------------------------------------

// Define NN_ALG charactereistic variables:
    Float_t dT_InBT_To_TestBT=0;
    Float_t dR_InBT_To_TestBT=0;
    Float_t dR_TestBT_To_InBT=0;
    Float_t zDist_TestBT_To_InBT=0;
    Float_t SpatialDist_TestBT_To_InBT=0;
    Float_t zDiff_TestBT_To_InBT=0;
    Float_t dT_NextBT_To_TestBT=0;
    Float_t dR_NextBT_To_TestBT=0;
    Float_t mean_dT_2before=0;
    Float_t mean_dR_2before=0;
    Float_t  mean_dT_before=0;
    Float_t mean_dR_before=0;
    Float_t  mean_dT_same=0;
    Float_t mean_dR_same=0;
    Float_t  mean_dT_after=0;
    Float_t mean_dR_after=0;
    Float_t  mean_dT_2after=0;
    Float_t  mean_dR_2after=0;

    Float_t min_dT_2before=0;
    Float_t min_dR_2before=0;
    Float_t  min_dT_before=0;
    Float_t min_dR_before=0;
    Float_t  min_dT_same=0;
    Float_t min_dR_same=0;
    Float_t  min_dT_after=0;
    Float_t min_dR_after=0;
    Float_t  min_dT_2after=0;
    Float_t  min_dR_2after=0;
    Int_t nseg_1before=0;
    Int_t nseg_2before=0;
    Int_t nseg_3before=0;
    Int_t nseg_1after=0;
    Int_t nseg_2after=0;
    Int_t nseg_3after=0;
    Int_t nseg_same=0;
    Int_t type;

    TString layout="";

    int ann_inputneurons=CUT_PARAMETER[1];

    if (ann_inputneurons==5) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,@SpatialDist_TestBT_To_InBT:6:5:type";
    }
    if (ann_inputneurons==10) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,SpatialDist_TestBT_To_InBT,nseg_2before,nseg_1before,nseg_same,nseg_1after,nseg_2after:11:10:type";
    }
    if (ann_inputneurons==20) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,SpatialDist_TestBT_To_InBT,nseg_2before,nseg_1before,nseg_same,nseg_1after,nseg_2after,mean_dT_2before,mean_dT_before,mean_dT_same,mean_dT_after,mean_dT_2after,mean_dR_2before,mean_dR_before,mean_dR_same,mean_dR_after,mean_dR_2after:21:20:type";
    }
    if (ann_inputneurons==30) {
        layout="@dT_InBT_To_TestBT,@dR_InBT_To_TestBT,@dR_TestBT_To_InBT,@zDiff_TestBT_To_InBT,SpatialDist_TestBT_To_InBT,nseg_2before,nseg_1before,nseg_same,nseg_1after,nseg_2after,mean_dT_2before,mean_dT_before,mean_dT_same,mean_dT_after,mean_dT_2after,mean_dR_2before,mean_dR_before,mean_dR_same,mean_dR_after,mean_dR_2after,min_dT_2before,min_dT_before,min_dT_same,min_dT_after,min_dT_2after,min_dR_2before,min_dR_before,min_dR_same,min_dR_after,min_dR_2after:31:30:type";
    }

    cout << "ann_inputneurons:   " << ann_inputneurons << endl;
    cout << "Layout of ANN:   " << layout << endl;

// Create the network:
    TMlpANN = new TMultiLayerPerceptron(layout,simu,"(Entry$)%2","(Entry$+1)%2");

    return;
}

//-------------------------------------------------------------------------------------------

void Load_NN_ALG_MLP_weights(TMultiLayerPerceptron* mlp, Int_t parasetnr)
{
    int ann_inputneurons=CUT_PARAMETER[1];

    if (ann_inputneurons==5) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_0_To_20.txt");
        gSystem->Exec("ls -ltr ANN_WEIGHTS_PARASET_0_To_20.txt");
    }
    if (ann_inputneurons==10) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_20_To_40.txt");
    }
    if (ann_inputneurons==20) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_40_To_60.txt");
    }
    if (ann_inputneurons==30) {
        mlp->LoadWeights("ANN_WEIGHTS_PARASET_60_To_80.txt");
    }

    mlp->GetStructure();

    return;
}

//-------------------------------------------------------------------------------------------
