#include "bool_function.h"

bool_function::bool_function() {

	bool res;
	res=validate();
	while (res!= true) 
	{
		cout << "Invalid boolean expression! Try again." << endl;
		literals.clear();
		lit.clear();
		res=validate();
	}
	cout << "valid function entered" << endl;
	gen_table();
	output_table();
    print_table();
	P_I();
	EPI();
	//canonical_sop();
	//canonical_pos();
	
}
bool bool_function::validate() 
{
	bool flat = false;
	string input;
	cout << "enter your function" << endl;
	getline(cin, input);
		for (int i = 0; i < input.size(); i++)
		{
			if (isalpha(input[i]))  //if it is literal
			{
				lit[input[i]] = 0;
				flat = true;
			}
			else if (input[i] == '+' && (i == 0 || i == input.size() - 1 || input[i - 1] == '+')) { //if '+' at the beginning or at the end or not between two literals
				flat = false;
				break;
			}
			else if (input[i] == '`' && i == 0) { //'if '`' at the beginning
				flat = false;
				break;
			}
			else if (input[i] == '`' && input[i - 1] == '+' && i != 0)  //check if the + and ` come together after each other in this way
			{ 
				flat = false;
				break;
			}
			else if (input[i] != '+' && input[i] != '`' && !isalpha(input[i])) //check if any other useless operator or number were put
			{
				cout << "Error: Invalid character '" << input[i] << "' found in input." << endl;
				flat = false;
				break;
			}
			else  //otherwise it is definitley true
				flat = true;
		}
	
		if (flat == false) {
			return flat;
		}

	int count = 0;
	for (auto it = lit.begin(); it != lit.end(); it++) {
		it->second = count++;  //this is to give every literal an index as it will appear in the truth table (A->0, B->1 ,...)
	}

	for (auto it = lit.begin(); it != lit.end(); it++) 
	{
		literals.push_back(it->first);  
	}
	expression = input;
	string temp = "";
	for (int i = 0; i < expression.size(); i++)
	{
		
		if (expression[i] == '`' )
		{
			
			int counter = 1;
			int j = i + 1;
			while (expression[j] == '`' && j < expression.size())
			{
				counter++;
				j++;
			}
			
			if (counter % 2 != 0) 
			{
				
				temp += '`';
				
			}
			
			i = j-1;
			
		}
		else {
			temp += expression[i];
		}
	}
	expression = temp;
	cout << "the expression" << " " << expression << endl;
	return flat;
}
void bool_function::gen_table()
{
	 size = literals.size();  // contains number of columns without output
	 rows = pow(2, size);
	vector<vector<bool>> temp_table(rows, vector<bool>(size+1));
	for (int i = 0; i < size; i++)  // for each column of the table
	{
		int flip = pow(2, size - (i + 1)); // sequence of TRUE AND FALSE
		
		for (int j = 0; j < rows; j++)  //for the rows of each column
		{
			
			if ((j/flip)%2==0)    // this is to identify the boolean of the rows in the column. 
			{                        
				temp_table[j][i] = false;  
			}
			else
				temp_table[j][i] = true; 
		}
	}

	truth_table = temp_table;
}

void bool_function::output_table()
{
	for (int i = 0; i < rows; i++) //looping over every row in the table
	{ 
		bool result = true; //result will multiply the multipliers like abc until it find + 

		for (int j = 0; j < expression.size(); j++) //looping over the expression (like a`b+cd)
		{ 
			char c = expression[j];
			if (c == '`')
				continue;
			if (j == expression.size() - 1) {
				result = result * truth_table[i][lit[c]];  //lit[c] will decide which colomn exactly has the value that we want
				break;
			}

			if (c != '+' && expression[j + 1] != '`') 
				result = result * truth_table[i][lit[c]];   //lit[c] this represents which colomn in the row i

			else if (c != '+' && expression[j + 1] == '`') 
				result = result *!truth_table[i][lit[c]];

			else if (c == '+')
			{
				if (result == true) // break here becasue if it is true then the output is true because 1+ anything is true
					break;
				else
					result = true; //set it equal true to start a new multiplier after the + 
			}
		}

		truth_table[i][size] = result;  
	}
}


void bool_function::print_table()
{
	cout << setw(7);
	for (int i = 0; i < size; i++)
	{
		cout << literals[i] << setw(3);
	}
	cout << "F";
	cout << endl;
	for (int i = 0; i < size; i++) {
		cout << "------";
	}
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		cout << setw(3) << i << ":";
		for (int j = 0; j < size + 1; j++)
		{
			cout << setw(3) << truth_table[i][j] << setw(3);
		}
		cout << endl;
	}
}

void bool_function::canonical_sop() {// take the output of the function, the table of a,b,..., the literals
	string sop_result = "";
	for (int i = 0; i < rows; i++) {
		if (truth_table[i][size] == true) { //if the output of the function equal one(which is in the last colomn)
			for (int j = 0; j < size; j++) { //will loop over the colomns (literals) and will see the value of each literal
				if (truth_table[i][j] == false) { //if it is equal zero then we put ' for representing (Not) in literals
					sop_result += literals[j];
					sop_result += "`";
				}
				else  //otherwise we only add the literal to the expression
					sop_result += literals[j];
			}
			sop_result += "+";//this adddition is added between the products
		}
	}
	sop_result.pop_back(); //becasue there will be (+) added at the end of the expression so we remove it
	cout << "The Canonical Sum of Products: " << sop_result << endl;
}

void bool_function::P_I()
{
	map<int, vector<string>> implication_table; //key is the number of ones, values are vectors of binary representations having this number of ones
	
	for (int i = 0; i < rows; i++)
	{
		string temp_string = "";
		int counter = 0;
		if (truth_table[i][size] == 1)
		{
			minterms[i] = true; 
			for (int j = 0; j < size; j++)
			{
				if (truth_table[i][j] == 1)
				{
					temp_string += "1";
					counter++;
				}
				else
					temp_string += "0";
			}
			implication_table[counter].push_back(temp_string);
			binary_rep_mins[temp_string].push_back(i);
		}
	}

	// this ends the grouping of the first stage
	

	map<int, vector<string>> map_temp;  //key is the number of ones, values are vectors of binary representations having this number of ones(0001,0101,...)
	unordered_map<string, bool> check_map; //key is the binary representation, value is to chech if this binary get combined or not
	vector<string>primes; //to store the prime implicants 
	while (!implication_table.empty())
	{
		map_temp = implication_table;
		implication_table.clear();
		
		
		if (map_temp.size()==1)   // only 1 group in the implication table. 
		{
			for (auto it = map_temp.begin(); it != map_temp.end(); it++)
			{
				for (auto x : it->second)
					check_map[x] = false;  // this indicates that this implicant is prime. 
			}
		}
		else
		{
			for (auto it = map_temp.begin(); it != map_temp.end(); it++)
			{
				if (it == --map_temp.end())   // to avoid out of bound error. 
					break;

				vector <string> temp = (*it).second; // first group to be compared

				map<int, vector<string>> ::iterator it2;
				it2 = ++it;
				vector <string> temp_1 = (*it2).second;  // second group to be compared 
				--it;

				for (int i = 0; i < temp.size(); i++)
				{                                                      // nested loops compare each string in the 2 groups. 
					for (int j = 0; j < temp_1.size(); j++)
					{
						ret_type result;
						result = compare_strings(temp[i], temp_1[j]);
						if (result.diff) // combination occured
						{
							check_map[temp[i]] = true;  // not prime
							check_map[temp_1[j]] = true; // not prime
							binary_rep_mins[result.comb] = result.minterm_comb;   // binary representaion of the combined minterms, and the minterms covered by both minterms 
							implication_table[result.num_of_1].push_back(result.comb);  // forms a new group. 
						}
						else
						{
							if (check_map[temp[i]] != true)
								check_map[temp[i]] = false;  // this indicates its a prime implicant
							if (check_map[temp_1[j]] != true)
								check_map[temp_1[j]] = false;
						}
					}
				}
			}
		}

			for (auto it = check_map.begin(); it != check_map.end(); it++)
			{
				if (it->second == false)
					primes.push_back(it->first);  // if the minterms were not combined, add to the primes vector
			}
			check_map.clear();
	}

	unordered_map<string, vector<int>> final_PI; //key is the binary representation(0-01,...) of prime implicants and values are minterms covered by them
	
	for (int i = 0; i < primes.size(); i++)
		final_PI[primes[i]] = binary_rep_mins[primes[i]];  //key =binary rep of prime implicants. // value= the minterms covered. 

	//printing the final prime implicants
	cout << "==============Prime implicants===========" << endl;
	for (auto it = final_PI.begin(); it != final_PI.end(); it++)
	{
		cout << it->first << " " << ":";
		for (auto x : it->second)
			cout << x << " " << ",";
		cout << endl;
	}

	binary_rep_mins = final_PI;
}

	

void bool_function::EPI()
{
	unordered_map <int, vector<string>>temp_map;  //key is the minterm, value is a vector of binary representations covering this minterm
	unordered_map<string, vector<int>> copy_EPIS;  //key is the EPIS in boolean expression(AD,BC,...), and value is the minterms covered by them 
	for (auto it = binary_rep_mins.begin(); it != binary_rep_mins.end(); it++)
	{                                                                           // loop over prime implicants and push them into the temp map 
		for (auto x : it->second)
			temp_map[x].push_back(it->first);
	}

	for (auto it = temp_map.begin(); it != temp_map.end(); it++)
	{
		if (it->second.size() == 1)   // indicates that this minterms is only covered by 1 implicant (i.e the implicant is essential)
			EPIS[it->second[0]] = binary_rep_mins[it->second[0]];
	}

	for (auto it = EPIS.begin(); it != EPIS.end(); it++)
	{
		string temp = "";
		for (int i = 0; i < it->first.size(); i++)
		{

			if (it->first[i] == '1')
			{
				temp += literals[i];
			}
			else if (it->first[i] == '0')
			{
				temp += literals[i];
				temp += '`';
			}
		}
		copy_EPIS[temp] = it->second;
	}
	cout << endl;

	for (auto it = copy_EPIS.begin(); it != copy_EPIS.end(); it++)
	{
		for (auto x : it->second)
			minterms[x] = false;
	}

	for (auto it = copy_EPIS.begin(); it != copy_EPIS.end(); it++)
	{
		cout << it->first << " ";
		for (auto x : it->second)
			cout << x << "  ";
		cout << endl;
	}
	cout << endl;
	cout << "mintermd" << endl;
	for (auto it = minterms.begin(); it != minterms.end(); it++) {
		cout << it->first << " ";
	}
	cout << endl;
	cout << "not covered" << endl;
	for (auto it = minterms.begin(); it != minterms.end(); it++) {
		if (it->second == true)
			cout << it->first << " ";
	}
}


void bool_function::canonical_pos() {// take the output of the function, the table of a,b,.., the literals
	string pos_result = "";
	for (int i = 0; i < rows; i++) {
		if (truth_table[i][size] == false) { //if the output of the function equal zero
			pos_result += "(";   //put every n literals added between prackets
			for (int j = 0; j < size; j++) { //will loop over the colomns (literals) and will see the value of each literal
				if (truth_table[i][j] == true) { //if it is equal one then we put ' for representing (Not) in literals
					pos_result += literals[j];
					pos_result += "`";
				}
				else //otherwise we only add the literal to the expression
					pos_result += literals[j];
				pos_result += "+"; //between the literals we add (+)
			}
		}
		pos_result.pop_back(); // there will be additional (+) added at the end of every n literals added between prackets so we remove it
		pos_result += ")";
	}
	cout << "The Canonical Product of Sums: " << pos_result << endl;
}


ret_type bool_function::compare_strings(string x, string y)
{
	int counter = 0;
	ret_type value;
	value.num_of_1 = 0;
	string temp;
	int pos = -1;
	for (int i = 0; i < x.size(); i++)
	{
		if (x[i] != y[i])
		{
			counter++;
			pos = i;
		}
		if (counter > 1)
		{
			value.diff = false;
			return value;
		}
	}
	if (counter == 1)
	{
		temp = x;
		temp[pos] = '-';
		value.comb = temp;
		value.diff = true;

	}

	for (auto it : binary_rep_mins[x])
		value.minterm_comb.push_back(it);

	for (auto it : binary_rep_mins[y])
		value.minterm_comb.push_back(it);

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] == '1')
			value.num_of_1++;
	}
	return value;
}
