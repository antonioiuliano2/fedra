
In order to compile FEDRA directly from MS Visual Studio .NET Development environment:

- run install.cmd in order to create lib, bin and include folders and to create the link to 
  the headers files in the include folder.

- from the menu toolbar: Tools -> Options -> Projects -> VC++ Directories 

	- show directories for "Executables files" and ADD $(ROOTSYS)\bin
    	  otherwise rootcint cannot be executed


 