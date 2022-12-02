## message manager issues

these are noticed when running debug versions of vsts on SSP we get various assertions 


it should be noted, that these assertions seem harmless when run not run in debug



```

(gdb) bt
#0  0xb67e5e9c in kill () from /lib/libc.so.6
#1  0x9d40c06c in juce::Timer::startTimer (this=0xb4a551d4, interval=100) at /Users/mark/projects/SSP.private/juce/modules/juce_events/timers/juce_Timer.cpp:335
#2  0x9d40c478 in juce::Timer::startTimerHz (this=0xb4a551d4, timerFrequencyHz=10)
    at /Users/mark/projects/SSP.private/juce/modules/juce_events/timers/juce_Timer.cpp:351
#3  0x9d07030c in juce::AudioProcessorValueTreeState::AudioProcessorValueTreeState (this=0xb4a551d4, p=..., um=0x0)
    at /Users/mark/projects/SSP.private/juce/modules/juce_audio_processors/utilities/juce_AudioProcessorValueTreeState.cpp:283
#4  0x9d07012c in juce::AudioProcessorValueTreeState::AudioProcessorValueTreeState (this=0xb4a551d4, processorToConnectTo=..., undoManagerToUse=0x0, 
    valueTreeType=..., parameterLayout=...)
    at /Users/mark/projects/SSP.private/juce/modules/juce_audio_processors/utilities/juce_AudioProcessorValueTreeState.cpp:227
#5  0x9d05059c in ssp::BaseProcessor::BaseProcessor (this=0xb4a55080, ioLayouts=..., pl=...)
    at /Users/mark/projects/SSP.private/technobear/common/ssp/BaseProcessor.cpp:21
#6  0x9d03db40 in PluginProcessor::PluginProcessor (this=0xb4a55080, ioLayouts=..., layout=...)
    at /Users/mark/projects/SSP.private/technobear/data/Source/PluginProcessor.cpp:15
#7  0x9d03d218 in PluginProcessor::PluginProcessor (this=0xb4a55080) at /Users/mark/projects/SSP.private/technobear/data/Source/PluginProcessor.cpp:10
#8  0x9cfc156c in createInstance () at /Users/mark/projects/SSP.private/technobear/data/Source/SSPApi.cpp:14
#9  0x00080e60 in Plugin::openAndInstantiate<Percussa::SSP::PluginInterface*, Percussa::SSP::PluginInterface* (*)()> (name=<optimized out>, f=..., error=...)
    at ../../common/Plugin.h:114
#10 0x00080aec in Plugin::loadPlugin (this=0xaa53af68, fileOrId=..., error=...) at ../../common/Plugin.h:608
#11 0x0007bd10 in Synthor::pluginLoadPlugin (this=this@entry=0xaa29c080, trackNr=trackNr@entry=1, fileOrId=..., errorMsg=...) at ../../common/Synthor.h:9911
#12 0x0007ae5c in Synthor::setUnitGenType (this=<optimized out>, trackNr=1, t=4918304906214899712) at ../../common/Synthor.h:11448
#13 0x0007843c in PresetReaderV5::readModuleChunk (this=this@entry=0xaaa32614, key=key@entry=220, s=..., is=..., lp=5425, length=20)
    at ../../common/PresetReaderV5.cpp:331
#14 0x000764c0 in PresetReaderV5::readChunk (this=0x0, this@entry=0xaaa32614, s=..., is=...) at ../../common/PresetReaderV5.cpp:156
#15 0x00076318 in PresetReaderV5::readPreset (this=0xaaa32614, synthor=0xaa29c080, is=..., startPos=33) at ../../common/PresetReaderV5.cpp:87
#16 0x000c0c50 in MIDIBridge::loadPreset (this=this@entry=0xaa29c040, file=..., presetName=...) at ../../common/MIDIBridgePresets.cpp:228
#17 0x000c2a18 in MIDIBridge::switchPreset (this=0xaa29c040, pchn=0) at ../../common/MIDIBridgePresets.cpp:993
#18 0x00087ee8 in MIDIBridge::predraw (this=0xaa29c040) at ../../common/MIDIBridge.cpp:959
#19 0x000cadec in main::$_0::operator() (this=0xb5eca740) at ../../common/Main.cpp:316
#20 std::_Function_handler<void (), main::$_0>::_M_invoke(std::_Any_data const&) (__functor=...)
    at /Users/mark/xc/arm-rockchip-linux-gnueabihf_sdk-buildroot/arm-rockchip-linux-gnueabihf/include/c++/8.4.0/bits/std_function.h:297
#21 0x000c9a7c in std::function<void ()>::operator()() const (this=0xbeedba88)
    at /Users/mark/xc/arm-rockchip-linux-gnueabihf_sdk-buildroot/arm-rockchip-linux-gnueabihf/include/c++/8.4.0/bits/std_function.h:687
#22 drm_loop(unsigned int, unsigned int, std::function<void ()>, std::function<void ()>, int, std::function<void ()>, std::function<void (float)>, std::function<void (unsigned int, unsigned int)>, std::function<bool ()>) (width=1600, height=480, predraw=..., postdraw=..., interval=1000, intfun=..., draw=..., prepare=..., 
    stop=...) at ../../common/SSPDRM.h:635
#23 main (argc=<optimized out>, argv=<optimized out>) at ../../common/Main.cpp:426

```

also... 
JUCE Assertion failure in juce_Component.cpp:1917
JUCE Assertion failure in juce_AsyncUpdater.cpp:66

theere are cause bu AudioProcessorValueTreeState, and ParameterAttachment attempting to use MessageManager.
the assertion is basically a check that MessageManager exists.


the way this usually works is any instance of a plugin will create ONE instance of the message manager with its own thread.
(e.g. 5 copies of DATA would all use same instance, but 1 DATA + 1 CLDS creates two)

no real way around this....
we could call MessageManager::Instance... so that juce believes its there, but it'll just never get serviced.
(so basically, it'll just please the assertion, but not do anytihng )

also the MessageManager function runDispatchLoop() (?) is blocking so has to be in its own thread.
and there are no methods to do this 'manually', since that code is all hidden away in the platform specifc implementation of the message manager.
(so this have 'private' messages queues)
... similarly, you cannot do anything about the posting to that queue, since that is done in MessageBase::post , which posts via (again) a platform specific method.

(basically JUCE, likes to declare functions in MessageMaanger.h and similar ) but then implement them in the platform layer...
this means they cannot be overriden, without implementing an entire new OS platform ! (which is not practicl ) 