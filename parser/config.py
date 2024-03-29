midi = 'midi'
instrument = 'instrument'
effect = 'effect'

plugins = {
	'bh_abletonMixerVolume': {
		'class': 'AbletonTrackMixerVolumeEffect',
		'type': effect,
		'numberOfParameters': 1,
	},
	'bh_bitcrush': {
		'class': 'BitcrushEffect',
		'type': effect,
		'numberOfParameters': 3,
	},
	'bh_chorus': {
		'class': 'ChorusEffect',
		'type': effect,
		'numberOfParameters': 12,
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
	'bh_filter': {
		'class': 'FilterEffect',
		'type': effect,
		'numberOfParameters': 11,
	},
	'bh_fm_synth': {
		'class': 'FmInstrument',
		'type': instrument,
		'numberOfParameters': 64,
	},
	'bh_reverb': {
		'class': 'ReverbEffect',
		'type': effect,
		'numberOfParameters': 4,
	},
	'bh_polysynth': {
		'class': 'PolysynthInstrument',
		'type': instrument,
		'numberOfParameters': 10,
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
		'numberOfParameters': 6,
	},
	'bh_monosynth': {
		'class': 'monosynthInstrument',
		'type': instrument,
		'numberOfParameters': 18,
	},
	'bh_rhythmgate': {
		'class': 'RhythmgateEffect',
		'type': effect,
		'numberOfParameters': 21,
	},
	'bh_volume': {
		'class': 'VolumeEffect',
		'type': effect,
		'numberOfParameters': 10,
	},
	'bh_pan': {
		'class': 'PanEffect',
		'type': effect,
		'numberOfParameters': 10,
	},
}
