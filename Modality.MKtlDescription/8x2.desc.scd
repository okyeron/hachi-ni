///////// Example for an external MIDI device ///////
/*
  This is the default pot values for the hachi-ni (8x2) device.
  
  Default Bank 1 - ccs 32-47
*/

(
deviceName: "hachi-ni",
protocol: \midi,
deviceType: \faderbox,
elementTypes: [\knob],
status: (
	linux: "unknown",
	osx: "tested",
	win: "unknown"),

idInfo: "hachi-ni",

deviceInfo: (
	vendorURI: "https://github.com/okyeron/hachi-ni",
	manualURI: "https://github.com/okyeron/hachi-ni",
	description: "Sixteen MIDI potentiometers",
	features: [
		"MIDI trs and USB)"],
	notes: "A customizable 16 channel midi controller. Available as a kit.",
	longName: "hachi-ni",
),

elementsDesc: (
	key: \kn,
	shared: (
		elementType: \knob, 
		midiMsgType: \control,
		midiChan: 0, 
		spec: \midiCC, \ioType: \inout),
	elements: (
		(1..16).collect { 
			|num, i| (key: num.asSymbol, midiNum: 31 + num, \style: (row: i div: 8, column: i % 8) )
		}

	)
)
);

