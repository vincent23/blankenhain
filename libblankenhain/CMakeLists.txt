cmake_minimum_required(VERSION 3.6)
project(libblankenhain)

# utility
function(prepend var prefix)
	set(stringsWithPrefix "")
	foreach(x ${ARGN})
		list(APPEND stringsWithPrefix "${prefix}/${x}")
	endforeach(x)
	set(${var} "${stringsWithPrefix}" PARENT_SCOPE)
endfunction(prepend)

prepend(HEADERS "include/"
	"AlignedType.h"
	"AuxFunc.h"
	"CircularBuffer.h"
	"EffectBase.h"
	"FloatParameter.h"
	"NormalizedRange.h"
	"ParameterBundle.h"
	"processFunctions.h"
	"Sample.h"
	"UpDownSampling.h"
)

prepend(SOURCES "src/"
	"AlignedType.cpp"
	"AuxFunc.cpp"
	"CircularBuffer.cpp"
	"EffectBase.cpp"
	"FloatParameter.cpp"
	"NormalizedRange.cpp"
	"ParameterBundle.cpp"
	"processFunctions.cpp"
	"UpDownSampling.cpp"
)

include_directories("include")
add_library(libblankenhain STATIC ${HEADERS} ${SOURCES})
