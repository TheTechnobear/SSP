
# TODO 


## milestone 1 - basic functionality - DONE
aim to have everything basically functional

## milestone 2 - usability test/refine
general goals:
- UI is clear and rational.
- start improving UI elements in Trax
- ensure the buttons mappings etc feel comfortable to use


specific tasks:
- add mini editor to existing plugins - see below
- make a minimum set of usable plugins , aka first-use case
- consider locking strategy... do we need both track and module?
- rename matrix to -> ?
- consider stream protocol versioning


## miletone 3 - refinements
- more UI refinements to Trax
- refine available plugins for trax, polish
- start adding more plugins, considering new use-cases for trax.


## other tasks
- review code and tidy up certain parts
- should matrix be renamed? review other nomenclature.
- review editors and views... getting very confusing!
- rationalise button box drawing esp with value buttons

- what is 'first' use-case?
I think its mixer + fx with modulation?
with perhaps some 'additions'? (e.g. playing plts via cv)

- potential modules to add?
-- scope 
-- midi file playback?
-- sequencer
-- drum machine (w/sequencer)



## existing plugin coversion
### NOT STARTED 


shq - use list control?, so native, but we may be able to use param?

mtmo - native ui, but looks trivial
logi - bit of native ui, but should be ok?
msw8 - not just param based, but should be easy enough
mmx4 - not just param based, but should be easy enough


data - bit of ui work, but worth it
cart - quite a bit of ui work, but worth it!
gra4 - needs new ui - update wavedisp, use new save/load dialogs?
loop - needs new ui  - update wavedisp, use new save/load dialogs?


### DO NOT SUPPORT?
pmix - not required? perhaps a smaller version?
dual - only valid for full screen
swat - low priority?

vost 
a lot of parameters... 10 pages/layers each with 16 voltages
feels like a variation, or a change to vost would be better... not sure how to name... 
e.g.
we could change to 8 layers, and then use buttons to select layer
thus we only get 4 pages for each layer
... we have an issue with select, but we could remove this and just have as CV input!?


## IN PROGRESS / PARTIAL

clkd - value button add?
omod - value button add?

### DONE
clds 
plts
rngs
pvca
dlyd
srvb
comp
harm
trax
mtin
mtot
mttr
drum - has been reduce to 4 params per drum, its probaly going to need some kind of page labelling!
ldrf
attn
sfft - generic rnbo
sflt - generic rnbo


-----------------------------------------------------


# Overview
Basic Plan ! 
DUAL module that can use compact UIs, this could include loading Trax


Trax is the modular 'mixer'
it's main focus will be a compact UI, however.. potentially later a large UI could be created.
(however, it will still have the same behaviour as on xmx, i.e. you use 2 in dmod if you want double!)



Trax 

the idea is to have 4 tracks in a mixer


each track has: 
- stereo input
- stereo output 
- a MOD module 
- a PRE module
- a MAIN module
- a POST module

the mixer has : 
- 4 tracks
- 8 inputs 
- 2 output

# processing and IO

IN 1-4 = Track 1-4 Left 
IN 5-8 = Track 1-4 Right OR 'free mod source' as MOD 1-4

IF 5-8 are used as MOD, then 
- LEFT is normalled to RIGHT for the track.

processing is : 

for each Track 1 - 4
- MOD
- PRE
- MAIN 
- POST

then the outputs are summed , for OUT 1/2 (nominally left/right)

tracks are processed 1 to 4 - initially this will be single threaded, 
later each track will be on a separate thread, and the SUM will be done on the main thread

# modules 

it is assumed...
PRE/POST modules have stereo input and output.
(we may possibly need some mapping here... or text io? look look for IN/OUT?) 

MOD 
assume no input
we will then MAP output to input for PRE/POST/MAIN module

MOD will need clocks support - 
both an internal clock, and also being able to assign from IN 5-8 (aka MOD 1-4) 




# Internal modules 
Im considering having some internal modules will are using SSPApi, but are compiled in.
in particular: 
-'PASS Thru' used (optionally) for PRE/POST
- MIX - the MAIN Mix module



# Mapping requirements:
MOD needs to be able to MAP its outputs to PRE/POST/MAIN
IN : MOD 1-4 to be able to be mapped to Track 1/4 then PRE/POST/MAIN/MOD?

we will need 1 : N mapping ... 
(probably not N:N initially?) 

currently not considering cross track modulation.


considerations
- we need to be able to ADD/REMOVE mapping
- IF a (PRE/POST/MAIN) module is changed, we will need to remove its mapping, since its different 



# load module requirements.
we'll need to be able to load MOD/PRE/MAIN/POST for each track.and store their settings
so 16 modules will be loaded and processed

It MAYBE initially the MAIN module cannot be swapped?!... is it the 'mixer' bit.



# Threading

tracks are on separate threads.
each will be given their own copy of the iobuffer.

basic process is: 
DSP threads are waiting for signal
plugin thread will copy audio to threads buffer
signal dsp to start
do its own DSP processing (!)
wait for all DSP threads to complete.

we should be able to use spin lock for main thread, using simple test n'set
we need to consider how to block other dsp thread.

module changes are currently protected by a test n set.


UI and DSP both attempt to grab lock for processing and changing.
if they cannot get it then
UI - will try again (assummption dsp is quick to execute- fast fail)

DSP - will just not process that module.
this will potentially cause audio glitch, since audio may not be passed thru, 
or will be different audio, but switching modules is generally likely to do this?!

this same approach will need to be done for modulation routing
(so that we dont end up visually changing routing whilst DSP is trying to use)

# alternative plugin flip - later alternative?
rather than blocking the DSP thread we could 'hot swap'
basically, load the plugin within UI thread, (as we do currently)
but 'queue' it so that the DSP thread does the actual 'flip' in a single operation. 
(like a backing buffer).
then once 'flipped' the 'old' plugin could be unloaded.

this would mean the audio thread would never have to wait, so no audio disruption.

note: we need to be a bit careful on 
- operations that need to be done on UI thread vs DSP thread
- UI needs to draw the new plugin editor.

this might be ok, if we BLOCK the UI thread until THE dsp thread actually has flipped.
(this might align nicely with the UI thread being able to do the unload)



