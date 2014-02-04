GXX=g++
GXX_FLAGS=-c -g -Wall -Werror -pedantic -std=c++11 -lncurses

cursestest: main.o Map.o MapGen.o ColoredText.o Space.o Enemy.o Player.o functions.o SpaceType.o Command.o Menu.o Option.o
	$(GXX) -lncurses main.o Map.o MapGen.o ColoredText.o Space.o Enemy.o Player.o functions.o SpaceType.o Command.o Menu.o Option.o -o $@

main.o: main.cpp Map.o Space.o Enemy.o Player.o Command.o Menu.o
	$(GXX) $(GXX_FLAGS) main.cpp

Map.o: Map.cpp Space.o ColoredText.o Player.o functions.o
	$(GXX) $(GXX_FLAGS) Map.cpp

MapGen.o: MapGen.cpp Space.o functions.o
	$(GXX) $(GXX_FLAGS) MapGen.cpp

ColoredText.o: ColoredText.cpp
	$(GXX) $(GXX_FLAGS) ColoredText.cpp

Space.o: Space.cpp Enemy.o SpaceType.o
	$(GXX) $(GXX_FLAGS) Space.cpp

Enemy.o: Enemy.cpp
	$(GXX) $(GXX_FLAGS) Enemy.cpp

Player.o: Player.cpp ColoredText.o functions.o
	$(GXX) $(GXX_FLAGS) Player.cpp

functions.o: functions.cpp
	$(GXX) $(GXX_FLAGS) functions.cpp

SpaceType.o: SpaceType.cpp
	$(GXX) $(GXX_FLAGS) SpaceType.cpp

Command.o: Command.cpp Menu.o
	$(GXX) $(GXX_FLAGS) Command.cpp

Menu.o: Menu.cpp Option.o
	$(GXX) $(GXX_FLAGS) Menu.cpp

Option.o: Option.cpp ColoredText.o
	$(GXX) $(GXX_FLAGS) Option.cpp

clean:
	rm -rf *.o cursestest