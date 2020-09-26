# Msw8 : Matrix Switch 8 

8 in / 8 out switch matrix



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

This plugin forms a signal switch matrix with user (and cv) selectable input and output.

This is similar to the switch (SWI) module by percussa.
the SWI allows user to have an input that is directed to **one** of 8 outputs, which is determined by the **selector**
so we can see this is   1 in -> 8 out **selectable** , so only one is actually output, others receive nothing.

usage example: 
take an oscillator and send it to different (selectable) fx 
e.g OSC -> REVERB **or** DELAY -> OUT



Initially I wanted the reverse... so we have 8 inputs (one selectable) which goes to 1 output  ie. N to 1

usage example:
take different oscillators (SQR, SAW) and send to a filter
e.g.SQR **or** SAW -> SVF -> OUT


However, I realised we could combine both ideas , by considering this as a matrix
one axis is inputs, the other is outputs.
and so this is what this plugin does!

it takes 8 possible inputs, and 8 possible outputs.

then we use :
IN SELECTOR to select the active input for the signal. 
OUT SELECTOR to select the active output for the signal. 


both selectors are bi-polar values (so -1 to +1 ), so by default each step is 2 / 8 = 0.25
e.g.  -1.0 > <=-0.75 = select 1, -0.5 > <=-0.25 = sel 2 etc 

both selectors are CV controllable, and are added to the parameter values

however, Msw8 has one more 'trick' up its sleeve :) 

### USE ACTIVE
tells the matrix to only switch between input / outputs that have connections. (aka active)
this means we can use this for small matrixs, e.g. 2x2  (doesn't have to be symmetric)
in this case, steps are 2/ # active connections .. so 2/2 = 1! 
e.g -1.0 >  <= 0 = sel 1 , 0 > <=1 = sel 2 

### SOFT  
switching an audio signal abruptly on/off will cause an audio click, 
due to creating an non-continuous wave.
with SOFT turned on when a switch is made this click is removed by fading in/out signals.
this fade is done over one audio buffer, the first half fades out, then second half fades in.


note: This document is based on the LATEST version of the plugin


## Tips


## Credits: 

Percussa SSP - https://www.percussa.com/ 

