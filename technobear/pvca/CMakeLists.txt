
cmake_minimum_required(VERSION 3.15)

project(PVCA VERSION 1.0.0)

include_directories("${PROJECT_SOURCE_DIR}/../common")


juce_add_plugin(PVCA
        COMPANY_NAME "Technobear"                  
        COMPANY_WEBSITE "www.technobear.com"
        COMPANY_EMAIL "mharris@technobear.com"
        DESCRIPTION "panning vca"
        COPY_PLUGIN_AFTER_BUILD FALSE                
        PLUGIN_MANUFACTURER_CODE TB00               
        PLUGIN_CODE PVCA                            
        FORMATS VST3                                
        PRODUCT_NAME "pvca")                        

include(${CMAKE_CURRENT_SOURCE_DIR}/../common/CMakeLists.txt)


target_sources(PVCA
        PUBLIC
        Source/SSPApi.cpp
        PRIVATE
        Source/PluginEditor.cpp
        Source/PluginMiniEditor.cpp
        Source/PluginProcessor.cpp
        ${COMMON_SRC}
        )

target_compile_definitions(PVCA
        PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0  
        JUCE_USE_CURL=0         
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(PVCA PRIVATE
        juce::juce_audio_utils)

set_target_properties(${PROJECT_NAME}_VST3 PROPERTIES PREFIX "")

if(APPLE)
    juce_enable_copy_plugin_step(${PROJECT_NAME})
endif()
