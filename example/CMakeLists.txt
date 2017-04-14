cmake_minimum_required(VERSION 3.6)
project(blankenhain_example)

set(SOURCES
	"Example.cpp"
)

include_directories("../soundtrack")
add_executable(blankenhain_example ${SOURCES})
set_target_properties(blankenhain_example PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /SAFESEH:NO /STACK:0x100000,0x100000")
target_link_libraries(blankenhain_example blankenhain_soundtrack)
target_link_libraries(blankenhain_example winmm.lib)
