cp ./technobear/README.txt ./releases
cp ~/projects/SSP.private/build.xmx/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/plugins.xmx
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins.xmx/*

cd ~/projects/SSP.private
rm -rf tmp
mkdir -p tmp
cd tmp
cp ../releases/README.txt . 
cp -r ../releases/other .
cp -r ../releases/plugins.ssp plugins
rm ../tb_plugins_xmx.zip
zip -r ../tb_plugins_xmx.zip .
rm -rf tmp
cd ~/projects/SSP.private