
if(APPLE)
    set(CMAKE_SYSTEM_NAME Darwin)
endif()


if (DEFINED ENV{BUILDROOT})
    set(BUILDROOT $ENV{BUILDROOT})
    set(CMAKE_SYSROOT ${BUILDROOT}/aarch64-rockchip-linux-gnu/sysroot)
else ()
    message("warning: BUILDROOT environment variable assuming in $ENV{HOME}/buildroot")
    set(BUILDROOT "$ENV{HOME}/buildroot/aarch64-rockchip-linux-gnu_sdk-buildroot")
    set(CMAKE_SYSROOT ${BUILDROOT}/aarch64-rockchip-linux-gnu/sysroot)
endif ()

if (DEFINED ENV{TOOLSROOT})
    set(TOOLSROOT $ENV{TOOLSROOT})
else ()
    # sensible defaults
    if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
        # assume we are using homebrew
        if (${CMAKE_HOST_SYSTEM_PROCESSOR} MATCHES "arm64")        
            set(TOOLSROOT /opt/homebrew/opt/llvm/bin)
        else ()
            set(TOOLSROOT /usr/local/opt/llvm/bin)
        endif()
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(TOOLSROOT "/usr/bin")
    endif ()
endif ()

message("using TOOLSROOT  :  ${TOOLSROOT}")
message("using BUILDROOT  :  ${BUILDROOT}")
message("using SYSROOT    :  ${CMAKE_SYSROOT}")

set(GCCROOT ${BUILDROOT}/lib/gcc/aarch64-rockchip-linux-gnu/8.4.0)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(triple aarch64-rockchip-linux-gnu)
set(CMAKE_C_COMPILER ${TOOLSROOT}/clang)
set(CMAKE_CXX_COMPILER ${TOOLSROOT}/clang++)
add_link_options("-fuse-ld=lld")


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

set(CPU_FLAGS "-mcpu=cortex-a35 -O3")
#set(CPU_FLAGS "-march=armv8-a+simd -mtune=cortex-a35 -O3")
set(CMAKE_C_FLAGS "${CMAKE_CFLAGS}  ${CPU_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPU_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -funwind-tables -fno-omit-frame-pointer  ")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJUCE_CHECK_MEMORY_LEAKS=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BUILDROOT}/aarch64-rockchip-linux-gnu/include/c++/8.4.0 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BUILDROOT}/aarch64-rockchip-linux-gnu/include/c++/8.4.0/aarch64-rockchip-linux-gnu")

set(TARGET_XMX 1)
