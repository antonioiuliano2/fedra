#include <iostream>
#include <map>
#include "ProcessRawData.h"


void toLower(std::string &inputString)
{
	for (unsigned int i = 0; i < inputString.length(); i++)
	{
		inputString[i] = tolower(inputString[i]); 
	}
}


// Value-Defintions of the different String values
enum StringValue { 
  evStringInputFile = 1,
  evStringOutputFile = 2,
  evStringClustersOption = 3,
  evStringUsage = 4,
  evStringFilterTracks = 5,
  evStringSkipCatalog = 6
};

// Map to associate the strings with the enum values
static std::map<std::string, StringValue> s_mapStringValues;

void Initialize()
{
  s_mapStringValues["/inputfile"] = evStringInputFile;
  s_mapStringValues["/outputfile"] = evStringOutputFile;
  s_mapStringValues["/clusters"] = evStringClustersOption;
  s_mapStringValues["/help"] = evStringUsage;
  s_mapStringValues["/filtertracks"] = evStringFilterTracks;
  s_mapStringValues["/skipcatalog"] = evStringSkipCatalog;
  
  //std::cout << "s_mapStringValues contains " << s_mapStringValues.size() << " entries." << std::endl;
}

int main(int argc, char **argv)
{
	
	Initialize();

	bool inputFileDefined = false;
	bool outputFileDefined = false;
	bool convertClusters = false;
	bool filterTracks = false;
	bool inputRWD = false;
	bool inputRWC = false;
  bool skipCatalog = false;
  std::string inputFile;
	std::string outputFile;

	try
	{
		std::string exception;
		if (argc == 1)
		{
			exception = "";
			throw exception;
		}
		for (int argnum = 1; argnum < argc; argnum++)
		{
			std::string value = argv[argnum];

			toLower(value);

			
			switch (s_mapStringValues[value.c_str()])
			{
			case evStringInputFile:
				{
                    inputFileDefined = true;
					if (argnum + 1 >= argc)
						throw "Missing Parameters to switch \"" + value + "\"";
					inputFile = argv[argnum + 1];
                    argnum++;
                    break;
                }
			case evStringOutputFile:
                {
					outputFileDefined = true;
					if (argnum + 1 >= argc)
						throw "Missing Parameters to switch \"" + value + "\"";
                    outputFile = argv[argnum + 1];
                    argnum++;
                    break;
                }
			case evStringClustersOption:
				{
					
					if (argnum + 1 >= argc)
						throw "Missing Parameters to switch \"" + value + "\"";
					std::string opt = argv[argnum +1];
					if (opt == "on")
						convertClusters = true;
					argnum++;
					break;
				}
			case evStringFilterTracks:
				{
									
					if (argnum + 1 >= argc)
						throw "Missing Parameters to switch \"" + value + "\"";
					std::string opt = argv[argnum +1];
					if (opt == "on")
						filterTracks = true;
					argnum++;
					break;
				}
        case evStringSkipCatalog:
        {
          if (argnum + 1 >= argc)
            throw "Missing Parameters to switch \"" + value + "\"";
          std::string opt = argv[argnum +1];
          if (opt == "on")
            skipCatalog = true;
          argnum++;
          break;
        }
        case evStringUsage:
				exception = "";
				throw exception;
				break;
			default: 
				{
				    exception = "Unknown switch \"" + value + "\".";
					throw exception;
					break;
				}
			}
			
		}
		
		if (!inputFileDefined || !outputFileDefined)
		{
			exception = "InputFile or OutputFile NOT defined!";
			throw exception;
		}

		// CODE START HERE
		ProcessRawData process;
		
		
		std::string::size_type rwc = inputFile.find( ".rwc", 0 );
		std::string::size_type rwd = inputFile.find( ".rwd", 0 );

		if( rwc != string::npos ) {
			inputRWC = true;
		}

		if( rwd != string::npos ) {
			inputRWD = true;
		}

    if (inputRWC)
    {
      process.setCatalogName(inputFile);
      process.setRootName(outputFile);
      process.setClOption(convertClusters);
      process.setFilterTrackOption(filterTracks);
      unsigned int nFragments =0;
      unsigned int failedFragments =0;
      if(!skipCatalog)
      {
        process.readCatalog();
        process.dumpCatalogInEdbStructure();
        nFragments = process.getFragmentsNumberFromCatalog();
      }
      else {
        nFragments=20000;
      }
      char rwdname[256], temp[256];
      for (unsigned int f = 1; f < nFragments+1; f++)
      {
        sprintf(temp, "%s", process.getCatalogName());
        sprintf(temp+strlen(temp)-1, "d");
        sprintf(rwdname, "%s.%08X", temp, f);
        cout <<"(tot. fragm.:"<<nFragments<<")  "; 
        process.setFragmentName(rwdname);
        if (process.readFragment())
        {
          process.dumpFragmentInEdbStructure();
          if (process.getFilterTracksOption())     process.makeTracksPlot("outputPlots.root");
        }
        else failedFragments++;
        if(failedFragments>5) break;
      }
    }
    else if (inputRWD)
		{
			process.setFragmentName(inputFile);
			process.setRootName(outputFile);
			process.setClOption(convertClusters);
			process.setFilterTrackOption(filterTracks);
			if (process.readFragment())
			{
			  process.dumpFragmentInEdbStructure();
			  if (process.getFilterTracksOption())
				  process.makeTracksPlot("outputPlots.root");
			}
		}
		else
		{
			exception = "input file has no .rwc extension nor .rwd extension";
			throw exception;
		}

		return 0;
	}

	catch (std::string str)
	{
		int strLengh = static_cast<int>(str.size());
		char *tab = new char[strLengh];
		for (int i = 0; i < strLengh; i++)
			tab[i] = '=';
		if (strLengh != 0)
		{
			std::cout << tab << std::endl;
			std::cout << str << std::endl;
			std::cout << tab << std::endl;
		}
		std::cout << "\nUsage: rwdToEdb.exe {parameters}" << std::endl;
		std::cout << "/inputfile     <p> - .rwc or .rwd input file." << std::endl;
		std::cout << "/outputfile    <p> - .root output file." << std::endl;
		std::cout << "/clusters      <p> - optional: on." << std::endl;
		std::cout << "/filterTracks  <p> - optional: on." << std::endl;
    std::cout << "/skipcatalog   <p> - optional: on." << std::endl;
    std::cout << "/help              - display the usage." << std::endl;
		delete []tab;
		return -1;
		
	}
}


