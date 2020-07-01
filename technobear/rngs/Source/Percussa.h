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
namespace Percussa {

	static const int sspFirst = 0; 

	// Parameters representing the raw data from the 
	// ssp's hardware user interface. The ssp's software
	// will send these parameters to your VST plugin when 
	// they change. These special parameters start at 0. 

	enum sspParams { 

		// encoder values 
		// parameter value -1 or +1 
		// turn right is +1 and turn left is -1 
		// you will get multiple calls to setParameter 
		// these are "pulses" which you can use to 
		// increase or decrease your internal variables. 
		sspEnc1 = sspFirst, 
		sspEnc2, 
		sspEnc3,
		sspEnc4, 

		// encoder push buttons 
		// parameter value 0 or 1 
		sspEncSw1, 
		sspEncSw2, 
		sspEncSw3, 
		sspEncSw4, 

		// soft key group (8 buttons) 
		// parameter value 0 or 1 
		sspSw1, 
		sspSw2, 
		sspSw3, 
		sspSw4, 
		sspSw5, 
		sspSw6, 
		sspSw7, 
		sspSw8, 

		// cursor keys + shift L/R 
		// parameter value 0 or 1 
		sspSwLeft, 
		sspSwRight, 
		sspSwUp, 
		sspSwDown, 
		sspSwShiftL,
		sspSwShiftR, 

		sspLast, 
	}; 

}; 


