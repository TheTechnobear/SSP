# Clds : Textural 
## Change Log
1.0.0 Initial Release 

## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear


## Installation
copy the plugins/*.so to /media/linaro/SYNTHOR/plugins

## Optional Installation
copy the presets to /media/linaro/SYNTHOR/
(same folder structure)


## General Usage 

The best source of information about this module is the Mutable Instruments Clouds manual
https://mutable-instruments.net/modules/clouds/manual/


note: 
currently, SSP VSTs are only allowed a maximum of 8 cv inputs - clouds hardware has 9 ;) 

I've left out 'Blend' since I never found it useful on the hardware module.
since it affects different values based on mode - which is confusing!
if/when VSTs can have > 8 cv inputs, I will add each 'blend' parameter as a separate cv in.

trig/freeze - this is more problematic, as I actually used these both on the hardware module quite abit.
so Ive included a button toggle between, which is used - as I tended not to use both at the same time.
again, hopefully in the future they will be broken out to separate cvs.



## Differences
Generally, I've tried to make as close as possible to the hardware module.

Clouds hardware runs at 32kHZ sample rate, clds runs on Percussa runs at 48kHz.
There is currently no lo-fi or mono mode.



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
Yes, you can create more in /media/linaro/SYNTHOR/plugin.presets/clds

(see note below about number/naming) 

### Can I rename them?
Yes, but bare in mind the SSP preset holds the 'number' not a name, this is an index into this directory. so preserve the number order, or the SSP **will** load the incorrect preset!
to circumvent this, I recommend you just append a description to the preset.
e.g. 
000.json -> 000-MyAwesomePreset.json
001.json -> 001-APrettyBell.json

this will preserve the ordering, and so mean your SSP presets will be unaffected.



## Tips

Play with the different modes - personally my favourite for 'general purpose' patching is looping delay :) 

In Gain - 0.0 = no gain (or 1x) , 1.0 goes to 17x for very quiet signals, if you turn it up to much you will distort, 
so listen carefully, and I'd recommend err'ing on the side of quiet.


I only recommend using at 48Khz sample rate, as this is how the clouds code is designed.
Using at other sample rates will lead to incorrect pitch, and possible other oddities.
( the only solution would be downsample/upsampling but this is sub-optimal on a platform such as the SSP)


## Credits: 
Mutable Instruments
Thanks to Emile for sharing her code with the community.
please note: this VST in no way endorsed or supported by Mutable Instruments.
