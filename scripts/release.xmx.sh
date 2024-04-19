cp ./technobear/README.txt ./releases
cp ~/projects/SSP.private/build.xmx/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/plugins.xmx
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins.xmx/*
cd ~/projects/SSP.private/releases 
rm ../tb_plugins_xmx.zip
zip -r ../tb_plugins_xmx.zip .
cd ~/projects/SSP.private
