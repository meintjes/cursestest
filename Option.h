#ifndef __OPTION_H__
#define __OPTION_H__

#include <functional>
#include <string>
#include "Cst.h"

class Option {
 public:
  Option(const Cst &nameIn, std::function<void(void)> fnIn);
  Option(std::function<Cst(void)> textfnIn, std::function<void(void)> fnIn);
  void operator()() const;
  Cst getText() const;
 private:
  Cst text;
  std::function<Cst(void)> textfn;
  std::function<void(void)> fn;
};

#endif
