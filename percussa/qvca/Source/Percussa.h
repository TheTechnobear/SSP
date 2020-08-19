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

#ifndef PERCUSSA_H_INCLUDED
#define PERCUSSA_H_INCLUDED

namespace Percussa {

	static const int sspFirst = 0; 

	// Parameters representing the raw data from the ssp's hardware user interface. 
	// The ssp's software will send these parameters to your VST plugin when 
	// they change. These special parameters start at 0. You can define your own 
	// VST parameters if you wish, starting at sspLast. You should do this in your 
	// own plugin header file and not here.  

	enum sspParams { 

		// encoder pulse values: values centered around 0.5, with values smaller
		// indicating turning left, and values larger indicating turning right. 
		// as a vst plugin developer you can decide how to process these values, 
		// how to smooth them, apply acceleration or some other physics model...
		// you will get multiple setParameter calls as the pulses are detected
		// by the hardware.  
		sspEnc1 = sspFirst, 
		sspEnc2, 
		sspEnc3,
		sspEnc4, 

		// encoder push buttons: parameter value 0 or 1 
		sspEncSw1, 
		sspEncSw2, 
		sspEncSw3, 
		sspEncSw4, 

		// soft key group (8 buttons): parameter value 0 or 1 
		sspSw1, 
		sspSw2, 
		sspSw3, 
		sspSw4, 
		sspSw5, 
		sspSw6, 
		sspSw7, 
		sspSw8, 

		// cursor keys + shift L/R: parameter value 0 or 1 
		sspSwLeft, 
		sspSwRight, 
		sspSwUp, 
		sspSwDown, 
		sspSwShiftL,
		sspSwShiftR, 

		// VST module host output connected flag: parameter value 0 or 1 
		sspOutEn1, 
		sspOutEn2, 
		sspOutEn3, 
		sspOutEn4, 
		sspOutEn5, 
		sspOutEn6, 
		sspOutEn7, 
		sspOutEn8, 
		sspOutEn9, 
		sspOutEn10, 
		sspOutEn11, 
		sspOutEn12, 
		sspOutEn13, 
		sspOutEn14, 
		sspOutEn15, 
		sspOutEn16, 
		sspOutEn17, 
		sspOutEn18, 
		sspOutEn19, 
		sspOutEn20, 
		sspOutEn21, 
		sspOutEn22, 
		sspOutEn23, 
		sspOutEn24, 

		// VST module host input connected flag: parameter value 0 or 1 
		sspInEn1, 
		sspInEn2, 
		sspInEn3, 
		sspInEn4, 
		sspInEn5, 
		sspInEn6, 
		sspInEn7, 
		sspInEn8, 
		sspInEn9, 
		sspInEn10, 
		sspInEn11, 
		sspInEn12, 
		sspInEn13, 
		sspInEn14, 
		sspInEn15, 
		sspInEn16, 
		sspInEn17, 
		sspInEn18, 
		sspInEn19, 
		sspInEn20, 
		sspInEn21, 
		sspInEn22, 
		sspInEn23, 
		sspInEn24, 

		sspLast, 
	}; 
}; 

#endif

