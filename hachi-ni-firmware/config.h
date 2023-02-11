#pragma once

#include <Arduino.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <ResponsiveAnalogRead.h>

/* * firmware metadata  */
const int MAJOR_VERSION = 0;
const int MINOR_VERSION = 6;
const int POINT_VERSION = 0;

const int DEVICE_ID     = 3;

const byte I2C_SDA = 2;
const byte I2C_SCL = 3;

// Increment this when data layout in EEPROM changes. May need to write version upgrade readers when this changes.
extern const uint8_t EEPROM_VERSION;

#define EEPROM_HEADER_ADDRESS            0
#define EEPROM_HEADER_SIZE               32
#define EEPROM_PATTERN_ADDRESS           32

// SYSEX 
const uint8_t INFO = 0x1F;
const uint8_t CONFIG_EDIT = 0x0E;
const uint8_t CONFIG_DEVICE_EDIT = 0x0D;

const int TXLED = 0;
const int RXLED = 1;
const int REDLED = 14;
const int NEOPIXPIN = 16;
const int NEOPWRPIN = 17;
const int numLEDS = 9;

const int channelCount = 16;
const int numKnobs = 16;
const int numBanks = 8;
const int buttons[2] = {25,24};
int buttonState[2] = {0,0};

int faderMin = 0;
int faderMax = 1019;
int shiftyTemp;
bool activity = true;

const char* save_file = "/saved_configs.json";

const int muxMapping[16] = {8, 9, 10, 11, 12, 13, 14, 15, 7, 6, 5, 4, 3, 2, 1, 0};
const int mux_common_pin = 29; // select a pin to share with the 16 channels of the CD74HC4067
const int mux1 = 10;
const int mux2 = 11;
const int mux3 = 9;
const int mux4 = 8;
