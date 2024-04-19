export BUILDROOT=~/xc/ssp/arm-rockchip-linux-gnueabihf_sdk-buildroot
echo "SSP Development  BUILDROOT=$BUILDROOT"
echo 'cd build.ssp'
echo 'cmake -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake ..'
echo 'cmake --build . -- VERBOSE=1'
