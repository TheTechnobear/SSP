# Plts : macro oscillator 

plts - an implementation of Mutable Instrument Plaits for the Percussa SSP.

## Change Log
1.0.0 Initial Release 

## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear


## Installation
copy the `plugins/*.so` to sdcard `/plugins`

## Optional Installation
you may want to copy and rename the demo presets contained in `/presets`
you may now remove `/plugins.preset` folder on SSP sdcard as its no longer used 

## General Usage 

The best source of information about this module is the Mutable Instruments Rings manual
https://mutable-instruments.net/modules/plaits/manual/


This document is based on the LATEST version of the plugin

## Differences


Generally, I've tried to make as close as possible to the hardware modules.



## Tips

I only recommend using at 48Khz sample rate, as this is how the Plaits code is designed.
Using at other sample rates may lead to incorrect pitch, and possible other oddities.
( the only solution would be downsample/upsampling but this is sub-optimal on a platform such as the SSP)


## Credits: 

Percussa SSP - https://www.percussa.com/ 

Mutable Instruments : https://www.mutable.com/ 
Thanks to Emile for sharing her code with the community.
please note: this VST in no way endorsed or supported by Mutable Instruments.
