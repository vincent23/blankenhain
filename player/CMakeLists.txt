cmake_minimum_required(VERSION 3.6)
project(blankenhain_player)

# utility
function(prepend var prefix)
	set(stringsWithPrefix "")
	foreach(x ${ARGN})
		list(APPEND stringsWithPrefix "${prefix}/${x}")
	endforeach(x)
	set(${var} "${stringsWithPrefix}" PARENT_SCOPE)
endfunction(prepend)

prepend(HEADERS "include/"
	"BlankenhainPlayer.h"
	"ChainDevice.h"
	"CombinedDevice.h"
	"Device.h"
	"EffectDevice.h"
	"GroupDevice.h"
	"InstrumentDevice.h"
	"MidiDevice.h"
	"MidiTrack.h"
	"ParameterTrack.h"
	"ReturnDevice.h"
	"SendDevice.h"
	"Song.h"
	"SongInfo.h"
)

prepend(SOURCES "src/"
	"BlankenhainPlayer.cpp"
	"ChainDevice.cpp"
	"CombinedDevice.cpp"
	"Device.cpp"
	"EffectDevice.cpp"
	"GroupDevice.cpp"
	"InstrumentDevice.cpp"
	"MidiDevice.cpp"
	"MidiTrack.cpp"
	"ParameterTrack.cpp"
	"ReturnDevice.cpp"
	"SendDevice.cpp"
	"Song.cpp"
	"SongInfo.cpp"
)

include_directories("include")
include_directories("../libblankenhain/include")
add_library(blankenhain_player STATIC ${HEADERS} ${SOURCES})
target_link_libraries(blankenhain_player libblankenhain)
