cmake_minimum_required(VERSION 3.6)
project(blankenhain_wavewriter)

set(SOURCES
	"Wavewriter.cpp"
)

include_directories("../soundtrack")
add_executable(blankenhain_wavewriter ${SOURCES})
set_target_properties(blankenhain_wavewriter PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO /STACK:0x100000,0x100000")
target_link_libraries(blankenhain_wavewriter blankenhain_soundtrack)
target_link_libraries(blankenhain_wavewriter winmm.lib)
