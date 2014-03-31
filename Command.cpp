#include "Command.h"
#include "Color.h"
#include <fstream>
#include <ncurses.h>

void resetControls() {
  CommandMap cmap = {COMMAND_FIRST};
  
  cmap['7'] = cmap['y'] = COMMAND_MOVE_UPLEFT;
  cmap['8'] = cmap['k'] = COMMAND_MOVE_UP;
  cmap['9'] = cmap['u'] = COMMAND_MOVE_UPRIGHT;
  cmap['4'] = cmap['h'] = COMMAND_MOVE_LEFT;
  cmap['5'] = cmap['s'] = COMMAND_WAIT;
  cmap['6'] = cmap['l'] = COMMAND_MOVE_RIGHT;
  cmap['1'] = cmap['b'] = COMMAND_MOVE_DOWNLEFT;
  cmap['2'] = cmap['j'] = COMMAND_MOVE_DOWN;
  cmap['3'] = cmap['n'] = COMMAND_MOVE_DOWNRIGHT;

  cmap['r'] = COMMAND_TOGGLE_RUN;

  cmap['a'] = COMMAND_EVOKE_ARTIFACT;
  cmap['i'] = COMMAND_USE_ITEM;
  cmap['d'] = COMMAND_DROP_ITEM;

  cmap['<'] = COMMAND_INTERACT_STAIRSUP;
  cmap['>'] = COMMAND_INTERACT_STAIRSDOWN;

  cmap['#'] = COMMAND_LONG_PROMPT;

  writeControls(cmap);
}

std::string getCommandName(Command command) {
  switch(command) {
  case COMMAND_MOVE_UPLEFT:
    return "Move up/left";
  case COMMAND_MOVE_UP:
    return "Move up";
  case COMMAND_MOVE_UPRIGHT:
    return "Move up/right";
  case COMMAND_MOVE_LEFT:
    return "Move left";
  case COMMAND_WAIT:
    return "Wait";
  case COMMAND_MOVE_RIGHT:
    return "Move right";
  case COMMAND_MOVE_DOWNLEFT:
    return "Move down/left";
  case COMMAND_MOVE_DOWN:
    return "Move down";
  case COMMAND_MOVE_DOWNRIGHT:
    return "Move down/right";
  case COMMAND_TOGGLE_RUN:
    return "Start/stop running";
  case COMMAND_EVOKE_ARTIFACT:
    return "Evoke artifact";
  case COMMAND_USE_ITEM:
    return "Use item";
  case COMMAND_DROP_ITEM:
    return "Drop item";
  case COMMAND_INTERACT_STAIRSUP:
    return "Go up stairs";
  case COMMAND_INTERACT_STAIRSDOWN:
    return "Go down stairs";
  case COMMAND_LONG_PROMPT:
    return "Enter long command";
  default:
    return "UNDEFINED COMMAND IN getCommandName: "
           + std::to_string(static_cast<int>(command));
  }
}


bool readControls(CommandMap cmap) {
  std::ifstream file;
  file.open("controls.txt");
  std::string line;

  for (int com = COMMAND_FIRST + 1; com < COMMAND_LAST; com++) {
    if (std::getline(file, line)) {
      for (unsigned int i = 0; i < line.length(); i++) {
	unsigned char nextChar = line[i];
        if (nextChar >= CMAP_SIZE) { //don't go out of bounds
          return false;
        }
        cmap[nextChar] = static_cast<Command>(com);
      }
    }
    else {
      return false; //the file ended early
    }
  }
  return true; //success!
}

void writeControls(const CommandMap cmap) {
  std::ofstream file;
  file.open("controls.txt");

  for (int com = COMMAND_FIRST + 1; com < COMMAND_LAST; com++) {
    for (unsigned char i = 0; i < CMAP_SIZE; i++) {
      if (cmap[i] == com) {
	file << i;
      }
    }
    file << '\n';
  }
}

void changeControl(Command command) {
  CommandMap cmap = {COMMAND_FIRST};
  if (readControls(cmap)) {
    addcs(26, 11, White("Press a key to bind to control (escape to clear)."));
    unsigned char ch = getch();
    if (ch == 27) {
      clearControl(command, cmap);
    }
    else if (ch < CMAP_SIZE) {
      cmap[ch] = command;
    }
    writeControls(cmap);
  }
  else {
    addcs(26, 11, Red("Failed to load controls."));
  }
}

void clearControl(Command command, CommandMap cmap) {
  for (unsigned char i = 0; i < CMAP_SIZE; i++) {
    if (cmap[i] == command) {
      cmap[i] = COMMAND_FIRST;
    }
  }
}

std::string listKeysForCommand(Command command, const CommandMap cmap) {
  std::string str = "";
  for (unsigned char i = 0; i < CMAP_SIZE; i++) {
    if (cmap[i] == command) {
      str += i;
    } 
  }
  return str;
}
