cmake_minimum_required(VERSION 3.15)
project(MMX4 VERSION 2.0.0)


include_directories("${PROJECT_SOURCE_DIR}/../common")


juce_add_plugin(MMX4
        # VERSION ...                               # Set this if the plugin version is different to the project version
        # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL ...
        COMPANY_NAME "Technobear"                   # Specify the name of the plugin's author
        COMPANY_WEBSITE "www.technobear.com"
        COMPANY_EMAIL "mharris@technobear.com"
        DESCRIPTION "matrix mixer"
        # IS_SYNTH TRUE/FALSE                       # Is this a synth or an effect?
        # NEEDS_MIDI_INPUT TRUE/FALSE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD FALSE                # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE TB00               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE MMX4                            # A unique four-character plugin id with at least one upper-case character
        FORMATS VST3                                # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
        VST3_AUTO_MANIFEST FALSE
        PRODUCT_NAME "mmx4")                        # The name of the final executable, which can differ from the target name

include(${CMAKE_CURRENT_SOURCE_DIR}/../common/CMakeLists.txt)

target_sources(MMX4
        PUBLIC
        Source/SSPApi.cpp
        PRIVATE
        Source/PluginEditor.cpp
        Source/PluginMiniEditor.cpp
        Source/PluginProcessor.cpp
        ${COMMON_SRC}
        )


target_compile_definitions(MMX4
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0)



target_link_libraries(MMX4 PRIVATE
        # AudioPluginData           # If we'd created a binary data target, we'd link to it here
        juce::juce_audio_utils)





set_target_properties(${PROJECT_NAME}_VST3 PROPERTIES PREFIX "")

if(INSTALL_PLUGIN)
    juce_enable_copy_plugin_step(${PROJECT_NAME})
endif()

