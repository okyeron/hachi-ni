# 八x二　aka hachi x ni aka 8x2

八x二 (8x2) is a compact midi controller with 16 potentiometers, 2 buttons, and 8 RGB leds.

MIDI I/O
* TRS MIDI in, out and thru
* Auto sensing TRS MIDI Type-A/Type-B input with LPZW Auto Crossover circuit
* Switch for TRS MIDI Type-A/Type-B output
* USBMIDI

Power
* USB-C
* Lipo Battery (optional)

Hacking
* RP2040 processor
* STEMMA-QT connector (i2c)
* Breakout connections for 3 analog (A0, A1, A2) and 3 digital pins (D4, D11, D12)

--- 

### PlatformIO
Things should be properly configured in the repo to directly compile with PlatformIO

###  Arduino Setup
Install RP2040 Arduino Core - see instructions at https://github.com/earlephilhower/arduino-pico#installing-via-arduino-boards-manager

Add `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json` to the "Additional Boards Manager URLs" field in Arduino's Preferences.

#### Required Libraries
Install from Arduino Library Manager if possible

Adafruit_NeoPixel   // https://github.com/adafruit/Adafruit_NeoPixel
Adafruit_TinyUSB    // https://github.com/adafruit/Adafruit_TinyUSB_Arduino
MIDI			    // https://github.com/FortySevenEffects/arduino_midi_library
ArduinoJson		    // https://arduinojson.org/
CD74HC4067          // https://github.com/waspinator/CD74HC4067
ResponsiveAnalogRead    // https://damienclarke.me/#responsiveanalogread
LittleFS
avdweb_Switch    // https://github.com/avandalen/avdweb_Switch ( not in library manager - needs to be manually installed)

---

# Web Editor 

Use Chrome or Edge. Firefox or Safari will not work.

---

# Hardware notes/links

Pots: 10Kb or B10K (linear, 10Kohms) 
Standard modular "Alpha Single-Gang 9mm Right-Angle PC Mount".
Shaft type will likely depend on your knob choice. D-shaft, T-18 or Round - This is up to you.

Links:  
https://modularaddict.com/9mm-d-shaft-vertical-potentiometers  
https://modularaddict.com/9mm-t18-shaft-vertical-potentiometers  
https://www.thonk.co.uk/shop/alpha-9mm-pots-dshaft/  
https://www.thonk.co.uk/shop/alpha-9mm-pots-vertical-t18/  


Knob options/suggestions:  
https://www.adafruit.com/product/5093  
https://modularaddict.com/re-an-p670-frac-style-16mm-soft-touch-knob-d-shaft  

---

# FAQ

Q: The 8x2 is not showing up as a MIDI device on the computer.  
A: Make sure the power switch is 'on' - which is switched to the right.  

Q: How do I load firmware?  
A: The top plate has two holes for the Reset and BootSel buttons. Reset is closest to the USB jack. To enter the bootloader mode, press both the buttons (using the enclosed toothpicks) and release the BootSel button slightly after Reset. Alternately you can hold BootsSel and turn the power switch off/on.  This should drop into bootloader mode. A RPI-RP2 drive will show up on the computer - drag and drop the firmware UF2 file onto the RPI-RP2 drive. The drive should unmount and 8x2 will reboot into the new firmware.  

Q: I'm stuck in the bootloader RPI-RP2 mode, what do I do?  
A: Unmount the RPI-RP2 drive from the computer and switch the 8x2 off and on again. This should boot into the normal firmware.  

