#include "Command.h"
#include <fstream>

bool readControls(std::string filename, CommandMap cmap) {
  std::ifstream file;
  file.open(filename);
  std::string line;

  for (int com = COMMAND_FIRST + 1; com < COMMAND_LAST; com++) {
    if (std::getline(file, line)) {
      for (unsigned int i = 0; i < line.length(); i++) {
	cmap[static_cast<unsigned char>(line[i])] = static_cast<Command>(com);
      }
    }
    else {
      return false; //the file ended early
    }
  }
  return true; //success!
}

void createControls() {
  /*
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

  
  writeControls("controls.txt");
  */
}

void writeControls(std::string filename, CommandMap cmap) {
  std::ofstream file;
  file.open(filename);

  for (int com = COMMAND_FIRST + 1; com < COMMAND_LAST; com++) {
    for (int i = 0; i < CMAP_SIZE; i++) {
      if (cmap[i] == com) {
	file << static_cast<unsigned char>(i);
      }
    }
    file << '\n';
  }
}
