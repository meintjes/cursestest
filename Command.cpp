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
    for (int i = 0; i < CMAP_SIZE; i++) {
      if (cmap[i] == com) {
	file << static_cast<unsigned char>(i);
      }
    }
    file << '\n';
  }
}

void changeControl(Command command) {
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
