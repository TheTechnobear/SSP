# thing left to do ...
mainly concerned with refactoring ! 



## refactor in progress !

SSPActions - new events! 

these events represent button clicks, and so 'gestures' and are an abstraction above onButton, which has up/down 
in particular we can use to represent a consistent ui, e.g. action on release.
also, we now have gestures like 'combo buttons' and long press
its important plugins move to these to prevent oddities wwhen used in more complex ui in particular xmix/dmod 

note : existing on.. calls will still work, and still do have uses
(e.g. pressing a key and wanting user feedback)

however, if pluging is going to be used within xmix it will need to use these new events.




TO DO  - HIGH PRIO
------------------
- BaseView has timer removed 
this was moved to SSPUI, timer used to call onSSPTimer, and repaint.
SSPUI calls onSSP however, we do not call repaint()
- check DATA due to above (as it needs to repaint periodically)


TO DO  - MED PRIO
------------------
- Text Edit... test check working as intended, see belo (for TextControl)
- retest all plugins on SSP look for 'oddities'
- PMIX ... check is not broken, perhaps move to new events - but its complex! 



TO DO  - LOW PRIO
------------------



OTHER
------------------
- FileSelector - either update FileBrowser to use, or updates plugin to use
- TextControl - either update TextEdit to use, or updates plugin to use
- FileSelector - support compact and non-compact mode (currently compact only)
- TextControl - support compact and non-compact mode (currently compact only)
- ListControl - review if other possible use cases
- ensure correct background colours for new controls (FileSelector/TextControl/ListControl)

- review Editor / View hierarchies ... reduce number of variations

- review ValueButton vs ParamButton, as this can cause issues missing the types eg. with ButtonBox
- use ButtonBox control in editor/view (do we need a ParamButtonBox , see above ValueButton vs ParamButton)
