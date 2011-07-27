//-- Author :  Valeri Tioukov   29/04/2011
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EdbProcPars                                                          //
//                                                                      //
//  processing parameters handling                                      //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
#include "EdbProcPars.h"

ClassImp(EdbProcPars)

//------------------------------------------------------------------------
void EdbProcPars::SetLinkDef(TEnv  &cenv)
{
  cenv.SetValue("fedra.link.BinOK"               , 6.   );
  cenv.SetValue("fedra.link.NcorrMin"            , 100  );
  cenv.SetValue("fedra.link.DoCorrectShrinkage"  , true );
  cenv.SetValue("fedra.link.shr.NsigmaEQ"        , 7.5  );
  cenv.SetValue("fedra.link.shr.Shr0"            , .85  );
  cenv.SetValue("fedra.link.shr.DShr"            , .3   );
  cenv.SetValue("fedra.link.shr.ThetaLimits","0.005  1.");
  cenv.SetValue("fedra.link.DoCorrectAngles"     , true );
  cenv.SetValue("fedra.link.ang.Chi2max"         , 1.5  );
  cenv.SetValue("fedra.link.DoFullLinking"       , true );
  cenv.SetValue("fedra.link.full.NsigmaEQ"       , 5.5  );
  cenv.SetValue("fedra.link.full.DR"             , 30.  );
  cenv.SetValue("fedra.link.full.DT"             , 0.1  );
  cenv.SetValue("fedra.link.full.CHI2Pmax"       , 3.   );
  cenv.SetValue("fedra.link.DoSaveCouples"       , true );
  cenv.SetValue("fedra.link.AFID"                ,  1   );   // 1 is usually fine for scanned data; for the db-read data use 0!
  cenv.SetValue("fedra.link.CheckUpDownOffset"   ,  1   );   // check dXdY offsets between up and correspondent down views
}
//------------------------------------------------------------------------
void EdbProcPars::SetAlignDef(TEnv &cenv)
{
  cenv.SetValue("fedra.align.OffsetMax"   , 1000. );
  cenv.SetValue("fedra.align.DZ"          ,  250. );
  cenv.SetValue("fedra.align.DPHI"        ,  0.02 );
  cenv.SetValue("fedra.align.SigmaR"      ,  25.  );
  cenv.SetValue("fedra.align.SigmaT"      ,  0.012);
  cenv.SetValue("fedra.align.DoFine"      ,  1    );
  cenv.SetValue("fedra.readCPcut"         , "eCHI2P<2.0&&s.eW>10&&eN1==1&&eN2==1&&s.Theta()>0.05&&s.Theta()<0.99");
  cenv.SetValue("fedra.align.SaveCouples" ,  1    );

  cenv.SetValue("emalign.outdir"          , ".."  );
  cenv.SetValue("emalign.env"             , "align.rootrc");
  cenv.SetValue("emalign.EdbDebugLevel"   ,  1    );
}
//------------------------------------------------------------------------
void EdbProcPars::SetTrackDef(TEnv &cenv)
{

}
