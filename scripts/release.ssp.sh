cd ~/projects/SSP.private
cp ./technobear/README.txt ./releases
mkdir -p ./releases/plugins/ssp
cp ~/projects/SSP.private/build.ssp/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/plugins/ssp
/opt/homebrew/bin/arm-linux-gnueabihf-strip --strip-unneeded ./releases/plugins/ssp/*

rm -rf tmp
mkdir -p tmp
cd tmp

cp ../releases/README.txt . 
cp -r ../releases/other .
cp -r ../releases/ssp/plugins plugins

cp ../resources/ssp/*  .
unzip SYNTHOR.zip
rm SYNTHOR.zip
rm ../tb_plugins_ssp.zip
zip -r ../tb_plugins_ssp.zip .

cd ..
rm -rf tmp
