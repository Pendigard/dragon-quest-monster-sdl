all: bin/program

clean:
	rm -f bin/* obj/*

bin/program: obj/GameTxt.o obj/Player.o obj/Monster.o obj/Fight.o obj/jsonFunction.o obj/Synthesis.o obj/database.o
	g++ obj/GameTxt.o obj/Player.o obj/Monster.o obj/jsonFunction.o obj/Fight.o obj/Synthesis.o obj/database.o -o bin/program

obj/GameTxt.o: src/GameTxt.cpp src/GameTxt.h src/Player.h src/Fight.h src/Monster.h src/Synthesis.h src/database.h
	g++ -Wall -c src/GameTxt.cpp -o obj/GameTxt.o

obj/Player.o: src/Player.cpp src/Player.h src/Monster.h
	g++ -Wall -c src/Player.cpp -o obj/Player.o

obj/Monster.o: src/Monster.cpp src/Monster.h src/jsonFunction.h src/database.h
	g++ -std=c++11 -Wall -c src/Monster.cpp -o obj/Monster.o

obj/Fight.o: src/Fight.cpp src/Fight.h src/Player.h src/Monster.h
	g++ -Wall -c src/Fight.cpp -o obj/Fight.o

obj/jsonFunction.o: src/jsonFunction.cpp src/jsonFunction.h
	g++ -Wall -c src/jsonFunction.cpp -o obj/jsonFunction.o -Iextern/rapidjson-master/include/rapidjson

obj/Synthesis.o: src/Synthesis.cpp src/Synthesis.h src/Monster.h
	g++ -Wall -c src/Synthesis.cpp -o obj/Synthesis.o

obj/database.o: src/database.cpp src/database.h src/jsonFunction.h
	g++ -Wall -c src/database.cpp -o obj/database.o