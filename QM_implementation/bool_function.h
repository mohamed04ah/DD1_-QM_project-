#pragma once
#include<iostream>
#include<string>
#include<vector>
using namespace std;
class bool_function
{
private: 
	string expression;  // to store the function expression as a whole after validation 
	vector<char> literals; // to store function literals after validation. (will be used later) 

	bool_function();
	void validate(); // validate the entered function 
	void gen_table(); // to generate the truth table. (output will be stored in a 2d vector)
	void print_table(); // to print the truth table
	void canonical_pos(); // extract pos form from turth table
	void canonical_sop(); // extract sop form from truth table 
	void P_I();   // identify prime implicants 
	void EPI(); // identify essential prime implicants

};


