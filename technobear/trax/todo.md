
# TODO 

# milestone 1 & 2 done 

- keyboard for save  - ability to delete characters?

- test rings
- other modules? see below
- use hold event rather than long press?


# Possible?
- add save/load to GRA4/LOOP?
make use of new components for text edit and file loading?
- add glide to CART?

- rename matrix to -> ? - DONE , called routing in UI
- consider locking strategy... do we need both track and module? - DONE, need track as used for locking matrix


## miletone 3 - refinements
- more UI refinements to Trax  
- refine available plugins for trax, polish
- start adding more plugins, considering new use-cases for trax.


## other tasks
- review code and tidy up certain parts
- should matrix be renamed? review other nomenclature.
- review editors and views... getting very confusing!
- rationalise button box drawing esp with value buttons - DONE


# potential modules to add?
- sample playback
- simple trigger sequencer
- euclid sequencer
- midi file playback?

things on SSP factory:
arp, bcr, bus, efl, lfo, env, gra, midi, noi (noise- shq?), 
sam, ste, svf ,ws (waveshaer) wto (wavetable) 

## wire offset/scaling
scaling of signal... we could add to route screen
use encoder 4 to allow for scaling (normal turn) or offset (press + turn)



# close of 'dialog views'
I dont like the way the editor, has to know about the buttons used.
it'd be better to have a isClosed function checked in SSP Timer?
or some kind of callback function... to notify parent view to switch back.

- not really liking the hold UP button to close module window.
but really there is no other choice, I can think of... as we use up/down to go thru sets of parameters

## performance interface?
idea is to allow a parameters to be moved to a 'central' performance window.
so you dont have to dig into to individual plugins to alter.
ideally this be across ALL tracks

note: 
parameters would need to be added to plugin descrition ex
we would also need get/set in the plugin processor api

notes: parameters cannot be added to description - due to way ParameterTree works.
so has to be done 'live' thru processor api - this is ok, since you only want to do with loaded module anyway

added : api changes, may need improvemnt

one 'issue' it will need to pull values, unless we implement a callback interface from plugin... 
which we do not currently have at all !



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


## existing plugin coversion  - restrictions - later enhancements?
- add save/load to GRA4/LOOP
- add glide to CART


### Modules NOT supported
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



