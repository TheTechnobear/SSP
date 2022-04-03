# Technobear Plugins for the SSP 
A collection of plugins for the Percussa SSP 


## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear


## Installation
copy `*.so` to SSP's sdcard `/plugins`


## Documentation 
Documentation can be found on my [SSP Wiki](
https://github.com/TheTechnobear/SSP/wiki/Modules%3ATheTechnobear)

### Midi 
My plugins now support midi.
to configure you must go to the 'general' panel of the VST.
this is accessed by pressing and holding Right Shift(RS), and then pressing Left Shift (LS).
use the same to exit this panel.

here you can set a midi input device, and also a midi output device.
you may also 'filter' the midi input to a fixed midi channel

these are used for all midi functions, including midi learn (see below)


#### Midi Learn
to use midi learn, select a midi input device 
then activate LEARN, 
now return to the VST main panel using RS+LS (as you used to enter general panel)

now press change a parameter on vst (could be encoders or buttons!)
then turn a pot/encoder on you midi controller.

you may repeat this process, so change parameter, then send midi...

note: when you send midi, you should immediatly see the parameter respond to the midim 
if NOT check you are getting midi on the SSP.


if you enable midi output, then the SSP will send out the corresponding midi to the output device, 
this allows you midi device to stay in sync (if your midi device supports this!)

note: currently only midi cc 7 bit, and note messages are supported.


#### Midi Tools modules
all these use the same 'general' panel for their midi input and output needs.



#### Midi Output 
currently there is an issue in the SSP firmware which means it will connect to the 'virtual' output device that is created.
this will mean the MIDI objects stops working, and cause issues when you switch patches.

workaround: re-connect a physical midi device so that the SSP picks that up 




note: these text files are using markdown format, this allows extra formatting in some editors/browsers



## License
These plugins are released as open source, under a GPL agreement, please see the LICENSE file.


## Credits: 

Thanks to Bert and Celine @ Percussa for creating a wonder open platforms
Percussa SSP - https://www.percussa.com/ 

These plugins also use JUCE - https://juce.com under the open source GPL agreement - thank you.


Some plugins may use other open source code, please see individual readmes for additioinal credits
