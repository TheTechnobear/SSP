
if(DEFINED ENV{XC_ROOT})
    set(CMAKE_SYSROOT $ENV{XC_ROOT}/sysroot)
else()
    set(CMAKE_SYSROOT "./sysroot")
endif()


message("using SYSROOT :  ${CMAKE_SYSROOT}")

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(triple arm-linux-gnueabihf)

set(CMAKE_C_COMPILER /opt/homebrew/opt/llvm/bin/clang)
set(CMAKE_CXX_COMPILER /opt/homebrew/opt/llvm/bin/clang++)


set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER_TARGET ${triple})


set(XC_TOOLS /opt/homebrew/bin)
set(XC_PREFIX arm-linux-gnueabihf-)

find_program(CMAKE_AR ${XC_PREFIX}ar PATHS ${XC_TOOLS})
find_program(CMAKE_RANLIB ${XC_PREFIX}ranlib PATHS ${XC_TOOLS})
find_program(CMAKE_LINKER ${XC_PREFIX}ld PATHS ${XC_TOOLS})
find_program(CMAKE_NM ${XC_PREFIX}nm PATHS ${XC_TOOLS})
find_program(CMAKE_OBJCOPY ${XC_PREFIX}objcopy PATHS ${XC_TOOLS})
find_program(CMAKE_OBJDUMP ${XC_PREFIX}objdump PATHS ${XC_TOOLS})
find_program(CMAKE_STRIP ${XC_PREFIX}strip PATHS ${XC_TOOLS})

set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_LIBDIR} "${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${CMAKE_SYSROOT}/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})


set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/lib -B${CMAKE_SYSROOT}/lib")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/lib")

set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf -B${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf ")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf")

set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf  -B${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf")

set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/usr/lib/gcc/arm-linux-gnueabihf  		 -B${CMAKE_SYSROOT}/usr/lib/gcc/arm-linux-gnueabihf")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -L${CMAKE_SYSROOT}/usr/lib/gcc/arm-linux-gnueabihf/6.3.0   -B${CMAKE_SYSROOT}/usr/lib/gcc/arm-linux-gnueabihf/6.3.0 ")
set(SSP_LINK_FLAGS "${SSP_LINK_FLAGS} -Wl,-rpath-link,${CMAKE_SYSROOT}/usr/lib/gcc/arm-linux-gnueabihf/6.3.0")



set(CMAKE_EXE_LINKER_FLAGS 		"${CMAKE_EXE_LINKER_FLAGS} 		${SSP_LINK_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS 	"${CMAKE_MODULE_LINKER_FLAGS} 	${SSP_LINK_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS 	"${CMAKE_SHARED_LINKER_FLAGS} 	${SSP_LINK_FLAGS}")

set(CMAKE_C_FLAGS 	"${CMAKE_C_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4")

