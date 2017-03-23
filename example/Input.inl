unsigned int track_0_samplePositions[] = {
	0, 11025, 22050, 33075, 44100, 55125, 66150, 77175, 88200, 99225, 110250, 121275, 132300, 143325, 154350, 165375, 176400, 187425, 198450, 209475, 220500, 231525, 242550, 253575, 264600, 275625, 286650, 297675, 308700, 319725, 330750, 341775
};
unsigned int track_0_keys[] = {
	48, 48, 48, 48, 48, 48, 48, 48, 52, 52, 52, 52, 52, 52, 52, 52, 55, 55, 55, 55, 55, 55, 55, 55, 52, 52, 52, 52, 52, 52, 52, 52
};
unsigned int track_0_velocities[] = {
	100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0, 100, 0
};
MidiTrack track_0(32, track_0_samplePositions, track_0_keys, track_0_velocities);
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
	0.56060606f
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
	0.69900006f
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
	0.00000000f
};
unsigned int device_0_parameter_9_samplePositions[] = {
	0
};
float device_0_parameter_9_values[] = {
	0.22978723f
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
gmSoundRegion device_0_instrument_regions[4];
device_0_instrument_regions[0].lowest = 0;
device_0_instrument_regions[0].highest = 47;
device_0_instrument_regions[0].rootNote = 49;
device_0_instrument_regions[0].sampleLength = 11810;
device_0_instrument_regions[0].startByte = 2831874;
device_0_instrument_regions[0].isLoopable = true;
device_0_instrument_regions[0].loopStart = 4790;
device_0_instrument_regions[0].loopLength = 1114;
device_0_instrument_regions[1].lowest = 48;
device_0_instrument_regions[1].highest = 69;
device_0_instrument_regions[1].rootNote = 55;
device_0_instrument_regions[1].sampleLength = 12024;
device_0_instrument_regions[1].startByte = 286226;
device_0_instrument_regions[1].isLoopable = true;
device_0_instrument_regions[1].loopStart = 5448;
device_0_instrument_regions[1].loopLength = 563;
device_0_instrument_regions[2].lowest = 70;
device_0_instrument_regions[2].highest = 86;
device_0_instrument_regions[2].rootNote = 79;
device_0_instrument_regions[2].sampleLength = 6250;
device_0_instrument_regions[2].startByte = 298418;
device_0_instrument_regions[2].isLoopable = true;
device_0_instrument_regions[2].loopStart = 2337;
device_0_instrument_regions[2].loopLength = 787;
device_0_instrument_regions[3].lowest = 87;
device_0_instrument_regions[3].highest = 127;
device_0_instrument_regions[3].rootNote = 89;
device_0_instrument_regions[3].sampleLength = 4820;
device_0_instrument_regions[3].startByte = 304836;
device_0_instrument_regions[3].isLoopable = true;
device_0_instrument_regions[3].loopStart = 1827;
device_0_instrument_regions[3].loopLength = 582;
gmInstrument device_0_instrument_gm_instrument(nullptr, 4, device_0_instrument_regions);
device_0_instrument.loadMidiInstrument(device_0_instrument_gm_instrument);
unsigned int device_1_parameter_0_samplePositions[] = {
	0
};
float device_1_parameter_0_values[] = {
	0.68493152f
};
unsigned int device_1_parameter_1_samplePositions[] = {
	0
};
float device_1_parameter_1_values[] = {
	0.38356164f
};
unsigned int device_1_parameter_2_samplePositions[] = {
	0
};
float device_1_parameter_2_values[] = {
	1.00000000f
};
unsigned int device_1_parameter_3_samplePositions[] = {
	0
};
float device_1_parameter_3_values[] = {
	1.00000000f
};
unsigned int device_1_parameter_4_samplePositions[] = {
	0
};
float device_1_parameter_4_values[] = {
	1.00000000f
};
ParameterTrack device_1_parameters[] = {
	ParameterTrack(1, device_1_parameter_0_samplePositions, device_1_parameter_0_values),
	ParameterTrack(1, device_1_parameter_1_samplePositions, device_1_parameter_1_values),
	ParameterTrack(1, device_1_parameter_2_samplePositions, device_1_parameter_2_values),
	ParameterTrack(1, device_1_parameter_3_samplePositions, device_1_parameter_3_values),
	ParameterTrack(1, device_1_parameter_4_samplePositions, device_1_parameter_4_values),
};
DistortionEffect device_1_effect;
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
	0.37963012f
};
unsigned int device_2_parameter_2_samplePositions[] = {
	0
};
float device_2_parameter_2_values[] = {
	0.20547946f
};
unsigned int device_2_parameter_3_samplePositions[] = {
	0
};
float device_2_parameter_3_values[] = {
	1.00000000f
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
};
DelayEffect device_2_effect;
EffectDevice device_2(device_2_effect, device_2_parameters);
unsigned int device_3_parameter_0_samplePositions[] = {
	0
};
float device_3_parameter_0_values[] = {
	0.75000000f
};
unsigned int device_3_parameter_1_samplePositions[] = {
	0
};
float device_3_parameter_1_values[] = {
	0.25000000f
};
unsigned int device_3_parameter_2_samplePositions[] = {
	0
};
float device_3_parameter_2_values[] = {
	0.83561641f
};
unsigned int device_3_parameter_3_samplePositions[] = {
	0
};
float device_3_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_3_parameter_4_samplePositions[] = {
	0
};
float device_3_parameter_4_values[] = {
	0.42465752f
};
unsigned int device_3_parameter_5_samplePositions[] = {
	0
};
float device_3_parameter_5_values[] = {
	0.19178082f
};
unsigned int device_3_parameter_6_samplePositions[] = {
	0
};
float device_3_parameter_6_values[] = {
	1.00000000f
};
ParameterTrack device_3_parameters[] = {
	ParameterTrack(1, device_3_parameter_0_samplePositions, device_3_parameter_0_values),
	ParameterTrack(1, device_3_parameter_1_samplePositions, device_3_parameter_1_values),
	ParameterTrack(1, device_3_parameter_2_samplePositions, device_3_parameter_2_values),
	ParameterTrack(1, device_3_parameter_3_samplePositions, device_3_parameter_3_values),
	ParameterTrack(1, device_3_parameter_4_samplePositions, device_3_parameter_4_values),
	ParameterTrack(1, device_3_parameter_5_samplePositions, device_3_parameter_5_values),
	ParameterTrack(1, device_3_parameter_6_samplePositions, device_3_parameter_6_values),
};
freeverbEffect device_3_effect;
EffectDevice device_3(device_3_effect, device_3_parameters);
unsigned int device_4_parameter_0_samplePositions[] = {
	0
};
float device_4_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_4_parameter_1_samplePositions[] = {
	0, 78484, 87490, 167261, 171764, 259897, 351890
};
float device_4_parameter_1_values[] = {
	0.50000000f, 0.50000000f, 0.12121212f, 0.13636363f, 0.84848487f, 0.84848487f, 0.03030303f
};
unsigned int device_4_parameter_2_samplePositions[] = {
	0
};
float device_4_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_4_parameter_3_samplePositions[] = {
	0
};
float device_4_parameter_3_values[] = {
	0.00000000f
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
	0.00000000f
};
unsigned int device_4_parameter_6_samplePositions[] = {
	0
};
float device_4_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_4_parameter_7_samplePositions[] = {
	0
};
float device_4_parameter_7_values[] = {
	0.00000000f
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
	1.00000000f
};
ParameterTrack device_4_parameters[] = {
	ParameterTrack(1, device_4_parameter_0_samplePositions, device_4_parameter_0_values),
	ParameterTrack(7, device_4_parameter_1_samplePositions, device_4_parameter_1_values),
	ParameterTrack(1, device_4_parameter_2_samplePositions, device_4_parameter_2_values),
	ParameterTrack(1, device_4_parameter_3_samplePositions, device_4_parameter_3_values),
	ParameterTrack(1, device_4_parameter_4_samplePositions, device_4_parameter_4_values),
	ParameterTrack(1, device_4_parameter_5_samplePositions, device_4_parameter_5_values),
	ParameterTrack(1, device_4_parameter_6_samplePositions, device_4_parameter_6_values),
	ParameterTrack(1, device_4_parameter_7_samplePositions, device_4_parameter_7_values),
	ParameterTrack(1, device_4_parameter_8_samplePositions, device_4_parameter_8_values),
	ParameterTrack(1, device_4_parameter_9_samplePositions, device_4_parameter_9_values),
};
WidthEffect device_4_effect;
EffectDevice device_4(device_4_effect, device_4_parameters);
Device* deviceList_5[] = {
	&device_0, &device_1, &device_2, &device_3, &device_4
};
ChainDevice device_5(5, deviceList_5);
Device* deviceList_6[] = {
	&device_5
};
GroupDevice device_6(1, deviceList_6);
unsigned int device_7_parameter_0_samplePositions[] = {
	0
};
float device_7_parameter_0_values[] = {
	0.50000000f
};
unsigned int device_7_parameter_1_samplePositions[] = {
	0
};
float device_7_parameter_1_values[] = {
	0.50000000f
};
unsigned int device_7_parameter_2_samplePositions[] = {
	0
};
float device_7_parameter_2_values[] = {
	0.50000000f
};
unsigned int device_7_parameter_3_samplePositions[] = {
	0
};
float device_7_parameter_3_values[] = {
	0.00000000f
};
unsigned int device_7_parameter_4_samplePositions[] = {
	0
};
float device_7_parameter_4_values[] = {
	0.00000000f
};
unsigned int device_7_parameter_5_samplePositions[] = {
	0
};
float device_7_parameter_5_values[] = {
	0.00000000f
};
unsigned int device_7_parameter_6_samplePositions[] = {
	0
};
float device_7_parameter_6_values[] = {
	0.00000000f
};
unsigned int device_7_parameter_7_samplePositions[] = {
	0
};
float device_7_parameter_7_values[] = {
	0.00000000f
};
unsigned int device_7_parameter_8_samplePositions[] = {
	0
};
float device_7_parameter_8_values[] = {
	0.00000000f
};
unsigned int device_7_parameter_9_samplePositions[] = {
	0
};
float device_7_parameter_9_values[] = {
	1.00000000f
};
ParameterTrack device_7_parameters[] = {
	ParameterTrack(1, device_7_parameter_0_samplePositions, device_7_parameter_0_values),
	ParameterTrack(1, device_7_parameter_1_samplePositions, device_7_parameter_1_values),
	ParameterTrack(1, device_7_parameter_2_samplePositions, device_7_parameter_2_values),
	ParameterTrack(1, device_7_parameter_3_samplePositions, device_7_parameter_3_values),
	ParameterTrack(1, device_7_parameter_4_samplePositions, device_7_parameter_4_values),
	ParameterTrack(1, device_7_parameter_5_samplePositions, device_7_parameter_5_values),
	ParameterTrack(1, device_7_parameter_6_samplePositions, device_7_parameter_6_values),
	ParameterTrack(1, device_7_parameter_7_samplePositions, device_7_parameter_7_values),
	ParameterTrack(1, device_7_parameter_8_samplePositions, device_7_parameter_8_values),
	ParameterTrack(1, device_7_parameter_9_samplePositions, device_7_parameter_9_values),
};
WidthEffect device_7_effect;
EffectDevice device_7(device_7_effect, device_7_parameters);
Device* deviceList_8[] = {
	&device_6, &device_7
};
ChainDevice device_8(2, deviceList_8);
MidiTrack* midiTracks[] = {
	&track_0
};
SongInfo songInfo(midiTracks, 120.0, 352800);
Song song(songInfo, device_8);
