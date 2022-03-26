if(APPLE)
    set(CMAKE_SYSTEM_NAME Darwin)
endif()


if (DEFINED ENV{BUILDROOT})
    set(BUILDROOT $ENV{BUILDROOT})
    set(CMAKE_SYSROOT ${BUILDROOT}/arm-rockchip-linux-gnueabihf/sysroot)
else ()
    message("warning: BUILDROOT environment variable assuming in current directory")
    set(BUILDROOT "./arm-rockchip-linux-gnueabihf_sdk-buildroot")
    set(CMAKE_SYSROOT ${BUILDROOT}/arm-rockchip-linux-gnueabihf/sysroot)
endif ()

if (DEFINED ENV{TOOLSROOT})
    set(TOOLSROOT $ENV{TOOLSROOT})
else ()
    # sensible defaults
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        # assume we are using homebrew
        set(TOOLSROOT /opt/homebrew/opt/llvm/bin)
        # non m1 macs need this instead, but you could also set TOOLS_ROOT
        # set(TOOLSROOT /usr/local/opt/llvm/bin)

    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(TOOLSROOT "/usr/bin")
    endif ()
endif ()

message("using TOOLSROOT  :  ${TOOLSROOT}")
message("using BUILDROOT  :  ${BUILDROOT}")
message("using SYSROOT    :  ${CMAKE_SYSROOT}")

set(GCCROOT ${BUILDROOT}/lib/gcc/arm-rockchip-linux-gnueabihf/8.4.0)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv7l)
set(triple arm-linux-gnueabihf)
set(CMAKE_C_COMPILER ${TOOLSROOT}/clang)
set(CMAKE_CXX_COMPILER ${TOOLSROOT}/clang++)

set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})


set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_LIBDIR} "${CMAKE_SYSROOT}/usr/lib/pkgconfig:${CMAKE_SYSROOT}/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})


set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/lib -B${CMAKE_SYSROOT}/lib")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/lib")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${GCCROOT} -B${GCCROOT}")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${GCCROOT}")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} 		${SSP_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} 	${SSP_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} 	${SSP_LINK_FLAGS}")


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJUCE_CHECK_MEMORY_LEAKS=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BUILDROOT}/arm-rockchip-linux-gnueabihf/include/c++/8.4.0 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BUILDROOT}/arm-rockchip-linux-gnueabihf/include/c++/8.4.0/arm-rockchip-linux-gnueabihf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")

