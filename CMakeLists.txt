cmake_minimum_required(VERSION 3.15)
project(sspdev)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

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

#if (DEFINED ENV{VSTSDK})
#    set(VSTSDK $ENV{VSTSDK})
#else ()
#    set(VSTSDK "$ENV{HOME}/SDKs/vst3-sdk")
#endif ()

#message("using VSTSDK  :  ${VSTSDK}")

#juce_set_vst2_sdk_path(${VSTSDK})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1")

if (DEFINED ENV{ALWAYS_COMPACT_UI})
    set(ALWAYS_COMPACT_UI 1)
else ()
    set(ALWAYS_COMPACT_UI 0)
endif()


#set(ALWAYS_COMPACT_UI 1)


if (DEFINED ENV{TARGET_SSP})
    message("target : SSP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTARGET_SSP=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMPACT_UI_SCALE=2")
elseif (DEFINED ENV{TARGET_QSP})
    message("target : QSP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTARGET_QSP=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMPACT_UI_SCALE=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFORCE_COMPACT_UI=1")
else()
    message("default target : SSP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTARGET_SSP=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMPACT_UI_SCALE=2")
endif()

if (ALWAYS_COMPACT_UI)
    message("force compact ui : enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFORCE_COMPACT_UI=1")
endif()

add_subdirectory(daisySP)
add_subdirectory(technobear)
#add_subdirectory(apps)

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

