#include "functions.h"
#include <cstdlib>
#include <assert.h>

int randTo(int x) {
  assert(x >= 0);
  return std::rand() % (x + 1);
}

int randRange(int min, int max) {
  if (min > max) {
    return max;
  }
  else {
    return min + randTo(max - min);
  }
}

int& decrementDuration(int &dur, unsigned int num) {
  dur -= num;
  if (dur < 0) {
    dur = 0;
  }
  return dur;
}

