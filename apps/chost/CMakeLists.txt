# Example Console App CMakeLists.txt

# To get started on a new console app, copy this entire folder (containing this file and C++
# sources) to a convenient location, and then start making modifications. For other examples of
# CMakeLists for console apps, check `extras/BinaryBuilder` and `extras/UnitTestRunner` in the JUCE
# repo.

# The first line of any CMake project should be a call to `cmake_minimum_required`, which checks
# that the installed CMake will be able to understand the following CMakeLists, and ensures that
# CMake's behaviour is compatible with the named version. This is a standard CMake command, so more
# information can be found in the CMake docs.cmake_minimum_required(VERSION 3.15)
project(ConsoleHost VERSION 0.0.1)

# If you've installed JUCE somehow (via a package manager, or directly using the CMake install
# target), you'll need to tell this project that it depends on the installed copy of JUCE. If you've
# included JUCE directly in your source tree (perhaps as a submodule), you'll need to tell CMake to
# include that subdirectory as part of the build.

# find_package(JUCE CONFIG REQUIRED)        # If you've installed JUCE to your system
# or
# add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE

# `juce_add_console_app` adds an executable target with the name passed as the first argument
# (ConsoleHost here). This target is a normal CMake target, but has a lot of extra properties
# set up by default. This function accepts many optional arguments. Check the readme at
# `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_console_app(ConsoleHost
        PRODUCT_NAME "chost")     # The name of the final executable, which can differ from the target name

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into the build tree. This header should be included with `#include <JuceHeader.h>`. The include
# path for this header will be automatically added to the target. The main function of the
# JuceHeader is to include all the JUCE module headers for a particular target; if you're happy to
# include module headers directly, you probably don't need to call this.

 juce_generate_juce_header(ConsoleHost)


# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

target_sources(ConsoleHost
        PRIVATE
        Main.cpp
        PluginHost.cpp
        )



target_compile_definitions(ConsoleHost
        PRIVATE
        JUCE_USE_FREETYPE=0
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_console_app` call
        JUCE_USE_CURL=0)    # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_console_app` call

# If the target needs extra binary assets, they can be added here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when the target is built.

# juce_add_binary_data(ConsoleAppData SOURCES ...)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_core` module. Inter-module dependencies are
# resolved automatically. If you'd generated a binary data target above, you would need to link to
# it here too. This is a standard CMake command.

target_link_libraries(ConsoleHost
        PRIVATE
        # ConsoleAppData            # If you'd created a binary data target, you'd link to it here
        juce::juce_core
        juce::juce_audio_basics
        juce_audio_devices
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
