// ProjASM.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ProjASM.h"


// This is an example of an exported variable
PROJASM_API int nProjASM=0;

// This is an example of an exported function.
PROJASM_API int fnProjASM(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ProjASM.h for the class definition
CProjASM::CProjASM()
{
    return;
}
