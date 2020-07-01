
// see header file for license 

#include "PluginProcessor.h"
#include "PluginEditor.h"

RngsEditor::RngsEditor (Rngs& p)
    : AudioProcessorEditor (&p), processor (p)
{
	// set this to true to see the parameter values update 
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons 

	processor.addListener(this);
	showParamValues = false; 

	setSize (1600, 480);

	startTimer(50); 
}

RngsEditor::~RngsEditor()
{
	processor.removeListener(this);
	stopTimer(); 
}

void RngsEditor::timerCallback() 
{ 
	// repaint our own canvas as well 
	repaint(); 
}


void RngsEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	// auto sp = Component::SafePointer<RngsEditor> (this);
	auto fx =
	[=] {
        // if (sp == nullptr) return;
        this->parameterChanged(parameterIndex,newValue);
    };

	MessageManager::callAsync (fx);
}

void 	RngsEditor::audioProcessorChanged (AudioProcessor *) {
	;

}

void RngsEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return; 
	if (index >= Percussa::sspLast) return; 

	// use this if you need to do something special 
	// to process parameter  
	switch(index) { 
		case Percussa::sspEnc1:
			break; 
		case Percussa::sspEnc2: 
			break; 
		case Percussa::sspEnc3:
			break; 
		case Percussa::sspEnc4: 
			break; 
		case Percussa::sspEncSw1: 
			break; 
		case Percussa::sspEncSw2: 
			break; 
		case Percussa::sspEncSw3: 
			break; 
		case Percussa::sspEncSw4: 
			break; 
		case Percussa::sspSw1: 
			break; 
		case Percussa::sspSw2: 
			break; 
		case Percussa::sspSw3: 
			break; 
		case Percussa::sspSw4: 
			break; 
		case Percussa::sspSw5: 
			break; 
		case Percussa::sspSw6: 
			break; 
		case Percussa::sspSw7: 
			break; 
		case Percussa::sspSw8: 
			break; 
		case Percussa::sspSwLeft: 
			break; 
		case Percussa::sspSwRight: 
			break; 
		case Percussa::sspSwUp: 
			break; 
		case Percussa::sspSwDown: 
			break; 
		case Percussa::sspSwShiftL:
			break; 
		case Percussa::sspSwShiftR: 
			break; 
		default:
			break; 
	}
}

const unsigned paramTopY=385-1;
const unsigned paramSpaceY=35;

bool altActive=false;


void displayParameter(Graphics& g, unsigned idx, bool alt, const String& label, float value, const String& unit) {
	unsigned labely=paramTopY+paramSpaceY;
	unsigned valuey=paramTopY+ (2 * paramSpaceY) + 5;
	unsigned spacingX=unsigned(900.0f/8.0f);
	unsigned x= ((idx * 2) + alt) * spacingX;

	String val= String::formatted("%4.2f",value);

	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	if(alt) {
		if(!altActive) 	{
			g.setColour(Colours::grey);
		}
		else {
			g.setColour(Colours::red);
			g.drawText(label,x,labely,spacingX,24, Justification::centred);
		}
		g.drawText(val+unit,x,valuey,spacingX,24, Justification::centred);
	} else {
		if(altActive) 	{
			g.setColour(Colours::grey);
		}
		else {
			g.setColour(Colours::red);
			g.drawText(label,x,labely,spacingX,24, Justification::centred);
		}
		g.drawText(val+unit,x,valuey,spacingX,24, Justification::centred);
	}
}

void displayMainMenu(Graphics&g, const String& label, unsigned idx,  bool active ) {
	const int w=70;
	const int h=45;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 36, Font::plain));
	unsigned x=1530 + 1;
	unsigned y=205 + (idx * h);

	if(!active) {
		g.setColour(Colours::black);
		g.fillRect (x, y, w-2, h-2);	
		g.setColour(Colours::red);
	} else {
		g.setColour(Colours::red);
		g.fillRect (x, y, w-2, h-2);	
		g.setColour(Colours::black);
	}

	g.drawText(label,x,y,w,h, Justification::centred);
}


void displayButton(Graphics&g, const String& label,unsigned r, unsigned c, bool active ) {
	const int w=100;
	const int h=50;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 36, Font::plain));
	unsigned x=900 + (c * w);
	unsigned y=paramTopY + (r * h);

	if(!active) {
		g.setColour(Colours::black);
		g.fillRect (x, y+1, w-2, h-2);	
		g.setColour(Colours::red);
	} else {
		g.setColour(Colours::red);
		g.fillRect (x, y+1, w-2, h-2);	
		g.setColour(Colours::black);
	}

	g.drawText(label,x,y,w,h, Justification::centred);
}


void drawRngs(Graphics& g) {
	unsigned x=1100;
	unsigned y=150;
	unsigned d=100;
	unsigned sp=75;

	g.setColour(Colours::white);
	g.drawEllipse(x,y,d,d,1);
	g.setColour(Colours::green);
	g.drawEllipse(x+sp,y,d,d,1);

	x=x-sp/2;
	y=y+sp;
	g.setColour(Colours::yellow);
	g.drawEllipse(x,y,d,d,1);
	g.setColour(Colours::blue);
	g.drawEllipse(x+sp,y,d,d,1);
	g.setColour(Colours::red);
	g.drawEllipse(x+(2*sp),y,d,d,1);
}


void RngsEditor::paint(Graphics& g)
{
	g.fillAll (Colours::black);

	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText( "rngs resonator", 20, 30 );

	drawRngs(g);
	int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}

	// display 1600x 480
  	// x= left/right (0..1599)
  	// y= top/botton (0..479)
	// title;

	// main menu box
	g.setColour(Colours::grey);
	unsigned y=205-1;
	unsigned x=1530-1;
	unsigned butTopY=paramTopY;
	unsigned butLeftX=900-1;
	g.setColour(Colours::grey);
	g.drawVerticalLine(x,y,butTopY);
	g.drawVerticalLine(1600-1,y,butTopY);
	for(int i=0;i<5;i++) {
		g.drawHorizontalLine(y+i*45,x,1600-1);
	}

	displayMainMenu(g,"G",0,false);
	displayMainMenu(g,"N",1,false);
	displayMainMenu(g,"P",2,false);
	displayMainMenu(g,"R",3,true);


	// button box
	x= butLeftX;
	y= butTopY; 
	g.setColour(Colours::grey);
	g.drawHorizontalLine(y,x,1600-1);
	g.drawHorizontalLine(y+50,x,1600-1);
	g.drawHorizontalLine(480-1,x,1600-1);
	for(int i=0;i<8;i++ ) {
		g.drawVerticalLine(x+ i*100 ,butTopY,480-1);
	}


	displayButton(g,"GR1",0,0,false);
	displayButton(g,"GR2",0,1,false);
	displayButton(g,"GR3",0,2,false);
	displayButton(g,"GR4",0,3,false);
	displayButton(g,"LS" ,0,4,false);
	displayButton(g,"+EN",0,5,false);
	displayButton(g,"RS" ,0,6,false);

	displayButton(g,"GR5",1,0,false);
	displayButton(g,"GR6",1,1,false);
	displayButton(g,"GR7",1,2,false);
	displayButton(g,"GR8",1,3,true);
	displayButton(g,"-PG",1,4,false);
	displayButton(g,"-EN",1,5,false);
	displayButton(g,"+PG",1,6,false);

	// y=butTopY+paramSpaceY;
	// x=butLeftX+20; 
	// g.setColour(Colours::red);
	// g.drawSingleLineText("GR1",x,y);
	// x+=100;
	// g.drawSingleLineText("GR2",x,y);
	// x+=100;
	// g.drawSingleLineText("GR3",x,y);
	// x+=100;
	// g.drawSingleLineText("GR4",x,y);
	// x+=100;
	// g.drawSingleLineText("LS",x,y);
	// x+=100;
	// g.drawSingleLineText("+EN",x,y);
	// x+=100;
	// g.drawSingleLineText("RS",x,y);

	// // row 2
	// y+=50;
	// x=butLeftX+20; 
	// g.drawSingleLineText("GR5",x,y);
	// x+=100;
	// g.drawSingleLineText("GR6",x,y);
	// x+=100;
	// g.drawSingleLineText("GR7",x,y);
	// x+=100;
	// g.drawSingleLineText("GR8",x,y);
	// x+=100;
	// g.drawSingleLineText("-PG",x,y);
	// x+=100;
	// g.drawSingleLineText("-EN",x,y);
	// x+=100;
	// g.drawSingleLineText("+PG",x,y);


	displayParameter(g,0,false,"Coarse",440.0f,"Hz");
	displayParameter(g,0,true,"abc",0,"");
	displayParameter(g,1,false,"Fine",0.0f,"Hz");
	displayParameter(g,1,true,"def",1,"");
	displayParameter(g,2,false,"Cutoff",2100.0f,"Hz");
	displayParameter(g,2,true,"asj",2,"");
	displayParameter(g,3,false,"Q",0.5f,"");
	displayParameter(g,3,true,"zxc",3,"");


	// paint parameter values 
	if (showParamValues) { 

		Font f(Font::getDefaultMonospacedFontName(), 24, Font::plain);
		g.setFont(f);
		g.setColour(Colours::white); 

		// first row 
		int x = 10; 
		for (int i=Percussa::sspEnc1; i<Percussa::sspSw1; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+20); 
			x += 150;  
		}

		// second row 
		x = 10; 
		for (int i=Percussa::sspSw1; i<Percussa::sspSwLeft; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+40); 
			x += 150;  
		}

		// third row 
		x = 10; 
		for (int i=Percussa::sspSwLeft; i<Percussa::sspLast; i++) { 
			g.drawSingleLineText(
				String::formatted("%4.2f", processor.getParameter(i)), 
				x, getHeight()-keepout+60); 
			x += 150;  
		}
	} 
}

	
void RngsEditor::resized()
{
	// int w=getWidth(); 
	int h=getHeight();

	if (showParamValues) { 
		h -= keepout; 
	}


}
