GXX=g++
GXX_FLAGS=-c -g -Wall -Werror -pedantic -std=c++11 -lncurses

cursestest: main.o Map.o MapGen.o Cch.o Cst.o Space.o Enemy.o Player.o functions.o SpaceType.o Command.o Menu.o Option.o
	$(GXX) -lncurses main.o Map.o MapGen.o Cch.o Cst.o Space.o Enemy.o Player.o functions.o SpaceType.o Command.o Menu.o Option.o -o $@

main.o: Map.o Space.o Enemy.o Player.o Command.o Menu.o
	$(GXX) $(GXX_FLAGS) main.cpp

Map.o: Space.o Cch.o Player.o functions.o
	$(GXX) $(GXX_FLAGS) Map.cpp

MapGen.o: Space.o functions.o
	$(GXX) $(GXX_FLAGS) MapGen.cpp

Cch.o:
	$(GXX) $(GXX_FLAGS) Cch.cpp

Cst.o:
	$(GXX) $(GXX_FLAGS) Cst.cpp

Space.o: Enemy.o SpaceType.o
	$(GXX) $(GXX_FLAGS) Space.cpp

Enemy.o:
	$(GXX) $(GXX_FLAGS) Enemy.cpp

Player.o: Cch.o functions.o
	$(GXX) $(GXX_FLAGS) Player.cpp

functions.o:
	$(GXX) $(GXX_FLAGS) functions.cpp

SpaceType.o:
	$(GXX) $(GXX_FLAGS) SpaceType.cpp

Command.o:
	$(GXX) $(GXX_FLAGS) Command.cpp

Menu.o: Option.o
	$(GXX) $(GXX_FLAGS) Menu.cpp

Option.o: Cst.o
	$(GXX) $(GXX_FLAGS) Option.cpp

clean:
	rm -rf *.o cursestest


stupid:
	g++ *.cpp -Wall -Werror -pedantic -std=c++11 -lncurses -o cursestest