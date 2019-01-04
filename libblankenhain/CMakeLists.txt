cmake_minimum_required(VERSION 3.6)
project(libblankenhain)

# utility
function(prepend var prefix)
	set(stringsWithPrefix "")
	foreach(x ${ARGN})
		list(APPEND stringsWithPrefix "${prefix}/${x}")
	endforeach(x)
	set(${var} "${stringsWithPrefix}" PARENT_SCOPE)
endfunction(prepend)

prepend(HEADERS "include/"
	"AbletonTrackMixerVolumeEffect.h"
	"AlignedType.h"
	"AuxFunc.h"
	"BitcrushEffect.h"
	"BhMath.h"
	"ChorusEffect.h"
	"CircularBuffer.h"
	"CompressorEffect.h"
	"Constants.h"
	"DelayEffect.h"
	"DistortionEffect.h"
	"EffectBase.h"
	"EnvelopeFollower.h"
	"EqualizerEffect.h"
	"ExampleEffect.h"
	"ExampleInstrument.h"
	"Filter.h"
	"FilterEffect.h"
	"FloatParameter.h"
	"FmInstrument.h"
	"FpuState.h"
	"ReverbEffect.h"
	"MonosynthInstrument.h"
	"GmSoundRegion.h"
	"GmInstrument.h"
	"GmSound.h"
	"GmsynthInstrument.h"
	"InstrumentBase.h"
	"InterpolatedValue.h"
	"InversePhaseEffect.h"
	"LimiterEffect.h"
	"MidiBase.h"
	"MidiEvent.h"
	"ModulationEnvelope.h"
	"NoiseGateEffect.h"
	"NormalizedRange.h"
	"OnePoleFilter.h"
	"Options.h"
	"Oscillators.h"
	"ParameterBundle.h"
	"PolyblepInstrument.h"
	"RhythmgateEffect.h"
	"Sample.h"
	"SoundFonts.h"
	"UpDownSampling.h"
	"VoiceState.h"
	"VolumeEffect.h"
	"Warnings.h"
	"PanEffect.h"
)

prepend(SOURCES "src/"
	"AbletonTrackMixerVolumeEffect.cpp"
	"AlignedType.cpp"
	"AuxFunc.cpp"
	"BitcrushEffect.cpp"
	"BhMath.cpp"
	"ChorusEffect.cpp"
	"CircularBuffer.cpp"
	"CompressorEffect.cpp"
	"DelayEffect.cpp"
	"DistortionEffect.cpp"
	"EffectBase.cpp"
	"EnvelopeFollower.cpp"
	"EqualizerEffect.cpp"
	"ExampleEffect.cpp"
	"ExampleInstrument.cpp"
	"Filter.cpp"
	"FilterEffect.cpp"
	"FloatParameter.cpp"
	"FmInstrument.cpp"
	"FpuState.cpp"
	"ReverbEffect.cpp"
	"MonosynthInstrument.cpp"
	"GmInstrument.cpp"
	"GmSound.cpp"
	"GmsynthInstrument.cpp"
	"InstrumentBase.cpp"
	"InterpolatedValue.cpp"
	"InversePhaseEffect.cpp"
	"LimiterEffect.cpp"
	"MidiEvent.cpp"
	"MidiBase.cpp"
	"NoiseGateEffect.cpp"
	"NormalizedRange.cpp"
	"OnePoleFilter.cpp"
	"Oscillators.cpp"
	"ParameterBundle.cpp"
	"polyblepInstrument.cpp"
	"RhythmgateEffect.cpp"
	"Sample.cpp"
	"UpDownSampling.cpp"
	"VoiceState.cpp"
	"VolumeEffect.cpp"
	"PanEffect.cpp"
)

include_directories("include")
add_library(libblankenhain STATIC ${HEADERS} ${SOURCES})

# Group items into folders for less crowded folder viewing experience
# via //http://stackoverflow.com/questions/7091385/how-to-organise-my-files-using-cmake
source_group(Effects REGULAR_EXPRESSION .*Effect.*)
source_group(Instruments REGULAR_EXPRESSION .*Instrument.*)
