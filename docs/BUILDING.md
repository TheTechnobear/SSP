# building

## disclaimer 

I do this in my free time, I do not have the spare time to provide developer support, so I assume if you want to build - you will know how to figure out things like dependancies. 

unfortunately, I setup my build environment a bit adhoc, so I do not currently have instructions for how to do this. 

I can assure you everything that is needed to build IS in this repo, but cannot support building it at this time.

I recognise this may be a bit crap/frustrating, but with my limited time available - id prefer to spend this on developing plugins for ALL ssp users.



## sub modules
get submodules using

```
git submodule update --init --recursive
```

## general build 
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

then its similar to a normal cmake build

```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake ..
cmake --build .
 ```

this builds and places plugins in ~/.vst and ~/.vst3


note: 
im using a mac m1, running macOS 11. but cross-compilation should be possible on any mac
sorry, I did not take note of exactly what I installed so you will have to figure this out yourself. 

similar, it may be you can get a cross-compilation running under windows, since im using standard cmake/clang cross compilation. (however, Ive never tried, nor wish too ;))  



## testing
I do a good amount of the development on macOS, so you can build the plugins under macOS using cmake. I use Jetbrain's CLion which can import the cmake project and 

depending on platform you may wish to specify the architecture 
e.g.
```
CMAKE_OSX_ARCHITECTURES=arm64; cmake ..
```
