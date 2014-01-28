#include "SpaceType.h"

bool SpaceType::operator==(const SpaceType &rhs) const {
  return (this->id == rhs.id);
}
