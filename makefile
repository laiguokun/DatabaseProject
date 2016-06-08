main: main.cpp common.h kdtree.h kdtree.cpp common.cpp
	g++-5 -std=c++11 main.cpp common.cpp kdtree.cpp -o main -g
pusu: pusu.cpp common.h common.cpp
	g++-5 -std=c++11 pusu.cpp common.cpp -o pusu -g
clean: 
	rm main
	rm pusu