#include "Catalog.h"
#include <iostream>

void Catalog::setHeaderInfo(HeaderInfo headerInfo)
{
	_headerInfo.part0 = headerInfo.part0;
	_headerInfo.part1 = headerInfo.part1;
	_headerInfo.part2 = headerInfo.part2;
	_headerInfo.part3 = headerInfo.part3;
}


bool Catalog::setFragmentIndexes(unsigned int y, unsigned int x, unsigned int value)
{
	if (!_pFragmentIndexes)
		return false;
	_pFragmentIndexes[y*_xViews + x] = value;
	return true;
}

int Catalog::findConfig(std::string configName)
{
	bool found = false;
	std::vector<std::string> tokens;
	const std::string delimiters=" ";
	unsigned int index = -1; 
	for (unsigned int i = 0; i <_setup.length; i++) 
	{
		tokens.clear();


		 // Skip delimiters at beginning.
		std::string::size_type lastPos = _setup.configs.at(i).name.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos     = _setup.configs.at(i).name.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(_setup.configs.at(i).name.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = _setup.configs.at(i).name.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = _setup.configs.at(i).name.find_first_of(delimiters, lastPos);
		}
//		std::cout << "============================" << std::endl;
		for (int j = 0; j < static_cast<int>(tokens.size()); j++)
		{
			
			if (tokens.at(j) == configName)
			{
				found = true;
//				std::cout << "FOUND!!!\n" << tokens.at(j) << std::endl;
				index = i;
				break;
			}
		}
		if (found)
			break;
	}

	return index;
}



std::string Catalog::getConfigValue(int index,std::string value)
{
	bool found = false;
	std::vector<std::string> tokens;
	const std::string delimiters=" ";
	unsigned int indexConf = -1; 
	//std::cout << _setup.configs.at(index).length << std::endl;
	for (unsigned int i = 0; i <_setup.configs.at(index).length; i++) 
	{
		tokens.clear();


		 // Skip delimiters at beginning.
		std::string::size_type lastPos = _setup.configs.at(index).keys.at(i).name.find_first_not_of(delimiters, 0);
		// Find first "non-delimiter".
		std::string::size_type pos     = _setup.configs.at(index).keys.at(i).name.find_first_of(delimiters, lastPos);

		while (std::string::npos != pos || std::string::npos != lastPos)
		{
			// Found a token, add it to the vector.
			tokens.push_back(_setup.configs.at(index).keys.at(i).name.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = _setup.configs.at(index).keys.at(i).name.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = _setup.configs.at(index).keys.at(i).name.find_first_of(delimiters, lastPos);
		}
		
		for (int j = 0; j < static_cast<int>(tokens.size()); j++)
		{
			if (tokens.at(j) == value)
			{
				found = true;
				indexConf = i;
				break;
			}
		}
		if (found)
			break;
	}
	
	_setup.configs.at(index).keys.at(indexConf);
	std::string retValue = _setup.configs.at(index).keys.at(indexConf).value;
//	std::cout << retValue << std::endl;


	return retValue;
}
