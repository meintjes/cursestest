#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "Item.h"

const Item Health {
  Cch{'+', Red},
  [](Player &you) {
    return you.heal(5);
  }
};

const Item BigHealth {
  Cch{'+', Orange},
  [](Player &you) {
    return you.heal(100);
  }
};


const Item Bomb {
  Cch{'!', LightGray},
  [](Player &you) {
    return you.addBombs(1);
  }
};

const Item BombPack {
  Cch{'!', Cyan},
  [](Player &you) {
    return you.addBombs(3);
  }
};

const Item BombBigPack {
  Cch{'!', LightCyan},
  [](Player &you) {
    return you.addBombs(6);
  }
};


const Item TorchPack {
  Cch{'^', Cyan},
  [](Player &you) {
    return you.addTorches(3);
  }
};

const Item TorchBigPack {
  Cch{'^', LightCyan},
  [](Player &you) {
    return you.addTorches(6);
  }
};


const Item Arrow {
  Cch{'|', LightGray},
  [](Player &you) {
    return you.addArrows(1);
  }
};

const Item ArrowPack {
  Cch{'|', Cyan},
  [](Player &you) {
    return you.addArrows(3);
  }
};

const Item ArrowBigPack {
  Cch{'|', LightCyan},
  [](Player &you) {
    return you.addArrows(6);
  }
};

#endif
