cmake_minimum_required(VERSION 3.6)
project(blankenhain_plugins)

# utility
function(prepend var prefix)
	set(stringsWithPrefix "")
	foreach(x ${ARGN})
		list(APPEND stringsWithPrefix "${prefix}/${x}")
	endforeach(x)
	set(${var} "${stringsWithPrefix}" PARENT_SCOPE)
endfunction(prepend)

prepend(HEADERS "include/"
	"PluginBase.h"
	"PluginParameterBundle.h"
)

prepend(SOURCES "src/"
	"PluginBase.cpp"
	"PluginParameterBundle.cpp"
)

# VST
prepend(VST_SOURCES "deps/vst2/"
	"aeffect.h"
	"aeffectx.h"
	"aeffeditor.h"
	"audioeffect.cpp"
	"audioeffect.h"
	"audioeffectx.cpp"
	"audioeffectx.h"
	"vstfxstore.h"
)
include_directories("deps/vst2")
add_library(vst2 STATIC ${VST_SOURCES})

include_directories("include")
include_directories("../libblankenhain/include")
add_library(bh_plugins_common STATIC ${HEADERS} ${SOURCES})
target_link_libraries(bh_plugins_common vst2)

set(PLUGIN_PATH "" CACHE PATH "Path to VST plugin install location")

function(add_plugin name)
	prepend(PLUGIN_SOURCES "src/" ${ARGN})
	# for now, add vstplugmain.cpp to each plugin separately so VSTPluginMain will be exported
	set(PLUGIN_SOURCES ${PLUGIN_SOURCES} "deps/vst2/vstplugmain.cpp")
	add_library(${name} SHARED ${PLUGIN_SOURCES})
	target_link_libraries(${name} libblankenhain bh_plugins_common)
	set_target_properties(${name} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS on)
	if(PLUGIN_PATH)
		install(TARGETS ${name} RUNTIME DESTINATION ${PLUGIN_PATH})
	endif()
endfunction(add_plugin)

add_plugin("bh_bitcrush"
	"BitcrushPlugin.cpp"
	"BitcrushPlugin.h"
)

add_plugin("bh_compressor"
	"CompressorPlugin.cpp"
	"CompressorPlugin.h"
)

add_plugin("bh_delay"
	"DelayPlugin.cpp"
	"DelayPlugin.h"
)

add_plugin("bh_distortion"
	"DistortionPlugin.cpp"
	"DistortionPlugin.h"
)

add_plugin("bh_equalizer"
	"EqualizerPlugin.cpp"
	"EqualizerPlugin.h"
)

add_plugin("bh_example"
	"ExamplePlugin.cpp"
	"ExamplePlugin.h"
)

add_plugin("bh_filter"
	"FilterPlugin.cpp"
	"FilterPlugin.h"
)

add_plugin("bh_pan"
	"PanPlugin.cpp"
	"PanPlugin.h"
)

add_plugin("bh_volume"
	"VolumePlugin.cpp"
	"VolumePlugin.h"
)
