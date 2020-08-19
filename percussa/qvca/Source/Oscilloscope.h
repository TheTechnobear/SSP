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
private: 
	const AudioSampleBuffer& _asb; 
	CriticalSection& _lock; 
	int _channel; 
	bool _showInfo; 
	String _info; 
	Colour _infoCol; 
public:
	Oscilloscope(const AudioSampleBuffer& asb, CriticalSection& lock, int ch): 
		_asb(asb), _lock(lock), _channel(ch) 
	{ 
		_showInfo = true; 
		_info = String("Info"); 
		_infoCol = Colours::grey; 
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
};

