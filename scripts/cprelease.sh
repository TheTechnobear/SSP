cp ./build/technobear/*/*/Release/VST/*.so ./releases/plugins
cp ./technobear/*/*.txt ./releases/docs
rm ./releases/docs/CMakeLists.txt
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins/*
