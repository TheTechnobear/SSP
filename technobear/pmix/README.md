# pmix : performance mixer

a performance mixer for the Percussa SSP

## Change Log
1.0.0 Initial Release 

## Developer: TheTechnobear
I develop these plugins for free, please consider supporting my efforts with a donation.
https://ko-fi.com/thetechnobear


## Installation
copy the `plugins/*.so` to sdcard `/plugins`

## Optional Installation
you may want to copy and rename the demo presets contained in `/presets`

## General Usage 

8 input channels with individual pan/mute/solo and levels for main,cue , aux 1 and 2

4 stereo output channels, main/cue and aux1/aux2, with levels, mute,solo

LS/RS/UP : select in 1-4, 5-6 and output groups for encoders and buttons

EN+/- : used to select what encoders control
MODE : selects button mode

encoder buttons : reset or toggle levels etc.

CUE - mutes channel on main output, and moves it to CUE channel
AC -  places a DC blocker on the channel, useful for almost all audio signals - you can remove if you are doing DC mixing

note: CUE and AUX levels are only for input channels.


## Tips

The channel selection buttons work as momentary switches, so you do not have to switch the focus, just hold button and use encoders.

Input levels are pre-fader (since levels are independent)
Output levels are post-fader

DC mixing is possible, but vu-meters might be 'unexpected' ;) 

GAIN is pre-fader


## Credits: 

Percussa SSP - https://www.percussa.com/ 

