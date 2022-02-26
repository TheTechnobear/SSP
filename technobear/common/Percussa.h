/*
	Copyright (c) 2022 - Bert Schiettecatte, Noisetron LLC. 

	This software is part of the Percussa SSP's software development kit (SDK). 
	For more info about Percussa or the SSP visit http://www.percussa.com/ 
	and our forum at http://forum.percussa.com/ 

	Permission is granted to use this software under the terms of either:
	a) the GPL v2 (or any later version)
	b) the Affero GPL v3

	Details of these licenses can be found at: www.gnu.org/licenses

	This software is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef PERCUSSA_H_INCLUDED
#define PERCUSSA_H_INCLUDED

#include <vector>
#include <string>

namespace Percussa {
namespace SSP {

    constexpr static unsigned API_MAJOR_VERSION = 3;
    constexpr static unsigned API_MINOR_VERSION = 4;

	// struct describing your plugin. for backwards compatibility, you should
	// assign the same values to the members in the struct as what you used
	// to use previously for your VST plugin. in particular, the uid field is
	// important and has to match to avoid breaking presets. the fileOrIdentifier
	// field does not have to be assigned by you, the plugin developer. it is
	// overwritten by the host application when loading your plugin.
	struct PluginDescriptor
	{
		std::string name;
		std::string descriptiveName;
		std::string manufacturerName;
		std::string version;
		std::string fileOrIdentifier;
		int uid = 0;
		std::vector<std::string> inputChannelNames;
		std::vector<std::string> outputChannelNames;
	};

	// class interface allowing the host application to ask your plugin
	// to draw its user interface graphics. the host will call renderToImage()
	// to make your plugin draw onto a texture image, which is mapped onto
	// a rectangle, drawn using OpenGLES. the rectangle has dimensions such
	// that it covers the full screen. this function will be called on each
	// frame rendered by the host application, so it needs to be as efficient
	// as possible. it is called from the application's foreground thread.
	// you need to implement a class that derives from this class interface
	// and which implements all the methods in the interface.
	class PluginEditorInterface
	{
	public:
		virtual ~PluginEditorInterface() {}

		// this function gets called at the start of each GUI frame rendered.
		// it is called before renderToImage(), regardless of whether the editor
		// is visible or not. this function is called from the UI thread.
		virtual void frameStart() {}

		// this function is called when the visibility of the plugin editor
		// changes. true is passed into the function when the editor becomes
		// visible, and false is passed when the editor becomes hidden.
		// typically, this is called before renderToImage() starts being called,
		// and after renderToImage() stops being called (see below).
		// this function is called from the UI thread.
		virtual void visibilityChanged(bool b) {}

		// this function gets passed a pointer to a BGRA image buffer,
		// and width and height contains the size of the image.
		// the function should draw the plugin GUI graphics into this buffer
		// by writing BGRA pixels to it. the pixels are stored as consecutive
		// bytes in the buffer: B G R A B G R A B G R A ...
		// note that the buffer is NOT CLEARED between successive calls.
		// the buffer passed in via this function, is also used when rendering
		// other plugin editors of potentially different plugin types, so you cannot
		// assume that the previously drawn graphics for this particular plugin instance
		// type are still valid. this function is called from the UI thread.
		// it is only called when the plugin editor is visible.
		virtual void renderToImage(unsigned char* buffer, int width, int height) = 0;
	};

	// class interface allowing the host application to ask your plugin
	// to process the next audio buffer, store and recall plugin state,
	// set parameters, prepare for audio DSP/playback, etc. the interface
	// also allows the host to get an editor interface for the plugin
	// (see above). you need to implement a class that derives from this
	// class interface, and which implements all the methods in the interface.
	class PluginInterface
	{
	public:
		virtual ~PluginInterface() {}

		// this should allocate an instance of a class which implements
		// the above PluginEditorInterface interface. this function should
		// only allocate one instance of the class, and subsequent calls
		// should return the same instance, which was allocated previously.
		// the host will deallocate the intance, so your plugin should not
		// deallocate it under any circumstances.
		// this function is called from the UI thread.
		virtual PluginEditorInterface* getEditor() = 0;

		// called when a button is pressed. mapping of buttons is as follows:
		//   0    Soft Key 1
		//   1    Soft Key 2
		//   2    Soft Key 3
		//   3    Soft Key 4
		//   4    Soft Key 5
		//   5    Soft Key 6
		//   6    Soft Key 7
		//   7    Soft Key 8
		//   8    Left
		//   9    Right
		//   10   Up
		//   11   Down
		//   12   Shift L
		//   13   Shift R
		// this function is called from the UI thread.
		virtual void buttonPressed(int n, bool val) {}

		// called when an encoder is pressed. n is a value from 0 to 3,
		// indicating encoder number, from left to right.
		// this function is called from the UI thread.
		virtual void encoderPressed(int n, bool val) {}

		// called when an encoder is turned. n is a value from 0 to 3,
		// indicating encoder number, from left to right. positive values
		// means the encoder is turning right, negative values means that
		// the encoder is turning left. as a plugin developer you can decide
		// how to process these values, how to smooth them, apply acceleration
		// or some other physics model... you will get multiple calls as the
		// pulses are detected by the hardware.
		// this function is called from the audio callback.
		virtual void encoderTurned(int n, int val) {}

		// called when an input is enabled. n is the input number.
		// this function is called from the UI thread.
		virtual void inputEnabled(int n, bool val) {}

		// called when an output is enabled. n is the output number.
		// this function is called from the UI thread.
		virtual void outputEnabled(int n, bool val) {}

		// this is called to get the current state of your plugin,
		// so it can be saved in the preset files written to disk.
		// you should return a pointer to a buffer containing your
		// plugin data, as well as the size of the buffer containing
		// the data.
		// this function is called from the UI thread.
		virtual void getState(void** buffer, size_t* size) {}

		// this is called to set the state of your plugin, after a
		// preset gets loaded. a pointer to a buffer is passed containing
		// the state data, as well as the size of the buffer.
		// this function is called from the UI thread.
		virtual void setState(void* buffer, size_t size) {}

		// called right before processBlock() will start to be called
		// by the audio DSP code in the host.
		// this function is called from the UI thread.
		virtual void prepare(double sampleRate, int samplesPerBlock) = 0;

		// this function processes the next block of audio samples.
		// it is called from the audio callback or DSP worker threads,
		// so it needs to be as efficient as possible, and cannot block
		// or allocate memory, under any circumstances.
		// this function is called from the audio callback.
		virtual void process(float** channelData, int numChannels, int numSamples) = 0;
	};

	// your plugin needs to implement the createDescriptor and createInstance
	// functions, allocating a struct of type PluginDescriptor (see above)
	// and an instance of your class, implementing the above PluginInterface.
	//
	// the functions need to have C linkage (use extern "C" {...}) so they can
	// be found when the host loads your plugin shared object file. both functions
	// take no arguments and return a pointer to PluginDescriptor/PluginInterface
	// respectively (see below for example). you need to make sure the functions are
	// visible/exported, so use the necessary visibility attribute (see below).
	//
	// these functions are called from the UI thread.
	//
	// NOTE: the host software is responsible for deallocating the descriptor and
	// instance, so you should not deallocate these yourself.
	//
	// extern "C" {
	//	__attribute__ ((visibility("default"))) Percussa::SSP::PluginDescriptor* createDescriptor();
	//	__attribute__ ((visibility("default"))) Percussa::SSP::PluginInterface* createInstance();
    //  __attribute__  ((visibility("default"))) void getApiVersion(unsigned& major, unsigned& minor);
	// }

	static const char* createInstanceName = "createInstance";
	static const char* createDescriptorName = "createDescriptor";
    static const char* getApiVersionName = "getApiVersion";

	typedef PluginInterface* (*InstantiateFun)();
	typedef PluginDescriptor* (*DescriptorFun)();
    typedef void (*VersionFun)(unsigned&, unsigned&);
};
};

#endif

