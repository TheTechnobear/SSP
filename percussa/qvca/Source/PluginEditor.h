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

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "Oscilloscope.h" 
#include "Percussa.h" 

class QVCAEditor: 
	public AudioProcessorEditor, public Timer, 
	public Percussa::SSP::PluginEditorInterface  
{
public:
	static const int nScopes = 8; 
	static const int keepout = 100; 

	QVCAEditor (QVCA&);
	~QVCAEditor();

	void paint (Graphics&) override;
	void resized() override;
	void timerCallback() override; 

	//////////////////////////////////////////////////////////////////////
	// SSP specific plugin editor interface overrides (see Percussa.h) 

	void renderToImage(unsigned char* buffer, int width, int height) override {
	
		// if we don't have an image yet, allocate one, and set the 
		// bounds and visibility for the editor component. 
		if (!img) { 
			// unfortunately, juce Image class cannot simply take 
			// a pointer to a buffer, so this will allocate a new 
			// image buffer. note that the image format used 
			// internally is BGRA.   
			img = new Image(Image::ARGB, width, height, false);
			setBounds(Rectangle<int>(0, 0, width, height));  
			setOpaque(true); 
			setVisible(true); 
		}
		// draw editor component onto image. 
		Graphics g (*img);
		paintEntireComponent(g, true); 
		// copy from Image internal buffer to host buffer. 
		// if the juce Image class could take the pointer passed
		// into renderToImage(), the above allocation and the copy 
		// below would not be necessary. 
		Image::BitmapData bitmap(*img, Image::BitmapData::readOnly); 
		memcpy(buffer, bitmap.data, width*height*4); 
	}

private:
	QVCA& processor;
	OwnedArray<Oscilloscope> in; 
	OwnedArray<Oscilloscope> out; 
	ScopedPointer<Image> img; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QVCAEditor)
};

#endif 

