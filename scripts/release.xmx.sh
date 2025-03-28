cp ./technobear/README.txt ./releases/xmx
mkdir -p ./releases/xmx/plugins
cp ~/projects/SSP.private/build.xmx/technobear/*/*/Release/VST3/*.vst3/Contents/*/*.so ./releases/xmx/plugins
/opt/homebrew/bin/aarch64-elf-strip --strip-unneeded ./releases/xmx/plugins/*
cd ~/projects/SSP.private/releases/xmx 
# for trax host only
mv plugins/trax.so .
rm ../tb_plugins_xmx.zip
zip -r ../tb_plugins_xmx.zip .
cd ~/projects/SSP.private
