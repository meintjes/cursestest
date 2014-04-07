GXX=g++
GXX_FLAGS=-c -g -Wall -Werror -pedantic -std=c++11 -lncurses -pipe

cursestest: main.o Map.o MapGen.o Cst.o Cch.o Space.o Enemy.o Player.o functions.o Command.o Menu.o Option.o Point.o Item.o Artifact.o Weapon.o generation.o Branch.o Archive.o Game.o
	$(GXX) -lncurses main.o Map.o MapGen.o Cst.o Cch.o Space.o Enemy.o Player.o functions.o Command.o Menu.o Option.o Point.o Item.o Artifact.o Weapon.o generation.o Branch.o Archive.o Game.o -o $@

less:
	make 2>&1 | less

main.o: main.cpp Map.o Space.o Enemy.o Player.o Command.o Menu.o Branch.o Game.o
	$(GXX) $(GXX_FLAGS) main.cpp

Map.o: Map.cpp Space.o Cch.o Player.o functions.o Point.o Archive.o
	$(GXX) $(GXX_FLAGS) Map.cpp

MapGen.o: MapGen.cpp Space.o functions.o generation.o
	$(GXX) $(GXX_FLAGS) MapGen.cpp

Cst.o: Cst.cpp
	$(GXX) $(GXX_FLAGS) Cst.cpp

Cch.o: Cch.cpp
	$(GXX) $(GXX_FLAGS) Cch.cpp

Space.o: Space.cpp Enemy.o generation.o functions.o
	$(GXX) $(GXX_FLAGS) Space.cpp

Enemy.o: Enemy.cpp Point.o Map.o functions.o generation.o Player.o Archive.o
	$(GXX) $(GXX_FLAGS) Enemy.cpp

Player.o: Player.cpp Cst.o Cch.o functions.o Item.o Branch.o Archive.o generation.o Map.o
	$(GXX) $(GXX_FLAGS) Player.cpp

functions.o: functions.cpp
	$(GXX) $(GXX_FLAGS) functions.cpp

Command.o: Command.cpp Menu.o
	$(GXX) $(GXX_FLAGS) Command.cpp

Menu.o: Menu.cpp Option.o Cst.o
	$(GXX) $(GXX_FLAGS) Menu.cpp

Option.o: Option.cpp Cst.o
	$(GXX) $(GXX_FLAGS) Option.cpp

Point.o: Point.cpp
	$(GXX) $(GXX_FLAGS) Point.cpp

Item.o: Item.cpp Cch.o Player.o functions.o Map.o
	$(GXX) $(GXX_FLAGS) Item.cpp

Artifact.o: Artifact.cpp Player.o Cst.o Cst.o Item.o generation.o
	$(GXX) $(GXX_FLAGS) Artifact.cpp

Weapon.o: Weapon.cpp Item.o Map.o Point.o generation.o
	$(GXX) $(GXX_FLAGS) Weapon.cpp

generation.o: getPointerFromArchive.h generation.cpp Item.o Enemy.o Weapon.o Artifact.o functions.o
	./regenerateUnserializer.sh >getPointerFromArchive.h
	$(GXX) $(GXX_FLAGS) generation.cpp

Branch.o: Branch.cpp Map.o
	$(GXX) $(GXX_FLAGS) Branch.cpp

Archive.o: Archive.cpp
	$(GXX) $(GXX_FLAGS) Archive.cpp

Game.o: Game.cpp Player.o Branch.o Archive.o Cst.o Command.o
	$(GXX) $(GXX_FLAGS) Game.cpp


clean:
	rm -rf *.o cursestest
