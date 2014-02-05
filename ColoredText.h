#ifndef __COLOREDTEXT_H__
#define __COLOREDTEXT_H__

#include "Color.h"
#include <string>

template <typename T> class ColoredText {
 public:
  ColoredText(T textIn, const Color &colorIn = LightGray)
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

  template<class U>
  friend const ColoredText<U> operator+(const ColoredText<U> &lhs,
					const std::string &rhs);
  template<class U>
  friend const ColoredText<U> operator+(const std::string &lhs,
					const ColoredText<U> &rhs);

 private:
  void print() const {
    addstr(std::to_string(text).c_str());
  }

  T text;
  const Color &color;
};

template <typename U>
const ColoredText<U> operator+(const ColoredText<U> &lhs,
			       const std::string &rhs) {
  return ColoredText<U>{lhs.text + rhs, lhs.color};
}

template <typename U>
const ColoredText<U> operator+(const std::string &lhs,
			       const ColoredText<U> &rhs) {
  return ColoredText<U>{lhs + rhs.text, rhs.color};
}

typedef ColoredText<std::string> Cst;
typedef ColoredText<char> Cch;

//specialized in cpp file
template <> void Cst::print() const;
template <> void Cch::print() const;

#endif
