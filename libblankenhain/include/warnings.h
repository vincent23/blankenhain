#pragma once
#ifndef _VC_NODEFAULTLIB

/////////////////////////////////////////////////////
//
// E N A B L E   W A R N I N G S 
#define _LIBBLANKENHAIN_ENABLE_WARNINGS
//
/////////////////////////////////////////////////////

#ifdef _LIBBLANKENHAIN_ENABLE_WARNINGS
#include <iostream>
#include <exception>
#include <iomanip>
#define _LIBBLANKENHAIN_ENABLE_FPU_ROUNDING_CHECK
#define _LIBBLANKENHAIN_ENABLE_NANCHECK
#endif
#else
#define _BLANKENHAIN_RUNTIME_MODE
#endif