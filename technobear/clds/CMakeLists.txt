

# set( CMAKE_VERBOSE_MAKEFILE on )cmake_minimum_required(VERSION 3.15)
project(CLDS VERSION 2.0.0)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
# add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_aax_sdk_path(...)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (AudioPluginExample here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.

include_directories("${PROJECT_SOURCE_DIR}/../../mi")
include_directories("${PROJECT_SOURCE_DIR}/../common")


juce_add_plugin(CLDS
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "Technobear"                   # Specify the name of the plugin's author
        COMPANY_WEBSITE "www.technobear.com"
        COMPANY_EMAIL "mharris@technobear.com"
        DESCRIPTION "texture synthesizer"
        # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
        # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD FALSE                # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE TB00               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE CLDS                            # A unique four-character plugin id with at least one upper-case character
        FORMATS VST3                                # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        VST3_AUTO_MANIFEST FALSE
        PRODUCT_NAME "clds")                        # The name of the final executable, which can differ from the target name



if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D TEST")
elseif (UNIX)
    if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "^aarch64") 
        message("disable MI 32bit optmisation  for aarch64")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D TEST")
    endif()   
endif ()

set(MI_SRC
        ../../mi/clouds/dsp/pvoc/frame_transformation.cc
        ../../mi/clouds/dsp/pvoc/phase_vocoder.cc
        ../../mi/clouds/dsp/pvoc/stft.cc
        ../../mi/clouds/dsp/correlator.cc
        ../../mi/clouds/dsp/granular_processor.cc
        ../../mi/clouds/dsp/mu_law.cc
        ../../mi/clouds/resources.cc
        ../../mi/stmlib/utils/random.cc
        ../../mi/stmlib/dsp/atan.cc
        ../../mi/stmlib/dsp/units.cc
        )

include(${CMAKE_CURRENT_SOURCE_DIR}/../common/CMakeLists.txt)

target_sources(CLDS
        PUBLIC
        Source/SSPApi.cpp
        PRIVATE
        Source/PluginEditor.cpp
        Source/PluginProcessor.cpp
        Source/PluginMiniEditor.cpp
        ${MI_SRC}
        ${COMMON_SRC}
        )


target_compile_definitions(CLDS
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)




# target_compile_options(CLDS PRIVATE -fsanitize=address)

target_link_libraries(CLDS PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils)

set_target_properties(${PROJECT_NAME}_VST3 PROPERTIES PREFIX "")

# target_link_options(CLDS_VST3 PRIVATE -fsanitize=address )

if(INSTALL_PLUGIN)
    juce_enable_copy_plugin_step(${PROJECT_NAME})
endif()

