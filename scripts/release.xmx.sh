cd ~/projects/SSP.private
cp ./technobear/README.txt ./releases
mkdir -p ./releases/plugins/xmx
cp ~/projects/SSP.private/build.xmx/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/xmx/plugins
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/xmx/plugins/*

rm -rf tmp
mkdir -p tmp
cd tmp

cp ../releases/README.txt . 
cp -r ../releases/other .
cp -r ../releases/xmx/plugins plugins

rm ../tb_plugins_xmx.zip
zip -r ../tb_plugins_xmx.zip .
cd ..
rm -rf tmp
