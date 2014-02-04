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

void writeControls(std::string filename, CommandMap cmap) {
  std::ofstream file;
  file.open(filename);

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
  if (readControls("controls.txt", cmap)) {
    move(11, 26);
    addstr("Press a key.");
    unsigned char ch = getch();
    if (ch < CMAP_SIZE) {
      cmap[ch] = command;
      writeControls("controls.txt", cmap);
    }
  }
  else {
    addstr("Failed to load controls.");
  }
}
