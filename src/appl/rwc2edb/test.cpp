char*  RandomString(int len)
{
	char* Temp =  new char[len];
	Temp = "";
	while (len-- > 0) Temp += (rand() % 26 + 'A');
	return Temp;
}



int Test() 
 {
#ifdef _WINDOWS

	 int jkl=0;
    
	char TempPath[256];
	char m_Path[256];
    sprintf(m_Path, "prova");

 	UINT	m_Fragments = 1;
	UINT	m_MaxTracks = 4;
	UINT	m_MinTracks = 1;
	UINT	m_Views = 2;

	UINT ExtErrorInfo; 
	IO_VS_Catalog Cat;
	IO_VS_Fragment Frag;
	IO_VS_Fragment2 Frag2;
	int i, j, k, fi;

   ISySalDataIO*  iIO;
   ISySalDataIO2* iIO2;
   CoInitialize(NULL);
   CoCreateInstance(CLSID_SySalDataIO, NULL, CLSCTX_INPROC_SERVER, 
		              IID_ISySalDataIO, (void **)&iIO);
   iIO->QueryInterface(IID_ISySalDataIO2, (void**)&iIO2);

/* BEGIN CATALOG TEST */

	Cat.Hdr.ID.Part[0] = 1234;
	Cat.Hdr.ID.Part[1] = 5678;
	Cat.Hdr.ID.Part[2] = 9101112;
	Cat.Hdr.ID.Part[3] = 13141516;
	Cat.Hdr.Type.InfoType = VS_HEADER_BYTES | VS_CATALOG_SECTION;
	Cat.Hdr.Type.HeaderFormat = VS_OLD_HEADER_TYPE;
	for (i = 0; i < sizeof(Cat.Reserved); i++) Cat.Reserved[i] = i;
	Cat.Area.Fragments = m_Fragments;
	Cat.Area.XViews = m_Views;
	Cat.Area.YViews = m_Fragments;
	Cat.Area.XMin = (rand() % 1000) * 500.;
	Cat.Area.YMin = ((rand() ^ 0xffdba324) % 1000) * 500.;
	Cat.Area.XStep = 330.f;
	Cat.Area.YStep = 330.f;
	Cat.Area.XMax = Cat.Area.XMin + Cat.Area.XViews * (Cat.Area.XStep + 1);
	Cat.Area.YMax = Cat.Area.YMax + Cat.Area.YViews * (Cat.Area.YStep + 1);
	Cat.pFragmentIndices = new int[Cat.Area.XViews * Cat.Area.YViews];
	for (i = 0; i < (Cat.Area.XViews * Cat.Area.YViews); i++)
		Cat.pFragmentIndices[i] = i / Cat.Area.XViews + 1;
	Cat.Config.CountOfConfigs = rand() % 8;
	Cat.Config.pConfigs = Cat.Config.CountOfConfigs ? new VS_Config[Cat.Config.CountOfConfigs] : 0;
	for (i = 0; i < Cat.Config.CountOfConfigs; i++)
	{
		strcpy(Cat.Config.pConfigs[i].ClassName, RandomString(16));
		strcpy(Cat.Config.pConfigs[i].Config.Name, RandomString(16));
		Cat.Config.pConfigs[i].Config.CountOfItems = rand() % 32;
		Cat.Config.pConfigs[i].Config.pItems = Cat.Config.pConfigs[i].Config.CountOfItems ? 
			new char[Cat.Config.pConfigs[i].Config.CountOfItems * 
			(SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN)] : 0;
		for (j = 0; j < Cat.Config.pConfigs[i].Config.CountOfItems; j++)
		{
			strcpy(Cat.Config.pConfigs[i].Config.pItems + j * 
				(SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN), RandomString(16));
			strcpy(Cat.Config.pConfigs[i].Config.pItems + j * 
				(SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN) + 
				SYSAL_MAXCONFIG_ENTRY_NAME_LEN, RandomString(16));
			};
		};

	sprintf(TempPath,"prova.rwc");
	if (iIO->Write(0, (BYTE *)&Cat, &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
	{
		printf("Error Writing", "Debug", MB_ICONERROR | MB_OK);
		};
	//TempPath.ReleaseBuffer();

cout << jkl++ ;

IO_VS_Catalog *pReadCat = 0;

	if (iIO->Read(0, (BYTE *)(&pReadCat), &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
	{
		printf("Error Reading", "Debug", MB_ICONERROR | MB_OK);
		};
	//TempPath.ReleaseBuffer();

	if (memcmp(&Cat.Area, &pReadCat->Area, sizeof(Cat.Area))) printf("Area section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(&Cat.Hdr, &pReadCat->Hdr, sizeof(Cat.Hdr))) printf("Header section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(&Cat.Reserved, &pReadCat->Reserved, sizeof(Cat.Reserved))) printf("Reserved section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(Cat.pFragmentIndices, pReadCat->pFragmentIndices, sizeof(int) * Cat.Area.XViews * Cat.Area.YViews)) printf("Fragment index section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(&Cat.Config.CountOfConfigs, &pReadCat->Config.CountOfConfigs, sizeof(Cat.Config.CountOfConfigs))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
	for (i = 0; i < Cat.Config.CountOfConfigs; i++)
	{
		if (memcmp(&Cat.Config.pConfigs[i].ClassName, &pReadCat->Config.pConfigs[i].ClassName, sizeof(Cat.Config.pConfigs[i].ClassName))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Cat.Config.pConfigs[i].Config.Name, &pReadCat->Config.pConfigs[i].Config.Name, sizeof(Cat.Config.pConfigs[i].Config.Name))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Cat.Config.pConfigs[i].Config.CountOfItems, &pReadCat->Config.pConfigs[i].Config.CountOfItems, sizeof(Cat.Config.pConfigs[i].Config.CountOfItems))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(Cat.Config.pConfigs[i].Config.pItems, pReadCat->Config.pConfigs[i].Config.pItems, Cat.Config.pConfigs[i].Config.CountOfItems * (SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		};

	if (pReadCat) CoTaskMemFree(pReadCat);

	if (Cat.pFragmentIndices) delete [] Cat.pFragmentIndices;
	if (Cat.Config.pConfigs) 
	{
		for (i = 0; i < Cat.Config.CountOfConfigs; i++)
			if (Cat.Config.pConfigs[i].Config.pItems)
				delete [] Cat.Config.pConfigs[i].Config.pItems;
		delete [] Cat.Config.pConfigs;
		};

//	DeleteFile(m_Path);

/* END CATALOG TEST */

/* BEGIN FRAGMENT TEST */

	for (fi = 0; fi < Cat.Area.YViews; )
	{
		Frag.Hdr.ID = Cat.Hdr.ID;
		Frag.Hdr.Type.InfoType = VS_HEADER_BYTES | VS_FRAGMENT_SECTION;
		Frag.Hdr.Type.HeaderFormat = VS_OLD_HEADER_TYPE;
		for (i = 0; i < sizeof(Frag.Reserved); i++) Frag.Reserved[i] = i;
		Frag.Fragment.CountOfViews = Cat.Area.XViews;
		Frag.Fragment.CodingMode = (rand() % 2) ? VS_COMPRESSION_NULL : VS_COMPRESSION_GRAIN_SUPPRESSION;
		Frag.Fragment.FitCorrectionDataSize = ((rand() % 4) >> 1) ? 16 : 0;
		Frag.Fragment.StartView = fi * Cat.Area.XViews;
		Frag.Fragment.Index = fi + 1;
		Frag.Fragment.pViews = new VS_View[Cat.Area.XViews];
		for (i = 0; i < Cat.Area.XViews; i++)
		{
			Frag.Fragment.pViews[i].TileX = i;
			Frag.Fragment.pViews[i].TileY = fi;
			Frag.Fragment.pViews[i].RelevantZs.TopExt = 125.f + (rand() % 16) * .5f;
			Frag.Fragment.pViews[i].RelevantZs.TopInt = Frag.Fragment.pViews[i].RelevantZs.TopExt - 25.f;
			Frag.Fragment.pViews[i].RelevantZs.BottomInt = Frag.Fragment.pViews[i].RelevantZs.TopInt - 200.f;
			Frag.Fragment.pViews[i].RelevantZs.BottomExt = Frag.Fragment.pViews[i].RelevantZs.BottomInt - 25.f;
			for (k = 0; k < 2; k++)
			{
				Frag.Fragment.pViews[i].X[k] = Cat.Area.XStep * Frag.Fragment.pViews[i].TileX + (rand() % 101 - 50) * .02;
				Frag.Fragment.pViews[i].Y[k] = Cat.Area.YStep * Frag.Fragment.pViews[i].TileY + (rand() % 101 - 50) * .02;
				Frag.Fragment.pViews[i].MapX[k] = Frag.Fragment.pViews[i].X[k];
				Frag.Fragment.pViews[i].MapY[k] = Frag.Fragment.pViews[i].Y[k];
				Frag.Fragment.pViews[i].ImageMat[k][0][0] = Frag.Fragment.pViews[i].ImageMat[k][1][1] = 1.;
				Frag.Fragment.pViews[i].ImageMat[k][0][1] = Frag.Fragment.pViews[i].ImageMat[k][1][0] = 0.;
				Frag.Fragment.pViews[i].Layers[k].Count = 12;
				Frag.Fragment.pViews[i].Layers[k].pZs = new float[Frag.Fragment.pViews[i].Layers[k].Count];
				for (j = 0; j < Frag.Fragment.pViews[i].Layers[k].Count; j++)
					Frag.Fragment.pViews[i].Layers[k].pZs[j] = (k ? Frag.Fragment.pViews[i].RelevantZs.BottomInt : Frag.Fragment.pViews[i].RelevantZs.TopExt) - j * 25.f / 11.f ;
				Frag.Fragment.pViews[i].Status[k] = rand() % 3;
				Frag.Fragment.pViews[i].TCount[k] = rand() % (m_MaxTracks - m_MinTracks) + m_MinTracks;
				Frag.Fragment.pViews[i].pTracks[k] = Frag.Fragment.pViews[i].TCount[k] ? new Track[Frag.Fragment.pViews[i].TCount[k]] : 0;
				for (j = 0; j < Frag.Fragment.pViews[i].TCount[k]; j++)
				{
					Track &T = Frag.Fragment.pViews[i].pTracks[k][j];
					T.Field = i;
					T.PointsN = 8 + rand() % 4;
					T.LastZ = (k ? Frag.Fragment.pViews[i].RelevantZs.BottomExt : Frag.Fragment.pViews[i].RelevantZs.TopInt);
					T.FirstZ = T.LastZ + T.PointsN * 25.f / 11.f;
					T.Intercept.X = rand() % (int)Cat.Area.XStep - Cat.Area.XStep * 0.5f;
					T.Intercept.Y = rand() % (int)Cat.Area.YStep - Cat.Area.YStep * 0.5f;
					T.Intercept.Z = (k ? Frag.Fragment.pViews[i].RelevantZs.BottomInt : Frag.Fragment.pViews[i].RelevantZs.TopInt);
					T.Slope.X = (rand() % 1001 - 500) * .001f;
					T.Slope.Y = (rand() % 1001 - 500) * .001f;
					T.Slope.Z = 1.f;
					T.InterceptErrors.X = (rand() % 10) * .1f;
					T.InterceptErrors.Y = (rand() % 10) * .1f;
					T.InterceptErrors.X = (rand() % 10) * .01f;
					T.SlopeErrors.X = (rand() % 10) * .0001f;
					T.SlopeErrors.Y = (rand() % 10) * .0001f;
					T.SlopeErrors.Z = 0.;
					T.Sigma = hypot(T.InterceptErrors.X, T.InterceptErrors.Y);
					T.Valid = true;
					T.pPoints = new TVector[T.PointsN];
					int p;
					for (p = 0; p < T.PointsN; p++)
					{
						T.pPoints[p].X = (rand() % 101 - 99) * .01;
						T.pPoints[p].Y = (rand() % 101 - 99) * .01;
						T.pPoints[p].Z = Frag.Fragment.pViews[i].Layers[k].pZs[p];
						};
					if (Frag.Fragment.FitCorrectionDataSize)
					{
						T.pCorrection = new BYTE[Frag.Fragment.FitCorrectionDataSize];
						for (p = 0; p < Frag.Fragment.FitCorrectionDataSize; T.pCorrection[p++] = rand() % 256);
						}
					else T.pCorrection = 0;
					};
				};
			};

		sprintf(TempPath,"%s.rwd.%08X", m_Path, fi + 1);
		if (iIO->Write(0, (BYTE *)&Frag, &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
		{
			printf("Error Writing", "Debug", MB_ICONERROR | MB_OK);
			};
		//TempPath.ReleaseBuffer();

		IO_VS_Fragment *pReadFrag = 0;
		if (iIO->Read(0, (BYTE *)&pReadFrag, &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
		{
			printf("Error Reading", "Debug", MB_ICONERROR | MB_OK);
			};
		//TempPath.ReleaseBuffer();

		if (memcmp(&Frag.Hdr, &pReadFrag->Hdr, sizeof(Frag.Hdr))) printf("Header section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag.Reserved, &pReadFrag->Reserved, sizeof(Frag.Reserved))) printf("Reserved section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag.Fragment.CodingMode, &pReadFrag->Fragment.CodingMode, sizeof(Frag.Fragment.CodingMode))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag.Fragment.CountOfViews, &pReadFrag->Fragment.CountOfViews, sizeof(Frag.Fragment.CountOfViews))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag.Fragment.FitCorrectionDataSize, &pReadFrag->Fragment.FitCorrectionDataSize, sizeof(Frag.Fragment.FitCorrectionDataSize))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag.Fragment.Index, &pReadFrag->Fragment.Index, sizeof(Frag.Fragment.Index))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag.Fragment.StartView, &pReadFrag->Fragment.StartView, sizeof(Frag.Fragment.StartView))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
		{
			if (memcmp(&Frag.Fragment.pViews[i].RelevantZs, &pReadFrag->Fragment.pViews[i].RelevantZs, sizeof(Frag.Fragment.pViews[i].RelevantZs))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag.Fragment.pViews[i].X, &pReadFrag->Fragment.pViews[i].X, sizeof(Frag.Fragment.pViews[i].X))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag.Fragment.pViews[i].Y, &pReadFrag->Fragment.pViews[i].Y, sizeof(Frag.Fragment.pViews[i].Y))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag.Fragment.pViews[i].TileX, &pReadFrag->Fragment.pViews[i].TileX, sizeof(Frag.Fragment.pViews[i].TileX))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag.Fragment.pViews[i].TileY, &pReadFrag->Fragment.pViews[i].TileY, sizeof(Frag.Fragment.pViews[i].TileY))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag.Fragment.pViews[i].TCount, &pReadFrag->Fragment.pViews[i].TCount, sizeof(Frag.Fragment.pViews[i].TCount))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag.Fragment.pViews[i].Status, &pReadFrag->Fragment.pViews[i].Status, sizeof(Frag.Fragment.pViews[i].Status))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			for (k = 0; k < 2; k++)
			{
				if (memcmp(&Frag.Fragment.pViews[i].Layers[k].Count, &pReadFrag->Fragment.pViews[i].Layers[k].Count, sizeof(Frag.Fragment.pViews[i].Layers[k].Count))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
				if (memcmp(Frag.Fragment.pViews[i].Layers[k].pZs, pReadFrag->Fragment.pViews[i].Layers[k].pZs, sizeof(float) * Frag.Fragment.pViews[i].Layers[k].Count)) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
				for (j = 0; j < Frag.Fragment.pViews[i].TCount[k]; j++)
				{
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].Field, &pReadFrag->Fragment.pViews[i].pTracks[k][j].Field, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].Field))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].PointsN, &pReadFrag->Fragment.pViews[i].pTracks[k][j].PointsN, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].PointsN))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].Intercept, &pReadFrag->Fragment.pViews[i].pTracks[k][j].Intercept, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].Intercept))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].InterceptErrors, &pReadFrag->Fragment.pViews[i].pTracks[k][j].InterceptErrors, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].InterceptErrors))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].Slope, &pReadFrag->Fragment.pViews[i].pTracks[k][j].Slope, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].Slope))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].SlopeErrors, &pReadFrag->Fragment.pViews[i].pTracks[k][j].SlopeErrors, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].SlopeErrors))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].FirstZ, &pReadFrag->Fragment.pViews[i].pTracks[k][j].FirstZ, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].FirstZ))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].LastZ, &pReadFrag->Fragment.pViews[i].pTracks[k][j].LastZ, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].LastZ))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag.Fragment.pViews[i].pTracks[k][j].Sigma, &pReadFrag->Fragment.pViews[i].pTracks[k][j].Sigma, sizeof(Frag.Fragment.pViews[i].pTracks[k][j].Sigma))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (!pReadFrag->Fragment.pViews[i].pTracks[k][j].Valid) printf("Inconsistency in track section!", "Debug", MB_ICONERROR | MB_OK);
					if ((Frag.Fragment.pViews[i].pTracks[k][j].pCorrection == 0) ^ (pReadFrag->Fragment.pViews[i].pTracks[k][j].pCorrection == 0)) printf("Inconsistency in track section!", "Debug", MB_ICONERROR | MB_OK);
					if (Frag.Fragment.pViews[i].pTracks[k][j].pCorrection)
						if (memcmp(Frag.Fragment.pViews[i].pTracks[k][j].pCorrection, pReadFrag->Fragment.pViews[i].pTracks[k][j].pCorrection, Frag.Fragment.FitCorrectionDataSize)) printf("Correction section different!", "Debug", MB_ICONERROR | MB_OK);
					if (Frag.Fragment.CodingMode == VS_COMPRESSION_GRAIN_SUPPRESSION)
					{
						if (pReadFrag->Fragment.pViews[i].pTracks[k][j].pPoints) printf("Inconsistency in points section!", "Debug", MB_ICONERROR | MB_OK);
						}
					else
					{
						if ((Frag.Fragment.pViews[i].pTracks[k][j].pPoints == 0) ^ (pReadFrag->Fragment.pViews[i].pTracks[k][j].pPoints == 0)) printf("Inconsistency in points section!", "Debug", MB_ICONERROR | MB_OK);
						if (Frag.Fragment.pViews[i].pTracks[k][j].pPoints)
							if (memcmp(Frag.Fragment.pViews[i].pTracks[k][j].pPoints, pReadFrag->Fragment.pViews[i].pTracks[k][j].pPoints, sizeof(TVector) * Frag.Fragment.pViews[i].pTracks[k][j].PointsN)) printf("Points section different!", "Debug", MB_ICONERROR | MB_OK);
						};
					};
				};
			};

		if (pReadFrag) CoTaskMemFree(pReadFrag);

//		DeleteFile(m_Path);

		for (i = 0; i < Frag.Fragment.CountOfViews; i++)
		{
			for (k = 0; k < 2; k++)
			{
				if (Frag.Fragment.pViews[i].Layers[k].pZs) delete [] Frag.Fragment.pViews[i].Layers[k].pZs;
				if (Frag.Fragment.pViews[i].pTracks[k])
				{
					for (j = 0; j < Frag.Fragment.pViews[i].TCount[k]; j++)
					{
						if (Frag.Fragment.pViews[i].pTracks[k][j].pPoints) delete [] Frag.Fragment.pViews[i].pTracks[k][j].pPoints; 
						if (Frag.Fragment.pViews[i].pTracks[k][j].pCorrection) delete [] Frag.Fragment.pViews[i].pTracks[k][j].pCorrection; 
						};
					delete [] Frag.Fragment.pViews[i].pTracks[k];
					};
				};
			};
		delete [] Frag.Fragment.pViews;

		};

/* END FRAGMENT TEST */

/* ----- NEW ------------------------------------------------------- */

/* BEGIN CATALOG TEST */

	Cat.Hdr.ID.Part[0] = 2457;
	Cat.Hdr.ID.Part[1] = 1386;
	Cat.Hdr.ID.Part[2] = 2329894;
	Cat.Hdr.ID.Part[3] = 4309943;
	Cat.Hdr.Type.InfoType = VS_HEADER_BYTES | VS_CATALOG_SECTION;
	Cat.Hdr.Type.HeaderFormat = VS_HEADER_TYPE;
	for (i = 0; i < sizeof(Cat.Reserved); i++) Cat.Reserved[i] = i;
	Cat.Area.Fragments = m_Fragments;
	Cat.Area.XViews = m_Views;
	Cat.Area.YViews = m_Fragments;
	Cat.Area.XMin = (rand() % 1000) * 500.;
	Cat.Area.YMin = ((rand() ^ 0xffdba324) % 1000) * 500.;
	Cat.Area.XStep = 330.f;
	Cat.Area.YStep = 330.f;
	Cat.Area.XMax = Cat.Area.XMin + Cat.Area.XViews * (Cat.Area.XStep + 1);
	Cat.Area.YMax = Cat.Area.YMax + Cat.Area.YViews * (Cat.Area.YStep + 1);
	Cat.pFragmentIndices = new int[Cat.Area.XViews * Cat.Area.YViews];
	for (i = 0; i < (Cat.Area.XViews * Cat.Area.YViews); i++)
		Cat.pFragmentIndices[i] = i / Cat.Area.XViews + 1;
	Cat.Config.CountOfConfigs = rand() % 8;
	Cat.Config.pConfigs = Cat.Config.CountOfConfigs ? new VS_Config[Cat.Config.CountOfConfigs] : 0;
	for (i = 0; i < Cat.Config.CountOfConfigs; i++)
	{
		strcpy(Cat.Config.pConfigs[i].ClassName, RandomString(16));
		strcpy(Cat.Config.pConfigs[i].Config.Name, RandomString(16));
		Cat.Config.pConfigs[i].Config.CountOfItems = rand() % 32;
		Cat.Config.pConfigs[i].Config.pItems = Cat.Config.pConfigs[i].Config.CountOfItems ? 
			new char[Cat.Config.pConfigs[i].Config.CountOfItems * 
			(SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN)] : 0;
		for (j = 0; j < Cat.Config.pConfigs[i].Config.CountOfItems; j++)
		{
			strcpy(Cat.Config.pConfigs[i].Config.pItems + j * 
				(SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN), RandomString(16));
			strcpy(Cat.Config.pConfigs[i].Config.pItems + j * 
				(SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN) + 
				SYSAL_MAXCONFIG_ENTRY_NAME_LEN, RandomString(16));
			};
		};

	sprintf(TempPath,"prova2.rwc");
	if (iIO2->Write2(0, (BYTE *)&Cat, &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
	{
		printf("Error Writing", "Debug", MB_ICONERROR | MB_OK);
		};
	//TempPath.ReleaseBuffer();

	pReadCat = 0;
	if (iIO2->Read2(0, (BYTE *)(&pReadCat), &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
	{
		printf("Error Reading", "Debug", MB_ICONERROR | MB_OK);
		};
	//TempPath.ReleaseBuffer();

	if (memcmp(&Cat.Area, &pReadCat->Area, sizeof(Cat.Area))) printf("Area section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(&Cat.Hdr, &pReadCat->Hdr, sizeof(Cat.Hdr))) printf("Header section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(&Cat.Reserved, &pReadCat->Reserved, sizeof(Cat.Reserved))) printf("Reserved section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(Cat.pFragmentIndices, pReadCat->pFragmentIndices, sizeof(int) * Cat.Area.XViews * Cat.Area.YViews)) printf("Fragment index section different!", "Debug", MB_ICONERROR | MB_OK);
	if (memcmp(&Cat.Config.CountOfConfigs, &pReadCat->Config.CountOfConfigs, sizeof(Cat.Config.CountOfConfigs))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
	for (i = 0; i < Cat.Config.CountOfConfigs; i++)
	{
		if (memcmp(&Cat.Config.pConfigs[i].ClassName, &pReadCat->Config.pConfigs[i].ClassName, sizeof(Cat.Config.pConfigs[i].ClassName))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Cat.Config.pConfigs[i].Config.Name, &pReadCat->Config.pConfigs[i].Config.Name, sizeof(Cat.Config.pConfigs[i].Config.Name))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Cat.Config.pConfigs[i].Config.CountOfItems, &pReadCat->Config.pConfigs[i].Config.CountOfItems, sizeof(Cat.Config.pConfigs[i].Config.CountOfItems))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(Cat.Config.pConfigs[i].Config.pItems, pReadCat->Config.pConfigs[i].Config.pItems, Cat.Config.pConfigs[i].Config.CountOfItems * (SYSAL_MAXCONFIG_ENTRY_NAME_LEN + SYSAL_MAXCONFIG_ENTRY_VALUE_LEN))) printf("Config section different!", "Debug", MB_ICONERROR | MB_OK);
		};

	if (pReadCat) CoTaskMemFree(pReadCat);

	if (Cat.pFragmentIndices) delete [] Cat.pFragmentIndices;
	if (Cat.Config.pConfigs) 
	{
		for (i = 0; i < Cat.Config.CountOfConfigs; i++)
			if (Cat.Config.pConfigs[i].Config.pItems)
				delete [] Cat.Config.pConfigs[i].Config.pItems;
		delete [] Cat.Config.pConfigs;
		};

//	DeleteFile(m_Path);

/* END CATALOG TEST */

/* BEGIN FRAGMENT TEST */

	for (fi = 0; fi < Cat.Area.YViews; )
	{
		Frag2.Hdr.ID = Cat.Hdr.ID;
		Frag2.Hdr.Type.InfoType = VS_HEADER_BYTES | VS_FRAGMENT_SECTION;
		Frag2.Hdr.Type.HeaderFormat = VS_HEADER_TYPE;
		for (i = 0; i < sizeof(Frag2.Reserved); i++) Frag2.Reserved[i] = i;
		Frag2.Fragment.CountOfViews = Cat.Area.XViews;
		Frag2.Fragment.CodingMode = (rand() % 2) ? VS_COMPRESSION_NULL : VS_COMPRESSION_GRAIN_SUPPRESSION;
		Frag2.Fragment.FitCorrectionDataSize = 0;//((rand() % 4) >> 1) ? 16 : 0;
		Frag2.Fragment.StartView = fi * Cat.Area.XViews;
		Frag2.Fragment.Index = fi + 1;
		Frag2.Fragment.pViews = new VS_View2[Cat.Area.XViews];
		for (i = 0; i < Cat.Area.XViews; i++)
		{
			Frag2.Fragment.pViews[i].TileX = i;
			Frag2.Fragment.pViews[i].TileY = fi;
			Frag2.Fragment.pViews[i].RelevantZs.TopExt = 125.f + (rand() % 16) * .5f;
			Frag2.Fragment.pViews[i].RelevantZs.TopInt = Frag2.Fragment.pViews[i].RelevantZs.TopExt - 25.f;
			Frag2.Fragment.pViews[i].RelevantZs.BottomInt = Frag2.Fragment.pViews[i].RelevantZs.TopInt - 200.f;
			Frag2.Fragment.pViews[i].RelevantZs.BottomExt = Frag2.Fragment.pViews[i].RelevantZs.BottomInt - 25.f;
			for (k = 0; k < 2; k++)
			{
				Frag2.Fragment.pViews[i].X[k] = Cat.Area.XStep * Frag2.Fragment.pViews[i].TileX + (rand() % 101 - 50) * .02;
				Frag2.Fragment.pViews[i].Y[k] = Cat.Area.YStep * Frag2.Fragment.pViews[i].TileY + (rand() % 101 - 50) * .02;
				Frag2.Fragment.pViews[i].MapX[k] = Frag2.Fragment.pViews[i].X[k];
				Frag2.Fragment.pViews[i].MapY[k] = Frag2.Fragment.pViews[i].Y[k];
				Frag2.Fragment.pViews[i].ImageMat[k][0][0] = Frag2.Fragment.pViews[i].ImageMat[k][1][1] = 1.;
				Frag2.Fragment.pViews[i].ImageMat[k][0][1] = Frag2.Fragment.pViews[i].ImageMat[k][1][0] = 0.;
				Frag2.Fragment.pViews[i].Layers[k].Count = 12;
				Frag2.Fragment.pViews[i].Layers[k].pLayerInfo = new VS_View2::t_LayerInfo[Frag2.Fragment.pViews[i].Layers[k].Count];
				for (j = 0; j < Frag2.Fragment.pViews[i].Layers[k].Count; j++)
				{
					Frag2.Fragment.pViews[i].Layers[k].pLayerInfo[j].Clusters = 500 + rand() % 1000;
					Frag2.Fragment.pViews[i].Layers[k].pLayerInfo[j].Z = (k ? Frag2.Fragment.pViews[i].RelevantZs.BottomInt : Frag2.Fragment.pViews[i].RelevantZs.TopExt) - j * 25.f / 11.f ;
					}
				Frag2.Fragment.pViews[i].Status[k] = rand() % 3;
				Frag2.Fragment.pViews[i].TCount[k] = rand() % (m_MaxTracks - m_MinTracks) + m_MinTracks;
				Frag2.Fragment.pViews[i].pTracks[k] = Frag2.Fragment.pViews[i].TCount[k] ? new Track2[Frag2.Fragment.pViews[i].TCount[k]] : 0;
				for (j = 0; j < Frag2.Fragment.pViews[i].TCount[k]; j++)
				{
					Track2 &T = Frag2.Fragment.pViews[i].pTracks[k][j];
					T.Field = i;
					T.AreaSum = 0;
					T.Grains = 8 + rand() % 4;
					T.LastZ = (k ? Frag2.Fragment.pViews[i].RelevantZs.BottomExt : Frag2.Fragment.pViews[i].RelevantZs.TopInt);
					T.FirstZ = T.LastZ + T.Grains * 25.f / 11.f;
					T.Intercept.X = rand() % (int)Cat.Area.XStep - Cat.Area.XStep * 0.5f;
					T.Intercept.Y = rand() % (int)Cat.Area.YStep - Cat.Area.YStep * 0.5f;
					T.Intercept.Z = (k ? Frag2.Fragment.pViews[i].RelevantZs.BottomInt : Frag2.Fragment.pViews[i].RelevantZs.TopInt);
					T.Slope.X = (rand() % 1001 - 500) * .001f;
					T.Slope.Y = (rand() % 1001 - 500) * .001f;
					T.Slope.Z = 1.f;
					T.Sigma = rand() % 1000 * 0.0001;
					T.Valid = true;
					T.pGrains = new Grain[T.Grains];
					int p;
					for (p = 0; p < T.Grains; p++)
					{
						T.pGrains[p].Area = 4 + rand() % 30;
						T.AreaSum += T.pGrains[p].Area;
						T.pGrains[p].X = (rand() % 101 - 99) * .01;
						T.pGrains[p].Y = (rand() % 101 - 99) * .01;
						T.pGrains[p].Z = Frag2.Fragment.pViews[i].Layers[k].pLayerInfo[p].Z;
						};
//					if (Frag2.Fragment.FitCorrectionDataSize)
//					{
//						T.pCorrection = new BYTE[Frag2.Fragment.FitCorrectionDataSize];
//						for (p = 0; p < Frag2.Fragment.FitCorrectionDataSize; T.pCorrection[p++] = rand() % 256);
//						}
//					else
					T.pCorrection = 0;
					};
				};
			};

		sprintf(TempPath,"%s2.rwd.%08X", m_Path, fi + 1);
		if (iIO2->Write2(0, (BYTE *)&Frag2, &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
		{
			printf("Error Writing", "Debug", MB_ICONERROR | MB_OK);
			};
		//TempPath.ReleaseBuffer();

		IO_VS_Fragment2 *pReadFrag2 = 0;
		if (iIO2->Read2(0, (BYTE *)&pReadFrag2, &ExtErrorInfo, (UCHAR *)TempPath) != S_OK)
		{
			printf("Error Reading", "Debug", MB_ICONERROR | MB_OK);
			};
		//TempPath.ReleaseBuffer();

		if (memcmp(&Frag2.Hdr, &pReadFrag2->Hdr, sizeof(Frag2.Hdr))) printf("Header section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag2.Reserved, &pReadFrag2->Reserved, sizeof(Frag2.Reserved))) printf("Reserved section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag2.Fragment.CodingMode, &pReadFrag2->Fragment.CodingMode, sizeof(Frag2.Fragment.CodingMode))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag2.Fragment.CountOfViews, &pReadFrag2->Fragment.CountOfViews, sizeof(Frag2.Fragment.CountOfViews))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag2.Fragment.FitCorrectionDataSize, &pReadFrag2->Fragment.FitCorrectionDataSize, sizeof(Frag2.Fragment.FitCorrectionDataSize))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag2.Fragment.Index, &pReadFrag2->Fragment.Index, sizeof(Frag2.Fragment.Index))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		if (memcmp(&Frag2.Fragment.StartView, &pReadFrag2->Fragment.StartView, sizeof(Frag2.Fragment.StartView))) printf("Fragment section different!", "Debug", MB_ICONERROR | MB_OK);
		for (i = 0; i < Frag2.Fragment.CountOfViews; i++)
		{
			if (memcmp(&Frag2.Fragment.pViews[i].RelevantZs, &pReadFrag2->Fragment.pViews[i].RelevantZs, sizeof(Frag2.Fragment.pViews[i].RelevantZs))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag2.Fragment.pViews[i].X, &pReadFrag2->Fragment.pViews[i].X, sizeof(Frag2.Fragment.pViews[i].X))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag2.Fragment.pViews[i].Y, &pReadFrag2->Fragment.pViews[i].Y, sizeof(Frag2.Fragment.pViews[i].Y))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag2.Fragment.pViews[i].TileX, &pReadFrag2->Fragment.pViews[i].TileX, sizeof(Frag2.Fragment.pViews[i].TileX))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag2.Fragment.pViews[i].TileY, &pReadFrag2->Fragment.pViews[i].TileY, sizeof(Frag2.Fragment.pViews[i].TileY))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag2.Fragment.pViews[i].TCount, &pReadFrag2->Fragment.pViews[i].TCount, sizeof(Frag2.Fragment.pViews[i].TCount))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			if (memcmp(&Frag2.Fragment.pViews[i].Status, &pReadFrag2->Fragment.pViews[i].Status, sizeof(Frag2.Fragment.pViews[i].Status))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
			for (k = 0; k < 2; k++)
			{
				if (memcmp(&Frag2.Fragment.pViews[i].Layers[k].Count, &pReadFrag2->Fragment.pViews[i].Layers[k].Count, sizeof(Frag2.Fragment.pViews[i].Layers[k].Count))) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
				if (memcmp(Frag2.Fragment.pViews[i].Layers[k].pLayerInfo, pReadFrag2->Fragment.pViews[i].Layers[k].pLayerInfo, sizeof(VS_View::t_Layers) * Frag2.Fragment.pViews[i].Layers[k].Count)) printf("View section different!", "Debug", MB_ICONERROR | MB_OK);
				for (j = 0; j < Frag2.Fragment.pViews[i].TCount[k]; j++)
				{
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].Field, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].Field, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].Field))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].AreaSum, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].AreaSum, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].AreaSum))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].Grains, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].Grains, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].Grains))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].Intercept, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].Intercept, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].Intercept))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].Slope, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].Slope, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].Slope))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].FirstZ, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].FirstZ, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].FirstZ))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].LastZ, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].LastZ, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].LastZ))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (memcmp(&Frag2.Fragment.pViews[i].pTracks[k][j].Sigma, &pReadFrag2->Fragment.pViews[i].pTracks[k][j].Sigma, sizeof(Frag2.Fragment.pViews[i].pTracks[k][j].Sigma))) printf("Track section different!", "Debug", MB_ICONERROR | MB_OK);
					if (!pReadFrag2->Fragment.pViews[i].pTracks[k][j].Valid) printf("Inconsistency in track section!", "Debug", MB_ICONERROR | MB_OK);
					if ((Frag2.Fragment.pViews[i].pTracks[k][j].pCorrection == 0) ^ (pReadFrag2->Fragment.pViews[i].pTracks[k][j].pCorrection == 0)) printf("Inconsistency in track section!", "Debug", MB_ICONERROR | MB_OK);
					if (Frag2.Fragment.pViews[i].pTracks[k][j].pCorrection)
						if (memcmp(Frag2.Fragment.pViews[i].pTracks[k][j].pCorrection, pReadFrag2->Fragment.pViews[i].pTracks[k][j].pCorrection, Frag2.Fragment.FitCorrectionDataSize)) printf("Correction section different!", "Debug", MB_ICONERROR | MB_OK);
					if (Frag2.Fragment.CodingMode == VS_COMPRESSION_GRAIN_SUPPRESSION)
					{
						if (pReadFrag2->Fragment.pViews[i].pTracks[k][j].pGrains) printf("Inconsistency in points section!", "Debug", MB_ICONERROR | MB_OK);
						}
					else
					{
						if ((Frag2.Fragment.pViews[i].pTracks[k][j].pGrains == 0) ^ (pReadFrag2->Fragment.pViews[i].pTracks[k][j].pGrains == 0)) printf("Inconsistency in points section!", "Debug", MB_ICONERROR | MB_OK);
						if (Frag2.Fragment.pViews[i].pTracks[k][j].pGrains)
							if (memcmp(Frag2.Fragment.pViews[i].pTracks[k][j].pGrains, pReadFrag2->Fragment.pViews[i].pTracks[k][j].pGrains, sizeof(TVector) * Frag2.Fragment.pViews[i].pTracks[k][j].Grains)) printf("Points section different!", "Debug", MB_ICONERROR | MB_OK);
						};
					};
				};
			};

		if (pReadFrag2) CoTaskMemFree(pReadFrag2);

//		DeleteFile(m_Path);

		for (i = 0; i < Frag2.Fragment.CountOfViews; i++)
		{
			for (k = 0; k < 2; k++)
			{
				if (Frag2.Fragment.pViews[i].Layers[k].pLayerInfo) delete [] Frag2.Fragment.pViews[i].Layers[k].pLayerInfo;
				if (Frag2.Fragment.pViews[i].pTracks[k])
				{
					for (j = 0; j < Frag2.Fragment.pViews[i].TCount[k]; j++)
					{
						if (Frag2.Fragment.pViews[i].pTracks[k][j].pGrains) delete [] Frag2.Fragment.pViews[i].pTracks[k][j].pGrains; 
						if (Frag2.Fragment.pViews[i].pTracks[k][j].pCorrection) delete [] Frag2.Fragment.pViews[i].pTracks[k][j].pCorrection; 
						};
					delete [] Frag2.Fragment.pViews[i].pTracks[k];
					};
				};
			};
		delete [] Frag2.Fragment.pViews;

		};

/* END FRAGMENT TEST */


	iIO2->Release();
	iIO->Release();
	return true;
#else
	cout <<"Back conversion not implemented for this OS" <<endl;
	return false;
#endif
}
