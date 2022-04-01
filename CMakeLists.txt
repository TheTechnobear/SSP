cmake_minimum_required(VERSION 3.15)
project(sspdev)


if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message("apple config")
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if (CMAKE_CROSSCOMPILING)
        message("cross compile config")
    else ()
        set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
        set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
    endif ()
endif ()


add_subdirectory(juce)

if (DEFINED ENV{VSTSDK})
    set(VSTSDK $ENV{VSTSDK})
else ()
    set(VSTSDK "$ENV{HOME}/SDKs/vst3-sdk")
endif ()

message("using VSTSDK  :  ${VSTSDK}")

juce_set_vst2_sdk_path(${VSTSDK})


add_subdirectory(daisySP))
add_subdirectory(technobear)

# set(CMAKE_FOLDER .)
function(_juce_add_pips)
    file(GLOB headers
            CONFIGURE_DEPENDS LIST_DIRECTORIES false
            "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

    if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    endif ()

    foreach (header IN ITEMS ${headers})
        juce_add_pip(${header} added_target)
        target_link_libraries(${added_target} PUBLIC
                juce::juce_recommended_config_flags
                juce::juce_recommended_lto_flags
                juce::juce_recommended_warning_flags)
    endforeach ()
endfunction()

