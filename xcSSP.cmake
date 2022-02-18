
if(DEFINED ENV{XC_ROOT})
    set(BUILDROOT $ENV{XC_ROOT}/arm-rockchip-linux-gnueabihf_sdk-buildroot)
    set(CMAKE_SYSROOT ${BUILDROOT}/arm-rockchip-linux-gnueabihf/sysroot)
else()
    set(BUILDROOT "./buildroot/arm-rockchip-linux-gnueabihf_sdk-buildroot")
    set(CMAKE_SYSROOT ${BUILDROOT}/arm-rockchip-linux-gnueabihf/sysroot)
endif()

message("using BUILDROOT :  ${BUILDROOT}")
message("using SYSROOT :  ${CMAKE_SYSROOT}")

set(GCCROOT ${BUILDROOT}/lib/gcc/arm-rockchip-linux-gnueabihf/8.4.0)


set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv7l)
set(triple arm-linux-gnueabihf)
set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)

set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})


set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_LIBDIR} "${CMAKE_SYSROOT}/usr/lib/pkgconfig:${CMAKE_SYSROOT}/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})


set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/lib -B${CMAKE_SYSROOT}/lib")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/lib")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${GCCROOT} -B${GCCROOT}")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${GCCROOT}")

set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} 		${SSP_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS 	"${CMAKE_MODULE_LINKER_FLAGS} 	${SSP_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} 	${SSP_LINK_FLAGS}")


set(CMAKE_C_FLAGS 	"${CMAKE_C_FLAGS}  -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DJUCE_CHECK_MEMORY_LEAKS=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BUILDROOT}/arm-rockchip-linux-gnueabihf/include/c++/8.4.0 ")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${BUILDROOT}/arm-rockchip-linux-gnueabihf/include/c++/8.4.0/arm-rockchip-linux-gnueabihf")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")

