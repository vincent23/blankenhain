cmake_minimum_required(VERSION 3.6)
project(blankenhain2)

add_compile_options(/arch:SSE2)
# link-time code generation disabled for compile speed
# add_compile_options(/GL)

string(REGEX REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT /Zl /GS- /Gs9999999 /QIfist /GR- /Oy-")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /Zl /GS- /Gs9999999 /QIfist /GR- /Oy-")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MTd")

#add_definitions(-D_LIBBLANKENHAIN_ENABLE_WARNINGS)
add_subdirectory("libblankenhain")
add_subdirectory("plugins")
add_subdirectory("player")
add_subdirectory("example")
