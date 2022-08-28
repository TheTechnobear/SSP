cp ./technobear/README.txt ./releases
cp ./build/technobear/*/*/Release/VST3/*.vst3/Contents/armv7l-linux/*.so ./releases/plugins
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins/*
cd releases 
rm ../tb_plugins.zip
zip -r ../tb_plugins.zip .
cd ..
