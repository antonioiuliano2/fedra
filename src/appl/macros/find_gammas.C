{
       Alg = new EdbShowerAlg_GS();
       Alg->SetEdbPVRec(pvr);
    //   Alg->SetInVtx(EdbVertex* vtx);
       Alg->Execute();
       Alg->GetRecoShowerArray();
			 Alg->Print();
}