cmake_minimum_required(VERSION 3.6)
project(blankenhain2)

add_compile_options(/arch:SSE2)
# link-time code generation disabled for compile speed
# add_compile_options(/GL)

#add_definitions(-D_LIBBLANKENHAIN_ENABLE_WARNINGS)
add_subdirectory("libblankenhain")
add_subdirectory("plugins")
add_subdirectory("player")
add_subdirectory("example")
