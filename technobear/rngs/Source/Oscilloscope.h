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

#pragma once

#include <assert.h>
#include "../JuceLibraryCode/JuceHeader.h"

class Oscilloscope: public Component
{
public:
	Oscilloscope()   
	{
		reset(); 
	}

	~Oscilloscope()
	{
	}

	void reset() { 
		_lock = 0;
		_asb = 0; 
		_channel = 0; 
		_showInfo = true; 
		_info = String("Info"); 
		_infoCol = Colours::grey; 
	}

	void start(AudioSampleBuffer& asb, 
		CriticalSection& lock, 
		int ch) { 

		_asb = &asb; 
		_lock = &lock; 
		_channel = ch; 
	}

	void setInfo(const String& info) { 
		_info = info; 
		repaint(); 
	}

	void setInfoCol(const Colour& col) { 
		_infoCol = col; 
		repaint(); 
	}

	void setShowInfo(bool show) { 
		_showInfo = show; 
		repaint(); 
	}

private:
	void paint(Graphics &g);
	juce_UseDebuggingNewOperator

private: 
	CriticalSection* _lock; 
	AudioSampleBuffer* _asb; 
	int _channel; 
	bool _showInfo; 
	String _info; 
	Colour _infoCol; 
};

