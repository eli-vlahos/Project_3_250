/*
 * openhttest.cpp
 *
 *  Created on: Oct. 5, 2020
 *      Author: elicv
 */

#include <iostream>
#include <vector>
#include <regex>
#include <math.h>
#include <cmath>
#include "Adjacency.h"


using namespace std;


int main()
{
	//initialize a quad tree for later use

	Adjacency m1;

	//use regex to initialize a string vector to parse input
	std::regex ws_re("\\s+");

	std::string cmdline;
	getline(std::cin, cmdline);

	//get each line of input and parse while the file hasn't ended
	while(! std::cin.eof()) {



		//if exit is read, program ends
		if (cmdline == "exit") {
			return 0;
		}


		//parse and then pass information to program
		std::vector<std::string> line {
			std::sregex_token_iterator(cmdline.begin(), cmdline.end(), ws_re, -1), {}
		};

		m1.direct(line);


		//get new line
		getline(std::cin, cmdline);


	}


	return 0;

}







