#ifndef __OPTION_H__
#define __OPTION_H__

#include "ColoredText.h"
#include <functional>

class Option {
 public:
  Option(std::string nameIn, std::function<void(void)> fnIn);
  Option(Cst nameIn, std::function<void(void)> fnIn);
  void operator()() const;
  Cst getText() const;
 private:
  Cst text;
  std::function<void(void)> fn;
};

#endif
