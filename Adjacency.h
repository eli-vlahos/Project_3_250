/*
 * QuadTree.h
 *
 *  Created on: Oct. 28, 2020
 *      Author: elicv
 */

#ifndef Adjacency_H_
#define Adjacency_H_

#include <iostream>
#include <vector>
#include <limits>
#include "Struct.h"



class Adjacency {


private:

	int n_nodes;
	double** adj;



public:
	Adjacency();
	~Adjacency();

	void direct(std::vector<std::string> line);
	std::vector<std::string> vector_cleaner(std::vector<std::string>);
	void initializer(int const &size);
	void MST();
	void clear();

	static const double INF;

};

#endif /* Adjacency_H_ */
