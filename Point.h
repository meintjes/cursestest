#ifndef __POINT_H__
#define __POINT_H__

struct Point {
  int x;
  int y;
  bool operator==(const Point &rhs) const;
  bool operator!=(const Point &rhs) const;
};

#endif
