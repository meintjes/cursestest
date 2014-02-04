#include "Option.h"

Option::Option(std::string nameIn, std::function<void(void)> fnIn) :
  text(Cst{nameIn, LightGray}), fn(fnIn) {
  
}

Option::Option(Cst nameIn, std::function<void(void)> fnIn) :
  text(nameIn), fn(fnIn) {
}

void Option::operator()() const {
  fn();
}

Cst Option::getText() const {
  return text;
}
