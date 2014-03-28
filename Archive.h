#ifndef __ARCHIVE_H__
#define __ARCHIVE_H__

#include <string>
#include <fstream>
#include <cassert>

//Archive wraps an fstream and handles input/output to and from save files.
//operator& both reads and writes so that one serialization function can both
//save and load. if differences need to exist, conditionalize on getType().

class Archive {
 public:
  enum Type {Save, Load};
  Archive(std::string path, Archive::Type typeIn);
  Archive::Type getType() const;

  explicit operator bool() const;

  template <typename T>
  Archive& operator&(T &t) {
    if (type == Save) {
      stream << t << ' ';
    }
    else {
      stream >> t;
    }
    return *this;
  }

  template <typename T>
  Archive& operator>>(T &t) {
    assert(type == Load); //you can't load into an rvalue or const
    stream >> t;
    return *this;
  }
  
  template <typename T>
  Archive& operator<<(const T &t) {
    assert(type == Save);
    stream << t << ' ';
    return *this;
  }

 private:
  const Archive::Type type;
  std::fstream stream;
};

//beautiful horrible compile-time hash function for generating tags from class
//names. if there's a collision it'll refuse to compile because there'll be
//multiple switch cases with the same value, so the awfulness shouldn't matter
constexpr unsigned long long tagize(const char* str, int value = 0) {
  return str[0] ? tagize(str + 1, 4*value + str[0]) : value;
}

#define CREATE_TAG_FOR(NAME) \
  std::string getSerializationTag() const {return #NAME;}

//creates a case in a getPointerFromTag function's switch statement
#define CREATE_CASE_FOR(NAME) case tagize(#NAME): \
                                return new NAME;

//creates a case for the "None" tag (no item)
#define CREATE_NONE_CASE case tagize("_"): \
                           return nullptr;

#endif
