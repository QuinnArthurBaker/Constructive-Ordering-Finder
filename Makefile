compile:
	g++ -std=c++17 -o bin/coSingle src/constructiveOrderingsSinglethread.cpp
	clang++ -std=c++17  -Wall -lpthread -o bin/coMultiC src/constructiveOrderingsMultithread.cpp
	g++ -std=c++17 -Wall -lpthread -o bin/coMultiG src/constructiveOrderingsMultithread.cpp
	g++ -std=c++17 -o bin/coAdjMatrix src/constructiveOrderingsAdjacencyMatrix.cpp
	clang++ -std=c++17 -o bin/coPrune src/constructiveOrderingsPruneTree.cpp
	clang++ -std=c++2a -lpthread -o bin/coPruneMulti src/constructiveOrderingsPruneMultithread.cpp

clean:
	rm bin/*
