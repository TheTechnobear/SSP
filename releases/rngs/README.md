# Rngs : Resonator 

rngs - an implementation of Mutable Instrument Rings for the Percussa SSP.

## Change Log
1.2.0 requires new ssp firmware for vst sdk 2 !
auto detection of audio/strum/voct 
mono capatibility
ssp preset now stores/loads vst parameters
pitch is now bipolar from 0.0f 
clicking encoder resets values

note: you will loose presets, as these are now stored in the SSP preset

1.1.0 internal release only - code cleanup

1.0.1 fix model persistence, add input gain, code re-org 
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
https://mutable-instruments.net/modules/rings/manual/


This document is based on the LATEST version of the plugin

## Differences


Generally, I've tried to make as close as possible to the hardware modules.

Pitch & Frequency are synonymous in this context. Rings actually uses pitches internally not frequency ;) 
Excitation choice - see below
Models, I use a number to represent the models, 0,1,2 are the 'normal' modes, 3,4,5 are alternative modes.

added In Gain, this allows the input signal to be amplified, 0 = no gain (1x), up to 1.0  = 17x


## Excitation source
as with the Mutable Rings module the VST will detect which input you are using to 'excite' rings.

tips/typical use for which input to use:
STRUM = for non-pitched excitation, or pitched where you just want the pitch encoder to be used.
STRUM + V/OCT = or pitched excitation
AUDIO + V/OCT = use external excitation, and pitch the resonator

## Outputs 
Outputs are ODD and EVEN, not stereo pairs, and their 'usage' changes depending on 'poly' (as detailed in the manual).
However, there is alot of fun to be had panning these in varying degrees.

note: like MI Module - if you only patch the ODD output, then ODD and EVEN are summed evenly.


## Tips

I highly recommend exciting Rngs with an external audio source (NOI/WTO/LFO), you can get all sorts of interesting sounds, that are not possible with internal exciter

I only recommend using at 48Khz sample rate, as this is how the Rings code is designed.
Using at other sample rates will lead to incorrect pitch, and possible other oddities.
( the only solution would be downsample/upsampling but this is sub-optimal on a platform such as the SSP)


## Credits: 

Percussa SSP - https://www.percussa.com/ 

Mutable Instruments : https://www.mutable.com/ 
Thanks to Emilie for sharing her code with the community.
please note: this VST in no way endorsed or supported by Mutable Instruments.
