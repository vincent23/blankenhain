# blankenhain2
## Not the synth the demoscene deserves, but the synth the demoscene needs

### Abstract (tl;dr)
blankenhain is a audio-synthesis framework written mainly in C++. The plattform was originally developed to be used as a synth in 64kb demoscene productions. blankenhain may also be used for quick-and-rough prototyping in audio-effect and audio-instrument development. Tracks composed using blankenhain code can be exported as executables. A number of synths (classic waveforms, FM, Windows MIDI Soundlibrary) and effects (Chorus, Distortion, Delay, ...) are already present; new modules can easily be added.

### Why the world needs blankenhain
blankenhain was concieved by members of the now dormant rtificial demogroup as they were unsatisfied with the available demoscene synths v2 and 4klang. The framework conceptually heavily builds on ideas presented by ferris in his talk at XXX, who turned these thoughts into the as-of-yet unreleased wavesabre application which probably is fairly similar to blankenhain2. Most blankenhain2 coders are perfectionist developers that are actually bad at writing code and thus, development took longer than anticipated. Over the years, the framework grew from a hacky JUCE-based demoscene application towards an object-oriented synthdemon fueled by multiple layers of VST-abstraction. As the name indicates, blankenhain2 is already the second version of the synth and if we were to code blankenhain3 there are many things we would go about differently (see below). Nevertheless, we hope that the public release of blankenhain2 will be of use to some sceners and fuel further development. Have fun!

### Key features of blankenhain
* Effect code (audio buffer filling / modification) is written once and used identically as VST2-synths and STL-free executables.
* VST-Effect GUI is provided by imgui. Wrapper functions for rendering parameters are contained. Writing a GUI seldomly takes longer than 5-10 minutes.
* Performance- and size-critical bottom-layer functionality is provided using vectorized SSE code and optimized asm-functions. New moduels can be coded without worrying about any of this (or the VST2-standard for that matter)
* Many effects/instruments are already provided
** Basic waveforms built using polyBLEPs. Wavetable and Additive-Synthesis options are contained in the code-base.
** FM-Synthesis
** MIDI-sounds from Windows ("gm.dll") are available
** Many effects: Chorus, Distortion, Delay, Reverb, Bitcrush, Rythmgate, Noisegate, Compressor, Limiter, Flanger, ...

### Workflow for creating demoscene music using blankenhain
The assumed workflow for blankenhain2 is to create music using solely blankenhain VST-Plugins in your favourite audio DAW software. In a second step, the save-file of the DAW software is parsed (capturing notes played and plugin parameter automatization). The parser creates a C++-sytle file containing these information, which is then linked by the blankenhain2 wavewriter. Thereby, one obtaines an executable containing the music. After shrinking these files are usually rather small <10-20kB and can be used for demoscene productions. Currently, only a Python-based parser for Ableton 8.2 save-files (".als") is available. However, writing your own parser for your favorite software should not be too hard. In any case, although Ableton 8.2 is fairly old by now it is still a great DAW and we suggest using it with blankenhain2. We are being told by other people we hardly know that Ableton Live 8.2 used to be available on filesharing sites on the world-wide-web, but this information is of no relevance and probably a false statement so nevermind. 

### Flowchart of the blankenhain2 code framework

### Dependencies

### Getting started: Executable Music File from Ableton Save File

### Getting started: Coding a audio-effec in blankenhain

### How to get a Visual Studio Project for blankenhain2
```
git submodule init
git submodule update

mkdir build
cd build
cmake -D PLUGIN_PATH='C:\Where\To\Put\Them\Plugins' ..
```

Open `build/blankenhain2.sln`. Build VST plugins as usual. For standalone wavewriter/soundtrack (and other demoscene-realted applications) use _noSTL build targets.

### Aknowledgement and thanks