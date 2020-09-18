# mmx4 matrix mixer

a matrix mixer with 4 stereo inputs and 4 stereo outputs, controllable via cv


## Change Log

1.0.0 Initial Release 

## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear


## Installation
copy `plugins/*.so` to sdcard `/plugins`

## Optional Installation
you may want to copy and rename the demo presets contained in `/presets`
you may now remove `/plugins.preset` folder on SSP sdcard as its no longer used 


## General Usage 

This document is based on the LATEST version of the plugin

A matrix mixer allows inputs to be mixed to multiple outputs.
In the case of this plugin each input ( 1 - 4 ) is stereo, and also each output ( A - D).

the user can mix with the parameters on the UI or also via CV.

the mixing can be consider like a bipolar vca or attenuverter, so can also be used to invert signals.

all mixing is done at audio rate, though can of course be used for CV mixing as well.

the 'level' is per input/output pair e.g.  2D, 3B , and there is no pan control.

all gain levels are linear


## Tips

whilst the mixer is stereo, you can use if for mono signal, the mixer is optimised to only calculate for the input and outputs connected.

also there is not 'connection' between left and right channels, the limitation is they use the same vcas.

whilst panning is not implemented, if you chain two mmx4 this is easy to achieve, with 2 mmx, connected something like:

1L->1L  cv=pan       -> 1L  vca1 -> 1L
2L->1R  cv=pan       -> 2L  vca1 -> 2R
1R->2L  cv=-ve pan   -> 1R  vca2 -> 1R
2R->2R  cv=-ve pan   -> 2R  vca2 -> 2R



## Possible future updates
Some ideas I have for improvements

vca 'scaling' 
there are a couple of different modes I see useful that do not use linear scaling
exponential scale - convert the linear vca values into exponential, useful for audio levels
pan law - if we want to cross fade, then things like cos/sin pan laws are often more useful than linear fading.



## Credits: 

Percussa SSP - https://www.percussa.com/ 

