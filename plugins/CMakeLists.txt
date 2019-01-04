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
	"ImguiEffectEditor.h"
	"InstrumentPluginBase.h"
	"MidiPluginBase.h"
	"PluginBase.h"
	"PluginParameterBundle.h"
	"ImguiCustomFunctions.h"
)

prepend(SOURCES "src/"
	"ImguiEffectEditor.cpp"
	"ImguiFrameCallback.h"
	"ImguiVstguiView.h"
	"ImguiVstguiView.cpp"
	"InstrumentPluginBase.cpp"
	"MidiPluginBase.cpp"
	"PluginBase.cpp"
	"PluginParameterBundle.cpp"
)

# VST
prepend(VST_SOURCES "deps/vst2/"
	"aeffect.h"
	"aeffectx.h"
	"public.sdk/source/vst2.x/aeffeditor.h"
	"public.sdk/source/vst2.x/audioeffect.cpp"
	"public.sdk/source/vst2.x/audioeffect.h"
	"public.sdk/source/vst2.x/audioeffectx.cpp"
	"public.sdk/source/vst2.x/audioeffectx.h"
	"vstfxstore.h"
)
include_directories("deps/vst2")
include_directories("deps/vst2/public.sdk/source/vst2.x")
add_library(vst2 STATIC ${VST_SOURCES})

# imgui
set(IMGUI_DIR "deps/imgui")
# TODO use own imconfig.h and ignore the one in the repository
prepend(IMGUI_SRC "${IMGUI_DIR}/"
	"imgui.cpp"
	"imgui.h"
	"imgui_demo.cpp"
	"imgui_draw.cpp"
	"imgui_internal.h"
	"stb_rect_pack.h"
	"stb_textedit.h"
	"stb_truetype.h"
	"imconfig.h"
)
include_directories(${IMGUI_DIR})
# glad
set(GLAD_DIR "deps/glad")
prepend(GLAD_SRC "${GLAD_DIR}/"
	"include/glad/glad.h"
	"include/KHR/khrplatform.h"
	"src/glad.c"
)
include_directories("${GLAD_DIR}/include")
# vstgui
set(VSTGUI_DIR "deps/vstgui/")
prepend(VSTGUI_SRC "${VSTGUI_DIR}/vstgui/"
	"vstgui_win32.cpp"
	"plugin-bindings/aeffguieditor.cpp"
)
include_directories("${VSTGUI_DIR}")
add_library(imgui STATIC ${IMGUI_SRC} ${GLAD_SRC} ${VSTGUI_SRC})
target_compile_definitions(imgui PRIVATE _CRT_SECURE_NO_WARNINGS)

include_directories("include")
include_directories("../libblankenhain/include")
add_library(bh_plugins_common STATIC ${HEADERS} ${SOURCES})
target_link_libraries(bh_plugins_common vst2 imgui)

set(PLUGIN_PATH "" CACHE PATH "Path to VST plugin install location")

find_package(OpenGL REQUIRED)

function(add_plugin name)
	prepend(PLUGIN_SOURCES "src/" ${ARGN})
	# for now, add vstplugmain.cpp to each plugin separately so VSTPluginMain will be exported
	set(PLUGIN_SOURCES ${PLUGIN_SOURCES} "deps/vst2/public.sdk/source/vst2.x/vstplugmain.cpp")
	add_library(${name} SHARED ${PLUGIN_SOURCES})
	target_link_libraries(${name} libblankenhain bh_plugins_common ${OPENGL_LIBRARIES})
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
	"CompressorPluginEditor.cpp"
	"CompressorPluginEditor.h"
)

add_plugin("bh_delay"
	"DelayPlugin.cpp"
	"DelayPlugin.h"
	"DelayPluginEditor.cpp"
	"DelayPluginEditor.h"
)

add_plugin("bh_distortion"
	"DistortionPlugin.cpp"
	"DistortionPlugin.h"
)

add_plugin("bh_equalizer"
	"EqualizerPlugin.cpp"
	"EqualizerPlugin.h"
	"EqualizerPluginEditor.cpp"
	"EqualizerPluginEditor.h"
)

add_plugin("bh_example"
	"ExamplePlugin.cpp"
	"ExamplePlugin.h"
	"ExamplePluginEditor.cpp"
	"ExamplePluginEditor.h"
)

add_plugin("bh_filter"
	"FilterPlugin.cpp"
	"FilterPlugin.h"
	"FilterPluginEditor.cpp"
	"FilterPluginEditor.h"
)

add_plugin("bh_volume"
	"VolumePlugin.cpp"
	"VolumePlugin.h"
	"VolumePluginEditor.h"
	"VolumePluginEditor.cpp"
)

add_plugin("bh_example_synth"
	"ExampleInstrumentPlugin.cpp"
	"ExampleInstrumentPlugin.h"
)

add_plugin("bh_polyblep_synth"
	"polyblepInstrumentPlugin.cpp"
	"polyblepInstrumentPlugin.h"
)

add_plugin("bh_glide_polyblep_synth"
	"glidePolyblepInstrumentPlugin.cpp"
	"glidePolyblepInstrumentPlugin.h"
	"glidePolyblepInstrumentPluginEditor.cpp"
	"glidePolyblepInstrumentPluginEditor.h"
)

add_plugin("bh_gm_synth"
	"gmDlsInfo.h"
	"gmsynthInstrumentPlugin.cpp"
	"gmsynthInstrumentPlugin.h"
	"gmsynthInstrumentPluginEditor.h"
	"gmsynthInstrumentPluginEditor.cpp"
)

add_plugin("bh_freeverb"
	"freeverbPlugin.cpp"
	"freeverbPlugin.h"
)

add_plugin("bh_rhythmgate"
	"rhythmgatePlugin.cpp"
	"rhythmgatePlugin.h"
	"rhythmgatePluginEditor.cpp"
	"rhythmgatePluginEditor.h"
)

add_plugin("bh_fm_synth"
	"fmInstrumentPlugin.cpp"
	"fmInstrumentPlugin.h"
	"fmInstrumentPluginEditor.cpp"
	"fmInstrumentPluginEditor.h"
)

add_plugin("bh_chorus"
	"ChorusPlugin.cpp"
	"ChorusPlugin.h"
	"ChorusPluginEditor.cpp"
	"ChorusPluginEditor.h"
)

add_plugin("bh_width"
	"WidthPlugin.cpp"
	"WidthPlugin.h"
	"WidthPluginEditor.h"
	"WidthPluginEditor.cpp"
)

add_plugin("bh_noisegate"
	"NoiseGatePlugin.cpp"
	"NoiseGatePlugin.h"
)

add_plugin("bh_limiter"
	"LimiterPlugin.cpp"
	"LimiterPlugin.h"
)

add_plugin("bh_inverse_phase"
	"InversePhasePlugin.cpp"
	"InversePhasePlugin.h"
)
