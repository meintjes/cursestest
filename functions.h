#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

//returns an int from 0 to x
int randTo(int x);

//returns an int from min to max
//if min > max, returns max
int randRange(int min, int max);

//decrements the given duration to a minimum of 0.
//returns a reference to the duration.
int& decrementDuration(int &dur, unsigned int num);

//returns the sign of the input value: -1, 0, or +1
template <typename T> int sgn(T val) {
  return (T(0) < val) - (val < T(0));
}

#endif
