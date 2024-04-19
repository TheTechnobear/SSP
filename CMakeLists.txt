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
        message("linux config - NO crosscompile")
    endif ()
endif ()


add_subdirectory(juce)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJUCE_DISABLE_JUCE_VERSION_PRINTING=1")

if (DEFINED ENV{ALWAYS_COMPACT_UI})
    set(ALWAYS_COMPACT_UI 1)
else ()
    set(ALWAYS_COMPACT_UI 0)
endif()


#set(ALWAYS_COMPACT_UI 1)


if (DEFINED ENV{TARGET_SSP} OR TARGET_SSP)
    message("target : SSP")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTARGET_SSP=1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCOMPACT_UI_SCALE=2")
elseif (DEFINED ENV{TARGET_XMX} OR TARGET_XMX)
    message("target : XMX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTARGET_XMX=1")
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

