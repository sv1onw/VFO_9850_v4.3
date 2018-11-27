//DDS_VFO General purpose VFO 10Hz-42MHz, OLED 128x64 SH1106 no-name
// V4.3 24-11-18 
// Code written by Antony Watts, M0IFA
// Adaptation FOR AD9850 DDS by SV1ONW
// Addition of Frequency Limits by SV1ONW

// HEADERS & LIBRARIES
#include "Oled.h"
#include "ADS9850.h"
#include "Rotary.h"

// CONNECTIONS
// AD9850
#define RESET 8     
#define DATA 9
#define FQ_UD 10
#define W_CLK 11
// ENCODER
#define CLK 2  //Pin A     
#define DT 3   //Pin B
#define SW 4   //Push Button on Rotary Encoder 
// OLED
// Remember OLED goes to A4(SDA) and A5(SCK). No need to define anything here!

// xtal calibration

// Change value from 0 to +/- the Crystal Deviation from its nominal 125000000 MHz, so that start frequency is correct
#define CORRECTION 0  // needs to be changed!!!

// OBJECTS
ADS9850 ads;

// Rotary Encoder
Rotary enc = Rotary(DT, CLK);

// GLOBAL VARIABLES
// initial freq
volatile double freqHz = 14285000; // (Hz) start frequency 14.285MHz
volatile double freqChz = 0;      // (cHz) additional 0cHz
volatile double freqStep = 1000;  // (Hz) init 1kHz freqStep
uint8_t phase = 0x00;             // init phase

//_________________________Enter frequency limits (KHz) below:_________________
const unsigned      F_min = 10;        // Lower Frequency Limit 10 Hz
const unsigned long F_max = 42000000;  // Upper Frequency Limit 42 MHz
//Can be changed to suit your needs, provided you stay within the AD9850 frequency limits 
//e.g. F_min = 14000000; F_max = 14350000 to cover 20 meter Band only

// freq change flag
volatile bool freqChange;



// SETUP
void setup() {
  // encoder, button pins
  pinMode(DT, INPUT_PULLUP);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);

  
  // freq change flag
  freqChange = false;

  // setup interrupts for tuning
  PCICR |= (1 << PCIE2);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19);
  sei();

  // oled init, sets I2C addr to 0x3C
  oled.begin();

  // init ads
  ads.begin(W_CLK, FQ_UD, DATA, RESET);

  // calibrate to xtal actual frequency
   ads.calibrate(125000000 + CORRECTION);  //125000000 Nominal AD9850 Crystal Value

  ads.setFreq(freqHz, freqChz, phase);

 
  dispUpdate();
}

// LOOP
// check button & freq tune, update display
void loop() {
  if (button()) {
    if (freqStep == 1000000) freqStep = 10; // back to 10Hz
    else freqStep = freqStep * 10; // or increase by x10
    dispUpdate();
  }

  if (freqChange) {
    // freq updated
    ads.setFreq(freqHz, freqChz, phase);
    freqChange = false;
    dispUpdate();
  }
}

// BUTTON
// button pressed?
bool button() {
  if (digitalRead(SW) == LOW) { // button pressed?
    while (!digitalRead(SW)); // wait for release
    return true;
  }
  else {
    return false;
  }
}

// INTERRUPT
// ISR - encoder interrupt service routine
ISR(PCINT2_vect) {
  unsigned char result;

  result = enc.process();
  if (result == DIR_CW ) {
    freqHz += freqStep;
    if (freqHz > F_max)
  {
    freqHz = F_max;
  }
    freqChange = true;
  }
  else if (result == DIR_CCW) {
    freqHz -= freqStep;
    if (freqHz < F_min)
  {
    freqHz = F_min;
  }
    freqChange = true;
  }
 
}


// PICTURE LOOP
// picture loop, display init data
void dispUpdate() {
  oled.firstPage();
  do {
    dispMsg(6, 0, "0-42 MHz DDS VFO");
    dispFreq(15, 25, freqHz, freqChz, 2);
    dispMsg(15, 50, "<--");
    dispStep(45, 50, freqStep);
    dispMsg(95, 50, "-->");

  } while ( oled.nextPage() );
}
