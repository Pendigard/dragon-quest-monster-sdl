all: bin/program

clean:
	rm -f bin/* obj/*

bin/program: obj/GameTxt.o obj/Player.o obj/Monster.o
	g++ obj/GameTxt.o obj/Player.o obj/Monster.o -o bin/program

obj/GameTxt.o: src/GameTxt.cpp src/GameTxt.h src/Player.h 
	g++ -Wall -c src/GameTxt.cpp -o obj/GameTxt.o

obj/Player.o: src/Player.cpp src/Player.h src/Monster.h
	g++ -Wall -c src/Player.cpp -o obj/Player.o

obj/Monster.o: src/Monster.cpp src/Monster.h
	g++ -Wall -c src/Monster.cpp -o obj/Monster.o -I/usr/local/include/rapidjson
