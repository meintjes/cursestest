#ifndef __OPTION_H__
#define __OPTION_H__

#include "Cst.h"
#include <functional>
#include <string>

class Option {
 public:
  Option(Cst nameIn, std::function<void(void)> fnIn);
  Option(std::function<Cst(void)> textfnIn, std::function<void(void)> fnIn);
  void operator()() const;
  Cst getText() const;
 private:
  Cst text;
  std::function<Cst(void)> textfn;
  std::function<void(void)> fn;
};

#endif
