# Rngs : Resonator 

## Change Log
1.1.0 requires new ssp firmware ! 
auto detection of audio/strum/voct, 

1.0.1 fix model persistence, add input gain, code re-org 
1.0.0 Initial Release 

## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear


## Installation
copy the plugins/*.so to sdcard /plugins

## Optional Installation
copy the plugins.presets/* to  sdcard /plugins/plugins.preset 
(same folder structure)



## General Usage 

The best source of information about this module is the Mutable Instruments Rings manual
https://mutable-instruments.net/modules/rings/manual/


## Differences


Generally, I've tried to make as close as possible to the hardware modules.

Pitch & Frequency are synonymous in this context. Rings actually uses pitches internally not frequency ;) 
Excitation choice - see below
Models, I use a number to represent the models, 0,1,2 are the 'normal' modes, 3,4,5 are alternative modes.

added In Gain, this allows the input signal to be amplified, 0 = no gain (1x), up to 1.0  = 17x


## Excitation source
The Rings Eurorack module automatically detects jacks being connected into IN, V/OCT and STRUM.
Unfortunately, this is not currently possible with SSP VSTs.

This functionality is replicated with the 'buttons'.

AUDIO: if active, use AUDIO IN - VST IN 1 as the excitation source.
STRUM: if active, use STRUM  - VST IN 2, to excite the internal exciter
V/OCT: if active, use V/OCT  - VST IN 3, changed notes will excite the internal exciter

tips/typical use:
STRUM = for non-pitched excitation, or pitched where you just want the pitch encoder to be used.
STRUM + V/OCT = or pitched excitation
AUDIO + V/OCT = use external excitation, and pitch the resonator


## VST Input/Outputs
Currently, the SSP does not use the VST's name input/output.
The help screen ( ? button ) shows the mapping


## Presets
Currently the SSP does **not** save the VST state within the SSP preset.
Instead, it associates the SSP preset with a numbered preset that the VST manages.

When you start this VST it will create 20 'preset slots' , which you can select and associate with the SSP preset.
you can then write this **VST preset**  using WriPr within the VST.

this means the 'normal process' is for saving an SSP preset with a particular VST state is:
a) select a unique VST preset
b) change when you have the VST sounding as you wish use VST:WriPr.
c) write the SSP preset  (which will associate this SSP preset with the preset selected in (a)) 


### Can you have more than 20 presets?
Yes, you can create more in /media/linaro/SYNTHOR/plugin.presets/rngs

(see note below about number/naming) 

### Can I rename them?
Yes, but bare in mind the SSP preset holds the 'number' not a name, this is an index into this directory. so preserve the number order, or the SSP **will** load the incorrect preset!
to circumvent this, I recommend you just append a description to the preset.
e.g. 
000.json -> 000-MyAwesomePreset.json
001.json -> 001-APrettyBell.json

this will preserve the ordering, and so mean your SSP presets will be unaffected.



## Tips
Note the outputs are ODD and EVEN, not stereo pairs, and their 'usage' changes depending on 'poly' (as detailed in the manual).
on the SSP we can simply sum these to one input on another module, and then use attenuation or a vca to mix.
However, there is alot of fun to be had panning these in varying degrees.

I highly recommend exciting Rngs with an external audio source (NOI/WTO/LFO), you can get all sorts of interesting sounds, that are not possible with internal exciter

I only recommend using at 48Khz sample rate, as this is how the Rings code is designed.
Using at other sample rates will lead to incorrect pitch, and possible other oddities.
( the only solution would be downsample/upsampling but this is sub-optimal on a platform such as the SSP)


## Credits: 
Mutable Instruments
Thanks to Emile for sharing her code with the community.
please note: this VST in no way endorsed or supported by Mutable Instruments.
