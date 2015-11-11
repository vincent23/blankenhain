#include <juce>

#include "plugins/example/ExampleAudioProcessor.h"
#include "plugins/filter/FilterAudioProcessor.h"
#include "plugins/wittersroda/WittersrodaAudioProcessor.h"
#include "plugins/volume/VolumeAudioProcessor.h"
#include "plugins/pan/PanAudioProcessor.h"
#include "plugins/bitcrush/BitcrushAudioProcessor.h"

// this creates the correct plugin based on preprocessor flags
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{

#ifdef PLUGIN_EXAMPLE
    return new ExampleAudioProcessor();
#elif PLUGIN_FILTER
	//Here we need to allocate and use !!"placement new"!! to construct
	//since we need aligned space, obtained using _aligned_malloc
	//return new FilterAudioProcessor(); <- this will allocate unaligned!!
	void *raw_memory = _aligned_malloc(sizeof(FilterAudioProcessor), 16);
	FilterAudioProcessor *pointerToBeReturned = new(raw_memory) FilterAudioProcessor;
	return pointerToBeReturned;
#elif PLUGIN_WITTERSRODA
	return new WittersrodaAudioProcessor();
#elif PLUGIN_VOLUME
	return new VolumeAudioProcessor();
#elif PLUGIN_PAN
	return new PanAudioProcessor();
#elif PLUGIN_BITCRUSH
	return new BitcrushAudioProcessor();
#else
	static_assert(false, "The build config of your plugin is broken. "
		"Please see README for instructions on how to add new plugins.");
#endif

}
