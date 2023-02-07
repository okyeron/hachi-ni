# Sysex spec

The device interfaces with its editor via MIDI Sysex. This document describes the supported messages.

_Work in progress, porting from 16n faderbank editor_

## `0x1F` - "1nFo"

Request for 8x2 to transmit current state via sysex. No other payload.

## `0x0F` - "c0nFig"

"Here is my current config." Only sent by device as an outbound message, in response to `0x1F`. 

## `0x0E` - "c0nfig Edit"

"Here is a new complete configuration for you". Payload (other than mfg header, top/tail, etc) of 80 bytes to go straight into device memory, according to the memory map described below.

## `0x0D` - "c0nfig edit (Device options)"

not implemented ~~"Here is a new set of device options for you". Payload (other than mfg header, top/tail, etc) of 32 bytes to go straight into appropriate locations of memory.~~

Example: 
`F0 7D 00 00 0D 09 00 00 00 15 16 17 18 07 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 5B 5D 67 68 69 00 00 00 F7`

## `0x0C` - "c0nfig edit (usb options)"

not implemented ~~"Here is a new set of USB options for you". Payload (other than mfg header, top/tail, etc) of 32 bytes to go straight into appropriate locations of EEPROM, according to the memory map described in `README.md`.~~ 

## `0x0B` - "c0nfig edit (trs options)"

not implemented ~~"Here is a new set of TRS options for you". Payload (other than mfg header, top/tail, etc) of 32 bytes to go straight into appropriate locations of EEPROM, according to the memory map described in `README.md`.~~ 

---

## SYSEX Map of config data - 90 bytes

F0 7D 00 00 (4 bytes)
command (1 byte)
modelnum (1 byte)
version (3 bytes) 
bank (1 byte)
other settings (15 bytes)
USB CCs (16 bytes)
TRS CCs (16 bytes)
USB Channels (16 bytes)
TRS Channels (16 bytes)
F7 (1 byte)

F0 7D 00 00 0F 03 00 06 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 F7