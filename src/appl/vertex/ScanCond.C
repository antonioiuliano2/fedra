//////////////////////////////////////////////
//
//   Collection of scanning conditions
//
//////////////////////////////////////////////

///------------------------------------------------------------
void Set_UTS_CHORUS_microtrack_bad( EdbScanCond *cond )
{
  cond->SetSigma0( 10., 10.,.035,.035 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 1. );                  // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,3,3);                 // bins in [sigma] for checks
  cond->SetPulsRamp0(  7000.,7000. );     // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 7000.,7000. );     //
  cond->SetName("UTS_CHORUS_microtrack_bad");
}

///------------------------------------------------------------
void Set_UTS_CHORUS_microtrack( EdbScanCond *cond )
{
  cond->SetSigma0( 2., 2.,.011,.013 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 1. );                  // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,3,3);                 // bins in [sigma] for checks
  cond->SetPulsRamp0(  100000.,130000. );     // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 100000.,120000. );     //
  cond->SetName("UTS_CHORUS_microtrack");
}

///------------------------------------------------------------
void Set_Prototype_OPERA_microtrack( EdbScanCond *cond )
{
  cond->SetSigma0( 2., 2.,.013,.013 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 6. );                  // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(3,3,3,3);                 // bins in [sigma] for checks
  cond->SetPulsRamp0(  6.,10. );     // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 5.,9. );     //
  cond->SetName("Prototype_OPERA_microtrack");
}

///------------------------------------------------------------
void Set_Prototype_OPERA_basetrack( EdbScanCond *cond )
{
  cond->SetSigma0( 9., 10.,.005,.006 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 5. );                 // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(3,3,3,3);                // bins in [sigma] for checks
  cond->SetPulsRamp0(  15.,15. );        // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 15.,15. );        //
  cond->SetName("Prototype_OPERA_basetrack");
}

///------------------------------------------------------------
void Set_Prototype_OPERA_basetrack_300( EdbScanCond *cond )
{
  cond->SetSigma0( 3., 3.,.005,.006 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 5. );                 // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,3,3);                // bins in [sigma] for checks
  cond->SetPulsRamp0(  13.,13. );        // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 13.,13. );        //
  cond->SetName("Prototype_OPERA_basetrack_300");
}

///------------------------------------------------------------
void Set_MC_microtrack( EdbScanCond *cond )
{
  cond->SetSigma0( 1., 1.,.025,.025 );  // sigma0 "x,y,tx,ty" at 0 angle
  cond->SetDegrad( 1. );                // sigma(tx) = sigma0*(1+degrad*tx)
  cond->SetBins(5,5,5,5);               // bins in [sigma] for checks
  cond->SetPulsRamp0(  4.,5. );         // in range (Pmin:Pmax) Signal/All is nearly linear
  cond->SetPulsRamp04( 3.,4. );         //
  cond->SetName("MC_microtrack");
}
