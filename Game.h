#ifndef __GAME_H__
#define __GAME_H__

#include <string>
#include <vector>
#include "Player.h"
#include "Branch.h"
#include "Command.h"

//Game facilitates the serialization and deserialization of the game as a
//whole, managing the relationship between the player and the branches and such
//in a less-insane way than trying to do it all in playGame

class Game {
 public:
  //construct a new game, numbered sequentially
  Game();

  //construct a game by loading the game with the given id
  Game(unsigned int idIn);

  //plays the game until it's done
  void play();
  
  //save the game to file and quit
  void save();

  //delete the files associated with the game
  void end();

 private:
  std::string getPath() const;
  
  bool getInput(const CommandMap cmap);
  bool getLongPrompt();

  unsigned int id;
  Player you;
  std::vector<Branch> branches;

};

#endif
