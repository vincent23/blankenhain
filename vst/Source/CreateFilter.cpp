#include <juce>

#include "plugins/example/ExampleAudioProcessor.h"
#include "plugins/filter/FilterAudioProcessor.h"
#include "plugins/wittersroda/WittersrodaAudioProcessor.h"
#include "plugins/volume/VolumeAudioProcessor.h"
#include "plugins/pan/PanAudioProcessor.h"
#include "plugins/bitcrush/BitcrushAudioProcessor.h"
#include "plugins/kick/KickAudioProcessor.h"
#include "plugins/compressor/CompressorAudioProcessor.h"

// this creates the correct plugin based on preprocessor flags
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{

#ifdef PLUGIN_EXAMPLE
    return new ExampleAudioProcessor();
#elif PLUGIN_FILTER
	return new FilterAudioProcessor() ;
#elif PLUGIN_WITTERSRODA
	return new WittersrodaAudioProcessor();
#elif PLUGIN_VOLUME
	return new VolumeAudioProcessor();
#elif PLUGIN_PAN
	return new PanAudioProcessor();
#elif PLUGIN_BITCRUSH
	return new BitcrushAudioProcessor();
#elif PLUGIN_KICK
	return new KickAudioProcessor();
#elif PLUGIN_COMPRESSOR
	return new CompressorAudioProcessor();
#else
	static_assert(false, "The build config of your plugin is broken. "
		"Please see README for instructions on how to add new plugins.");
#endif

}
