midi = 'midi'
instrument = 'instrument'
effect = 'effect'

plugins = {
	'bh_arp': {
		'class': 'ArpMidiEffect',
		'type': midi,
		'numberOfParameters': 1,
	},
	'bh_basedrum1_synth': {
		'class': 'basedrum1Instrument',
		'type': instrument,
		'numberOfParameters': 14,
	},
	'bh_bitcrush': {
		'class': 'BitcrushEffect',
		'type': effect,
		'numberOfParameters': 4,
	},
	'bh_chorus': {
		'class': 'ChorusEffect',
		'type': effect,
		'numberOfParameters': 11,
	},
	'bh_compressor': {
		'class': 'CompressorEffect',
		'type': effect,
		'numberOfParameters': 8,
	},
	'bh_delay': {
		'class': 'DelayEffect',
		'type': effect,
		'numberOfParameters': 7,
	},
	'bh_distortion': {
		'class': 'DistortionEffect',
		'type': effect,
		'numberOfParameters': 4,
	},
	'bh_equalizer': {
		'class': 'EqualizerEffect',
		'type': effect,
		'numberOfParameters': 41,
	},
	'bh_example': {
		'class': 'ExampleEffect',
		'type': effect,
		'numberOfParameters': 5,
	},
	'bh_example_synth': {
		'class': 'ExampleInstrument',
		'type': instrument,
		'numberOfParameters': 9,
	},
	'bh_filter': {
		'class': 'FilterEffect',
		'type': effect,
		'numberOfParameters': 4,
	},
	'bh_fm_synth': {
		'class': 'FmInstrument',
		'type': instrument,
		'numberOfParameters': 116,
	},
	'bh_freeverb': {
		'class': 'freeverbEffect',
		'type': effect,
		'numberOfParameters': 6,
	},
	'bh_glide_polyblep_synth': {
		'class': 'glidePolyblepInstrument',
		'type': instrument,
		'numberOfParameters': 18,
	},
	'bh_gm_synth': {
		'class': 'gmsynthInstrument',
		'type': instrument,
		'numberOfParameters': 10,
	},
	'bh_limiter': {
		'class': 'LimiterEffect',
		'type': effect,
		'numberOfParameters': 5,
	},
	'bh_noisegate': {
		'class': 'NoiseGateEffect',
		'type': effect,
		'numberOfParameters': 5,
	},
	'bh_pitchshift_midi': {
		'class': 'pitchshiftMidiEffect',
		'type': midi,
		'numberOfParameters': 1,
	},
	'bh_polyblep_synth': {
		'class': 'polyblepInstrument',
		'type': instrument,
		'numberOfParameters': 9,
	},
	'bh_rhythmgate': {
		'class': 'RhythmgateEffect',
		'type': effect,
		'numberOfParameters': 20,
	},
	'bh_volume': {
		'class': 'VolumeEffect',
		'type': effect,
		'numberOfParameters': 10,
	},
	'bh_wavetable_synth': {
		'class': 'wavetableInstrument',
		'type': instrument,
		'numberOfParameters': 9,
	},
	'bh_width': {
		'class': 'WidthEffect',
		'type': effect,
		'numberOfParameters': 9,
	},
}
