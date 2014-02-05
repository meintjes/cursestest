#include "Option.h"

Option::Option(std::string nameIn, std::function<void(void)> fnIn) :
  text(nameIn), textfn(nullptr), fn(fnIn) {
}

Option::Option(Cst nameIn, std::function<void(void)> fnIn) :
  text(nameIn), textfn(nullptr), fn(fnIn) {
}

Option::Option(std::function<Cst(void)> textfnIn,
	       std::function<void(void)> fnIn) :
  text(Cst{"ERROR", Red}), textfn(textfnIn), fn(fnIn) {

}


void Option::operator()() const {
  fn();
}

Cst Option::getText() const {
  if (textfn) {
    return textfn();
  }
  else {
    return text;
  }
}
