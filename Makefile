compile:
	g++ -std=c++11 -o bin/coSingle src/constructiveOrderingsSinglethread.cpp
	clang++ -std=c++11 -lpthread -o bin/coMultiC src/constructiveOrderingsMultithread.cpp
	g++ -std=c++11 -lpthread -o bin/coMultiG src/constructiveOrderingsMultithread.cpp
	g++ -std=c++11 -o bin/coAdjMatrix src/constructiveOrderingsAdjacencyMatrix.cpp

clean:
	rm bin/*
