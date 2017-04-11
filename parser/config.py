midi = 'midi'
instrument = 'instrument'
effect = 'effect'

plugins = {
	'bh_arp': {
		'class': 'ArpMidiEffect',
		'type': midi
	},
	'bh_basedrum1_synth': {
		'class': 'basedrum1Instrument',
		'type': instrument
	},
	'bh_bitcrush': {
		'class': 'BitcrushEffect',
		'type': effect
	},
	'bh_chorus': {
		'class': 'ChorusEffect',
		'type': effect
	},
	'bh_compressor': {
		'class': 'CompressorEffect',
		'type': effect
	},
	'bh_delay': {
		'class': 'DelayEffect',
		'type': effect
	},
	'bh_distortion': {
		'class': 'DistortionEffect',
		'type': effect
	},
	'bh_equalizer': {
		'class': 'EqualizerEffect',
		'type': effect
	},
	'bh_example': {
		'class': 'ExampleEffect',
		'type': effect
	},
	'bh_example_synth': {
		'class': 'ExampleInstrument',
		'type': instrument
	},
	'bh_filter': {
		'class': 'FilterEffect',
		'type': effect
	},
	'bh_fm_synth': {
		'class': 'FmInstrument',
		'type': instrument
	},
	'bh_freeverb': {
		'class': 'freeverbEffect',
		'type': effect
	},
	'bh_glide_polyblep_synth': {
		'class': 'glidePolyblepInstrument',
		'type': instrument
	},
	'bh_gm_synth': {
		'class': 'gmsynthInstrument',
		'type': instrument
	},
	'bh_noisegate': {
		'class': 'NoiseGateEffect',
		'type': effect
	},
	'bh_pitchshift_midi': {
		'class': 'pitchshiftMidiEffect',
		'type': midi
	},
	'bh_polyblep_synth': {
		'class': 'polyblepInstrument',
		'type': instrument
	},
	'bh_rhythmgate': {
		'class': 'RhythmgateEffect',
		'type': effect
	},
	'bh_volume': {
		'class': 'VolumeEffect',
		'type': effect
	},
	'bh_wavetable_synth': {
		'class': 'wavetableInstrument',
		'type': instrument
	},
	'bh_width': {
		'class': 'WidthEffect',
		'type': effect
	},
}
