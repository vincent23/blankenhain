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

Plugins are managed as separate build targets, as this is the easiest way for sharing code and not having to maintain many JUCE projects.
Create a new directory for your plugin in `Source/plugins` and add an `AudioProcessorEditor` and an `AudioProcessor` subclass (just look at the ExamplePlugin in `Source/plugins/example`).

Now, open up the Introjucer and create two new targets, on as 'Linux Makefile' and the other as 'Visual Studio 2015'.
Just copy the options from the ExamplePlugin (be sure not to miss the preprocessor definitions and compiler flags).
Don't change the Release or Debug targets, just the common flags!

You have to adjust the options for your plugin in the preprocessor definitions, so change the name and the description as you like.
The escaping rules are a bit weird and different in the Visual Studio and the Makefile target, so just look at the example.
We'll use the `PLUGIN_EXAMPLE` for choosing which plugin to build, so you have to rename this to anything you like for your plugin as an identifier.
Another important setting is the plugin code, which has to be unique for every plugin.
Blankenhain uses `Bh` as a prefix.
Please [register your chosen plugin id](http://service.steinberg.de/databases/plugin.nsf/plugIn?openForm) and set `JucePlugin_PluginCode` to this value.
There are additional settings like channel configurations and some more VST options you might need to change.
You can look up those in the `JuceLibraryCode/AppConfig.h` file (but don't change them there!).

The last thing to do is add your plugin to the list in `Source/CreateFilter.cpp`.
Just include the header for your `AudioProcessor` subclass and add a new `ifdef` rule for your plugin, based on the define flag you chose before.
