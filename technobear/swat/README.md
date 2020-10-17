# swat - swiss army tool

swat is designed to be like the famous swiss army knife, a useful utility to bring out to solve your problems.

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

The goal of SWAT is pretty open ended, its designed to be a plugin that I can extend quickly and easily.
It's inspired by Expert Sleepers Disting (but shares no code etc...)

The plugin can run a variety of algorithms, that can do a variety of (unrelated) functions.

The 'interface' into swat is the same for all algos - each has 3 inputs X, Y and Z, and has 2 outputs A and B.
(This is fixed and cannot change!)

I aim to use thee as follows...
### inputs
X - primary input signal
Y - secondardy modifier signal
Z - third input, often a gate or trigger

### outputs 
A - main output
B - alternative output

however, the specific meaning of each parameter is particular to each algo.

notes: 
- some algos may be designed to work at audio rate.
- different algos will have different computational costs


## List of algorithms






## Can you add this algorithm?
Possibly...  of course this comes down to a couple of aspects.
- do I have time :) 
- does this fit SWAT? or would it be better of as a dedicated module?

Note: Whilst Disting may be a source of ideas, I do **not** plan to implement its algorithms!
This would be a mammoth task, 
but also pretty pointless... 
there are many functions in Disting that can already be achieved by other modules or by the SSP natively.
e.g. you dont need a precision added on the SSP since summing is automatic and lossless on the SSP anyway.

We are not limited by hardware production costs or physical space..
So more complex functions would likely be better implemented as dedicted modules.
e.g. having simple sample/looper in SWAT doesn't really make much sense, it'd be better to have a dedicated module to do this.




This document is based on the LATEST version of the plugin


## Tips



## Credits: 

Percussa SSP - https://www.percussa.com/ 

