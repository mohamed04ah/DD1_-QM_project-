#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<unordered_map>
#include<iomanip>
using namespace std;
struct  ret_type    
{

	bool diff ; //if the difference between two combined binary representations is 1 or not;
	string comb; //contain the binary representation of two combined binary representations
	vector<int> minterm_comb;// contain the minterms (0,1,...) of the combined binary representations
	int num_of_1=0; //the number of ones in the combined binary representation

};
class bool_function
{
public: 
	bool_function(); //constructor
private: 
	
	unordered_map<string, vector<int>> binary_rep_mins; // key is the binary representations and the value is vector of minterms covered by it
	unordered_map<int, bool> minterms;   //keys are minterms (0,1,...) and values are bools expressing if the minterm covered in EPI or not
	string expression;  // to store the function expression as a whole after validation 
	vector<char> literals; // to store function literals after validation.
	vector<vector<bool>> truth_table; 
	unordered_map<string, vector<int>> EPIS; //key is the binary representation of EPI and values are minterms covered by it
	map<char, int> lit; //key is literal (A,B,...) and value is their index in truth table
	int rows=0, size=0; //used to set size of the truth table and print it. Size here is equal to the size of the colomns of truth table -1
	                    //rows equal number of rows
	
	bool validate(); // validate the entered function 
	bool validate_1(string input);        // same implementation of  normal validate function above but is slightly modified to run test cases automatically. 
	void gen_table(); // to generate the truth table. (truth table will be stored in a 2d vector)
	void output_table();//to calculate the output of the truth table
	void print_table(); // to print the truth table 
	void canonical_pos(); // extract pos form from turth table
	void canonical_sop(); // extract sop form from truth table 
	void P_I();   // identify prime implicants 
	void EPI(); // identify essential prime implicants
	ret_type compare_strings(string x, string y);  // x and y are the binary representations(0001,-001,1-1-,...)
	

};


