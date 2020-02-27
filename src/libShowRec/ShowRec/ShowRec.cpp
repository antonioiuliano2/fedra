#include "ShowRecInclude.h"
#include "ShowRec.h"
#include "ShowRec_Alg_NN.h"
#include "ShowRec_Alg_N3.h"
// Everyone says, including cpp files is not good style,
// but for the moment it just works - that is just what
// I need now:
#include "ShowRecHelper.cpp"
#include "ShowRecCalculatingFunctions.cpp"
#include "ShowRec_InBTFilling.cpp"
#include "ShowRec_Alg_NN.cpp"
#include "ShowRec_Alg_N3.cpp"

using namespace  std;
using namespace  TMath;



int main(int argc, char *argv[])
{
    if (argc < 2) {
        cout << "-----------------------------------------------"<< endl;
        cout << "---      ShowRec      ---" << endl;
        cout << "---      A programm for running multiple shower reconstruction algorithms      ---" << endl;
        cout << "---      with different parameter settings for finding the optimum             ---" << endl;
        cout << "---      reconstruction set.                                                   ---" << endl;
        cout << "-----------------------------------------------" << endl;
        cout << "---      Usage:  ShowRec  -OPTION    lnk.def ---" << endl << endl;
        cout << "--- \t\t :  -FP  FirstPlate \t (1..57)\n";
        cout << "--- \t\t\t :  -default 1\n";
        cout << "--- \t\t :  -LP  LastPlate  \t (1..57) \n";
        cout << "--- \t\t\t :  -default 57 or number of plates in volume\n";
        cout << "--- \t\t :  -MP  MiddlePlate  \t (FP..LP)\n";
        cout << "--- \t\t\t :  -default 57 or number of plates in volume\n";
        cout << "--- \t\t :  -NP  Maximal NumberofPlates reconstructed \t (1..57)\n";
        cout << "--- \t\t\t :  -default 57 or number of plates in volume\n";

        cout << "--- \t\t :  -LT  use LinkedTracks.root for InBT \n";
        cout << "--- \t\t\t :    0:  dont use LinkedTracks.root for InBT\n";
        cout << "--- \t\t\t :    1:  use first basetrack of track\n";
        cout << "--- \t\t\t :    2:  use last basetrack of track\n";
        cout << "--- \t\t\t :    3:  use all  basetrack of track\n";
        cout << "--- \t\t\t :    4:  use basetrack in [fp,mp] of track\n";
        cout << "--- \t\t\t :  -default 0\n";

        cout << "--- \t\t :  -MC  use only SIM/BG events for InBT \n";
        cout << "--- \t\t\t : 0:  SIM+BG (i.e. all BT) \n";
        cout << "--- \t\t\t : 1:  SIM \n";
        cout << "--- \t\t\t : 2:  BG \n";
        cout << "--- \t\t\t :  -default 0\n";

        cout << "--- \t\t :  -VTX   For InBT: Cut to IP for MC vertex  (needs BRICK.TreePGunInfo.txt and -MC=1) \t (0,1:Ipcut:100,2:Ipcut250,3:500)\n";
        cout << "--- \t\t\t :  -default ????????\n";

        cout << "--- \t\t :  -HPLZ  use specific P() or Z() conditions for MCEvt InBTs \n";
        cout << "--- \t\t\t :  0:   All \n";
        cout << "--- \t\t\t :  1:   Take Highest P() value for each MCEvt \n";
        cout << "--- \t\t\t :  2:   Take Lowest (i.e. lowest) z occurence for each MCEvt \n";
        cout << "--- \t\t\t :  -default 0\n";

        cout << "--- \t\t :  -FLMC  use only PdgId Flag  \t (PdgId)\n";
        cout << "--- \t\t\t :  -default 0\n";

        cout << "--- \t\t :  -ALI  use gALI either\n";
        cout << "--- \t\t\t :  0: from directory structured: cp.root files \n";
        cout << "--- \t\t\t :  1: from a tracked root file : linkedtracks.root file\n";
        cout << "--- \t\t\t :  2: from a volume root file  : ScanVolume_Ali.root\n";
        cout << "--- \t\t\t :  3: from a written volume file  : ScanVolumeLinkedTracks_Ali.root \n";
        cout << "--- \t\t\t :  -default ????????\n";

        cout << "--- \t\t :  -MIXMC   Extract Subpattern with all MCEvents mixed! --!WARNING!--  \n";
        cout << "--- \t\t :  -EXTHETA Extract Subpattern with Delta Theta Cut on Initiator BT angle. \n";
        cout << "--- \t\t :  -PADI  ParentDirectory            (only for naming the output file)\n";
        cout << "--- \t\t :  -BTPA  BasetrackParametrisation   (only for naming the output file)\n";
        cout << "--- \t\t :  -BGTP  BackgroundType             (only for naming the output file)\n";

        cout << "--- \t\t :  -ALTP  AlgorithmType  \n";
        cout << "--- \t\t\t :  0:  ReconstructShowers_CT  ().. \n";
        cout << "--- \t\t\t :  1:  ReconstructShowers_CL  (NOT USED ANYMORE, EXPERIMENTAL)\n";
        cout << "--- \t\t\t :  2:  ReconstructShowers_CA  \n";
        cout << "--- \t\t\t :  3:  ReconstructShowers_NN  \n";
        cout << "--- \t\t\t :  4:  ReconstructShowers_OI  BEST WORKING ALGORITHM CURRENTLY\n";
        cout << "--- \t\t\t :  5:  ReconstructShowers_SA  (take all MC Events only)\n";
        cout << "--- \t\t\t :  6:  ReconstructShowers_TC  \n";
        cout << "--- \t\t\t :  7:  ReconstructShowers_RC  \n";
        cout << "--- \t\t\t :  8:  ReconstructShowers_BW  (EXPERIMENTAL, BEST PARAMETERS STILL TO BE SEARCHED)\n";
        cout << "--- \t\t\t :  9:  ReconstructShowers_AG  (EXPERIMENTAL, BEST PARAMETERS STILL TO BE SEARCHED)\n";
        cout << "--- \t\t\t :  10:  ReconstructShowers_GS  (same Implementation as in libShowRec----- BEST PARAMETERS STILL TO BE SEARCHED)\n";
        cout << "--- \t\t\t :  11:  ReconstructShowers_N3  REWRITING OF THE IMPLEMENTATION OF NN ALG - with some modifications.\n";
        cout << "--- \t\t\t\t :  -ALN3TRAIN1    Do Training of the Neural Net (default: 0, run)\n";
        cout << "--- \t\t\t\t :  -ALN3DUMP1     Write also TrainingsTrees in the Neural Net Root Info File  (default: 0)\n";
        cout << "--- \t\t\t\t :  -ALN3EQUALIZE1 Try to have same number of SG/BG tracks for training (default: 1, yes)\n";
        cout << "--- \t\t\t :  -default 4\n";


        cout << "--- \t\t :  -PASTART ParametersetStart  \n";
        cout << "--- \t\t :  -PAEND  ParameterSetEnd  \n";

        cout << "--- \t\t :  -CUTTP  Algorithm CutType \n";
        cout << "--- \t\t\t :  0:  standard  \n";
        cout << "--- \t\t\t :  1:  high purity   \n";
        cout << "--- \t\t\t :  2:  high efficiency  \n";
        cout << "--- \t\t\t :  3:  FJ_highPurity   \n";
        cout << "--- \t\t\t :  4:  FJ_Standard   \n";
        cout << "--- \t\t\t :  -default ????????\n";

        cout << "--- \t\t :  -CLEAN  InputData BG Cleaning: 0: No, 1:20BT/mm2  2: 40BT/mm2  3:10BT/mm2 4:60BT/mm2 \n";
        cout << "--- \t\t\t :  -default ????????\n";
        cout << "--- \t\t\t :  ATTENTION ... NOT FULLY FUNCTIONING YET ... TO BE CHECK WITH\n";
        cout << "--- \t\t\t :  THE BG-CELANING IMPLEMENTATION IN LIBSHOWREC\n";

        cout << "--- \t\t :    InputData BG Cleaning: 10: Remove DoubleBT and Passing, No dens cut, 11: &&10BT/mm2  12: &&20BT/mm2  13: &&30BT/mm2 ... \n";
        cout << "--- \t\t\t :  -default ????????\n";
        cout << "--- \t\t\t :  ATTENTION ... NOT FULLY FUNCTIONING YET ... TO BE CHECK WITH\n";
        cout << "--- \t\t\t :  THE BG-CELANING IMPLEMENTATION IN LIBSHOWREC\n";

        cout << "--- \t\t :  -FILETP Filetype: Additional (distinguish-) variable to be written into treebranch. (only for naming the output tree)\n";
        cout << "--- \t\t\t :  -default ????????\n";
        cout << "--- \t\t :  -GBMC  Global MC: addition variable to tell the program which MCEvt is doing (if only one is done).\n";
        cout << "--- \t\t\t :  -default ????????\n";
        cout << "--- \t\t :  -DEBUG  gEDBDEBUGLEVEL \t (1..5)\n";
        cout << "--- \t\t\t :  -default 2\n";
        cout << "--- \t\t :  -OUT  OUTPUTLEVEL  \t (1,2,3)\n";
        cout << "--- \t\t\t :  -default 1\n";
        cout << "--- \t\t :  -STOP  STOPLEVEL  \t (0,1,2,3,4,5)\n";
        cout << "--- \t\t\t :  0: Run until end of program \n";
        cout << "--- \t\t\t :  1: Run until Read_ParasetDefinitionTree().\n";
        cout << "--- \t\t\t :  2: Run until Do Background cleaning of input.\n";
        cout << "--- \t\t\t :  3: Run until Fill the Initiator BT array.\n";
        cout << "--- \t\t\t :  4: Run until Reconstruct showers is done.\n";
        cout << "--- \t\t\t :  5: Run until Fill output structures.\n";
        cout << "--- \t\t\t :  -default 0\n";

        cout << "--- \t\t :  -LIST-PRESET  \n";
        cout << "--- \t\t\t :  List built-in preset options. Most usage cases will be covered here. \n";

        cout << "--- \t\t :  -PRESET  Number  \t (0,1,2,3)\n";
        cout << "--- \t\t\t :  Details of switches: see option -LIST-PRESET \n";
        cout << "--- \t\t\t :  0: Use all BTs of volume for InBT. Loooooong running time! (data driven reco)\n";
        cout << "--- \t\t\t :  1: Use highest P() MCEvt BTs of whole volume for InBT.! (mc driven reco) \n";
        cout << "--- \t\t\t :  2: Use all BTs linked_tracks.root file for InBT. (data driven reco) \n";
        cout << "--- \t\t\t :  3: Use all BTs with IP to list of vertices for InBT. Possibly shortest running time. \n";
        cout << "--- \t\t\t :  -default 0\n";




        cout << "---      Example usages:  " << endl;

        cout << "---      Reconstruct all possible showers for all possible Initiator Basetracks in the whole volume -- looooong runnning time!:  ShowRec lnk_all.def" << endl<< endl;

        cout << "---      Reconstruct all possible showers for the highest P()- MC Initiator Basetrack, given a ready ali.root file: ShowRec  -HPLZ1 -MC1 -ALI2 " << endl<< endl;


        cout << "---      Reconstruct all possible showers for the highest P()- MC Initiator Basetrack, given a linked tracks file for starting basetracks: ShowRec  -LT1 -MC1 lnk_all.def " << endl<< endl;

        cout << "---      Usage:  ShowRec  -FP1 -LP31 -MP1 -NP30 -HPLZ1 -MC1 -ALTP4  -PASTART0 -PAEND0  lnk.def ---" << endl<< endl;
        cout << "---      Usage:  ShowRec  -FP1 -LP31 -MP30 -NP30 -HPLZ1 -MC1 -ALTP4 lnk_all.def ---" << endl<< endl;
        cout << "---      Usage:  ShowRec  -HPLZ1 -MC1 lnk_all.def ---" << endl<< endl;
        cout << "---      Usage:  ShowRec  -LT1 -MC1 lnk_all.def ---" << endl<< endl;
        cout << "-----------------------------------------------"<<endl;
        return 0;
    }

    //----------------------------------------------------------------------------------
    SetDefaultValues_CommandLine();
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    //-C- Take over values from the command line:
    //-C- wARNING !!!
    // TWO OPTIONS ARE NOT ALLOWED TO HAVE THE SAME CHARACTER CODE, otherwise complications
    // will appear: so -MC and -MCMIX  WILL NOT WORK!! use -MC and -MIXMC
    //-C- wARNING !!!
    //-C- ATTENTION FIXME
    //-C- If the volume has not 57 plates than WE HAVE TO READAPT THE DEFAULT VALUES !!!
    //-C- wARNING !!!
    //----------------------------------------------------------------------------------

    Bool_t IsSet_cmd_PRESET=kFALSE;

    char *name = argv[argc];
    for (int i=1; i<argc; ++i ) {
        char *key  = argv[i];

        // ------------------------------
        // Option Types "HELP" and "-LIST-PRESET"
        // automatically stop program after displaying
        // information

        if      (!strncmp(key,"-HELP",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_HELP);
            }
            PrintHelp();
            return 0;
        }
        if      (!strncmp(key,"-LIST-PRESET",12)) {
            if (strlen(key)>12) {
                sscanf(key+12,"%d",&cmd_PRESET);
            }
            PrintPresetList();
            return 0;
        }
        // ------------------------------
        // These three value options have to be read in any case,
        // independently from any other option key.
        else if (!strncmp(key,"-DEBUG",6)) {
            if (strlen(key)>6) {
                sscanf(key+6,"%d",&cmd_gEDBDEBUGLEVEL);
            }
        }
        else if (!strncmp(key,"-OUT",4)) {
            if (strlen(key)>4) {
                sscanf(key+4,"%d",&cmd_OUTPUTLEVEL);
            }
        }
        else if (!strncmp(key,"-STOP",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_STOPLEVEL);
            }
        }
        // ------------------------------

        if      (!strncmp(key,"-PRESET",7)) {
            if (strlen(key)>7) {
                sscanf(key+7,"%d",&cmd_PRESET);
            }
            // If PRESET value was given then take only the given values for this
            // preset scenario. Dont read any other given option values anymore.
            // Skip directly out of this loop!
            cout <<" If PRESET value was given then take only the given values for this preset scenario. Dont read any other given option values anymore. Skip directly out of this loop! "<< endl;
            //
            SetPresetParameters(cmd_PRESET);
            IsSet_cmd_PRESET=kTRUE;
        }

        if (IsSet_cmd_PRESET == kTRUE ) continue;


        if      (!strncmp(key,"-FP",3)) {
            if (strlen(key)>3) {
                sscanf(key+3,"%d",&cmd_FP);
            }
        }
        else if (!strncmp(key,"-LP",3)) {
            if (strlen(key)>3) {
                sscanf(key+3,"%d",&cmd_LP);
            }
        }
        else if (!strncmp(key,"-NP",3)) {
            if (strlen(key)>3) {
                sscanf(key+3,"%d",&cmd_NP);
            }
        }
        else if (!strncmp(key,"-MP",3)) {
            if (strlen(key)>3) {
                sscanf(key+3,"%d",&cmd_MP);
            }
        }
        else if (!strncmp(key,"-PADI",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_PADI);
            }
        }
        else if (!strncmp(key,"-BTPA",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_BTPA);
            }
        }
        else if (!strncmp(key,"-BGTP",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_BGTP);
            }
        }
        else if (!strncmp(key,"-ALTP",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_ALTP);
            }
        }
        else if (!strncmp(key,"-ALN3TRAIN",10)) {
            if (strlen(key)>10) {
                sscanf(key+10,"%d",&cmd_ALN3TRAIN);
            }
        }
        else if (!strncmp(key,"-ALN3DUMP",9)) {
            if (strlen(key)>9) {
                sscanf(key+9,"%d",&cmd_ALN3DUMP);
            }
        }
        else if (!strncmp(key,"-PASTART",8)) {
            if (strlen(key)>8) {
                sscanf(key+8,"%d",&cmd_PASTART);
            }
        }
        else if (!strncmp(key,"-PAEND",6)) {
            if (strlen(key)>6) {
                sscanf(key+6,"%d",&cmd_PAEND);
            }
        }
        else if (!strncmp(key,"-CUTTP",6)) {
            if (strlen(key)>6) {
                sscanf(key+6,"%d",&cmd_CUTTP);
            }
        }
        else if (!strncmp(key,"-CLEAN",6)) {
            if (strlen(key)>6) {
                sscanf(key+6,"%d",&cmd_CLEAN);
            }
        }
        else if (!strncmp(key,"-LT",3)) {
            if (strlen(key)>3) {
                sscanf(key+3,"%d",&cmd_LT);
            }
        }
        else if (!strncmp(key,"-MC",3)) {
            if (strlen(key)>3) {
                sscanf(key+3,"%d",&cmd_MC);
            }
        }
        else if (!strncmp(key,"-FLMC",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_MCFL);
            }
        }
        else if (!strncmp(key,"-HPLZ",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_HPLZ);
            }
        }
        else if (!strncmp(key,"-ALI",4)) {
            if (strlen(key)>4) {
                sscanf(key+4,"%d",&cmd_ALI);
            }
        }
        else if (!strncmp(key,"-MIXMC",6)) {
            if (strlen(key)>6) {
                sscanf(key+6,"%d",&cmd_MCMIX);
            }
        }
        else if (!strncmp(key,"-EXTHETA",8)) {
            if (strlen(key)>8) {
                sscanf(key+8,"%d",&cmd_EXTHETA);
            }
        }
        else if (!strncmp(key,"-VTX",4)) {
            if (strlen(key)>4) {
                sscanf(key+4,"%d",&cmd_vtx);
            }
        }

        else if (!strncmp(key,"-FILETP",7)) {
            if (strlen(key)>7) {
                sscanf(key+7,"%d",&cmd_FILETP);
            }
        }
        else if (!strncmp(key,"-GBMC",5)) {
            if (strlen(key)>5) {
                sscanf(key+5,"%d",&cmd_GBMC);
            }
        }
    }
    gEDBDEBUGLEVEL=cmd_gEDBDEBUGLEVEL;


    //----------------------------------------------------------------------------------
    // ReadDefaultValues_CommandLine(); // sorry, doesnt work, because argv and argc need
    // to be called in the main function loop....
    // Just used to print the values...
    // PrintValues_CommandLine();
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    CheckInputParameters();
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    // Read the data Objects
    // This step needs to come at the very beginning, since the data object
    // itsselfs determines some reconstruction parameters, like FP, MP and LP.
    GLOBAL_gAli = ReadEdbPVRecObjectFromCurrentDirectory();
    GLOBAL_gAli->Print();
    //--------------------------
    // I checked, this is not necessary, since PID is set new when Reading EdbVRec object.
    RewriteSegmentPIDs_BGPID_To_SGPID(GLOBAL_gAli);
    //--------------------------
    // Since the GLOBAL_gAli is now known, one has to check if First-,Middle- and Lastplate
    // are correctly set. If not, set them to the default values of that GLOBAL_gAli.
    CheckInputParametersNEW();
    //----------------------------------------------------------------------------------



    //----------------------------------------------------------------------------------
    PrintValues_CommandLine();
    //----------------------------------------------------------------------------------



    //----------------------------------------------------------------------------------
    // Open ParameterSet definition file if it is there.
    // Otherwise set back default Parameterset values
    Open_ParasetDefinitionFile();
    Read_ParasetDefinitionTree();
    cout << "Reached STOPLEVEL 1" << endl;
    if (cmd_STOPLEVEL==1) return 1;
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    // Create Root Files, Trees and Histograms for further processing
    CreateOutPutStructures();
    //----------------------------------------------------------------------------------





    //----------------------------------------------------------------------------------
    // Do Background Data Cleaning with the Clean algorithms..
    // ATTENTION: This is yet to be modified, depending on the cleaning algorithm!
    cout << "ATTENTION: Do Background Data Cleaning  IS STILL TO BE MODIFIED " << endl;
    cout << "ATTENTION: DO NOT USE FOR NOW UNLESS ALGO IS FINISHED. " << endl;
    DoBGTargetCleaning();
    cout << "Reached STOPLEVEL 2" << endl;
    if (cmd_STOPLEVEL==2) return 1;
    //----------------------------------------------------------------------------------


    //----------------------------------------------------------------------------------
    // Fill the Initiator BT array:
    // FillGlobalInBTArray();
    cout << "// FillGlobalInBTArray();" << endl;
    cout << "instead" << endl;
    cout << "FillGlobalInBTArrayNEW();" << endl;
    // NEW VARIANT 2018 06 22:
    // Use the refactored fill function, which might be
    // better written code (more clearly)
    FillGlobalInBTArrayNEW();
    // Add additional MC-Event info for vertex filling:
    BuildParametrizationsMCInfo_PGun("BRICK.TreePGunInfo.txt");
    Fill2GlobalInBTArray();
    cout << "Fill the Initiator BT array:GLOBAL_InBTArray->GetEntries()= " << GLOBAL_InBTArray->GetEntries() << endl;
    cout << "Reached STOPLEVEL 3" << endl;
    if (cmd_STOPLEVEL==3) return 1;
    //----------------------------------------------------------------------------------





    //----------------------------------------------------------------------------------
    // Loop over (possible) Parametersets and Reconstruct Showers!
    //----------------------------------------------------------------------------------
    if (cmd_PASTART>=0 && cmd_PAEND==-1) cmd_PAEND=cmd_PASTART;
    for (Int_t i=cmd_PASTART; i<=cmd_PAEND; i++) {
        GLOBAL_PARASETNR=i;
        if (gEDBDEBUGLEVEL>2) cout << "Doing PARASET  "<< i <<endl;

        GetEvent_ParasetDefinitionTree(i);
        if (gEDBDEBUGLEVEL>2) cout << "GetEvent_ParasetDefinitionTree("<<i<<") done."<<endl;

        ReconstructShowers(i);
        if (gEDBDEBUGLEVEL>2) cout << "ReconstructShowers("<<i<<") done."<<endl;
    }
    cout << "Reached STOPLEVEL 4" << endl;
    if (cmd_STOPLEVEL==4) return 1;
    //----------------------------------------------------------------------------------



    //----------------------------------------------------------------------------------
    // Done with all reconstruction, fill files now:
    cout << "Done with all reconstruction, fill files now:" << endl;
    FillOutPutStructures();
    cout << "Reached STOPLEVEL 5" << endl;
    if (cmd_STOPLEVEL==5) return 1;
    //----------------------------------------------------------------------------------




    //----------------------------------------------------------------------------------
    cout << "Closing files, destructing elements and cleaning up orphaned stuff:" << endl;
    Finalize();
    //----------------------------------------------------------------------------------

    cout << "Reached STOPLEVEL 0" << endl;
    cout << "ShowRec finished." << endl;
    return 0;
}

//-------------------------------------------------------------------------------------------

void Finalize() {

    Log(2, "ShowRec.cpp", "--- void Finalize() ---");
    cout << "Do some rest of the GSNN Algorithm, to be packed in extrafunction" << endl;
    cout << "for some later time...." << endl;
    Write_Alg_GS_Histograms();

}


//-------------------------------------------------------------------------------------------

void SetDefaultValues_CommandLine() {

    Log(2, "ShowRec.cpp", "--- void SetDefaultValues_CommandLine() ---");

    //--- Set default Values:
    cmd_FP=1;    // This default setting (FP,MP,LP,NP) takes all BTs in the volume
    cmd_LP=57;   // for possible initiator basetracks and starts reconstruction
    cmd_MP=57;   // from them all.
    cmd_NP=57;   // Attention: this may take a very long time.
    cmd_PADI=0;
    cmd_BTPA=0;
    cmd_BGTP=0;
    cmd_ALTP=4;
    cmd_PASTART=-1;
    cmd_PAEND=-1;
    cmd_LT=0;
    cmd_MC=1;
    cmd_MCFL=0;
    cmd_HPLZ=1;
    cmd_vtx=0;
    cmd_gEDBDEBUGLEVEL=2;
    cmd_OUTPUTLEVEL=1;
    cmd_ALI=0;
    cmd_MCMIX=0;
    cmd_FILETP=0;
    cmd_GBMC=0;
    cmd_lnkdef_name = "lnk.def";
    cmd_EXTHETA=0;
    return;
}

//-------------------------------------------------------------------------------------------


void CreateOutPutStructures() {

    Log(2, "ShowRec.cpp", "--- void CreateOutPutStructures() ---");

    // Create unique Outputfile where efficencies and purities are written in:
    //-----------------------------------
    STREAM_ShowRecEffName="ShowRecEff__PADI_"+TString(Form("%d",cmd_PADI))+"_BTPA_"+TString(Form("%d",cmd_BTPA))+"_BGTP_"+TString(Form("%d",cmd_BGTP))+"_ALTP_"+TString(Form("%d",cmd_ALTP))+"_FP_"+TString(Form("%d",cmd_FP))+"_MP_"+TString(Form("%d",cmd_MP))+"_NP_"+TString(Form("%d",cmd_NP))+"_LT_"+TString(Form("%d",cmd_LT))+"_MC_"+TString(Form("%d",cmd_MC))+"_HPLZ_"+TString(Form("%d",cmd_HPLZ))+"__ParaSet_"+TString(Form("%d",cmd_PASTART))+"_To_"+TString(Form("%d",cmd_PAEND))+".txt";
    //------------------
    STRING_ShowShowerName="ShowRecShower__PADI_"+TString(Form("%d",cmd_PADI))+"_BTPA_"+TString(Form("%d",cmd_BTPA))+"_BGTP_"+TString(Form("%d",cmd_BGTP))+"_ALTP_"+TString(Form("%d",cmd_ALTP))+"_FP_"+TString(Form("%d",cmd_FP))+"_MP_"+TString(Form("%d",cmd_MP))+"_NP_"+TString(Form("%d",cmd_NP))+"_LT_"+TString(Form("%d",cmd_LT))+"_MC_"+TString(Form("%d",cmd_MC))+"_HPLZ_"+TString(Form("%d",cmd_HPLZ))+"__ParaSet_"+TString(Form("%d",cmd_PASTART))+"_To_"+TString(Form("%d",cmd_PAEND))+".txt";
    FILE_ShowShower = new TFile(STRING_ShowShowerName+".root","RECREATE");
    //------------------
    STRING_ShowTracksName="ShowRecTracks__PADI_"+TString(Form("%d",cmd_PADI))+"_BTPA_"+TString(Form("%d",cmd_BTPA))+"_BGTP_"+TString(Form("%d",cmd_BGTP))+"_ALTP_"+TString(Form("%d",cmd_ALTP))+"_FP_"+TString(Form("%d",cmd_FP))+"_MP_"+TString(Form("%d",cmd_MP))+"_NP_"+TString(Form("%d",cmd_NP))+"_LT_"+TString(Form("%d",cmd_LT))+"_MC_"+TString(Form("%d",cmd_MC))+"_HPLZ_"+TString(Form("%d",cmd_HPLZ))+"__ParaSet_"+TString(Form("%d",cmd_PASTART))+"_To_"+TString(Form("%d",cmd_PAEND))+".txt";
    FILE_ShowTracks = new TFile(STRING_ShowTracksName+".root","RECREATE");
    //------------------
    TString HistoOutputFile="ShowRecHistos__PADI_"+TString(Form("%d",cmd_PADI))+"_BTPA_"+TString(Form("%d",cmd_BTPA))+"_BGTP_"+TString(Form("%d",cmd_BGTP))+"_ALTP_"+TString(Form("%d",cmd_ALTP))+"_FP_"+TString(Form("%d",cmd_FP))+"_MP_"+TString(Form("%d",cmd_MP))+"_NP_"+TString(Form("%d",cmd_NP))+"_LT_"+TString(Form("%d",cmd_LT))+"_MC_"+TString(Form("%d",cmd_MC))+"_HPLZ_"+TString(Form("%d",cmd_HPLZ))+"__ParaSet_"+TString(Form("%d",cmd_PASTART))+"_To_"+TString(Form("%d",cmd_PAEND))+".txt";
    FILE_ShowRecHistos = new TFile(HistoOutputFile+".root","RECREATE");
    //------------------

    cout << "--- void CreateOutPutStructures() --- Root Files Recreated."<<endl;

    NBTeMC_pure      = new TProfile("NBTeMCvspure","NBTeMCvspure",100,0,1.05,0,300);
    NBTallMC_purall  = new TProfile("NBTallMCvspurall","NBTallMCvspurall",100,0,1.05,0,300);
    NBTeMC_NBTMC = new TProfile("NBTeMCvsNBTMC","NBTeMCvsNBTMC",100,0,200,0,300);
    NBTeMC_NBT = new TProfile("NBTeMCvsNBT","NBTeMCvsNBT",100,0,200,0,300);
    NBT_InBTE = new TProfile("NBTvsInBTE","NBTvsInBTE",100,0,30,0,300);
    NBTeMC_InBTE = new TProfile("NBTeMCvsInBTE","NBTeMCvsInBTE",100,0,30,0,300);
    pure_InBTE = new TProfile("pure_InBTE","pure_InBTE",100,0,30,0,1.05);
    purall_InBTE = new TProfile("purall_InBTE","purall_InBTE",100,0,30,0,1.05);

    Hist_NBTeMC_pure = new TH2F("Hist_NBTeMC_pure","Hist_NBTeMC_pure",105,0,1.05,200,0,400);
    Hist_NBTallMC_purall = new TH2F("Hist_NBTallMC_purall","Hist_NBTallMC_purall",105,0,1.05,200,0,400);
    Hist_NBTeMC_NBTMC = new TH2F("Hist_NBTeMC_NBTMC","Hist_NBTeMC_NBTMC",200,0,400,200,0,200);
    Hist_NBTeMC_NBT = new TH2F("Hist_NBTeMC_NBT","Hist_NBTeMC_NBT",200,0,400,200,0,200);
    Hist_NBT_InBTE = new TH2F("Hist_NBT_InBTE","Hist_NBT_InBTE",120,0,30,200,0,200);
    Hist_NBTeMC_InBTE = new TH2F("Hist_NBTeMC_InBTE","Hist_NBTeMC_InBTE",120,0,30,200,0,200);
    Hist_pure_InBTE = new TH2F("Hist_pure_InBTE","Hist_pure_InBTE",120,0,30,100,0,1.05);
    Hist_purall_InBTE = new TH2F("Hist_purall_InBTE","Hist_purall_InBTE",120,0,30,100,0,1.05);

    cout << "--- void CreateOutPutStructures() --- Histos Recreated."<<endl;
    //------------------
    FILE_ShowRecEff = new TFile(STREAM_ShowRecEffName+".root","RECREATE");
    TREE_ShowRecEff = new TTree("TreeSTREAM_ShowRecEff","TreeWithvaluesEqualToSTREAM_ShowRecEffTextFile");
    TREE_ShowRecEff->SetDirectory(FILE_ShowRecEff);
    cout << "--- void CreateOutPutStructures() --- Tree Recreated."<<endl;
    //-----------------------------------
    TREE_ShowRecEff->Branch("PADI", &cmd_PADI, "PADI/I");
    TREE_ShowRecEff->Branch("BTPA", &cmd_BTPA, "BTPA/I");
    TREE_ShowRecEff->Branch("BGTP", &cmd_BGTP, "BGTP/I");
    TREE_ShowRecEff->Branch("ALTP", &cmd_ALTP, "ALTP/I");

    TREE_ShowRecEff->Branch("FP", &cmd_FP, "FP/I");
    TREE_ShowRecEff->Branch("MP", &cmd_MP, "MP/I");
    TREE_ShowRecEff->Branch("LP", &cmd_LP, "LP/I");
    TREE_ShowRecEff->Branch("NP", &cmd_NP, "NP/I");

    TREE_ShowRecEff->Branch("LT", &cmd_LT, "LT/I");
    TREE_ShowRecEff->Branch("MC", &cmd_MC, "MC/I");

    TREE_ShowRecEff->Branch("PARASETNR", &GLOBAL_PARASETNR, "PARASETNR/I");
    TREE_ShowRecEff->Branch("ShowerNr", &GLOBAL_INBTSHOWERNR, "ShowerNr/I");

    TREE_ShowRecEff->Branch("EvtBT_E", &GLOBAL_EvtBT_E, "EvtBT_E/D");
    TREE_ShowRecEff->Branch("EvtBT_TanTheta", &GLOBAL_EvtBT_TanTheta, "EvtBT_TanTheta/D");
    TREE_ShowRecEff->Branch("EvtBT_Flag", &GLOBAL_EvtBT_Flag, "EvtBT_Flag/I");
    TREE_ShowRecEff->Branch("EvtBT_MC", &GLOBAL_EvtBT_MC, "EvtBT_MC/I");

    TREE_ShowRecEff->Branch("InBT_E", &GLOBAL_InBT_E, "InBT_E/D");
    TREE_ShowRecEff->Branch("InBT_TanTheta", &GLOBAL_InBT_TanTheta, "InBT_TanTheta/D");
    TREE_ShowRecEff->Branch("InBT_Flag", &GLOBAL_InBT_Flag, "InBT_Flag/I");
    TREE_ShowRecEff->Branch("InBT_MC", &GLOBAL_InBT_MC, "InBT_MC/I");

    TREE_ShowRecEff->Branch("NBT", &GLOBAL_NBT, "NBT/I");
    TREE_ShowRecEff->Branch("NBTMC", &GLOBAL_NBTMC, "NBTMC/I"); // kept for backward compability
    TREE_ShowRecEff->Branch("NBTallMC", &GLOBAL_NBTallMC, "NBTallMC/I");
    TREE_ShowRecEff->Branch("NBTeMC", &GLOBAL_NBTeMC, "NBTeMC/I");
    TREE_ShowRecEff->Branch("purall", &GLOBAL_purall, "purall/D");
    TREE_ShowRecEff->Branch("pure", &GLOBAL_pure, "pure/D");
    TREE_ShowRecEff->Branch("effall", &GLOBAL_effall, "effall/D");
    TREE_ShowRecEff->Branch("effe", &GLOBAL_effe, "effe/D");

    TREE_ShowRecEff->Branch("trckdens", &GLOBAL_trckdens, "trckdens/D");
    //-----------------------------------



    //-----------------------------------
    // Histograms and TTree ONLY RELEVANT FOR GS Algo
    cout << "Create Objects ONLY RELEVANT FOR GS Algo" << endl;
    f_GSNN = new TFile("f_GSNN.root","RECREATE");
    t_GSNN = new TTree("t_GSNN","t_GSNN");
    t_GSNN->Branch("value_GSNN_varInput",&value_GSNN_varInput,"value_GSNN_varInput/F");
    t_GSNN->Branch("value_GSNN_var00",&value_GSNN_var00,"value_GSNN_var00/F");
    t_GSNN->Branch("value_GSNN_var01",&value_GSNN_var01,"value_GSNN_var01/F");
    t_GSNN->Branch("value_GSNN_var02",&value_GSNN_var02,"value_GSNN_var02/F");
    t_GSNN->Branch("value_GSNN_var03",&value_GSNN_var03,"value_GSNN_var03/F");
    t_GSNN->Branch("value_GSNN_var04",&value_GSNN_var04,"value_GSNN_var04/F");
    t_GSNN->Branch("value_GSNN_var05",&value_GSNN_var05,"value_GSNN_var05/F");
    t_GSNN->Branch("value_GSNN_var06",&value_GSNN_var06,"value_GSNN_var06/F");
    h_GSNN_var00=new TH1F("h_GSNN_var00","h_GSNN_var00",1000,0,1000);
    h_GSNN_var01=new TH1F("h_GSNN_var01","h_GSNN_var01",1000,0,1000);
    h_GSNN_var02=new TH1F("h_GSNN_var02","h_GSNN_var02",1000,0,1000);
    h_GSNN_var03=new TH1F("h_GSNN_var03","h_GSNN_var03",100,0,70000);
    h_GSNN_var04=new TH1F("h_GSNN_var04","h_GSNN_var04",1000,0,1);
    h_GSNN_var05=new TH1F("h_GSNN_var05","h_GSNN_var05",10,0,10);
    h_GSNN_var06=new TH1F("h_GSNN_var06","h_GSNN_var06",1100,-1000,100);
    cout << "Create Objects ONLY RELEVANT FOR GS Algo...done." << endl;
    //-----------------------------------



    //-----------------------------------
    Log(2, "ShowRec.cpp", "--- void CreateOutPutStructures() ---done.");
    return;
}


//-------------------------------------------------------------------------------------------


EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory()
{
    Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() ---");

    // Create EdbPVRec on the heap:
    EdbPVRec *gAli= new EdbPVRec();

//   cmd_ALI==0: read gAli from lnk.def Basetracks
//   cmd_ALI==1: read gAli from linked.tracks.root
//   cmd_ALI==2: read gAli from ScanVolume_Ali.root
//   cmd_ALI==3: read gAli from ScanVolumeLinkedTracks_Ali.root

    if (cmd_ALI==3) {
        Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() cmd_ALI==3: read gAli from ScanVolumeLinkedTracks_Ali.root");
        TFile* f= new TFile("ScanVolumeLinkedTracks_Ali.root");
        gAli= (EdbPVRec*) f->Get("EdbPVRec");
        return gAli;
    }
    if (cmd_ALI==2) {
        Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() cmd_ALI==2: read gAli from ScanVolume_Ali.root");
        TFile* f= new TFile("ScanVolume_Ali.root");
        f->ls();
        gAli= (EdbPVRec*) f->Get("EdbPVRec");
        return gAli;
    }

    //-----------------------------------
    // current dir has to contain:
    // default.par, lnk_all.lst, lnk_all.def
    // data, par directory
    // the definition of filenames and structurs
    // is given in the lnk.def file (cmd_lnkdef_name)
    // Warning:
    // CHECK IF THIS FILE EXISTS. IF NOT, STOP HERE
    // BECAUSE ONE DOES NOT KNOW WHAT TO DO !!!
    //-----------------------------------
    Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() Read EdbDataProc object");
    Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() from file: %s",cmd_lnkdef_name);

    string filename = cmd_lnkdef_name;
    ifstream fin( filename.c_str() );
    if ( !fin ) {
        cout << "WARNING:  Opening " << filename << " failed!!! SEVERE ERROR POSSIBLE." << endl;
    }

    // DECLARE MAIN OBJECTS
    // Data set initialization
    // string handling from cmd_lnkdef_name file here.
    EdbDataProc   *dset;
    dset = new EdbDataProc(cmd_lnkdef_name);
    //dset->Dump();

    // Volume initialization
    // The differentiation LT or not is made when filling the array
    // new: (4.2.2010) can distinguish between gAli from cp.root or from linked tracks

    if (cmd_ALI==1) {
        Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() cmd_ALI==1: read gAli from linked.tracks.root");
        dset->InitVolume(100, ""); // Read in (all) BT from linked_tracks.root
    }
    if (cmd_ALI==0) {
        Log(2, "ShowRec.cpp", "--- EdbPVRec* ReadEdbPVRecObjectFromCurrentDirectory() cmd_ALI==0: read gAli from lnk.def Basetracks");
        cout << "TODO HERE ... CHECK IF cp.root files exist !!!" << endl;
        cout << "OTHERWISE CRASH IN THE EdbDataProc::InitVolume FUNCTION" << endl;
        dset->InitVolume(0, ""); // Read in (all) BT from cp.root.
    }

    // Finally: get Pattern Volume Reconstruction object
    gAli = dset->PVR();
    //-------------------------------------
    if (gEDBDEBUGLEVEL>2)  gAli->Print();
    //-------------------------------------
    return gAli;
}


//-------------------------------------------------------------------------------------------

Int_t Open_ParasetDefinitionFile()
{
    Log(2, "ShowRec.cpp", "--- Int_t Open_ParasetDefinitionFile() ---");

    FILE_ParaSetDefinitions = new TFile("PARAMETERSET_DEFINITIONFILE.root","READ");
    TREE_ParaSetDefinitions = 0;
    TREE_ParaSetDefinitions = (TTree*)FILE_ParaSetDefinitions->Get("ParaSet_Variables");

    if (gEDBDEBUGLEVEL>2) if (TREE_ParaSetDefinitions!=0) TREE_ParaSetDefinitions->Print();
    
    // Possible BugFix: 
    if (cmd_PASTART==-1 && cmd_PAEND==-1) {
     cout << "Possible BugFix:  if (cmd_PASTART==-1 and cmd_PAEND==-1)" << endl;
     cout << "then we assume no FILE_ParaSetDefinitions is needed, and therefore, " << endl;
     cout << "even if it exists, we set it to NULL: TREE_ParaSetDefinitions = 0;" << endl;
     
     TREE_ParaSetDefinitions = 0;
    }
        
    if (TREE_ParaSetDefinitions==0) {
        cout << "WARNING: In --- Open_ParasetDefinitionFile ---  Empty TREE_ParaSetDefinitions. (this may have several reasons.)" << endl;
        cout << "Either corrupted FILE_ParaSetDefinitions or default cmd_PASTART/cmd_PAEND given." << endl;
        cout << "         Switching to default parametersets (algorithm specific)." << endl;
        cout << "         Set cmd_PASTART=-1   cmd_PAEND  =-1 " << endl;
        cmd_PASTART=-1;
        cmd_PAEND=-1;
    }
    return 1;
}


//-------------------------------------------------------------------------------------------

void Read_ParasetDefinitionTree()
{
    Log(2, "ShowRec.cpp", "--- Int_t Read_ParasetDefinitionTree() ---");

    // ALTP 0,2,4 and others ...
    Double_t dr_max,dt_max,coneangle,tubedist;
    Int_t    nholes_max;
    Double_t ann_output;
    Int_t ann_inputneurons;
    Double_t  CUT_P;
    Double_t  CUT_ALISUBSIZE;
    Double_t distMin_max;
    Int_t  tracksegs_max;
    Double_t distMin_dt_max;
    Double_t cut_back_dmin,cut_for_dmin,cut_back_dtheta,cut_for_dtheta,cut_back_dr,cut_for_dr,cut_back_dz,cut_for_dz;

    // ALTP 11   N3_ALG
    Double_t     ANN_OUTPUT;
    Int_t        ANN_PLATEN;
    Int_t        ANN_PLATEDIRECTION;
    Int_t        ANN_HIDDENLAYER;
    Int_t        ANN_INPUTNEURONS;

    // ALTP 10   GS from libShowRec
    Double_t cut_gs_cut_dip=150;
    Double_t cut_gs_cut_dmin=40;
    Double_t cut_gs_cut_dr=60;
    Double_t cut_gs_cut_dz=19000;
    Double_t cut_gs_cut_dtheta=0.06;
    Double_t cut_gs_cut_piddiff=1;
    Int_t cut_gs_cut_oppositeflag=0;

    // Create on Tree if its not there and fill it with one entry:
    if (TREE_ParaSetDefinitions==0 && (cmd_ALTP==0 || cmd_ALTP==2 || cmd_ALTP==4)) {
        if (TREE_ParaSetDefinitions==0) cout << "WARNING: In --- Read_ParasetDefinitionTree ---  Empty TREE_ParaSetDefinitions. Using only one, standard parameterset  (...) ."<<endl;
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_ZYLINDER_R_MAX",&tubedist,"CUT_ZYLINDER_R_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_ZYLINDER_ANGLE_MAX",&coneangle,"CUT_ZYLINDER_ANGLE_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max,"CUT_SHOWERFOLLOWERBT_DR_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max,"CUT_SHOWERFOLLOWERBT_DTAN_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_NHOLES_MAX",&nholes_max,"CUT_NHOLES_MAX/I");
        dr_max=100.0;
        dt_max=0.1;
        coneangle=0.1;
        tubedist=800.0;
        nholes_max=3;

        if (cmd_ALTP==0||cmd_ALTP==2) {
            if (cmd_CUTTP==2) {
                cout << "----- cmd_CUTTP==2 -----"<<endl;
                dr_max=100.0;
                dt_max=0.1;
                coneangle=0.1;
                tubedist=800.0;
            }
            else if (cmd_CUTTP==1) {
                cout << "----- cmd_CUTTP==1 -----"<<endl;
                dr_max=100.0;
                dt_max=0.1;
                coneangle=0.1;
                tubedist=800.0;
            }
            else {
                cout << "----- cmd_CUTTP==else -----"<<endl;
                dr_max=100.0;
                dt_max=0.1;
                coneangle=0.1;
                tubedist=800.0; // CA ALG, cmd_ALTP==2 , CT ALG, cmd_ALTP==0
            }
        }
        if (cmd_ALTP==4) {
            if (cmd_CUTTP==4) {
                cout << "----- cmd_CUTTP==4 : FJ_Standard -----"<<endl;
                dr_max=150.0;
                dt_max=0.15;
                coneangle=0.02;
                tubedist=800.0; // cuttype. FJ_Standard   // OI ALG, cmd_ALTP==4
            }
            if (cmd_CUTTP==3) {
                cout << "----- cmd_CUTTP==3 : FJ_HighPur -----"<<endl;
                dr_max=100.0;
                dt_max=0.05;
                coneangle=0.025;
                tubedist=400.0; // cuttype. FJ_HighPur   // OI ALG, cmd_ALTP==4
            }
            else if (cmd_CUTTP==2) {
                cout << "----- cmd_CUTTP==2 -----"<<endl;
                dr_max=200.0;
                dt_max=0.15;
                coneangle=0.03;
                tubedist=800.0; // cuttype. high eff   // OI ALG, cmd_ALTP==4
            }
            else if (cmd_CUTTP==1) {
                cout << "----- cmd_CUTTP==1 -----"<<endl;
                dr_max=120.0;
                dt_max=0.11;
                coneangle=0.02;
                tubedist=600.0; // cuttype. high pur   // OI ALG, cmd_ALTP==4
            }
            else {
                cout << "----- cmd_CUTTP==else -----"<<endl;
                dr_max=150.0;
                dt_max=0.15;
                coneangle=0.02;
                tubedist=800.0; // cuttype. FJ_Standard   // OI ALG, cmd_ALTP==4
            }
        }

        //cout << "----------- SAME VALUES AS IN EdbShowerRec FOR tESTING... REMOVE AFTER YOURE DONE HERE!!"<<endl;
        //if (cmd_ALTP==4) dr_max=150.0;dt_max=0.15;coneangle=0.02;tubedist=800.0; // OI ALG, cmd_ALTP==4

        // ParaSetNr  CUT_ZYLINDER_R_MAX CUT_ZYLINDER_ANGLE_MAX CUT_SHOWERFOLLOWERBT_DR_MAX CUT_SHOWERFOLLOWERBT_DTAN_MAX
        // 7001    800  0.1  100  0.1 (high NBT, middle purity)
        // 4401    500  0.5  100  0.1 (middle NBT, high purity) (comparable to off. Recoalg.);

        TREE_ParaSetDefinitions -> Fill();
        if (gEDBDEBUGLEVEL>2) cout << "--- TREE_ParaSetDefinitions -> GetEntries()  " << TREE_ParaSetDefinitions -> GetEntries()<<endl;
        if (gEDBDEBUGLEVEL>3) TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);

        // Reset here then also command line arguments:
        //     cmd_PASTART=0; cmd_PAEND=0;
        // Update CUT_PARAMETER[0,1,2,3,]
    }

    if (TREE_ParaSetDefinitions==0 && cmd_ALTP==3) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_ANN_INPUTNEURONS",&ann_inputneurons,"CUT_ANN_INPUTNEURONS/I");
        TREE_ParaSetDefinitions -> Branch("CUT_ANN_OUTPUT",&ann_output,"CUT_ANN_OUTPUT/D");
        ann_output=0.85;
        ann_inputneurons=5;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }

    if (TREE_ParaSetDefinitions==0 && cmd_ALTP==11) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions->Branch("ANN_PLATE_DELTANMAX",&N3_ANN_PLATE_DELTANMAX,"ANN_PLATE_DELTANMAX/I");
        TREE_ParaSetDefinitions->Branch("ANN_NTRAINEPOCHS",&N3_ANN_NTRAINEPOCHS,"ANN_NTRAINEPOCHS/I");
        TREE_ParaSetDefinitions->Branch("ANN_NHIDDENLAYER",&N3_ANN_NHIDDENLAYER,"ANN_NHIDDENLAYER/I");
        TREE_ParaSetDefinitions->Branch("ANN_EQUALIZESGBG",&N3_ANN_EQUALIZESGBG,"ANN_ANN_EQUALIZESGBG/I");
        TREE_ParaSetDefinitions->Branch("ANN_OUTPUTTHRESHOLD",&N3_ANN_OUTPUTTHRESHOLD,"ANN_OUTPUTTHRESHOLD/D");
        TREE_ParaSetDefinitions->Branch("ANN_INPUTNEURONS",&N3_ANN_INPUTNEURONS,"ANN_INPUTNEURONS/I");

        // Default, maximal settings. Same plate, Two plates up- downstream connections looking,
        // that for 5 inputvariables there
        // plus 4 fixed input varibles for BT(i) to InBT connections: 4+5*5 = 29
        N3_ANN_PLATE_DELTANMAX=5;
        N3_ANN_NHIDDENLAYER=5;
        N3_ANN_NTRAINEPOCHS=100;
        N3_ANN_EQUALIZESGBG=0;
        N3_ANN_INPUTNEURONS=29;
        N3_ANN_OUTPUTTHRESHOLD=0.85;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }

    if (TREE_ParaSetDefinitions==0 && cmd_ALTP==5) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_P",&CUT_P,"CUT_P/D");
        TREE_ParaSetDefinitions -> Branch("CUT_ALISUBSIZE",&CUT_ALISUBSIZE,"CUT_ALISUBSIZE/D");
        CUT_P=0;
        CUT_ALISUBSIZE=1000;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }

    if (TREE_ParaSetDefinitions==0 && cmd_ALTP==6) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_ZYLINDER_R_MAX",&tubedist,"CUT_ZYLINDER_R_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_ZYLINDER_ANGLE_MAX",&coneangle,"CUT_ZYLINDER_ANGLE_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max,"CUT_SHOWERFOLLOWERBT_DR_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max,"CUT_SHOWERFOLLOWERBT_DTAN_MAX/D");

        TREE_ParaSetDefinitions -> Branch("CUT_TRACKATTACH_DISTMIN",&distMin_max,"CUT_TRACKATTACH_DISTMIN/D");
        TREE_ParaSetDefinitions -> Branch("CUT_TRACKATTACH_DTAN_MAX",&distMin_dt_max,"CUT_TRACKATTACH_DTAN_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_TRACKATTACH_NTRACKSEG",&tracksegs_max,"CUT_TRACKATTACH_NTRACKSEG/I");

        dr_max=150.0;
        dt_max=0.13;
        coneangle=0.025;
        tubedist=700.0;
        distMin_max=500;
        tracksegs_max=1;
        distMin_dt_max=200;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }


    if (TREE_ParaSetDefinitions==0 && (cmd_ALTP==7 || cmd_ALTP==9)) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_ZYLINDER_R_MAX",&tubedist,"CUT_ZYLINDER_R_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_ZYLINDER_ANGLE_MAX",&coneangle,"CUT_ZYLINDER_ANGLE_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max,"CUT_SHOWERFOLLOWERBT_DR_MAX/D");
        TREE_ParaSetDefinitions -> Branch("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max,"CUT_SHOWERFOLLOWERBT_DTAN_MAX/D");
        dr_max=150.0;
        dt_max=0.13;
        coneangle=0.025;
        tubedist=700.0;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }


    if (TREE_ParaSetDefinitions==0 && cmd_ALTP==8) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_BACK_DMIN",&cut_back_dmin,"CUT_BACK_DMIN/D");
        TREE_ParaSetDefinitions -> Branch("CUT_BACK_DTHETA",&cut_back_dtheta,"CUT_BACK_DTHETA/D");
        TREE_ParaSetDefinitions -> Branch("CUT_BACK_DR",&cut_back_dr,"CUT_BACK_DR/D");
        TREE_ParaSetDefinitions -> Branch("CUT_BACK_DZ",&cut_back_dz,"CUT_BACK_DZ/D");
        TREE_ParaSetDefinitions -> Branch("CUT_FOR_DMIN",&cut_for_dmin,"CUT_FOR_DMIN/D");
        TREE_ParaSetDefinitions -> Branch("CUT_FOR_DTHETA",&cut_for_dtheta,"CUT_FOR_DTHETA/D");
        TREE_ParaSetDefinitions -> Branch("CUT_FOR_DR",&cut_for_dr,"CUT_FOR_DR/D");
        TREE_ParaSetDefinitions -> Branch("CUT_FOR_DZ",&cut_for_dz,"CUT_FOR_DZ/D");
        cut_back_dmin=cut_for_dmin=120;
        cut_back_dtheta=cut_for_dtheta=0.120;
        cut_back_dr=cut_for_dr=120;
        cut_back_dz=cut_for_dz=5000;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }

    if (TREE_ParaSetDefinitions==0 && cmd_ALTP==10) {
        TREE_ParaSetDefinitions = new TTree("ParaSet_Variables","ParaSet_Variables");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_DIP",&cut_gs_cut_dip,"CUT_GS_CUT_DIP/D");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_DMIN",&cut_gs_cut_dmin,"CUT_GS_CUT_DMIN/D");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_DR",&cut_gs_cut_dr,"CUT_GS_CUT_DR/D");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_DZ",&cut_gs_cut_dz,"CUT_GS_CUT_DZ/D");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_DTHETA",&cut_gs_cut_dtheta,"CUT_GS_CUT_DTHETA/D");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_PIDDIFF",&cut_gs_cut_piddiff,"CUT_GS_CUT_PIDDIFF/D");
        TREE_ParaSetDefinitions -> Branch("CUT_GS_CUT_OPPOSITEFLAG",&cut_gs_cut_oppositeflag,"CUT_GS_CUT_OPPOSITEFLAG/I");
        // PARAMETERSET_DEFINITIONFILE_LONG_GS_ALG.txt:  19642    393.212  35.5454  85.268  25062.6  0.117141  3  0
        cut_gs_cut_dip=393;
        cut_gs_cut_dmin=35.5;
        cut_gs_cut_dr=85.;
        cut_gs_cut_dz=25000;
        cut_gs_cut_dtheta=0.11;
        cut_gs_cut_piddiff=1;
        cut_gs_cut_oppositeflag=0;
        TREE_ParaSetDefinitions -> Fill();
        TREE_ParaSetDefinitions -> Show(TREE_ParaSetDefinitions -> GetEntries()-1);
        TREE_ParaSetDefinitions->Print();
    }


    // Now, TREE_ParaSetDefinitions exists and is filled with one entry at least
    // Then set branch addresses for the variables
    
    // Attention: Here is still a  __BUG__ possible.
    // If the file containing from Open_ParasetDefinitionFile contains a parameter-
    // set for the wrong algorithm, the program thinks it has a TREE_ParaSetDefinitions
    // (which it actually has), but the variables dont match, and then the program
    // crashes!
    cout << "Attention: Here is still a  __BUG__ possible. If the file containing from Open_ParasetDefinitionFile contains a parameter-" << endl;
    cout << "set for the wrong algorithm, the program thinks it has a TREE_ParaSetDefinitions (which it actually has), " << endl;
    cout << "but the variables dont match, and then the program crashes!" << endl;

    if     (cmd_ALTP==0) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
    }
    else if  (cmd_ALTP==1) {
        cout << "TODO"<<endl;
    }
    else if  (cmd_ALTP==2) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
    }
    else if  (cmd_ALTP==4) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_NHOLES_MAX",&nholes_max);
    }
    else if  (cmd_ALTP==5) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_P",&CUT_P);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ALISUBSIZE",&CUT_ALISUBSIZE);
    }
    else if  (cmd_ALTP==3) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ANN_OUTPUT",&ann_output);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_ANN_INPUTNEURONS",&ann_inputneurons);
    }

    else if   (cmd_ALTP==6) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_TRACKATTACH_DISTMIN",&distMin_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_TRACKATTACH_DTAN_MAX",&distMin_dt_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_TRACKATTACH_NTRACKSEG",&tracksegs_max);
    }
    else if   (cmd_ALTP==7) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
    }
    else if   (cmd_ALTP==8) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DMIN",&cut_back_dmin);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DTHETA",&cut_back_dtheta);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DR",&cut_back_dr);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DZ",&cut_back_dz);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DMIN",&cut_for_dmin);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DTHETA",&cut_for_dtheta);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DR",&cut_for_dr);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DZ",&cut_for_dz);
    }
    else if (cmd_ALTP==10) {
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DIP",&cut_gs_cut_dip);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DMIN",&cut_gs_cut_dmin);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DR",&cut_gs_cut_dr);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DZ",&cut_gs_cut_dz);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DTHETA",&cut_gs_cut_dtheta);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_PIDDIFF",&cut_gs_cut_piddiff);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_OPPOSITEFLAG",&cut_gs_cut_oppositeflag);
    }
    else if (cmd_ALTP==11) {
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_PLATE_DELTANMAX",&N3_ANN_PLATE_DELTANMAX);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_NTRAINEPOCHS",&N3_ANN_NTRAINEPOCHS);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_NHIDDENLAYER",&N3_ANN_NHIDDENLAYER);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_EQUALIZESGBG",&N3_ANN_EQUALIZESGBG);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_INPUTNEURONS",&N3_ANN_INPUTNEURONS);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_OUTPUTTHRESHOLD",&N3_ANN_OUTPUTTHRESHOLD);
    }

    // Check: if PASTART is given (a number),  but PAEND is default,
    // then set PAEND to PASTART
    if (cmd_PASTART>=0 && cmd_PAEND==-1) cmd_PAEND=cmd_PASTART;

    if (gEDBDEBUGLEVEL>2) cout << "--- Updated commandline values: cmd_PASTART=" << cmd_PASTART << " and  cmd_PAEND=" << cmd_PAEND << endl;

    Log(2, "ShowRec.cpp", "--- Int_t Read_ParasetDefinitionTree() done.");
    return;
}






//-------------------------------------------------------------------------------------------
void GetEvent_ParasetDefinitionTree(Int_t nr)
{
    Log(2, "ShowRec.cpp", "--- void GetEvent_ParasetDefinitionTree( Int_t %d) ---", nr);

    // Get the Parameter set definition variables for all the possible algorithms.
    // We need to distinguish the parameter values from the TreeDefinition file
    // for each algorithm.

    // ALTP 1..9: diverse algorithm cut parameter values.
    Double_t dr_max,dt_max,coneangle,tubedist;
    Int_t nholes_max;
    Double_t ann_output;
    Int_t ann_inputneurons;
    Double_t distMin_max;
    Int_t tracksegs_max;
    Double_t distMin_dt_max;
    Double_t cut_back_dmin,cut_for_dmin,cut_back_dtheta,cut_for_dtheta,cut_back_dr,cut_for_dr,cut_back_dz,cut_for_dz;

    // ALTP 10:  GS from libShowRec
    Double_t cut_gs_cut_dip=150;
    Double_t cut_gs_cut_dmin=40;
    Double_t cut_gs_cut_dr=60;
    Double_t cut_gs_cut_dz=19000;
    Double_t cut_gs_cut_dtheta=0.06;
    Double_t cut_gs_cut_piddiff=1;
    Int_t cut_gs_cut_oppositeflag=0;

    // ALTP 11:  N3 Alg
    Double_t     ANN_OUTPUT;
    Int_t        ANN_INPUTLEVEL;
    Int_t        ANN_PLATEN;
    Int_t        ANN_PLATEDIRECTION;
    Int_t        ANN_HIDDENLAYER;
    Int_t        ANN_INPUTNEURONS;


    // Reset Cut Paramters, just for safety reasons,
    // not to leave them uninitialized!
    for (int i=0; i<10; i++ ) {
        CUT_PARAMETER[i]=0.0;
    }

    // If the "nr" equals -1, then there is no given TREE_ParaSetDefinitions
    // That means, the default set of values is taken, i.e. entry zero.
    // So nr is set to 0:
    if (nr==-1)  {
        nr=0;
//         TREE_ParaSetDefinitions->GetEntry(nr);
        cout << "--- Got TREE_ParaSetDefinitions->GetEntry(0) instead of -1 due to no given PARAMETERSET_DEFINITIONFILE.root file."<<endl;
//         TREE_ParaSetDefinitions->Show(nr);

    }

    //TREE_ParaSetDefinitions->Print();

    // Switches Statements would be nicer, but anyway.
    if     (cmd_ALTP==0) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=tubedist;
        CUT_PARAMETER[1]=coneangle;
        CUT_PARAMETER[2]=dr_max;
        CUT_PARAMETER[3]=dt_max;
    }
    else if  (cmd_ALTP==1) {
        cout << "cmd_ALTP==1 TODO"<<endl;
    }
    else if  (cmd_ALTP==2) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=tubedist;
        CUT_PARAMETER[1]=coneangle;
        CUT_PARAMETER[2]=dr_max;
        CUT_PARAMETER[3]=dt_max;
    }
    else if  (cmd_ALTP==3) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ANN_OUTPUT",&ann_output);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_ANN_INPUTNEURONS",&ann_inputneurons);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=ann_output;
        CUT_PARAMETER[1]=ann_inputneurons;

        cout << "TODO"<<endl;
        // I DONT KNOW WHY I HAVE THAT WRITTEN, BUT AT SOME POINT MUST HAVE MADE SOME SENSE .....

    }
    else if  (cmd_ALTP==4) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_NHOLES_MAX",&nholes_max);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=tubedist;
        CUT_PARAMETER[1]=coneangle;
        CUT_PARAMETER[2]=dr_max;
        CUT_PARAMETER[3]=dt_max;
        CUT_PARAMETER[4]=nholes_max;

    }
    else if  (cmd_ALTP==5) {
        Double_t  CUT_P;                        // s->P()
        Double_t  CUT_ALISUBSIZE;               // eAli_local_half_size

        TREE_ParaSetDefinitions->SetBranchAddress("CUT_P",&CUT_P);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ALISUBSIZE",&CUT_ALISUBSIZE);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=CUT_P;
        CUT_PARAMETER[1]=CUT_ALISUBSIZE;
    }
    else if   (cmd_ALTP==6) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_TRACKATTACH_DISTMIN",&distMin_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_TRACKATTACH_DTAN_MAX",&distMin_dt_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_TRACKATTACH_NTRACKSEG",&tracksegs_max);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=tubedist;
        CUT_PARAMETER[1]=coneangle;
        CUT_PARAMETER[2]=dr_max;
        CUT_PARAMETER[3]=dt_max;
        CUT_PARAMETER[4]=distMin_max;
        CUT_PARAMETER[5]=distMin_dt_max;
        CUT_PARAMETER[6]=tracksegs_max;
    }
    else if   (cmd_ALTP==7||cmd_ALTP==9) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_R_MAX",&tubedist);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_ZYLINDER_ANGLE_MAX",&coneangle);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DR_MAX",&dr_max);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_SHOWERFOLLOWERBT_DTAN_MAX",&dt_max);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=tubedist;
        CUT_PARAMETER[1]=coneangle;
        CUT_PARAMETER[2]=dr_max;
        CUT_PARAMETER[3]=dt_max;
    }
    else if   (cmd_ALTP==8) {
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DMIN",&cut_back_dmin);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DTHETA",&cut_back_dtheta);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DR",&cut_back_dr);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_BACK_DZ",&cut_back_dz);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DMIN",&cut_for_dmin);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DTHETA",&cut_for_dtheta);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DR",&cut_for_dr);
        TREE_ParaSetDefinitions->SetBranchAddress("CUT_FOR_DZ",&cut_for_dz);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=cut_back_dmin;
        CUT_PARAMETER[1]=cut_back_dtheta;
        CUT_PARAMETER[2]=cut_back_dr;
        CUT_PARAMETER[3]=cut_back_dz;
        CUT_PARAMETER[4]=cut_for_dmin;
        CUT_PARAMETER[5]=cut_for_dtheta;
        CUT_PARAMETER[6]=cut_for_dr;
        CUT_PARAMETER[7]=cut_for_dz;
    }
    else if   (cmd_ALTP==10) {
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DIP",&cut_gs_cut_dip);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DMIN",&cut_gs_cut_dmin);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DR",&cut_gs_cut_dr);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DZ",&cut_gs_cut_dz);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_DTHETA",&cut_gs_cut_dtheta);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_PIDDIFF",&cut_gs_cut_piddiff);
        TREE_ParaSetDefinitions -> SetBranchAddress("CUT_GS_CUT_OPPOSITEFLAG",&cut_gs_cut_oppositeflag);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=cut_gs_cut_dip;
        CUT_PARAMETER[1]=cut_gs_cut_dmin;
        CUT_PARAMETER[2]=cut_gs_cut_dr;
        CUT_PARAMETER[3]=cut_gs_cut_dz;
        CUT_PARAMETER[4]=cut_gs_cut_dtheta;
        CUT_PARAMETER[5]=cut_gs_cut_piddiff;
        CUT_PARAMETER[6]=cut_gs_cut_oppositeflag;
    }
    else if   (cmd_ALTP==11) {
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_PLATE_DELTANMAX",&N3_ANN_PLATE_DELTANMAX);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_NTRAINEPOCHS",&N3_ANN_NTRAINEPOCHS);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_NHIDDENLAYER",&N3_ANN_NHIDDENLAYER);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_EQUALIZESGBG",&N3_ANN_EQUALIZESGBG);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_INPUTNEURONS",&N3_ANN_INPUTNEURONS);
        TREE_ParaSetDefinitions -> SetBranchAddress("ANN_OUTPUTTHRESHOLD",&N3_ANN_OUTPUTTHRESHOLD);

        TREE_ParaSetDefinitions->GetEntry(nr);

        CUT_PARAMETER[0]=N3_ANN_PLATE_DELTANMAX;
        CUT_PARAMETER[1]=N3_ANN_NTRAINEPOCHS;
        CUT_PARAMETER[2]=N3_ANN_NHIDDENLAYER;
        CUT_PARAMETER[3]=N3_ANN_INPUTNEURONS; // not used in input, but anywy set here.
        CUT_PARAMETER[4]=N3_ANN_OUTPUTTHRESHOLD;
        CUT_PARAMETER[5]=N3_ANN_EQUALIZESGBG;
    }


    if (TREE_ParaSetDefinitions) TREE_ParaSetDefinitions->Show(nr);

    cout << "--- CUT_PARAMETER 0 1 2 3: " << CUT_PARAMETER[0] <<"  "<<CUT_PARAMETER[1]<<"  "<< CUT_PARAMETER[2] <<"  "<< CUT_PARAMETER[3] <<endl;
    cout << "--- CUT_PARAMETER 4 5 6 7: " << CUT_PARAMETER[4] <<"  "<<CUT_PARAMETER[5]<<"  "<< CUT_PARAMETER[6]<< "  "<< CUT_PARAMETER[7]<<"  "<<endl;
    return;
}




//-------------------------------------------------------------------------------------------


void ReconstructShowers(Int_t nr)
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers() for parameterset %d---",nr);

    //-----------------------------------
    // Create the ShowerOutputtree:
    TREE_ShowShower = CreateTreeBranchShowerTree(GLOBAL_PARASETNR);
    //-----------------------------------

    //-----------------------------------
    // Call main reconstruction function:
    //-----------------------------------
    if     (cmd_ALTP==0) {
        cout << "ReconstructShowers::   cmd_ALTP==0   ReconstructShowers_CT()    IS NOW THE SAME AS CA ALG!  "<< endl;
        cout << "ReconstructShowers::   cmd_ALTP==0   ReconstructShowers_CTA()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_CT();
    }
    else if  (cmd_ALTP==1) {
        cout << "TODO... CL ...."<<endl;
        cout << "ReconstructShowers::   cmd_ALTP==1   ReconstructShowers_CL()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_CL();
    }
    else if  (cmd_ALTP==2) {
        cout << "ReconstructShowers::   cmd_ALTP==2   ReconstructShowers_CA()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_CA();
    }
    else if  (cmd_ALTP==3) {
        cout << "ReconstructShowers::   cmd_ALTP==3   ReconstructShowers_NN()    Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_NN();
    }
    else if  (cmd_ALTP==4) {
        cout << "ReconstructShowers::   cmd_ALTP==4   ReconstructShowers_OI()    Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_OI();
    }
    else if  (cmd_ALTP==5) {
        cout << "ReconstructShowers::   cmd_ALTP==5   ReconstructShowers_SA()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_SA();
    }
    else if   (cmd_ALTP==6) {
        cout << "ReconstructShowers::   cmd_ALTP==6   ReconstructShowers_TC()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_TC();
    }
    else if   (cmd_ALTP==7) {
        cout << "ReconstructShowers::   cmd_ALTP==7   ReconstructShowers_RC()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_RC();
    }
    else if   (cmd_ALTP==8) {
        cout << "ReconstructShowers::   cmd_ALTP==8   ReconstructShowers_BW()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_BW();
    }
    else if   (cmd_ALTP==9) {
        cout << "ReconstructShowers::   cmd_ALTP==9   ReconstructShowers_AG()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_AG();
    }
    else if   (cmd_ALTP==10) {
        cout << "ReconstructShowers::   cmd_ALTP==10  ReconstructShowers_GS()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_GS();
    }
    else if   (cmd_ALTP==11) {
        cout << "ReconstructShowers::   cmd_ALTP==11  ReconstructShowers_N3()   Reconstruction of ParameterSet: "<< nr <<endl;
        ReconstructShowers_N3();
    }
    else if   (cmd_ALTP==999) {
        cout << "ReconstructShowers::   cmd_ALTP==9999  OpenMP TEST!!!!!  PROBABLY TO BE DEPRECATED ..."<<endl;
        ReconstructShowers_OPENMP();
    }
    //-----------------------------------


    //-----------------------------------
    if (TREE_ShowShower->GetEntries()<1) {
        cout << "//-----------------------------------------------------------------------------//"<<endl;
        cout << "TREE_ShowShower->GetEntries()<1.  NO SHOWER HAS BEEN RECONSTRUCTED."<<endl;
        cout << "CHECK INPUT BTs, INPUT PARAMETERS, SOURCE FILES, etc.."<<endl;
        cout << "//-----------------------------------------------------------------------------//"<<endl;
    }
    cout << "TREE_ShowShower->GetEntries() = " << TREE_ShowShower->GetEntries()<< endl;
    //-----------------------------------


    //-----------------------------------
    // MakeShowerTree:
    // Writing showers in treebranch style
    // as done by FJ algorithm.
    //-----------------------------------
    FILE_ShowShower->cd();
    TREE_ShowShower->Write("",TObject::kOverwrite);
    //-----------------------------------


    //-----------------------------------
    // MakeTracksTree:
    // Writing showers in tracks style
    // copied almost verbatim from GL.
    //-----------------------------------
    ///MakeTracksTree(TREE_ShowShower);
    cout << "Make Tracks Tree commented out!!!"<< endl;
    //-----------------------------------


    //-----------------------------------
    // Delete the ShowerOutputtree:
    delete TREE_ShowShower;
    TREE_ShowShower=0;
    //-----------------------------------
    return;
}
//-------------------------------------------------------------------------------------------





























































Int_t Reco_CL_AssignZValueToGroup(Double_t z, Double_t z0, Int_t NGroupedPLATES, Int_t PLATESPerGroup)
{
    // z:  Position of BT to be checked,
    // z0: ZStart if for example FIRSTPLATE z=3900 (PLATE 4) instead of 0
    Int_t actplate=(Int_t)(z-z0)/1300+1;
    Int_t actgroup=(actplate-1)/PLATESPerGroup;
    //  cout << "z  z0 actplate "<< z << "   " << z0 << "  " <<actplate << endl;
    //  cout << "actplate  actgroup "<< actplate << "   " << actgroup << endl;
    //This Assignment seems ok.
    return actgroup;
}

void Reco_CL_BuildGlobalHistogramsOnHeap()
{
    TString HistoNameTitle="XXX";
    for (Int_t h=0; h<50; h++) {
        //    cout << "Creating    Hist2DimOnlyBG_Groupe_ " << h << endl;
        HistoNameTitle=TString(Form("Hist2DimOnlyBG_Groupe_%d",h));
        Hist2DimOnlyBGOneGroupedPlate[h]=new TH2F(HistoNameTitle,HistoNameTitle,10,0,1,10,0,1);
        //    cout << "Creating    Hist2DimOnlySimOneEvent_Groupe_ " << h << endl;
        HistoNameTitle=TString(Form("Hist2DimOnlySimOneEvent_Groupe_%d",h));
        Hist2DimOnlySimOneEventOneGroupedPlate[h]=new  TH2F(HistoNameTitle,HistoNameTitle,10,0,1,10,0,1);
        //    cout << "Creating    Hist2DimBGAndSimOneEvent_Groupe_ " << h << endl;
        HistoNameTitle=TString(Form("Hist2DimBGAndSimOneEvent_Groupe_%d",h));
        Hist2DimBGAndSimOneEventOneGroupedPlate[h]=new  TH2F(HistoNameTitle,HistoNameTitle,10,0,1,10,0,1);
    }
    return;
}



//-------------------------------------------------------------------------------------------
void ReconstructShowers_CL()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_CL() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of CLuster Algorithm
    //-----------------------------------------------------------------
    //
    // For each InitiatorBT this is
    // divided in several small parts:
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    //-----------------------------------
    //   0)  Predefinitions
    //-----------------------------------

    // Set ClusterBoxSize of X,Y in microns: (f.e. 50x50)
    Float_t BOXSIZE=50;
    Float_t ClusterBoxSizeX=BOXSIZE;
    Float_t ClusterBoxSizeY=ClusterBoxSizeX;
    Float_t ClusterBoxSize[]= {ClusterBoxSizeX,ClusterBoxSizeY};

    // Set GLOBAL Boundaries of the BrickSize, depends on your MC Simulation and on your scanned BG size
    // Later we will set the frame size, which can be f.e. 1mmX1mm, 3mmX3mm ...
    // This does not need to be changed...
    Float_t LeftX=-11000;
    Float_t RightX=21000;
    Float_t LeftY=-11000;
    Float_t RightY=16000;

    cout << " ......."<<endl;
    // Set SquareSize of The gAliSub -> Coorseponds to the totral scan Area:
    Float_t VIEWSIZE=1000;
    Float_t gAliSub_Length_X=VIEWSIZE;
    Float_t gAliSub_Length_Y=VIEWSIZE; //1mm

    // Calculate how many NbinsX,Y are needed:
    Int_t NbinsX=Int_t((RightX-LeftX)/ClusterBoxSizeX);
    Int_t NbinsY=Int_t((RightY-LeftY)/ClusterBoxSizeY);


    cout << " ......."<<endl;

    Int_t NPLATES=GLOBAL_gAli->Npatterns();
    Int_t NbinsZ=NPLATES;

    // Calculate how many NbinsX,Y are needed:
    Int_t NbinsX_gAliSub=Int_t((gAliSub_Length_X)/ClusterBoxSizeX);
    Int_t NbinsY_gAliSub=Int_t((gAliSub_Length_Y)/ClusterBoxSizeY);

    // Arrays for the Spectrum source and dests...
    // Float_t ** source = new float *[NbinsX];
    Double_t ** source = new double *[NbinsX];
    for (Int_t cnti=0; cnti<NbinsX; cnti++) {
        // source[cnti]=new float[NbinsY];
        source[cnti]=new double[NbinsY];
    }
    // Float_t ** dest = new float *[NbinsX];
    Double_t ** dest = new double *[NbinsX];
    for (Int_t cnti=0; cnti<NbinsX; cnti++) {
        // dest[cnti]=new float[NbinsY];
        dest[cnti]=new double[NbinsY];
    }

    // Arrays for the Spectrum source and dests...
    // Unused ??? Not to be found in any other part of the code ...
    // Comment out!
    /*
    Float_t ** source_gAliSub = new float *[NbinsX_gAliSub];
    for (Int_t cnti=0; cnti<NbinsX_gAliSub; cnti++) {
        source_gAliSub[cnti]=new float[NbinsY_gAliSub];
    }
    Float_t ** dest_gAliSub = new float *[NbinsX_gAliSub];
    for (Int_t cnti=0; cnti<NbinsX_gAliSub; cnti++) {
        dest_gAliSub[cnti]=new float[NbinsY_gAliSub];
    }
    */

    // Fill Extrapolated BT with direction into next plate?
    Bool_t FillTANData=kTRUE;
    cout << " ......."<<endl;

    // Plates to be grouped Together:
    Int_t  PLATESPERGROUP=1;
    Int_t  FIRSTPLATE=0;
    Int_t LASTPLATE=0;
    Int_t  MAXPLATE=GLOBAL_gAli->Npatterns()-1;
    Int_t NGroupedPLATES=ceil( (Double_t)NPLATES/(Double_t)PLATESPERGROUP);

    // Spectrum Position Peaks
    Float_t* fPositionX;
    Float_t* fPositionY;
    Int_t fNPeaks;

    // Start Position...
    Float_t StartPosX,StartPosY,StartPosTX,StartPosTY;

    // Calculate the z-positions for the grouped plates:
    Float_t ZPosGroupedPlates[50];
    ///=== DEBUG===
    cout << "gAli->GetPattern(firstplate)->GetSegment(0)->Z;  "<<GLOBAL_gAli->GetPattern(0)->Z()<<endl;
    cout << "gAli->GetPattern(lastplate)->GetSegment(0)->Z;  "<<GLOBAL_gAli->GetPattern(GLOBAL_gAli->Npatterns()-1)->Z()<<endl;
    ///===END DEBUG===

    for (Int_t i=0; i<NGroupedPLATES; i++) {
        ZPosGroupedPlates[i]=GLOBAL_gAli->GetPattern(0)->Z()+PLATESPERGROUP*1300*i;
        ///===DEBUG===
        cout << "i PLATESPERGROUP NGroupedPLATES ZPosGroupedPlates[i] " << i << " " <<  PLATESPERGROUP << " " << NGroupedPLATES  << " " << ZPosGroupedPlates[i] << " " << endl;
    }

    ///====== DEBUG===
    if (gEDBDEBUGLEVEL==2) {
        cout << "NbinsX = " << NbinsX << endl;
        cout << "NbinsY = " << NbinsY << endl;
        cout << "NbinsZ = " << NbinsZ << endl;
        cout << "NbinsX_gAliSub = " << NbinsX_gAliSub << endl;
        cout << "NbinsY_gAliSub = " << NbinsY_gAliSub << endl;
        cout << "FIRSTPLATE=  "<<FIRSTPLATE<<endl;
        cout << "LASTPLATE=  "<<LASTPLATE<<endl;
        cout << "MAXPLATE=  "<<MAXPLATE<<endl;
        cout << "NPLATES=  "<<NPLATES<<endl;
        cout << "NGroupedPLATES=  "<<NGroupedPLATES<<endl;
        cout << "PLATESPERGROUP=  "<<PLATESPERGROUP<<endl;
    }
    ///===END DEBUG===

    //=========================================
    // Create the 2Dim  Histograms:
    Reco_CL_BuildGlobalHistogramsOnHeap();
    // Create the 2Dim  Histograms:
    Hist2DimOnlyBGAllPlates= new TH2F();
    Hist2DimOnlySimOneEventAllPlates= new TH2F();
    Hist2DimBGAndSimOneEventAllPlates= new TH2F();
    // Create the 3Dim  Histograms:
    Hist3DimOnlyBG = new TH3F();
    Hist3DimRecoEvent_gAli = new TH3F();
    //=========================================
    // Allocate TSectrum2 pointer on the heap, and try to speed up program....
    spectrum2dim= new TSpectrum2();
    spectrum2dim->Print();
    //=========================================


    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;
    cout << "GLOBAL_InBTArrayEntries = " << GLOBAL_InBTArrayEntries << endl;

    TClonesArray  *as;
    EdbSegP  *sa;
    EdbPattern  *as_pattern;
    EdbPattern  *as_pattern_sub;


    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    //   for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=GLOBAL_InBTArrayEntries-1; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        if (gEDBDEBUGLEVEL==1) {
            int modulo=GLOBAL_InBTArrayEntries/20;
            if ((i%modulo)==0) cout << i <<" : 5% more done"<<endl;
        }
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------

        //-----------------------------------
        // 1a) Set Some Variables
        //-----------------------------------
        StartPosX=InBT->X();
        StartPosY=InBT->Y();
        StartPosTX=InBT->TX();
        StartPosTY=InBT->TY();
        //-----------------------------------

        //-----------------------------------
        //  1b) Now SetBins for the 2D, 3D Histograms for THIS (specific) Event
        //-----------------------------------

        cout << "Reset now some other histos"<<endl;
        for (Int_t h=0; h<50; h++) {
            Hist2DimOnlyBGOneGroupedPlate[h]->Reset();
            Hist2DimOnlySimOneEventOneGroupedPlate[h]->Reset();
            Hist2DimBGAndSimOneEventOneGroupedPlate[h]->Reset();
        }

        cout << "Reset now some other SetNameTitle "<<endl;
        Hist2DimOnlyBGAllPlates->SetNameTitle("Hist2DimOnlyBGAllPlates","Hist2DimOnlyBGAllPlates");
        Hist2DimOnlySimOneEventAllPlates->SetNameTitle("Hist2DimOnlySimOneEventAllPlates","Hist2DimOnlySimOneEventAllPlates");
        Hist2DimBGAndSimOneEventAllPlates->SetNameTitle("Hist2DimBGAndSimOneEventAllPlates","Hist2DimBGAndSimOneEventAllPlates");
        Hist2DimOnlyBGAllPlates->SetBins(  NbinsX_gAliSub, StartPosX-gAliSub_Length_X/2.0, StartPosX +gAliSub_Length_X/2.0, NbinsY_gAliSub, StartPosY-gAliSub_Length_Y/2.0, StartPosY +gAliSub_Length_Y/2.0);
        Hist2DimOnlySimOneEventAllPlates->SetBins(  NbinsX_gAliSub, StartPosX-gAliSub_Length_X/2.0, StartPosX +gAliSub_Length_X/2.0, NbinsY_gAliSub, StartPosY-gAliSub_Length_Y/2.0, StartPosY +gAliSub_Length_Y/2.0);
        Hist2DimBGAndSimOneEventAllPlates->SetBins(  NbinsX_gAliSub, StartPosX-gAliSub_Length_X/2.0, StartPosX +gAliSub_Length_X/2.0, NbinsY_gAliSub, StartPosY-gAliSub_Length_Y/2.0, StartPosY +gAliSub_Length_Y/2.0);

        cout << "Reset now some other SetNameTitle "<<endl;
        Hist3DimOnlyBG->SetNameTitle("Hist3DimOnlyBG","Hist3DimOnlyBG");
        Hist3DimOnlyBG->SetBins( NbinsX_gAliSub, StartPosX -gAliSub_Length_X/2.0, StartPosX +gAliSub_Length_X/2.0, NbinsY_gAliSub, StartPosY -gAliSub_Length_Y/2.0, StartPosY +gAliSub_Length_Y/2.0, NGroupedPLATES, (FIRSTPLATE-1)*1300, NbinsZ*1300);
        Hist3DimRecoEvent_gAli->SetNameTitle("Hist3DimRecoEvent_gAli","Hist3DimRecoEvent_gAli");
        Hist3DimRecoEvent_gAli->SetBins( NbinsX, LeftX, RightX, NbinsY, LeftY, RightY, NGroupedPLATES, (FIRSTPLATE-1)*1300, (LASTPLATE)*1300);

        cout << "Reset now some other SetNameTitle "<<endl;
        Hist2DimOnlyBGAllPlates->Reset();
        Hist2DimOnlySimOneEventAllPlates->Reset();
        Hist2DimBGAndSimOneEventAllPlates->Reset();
        Hist3DimOnlyBG->Reset();
        Hist3DimRecoEvent_gAli->Reset();

        //-----------------------------------
        // 1c) Define the Histograms Bounds and Bins,
        // 1c) Since Bounds are depending on h ...
        //-----------------------------------
        Float_t InBT_X=InBT->X();
        Float_t InBT_TX=InBT->TX();
        Float_t InBT_X_Extrapolated;
        Float_t InBT_Y=InBT->Y();
        Float_t InBT_TY=InBT->TY();
        Float_t InBT_Y_Extrapolated;
        Float_t InBT_Z=InBT->Z();
        for (Int_t h=0; h<NGroupedPLATES; h++) {
            //cout << "--------------------"<<endl;
            Float_t zdiff=ZPosGroupedPlates[h]-InBT_Z;
            InBT_X_Extrapolated=InBT_X+InBT_TX*zdiff;
            InBT_Y_Extrapolated=InBT_Y+InBT_TY*zdiff;
            cout << "h InBT_X InBT_TX InBT_Z ZPosGroupedPlates[h] zdiff  InBT_X_Extrapolated " << h << " " <<  InBT_X << " " << InBT_TX << " " << InBT_Z << " " << ZPosGroupedPlates[h] << " " << zdiff << " " <<InBT_X_Extrapolated << endl;
            cout << "h InBT_Y InBT_TY InBT_Z ZPosGroupedPlates[h] zdiff  InBT_Y_Extrapolated " << h << " " <<  InBT_Y << " " << InBT_TY << " " << InBT_Z << " " << ZPosGroupedPlates[h] << " " << zdiff << " " <<InBT_Y_Extrapolated << endl;
            Hist2DimOnlyBGOneGroupedPlate[h]->SetBins(NbinsX_gAliSub, InBT_X_Extrapolated-gAliSub_Length_X/2.0,InBT_X_Extrapolated+gAliSub_Length_X/2.0, NbinsY_gAliSub,InBT_Y_Extrapolated-gAliSub_Length_Y/2.0,InBT_Y_Extrapolated+gAliSub_Length_Y/2.0);
            Hist2DimOnlySimOneEventOneGroupedPlate[h]->SetBins(NbinsX_gAliSub, InBT_X_Extrapolated-gAliSub_Length_X/2.0,InBT_X_Extrapolated+gAliSub_Length_X/2.0, NbinsY_gAliSub,InBT_Y_Extrapolated-gAliSub_Length_Y/2.0,InBT_Y_Extrapolated+gAliSub_Length_Y/2.0);
            Hist2DimBGAndSimOneEventOneGroupedPlate[h]->SetBins(NbinsX_gAliSub, InBT_X_Extrapolated-gAliSub_Length_X/2.0,InBT_X_Extrapolated+gAliSub_Length_X/2.0, NbinsY_gAliSub,InBT_Y_Extrapolated-gAliSub_Length_Y/2.0,InBT_Y_Extrapolated+gAliSub_Length_Y/2.0);
        }
        //=========================================



        //-----------------------------------
        // 1c) Loop over (whole) local_gAli, Fill the Histograms  (from firstplate up to lastplate)
        //-----------------------------------
        Int_t val, val_tan;
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            as = (TClonesArray*)local_gAli->GetPattern(patterloop_cnt)->GetSegments();
            as_pattern= (EdbPattern*)local_gAli->GetPattern(patterloop_cnt);

            cout << "===DEBUG   patterloop_cnt=  " << patterloop_cnt << "   Z pos (k) " << local_gAli->GetPattern(patterloop_cnt)->Z() << endl;  // get Z pos of the plate k
            cout << "===DEBUG   (patterloop_cnt) EdbSegments in the Pattern (total): " << as->GetEntries() << endl;

            for (Int_t h=0; h<as->GetEntries(); h++) {
                sa = (EdbSegP*)( as->At(h) );
                ///if (sa->X()<LeftX || sa->X()>RightX || sa->Y()<LeftY || sa->Y()>RightY) continue;  /// DEBUG
                val=Reco_CL_AssignZValueToGroup(sa->Z(), ZPosGroupedPlates[0], NGroupedPLATES, PLATESPERGROUP);
                cout << "entry, val  "<<h << "  "  << val<<endl;
                Hist2DimBGAndSimOneEventOneGroupedPlate[val]->Fill(sa->X(),sa->Y());
                if (sa->MCEvt()<0) Hist2DimOnlyBGOneGroupedPlate[val]->Fill(sa->X(),sa->Y());
                if (FillTANData) {
                    val_tan=Reco_CL_AssignZValueToGroup(sa->Z()+1300, ZPosGroupedPlates[0], NGroupedPLATES, PLATESPERGROUP);
                    cout << "(TAN) entry, val_tan  "<<h << "  "  << val_tan<<endl;
                    if (val_tan>=NGroupedPLATES) {
                        /// --val_tan; // Project the last on beack to its owns
                        ++val_tan; // Project the last on beack to its owns
                        cout << "(TAN) val_tan changed to "<<h << "  "  << val_tan<<endl;
                    }
                    Hist2DimBGAndSimOneEventOneGroupedPlate[val_tan]->Fill(sa->X()+sa->TX()*1300,sa->Y()+sa->TY()*1300);
                    if (sa->MCEvt()<0) Hist2DimOnlyBGOneGroupedPlate[val_tan]->Fill(sa->X()+sa->TX()*1300,sa->Y()+sa->TY()*1300);
                }
                if (sa->MCEvt()<0) continue;
                if (sa->MCEvt()!=GLOBAL_InBT_MC) continue;
                cout << "===   ==DEBUG   Filling i with MCEvt  "<< i <<"("<< GLOBAL_InBT_MC <<")  X,Y,Z: "<<sa->X()<<" "<<sa->Y()<<" "<<sa->Z()<<"  "<<sa->MCEvt()<< "   to val= " <<val <<endl;
                Hist2DimOnlySimOneEventOneGroupedPlate[val]->Fill(sa->X(),sa->Y());
                if (FillTANData) {
                    Hist2DimOnlySimOneEventOneGroupedPlate[val_tan]->Fill(sa->X()+sa->TX()*1300,sa->Y()+sa->TY()*1300);
                }
            }

        } //of Loop over all plates of local_gAli,


        cout<<endl<<endl;
        cout <<"Loop over the Grouped plates and search for spectrum peaks in each groupedPlate."<<endl;
        ///-------------------------------------------
        /// ====   Now loop over the Grouped plates and search for spectrum peaks in each groupedPlate...
        for (Int_t h=0; h<NGroupedPLATES; h++) {

            /// Set the bin arrays for the Spectrum...
            for (Int_t cnti = 0; cnti < NbinsX_gAliSub; cnti++) {
                for (Int_t cntj = 0; cntj < NbinsY_gAliSub; cntj++) {
                    source[cnti][cntj] = 0; // Reset source before filling it new...
                    dest[cnti][cntj] = 0; // Reset dest before filling it new...
                    source[cnti][cntj] = Hist2DimBGAndSimOneEventOneGroupedPlate[h]->GetBinContent(cnti + 1,cntj + 1);
                }
            }
            ///--------------------------------------------
            /// Fit each grouped Plate with Spectrum...
            Int_t nfound=0;
            cout << " Do now peak search..."<<endl;
            nfound = spectrum2dim->SearchHighRes(source, dest, NbinsX_gAliSub, NbinsY_gAliSub, 2, 20, kTRUE, 10, kFALSE, 5);// TO BE OPTIMIZED...
            cout << " Peak search finished. Go on."<<endl;
            ///--------------------------------------------
            spectrum_interim=(TH2F*)Hist2DimBGAndSimOneEventOneGroupedPlate[h]->Clone();
            spectrum_interim->Reset();
            ///.---------------
            Int_t interimsbin=0;
            TAxis *xax;
            TAxis *yax;
            TAxis *zax;
            Double_t xax_bin_value;
            Double_t yax_bin_value;
            Double_t zax_bin_value;
            Double_t value;

            /// Fill interimSpectrum For Drawing... without the THRESHOLD_SMOOTHED_DEST Cut to find Maximum
            for (Int_t ii = 0; ii < NbinsX_gAliSub; ii++) {
                for (Int_t jj = 0; jj < NbinsY_gAliSub; jj++) {
                    value=dest[ii][jj];
                    spectrum_interim->SetBinContent(ii + 1,jj + 1, value);
                }
            }
            Float_t THRESHOLD_SMOOTHED_DEST=0.5;
            cout << "THRESHOLD_SMOOTHED_DEST  spectrum_interim->GetMaximum()  : " << THRESHOLD_SMOOTHED_DEST << "  " << spectrum_interim->GetMaximum()<< endl;
            cout << "----------_"<<endl;

            Double_t spec_int_maximum=spectrum_interim->GetMaximum();

            /// Fill interimSpectrum For Drawing...only when Entry is > THRESHOLD_SMOOTHED_DEST * Maximum
            /// THRESHOLD_SMOOTHED_DEST is in % Units of Maxium...
            for (Int_t ii = 0; ii < NbinsX_gAliSub; ii++) {
                for (Int_t jj = 0; jj < NbinsY_gAliSub; jj++) {
                    // cout << " h ii jj THRESHOLD_SMOOTHED_DEST dest[ii][jj] "<< h << " " <<  ii << "  " << jj << "  " << THRESHOLD_SMOOTHED_DEST  << "  " << dest[ii][jj] << endl;
                    if (dest[ii][jj]<THRESHOLD_SMOOTHED_DEST*spec_int_maximum) continue;
                    value=dest[ii][jj];
                    spectrum_interim->SetBinContent(ii + 1,jj + 1, value);
                    xax_bin_value=spectrum_interim->GetXaxis()->GetBinCenter(ii + 1);
                    yax_bin_value=spectrum_interim->GetYaxis()->GetBinCenter(jj + 1);
                    zax_bin_value=ZPosGroupedPlates[h];
                    //      cout <<" xax_bin   ii jj (xyz) value  "<<  xax_bin_value  << "  " <<  ii <<  "  " << jj<< "  " <<xax_bin_value  << "  " << yax_bin_value<< "  " << zax_bin_value<<"    " << value << endl;
                    Hist3DimRecoEvent_gAli->Fill(xax_bin_value,yax_bin_value,zax_bin_value, value);
                }
            }
            cout << "THRESHOLD_SMOOTHED_DEST  spectrum_interim->GetMaximum()  : " << THRESHOLD_SMOOTHED_DEST << "  " << spectrum_interim->GetMaximum()<< endl;
            cout << "----------_"<<endl;

            //AllGroupsSimPlusBGittedSpectrum->cd(h+1);
            //spectrum_interim->DrawCopy("colz");
        }
        /// end of loop over the Grouped plates
        ///-------------------------------------------





        return;

        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        //     Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: CT ConeTube Alg  --------------------
                if (!GetConeOrTubeDistanceToInBT(seg, InBT, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTs(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: CT ConeTube Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    ;    // do nothing;
                }
                else {
                    GLOBAL_ShowerSegArray -> Add(seg);
                }
            }
            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli
        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------
void ReconstructShowers_OPENMP() {
    cout << endl << "=======================================================" << endl << endl;
    gEDBDEBUGLEVEL=3;
    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    cout << "GLOBAL_InBTArrayEntries = " << GLOBAL_InBTArrayEntries << endl;
    int tid=0;
    int i=0;
    int nthreads=0;
    int chunk=2;

    #pragma omp parallel shared(nthreads) private(i,tid)
    {
        //   cout << "tid = omp_get_thread_num() " <<  omp_get_thread_num() << endl;
        if (tid == 0)
        {
            //    nthreads = omp_get_num_threads();
            printf("Number of threads = %d\n", nthreads);
        }

        printf("Thread %d starting...\n",tid);

        PrintShowerObjectArray(GLOBAL_ShowerSegArray);

        cout << endl;

        #pragma omp for
        for (Int_t ii=GLOBAL_InBTArrayEntries-1; ii>=0; --ii) {
            EdbSegP* seg = (EdbSegP*) GLOBAL_InBTArray -> At(ii);
            printf("Thread %d: %i execute now ReconstructShowers_OPENMP_TEST \n",tid,ii);
            seg->PrintNice();
            TObjArray LOCAL_ShowerSegArray;
            ReconstructShowers_OPENMP_TEST(seg, LOCAL_ShowerSegArray);
            printf("Thread %d: %i  %d entires in LOCAL_ShowerSegArray: \n",tid,ii,LOCAL_ShowerSegArray.GetEntries() );
        }

    }  /* end of parallel section */


    // endo of OPENMP_session
    return;
}
void ReconstructShowers_OPENMP_TEST(EdbSegP* InBT, TObjArray array) {

    // Dummy Function: Create 4 Segments just by propagating to 2=2,3,4,5*InBT->Z();
    array.Add(InBT);
    for (Int_t j=0; j<3; ++j) {
        cout << "   j = " << j << endl;
        EdbSegP* s=new EdbSegP(j,InBT->X(),InBT->Y(),InBT->TX(),InBT->TY());
        s->SetZ(InBT->Z());
        s->PropagateTo((j+1)*1300+InBT->Z());
        s->SetMC(InBT->MCEvt(),InBT->MCEvt());
        s->PrintNice();
        array.Add(s);
    }
    //PrintShowerObjectArray(array);

    cout << " Since GLOBAL_ShowerSegArray is global we SHOULD expect here parallel conflicts !!!! " << endl;
    return;
}




//-------------------------------------------------------------------------------------------
void ReconstructShowers_CT()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_CT() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of ConeTube Algorithm
    //-----------------------------------------------------------------
    //
    // For each InitiatorBT this is
    // divided in several small parts:
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;
    cout << "GLOBAL_InBTArrayEntries = " << GLOBAL_InBTArrayEntries << endl;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        if (gEDBDEBUGLEVEL==1) {
            int modulo=GLOBAL_InBTArrayEntries/20;
            if ((i%modulo)==0) cout << i <<" : 5% more done"<<endl;
        }
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: CT ConeTube Alg  --------------------
                if (!GetConeOrTubeDistanceToInBT(seg, InBT, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTs(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: CT ConeTube Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    ;    // do nothing;
                }
                else {
                    GLOBAL_ShowerSegArray -> Add(seg);
                }
            }
            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli
        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------






//-------------------------------------------------------------------------------------------
void ReconstructShowers_CA()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_CA() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of ConeAdvanced Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;
    cout << "GLOBAL_InBTArrayEntries = " << GLOBAL_InBTArrayEntries << endl;


    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        if (gEDBDEBUGLEVEL==1) {
            int modulo=GLOBAL_InBTArrayEntries/20;
            if ((i%modulo)==0) cout << i <<" : 5% more done"<<endl;
        }
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: CA ConeAdvanced Alg  --------------------
                if (!GetConeOrTubeDistanceToInBT(seg, InBT, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTs(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: CA ConeAdvanced Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    ;    // do nothing;
                }
                else {
                    GLOBAL_ShowerSegArray -> Add(seg);
                }
            }
            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli
        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);


        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------


/// void ReconstructShowers_CL()  /// Still Missing in the Implementation !!!

//-------------------------------------------------------------------------------------------
void ReconstructShowers_OI()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_OI() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of OFFICIAL IMPLEMENTATION Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        if (gEDBDEBUGLEVEL==1) {
            int modulo=TMath::Max(GLOBAL_InBTArrayEntries/20,1);
            if ((i%modulo)==0) cout << i <<" : 5% more done"<<endl;
        }
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);

        /*
        //-----------------------------------
                // 1) BG Clean of local_gAli
        // that maybe not necessary always , only for the large
        // MC pasted Backgrounds...
                //-----------------------------------
        Float_t BGTargetDensity=0;
        //   cout << "--- \t\t :  -CLEAN  InputData BG Cleaning: 0: No, 1:20BT/mm2  2: 40BT/mm2  3:10BT/mm2 4:60BT/mm2 \n";
        if (cmd_CLEAN==1) BGTargetDensity=20;
        if (cmd_CLEAN==2) BGTargetDensity=40;
        if (cmd_CLEAN==3) BGTargetDensity=10;
        if (cmd_CLEAN==4) BGTargetDensity=60;
        if (cmd_CLEAN==0) BGTargetDensity=1000;

        EdbPVRQuality* localPVRQualCheck = new EdbPVRQuality(local_gAli,BGTargetDensity);
        EdbPVRec* new_local_gAli;
        new_local_gAli = localPVRQualCheck->GetEdbPVRec(1);
        localPVRQualCheck->Print();
        local_gAli=new_local_gAli;
        */

//     InBT->PrintNice();
//     gSystem->Exit(1);

        //-----------------------------------
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: OFFICIAL IMPLEMENTATION Alg  --------------------
                if (!GetConeOrTubeDistanceToInBT(seg, InBT, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTsSingleThetaAngle(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: OFFICIAL IMPLEMENTATION Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    ;    // do nothing;
                }
                else {
                    GLOBAL_ShowerSegArray -> Add(seg);
                }
            }
            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli
        if (gEDBDEBUGLEVEL>2)  PrintShowerObjectArray(GLOBAL_ShowerSegArray);


        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }



        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void ReconstructShowers_SA()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_SA() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of SA Algorithm : only MCEvents with different cuts on MC parameters
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        //local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        local_gAli = TransformEdbPVRec_SA(GLOBAL_gAli, InBT); // Especially for SA-Algorithm to set the size of the local_gAli also.

        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: ReconstructShowers_SA Alg  --------------------
                if (seg->MCEvt()<0) continue;
                if (seg->P() < CUT_PARAMETER[0]) continue;
                // end of    cut conditions: ReconstructShowers_SA Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                if (seg->X()==InBT->X()&&seg->Y()==InBT->Y()) {
                    ;    // do nothing;
                }
                else {
                    GLOBAL_ShowerSegArray -> Add(seg);
                }
            }

            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli
        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);


        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void ReconstructShowers_OI2()
{
    ///  DEPRECATED !!!!
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_OI2() --- DEPRECATED !!!!   RETURN !!!");
    return;
}
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
void ReconstructShowers_TC()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_TC() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of TC "Track(s) (in) Cone" Algorithm
    // Connects tracks to ConteTube Reconstructed Object like in the CA or OI Alg.
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 2a) Loop over (whole) local_gAli, check BT for Cuts (ConeTube)
    // 2b) Loop over (whole) tracks from a linked_tracks file, try to attach these tracks to BT of these within the cone
    // 2c) Loop over (whole) attached basetracks, start ConeTube Reconstruction for these BT itsself.
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    Float_t gAliZMax=TMath::Max(GLOBAL_gAli->GetPattern(GLOBAL_gAli->Npatterns()-1)->Z(),GLOBAL_gAli->GetPattern(0)->Z());

    if (gEDBDEBUGLEVEL>2) {
        cout << "--- --- --- Printing GLOBAL_InBTArray  MaximumZ Position " << gAliZMax << "  --- --- ---"<<endl;
        PrintShowerObjectArray(GLOBAL_InBTArray);
    }

    // Define TObjArray which contains Bastracks Clones from the tracking.
    EdbSegP * segment=0;
    EdbSegP * s2=0;
    EdbTrackP  *t  = 0;
    TFile * fil = new TFile("linked_tracks.root");
    TTree* tr= (TTree*)fil->Get("tracks");
    TClonesArray *segClonesArray= new TClonesArray("EdbSegP",60);
    int nentr = int(tr->GetEntries());
    int nseg,n0,npl;
    cout << "Of linked_tracks we have " << nentr << "  entries Total"<<endl;
    tr->SetBranchAddress("t.", &t  );
    tr->SetBranchAddress("s", &segClonesArray  );
    tr->SetBranchAddress("nseg", &nseg  );
    tr->SetBranchAddress("n0", &n0  );
    tr->SetBranchAddress("npl", &npl  );
    if (gEDBDEBUGLEVEL>2) tr->Show(0);

    cout << "Try Creating Arrays"<< endl;
    TObjArray* LOCAL_TrackBTArray= new TObjArray(nentr);
    TObjArray* LOCAL_NewAddedBTArray= new TObjArray(nentr);
    cout << "Creating Arrays done."<< endl;

    for (int i=0; i<nentr; i++ ) {
        //if (i%10!=0) continue;
        tr->GetEntry(i);

        if (gEDBDEBUGLEVEL>3) cout << i << "    t.eX Y Z " << t->X() << "   "<< t->Y() << "   "<< t->Z() << endl;

        // // Take only Basetracks from tracks which pass 3 or more plates:
        // if (npl<3) continue;


        // Take only tracks which are inside the gAli Volume:
        // Cause Linking is done with all 57 plates, but reconstruction can be done of coure on less:
        s2=(EdbSegP*) segClonesArray->At(nseg-1);
        if (s2->Z()>gAliZMax) {
            cout << " TrackEND is outside of gALi Volume!  Dont take track." << endl;
            continue;
        }

        //       CUTPARAMETER[6]=CUT_TRACKATTACH_NTRACKSEG
        //       CUT_TRACKATTACH_NTRACKSEG ranges from [0..nseg-2]
        //       nseg ranges from [2..nseg-1]

        int takeN=TMath::Min(nseg-1,int(CUT_PARAMETER[6])+1);
        //cout << " takeN =  " << takeN << endl;

        for (int hh=0; hh<takeN; hh++) {
            // Take first BT of the tracks:
            s2=(EdbSegP*) segClonesArray->At(hh);
            if (gEDBDEBUGLEVEL>3) s2->PrintNice();

            // Note: here we take all tracks, regardingless if they are in our desired range. (FP,MP,LP)
            // This we check further down:
            // Clone segment, otherwise it will be overwritten by the next.
            segment=(EdbSegP*)s2->Clone();
            // Add it to LOCAL_TrackBTArray
            LOCAL_TrackBTArray->Add(segment);
        }
        //cout << "for (int hh=0; hh<takeN; hh++) done."<<endl;

    }
    delete tr;
    delete fil;

    if (gEDBDEBUGLEVEL>1) cout << "--- Filled " << LOCAL_TrackBTArray->GetEntries()  << " Segments into LOCAL_TrackBTArray."<<endl;


    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    // This part of the Algorithm is the same as OI Alg part:
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
            cout << " InBT->MCEvt()  "<< InBT->MCEvt()  << "   InBT->P()  " << InBT->P()  << endl;
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        //local_gAli = TransformEdbPVRec_SA(GLOBAL_gAli, InBT); // Especially for SA-Algorithm to set the size of the local_gAli also.

        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2a) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

            for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: TrackCone Alg  --------------------
                // (for first iteration, thi is equivalent to OI Alg Implementation:
                //         if (GetdeltaRWithPropagation(seg,InBT)>100) continue;
                //         if (TMath::Abs(seg->Z()-InBT->Z())>5000) continue;
                if (!GetConeOrTubeDistanceToInBT(seg, InBT, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTsSingleThetaAngle(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: TrackCone IMPLEMENTATION Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                AddBTToArrayWithCeck(seg,  GLOBAL_ShowerSegArray);
            }

        }  // end of loop over all plates of local_gAli
//     if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);


        Float_t NBT_before=0;
        Float_t NBT_after=0;
        Float_t NBTMC_before=0;
        Float_t NBTMC_after=0;
        Float_t pur_before=0;
        Float_t pur_after=0;

        // Quickly calculate NEW NBT and pur------------------------
        EdbSegP* interim=0;
        for (int count=0; count<GLOBAL_ShowerSegArray->GetEntries(); count++) {
            NBT_before++;
            interim=(EdbSegP*)GLOBAL_ShowerSegArray->At(count);
            if (interim->MCEvt()>0) NBTMC_before++;
        }
        pur_before=(Float_t)NBTMC_before/(Float_t)NBT_before;
        // Quickly calculate NEW NBT and pur------------------------



        //-----------------------------------
        // 2b) Loop over (whole) tracks from a linked_tracks file, try to attach these tracks to BT of these within the cone
        // wARNING,wARNING: the linked tracks root file BaseTrack PIDs  and the PIDs of the BTs in the eAli DO NOT
        // NECESSARILY have to be identical, so please, make comparisons rather on Z instead on PID.
        //-----------------------------------


        //cout << "=============================================================================="<<endl;
        //cout << "=" << endl;
        //cout << "=   NOW try to attach tracks to the reconstructed showers:" << endl;

        // Clear LOCAL_NewAddedBTArray to have only same MCEvts in it:
        LOCAL_NewAddedBTArray->Clear();

        int nentr_LOCAL_TrackBTArray=LOCAL_TrackBTArray->GetEntries();
        int nentr_GLOBAL_ShowerSegArray=GLOBAL_ShowerSegArray->GetEntries();

        int  nentr2=nentr_LOCAL_TrackBTArray;

        EdbSegP* tryAttachedSegment=0;
        EdbSegP* alreadyTakenSegment=0;

        for (Int_t icount=0; icount<nentr2; icount++ ) {
            //for(int icount=nentr2/2; icount<(nentr2/2)+2; icount++ ) {

            tryAttachedSegment=(EdbSegP*)LOCAL_TrackBTArray->At(icount);

            // But take only segemnst with same MC Number as InBT of course!!
            if (tryAttachedSegment->MCEvt()>0 && tryAttachedSegment->MCEvt()!=InBT->MCEvt()) continue;

            //cout << "Try to attac now segment (if array) " << icount << " with: " << endl;
            //tryAttachedSegment->PrintNice();
            //cout << "to any of the already attached segments: " << endl;

            for (Int_t j=0; j<GLOBAL_ShowerSegArray->GetEntries(); j++ ) {
                alreadyTakenSegment=(EdbSegP*)GLOBAL_ShowerSegArray->At(j);

                Float_t dRTEST= GetdeltaRWithPropagation(tryAttachedSegment,alreadyTakenSegment);
                Float_t dMinDist=GetMinimumDist(tryAttachedSegment,alreadyTakenSegment);
                Float_t dMinDist_DT=GetdeltaThetaSingleAngles(tryAttachedSegment,alreadyTakenSegment);

                /*
                if (gEDBDEBUGLEVEL>2) {
                cout << "     Test GLOBAL_ShowerSegArray segment (if array) " << j << " with: " << endl;
                alreadyTakenSegment->PrintNice();
                cout << "     to connect: deltaRWith Propagation (tryAttachedSegment,alreadyTakenSegment) : ";
                cout << dRTEST;;
                cout << "     GetMinimumDist(tryAttachedSegment,alreadyTakenSegment) : " << dMinDist << " " << dMinDist_DT << endl;
                }
                */

                // The testing trackBT has to satisfy both criteria:
                // minimum Distance and deltaThetaSingleAngels
                // CUT_PARAMETER[4]=CUT_TRACKATTACH_DISTMIN
                // CUT_PARAMETER[5]=CUT_TRACKATTACH_DTAN_MAX
                if (dMinDist< CUT_PARAMETER[4] && dMinDist_DT< CUT_PARAMETER[5]) {
                    LOCAL_NewAddedBTArray->Add(tryAttachedSegment);
                    //if (gEDBDEBUGLEVEL>2) cout << "Found a close connection. Add tryAttachedSegment to  LOCAL_NewAddedBTArray"<<endl;
                    break;
                }

            } // of (int j=0;)
        } // of (int icount=0; );

        //cout << "LOCAL_NewAddedBTArray->GetEntries()  = " << LOCAL_NewAddedBTArray->GetEntries() << endl;
        //cout << "=" << endl;
        //cout << "=============================================================================="<<endl;


        //-----------------------------------
        // 2c) Loop over (whole) local_gAli, again, but now with ONLY the new LOCAL_NewAddedBTArray as starting BTs
        // 2c) and only if the local_gAli Z position id greater equal than LOCAL_NewAddedBTArray z position:
        // 2c) Do this for every new added BaseTrack:
        // 2c) Rember that not the BaseTracks of the linked_tracks.root file are NOT given into ShowerOject array,
        // 2c) Only the intrinsic ones which come out from the gAli directly.
        // 2c) This is to avoid duplication of tracks, since the may not be totally equal in X,Y,Z
        // 2c) For example if tracking was done with small different par sets...
        //-----------------------------------
        //cout << " // 2c) Loop over (whole) local_gAli, again, but now with ONLY the new LOCAL_NewAddedBTArray as starting BTs"<<endl;
        //cout << " // 2c) LOCAL_NewAddedBTArray->GetEntries()  " << LOCAL_NewAddedBTArray->GetEntries() << endl;
        //PrintShowerObjectArray(LOCAL_NewAddedBTArray);

        for (Int_t icount=0; icount<LOCAL_NewAddedBTArray->GetEntries(); icount++) {

            tryAttachedSegment=(EdbSegP*)LOCAL_NewAddedBTArray->At(icount);
            if (tryAttachedSegment->MCEvt()>0 && tryAttachedSegment->MCEvt()!=InBT->MCEvt()) continue;

            //cout << "// 2c) Loop over (whole) local_gAli, again, but now with ONLY the new LOCAL_NewAddedBTArray as starting BTs"<< endl;
            //cout << "// 2c) and only if the local_gAli Z position id greater equal than LOCAL_NewAddedBTArray z position:"<< endl;
            //cout << " Doing this for  tryAttachedSegment  " << tryAttachedSegment->X() << " " << tryAttachedSegment->Y() << " " << tryAttachedSegment->Z() << " " << tryAttachedSegment->TX() << " " <<  tryAttachedSegment->TY() << " " << tryAttachedSegment->MCEvt() << " " << endl;

            local_gAli_npat=local_gAli->Npatterns();
            // cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

            // Loop over all plates of local_gAli, since this is already
            // extracted with the right numbers of plates...
            for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {

                float_t local_gAliZ=local_gAli->GetPattern(patterloop_cnt)->Z();

                //         cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
                //         cout << "--- --- local_gAli->GetPattern(patterloop_cnt)->Z()= " << local_gAliZ << endl;
                //         cout << "--- --- Is tryAttachedSegment->Z() gtreater equal local_gAliZ()= " << tryAttachedSegment->Z() << " " << local_gAliZ << endl;

                // We do not want to search for local_gAli which has lower Z than the attacet BT itself
                //if (tryAttachedSegment->Z() < local_gAliZ) continue;
                //if (tryAttachedSegment->Z() > local_gAliZ) continue;

                int matches_found=0;
                //cout << "--- --- YES its greater, searching BTs of localgAli now for matches  ?? ? GREATER OR SMALLER ???"<<endl;


                for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
                    seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                    if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                    // Now what was formerly InBT is now the new tryAttachedSegment:
                    // cout << " comparing now Segment "  << seg << "  (Z= " << seg->Z() << ")  with the tryAttachedSegment (Z= " << tryAttachedSegment->Z() << ") GetdeltaRWithPropagation: "<< GetdeltaRWithPropagation(seg,tryAttachedSegment) << endl;

                    // Now apply cut conditions: TrackCone Alg  --------------------
                    if (!GetConeOrTubeDistanceToInBT(seg, tryAttachedSegment, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                    // cout << "Passed GetConeOrTubeDistanceToInBT"<< endl;
                    if (!FindPrecedingBTsSingleThetaAngleTCDEBUG(seg, tryAttachedSegment, local_gAli, GLOBAL_ShowerSegArray)) continue;
                    // cout << "Passed FindPrecedingBTsSingleThetaAngleTCDEBUG"<< endl;
                    // end of    cut conditions: TrackCone IMPLEMENTATION Alg  --------------------

                    // If we arrive here, Basetrack seg has passed criteria
                    // and is then added to the shower array:
                    // Check if its not the InBT which is already added:
                    Bool_t isBTadded=kFALSE;
                    isBTadded = AddBTToArrayWithCeck(seg,GLOBAL_ShowerSegArray);

                    if (gEDBDEBUGLEVEL>2) {
                        if (isBTadded) cout << "...Yes this was a new one, and is added to shower array! ..."<< endl;
                        if (isBTadded) seg->PrintNice();
                    }
                    if (isBTadded) matches_found++;

                } // of  (Int_t btloop_cnt=0; );

                //if (matches_found>0) cout << "--- --- In this plate we have found _ADDITIONAL_ (more than zero) BTs: "<< matches_found << endl;

                // Calculate Efficency numbers only for the last element of the loop,
                // otherwise we add to much numbers...
                if (icount==LOCAL_NewAddedBTArray->GetEntries()-1) {
                    // Calc BT density around shower:
                    EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
                    CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);
                    // Calc TrackNumbers for plate for efficency numbers:
                    CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
                }

            }  // for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt)  // end of loop over all plates of local_gAli

        } // for(int icount=0; icount<OCAL_NewAddedBTArray->GetEntries(); icount++ )
        LOCAL_NewAddedBTArray->Clear();

//      if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);


        // Quickly calculate NEW NBT and pur------------------------
        NBT_after=0;
        NBTMC_after=0;
        for (int count=0; count<GLOBAL_ShowerSegArray->GetEntries(); count++) {
            NBT_after++;
            interim=(EdbSegP*)GLOBAL_ShowerSegArray->At(count);
            if (interim->MCEvt()>0) NBTMC_after++;
        }
        pur_after=(Float_t)NBTMC_after/(Float_t)NBT_after;
        // Quickly calculate NEW NBT and pur------------------------

        if (gEDBDEBUGLEVEL>2) {
            cout << "  Difference between before and after: " << endl;
            cout << "  NBT =  " << NBT_after <<  "   Before: "<< NBT_before << endl;
            cout << "  NBTMC =  " << NBTMC_after <<  "   Before: "<< NBTMC_before << endl;
            cout << "  pur =  " << pur_after <<  "   Before: "<< pur_before << endl;
        }

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------

        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------


    delete LOCAL_TrackBTArray;
    delete LOCAL_NewAddedBTArray;

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void ReconstructShowers_RC()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_RC() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of "Recursive Cone" Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------

        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        Int_t btloop_cnt_N=0;
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
            btloop_cnt_N=local_gAli->GetPattern(patterloop_cnt)->GetN();

            // if pattern is before the Initiator BT->Z() position we just go on:
            if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;

            for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: RECURSIVE CONE Alg  --------------------
                if (!GetConeOrTubeDistanceToBTOfShowerArray(seg, InBT, GLOBAL_ShowerSegArray, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTsSingleThetaAngle(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: RECURSIVE CONE Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                AddBTToArrayWithCeck(seg, GLOBAL_ShowerSegArray);
            }

            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli

        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);

        Int_t f_NBT=0;
        Int_t f_NBTMC=0;
        Int_t f_NBTallMC=0;
        Int_t f_NBTeMC=0;
        Double_t  f_eff=0;
        Double_t f_purall=0;
        Double_t f_pure=0;
        CalcEffPurOfShower(GLOBAL_ShowerSegArray, f_NBT, f_NBTMC, f_NBTallMC, f_NBTeMC, f_purall, f_pure);
        /// last to variables are dummyvariabes.

        //-----------------------------------
        // 3) Try RE-ITERATION over (whole) local_gAli!!
        // 3) Using the iterated shower Gravity Center!
        // 3)
        // 3) WILL NOT WORK FOR ELECTRONS ... CAUSE FISRT POSITION IS SHIFTED AFTER ITERATION !!!
        // 3) DEPRECIAPTED
        // 3) DEPRECIAPTED  .... DO NOT USE IT NOW!!!!!!!!!!!!!!
        // 3) DEPRECIAPTED  (even if it works we dont need to reevaluate the
        // 3)                efficency numbers anymore, since calculated above)
        //-----------------------------------

        Bool_t DoIteration=kFALSE;
        //    Bool_t DoIteration=kTRUE// 3) DEPRECIAPTED  .... DO NOT USE IT NOW!!!!!!!!!!!!!!

        if (DoIteration) {

            cout << "-------------------------------------------"<<endl;
            EdbSegP* ShowerAxisCenterGravityBT = BuildShowerAxis(GLOBAL_ShowerSegArray);
            GLOBAL_ShowerSegArray->Clear();
            //-----------------------------------
            // 3) Loop again over (whole) local_gAli, check BT for Cuts
            //-----------------------------------
            local_gAli_npat=local_gAli->Npatterns();
            btloop_cnt_N=0;
            if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

            // Loop over all plates of local_gAli, since this is already
            // extracted with the right numbers of plates...
            for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
                if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
                btloop_cnt_N=local_gAli->GetPattern(patterloop_cnt)->GetN();

                for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                    seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                    if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                    // Now apply cut conditions: RECURSIVE CONE Alg  --------------------
                    //      if (!GetConeOrTubeDistanceToBTOfShowerArray(seg, ShowerAxisCenterGravityBT, GLOBAL_ShowerSegArray, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                    /// if this is in, it doesnt work for iteratet ShowerAxisCenterGravityBT
                    if (!FindPrecedingBTsSingleThetaAngle(seg, ShowerAxisCenterGravityBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                    // end of    cut conditions: RECURSIVE CONE Alg  --------------------

                    // If we arrive here, Basetrack seg has passed criteria
                    // and is then added to the shower array:
                    // Check if its not the InBT which is already added:
                    //      if (seg->X()==ShowerAxisCenterGravityBT->X()&&seg->Y()==ShowerAxisCenterGravityBT->Y()) { ; } // do nothing;
                    //      else {GLOBAL_ShowerSegArray -> Add(seg);}
                    AddBTToArrayWithCeck(seg, GLOBAL_ShowerSegArray);
                }
            }
            // end of loop over all plates of local_gAli
            if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);
            cout << "-------------------------------------------"<<endl;


        } // of if (DoIteration)

        Int_t s_NBT=0;
        Int_t s_NBTMC=0;
        Int_t s_NBTallMC=0;
        Int_t s_NBTeMC=0;
        Double_t  s_eff=0;
        Double_t s_purall=0;
        Double_t s_pure=0;
        CalcEffPurOfShower(GLOBAL_ShowerSegArray, s_NBT, s_NBTMC, s_NBTallMC, s_NBTeMC, s_purall, s_pure);

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //-----------------------------------
        // 7) cout some numbers from shower reco:
        //-----------------------------------
        if (gEDBDEBUGLEVEL>2) {
            cout << "-------------------------------  _NUMBERS ----------------------"<<endl;
            cout << "ITERATION 0 : --- NBT    = " << f_NBT << endl;
            cout << "ITERATION 0 : --- NBTMC  = " << f_NBTMC << endl;
            cout << "ITERATION 0 : --- purall = " << f_purall << endl;
            cout << "------------"<<endl;
            cout << "ITERATION 1 : --- NBT    = " << s_NBT << endl;
            cout << "ITERATION 1 : --- NBTMC  = " << s_NBTMC << endl;
            cout << "ITERATION 1 : --- purall = " << s_purall << endl;
            cout << "NUMBERS  " << f_NBT << " " <<  f_NBTMC << " " <<  f_purall << " " <<  s_NBT << " " <<  s_NBTMC << " " <<  s_purall << " " << endl;
        }

        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void ReconstructShowers_BW()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_BW() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of "BackWard" Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    //-----------------------------------
    // Algorithm Iteration Steps:
    //
    //  0) Start Adjust Shower Axis, Virtual Vertex
    //  0) Calc dR/dT/dIP(trk,trk) to same plate.
    //  ---Loop up to firstplate;
    //  //  ---Loop for deltaN (plate)=1,2,3:
    // //  //  1) Calc dR/dT/dIP(trk,trk) to backward plate.
    // //  //  1) Check for already in shower array; add BT to shower
    // //  //  1) Adjust Shower Axis, Adjust Virtual Vertex
    //  //  ---Loop for deltaN (plate)=1,2,3:
    //  ---Loop up to firstplate;
    //  2) Adjust Shower Axis, Virtual Vertex.
    //
    //-----------------------------------


    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    EdbSegP* segShower;
    EdbSegP* ShowerAxis;
    EdbVertex* VirtualVertex;
    Float_t local_gAli_pat_interim_halfsize=0;
    Double_t dR,dT,dminDist;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t show_NP=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL!=2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " (MCEvt=" << InBT->MCEvt() << ") now: "<<endl;
            InBT->PrintNice();
            cout << "InBT->P() " << InBT->P() << endl;
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec_BackWard(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        if (gEDBDEBUGLEVEL>2) cout << "--- TransformEdbPVRec done."<<endl;
        //-----------------------------------


        //-----------------------------------
        // 2) Start at plate for Initator BT:
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        Int_t btloop_cnt_N=0;
        //   if (gEDBDEBUGLEVEL>2)
        cout << "--- Starting BACKWARD Reco... ---" << endl;
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        Int_t patterloop_cnt_InBT=-1;
        Float_t patterloop_cnt_InBT_Z=0;
        for (Int_t patterloop_cnt=0; patterloop_cnt<local_gAli_npat; patterloop_cnt++) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
            // if pattern is after the Initiator BT->Z() position we stop:
            if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;
            patterloop_cnt_InBT=patterloop_cnt;
            patterloop_cnt_InBT_Z=local_gAli->GetPattern(patterloop_cnt)->Z();
        }
        if (gEDBDEBUGLEVEL>2) cout << "--- patterloop_cnt_InBT=  " << patterloop_cnt_InBT << endl;
        if (gEDBDEBUGLEVEL>2) cout << "--- patterloop_cnt_InBT_Z=  " << patterloop_cnt_InBT_Z << endl;

        // Check wich Steptype is needed for BackwardStepping:
        // Convention: -1 if Patter(0)_Z<Pattern(npat)_Z
        // Convention: +1 if Patter(0)_Z>Pattern(npat)_Z
        Int_t StepNr=0;
        if (gEDBDEBUGLEVEL>2) cout << "local_gAli->GetPattern(0)->Z()  " <<  local_gAli->GetPattern(0)->Z() << endl;
        if (gEDBDEBUGLEVEL>2) cout << "local_gAli->GetPattern(local_gAli_npat-1)->Z()  " <<  local_gAli->GetPattern(local_gAli_npat-1)->Z() << endl;
        if (local_gAli->GetPattern(0)->Z() > local_gAli->GetPattern(local_gAli_npat-1)->Z()) {
            StepNr=1;
        }
        else {
            StepNr=-1;
        }
        if (gEDBDEBUGLEVEL>2) cout << "--- StepNr for going backwards (decreasing Z) to next plate=  " << StepNr << endl;

        // return;
        Bool_t FP_reached=kFALSE;
        Int_t patterloop_cnt=patterloop_cnt_InBT;
        if (StepNr==1)  patterloop_cnt=patterloop_cnt_InBT;
        if (StepNr==-1)  cout << "DONT KNOW HERE WHICH    patterloop_cnt=local_gAli_npat-1;   to calculate!!! " << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        while (!FP_reached) {
            if (gEDBDEBUGLEVEL>2) cout << " FP NOT YET REACHED .... << doing patterloop_cnt = " << patterloop_cnt << endl;
            btloop_cnt_N=local_gAli->GetPattern(patterloop_cnt)->GetN();
            if (gEDBDEBUGLEVEL>2) cout << " FP NOT YET REACHED .... << with  btloop_cnt_N = " << btloop_cnt_N << endl;

            // Loop over all BTs in the actual plate. Calculate dR/dT/dIP to
            // all BTs which are already in the shower (at beginning it is
            // only the InBT.
            for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                Bool_t add_seg=kFALSE;
                if (gEDBDEBUGLEVEL>3) cout << "btloop_cnt (ot of ) " << btloop_cnt << "  ( "  << btloop_cnt_N << " ) " << endl;

                // Now apply cut conditions: BACKWARD Alg  --------------------
                // Check for dTheta to tracks already in shower:
                Int_t actualEntries=GLOBAL_ShowerSegArray->GetEntries();
                if (gEDBDEBUGLEVEL>3) cout << "actualEntries of  GLOBAL_ShowerSegArray:  " << actualEntries << endl;
                for (int i=0; i<actualEntries; ++i) {
                    segShower=(EdbSegP*)GLOBAL_ShowerSegArray->At(i);
                    Float_t dZ=TMath::Abs(segShower->Z()-seg->Z());
                    dT=GetdeltaThetaSingleAngles(seg,segShower); // DO NOT TAKE GetdeltaTheta since this is calculation based on absolute theta differences (not on TX,TY relative ones) !!
                    dR=GetdR(seg,segShower);
                    dminDist=GetMinimumDist(seg,segShower);
                    if (gEDBDEBUGLEVEL>3) cout << "btloop_cnt i  dT  dR  dminDist dZ: " << btloop_cnt << "  " << i << "  " << dT << "  " << dR << "  " << dminDist << "  " << dZ << endl;
                    if (TMath::Abs(dZ)>CUT_PARAMETER[3]) continue;
                    if (TMath::Abs(dT)>CUT_PARAMETER[1]) continue;
                    if (TMath::Abs(dR)>CUT_PARAMETER[2]) continue;
                    if (TMath::Abs(dminDist)>CUT_PARAMETER[0]) continue;
                    if (gEDBDEBUGLEVEL>3) cout << "try to add this BT (if not already in there...) "<< endl;
                    add_seg=kTRUE;
                    break;
                } // of for (int i=0; i<actualEntries; ++i)
                if (add_seg) {
                    AddBTToArrayWithCeck(seg,GLOBAL_ShowerSegArray);
                    EdbSegP* BT_1=(EdbSegP*)GLOBAL_ShowerSegArray->At(0);
                    EdbSegP* BT_2=(EdbSegP*)GLOBAL_ShowerSegArray->At(GLOBAL_ShowerSegArray->GetEntries()-1);
                    show_NP=TMath::Max(show_NP,TMath::Abs(BT_1->PID()-BT_2->PID())+1);
                    //cout << "show_NP = " << show_NP << endl;
                }
            } // of for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt)

            // Now goto next plate:
            patterloop_cnt=patterloop_cnt+StepNr;
            if (patterloop_cnt<0) FP_reached=kTRUE;
            if (patterloop_cnt>=local_gAli_npat) FP_reached=kTRUE;
            // Or we stop also if the number of plates is more than cmd_NP
            if (show_NP>=cmd_NP) FP_reached=kTRUE;
        } // while (!FP_reached)


        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  purall, pure;
            CalcEffPurOfShower(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure);
            GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[i];
            GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[i];
            GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[i];
            GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[i];
            GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[i];
        }

        if (gEDBDEBUGLEVEL>2) cout << " Check if its sorted: GLOBAL_ShowerSegArray->At(0)->Z() should be the lowest one...continue;" << endl;
        if (gEDBDEBUGLEVEL>2) cout << " IsShowerSortedZ(GLOBAL_ShowerSegArray)  " << IsShowerSortedZ(GLOBAL_ShowerSegArray) << endl;
        SortShowerZ(GLOBAL_ShowerSegArray);

        // After Sort Shower we can build axis right now...
        ShowerAxis=BuildShowerAxis(GLOBAL_ShowerSegArray);
        ShowerAxis->PrintNice();
        Float_t mindTTT=999999;
        Int_t mindTTT_h=0;
        for (int h=0; h<GLOBAL_ShowerSegArray->GetEntries(); ++h) {
            segShower=(EdbSegP*)GLOBAL_ShowerSegArray->At(h);
            //   segShower->PrintNice();
            if (segShower->Z()!=ShowerAxis->Z()) continue;
            Float_t dTTT = GetdeltaTheta(ShowerAxis,segShower); //cout << "dTTT  = " << dTTT << endl;
            if (dTTT<mindTTT) {
                mindTTT=dTTT;
                mindTTT_h=h;
            }
        }

        // Now here comes the forward (==downstream==inbeamdirection) reconstruction
        if (gEDBDEBUGLEVEL>2) cout << "--- Starting FORWARD Reco... ---" << endl;

        // Just take the BT which is closest to shower axis:
        ShowerAxis=(EdbSegP*)GLOBAL_ShowerSegArray->At(mindTTT_h);
        GLOBAL_ShowerSegArray->Clear();
        // The First we have to add by hand because in the reco routine there has to be at least one BT to check.
        GLOBAL_ShowerSegArray->Add(ShowerAxis);
        if (gEDBDEBUGLEVEL>3) ShowerAxis->PrintNice();

        for (Int_t patterloop_cnt=0; patterloop_cnt<local_gAli_npat; patterloop_cnt++) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
            // if pattern is after the ShowerAxis BT->Z() position we stop:
            if (local_gAli->GetPattern(patterloop_cnt)->Z()<ShowerAxis->Z()) continue;
            patterloop_cnt_InBT=patterloop_cnt;
            patterloop_cnt_InBT_Z=local_gAli->GetPattern(patterloop_cnt)->Z();
        }
        if (gEDBDEBUGLEVEL>2) cout << "--- patterloop_cnt_InBT=  " << patterloop_cnt_InBT << endl;
        if (gEDBDEBUGLEVEL>2) cout << "--- patterloop_cnt_InBT_Z=  " << patterloop_cnt_InBT_Z << endl;

        // InvertSpeNr now for forward Step:
        Int_t StepNrForward=StepNr*-1;
        StepNr=StepNrForward;
        show_NP=0;

        Bool_t LP_reached=kFALSE;
        patterloop_cnt=patterloop_cnt_InBT;

        while (!LP_reached) {
            if (gEDBDEBUGLEVEL>2) cout << " LP NOT YET REACHED .... << doing patterloop_cnt = " << patterloop_cnt << endl;
            btloop_cnt_N=local_gAli->GetPattern(patterloop_cnt)->GetN();
            if (gEDBDEBUGLEVEL>2) cout << " LP NOT YET REACHED .... << with  btloop_cnt_N = " << btloop_cnt_N << endl;

            // Loop over all BTs in the actual plate. Calculate dR/dT/dIP to
            // all BTs which are already in the shower (at beginning it is
            // only the InBT.
            for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();
                Bool_t add_seg=kFALSE;
                if (gEDBDEBUGLEVEL>3) cout << "btloop_cnt (ot of ) " << btloop_cnt << "  ( "  << btloop_cnt_N << " ) " << endl;

                // Now apply cut conditions: BACKWARD Alg  --------------------
                // Check for dTheta to tracks already in shower:
                Int_t actualEntries=GLOBAL_ShowerSegArray->GetEntries();
                if (gEDBDEBUGLEVEL>3) cout << "actualEntries of  GLOBAL_ShowerSegArray:  " << actualEntries << endl;
                for (int i=0; i<actualEntries; ++i) {
                    segShower=(EdbSegP*)GLOBAL_ShowerSegArray->At(i);
                    Float_t dZ=TMath::Abs(segShower->Z()-seg->Z());
                    dT=GetdeltaThetaSingleAngles(seg,segShower); // DO NOT TAKE GetdeltaTheta since this is calculation based on absolute theta differences (not on TX,TY relative ones) !!
                    dR=GetdR(seg,segShower);
                    dminDist=GetMinimumDist(seg,segShower);
                    if (gEDBDEBUGLEVEL>3) cout << "btloop_cnt i  dT  dR  dminDist dZ: " << btloop_cnt << "  " << i << "  " << dT << "  " << dR << "  " << dminDist << "  " << dZ << endl;
                    if (TMath::Abs(dZ)>CUT_PARAMETER[7]) continue;
                    if (TMath::Abs(dT)>CUT_PARAMETER[5]) continue;
                    if (TMath::Abs(dR)>CUT_PARAMETER[6]) continue;
                    if (TMath::Abs(dminDist)>CUT_PARAMETER[4]) continue;
                    if (gEDBDEBUGLEVEL>3) cout << "try to add this BT (if not already in there...) "<< endl;
                    add_seg=kTRUE;
                    break;
                } // of for (int i=0; i<actualEntries; ++i)
                if (add_seg) {
                    AddBTToArrayWithCeck(seg,GLOBAL_ShowerSegArray);
                    EdbSegP* BT_1=(EdbSegP*)GLOBAL_ShowerSegArray->At(0);
                    EdbSegP* BT_2=(EdbSegP*)GLOBAL_ShowerSegArray->At(GLOBAL_ShowerSegArray->GetEntries()-1);
                    show_NP=TMath::Max(show_NP,TMath::Abs(BT_1->PID()-BT_2->PID())+1);
                    //cout << "show_NP = " << show_NP << endl;
                }
            } // of for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt)


            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Now goto next plate:
            patterloop_cnt=patterloop_cnt+StepNr;
            if (patterloop_cnt<0) LP_reached=kTRUE;
            if (patterloop_cnt>=local_gAli_npat) LP_reached=kTRUE;
            // Or we stop also if the number of plates is more than cmd_NP
            if (show_NP>=cmd_NP) LP_reached=kTRUE;

        } // of while (!LP_reached)
        ///   return;  --------------------------------------------------------------------------

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }



        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }

        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
void ReconstructShowers_AG()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_AG() ---");

    //-----------------------------------------------------------------
    // Main function for reconstruction of "Advanced Gamma" Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, check BT for Cuts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------

        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);
        // Add InBT to GLOBAL_ShowerSegArray
        GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check InitiatorBT
        // 2) compatible with a segment forming a e+e- pair:
        //-----------------------------------
        Int_t NPairs=0,PIDDIFF=0,minPIDDIFF=0;

        Int_t local_gAli_npat=local_gAli->Npatterns();
        Int_t btloop_cnt_N=0;
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
            btloop_cnt_N=local_gAli->GetPattern(patterloop_cnt)->GetN();
            if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;
            if (TMath::Abs(local_gAli->GetPattern(patterloop_cnt)->Z()-InBT->Z())>3000) continue;
            if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;
            for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();
                if (IsSameSegment(seg,InBT)) continue;
                PIDDIFF=TMath::Abs(seg->PID()-InBT->PID());
                if (GetMinimumDist(InBT,seg)>50) continue;
                if (GetdR(InBT,seg)>200) continue;
                if (GetdeltaThetaSingleAngles(InBT,seg)>0.5) continue;
                if (PIDDIFF>minPIDDIFF) continue;
                //     cout << " GetMinimumDist(InBT,seg); " << GetMinimumDist(InBT,seg) << "PIDDIFF "<< PIDDIFF<< endl;

                AddBTToArrayWithCeck(seg, GLOBAL_ShowerSegArray);
                minPIDDIFF=PIDDIFF;
            }
        }
        PrintShowerObjectArray(GLOBAL_ShowerSegArray);
        //   return;

        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check BT for Cuts
        //-----------------------------------
        if (gEDBDEBUGLEVEL>2) cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

        // Loop over all plates of local_gAli, since this is already
        // extracted with the right numbers of plates...
        for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
            if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;
            btloop_cnt_N=local_gAli->GetPattern(patterloop_cnt)->GetN();

            // if pattern is before the Initiator BT->Z() position we just go on:
            if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;

            for (Int_t btloop_cnt=0; btloop_cnt<btloop_cnt_N; ++btloop_cnt) {
                seg = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
                if (gEDBDEBUGLEVEL>3) seg->PrintNice();

                // Now apply cut conditions: Advanced Gamma Alg  --------------------
                if (!GetConeOrTubeDistanceToBTOfShowerArray(seg, InBT, GLOBAL_ShowerSegArray, CUT_PARAMETER[0], CUT_PARAMETER[1])) continue;
                if (!FindPrecedingBTsSingleThetaAngle(seg, InBT, local_gAli, GLOBAL_ShowerSegArray)) continue;
                // end of    cut conditions: Advanced Gamma Alg  --------------------

                // If we arrive here, Basetrack seg has passed criteria
                // and is then added to the shower array:
                // Check if its not the InBT which is already added:
                //     if (IsSameSegment(seg,InBT)) { ; } // do nothing;
                //     else {GLOBAL_ShowerSegArray -> Add(seg);}
                AddBTToArrayWithCeck(seg, GLOBAL_ShowerSegArray);
            }


            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(patterloop_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);
        }
        // end of loop over all plates of local_gAli

        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);




        Int_t s_NBT=0;
        Int_t s_NBTMC=0;
        Int_t s_NBTallMC=0;
        Int_t s_NBTeMC=0;
        Double_t  s_eff=0;
        Double_t s_purall=0;
        Double_t s_pure=0;
        CalcEffPurOfShower(GLOBAL_ShowerSegArray, s_NBT, s_NBTMC, s_NBTallMC, s_NBTeMC, s_purall, s_pure);

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }


        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>3) cout << "--- ---GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;


    return;
}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
void ReconstructShowers_GS()
{
    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_GS() ---");
    //-----------------------------------------------------------------
    // Main function for reconstruction of "Gamma Search" Algorithm
    //-----------------------------------------------------------------

    //-----------------------------------
    // For each InitiatorBT this is
    // divided in several small parts:
    //
    // 1) Make local_gAli with cut parameters, Make GetPID of InBT and corresponding of plates
    // 3) Loop over (whole) local_gAli, search and check BT pairs for suts
    // 4) Calculate pur/eff/NBT numbers
    // 5) Fill Trees
    //-----------------------------------

    // Define Helper Variables:
    EdbPVRec* local_gAli;
    EdbSegP* InBT;
    EdbSegP* seg;
    Float_t local_gAli_pat_interim_halfsize=0;

    GLOBAL_InBTArrayEntries=GLOBAL_InBTArray->GetEntries();
    GLOBAL_INBTSHOWERNR=0;

    Bool_t IsFirstLoopCount=kTRUE;
    Int_t LastGlobalMCEventNr=1;

    EdbVertex* vtx=new EdbVertex();

    //-----------------------------------------------------------------
    // Since GLOBAL_InBTArray is filled in ascending ordering by zpositon
    // We use the descending loop to begin with BT with lowest z first.
    for (Int_t i=GLOBAL_InBTArrayEntries-1; i>=0; --i) {

        //-----------------------------------
        // CounterOutPut
        if (gEDBDEBUGLEVEL==2) if ((i%1)==0) cout << GLOBAL_InBTArrayEntries <<" InBT in total, still to do:"<<Form("%4d",i)<< "\r\r\r\r"<<flush;
        //-----------------------------------

        //-----------------------------------
        // Get InitiatorBT from GLOBAL_InBTArray
        InBT=(EdbSegP*)GLOBAL_InBTArray->At(i);
        //--------
        GLOBAL_InBT_E=InBT->P();
        GLOBAL_InBT_TanTheta=TMath::Sqrt(InBT->TX()*InBT->TX()+InBT->TY()*InBT->TY());
        GLOBAL_InBT_Flag=InBT->Flag();
        GLOBAL_InBT_MC=InBT->MCEvt();
        //--------
        Int_t local_NBT=0;
        Int_t local_NBTMC=0;
        Int_t local_NBTallMC=0;
        Int_t local_NBTeMC=0;
        float_t local_pure=-1;
        float_t local_purall=-1;
        Int_t npat_int=0;
        Int_t npat_total=0;
        Int_t npatN=0;
        Int_t npat_Neff=0;
        Int_t NBT_Neff=0;
        Int_t NBTMC_Neff=0;
        Int_t NBTMCe_Neff=0;
        //--------

        if (gEDBDEBUGLEVEL>2) {
            cout << endl << endl << "--- Starting Shower for Number " << i << " now: "<<endl;
            InBT->PrintNice();
        }
        //-----------------------------------



        //-----------------------------------
        // 0) Set vtx according to MC event,
        //    or to a half plate backward propagated segment.
        //    Do this only if the value -GBMC is NOT set,
        //    (in the case -GBMC is set, we should know the vertex anyway).
        //-----------------------------------
        //
        if (GLOBAL_InBT_MC>LastGlobalMCEventNr) LastGlobalMCEventNr=GLOBAL_InBT_MC;
        //
        //
        if (GLOBAL_InBT_MC<0&& cmd_GBMC<0) {
            // This is an "equalization" effect. If we dont put this in, we will have BG base
            // tracks always extrapolated to themselves, but SIM basetracks extrapolated to
            // the vertex. This leads to a wrong deltaZ distribution which spoils the
            // ANN Training.
            if( IsFirstLoopCount==kTRUE) cout << "Option: GLOBAL_InBT_MC<0&& cmd_GBMC<0"<<endl;
            vtx->SetXYZ(GLOBAL_VtxArrayX[LastGlobalMCEventNr],GLOBAL_VtxArrayY[LastGlobalMCEventNr],GLOBAL_VtxArrayZ[LastGlobalMCEventNr]);
        }
        //
        else if (GLOBAL_InBT_MC<0 && cmd_GBMC==0) {
            if( IsFirstLoopCount==kTRUE) cout << "Option: GLOBAL_InBT_MC<0 && cmd_GBMC==0"<<endl;
            vtx->SetXYZ(InBT->X()-650*InBT->TX(),InBT->Y()-650*InBT->TY(),InBT->Z()-650);
        }
        else if (GLOBAL_InBT_MC<0 && cmd_GBMC>0) {
            if( IsFirstLoopCount==kTRUE)  cout << "Option: GLOBAL_InBT_MC<0 && cmd_GBMC>0"<<endl;
            vtx->SetXYZ(GLOBAL_VtxArrayX[cmd_GBMC],GLOBAL_VtxArrayY[cmd_GBMC],GLOBAL_VtxArrayZ[cmd_GBMC]);
        }
        else if (GLOBAL_InBT_MC>0 && GLOBAL_IsBrickTreePGunInfo==kFALSE) {
            if( IsFirstLoopCount==kTRUE) cout << "Option: GLOBAL_InBT_MC>0 && GLOBAL_IsBrickTreePGunInfo==kFALSE"<<endl;
            vtx->SetXYZ(InBT->X()-650*InBT->TX(),InBT->Y()-650*InBT->TY(),InBT->Z()-650);
        }
        else {
            if( IsFirstLoopCount==kTRUE) cout << "Option: else"<<endl;
            vtx->SetXYZ(GLOBAL_VtxArrayX[GLOBAL_InBT_MC],GLOBAL_VtxArrayY[GLOBAL_InBT_MC],GLOBAL_VtxArrayZ[GLOBAL_InBT_MC]);
        }
        vtx->SetMC(GLOBAL_InBT_MC);

        if (gEDBDEBUGLEVEL>2) {
            cout << "The vtx info for this MC event: X:Y:Z:MC:  " << vtx->X()  << " " << vtx->Y() << " " << vtx->Z() << " " << GLOBAL_InBT_MC << endl;
            cout << "The vtx info for this MC event: IP(INBT,vtx):  " << CalcIP(InBT,vtx)<< endl;
        }
        //-----------------------------------



        //-----------------------------------
        // 1) Make local_gAli with cut parameters:
        //-----------------------------------
        local_gAli = TransformEdbPVRec(GLOBAL_gAli, InBT);

        // IN THIS ALGORITHM WE DO NOT YET AUTOMATICALLY ADD THE FIRST BT
        // SINCE WE NEED AN ADDIITONAL VERTEX CUT OF THIS FIRST BT TO CHECK!
        // // Add InBT to GLOBAL_ShowerSegArray
        // GLOBAL_ShowerSegArray -> Add(InBT);
        //-----------------------------------


        //-----------------------------------
        // 2) Loop over (whole) local_gAli, check InitiatorBT
        // 2) compatible with a segment forming a e+e- pair:
        // 2) (Loop over all plates of local_gAli, since this is already
        // 2) extracted with the right numbers of plates...)
        //-----------------------------------
        Int_t local_gAli_npat=local_gAli->Npatterns();
        Int_t btloop_cnt_N=0;

        ///============================================================================================
        ///========================  CODE FROM EdbShowAlg_GS  FROM libShowRec =========================
        ///============================================================================================



        Int_t npat=local_gAli->Npatterns();
        Int_t pat_one_bt_cnt_max,pat_two_bt_cnt_max=0;
        EdbPattern* pat_one=0;
        EdbPattern* pat_two=0;
        EdbSegP* Segment=0;
        EdbSegP* Segment2=0;
        Float_t distZ,IP_Pair_To_InBT,IP_Pair_To_InBT_SegSum;
        Float_t IP_Pair_To_InBT_Seg2;


        Segment = InBT;
        IP_Pair_To_InBT=CalcIP(Segment, vtx);
        /// Change with respect to libShowRec: here we assume that Segment will always be
        /// the Initiator BaseTrack and Segment2 is the other segment to check.







        // Loop over pattern for the first BT of the pairs:
        // Start first with the pattern with the lowest Z position.
        pat_one=local_gAli->GetPatternZLowestHighest(1);
        Float_t pat_one_Z=pat_one->Z();
        pat_one_bt_cnt_max=pat_one->GetN();

        for (Int_t pat_one_cnt=0; pat_one_cnt<npat; ++pat_one_cnt) {

            if (pat_one_cnt>0) {
                pat_one=(EdbPattern*)local_gAli->NextPattern(pat_one_Z,1);
                pat_one_Z=pat_one->Z();
                pat_one_bt_cnt_max=pat_one->GetN();
            }

            // Check if pattern Z() equals the InBZ->Z(), since we wanna have the
            // pattern one the pattern to contain the InBT:
            distZ=pat_one->Z()-InBT->Z();
            if (TMath::Abs(distZ)>10) continue;
            //cout << "distZ (pat_one->Z()-InBT->Z())= " << distZ << endl;

            // Check if InBT fulfills criteria for IP to vertex:
            if (IP_Pair_To_InBT>CUT_PARAMETER[0]) continue;
            // Check if InBT fulfills criteria for z-diff to vertex:
            if ((InBT->Z()-vtx->Z())>CUT_PARAMETER[3]) continue;

            // Now here we can add InBT since it passed also the vertex cut.
            // Therefore, the Reconstructed Shower has always InBT as first BT stored.
            if (GLOBAL_ShowerSegArray->GetEntries()==0) {
                GLOBAL_ShowerSegArray->Add(InBT);
                if (gEDBDEBUGLEVEL>2) {
                    cout << "I have added the first InBT  " << InBT << "  to GLOBAL_ShowerSegArray." << endl;
                    InBT->PrintNice();
                }
            }

            // Check if pattern dist Z to Vtx  is ok:
            distZ=pat_one->Z()-vtx->Z();
            // Z distance has to be greater zero, cause the InBT
            // and other pair BTs shall come downstream the vertex:
            if (distZ<0) continue;
            //cout << "distZ (pat_one->Z()-vtx->Z();) = " << distZ << endl;

            if (gEDBDEBUGLEVEL>2) cout << "Searching patterns: pat_one_cnt=" << pat_one_cnt << "  pat_one->Z() = " << pat_one->Z() << " pat_one_bt_cnt_max= "<< pat_one_bt_cnt_max <<endl;


            // Loop over pattern for the second BT of the pairs:
            //
            //cout << "// Loop over pattern for the second BT of the pairs: "<< endl;

            pat_two=local_gAli->GetPatternZLowestHighest(1);
            Float_t pat_two_Z=pat_two->Z();
            pat_two_bt_cnt_max=pat_two->GetN();

            for (Int_t pat_two_cnt=0; pat_two_cnt<npat; ++pat_two_cnt) {

                if (pat_two_cnt>0) {
                    pat_two=(EdbPattern*)local_gAli->NextPattern(pat_two_Z,1);
                    pat_two_Z=pat_two->Z();
                    pat_two_bt_cnt_max=pat_two->GetN();
                }

                // PID diff of two plates may be maximum [0..PidDIFFN]
                if (TMath::Abs(pat_one_cnt-pat_two_cnt)>CUT_PARAMETER[5]) continue;

                // pattern two should come downstream pattern one:
                if (pat_two->Z()<pat_one->Z()) continue;


                if (gEDBDEBUGLEVEL>2) cout << " Searching patterns: pat_two_cnt=" << pat_two_cnt << "  pat_two->Z() = " << pat_two->Z() << " pat_two_bt_cnt_max= "<< pat_two_bt_cnt_max <<endl;


                for (Int_t pat_one_bt_cnt=0; pat_one_bt_cnt<pat_one_bt_cnt_max; ++pat_one_bt_cnt) {
                    /// Segment =  (EdbSegP*)pat_one->GetSegment(pat_one_bt_cnt);
                    /// Segment = InBT;
                    /// Change with respect to libShowRec: here we assume that Segment will always be
                    /// the Initiator BaseTrack.

                    for (Int_t pat_two_bt_cnt=0; pat_two_bt_cnt<pat_two_bt_cnt_max; ++pat_two_bt_cnt) {
                        Segment2 = (EdbSegP*)pat_two->GetSegment(pat_two_bt_cnt);

                        // Ceck if segments are not (by chance) the same:
                        if (Segment2==Segment) continue;
                        if (Segment2->ID()==Segment->ID()&&Segment2->PID()==Segment->PID()) continue;
                        if (IsSameSegment(Segment2,Segment)) continue;



                        /// At first:  Check for already duplicated pairings:
                        /// if (CheckPairDuplications(Segment->PID(),Segment->ID(),Segment2->PID(),Segment2->ID(), SegmentPIDArray,SegmentIDArray,Segment2PIDArray,Segment2IDArray, RecoShowerArrayN)) continue;

                        // Now apply cut conditions: GS  GAMMA SEARCH Alg  --------------------

                        // Check if IP of both to vtx (BT) is ok:
                        IP_Pair_To_InBT_Seg2  =CalcIP(Segment2, vtx);
                        if (IP_Pair_To_InBT_Seg2>CUT_PARAMETER[0]) continue;

                        // if InBT is flagged as MC InBT, take care that only BG or same MC basetracks are taken:
                        if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment->MCEvt()!=Segment2->MCEvt()) continue;
                        if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment->MCEvt()!=InBT->MCEvt()) continue;
                        if (InBT->MCEvt()>0) if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) if (Segment2->MCEvt()!=InBT->MCEvt()) continue;

                        // In case of two MC events, check for e+ e- pairs
                        // Do this ONLY IF parameter eParaValue[6] is set to choose different Flag() pairs:
                        if (InBT->MCEvt()>0 && CUT_PARAMETER[6]==1) {
                            if (Segment->MCEvt()>0&&Segment2->MCEvt()>0) {
                                if ((Segment2->Flag()+Segment->Flag())!=0) continue;
                            }
                        }

                        // a) Check dR between tracks:
                        if (GetdeltaRWithPropagation(Segment,Segment2)>CUT_PARAMETER[2]) continue;
                        // b) Check dT between tracks:
                        if (GetdeltaThetaSingleAngles(Segment,Segment2)>CUT_PARAMETER[4]) continue;
                        // c) Check dMinDist between tracks:
                        if (GetMinimumDist(Segment,Segment2)>CUT_PARAMETER[1]) continue;

                        // f) Check if this is not a possible fake doublet which is
                        //   sometimes caused by view overlap in the scanning:
                        //    in the EdbPVRQuality class this will be done at start for the whole
                        //    PVR object so this will be later on obsolete.
                        ///if (IsPossibleFakeDoublet(Segment,Segment2) ) continue;
                        //
                        // end of    cut conditions: GS  GAMMA SEARCH Alg  --------------------
                        //


                        if (gEDBDEBUGLEVEL>3) {
                            cout << "EdbShowAlg_GS::FindPairs Pair (PID:" << Segment->PID() << ",ID:" << Segment->ID()<< ";"<< Segment2->PID() << "," << Segment2->ID() << ") has passed all cuts w.r.t to InBT:" << endl;
                            cout << "EdbShowAlg_GS::FindPairs GetdeltaRWithPropagation(Segment,Segment2)  = " << GetdeltaRWithPropagation(Segment,Segment2) << endl;
                            cout << "EdbShowAlg_GS::FindPairs GetdeltaThetaSingleAngles(Segment,Segment2)  = " << GetdeltaThetaSingleAngles(Segment,Segment2) << endl;
                            cout << "EdbShowAlg_GS::FindPairs GetMinimumDist(Segment,Segment2)  = " << GetMinimumDist(Segment,Segment2) << endl;
                            cout << "EdbShowAlg_GS::FindPairs CalcIP(BetterSegment,InBT)  = " << IP_Pair_To_InBT << endl;
                        }

                        if (gEDBDEBUGLEVEL>3)  cout <<"------------"<< endl;

                        // And Add Segment2 to to shower array:
//                         cout << "// Add Segment2 to to shower array (AddBTToArrayWithCeck):" << endl;
                        Bool_t isContained = AddBTToArrayWithCeck(Segment2, GLOBAL_ShowerSegArray);
                        //PrintShowerObjectArray(GLOBAL_ShowerSegArray);
//             cout << " isContained == " << isContained << endl;
                        if (isContained==kTRUE) continue;

                    } //for (Int_t pat_two_bt_cnt=0; ...

                } //for (Int_t pat_one_bt_cnt=0; ...

            } // for (Int_t pat_two_cnt=0; ...


            // Now here do the usual rest for BG density calculation:
            //...
            // Calc BT density around shower:
            EdbPattern* pat_interim=local_gAli->GetPattern(pat_one_cnt);
            CalcTrackDensity(pat_interim,local_gAli_pat_interim_halfsize,npat_int,npat_total,npatN);

            // Calc TrackNumbers for plate for efficency numbers:
            CalcEfficencyNumbers(pat_interim, InBT->MCEvt(), NBT_Neff, NBTMC_Neff,NBTMCe_Neff);

        } //for (Int_t pat_one_cnt=0; ...


        ///============================================================================================
        ///============================================================================================
        ///============================================================================================

        if (gEDBDEBUGLEVEL>2) cout << " GLOBAL_ShowerSegArray->GetEntries() " << GLOBAL_ShowerSegArray->GetEntries() << endl;

        ///----------------------
        /// ONLY FOR THE ANN TRAINING FOR GS ALGO:
        /// DO THIS ONLY IF YOU HAVE PAIRS !!!

        if (GLOBAL_ShowerSegArray->GetEntries()>1) {

            Segment=(EdbSegP*)GLOBAL_ShowerSegArray->At(0);
            Segment2=(EdbSegP*)GLOBAL_ShowerSegArray->At(1);



            if (gEDBDEBUGLEVEL>2) {
                cout << "ONLY FOR THE ANN TRAINING FOR GS ALGO: "  << endl;
                cout << "DO THIS ONLY IF YOU HAVE PAIRS !!!: "  << endl;
                cout << "YES WE HAVE A PAIR Print all segs in the array: "  << endl;
                for (int l=0; l<GLOBAL_ShowerSegArray->GetEntries(); ++l) {
                    EdbSegP* s=(EdbSegP*)GLOBAL_ShowerSegArray->At(l);
                    s->PrintNice();
                }
//      Segment->PrintNice();
//      Segment2->PrintNice();
                cout << "Print again the InBT for crosscheck: InBT= " << endl;
                InBT->PrintNice();
                cout << "Address of Segment   = " << Segment << endl;
                cout << "Address of Segment2  = " << Segment2 << endl;
            }



            // Implicitly, we assume that InBT= Seg->At(0), which I checked is
            // correct. Also in the code it is done this way that InBT is added
            // as first BT.
            //Segment->PrintNice();
            //Segment2->PrintNice();
            //InBT->PrintNice();

            IP_Pair_To_InBT_Seg2=CalcIP(Segment2,vtx);
            IP_Pair_To_InBT=CalcIP(Segment,vtx);

            // Check if both basetracks have a vertex which is upstream
            // of both tracks (only then the two BT are really pointing).
            TObjArray *segments = new TObjArray(2);
            segments->Clear();
            segments->Add(Segment);
            segments->Add(Segment2);
            EdbVertex* vetex = new EdbVertex();
            vetex = CalcVertex(segments);
            cout << "Calculated helper _vetex_ out of the first two segments. " << endl;
            cout <<"vetex ->X,Y,Z:  " << vetex ->X() << " " << vetex ->Y() << " " << vetex ->Z()<< endl;
            if (vetex ->Z()> TMath::Min(Segment->Z(),Segment2->Z()) ) cout << "The interims vertex to which the two segments points lies downstream, or in between the to segmnets, i.e. the do NOT point to an originating vertex. continue now...." << endl;
            cout << "Check:  TMath::Min(Segment->Z(),Segment2->Z())  " <<  TMath::Min(Segment->Z(),Segment2->Z())  << endl;
            cout <<"Segment->Z(): " << Segment->Z() << "  Segment2->Z() " << Segment2->Z() << endl;
            cout << "Address of Segment   = " << Segment << endl;
            cout << "Address of Segment2  = " << Segment2 << endl;
            if (vetex ->Z()> TMath::Min(Segment->Z(),Segment2->Z()) ) {
                cout << " Hmm, do nothing..." << endl;
            }
            else {

                Float_t IP_Seg1ToVtxSeg1Seg2=0;
                Float_t IP_Seg2ToVtxSeg1Seg2=0;
                IP_Seg1ToVtxSeg1Seg2 = CalcIP(Segment,vetex);
                IP_Seg2ToVtxSeg1Seg2 = CalcIP(Segment2,vetex);

                Int_t eRecoMode=2;
                cout << "WARNING   eRecoMode konstant set to =2   TODO ....   change on prompt!!" << endl;
                Float_t eValueGSNN_var00;
                Float_t IP_InBT_To_Vtx=IP_Pair_To_InBT;
                if (eRecoMode==0) eValueGSNN_var00=IP_InBT_To_Vtx;
                if (eRecoMode==1) eValueGSNN_var00=IP_InBT_To_Vtx;
                if (eRecoMode==2) {
                    eValueGSNN_var00=TMath::Min(IP_Seg1ToVtxSeg1Seg2,IP_Seg2ToVtxSeg1Seg2);
                }

                h_GSNN_var00->Fill(eValueGSNN_var00);
                h_GSNN_var01->Fill(GetMinimumDist(Segment,Segment2));
                h_GSNN_var02->Fill(GetdeltaRWithPropagation(Segment,Segment2));
                h_GSNN_var03->Fill(InBT->Z()-vtx->Z());
                h_GSNN_var04->Fill(GetdeltaThetaSingleAngles(Segment,Segment2));
                h_GSNN_var05->Fill(TMath::Abs(Segment->PID()-Segment2->PID()));
                h_GSNN_var06->Fill(Segment2->Flag()+Segment->Flag());

                // Purity 1:    Input =  1.0;
                // Purity 0.5:  Input =  0.5;
                // Purity else: Input =  0.0;
                if (Segment2->Flag()+Segment->Flag()==0&&TMath::Abs(Segment2->Flag())==11&&Segment->MCEvt()>0) {
                    value_GSNN_varInput=1;
                }
                else if (Segment2->Flag()+Segment->Flag()!=0&&TMath::Abs(Segment2->Flag())==11&&Segment->MCEvt()>0) {
                    value_GSNN_varInput=0.5;
                }
                else {
                    value_GSNN_varInput=0;
                }
                value_GSNN_var00=eValueGSNN_var00;
                value_GSNN_var01=GetMinimumDist(Segment,Segment2);
                value_GSNN_var02=GetdeltaRWithPropagation(Segment,Segment2);
                value_GSNN_var03=InBT->Z()-vtx->Z();
                value_GSNN_var04=GetdeltaThetaSingleAngles(Segment,Segment2);
                value_GSNN_var05=TMath::Abs(Segment->PID()-Segment2->PID());
                value_GSNN_var06=Segment2->Flag()+Segment->Flag();

                t_GSNN->Fill();

                cout << "I have filled the GSNN Tree now. End of this bracket." << endl;
            }
        }
        ///----------------------

        if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(GLOBAL_ShowerSegArray);

        Int_t s_NBT=0;
        Int_t s_NBTMC=0;
        Int_t s_NBTallMC=0;
        Int_t s_NBTeMC=0;
        Double_t  s_eff=0;
        Double_t s_purall=0;
        Double_t s_pure=0;
        CalcEffPurOfShower(GLOBAL_ShowerSegArray, s_NBT, s_NBTMC, s_NBTallMC, s_NBTeMC, s_purall, s_pure);

        //-----------------------------------
        // 4) Calculate pur/eff/NBT numbers,
        // not needed when only reconstruction
        // done:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>=2 || cmd_OUTPUTLEVEL==0 ) {
            Int_t NBT=0;
            Int_t NBTMC=0;
            Int_t NBTallMC=0;
            Int_t NBTeMC=0;
            Double_t  eff, purall, pure;
            CalcEffPurOfShower2(GLOBAL_ShowerSegArray, NBT, NBTMC, NBTallMC, NBTeMC, purall, pure, NBT_Neff, NBTMC_Neff,NBTMCe_Neff);


            // Fill only for MC Event:
            if (GLOBAL_InBT_MC>0) {
                GLOBAL_EvtBT_Flag=GLOBAL_EvtBT_FlagArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_MC=GLOBAL_EvtBT_MCArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_E=GLOBAL_EvtBT_EArray[GLOBAL_InBT_MC];
                GLOBAL_EvtBT_TanTheta=GLOBAL_EvtBT_TanThetaArray[GLOBAL_InBT_MC];
            }
            GLOBAL_trckdens=shower_trackdensb;
        }
//         cout << "// 4) Calculate pur/eff/NBT numbers...done." << endl;


//     XXXDEBUG

        //-----------------------------------
        // 5) Fill Tree:
        //-----------------------------------
        if (gEDBDEBUGLEVEL>2) cout << "// 5) Fill Tree:" << endl;
        TREE_ShowRecEff->Fill();
        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(TREE_ShowRecEff->GetEntries()-1);


        //-----------------------------------
        // 6a) Transfer ShowerArray to treebranchTreeEntry:
        //-----------------------------------
        if (cmd_OUTPUTLEVEL>0) {
            //cout << "// 6a) Transfer ShowerArray to treebranchTreeEntry:" << endl;
            TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TREE_ShowShower,GLOBAL_ShowerSegArray);
        }


        //------------------------------------
        // Reset and delete important things:
        // also  to avoid memory problems ...
        //-----------------------------------
        GLOBAL_ShowerSegArray->Clear();
        if (gEDBDEBUGLEVEL>2) cout << "--- --- after clear:  GLOBAL_ShowerSegArray->GetEntries(): "<< GLOBAL_ShowerSegArray->GetEntries() << endl;
        delete local_gAli;
        local_gAli=0;
        ++GLOBAL_INBTSHOWERNR;
        //------------------------------------



        IsFirstLoopCount=kFALSE;
    }
    // end of loop over GLOBAL_InBTArrayEntries
    //-----------------------------------------------------------------

    if (gEDBDEBUGLEVEL==2) cout << endl<<flush;
    if (gEDBDEBUGLEVEL>3) cout << "---TREE_ShowRecEff->GetEntries() ... " << TREE_ShowRecEff->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) cout << "---GLOBAL_INBTSHOWERNR ... " << GLOBAL_INBTSHOWERNR<< endl;



    Log(2, "ShowRec.cpp", "--- void ReconstructShowers_GS() done.");
    return;
}
//-------------------------------------------------------------------------------------------






































Bool_t AddBTToArrayWithCeck(EdbSegP* tryAttachedSegment, TObjArray* GLOBAL_ShowerSegArray)
{
    // We add by comparison with X,Y,TX,TY values, since this seems to be more failsafe
    // than adding by adresses.
    // This function returns   kTRUE   if  BT is already in the shower array
    // This function returns   kFALSE  if  BT is not yet in the shower array

    int nent=GLOBAL_ShowerSegArray->GetEntries();
    EdbSegP* ComparingSegment;


    if (gEDBDEBUGLEVEL>3) cout << "AddBTToArrayWithCeck()   Check: "<< tryAttachedSegment << " with all "<< nent << " entries of GLOBAL_ShowerSegArray "<< endl;

    Bool_t IsContained=kFALSE;

    for (int i=0; i<nent; ++i) {
        ComparingSegment=(EdbSegP*)GLOBAL_ShowerSegArray->At(i);
        if (TMath::Abs(tryAttachedSegment->X()-ComparingSegment->X())>0.1) continue;
        if (TMath::Abs(tryAttachedSegment->Y()-ComparingSegment->Y())>0.1) continue;

        if (TMath::Abs(tryAttachedSegment->TX()-ComparingSegment->TX())>0.01) continue;
        if (TMath::Abs(tryAttachedSegment->TY()-ComparingSegment->TY())>0.01) continue;

        IsContained=kTRUE;
    }


    if (gEDBDEBUGLEVEL>3) cout << "AddBTToArrayWithCeck DO WE ADD THIS BT ?? "<< !IsContained << endl;

    if (!IsContained) {
        GLOBAL_ShowerSegArray->Add(tryAttachedSegment);
    }

    return !IsContained;
}






//-------------------------------------------------------------------------------------------
EdbPVRec* TransformEdbPVRec(EdbPVRec* gAli, EdbSegP* InitiatorBT)
{
    Log(3, "ShowRec.cpp", "--- void TransformEdbPVRec() ---");

    //   local_halfpatternsize=11250;// debugTEST how long it takes more if we take a big area to reconstruct.
    Float_t halfpatternsize=local_halfpatternsize;


    // Informational Debug Output
    // DOWNSTREAM ORDER ASSUMED !!!
    /// THIS IS VERY IMPORTANT, because gAliSub has to be ordered in a way that 2nd plate follows after first,
    /// since we loop in the Reconstruct_??() Alogrithms over the Basetracks already added in the shower!
    Int_t npat = GLOBAL_gAli->Npatterns();  //number of plates
    Int_t firstplate= npat-cmd_FP;
    Int_t middleplate= npat-cmd_MP;
    Int_t actualplate= npat-cmd_FP;
    Int_t lastplate= TMath::Max(npat-cmd_LP-1,0);
    Int_t InBTplate= InitiatorBT->PID();
    Int_t InBTplateandNplate= InitiatorBT->PID()-cmd_NP+1;
    Int_t endlplatetopropagate=TMath::Max(InBTplateandNplate,lastplate);
    Float_t InBTZ= InitiatorBT->Z();

    if (gEDBDEBUGLEVEL>2) {
        cout << "--- TransformEdbPVRec --- DOWNSTREAM ORDER = " <<endl;
        cout << "--- TransformEdbPVRec --- npat = " << npat << endl;
        cout << "--- TransformEdbPVRec --- firstplate = " << firstplate << endl;
        cout << "--- TransformEdbPVRec --- middleplate = " << middleplate << endl;
        cout << "--- TransformEdbPVRec --- lastplate = " << lastplate << endl;
        cout << "--- TransformEdbPVRec --- InBTplate = " << InBTplate << endl;
        cout << "--- TransformEdbPVRec --- InBTplateandNplate = " << InBTplateandNplate << endl;
        cout << "--- TransformEdbPVRec --- endlplatetopropagate = " << endlplatetopropagate << endl;
        cout << "--- TransformEdbPVRec --- InBTZ = " << InBTZ << endl;
        cout << "--- TransformEdbPVRec --- InBTplate = " << InBTplate << endl;
    }

    // has to be deleted in some part of the script outside this function...
    // Dont forget , otherwise memory heap overflow!
    EdbPVRec* gAli_Sub = new EdbPVRec();

    // Create SubPattern objects
    EdbSegP* ExtrapolateInitiatorBT;
    ExtrapolateInitiatorBT = (EdbSegP*)InitiatorBT->Clone();

    // Create Variables For ExtractSubpattern boundaries
    Float_t mini[5];
    Float_t maxi[5];
    mini[0]=ExtrapolateInitiatorBT->X()-halfpatternsize;
    mini[1]=ExtrapolateInitiatorBT->Y()-halfpatternsize;
    maxi[0]=ExtrapolateInitiatorBT->X()+halfpatternsize;
    maxi[1]=ExtrapolateInitiatorBT->Y()+halfpatternsize;
    mini[2]=-0.5;
    mini[3]=-0.5;
    mini[4]=0.0;
    maxi[2]=0.5;
    maxi[3]=0.5;
    maxi[4]=100.0;


    ///----------------------------------------------------------
    /// DEBUG TEST !!! MAYBE IT GOES FASTER LIKE THIS;
    /// AND ALSO THE BG CALCULATION IS BETTER PERFORMED .....
    if (cmd_EXTHETA==1) {
        mini[2]=ExtrapolateInitiatorBT->TX()-0.15;
        mini[3]=ExtrapolateInitiatorBT->TY()-0.15;
        maxi[2]=ExtrapolateInitiatorBT->TX()+0.15;
        maxi[3]=ExtrapolateInitiatorBT->TY()+0.15;
    }
    ///----------------------------------------------------------

    EdbPattern* singlePattern;
    Int_t MCMixFlag=-1;
    if (cmd_MCMIX==1) {
        MCMixFlag=-1;
    }
    else {
        MCMixFlag=InitiatorBT->MCEvt();
    }

    // Add the subpatterns in a loop for the plates:
    // in reverse ordering.due to donwstream behaviour (!):
    // (Only downstream is supported now...)
    for (Int_t ii=endlplatetopropagate; ii<=InBTplate; ++ii) {

        Float_t zpos=gAli->GetPattern(ii)->Z();
        if (gEDBDEBUGLEVEL>3) cout << "--- --- Loop: ii, zpos  "<< ii  <<  "  "  << zpos << "; Print InitiatorBT,ExtrapolateInitiatorBT"<<endl;

        ExtrapolateInitiatorBT->PropagateTo(zpos);
        if (gEDBDEBUGLEVEL>3) {
            InitiatorBT->PrintNice();
            ExtrapolateInitiatorBT->PrintNice();
        }

        mini[0]=ExtrapolateInitiatorBT->X()-halfpatternsize;
        mini[1]=ExtrapolateInitiatorBT->Y()-halfpatternsize;
        maxi[0]=ExtrapolateInitiatorBT->X()+halfpatternsize;
        maxi[1]=ExtrapolateInitiatorBT->Y()+halfpatternsize;

        singlePattern=(EdbPattern*)gAli->GetPattern(ii)->ExtractSubPattern(mini,maxi,MCMixFlag);
//      cout << "singlePattern with MCMixFlag= " << MCMixFlag << "  nentries= " <<  singlePattern->N() << endl;


        singlePattern-> SetID(gAli->GetPattern(ii)->ID());
        singlePattern-> SetPID(gAli->GetPattern(ii)->PID());
        gAli_Sub->AddPattern(singlePattern);
    }
    if (gEDBDEBUGLEVEL>2) cout <<"--- gAli_Sub->Print():"<<endl;
    if (gEDBDEBUGLEVEL>2) gAli_Sub->Print();
    if (gEDBDEBUGLEVEL>2) cout <<"--- ----------------------------------"<<endl;

    return gAli_Sub;
}
//-------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------
EdbPVRec* TransformEdbPVRec_BackWard(EdbPVRec* gAli, EdbSegP* InitiatorBT)
{
    Log(3, "ShowRec.cpp", "--- void TransformEdbPVRec_BackWard() ---");

    local_halfpatternsize=11250;// debugTEST how long it takes more if we take a big area to reconstruct.
    local_halfpatternsize=5000;// debugTEST how long it takes more if we take a big area to reconstruct.
    local_halfpatternsize=2000;// debugTEST how long it takes more if we take a big area to reconstruct.
    Float_t halfpatternsize=local_halfpatternsize;

    // Informational Debug Output
    // DOWNSTREAM ORDER ASSUMED !!!
    /// THIS IS VERY IMPORTANT, because gAliSub has to be ordered in a way that 2nd plate follows after first,
    /// since we loop in the Reconstruct_??() Alogrithms over the Basetracks already added in the shower!
    Int_t npat = GLOBAL_gAli->Npatterns();  //number of plates
    Int_t firstplate= npat-cmd_FP;
    Int_t middleplate= npat-cmd_MP;
    Int_t actualplate= npat-cmd_FP;
    Int_t lastplate= TMath::Max(npat-cmd_LP-1,0);
    Int_t InBTplate= InitiatorBT->PID();
    Int_t InBTplateandNplate= InitiatorBT->PID()-cmd_NP+1;
    Int_t endlplatetopropagate=TMath::Max(InBTplateandNplate,lastplate);
    Float_t InBTZ= InitiatorBT->Z();

    if (gEDBDEBUGLEVEL>3) {
        cout << "--- DOWNSTREAM ORDER = " <<endl;
        cout << "--- npat = " << npat << endl;
        cout << "--- firstplate = " << firstplate << endl;
        cout << "--- middleplate = " << middleplate << endl;
        cout << "--- lastplate = " << lastplate << endl;
        cout << "--- InBTplate = " << InBTplate << endl;
        cout << "--- InBTplateandNplate = " << InBTplateandNplate << endl;
        cout << "--- endlplatetopropagate = " << endlplatetopropagate << endl;
        cout << "--- InBTZ = " << InBTZ << endl;
    }

    // has to be deleted in some part of the script outside this function...
    // Dont forget , otherwise memory heap overflow!
    EdbPVRec* gAli_Sub = new EdbPVRec();

    // Create SubPattern objects
    EdbSegP* ExtrapolateInitiatorBT;
    ExtrapolateInitiatorBT = (EdbSegP*)InitiatorBT->Clone();

    // Create Variables For ExtractSubpattern boundaries
    Float_t mini[5];
    Float_t maxi[5];
    mini[0]=ExtrapolateInitiatorBT->X()-halfpatternsize;
    mini[1]=ExtrapolateInitiatorBT->Y()-halfpatternsize;
    maxi[0]=ExtrapolateInitiatorBT->X()+halfpatternsize;
    maxi[1]=ExtrapolateInitiatorBT->Y()+halfpatternsize;
    mini[2]=-0.5;
    mini[3]=-0.5;
    mini[4]=0.0;
    maxi[2]=0.5;
    maxi[3]=0.5;
    maxi[4]=100.0;

    EdbPattern* singlePattern;

    for (Int_t ii=0; ii<npat; ++ii) {

        Float_t zpos=gAli->GetPattern(ii)->Z();
        if (gEDBDEBUGLEVEL>3) cout << "--- --- Loop: ii, zpos  "<< ii  <<  "  "  << zpos << "; Print InitiatorBT,ExtrapolateInitiatorBT"<<endl;

        ExtrapolateInitiatorBT->PropagateTo(zpos);
        if (gEDBDEBUGLEVEL>3) {
            InitiatorBT->PrintNice();
            ExtrapolateInitiatorBT->PrintNice();
        }

        mini[0]=ExtrapolateInitiatorBT->X()-halfpatternsize;
        mini[1]=ExtrapolateInitiatorBT->Y()-halfpatternsize;
        maxi[0]=ExtrapolateInitiatorBT->X()+halfpatternsize;
        maxi[1]=ExtrapolateInitiatorBT->Y()+halfpatternsize;

        singlePattern=(EdbPattern*)gAli->GetPattern(ii)->ExtractSubPattern(mini,maxi,InitiatorBT->MCEvt());
        singlePattern-> SetID(gAli->GetPattern(ii)->ID());
        singlePattern-> SetPID(gAli->GetPattern(ii)->PID());
        gAli_Sub->AddPattern(singlePattern);
    }
    if (gEDBDEBUGLEVEL>3) cout <<"--- gAli_Sub->Print():"<<endl;
    if (gEDBDEBUGLEVEL>3) gAli_Sub->Print();
    if (gEDBDEBUGLEVEL>3) cout <<"--- ----------------------------------"<<endl;

    if (gEDBDEBUGLEVEL>2) cout <<"--- gAli_Sub with ():"<< gAli_Sub->Npatterns() << "patterns."<<endl;

    return gAli_Sub;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
EdbPVRec* TransformEdbPVRec_SA(EdbPVRec* gAli, EdbSegP* InitiatorBT)
{
    Log(3, "ShowRec.cpp", "--- void TransformEdbPVRec_SA() ---");

    // Informational Debug Output
    // DOWNSTREAM ORDER ASSUMED !!!
    /// THIS IS VERY IMPORTANT, because gAliSub has to be ordered in a way that 2nd plate follows after first,
    /// since we loop in the Reconstruct_??() Alogrithms over the Basetracks already added in the shower!
    Int_t npat = GLOBAL_gAli->Npatterns();  //number of plates
    Int_t firstplate= npat-cmd_FP;
    Int_t middleplate= npat-cmd_MP;
    Int_t actualplate= npat-cmd_FP;
    Int_t lastplate= TMath::Max(npat-cmd_LP-1,0);
    Int_t InBTplate= InitiatorBT->PID();
    Int_t InBTplateandNplate= InitiatorBT->PID()-cmd_NP+1;
    Int_t endlplatetopropagate=TMath::Max(InBTplateandNplate,lastplate);
    Float_t InBTZ= InitiatorBT->Z();

    if (gEDBDEBUGLEVEL>2) {
        cout << "--- DOWNSTREAM ORDER = " <<endl;
        cout << "--- npat = " << npat << endl;
        cout << "--- firstplate = " << firstplate << endl;
        cout << "--- middleplate = " << middleplate << endl;
        cout << "--- lastplate = " << lastplate << endl;
        cout << "--- InBTplate = " << InBTplate << endl;
        cout << "--- InBTplateandNplate = " << InBTplateandNplate << endl;
        cout << "--- endlplatetopropagate = " << endlplatetopropagate << endl;
        cout << "--- InBTZ = " << InBTZ << endl;
    }

    // has to be deleted in some part of the script outside this function...
    // Dont forget , otherwise memory heap overflow!
    EdbPVRec* gAli_Sub = new EdbPVRec();

    // Create SubPattern objects
    EdbSegP* ExtrapolateInitiatorBT;
    ExtrapolateInitiatorBT = (EdbSegP*)InitiatorBT->Clone();

    Float_t halfsize=CUT_PARAMETER[1];

    // Create Variables For ExtractSubpattern boundaries
    Float_t mini[5];
    Float_t maxi[5];
    mini[0]=ExtrapolateInitiatorBT->X()-halfsize;
    mini[1]=ExtrapolateInitiatorBT->Y()-halfsize;
    maxi[0]=ExtrapolateInitiatorBT->X()+halfsize;
    maxi[1]=ExtrapolateInitiatorBT->Y()+halfsize;
    mini[2]=-0.5;
    mini[3]=-0.5;
    mini[4]=0.0;
    maxi[2]=0.5;
    maxi[3]=0.5;
    maxi[4]=100.0;

    EdbPattern* singlePattern;

    // Add the subpatterns in a loop for the plates:
    // in reverse ordering.due to donwstream behaviour (!):
    // (Only downstream is supported now...)
    for (Int_t ii=endlplatetopropagate; ii<=InBTplate; ++ii) {

        Float_t zpos=gAli->GetPattern(ii)->Z();
        if (gEDBDEBUGLEVEL>3) cout << "--- --- Loop: ii, zpos  "<< ii  <<  "  "  << zpos << "; Print InitiatorBT,ExtrapolateInitiatorBT"<<endl;

        ExtrapolateInitiatorBT->PropagateTo(zpos);
        if (gEDBDEBUGLEVEL>3) {
            InitiatorBT->PrintNice();
            ExtrapolateInitiatorBT->PrintNice();
        }

        mini[0]=ExtrapolateInitiatorBT->X()-halfsize;
        mini[1]=ExtrapolateInitiatorBT->Y()-halfsize;
        maxi[0]=ExtrapolateInitiatorBT->X()+halfsize;
        maxi[1]=ExtrapolateInitiatorBT->Y()+halfsize;

        singlePattern=(EdbPattern*)gAli->GetPattern(ii)->ExtractSubPattern(mini,maxi,InitiatorBT->MCEvt());
        singlePattern-> SetID(gAli->GetPattern(ii)->ID());
        singlePattern-> SetPID(gAli->GetPattern(ii)->PID());
        gAli_Sub->AddPattern(singlePattern);
    }
    if (gEDBDEBUGLEVEL>2) cout <<"--- gAli_Sub->Print():"<<endl;
    if (gEDBDEBUGLEVEL>2) gAli_Sub->Print();
    if (gEDBDEBUGLEVEL>2) cout <<"--- ----------------------------------"<<endl;

    return gAli_Sub;
}
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
Bool_t FindFollowingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *local_gAli, TObjArray* showersegarray)
{
    Log(4, "ShowRec.cpp", "--- Bool_t FindFollowingBTs() ---");
    cout << "WARNING::WARNING   Bool_t FindFollowingBTs() ...   IS NOT SUPPORTED ANYMORE. RETURN kFALSE now !!!"<<endl;
    return kFALSE;

    // ATTENTION NEW CUT CONDITION ITRODUCED...
    // SIGMA OF DTHETA CUT IS DEPENDING AN DIFERNECE TO INBT PLATE....
    // CUT_dtheta=1.5,2.0,2.5*CUT_dtheta_original
    int diff_pid=0;
    Float_t CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) <3) CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) >2) CUTFACTOR=1.5;
    if (TMath::Abs(InBT->PID()-s->PID()) >5) CUTFACTOR=2.0;
    //   cout << TMath::Abs(InBT->PID()-s->PID()) << "   " << CUTFACTOR << endl;
    //NORMAL:
    CUTFACTOR=1.0;

    EdbSegP* s_TestBT;
    EdbSegP* seg;
    Int_t nentries=showersegarray->GetEntries();
    Double_t dZ;

    // For the very first Z position we do not test
    // if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
    // Take 50microns and 80mrad in (dR/dT) around.
    // This does not affect the normal results, but helps for
    // events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
        //cout << "Test here..."<<endl;
        //cout << "GetdeltaTheta(s, InBT)= " << GetdeltaTheta(s, InBT) << endl;
        //cout << "GetdeltaRWithPropagation(s, InBT)= " << GetdeltaRWithPropagation(s, InBT) << endl;
        if (GetdeltaTheta(s, InBT) < 0.08 && GetdeltaRWithPropagation(s, InBT) < 50.0) {
            return kTRUE;
        }
        //cout << "Test here...done. Not fullfilled condition."<<endl;
    }


    // it is true to use  local_gAli  since this is given over in the function head as local_gAli  ...
    Int_t local_gAli_npat=local_gAli->Npatterns();
    //if (gEDBDEBUGLEVEL>2)
    //  cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

    // Loop over all plates of local_gAli, since this is already
    // extracted with the right numbers of plates...
    for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
        if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

        //cout << local_gAli->GetPattern(patterloop_cnt)->Z() <<  "   " <<  InBT->Z() << endl;
        if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;
        if (TMath::Abs(local_gAli->GetPattern(patterloop_cnt)->Z()-s->Z())>4.0*1300.0+50.0) continue;// Exclude the case of more than 4 plates after..

        for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
            s_TestBT = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);
            if (gEDBDEBUGLEVEL>3) seg->PrintNice();


            dZ=TMath::Abs(s_TestBT->Z()-s->Z());
            ///if (dZ<30) continue;                  // Exclude the case of same Zpositions...
            if (dZ<0.1&&TMath::Abs(s_TestBT->X()-s->X())<1.0) continue;     // Exclude the case of same Basetracks:
            if (dZ>(4*1300.0)+30.0) continue;     // Exclude the case of more than 4 plates after...


            if (GetdeltaThetaSingleAngles(s, s_TestBT) > CUTFACTOR*CUT_PARAMETER[3] ) continue;
            if (GetdeltaRWithPropagation(s, s_TestBT) > CUT_PARAMETER[2]) continue;
            return kTRUE;
        }

    }

    //---------------------------------------------
    return kFALSE;
}
//-------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------
Bool_t FindPrecedingBTs_local_gAli(EdbSegP* s, EdbSegP* InBT, EdbPVRec *local_gAli, TObjArray* showersegarray)
{
    Log(4, "ShowRec.cpp", "--- Bool_t FindPrecedingBTs_local_gAli() ---");

    int diff_pid=0;
    Float_t CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) <3) CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) >2) CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) >5) CUTFACTOR=2.0;

    EdbSegP* s_TestBT;
    EdbSegP* seg;
    Int_t nentries=showersegarray->GetEntries();
    Double_t dZ;


    // Dont check the BT before the InBT position:
    if (s->Z()<InBT->Z()) {
        return kFALSE;
    }

    // For the very first Z position we do not test
    // if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
    // Take 50microns and 80mrad in (dR/dT) around.
    // This does not affect the normal results, but helps for
    // events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
        //cout << "Test here..."<<endl;
        //cout << "GetdeltaThetaSingleAngles(s, InBT)= " << GetdeltaThetaSingleAngles(s, InBT) << endl;
        //cout << "GetdeltaRWithPropagation(s, InBT)= " << GetdeltaRWithPropagation(s, InBT) << endl;
        if (GetdeltaThetaSingleAngles(s, InBT) < 0.08 && GetdeltaRWithPropagation(s, InBT) < 50.0) {
            return kTRUE;
        }
        //cout << "Test here...done. Not fullfilled condition."<<endl;
    }


    // it is true to use  local_gAli  since this is given over in the function head as local_gAli  ...
    Int_t local_gAli_npat=local_gAli->Npatterns();
    //if (gEDBDEBUGLEVEL>2)
    //  cout << "--- local_gAli_npat=  " << local_gAli_npat << endl;

    // Loop over all plates of local_gAli, since this is already
    // extracted with the right numbers of plates...
    for (Int_t patterloop_cnt=local_gAli_npat-1; patterloop_cnt>=0; --patterloop_cnt) {
        if (gEDBDEBUGLEVEL>3) cout << "--- --- Doing patterloop_cnt= " << patterloop_cnt << endl;

        //cout << local_gAli->GetPattern(patterloop_cnt)->Z() <<  "   " <<  InBT->Z() << endl;
        if (local_gAli->GetPattern(patterloop_cnt)->Z()<InBT->Z()) continue;
        if (TMath::Abs(local_gAli->GetPattern(patterloop_cnt)->Z()-s->Z())>3.0*1300.0+50.0) continue;// Exclude the case of more than 4 plates after/before..

        for (Int_t btloop_cnt=0; btloop_cnt<local_gAli->GetPattern(patterloop_cnt)->GetN(); ++btloop_cnt) {
            s_TestBT = (EdbSegP*)local_gAli->GetPattern(patterloop_cnt)->GetSegment(btloop_cnt);

            dZ=TMath::Abs(s_TestBT->Z()-s->Z());

            // test only BTs which have LOWER Z (so they are before)....
            if (s_TestBT->Z()-s->Z()>=0) continue;

            if (dZ<30) continue;                  // Exclude the case of same Zpositions...
            if (dZ<0.1&&TMath::Abs(s_TestBT->X()-s->X())<1.0) continue;     // Exclude the case of same Basetracks:

            // but why not search in both directions foreward and afterward ???
            if (dZ>(3.0*1300.0+30.0)) continue;     // Exclude the case of more than 4 plates after...

            if (GetdeltaThetaSingleAngles(s, s_TestBT) > CUTFACTOR*CUT_PARAMETER[3] ) continue;
            if (GetdeltaRWithPropagation(s, s_TestBT) > CUT_PARAMETER[2]) continue;
            return kTRUE;
        }

    }

    //---------------------------------------------
    return kFALSE;
}
//-------------------------------------------------------------------------------------------






//-------------------------------------------------------------------------------------------
Bool_t FindPrecedingBTs(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, TObjArray* showersegarray)
{
    Log(4, "ShowRec.cpp", "--- Bool_t FindPrecedingBTs() ---");

    int diff_pid=0;
    Float_t CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) <3) CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) >2) CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) >5) CUTFACTOR=2.0;

    EdbSegP* s_TestBT;
    Int_t nentries=showersegarray->GetEntries();
    Double_t dZ;

    // Dont check the BT before the InBT position:
    if (s->Z()<InBT->Z()) {
        return kFALSE;
    }

    // For the very first Z position we do not test
    // if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
    // Take 50microns and 80mrad in (dR/dT) around.
    // This does not affect the normal results, but helps for
    // events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
        //cout << "Test here..."<<endl;
        //cout << "GetdeltaTheta(s, InBT)= " << GetdeltaTheta(s, InBT) << endl;
        //cout << "GetdeltaRWithPropagation(s, InBT)= " << GetdeltaRWithPropagation(s, InBT) << endl;
        if (GetdeltaTheta(s, InBT) < 0.08 && GetdeltaRWithPropagation(s, InBT) < 50.0) {
            return kTRUE;
        }
        //cout << "Test here...done. Not fullfilled condition."<<endl;
    }

    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( showersegarray->At(i) );

        if (gEDBDEBUGLEVEL>3) cout << "--- --- Do   "<< s_TestBT->ID() << " " <<  s_TestBT->PID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << endl;

        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(3*1300.0)+30.0) continue;     // Exclude the case of more than 4 plates before...

        if (gEDBDEBUGLEVEL>3) cout << "--- --- Checking dT,dR and dZ for i:  " << i << "  " << GetdeltaTheta(s, s_TestBT)  << "  " << GetdeltaRWithPropagation(s, s_TestBT) << "  "<<dZ << endl;

        if (GetdeltaTheta(s, s_TestBT) > CUTFACTOR*CUT_PARAMETER[3] ) continue;
        if (GetdeltaRWithPropagation(s, s_TestBT) > CUT_PARAMETER[2]) continue;

        if (gEDBDEBUGLEVEL>3) cout << "--- --- Checking dT,dR and dZ for i:  " << i << "  " << GetdeltaTheta(s, s_TestBT)  << "  " << GetdeltaRWithPropagation(s, s_TestBT) << "  "<<dZ << "   ok!"<<endl;
        return kTRUE;
    }
    //---------------------------------------------
    return kFALSE;
}
//-------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------
Bool_t FindPrecedingBTsSingleThetaAngle(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, TObjArray* showersegarray)
{
    Log(4, "ShowRec.cpp", "--- Bool_t FindPrecedingBTsSingleThetaAngle() ---");

    int diff_pid=0;
    Float_t CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) <3) CUTFACTOR=1.0;
    if (TMath::Abs(InBT->PID()-s->PID()) >2) CUTFACTOR=1.0;

    EdbSegP* s_TestBT;
    Int_t nentries=showersegarray->GetEntries();
    Double_t dZ;

    // Dont check the BT before the InBT position:
    if (s->Z()<InBT->Z()) {
        return kFALSE;
    }

    // For the very first Z position we do not test
    // if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
    // Take 50microns and 80mrad in (dR/dT) around.
    // This does not affect the normal results, but helps for
    // events which may have a second BT close to InBT (like in e+e-)
    if (s->Z()==InBT->Z()) {
        /*
          cout << "Bool_t FindPrecedingBTsSingleThetaAngle(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, TObjArray* showersegarray)" << endl;
          cout << "Test here... for s ( " << s << " ) and InBT ( " << InBT << " ) :" <<   endl;
        s->PrintNice();
        InBT->PrintNice();
               cout << "GetdeltaThetaSingleAngles(s, InBT)= " << GetdeltaThetaSingleAngles(s, InBT) << endl;
               cout << "GetdeltaRWithPropagation(s, InBT)= " << GetdeltaRWithPropagation(s, InBT) << endl;
        */
        if (GetdeltaThetaSingleAngles(s, InBT) < 0.08 && GetdeltaRWithPropagation(s, InBT) < 50.0) {
            return kTRUE;
        }
        //cout << "Test here...done. Not fullfilled condition."<<endl;
    }


    /// 11.06.2010: New including case:
    Float_t nmaxholes=3.0;
    if (CUT_PARAMETER[4]==3) nmaxholes=3.0;
    if (CUT_PARAMETER[4]==5) nmaxholes=5.0;
    if (CUT_PARAMETER[4]==9) nmaxholes=9.0;
//   cout << "DEBUG    CUT_PARAMETER[4] = "  << CUT_PARAMETER[4] << endl;
//   cout << "DEBUG    nmaxholes = "  << nmaxholes << endl;

    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( showersegarray->At(i) );

        if (gEDBDEBUGLEVEL>3) cout << "--- --- Do   "<< s_TestBT->ID() << " " <<  s_TestBT->PID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << endl;

        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(nmaxholes*1300.0)+30.0) continue;     // Exclude the case of more than 4 plates before...

        if (gEDBDEBUGLEVEL>3) cout << "--- --- Checking dT,dR and dZ for i:  " << i << "  " << GetdeltaThetaSingleAngles(s, s_TestBT)  << "  " << GetdeltaRWithPropagation(s, s_TestBT) << "  "<<dZ << endl;

        if (GetdeltaThetaSingleAngles(s, s_TestBT) > CUTFACTOR*CUT_PARAMETER[3] ) continue;
        if (GetdeltaRWithPropagation(s, s_TestBT) > CUT_PARAMETER[2]) continue;

        if (gEDBDEBUGLEVEL>3) {
            cout << "--- --- Checking dT,dR and dZ for i:  " << i << "  " << GetdeltaThetaSingleAngles(s, s_TestBT)  << "  " << GetdeltaRWithPropagation(s, s_TestBT) << "  "<<dZ << ".  ok! Print segment to check and the shower segment which maches:"<<endl;
            s->PrintNice();
            s_TestBT->PrintNice();
        }
        return kTRUE;
    }
    //---------------------------------------------

    return kFALSE;
}
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
Bool_t FindPrecedingBTsSingleThetaAngleTCDEBUG(EdbSegP* s, EdbSegP* InBT, EdbPVRec *gAli, TObjArray* showersegarray)
{
    Log(4, "ShowRec.cpp", "--- Bool_t FindPrecedingBTsSingleThetaAngleTCDEBUG() ---");

    Float_t CUTFACTOR=1.0;

    EdbSegP* s_TestBT;
    Int_t nentries=showersegarray->GetEntries();
    Double_t dZ;

    // Dont check the BT before the InBT position:
    if (s->Z()<InBT->Z()) {
        return kFALSE;
    }



    // For the very first Z position we do not test
    // if testBT has Preceeders, only if it it has a BT around (case for e+e- coming from gammma):
    // Take 50microns and 80mrad in (dR/dT) around.
    // This does not affect the normal results, but helps for
    // events which may have a second BT close to InBT (like in e+e-)
    if (TMath::Abs(s->Z()-InBT->Z())<5.0) {
        //cout << "Test here..."<<endl;
        //cout << "GetdeltaThetaSingleAngles(s, InBT)= " << GetdeltaThetaSingleAngles(s, InBT) << endl;
        //cout << "GetdeltaRWithPropagation(s, InBT)= " << GetdeltaRWithPropagation(s, InBT) << endl;
        if (GetdeltaThetaSingleAngles(s, InBT) < 0.08 && GetdeltaRWithPropagation(s, InBT) < 50.0) {
            return kTRUE;
        }


        //cout << "Test here...done. Not fullfilled condition."<<endl;
    }


    for (Int_t i=nentries-1; i>=0; --i) {
        s_TestBT = (EdbSegP*)( showersegarray->At(i) );

        // cout << "--- --- FindPrecedingBTsSingleThetaAngleTCDEBUG  Do   "<< s_TestBT->ID() << " " <<  s_TestBT->PID() << " " << s_TestBT->MCEvt() <<"  " << s_TestBT->Z() << endl;

        dZ=TMath::Abs(s_TestBT->Z()-s->Z());
        if (dZ<30) continue;                  // Exclude the case of same Zpositions...
        if (dZ>(3*1300.0)+30.0) continue;     // Exclude the case of more than 4 plates before...

        Float_t interim_dT=GetdeltaThetaSingleAngles(s, s_TestBT);
        Float_t interim_dRProp=GetdeltaRWithPropagation(s, s_TestBT);

        // cout << "--- --- FindPrecedingBTsSingleThetaAngleTCDEBUG  Checking dT,dR and dZ for i:  " << i << "  " << interim_dT << "  " << interim_dRProp << "  "<<dZ << endl;

        if (GetdeltaThetaSingleAngles(s, s_TestBT) > CUTFACTOR*CUT_PARAMETER[3] ) continue;
        if (GetdeltaRWithPropagation(s, s_TestBT) > CUT_PARAMETER[2]) continue;

        // cout << "--- --- Checking dT,dR and dZ:    ok!"<<endl;
        return kTRUE;
    }
    //---------------------------------------------

    return kFALSE;
}
//-------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------
Bool_t GetConeOrTubeDistanceToInBT(EdbSegP* sa, EdbSegP* InBT, Double_t CylinderRadius, Double_t ConeAngle)
{
    Log(4,"ShowRec.cpp", "--- Bool_t GetConeOrTubeDistanceToInBT() ---");

    TVector3 x1(InBT->X(),InBT->Y(),InBT->Z());
    TVector3 x2(sa->X(),sa->Y(),sa->Z());
    TVector3 direction_x1(InBT->TX()*1300,InBT->TY()*1300,1300);
    TVector3 direction_x2(sa->TX()*1300,sa->TY()*1300,1300);
    TVector3 u1=x2-x1;

    Double_t direction_x1_norm= direction_x1.Mag();
    Double_t cosangle=  (direction_x1*u1)/(u1.Mag()*direction_x1_norm);
    Double_t angle = TMath::ACos(cosangle);
    // NO THIS IS NOT THE CONE ANGLE!!

    TVector3 direction_1(InBT->TX()*1300,InBT->TY()*1300,1300);
    TVector3 direction_2(sa->TX()*1300,sa->TY()*1300,1300);

    /*
    cout <<"------"<<endl;
    cout << "angle =  " << angle << endl;
    cout << "angle V2 =  " << x1.Angle(x2) << endl;
    cout << "angle V3 =  " << direction_x1.Angle(direction_x2) << endl;
    cout << "angle V4 =  " << direction_1.Angle(direction_2) << "   " <<  direction_1.Angle(direction_2)/3.14*180.0 << endl;

    cout << "angle V5   direction_x1.Angle(u1  =  " << direction_x1.Angle(u1) << endl;
    cout << "angle V5   u1.Angle(direction_x1  =  " << u1.Angle(direction_x1) << endl;
    */
    angle=u1.Angle(direction_x1);


    // For the case where the two basetracks have same z position
    // the angle is about 90 degree so it makes no sense to calculate it...
    // therefore we set it artificially to zero:
    if (TMath::Abs(InBT->Z()-sa->Z())<5.0 ) {
        angle=0.0;
        //if (gEDBDEBUGLEVEL>3) //cout << "same z position, set angle artificially to zero" << endl;
    }

    //   cout << "--- Bool_t GetConeOrTubeDistanceToInBT() ---"<<endl;
    //  InBT->PrintNice();
    //  sa->PrintNice();
    //  cout << "angle= " <<  angle << "  ConeAngle= " << ConeAngle << endl;


    // If this InBT is in z position AFTER the testBT, the cone goes in the other direction and therefore, we have
    // to mirror the angle by 180 degree:
    if (angle>TMath::Pi()/2.0) {
        angle=TMath::Abs(TMath::Pi()-angle);
        //cout << "reverse angle: " << angle << endl;
    }

    /// Outside if angle greater than ConeAngle (to be fulfilled for Cone and Tube in both cases)
    if (angle>ConeAngle) {
        return kFALSE;
    }

    /// if angle smaller than ConeAngle, then you can differ between Tuberadius and CylinderRadius
    Double_t TubeDistance = 1.0/direction_x1_norm  *  ( (x2-x1).Cross(direction_x1) ).Mag();
    //  cout << "CylinderRadius= " <<  CylinderRadius << "  TubeDistance= " << TubeDistance << endl;
    if (TubeDistance>CylinderRadius) {
        return kFALSE;
    }

    return kTRUE;
}
//-------------------------------------------------------------------------------------------













//-------------------------------------------------------------------------------------------

Bool_t GetConeOrTubeDistanceToBTOfShowerArray(EdbSegP* sa, EdbSegP* InBT, TObjArray* showersegarray, Double_t CylinderRadius, Double_t ConeAngle)
{

    Bool_t isTrueForBT=kFALSE;
    Int_t lastI=0;
    Double_t factor=1.0;

    EdbSegP* s_TestBT;
    Int_t nentries=showersegarray->GetEntries();
    //cout << "nentries=showersegarray->GetEntries(); "  <<  nentries << endl;

    // Now call GetConeOrTubeDistanceToInBT for every BT which was reconstructed up to now in the shower:
    for (Int_t i=0; i<nentries; i++) {
        s_TestBT = (EdbSegP*)( showersegarray->At(i) );
        // Dont check the BT which is before or same Z as the BaseTrack(i) position:
        // But ---not--- for the first Basetrack. There we allow it!
        //if (s_TestBT->Z()>=sa->Z() && i>0) continue;
        if (s_TestBT->Z()>sa->Z() ) continue;

        lastI=i;

        if (i==0) {
            factor=1.0;
        }
        else {
            factor=3.0;
        }


        if (GetConeOrTubeDistanceToInBT(sa, s_TestBT, factor*CylinderRadius, factor*ConeAngle)==kTRUE) {
            isTrueForBT=kTRUE;
            break;
        }

        //if (i>0) cout << "GetConeOrTubeDistanceToBTOfShowerArray::  i="<<i<<endl;

    }


    if (isTrueForBT) {
        //if (lastI>0) cout <<"for i= " << lastI<< " return true with factor "<< factor << endl;
        return kTRUE;
    }

    return kFALSE;
}

//-------------------------------------------------------------------------------------------























//-------------------------------------------------------------------------------------------
Bool_t CalcConeOrTubeDistanceToInBT(EdbSegP* sa, EdbSegP* InBT, Double_t CylinderRadius, Double_t ConeAngle)
{
    Log(4,"ShowRec.cpp", "--- Bool_t CalcConeOrTubeDistanceToInBT() ---");

    TVector3 x1(InBT->X(),InBT->Y(),InBT->Z());
    TVector3 x2(sa->X(),sa->Y(),sa->Z());
    TVector3 direction_x1(InBT->TX()*1300,InBT->TY()*1300,1300);
    TVector3 direction_x2(sa->TX()*1300,sa->TY()*1300,1300);
    TVector3 u1=x2-x1;

    Double_t direction_x1_norm= direction_x1.Mag();
    Double_t cosangle=  (direction_x1*u1)/(u1.Mag()*direction_x1_norm);
    Double_t angle = TMath::ACos(cosangle);
    // NO THIS IS NOT THE CONE ANGLE!!

    TVector3 direction_1(InBT->TX()*1300,InBT->TY()*1300,1300);
    TVector3 direction_2(sa->TX()*1300,sa->TY()*1300,1300);

    angle=u1.Angle(direction_x1);


    // For the case where the two basetracks have same z position
    // the angle is about 90 degree so it makes no sense to calculate it...
    // therefore we set it artificially to zero:
    if (TMath::Abs(InBT->Z()-sa->Z())<5.0 ) {
        angle=0.0;
        //if (gEDBDEBUGLEVEL>3) //cout << "same z position, set angle artificially to zero" << endl;
    }



    // If this InBT is in z position AFTER the testBT, the cone goes in the other direction and therefore, we have
    // to mirror the angle by 180 degree:
    if (angle>TMath::Pi()/2.0) {
        angle=TMath::Abs(TMath::Pi()-angle);
        //cout << "reverse angle: " << angle << endl;
    }

    /// Outside if angle greater than ConeAngle (to be fulfilled for Cone and Tube in both cases)
    if (angle>ConeAngle) {
        return kFALSE;
    }

    /// if angle smaller than ConeAngle, then you can differ between Tuberadius and CylinderRadius
    Double_t TubeDistance = 1.0/direction_x1_norm  *  ( (x2-x1).Cross(direction_x1) ).Mag();
    //  cout << "CylinderRadius= " <<  CylinderRadius << "  TubeDistance= " << TubeDistance << endl;
    if (TubeDistance>CylinderRadius) {
        return kFALSE;
    }



    //cout << "--- Bool_t CalcConeOrTubeDistanceToInBT() ---"<<endl;
    //InBT->PrintNice();
    //sa->PrintNice();
    //cout << "angle= " <<  angle << "  ConeAngle= " << ConeAngle << endl;
    //cout << "CylinderRadius= " <<  CylinderRadius << "  TubeDistance= " << TubeDistance << endl;

    return kTRUE;
}
//-------------------------------------------------------------------------------------------





//-------------------------------------------------------------------------------------------
Double_t GetdeltaRWithPropagation(EdbSegP* s,EdbSegP* stest)
{
    Log(4, "ShowRec.cpp", "--- Bool_t GetdeltaRWithPropagation() ---");
    // Propagate s to z-position of stest:
    Double_t zorig;
    Double_t dR;
    zorig=s->Z();
    s->PropagateTo(stest->Z());
    dR=(s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y());
    dR=TMath::Sqrt(dR);
    s->PropagateTo(zorig);
    return dR;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
Double_t GetdeltaRNoPropagation(EdbSegP* s,EdbSegP* stest)
{
    Log(4, "ShowRec.cpp", "--- Bool_t GetdeltaRNoPropagation() ---");
    return TMath::Sqrt((s->X()-stest->X())*(s->X()-stest->X())+(s->Y()-stest->Y())*(s->Y()-stest->Y()));
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
Double_t GetdeltaTheta(EdbSegP* s1,EdbSegP* s2)
{
    Log(4, "ShowRec.cpp", "--- Bool_t GetdeltaTheta() ---");

    Double_t tx1,tx2,ty1,ty2;
    tx1=s1->TX();
    tx2=s2->TX();
    ty1=s1->TY();
    ty2=s2->TY();
    //   Double_t dt= TMath::Sqrt(tx1*tx1+ty1*ty1) - TMath::Sqrt(tx2*tx2+ty2*ty2); // version which was used for all studies up to now...
    //Double_t dt= TMath::Sqrt( (tx1-tx2)*(tx1-tx2) + (ty1-ty2)*(ty1-ty2) ); // new version to test... => implemented in GetdeltaThetaSingleAngles
    Double_t dt= TMath::Abs(TMath::Sqrt(tx1*tx1+ty1*ty1) - TMath::Sqrt(tx2*tx2+ty2*ty2)); // version which was used for all studies up to now...  NOW mit abs()
    return dt;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
Double_t GetdeltaThetaSingleAngles(EdbSegP* s1,EdbSegP* s2)
{
    Log(4, "ShowRec.cpp", "--- Bool_t GetdeltaThetaSingleAngles() ---");

    Double_t tx1,tx2,ty1,ty2;
    tx1=s1->TX();
    tx2=s2->TX();
    ty1=s1->TY();
    ty2=s2->TY();
    //Double_t dt= TMath::Sqrt(tx1*tx1+ty1*ty1) - TMath::Sqrt(tx2*tx2+ty2*ty2); // version which was used for all studies up to now...
    Double_t dt= TMath::Sqrt( (tx1-tx2)*(tx1-tx2) + (ty1-ty2)*(ty1-ty2) ); // new version to test...
    return dt;
}
//-------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------
Double_t GetSpatialDist(EdbSegP* s1,EdbSegP* s2)
{
    Log(4, "ShowRec.cpp", "--- Bool_t GetSpatialDist() ---");
    // Mainly Z values should dominate... since the are at the order of 10k microns and x,y of 1k microns
    Double_t x1,x2,y1,y2,z1,z2;
    x1=s1->X();
    x2=s2->X();
    y1=s1->Y();
    y2=s2->Y();
    z1=s1->Z();
    z2=s2->Z();
    Double_t dist= TMath::Sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2)  );
    //cout << "dist = "  <<  dist << endl;
    return dist;
}
//-------------------------------------------------------------------------------------------



Int_t GetMinsBeforeAndAfter(Float_t& min_dT, Float_t& min_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter)
{
    //   cout << "GetMinsBeforeAndAfter( XX, " << patterloop_cnt <<  " , Seg,  " <<  n_patterns << ",  " << BeforeOrAfter << endl;
    min_dT=-1;
    min_dR=-1;

    float Z_minus1=0;
    float Z_normal=local_gAli->GetPattern(patterloop_cnt)->Z();
    float Z_plus1=0;

    int npat=local_gAli->Npatterns();
    Bool_t edge_npat_upper=kFALSE;
    Bool_t edge_npat_lower=kFALSE;
    Int_t Factor=-1;

    if (patterloop_cnt==npat-1) {
        edge_npat_upper=kTRUE;
    }
    if (patterloop_cnt==0) {
        edge_npat_lower=kTRUE;
    }

    if (!edge_npat_lower) {
        Z_minus1=local_gAli->GetPattern(patterloop_cnt-1)->Z();
        if (Z_minus1<Z_normal) Factor=1;
    }
    if (!edge_npat_upper) {
        Z_plus1 =local_gAli->GetPattern(patterloop_cnt+1)->Z();
        if (Z_plus1>Z_normal) Factor=1;
    }
    // New PID we want to have:
    Int_t patterloop_test=patterloop_cnt+Factor*n_patterns*BeforeOrAfter;

    // Does this plate exist? If not, return 0 directly:
    if (patterloop_test>=npat || patterloop_test<0) {
        //cout << "So NEW n_patterns would be " << patterloop_test << " BUT IT DOES NOT MATHC in our local_gAli sceheme, which means its not existing. RETURNING 0 " << endl;
        return 0;
    }

    // Since we have checked now for bounds we can FindCompliments:
    Int_t n_return=0;
    TObjArray array;
    array.Clear();
    EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_test);
    TestPattern               ->  FillCell(20,20,0.01,0.01);
    n_return = TestPattern->FindCompliments(*seg,array,3,3);
    //cout << " Found  " << n_return  << "  compliments in 2,2 sigma area:" << endl;

    if (n_return==0) return n_return;

    if (n_return==1) {
        EdbSegP* s_of_array=(EdbSegP*)array.At(0);
        min_dT=GetdeltaThetaSingleAngles(seg,s_of_array);
        min_dR=GetdeltaRNoPropagation(seg,s_of_array);
    }


    Float_t tmp_min_dT=-1;
    Float_t tmp_min_dR=-1;
    Float_t tmp2_min_dT=-1;
    Float_t tmp2_min_dR=-1;
    Float_t angle;
    Float_t dist;

    if (n_return>1) {
        for (int i=0; i<n_return; i++) {
            EdbSegP* s_of_array=(EdbSegP*)array.At(i);
            if (i==0) {
                min_dT=999999;
                min_dR=9999999;
            }
            angle=(Float_t)GetdeltaThetaSingleAngles(seg,s_of_array);
            tmp_min_dT=min_dT;
            tmp2_min_dT=TMath::Min(angle, tmp_min_dT);
            min_dT=tmp2_min_dT;

            dist=(Float_t)GetdeltaRNoPropagation(seg,s_of_array);
            tmp_min_dR=min_dR;
            tmp2_min_dR=TMath::Min(dist, tmp_min_dR);
            min_dR=tmp2_min_dR;
        }
    }
    return n_return;
}
//-------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------
Int_t GetMeansBeforeAndAfter(Float_t& mean_dT, Float_t& mean_dR, EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter)
{
    //   cout << "GetMeansBeforeAndAfter( XX, " << patterloop_cnt <<  " , Seg,  " <<  n_patterns << ",  " << BeforeOrAfter << endl;
    mean_dT=-1;
    mean_dR=-1;

    float Z_minus1=0;
    float Z_normal=local_gAli->GetPattern(patterloop_cnt)->Z();
    float Z_plus1=0;

    int npat=local_gAli->Npatterns();
    Bool_t edge_npat_upper=kFALSE;
    Bool_t edge_npat_lower=kFALSE;
    Int_t Factor=-1;

    if (patterloop_cnt==npat-1) {
        edge_npat_upper=kTRUE;
    }
    if (patterloop_cnt==0) {
        edge_npat_lower=kTRUE;
    }

    if (!edge_npat_lower) {
        Z_minus1=local_gAli->GetPattern(patterloop_cnt-1)->Z();
        if (Z_minus1<Z_normal) Factor=1;
    }
    if (!edge_npat_upper) {
        Z_plus1 =local_gAli->GetPattern(patterloop_cnt+1)->Z();
        if (Z_plus1>Z_normal) Factor=1;
    }
    // New PID we want to have:
    Int_t patterloop_test=patterloop_cnt+Factor*n_patterns*BeforeOrAfter;

    // Does this plate exist? If not, return 0 directly:
    if (patterloop_test>=npat || patterloop_test<0) {
        //cout << "So NEW n_patterns would be " << patterloop_test << " BUT IT DOES NOT MATHC in our local_gAli sceheme, which means its not existing. RETURNING 0 " << endl;
        return 0;
    }

    // Since we have checked now for bounds we can FindCompliments:
    Int_t n_return=0;
    TObjArray array;
    array.Clear();
    EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_test);
    TestPattern               ->  FillCell(20,20,0.01,0.01);
    n_return = TestPattern->FindCompliments(*seg,array,3,3);
    //cout << " Found  " << n_return  << "  compliments in 2,2 sigma area:" << endl;

    if (n_return==0) return n_return;

    //seg->PrintNice();
    for (int i=0; i<n_return; i++) {
        EdbSegP* s_of_array=(EdbSegP*)array.At(i);
        if (i==0) {
            mean_dT=0;
            mean_dR=0;
        }
        //s_of_array->PrintNice();
        mean_dT+=GetdeltaThetaSingleAngles(seg,s_of_array);
        mean_dR+=GetdeltaRNoPropagation(seg,s_of_array);
    }
    if (n_return>0) mean_dT=mean_dT/(Double_t)n_return;
    if (n_return>0) mean_dR=mean_dR/(Double_t)n_return;

    //   cout << " mean_dT  = " <<  mean_dT  << endl;
    //   cout << " mean_dR  = " <<  mean_dR  << endl;

    // „Hab ich mich aus einfachsten Verhältnissen emporgequält, um dann zu bitten?“
    return n_return;
}
//-------------------------------------------------------------------------------------------







//-------------------------------------------------------------------------------------------
Int_t GetNSegBeforeAndAfter(EdbPVRec* local_gAli, Int_t patterloop_cnt, EdbSegP* seg, Int_t n_patterns, Int_t BeforeOrAfter)
{
    //   cout << "GetNSegBeforeAndAfter( XX, " << patterloop_cnt <<  " , Seg,  " <<  n_patterns << ",  " << BeforeOrAfter << endl;

    float Z_minus1=0;
    float Z_normal=local_gAli->GetPattern(patterloop_cnt)->Z();
    float Z_plus1=0;

    int npat=local_gAli->Npatterns();
    Bool_t edge_npat_upper=kFALSE;
    Bool_t edge_npat_lower=kFALSE;
    Int_t Factor=-1;

    if (patterloop_cnt==npat-1) {
        edge_npat_upper=kTRUE;
    }
    if (patterloop_cnt==0) {
        edge_npat_lower=kTRUE;
    }

    if (!edge_npat_lower) {
        Z_minus1=local_gAli->GetPattern(patterloop_cnt-1)->Z();
        //     cout << "WHAT IS GREATER?   Z_normal  Z_minus1  " << Z_normal << "  " << Z_minus1 << endl;
        //Factor=(Int_t)TMath::Sign(Z_normal,Z_minus1);
        if (Z_minus1<Z_normal) Factor=1;
    }
    if (!edge_npat_upper) {
        Z_plus1 =local_gAli->GetPattern(patterloop_cnt+1)->Z();
        //     cout << "WHAT IS GREATER?   Z_normal  Z_plus1  " << Z_normal << "  " << Z_plus1 << endl;
        if (Z_plus1>Z_normal) Factor=1;
    }

    //   cout << Z_minus1 << endl;
    //   cout << Z_normal << endl;
    //   cout << Z_plus1 << endl;
    //   cout << "Is edge_npat_lower = " << edge_npat_lower << endl;
    //   cout << "Is edge_npat_upper = " << edge_npat_upper << endl;
    //   cout << "Factor =  " << Factor << endl;

    // New PID we want to have:
    Int_t patterloop_test=patterloop_cnt+Factor*n_patterns*BeforeOrAfter;
    //   cout << "So NEW n_patterns would be " << patterloop_test << endl;

    // Does this plate exist? If not, return 0 directly:
    if (patterloop_test>=npat || patterloop_test<0) {
        //cout << "So NEW n_patterns would be " << patterloop_test << " BUT IT DOES NOT MATHC in our local_gAli sceheme, which means its not existing. RETURNING 0 " << endl;
        return 0;
    }

    // Since we have checked now for bounds we can FindCompliments:
    TObjArray array;
    array.Clear();
    EdbPattern* TestPattern= (EdbPattern*)local_gAli->GetPattern(patterloop_test);
    TestPattern               ->  FillCell(20,20,0.01,0.01);
    int n_return = TestPattern->FindCompliments(*seg,array,3,3);
    //cout << " Found  " << n_return  << "  compliments in 2,2 sigma area:" << endl;

    return n_return;
}
//-------------------------------------------------------------------------------------------


























//-------------------------------------------------------------------------------------------
void CalcEffPurOfShower(TObjArray* arr, Int_t &NBT, Int_t &NBTMC, Int_t &NBTallMC, Int_t &NBTeMC, Double_t &purall, Double_t  &pure)
{
    Log(3, "ShowRec.cpp", "--- void CalcEffPurOfShower() ---");
    EdbSegP* seg;
    for (int i=0; i<arr->GetEntries(); ++i) {
        seg=(EdbSegP*)arr->At(i);
        ++NBT;
        if (seg->MCEvt()>0) {
            ++NBTMC;     //NBTMC is kept for backward compability
            ++NBTallMC;
        }
        if (seg->MCEvt()>0 && TMath::Abs(seg->Flag())==11) {
            ++NBTeMC;
        }
    }

    purall=-1;
    pure=-1;
    if (NBT!=0) purall=(Double_t)NBTallMC/(Double_t)NBT;
    if (NBT!=0) pure=(Double_t)NBTeMC/(Double_t)NBT;

    if (gEDBDEBUGLEVEL>2) {
        cout << "CalcEffPurOfShower---------------- NBT,NBTallMC,NBTeMC,purall,pure=  "<< NBT <<" " <<  NBTallMC <<" " << NBTeMC <<" " << purall <<" " << pure <<" " <<endl;
    }

    GLOBAL_NBT=NBT;
    GLOBAL_NBTallMC=NBTallMC;
    GLOBAL_NBTMC=NBTallMC; // kept for backward compability
    GLOBAL_NBTeMC=NBTeMC;
    GLOBAL_purall=purall;
    GLOBAL_pure=pure;
    return;
}
//-------------------------------------------------------------------------------------------
void CalcEffPurOfShower2(TObjArray* arr, Int_t &NBT, Int_t &NBTMC, Int_t &NBTallMC, Int_t &NBTeMC, Double_t &purall, Double_t  &pure,Int_t NBT_Neff,Int_t NBTMC_Neff,Int_t NBTMCe_Neff)
{
    Log(3, "ShowRec.cpp", "--- void CalcEffPurOfShower2() ---");
    EdbSegP* seg;

    for (int i=0; i<arr->GetEntries(); ++i) {
        seg=(EdbSegP*)arr->At(i);
        ++NBT;
        if (seg->MCEvt()>0) {
            ++NBTMC;     //NBTMC is kept for backward compability
            ++NBTallMC;
        }
        if (seg->MCEvt()>0 && TMath::Abs(seg->Flag())==11) {
            ++NBTeMC;
        }
    }

    purall=-1;
    pure=-1;
    if (NBT!=0) purall=(Double_t)NBTallMC/(Double_t)NBT;
    if (NBT!=0) pure=(Double_t)NBTeMC/(Double_t)NBT;

    // eff_all = NBTMC_SHOWER/NBTMC_VOLUME
    // eff_e = NBTMCe_SHOWER/NBTMCe_VOLUME

    Double_t effall=0;
    if (NBTMC_Neff!=0) effall = (Double_t)NBTMC/(Double_t)NBTMC_Neff;
    Double_t effe=0;
    if (NBTMC_Neff!=0) effe  = (Double_t)NBTeMC/(Double_t)NBTMCe_Neff;

    if (gEDBDEBUGLEVEL>2) {
        cout << "CalcEffPurOfShower2---------------- NBT,NBTallMC,NBTeMC,purall,pure, effall, effe=  "<< NBT <<" " <<  NBTallMC;
        cout <<" " << NBTeMC <<" " << purall <<" " << pure <<" " << effall <<" " << effe <<" " << endl;
    }

    GLOBAL_NBT=NBT;
    GLOBAL_NBTallMC=NBTallMC;
    GLOBAL_NBTMC=NBTallMC; // kept for backward compability
    GLOBAL_NBTeMC=NBTeMC;
    GLOBAL_purall=purall;
    GLOBAL_pure=pure;
    GLOBAL_effall=effall;
    GLOBAL_effe=effe;

    Log(3, "ShowRec.cpp", "--- void CalcEffPurOfShower2() done.");
    return;
}



//-------------------------------------------------------------------------------------------
void FillOutPutStructures()
{
    if (cmd_OUTPUTLEVEL==1) return;

    Log(2, "ShowRec.cpp", "--- void FillOutPutStructures() ---");

    STREAM_ShowRecEff.open(STREAM_ShowRecEffName,ios::app);    // Fill Later, Open now :-)
    InitCutParameters();

    Int_t PARASETNR,INBTSHOWERNR;
    Double_t EvtBT_E,EvtBT_TanTheta;
    Int_t EvtBT_Flag,EvtBT_MC;
    Double_t InBT_E,InBT_TanTheta;
    Int_t InBT_Flag,InBT_MC;
    Int_t NBT,NBTMC,NBTallMC,NBTeMC;
    Double_t purall,pure;
    Double_t effall,effe;
    Double_t trckdens;

    TREE_ShowRecEff->SetBranchAddress("PARASETNR", &PARASETNR);
    TREE_ShowRecEff->SetBranchAddress("ShowerNr", &INBTSHOWERNR);

    TREE_ShowRecEff->SetBranchAddress("EvtBT_E", &EvtBT_E);
    TREE_ShowRecEff->SetBranchAddress("EvtBT_TanTheta", &EvtBT_TanTheta);
    TREE_ShowRecEff->SetBranchAddress("EvtBT_Flag", &EvtBT_Flag);
    TREE_ShowRecEff->SetBranchAddress("EvtBT_MC", &EvtBT_MC);

    TREE_ShowRecEff->SetBranchAddress("InBT_E", &InBT_E);
    TREE_ShowRecEff->SetBranchAddress("InBT_TanTheta", &InBT_TanTheta);
    TREE_ShowRecEff->SetBranchAddress("InBT_Flag", &InBT_Flag);
    TREE_ShowRecEff->SetBranchAddress("InBT_MC", &InBT_MC);

    TREE_ShowRecEff->SetBranchAddress("NBT", &NBT);
    TREE_ShowRecEff->SetBranchAddress("NBTMC", &NBTMC);
    TREE_ShowRecEff->SetBranchAddress("NBTallMC", &NBTallMC);
    TREE_ShowRecEff->SetBranchAddress("NBTeMC", &NBTeMC);
    TREE_ShowRecEff->SetBranchAddress("purall", &purall);
    TREE_ShowRecEff->SetBranchAddress("pure", &pure);

    TREE_ShowRecEff->SetBranchAddress("effall", &effall);
    TREE_ShowRecEff->SetBranchAddress("effe", &effe);
    TREE_ShowRecEff->SetBranchAddress("trckdens", &trckdens);

    for (int i=0; i<TREE_ShowRecEff->GetEntries(); ++i) {

        if (gEDBDEBUGLEVEL>3) TREE_ShowRecEff->Show(i);
        TREE_ShowRecEff->GetEntry(i);

        STREAM_ShowRecEff <<  setw(10) << cmd_PADI;
        STREAM_ShowRecEff <<  setw(10) << cmd_BTPA;
        STREAM_ShowRecEff <<  setw(10) << cmd_BGTP;
        STREAM_ShowRecEff <<  setw(10) << cmd_ALTP;
        STREAM_ShowRecEff <<  setw(10) << cmd_FP;
        STREAM_ShowRecEff <<  setw(10) << cmd_MP;
        STREAM_ShowRecEff <<  setw(10) << cmd_LP;
        STREAM_ShowRecEff <<  setw(10) << cmd_NP;

        STREAM_ShowRecEff <<  setw(10) << cmd_LT;
        STREAM_ShowRecEff <<  setw(10) << cmd_MC;

        STREAM_ShowRecEff <<  setw(14) << PARASETNR;
        STREAM_ShowRecEff <<  setw(14) << INBTSHOWERNR;

        STREAM_ShowRecEff <<  setw(10) << EvtBT_E;
        STREAM_ShowRecEff <<  setw(14) << EvtBT_TanTheta;
        STREAM_ShowRecEff <<  setw(14) << EvtBT_Flag;
        STREAM_ShowRecEff <<  setw(10) << EvtBT_MC;

        STREAM_ShowRecEff <<  setw(10) << InBT_E;
        STREAM_ShowRecEff <<  setw(14) << InBT_TanTheta;
        STREAM_ShowRecEff <<  setw(14) << InBT_Flag;
        STREAM_ShowRecEff <<  setw(10) << InBT_MC;

        STREAM_ShowRecEff <<  setw(10) << NBT;
        STREAM_ShowRecEff <<  setw(10) << NBTallMC;
        STREAM_ShowRecEff <<  setw(10) << NBTeMC;
        STREAM_ShowRecEff <<  setw(10) << purall;
        STREAM_ShowRecEff <<  setw(10) << pure;
        STREAM_ShowRecEff <<  setw(10) << effall;
        STREAM_ShowRecEff <<  setw(10) << effe;
        STREAM_ShowRecEff <<  setw(10) << trckdens;
        STREAM_ShowRecEff << endl;

        // ---------
        NBTeMC_pure->Fill(pure,NBTeMC);
        NBTallMC_purall->Fill(purall,NBTallMC);

        NBTeMC_NBTMC->Fill(NBTallMC,NBTeMC);
        NBTeMC_NBT->Fill(NBT,NBTeMC);

        NBT_InBTE->Fill(InBT_E/1000.0,NBT);
        NBTeMC_InBTE->Fill(InBT_E/1000.0,NBTeMC);

        pure_InBTE->Fill(InBT_E/1000.0,pure);
        purall_InBTE->Fill(InBT_E/1000.0,purall);
        // ---------
        Hist_NBTeMC_pure->Fill(pure,NBTeMC);
        Hist_NBTallMC_purall->Fill(purall,NBTallMC);

        Hist_NBTeMC_NBTMC->Fill(NBTallMC,NBTeMC);
        Hist_NBTeMC_NBT->Fill(NBT,NBTeMC);

        Hist_NBT_InBTE->Fill(InBT_E/1000.0,NBT);
        Hist_NBTeMC_InBTE->Fill(InBT_E/1000.0,NBTeMC);

        Hist_pure_InBTE->Fill(InBT_E/1000.0,pure);
        Hist_purall_InBTE->Fill(InBT_E/1000.0,purall);

    }

    FILE_ShowRecEff->cd();
    TREE_ShowRecEff->Write();

    FILE_ShowRecHistos->cd();
    TCanvas* ShowRecEffPlots= new TCanvas("ShowRecEffPlots","ShowRecEffPlots",1200,1200);
    ShowRecEffPlots->Divide(2,4);
    ShowRecEffPlots->cd(1);
    NBTeMC_pure->Draw();
    ShowRecEffPlots->cd(2);
    NBTallMC_purall->Draw();
    ShowRecEffPlots->cd(3);
    NBTeMC_NBTMC->Draw();
    ShowRecEffPlots->cd(4);
    NBTeMC_NBT->Draw();
    ShowRecEffPlots->cd(5);
    NBT_InBTE->Draw();
    ShowRecEffPlots->cd(6);
    NBTeMC_InBTE->Draw();
    ShowRecEffPlots->cd(7);
    pure_InBTE->Draw();
    ShowRecEffPlots->cd(7);
    purall_InBTE->Draw();
    TCanvas* ShowRecEffPlots2= new TCanvas("ShowRecEffPlots2","ShowRecEffPlots2",1200,1200);
    ShowRecEffPlots2->Divide(2,4);
    ShowRecEffPlots2->cd(1);
    Hist_NBTeMC_pure->Draw("colz");
    ShowRecEffPlots2->cd(2);
    Hist_NBTallMC_purall->Draw("colz");
    ShowRecEffPlots2->cd(3);
    Hist_NBTeMC_NBTMC->Draw("colz");
    ShowRecEffPlots2->cd(4);
    Hist_NBTeMC_NBT->Draw("colz");
    ShowRecEffPlots2->cd(5);
    Hist_NBT_InBTE->Draw("colz");
    ShowRecEffPlots2->cd(6);
    Hist_NBTeMC_InBTE->Draw("colz");
    ShowRecEffPlots2->cd(7);
    Hist_pure_InBTE->Draw("colz");
    ShowRecEffPlots2->cd(8);
    Hist_purall_InBTE->Draw("colz");

    NBTeMC_pure->Write();
    NBTallMC_purall->Write();
    NBTeMC_NBTMC->Write();
    NBTeMC_NBT->Write();
    NBT_InBTE->Write();
    NBTeMC_InBTE->Write();
    Hist_NBTeMC_pure->Write();
    Hist_NBTallMC_purall->Write();
    Hist_NBTeMC_NBTMC->Write();
    Hist_NBTeMC_NBT->Write();
    Hist_NBT_InBTE->Write();
    Hist_NBTeMC_InBTE->Write();
    Hist_pure_InBTE->Write();
    Hist_purall_InBTE->Write();

    ShowRecEffPlots->Write();
    ShowRecEffPlots2->Write();

    STREAM_ShowRecEff.close();
    FILE_ShowRecEff->Close();

    delete ShowRecEffPlots;
    delete ShowRecEffPlots2;
    return;
}
//-------------------------------------------------------------------------------------------










//-------------------------------------------------------------------------------------------
TTree* CreateTreeBranchShowerTree(Int_t ParaSetNr)
{
    Log(2, "ShowRec.cpp", "--- TTree* CreateTreeBranchShowerTree() ---");


    // ParasetNr == -1 (no paraset from the paradefinition.root file is given and the
    // standard built in parasets are used: "treebranch" instead of "treebranch_-1"
    // ParasetNr != -1 ( paraset from the paradefinition.root file is given)

    TString treenname;
    if (ParaSetNr==-1) {
        treenname="treebranch";
    }
    else {
        treenname=TString(Form("treebranch_%d",ParaSetNr));
    }

    TTree* eShowerTree = new TTree(treenname,treenname);
    eShowerTree->Branch("number_eventb",&shower_number_eventb,"number_eventb/I");
    eShowerTree->Branch("sizeb",&shower_sizeb,"sizeb/I");
    eShowerTree->Branch("sizeb15",&shower_sizeb15,"sizeb15/I");
    eShowerTree->Branch("sizeb20",&shower_sizeb20,"sizeb20/I");
    eShowerTree->Branch("sizeb30",&shower_sizeb30,"sizeb30/I");
    eShowerTree->Branch("isizeb",&shower_isizeb,"isizeb/I");
    eShowerTree->Branch("xb",shower_xb,"xb[sizeb]/F");
    eShowerTree->Branch("yb",shower_yb,"yb[sizeb]/F");
    eShowerTree->Branch("zb",shower_zb,"zb[sizeb]/F");
    eShowerTree->Branch("txb",shower_txb,"txb[sizeb]/F");
    eShowerTree->Branch("tyb",shower_tyb,"tyb[sizeb]/F");
    eShowerTree->Branch("nfilmb",shower_nfilmb,"nfilmb[sizeb]/I");
    eShowerTree->Branch("ntrace1simub",shower_ntrace1simub,"ntrace1simu[sizeb]/I",128000);  // s.eMCEvt
    eShowerTree->Branch("ntrace2simub",shower_ntrace2simub,"ntrace2simu[sizeb]/I",128000); // s.eW
    eShowerTree->Branch("ntrace3simub",shower_ntrace3simub,"ntrace3simu[sizeb]/F",128000); // s.eP
    eShowerTree->Branch("ntrace4simub",shower_ntrace4simub,"ntrace4simu[sizeb]/I",128000); // s.eFlag
    eShowerTree->Branch("chi2btkb",shower_chi2btkb,"chi2btkb[sizeb]/F");
    eShowerTree->Branch("deltarb",shower_deltarb,"deltarb[sizeb]/F");
    eShowerTree->Branch("deltathetab",shower_deltathetab,"deltathetab[sizeb]/F");
    eShowerTree->Branch("deltaxb",shower_deltaxb,"deltaxb[sizeb]/F");
    eShowerTree->Branch("deltayb",shower_deltayb,"deltayb[sizeb]/F");
    eShowerTree->Branch("tagprimary",shower_tagprimary,"tagprimary[sizeb]/F");
    eShowerTree->Branch("energy_shot_particle",&shower_energy_shot_particle,"energy_shot_particle/F");
    eShowerTree->Branch("E_MC",&shower_energy_shot_particle,"E_MC/F");
    eShowerTree->Branch("showerID",&shower_showerID,"showerID/I");
    eShowerTree->Branch("idb",shower_idb,"idb/I");
    eShowerTree->Branch("plateb",shower_plateb,"plateb[sizeb]/I");
    eShowerTree->Branch("deltasigmathetab",shower_deltasigmathetab,"deltasigmathetab[59]/F");
    eShowerTree->Branch("lengthfilmb",&shower_numberofilms,"lengthfilmb/I",128000);
    eShowerTree->Branch("purityb",&shower_purb,"purityb/F",128000); // shower purity
    eShowerTree->Branch("trackdensb",&shower_trackdensb,"trackdensb/F",128000); // track density _around_ the shower (not _in_ shower)
    eShowerTree->Branch("nholesb",&shower_numberofholes,"nholesb/I",128000); // #of (single) empty plates
    eShowerTree->Branch("nholesmaxb",&shower_numberofholesconseq,"nholesmaxb/I",128000); // #of (consecutive) empty plates

    eShowerTree->Branch("axis_xb",&shower_axis_xb,"shower_axis_xb/F"); // Shower Axis Values...
    eShowerTree->Branch("axis_yb",&shower_axis_yb,"shower_axis_yb/F");
    eShowerTree->Branch("axis_zb",&shower_axis_zb,"shower_axis_zb/F");
    eShowerTree->Branch("axis_txb",&shower_axis_txb,"shower_axis_txb/F");
    eShowerTree->Branch("axis_tyb",&shower_axis_tyb,"shower_axis_tyb/F");

    // distuingish variable for more than one kind of showers merged into treebranch
    eShowerTree->Branch("filetype",&shower_filetype,"filetype/I");

    eShowerTree->SetDirectory(FILE_ShowShower);

    if (gEDBDEBUGLEVEL>2) {
        cout << "--- CreateTreeBranchShowerTree: eShowerTree: Name, Entries:"<<endl;
        cout << eShowerTree->GetName() << "    " << eShowerTree->GetEntries() <<endl;
        cout << "------eShowerTree-----------------------------------------"<<endl;
    }
    return eShowerTree;
}
//-------------------------------------------------------------------------------------------




//-------------------------------------------------------------------------------------------
void TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree(TTree* treebranchtree, TObjArray* segarray)
{
    Log(3, "ShowRec.cpp", "--- void* TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree() ---");

    // SaveCheck if shower has at least one basetrack:
    if (segarray->GetEntries()<1) return;

    EdbSegP* seg;
    EdbSegP* Inseg;
    Int_t helper_nfilmb;
    Int_t diff_pid;
    Float_t min_shower_deltathetab=99999;
    Float_t min_shower_deltar=99999;
    Float_t test_shower_deltathetab=99999;
    Float_t test_shower_deltar=99999;
    Float_t test_shower_deltax,test_shower_deltay;
    Int_t max_diff_pid=0;


    Float_t shower_sizebNHELP=0;
    Float_t shower_sizebMCNHELP=0;

    Float_t extrapol_x,extrapol_y, extrapo_diffz;

    // Initialize arrays...
    shower_sizeb15=0;
    shower_sizeb20=0;
    shower_sizeb30=0;
    shower_sizeb=0;
    shower_energy_shot_particle=0.0;
    shower_numberofilms=0;
    shower_numberofholesconseq=0;
    shower_numberofholes=0;
    shower_filetype=0;
    for (int ii=0; ii<5000; ii++)  {
        shower_xb[ii]=0;
        shower_yb[ii]=0;
        shower_zb[ii]=0;
        shower_txb[ii]=0;
        shower_tyb[ii]=0;
        shower_nfilmb[ii]=0;
        shower_tagprimary[ii]=0;
        shower_ntrace1simub[ii]=0;
        shower_ntrace2simub[ii]=0;
        shower_ntrace3simub[ii]=0;
        shower_ntrace4simub[ii]=0;
        shower_deltaxb[ii]=0;
        shower_deltayb[ii]=0;
        shower_chi2btkb[ii]=0;
        shower_idb[ii]=0;
        shower_plateb[ii]=0;
    }
    for (int i=1; i<58; ++i) {
        shower_deltasigmathetab[i]=0;
    }

    // Part To calculate the TransfereedVariables....
    shower_sizeb=segarray->GetEntries();
    Inseg=(EdbSegP*)segarray->At(0);
    shower_energy_shot_particle=Inseg->P();
    shower_number_eventb=Inseg->MCEvt();
    shower_filetype=cmd_FILETP;


    if (gEDBDEBUGLEVEL>3) cout << "--- --- ---------------------"<<endl;
    //-------------------------------------
    for (int ii=0; ii<shower_sizeb; ii++)  {

        if (ii>=5000) {
            cout << "WARNING: shower_sizeb ( " << shower_sizeb<< ") greater than SHOWERARRAY.   Set sizeb to 4999 and  Stop filling!."<<endl;
            shower_sizeb=4999;
            continue;
        }
        seg=(EdbSegP*)segarray->At(ii);

        //-------------------------------------
        shower_xb[ii]=seg->X();
        shower_yb[ii]=seg->Y();
        shower_txb[ii]=seg->TX();
        shower_tyb[ii]=seg->TY();
        shower_zb[ii]=seg->Z();
        shower_chi2btkb[ii]=seg->Chi2();
        shower_deltathetab[ii]=0.5;
        shower_deltarb[ii]=200;
        shower_tagprimary[ii]=0;
        if (ii==0) shower_tagprimary[ii]=1;
        shower_isizeb=1; // always 1, not needed anymore
        if (seg->MCEvt()>0) {
            shower_ntrace1simub[ii]=seg->MCEvt();
            shower_ntrace2simub[ii]=seg->W();
            shower_ntrace3simub[ii]=seg->P();
            shower_ntrace4simub[ii]=seg->Flag();
        }
        else {
            // keep the seg->BT settings for BG:
            // that come out of "normal" scanned data from fedra:
            // shower_ntrace1simub=-999
            // shower_ntrace2simub=seg->W();
            // shower_ntrace3simub=-999
            // shower_ntrace4simub=0
            shower_ntrace1simub[ii]=-999;
            shower_ntrace2simub[ii]=seg->W();
            shower_ntrace3simub[ii]=-999;
            shower_ntrace4simub[ii]=0;
        }
        shower_idb[ii]=seg->ID();
        shower_plateb[ii]=seg->PID();


        //-------------------------------------
        // PUT HERE:  deltarb,deltarb, nflimb, sizeb15......
        diff_pid=TMath::Abs( Inseg->PID()-seg->PID() )+1;
        // (does this work for up/downsream listing??)
        // (yes, since InBT->PID is also changed.)
        // but works only if the gAli Object has no missing plates
        // otherwise f.e. PID(1) and PID(2) are not necessaryly abay by dZ=1300
        // (could be Z(1)=1300 and Z(2)=3900...)

        // Calc pur:
        // New: (16.02.2010) define purity w.r.t. MC of Initiabtor Basetrack
        // So if other MC-events (like in testbeam simulation case) had been taken, they
        // count as well as "background!"
        shower_sizebNHELP++;
        if (seg->MCEvt()==shower_number_eventb&&shower_number_eventb>0) shower_sizebMCNHELP++;
        // for example: InBT:MCEvt==4 and Basetrack has MCEvt==18 then
        // shower_sizebNHELP++, but not shower_sizebMCNHELP !!
        // But also: if shower_number_eventb==-999 i.e. we start from a BG basetrack
        // and all other shower collected BTs have MCEvt==-999 then we get also
        // a purity of 1.
        // That means shower consisting only of BG events are in that sense also "very pure".
        // How to deal with this?
        // if (shower_number_eventb=-999) then we do NOT increment
        // shower_sizebMCNHELP , this is by changing the statement from
        // if (seg->MCEvt()==shower_number_eventb) shower_sizebMCNHELP++;
        // to
        // if (seg->MCEvt()==shower_number_eventb&&shower_number_eventb>0) shower_sizebMCNHELP++;

        // InBT:
        if (ii==0) {
            shower_deltathetab[0]=0.5;
            shower_deltarb[0]=200;
            shower_nfilmb[0]=1;
        }
        // All other BTs:
        if (ii>0) {
            // its correct like this, since this is the way it is done in
            // the official FJ-Algorithm:
            shower_nfilmb[ii]=diff_pid;
            if (gEDBDEBUGLEVEL>3) cout << "--- ---Inseg->PID() seg->PID() ii diif_pid shower_nfilmb[ii]  " << Inseg->PID()<< "   "  <<  seg->PID() << "   " << ii<< "  " << diff_pid<<"  "<< shower_nfilmb[ii]<<"  " << endl;

            shower_numberofilms=TMath::Max(shower_numberofilms,diff_pid);

            if (diff_pid >= 15 ) shower_sizeb15++;
            if (diff_pid >= 20 ) shower_sizeb20++;
            if (diff_pid >= 30 ) shower_sizeb30++;

            // PUT HERE:  calculation routine for shower_deltasigmathetab
            // see referenc in thesis of Luillo Esposito, page 109.
            shower_deltasigmathetab[diff_pid]=shower_deltasigmathetab[diff_pid]+(Power(shower_txb[ii]-shower_txb[0],2)+Power(shower_tyb[ii]-shower_tyb[0],2));

            // PUT HERE:  calculation routine for shower_deltathetab, shower_deltarb
            // ExSetTreebranchNametrapolate the BT [ii] to the position [jj] and then calc the
            // position and slope differences;
            // For the backward extrapolation of the   shower_deltathetab and shower_deltarb
            // calulation for BaseTrack(ii), Basetrack(jj)->Z() hast to be smaller.
            min_shower_deltathetab=99999;   // Reset
            min_shower_deltar=99999;        // Reset

            for (int jj=0; jj<shower_sizeb; jj++)  {
                if (ii==jj) continue;

                // since we do not know if BTs are ordered by their Z positions:
                // and cannot cut directly on the number in the shower entry:
                // Entry jj has to have lower Z than ii:
                if (shower_zb[ii]<=shower_zb[jj]) continue;

                extrapo_diffz=shower_zb[ii]-shower_zb[jj];
                if (TMath::Abs(extrapo_diffz)>4*1300+1.0) continue;
//       if (TMath::Abs(extrapo_diffz)>6*1300+1.0) continue;
//      if (TMath::Abs(extrapo_diffz)>9*1300+1.0) continue;
                // if 4/6/9 gives only similar results...
                if (TMath::Abs(extrapo_diffz)<1.0) continue; // remove same positions.

                extrapol_x=shower_xb[ii]-shower_txb[ii]*extrapo_diffz; // minus, because its ii after jj.
                extrapol_y=shower_yb[ii]-shower_tyb[ii]*extrapo_diffz; // minus, because its ii after jj.

                // Delta radius we need to extrapolate.
//       test_shower_deltax=extrapol_x;//shower_txb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_xb[ii];
//       test_shower_deltay=extrapol_y;//shower_tyb[ii]*(shower_zb[ii]-shower_zb[jj])+shower_yb[ii];
                test_shower_deltax=extrapol_x-shower_xb[jj];
                test_shower_deltay=extrapol_y-shower_yb[jj];
                test_shower_deltar=TMath::Sqrt(test_shower_deltax*test_shower_deltax+test_shower_deltay*test_shower_deltay);

                // Delta theta we do not need to extrapolate. (old version...)
                //test_shower_deltathetab=TMath::Sqrt(shower_txb[ii]*shower_txb[ii]+shower_tyb[ii]*shower_tyb[ii]);
                //test_shower_deltathetab=test_shower_deltathetab-TMath::Sqrt(shower_txb[jj]*shower_txb[jj]+shower_tyb[jj]*shower_tyb[jj]);
                //test_shower_deltathetab=TMath::Abs(test_shower_deltathetab);
                //----
                // As before in ShowRec this way of calculation is not equivalent as calculating
                // DeltaTheta domponentwise:
                // Code from libShower:
                // delta = sqrt((SX0-a->GetTXb(l2))*(SX0-a->GetTXb(l2))+((SY0-a->GetTYb(l2))*(SY0-a->GetTYb(l2))));
                test_shower_deltathetab=TMath::Sqrt(TMath::Power(shower_txb[ii]-shower_txb[jj],2)+TMath::Power(shower_tyb[ii]-shower_tyb[jj],2));

                // Check if both dr,dt match parameter criteria and then just take these values.....
                // Maybe a change is necessary because it is not exactly the same as in the off. algorithm.
//        if (test_shower_deltar<400 && test_shower_deltathetab<0.8 ) {
//        if (test_shower_deltar<150 && test_shower_deltathetab<0.15) {
                if (test_shower_deltar<1000 && test_shower_deltathetab<2.0 ) {
                    /// -----     IMPORTANT::  these areopen cut values for best combifinding of pair BT deltaR/Theta values
                    /// -----     IMPORTANT::  then you do NOT necessarily get back your values which you put in durign
                    /// -----     IMPORTANT::  your shower reconstruction cone ( deltaR/Theta cutvalues could be NO cutvalues
                    /// -----     IMPORTANT::  for some reconstruction algorithms for example, but we wanna have these values anyway.
                    ///  In Any Case:
                    ///  Frederics Cut looks only for best min_shower_deltar so we do also.
                    if (test_shower_deltar<min_shower_deltar) {
                        min_shower_deltathetab=test_shower_deltathetab;
                        min_shower_deltar=test_shower_deltar;
                        shower_deltathetab[ii]=min_shower_deltathetab;
                        shower_deltarb[ii]=min_shower_deltar;
                        //cout << ii << " " << jj << "  " << test_shower_deltathetab  << " " << test_shower_deltar << "       " <<min_shower_deltathetab << " " <<  min_shower_deltar  << endl;

                    }   // if (test_shower_deltar<min_shower_deltar)
                } // if (test_shower_deltar<150 && test_shower_deltathetab<0.15 )
            } // for (int jj=0;jj<shower_sizeb;jj++)


            //cout << "For ii= " << ii << "we found the best matcjgin dRtehta values: " << min_shower_deltathetab << " " <<  min_shower_deltar  << endl;
        } // if (ii>0)
        //-------------------------------------


        shower_purb=shower_sizebMCNHELP/shower_sizebNHELP;
    } //  for (int ii=0;ii<shower_sizeb;ii++)  {
    if (gEDBDEBUGLEVEL>2) {
        cout << "ShowRec.cpp     : --- void* TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree() ---"<<endl;
        cout << "Loop over for (int ii=0;ii<shower_sizeb;ii++) done." << endl;
    }

    //-------------------------------------
    for (int i=1; i<58; ++i) {
        shower_deltasigmathetab[i]=Sqrt(shower_deltasigmathetab[i]);
    }
    shower_numberofilms=shower_nfilmb[shower_sizeb-1]; // there we assume (this is correct always?) when
    // the last shower BT is in the last film...(otherwise we would again have to loop on sizeb array);
    //cout << "TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree   shower_numberofilms= "<< shower_numberofilms <<endl;

    int eN0=0;
    for (int i=0; i<shower_nfilmb[shower_sizeb-1]; i++) if (shower_nfilmb[i]==0) ++eN0;
    shower_numberofholes=eN0;

    int eN00int=0;
    int eN00=0;
    for (int i=1; i<shower_nfilmb[shower_sizeb-1]; i++) {
        //cout << i << "------------" << shower_nfilmb[i]-shower_nfilmb[i-1] << " --  " <<  eN00 <<  endl;
        if (shower_nfilmb[i]-shower_nfilmb[i-1]>1) {
            eN00=shower_nfilmb[i]-shower_nfilmb[i-1]-1;
            if (eN00>eN00int) eN00int=eN00;
        }
        if (shower_nfilmb[i]-shower_nfilmb[i-1]==1) eN00=0;
    }
    eN00=eN00int;
    shower_numberofholesconseq=eN00;

    //cout << "TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree   shower_numberofholesconseq= "<< shower_numberofholesconseq <<endl;

    // Also calculate now Shower axis and fill axis values into the treebranch:
//   EdbSegP* axis= new EdbSegP();
    EdbSegP* axis = BuildShowerAxis(segarray);
//   axis->PrintNice();
    //cout << "TransferShowerObjectArrayIntoEntryOfTreebranchShowerTree   BuildShowerAxis done."<<endl;
    shower_axis_xb=axis->X();
    shower_axis_yb=axis->Y();
    shower_axis_zb=axis->Z();
    shower_axis_txb=axis->TX();
    shower_axis_tyb=axis->TY();
//   delete axis;

    // Fill Tree:
    treebranchtree->Fill();

    if (gEDBDEBUGLEVEL>2) cout << "Now we have treebranchtree  Entries: "  << treebranchtree->GetEntries() << endl;
    if (gEDBDEBUGLEVEL>3) treebranchtree->Show(treebranchtree->GetEntries()-1);

    return;
}
//-------------------------------------------------------------------------------------------








//-------------------------------------------------------------------------------------------
void MakeTracksTree(TTree* treebranch)
{
    if (cmd_OUTPUTLEVEL<2) return;

    Log(2, "ShowRec.cpp", "--- void MakeTracksTree() ---");
    if (gEDBDEBUGLEVEL>3) cout <<"--- --- MakeTracksTree Part 2:"<<endl;

    //   cout << treebranch->GetName() <<  "           treebranch->GetName(); " << endl;

    EdbSegP *seg;
    EdbTrackP *track2;
    EdbPVRec *ali;
    ali = new EdbPVRec();
    EdbPattern *pat=0;
    char fname_e[128];
    float x,y,tx,ty,z;
    int w2;

    for (int i=0; i<treebranch->GetEntries(); ++i) {
        treebranch->GetEntry(i);
        track2 = new EdbTrackP();

        for (int j=0; j<shower_sizeb; ++j) {

            x=shower_xb[j];
            y=shower_yb[j];
            tx=shower_txb[j];
            ty=shower_tyb[j];
            z=shower_zb[j];
            w2=shower_ntrace2simub[j];

            seg = new EdbSegP();
            seg->Set(j, x, y, tx, ty, w2, 0);
            seg->SetZ(z);
            seg->SetDZ(300.);
            // DISPLAY PROBLEM IF RIGHT PID IS USED !!!!!!!!!!!!!!
            pat = ali->GetPattern( seg->PID() );
            if (!pat) {
                printf("WARNING: no pattern with pid %d: creating new one!\n",seg->PID());
                pat = new EdbPattern( 0., 0., seg->Z() );
                //pat->SetID(seg->PID());
                ali->AddPatternAt(pat,seg->PID());
                ali->AddPatternAt(pat,j);
            }
            pat->AddSegment(*seg);
            track2->AddSegment(seg);
        }
        track2->SetSegmentsTrack(i);
        track2->SetID(i);
        track2->SetNpl(shower_nfilmb[shower_sizeb-1]);
        track2->SetProb(1.);
        track2->SetFlag(10);
        track2->FitTrack();
        ali->AddTrack(track2);

        //     cout << "i = delete track2"<< i << endl;
        //     delete track2;track2=0;
    }
    if (gEDBDEBUGLEVEL>3) ali->Print();

    //   if (gEDBDEBUGLEVEL>3)
    //     cout <<"--- --- MakeTracksTree Part 2:"<<endl;

    if (!ali) return;
    TObjArray *trarr = ali->eTracks;
    if (!trarr) return;
    float xv;
    xv=ali->X();
    float yv;
    yv=ali->Y();

    //   if (gEDBDEBUGLEVEL>2)
    //     cout <<"--- Write tracks to file:  " << FILE_ShowTracks->GetName() <<endl;

    TString tracksname="tracks"+TString(treebranch->GetName());

    FILE_ShowTracks->cd();
    //   TFile::Open(FILE_ShowTracks->GetName(),"UPDATE");
    TTree *tracks= new TTree(tracksname,tracksname);
    tracks->SetDirectory(FILE_ShowTracks);
    if (gEDBDEBUGLEVEL>2) cout <<"--- tracks->SetDirectory(FILE_ShowTracks)"<<endl;


    EdbTrackP    *track = new EdbTrackP(8);
    EdbSegP      *tr = (EdbSegP*)track;
    TClonesArray *segments  = new TClonesArray("EdbSegP");
    TClonesArray *segmentsf  = new TClonesArray("EdbSegP");

    int   nseg,trid,npl,n0;
    float w=0.;

    tracks->Branch("trid",&trid,"trid/I");
    tracks->Branch("nseg",&nseg,"nseg/I");
    tracks->Branch("npl",&npl,"npl/I");
    tracks->Branch("n0",&n0,"n0/I");
    tracks->Branch("xv",&xv,"xv/F");
    tracks->Branch("yv",&yv,"yv/F");
    tracks->Branch("w",&w,"w/F");
    tracks->Branch("t.","EdbSegP",&tr,32000,99);
    tracks->Branch("s", &segments);
    //fitted segments is kept for linked_track.root compability
    tracks->Branch("sf",&segmentsf);

    int ntr = trarr->GetEntriesFast();

    if (gEDBDEBUGLEVEL>2) cout <<"--- ntr = trarr->GetEntriesFast(); " << ntr << "    "  << trarr->GetEntries() << endl;

    for (int itr=0; itr<ntr; itr++) {
        track = (EdbTrackP*)(trarr->At(itr));
        trid = track->ID();
        nseg = track->N();
        npl  = track->Npl();
        n0   = track->N0();
        tr = (EdbSegP*)track;
        segments->Clear("C");
        nseg = track->N();
        w    = track->Wgrains();
        EdbSegP *s=0,*sf=0;
        for (int is=0; is<nseg; is++) {
            s = track->GetSegment(is);
            if (s) new((*segments)[is])  EdbSegP( *s );
            sf = track->GetSegment(is);
            if (sf) new((*segmentsf)[is])  EdbSegP( *sf );
        }
        track->SetVid( 0, tracks->GetEntries() );  // put track counter in t.eVid[1]
        tracks->Fill();
        track->Clear();
    }
    tracks->Write();



    return;
}
//-------------------------------------------------------------------------------------------










//______________________________________________________________________________

EdbSegP* BuildShowerAxis(TObjArray* ShowerSegArray)
{
    Int_t eNBT= ShowerSegArray->GetEntries();
    if (eNBT==0) return NULL;
    if (eNBT==1) return (EdbSegP*)ShowerSegArray->At(0);
    Double_t eNBT_float=(Double_t)eNBT;
//   cout << "eNBT= " << eNBT << "  eNBT_float= " << eNBT_float << endl;

    // Code taken from EdbMath::LFIT3 to fit line to a collection of points in spce!
    //int EdbMath::LFIT3( float *X, float *Y, float *Z, float *W, int L,
    //float &X0, float &Y0, float &Z0, float &TX, float &TY, float &EX, float &EY )
    // Linar fit in 3-d case (microtrack-like)
    // Input: X,Y,Z - coords, W -weight - arrays of the lengh >=L
    // Note that X,Y,Z modified by function
    // Output: X0,Y0,Z0 - center of gravity of segment
    // TX,TY : tangents in respect to Z axis

    Float_t X0,Y0,Z0;
    Float_t TX,TY;
    Float_t EX,EY;
    // Float_t x[eNBT]; Float_t z[eNBT]; Float_t y[eNBT]; Float_t W[eNBT];
    // Compiled with -pedantic -Wall -W -Wstrict-prototypes this gives error message: Fehler: ISO-C++ verbietet Feld »x« variabler Länge
    // So we take it as fixed length:
    // As before we take the global maximum of 5k BT /shower.
    Float_t x[5000];
    Float_t z[5000];
    Float_t y[5000];
    Float_t W[5000];


    // SUM UP TANGES VALUES OF ALL BTS SO FAR TO GET tx and TY of SHOWERCENTER ???
    Float_t sumX=0;
    Float_t sumY=0;
    Float_t sumTX=0;
    Float_t sumTY=0;
    Float_t sumW=0;
//
    // The weighting of the segments is a crucial point here, since it influences
    // the fitting to the shower axis!
    int eNBTMAX=TMath::Min(5000,eNBT);
    for (int i=0; i<eNBTMAX; i++) {
        EdbSegP* TestBT = (EdbSegP*)ShowerSegArray->At(i);
        x[i]=TestBT->X();
        y[i]=TestBT->Y();
        z[i]=TestBT->Z();
        // Choose weighting to be one!
        W[i]=1;
        W[i]=1.0/(TMath::Abs(z[i]-z[0])/1300.0*TMath::Abs(z[i]-z[0])/1300.0+1);
        //   W[i]=1.0/TMath::Sqrt((TMath::Abs(z[i]-z[0])/1300.0*TMath::Abs(z[i]-z[0])/1300.0+1));
        if (TMath::Abs(z[i]-z[0])/1300.0>4) W[i]=0;
        // chose this,....could be that this will  lead to something different results....
        sumX+=W[i]*TestBT->X();
        sumY+=W[i]*TestBT->Y();
        sumTX+=W[i]*TestBT->TX();
        sumTY+=W[i]*TestBT->TY();
        sumW+=W[i];
    }
    //cout << " Set all others to zero...." << endl;
    if  (eNBT<5000) {
        for (int i=eNBT; i<5000; i++) {
            W[i]=0;
            x[i]=0;
            y[i]=0;
            z[i]=0;
        }
    }


    //cout << "BuildShowerAxis   Invoke fit function:" << endl;
    // Invoke fit function:
    EdbMath::LFIT3( x,y,z,W,eNBTMAX,X0,Y0,Z0,TX,TY,EX,EY);

    //cout <<"AfterLinefit: x=" << x << " y=" << y << " z=" << z << " W=" << W << " eNBTMAX=" << eNBTMAX << " X0=" << X0 << " Y0=" << Y0 << " Z0=" << Z0 << " TX=" << TX << " TY= " << TY << endl;

    sumTX=sumTX/sumW;
    sumTY=sumTY/sumW;
    sumX=sumTX/sumW;
    sumY=sumTY/sumW;
    ///  MNORMINERUNG !!!!
    /// STILL NOT AN IMPROVEMENT FOR GAMMAS.....

    //==C== In few cases "nan" can happen, then we put val to -1;
    if (TMath::IsNaN(X0)) {
        //cout << "EdbShowerP::BuildShowerAxis   WARNING! FIT DID NOT CONVVERGE, RETURN FIRST SEGMENT! " << endl;
        EdbSegP* seg0=(EdbSegP*)ShowerSegArray->At(0);
        EdbSegP* eShowerAxisCenterGravityBT = new EdbSegP(0,seg0->X(),seg0->Y(),seg0->TX(),seg0->TY(),0,0);
        eShowerAxisCenterGravityBT -> SetZ(((EdbSegP*)ShowerSegArray->At(0))->Z());
        //eShowerAxisCenterGravityBT->PrintNice();
        return eShowerAxisCenterGravityBT;
    }

    //cout << "EdbShowerP::BuildShowerAxis   Axis  X0,Y0,Z0, TX, TY " <<  X0 << " " << Y0 << " " << Z0 << " " << TX << " " << TY << " " << endl;
    //cout << "sumTX=  " << sumTX << "   sumTY=  " << sumTY <<  "   sumW=  " << sumW << endl;

    EdbSegP* eShowerAxisCenterGravityBT = 0;
    if (!eShowerAxisCenterGravityBT) eShowerAxisCenterGravityBT = new EdbSegP(-1,X0,Y0,sumTX,sumTY,0,0); // with linefit
//     if (!eShowerAxisCenterGravityBT) eShowerAxisCenterGravityBT = new EdbSegP(-1,sumX,sumY,sumTX,sumTY,0,0); // without linefit
    eShowerAxisCenterGravityBT -> SetZ(((EdbSegP*)ShowerSegArray->At(0))->Z());


    /// Differene NOW FOR DEBUG TESTS !!!!
    //   Bool_t UseOnlyFirstBT=kTRUE;
    Bool_t UseOnlyFirstBT=kFALSE;
    if (UseOnlyFirstBT) {
        cout << "Attention: UseOnlyFirstBT"<< endl;
        eShowerAxisCenterGravityBT -> SetX(((EdbSegP*)ShowerSegArray->At(0))->X());
        eShowerAxisCenterGravityBT -> SetY(((EdbSegP*)ShowerSegArray->At(0))->Y());
        eShowerAxisCenterGravityBT -> SetZ(((EdbSegP*)ShowerSegArray->At(0))->Z());
        eShowerAxisCenterGravityBT -> SetTX(((EdbSegP*)ShowerSegArray->At(0))->TX());
        eShowerAxisCenterGravityBT -> SetTY(((EdbSegP*)ShowerSegArray->At(0))->TY());
    }

    //eShowerAxisCenterGravityBT->Print();
    return eShowerAxisCenterGravityBT;
}



//______________________________________________________________________________

void CalcTrackDensity(EdbPattern* pat_interim,Float_t pat_interim_halfsize,Int_t& npat_int,Int_t& npat_total,Int_t& npatN)
{
    if (gEDBDEBUGLEVEL>3) cout << "-------------void CalcTrackDensity(&pat_interim,pat_interim_halfsize,&npat_int,&npat_total)"<<endl;
    npat_int=pat_interim->GetN();
    if (npat_int<=0) return;
    npat_total+=npat_int;
    ++npatN;
    if (npatN>0) shower_trackdensb=(Float_t)npat_total/(Float_t)npatN/local_halfpatternsize/local_halfpatternsize/4.0*1000.0*1000.0; // BT/mm2  // contains SG and BG tracks!

    if (gEDBDEBUGLEVEL>3) {
        cout << "pat_interim->Z() = " << pat_interim->Z() << endl;
        cout << "pat_interim->GetN() = " << pat_interim->GetN() << endl;
        cout << "npat_int = " << npat_int << endl;
        cout << "npat_total = " << npat_total << endl;
        cout << "npatN = " << npatN << endl;
        cout << "shower_trackdensb = " << shower_trackdensb << endl;
    }
    return;
}

//______________________________________________________________________________

void CalcEfficencyNumbers(EdbPattern* pat_interim, Int_t MCCheck, Int_t& NBT_Neff,Int_t& NBTMC_Neff,Int_t& NBTMCe_Neff)
{
    if (gEDBDEBUGLEVEL>3) cout << "-------------void CalcEfficencyNumbers()"<<endl;
    Int_t npat_int=pat_interim->GetN();
    if (npat_int<=0) return;

    for (int i=0; i<pat_interim->N(); ++i) {
        EdbSegP* seg=(EdbSegP* )pat_interim->GetSegment(i);
        if (seg->MCEvt()<0||seg->MCEvt()==MCCheck) ++NBT_Neff;
        if (seg->MCEvt()==MCCheck) ++NBTMC_Neff;
        if (seg->MCEvt()==MCCheck&&TMath::Abs(seg->Flag())==11) ++NBTMCe_Neff;
    }
    if (gEDBDEBUGLEVEL>3) {
        cout << "npat_int = " << npat_int << endl;
        cout << "NBT_Neff = " << NBT_Neff << endl;
        cout << "NBTMC_Neff = " << NBTMC_Neff << endl;
        cout << "NBTMCe_Neff = " << NBTMCe_Neff << endl;
    }
    return;
}


//______________________________________________________________________________

Bool_t IsShowerSortedZ(TObjArray* showerarray) {
    // Condition: z[0]<= z[1]<=....<=z[nseg]
    if (showerarray->GetEntries()<1) return kTRUE;
    EdbSegP* s0;
    EdbSegP* s1;
    for (int i=0; i<showerarray->GetEntries()-1; i++) {
        s0= (EdbSegP*)showerarray->At(i);
        s1= (EdbSegP*)showerarray->At(i+1);
        if (s0->Z()>s1->Z()) return kFALSE;
    }
    return kTRUE;
}

//______________________________________________________________________________

void SortShowerZ(TObjArray* showerarray) {

    // A simple reverting sort: assuming that segments in shower are already sorted, but in
    // descending direction, so we just invert them!
    // CANNOT BE USED WHEN segments wer put in an arbitrary way!!!

    if (IsShowerSortedZ(showerarray)) {
        if (gEDBDEBUGLEVEL>2) cout << "Shower already sorted in ascending Z-direction. Do nothing." << endl;
        return;
    }
    Int_t nent_showerarray=showerarray->GetEntries();
    Int_t nent=showerarray->GetEntries();
    TObjArray* interimShower = new TObjArray(nent_showerarray);
    if (gEDBDEBUGLEVEL>2) cout << "interif (gEDBDEBUGLEVEL>2)imShower->GetEntries()  " << interimShower->GetEntries()  << endl;
    if (gEDBDEBUGLEVEL>2) cout << "showerarray->GetEntries()  " << showerarray->GetEntries()  << endl;
    EdbSegP* s0;
    EdbSegP* s1;
    for (Int_t k=0; k<nent; ++k) {
        EdbSegP* s0=(EdbSegP*)showerarray->At(nent-k-1);
        interimShower->AddAt(s0,k);
    }
    showerarray->Clear();
    for (Int_t k=0; k<nent; ++k) {
        EdbSegP* s0=(EdbSegP*)interimShower->At(k);
        showerarray->AddAt(s0,k);
    }
    if (gEDBDEBUGLEVEL>2) PrintShowerObjectArray(showerarray);

    if (IsShowerSortedZ(showerarray)) {
        if (gEDBDEBUGLEVEL>2) cout << "Shower now sorted in ascending Z-direction. Done." << endl;
        return;
    }
    if (!IsShowerSortedZ(showerarray)) {
        cout << "WARNING   WARNING   Shower is still NOT sorted in ascending Z-direction. YOU HAVE TO CHECK MANUALLY." << endl;
        return;
    }
    return;
}


//______________________________________________________________________________

void BuildParametrizationsMCInfo_PGun(TString MCInfoFilename) {

    Log(2, "ShowRec.cpp", "--- void BuildParametrizationsMCInfo_PGun() ---");

    // Monte-Carlo Information on the event, take from the
    // pre-prepared root-file MCInfoFilename

    //Declare Tree Variables
    Int_t MCEvt, PDGId;
    Float_t energy, tantheta,dirx,diry,dirz,vtxposx,vtxposy,vtxposz;
    Float_t TX,TY,Y,X,Z;

    // Assume
    GLOBAL_IsBrickTreePGunInfo=kFALSE;

    // Read Tree with File:
    TTree* PGunTree = new TTree();
    Int_t ReadSuccess = PGunTree->ReadFile(MCInfoFilename,"MCEvt/I:energy/F:tantheta/F:dirx/F:diry/F:dirz/F:vtxposx/F:vtxposy/F:vtxposz/F:TX/F:TY/F:X/F:Y/F:Z/F:PDGId/I");

    // Check if File exists:
    cout << "BuildParametrizationsMCInfo_PGun ReadSuccess = PGunTree->ReadFile(MCInfoFilename) " <<  ReadSuccess  << endl;
    cout << "BuildParametrizationsMCInfo_PGun 0: File / Tree Reading was not successful " << endl;
    cout << "BuildParametrizationsMCInfo_PGun 1: File / Tree Reading was     successful " << endl;

    // If tree reading was not successful, we must return here.
    if (ReadSuccess==0) {
        cout << "BuildParametrizationsMCInfo_PGun() ReadSuccess==0. return. " << endl;
        return;
    }

    PGunTree->SetBranchAddress("MCEvt",&MCEvt);
    PGunTree->SetBranchAddress("PDGId",&PDGId);
    PGunTree->SetBranchAddress("energy",&energy);
    PGunTree->SetBranchAddress("tantheta",&tantheta);
    PGunTree->SetBranchAddress("dirx",&dirx);
    PGunTree->SetBranchAddress("diry",&diry);
    PGunTree->SetBranchAddress("dirz",&dirz);
    PGunTree->SetBranchAddress("vtxposx",&vtxposx);
    PGunTree->SetBranchAddress("vtxposy",&vtxposy);
    PGunTree->SetBranchAddress("vtxposz",&vtxposz);
    PGunTree->SetBranchAddress("TX",&TX);
    PGunTree->SetBranchAddress("TY",&TY);
    PGunTree->SetBranchAddress("X",&X);
    PGunTree->SetBranchAddress("Z",&Z);
    PGunTree->SetBranchAddress("Y",&Y);

    // If the PGunTree is not filled, this can be a hint that the
    // MCInfoFilename might not be there. This is important for alorithms that
    // rely on Vertex Informations, like the _GS() alg.
    if (PGunTree->GetEntries()==0) {
        cout << "BuildParametrizationsMCInfo_PGun ATTENTION!   PGunTree->GetEntries()==0" << endl;
        cout << "BuildParametrizationsMCInfo_PGun Bool_t    GLOBAL_IsBrickTreePGunInfo=kFALSE;" << endl;
        GLOBAL_IsBrickTreePGunInfo=kFALSE;
    }
    else {
        GLOBAL_IsBrickTreePGunInfo=kTRUE;
    }

    if (gEDBDEBUGLEVEL>2) PGunTree->Print();
    cout << "BuildParametrizationsMCInfo_PGun() PGunTree->GetEntries();    " <<  PGunTree->GetEntries() <<  endl;

    if (cmd_GBMC>0) {
        cout << " BuildParametrizationsMCInfo_PGun() cmd_GBMC>0 Show entry (cmd_GBMC):" << endl;
        PGunTree->Show(cmd_GBMC);
    }

    // --------------------------------
    // The number 69999 comes from my (frank) maximal number of mc Events
    // I have put in a simulation file.
    if (!GLOBAL_VtxArray) GLOBAL_VtxArray=new TObjArray(69999);
    GLOBAL_VtxArrayX[0]=0;
    GLOBAL_VtxArrayZ[0]=0;
    GLOBAL_VtxArrayY[0]=0;

    //------------------
    // PGunTreeEntry_MCEvt_Correspondance[0]=TreeEntry(0)->MC()
    Int_t PGunTreeEntry_MCEvt_Correspondance[69999];
    for (Int_t i=0; i<PGunTree->GetEntries(); ++i) {
        PGunTree->GetEntry(i);
        PGunTreeEntry_MCEvt_Correspondance[MCEvt]=i;
        EdbVertex* vtx=new EdbVertex();
        // Why are these numbers the way they are (2018_07_04) ???
        // I cannot remember anymore, unfortunately.
        vtx->SetXYZ(vtxposx*1000,vtxposy*1000,(vtxposz+40.0)*1000);
        vtx->SetMC(MCEvt);
        // vtx->Print(); // Gives Crash!
        GLOBAL_VtxArray->Add(vtx);
        GLOBAL_VtxArrayX[MCEvt]=vtxposx*1000;
        GLOBAL_VtxArrayY[MCEvt]=vtxposy*1000;
        GLOBAL_VtxArrayZ[MCEvt]=(vtxposz+40.0)*1000;
//         vtx->Print();
// gSystem->Exit(1);
        GLOBAL_EvtBT_EArray[MCEvt]=energy;
        GLOBAL_EvtBT_TanThetaArray[MCEvt]=tantheta;
        GLOBAL_EvtBT_FlagArray[MCEvt]=0;
        GLOBAL_EvtBT_MCArray[MCEvt]=MCEvt;
        GLOBAL_EvtBT_ZArray[MCEvt]=(vtxposz+40.0)*1000;
    }
    //------------------
    cout << "BuildParametrizationsMCInfo_PGun.... done." << endl;
    return;
}

//______________________________________________________________________________

void Fill2GlobalInBTArray() {

    // if we have a vertex file we can cut for InBT tracks with a given vertex (to pure up the starting inbt sample)
    // (for now 100micron ip). (electrons,pi+-);
    // (for now 250micron ip). (photons.);
    // (for now 300micron ip). (other.);
//   if (cmd_vtx!=1) return;
    if (cmd_vtx==0) return;
//     if (cmd_MC!=1) return; // why did we say that we wanna have only starting mc inbts?
    // also it can be possible that we wanna seacrh all in bt from the volume to an given
    //  vertex, for a mc event (but not necessarily for mc In BTs).
    // better comment it out?... ah, i know why because otherwise, we do not know which mc event vtx to take... so what to do???

    Float_t cutIPMax=100;
    if (cmd_vtx==2) cutIPMax=250;
    if (cmd_vtx==3) cutIPMax=500;
    if (cmd_vtx==4) cutIPMax=1000;
    if (cmd_vtx==5) cutIPMax=5000;

    Float_t cutZVtxDist=999999;

    TObjArray* GLOBAL_InBTArray2 = new TObjArray();
    cout << "Fill2GlobalInBTArray Calc IPs for the " << GLOBAL_InBTArray->GetEntries() << " entries" << endl;

    for (Int_t i=0; i<GLOBAL_InBTArray->GetEntries(); ++i) {
        if (i%1000==0) cout << "." << flush;
        EdbSegP* s1=(EdbSegP*)GLOBAL_InBTArray->At(i);
        // Calculate IP:
        // in case InBT is of BGType, we check for the GBMC variable set.
        Int_t MCEvt=s1->MCEvt();
        if (MCEvt<0 && cmd_GBMC>0) MCEvt=cmd_GBMC;

        Double_t ip=CalcIP(s1,Double_t(GLOBAL_VtxArrayX[MCEvt]),Double_t(GLOBAL_VtxArrayY[MCEvt]),Double_t(GLOBAL_VtxArrayZ[MCEvt]));

        if (ip>cutIPMax) continue;

        cutZVtxDist=s1->Z()-Double_t(GLOBAL_VtxArrayZ[MCEvt]);
        if (i==0||i==GLOBAL_InBTArray->GetEntries()-1) {
            cout << "INBT " << i << " :VTX:X:Y:Z:  " << GLOBAL_VtxArrayX[MCEvt] << " " << GLOBAL_VtxArrayY[MCEvt] << " " << GLOBAL_VtxArrayZ[MCEvt] << " ip= " << ip << " ZdistVtx=  " <<  cutZVtxDist << " MCEvt= " << MCEvt << endl;
        }

        if (ip<cutIPMax) GLOBAL_InBTArray2->Add(s1);
    }

    cout << endl;
    cout << "Fill2GlobalInBTArray::Before IP cut " << GLOBAL_InBTArray->GetEntries() << endl;
    cout << "Fill2GlobalInBTArray::After IP cut("<<cutIPMax<<") " << GLOBAL_InBTArray2->GetEntries() << endl;
    // Swap Arrays:
    GLOBAL_InBTArray= GLOBAL_InBTArray2;
    cout << GLOBAL_InBTArray->GetEntries() <<endl;
    return;
}



//-------------------------------------------------------------------------------------------
void Write_Alg_GS_Histograms() {

    Log(2, "ShowRec.cpp", "--- void Write_Alg_GS_Histograms() ---");
    f_GSNN->cd();
    f_GSNN->ls();
    h_GSNN_var00->Write();
    h_GSNN_var01->Write();
    h_GSNN_var02->Write();
    h_GSNN_var03->Write();
    h_GSNN_var04->Write();
    h_GSNN_var05->Write();
    h_GSNN_var06->Write();
    t_GSNN->Write();
    f_GSNN->Close();
    Log(2, "ShowRec.cpp", "--- void Write_Alg_GS_Histograms()...done.");
    return;
}


//-------------------------------------------------------------------------------------------

void DoBGTargetCleaning() {

    // Clean the input data Objects if necessary:
    Float_t BGTargetDensity=0;
    // cout << "--- \t\t :  -CLEAN  InputData BG Cleaning: 0: No, 1:20BT/mm2  2: 40BT/mm2  3:10BT/mm2 4:60BT/mm2 \n";
    // cout << "--- \t\t :    InputData BG Cleaning: 10: Remove DoubleBT and Passing, No dens cut, 11: &&10BT/mm2  12: &&20BT/mm2  13: &&30BT/mm2 ... \n";
    if (cmd_CLEAN==1) BGTargetDensity=20;
    if (cmd_CLEAN==2) BGTargetDensity=40;
    if (cmd_CLEAN==3) BGTargetDensity=10;
    if (cmd_CLEAN==4) BGTargetDensity=60;
    if (cmd_CLEAN==0) BGTargetDensity=1000;
    EdbPVRQuality* PVRQualCheck;
    EdbPVRec* new_GLOBAL_gAli;
    EdbPVRec* newnew_GLOBAL_gAli;


    // Just density cleaning, no double or passing removal.
    if (cmd_CLEAN!=0&&cmd_CLEAN<10) {
        cout << "Just density cleaning, no double or passing removal. " << endl;
        PVRQualCheck = new EdbPVRQuality(GLOBAL_gAli,BGTargetDensity);
        new_GLOBAL_gAli = PVRQualCheck->GetEdbPVRec(1);
        PVRQualCheck->Print();
        GLOBAL_gAli=new_GLOBAL_gAli;
    }

    // Density cleaning, with double and passing removal.
    // Additional BG cleaning, depending on the last number of the number switch:
    if (cmd_CLEAN>=10&&cmd_CLEAN<=20) {
        cout << "Density cleaning, with double and passing removal." << endl;
        cout << " THIS WILL BE THE PART WHERE REMOVE PASSING   IS IMPLEMENTED !!! " << endl;
        cout << " THIS WILL BE THE PART WHERE REMOVE DOUBLE BT IS IMPLEMENTED !!! " << endl;
        Int_t rest=cmd_CLEAN-10;
        Float_t BGTargetDensity=rest*10;
        if (rest==0) BGTargetDensity=100000;
        //cout << "BGTargetDensity  " << BGTargetDensity << endl;
        PVRQualCheck = new EdbPVRQuality(GLOBAL_gAli,BGTargetDensity);
        new_GLOBAL_gAli = PVRQualCheck->GetEdbPVRec(1);
        GLOBAL_gAli=new_GLOBAL_gAli;
        GLOBAL_gAli->Print();
        newnew_GLOBAL_gAli=PVRQualCheck->Remove_DoubleBT(GLOBAL_gAli);
        GLOBAL_gAli=newnew_GLOBAL_gAli;
        new_GLOBAL_gAli=PVRQualCheck->Remove_Passing(GLOBAL_gAli);
        GLOBAL_gAli=new_GLOBAL_gAli;
    }

    return;
} // of void DoBGTargetCleaning()


void SetPresetParameters(Int_t cmd_PRESET) {

    Log(2, "ShowRec.cpp", "--- SetPresetParameters() ---");

    cout << "ATTentION : Currently we set cmd_ALI=2 for all presets for testing purposes!!! " << endl;
    cmd_ALI=2;

    if (cmd_PRESET==0) {
        cmd_MC=0;
        cmd_HPLZ=0;
        cmd_CLEAN=0;
    }
    else if (cmd_PRESET==1) {
        cmd_MC=1;
        cmd_HPLZ=1;
        cmd_CLEAN=0;
    }
    else if (cmd_PRESET==2) {
        cmd_MC=0;
        cmd_HPLZ=0;
        cmd_LT=1;
    }
    else if (cmd_PRESET==3) {
        cmd_MC=0;
        cmd_HPLZ=0;
        cmd_vtx=1;
    }
    else {
        cout << "No Preset List for this parameter found. Using values for PRESET 0." << endl;
        cmd_MC=0;
        cmd_HPLZ=0;
        cmd_CLEAN=0;
    }
    Log(2, "ShowRec.cpp", "--- SetPresetParameters()...done. ---");
    return;
}
