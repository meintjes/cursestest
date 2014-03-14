#ifndef __POINT_H__
#define __POINT_H__

struct Point {
  Point();
  Point(int xIn, int yIn);
  int x;
  int y;
  bool operator==(const Point &rhs) const;
  bool operator!=(const Point &rhs) const;
};

#endif
