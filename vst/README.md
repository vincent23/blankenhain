blankenhain VST
===============

Build Instructions
------------------

### Install JUCE

First you have to download the JUCE source code.
Just do a `git clone --depth 1 git://github.com/julianstorer/JUCE.git` in any folder you want.
On Windows you can use the Github Git Shell for that.

Then you have to tell the compiler where to find JUCE.
This is done by creating a link called `juce_modules` in this directory to the `JUCE/modules` directory.

#### Windows

	mklink /D juce_modules path\to\JUCE\modules

#### Linux

	ln -s path/to/JUCE/modules juce_modules

### VST SDK

You also have to get the [VST SDK from Steinberg](https://www.steinberg.net/en/company/developer.html).
Download the zip files and copy it's contents on Windows to `c:\SDKs\VST3 SDK` and on Linux to `~/SDKs/VST3 SDK` (I didn't find a way to make this a relative path, yet).

### Building

There is not yet an automated way for building all plugins at once, so you have to build them separately.

#### Windows

You'll need Visual Studio 2015, there's not really a way around it.
Go to `Build\ExamplePlugin\VisualStudio2015` and open the `.sln` file (the solution).
You should be able to build it and find the plugin as a `.dll` file somewhere in this directory (in `Release` or `Debug`, depending on your build config).

#### Linux

Go to `Build/ExamplePlugin/LinuxMakefile` and run `make` with your preferred settings, e.g. I do `CXX=clang++ make -j 4`.
It should create the plugin as a `.so` file in the `Release` or `Debug` subdirectory.

Creating New Plugins
--------------------
