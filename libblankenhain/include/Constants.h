#pragma once

/**
 * Constants for the blankenhain2 environment
 * 
 * blockSize = bh2 processes incomming buffers in chunks. This integer values defines the maximum
 * chunk size that will be processed.

 **/
 namespace constants {
	constexpr unsigned int blockSize = 16;

	constexpr unsigned int parameterInterpolationLength = 16;

	constexpr float pi = 3.14159265359f;

	constexpr float sampleRate = 44100;

	constexpr unsigned int maxMidiEventsPerVSTEventBlock = 256;

	constexpr unsigned int maxMidiNotesInFlight = 256;
}

 using uint32_t = unsigned int;
 using int16_t = short;