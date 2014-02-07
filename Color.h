#ifndef __COLOR_H__
#define __COLOR_H__

#include <string>

class Cst;
class Cch;

struct Color {
  int id;
  bool bold;

  const Cst operator()(const Cst &cst) const;
  const Cch operator()(const Cch &cch) const;

  friend void addc(const Cst &cst);
};

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

#endif
