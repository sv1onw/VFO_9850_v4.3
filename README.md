# VFO_9850_v4.3
Arduino controlled Signal Generator with AD9850 and OLED 128x64 display

This is a sketch to control an AD9850 oem board with an Arduino Nano or Uno. 
It uses an OLED 128x64 monochrome display and has very simple functions such as Frequency Change, Frequency Step Control and predefined Frequency Limits. 

It is the work of Antony Watts, M0IFA and has been slightly modified to cover my needs as a Portable Sine/Square Generator from 10 Hz to 42 MHz. 

Schematic is in file dds1_bb.png

To compile the sketch without producing errors, you will need to have Oliver Kraus U8g2 very fine library installed in your IDE and copy all Rotary, Oled and ADS9850 files from this repository here, inside the folder where the VFO_9850_v4.3.ino will reside. Please do that before verifying or uploading the sketch.

Read carefully the comments within the sketch for changing the Frequency Limits and how to offset the AD9850 external Crystal nominal frequency of 125.000000 MHz to the actual frequency of your AD9850 board's actual Crystal value (usually +/- 2 to 3 KHz). You can check the accuracy of your produced Frequency sine waveform by connecting the output (see schematic) to a Frequency Counter or a Shotwave Receiver in AM mode. Initial sketch has a "0" offset Crystal correction value.

The sketch is set initially set to produce a Frequency range from 10 Hz to 42 MHz, Frequency Steps of 10 Hz, 100 Hz, 1KHz, 10 KHz, 100 KHz, 1 MHz and has a start up Frequency of 14.285000 MHz and a Step of 1 KHz.
The rotary encoder is a plain vanila stock type with a built-in push button for the step.






