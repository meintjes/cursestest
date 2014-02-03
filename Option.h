#ifndef __OPTION_H__
#define __OPTION_H__

#include "Cst.h"

class Option {
 public:
  Option(std::string nameIn, void (*fnIn)(), const Color &color = LightGray);
  void operator()() const;
  Cst getText() const;
 private:
  Cst text;
  void (*fn)();
};

#endif
