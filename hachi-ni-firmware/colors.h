#pragma once

#include <stdint.h>

// COLOR PRESETS
// https://www.rapidtables.com/web/color/color-wheel.html
// hsl(xxx, 100%, 50%)
const auto RED = 0xFF0000;
const auto ORANGE = 0xFF8000;
const auto YELLOW = 0xFFFF00;
const auto LIME = 0x80FF00;
const auto GREEN = 0x00FF00;
const auto MINT = 0x00FF80;
const auto CYAN = 0x00FFFF;
const auto RBLUE = 0x007FFF;
const auto BLUE = 0x0000FF;
const auto PURPLE = 0x7F00FF;
const auto MAGENTA = 0xFF00FF;
const auto ROSE = 0xFF0080;

// hsl(xxx, 50%, 50%)
const auto MEDRED = 0xBF4040;
const auto MEDBLUE = 0x4040BF;
const auto MEDYELLOW = 0xBFBF40;

// hsl(xxx, 100%, 75%)
const auto LTCYAN = 0x80FFFF;
const auto LTPURPLE = 0xBF80FF;
const auto SALMON = 0xFF8080;
const auto PINK = 0xFF80D4;
const auto LTYELLOW = 0xFFFF80;

// hsl(xxx, 100%, 15%)
const auto DKRED = 0x800000;
const auto DKORANGE = 0x4D2600;
const auto DKYELLOW = 0x4C4D00;
const auto DKLIME = 0x408000;
const auto DKGREEN = 0x264D00;
const auto DKCYAN = 0x004C4D;
const auto DKBLUE = 0x00001A;
const auto DKPURPLE = 0x26004D;
const auto DKMAGENTA = 0x4D004C;
const auto INDIGO = 0x4B0082;

// hsl(xxx, 100%, 5%)
const auto VDKRED = 0x0B0404;
const auto VDKORANGE = 0x0B0804;
const auto VDKYELLOW = 0x0B0B04;
const auto VDKGREEN = 0x080B04;
const auto VDKCYAN = 0x040B0B;
const auto VDKBLUE = 0x04040B;
const auto VDKPURPLE = 0x08040B;
const auto VDKMAGENTA = 0x0B040B;


// hsl(xxx, 50%, 75%)
const auto LBLUE = 0x9FCFDF;
const auto VIOLET = 0xDF9FDF;

// hsl(xxx, 25%, 50%)
const auto DIMRED = 0x9F6060;
const auto DIMORANGE = 0x9F8060;
const auto DIMYELLOW = 0x9F9F60;
const auto DIMLIME = 0x809F60;
const auto DIMGREEN = 0x609F60;
const auto DIMMAGENTA = 0x9F609F;
const auto DIMCYAN = 0x609F9F;
const auto DIMBLUE = 0x60609F;
const auto DIMPURPLE = 0x7F609F;


// other
const auto AMBER = 0x999900;
const auto BEIGE = 0xFFCC33;

// no color

const auto WHITE = 0xFFFFFF;
const auto HALFWHITE = 0x808080;
const auto LOWWHITE = 0x202020;
const auto VLOWWHITE = 0x101010;
const auto LEDOFF = 0x000000;

const uint32_t bankColors[] = {ORANGE,YELLOW,GREEN,MAGENTA,CYAN,BLUE,PURPLE,RED};
// const uint32_t bankColors2[] = {DIMORANGE,DIMYELLOW,DIMGREEN,DIMMAGENTA,DIMCYAN,DIMBLUE,DIMPURPLE,DIMRED};
const uint32_t bankColors2[] = {VDKORANGE,VDKYELLOW,VDKGREEN,VDKMAGENTA,VDKCYAN,VDKBLUE,VDKPURPLE,VDKRED};

