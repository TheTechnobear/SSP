# shq - sample and hold, with quantizer

sample and hold module with optional quantizer and tranposition

## Change Log

1.0.0 Initial Release 

## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear



## General Usage 

SHQ is a quad sample and hold module with optional quantizing.

The intention behind SHQ was to provide both a flexible and easy to use sample and hold module, 
and also a simple to use quantizer for common scales.
There are 87 fixed scales, if you need other scales (!) then I would point you to the MTQ module, which is fully customisable.


each of the SHQ 4 instances has its own signal input (SIG) , and a trigger (TRIG) input.

the basic operation is: 

on a rising edge TRIG input (> 0.5) the SIGnal is read, and output  (along with TRIG output)

if no SIG input is connected, S&H will use a random voltage (unique for each S&H instance)

if the QUANT button is pressed, then the outputs will all be quantised.
the quantisation is based on the SCALE selected and the ROOT note.


Note: This document is based on the LATEST version of the plugin

## Tips

the module is optimised so that it only evaluate those sample and holds that have outputs connected to them.

the 4 instance of the sample and hold have the trigger normalised.
this means if you connect the 4 outputs to something, and don't have a signal input for each.
then trigger the first instance, will emit random voltage on all 4 outputs.

essentially ROOT acts as a tranposition, where A4 = 0v
This in practice means that you can leave your oscillators tuned to A (default for SSP), and use SHQ to ensure pitches are transposed to a particular key (scale + tonic)


## Credits: 

Percussa SSP - https://www.percussa.com/ 

