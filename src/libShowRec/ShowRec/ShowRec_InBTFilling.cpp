//-------------------------------------------------------------------------------------------

void FillGlobalInBTArrayNEW()
{
    Log(2, "ShowRec.cpp", "--- void FillGlobalInBTArrayNEW() ---");

    // Old FillGlobalInBTArray is unclear and poorly written.

    //--------------------------------------------------------
    // To fill the Initiator BT array from the gAli
    // watching boundary conditions for the Plates, MC Cut.
    //--------------------------------------------------------
    GLOBAL_InBTArray = new TObjArray();
    GLOBAL_ShowerSegArray = new TObjArray();
    EdbSegP  *segment;
    //--------------------------------------------------------

    // Informational Debug Output
    cout << "---!! DOWNSTREAM ORDER ASSUMED !!---"<<endl;
    cout << "---!! IF PROGRAM CRASH HERE, CHECK YOUR INPUT FP,NP;MP;LP !!---"<<endl;

    cout << "---!! TO BE DONE: WHAT IF BRICK IS IN UPSTREAM ORDERING ??? !!---"<<endl;

    Int_t npat = GLOBAL_gAli->Npatterns();  //number of plates
    Int_t firstplate=   npat-cmd_FP;
    Int_t middleplate=   npat-cmd_MP;
    // Change because the lastplate can also be the middleplate (then, by definition, only
    // showers on the plate will be reconstructed..)
    //Int_t lastplate=   TMath::Max(npat-cmd_LP-1,0); // attention, change (date: 2018 06 20)
    Int_t lastplate=     TMath::Max(npat-cmd_LP,0); // attention, change (date: 2018 06 20)

    Float_t GLOBAL_gAli__firstplate_Z=GLOBAL_gAli->GetPattern(firstplate)->Z();
    Float_t GLOBAL_gAli__middleplate_Z=GLOBAL_gAli->GetPattern(middleplate)->Z();
    Float_t GLOBAL_gAli__lastplate_Z=GLOBAL_gAli->GetPattern(lastplate)->Z();

    //--------------------------------------------------------
    if (gEDBDEBUGLEVEL>2) {
        cout << "--- FillGlobalInBTArray: DOWNSTREAM ORDER" <<endl;
        cout << "--- npat = " << npat << endl;
        cout << "--- firstplate = " << firstplate << endl;
        cout << "--- middleplate = " << middleplate << endl;
        cout << "--- lastplate = " << lastplate << endl;
        cout << " GLOBAL_gAli->GetPattern(firstplate)->Z()  =  " << GLOBAL_gAli->GetPattern(firstplate)->Z() << endl;
        cout << " GLOBAL_gAli->GetPattern(middleplate)->Z() =  " << GLOBAL_gAli->GetPattern(middleplate)->Z() << endl;
        cout << " GLOBAL_gAli->GetPattern(lastplate)->Z()   =  " << GLOBAL_gAli->GetPattern(lastplate)->Z() << endl;
    }
    //--------------------------------------------------------

    // return;


    // Decision Branch for efficiently filling the InBT Array:
    /*
    0) criterion Source:
                -> From Volume
                -> From LinkedTrack file
    1) criterion Plate range:
                -> All from [FP..MP]
    2) criterion MC-value
                -> All
                -> specific MC-event number
    3) criterion PDG-value
                -> All
                -> specific PDG-number (only one at the moment)
    4) criterion HPLZ
                -> HPLZ=0 All
                -> HPLZ=1 Take Highest P() value for each MCEvt
                -> HPLZ=2 Take Lowest (i.e. lowest) z occurence for each MCEvt

    5) criterion vertex IP Cut cmd_vtx
                -> cmd_vtx=0 All
                -> cmd_vtx=1 IP<100   microns
                -> cmd_vtx=2 IP<250   microns
                -> cmd_vtx=3 IP<500   microns
                -> cmd_vtx=4 IP<1000  microns
                -> cmd_vtx=5 IP<5000  microns
                    vtx either given from BRICK.TreePGunInfo.txt and -MC=1
                    or from a "real" vertex file ... to be checked !!
    */


    /* Technical Realisation
     * First time:
     * Add all (EdbSegP*) selected to TObjArray
     * Then for each criterion:
     * Those which do NOT fit the criterion, remove from the TObjArray
     * CHECK IF BY ROOT MEANS THIS IS POSSIBLE
     * https://root.cern.ch/root/html604/TObjArray.html
     * virtual TObject*	Remove(TObject* obj)
     * TObject * Remove(TObject* obj)
     */
    cout << "Technical Realisation     to be implemented here .... " << endl;

    TObjArray* arrayIntermediate  = new TObjArray();

    //--------------------------------------------------------
    /*
    0) criterion Source:
                -> From Volume
                -> From LinkedTrack file
    */
    //--------------------------------------------------------
    // Do this in case of -cmd_LT==1,2,3,4 (i.e. -cmd_LT>0)
    // 1: all starting BTs of a track are taken.
    // 2: all ending   BTs of a track are taken.
    // 3: all          BTs of a track are taken.
    // 4: (to be specified)
    //--------------------------------------------------------
    if (cmd_LT>0) {

        if (gEDBDEBUGLEVEL>2) cout << "--- Doing filling InBT case  cmd_LT>0"<<endl;
        EdbSegP * s2=0;
        EdbTrackP  *t  = 0;
        TFile * fil = new TFile("linked_tracks.root");
        TTree* tr= (TTree*)fil->Get("tracks");
        TClonesArray *seg= new TClonesArray("EdbSegP",60);
        int nentr = int(tr->GetEntries());
        int nseg,n0,npl;
        tr->SetBranchAddress("t.", &t  );
        tr->SetBranchAddress("s", &seg  );
        tr->SetBranchAddress("nseg", &nseg  );
        tr->SetBranchAddress("n0", &n0  );
        tr->SetBranchAddress("npl", &npl  );
        //tr->Show(0);

        // Just add the basetracks from the tracks to the global InBTArray.
        for (int i=0; i<nentr; i++ ) {
            tr->GetEntry(i);
            if (gEDBDEBUGLEVEL>3) tr->Show(i);

            // depending on cmd_LT=1,2,3 different loops have to be gone through:
            for (int k=0; k<nseg; k++ ) {
                //cout << "----- Doing  i= " <<  i <<  " and  k= " <<  k<< endl;
                if (cmd_LT==1 && k>0) continue;    // only first BT
                if (cmd_LT==2 && k<nseg-1) continue;  // only last BT
                // Take first BT of the tracks:
                s2=(EdbSegP*) seg->At(k);
                segment=(EdbSegP*)s2->Clone();
                //--------------------------------------------------------
                /*
                1) criterion Plate range:
                            -> All from [FP..MP]
                */
                //--------------------------------------------------------
                // Comparison is done via the z-values of the plates
                Float_t patZ=segment->Z();
                if (patZ<GLOBAL_gAli__firstplate_Z || patZ>GLOBAL_gAli__middleplate_Z) continue;

                arrayIntermediate->Add(segment);

            } // of for(int k=0; k<nseg; k++ )
        } // for(int i=0; i<nentr; i++ )
        delete tr;
        delete fil;
        if (gEDBDEBUGLEVEL>2) cout << "--- Filled " << arrayIntermediate->GetEntries()  << " Segments into arrayIntermediate."<<endl;
    } // end of Do this in case of -cmd_LT>0
    else if (cmd_LT==0) {
        cout << "cmd_LT==0" << endl;
        for (Int_t nr=0; nr<npat; ++nr) {

            //--------------------------------------------------------
            /*
            1) criterion Plate range:
                        -> All from [FP..MP]
            */
            //--------------------------------------------------------
            // Comparison is done via the z-values of the plates
            Float_t patZ=GLOBAL_gAli->GetPattern(nr)->Z();
            // cout << "Doing pattern at: " << nr  << "  with Z()= " << patZ << endl;

            Bool_t InRange=1;
            if (patZ<GLOBAL_gAli__firstplate_Z || patZ>GLOBAL_gAli__middleplate_Z) InRange=0;
            // cout << "Is this pattern element [FP,MP]?" << InRange  << endl;
            if ( !InRange) continue;

            //cout << "GLOBAL_gAli->GetPattern(nr)->GetN() " << GLOBAL_gAli->GetPattern(nr)->GetN() << endl;
            //cout << "arrayIntermediate->GetEntries() " << arrayIntermediate->GetEntries() << endl;
            for (Int_t n=0; n<GLOBAL_gAli->GetPattern(nr)->GetN(); ++n) {
                segment=GLOBAL_gAli->GetPattern(nr)->GetSegment(n);
                arrayIntermediate->Add(segment);
            }
        }
    } // Do this in case of -cmd_LT==0
    //--------------------------------------------------------
    else {
        cout << "--- Attention: given cmd_LT value does not macht any criterion. " <<  endl;
        cout << "--- Dont fill the arrayIntermediate with anything." <<  endl;
    }

    cout << "--- Criterion 0) Source done." <<  endl;
    cout << "--- Criterion 1) Plate range done." << endl;
    cout << "--- arrayIntermediate->GetEntries() =  " <<  arrayIntermediate->GetEntries() << endl;

    //--------------------------------------------------------
    /*
    2) criterion MC-value
                -> All
                -> specific MC-event number (cmd_MC = #nr)
    */
    //--------------------------------------------------------
    TObjArray* arrayIntermediate2  = new TObjArray();

    Int_t nEnt = arrayIntermediate->GetEntries();
    Int_t numberMC;

    for (Int_t n=0; n<nEnt; ++n) {
        segment=(EdbSegP*) arrayIntermediate->At(n);
        numberMC = segment->MCEvt();
        if (cmd_MC == 0 ) arrayIntermediate2->Add(segment);
        if (cmd_MC == 1 ) if (numberMC>=0) arrayIntermediate2->Add(segment);
        if (cmd_MC == 2 ) if (numberMC <0) arrayIntermediate2->Add(segment);
    }
    cout << "--- Criterion 2) MC flag range done." << endl;
    cout << "--- arrayIntermediate2->GetEntries() =  " <<  arrayIntermediate2->GetEntries() << endl;

    //--------------------------------------------------------
    /*
    3) criterion PDG-value
                -> All (cmd_MCFL = 0)
                -> specific PDG-number (only one at the moment)
    */
    //--------------------------------------------------------
    TObjArray* arrayIntermediate3  = new TObjArray();
    // if cmd_MCFL==0 (default) then add all segments anyway,
    // which is equivalent to just copy the array.
    if (cmd_MCFL == 0 ) {
        arrayIntermediate3 = arrayIntermediate2;
    }
    else {
        nEnt = arrayIntermediate2->GetEntries();
        Int_t numberPDGId;
        for (Int_t n=0; n<nEnt; ++n) {
            segment=(EdbSegP*) arrayIntermediate2->At(n);
            //segment->Print();
            numberPDGId = segment->Flag();
            if (numberPDGId == cmd_MCFL ) arrayIntermediate3->Add(segment);
        }
    }
    cout << "--- Criterion 3) PDG-value done." << endl;
    cout << "--- Criterion 3) cmd_MCFL = " << cmd_MCFL << endl;
    cout << "--- arrayIntermediate3->GetEntries() =  " <<  arrayIntermediate3->GetEntries() << endl;

    //--------------------------------------------------------
    /*
      4) criterion HPLZ
                -> HPLZ=0 All
                -> HPLZ=1 Take highest P() value for each MCEvt
                -> HPLZ=2 Take first (i.e. lowest) z occurence for each MCEvt
                        (if more BT have same minimal Z() value, then take highest P())
    */
    //--------------------------------------------------------
    TObjArray* arrayIntermediate4 = new TObjArray();
    if (cmd_HPLZ == 0) {
        arrayIntermediate4 = arrayIntermediate3 ;
    }
    else {
        // Split the two arrays, since only MC array info can be sorted:
        TObjArray* arrayIntermediateBG  = new TObjArray();
        TObjArray* arrayIntermediateMC  = new TObjArray();
        TObjArray* arrayIntermediateFHZP  = new TObjArray();

        nEnt = arrayIntermediate3->GetEntries();
        for (Int_t n=0; n<nEnt; ++n) {
            segment=(EdbSegP*) arrayIntermediate3->At(n);
            if ( segment->MCEvt()<0) arrayIntermediateBG->Add(segment);
            else  arrayIntermediateMC->Add(segment);
        }
        //cout << "--- arrayIntermediateBG->GetEntries() =  " <<  arrayIntermediateBG->GetEntries() << endl;
        //cout << "--- arrayIntermediateMC->GetEntries() =  " <<  arrayIntermediateMC->GetEntries() << endl;
        EdbSegP* seg;
        EdbSegP* segComp;
        Int_t NarrayIntermediateMC=arrayIntermediateMC->GetEntries();
        Bool_t OtherSegGreaterPSameMC=kFALSE;
        Bool_t OtherSegLowerZSameMC=kFALSE;
        Int_t segMC=-1;
        Float_t segZ=-1;
        Int_t segCompMC=-1;

        // Find minimum and maximum Eventnumber and Z position
        // in the arrayIntermediateMC array:
        Int_t segMCmin=NarrayIntermediateMC;
        Int_t segMCmax=-1;

        for (Int_t n=0; n<NarrayIntermediateMC; ++n) {
            seg=(EdbSegP*) arrayIntermediateMC->At(n);
            segMC = seg->MCEvt();
            segZ = seg->Z();
            if (segMC>segMCmax) segMCmax = segMC;
            if (segMC<segMCmin) segMCmin = segMC;
        }

        // cmd_HPLZ == 1 Take highest P() segment per each MCEvt.
        // cmd_HPLZ == 2 Take first (lowest) Z() semgment per each MCEvt
        // In case two (or more) BTs have same lowest Z()
        // then take the highest P() BT.

        if (cmd_HPLZ == 1 || cmd_HPLZ == 2) {
            if (cmd_HPLZ == 1) cout << "Take highest P() segment per each MCEvt." << endl;
            if (cmd_HPLZ == 2) cout << "Take lowest  Z() segment per each MCEvt." << endl;
            // Try search with arrays instead of double loops! Yes, works much faster!
            // After MCEvt range is set, start to look for highest P() value for each MCEvt:
            Float_t segP, segZ;
            Float_t segPmax[segMCmax+1];
            Int_t   segn[segMCmax+1];
            Float_t segZmin[segMCmax+1];
            Int_t   segm[segMCmax+1];

            // Reset all array values
            for (Int_t nrMC=segMCmin; nrMC<=segMCmax; ++nrMC) {
                segPmax[nrMC]=0;
                segn[nrMC]=-1;
                segZmin[nrMC]=9999999999;
                segm[nrMC]=-1;
            }

            EdbSegP* seg2;
            for (Int_t n=0; n<NarrayIntermediateMC; ++n) {
                seg=(EdbSegP*) arrayIntermediateMC->At(n);
                segMC = seg->MCEvt();
                segP = seg->P();
                segZ = seg->Z();
                if (segP>segPmax[segMC]) {
                    segPmax[segMC] = segP;
                    segn[segMC] = n;
                }
                if (segZ<segZmin[segMC]) {
                    segZmin[segMC] = segZ;
                    segm[segMC] = n;
                }
                // Safe method agains rounding errors:
                if (TMath::Abs(segZ-segZmin[segMC])<0.1) {
                    // Compare P() with the existing lowest Z segment
                    seg2=(EdbSegP*) arrayIntermediateMC->At(segm[segMC]);
                    //cout << "segP= " << segP <<   " and   seg2->P()= " << seg2->P() << endl;
                    if (  segP > seg2->P() ) {
                        segZmin[segMC] = segZ;
                        segm[segMC] = n;
                    }
                }
            }

            Int_t SegHPLZNumberInArray=0;
            for (Int_t nrMC=segMCmin; nrMC<=segMCmax; ++nrMC) {
                if (segn[nrMC]==-1) continue;
                if (cmd_HPLZ == 1) SegHPLZNumberInArray = segn[nrMC];
                if (cmd_HPLZ == 2) SegHPLZNumberInArray = segm[nrMC];
                // cout << "--- For MCEvt " <<  nrMC << " P() maximal is =  " <<  segPmax[nrMC] << endl;
                // cout << "--- For MCEvt " <<  nrMC << " Z() minimal is =  " <<  segZmin[nrMC] << endl;
                seg=(EdbSegP*) arrayIntermediateMC->At(segn[nrMC]);
                // cout << "Highest P() segment:" << endl;
                // seg->PrintNice();
                seg=(EdbSegP*) arrayIntermediateMC->At(segm[nrMC]);
                // cout << "Lowest  Z() segment:" << endl;
                // seg->PrintNice();
                //---------------
                // Add now the corresponding segment
                seg=(EdbSegP*) arrayIntermediateMC->At(SegHPLZNumberInArray);
                arrayIntermediateFHZP->Add(seg);
            }

            cout << " Added all FHZP segments. Number of entries = " <<  arrayIntermediateFHZP->GetEntries()  << endl;

        } // of if (cmd_HPLZ == 1)
        // unsupported cmd_HPLZ value: copy array before.
        else {
            cout << "Warning: unsupported cmd_HPLZ value. Do no cut." << endl;
            arrayIntermediateFHZP = arrayIntermediate3 ;
        }

        // Now merge arrays
        // arrayIntermediateBG and arrayIntermediateFHZP
        // into TObjarray* arrayIntermediate4 = new TObjArray();
        nEnt = arrayIntermediateFHZP->GetEntries();
        for (Int_t n=0; n<nEnt; ++n) {
            segment=(EdbSegP*) arrayIntermediateFHZP->At(n);
            arrayIntermediate4->Add(segment);
        }
        nEnt = arrayIntermediateBG->GetEntries();
        for (Int_t n=0; n<nEnt; ++n) {
            segment=(EdbSegP*) arrayIntermediateBG->At(n);
            arrayIntermediate4->Add(segment);
        }

    } // of else of   if (cmd_HPLZ == 0)

    cout << "--- Criterion 4) criterion HPLZ " << endl;
    cout << "--- Criterion 4) cmd_HPLZ = " << cmd_HPLZ << endl;
    cout << "--- arrayIntermediate4->GetEntries() =  " <<  arrayIntermediate4->GetEntries() << endl;

    cout << "---- " << endl  << endl;
    cout << "TODO HERE:   CRITERION 5   VTX IP CUT ... " << endl;
    cout << "TODO HERE:   CRITERION 5   VTX IP CUT ... " << endl;
    cout << "WARNING As of July 4th 2018, the VTX IP CUT is not yet implemented  " << endl;
    cout << "in the new FillGlobalInBTArrayNEW() function.  TO BE DONE !!!  " << endl;
    cout << "TODO HERE:   CRITERION 5   VTX IP CUT ... " << endl;
    cout << "TODO HERE:   CRITERION 5   VTX IP CUT ... " << endl;
    cout << "---- " << endl  << endl;

    /*
     5) criterion vertex IP Cut cmd_vtx
                -> cmd_vtx=0 All
                -> cmd_vtx=1 IP<100   microns
                -> cmd_vtx=2 IP<250   microns
                -> cmd_vtx=3 IP<500   microns
                -> cmd_vtx=4 IP<1000  microns
                -> cmd_vtx=5 IP<5000  microns
                    vtx either given from BRICK.TreePGunInfo.txt and -MC=1
                    or from a "real" vertex file ... to be checked !!
                    */


    // Change the Array:
    // might leave behind the original array in an unused state...
    GLOBAL_InBTArray = arrayIntermediate4;
    Int_t GLOBAL_InBTArrayN= GLOBAL_InBTArray->GetEntries();

    cout << "GLOBAL_InBTArray filling done.................."<<endl;
    if (GLOBAL_InBTArray->GetEntries()==0) {
        cout << "--- GLOBAL_InBTArray->GetEntries() =  0 .  RETURN FILL FUNCTION !" << endl;
        return;
    }

    /// --- ONLY DEBUG ---
    if (gEDBDEBUGLEVEL<0) {
        EdbSegP* seg0=(EdbSegP*)GLOBAL_InBTArray->At(0);
        EdbSegP* segend=(EdbSegP*)GLOBAL_InBTArray->At(GLOBAL_InBTArray->GetEntries()-1);
        cout << "--- GLOBAL_InBTArray->GetEntries() =  " <<  GLOBAL_InBTArray->GetEntries() << endl;
        cout << "--- GLOBAL_InBTArray->At(0) =  " <<  endl;
        seg0->PrintNice();
        cout << "--- GLOBAL_InBTArray->At(Last) =  " << endl;
        segend->PrintNice();
    }

    /// --- ONLY DEBUG ---
    if (gEDBDEBUGLEVEL>2) {
        for (Int_t n=0; n<GLOBAL_InBTArrayN; ++n) {
            EdbSegP* seg0=(EdbSegP*)GLOBAL_InBTArray->At(n);
            seg0->PrintNice();
        }
    }

    // What to do, if GLOBAL_InBTArray has zero entries???
    cout << "What to do, if GLOBAL_InBTArray has zero entries???" << endl;
    cout << "Only logical would be that the program stops." << endl;
    return;
}

