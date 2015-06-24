# Cymatic Lighting
Open source Cymatic Lighting system for Deaf &amp; Hard-of-Hearing to see music &amp; alerts using Arduino &amp; digital LEDs.

*Brought to you by www.cymaspace.org.*

Video Primer: https://youtu.be/OnrxPVjEhr8

Solution submitted to the AT&T Connectability Challenge: http://connectability.challengepost.com/.
*Our solution requires some additional hardware & software components.*

**Hardware:**
* Arduino UNO (or other compatible microcontroller)
* MSGEQ7 Spectrum Audio Shield: https://www.sparkfun.com/products/13116
* Neopixel (WS2811 or WS2812B) Individually Addressable LED Strip: http://www.adafruit.com/products/1376

**Software:**
* Adafruit Neopixel library: https://github.com/adafruit/Adafruit_NeoPixel
* Arduino IDE: https://www.arduino.cc/en/Main/Software
* CymaSpace Cymatic Lighting Software Sketch:
https://github.com/CymaSpace/Cymatic-Lighting

**Step 1:**
Connect Sparkfun Spectrum Audio Shield to the Arduino (it will fit exactly onto an Arduino Uno).

**Step 2:**
To quickly get up and running follow the steps in the Adafruit Neopixel Uberguide: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library

Connect the +5V input on the LED strip to the + (positive) terminal on the Arduino/Spectrum Shield, DIN to digital pin 5 on the Arduino/Spectrum Shield, and – (minus or GND) on the strip must connect to a GND pin on the Arduino/Spectrum Shield (there are usually several — any will do).

**Step 3:**
Install Arduino IDE & Arduino NeoPixel Library (again reference Adafruit Neopixel Uberguide for more detailed instructions).

**Step 4:**
Download our software sketch '/Cymatic-Lighting' from Github and unzip the contents into your Arduino Sketches folder.

**Step 5:**
Upload sketch from Arduino software IDE to the Arduino hardware (make sure USB cable is connected to computer if not already!).

**Step 6:**
Plug in a standard 3.5mm audio cable into either audio jack on the Spectrum Audio Shield and the other end into a sound-source (MP3 Player, Phone, “Handimic” Microphone with line level output etc).

**Step 7:**
Play music or make sound and enjoy the visualization on the LED strip.
