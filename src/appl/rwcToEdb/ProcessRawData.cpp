#include "ProcessRawData.h"


ProcessRawData::ProcessRawData()
{
	_fragment = 0;
	_catalog = 0;
	_clusters = false;
	_filterTracks = false;
}

ProcessRawData::~ProcessRawData()
{
	if (_fragment)
		delete _fragment;
	if (_catalog)
		delete _catalog;
	_fragment = 0;
	_catalog = 0;
}

void ProcessRawData::initCatalog()
{
	if (_catalog)
		delete _catalog;
	_catalog = new Catalog();
}
void ProcessRawData::initFragment()
{
	if (_fragment)
		delete _fragment;
	_fragment = new Fragment();
}
unsigned int ProcessRawData::getFragmentsNumberFromCatalog()
{
	if (_catalog)
		return _catalog->getNfragments();
	return 0;
}
bool ProcessRawData::readCatalog()
{
	std::ifstream infile(_catFileName.c_str(),std::ios::binary);
	
	if (infile.fail())
	{
		std::cout << "Failed to open file" << std::endl;
		return false;
	}
	initCatalog();
	
	char byteInfo;
	infile.read((char*)&byteInfo,SIZE_OF_CHAR);
	unsigned short headerFormat;
	infile.read((char*)&headerFormat,SIZE_OF_UNSIGNED_SHORT_INT);
	//if (something) format not supported!
	HeaderInfo id;
	infile.read((char*)&(id.part0),SIZE_OF_INT);
	infile.read((char*)&(id.part1),SIZE_OF_INT);
	infile.read((char*)&(id.part2),SIZE_OF_INT);
	infile.read((char*)&(id.part3),SIZE_OF_INT);

	std::cout << "infoType: " << byteInfo << "\nheaderFormat: " << headerFormat << std::endl;
	std::cout << "Part0: " << id.part0 << 
		 "\nPart1: " << id.part1 << 
		 "\nPart2: " << id.part2 << 
		 "\nPart3: " << id.part3 << std::endl;

	_catalog->setHeaderInfo(id); 
	double minx,miny,maxx,maxy,stepx,stepy;
	unsigned int xviews,yviews,nFragments;
	if (headerFormat == 1795) // NormalDouble = 0x705
	{
		
		infile.read((char*)&(minx),SIZE_OF_DOUBLE);
		_catalog->setXmin(minx);
		infile.read((char*)&(maxx),SIZE_OF_DOUBLE);
		_catalog->setXmax(maxx);
		infile.read((char*)&(miny),SIZE_OF_DOUBLE);
		_catalog->setYmin(miny);
		infile.read((char*)&(maxy),SIZE_OF_DOUBLE);
		_catalog->setYmax(maxy);
		infile.read((char*)&(stepx),SIZE_OF_DOUBLE);
		_catalog->setXstep(stepx);
		infile.read((char*)&(stepy),SIZE_OF_DOUBLE);
		_catalog->setYstep(stepy);
		infile.read((char*)&(xviews),SIZE_OF_UNSIGNED_INT);
		_catalog->setXviews(xviews);
		infile.read((char*)&(yviews),SIZE_OF_UNSIGNED_INT);
		_catalog->setYviews(yviews);
		infile.read((char*)&(nFragments),SIZE_OF_UNSIGNED_INT);
		_catalog->setNfragments(nFragments);
		_catalog->initFragmentIndexes(yviews,xviews);
		SetupStringRepresentation *set = _catalog->getSetup();
		set->name = "Setup Info";
		//unsigned char readUInt;
		unsigned int readUInt;
		infile.read((char*)&(readUInt),SIZE_OF_UNSIGNED_INT);
		set->length = readUInt;
		for (unsigned int i = 0; i < set->length; i++)
		{
			ConfigStringRepresentation cfg;
			std::string className; infile.read((char*)(&className),64);
			std::string name; infile.read((char*)(&name),64);
			infile.read((char*)&(readUInt),SIZE_OF_UNSIGNED_INT);
			cfg.className = className;
			cfg.name = name;
			cfg.length = readUInt; 
			std::cout << " === ClassName === \n\n" << className.c_str() << 
				"\n\n ==== name === \n\n" << name.c_str() << std::endl;
			for (unsigned int j=0; j<cfg.length; j++)
			{
				KeyStringRepresentation *keys = new KeyStringRepresentation();
				std::string name;   infile.read((char*)(&name),64);
				std::string value;  infile.read((char*)(&value),64);
				keys->name = name;
				keys->value = value;
				cfg.keys.push_back(*keys);
			}
		}
		for (unsigned int i = 0; i < _catalog->getXviews(); i++)
		{
			for (unsigned int j = 0; i < _catalog->getYviews(); j++)
			{
				unsigned int value; infile.read((char*)(&value),SIZE_OF_UNSIGNED_INT);
				_catalog->setFragmentIndexes(j,i,value);
			}
		}
		std::string reservedBytes;   infile.read((char*)(&reservedBytes),256);
		
	}
	else   // --- USED IN SCANGRID MODE ----
	{
		infile.read((char*)&(minx),SIZE_OF_INT);
		_catalog->setXmin(minx);
		infile.read((char*)&(maxx),SIZE_OF_INT);
		_catalog->setXmax(maxx);
		infile.read((char*)&(miny),SIZE_OF_INT);
		_catalog->setYmin(miny);
		infile.read((char*)&(maxy),SIZE_OF_INT);
		_catalog->setYmax(maxy);
		infile.read((char*)&(stepx),SIZE_OF_INT);
		_catalog->setXstep(stepx);
		infile.read((char*)&(stepy),SIZE_OF_INT);
		_catalog->setYstep(stepy);
		infile.read((char*)&(xviews),SIZE_OF_UNSIGNED_INT);
		_catalog->setXviews(xviews);
		infile.read((char*)&(yviews),SIZE_OF_UNSIGNED_INT);
		_catalog->setYviews(yviews);
		infile.read((char*)&(nFragments),SIZE_OF_UNSIGNED_INT);
		_catalog->setNfragments(nFragments);
		_catalog->initFragmentIndexes(yviews,xviews);
		SetupStringRepresentation *set = _catalog->getSetup();
		set->name = "Setup Info";
		unsigned int readUInt;
		infile.read((char*)&(readUInt),SIZE_OF_UNSIGNED_INT);
		set->length = readUInt;
		std::cout << "nFragments: " << nFragments << std::endl;
		for (unsigned int i = 0; i < set->length; i++)
		{
			ConfigStringRepresentation cfg;
			char className[65], name[65], value[65]; 
			infile.read((char*)(&className),64);
			infile.read((char*)(&name),64);
			cfg.className = className;
			cfg.name = name;
			//long a;
			unsigned int a;
			
			infile.read((char*)&(a),4);
			cfg.length = a;
			//std::cout << "a: " << a << std::endl;
			
			for (int j = 0; j < cfg.length; j++)
			{
				
				KeyStringRepresentation key;
				infile.read((char*)(&name),64);
				infile.read((char*)(&value),64);
				key.name = name;
				key.value = value;
				cfg.keys.push_back(key);
			}
			set->configs.push_back(cfg);
		}

		for (unsigned int i = 0; i < _catalog->getYviews(); i++)
		{
			for (unsigned int j = 0; j < _catalog->getXviews(); j++)
			{
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				_catalog->setFragmentIndexes(i,j,readUInt);			
				
			}

		}
		infile.seekg(256,std::ios::cur);
	}
	infile.close();
	return true;
}


bool ProcessRawData::readFragment()
{
	initFragment();
	
	
	View view; 
	std::vector<View> viewArr;
	std::vector<Track> trackArr;


	std::ifstream infile(_fragFileName.c_str(),std::ios::binary);
	if (infile.fail())
	{
		std::cout << "Failed to open file: " << _fragFileName.c_str() << std::endl;
		return false;
	}
	char infoType; 
	unsigned short headerFormat;
	int codingMode;
	unsigned int index, startView, nViews, fitCorrectionDataSize;
	
	infile.read((char*)&infoType,SIZE_OF_CHAR);
	infile.read((char*)&headerFormat,SIZE_OF_UNSIGNED_SHORT_INT);
	
// if header && infotype da implementare
	HeaderInfo id; 
	infile.read((char*)&id.part0,SIZE_OF_INT);
	infile.read((char*)&id.part1,SIZE_OF_INT);
	infile.read((char*)&id.part2,SIZE_OF_INT);
	infile.read((char*)&id.part3,SIZE_OF_INT);
	infile.read((char*)&index,SIZE_OF_UNSIGNED_INT);
	infile.read((char*)&startView,SIZE_OF_UNSIGNED_INT);
	infile.read((char*)&nViews,SIZE_OF_UNSIGNED_INT);
	infile.read((char*)&fitCorrectionDataSize,SIZE_OF_UNSIGNED_INT);
	infile.read((char*)&codingMode,SIZE_OF_INT);
	
	_fragment->setId(id);
	_fragment->setIndex(index);
	_fragment->setStartView(startView);
	_fragment->setViews(nViews);
	_fragment->setFragmentCoding(codingMode);

	
	if (!(codingMode == 0 || codingMode == 258)) // 0x0 == Normal, 0x102 == GrainSuppresion
	{
		std::cerr << "Unsupported fragment coding mode" << std::endl;
		return -3;
	}
	infile.seekg(256,std::ios::cur);
	
	if (headerFormat == 1795) // NormalDouble --> SCANGRID MODE
	{
		double readDouble;
		int readInt;
		char readChar;
		for (unsigned int i = 0; i < nViews; i++)
		{
			view.setId(i);
			Side topSide, bottomSide;
			infile.read((char*)&readInt,SIZE_OF_INT);
			view.setTileX(readInt);		
			infile.read((char*)&readInt,SIZE_OF_INT);
			view.setTileY(readInt);		
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			topSide.setStageFovPosX(readDouble); 
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			bottomSide.setStageFovPosX(readDouble); 
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			topSide.setStageFovPosY(readDouble); 
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			bottomSide.setStageFovPosY(readDouble); 
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE); 
			topSide.setMappedFovPosX(readDouble);   
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			bottomSide.setMappedFovPosX(readDouble);
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			topSide.setMappedFovPosY(readDouble);
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			bottomSide.setMappedFovPosY(readDouble);
			
			double mxx,mxy,myx,myy;
			infile.read((char*)&mxx,SIZE_OF_DOUBLE);
			infile.read((char*)&mxy,SIZE_OF_DOUBLE);
			infile.read((char*)&myx,SIZE_OF_DOUBLE);
			infile.read((char*)&myy,SIZE_OF_DOUBLE);
			topSide.setM(mxx,mxy,myx,myy);
			infile.read((char*)&mxx,SIZE_OF_DOUBLE);
			infile.read((char*)&mxy,SIZE_OF_DOUBLE);
			infile.read((char*)&myx,SIZE_OF_DOUBLE);
			infile.read((char*)&myy,SIZE_OF_DOUBLE);
			bottomSide.setM(mxx,mxy,myx,myy);
			infile.read((char*)&readInt,SIZE_OF_INT); 
			topSide.setLayers(readInt); 
			infile.read((char*)&readInt,SIZE_OF_INT); 
			bottomSide.setLayers(readInt); 
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			topSide.setTopZ(readDouble);
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			topSide.setBottomZ(readDouble);
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			bottomSide.setTopZ(readDouble);
			infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
			bottomSide.setBottomZ(readDouble);
			
			infile.read((char*)&readChar,SIZE_OF_CHAR);
			topSide.setFlag(readChar);
			infile.read((char*)&readChar,SIZE_OF_CHAR);
			bottomSide.setFlag(readChar);
			
			infile.read((char*)&readInt,SIZE_OF_INT); 
			topSide.setTracks(readInt);
			infile.read((char*)&readInt,SIZE_OF_INT); 
			bottomSide.setTracks(readInt);
						
			view.setTopSide(topSide); 
			view.setBottomSide(bottomSide); 
			viewArr.push_back(view);
		}
		for (unsigned int i = 0; i < nViews; i++)
		{
			
			for (int j = 0; j < viewArr.at(i).getTopSide()->getLayers(); j++)
			{
				unsigned int grains;
				double z;
				if(headerFormat == 1794 || headerFormat == 1795)
				{
					infile.read((char*)&grains,SIZE_OF_UNSIGNED_INT);
				}
				else 
					grains = 0;
				infile.read((char*)&z,SIZE_OF_DOUBLE);
				viewArr.at(i).getTopSide()->addLayer(grains,z);
				
			}
			
			for (int j = 0; j < viewArr.at(i).getBottomSide()->getLayers(); j++)
			{
				
				unsigned int grains;
				double z;
				if(headerFormat == 1794 || headerFormat == 1795)
				{
					infile.read((char*)&grains,SIZE_OF_UNSIGNED_INT);
				}
				else 
					grains = 0;
				infile.read((char*)&z,SIZE_OF_DOUBLE);
				viewArr.at(i).getBottomSide()->addLayer(grains,z);
			
			}
			
		}
		for (unsigned int i = 0; i < nViews; i++)
		{
			Track tr; 
			unsigned int readUInt;
			for (int j = 0; j < viewArr.at(i).getTopSide()->getTracks(); j++)
			{
				
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setAreaSum(readUInt);  
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setCount(static_cast<short>(readUInt)); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setX(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setY(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setZ(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSX(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSY(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSZ(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSigma(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setTopZ(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setBottomZ(readDouble); 
				viewArr.at(i).getTopSide()->addTrack(tr);
						
			}
			for (int j = 0; j < viewArr.at(i).getBottomSide()->getTracks(); j++)
			{
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setAreaSum(readUInt);
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setCount(static_cast<short>(readUInt));
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setX(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setY(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setZ(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSX(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSY(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSZ(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setSigma(readDouble); 
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setTopZ(readDouble);
				infile.read((char*)&readDouble,SIZE_OF_DOUBLE);
				tr.setBottomZ(readDouble);
				viewArr.at(i).getBottomSide()->addTrack(tr);
			}
		}
		if (codingMode == 0)
		{
		
			for (unsigned int i = 0; i < nViews; i++)
			{
				for (int j = 0; j < viewArr.at(i).getTopSide()->getTracks(); j++)
				{
					Grain gr;
					Track *tr = viewArr.at(i).getTopSide()->getTrackAt(j);
					for (int k = 0; k < tr->getCount(); k++)
					{
						if (headerFormat == 1795)
						{
							unsigned int readUInt;
							infile.read((char*)&readUInt,4);
							gr.area = readUInt;
						}
						else
							gr.area = 0;
						infile.read((char*)&readDouble,8);
						gr.x = readDouble;	
						infile.read((char*)&readDouble,8);
						gr.y = readDouble;	
						infile.read((char*)&readDouble,8);
						gr.z = readDouble;	
						tr->addGrain(gr);
					}
					if (_filterTracks)
						tr->evaluateEstimators();
				}
				for (int j = 0; j < viewArr.at(i).getBottomSide()->getTracks(); j++)
				{
						Grain gr;
					Track *tr = viewArr.at(i).getBottomSide()->getTrackAt(j);
					for (int k = 0; k < tr->getCount(); k++)
					{
						if (headerFormat == 1795)
						{
							unsigned int readUInt;
							infile.read((char*)&readUInt,4);
							gr.area = readUInt;
						}
						else
							gr.area = 0;
						infile.read((char*)&readDouble,8);
						gr.x = readDouble;	
						infile.read((char*)&readDouble,8);
						gr.y = readDouble;	
						infile.read((char*)&readDouble,8);
						gr.z = readDouble;	
						tr->addGrain(gr);
					}
					if (_filterTracks)
						tr->evaluateEstimators();
				}
			}
		}
	}
	else
	{
		

		float readFloat;
		int readInt;



		for (unsigned int i = 0; i < nViews; i++)
		{
			view.setId(i);
			Side topSide, bottomSide;
			infile.read((char*)&readInt,SIZE_OF_INT);
			view.setTileX(readInt);		
			infile.read((char*)&readInt,SIZE_OF_INT);
			view.setTileY(readInt);	
			
			
			infile.read((char*)&readFloat,4);
			topSide.setStageFovPosX(readFloat); 
			infile.read((char*)&readFloat,4);
			bottomSide.setStageFovPosX(readFloat); 
			infile.read((char*)&readFloat,4);
			topSide.setStageFovPosY(readFloat); 
			infile.read((char*)&readFloat,4);
			bottomSide.setStageFovPosY(readFloat); 
			infile.read((char*)&readFloat,4);
			topSide.setMappedFovPosX(readFloat);   
			infile.read((char*)&readFloat,4);
			bottomSide.setMappedFovPosX(readFloat);
			infile.read((char*)&readFloat,4);
			topSide.setMappedFovPosY(readFloat);
			infile.read((char*)&readFloat,4);
			bottomSide.setMappedFovPosY(readFloat);
			
			float mxx4bytes,mxy4bytes,myx4bytes,myy4bytes;
			infile.read((char*)&mxx4bytes,4);
			infile.read((char*)&mxy4bytes,4);
			infile.read((char*)&myx4bytes,4);
			infile.read((char*)&myy4bytes,4);
			topSide.setM(mxx4bytes,mxy4bytes,myx4bytes,myy4bytes);
			
			infile.read((char*)&mxx4bytes,4);
			infile.read((char*)&mxy4bytes,4);
			infile.read((char*)&myx4bytes,4);
			infile.read((char*)&myy4bytes,4);
			bottomSide.setM(mxx4bytes,mxy4bytes,myx4bytes,myy4bytes);

			infile.read((char*)&readInt,SIZE_OF_INT); 
			topSide.setLayers(readInt); 
			infile.read((char*)&readInt,SIZE_OF_INT); 
			bottomSide.setLayers(readInt); 

			
			infile.read((char*)&readFloat,4);
			topSide.setTopZ(readFloat);
			infile.read((char*)&readFloat,4);
			topSide.setBottomZ(readFloat);
			infile.read((char*)&readFloat,4);
			bottomSide.setTopZ(readFloat);
			infile.read((char*)&readFloat,4);
			bottomSide.setBottomZ(readFloat);
			
			unsigned int readUInt;
			infile.read((char*)&readUInt,SIZE_OF_CHAR);
			topSide.setFlag(readUInt);
			infile.read((char*)&readUInt,SIZE_OF_CHAR);
			bottomSide.setFlag(readUInt);
			
			infile.read((char*)&readInt,SIZE_OF_INT); 
			topSide.setTracks(readInt);
			infile.read((char*)&readInt,SIZE_OF_INT); 
			bottomSide.setTracks(readInt);
						
			view.setTopSide(topSide); 
			view.setBottomSide(bottomSide); 
			viewArr.push_back(view);
		}
		for (unsigned int i = 0; i < nViews; i++)
		{
			
			for (int j = 0; j < viewArr.at(i).getTopSide()->getLayers(); j++)
			{
				unsigned int grains;
				double z;
				if(headerFormat == 1794 || headerFormat == 1795)
				{
					infile.read((char*)&grains,SIZE_OF_UNSIGNED_INT);
				}
				else 
					grains = 0;
				infile.read((char*)&readFloat,4);
				z = readFloat;
				viewArr.at(i).getTopSide()->addLayer(grains,z);
				
			}
			
			for (int j = 0; j < viewArr.at(i).getBottomSide()->getLayers(); j++)
			{
				
				unsigned int grains;
				double z;
				if(headerFormat == 1794 || headerFormat == 1795)
				{
					infile.read((char*)&grains,SIZE_OF_UNSIGNED_INT);
				}
				else 
					grains = 0;
				infile.read((char*)&readFloat,4);
				z = readFloat;
				viewArr.at(i).getBottomSide()->addLayer(grains,z);
			
			}
			
		}
		for (unsigned int i = 0; i < nViews; i++)
		{
			Track tr; 
			unsigned int readUInt;
			for (int j = 0; j < viewArr.at(i).getTopSide()->getTracks(); j++)
			{
				
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setAreaSum(readUInt);  
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setCount(static_cast<short>(readUInt)); 
				
				infile.read((char*)&readFloat,4);
				tr.setX(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setY(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setZ(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setSX(readFloat);
				infile.read((char*)&readFloat,4);
				tr.setSY(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setSZ(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setSigma(readFloat);
				infile.read((char*)&readFloat,4);
				tr.setTopZ(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setBottomZ(readFloat); 
				if(headerFormat  == 1793) //old format
				{
					infile.read((char*)&readFloat,4);infile.read((char*)&readFloat,4);
					infile.read((char*)&readFloat,4);infile.read((char*)&readFloat,4);
					infile.read((char*)&readFloat,4);infile.read((char*)&readFloat,4);
					infile.read((char*)&readFloat,SIZE_OF_UNSIGNED_INT);
					
				}
				viewArr.at(i).getTopSide()->addTrack(tr);
				
						
			}
			for (int j = 0; j < viewArr.at(i).getBottomSide()->getTracks(); j++)
			{
				
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setAreaSum(readUInt);  
				infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
				tr.setCount(static_cast<short>(readUInt)); 
				
				infile.read((char*)&readFloat,4);
				tr.setX(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setY(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setZ(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setSX(readFloat);
				infile.read((char*)&readFloat,4);
				tr.setSY(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setSZ(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setSigma(readFloat);
				infile.read((char*)&readFloat,4);
				tr.setTopZ(readFloat); 
				infile.read((char*)&readFloat,4);
				tr.setBottomZ(readFloat); 
				if(headerFormat  == 1793) //old format
				{
					infile.read((char*)&readFloat,4);infile.read((char*)&readFloat,4);
					infile.read((char*)&readFloat,4);infile.read((char*)&readFloat,4);
					infile.read((char*)&readFloat,4);infile.read((char*)&readFloat,4);
					infile.read((char*)&readFloat,SIZE_OF_UNSIGNED_INT);
					
				}
				viewArr.at(i).getBottomSide()->addTrack(tr);
				
			}
		}
		if (codingMode == 0)
		{
				for (unsigned int i = 0; i < nViews; i++)
				{
					for (int j = 0; j < viewArr.at(i).getTopSide()->getTracks(); j++)
					{
						Grain gr;
						Track *tr = viewArr.at(i).getTopSide()->getTrackAt(j);
						for (int k = 0; k < tr->getCount(); k++)
						{
							if (headerFormat == 1794)
							{
								unsigned int readUInt;
								infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
								gr.area = readUInt;
							}
							else
								gr.area = 0;
							infile.read((char*)&readFloat,4);
							gr.x = readFloat;	
							infile.read((char*)&readFloat,4);
							gr.y = readFloat;
							infile.read((char*)&readFloat,4);
							gr.z = readFloat;
							tr->addGrain(gr);
						}
						if (_filterTracks)
							tr->evaluateEstimators();
						
					}
					for (int j = 0; j < viewArr.at(i).getBottomSide()->getTracks(); j++)
					{
							Grain gr;
						Track *tr = viewArr.at(i).getBottomSide()->getTrackAt(j);
						for (int k = 0; k < tr->getCount(); k++)
						{
							if (headerFormat == 1794)
							{
								unsigned int readUInt;
								infile.read((char*)&readUInt,SIZE_OF_UNSIGNED_INT);
								gr.area = readUInt;
							}
							else
								gr.area = 0;
							infile.read((char*)&readFloat,4);
							gr.x = readFloat;	
							infile.read((char*)&readFloat,4);
							gr.y = readFloat;
							infile.read((char*)&readFloat,4);
							gr.z = readFloat;
							tr->addGrain(gr);
						}
						if (_filterTracks)
							tr->evaluateEstimators();
					
				}
			}
				

		}
	}
	_fragment->setViewsArray(viewArr);

	infile.close();
	return true;
	
}

bool ProcessRawData::dumpCatalogInEdbStructure()
{
	EdbRun *run = new EdbRun(_outputRootFileName.c_str(), "RECREATE" );
	
    
    run->GetTree()->SetMaxTreeSize(static_cast<long long>(10e9)); 
	EdbRunHeader *Header = run->GetHeader();
	Header->SetFlag(0,2);  
	Header->SetFlag(1,1);
	Header->SetFlag(2,1);
	Header->SetLimits(static_cast<float>(_catalog->getXmin()),
		static_cast<float>(_catalog->getXmax()),
		static_cast<float>(_catalog->getYmin()),
		static_cast<float>(_catalog->getYmax()));
	int index = _catalog->findConfig("Vertigo");
	std::string retValue = _catalog->getConfigValue(_catalog->findConfig("Vertigo"),"VLayers");
			

	Header->SetArea(static_cast<int>(_catalog->getXviews()*_catalog->getYviews()), 
		  static_cast<float>(_catalog->getXstep()),static_cast<float>(_catalog->getYstep()) 
		  , atoi(retValue.c_str()),atoi(retValue.c_str()), 0); 
	
	 Header->SetCCD(atoi(_catalog->getConfigValue(_catalog->findConfig("Objective"),"Width").c_str()),
		 atoi(_catalog->getConfigValue(_catalog->findConfig("Objective"),"Height").c_str()),
		 -999,		// physical pixel size in microns along X
		 -999,		// physical pixel size in microns along Y
		 "",		// es. "Dalsa CAD4"
		 "");		// es. "Bologna"
	Header->SetObjective(-999,		// magnification  
		       static_cast<float>(atof(_catalog->getConfigValue(_catalog->findConfig("Objective"),"Width").c_str())*
		       fabs(atof(_catalog->getConfigValue(_catalog->findConfig("Objective"),"PixelToMicronX").c_str()))), 
		       0,0,
		       static_cast<float>(atof(_catalog->getConfigValue(_catalog->findConfig("Objective"),"Height").c_str())*
		       fabs(atof(_catalog->getConfigValue(_catalog->findConfig("Objective"),"PixelToMicronY").c_str()))),
		       0,0,
		       "",					// es. "Nikon CFI - oil"
		       "");				   // es, "50x"
	float vStepTop = static_cast<float>(atof(_catalog->getConfigValue(_catalog->findConfig("Vertigo"),"TopStep").c_str()));
	float vStepBottom = static_cast<float>(atof(_catalog->getConfigValue(_catalog->findConfig("Vertigo"),"BottomStep").c_str()));
	float shr = static_cast<float>(atof(_catalog->getConfigValue(_catalog->findConfig("Vertigo"),"Shrinkage").c_str()));
	float base = static_cast<float>(atof(_catalog->getConfigValue(_catalog->findConfig("Vertigo"),"BaseThickness").c_str()));
	Header->SetPlate(-999,		// plate ID
		   vStepTop*shr,
		   base,
		   vStepBottom*shr,shr,shr,
		   "",					// es. "Test Plate"
		   "");	
	// Store the catalog file as TObjString 
  TObjString objstr ;
  int  i, ch;
  FILE *stream;
  if( (stream = fopen( _catFileName.c_str(), "rb" )) == NULL ) return false;
  ch = fgetc( stream );
  for( i=0;  feof( stream ) == 0 ; i++ )
    {
      objstr.String().Append( ch ); 
      ch = fgetc( stream );
    }
  fclose( stream );
  objstr.Write("catalog"); 
  run->Save() ;
  run->Close();
  return true;
}

bool ProcessRawData::dumpFragmentInEdbStructure()
{
	EdbRun* run = 0;
    if ( gSystem->AccessPathName( _outputRootFileName.c_str() ) )
      run = new EdbRun( _outputRootFileName.c_str(), "RECREATE" );
    else

		run = new EdbRun( _outputRootFileName.c_str(), "UPDATE" );
    run->GetTree()->SetMaxTreeSize(static_cast<long long>(10e9));   
   
	EdbView*    edbView = run->GetView();
   EdbSegment* edbSegment = new EdbSegment(0,0,0,0,0,0,0,0);
   int v, t, p;   // v=view, s=side, t=track, p=point
   int tracks;		   // number of tracks in the fragment
   int fclusters;	   // number of clusters in the fragment 
   int vclusters;	   // number of clusters in the view
   float dz	;		   // z-length of the track segment
   int tr_clusters;	// number of cluster of the track
   int puls ;        // ePuls = (number of clusters ) OR  (sum of clust areas)*1000 + (number of clusters)
   float volume = 0;
   int frameId = 0;
   tracks = 0;
   fclusters = 0;
   run->GetHeader()->SetNareas(run->GetHeader()->GetNareas()+1);
   for (v = 0; v < _fragment->getViews(); v++) //TOP
   {
	   View *rwdView = _fragment->getView(v);
	   Side *topSide = rwdView->getTopSide();
	   vclusters = 0;
	   tracks+=topSide->getTracks();
	   edbView->Clear();
	   EdbViewHeader* edbViewHeader = edbView->GetHeader();
	   edbViewHeader->SetAffine(static_cast<float>(topSide->getMXX()),
		   static_cast<float>(topSide->getMXY()),static_cast<float>(topSide->getMYX()),
		   static_cast<float>(topSide->getMYY()),static_cast<float>(topSide->getMappedFovPosX()),
		   static_cast<float>(topSide->getMappedFovPosY()));
	
	   edbViewHeader->SetAreaID(_fragment->getIndex());
	   edbViewHeader->SetCoordXY(static_cast<float>(topSide->getStageFovPosX()), static_cast<float>(topSide->getStageFovPosY())); 
	   edbViewHeader->SetCoordZ(static_cast<float>(topSide->getTopZ()),static_cast<float>(topSide->getBottomZ()),
			       static_cast<float>(rwdView->getBottomSide()->getTopZ()),static_cast<float>(rwdView->getBottomSide()->getBottomZ())); // NON capito i relevant Z

	   edbViewHeader->SetNframes(topSide->getLayers(),0);
	   edbViewHeader->SetNsegments(topSide->getTracks());
	   edbViewHeader->SetViewID(rwdView->getId());
	   edbViewHeader->SetColRow(rwdView->getTileX()  ,rwdView->getTileY() );
	   int nframes = topSide->getLayers();
	   for( int nlvl=0; nlvl<nframes; nlvl++ )
	   {
	      edbView->AddFrame(nlvl,static_cast<float>(topSide->getLayerAt(nlvl)->zLayer),
			        topSide->getLayerAt(nlvl)->grains);
	   }
	   for (t = 0; t < topSide->getTracks(); t++) {
		   Track *tr = topSide->getTrackAt(t);
		   tr_clusters = tr->getCount();
		   puls = tr->getCount(); // dovrei mettere l'if per le opzioni di rwc2edb
		   if (tr->getGrains().size()!=0)
			   dz = static_cast<float>(tr->getGrains().at(0).z - tr->getGrains().at(tr_clusters-1).z);
		   else dz = 0.;

		   edbSegment->Set(  static_cast<float>(tr->getX()),static_cast<float>(tr->getY()),
	                           static_cast<float>(tr->getZ()),static_cast<float>(tr->getSX()),static_cast<float>(tr->getSY()),
	                           dz, 0 , puls, t);
		   edbSegment->SetSigma(static_cast<float>(tr->getSigma()),-999.);
		   if (tr->getGrains().size()!=0 ) {       // fixare opzioni nocl
               volume = 0;
               frameId  = 0;
		   }
		   if (_clusters)
		   {
			   for ( p=0; p<tr_clusters;p++)  {
				   for(int nlvl=frameId;nlvl<nframes;nlvl++) {
					   if(tr->getGrains().at(p).z == rwdView->getTopSide()->getLayerAt(nlvl)->zLayer) {
							   frameId=nlvl;
							   break;
							}
				   }
				   
				   edbView->AddCluster( static_cast<float>(tr->getGrains().at(p).x),
										   static_cast<float>(tr->getGrains().at(p).y),
										   static_cast<float>(tr->getGrains().at(p).z),
										   static_cast<float>(tr->getGrains().at(p).area),
										   volume,
										   frameId,
										   0,t);										
										   
			   }
		   }
	   	   edbViewHeader->SetNclusters(vclusters);
           edbView->AddSegment(edbSegment) ;
           vclusters += tr_clusters;
	   } // end of tracks (t)
	   run->AddView(edbView);
       fclusters += vclusters;
   } //end of views (v)
   



   for (v = 0; v < _fragment->getViews(); v++) //BOTTOM
   {
	   View *rwdView = _fragment->getView(v);
	   Side *bottomSide = rwdView->getBottomSide();
	   vclusters = 0;
	   tracks+=bottomSide->getTracks();
	   edbView->Clear();
	   EdbViewHeader* edbViewHeader = edbView->GetHeader();
	   edbViewHeader->SetAffine(static_cast<float>(bottomSide->getMXX()),static_cast<float>(bottomSide->getMXY()),
		   static_cast<float>(bottomSide->getMYX()),static_cast<float>(bottomSide->getMYY()),
		   static_cast<float>(bottomSide->getMappedFovPosX()),static_cast<float>(bottomSide->getMappedFovPosY()));
	   //edbViewHeader->SetAreaID(run->GetHeader()->GetNareas());
	   edbViewHeader->SetAreaID(_fragment->getIndex());
	   edbViewHeader->SetCoordXY(static_cast<float>(bottomSide->getStageFovPosX()), static_cast<float>(bottomSide->getStageFovPosY())); 
	   edbViewHeader->SetCoordZ(static_cast<float>(rwdView->getTopSide()->getTopZ()),static_cast<float>(rwdView->getTopSide()->getBottomZ()),
			       static_cast<float>(rwdView->getBottomSide()->getTopZ()),static_cast<float>(rwdView->getBottomSide()->getBottomZ())); // NON capito i relevant Z
	   //edbViewHeader->SetNframes(bottomSide->getLayers(),0);
	   edbViewHeader->SetNframes(0,bottomSide->getLayers());
	   edbViewHeader->SetNsegments(bottomSide->getTracks());
	   edbViewHeader->SetViewID(rwdView->getId());
	   edbViewHeader->SetColRow(rwdView->getTileX()  ,rwdView->getTileY() );
	   int nframes = bottomSide->getLayers();
	   for( int nlvl=0; nlvl<nframes; nlvl++ )
	   {
	      edbView->AddFrame(nlvl,static_cast<float>(bottomSide->getLayerAt(nlvl)->zLayer),
			        bottomSide->getLayerAt(nlvl)->grains);
	   }
	   for (t = 0; t < bottomSide->getTracks(); t++) {
		   Track *tr = bottomSide->getTrackAt(t);
		   tr_clusters = tr->getCount();
		   puls = tr->getCount(); // dovrei mettere l'if per le opzioni di rwc2edb
		   if (tr->getGrains().size()!=0)
			   dz = static_cast<float>(tr->getGrains().at(0).z - tr->getGrains().at(tr_clusters-1).z);
		   else dz = 0;

		   edbSegment->Set( static_cast<float>(tr->getX()),static_cast<float>(tr->getY()),
	                           static_cast<float>(tr->getZ()),static_cast<float>(tr->getSX()),
							   static_cast<float>(tr->getSY()), dz, 1 , puls, t);
		   edbSegment->SetSigma(static_cast<float>(tr->getSigma()),-999.);
		   if (tr->getGrains().size()!=0 ) {       // fixare opzioni nocl
               volume = 0;
               frameId  = 0;
		   }
		   if (_clusters)
		   {
			   for ( p=0; p<tr_clusters;p++)  {
				   for(int nlvl=frameId;nlvl<nframes;nlvl++) {
					   if(tr->getGrains().at(p).z == rwdView->getTopSide()->getLayerAt(nlvl)->zLayer) {
							   frameId=nlvl;
							   break;
							}
						 }
					 edbView->AddCluster( static_cast<float>(tr->getGrains().at(p).x),
										  static_cast<float>(tr->getGrains().at(p).y),
										   static_cast<float>(tr->getGrains().at(p).z),
										   static_cast<float>(tr->getGrains().at(p).area),
										   volume,
										   frameId,
										   1,t);										
			   }
		   }
	   	   edbViewHeader->SetNclusters(vclusters);
           edbView->AddSegment(edbSegment) ;
           vclusters += tr_clusters;
	   } // end of tracks (t)
	   run->AddView(edbView);
       fclusters += vclusters;
   } //end of views (v)

   run->Close();
   std::cout<< "Fragment: " << _fragment->getIndex() << "\tmicrotracks: " << tracks << "\tclusters: " << fclusters << std::endl;
   return true;
}

int ProcessRawData::makeTracksPlot(const char *plotFileName)
{
	int nViews = _fragment->getViews();
	int counter = 0;
	for (int i = 0; i < nViews; i++)
	{
		counter += _fragment->getView(i)->getTopSide()->getTracks();
		counter += _fragment->getView(i)->getBottomSide()->getTracks();
	}
	if (counter == 0)
		return 0;
	
	double x0,y0,z0,tx,ty,dz,sigma; 				   // segments parameters
	unsigned int side,puls,id;      				   // segments parameters
	unsigned int areasum;           				   // segments parameters
	double meanDeltaTheta3D;        				   // mean 3D angular variation (computed between adiacent clusters)
	double sigmaDeltaTheta3D;       				   // sigma 3D angular variation (computed between adiacent clusters)
	double meanDeltaThetaXZ;	    				   // mean 2D angular variation - plane XZ
	double sigmaDeltaThetaXZ;						   // sigma 2D angular variation - plane XZ
	double meanDeltaThetaYZ;        				   // mean 2D angular variation - plane YZ
	double sigmaDeltaThetaYZ;						   // sigma 2D angular variation - plane YZ
	double meanDistanceClustersTo3DLine;               // mean Distance beetween cluster and 3D line fit
	double meanGapClusterToCluster;                    // mean gap beetween adiacent clusters
	double mx, mxSigma, qx, qxSigma, rx, xCov, xChi2;  // 2D fit parameters for XZ plane
	double my, mySigma, qy, qySigma, ry, yCov, yChi2;  // 2D fit parameters for YZ plane
	
	TTree *tree = 0;
	TFile *outf = 0;

	if(gSystem->AccessPathName(plotFileName))
    { 

		outf = new TFile(plotFileName,"UPDATE");                 // update an existing file (to super-impose all fragments in the same file)
		tree = new TTree("plots","plots");

		tree->Branch("x0",&x0,"x0/D");						   
		tree->Branch("y0",&y0,"y0/D");						   
		tree->Branch("z0",&z0,"z0/D");						   
		tree->Branch("tx",&tx,"tx/D");						   
		tree->Branch("ty",&ty,"ty/D");						   
		tree->Branch("dz",&dz,"dz/D");						      
		tree->Branch("side",&side,"side/i");						
		tree->Branch("puls",&puls,"puls/i");					
		tree->Branch("id",&id,"id/i");				
		tree->Branch("areasum",&areasum,"areasum/i");
		tree->Branch("sigma",&sigma,"sigma/D");		
		tree->Branch("meanDeltaTheta3D",&meanDeltaTheta3D,"meanDeltaTheta3D/D");				
		tree->Branch("sigmaDeltaTheta3D",&sigmaDeltaTheta3D,"sigmaDeltaTheta3D/D");				
		tree->Branch("meanDeltaThetaXZ",&meanDeltaThetaXZ,"meanDeltaThetaXZ/D");				
		tree->Branch("sigmaDeltaThetaXZ",&sigmaDeltaThetaXZ,"sigmaDeltaThetaXZ/D");			    
		tree->Branch("meanDeltaThetaYZ",&meanDeltaThetaYZ,"meanDeltaThetaYZ/D");				
		tree->Branch("sigmaDeltaThetaYZ",&sigmaDeltaThetaYZ,"sigmaDeltaThetaYZ/D");			    
		tree->Branch("meanDistanceClustersTo3DLine",&meanDistanceClustersTo3DLine,"meanDistanceClustersTo3DLine/D");				     
		tree->Branch("meanGapClusterToCluster",&meanGapClusterToCluster,"meanGapClusterToCluster/D");				     
		tree->Branch("mx",&mx,"mx/D");						      
		tree->Branch("mxSigma",&mxSigma,"mxSigma/D");			 
		tree->Branch("qx",&qx,"qx/D");						      
		tree->Branch("qxSigma",&qxSigma,"qxSigma/D");			
		tree->Branch("xCov",&xCov,"xCov/D");					
		tree->Branch("xChi2",&xChi2,"xChi2/D");					
		tree->Branch("rx",&rx,"rx/D");						      
		tree->Branch("my",&my,"my/D");						      
		tree->Branch("qy",&qy,"qy/D");						      
		tree->Branch("mySigma",&mySigma,"mySigma/D");			
		tree->Branch("qySigma",&qySigma,"qySigma/D");			
		tree->Branch("yCov",&yCov,"yCov/D");					
		tree->Branch("yChi2",&yChi2,"yChi2/D");	
		tree->Branch("ry",&ry,"ry/D");

	}
	else
	{
		outf = new TFile(plotFileName,"UPDATE");
		tree = (TTree*)(outf->Get("plots"));
		tree->SetBranchAddress("x0",&x0);
		tree->SetBranchAddress("y0",&y0);
		tree->SetBranchAddress("z0",&z0);
		tree->SetBranchAddress("tx",&tx);
		tree->SetBranchAddress("ty",&ty);
		tree->SetBranchAddress("dz",&dz);
		tree->SetBranchAddress("side",&side);
		tree->SetBranchAddress("puls",&puls);
		tree->SetBranchAddress("id",&id);
		tree->SetBranchAddress("areasum",&areasum);
		tree->SetBranchAddress("sigma",&sigma);
		tree->SetBranchAddress("meanDeltaTheta3D",&meanDeltaTheta3D);
		tree->SetBranchAddress("sigmaDeltaTheta3D",&sigmaDeltaTheta3D);
		tree->SetBranchAddress("meanDeltaThetaXZ",&meanDeltaThetaXZ);
		tree->SetBranchAddress("sigmaDeltaThetaXZ",&sigmaDeltaThetaXZ);
		tree->SetBranchAddress("meanDeltaThetaYZ",&meanDeltaThetaYZ);
		tree->SetBranchAddress("sigmaDeltaThetaYZ",&sigmaDeltaThetaYZ);
		tree->SetBranchAddress("meanDistanceClustersTo3DLine",&meanDistanceClustersTo3DLine);
		tree->SetBranchAddress("meanGapClusterToCluster",&meanGapClusterToCluster);
		tree->SetBranchAddress("mx",&mx);
		tree->SetBranchAddress("qx",&qx);
		tree->SetBranchAddress("xChi2",&xChi2);
		tree->SetBranchAddress("rx",&rx);
		tree->SetBranchAddress("my",&my);
		tree->SetBranchAddress("qy",&qy);
		tree->SetBranchAddress("yChi2",&yChi2);
		tree->SetBranchAddress("ry",&ry);
	}

	
	std::cout << "Saving plots into the file " << plotFileName << ": " <<  std::setw(2) << std::setprecision(2) << 0 << "%" << std::flush;;
	std::cout << "\b\b\b";
	
	
	for (int i = 0; i < nViews; i++)
	{
		View *view   = _fragment->getView(i);
		Side *top    = view->getTopSide();
		Side *bottom = view->getBottomSide();
		int nTracksTop = top->getTracks();
		int nTracksBottom = bottom->getTracks();

		for (int t = 0; t < nTracksTop; t++)
		{
			Track *tr = top->getTrackAt(t);
			x0 = tr->getX();
			y0 = tr->getY();
			z0 = tr->getZ();
			tx = tr->getSX();
			ty = tr->getSY();
			sigma = tr->getSigma();
			dz = tr->dz;
			side = 1;
			puls = tr->getCount();
			id = t;      				  
			areasum = tr->getAreaSum();           				  
			meanDeltaTheta3D = tr->meanDeltaTheta3D;        				  
			sigmaDeltaTheta3D = tr->sigmaDeltaTheta3D;       				  
			meanDeltaThetaXZ = tr->meanDeltaThetaXZ;	    				  
			sigmaDeltaThetaXZ = tr->sigmaDeltaThetaXZ;						  
			meanDeltaThetaYZ = tr->meanDeltaThetaYZ;        				  
			sigmaDeltaThetaYZ = tr->sigmaDeltaThetaYZ;						  
			meanDistanceClustersTo3DLine = tr->meanDistanceClustersTo3DLine;              
			meanGapClusterToCluster = tr->meanGapClusterToCluster;                   
			mx = tr->mx; 
			qx = tr->qx; 
			rx = tr->rx;
			xChi2 = tr->xChi2; 
			my = tr->my;
			qy = tr->qy;
			ry = tr->ry;
			yChi2 = tr->yChi2;
			tree->Fill();
			counter++;
		}

		for (int b = 0; b < nTracksBottom; b++)
		{
			Track *tr = bottom->getTrackAt(b);
			x0 = tr->getX();
			y0 = tr->getY();
			z0 = tr->getZ();
			tx = tr->getSX();
			ty = tr->getSY();
			sigma = tr->getSigma();
			dz = tr->dz;
			side = 2;
			puls = tr->getCount();
			id = b;      				  
			areasum = tr->getAreaSum();           				  
			meanDeltaTheta3D = tr->meanDeltaTheta3D;        				  
			sigmaDeltaTheta3D = tr->sigmaDeltaTheta3D;       				  
			meanDeltaThetaXZ = tr->meanDeltaThetaXZ;	    				  
			sigmaDeltaThetaXZ = tr->sigmaDeltaThetaXZ;						  
			meanDeltaThetaYZ = tr->meanDeltaThetaYZ;        				  
			sigmaDeltaThetaYZ = tr->sigmaDeltaThetaYZ;						  
			meanDistanceClustersTo3DLine = tr->meanDistanceClustersTo3DLine;              
			meanGapClusterToCluster = tr->meanGapClusterToCluster;                   
			mx = tr->mx; 
			qx = tr->qx; 
			rx = tr->rx;
			xChi2 = tr->xChi2; 
			my = tr->my;
			qy = tr->qy;
			ry = tr->ry;
			yChi2 = tr->yChi2;
			tree->Fill();
			counter++;

		}
		std::cout << std::setw(2) << std::setprecision(2) << int((double) i/(nViews-1)*100) << "%" << std::flush;
		std::cout << "\b\b\b";
	}
	std::cout << std::endl;

	//outf->Write();
	//tree->Write();
	tree->Write("", TObject::kOverwrite);
	outf->Close();
	return counter;

}

