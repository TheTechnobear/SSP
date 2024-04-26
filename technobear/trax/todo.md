
# TODO 

## milestone 1 - basic functionality - DONE
aim to have everything basically functional

## milestone 2 - usability test/refine
general goals:
- UI is clear and rational.
- start improving UI elements in Trax
- ensure the buttons mappings etc feel comfortable to use


specific tasks:
- add mini editor to existing plugins - see below - DONE
- make a minimum set of usable plugins , aka first-use case - DONE?
- consider locking strategy... do we need both track and module? - POSTPONE
- rename matrix to -> ? - DONE , called routing in UI
- consider stream protocol versioning - NEED TO CONSIDER


## miletone 3 - refinements
- more UI refinements to Trax  
- refine available plugins for trax, polish
- start adding more plugins, considering new use-cases for trax.

------------------------------------------


## other tasks
- review code and tidy up certain parts
- should matrix be renamed? review other nomenclature.
- review editors and views... getting very confusing!
- rationalise button box drawing esp with value buttons - DONE

- what is 'first' use-case?
I think its mixer + fx with modulation?
with perhaps some 'additions'? (e.g. playing plts via cv)

- potential modules to add?
-- scope 
-- midi file playback?
-- sequencer
-- drum machine (w/sequencer)

## wire offset/scaling
scaling of signal... we could add to route screen
use encoder 4 to allow for scaling (normal turn) or offset (press + turn)

## bugs/ thoughts? (high priorty)
- should we remove connections (wires) when we remove modules
(currently you end up with wires you cannot edit, since Empty has no connections!)

- ui concerns
not sure I like the preset TRACK button, then encoder for plugin
... doesn't feel right... 
its likely the issue is the 'track screen', its not very intuitive whats on each track

- keyboard for save 
input field needs to be bigger
perhaps use second encode for up/down 
ability to delete characters?

- close of 'dialog views'
I dont like the way the editor, has to know about the buttons used.
it'd be better to have a isClosed function checked in SSP Timer?
or some kind of callback function... to notify parent view to switch back.

- not really liking the hold UP button to close module windw.
but really there is no other choice, I can think of... as we use up/down to go thru sets of parameters



## performance interface?
idea is to allow a parameters to be moved to a 'central' performance window.
so you dont have to dig into to individual plugins to alter.
ideally this be across ALL tracks
note: 
parameters would need to be added to plugin descrition in extended api
we would also need get/set in the plugin processor api
(shouldn't be an issue to implement on plugin side, as we already automate to some extent)



# view/editor refactoring - lower priority - post demo/release?
code refactor... the views need refactoring... too much code duplication
too many variations possible, around how much of the UI and editor wants to draw, or just default 'and work'
this includes if buttons are parameters, or are just ui elements.
also perhaps there is an over use of 'multi views' to allow for switching'

I think a better approach is using more components. 
I recently introduced button box... 
the idea is the editor can then just include particular components it needs, 
and then manage the connection between these and buttons presses with minimal code.

needs to be thought thru though, e.g. do we have pages of buttons and paramters do they get linked.
the potentially issue being, we just end up movin the 'mess' from the view to components ;) 


## existing plugin coversion
## restrictions - later enhancements?
- add save/load to GRA4/LOOP
- add glide to CART

### NOT STARTED 


### DO NOT SUPPORT?
pmix - not required? perhaps a smaller version?
dual - only valid for full screen
swat - low priority - not used much, also algos paint directly, so alot of work

vost 
a lot of parameters... 10 pages/layers each with 16 voltages
feels like a variation, or a change to vost would be better... not sure how to name... 
e.g.
we could change to 8 layers, and then use buttons to select layer
thus we only get 4 pages for each layer
... we have an issue with select, but we could remove this and just have as CV input!?

## IN PROGRESS / PARTIAL


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
shq
mtmo - uses basic view to implement ui
data - bit of ui work, but worth it
logi - uses params but also has custom graphics, should fit ok
msw8 - custom grid ui , with single line param
mmx4 - custom grid ui , with single line param
omod 
clkd 
cart - quite a bit of ui work, but worth it!
gra4 - needs new ui - update wavedisp,
loop - needs new ui - update wavedisp,



-------------------------------------------------------------------



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



