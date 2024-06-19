# dual module - todo

my current thoughts on how to update according to trax

Trax and Dual will be necessarily different.
Trax is 4 tracks + 8 user modules + in/out modules
Dual will be 1 track, 2 modules no IO module.
essentially dual is a bit of a mix between TraxHost and Trax.

what are the changes from current version of Dual?
- LoadModuleView - module categories
- LoadModuleView - button changes
- Module - has moved from PluginProcessor.h -> Module.h/cpp
- ModuleView - looks very similar


I've copied these across now need to adapt surrouding code whilst keeping similar as possible.
mainly this is in PluginProcessor.h/cpp 
we can copy some of this from Trax...

main difference (currently), we do not have Track (h/cpp) not sure if I want to copy this,  or create a similar interface.. 
let's see if we can avoid... without having to change the above copied code too much.


ALSO 
PluginProcessor.h/cpp - Trax now uses XML for storage.
this is now using                 
tracks_[trackIdx].setStateInformation(*xmlTrack);




other to do is obviously testing on ssp ;) 


## previous todo... review ! 
- SSP UI is not updating corrctly
refresh is not handled correctly see trax... likely repaints are missing
