# building


##sub modules
get submodules using

```
git submodule update --init --recursive
```

## general 
I use cmake, so the basic build, assuming you have dependancies etc , is

```
mkdir build
cd build
cmake ..
cmake --build .
```


## build under linux

as described in this post

https://forum.percussa.com/t/how-to-build-modules-vst-plugins-for-the-ssp/348

note: I have moved away from this, so there may be the odd issue ;) 

## cross compile on macOS

I use my cross compilation project called xc. there are variations for a variety of platforms, including the SSP , with xcSSP.

this can be found at
https://github.com/thetechnobear/xcSSP

install as per instructions in this repo

we then need to use homebrew go get clang, cmake etc.
(more details later!)

then its similar to a normal cmake build

```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake ..
cmake --build .
 ```

this builds and places plugins in ~/.vst and ~/.vst3


note: im using a mac m1, running macOS 11. but cross-compilation should be possible on any mac
similar, it may be you can get a cross-compilation running under windows, since im using standard cmake/clang cross compilation.
however... I do not support it, sorry 


## testing
I do a good amount of the development on macOS, so you can build the plugins under macOS using cmake.
I use Jetbrain's CLion which can import the cmake project and 

depending on platform you may wish to specify the architecture 
e.g.
```
CMAKE_OSX_ARCHITECTURES=arm64; cmake ..
```
