#include "BlankenhainPlayer.h"

#include "MidiTrack.h"
#include "ParameterTrack.h"
#include "InstrumentDevice.h"
#include "ChainDevice.h"
#include "GroupDevice.h"
#include "SongInfo.h"
#include "Song.h"
#include "ReturnDevice.h"
#include "SendDevice.h"

#include "polyblepInstrument.h"
#include "WidthEffect.h"
#include "gmsynthInstrument.h"
#include "DistortionEffect.h"
#include "DelayEffect.h"
#include "freeverbEffect.h"
#include "VolumeEffect.h"
#include "EqualizerEffect.h"
#include "basedrum1Instrument.h"
#include "CompressorEffect.h"
#include "FilterEffect.h"
#include "ChorusEffect.h"
#include "FmInstrument.h"
#include "RhythmgateEffect.h"
#include "glidePolyblepInstrument.h"

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
	int _fltused = 1;
}

extern "C" int _cdecl _purecall(void) {
	return 0;
}

int CALLBACK
WinMain(HINSTANCE Instance,
	HINSTANCE PrevInstance,
	LPSTR CommandLine,
	int ShowCode)
{
#include "Input.inl"
	BlankenhainPlayer player;
	player.play(song);
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
int __cdecl atexit(ePVFV func);


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

int __cdecl atexit(ePVFV func) {
	// get next free entry in atexist list
	unsigned int index = 0;
	while (g_atExitList[index++]);

	if (index < MAX_ATEXITS) {
		g_atExitList[index] = func;
		return 0;
	}

	return -1;
}

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
