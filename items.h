#ifndef __ITEMS_H__
#define __ITEMS_H__

#include "Item.h"

const Item Health {
  Red('+'),
  [](Player &you) {
    return you.heal(5);
  }
};

const Item BigHealth {
  Orange('+'),
  [](Player &you) {
    return you.heal(100);
  }
};


const Item Bomb {
  LightGray('!'),
  [](Player &you) {
    return you.addBombs(1);
  }
};

const Item BombPack {
  Cyan('!'),
  [](Player &you) {
    return you.addBombs(3);
  }
};

const Item BombBigPack {
  LightCyan('!'),
  [](Player &you) {
    return you.addBombs(6);
  }
};


const Item TorchPack {
  Cyan('^'),
  [](Player &you) {
    return you.addTorches(3);
  }
};

const Item TorchBigPack {
  LightCyan('^'),
  [](Player &you) {
    return you.addTorches(6);
  }
};


const Item Arrow {
  LightGray('|'),
  [](Player &you) {
    return you.addArrows(1);
  }
};

const Item ArrowPack {
  Cyan('|'),
  [](Player &you) {
    return you.addArrows(3);
  }
};

const Item ArrowBigPack {
  LightCyan('|'),
  [](Player &you) {
    return you.addArrows(6);
  }
};

#endif
