cmake_minimum_required(VERSION 3.6)
project(blankenhain_example)

set(SOURCES
	"Example.cpp"
	"Input.inl"
)

include_directories("../player/include")
include_directories("../libblankenhain/include")
add_executable(blankenhain_example ${SOURCES})
target_link_libraries(blankenhain_example blankenhain_player)
target_link_libraries(blankenhain_example winmm.lib)
