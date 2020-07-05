#include "PluginProcessor.h"
#include "PluginEditor.h"

static constexpr unsigned paramTopY = 380 - 1;
static constexpr unsigned paramSpaceY = 50;


RngsEditor::RngsEditor (Rngs& p)
	: AudioProcessorEditor (&p), processor (p)
{
	// set this to true to see the parameter values update
	// in the vst plugin GUI (editor) when turning encoders/
	// pushing buttons

	processor.addListener(this);

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
	repaint();
}


void RngsEditor::audioProcessorParameterChanged (AudioProcessor *, int parameterIndex, float newValue) {
	auto fx =
	[ = ] {
		this->parameterChanged(parameterIndex, newValue);
	};

	MessageManager::callAsync (fx);
}

void 	RngsEditor::audioProcessorChanged (AudioProcessor *) {
	;
}

void RngsEditor::parameterChanged (int index, float value) {
	if (index < Percussa::sspFirst) return;
	if (index >= Percussa::sspLast) return;


	// unused so far
	// std::atomic<float>  f_transpose;
	// std::atomic<float>  f_bypass;
	// std::atomic<float>  f_easter_egg;
	// std::atomic<float>  f_chord;
	// std::atomic<float>  f_trig;
	// std::atomic<float>  f_fm;

	// use this if you need to do something special
	// to process parameter
	switch (index) {
	case Percussa::sspEnc1:
		if (altActive_) {
			float v = processor.data().f_position + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor.data().f_position = v;
		} else {
			float v = processor.data().f_pitch + value;
			v = constrain(v, 0.0f, 60.0f);
			processor.data().f_pitch = v;
		}
		break;
	case Percussa::sspEnc2:
		if (altActive_) {
			float v = processor.data().f_polyphony + value;
			v = constrain(v, 0.0f, 3.0f);
			processor.data().f_polyphony = v;

		} else {
			float v = processor.data().f_structure + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor.data().f_structure = v;
		}
		break;
	case Percussa::sspEnc3:
		if (altActive_) {
			float v = processor.data().f_model + value;
			v = constrain(v, 0.0f, 5.0f);
			processor.data().f_model = v;
		} else {
			float v = processor.data().f_brightness + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor.data().f_brightness = v;
		}
		break;
	case Percussa::sspEnc4:
		if (altActive_) {

		} else {
			float v = processor.data().f_damping + value / 100.0f;
			v = constrain(v, 0.0f, 1.0f);
			processor.data().f_damping = v;
		}
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
		if (paramState_[index] != value && !value) {
			processor.data().f_internal_exciter =
			    ! processor.data().f_internal_exciter;
		}
		break;
	case Percussa::sspSw2:
		if (paramState_[index] != value && !value) {
			processor.data().f_internal_strum =
			    ! processor.data().f_internal_strum ;
		}
		break;
	case Percussa::sspSw3:
		if (paramState_[index] != value && !value) {
			processor.data().f_internal_note =
			    ! processor.data().f_internal_note;
		}
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
		if (paramState_[index] != value && !value) {
			altActive_ = ! altActive_;
		}
		break;
	case Percussa::sspSwDown:
		if (paramState_[index] != value && !value) {
			altActive_ = ! altActive_;
		}
		break;
	case Percussa::sspSwShiftL:
		break;
	case Percussa::sspSwShiftR:
		break;
	default:
		break;
	}
	paramState_[index] = value;
}



void RngsEditor::displayParameter(Graphics& g, unsigned idx, bool alt, const String& label, float value, const String& unit) {
	unsigned labely = paramTopY + 5;
	unsigned valuey = labely +  paramSpaceY;
	unsigned spacingX = unsigned(900.0f / 8.0f);
	unsigned x = ((idx * 2) + alt) * spacingX;

	String val = String::formatted("%4.2f", value);

	static constexpr unsigned fh = 36;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), fh, Font::plain));
	if (alt) {
		if (!altActive_) 	{
			g.setColour(Colours::grey);
		}
		else {
			g.setColour(Colours::red);
		}
		g.drawText(label, x, labely, spacingX, fh, Justification::centred);
		g.drawText(val + unit, x, valuey, spacingX, fh, Justification::centred);
	} else {
		if (altActive_) 	{
			g.setColour(Colours::grey);
		}
		else {
			g.setColour(Colours::red);
		}
		g.drawText(label, x, labely, spacingX, fh, Justification::centred);
		g.drawText(val + unit, x, valuey, spacingX, fh, Justification::centred);
	}
}

void RngsEditor::displayMainMenu(Graphics&g, const String& label, unsigned idx,  bool active ) {
	const int w = 70;
	const int h = 45;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 36, Font::plain));
	unsigned x = 1530 + 1;
	unsigned y = 205 + (idx * h);

	if (!active) {
		g.setColour(Colours::black);
		g.fillRect (x, y, w - 2, h - 2);
		g.setColour(Colours::red);
	} else {
		g.setColour(Colours::red);
		g.fillRect (x, y, w - 2, h - 2);
		g.setColour(Colours::black);
	}

	g.drawText(label, x, y, w, h, Justification::centred);
}


void RngsEditor::displayButton(Graphics&g, const String& label, unsigned r, unsigned c, bool active ) {
	const int w = 100;
	const int h = paramSpaceY;
	g.setFont(Font(Font::getDefaultMonospacedFontName(), 36, Font::plain));
	unsigned x = 900 + (c * w);
	unsigned y = paramTopY + (r * h);

	if (!active) {
		g.setColour(Colours::black);
		g.fillRect (x, y + 1, w - 2, h - 2);
		g.setColour(Colours::red);
	} else {
		g.setColour(Colours::red);
		g.fillRect (x, y + 1, w - 2, h - 2);
		g.setColour(Colours::black);
	}

	g.drawText(label, x, y, w, h, Justification::centred);
}


void RngsEditor::drawRngs(Graphics& g) {
	unsigned x = 1100;
	unsigned y = 150;
	unsigned d = 100;
	unsigned sp = 75;

	g.setColour(Colours::white);
	g.drawEllipse(x, y, d, d, 1);
	g.setColour(Colours::green);
	g.drawEllipse(x + sp, y, d, d, 1);

	x = x - sp / 2;
	y = y + sp;
	g.setColour(Colours::yellow);
	g.drawEllipse(x, y, d, d, 1);
	g.setColour(Colours::blue);
	g.drawEllipse(x + sp, y, d, d, 1);
	g.setColour(Colours::red);
	g.drawEllipse(x + (2 * sp), y, d, d, 1);
}

void RngsEditor::paint(Graphics& g) {
	paintBack(g);
	paintParams(g);
}

void RngsEditor::paintBack(Graphics& g)
{
	g.fillAll (Colours::black);

	g.setFont(Font(Font::getDefaultMonospacedFontName(), 24, Font::plain));
	g.setColour(Colours::yellow);
	g.drawSingleLineText( "rngs resonator", 20, 30 );

	drawRngs(g);

	// display 1600x 480
	// x= left/right (0..1599)
	// y= top/botton (0..479)
	// title;

	// main menu box
	g.setColour(Colours::grey);
	unsigned y = 205 - 1;
	unsigned x = 1530 - 1;
	unsigned butTopY = paramTopY;
	unsigned butLeftX = 900 - 1;
	g.setColour(Colours::grey);
	g.drawVerticalLine(x, y, butTopY);
	g.drawVerticalLine(1600 - 1, y, butTopY);
	for (int i = 0; i < 5; i++) {
		g.drawHorizontalLine(y + i * 45, x, 1600 - 1);
	}

	displayMainMenu(g, "G", 0, false);
	displayMainMenu(g, "N", 1, false);
	displayMainMenu(g, "P", 2, false);
	displayMainMenu(g, "R", 3, false);


	// button box
	x = butLeftX;
	y = butTopY;
	g.setColour(Colours::grey);
	g.drawHorizontalLine(y, x, 1600 - 1);
	g.drawHorizontalLine(y + paramSpaceY, x, 1600 - 1);
	g.drawHorizontalLine(480 - 1, x, 1600 - 1);
	for (int i = 0; i < 8; i++ ) {
		g.drawVerticalLine(x + i * 100 , butTopY, 480 - 1);
	}
}

void RngsEditor::paintParams(Graphics& g)
{
	displayButton(g, "Audio", 0, 0, processor.data().f_internal_exciter < 0.5);
	displayButton(g, "Strum", 0, 1, processor.data().f_internal_strum < 0.5);
	displayButton(g, "V/Oct", 0, 2, processor.data().f_internal_note < 0.5);
	displayButton(g, "", 0, 3, false);
	displayButton(g, "" , 0, 4, false);
	displayButton(g, "+EN", 0, 5, false);
	displayButton(g, "" , 0, 6, false);


	displayButton(g, "", 1, 0, false);
	displayButton(g, "", 1, 1, false);
	displayButton(g, "", 1, 2, false);
	displayButton(g, "", 1, 3, false);
	displayButton(g, "", 1, 4, false);
	displayButton(g, "-EN", 1, 5, false);
	displayButton(g, "", 1, 6, false);

	displayParameter(g, 0, false, "Pitch", processor.data().f_pitch, "");
	displayParameter(g, 0, true, "Pos", processor.data().f_position, "");
	displayParameter(g, 1, false, "Struct", processor.data().f_structure, "");
	displayParameter(g, 1, true, "Poly", processor.data().f_polyphony, "");
	displayParameter(g, 2, false, "Bright", processor.data().f_brightness, "");
	displayParameter(g, 2, true, "Model", processor.data().f_model, "");
	displayParameter(g, 3, false, "Damp", processor.data().f_damping, "");
	displayParameter(g, 3, true, "", 0, "");
}


void RngsEditor::resized()
{
	;
}
