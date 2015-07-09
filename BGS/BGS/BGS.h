// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the BGS_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// BGS_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef BGS_EXPORTS
#define BGS_API __declspec(dllexport)
#else
#define BGS_API __declspec(dllimport)
#endif

// This class is exported from the BGS.dll
class BGS_API CBGS {
public:
	CBGS(void);
	// TODO: add your methods here.
};

extern BGS_API int nBGS;

BGS_API int fnBGS(void);
