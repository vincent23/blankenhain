#include "BhSoundtrack.h"
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

///////////////////////////
////// O P T I O N S //////
///////////////////////////

// Put the desired filename here
const char* nameOfOutputFile = "blankenhainWaveWriter.wav";

// Set to true to write RAW floating point audio instead of a wav file
const bool writeLegacyRAWAudio = false;

////////////////////////////
////////////////////////////
////////////////////////////



// Setting up floating point coprocessor, probably in a naughty way
// background: http://stackoverflow.com/questions/12517429/error-lnk2001-unresolved-external-symbol-fltused-in-wdk
extern "C" {
	int _fltused = 1;
}

// Taking care of VC++ compiler pure virtual function calling convention stuff.
// background: https://msdn.microsoft.com/en-us/en-en/library/ff798096.aspx
extern "C" int _cdecl _purecall(void) {
	return 0;
}

int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
	//////////////////////////////////////
	// Set up FPU according to entire 64k demoscene standards appareantly
	//////////////////////////////////////
	const unsigned short foo = 0u;
	_asm FSTCW foo
	const unsigned short fcw = 3711;
	__asm fldcw fcw;
	//////////////////////////////////////

	const unsigned int numberOfSamples = blankenhain::lengthInSamples();
	float* audioBuffer = new float[numberOfSamples * 2];
	blankenhain::render(audioBuffer);

	if (!writeLegacyRAWAudio)
	{
		// Write Riff-Wave
		// via http://soundfile.sapp.org/doc/WaveFormat/
		HANDLE outputFile;
		do {
			outputFile = CreateFile(nameOfOutputFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		} while (outputFile == INVALID_HANDLE_VALUE);


		WriteFile(outputFile, "RIFF", 4, NULL, NULL);
		const unsigned char c_subchunksize2 = blankenhain::lengthInSamples() * 2 * 2;
		const unsigned char c_chunksize = 4 + (8 + 16) + (8 + c_subchunksize2);
		WriteFile(outputFile, &c_chunksize, 4, NULL, NULL);
		WriteFile(outputFile, "WAVE", 4, NULL, NULL);
		WriteFile(outputFile, "fmt ", 4, NULL, NULL);

		const unsigned int c_subchunk1size = 16;
		WriteFile(outputFile, &c_subchunk1size, 4, NULL, NULL);
		const short s_audioformat = 1;
		WriteFile(outputFile, &s_audioformat, 2, NULL, NULL);
		const short s_numchannels = 2;
		WriteFile(outputFile, &s_numchannels, 2, NULL, NULL);
		const unsigned int c_samplerate = 44100;
		WriteFile(outputFile, &c_samplerate, 4, NULL, NULL);
		const unsigned int c_byterate = 44100 * 2 * 16 / 8;
		WriteFile(outputFile, &c_byterate, 4, NULL, NULL);

		const short s_blockalign = 4;
		WriteFile(outputFile, &s_blockalign, 2, NULL, NULL);
		const short s_bitspersample = 16;
		WriteFile(outputFile, &s_bitspersample, 2, NULL, NULL);
		WriteFile(outputFile, "data", 4, NULL, NULL);
		WriteFile(outputFile, &c_subchunksize2, 4, NULL, NULL);

		__int16* audioBufferAs16BitInt = new __int16[blankenhain::lengthInSamples() * 2];
		for (unsigned int i = 0u; i < blankenhain::lengthInSamples() * 2; i++)
		{
			float& fval = (audioBuffer[i]);
			__int16 value = static_cast<__int16>((fval + 1.f) / 2.f * 65535.f) - 32768;
			audioBufferAs16BitInt[i] = value;
		}
		WriteFile(outputFile, audioBufferAs16BitInt, sizeof(__int16) * blankenhain::lengthInSamples() * 2, NULL, NULL);
		CloseHandle(outputFile);
		if (audioBufferAs16BitInt != nullptr)
		{
			delete[] audioBufferAs16BitInt;
			audioBufferAs16BitInt = nullptr;
		}
	}
	else
	{
		// Legacy Feature
		// Write audio as raw floating point values
		HANDLE outputFile;
		do {
			outputFile = CreateFile(nameOfOutputFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		} while (outputFile == INVALID_HANDLE_VALUE);
		WriteFile(outputFile, audioBuffer, sizeof(float) * numberOfSamples * 2, NULL, NULL);
		CloseHandle(outputFile);
	}

	delete audioBuffer;
	return 0;
}

void WinMainCRTStartup()
{
	int Result = WinMain(GetModuleHandle(0), 0, 0, 0);
	ExitProcess(Result);
}


/////////////////////////////////////////////////////////
////// C U S T O M   M E M O R Y   H A N D L I N G //////
/////////////////////////////////////////////////////////
#ifdef _DEBUG
#pragma warning ( push )
#pragma warning(disable: 4595)
#endif

#ifdef _DEBUG
__forceinline
#endif
void* __cdecl operator new(unsigned int bytes) {
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

#ifdef _DEBUG
__forceinline
#endif
void* __cdecl operator new [](unsigned int bytes) {
	return operator new(bytes);
}

#ifdef _DEBUG
__forceinline
#endif
void* __cdecl operator new(unsigned int bytes, void* here) {
	return here;
}

#ifdef _DEBUG
__forceinline
#endif
void __cdecl operator delete(void* ptr) {
	if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

#ifdef _DEBUG
__forceinline
#endif
void __cdecl operator delete(void* ptr, unsigned int) {
	if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

#ifdef _DEBUG
__forceinline
#endif
void __cdecl operator delete [](void* ptr) {
	operator delete(ptr);
}

#ifdef _DEBUG
#pragma warning (pop)
#endif

extern "C"
{
#pragma function(memset)
	void *memset(void *dest, int c, size_t count)
	{
		char *bytes = (char *)dest;
		while (count--)
		{
			*bytes++ = (char)c;
		}
		return dest;
	}
}

void memset2(void* dst, unsigned char val, unsigned int count) {
	__asm {
		mov     eax, dword ptr[val]
		mov     ecx, dword ptr[count]
		mov     edi, dword ptr[dst]
		rep     stosb
	}
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////