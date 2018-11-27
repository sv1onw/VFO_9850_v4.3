// Arduino Library for AD9850 frequency synthesiser module, with 125MHz clock
// V1.2 4-9-17 Antony Watts, M0IFA, code update
// Modified from AD9851 library 
// frequency in Hz and cHz
// functions
// void begin(int W_CLK, int FQ_UD, int DATA, int RESET); intialise pins and reset AD9850
// void setFreq(double Hz, double Chz, uint8_t p); set frequency(Hz) and centi-freq(Chz)
//   phase coding, 0-180 in 11.25deg steps 0x00, 0x01, 0x02, 0x04, 0x08, 0x10
// void calibrate(double calHz); change xtal frequency from standard 125MHz to new value
// void down(); power down on, x6 remains on b00000101
// void up(); zero freq, power down off, x6 remains on b00000001 ;or use setfreq()

#ifndef ADS9850_H
#define ADS9850_H

#include <Arduino.h>

#define ADS_XTAL 125000000.0

class ADS9850 {

	public:
		ADS9850();

		void begin(int W_CLK, int FQ_UD, int DATA, int RESET);
		void setFreq(double Hz, double Chz, uint8_t phase);
		void calibrate(double calHz);
                void down();

	private:
		int _W_CLK;
		int _FQ_UD;
		int _DATA;
		int _RESET;

		double _calFreq;

                void update(uint32_t d, uint8_t p);
		void pulse(int _pin);

};

#endif
