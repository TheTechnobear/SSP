/*
	Copyright (c) 2018 - Bert Schiettecatte, Noisetron LLC. 

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

class QVCAEditor  : public AudioProcessorEditor, public Timer 
{
public:
	static const int nScopes = 8; 
	static const int keepout = 100; 

	QVCAEditor (QVCA&);
	~QVCAEditor();

	void paint (Graphics&) override;
	void resized() override;
	void timerCallback(); 

private:
	QVCA& processor;
	bool showParamValues; 

	OwnedArray<Oscilloscope> in; 
	OwnedArray<Oscilloscope> out; 

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QVCAEditor)
};

#endif  
