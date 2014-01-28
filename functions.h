#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

//returns an int from 0 to x
int randTo(int x);

//returns an int from min to max
//if min > max, returns max
int randRange(int min, int max);

template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

#endif
