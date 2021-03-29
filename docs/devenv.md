# mac install


note: im doing this on a mac mini m1, it should be similar/same for intel, but this is untested at the moment

## disclaimer

it is **NOT** my intention here to teach someone how to setup a development environment, 
or build my tools, rather just to give some pointers on how I have got myself up n' running.

if you want to build this repo, you should look at each of these steps and **understand** what it is doing, 
so that if it does not work, you can research why on the internet.

apologies, but I do not have time for developer support, the free time I have Id prefer to use for my own development.


## xcode
I use xcode command tools and so install this as well, Im not sure if this is a requirement or not! 

```
xcode-select --install
```

(actually, I think homebrew will install xcode command tools, so likely this step is not required)


## install homebrew 
go to https://brew.sh/ for more details

its simple though...

cut n' paste this into terminal

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

## script
I then have a script (brew-arm.sh), which I use to add homebrew to path, and also launch a bash shell (big sur now defalts to zsh)

```
#!/bin/bash
export PATH=/opt/homebrew/bin:"${PATH}"
bash
```


note: intel macs default to  /usr/local


run this script before any development or usng homebrew commands (e.g. brew install)

## packages 

we now need to install packages needed to build/cross compile

```
brew install cmake coreutils llvm cairo liblo libusb arm-linux-gnueabihf-binutils
```

thats it we are build ready for building

### other setup
you need to make it such that ssh/scp do not need to pass a password.
search the internet for this :)


## each session


### setup environment

prime brew tools (see above)
```
~/bin/brew-arm.sh
```

prime xc environent (see xcSSP notes)

```
. ~/xc/xcSSP/xcSetEnv
```


### initial build

setup build directoy
```
mkdir build;cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake ..
```


### subsequent builds
build using cmake
```
cmake --build .. -- -j 4
```


## misc notes

this is just some misc into that I will delete once I know build is ok :) 


brew list , on my primary build machine shows...

```
arm-linux-gnueabihf-binutils	llvm
cairo				lz4
cmake				lzo
coreutils			mpdecimal
fontconfig			openssl@1.1
freetype			pcre
gdbm				pixman
gettext				pkg-config
glib				popt
libffi				python@3.9
liblo				readline
libpng				rsync
libpthread-stubs	
libusb				sphinx-doc
libx11				sqlite
libxau				tcl-tk
libxcb				xorgproto
libxdmcp			xxhash
libxext				xz
libxrender			zstd
```



 

todo: missing after this
```
lz4
popt
rsync
sphinx-doc
tcl-tk
xxhash
zstd
```






