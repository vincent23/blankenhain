#include "gmsynthInstrumentPlugin.h"
#include "gmsynthInstrumentPluginEditor.h"
#include "gmsynthInstrument.h"
#include "gmDlsInfo.h"

gmsynthInstrumentPlugin::gmsynthInstrumentPlugin(audioMasterCallback audioMaster)
	: InstrumentPluginBase(audioMaster, new gmsynthInstrument)
{
	setUniqueID('bhGM');
	instruments = getInstrumentInfo();
	setEditor(new gmsynthPluginEditor(this));
}

gmsynthInstrumentPlugin::~gmsynthInstrumentPlugin()
{
	if (instruments != nullptr) {
		for (unsigned int i = 0; i < 235; i++) {
			if (instruments[i] != nullptr) {
				delete instruments[i];
			}
		}
		delete[] instruments;
	}
}

void gmsynthInstrumentPlugin::loadMidiInstrument(unsigned int index)
{
	loadedInstrument = index;
	static_cast<gmsynthInstrument*>(effect)->loadMidiInstrument(*instruments[index]);
}

void gmsynthInstrumentPlugin::onBeforeBlock(unsigned int blockOffset)
{
	InstrumentPluginBase::onBeforeBlock(blockOffset);

	FloatParameter const* instrumentParameter = getParameters().getParameter(9);
	// TODO dirty workaround to skip interpolation
	// TODO merely commented out during revision of bh2-framework. Still needs to be adressed. Maybe through IntegerParameters?
	// instrumentParameter->next(1000);
	unsigned int instrumentAccordingToParameter = static_cast<unsigned int>(instrumentParameter->getValueUnnormalized());
	if (instrumentAccordingToParameter != loadedInstrument) {
		loadMidiInstrument(instrumentAccordingToParameter);
	}
}

AudioEffect* createEffectInstance(audioMasterCallback audioMaster)
{
	// Change name here
	return new gmsynthInstrumentPlugin(audioMaster);
}