#include "Option.h"

Option::Option(Cst nameIn, std::function<void(void)> fnIn) :
  text(nameIn), textfn(nullptr), fn(fnIn) {
}

Option::Option(std::function<Cst(void)> textfnIn,
	       std::function<void(void)> fnIn) :
  text(Red("ERROR")), textfn(textfnIn), fn(fnIn) {

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
