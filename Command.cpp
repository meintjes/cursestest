#include "Command.h"
#include "Color.h"
#include <fstream>
#include <ncurses.h>

bool readControls(CommandMap cmap) {
  std::ifstream file;
  file.open("controls.txt");
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

void writeControls(CommandMap cmap) {
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

  cmap['a'] = COMMAND_EVOKE_ARTIFACT,
  cmap['i'] = COMMAND_USE_ITEM,
  cmap['d'] = COMMAND_DROP_ITEM,

  cmap['<'] = COMMAND_INTERACT_STAIRSUP;
  cmap['>'] = COMMAND_INTERACT_STAIRSDOWN;

  cmap['#'] = COMMAND_LONG_PROMPT;

  writeControls(cmap);
}
