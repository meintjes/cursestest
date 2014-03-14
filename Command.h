#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "Menu.h"
#include <string>
#include "Option.h"

//new commands must be defined here, given default values in resetControls,
//actually implemented in getInput, and given a name in getCommandName. as
//long as you do those things, the controls menu is automatically generated.
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

	      COMMAND_EVOKE_ARTIFACT,
              COMMAND_USE_ITEM,
              COMMAND_DROP_ITEM,

	      COMMAND_INTERACT_STAIRSUP,
	      COMMAND_INTERACT_STAIRSDOWN,

              COMMAND_LONG_PROMPT,

	      COMMAND_LAST}; //exits to help iterate through cmaps

//if size is changed to 256, writeControls() overflows, loops infinitely,
//and takes up all the space on your drive. it's hilarious, definitely do it
const int CMAP_SIZE = 128;
typedef Command CommandMap[CMAP_SIZE];

//The following two functions must be updated when new commands are added:
//rewrites controls.txt with the default controls
void resetControls();
//get a string describing what the command does
std::string getCommandName(Command command);


//The rest should work fine without modification.
//reads controls from file into the provided command map. arrays are passed
//by reference, which is why this isn't wrong (even though it looks wrong)
bool readControls(CommandMap cmap);

//write the provided command map into controls.txt 
void writeControls(const CommandMap cmap);

//modifies controls.txt by changing only the given control, either by adding
//another key or by clearing all keys
void changeControl(Command command);

//takes the given command map and removes all keys for the given command
void clearControl(Command command, CommandMap cmap);

//returns a string containing all keys mapped to the given command 
std::string listKeysForCommand(Command command, const CommandMap cmap);

#endif
