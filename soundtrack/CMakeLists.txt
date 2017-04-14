cmake_minimum_required(VERSION 3.6)
project(blankenhain_soundtrack)

include_directories("../player/include")
include_directories("../libblankenhain/include")
add_library(blankenhain_soundtrack STATIC "BhSoundtrack.h" "BhSoundtrack.cpp" "Input.inl")
target_link_libraries(blankenhain_soundtrack libblankenhain)
target_link_libraries(blankenhain_soundtrack blankenhain_player)
