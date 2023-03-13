all: a.out

a.out: main.cpp game.cpp game.hpp cell.cpp cell.hpp score.cpp score.hpp scoreboard.cpp scoreboard.hpp
	g++ -g -Wall -std=c++20 main.cpp game.cpp cell.cpp score.cpp scoreboard.cpp

clean:
	rm a.out
