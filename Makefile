all:	kruskaldriver	
kruskaldriver:	kruskaltest.cpp
	g++	-std=c++11	-o	kruskaldriver	kruskaltest.cpp	Adjacency.cpp
clean:	
	rm	kruskaldriver