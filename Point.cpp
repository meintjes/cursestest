#include "Point.h"

Point::Point() {}

Point::Point(int xIn, int yIn) :
  x(xIn),
  y(yIn)
{}

bool Point::operator==(const Point &rhs) const {
  return (rhs.x == this->x && rhs.y == this->y);
}

bool Point::operator!=(const Point &rhs) const {
  return !(*this == rhs);
}
