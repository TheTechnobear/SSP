# investigations into keyboard and mouse support for SSP


## Goals?

Why add keyboard/mouse support?

the obvious 'user' one is to enter things like filename... 
but frankly, this is alot of effort, for very little benefit.
few are going to attach a keyboard, for this very infrequent task!


however, I have a couple of other scenarios...

if I could add broad support for keyboard/mouse for my plugins this might be interesting.
ie. using a keyboad to select parameters, and a mouse to dial them in.
this could be esp. useful in test environment on macOS!


specific module support
Ive a couple of ideas for modules which would benefit from keyboard support.
i.e. you'd use a keyboard **because** you were using this module.
similarly I could envisage mouse as an 'input source' for generating cv ! 






## Native juce

**tl;dr;  - kind of works on macOS, not on SSP**


as expected when running on the SSP , the keyboard and mouse handling in juce is not working.
this is likely because its not using any kind of desktop.

on macOS, 
mouse events work as expected for components automatically (i.e  if its a visible component) 

you can get keyPressed to work IF you are very careful with grabKeyboardFocus()
the main issue is by default all components (including my editors) try to grab the focus...
so the focus is not where you need it to be 'by default' 
(aka keyboard handling needs some work, to make sure focus is where we want) 


## SSP using input events (raw) 
the above means we need to be reading the raw device input to get this to work on the SSP.
we can use the inputs events to do this



### SSP Keyboard

keyboard is a bit tricky, as it can move around...
mine is on  `/dev/input/event6`


how do we 'officially' determine location?

we look at `/proc/bus/input/devices`
this sees all devices and we can see the entry

```
I: Bus=0003 Vendor=1ddd Product=1130 Version=0110
N: Name="HID 1ddd:1130"
P: Phys=usb-ff500000.usb-1.1/input0
S: Sysfs=/devices/platform/ff500000.usb/usb2/2-1/2-1.1/2-1.1:1.0/0003:1DDD:1130.0001/input/input6
U: Uniq=
H: Handlers=sysrq kbd leds event6 
B: PROP=0
B: EV=120013
B: KEY=10000 7 ff9f207a c14057ff febeffdf ffefffff ffffffff fffffffe
B: MSC=10
B: LED=7
```

this is 'likely' the case, as event0-5 are mapped to the hardware buttons, so event 6 is the 'next event' after these.
seemed to be ok, for a couple of keyboards I tried, and same for if present on power-on, or if plugged in later


note: these event queues (incl mouse0) only exist if device is plugged in.


to read... (essentials only)
```
#include <linux/input.h>
struct input_event ie;

```




### SSP Mouse
for mice this is 'easy' `/dev/input/mouse0`   0 we know this will alway be there... when plugged in.

however, seems to be most use then event queue like keyboard...
here we see it on `/dev/input/event7`

```
I: Bus=0003 Vendor=1ddd Product=1130 Version=0110
N: Name="HID 1ddd:1130 Mouse"
P: Phys=usb-ff500000.usb-1.1/input1
S: Sysfs=/devices/platform/ff500000.usb/usb2/2-1/2-1.1/2-1.1:1.1/0003:1DDD:1130.0006/input/input14
U: Uniq=
H: Handlers=mouse0 event7 
B: PROP=0
B: EV=17
B: KEY=1f0000 0 0 0 0 0 0 0 0
B: REL=1943
B: MSC=10

```

seems like we'd want to use similar event code for mouse/keyboard, but the above makes it LOOK like mouse0 and event7 are same.
if so we can use the alias? but this will need to be tested.

note: mouse exposed relative coordinates 



https://www.kernel.org/doc/Documentation/input/input.txt

https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/tree/include/uapi/linux/input-event-codes.h


a pretty simple example...
https://gist.github.com/uobikiemukot/457338b890e96babf60b



## Approach?


the juce/macOS approach doesn't really just work 'out of the box', and the SSP doesnt work at all.


so my temptation is...
add keyboard/mouse support to the SSP 'api', 
basically, I would have all keyboard/mouse handling inside EditorHost to generate `SSPActions`


the idea is simple... with the sole implementation in EditorHost, 
most of code inside editors/component will be for SSPActions, so common for mac and SSP.

rather than dealing with 'quirks' of juce/input device inside all editors etc.



### Editor Host

there would be two implementations inside EditorHost.

one for juce, this would take ALL event.. so ensure we dont get any standard component behaviour
second, for SSP, which reads the input events (as described above)


note: 
I need to be careful to ensure juce does not start distributing by default to Components in my setup. ... I want to take full control.

I might need to ensure I subclass Component, to ensure my components NEVER take keyboard actons...
(but this might be enough to do in base editors etc)



### SSP Actions 

extend the SSPActions interface, and add support into things like BaseEditor


e.g. 
```
void onKeyboard(int keycode, bool value);
void onMouseMove(float x, float y, bool btn1Down, bool btn2Down);
void onMouseButton(int button, bool value);

```

Im trying to avoid any clash with the Juce API (e.g. thing like keyPressed which exist in Component),
though Id like it to be consistent with ssp api.

following this approach, I can then forward these events in the same way as I do current SSP events (onEncoder)

this means I can be very explicit about where these events go, and who handles them.

I need to conside the whole relative/absolute aspect for mouse handling





