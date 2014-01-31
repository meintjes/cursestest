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
/*
static std::string CommandNames[COMMAND_LAST];

CommandNames[COMMAND_MOVE_UPLEFT] = "Move up/left";
CommandNames[COMMAND_MOVE_UP] = "Move up";
CommandNames[COMMAND_MOVE_UPRIGHT] = "Move up/right";
CommandNames[COMMAND_MOVE_LEFT] = "Move left";
CommandNames[COMMAND_WAIT] = "Wait";
CommandNames[COMMAND_MOVE_RIGHT] = "Move right";
CommandNames[COMMAND_MOVE_DOWNLEFT] = "Move down/left";
CommandNames[COMMAND_MOVE_DOWN] = "Move down";
CommandNames[COMMAND_MOVE_DOWNRIGHT] = "Move down/right";

CommandNames[COMMAND_USE_BOMB] = "Use bomb";
CommandNames[COMMAND_USE_TORCH] = "Use torch";
CommandNames[COMMAND_USE_ARROW] = "Use arrow";

CommandNames[COMMAND_INTERACT_STAIRSUP] = "Go up stairs";
CommandNames[COMMAND_INTERACT_STAIRSDOWN] = "Go down stairs";
*/

const int CMAP_SIZE = 256;
typedef Command CommandMap[CMAP_SIZE];

bool readControls(std::string filename, CommandMap cmap);
void createControls(CommandMap cmap);
void writeControls(std::string filename, CommandMap cmap);

#endif
