export BUILDROOT=~/xc/xmx/aarch64-rockchip-linux-gnu_sdk-buildroot
echo "XMX Development  BUILDROOT=$BUILDROOT"
echo 'cd build.xmx'
echo 'cmake -DCMAKE_TOOLCHAIN_FILE=../xcXMX.cmake ..'
echo 'cmake --build . -- VERBOSE=1'

