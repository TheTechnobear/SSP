cmake_minimum_required(VERSION 3.15)
project(sspdev)


if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("apple config")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  if (CMAKE_CROSSCOMPILING)
    message("cross compile config")
  else ()
   set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
   set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
  endif()
endif()


add_subdirectory(juce)


if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    juce_set_vst2_sdk_path("$ENV{HOME}/SDKs/Steinberg/VST3/VST3 SDK")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    juce_set_vst2_sdk_path("$ENV{HOME}/SDKs/vst3-sdk")
endif ()



add_subdirectory(technobear)

# set(CMAKE_FOLDER .)
function(_juce_add_pips)
    file(GLOB headers
        CONFIGURE_DEPENDS LIST_DIRECTORIES false
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    endif()

    foreach(header IN ITEMS ${headers})
        juce_add_pip(${header} added_target)
        target_link_libraries(${added_target} PUBLIC
            juce::juce_recommended_config_flags
            juce::juce_recommended_lto_flags
            juce::juce_recommended_warning_flags)
    endforeach()
endfunction()

