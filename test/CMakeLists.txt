cmake_minimum_required(VERSION 3.6)
project(blankenhain_test)

set(SOURCES
	"tests.cpp"
)

include_directories("../libblankenhain/include")
include_directories("../deps/Catch2/single_include")
add_executable(blankenhain_test ${SOURCES})
target_link_libraries(blankenhain_test libblankenhain)
