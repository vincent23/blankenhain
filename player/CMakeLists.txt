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
)

prepend(SOURCES "src/"
	"BlankenhainPlayer.cpp"
)

include_directories("include")
include_directories("../libblankenhain/include")
add_library(blankenhain_player STATIC ${HEADERS} ${SOURCES})
target_link_libraries(blankenhain_player libblankenhain)