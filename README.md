# 八x二　aka "hachi-ni" or "8x2"

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

Dimensions: 10in x 2.52in (245 x 64 mm)  

--- 
## How to load firmware

Firmware is available on the [Release pages](https://github.com/okyeron/hachi-ni/releases) as a UF2 file.  

[UF2 info from adafruit...](https://learn.adafruit.com/getting-started-with-raspberry-pi-pico-circuitpython)  
> Inside the RP2040 is a 'permanent ROM' USB UF2 bootloader. What that means is when you want to program new firmware, you can hold down the BOOTSEL button while plugging it into USB (or pulling down the RUN/Reset pin to ground) and it will appear as a USB disk drive you can drag the firmware onto. Folks who have been using Adafruit products will find this very familiar - we use the technique on all our native-USB boards. Just note you don't double-click reset, instead hold down BOOTSEL during boot to enter the bootloader!  

On __hachi-ni__, you have both a BOOTSEL and a RESET button. These are under the holes on the right side of the top panel. The hole closest to the USB is RESET. The hole farther away is BOOTSEL. To access the buttons, insert the bespoke bamboo switch actuators (toothpicks) included with your kit.  

To enter the UF2 bootloader mode, hold BOOTSEL then press and release RESET and then release BOOTSEL. Alternately you can hold BOOTSEL and then toggle the power switch from off to on then release BOOTSEL. This should drop into UF2 bootloader mode.  

A RPI-RP2 drive will show up on the computer - drag and drop the firmware UF2 file onto the RPI-RP2 drive. After the UF2 is dragged onto the USB disk drive, the device should automatically restart with the new firmware.    

--- 
## How to compile the firmware yourself

### PlatformIO
Things should be properly configured in the repo to directly compile with PlatformIO

###  Arduino Setup
Install RP2040 Arduino Core - see instructions at https://github.com/earlephilhower/arduino-pico#installing-via-arduino-boards-manager

Add `https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json` to the "Additional Boards Manager URLs" field in Arduino's Preferences.


### Arduino IDE setup:
 * Select Tools / Board: Raspberry Pi Pico  
 * Select Tools / Flash Size: 2MB (Sketch: 1MB / FS: 1MB)  
 * Select Tools / USB Stack: Adafruit TinyUSB  
 * Optional - Tools / Debug Level: Core  
 * Optional - Tools / Debug Port: Serial  


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

# Hachi x Ni Configurator (Web-based editor) 

Visit: https://okyeron.github.io/8x2  

Use Chrome or Edge. Firefox or Safari will not work.

Or...  

There's also a bare-bones version you can download this repo - load the file   `webconfig/index.html` locally in your browser.  

---

# Hardware notes/links

Pots: use 10Kb or B10K (linear, 10Kohms)  
Standard modular "Alpha Single-Gang 9mm PC Mount" (aka "vertical") are what you want.  
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

[Lipo Battery Information](<battery.md>)

---

# FAQ

Q: The 8x2 is not showing up as a MIDI device on the computer.  
A: Make sure the power switch is 'on' - which is switched to the right.  

Q: How do I load firmware?  
A: [See above](https://github.com/okyeron/hachi-ni#how-to-load-firmware)  

Q: I'm stuck in the bootloader RPI-RP2 mode, what do I do?  
A: Unmount the RPI-RP2 drive from the computer and switch the 8x2 off and on again. This should boot into the normal firmware.  

