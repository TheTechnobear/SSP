
# TODO 

# milestone 1 & 2 done 

- keyboard for save  - ability to delete characters?
- other modules? see below


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


# potential modules to add?
- sample playback
- simple trigger sequencer
- euclid sequencer
- midi file playback?

things on SSP factory:
arp, bcr, bus, efl, lfo, env, gra, midi, noi (noise- shq?), 
sam, ste, svf ,ws (waveshaer) wto (wavetable) 


# close of 'dialog views'
I dont like the way the editor, has to know about the buttons used.
it'd be better to have a isClosed function checked in SSP Timer?
or some kind of callback function... to notify parent view to switch back.

- not really liking the hold UP button to close module window.
but really there is no other choice, I can think of... as we use up/down to go thru sets of parameters



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



