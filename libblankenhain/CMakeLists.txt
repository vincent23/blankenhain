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
	"AlignedType.h"
	"ArpMidiEffect.h"
	"AuxFunc.h"
	"Basedrum1Instrument.h"
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
	"freeverbEffect.h"
	"glidePolyblepInstrument.h"
	"gmSoundRegion.h"
	"gmInstrument.h"
	"gmSound.h"
	"gmsynthInstrument.h"
	"InstrumentBase.h"
	"InterpolatedValue.h"
	"MidiBase.h"
	"MidiEvent.h"
	"ModulationEnvelope.h"
	"NoiseGateEffect.h"
	"NormalizedRange.h"
	"OnePoleFilter.h"
	"Oscillators.h"
	"ParameterBundle.h"
	"pitchshiftMidiEffect.h"
	"polyblepInstrument.h"
	"RhythmgateEffect.h"
	"Sample.h"
	"SoundFonts.h"
	"UpDownSampling.h"
	"VoiceState.h"
	"VolumeEffect.h"
	"warnings.h"
	"WavetableInstrument.h"
	"WidthEffect.h"
)

prepend(SOURCES "src/"
	"AlignedType.cpp"
	"ArpMidiEffect.cpp"
	"AuxFunc.cpp"
	"Basedrum1Instrument.cpp"
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
	"freeverbEffect.cpp"
	"glidePolyblepInstrument.cpp"
	"gmInstrument.cpp"
	"gmSound.cpp"
	"gmsynthInstrument.cpp"
	"InstrumentBase.cpp"
	"InterpolatedValue.cpp"
	"MidiEvent.cpp"
	"MidiBase.cpp"
	"NoiseGateEffect.cpp"
	"NormalizedRange.cpp"
	"OnePoleFilter.cpp"
	"Oscillators.cpp"
	"ParameterBundle.cpp"
	"pitchshiftMidiEffect.cpp"
	"polyblepInstrument.cpp"
	"RhythmgateEffect.cpp"
	"Sample.cpp"
	"UpDownSampling.cpp"
	"VoiceState.cpp"
	"VolumeEffect.cpp"
	"WavetableInstrument.cpp"
	"WidthEffect.cpp"
)

include_directories("include")
add_library(libblankenhain STATIC ${HEADERS} ${SOURCES})

# Group items into folders for less crowded folder viewing experience
# via //http://stackoverflow.com/questions/7091385/how-to-organise-my-files-using-cmake
source_group(Effects REGULAR_EXPRESSION .*Effect.*)
source_group(Instruments REGULAR_EXPRESSION .*Instrument.*)
