#include "Option.h"

Option::Option(std::string nameIn, void (*fnIn)(), const Color &color) :
  text(Cst{nameIn, color}), fn(fnIn) {
  
}

void Option::operator()() const {
  fn();
}


Cst Option::getText() const {
  return text;
}
