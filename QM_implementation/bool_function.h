#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<iomanip>
using namespace std;
class bool_function
{
public: 
	bool_function(string input); // will take in the boolean function entered
private: 
	string expression;  // to store the function expression as a whole after validation 
	vector<char> literals; // to store function literals after validation. (will be used later) 
	vector<vector<bool>> truth_table; 
	int rows=0, size=0; // used to set size of the truth table and print it. 

	
	bool validate(string expression); // validate the entered function 
	void gen_table(); // to generate the truth table. (output will be stored in a 2d vector)
	void print_table(); // to print the truth table
	void canonical_pos(); // extract pos form from turth table
	void canonical_sop(); // extract sop form from truth table 
	void P_I();   // identify prime implicants 
	void EPI(); // identify essential prime implicants

};


