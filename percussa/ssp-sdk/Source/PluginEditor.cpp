// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"

QVCAEditor::QVCAEditor (QVCA& p)
    : AudioProcessorEditor (&p), processor (p)
{
	for (int i=0; i<nScopes; i++) { 
		Oscilloscope* o = new Oscilloscope(
			processor.inBuffer, processor.lock, i);  
		o->setInfo(String("In")+String(i+1)); 
		o->setInfoCol(Colours::white); 
		addAndMakeVisible(o); 
		in.add(o); 
	}

	for (int i=0; i<nScopes; i++) { 
		Oscilloscope* o = new Oscilloscope(
			processor.outBuffer, processor.lock, i); 
		o->setInfoCol(Colours::red); 
		addAndMakeVisible(o); 
		out.add(o); 
	}

	out[0]->setInfo(String("Out1=In1*In2")); 
	out[1]->setInfo(String("Out2=-In1*In2")); 
	out[2]->setInfo(String("Out3=In3*In4")); 
	out[3]->setInfo(String("Out4=-In3*In4")); 
	out[4]->setInfo(String("Out5=In5*In6")); 
	out[5]->setInfo(String("Out6=-In5*In6")); 
	out[6]->setInfo(String("Out7=In7*In8")); 
	out[7]->setInfo(String("Out8=-In7*In8")); 

	setSize (1600, 480);
	startTimer(50); 
}

QVCAEditor::~QVCAEditor()
{
}

void QVCAEditor::timerCallback() 
{ 
	for (int i=0; i<nScopes; i++) { 
		in[i]->repaint(); 
	}

	for (int i=0; i<nScopes; i++) { 
		out[i]->repaint(); 
	}

	// repaint our own canvas as well 
	repaint(); 
}

void QVCAEditor::paint(Graphics& g)
{
	g.fillAll (Colours::black);

	// show plugin name in the background 
	Font f(Font::getDefaultMonospacedFontName(), 
		0.9f*getHeight(), Font::plain);
	g.setFont(f);
	g.setColour(Colour(30, 30, 30));

	int w=getWidth(); 
	int h=getHeight();

	g.drawFittedText(
		JucePlugin_Name,
		0, 0,
		w, h,
		Justification::centred,
		1);
}

void QVCAEditor::resized()
{
	int w=getWidth(); 
	int h=getHeight();

	int scopeWidth=w/nScopes; 
	int scopeHeight=h/2; 

	for (int col=0; col<in.size(); col++) { 

		Oscilloscope* o = in[col]; 
		assert(o); 

		o->setBounds(
			col*scopeWidth, 
			0, 
			scopeWidth, 
			scopeHeight); 
	}	

	for (int col=0; col<out.size(); col++) { 
	
		Oscilloscope* o = out[col]; 
		assert(o); 

		o->setBounds(
			col*scopeWidth, 
			scopeHeight, 
			scopeWidth, 
			scopeHeight); 
	}	
}
