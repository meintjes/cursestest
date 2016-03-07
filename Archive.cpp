#include "Archive.h"

Archive::Archive(std::string path, Archive::Type typeIn) :
  type(typeIn),
  stream(path.c_str(), typeIn == Save ?
                       std::fstream::out | std::fstream::trunc :
                       std::fstream::in
        )
{}

Archive::operator bool() const {
  return bool(stream);
}

Archive::Type Archive::getType() const {
  return type;
}
