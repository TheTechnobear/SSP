Basic Plan ! 



DMOD will be the DUAL module that can use compact UIs
this could include loading XMIX


XMIX is the modular 'mixer'
it's main focus will be a compact UI, however.. potentially later a large UI could be created.
(however, it will still have the same behaviour as on xmx, i.e. you use 2 in dmod if you want double!)



XMIX 

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





