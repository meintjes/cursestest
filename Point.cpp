#include "Point.h"

bool Point::operator==(const Point &rhs) const {
  return (rhs.x == this->x && rhs.y == this->y);
}
  
