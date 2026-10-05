#if defined(__CLING__) || defined(__ROOTCLING__)

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

// Selection rules (+ enables automatic ROOT I/O streamer generation)
#pragma link C++ class DataConversion+;

#endif