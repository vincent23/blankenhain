# Automatically generated makefile, created by the Introjucer
# Don't edit this file! Your changes will be overwritten when you re-save the Introjucer project!

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef CONFIG
  CONFIG=Debug
endif

ifeq ($(CONFIG),Debug)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Debug
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "PLUGIN_FILTER" -D JucePlugin_Name=\"Blankenhain\ Filter\" -D JucePlugin_Desc=\"Filter\ plugin.\" -D "JucePlugin_PluginCode='BhFI'" -D "JUCER_LINUX_MAKE_A3D28711=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../../JuceLibraryCode -I ../../../juce_modules -I ../../../../../blankenhain
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0 -I../../../Source
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -Wl,--no-undefined -shared -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := Blankenhain.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := -march=native
  endif

  CPPFLAGS := $(DEPFLAGS) -D "LINUX=1" -D "NDEBUG=1" -D "PLUGIN_FILTER" -D JucePlugin_Name=\"Blankenhain\ Filter\" -D JucePlugin_Desc=\"Filter\ plugin.\" -D "JucePlugin_PluginCode='BhFI'" -D "JUCER_LINUX_MAKE_A3D28711=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/VST3\ SDK -I ../../../JuceLibraryCode -I ../../../juce_modules -I ../../../../../blankenhain
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 -I../../../Source
  CXXFLAGS += $(CFLAGS) -std=c++11
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -Wl,--no-undefined -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 

  TARGET := Blankenhain.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/WaveVerschwurbelungComponent_cf2fc23b.o \
  $(OBJDIR)/WaveGenerationComponent_9dda340d.o \
  $(OBJDIR)/AdsrComponent_813055dc.o \
  $(OBJDIR)/LfoComponent_8e5e1c31.o \
  $(OBJDIR)/ExampleAudioProcessor_ee4a53e2.o \
  $(OBJDIR)/ExampleAudioProcessorEditor_2e557d2f.o \
  $(OBJDIR)/FilterAudioProcessor_1254b8cc.o \
  $(OBJDIR)/FilterAudioProcessorEditor_ec770a99.o \
  $(OBJDIR)/WittersrodaAudioProcessor_3fce612a.o \
  $(OBJDIR)/WittersrodaAudioProcessorEditor_de9c7477.o \
  $(OBJDIR)/BlankenhainAudioProcessor_79973627.o \
  $(OBJDIR)/CreateFilter_f99fea0e.o \
  $(OBJDIR)/FloatParameter_1174567.o \
  $(OBJDIR)/juce_audio_basics_2a5f169b.o \
  $(OBJDIR)/juce_audio_devices_949f3099.o \
  $(OBJDIR)/juce_audio_formats_c3207a39.o \
  $(OBJDIR)/juce_audio_processors_b4d2553.o \
  $(OBJDIR)/juce_core_e866357d.o \
  $(OBJDIR)/juce_data_structures_63793d79.o \
  $(OBJDIR)/juce_events_54b79b71.o \
  $(OBJDIR)/juce_graphics_7f4f5a55.o \
  $(OBJDIR)/juce_gui_basics_36438f51.o \
  $(OBJDIR)/juce_gui_extra_c336119.o \
  $(OBJDIR)/juce_PluginUtilities_e98121a5.o \
  $(OBJDIR)/juce_VST_Wrapper_86d571ae.o \
  $(OBJDIR)/juce_VST3_Wrapper_30d9f76c.o \
  $(OBJDIR)/synth_bf08319e.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(RESOURCES)
	@echo Linking Blankenhain
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning Blankenhain
	@$(CLEANCMD)

strip:
	@echo Stripping Blankenhain
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/WaveVerschwurbelungComponent_cf2fc23b.o: ../../../Source/components/WaveVerschwurbelungComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling WaveVerschwurbelungComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WaveGenerationComponent_9dda340d.o: ../../../Source/components/WaveGenerationComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling WaveGenerationComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/AdsrComponent_813055dc.o: ../../../Source/components/AdsrComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling AdsrComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/LfoComponent_8e5e1c31.o: ../../../Source/components/LfoComponent.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling LfoComponent.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ExampleAudioProcessor_ee4a53e2.o: ../../../Source/plugins/example/ExampleAudioProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ExampleAudioProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/ExampleAudioProcessorEditor_2e557d2f.o: ../../../Source/plugins/example/ExampleAudioProcessorEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling ExampleAudioProcessorEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterAudioProcessor_1254b8cc.o: ../../../Source/plugins/filter/FilterAudioProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterAudioProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FilterAudioProcessorEditor_ec770a99.o: ../../../Source/plugins/filter/FilterAudioProcessorEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FilterAudioProcessorEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WittersrodaAudioProcessor_3fce612a.o: ../../../Source/plugins/wittersroda/WittersrodaAudioProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling WittersrodaAudioProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/WittersrodaAudioProcessorEditor_de9c7477.o: ../../../Source/plugins/wittersroda/WittersrodaAudioProcessorEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling WittersrodaAudioProcessorEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/BlankenhainAudioProcessor_79973627.o: ../../../Source/BlankenhainAudioProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling BlankenhainAudioProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/CreateFilter_f99fea0e.o: ../../../Source/CreateFilter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling CreateFilter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/FloatParameter_1174567.o: ../../../Source/FloatParameter.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling FloatParameter.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_2a5f169b.o: ../../../juce_modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_949f3099.o: ../../../juce_modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_c3207a39.o: ../../../juce_modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_b4d2553.o: ../../../juce_modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_e866357d.o: ../../../juce_modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_63793d79.o: ../../../juce_modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_54b79b71.o: ../../../juce_modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_7f4f5a55.o: ../../../juce_modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_36438f51.o: ../../../juce_modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_c336119.o: ../../../juce_modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_e98121a5.o: ../../../juce_modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_86d571ae.o: ../../../juce_modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_30d9f76c.o: ../../../juce_modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/synth_bf08319e.o: ../../../../synth/synth.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling synth.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
