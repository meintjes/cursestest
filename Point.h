#ifndef __POINT_H__
#define __POINT_H__

namespace boost {
  namespace serialization {
    class access;
  }
}

struct Point {
  Point();
  Point(int xIn, int yIn);
  int x;
  int y;
  bool operator==(const Point &rhs) const;
  bool operator!=(const Point &rhs) const;

  friend class boost::serialization::access;

 private:
  template <class Archive>
  void serialize(Archive & ar, const unsigned int version) {
    ar & x;
    ar & y;
  }
};

#endif
