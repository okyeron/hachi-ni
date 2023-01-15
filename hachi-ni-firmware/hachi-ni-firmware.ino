/*
// Hachi x Ni (8x2) MIDI Controller
// v0.5
// by Steven Noreyko
//

* Arduino IDE setup:
 * - Select Tools / Board: Rasberry Pi Pico
 * - Select Tools / Flash Size: 2MB (Sketch: 1MB / FS: 1MB)
 * - Select Tools / USB Stack: Adafruit TinyUSB
 * - Optional - Tools / Debug Level: Core
 * - Optional - Tools / Debug Port: Serial

*/

#include <Arduino.h>
#include <Adafruit_NeoPixel.h> 
#include <Adafruit_TinyUSB.h>	// https://github.com/adafruit/Adafruit_TinyUSB_Arduino
#include <MIDI.h>				// https://github.com/FortySevenEffects/arduino_midi_library
#include <ArduinoJson.h>		// https://arduinojson.org/
#include <CD74HC4067.h>
#include <ResponsiveAnalogRead.h>
#include <LittleFS.h>
#include <avdweb_Switch.h>

#include "config.h"


//Switch(byte _pin, byte PinMode=INPUT_PULLUP, bool polarity=LOW, int debouncePeriod=50, int longPressPeriod=300, int doubleClickPeriod=250, int deglitchPeriod=10);
Switch leftButton = Switch(buttons[0], INPUT_PULLUP, LOW, 1, 450, 350);
Switch rightButton = Switch(buttons[1], INPUT_PULLUP, LOW, 1, 450, 350);


// USB MIDI object
Adafruit_USBD_MIDI usb_midi;
// Create USB and Hardware MIDI interfaces
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, USBMIDI);
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, HWMIDI);

// NEOPIXELs
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(9, NEOPIXPIN, NEO_GRB + NEO_KHZ800);

// ResponsiveAnalogRead
ResponsiveAnalogRead *analog[channelCount];

// MUX SETUP
CD74HC4067 my_mux(mux1, mux2, mux3, mux4);  // create a new CD74HC4067 object with its four control pins

//TR-09 - tune/levels
// int usbCCs[16] = {20,28,46,49,52,59,61,80, 24,29,48,51,54,60,63,82};
// int trsCCs[16] = {20,28,46,49,52,59,61,80, 24,29,48,51,54,60,63,82};

//	int usbCCs[16] = {20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
//	int trsCCs[16] = {20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35};
//	int trsCCs[16] = {14,16,19,20,21,24,26,28,42,43,44,53,54,30,31,33}; // NTS-1 (ish)

// CC Banks
class bank {
	public:
    	int usbChannel[16];
	    int trsChannel[16];
	    int usbCC[16];
    	int trsCC[16];

		bank() {
			fillRange(0,0);
			fillChan(0,0);
		}
		void fillChan(int usbChan,int trsChan) {
			for(int i=0; i < 16; i++) {
				usbChannel[i] = usbChan;
				trsChannel[i] = trsChan;
			}
		}
		void fillRange(int usbStart, int trsStart) {
			for(int i=0; i < 16; i++) {
				usbCC[i] = i + usbStart;
				trsCC[i] = i + trsStart;
			}
		}
};

// ccBanks[x].trsCC[y])
// ccBanks[x].trsChannel[y])
bank ccBanks[16];
int activeBank = 0;


// Button callbacks
void leftButtonCallback(void* s) {
//   Serial.print("Left: ");
//   Serial.println((char*)s);
	activeBank--;
  	activeBank = constrain(activeBank, 0, numBanks-1);
	pixelsOff();
	pixels.setPixelColor(activeBank+1, 128, 0, 0); // bank pixels are 1-8 not 0-7
	pixels.show();
	Serial.println(activeBank);
}

void rightButtonCallback(void* s) {
//   Serial.print("Right: ");
//   Serial.println((char*)s);
	activeBank++;
  	activeBank = constrain(activeBank, 0, numBanks-1);
	pixelsOff();
	pixels.setPixelColor(activeBank+1, 128, 0, 0);
	pixels.show();
	Serial.println(activeBank);
}


// USB DEVICE INFO 
char mfgstr[32] = "denki-oto";
char prodstr[32] = "hachi-ni";

// *** SETUP ****
void setup() {
	TinyUSBDevice.setManufacturerDescriptor(mfgstr);
	TinyUSBDevice.setProductDescriptor(prodstr);

	// pinMode(buttons[0], INPUT_PULLUP);
	// pinMode(buttons[1], INPUT_PULLUP);
	pinMode(mux_common_pin, INPUT);
	pinMode(TXLED, OUTPUT); // TX
	pinMode(REDLED, OUTPUT);	// RED LED
	pinMode(NEOPWRPIN, OUTPUT); // NEOPWR Pin
	digitalWrite(NEOPWRPIN, HIGH);	// Turn NEOPWR ON


	LittleFS.begin();

// 	Serial1.setRX(midi_rx_pin);
//  Serial1.setTX(midi_tx_pin);
    
	// Initialize MIDI, and listen to all MIDI channels
	USBMIDI.begin(MIDI_CHANNEL_OMNI);
	HWMIDI.begin(MIDI_CHANNEL_OMNI);
	USBMIDI.turnThruOff();
	HWMIDI.turnThruOff();

	// handlers for receiving MIDI messages.
	USBMIDI.setHandleNoteOn(sendNoteOn);
	USBMIDI.setHandleNoteOff(sendNoteOff);
	HWMIDI.setHandleNoteOn(sendNoteOn);
	HWMIDI.setHandleNoteOff(sendNoteOff);
	USBMIDI.setHandleControlChange(sendControlChange);
	HWMIDI.setHandleControlChange(sendControlChange);
	USBMIDI.setHandleProgramChange(onProgramChange);
	HWMIDI.setHandleProgramChange(onProgramChange);
	USBMIDI.setHandleSystemExclusive(OnSysEx);


	Serial.begin(115200);
	
	for (int i = 0; i < channelCount; i++)
	{
		analog[i] = new ResponsiveAnalogRead(mux_common_pin, true);
	}
	
	// wait until device mounted
//	   while( !TinyUSBDevice.mounted() ) delay(1);
	if (!TinyUSBDevice.mounted()){
		delay(1000);
	}

	Serial.println("MIDI Test");
	
	// Button setup
	// leftButton.setPushedCallback(&leftButtonCallback, (void*)"turned on");
	// leftButton.setReleasedCallback(&leftButtonCallback, (void*)"turned off");
  	leftButton.setSingleClickCallback(&leftButtonCallback, (void*)"left single click");

	// rightButton.setLongPressCallback(&rightButtonCallback, (void*)"long press");
  	// rightButton.setDoubleClickCallback(&rightButtonCallback, (void*)"double click");
  	rightButton.setSingleClickCallback(&rightButtonCallback, (void*)"right single click");


	if (!config_read()){
		initCCbanks();
		config_write();
	}
    config_load(0);
    

	pixels.begin();				  // Start the NeoPixel object
	pixels.clear();				  // Set NeoPixel color to black (0,0,0)
	pixels.setBrightness(50);	  // Affects all subsequent settings

	rainbow(2); 
	pixels.setPixelColor(0, 0, 20, 20);
	pixels.show();

}	
// END SETUP

// *** MAIN LOOP ****
void loop()
{
	activity = false;
	// bool button0Temp = digitalRead(buttons[0]);
	// bool button1Temp = digitalRead(buttons[1]);
	leftButton.poll();
	rightButton.poll();
	

	for (int i = 0; i < 16; i++) {
		int temp;
		my_mux.channel(muxMapping[i]);

		temp = analogRead(mux_common_pin); // mux goes into g_common_pin // sensorValue = analogRead(g_common_pin);
		analog[i]->update(temp);
		if(analog[i]->hasChanged()) {
			temp = analog[i]->getValue();
			temp = constrain(temp, faderMin, faderMax);
			temp = map(temp, faderMin, faderMax, 1024, 0); // flip the value for backwards pots
			
			shiftyTemp = temp >> 3;
			int tempR = 0;
			int tempB = 0;
			int tempG = 0; 
			if (i < 8) {
				tempG = shiftyTemp*2;
				pixels.setPixelColor(i+1, tempR, tempG, tempB);
			} else {
				tempG = shiftyTemp; 
				tempB = shiftyTemp*2;
				pixels.setPixelColor(i-7, tempR, tempG, tempB);
			}
			
			
			// send the message over USB and physical MIDI
			USBMIDI.sendControlChange(ccBanks[activeBank].usbCC[i], shiftyTemp, ccBanks[activeBank].usbChannel[i]+1);
			HWMIDI.sendControlChange(ccBanks[activeBank].trsCC[i], shiftyTemp, ccBanks[activeBank].trsChannel[i]+1);

			// Serial.print(i);
			// Serial.print(": ");
			// Serial.print(ccBanks[activeBank].usbCC[i]);
			// Serial.print(": ");
			// Serial.println(shiftyTemp);
		}
		// pixels.show();
	}
	
	// READ HARDWARE MIDI
	while (HWMIDI.read()) {
	}
	// READ USBMIDI
	while (USBMIDI.read()) {
	}

// 	while (HWMIDI.read()) {
// 		// get a MIDI IN (Serial) message
// 		midi::MidiType type = HWMIDI.getType();
// 		byte channel = HWMIDI.getChannel();
// 		byte data1 = HWMIDI.getData1();
// 		byte data2 = HWMIDI.getData2();
// 
// 		if (type == midi::Clock) {
// 			// no led activity on clock
// 			//Serial.println("clock");
// 			activity = false;
// //			   ledOnMillis = 0;
// 		} else {
// 			activity = true;
// 		}
// 
// 		// send the message to USB MIDI 
// 		if (type != midi::SystemExclusive) {
// 			// Normal messages, simply give the data to the USBMIDI.send()
// 			Serial.print(type);
// 			Serial.print(":");
// 			Serial.print(data1);
// 			Serial.print(":");
// 			Serial.print(data2);
// 			Serial.print(":");
// 			Serial.println(channel);
// 			USBMIDI.send(type, data1, data2, channel);
// 		} else {
// 			// SysEx messages are special.	The message length is given in data1 & data2
// 			unsigned int SysExLength = data1 + data2 * 256;
// 			//USBMIDI.sendSysEx(SysExLength, MIDI.getSysExArray(), true, 0);
// 		}
// 	}
// 	// READ USBMIDI
// 	while(usb_midi.available()){
// 		if (USBMIDI.read()) {
// 			// get the USB MIDI message	 (except SysEX)
// 			midi::MidiType type = USBMIDI.getType();
// 			byte channel = USBMIDI.getChannel();
// 			byte data1 = USBMIDI.getData1();
// 			byte data2 = USBMIDI.getData2();
// 		
// 			if (type == midi::Clock) {
// 				// no led activity on clock
// 				activity = false;
// //				   ledOnMillis = 0;
// 			} else {
// 				activity = true;
// 			}
// 
// 			// send this message to Serial MIDI OUT
// 			if (type != midi::SystemExclusive) {
// 				// Normal messages, first we must convert MIDI2's type (an ordinary
// 				// byte) to the MIDI library's special MidiType.
// 				midi::MidiType mtype = (midi::MidiType)type;
// 
// 				// Then simply give the data to the MIDI library send()
// 				HWMIDI.send(mtype, data1, data2, channel);
// 			
// 			} else {
// 				// SysEx messages are special.	The message length is given in data1 & data2
// 				unsigned int SysExLength = data1 + data2 * 256;
// 				//HWMIDI.sendSysEx(SysExLength, USBMIDI.getSysExArray(), true);
// 			}
// 		}
// 	}

}
// END LOOP

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value);
void handleProgramChange(byte channel, byte number);
void handleSystemExclusive(byte* array, unsigned size);
void handleClock(void);
void handleStart(void);
void handleContinue(void);
void handleStop(void);

void sendNoteOn(byte channel, byte note, byte velocity) {
	USBMIDI.sendNoteOn(note, velocity, channel);
	HWMIDI.sendNoteOn(note, velocity, channel);
}

void sendNoteOff(byte channel, byte note, byte velocity) {
	USBMIDI.sendNoteOff(note, velocity, channel);
	HWMIDI.sendNoteOff(note, velocity, channel);
}

void sendControlChange(byte channel, byte control, byte value) {
	USBMIDI.sendControlChange(control, value, channel);
	HWMIDI.sendControlChange(control, value, channel);
}

void onProgramChange(byte channel, byte program) {
	Serial.print (channel);
	Serial.print (":");
	Serial.println (program);
	activeBank = constrain(program, 0, numBanks-1);
	pixelsOff();
	pixels.setPixelColor(activeBank+1, 128, 0, 0);
	pixels.show();
//	sendCurrentState();
	
//	USBMIDI.sendProgramChange(program, channel);
//	HWMIDI.sendProgramChange(program, channel);
}

void sendSysEx(uint32_t length, const uint8_t *sysexData, bool hasBeginEnd) {
	USBMIDI.sendSysEx(length, sysexData, hasBeginEnd);
}

void OnSysEx(byte* sysexData, unsigned length) 
{
	processIncomingSysex(sysexData, length);
}

void sendClock() {
	USBMIDI.sendClock();
	HWMIDI.sendClock();
}

void startClock(){
	USBMIDI.sendStart();
	HWMIDI.sendStart();
}

void continueClock(){
	USBMIDI.sendContinue();
	HWMIDI.sendContinue();
}

void stopClock(){
	USBMIDI.sendStop();
	HWMIDI.sendStop();
}

// Write all CC settings to memory
void config_write() {
    Serial.println("config_write");
    DynamicJsonDocument doc(12672); // assistant said 12672
    for( int j=0; j < numBanks; j++ ) {
        JsonArray banks = doc.createNestedArray();
			JsonArray usb = banks.createNestedArray();
			for( int i=0; i< numKnobs; i++ ) {
				JsonArray usbdata = usb.createNestedArray();
				usbdata.add(ccBanks[j].usbCC[i]);
				usbdata.add(ccBanks[j].usbChannel[i]);
			}
			JsonArray trs = banks.createNestedArray();
			for( int i=0; i< numKnobs; i++ ) {
				JsonArray trsdata = trs.createNestedArray();
				trsdata.add(ccBanks[j].trsCC[i]);
				trsdata.add(ccBanks[j].trsChannel[i]);
			}
    }

    LittleFS.remove( save_file );
    File file = LittleFS.open( save_file, "w");
    if( !file ) {
        Serial.println("config_write: Failed to create file");
        return;
    }
    if(serializeJson(doc, file) == 0) {
        Serial.println(F("config_write: Failed to write to file"));
    }
    file.close();
    serializeJson(doc, Serial);
}

// Read all CC settings to memory
bool config_read() {
    Serial.println("config_read");

    File f = LittleFS.open( save_file, "r");
    String s = f.readStringUntil('\n');
    f.close();
    Serial.println("  contents:"); Serial.println(s);

    File file = LittleFS.open( save_file, "r");
    if( !file ) {
        Serial.println("config_read: no config file, creating one");
        initCCbanks();
		config_write();
        return true;
    }

    DynamicJsonDocument doc(12672); // assistant said 12672
    DeserializationError error = deserializeJson(doc, file); // inputLength);
    if(error) {
        Serial.print("config_read: deserialize failed: ");
        Serial.println(error.c_str());
        return false;
    }

    for( int j=0; j < numBanks; j++ ) {
        JsonArray bank = doc[j];
		JsonArray usb = bank[0];
		JsonArray trs = bank[1];
		for( int i=0; i< numKnobs; i++ ) {
			// Serial.print(j);
			// Serial.print(":usb:");
			// Serial.print(i);
			// Serial.print(":");
			// Serial.println(usb[i][0].as<int>());
			ccBanks[j].usbCC[i] = usb[i][0].as<int>();
			ccBanks[j].usbChannel[i] = usb[i][1].as<int>();
			ccBanks[j].trsCC[i] = trs[i][0].as<int>();
			ccBanks[j].trsChannel[i] = trs[i][1].as<int>();
			// usbCC[i]
			// usbChannel[i]
			// trsCC[i]
			// trsChannel[i]
		}
	}
        // Serial.println(" ");
    file.close();
	return true;
}

// Load a single config
void config_load(int config_num) {
    Serial.printf("config_load:%d\n", config_num);
    for( int i=0; i< numKnobs; i++) {
//         seqr.steps[i] = sequences[config_num][i];
    }
//     seqr.seqno = config_num;
}

// Store current config to storage
void config_save(int config_num) {
    Serial.printf("config_save:%d\n", config_num);
    for( int i=0; i< numKnobs; i++) {
//         sequences[config_num][i] = seqr.steps[i];
    }
}

void pixelsOff(){
	for( int i=0; i< numLEDS; i++) {
		pixels.setPixelColor(i, 0, 0, 0);
	}
}
// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
	// strip.rainbow() can take a single argument (first pixel hue) or
	// optionally a few extras: number of rainbow repetitions (default 1),
	// saturation and value (brightness) (both 0-255, similar to the
	// ColorHSV() function, default 255), and a true/false flag for whether
	// to apply gamma correction to provide 'truer' colors (default true).
	pixels.rainbow(firstPixelHue);
	// Above line is equivalent to:
	// strip.rainbow(firstPixelHue, 1, 255, 255, true);
	pixels.show(); // Update strip with new contents
	delay(wait);  // Pause for a moment
  }
}

void initCCbanks(){
	ccBanks[0].fillRange(32,32);
	ccBanks[1].fillRange(16,16);
	ccBanks[1].fillChan(1,1);
	ccBanks[2].fillRange(48,48);
	ccBanks[3].fillRange(0,0);
	ccBanks[4].fillRange(0,0);
	ccBanks[5].fillRange(0,0);
	ccBanks[6].fillRange(0,0);
	ccBanks[7].fillRange(0,0);
}

void printCCBanks(){
 for( int j=0; j < numBanks; j++ ) {
 		for( int i=0; i< numKnobs; i++ ) {
			Serial.print(j);
			Serial.print(":trs:");
			Serial.print(i);
			Serial.print(":");
			Serial.println(ccBanks[j].trsCC[i]);
		}
 }

}

// SYSEX

void processIncomingSysex(const uint8_t* sysexData, unsigned size) {
	if(size < 3) {
		Serial.println("That's an empty sysex");
		return;
	}
	// F0 7D 00 00
	if(!(sysexData[1] == 0x7d && sysexData[2] == 0x00 && sysexData[3] == 0x00)) {
		Serial.println("Not a valid sysex message for us");
		return;
	}

	switch(sysexData[4]) {
		case INFO:
			// 1F = "1nFo" - please send me your current config
			Serial.println("Got an 1nFo request");
			sendCurrentState();
			break;
		case CONFIG_EDIT:
			// 0E - c0nfig Edit - here is a new config
 			Serial.println("Got an c0nfig Edit");
			// this->updateAllSettingsAndStore(sysexData, size);
			break;
		case CONFIG_DEVICE_EDIT:
			// 0D - c0nfig Device edit - new config just for device opts
 			Serial.println("Got an c0nfig Device Edit");
			// this->updateDeviceSettingsAndStore(sysexData, size);
			break;
		default:
			break;
// 		case 0x0a:
// 			// 0a - change config, don't store
// 			this->updateDeviceSettings(sysexData, size);
// 			break;
//		case 0x0c:
//			// 0C - c0nfig usb edit - here is a new config just for usb
//			updateUSBSettingsAndStore(sysexData, size);
//			break;
//		case 0x0b:
//			// 0B - c0nfig trs edit - here is a new config just for trs
//			updateTRSSettingsAndStore(sysexData, size);
//			break;
	}
}
void sendCurrentState() {
	//   0F - "c0nFig" - outputs its config:
	int offset = 8;
	int buffersize = 80;
	int fullbuffer = offset + buffersize;
	uint8_t sysexData[fullbuffer];

	// 8 bytes to start things off
	sysexData[0] = 0x7d; // manufacturer
	sysexData[1] = 0x00;
	sysexData[2] = 0x00;

	sysexData[3] = 0x0F; // ConFig;

	sysexData[4] = DEVICE_ID; // Device 01, ie, dev board
	sysexData[5] = MAJOR_VERSION; // major version
	sysexData[6] = MINOR_VERSION; // minor version
	sysexData[7] = POINT_VERSION; // point version

// 	16 bytes of config flags?
//	BANK NUMBER
// 	LED BLINK ?
// 	ROTATE (flip+reverse) ?
// 	MIDI THRU ?
//  X
//  X
//  X
//  X

//	64 bytes for usb and trs settings
// 	16x USBccs
// 	16x TRSccs
// 	16x USB channels
// 	16x TRS channels

	uint8_t buffer[buffersize];
	// Read 80 bytes into the array
	readSettingsToArray(activeBank, 0, buffer, buffersize);

	for(int i = 0; i < buffersize; i++) {
		int data = buffer[i];
		if(data == 0xff) {
		  data = 0x7f;
		}
		sysexData[i+offset] = data;
	}
	USBMIDI.sendSysEx(fullbuffer, sysexData, false);
}

void readSettingsToArray(uint8_t bank, size_t address, uint8_t buffer[], int length){
	buffer[0] = bank;
	for (int i = 1; i < 16; i++) {
		// settings
		buffer[i] = 0;
	}
	int j = 16;
	for (int k = 0; k < 16; k++) {
		buffer[j] = ccBanks[bank].usbCC[k];
		j = j+1;
	}
	for (int k = 0; k < 16; k++) {
		buffer[j] = ccBanks[bank].trsCC[k];
		j = j+1;
	}
	for (int k = 0; k < 16; k++) {
		buffer[j] = ccBanks[bank].usbChannel[k];
		j = j+1;
	}
	for (int k = 0; k < 16; k++) {
		buffer[j] = ccBanks[bank].trsChannel[k];
		j = j+1;
	}
	
}