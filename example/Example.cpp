#include "BlankenhainPlayer.h"

#include "MidiTrack.h"
#include "ParameterTrack.h"
#include "InstrumentDevice.h"
#include "ChainDevice.h"
#include "GroupDevice.h"
#include "SongInfo.h"
#include "Song.h"

#include "polyblepInstrument.h"
#include "WidthEffect.h"

int main(int argc, char* argv[]) {
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
		0, 209250, 258809
	};
	float device_0_parameter_0_values[] = {
		0.5606060624, 0.3939394057, 0.2121212184
	};
	unsigned int device_0_parameter_1_samplePositions[] = {
		0
	};
	float device_0_parameter_1_values[] = {
		0.4262187183
	};
	unsigned int device_0_parameter_2_samplePositions[] = {
		0
	};
	float device_0_parameter_2_values[] = {
		1.0
	};
	unsigned int device_0_parameter_3_samplePositions[] = {
		0
	};
	float device_0_parameter_3_values[] = {
		0.4262187183
	};
	unsigned int device_0_parameter_4_samplePositions[] = {
		0
	};
	float device_0_parameter_4_values[] = {
		0.0
	};
	unsigned int device_0_parameter_5_samplePositions[] = {
		0
	};
	float device_0_parameter_5_values[] = {
		0.4262187183
	};
	unsigned int device_0_parameter_6_samplePositions[] = {
		0
	};
	float device_0_parameter_6_values[] = {
		1.0
	};
	unsigned int device_0_parameter_7_samplePositions[] = {
		0, 66079, 203743, 291848
	};
	float device_0_parameter_7_values[] = {
		0.2121212184, 0.2121212184, 0.4090909064, 0.7878788114
	};
	unsigned int device_0_parameter_8_samplePositions[] = {
		0
	};
	float device_0_parameter_8_values[] = {
		0.3333333433
	};
	unsigned int device_0_parameter_9_samplePositions[] = {
		0
	};
	float device_0_parameter_9_values[] = {
		1.0
	};
	ParameterTrack device_0_parameters[] = {
		ParameterTrack(4, device_0_parameter_0_samplePositions, device_0_parameter_0_values),
		ParameterTrack(1, device_0_parameter_1_samplePositions, device_0_parameter_1_values),
		ParameterTrack(1, device_0_parameter_2_samplePositions, device_0_parameter_2_values),
		ParameterTrack(1, device_0_parameter_3_samplePositions, device_0_parameter_3_values),
		ParameterTrack(1, device_0_parameter_4_samplePositions, device_0_parameter_4_values),
		ParameterTrack(1, device_0_parameter_5_samplePositions, device_0_parameter_5_values),
		ParameterTrack(1, device_0_parameter_6_samplePositions, device_0_parameter_6_values),
		ParameterTrack(4, device_0_parameter_7_samplePositions, device_0_parameter_7_values),
		ParameterTrack(1, device_0_parameter_8_samplePositions, device_0_parameter_8_values),
		ParameterTrack(1, device_0_parameter_9_samplePositions, device_0_parameter_9_values),
	};
	polyblepInstrument device_0_instrument;
	InstrumentDevice device_0(device_0_instrument, device_0_parameters, 0);
	unsigned int device_1_parameter_0_samplePositions[] = {
		0
	};
	float device_1_parameter_0_values[] = {
		0.5
	};
	unsigned int device_1_parameter_1_samplePositions[] = {
		0
	};
	float device_1_parameter_1_values[] = {
		0.5
	};
	unsigned int device_1_parameter_2_samplePositions[] = {
		0
	};
	float device_1_parameter_2_values[] = {
		0.5
	};
	unsigned int device_1_parameter_3_samplePositions[] = {
		0
	};
	float device_1_parameter_3_values[] = {
		0
	};
	unsigned int device_1_parameter_4_samplePositions[] = {
		0
	};
	float device_1_parameter_4_values[] = {
		0
	};
	unsigned int device_1_parameter_5_samplePositions[] = {
		0
	};
	float device_1_parameter_5_values[] = {
		0
	};
	unsigned int device_1_parameter_6_samplePositions[] = {
		0
	};
	float device_1_parameter_6_values[] = {
		0
	};
	unsigned int device_1_parameter_7_samplePositions[] = {
		0
	};
	float device_1_parameter_7_values[] = {
		0
	};
	unsigned int device_1_parameter_8_samplePositions[] = {
		0
	};
	float device_1_parameter_8_values[] = {
		0
	};
	unsigned int device_1_parameter_9_samplePositions[] = {
		0
	};
	float device_1_parameter_9_values[] = {
		1
	};
	ParameterTrack device_1_parameters[] = {
		ParameterTrack(1, device_1_parameter_0_samplePositions, device_1_parameter_0_values),
		ParameterTrack(1, device_1_parameter_1_samplePositions, device_1_parameter_1_values),
		ParameterTrack(1, device_1_parameter_2_samplePositions, device_1_parameter_2_values),
		ParameterTrack(1, device_1_parameter_3_samplePositions, device_1_parameter_3_values),
		ParameterTrack(1, device_1_parameter_4_samplePositions, device_1_parameter_4_values),
		ParameterTrack(1, device_1_parameter_5_samplePositions, device_1_parameter_5_values),
		ParameterTrack(1, device_1_parameter_6_samplePositions, device_1_parameter_6_values),
		ParameterTrack(1, device_1_parameter_7_samplePositions, device_1_parameter_7_values),
		ParameterTrack(1, device_1_parameter_8_samplePositions, device_1_parameter_8_values),
		ParameterTrack(1, device_1_parameter_9_samplePositions, device_1_parameter_9_values),
	};
	WidthEffect device_1_effect;
	EffectDevice device_1(device_1_effect, device_1_parameters);
	Device* deviceList_2[] = {
		&device_0, &device_1
	};
	ChainDevice device_2(2, deviceList_2);
	Device* deviceList_3[] = {
		&device_2
	};
	GroupDevice device_3(1, deviceList_3);
	unsigned int device_4_parameter_0_samplePositions[] = {
		0
	};
	float device_4_parameter_0_values[] = {
		0.5
	};
	unsigned int device_4_parameter_1_samplePositions[] = {
		0
	};
	float device_4_parameter_1_values[] = {
		0.5
	};
	unsigned int device_4_parameter_2_samplePositions[] = {
		0
	};
	float device_4_parameter_2_values[] = {
		0.5
	};
	unsigned int device_4_parameter_3_samplePositions[] = {
		0
	};
	float device_4_parameter_3_values[] = {
		0
	};
	unsigned int device_4_parameter_4_samplePositions[] = {
		0
	};
	float device_4_parameter_4_values[] = {
		0
	};
	unsigned int device_4_parameter_5_samplePositions[] = {
		0
	};
	float device_4_parameter_5_values[] = {
		0
	};
	unsigned int device_4_parameter_6_samplePositions[] = {
		0
	};
	float device_4_parameter_6_values[] = {
		0
	};
	unsigned int device_4_parameter_7_samplePositions[] = {
		0
	};
	float device_4_parameter_7_values[] = {
		0
	};
	unsigned int device_4_parameter_8_samplePositions[] = {
		0
	};
	float device_4_parameter_8_values[] = {
		0
	};
	unsigned int device_4_parameter_9_samplePositions[] = {
		0
	};
	float device_4_parameter_9_values[] = {
		1
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
	};
	WidthEffect device_4_effect;
	EffectDevice device_4(device_4_effect, device_4_parameters);
	Device* deviceList_5[] = {
		&device_3, &device_4
	};
	ChainDevice device_5(2, deviceList_5);
	MidiTrack* midiTracks[] = {
		&track_0
	};
	SongInfo songInfo(midiTracks, 120.0, 352800);
	Song song(songInfo, device_5);




	BlankenhainPlayer player;
	player.play(song);


	return 0;
}