#include <juce>

#include "plugins/example/ExampleAudioProcessor.h"
#include "plugins/filter/FilterAudioProcessor.h"
#include "plugins/wittersroda/WittersrodaAudioProcessor.h"

// this creates the correct plugin based on preprocessor flags
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{

#ifdef PLUGIN_EXAMPLE
    return new ExampleAudioProcessor();
#elif PLUGIN_FILTER
	return new FilterAudioProcessor();
#elif PLUGIN_WITTERSRODA
	return new WittersrodaAudioProcessor();
#else
	static_assert(false, "The build config of your plugin is broken. "
		"Please see README for instructions on how to add new plugins.");
#endif

}
