cmake_minimum_required(VERSION 3.6)
project(blankenhain2)

add_definitions(-D_LIBBLANKENHAIN_ENABLE_WARNINGS)
add_subdirectory("libblankenhain")
add_subdirectory("plugins")
