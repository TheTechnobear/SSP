// see header file for license 

#include <assert.h> 
#include "Oscilloscope.h"

void Oscilloscope::paint(Graphics &g)
{
	const ScopedLock sl(_lock); 

	float val=0.00f;
	float w=(float)getWidth();
	float h=(float)getHeight();
	float barWidth=1;

	//g.fillAll(Colours::black);
	Font f(Font::getDefaultMonospacedFontName(), 0.1f*h, Font::plain);
	g.setFont(f);
	g.setColour(Colours::red);

	// draw border 
	g.setColour(Colours::grey); 
	g.drawRect(0.0f,
		0.0f,
		w,
		h); 

	if (_showInfo) { 
		g.setColour(_infoCol); 
		g.drawMultiLineText(_info, 10, h-30, w); 
	}

	if (_channel < 0) return; 
	if (_channel >= _asb.getNumChannels()) return; 

	float step = _asb.getNumSamples()/w; 
	float phase = 0.00f;

	// draw waveform samples as pixels/short lines
	for (int i=0; i<(int)w; i++) {

		val = _asb.getSample(_channel, (int)phase) * 0.75f;

		if (val != val) val = 0.00f; 
		if (val < -1.00f) val = -1.00f; 
		if (val > 1.00f) val = 1.00f; 

		val = 1.0f - (val+1.0f)*0.5f;
		val = val * h;

		if (val>h) val=h;
		else if (val<0.0f) val=0.0f;

		g.drawLine(i*barWidth, val, i*barWidth+barWidth, val, 2.0f);

		phase += step; 
	}
}

