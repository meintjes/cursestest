#ifndef __OPTION_H__
#define __OPTION_H__

#include <functional>
#include <string>
#include "Cst.h"

//class used to represent an indivdual option in a scrolling menu. consists
//of text (either a regular Cst or a function returning one) and a function
//to be called when the menu is chosen (use std::bind or lambdas, probably).
//note that menus are also functions, so an option can contain another menu. 

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
