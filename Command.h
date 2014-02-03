#ifndef __COMMAND_H__
#define __COMMAND_H__

#include <string>

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
void createControls();
void writeControls(std::string filename, CommandMap cmap);

#endif