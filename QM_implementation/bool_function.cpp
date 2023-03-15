#include "bool_function.h"

bool_function::bool_function() {

	bool res;
	res=validate();
	while (res!= true) 
	{
		cout << "try again" << endl;
		literals.clear();
		lit.clear();
		res=validate();
	}
	cout << "valid function entered" << endl;
	gen_table();
	output_table();
    print_table();
	P_I();
	canonical_sop();
	canonical_pos();
}
bool bool_function::validate() 
{
	bool flat = false;
	string input;
	cout << "enter your function" << endl;
	getline(cin, input);
		for (int i = 0; i < input.size(); i++)
		{
			if (isalpha(input[i])) 
			{
				lit[input[i]] = 0;
				flat = true;
				//literals.push_back(input[i]);
			}
			else if (input[i] == '+' && (i == 0 || i == input.size() - 1 || input[i - 1] == '+')) { //if '+' at the beginning or at the end or not between two literals
				//cout << "error!" << endl << "try again" << endl;
				flat = false;
				break;
			}
			else if (input[i] == '`' && i == 0) { //'if '`' at the beginning
			//	cout << "error!" << endl << "try again" << endl;
				flat = false;
				break;
			}
			else if (input[i] == '`' && input[i - 1] == '+' && i != 0) //--i is the major reason for the infinte loop, it decreases the i
			{   //check if the +` come together in this way
				cout << "Error: Invalid character '" << input[i] << "' found in input." << endl;
				//cout << "try again" << endl;
				flat = false;
				break;
			}
			else if (input[i] != '+' && input[i] != '`' && !isalpha(input[i])) //check if any other useless operator or number were put
			{
				cout << "Error: Invalid character '" << input[i] << "' found in input." << endl;
			//	cout << "try again" << endl;
				flat = false;
				break;
			}
			else
				flat = true;
		}
	
		if (flat == false) {
			return flat;
		}

	int count = 0;
	for (auto it = lit.begin(); it != lit.end(); it++) {
		it->second = count++;  //this is to give every literal a number as it will appear in the truth table
	}

	for (auto it = lit.begin(); it != lit.end(); it++) 
	{
		literals.push_back(it->first);
	}
	expression = input;
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
			
			if ((j/flip)%2==0)
			{
				temp_table[j][i] = false;
			}
			else
				temp_table[j][i] = true; 
		}
	}

	truth_table = temp_table;
}

void bool_function::output_table() {
	vector<bool> real_output; //contain the final output of the function
	for (int i = 0; i < truth_table.size(); i++) { //looping over every row in the table
		vector<bool> storing;  //will store the multiplies (like ab,cd)
		bool result = true;
		for (int j = 0; j < expression.size(); j++) { //looping over the expression (like ab+cd)
			char c = expression[j];
			if (j == expression.size() - 1) {
				if (c != '`')
					result = result * truth_table[i][lit[c]];
				break;
			}
			if (c != '+' && expression[j + 1] != '`' && j != expression.size() - 1)//take care because there might be spaces between the characters in the expression 
				result = result * truth_table[i][lit[c]];   //lit[c] this represents which colomn in the row i
			else if (c != '+' && expression[j + 1] == '`' && j != expression.size() - 1) {
				result = result * !truth_table[i][lit[c]];
			}
			else if (c == '+') {
				storing.push_back(result);
				result = true;
			}
		}
		storing.push_back(result); //because the final result will not be pushed back as there is no + at the end of the expression
		bool sum = false; //this will add the multiplies in the storing vector
		for (int i = 0; i < storing.size(); i++) {
			sum = sum + storing[i];
		}
		real_output.push_back(sum);
	}
	for (int i = 0; i < rows; i++)
		truth_table[i][size] = real_output[i];    // adds this output to the truth table. 
}


void bool_function::print_table()
{
	for (int i = 0; i < size; i++)
	{
		cout << setw(3)<<literals[i] << setw(3);
	}
	cout << "F";
	cout << endl;
	for (int i = 0; i < size; i++) {
		cout << "------";
	}
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		cout << i << " : ";
		for (int j = 0; j < size+1; j++)
		{
			cout <<setw(3)<< truth_table[i][j] << setw(3);
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
	map<int, vector<string>> implication_table; // keys are number of 1s, value is the binary rep 
	vector<string>primes;
	int counter = 0;
	for (int i = 0; i < rows; i++)
	{

		string temp_string = "";
		if (truth_table[i][size] == 1)
		{
			for (int j = 0; j < size; j++)
			{

				if (truth_table[i][j] == 1)
				{
					temp_string += "1";
					counter++;
				}
				else
				{
					temp_string += "0";
				}



			}
			if (implication_table.find(counter) == implication_table.end())
			{
				vector<string> temp;
				temp.push_back(temp_string);
				implication_table.insert(pair<int, vector<string>>(counter, temp));
			}
			else {
				implication_table[counter].push_back(temp_string);
			}
			binary_rep_mins[temp_string].push_back(i);
		}

		counter = 0;
		temp_string.clear();
	}

	/*for (map<int, vector<string>> ::iterator it = implication_table.begin(); it != implication_table.end(); it++)
	{
		cout << (*it).first << ":";
		vector<string>temp = ((*it).second);
		for (int j = 0; j < temp.size(); j++)
		{
			cout << temp[j] << " ,";
		}
		cout << endl;
	}*/

	cout << "====================================================" << endl;
	map<int, vector<string>> map_temp;
	while (!implication_table.empty())
	{
		map_temp = implication_table;
		implication_table.clear();
		for (map<int, vector<string>> ::iterator it = map_temp.begin(); it != map_temp.end(); it++)
		{
			if (it == --map_temp.end())
				break;

			vector <string> temp = (*it).second;

			map<int, vector<string>> ::iterator it2;
			it2 = ++it;
			vector <string> temp_1 = (*it2).second;
			it--;


			for (int i = 0; i < temp.size(); i++)
			{
				int test = 0;
				for (int j = 0; j < temp_1.size(); j++)
				{
					ret_type result;
					result = compare_strings(temp[i], temp_1[j]);

					if (result.diff)
					{
						test = 1;

						binary_rep_mins[result.comb] = result.minterm_comb;
						implication_table[result.num_of_1].push_back(result.comb);
					}

				}
				if (test == 0)
				{
					primes.push_back(temp[i]);
				}

			}


		}

		/*for (auto it = map_temp.begin(); it != map_temp.end(); it++)
		{
			cout << it->first << " " << ":";
			for (auto x : it->second)
				cout << x << " " << ",";
			cout << endl;
		}*/

	}

	map<string, vector<int>> final_PI;
	for (auto it = map_temp.begin(); it != map_temp.end(); it++)
	{
		for (auto x : it->second)
		{
			final_PI[x] = binary_rep_mins[x];

		}

	}

	for (int i = 0; i < primes.size(); i++)
	{
		final_PI[primes[i]] = binary_rep_mins[primes[i]];
	}

	for (auto it = final_PI.begin(); it != final_PI.end(); it++)
	{
		cout << it->first << " " << ":";
		for (auto x : it->second)
			cout << x << " " << ",";
		cout << endl;
	}
	binary_rep_mins = final_PI;



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
	{

		value.minterm_comb.push_back(it);
	}

	for (auto it : binary_rep_mins[y]) {

		value.minterm_comb.push_back(it);
	}

	for (int i = 0; i < temp.size(); i++)
	{
		if (temp[i] == '1')
			value.num_of_1++;
	}

	return value;


}
