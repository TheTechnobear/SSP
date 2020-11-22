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
  OUTDIR := build/Debug

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vst3-sdk -I ../../JuceLibraryCode -I ../../../../juce/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -g -ggdb -fPIC -O0 -I../../../../mi -I ../../../common
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "DEBUG=1" -D "_DEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vst3-sdk -I ../../JuceLibraryCode -I ../../../../juce/modules
  TARGET := plts.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

ifeq ($(CONFIG),Release)
  BINDIR := build
  LIBDIR := build
  OBJDIR := build/intermediate/Release
  OUTDIR := build/Release

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  CPPFLAGS := $(DEPFLAGS) -std=c++11 -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vst3-sdk -I ../../JuceLibraryCode -I ../../../../juce/modules
  CFLAGS += $(CPPFLAGS) $(TARGET_ARCH) -fPIC -O3 -I../../../../mi -I ../../../common
  CXXFLAGS += $(CFLAGS)
  LDFLAGS += $(TARGET_ARCH) -L$(BINDIR) -L$(LIBDIR) -shared -fvisibility=hidden -L/usr/X11R6/lib/ -lX11 -lXext -lXinerama -lasound -ldl -lfreetype -lpthread -lrt 
  LDDEPS :=
  RESFLAGS :=  -D "LINUX=1" -D "NDEBUG=1" -D "JUCER_LINUX_MAKE_7346DA2A=1" -D "JUCE_APP_VERSION=1.0.0" -D "JUCE_APP_VERSION_HEX=0x10000" -I /usr/include -I /usr/include/freetype2 -I ~/SDKs/vst3-sdk -I ../../JuceLibraryCode -I ../../../../juce/modules
  TARGET := plts.so
  BLDCMD = $(CXX) -o $(OUTDIR)/$(TARGET) $(OBJECTS) $(LDFLAGS) $(RESOURCES) $(TARGET_ARCH)
  CLEANCMD = rm -rf $(OUTDIR)/$(TARGET) $(OBJDIR)
endif

OBJECTS := \
  $(OBJDIR)/resources_77dc8502.o \
  $(OBJDIR)/voice_15f82f63.o \
  $(OBJDIR)/lpc_speech_synth_f47c9e89.o \
  $(OBJDIR)/lpc_speech_synth_controller_cbc5a198.o \
  $(OBJDIR)/lpc_speech_synth_phonemes_bf36e767.o \
  $(OBJDIR)/lpc_speech_synth_words_d0fe005f.o \
  $(OBJDIR)/naive_speech_synth_ad938aef.o \
  $(OBJDIR)/sam_speech_synth_363bb709.o \
  $(OBJDIR)/modal_voice_2b8ee0df.o \
  $(OBJDIR)/resonator_fda549ca.o \
  $(OBJDIR)/string_867bfbcc.o \
  $(OBJDIR)/string_voice_ac33e039.o \
  $(OBJDIR)/additive_engine_d69feddf.o \
  $(OBJDIR)/bass_drum_engine_997e994d.o \
  $(OBJDIR)/chord_engine_4dfb8a23.o \
  $(OBJDIR)/fm_engine_558ebd9a.o \
  $(OBJDIR)/grain_engine_898e382.o \
  $(OBJDIR)/hi_hat_engine_592760b0.o \
  $(OBJDIR)/modal_engine_e2568734.o \
  $(OBJDIR)/noise_engine_5cfdcf61.o \
  $(OBJDIR)/particle_engine_30c0019.o \
  $(OBJDIR)/snare_drum_engine_809ffe9f.o \
  $(OBJDIR)/speech_engine_13c060f5.o \
  $(OBJDIR)/string_engine_6d3ac964.o \
  $(OBJDIR)/swarm_engine_5890ec3f.o \
  $(OBJDIR)/virtual_analog_engine_a71dfc37.o \
  $(OBJDIR)/waveshaping_engine_6237660c.o \
  $(OBJDIR)/wavetable_engine_f94da1fc.o \
  $(OBJDIR)/random_3aa6eea.o \
  $(OBJDIR)/atan_dc30eefd.o \
  $(OBJDIR)/units_9838f790.o \
  $(OBJDIR)/SSP_dd798310.o \
  $(OBJDIR)/SSPButton_f6a7ad22.o \
  $(OBJDIR)/SSPParam_83ec7d27.o \
  $(OBJDIR)/PluginProcessor_a059e380.o \
  $(OBJDIR)/PluginEditor_94d4fb09.o \
  $(OBJDIR)/juce_audio_basics_50fb0bc.o \
  $(OBJDIR)/juce_audio_devices_8591c67a.o \
  $(OBJDIR)/juce_audio_formats_b413101a.o \
  $(OBJDIR)/juce_audio_processors_7d960e74.o \
  $(OBJDIR)/juce_core_eaa9319e.o \
  $(OBJDIR)/juce_data_structures_cb7bfada.o \
  $(OBJDIR)/juce_events_46ed9f12.o \
  $(OBJDIR)/juce_graphics_99a8a576.o \
  $(OBJDIR)/juce_gui_basics_9ed061f2.o \
  $(OBJDIR)/juce_gui_extra_f56667fa.o \
  $(OBJDIR)/juce_PluginUtilities_79f5d586.o \
  $(OBJDIR)/juce_VST_Wrapper_8a88968f.o \
  $(OBJDIR)/juce_VST3_Wrapper_1457680d.o \

.PHONY: clean

$(OUTDIR)/$(TARGET): $(OBJECTS) $(LDDEPS) $(RESOURCES)
	@echo Linking plts
	-@mkdir -p $(BINDIR)
	-@mkdir -p $(LIBDIR)
	-@mkdir -p $(OUTDIR)
	@$(BLDCMD)

clean:
	@echo Cleaning plts
	@$(CLEANCMD)

strip:
	@echo Stripping plts
	-@strip --strip-unneeded $(OUTDIR)/$(TARGET)

$(OBJDIR)/resources_77dc8502.o: ../../../../mi/plaits/resources.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resources.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/voice_15f82f63.o: ../../../../mi/plaits/dsp/voice.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling voice.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lpc_speech_synth_f47c9e89.o: ../../../../mi/plaits/dsp/speech/lpc_speech_synth.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lpc_speech_synth.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lpc_speech_synth_controller_cbc5a198.o: ../../../../mi/plaits/dsp/speech/lpc_speech_synth_controller.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lpc_speech_synth_controller.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lpc_speech_synth_phonemes_bf36e767.o: ../../../../mi/plaits/dsp/speech/lpc_speech_synth_phonemes.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lpc_speech_synth_phonemes.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/lpc_speech_synth_words_d0fe005f.o: ../../../../mi/plaits/dsp/speech/lpc_speech_synth_words.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling lpc_speech_synth_words.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/naive_speech_synth_ad938aef.o: ../../../../mi/plaits/dsp/speech/naive_speech_synth.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling naive_speech_synth.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/sam_speech_synth_363bb709.o: ../../../../mi/plaits/dsp/speech/sam_speech_synth.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling sam_speech_synth.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modal_voice_2b8ee0df.o: ../../../../mi/plaits/dsp/physical_modelling/modal_voice.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modal_voice.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/resonator_fda549ca.o: ../../../../mi/plaits/dsp/physical_modelling/resonator.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling resonator.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/string_867bfbcc.o: ../../../../mi/plaits/dsp/physical_modelling/string.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling string.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/string_voice_ac33e039.o: ../../../../mi/plaits/dsp/physical_modelling/string_voice.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling string_voice.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/additive_engine_d69feddf.o: ../../../../mi/plaits/dsp/engine/additive_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling additive_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/bass_drum_engine_997e994d.o: ../../../../mi/plaits/dsp/engine/bass_drum_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling bass_drum_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/chord_engine_4dfb8a23.o: ../../../../mi/plaits/dsp/engine/chord_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling chord_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/fm_engine_558ebd9a.o: ../../../../mi/plaits/dsp/engine/fm_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling fm_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/grain_engine_898e382.o: ../../../../mi/plaits/dsp/engine/grain_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling grain_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/hi_hat_engine_592760b0.o: ../../../../mi/plaits/dsp/engine/hi_hat_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling hi_hat_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/modal_engine_e2568734.o: ../../../../mi/plaits/dsp/engine/modal_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling modal_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/noise_engine_5cfdcf61.o: ../../../../mi/plaits/dsp/engine/noise_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling noise_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/particle_engine_30c0019.o: ../../../../mi/plaits/dsp/engine/particle_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling particle_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/snare_drum_engine_809ffe9f.o: ../../../../mi/plaits/dsp/engine/snare_drum_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling snare_drum_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/speech_engine_13c060f5.o: ../../../../mi/plaits/dsp/engine/speech_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling speech_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/string_engine_6d3ac964.o: ../../../../mi/plaits/dsp/engine/string_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling string_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/swarm_engine_5890ec3f.o: ../../../../mi/plaits/dsp/engine/swarm_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling swarm_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/virtual_analog_engine_a71dfc37.o: ../../../../mi/plaits/dsp/engine/virtual_analog_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling virtual_analog_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/waveshaping_engine_6237660c.o: ../../../../mi/plaits/dsp/engine/waveshaping_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling waveshaping_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/wavetable_engine_f94da1fc.o: ../../../../mi/plaits/dsp/engine/wavetable_engine.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling wavetable_engine.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/random_3aa6eea.o: ../../../../mi/stmlib/utils/random.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling random.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/atan_dc30eefd.o: ../../../../mi/stmlib/dsp/atan.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling atan.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/units_9838f790.o: ../../../../mi/stmlib/dsp/units.cc
	-@mkdir -p $(OBJDIR)
	@echo "Compiling units.cc"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SSP_dd798310.o: ../../../common/SSP.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SSP.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SSPButton_f6a7ad22.o: ../../../common/SSPButton.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SSPButton.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/SSPParam_83ec7d27.o: ../../../common/SSPParam.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling SSPParam.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginProcessor_a059e380.o: ../../Source/PluginProcessor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginProcessor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/PluginEditor_94d4fb09.o: ../../Source/PluginEditor.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling PluginEditor.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_basics_50fb0bc.o: ../../../../juce/modules/juce_audio_basics/juce_audio_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_devices_8591c67a.o: ../../../../juce/modules/juce_audio_devices/juce_audio_devices.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_devices.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_formats_b413101a.o: ../../../../juce/modules/juce_audio_formats/juce_audio_formats.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_formats.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_audio_processors_7d960e74.o: ../../../../juce/modules/juce_audio_processors/juce_audio_processors.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_audio_processors.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_core_eaa9319e.o: ../../../../juce/modules/juce_core/juce_core.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_core.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_data_structures_cb7bfada.o: ../../../../juce/modules/juce_data_structures/juce_data_structures.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_data_structures.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_events_46ed9f12.o: ../../../../juce/modules/juce_events/juce_events.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_events.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_graphics_99a8a576.o: ../../../../juce/modules/juce_graphics/juce_graphics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_graphics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_basics_9ed061f2.o: ../../../../juce/modules/juce_gui_basics/juce_gui_basics.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_basics.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_gui_extra_f56667fa.o: ../../../../juce/modules/juce_gui_extra/juce_gui_extra.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_gui_extra.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_PluginUtilities_79f5d586.o: ../../../../juce/modules/juce_audio_plugin_client/utility/juce_PluginUtilities.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_PluginUtilities.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST_Wrapper_8a88968f.o: ../../../../juce/modules/juce_audio_plugin_client/VST/juce_VST_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

$(OBJDIR)/juce_VST3_Wrapper_1457680d.o: ../../../../juce/modules/juce_audio_plugin_client/VST3/juce_VST3_Wrapper.cpp
	-@mkdir -p $(OBJDIR)
	@echo "Compiling juce_VST3_Wrapper.cpp"
	@$(CXX) $(CXXFLAGS) -o "$@" -c "$<"

-include $(OBJECTS:%.o=%.d)
