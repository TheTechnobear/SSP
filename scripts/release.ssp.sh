cp ./technobear/README.txt ./releases/ssp
mkdir -p ./releases/ssp/plugins
cp ~/projects/SSP.private/build.ssp/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/ssp/plugins
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/ssp/plugins/*
cd ~/projects/SSP.private/releases/ssp
rm ../tb_plugins_ssp.zip
zip -r ../tb_plugins_ssp.zip .
cd ~/projects/SSP.private
