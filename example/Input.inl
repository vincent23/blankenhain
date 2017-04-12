unsigned int track_0_samplePositions[] = {
	0, 0, 0, 44100, 44100, 44100, 44100, 44100, 44100, 88200, 88200, 88200, 88200, 88200, 88200, 132300, 132300, 132300, 132300, 132300, 132300, 176400, 176400, 176400, 176400, 176400, 176400, 220500, 220500, 220500, 220500, 220500, 220500, 264600, 264600, 264600, 264600, 264600, 264600, 308700, 308700, 308700, 308700, 308700, 308700, 352800, 352800, 352800
};
unsigned int track_0_keys[] = {
	48, 52, 55, 48, 50, 52, 55, 55, 59, 48, 50, 52, 55, 55, 59, 48, 50, 52, 55, 55, 59, 48, 50, 52, 55, 55, 59, 48, 50, 52, 55, 55, 59, 48, 50, 52, 55, 55, 59, 48, 50, 52, 55, 55, 59, 50, 55, 59
};
unsigned int track_0_velocities[] = {
	100, 100, 100, 0, 100, 0, 0, 100, 100, 100, 0, 100, 0, 100, 0, 0, 100, 0, 0, 100, 100, 100, 0, 100, 0, 100, 0, 0, 100, 0, 0, 100, 100, 100, 0, 100, 0, 100, 0, 0, 100, 0, 0, 100, 100, 0, 0, 0
};
MidiTrack track_0(48, track_0_samplePositions, track_0_keys, track_0_velocities);
unsigned int track_1_samplePositions[] = {
	0, 11025, 22050, 33075, 44100, 55125, 66150, 77175, 88200, 99225, 110250, 121275, 132300, 143325, 154350, 165375, 176400, 187425, 198450, 209475, 220500, 231525, 242550, 253575, 264600, 275625, 286650, 297675, 308700, 319725, 330750, 341775
};
unsigned int track_1_keys[] = {
	60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60, 60
};
unsigned int track_1_velocities[] = {
	100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0
};
MidiTrack track_1(32, track_1_samplePositions, track_1_keys, track_1_velocities);
unsigned int device_0_parameter_0_samplePositions[] = {
	0
};
float device_0_parameter_0_values[] = {
	0.02696926f
};
unsigned int device_0_parameter_1_samplePositions[] = {
	0
};
float device_0_parameter_1_values[] = {
	0.42621872f
};
unsigned int device_0_parameter_2_samplePositions[] = {
	0
};
float device_0_parameter_2_values[] = {
	1.00000000f
};
unsigned int device_0_parameter_3_samplePositions[] = {
	0
};
float device_0_parameter_3_values[] = {
	0.42621872f
};
unsigned int device_0_parameter_4_samplePositions[] = {
	0
};
float device_0_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_0_parameter_5_samplePositions[] = {
	0
};
float device_0_parameter_5_values[] = {
	0.42621872f
};
unsigned int device_0_parameter_6_samplePositions[] = {
	0
};
float device_0_parameter_6_values[] = {
	1.00000000f
};
unsigned int device_0_parameter_7_samplePositions[] = {
	0
};
float device_0_parameter_7_values[] = {
	0.42621872f
};
unsigned int device_0_parameter_8_samplePositions[] = {
	0
};
float device_0_parameter_8_values[] = {
	1.00000000f
};
unsigned int device_0_parameter_9_samplePositions[] = {
	0
};
float device_0_parameter_9_values[] = {
	0.92340428f
};
unsigned int device_0_parameter_10_samplePositions[] = {
	0
};
float device_0_parameter_10_values[] = {
	1.00000000f
};
ParameterTrack device_0_parameters[] = {
	ParameterTrack(1, device_0_parameter_0_samplePositions, device_0_parameter_0_values),
	ParameterTrack(1, device_0_parameter_1_samplePositions, device_0_parameter_1_values),
	ParameterTrack(1, device_0_parameter_2_samplePositions, device_0_parameter_2_values),
	ParameterTrack(1, device_0_parameter_3_samplePositions, device_0_parameter_3_values),
	ParameterTrack(1, device_0_parameter_4_samplePositions, device_0_parameter_4_values),
	ParameterTrack(1, device_0_parameter_5_samplePositions, device_0_parameter_5_values),
	ParameterTrack(1, device_0_parameter_6_samplePositions, device_0_parameter_6_values),
	ParameterTrack(1, device_0_parameter_7_samplePositions, device_0_parameter_7_values),
	ParameterTrack(1, device_0_parameter_8_samplePositions, device_0_parameter_8_values),
	ParameterTrack(1, device_0_parameter_9_samplePositions, device_0_parameter_9_values),
	ParameterTrack(1, device_0_parameter_10_samplePositions, device_0_parameter_10_values),
};
gmsynthInstrument device_0_instrument;
InstrumentDevice device_0(device_0_instrument, device_0_parameters, 0);
gmSoundRegion device_0_instrument_regions[1];
device_0_instrument_regions[0].lowest = 0;
device_0_instrument_regions[0].highest = 127;
device_0_instrument_regions[0].rootNote = 64;
device_0_instrument_regions[0].sampleLength = 34868;
device_0_instrument_regions[0].startByte = 1507508;
device_0_instrument_regions[0].isLoopable = false;
gmInstrument device_0_instrument_gm_instrument(nullptr, 1, device_0_instrument_regions);
device_0_instrument.loadMidiInstrument(device_0_instrument_gm_instrument);
unsigned int device_1_parameter_0_samplePositions[] = {
	0
};
float device_1_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_1_samplePositions[] = {
	0
};
float device_1_parameter_1_values[] = {
	0.75000000f
};
unsigned int device_1_parameter_2_samplePositions[] = {
	0, 39359, 198640, 198640, 220711, 247687, 434066
};
float device_1_parameter_2_values[] = {
	0.92424244f, 0.92424244f, 0.18181819f, 0.75757575f, 0.84848487f, 0.33333334f, 0.60606062f
};
unsigned int device_1_parameter_3_samplePositions[] = {
	0
};
float device_1_parameter_3_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_4_samplePositions[] = {
	0
};
float device_1_parameter_4_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_5_samplePositions[] = {
	0
};
float device_1_parameter_5_values[] = {
	0.84999996f
};
unsigned int device_1_parameter_6_samplePositions[] = {
	0
};
float device_1_parameter_6_values[] = {
	0.75000000f
};
unsigned int device_1_parameter_7_samplePositions[] = {
	0
};
float device_1_parameter_7_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_8_samplePositions[] = {
	0
};
float device_1_parameter_8_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_9_samplePositions[] = {
	0
};
float device_1_parameter_9_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_10_samplePositions[] = {
	0
};
float device_1_parameter_10_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_11_samplePositions[] = {
	0
};
float device_1_parameter_11_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_12_samplePositions[] = {
	0
};
float device_1_parameter_12_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_13_samplePositions[] = {
	0
};
float device_1_parameter_13_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_14_samplePositions[] = {
	0
};
float device_1_parameter_14_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_15_samplePositions[] = {
	0
};
float device_1_parameter_15_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_16_samplePositions[] = {
	0
};
float device_1_parameter_16_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_17_samplePositions[] = {
	0
};
float device_1_parameter_17_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_18_samplePositions[] = {
	0
};
float device_1_parameter_18_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_19_samplePositions[] = {
	0
};
float device_1_parameter_19_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_20_samplePositions[] = {
	0
};
float device_1_parameter_20_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_21_samplePositions[] = {
	0
};
float device_1_parameter_21_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_22_samplePositions[] = {
	0
};
float device_1_parameter_22_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_23_samplePositions[] = {
	0
};
float device_1_parameter_23_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_24_samplePositions[] = {
	0
};
float device_1_parameter_24_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_25_samplePositions[] = {
	0
};
float device_1_parameter_25_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_26_samplePositions[] = {
	0
};
float device_1_parameter_26_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_27_samplePositions[] = {
	0
};
float device_1_parameter_27_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_28_samplePositions[] = {
	0
};
float device_1_parameter_28_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_29_samplePositions[] = {
	0
};
float device_1_parameter_29_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_30_samplePositions[] = {
	0
};
float device_1_parameter_30_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_31_samplePositions[] = {
	0
};
float device_1_parameter_31_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_32_samplePositions[] = {
	0
};
float device_1_parameter_32_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_33_samplePositions[] = {
	0
};
float device_1_parameter_33_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_34_samplePositions[] = {
	0
};
float device_1_parameter_34_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_35_samplePositions[] = {
	0
};
float device_1_parameter_35_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_36_samplePositions[] = {
	0
};
float device_1_parameter_36_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_37_samplePositions[] = {
	0
};
float device_1_parameter_37_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_38_samplePositions[] = {
	0
};
float device_1_parameter_38_values[] = {
	0.50000000f
};
unsigned int device_1_parameter_39_samplePositions[] = {
	0
};
float device_1_parameter_39_values[] = {
	0.41588983f
};
unsigned int device_1_parameter_40_samplePositions[] = {
	0
};
float device_1_parameter_40_values[] = {
	0.00000000f
};
unsigned int device_1_parameter_41_samplePositions[] = {
	0
};
float device_1_parameter_41_values[] = {
	1.00000000f
};
ParameterTrack device_1_parameters[] = {
	ParameterTrack(1, device_1_parameter_0_samplePositions, device_1_parameter_0_values),
	ParameterTrack(1, device_1_parameter_1_samplePositions, device_1_parameter_1_values),
	ParameterTrack(7, device_1_parameter_2_samplePositions, device_1_parameter_2_values),
	ParameterTrack(1, device_1_parameter_3_samplePositions, device_1_parameter_3_values),
	ParameterTrack(1, device_1_parameter_4_samplePositions, device_1_parameter_4_values),
	ParameterTrack(1, device_1_parameter_5_samplePositions, device_1_parameter_5_values),
	ParameterTrack(1, device_1_parameter_6_samplePositions, device_1_parameter_6_values),
	ParameterTrack(1, device_1_parameter_7_samplePositions, device_1_parameter_7_values),
	ParameterTrack(1, device_1_parameter_8_samplePositions, device_1_parameter_8_values),
	ParameterTrack(1, device_1_parameter_9_samplePositions, device_1_parameter_9_values),
	ParameterTrack(1, device_1_parameter_10_samplePositions, device_1_parameter_10_values),
	ParameterTrack(1, device_1_parameter_11_samplePositions, device_1_parameter_11_values),
	ParameterTrack(1, device_1_parameter_12_samplePositions, device_1_parameter_12_values),
	ParameterTrack(1, device_1_parameter_13_samplePositions, device_1_parameter_13_values),
	ParameterTrack(1, device_1_parameter_14_samplePositions, device_1_parameter_14_values),
	ParameterTrack(1, device_1_parameter_15_samplePositions, device_1_parameter_15_values),
	ParameterTrack(1, device_1_parameter_16_samplePositions, device_1_parameter_16_values),
	ParameterTrack(1, device_1_parameter_17_samplePositions, device_1_parameter_17_values),
	ParameterTrack(1, device_1_parameter_18_samplePositions, device_1_parameter_18_values),
	ParameterTrack(1, device_1_parameter_19_samplePositions, device_1_parameter_19_values),
	ParameterTrack(1, device_1_parameter_20_samplePositions, device_1_parameter_20_values),
	ParameterTrack(1, device_1_parameter_21_samplePositions, device_1_parameter_21_values),
	ParameterTrack(1, device_1_parameter_22_samplePositions, device_1_parameter_22_values),
	ParameterTrack(1, device_1_parameter_23_samplePositions, device_1_parameter_23_values),
	ParameterTrack(1, device_1_parameter_24_samplePositions, device_1_parameter_24_values),
	ParameterTrack(1, device_1_parameter_25_samplePositions, device_1_parameter_25_values),
	ParameterTrack(1, device_1_parameter_26_samplePositions, device_1_parameter_26_values),
	ParameterTrack(1, device_1_parameter_27_samplePositions, device_1_parameter_27_values),
	ParameterTrack(1, device_1_parameter_28_samplePositions, device_1_parameter_28_values),
	ParameterTrack(1, device_1_parameter_29_samplePositions, device_1_parameter_29_values),
	ParameterTrack(1, device_1_parameter_30_samplePositions, device_1_parameter_30_values),
	ParameterTrack(1, device_1_parameter_31_samplePositions, device_1_parameter_31_values),
	ParameterTrack(1, device_1_parameter_32_samplePositions, device_1_parameter_32_values),
	ParameterTrack(1, device_1_parameter_33_samplePositions, device_1_parameter_33_values),
	ParameterTrack(1, device_1_parameter_34_samplePositions, device_1_parameter_34_values),
	ParameterTrack(1, device_1_parameter_35_samplePositions, device_1_parameter_35_values),
	ParameterTrack(1, device_1_parameter_36_samplePositions, device_1_parameter_36_values),
	ParameterTrack(1, device_1_parameter_37_samplePositions, device_1_parameter_37_values),
	ParameterTrack(1, device_1_parameter_38_samplePositions, device_1_parameter_38_values),
	ParameterTrack(1, device_1_parameter_39_samplePositions, device_1_parameter_39_values),
	ParameterTrack(1, device_1_parameter_40_samplePositions, device_1_parameter_40_values),
	ParameterTrack(1, device_1_parameter_41_samplePositions, device_1_parameter_41_values),
};
EqualizerEffect device_1_effect;
EffectDevice device_1(device_1_effect, device_1_parameters);
unsigned int device_2_parameter_0_samplePositions[] = {
	0
};
float device_2_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_2_parameter_1_samplePositions[] = {
	0
};
float device_2_parameter_1_values[] = {
	0.50000000f
};
unsigned int device_2_parameter_2_samplePositions[] = {
	0
};
float device_2_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_2_parameter_3_samplePositions[] = {
	0
};
float device_2_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_2_parameter_4_samplePositions[] = {
	0
};
float device_2_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_2_parameter_5_samplePositions[] = {
	0
};
float device_2_parameter_5_values[] = {
	0.00000000f
};
unsigned int device_2_parameter_6_samplePositions[] = {
	0
};
float device_2_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_2_parameter_7_samplePositions[] = {
	0
};
float device_2_parameter_7_values[] = {
	0.00000000f
};
unsigned int device_2_parameter_8_samplePositions[] = {
	0
};
float device_2_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_2_parameter_9_samplePositions[] = {
	0
};
float device_2_parameter_9_values[] = {
	1.00000000f
};
ParameterTrack device_2_parameters[] = {
	ParameterTrack(1, device_2_parameter_0_samplePositions, device_2_parameter_0_values),
	ParameterTrack(1, device_2_parameter_1_samplePositions, device_2_parameter_1_values),
	ParameterTrack(1, device_2_parameter_2_samplePositions, device_2_parameter_2_values),
	ParameterTrack(1, device_2_parameter_3_samplePositions, device_2_parameter_3_values),
	ParameterTrack(1, device_2_parameter_4_samplePositions, device_2_parameter_4_values),
	ParameterTrack(1, device_2_parameter_5_samplePositions, device_2_parameter_5_values),
	ParameterTrack(1, device_2_parameter_6_samplePositions, device_2_parameter_6_values),
	ParameterTrack(1, device_2_parameter_7_samplePositions, device_2_parameter_7_values),
	ParameterTrack(1, device_2_parameter_8_samplePositions, device_2_parameter_8_values),
	ParameterTrack(1, device_2_parameter_9_samplePositions, device_2_parameter_9_values),
};
WidthEffect device_2_effect;
EffectDevice device_2(device_2_effect, device_2_parameters);
Device* deviceList_3[] = {
	&device_0, &device_1, &device_2
};
ChainDevice device_3(3, deviceList_3);
unsigned int device_4_parameter_0_samplePositions[] = {
	0
};
float device_4_parameter_0_values[] = {
	0.02696926f
};
unsigned int device_4_parameter_1_samplePositions[] = {
	0
};
float device_4_parameter_1_values[] = {
	0.42621872f
};
unsigned int device_4_parameter_2_samplePositions[] = {
	0
};
float device_4_parameter_2_values[] = {
	1.00000000f
};
unsigned int device_4_parameter_3_samplePositions[] = {
	0
};
float device_4_parameter_3_values[] = {
	0.42621872f
};
unsigned int device_4_parameter_4_samplePositions[] = {
	0
};
float device_4_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_4_parameter_5_samplePositions[] = {
	0
};
float device_4_parameter_5_values[] = {
	0.42621872f
};
unsigned int device_4_parameter_6_samplePositions[] = {
	0
};
float device_4_parameter_6_values[] = {
	1.00000000f
};
unsigned int device_4_parameter_7_samplePositions[] = {
	0
};
float device_4_parameter_7_values[] = {
	0.42621872f
};
unsigned int device_4_parameter_8_samplePositions[] = {
	0
};
float device_4_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_4_parameter_9_samplePositions[] = {
	0
};
float device_4_parameter_9_values[] = {
	0.77021277f
};
unsigned int device_4_parameter_10_samplePositions[] = {
	0
};
float device_4_parameter_10_values[] = {
	1.00000000f
};
ParameterTrack device_4_parameters[] = {
	ParameterTrack(1, device_4_parameter_0_samplePositions, device_4_parameter_0_values),
	ParameterTrack(1, device_4_parameter_1_samplePositions, device_4_parameter_1_values),
	ParameterTrack(1, device_4_parameter_2_samplePositions, device_4_parameter_2_values),
	ParameterTrack(1, device_4_parameter_3_samplePositions, device_4_parameter_3_values),
	ParameterTrack(1, device_4_parameter_4_samplePositions, device_4_parameter_4_values),
	ParameterTrack(1, device_4_parameter_5_samplePositions, device_4_parameter_5_values),
	ParameterTrack(1, device_4_parameter_6_samplePositions, device_4_parameter_6_values),
	ParameterTrack(1, device_4_parameter_7_samplePositions, device_4_parameter_7_values),
	ParameterTrack(1, device_4_parameter_8_samplePositions, device_4_parameter_8_values),
	ParameterTrack(1, device_4_parameter_9_samplePositions, device_4_parameter_9_values),
	ParameterTrack(1, device_4_parameter_10_samplePositions, device_4_parameter_10_values),
};
gmsynthInstrument device_4_instrument;
InstrumentDevice device_4(device_4_instrument, device_4_parameters, 1);
gmSoundRegion device_4_instrument_regions[1];
device_4_instrument_regions[0].lowest = 0;
device_4_instrument_regions[0].highest = 127;
device_4_instrument_regions[0].rootNote = 72;
device_4_instrument_regions[0].sampleLength = 1872;
device_4_instrument_regions[0].startByte = 3087450;
device_4_instrument_regions[0].isLoopable = true;
device_4_instrument_regions[0].loopStart = 831;
device_4_instrument_regions[0].loopLength = 104;
gmInstrument device_4_instrument_gm_instrument(nullptr, 1, device_4_instrument_regions);
device_4_instrument.loadMidiInstrument(device_4_instrument_gm_instrument);
unsigned int device_5_parameter_0_samplePositions[] = {
	0
};
float device_5_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_5_parameter_1_samplePositions[] = {
	0
};
float device_5_parameter_1_values[] = {
	0.50000000f
};
unsigned int device_5_parameter_2_samplePositions[] = {
	0
};
float device_5_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_5_parameter_3_samplePositions[] = {
	0
};
float device_5_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_5_parameter_4_samplePositions[] = {
	0
};
float device_5_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_5_parameter_5_samplePositions[] = {
	0
};
float device_5_parameter_5_values[] = {
	0.00000000f
};
unsigned int device_5_parameter_6_samplePositions[] = {
	0
};
float device_5_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_5_parameter_7_samplePositions[] = {
	0
};
float device_5_parameter_7_values[] = {
	0.00000000f
};
unsigned int device_5_parameter_8_samplePositions[] = {
	0
};
float device_5_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_5_parameter_9_samplePositions[] = {
	0
};
float device_5_parameter_9_values[] = {
	1.00000000f
};
ParameterTrack device_5_parameters[] = {
	ParameterTrack(1, device_5_parameter_0_samplePositions, device_5_parameter_0_values),
	ParameterTrack(1, device_5_parameter_1_samplePositions, device_5_parameter_1_values),
	ParameterTrack(1, device_5_parameter_2_samplePositions, device_5_parameter_2_values),
	ParameterTrack(1, device_5_parameter_3_samplePositions, device_5_parameter_3_values),
	ParameterTrack(1, device_5_parameter_4_samplePositions, device_5_parameter_4_values),
	ParameterTrack(1, device_5_parameter_5_samplePositions, device_5_parameter_5_values),
	ParameterTrack(1, device_5_parameter_6_samplePositions, device_5_parameter_6_values),
	ParameterTrack(1, device_5_parameter_7_samplePositions, device_5_parameter_7_values),
	ParameterTrack(1, device_5_parameter_8_samplePositions, device_5_parameter_8_values),
	ParameterTrack(1, device_5_parameter_9_samplePositions, device_5_parameter_9_values),
};
WidthEffect device_5_effect;
EffectDevice device_5(device_5_effect, device_5_parameters);
Device* deviceList_6[] = {
	&device_4, &device_5
};
ChainDevice device_6(2, deviceList_6);
ReturnDevice device_7(0);
unsigned int device_8_parameter_0_samplePositions[] = {
	0
};
float device_8_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_8_parameter_1_samplePositions[] = {
	0
};
float device_8_parameter_1_values[] = {
	0.50000000f
};
unsigned int device_8_parameter_2_samplePositions[] = {
	0
};
float device_8_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_8_parameter_3_samplePositions[] = {
	0
};
float device_8_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_8_parameter_4_samplePositions[] = {
	0
};
float device_8_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_8_parameter_5_samplePositions[] = {
	0
};
float device_8_parameter_5_values[] = {
	0.00000000f
};
unsigned int device_8_parameter_6_samplePositions[] = {
	0
};
float device_8_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_8_parameter_7_samplePositions[] = {
	0
};
float device_8_parameter_7_values[] = {
	0.00000000f
};
unsigned int device_8_parameter_8_samplePositions[] = {
	0
};
float device_8_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_8_parameter_9_samplePositions[] = {
	0
};
float device_8_parameter_9_values[] = {
	1.00000000f
};
ParameterTrack device_8_parameters[] = {
	ParameterTrack(1, device_8_parameter_0_samplePositions, device_8_parameter_0_values),
	ParameterTrack(1, device_8_parameter_1_samplePositions, device_8_parameter_1_values),
	ParameterTrack(1, device_8_parameter_2_samplePositions, device_8_parameter_2_values),
	ParameterTrack(1, device_8_parameter_3_samplePositions, device_8_parameter_3_values),
	ParameterTrack(1, device_8_parameter_4_samplePositions, device_8_parameter_4_values),
	ParameterTrack(1, device_8_parameter_5_samplePositions, device_8_parameter_5_values),
	ParameterTrack(1, device_8_parameter_6_samplePositions, device_8_parameter_6_values),
	ParameterTrack(1, device_8_parameter_7_samplePositions, device_8_parameter_7_values),
	ParameterTrack(1, device_8_parameter_8_samplePositions, device_8_parameter_8_values),
	ParameterTrack(1, device_8_parameter_9_samplePositions, device_8_parameter_9_values),
};
WidthEffect device_8_effect;
EffectDevice device_8(device_8_effect, device_8_parameters);
Device* deviceList_9[] = {
	&device_7, &device_8
};
ChainDevice device_9(2, deviceList_9);
ReturnDevice device_10(1);
unsigned int device_11_parameter_0_samplePositions[] = {
	0
};
float device_11_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_11_parameter_1_samplePositions[] = {
	0
};
float device_11_parameter_1_values[] = {
	0.50000000f
};
unsigned int device_11_parameter_2_samplePositions[] = {
	0
};
float device_11_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_11_parameter_3_samplePositions[] = {
	0
};
float device_11_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_11_parameter_4_samplePositions[] = {
	0
};
float device_11_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_11_parameter_5_samplePositions[] = {
	0
};
float device_11_parameter_5_values[] = {
	0.00000000f
};
unsigned int device_11_parameter_6_samplePositions[] = {
	0
};
float device_11_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_11_parameter_7_samplePositions[] = {
	0
};
float device_11_parameter_7_values[] = {
	0.00000000f
};
unsigned int device_11_parameter_8_samplePositions[] = {
	0
};
float device_11_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_11_parameter_9_samplePositions[] = {
	0
};
float device_11_parameter_9_values[] = {
	1.00000000f
};
ParameterTrack device_11_parameters[] = {
	ParameterTrack(1, device_11_parameter_0_samplePositions, device_11_parameter_0_values),
	ParameterTrack(1, device_11_parameter_1_samplePositions, device_11_parameter_1_values),
	ParameterTrack(1, device_11_parameter_2_samplePositions, device_11_parameter_2_values),
	ParameterTrack(1, device_11_parameter_3_samplePositions, device_11_parameter_3_values),
	ParameterTrack(1, device_11_parameter_4_samplePositions, device_11_parameter_4_values),
	ParameterTrack(1, device_11_parameter_5_samplePositions, device_11_parameter_5_values),
	ParameterTrack(1, device_11_parameter_6_samplePositions, device_11_parameter_6_values),
	ParameterTrack(1, device_11_parameter_7_samplePositions, device_11_parameter_7_values),
	ParameterTrack(1, device_11_parameter_8_samplePositions, device_11_parameter_8_values),
	ParameterTrack(1, device_11_parameter_9_samplePositions, device_11_parameter_9_values),
};
WidthEffect device_11_effect;
EffectDevice device_11(device_11_effect, device_11_parameters);
Device* deviceList_12[] = {
	&device_10, &device_11
};
ChainDevice device_12(2, deviceList_12);
Device* deviceList_13[] = {
	&device_3, &device_6, &device_9, &device_12
};
GroupDevice device_13(4, deviceList_13);
unsigned int device_14_parameter_0_samplePositions[] = {
	0
};
float device_14_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_14_parameter_1_samplePositions[] = {
	0
};
float device_14_parameter_1_values[] = {
	0.50000000f
};
unsigned int device_14_parameter_2_samplePositions[] = {
	0
};
float device_14_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_14_parameter_3_samplePositions[] = {
	0
};
float device_14_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_14_parameter_4_samplePositions[] = {
	0
};
float device_14_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_14_parameter_5_samplePositions[] = {
	0
};
float device_14_parameter_5_values[] = {
	0.00000000f
};
unsigned int device_14_parameter_6_samplePositions[] = {
	0
};
float device_14_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_14_parameter_7_samplePositions[] = {
	0
};
float device_14_parameter_7_values[] = {
	0.00000000f
};
unsigned int device_14_parameter_8_samplePositions[] = {
	0
};
float device_14_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_14_parameter_9_samplePositions[] = {
	0
};
float device_14_parameter_9_values[] = {
	1.00000000f
};
ParameterTrack device_14_parameters[] = {
	ParameterTrack(1, device_14_parameter_0_samplePositions, device_14_parameter_0_values),
	ParameterTrack(1, device_14_parameter_1_samplePositions, device_14_parameter_1_values),
	ParameterTrack(1, device_14_parameter_2_samplePositions, device_14_parameter_2_values),
	ParameterTrack(1, device_14_parameter_3_samplePositions, device_14_parameter_3_values),
	ParameterTrack(1, device_14_parameter_4_samplePositions, device_14_parameter_4_values),
	ParameterTrack(1, device_14_parameter_5_samplePositions, device_14_parameter_5_values),
	ParameterTrack(1, device_14_parameter_6_samplePositions, device_14_parameter_6_values),
	ParameterTrack(1, device_14_parameter_7_samplePositions, device_14_parameter_7_values),
	ParameterTrack(1, device_14_parameter_8_samplePositions, device_14_parameter_8_values),
	ParameterTrack(1, device_14_parameter_9_samplePositions, device_14_parameter_9_values),
};
WidthEffect device_14_effect;
EffectDevice device_14(device_14_effect, device_14_parameters);
Device* deviceList_15[] = {
	&device_13, &device_14
};
ChainDevice device_15(2, deviceList_15);
MidiTrack* midiTracks[] = {
	&track_0, &track_1
};
Sample sendBuffers[constants::blockSize * 2];
SongInfo songInfo(midiTracks, sendBuffers, 2, 120.0, 352800);
Song song(songInfo, device_15);
