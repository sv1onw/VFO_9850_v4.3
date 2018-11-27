// Arduino Library for AD9850 frequency synthesiser module, with 125MHz clock
// V1.2 4-9-17 Antony Watts, M0IFA, pulser code update
// frequency in Hz and cHz
// W_CLK, FQ_UD, DATA, RESET to (e.g.) D11, D10, D9 & D8
// functions
// void begin(int W_CLK, int FQ_UD, int DATA, int RESET); reset, serial, down
// void setFreq(double Hz, double Chz, uint8_t p); set f(Hz) and cHz(Chz),
// phase in steps 0x00, 0x01, 0x02, 0x04, 0x08, 0x10 (11.5, 22.5, 45. 90, 180deg)
// void calibrate(double calHz); correct xtal frequency from standard 125MHz 
// void down(); freq zero, power down (power up with setFreq())

#include "Arduino.h"
#include "ADS9850.h"

// constructor
ADS9850::ADS9850() {

}

// init calFreq, pins, reset & serial mode
void ADS9850::begin(int W_CLK, int FQ_UD, int DATA, int RESET) {
	_W_CLK = W_CLK;
	_FQ_UD = FQ_UD;
	_DATA = DATA;
	_RESET = RESET;

	_calFreq = ADS_XTAL;

	pinMode(_W_CLK, OUTPUT); // outputs default to LOW
	pinMode(_FQ_UD, OUTPUT);
	pinMode(_DATA, OUTPUT);
	pinMode(_RESET, OUTPUT);

	pulse(_RESET); // reset, parallel mode, ptr to W0

        pulse(_W_CLK); // switch to serial mode, xxxxx011 wired on d2-d0 

        down(); // init/clear freq & phase registers, power down
}

// write 40 bits, 4x8 freq + 8 control & phase
void ADS9850::update(uint32_t fW, uint8_t cP) {
    for (int i=0; i <4 ; i++, fW >>= 8) {
       shiftOut(_DATA, _W_CLK, LSBFIRST, fW); // output 32 freq bits
    }
    shiftOut(_DATA, _W_CLK, LSBFIRST, cP); // output 8 control & phase bits

    pulse(_FQ_UD);
}

// calculate 32 freq bits, convert double to to uint32_t, set PD off & x6, update
void ADS9850::setFreq(double f, double cf, uint8_t p) {
     uint32_t delta;

	delta = (uint32_t)((f + cf/100.0) * 4294967296.0 / _calFreq);
	p = p << 3; // PD off = ppppp000
//  bitSet(p, 0); // ppppp001 set x6 REFCLK on <==== ATT.:THIS SHOULD NOT BE SET FOR AD9850, Do not uncomment!
	update(delta, p);
}

// clear freq & phase registers
void ADS9850::down() {
        update(0x00000000, 0x04); // freq zero, ppppp100 set PD on, x6 REFCLK off <==== FINE FOR AD9850
}


// pulse a pin LOW-HIGH-LOW
void ADS9850::pulse(int _pin) {
        digitalWrite(_pin, LOW);
	    digitalWrite(_pin, HIGH);	
        digitalWrite(_pin, LOW);
}

// load a new value for _calFreq
void ADS9850::calibrate(double calXtal) {
	_calFreq = calXtal;
}
