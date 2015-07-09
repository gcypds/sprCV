// BGS.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "BGS.h"


// This is an example of an exported variable
BGS_API int nBGS=0;

// This is an example of an exported function.
BGS_API int fnBGS(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see BGS.h for the class definition
CBGS::CBGS()
{
	return;
}
