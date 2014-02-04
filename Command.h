#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Menu.h"
#include <string>
#include <ncurses.h>

enum Command {COMMAND_FIRST, //exists so cmaps are initialized with an
	                     //unused command rather than something useful
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

	      COMMAND_LAST}; //exits to help iterate through cmaps

//if size is changed to 256, writeControls() overflows, loops infinitely,
//and takes up all the space on your drive. it's hilarious, definitely do it
const int CMAP_SIZE = 128;
typedef Command CommandMap[CMAP_SIZE];

bool readControls(std::string filename, CommandMap cmap);
void writeControls(std::string filename, CommandMap cmap);
void changeControl(Command command);
void clearControl(Command command, CommandMap cmap);

#endif
