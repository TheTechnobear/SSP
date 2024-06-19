# Dual

Dual is related (technically) to Trax.. however its fundamentally different too.
this means, I tend to copy code from Trax into Dual (which is simpler)
In this document I will keep track of this, so that as Trax is updated, 
where appropriate I can move these changes into Dual


# Dual vs Trax
Dual is MUCH simpler, it has two modules (left and right) these modules are directly linked to the IO of the DUAL module.
therefore there is no 'internal patching' via matrix.

the only real similarities are: 
a) it loads modules dynamically
b) it forwards UI messages from the buttons/encoders to active module
c) it displays two compact UI for the modules

a is handled by LoadModuleView
b & c are handled by ModuleView

my aim (ideally) would be that LoadModuleView and ModuleView are identical between Trax/Dual
therefore the 'functionality' in dual is handled simply by PluginProcessor, PluginEditorVew and DualView


note: LoadModuleView/ModuleView do call into PluginProcessor (etc) so the idea is we have a similar interface.
and in some cases methods from Trax::PluginProcessor may also be copied (detailed later here)



# Copied code and changes

## ModuleView.h/cpp
copied

## LoadModuleVuew.h/cpp
copied

## Module.h/cpp
copied

