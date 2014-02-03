#ifndef __COLOREDTEXT_H__
#define __COLOREDTEXT_H__

#include "Color.h"
#include <string>

template <typename T> class ColoredText {
 public:
  ColoredText(T textIn, const Color &colorIn)
    : text(textIn), color(colorIn) {}

  void add() const {
    if (color.bold) {
      attron(A_BOLD);
      attron(COLOR_PAIR(color.id));
      print();
      attroff(COLOR_PAIR(color.id));
      attroff(A_BOLD);
    }
    else {
      attron(COLOR_PAIR(color.id));
      print();
      attroff(COLOR_PAIR(color.id));
    }
  }

  ColoredText<T> shift(const Color &newcolor) const {
    return ColoredText<T>{this->text, newcolor};
  }

 private:
  void print() const {
    addstr(std::to_string(text).c_str());
  }

  T text;
  const Color &color;
};

typedef ColoredText<std::string> Cst;
typedef ColoredText<char> Cch;

template <> void ColoredText<std::string>::print() const;
template <> void ColoredText<char>::print() const;

#endif
