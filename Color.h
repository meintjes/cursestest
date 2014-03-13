#ifndef __COLOR_H__
#define __COLOR_H__

#include <string>
#include "Cst.h"
#include "Cch.h"

//Color is used to construct colored text and characters, which circumvent the
//unwieldy ncurses output functions. Cst ("colored strings") are output using
//addcs(cst) (or addcs(x, y, cst), which moves to coords x, y before outputting
//the string). Cch ("colorer chars") are output using addc(cch) (or addch(x, y,
//cch)). Manually calling ncurses' move(), which uses weird (y, x) syntax, is
//usually not necessary. Manually using ncurses to set output attributes is
//even less necessary. Don't do that.

//Cst and Cch have private constructors. Use Color() to construct them; for
//example, Red("words words words") returns "words words words" in red, and
//White('x') returns an 'x' in white. Cst can also be implicitly converted from
//const char* and std::string, in which case they're just light gray. In the
//same way, light gray Cch can be converted from regular chars.

struct Color {
  int hue;
  bool bold;

  const Cst operator()(const Cst &cst) const {
    return Cst(cst.text, this->hue, this->bold);
  }

  constexpr const Cch operator()(const Cch &cch) const {
    return Cch(cch.glyph, this->hue, this->bold);
  }
};

//list of defined colors. new colors probably require initialization in main().
const Color DarkGray {1, true};
const Color Red {2, false};
const Color Orange {2, true};
const Color DarkGreen {3, false};
const Color LightGreen {3, true};
const Color Brown {4, false};
const Color Yellow {4, true};
const Color DarkBlue {5, false};
const Color LightBlue {5, true};
const Color Purple {6, false};
const Color LightMagenta {6, true};
const Color Cyan {7, false};
const Color LightCyan {7, true};
const Color LightGray {8, false};
const Color White {8, true};

const Color BlackOnWhite {9, false};
const Color BlackOnRed {10, false};
const Color BlackOnGreen {11, false};
const Color BlackOnBrown {12, false};
const Color BlackOnCyan {13, false};
const Color BrownOnCyan {14, false};
const Color DarkGrayOnCyan {13, true};
const Color WhiteOnGray {15, true};

#endif
