# wsuRacing_wheelDisplay
 Files related to my project for Wazzu Racing - 2020-2021

Revision history (starting with V2->V3):

2->3: 	
	- After speaking with Clint I found a microcontroller with 2 UART ports to get rid of the
		conflicting I/O lines of the CAN BUS and the USB in. 
		
	- Also Changed from the CH340C to the FT232RL. Reasoning is that the CH340C will not have
    		native USB drivers on machines, documentation is more difficult to find and not as detailed,
		and chip availability is very uncertain. In general the compatability issue woudn't be a big
		deal, but I'm probably not going to be the only person to work on this. The FTDI chip (FT232RL)
		will have native windows support, is a more reliable product in terms of supply and support for
		potential issues, and has better documentation. Only downside is that the cost is much higher:
		the CH340C can be found for around 60 to 80 cents per chip where the FT232RL is around $2.50 a
		pop. We're only making a few of these at most so the cost is negligable.
		
	- Switched out the AZ1117 5V to 3V3 reg. and put in place a 12V to 5V and a 12V to 3V3 linear
		regulator. Both of the LD1086 line of regulators.
		
	- In place of the Crystal we have the ASDMB - 16.0000 MHz - LC Oscillator. Clint said the
		issue with crystals is balancing the capacatance of the wires after a reset (if I interpreted
		what he said correctly). To avoid this we use the oscillator which will give a stable output
		the entire time it's powered (and enabled).
		
	- Adjusted the MAX7219 connections to the LEDs. used to be that DIGIT0 (LSB or right hand
		set of LEDs on the wheel display) had 8 LEDS and DIGIT1 had 7 for a total of 15. I made no
		changes to the quantity but I changed it so that DIGIT1 has 8 and DIGIT0 has 7. This is so the
		code is slightly more readable. So we can just fill a byte from left to right and it will 
		directly coorelate to going from left to right on the LED display. Before we had to adjust
		the code to accomidate the fact that we were first filling up the left 7 bits then then next
		8 bits so we had to have a bit at 0 in the middle of the bytes being shifted out. (we are 
		constrained to a byte of data to send)
	
3->4: (haven't made these changes yet just ideas)
		- Switched the oscillator to one that operates on 5V, and automotive temp range -40C to 125C.
		- Switched to a 5V supply CAN BUS transciever and automotive operating temp range.
		- Got rid of the 12V to 3V3 regulator.
		- 
