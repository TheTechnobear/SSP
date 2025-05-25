cp ./technobear/README.txt ./releases
cp ~/projects/SSP.private/build.ssp/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/plugins.ssp
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins.ssp/*

cd ~/projects/SSP.private
rm -rf tmp
mkdir -p tmp
cd tmp
cp ../releases/README.txt . 
cp -r ../releases/other .
cp -r ../releases/plugins.ssp plugins
rm ../tb_plugins_ssp.zip
zip -r ../tb_plugins_ssp.zip .
rm -rf tmp
cd ~/projects/SSP.private
