/*
 * QuadTree.cpp
 *
 *  Created on: Oct. 28, 2020
 *      Author: elicv
 */

//include necessary libraries
#include "Adjacency.h"
#include "Struct.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <regex>
#include <math.h>
#include <cmath>
#include <iomanip>
#include <algorithm>

//constant for infinity, to fill the adjacency
const double Adjacency::INF = std::numeric_limits<double>::infinity();

// use a function indepedent of class for the sort function. Had errors including it with the class
bool function(Struct const &i, Struct  const &j){

	return (i.weight<j.weight);
}


//class is initially empty when initialized
Adjacency::Adjacency() {

	n_nodes = 0;
	adj = nullptr;

}

//clear function already exists, just call it
Adjacency::~Adjacency() {

	if (this->n_nodes !=0){
	for (int i = 0; i< this->n_nodes; i++){
		delete [] this->adj[i];
	}

	delete [] this->adj;

	this->n_nodes = 0;
	}

}

//function that directs string input
void Adjacency::direct(std::vector<std::string> line) {

	//has a sorting based on input


	if(line[0] == "n") {

		//takes second input as size
		int size = stoi(line[1]);

		//checks to make sure size is appropriate
		if (size > 0){
			this->initializer(size);
			std::cout<<"success"<<std::endl;
		}

		else {
			//otherwise invalid argument
			std::cout<<"invalid argument"<<std::endl;
		}
	}
	else if(line[0] == "i"){

		//takes vector, cleans it and takes input
		line = vector_cleaner(line);
		int x = stoi(line[0]);
		int y = stoi(line[1]);

		//checks to make sure input was valid
		if (stod(line[2]) > 0.0 && x < this->n_nodes && x >= 0 && y >= 0 && y < this->n_nodes){

			//adds to adjacency matrix
			this->adj[x][y] = stod(line[2]);
			this->adj[y][x] = stod(line[2]);
			std::cout<<"success"<<std::endl;
		}
		else{
			std::cout<<"invalid argument"<<std::endl;
		}

	}
	else if (line[0] == "d"){

		line = vector_cleaner(line);

		int x = stoi(line[0]);
		int y = stoi(line[1]);

		//checks to see if the input is valid
		if (x >= this->n_nodes || y >= this->n_nodes || y < 0 || x < 0){
			std::cout << "invalid argument" <<std::endl;
		}
		//checks to see if values are already zero
		else if (adj[x][y] == 0 || adj[x][y] == INF){
			std::cout << "failure" <<std::endl;
		}
		//deletes it
		else{
			if (x == y){
			this->adj[x][y] = 0;
			}
			else{
			this->adj[x][y] = this->INF;
			this->adj[y][x] = this->INF;
			}
			std::cout<< "success"<<std::endl;
		}

	}
	else if (line[0] == "degree"){

		int y = stoi(line[1]);
		int degree = 0;

		//makes sure input is valid
		if (y < this->n_nodes && y >= 0){
			//runs a loop for however large the size of the matrix is
			for (int i = 0; i < this->n_nodes; i++){
				//counts every time there is a new degree
				if (this->adj[y][i] != INF and this->adj[y][i] != 0){

					if (y == i){
						degree = degree +2;
					}
					else {
						degree++;
					}
				}
			}
			std::cout<< "degree of "<<y<< " is "<< degree <<std::endl;
		}
		else{
			std::cout<<"invalid argument"<<std::endl;
		}

	}
	else if (line[0] == "edge_count"){

		int count = 0;
		int count_diagonal = 0;

		for (int i = 0; i < this->n_nodes; i++){

			for (int j = 0; j < this->n_nodes; j++){

				if (this->adj[i][j] != INF && this->adj[i][j] != 0) {
					if (j == i){
						count_diagonal++;
					}
					else{
						count++;
					}
				}

			}
		}

		//dividing by two because edges were counted twice in above algorithm
		count = count/2;
		//edges along the diagonal count twice
		count_diagonal = count_diagonal*2;

		int total_count = count + count_diagonal;

		std::cout<< "edge count is " << total_count <<std::endl;

	}
	else if (line[0] == "clear"){

		//clears it
		if (this->n_nodes != 0){
		this->clear();
		}
		std::cout<<"success"<<std::endl;

	}
	else if (line[0] == "mst"){

		//calls mst function
		this->MST();

	}
}

void Adjacency::initializer(int const &size){

	//initializes the size and matrix constants. The diagonal is full of zeros, there are zeros everywhere else
	this->n_nodes = size;
	this->adj = new double * [n_nodes];

	for (int i = 0; i< n_nodes; i++){
		this->adj[i] = new double[n_nodes];
	}

	for (int i = 0; i < n_nodes; i++){
		for (int j = 0; j < n_nodes; j++){
			this->adj[i][j] = this->INF;
		}

		this->adj[i][i] = 0.0;
	}

}

std::vector<std::string> Adjacency::vector_cleaner(std::vector<std::string> line) {

	//cleans the input seperating by semi colons, white spaces are already eliminated

	line.erase(line.begin());
	std::regex comma(";");

	std::vector<std::string> line_update {
		std::sregex_token_iterator(line[0].begin(), line[0].end(), comma, -1), {}
	};

	return line_update;
}

void Adjacency::clear(){

	// goes through every space of the array and clears it
	if (this->n_nodes != 0){
		for (int i = 0; i < this->n_nodes; i++){

			for (int j = 0; j < this->n_nodes; j++){

				if (j == i){
				this->adj[i][j] = 0;
				}
				else {
					this->adj[i][j] = INF;
				}
			}
		}
	}


}

void Adjacency::MST(){

	int results[this->n_nodes][this->n_nodes];
	//this one dimensional array stores the location of each node relative to a row
	int location[this->n_nodes];

	std::cout<<std::fixed; //setting output to fixed early

	// uses a vector to represent the various weights and their respective coordinates
	std::vector<Struct> weights;

	//initializes matrix to store adjacent values
	//idea is to store a sum of the weights, and then add two connected nodes in the same array row.
	//if two nodes are in the same matrix it means they are connected
	//once one row is full the process is done. Each weight has been added
	for (int i = 0; i < this->n_nodes; i++){
		//do -i to make sure element isn't inserted twice
		for (int j = i; j < this->n_nodes; j++) {

			if (adj[i][j] != INF && adj[i][j] != 0.0){

				Struct line;
				//this is initializing the vector of weights
				line.row = i;
				line.column = j;
				line.weight = adj[i][j];

				weights.push_back(line);
			}

		}

		//kill two birds with one stone, initialize the storage vector as well
		location[i] = i;
	}

	//sorts the matrix full of weights
	std::sort (weights.begin(), weights.end(), function);


	for (int i = 0; i < this->n_nodes; i++){

		//initalizes matrix to store values
		for (int j = 0; j < this->n_nodes; j++){
			if (j == i){
				results[i][j] = i;
			}
			else {
				results[i][j] = -1;
			}
		}
	}

	double sum = 0;
	int count = 0;

	//runs for every single element inside the vector full of weights
	for (int i = 0; i < weights.size(); i++) {

		int x = weights[i].row;
		int y = weights[i].column;

		int x_row = location[x];
		int y_row = location[y];


		if (x_row != y_row){
			count++;

			sum = sum + weights[i].weight;


			for (int j = 0; j < this->n_nodes; j++){
				//merges the two rows together and fills one of the rows with -1
				if (results[y_row][j] != -1){

					results[x_row][j] = j;
					results[y_row][j] = -1;

					//change the location of the y element
					location[j] = location[x];

				}
			}

		}

	}

	if (count >= this->n_nodes-1){
		std::cout.precision(2);
		std::cout<< "mst " <<sum<<std::endl;
	}
	else {
		std::cout<<"not connected"<<std::endl;
	}

}
