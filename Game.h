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

const unsigned int ARBITRARY_SAVED_GAMES_LIMIT = 100;

class Game {
 public:
  //check whether the given ID is currently in use by a saved game.
  static bool existsID(unsigned int id);

  //find the lowest ID not currently in use by any saved game.
  static unsigned int findUnusedID();

  //construct a new game with an id that doesn't collide with any saves
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

  //get a single key of input from a player, interpret the key according to
  //the provided cmap, execute the command and return whether it took a turn 
  bool getInput(const CommandMap cmap);

  //prompt for and interpret multi-character commands. like getInput, returns
  //whether the given command took a turn or not
  bool getLongPrompt();

  unsigned int id;
  Player you;
  std::vector<Branch> branches;

};

#endif
