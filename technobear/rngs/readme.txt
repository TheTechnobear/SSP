# Rngs : Resonator 
Version 1.0

Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear

## Installation
copy the plugins/rngs.so to /media/linaro/SYNTHOR/plugins

## Optional Installation
copy the presets to /media/linaro/SYNTHOR/plugins
(same folder structure)


## General Usage 

The best source of information about this module is the Mutable Instruments Rings manual
https://mutable-instruments.net/modules/rings/manual/


## Differences

Pitch & Frequency are synonymous in this context. Rings actually uses pitches internally not frequency ;) 
Excitation choice - see below
Models, I use a number to represent the models, 0,1,2 are the 'normal' modes, 3,4,5 are alternative modes.

Generally, I've tried to make as close as possible 


## Excitation source
The Rings Eurorack module automatically detects jacks being connected into IN, V/OCT and STRUM.
Unfortunately, this is not currently possible with SSP VSTs.

This functonality is replicated with the 'buttons'.

AUDIO: if active, use AUDIO IN - VST IN 1 as the excitation source.
STRUM: if active, use STRUM  - VST IN 2, to excite the internal exciter
V/OCT: if active, use V/OCT  - VST IN 3, changed notes will excite the internal exciter

tips/typical use:
AUDIO + V/OCT = use external excitation, and pitch the resonator
STRUM = for non-pitched excitation, or pitched where you just want the pitch encoder to be used.
STRUM + V/OCT = or pitched excitation


## VST Input/Outputs
Currently, the SSP does not use the VST's name input/output.
The help screen ( ? button ) shows the mapping

## Tips
Note the outputs are ODD and EVEN, not stereo pairs, and their 'usage' changes depending on 'poly' (as detailed in the manaul).
on the SSP we can simply sum these to one input on another module, and then use attenuation or a vca to mix.
However, there is alot of fun to be had panning these in varying degrees.

I only recommend using at 48Khz sample rate, as this is how the Rings code is designed.
Using at other sample rates will lead to incorrect pitch, and possible other oddities.
( the only solution would be downsample/upsampling but this is sub-optimal on a platform such as the SSP)


## Credits: 
Mutable Instruments
Thanks to Emile for sharing her code with the community.
please note: this VST in no way endorsed or supported by Mutable Instruments.