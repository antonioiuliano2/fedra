//-------------------------------------------------------------------------------------------

void PrintHelp() {
    Log(2, "ShowRec.cpp", "--- PrintHelp() ---");
    Log(2, "ShowRec.cpp", "--- PrintHelp()...done. ---");
    return;
}

//-------------------------------------------------------------------------------------------

void PrintPresetList(){
    cout << "void PrintPresetList() " << endl;
    cout << "   -PRESET0 " << endl;
    cout << "   -PRESET1 " << endl;
    cout << "   -PRESET2 " << endl;
    return;
}

//-------------------------------------------------------------------------------------------

void PrintValues_CommandLine() {

    Log(2, "ShowRec.cpp", "--- void PrintValues_CommandLine() ---");

    cout << "-----------------------------------------------"<<endl;
    cout << "--- " << setw(10) << "SWITCH"  << setw(10) << "VALUE" << setw(40) << "DESCRIPTION" << endl;

    cout << "--- " << setw(10) << " -FP"  << setw(10) << cmd_FP << setw(40) << "FirstPlate" << endl;
    cout << "--- " << setw(10) << " -LP"  << setw(10) << cmd_LP << setw(40) << "LastPlate" << endl;
    cout << "--- " << setw(10) << " -MP"  << setw(10) << cmd_MP << setw(40) << "MiddlePlate" << endl;
    cout << "--- " << setw(10) << " -NP"  << setw(10) << cmd_NP << setw(40) << "NumberofPlates" << endl;

    cout << "--- " << setw(10) << " -LT"  << setw(10) << cmd_LT << setw(40) << "use LinkedTracks.root" << endl;
    cout << "--- " << setw(10) << " -MC"  << setw(10) << cmd_MC << setw(40) << "use only MC Events for InBT" << endl;
    cout << "--- " << setw(10) << " -MCFL"  << setw(10) << cmd_MCFL << setw(40) << "use only MC Flag" << endl;
    cout << "--- " << setw(10) << " -HPLZ"  << setw(10) << cmd_HPLZ << setw(40) << "use highest P() or lowest Z() MC-InBTs" << endl;
    cout << "--- " << setw(10) << " -ALI"  << setw(10) << cmd_ALI << setw(40) << "source of patterns volume" << endl;

    cout << "--- " << setw(10) << " -MCMIX"  << setw(10) << cmd_MCMIX << setw(40) << "use MCMIX DANGEROUS !!! Be Careful !!!" << endl;
    cout << "--- " << setw(10) << " -EXTHETA"  << setw(10) << cmd_EXTHETA << setw(40) << "Extract Subpattern with " << endl;
    cout << "--- " << setw(10) << "         "  << setw(10) << cmd_EXTHETA << setw(40) << "Delta Theta Cut on Initiator BT angle " << endl;
    cout << "--- " << setw(10) << " -VTX"  << setw(10) << cmd_vtx << setw(40) << "For InBT: Cut to IP for MC vertex " << endl;
    cout << "--- " << setw(10) << " -PADI"  << setw(10) << cmd_PADI << setw(40) << "ParentDirectory" << endl;
    cout << "--- " << setw(10) << " -BTPA"  << setw(10) << cmd_BTPA << setw(40) << "BasetrackParametrisation" << endl;
    cout << "--- " << setw(10) << " -BGTP"  << setw(10) << cmd_BGTP << setw(40) << "BackgroundType" << endl;
    cout << "--- " << setw(10) << " -ALTP"  << setw(10) << cmd_ALTP << setw(40) << "AlgorithmType" << endl;
    cout << "--- " << setw(10) << " -PASTART"  << setw(10) << cmd_PASTART << setw(40) << "ParametersetStart" << endl;
    cout << "--- " << setw(10) << " -PAEND"  << setw(10) << cmd_PAEND << setw(40) << "ParameterSetEnd" << endl;
    cout << "--- " << setw(10) << " -CUTTP"  << setw(10) << cmd_CUTTP << setw(40) << "Algo Cuttype" << endl;
    cout << "--- " << setw(10) << " -CLEAN"  << setw(10) << cmd_CLEAN << setw(40) << "InputData BG Cleaning" << endl;

    cout << "--- " << setw(10) << " -FILETP"  << setw(10) << cmd_FILETP << setw(40) << "FILE Type tag" << endl;
    cout << "--- " << setw(10) << " -GBMC"  << setw(10) << cmd_GBMC << setw(40) << "Global MC Evt type tag " << endl;
    cout << "--- " << setw(10) << " -DEBUG"  << setw(10) << cmd_gEDBDEBUGLEVEL << setw(40) << "DEBUGLEVEL" << endl;
    cout << "--- " << setw(10) << " -OUT"  << setw(10) << cmd_OUTPUTLEVEL << setw(40) << "OUTPUTLEVEL" << endl;
    cout << "--- " << setw(10) << " -STOP"  << setw(10) << cmd_STOPLEVEL << setw(40) << "STOPLEVEL" << endl;


    cout << "-----------------------------------------------"<<endl<<endl;
    return;
}

//-------------------------------------------------------------------------------------------

void PrintShowerObjectArray(TObjArray* segarray)
{
    Log(3, "ShowRec.cpp", "--- Bool_t PrintShowerObjectArray() ---");

    if (gEDBDEBUGLEVEL>2) cout << "--- -----------------------------------------"<<endl;
    Int_t nent=segarray->GetEntries();
    if (gEDBDEBUGLEVEL>2) cout << "--- segarray->GetEntries:   " << nent << endl;
    EdbSegP* s;
    if (gEDBDEBUGLEVEL>2) printf("Entry X  Y  Z  TX  TY  W  Chi2  MCEvt  PID  P  Flag\n");
    for (int i=0; i<nent; i++)  {
        s=(EdbSegP*)segarray->At(i);
        if (gEDBDEBUGLEVEL>2) printf("%8d %14.3f  %14.3f  %14.3f  %7.4f  %7.4f  %7.4f  %7.4f  %8d %8d %6.2f %6d\n", i, s->X(), s->Y(), s->Z(), s->TX(), s->TY(),s->W(),s->Chi2(),s->MCEvt(), s->PID(), s->P(), s->Flag());
    }
    if (gEDBDEBUGLEVEL>2) cout << "--- -----------------------------------------"<<endl;
    Log(3, "ShowRec.cpp", "--- Bool_t PrintShowerObjectArray() done.");
    return;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
void CheckInputParameters()
{
    if (cmd_BTPA>4 ) {
        cout << "CheckInputParameters: cmd_BTPA>4 not supported. Here it has no effect but please check your nput!"<<endl;
        cout << "CheckInputParameters: WHAT TO DO HERE ???"<<endl;
    }
    if (cmd_BGTP>43) {
        cout << "CheckInputParameters: cmd_BGTP>43 not supported. Here it has no effect but pleas check your input!"<<endl;
        cout << "CheckInputParameters: WHAT TO DO HERE ???"<<endl;
    }
    if (cmd_ALTP>8) {
        cout << "CheckInputParameters:WARNING : cmd_ALTP>8 " << endl;
//         cout << "CheckInputParameters: cmd_ALTP>8 not supported. Please check your input!"<<endl;
//         cout << "CheckInputParameters: WHAT TO DO HERE ???"<<endl;
    }

    if (cmd_FP<1) {
        cout << "CheckInputParameters: cmd_FP<1. Set cmd_FP=1;"<<endl;
        cmd_FP=1;
    }
    if (cmd_LP>57) {
        cout << "CheckInputParameters: cmd_LP>57. Set cmd_LP=57;"<<endl;
        cmd_LP=57;
    }
    if (cmd_MP<cmd_FP) {
        cout << "CheckInputParameters: cmd_MP<cmd_FP. Set cmd_MP=cmd_FP;"<<endl;
        cmd_MP=cmd_FP;
        cout << "CheckInputParameters: New value: cmd_MP = "<< cmd_MP << endl;
    }
    if (cmd_FP>cmd_LP) {
        cout << "CheckInputParameters: cmd_FP>cmd_LP. Set cmd_FP=1;"<<endl;
        cmd_FP=1;
        cout << "CheckInputParameters: New value: cmd_FP = "<< cmd_FP << endl;
    }
    if (cmd_MP>cmd_LP) {
        cout << "CheckInputParameters: cmd_MP>cmd_LP. Set cmd_MP=cmd_LP;"<<endl;
        cmd_MP=cmd_LP;
        cout << "CheckInputParameters: New value: cmd_MP = "<< cmd_MP << endl;
    }

    if (cmd_MCFL<0) {
        cout << "CheckInputParameters: cmd_MCFL<0. Absolute Number will be taken;"<<endl;
    }

    if (cmd_gEDBDEBUGLEVEL>5) {
        cout << "CheckInputParameters: cmd_gEDBDEBUGLEVEL>5 not supported.  Here it has no effect but pleas check your input!"<<endl;
    }
    return;
}

//-------------------------------------------------------------------------------------------
void CheckInputParametersNEW()
{
    Log(2, "ShowRec.cpp", "--- void CheckInputParametersNEW() ---");
    Log(2, "ShowRec.cpp", "--- Assuming data object has been now read in ---");
    
    cout << "GLOBAL_gAli    at memory " << GLOBAL_gAli << endl;
    cout << "GLOBAL_gAli->Npatterns() " << GLOBAL_gAli->Npatterns() << endl;
    
    Int_t NPatterns = GLOBAL_gAli->Npatterns();

    if (cmd_FP<1) {
        cout << "CheckInputParameters: cmd_FP<1. Set cmd_FP=1;"<<endl;
        cmd_FP=1;
    }
    if (cmd_LP>NPatterns) {
        cout << "CheckInputParameters: cmd_LP>NPatterns. Set cmd_LP to "<< NPatterns << endl;
        cmd_LP=NPatterns;
    }
    if (cmd_MP<cmd_FP) {
        cout << "CheckInputParameters: cmd_MP<cmd_FP. Set cmd_MP=cmd_FP;"<<endl;
        cmd_MP=cmd_FP;
        cout << "CheckInputParameters: New value: cmd_MP = "<< cmd_MP << endl;
    }
    if (cmd_FP>cmd_LP) {
        cout << "CheckInputParameters: cmd_FP>cmd_LP. Set cmd_FP=1;"<<endl;
        cmd_FP=1;
        cout << "CheckInputParameters: New value: cmd_FP = "<< cmd_FP << endl;
    }
    if (cmd_MP>cmd_LP) {
        cout << "CheckInputParameters: cmd_MP>cmd_LP. Set cmd_MP=cmd_LP;"<<endl;
        cmd_MP=cmd_LP;
        cout << "CheckInputParameters: New value: cmd_MP = "<< cmd_MP << endl;
    }
    
    if (cmd_NP>NPatterns) {
        cout << "CheckInputParameters: cmd_NP>NPatterns. Set cmd_NP to "<< NPatterns << endl;
        cmd_NP=NPatterns;
    }

    return;
}



//-------------------------------------------------------------------------------------------
void InitCutParameters()
{
    Log(2, "ShowRec.cpp", "--- void InitCutParameters() ---");
    // Following the convention:
    // -FP -LP -MP -NP
    //  -PADI -BTPA -BGTP -ALTP
    // -LT -MC
    STREAM_ShowRecEff <<  setw(10) << "#PADI";
    STREAM_ShowRecEff <<  setw(10) << "BTPA";
    STREAM_ShowRecEff <<  setw(10) << "BGTP";
    STREAM_ShowRecEff <<  setw(10) << "ALTP";
    STREAM_ShowRecEff <<  setw(10) << "FP";
    STREAM_ShowRecEff <<  setw(10) << "MP";
    STREAM_ShowRecEff <<  setw(10) << "LP";
    STREAM_ShowRecEff <<  setw(10) << "NP";

    STREAM_ShowRecEff <<  setw(10) << "LT";
    STREAM_ShowRecEff <<  setw(10) << "MC";

    STREAM_ShowRecEff <<  setw(14) << "PARASETNR";
    STREAM_ShowRecEff <<  setw(14) << "INBTSHOWERNR";

    STREAM_ShowRecEff <<  setw(10) << "EvtBT_E";
    STREAM_ShowRecEff <<  setw(14) << "EvtBT_TT";
    STREAM_ShowRecEff <<  setw(14) << "EvtBT_Flag";
    STREAM_ShowRecEff <<  setw(10) << "EvtBT_MC";

    STREAM_ShowRecEff <<  setw(10) << "InBT_E";
    STREAM_ShowRecEff <<  setw(14) << "InBT_TT";
    STREAM_ShowRecEff <<  setw(14) << "InBT_Flag";
    STREAM_ShowRecEff <<  setw(10) << "InBT_MC";

    STREAM_ShowRecEff <<  setw(10) << "NBT";
    STREAM_ShowRecEff <<  setw(10) << "NBTallMC";
    STREAM_ShowRecEff <<  setw(10) << "NBTeMC";
    STREAM_ShowRecEff <<  setw(10) << "purall";
    STREAM_ShowRecEff <<  setw(10) << "pure";

    STREAM_ShowRecEff <<  setw(10) << "effall";
    STREAM_ShowRecEff <<  setw(10) << "effe";
    STREAM_ShowRecEff <<  setw(10) << "trckdens";
    STREAM_ShowRecEff << endl;
    return;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
void RewriteSegmentPIDs_SGPID_To_BGPID(EdbPVRec* gAli)
{
    Log(3, "ShowRec.cpp", "--- void RewriteSegmentPIDs_SGPID_To_BGPID() ---");
    return;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
void RewriteSegmentPIDs_BGPID_To_SGPID(EdbPVRec* gAli)
{
    /// NEED TO WRITE BG PID TO GALI PIDS IF THERE ARE NO SG BT THERE !!!
    /// STILL IN EXPERIMENTAL PHASE !!!

    // This function is inteded if you have mixture of Simulation and Data, some
    // of the values are not equally filled, and here the affected PID are made
    // same.

    // If during the read in procedure (via lnk.lst lnk.def the segments are automatically
    // assigned new values, then this is not necessary!

    /*
    EdbPatternsVolume with 11 patterns
    id=  0 pid=  0   x:y:z =         0.000        0.000    13000.025  n=     1001
    id=  1 pid=  1   x:y:z =         0.000        0.000    11700.037  n=      994
    id=  2 pid=  2   x:y:z =         0.000        0.000    10399.992  n=      983
    id=  3 pid=  3   x:y:z =         0.000        0.000     9100.037  n=     1039
    id=  4 pid=  4   x:y:z =         0.000        0.000     7800.044  n=      532
    id=  5 pid=  5   x:y:z =         0.000        0.000     6500.041  n=      964
    id=  6 pid=  6   x:y:z =         0.000        0.000     5200.023  n=      904
    id=  7 pid=  7   x:y:z =         0.000        0.000     3900.070  n=      823
    id=  8 pid=  8   x:y:z =         0.000        0.000     2600.014  n=      782
    id=  9 pid=  9   x:y:z =         0.000        0.000     1300.052  n=      783
    id= 10 pid= 10   x:y:z =         0.000        0.000        0.000  n=      681

    There should be   PID(firstplate) always 1
    There should be   PID(lastplate) always npatters-1

    There is a difference between the pid of the EdbPatternsVolume and the PID() of
    the segment couples,
    but at the point where the EdbPatternsVolume is read in, the information on the
    PID() of s.ePID is overwritten.
    Example: Basetracks of 01.cp.root have s.ePID==1 for Sim Basetracks
    after reading in, here they have  pid= 10

    For our simulated data this has alrady written in at reconstruction level:
    segments of 01_001.cp.root have PID()==1    (z==0)
    segments of 56_001.cp.root have PID()==56    (Z==71200)
    segments of 57_001.cp.root have PID()==57    (Z==72500) [but pl 57 not existing in simulation]
    But for data from BG scannings this is arbitrarily (mostly zero)
    So we set this automatically here:
    */


    //  return;
    Log(2, "ShowRec.cpp", "--- void RewriteSegmentPIDs_BGPID_To_SGPID() --- returning, because no good implementation for this function yet........");
    return;

    Int_t npat = gAli->Npatterns();
    Int_t nseg =0;
    Int_t nominal_PID_SG=1234567890;
    Int_t nominal_PID_gALI=1234567890;

    EdbSegP* testseg=0;

    cout << "==============DEBUG====================="<<endl;

    gAli->GetPattern(0)->GetSegment(0)->Print();

    gAli->GetPattern(npat-1)->GetSegment(0)->Print();


    //  return;

    gAli->Print();


    //  for (Int_t i=0; i<npat; ++i){
    for (Int_t i=0; i<npat; ++i) {
        nseg=gAli->GetPattern(i)->N();


        cout << "gAli->GetPattern(i)->PID= " << gAli->GetPattern(i)->PID() <<  endl;
        nominal_PID_gALI=gAli->GetPattern(i)->PID();

        cout << "this is for MCEV>0" << endl;

        for (Int_t ii=0; ii<nseg; ++ii) {
            testseg=(EdbSegP*)gAli->GetPattern(i)->GetSegment(ii);
            if (testseg->MCEvt()<0) continue;
            cout << "PID of (MC) testseg = " << testseg->PID()<< endl;
            //    testseg->Print();
            break;
        }

        cout << "this is for MCEV<0" << endl;

        for (Int_t ii=0; ii<nseg; ++ii) {
            testseg=(EdbSegP*)gAli->GetPattern(i)->GetSegment(ii);
            if (testseg->MCEvt()>0) continue;
            cout << "PID of (MC) testseg = " << testseg->PID()<< endl;
            //    testseg->Print();
            break;
        }

        /*
        /// INFINITE LOOP IF NO FOUND BT!!!!
        //   for (Int_t ii=0; ii<nseg; ++ii){
        Bool_t found_SG_BT=kFALSE;
        Int_t ii=0;
        while (found_SG_BT==kFALSE) {
        testseg=(EdbSegP*)gAli->GetPattern(i)->GetSegment(ii);
        if (testseg->MCEvt()<0) continue;
        cout << "PID of (MC) testseg = " << testseg->PID()<< endl;
        testseg->Print();
        ++ii;
        found_SG_BT=kTRUE;
        }

        Bool_t found_BG_BT=kFALSE;
        ii=0;
        while (found_BG_BT==kFALSE) {
        testseg=(EdbSegP*)gAli->GetPattern(i)->GetSegment(ii);
        if (testseg->MCEvt()>0) continue;
        cout << "PID of (MC) testseg = " << testseg->PID()<< endl;
        testseg->Print();
        ++ii;
        found_BG_BT=kTRUE;
        }
        */


        /*
        nominal_PID_SG=i+1; /// THIS DEPENDS STRONGLY ON THE STRUCTURE OF gALI OBJECT !!!!
        cout << "void RewriteSegmentPIDs_BGPID_To_SGPID()    nominal_PID_SG = " << nominal_PID_SG << " set CHECK CAREFULLY. EXPERIMENTAL HELP FUNCTION PHASE!!" << endl;
        cout << "gAli->GetPattern(i)->PID() " << gAli->GetPattern(i)->PID() << "  " << nominal_PID_SG << endl;
        cout << "gAli->GetPattern(i)->Z() " << gAli->GetPattern(i)->Z() << "  " << nominal_PID_SG << endl;
        for (Int_t ii=0; ii<nseg; ++ii){
        if (gAli->GetPattern(i)->GetSegment(ii)->MCEvt()<0) gAli->GetPattern(i)->GetSegment(ii)->SetPID(i+1);
        //    gAli->GetPattern(i)->GetSegment(ii)->Print();
        }

        */
    }
    return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void RewriteSegmentPIDs(EdbPVRec* gAli)
{
    Log(3, "ShowRec.cpp", "--- void RewriteSegmentPIDs() ---");

    // This becomes necessary since when you have simaulation by ORFEO
    // and real measured tackground, they may have different PID() and the function
    // TransformEdbPVRec() will eventually not wor anymore since it relies on PIDs of
    // the Initiator BaseTracks, so if you start from BG Basetracks, this may crash:
    Bool_t eSGPID_To_BGPID=kFALSE;
    Bool_t eBGPID_To_SGPID=kTRUE;

    if (eSGPID_To_BGPID) RewriteSegmentPIDs_SGPID_To_BGPID(gAli);
    if (eBGPID_To_SGPID) RewriteSegmentPIDs_BGPID_To_SGPID(gAli);
    return;
}
//-------------------------------------------------------------------------------------------

