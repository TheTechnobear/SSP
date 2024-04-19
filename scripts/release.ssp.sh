cp ./technobear/README.txt ./releases
cp ~/projects/SSP.private/build.ssp/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/plugins.ssp
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins.ssp/*
cd ~/projects/SSP.private/releases 
rm ../tb_plugins_ssp.zip
zip -r ../tb_plugins_ssp.zip .
cd ~/projects/SSP.private
