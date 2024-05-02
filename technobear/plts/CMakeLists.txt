cmake_minimum_required(VERSION 3.15)
project(PLTS VERSION 2.0.0)


include_directories("${PROJECT_SOURCE_DIR}/../../mi")
include_directories("${PROJECT_SOURCE_DIR}/../common")


juce_add_plugin(PLTS
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "Technobear"                   # Specify the name of the plugin's author
        COMPANY_WEBSITE "www.technobear.com"
        COMPANY_EMAIL "mharris@technobear.com"
        DESCRIPTION "macro oscillator"
        # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
        # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD FALSE                # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE TB00               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE PLTS                            # A unique four-character plugin id with at least one upper-case character
        FORMATS VST3                                # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        PRODUCT_NAME "plts")                        # The name of the final executable, which can differ from the target name





if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D TEST")
elseif (UNIX)
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^aarch64") 
        message("disable MI 32bit optmisation  for aarch64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D TEST")
    endif()   
endif ()

set(MI_SRC
        ../../mi/plaits/resources.cc
        ../../mi/plaits/dsp/voice.cc

        ../../mi/plaits/dsp/speech/lpc_speech_synth.cc
        ../../mi/plaits/dsp/speech/lpc_speech_synth_controller.cc
        ../../mi/plaits/dsp/speech/lpc_speech_synth_phonemes.cc
        ../../mi/plaits/dsp/speech/lpc_speech_synth_words.cc
        ../../mi/plaits/dsp/speech/naive_speech_synth.cc
        ../../mi/plaits/dsp/speech/sam_speech_synth.cc

        ../../mi/plaits/dsp/physical_modelling/modal_voice.cc
        ../../mi/plaits/dsp/physical_modelling/resonator.cc
        ../../mi/plaits/dsp/physical_modelling/string.cc
        ../../mi/plaits/dsp/physical_modelling/string_voice.cc


        ../../mi/plaits/dsp/engine/additive_engine.cc
        ../../mi/plaits/dsp/engine/bass_drum_engine.cc
        ../../mi/plaits/dsp/engine/chord_engine.cc
        ../../mi/plaits/dsp/engine/fm_engine.cc
        ../../mi/plaits/dsp/engine/grain_engine.cc
        ../../mi/plaits/dsp/engine/hi_hat_engine.cc
        ../../mi/plaits/dsp/engine/modal_engine.cc
        ../../mi/plaits/dsp/engine/noise_engine.cc
        ../../mi/plaits/dsp/engine/particle_engine.cc
        ../../mi/plaits/dsp/engine/snare_drum_engine.cc
        ../../mi/plaits/dsp/engine/speech_engine.cc
        ../../mi/plaits/dsp/engine/string_engine.cc
        ../../mi/plaits/dsp/engine/swarm_engine.cc
        ../../mi/plaits/dsp/engine/virtual_analog_engine.cc
        ../../mi/plaits/dsp/engine/waveshaping_engine.cc
        ../../mi/plaits/dsp/engine/wavetable_engine.cc


        ../../mi/plaits/dsp/engine2/chiptune_engine.cc
        ../../mi/plaits/dsp/engine2/phase_distortion_engine.cc
        ../../mi/plaits/dsp/engine2/six_op_engine.cc
        ../../mi/plaits/dsp/engine2/string_machine_engine.cc
        ../../mi/plaits/dsp/engine2/virtual_analog_vcf_engine.cc
        ../../mi/plaits/dsp/engine2/wave_terrain_engine.cc

        ../../mi/plaits/dsp/chords/chord_bank.cc
        ../../mi/plaits/dsp/fm/algorithms.cc
        ../../mi/plaits/dsp/fm/dx_units.cc

        ../../mi/stmlib/utils/random.cc
        ../../mi/stmlib/dsp/atan.cc
        ../../mi/stmlib/dsp/units.cc
        )

include(${CMAKE_CURRENT_SOURCE_DIR}/../common/CMakeLists.txt)


target_sources(PLTS
        PUBLIC
        Source/SSPApi.cpp
        PRIVATE
        Source/PluginEditor.cpp
        Source/PluginMiniEditor.cpp
        Source/PluginProcessor.cpp
        ${COMMON_SRC}
        ${MI_SRC}
        )


target_compile_definitions(PLTS
        PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)



target_link_libraries(PLTS PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils)

set_target_properties(${PROJECT_NAME}_VST3 PROPERTIES PREFIX "")

if(INSTALL_PLUGIN)
    juce_enable_copy_plugin_step(${PROJECT_NAME})
endif()

