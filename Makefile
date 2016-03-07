CXX = g++
CXXFLAGS = -g -Wall -Werror -Wextra -pedantic -std=c++11 -Wno-char-subscripts
LD_FLAGS = -lncurses

TAG_FILES = $(shell find . -type f -name '*.h' -exec grep 'CREATE_TAG_FOR' -l {} \;)
TAG_HEADER = getPointerFromArchive.h
TAG_GEN = ./regenerateUnserializer.sh

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

cursestest: $(OBJECTS)
	$(CXX) $(LD_FLAGS) $(OBJECTS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

generation.o: $(TAG_HEADER)

$(TAG_HEADER): $(TAG_FILES) $(TAG_GEN)
	$(TAG_GEN) >$(TAG_HEADER)

clean:
	rm -rf *.o cursestest $(TAG_HEADER)

Archive.o: Archive.cpp Archive.h
Artifact.o: Artifact.cpp Artifact.h Cst.h Cch.h Item.h Archive.h Color.h \
  functions.h Map.h Space.h Point.h Enemy.h Player.h
Branch.o: Branch.cpp Map.h Space.h Point.h Cch.h Enemy.h Archive.h \
  Branch.h
Cch.o: Cch.cpp Cch.h Color.h Cst.h
Command.o: Command.cpp Command.h Option.h Cst.h Color.h Cch.h
Cst.o: Cst.cpp Cst.h
Enemy.o: Enemy.cpp Enemy.h Archive.h Point.h Player.h Cch.h Color.h Cst.h \
  Map.h Space.h Item.h functions.h
Game.o: Game.cpp Game.h Player.h Cch.h Point.h Branch.h Command.h \
  Option.h Cst.h Map.h Space.h Enemy.h Archive.h Color.h
Item.o: Item.cpp Item.h Archive.h Player.h Cch.h Point.h Color.h Cst.h \
  Map.h Space.h Enemy.h functions.h
Map.o: Map.cpp Archive.h Map.h Space.h Point.h Cch.h Enemy.h Color.h \
  Cst.h Player.h functions.h Command.h Option.h generation.h
MapGen.o: MapGen.cpp Map.h Space.h Point.h Cch.h Enemy.h Archive.h Item.h \
  functions.h generation.h
Menu.o: Menu.cpp Menu.h Option.h Cst.h Cch.h Color.h
Option.o: Option.cpp Option.h Cst.h Color.h Cch.h
Player.o: Player.cpp Player.h Cch.h Point.h functions.h Map.h Space.h \
  Enemy.h Archive.h Branch.h Item.h Weapon.h Artifact.h Cst.h Color.h \
  generation.h
Point.o: Point.cpp Point.h
Space.o: Space.cpp Space.h Point.h Cch.h Archive.h generation.h Item.h \
  Enemy.h Color.h Cst.h functions.h
Weapon.o: Weapon.cpp Color.h Cst.h Cch.h Weapon.h Item.h Archive.h \
  Player.h Point.h Map.h Space.h Enemy.h
functions.o: functions.cpp functions.h
generation.o: generation.cpp generation.h Archive.h functions.h Item.h \
  Artifact.h Cst.h Cch.h Weapon.h Enemy.h Point.h \
  getPointerFromArchive.h
main.o: main.cpp Map.h Space.h Point.h Cch.h Enemy.h Archive.h Game.h \
  Player.h Branch.h Command.h Option.h Cst.h Color.h Menu.h
