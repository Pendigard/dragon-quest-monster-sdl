all: bin/game_text bin/game

clean:
	rm -f bin/* obj/*

bin/game: obj/Game.o obj/Player.o obj/Monster.o obj/Fight.o obj/jsonFunction.o obj/Synthesis.o obj/Database.o obj/Sprite.o obj/Menu.o obj/FightSDL.o
	g++ obj/Game.o obj/Player.o obj/Monster.o obj/jsonFunction.o obj/Fight.o obj/Synthesis.o obj/Database.o obj/Sprite.o obj/Menu.o obj/FightSDL.o -o bin/game -lSDL2 -lSDL2_image -lSDL2_ttf

bin/game_text: obj/GameTxt.o obj/Player.o obj/Monster.o obj/Fight.o obj/jsonFunction.o obj/Synthesis.o obj/Database.o
	g++ obj/GameTxt.o obj/Player.o obj/Monster.o obj/jsonFunction.o obj/Fight.o obj/Synthesis.o obj/Database.o -o bin/game_text

obj/GameTxt.o: src/GameTxt.cpp src/GameTxt.h src/Player.h src/Fight.h src/Monster.h src/Synthesis.h src/Database.h
	g++ -Wall -c src/GameTxt.cpp -o obj/GameTxt.o

obj/Game.o: src/Game.cpp src/Game.h src/Player.h src/Fight.h src/Monster.h src/Synthesis.h src/Database.h src/Sprite.h src/Menu.h
	g++ -Wall -c src/Game.cpp -o obj/Game.o

obj/Player.o: src/Player.cpp src/Player.h src/Monster.h
	g++ -Wall -c src/Player.cpp -o obj/Player.o

obj/Monster.o: src/Monster.cpp src/Monster.h src/jsonFunction.h src/Database.h
	g++ -std=c++11 -Wall -c src/Monster.cpp -o obj/Monster.o

obj/Fight.o: src/Fight.cpp src/Fight.h src/Player.h src/Monster.h
	g++ -Wall -c src/Fight.cpp -o obj/Fight.o

obj/jsonFunction.o: src/jsonFunction.cpp src/jsonFunction.h
	g++ -Wall -c src/jsonFunction.cpp -o obj/jsonFunction.o -Iextern/rapidjson-master/include/rapidjson

obj/Synthesis.o: src/Synthesis.cpp src/Synthesis.h src/Monster.h
	g++ -Wall -c src/Synthesis.cpp -o obj/Synthesis.o

obj/Database.o: src/Database.cpp src/Database.h src/jsonFunction.h
	g++ -Wall -c src/Database.cpp -o obj/Database.o

obj/Sprite.o: src/Sprite.cpp src/Sprite.h
	g++ -Wall -c src/Sprite.cpp -o obj/Sprite.o -ISDL2

obj/Menu.o: src/Menu.cpp src/Menu.h src/jsonFunction.h
	g++ -Wall -c src/Menu.cpp -o obj/Menu.o -ISDL2

obj/FightSDL.o: src/FightSDL.cpp src/FightSDL.h src/Fight.h src/Sprite.h src/Menu.h
	g++ -Wall -c src/FightSDL.cpp -o obj/FightSDL.o -ISDL2