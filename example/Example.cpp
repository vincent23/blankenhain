#include "BhSoundtrack.h"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>

extern "C" {
	int _fltused = 1;
}

extern "C" int _cdecl _purecall(void) {
	return 0;
}

#include <stdio.h>

int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
	unsigned int numberOfSamples = blankenhain::lengthInSamples();
	float* audioBuffer = new float[numberOfSamples * 2];
	bool threaded = false;
	if (threaded) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)blankenhain::render, audioBuffer, 0, 0);
		// wait one second
		//Sleep(100000);
	}
	else {
		blankenhain::render(audioBuffer);
	}

	//const unsigned int sampleRate = 44100;
	//const unsigned int blockAlignment = sizeof(float) * 2;
	//const unsigned int totalBytes = numberOfSamples * blockAlignment;
	//const unsigned int bytesPerSecond = sampleRate * blockAlignment;
	//const unsigned int bitsPerSample = sizeof(float) * 8;

	//HWAVEOUT audio_wave_out;
	//WAVEHDR audio_wave_header;
	//audio_wave_header = {
	//	(LPSTR)audioBuffer,
	//	totalBytes,
	//	0,
	//	0,
	//	0,
	//	0,
	//	0,
	//	0
	//};
	//WAVEFORMATEX wave_format = {
	//	WAVE_FORMAT_IEEE_FLOAT,
	//	/* channels        */ 2,
	//	/* samples/second  */ sampleRate,
	//	/* bytes/second    */ bytesPerSecond,
	//	/* block alignment */ blockAlignment,
	//	/* bits/sample     */ bitsPerSample,
	//	/* no extensions   */ 0
	//};

	//waveOutOpen(&audio_wave_out, WAVE_MAPPER, &wave_format, NULL, 0, CALLBACK_NULL);
	//waveOutPrepareHeader(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));

	//// play

	//waveOutWrite(audio_wave_out, &audio_wave_header, sizeof(audio_wave_header));

	//Sleep((numberOfSamples / 44100 + 3) * 1000);


	FILE* const audio_file = fopen("audio.out", "wb");
	if (audio_file != NULL) {
	puts("writing file...");
	int bytes = fwrite(audioBuffer, sizeof(float), numberOfSamples * 2, audio_file);
	fclose(audio_file);
	printf("bytes written: %i\n", bytes);
	}


	delete audioBuffer;

	return 0;
}

void WinMainCRTStartup()
{
	int Result = WinMain(GetModuleHandle(0), 0, 0, 0);
	ExitProcess(Result);
}

void* __cdecl operator new(unsigned int bytes) {
	return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

void* __cdecl operator new [](unsigned int bytes) {
	return operator new(bytes);
}

void* __cdecl operator new(unsigned int bytes, void* here) {
	return here;
}

void __cdecl operator delete(void* ptr) {
	if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

void __cdecl operator delete(void* ptr, unsigned int) {
	if (ptr) HeapFree(GetProcessHeap(), 0, ptr);
}

void __cdecl operator delete [](void* ptr) {
	operator delete(ptr);
}
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


typedef void(__cdecl *ePVFV)();

void memset2(void* dst, unsigned char val, unsigned int count);

#define MAX_ATEXITS 32
static void initTerm(ePVFV *pfbegin, ePVFV *pfend);
static void initAtExit();
static void doAtExit();
//int __cdecl atexit(ePVFV func);


#pragma data_seg(".CRT$XCA")
ePVFV __xc_a[] = { nullptr };
#pragma data_seg(".CRT$XCZ")
ePVFV __xc_z[] = { nullptr };
#pragma data_seg() // reset data segment


void memset2(void* dst, unsigned char val, unsigned int count) {
	__asm {
		mov     eax, dword ptr[val]
		mov     ecx, dword ptr[count]
		mov     edi, dword ptr[dst]
		rep     stosb
	}
}


static void initTerm(ePVFV *pfbegin, ePVFV *pfend) {
	while (pfbegin < pfend) {
		if (*pfbegin) {
			(**pfbegin)();
		}

		pfbegin++;
	}
}

static ePVFV g_atExitList[MAX_ATEXITS];

static void initAtExit() {
	memset2(g_atExitList, 0, sizeof(g_atExitList));
}

static void doAtExit() {
	initTerm(g_atExitList, g_atExitList + MAX_ATEXITS);
}

/*
int __cdecl atexit(ePVFV func) {
	// get next free entry in atexist list
	unsigned int index = 0;
	while (g_atExitList[index++]);

	if (index < MAX_ATEXITS) {
		g_atExitList[index] = func;
		return 0;
	}

	return -1;
}*/

void GlobalsStaticsInit() {
#	ifdef NDEBUG
	initAtExit();
	initTerm(__xc_a, __xc_z);
#	endif
}

void GlobalsStaticsFree() {
#	ifdef NDEBUG
	doAtExit();
#	endif
}
