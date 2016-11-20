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
	"BitcrushEffect.h"
	"CircularBuffer.h"
	"CompressorEffect.h"
	"DelayEffect.h"
	"DistortionEffect.h"
	"EffectBase.h"
	"EqualizerEffect.h"
	"ExampleEffect.h"
	"FilterEffect.h"
	"FloatParameter.h"
	"NormalizedRange.h"
	"PanEffect.h"
	"ParameterBundle.h"
	"processFunctions.h"
	"Sample.h"
	"UpDownSampling.h"
	"VolumeEffect.h"
)

prepend(SOURCES "src/"
	"AlignedType.cpp"
	"AuxFunc.cpp"
	"BitcrushEffect.cpp"
	"CircularBuffer.cpp"
	"CompressorEffect.cpp"
	"DelayEffect.cpp"
	"DistortionEffect.cpp"
	"EffectBase.cpp"
	"EqualizerEffect.cpp"
	"ExampleEffect.cpp"
	"FilterEffect.cpp"
	"FloatParameter.cpp"
	"NormalizedRange.cpp"
	"PanEffect.cpp"
	"ParameterBundle.cpp"
	"processFunctions.cpp"
	"UpDownSampling.cpp"
	"VolumeEffect.cpp"
)

include_directories("include")
add_library(libblankenhain STATIC ${HEADERS} ${SOURCES})
