#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Menu.h"
#include <string>
#include <ncurses.h>

enum Command {COMMAND_FIRST,

	      COMMAND_MOVE_UPLEFT,
	      COMMAND_MOVE_UP,
	      COMMAND_MOVE_UPRIGHT,
	      COMMAND_MOVE_LEFT,
	      COMMAND_WAIT,
	      COMMAND_MOVE_RIGHT,
	      COMMAND_MOVE_DOWNLEFT,
	      COMMAND_MOVE_DOWN,
	      COMMAND_MOVE_DOWNRIGHT,

	      COMMAND_USE_BOMB,
	      COMMAND_USE_TORCH,
	      COMMAND_USE_ARROW,

	      COMMAND_INTERACT_STAIRSUP,
	      COMMAND_INTERACT_STAIRSDOWN,

	      COMMAND_LAST};

const int CMAP_SIZE = 256;
typedef Command CommandMap[CMAP_SIZE];

bool readControls(std::string filename, CommandMap cmap);
void writeControls(std::string filename, CommandMap cmap);

template <Command command> void changeControl() {
  CommandMap cmap;
  if (readControls("controls.txt", cmap)) {
    unsigned char ch = getch();
    while (ch != 13 && ch != 27) {
      erase();
      move(0, 0);
      if (ch < CMAP_SIZE) {
	addch(ch);
	addstr(" has been bound.");
	cmap[ch] = command;
	ch = getch();
      }
      else {
	addstr("Character out of range.");
      }
    } 
    writeControls("controls.txt", cmap);
  }
  else {
    addstr("Failed to load controls.");
    getch();
  }
}

#endif
