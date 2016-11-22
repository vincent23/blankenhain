#pragma once

#include "EffectBase.h"

class InstrumentBase : public EffectBase
{
public:
	using EffectBase::EffectBase;

	void handleNoteEvent(bool isNoteOn, unsigned int key, unsigned int velocity);
};