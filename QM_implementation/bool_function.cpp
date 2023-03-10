#include <set>
#include "bool_function.h"

bool bool_function::validate(string x) {
    int i = 0;
    vector<bool> User_given_function;

    set<char> vars;  // Set to keep track of input variables

    bool has_consecutive = false;
    char prev = '\0';

    int var_count = 0;
    while (i < x.size()) {
        char c = x[i];
        if (isalpha(c)) {
            if (prev == c) {
                has_consecutive = true;
            }
            vars.insert(c);
            var_count++;
        }
        else if (c == '`') {
            i++;
            char next_c = x[i];
            if (isalpha(next_c)) {
                vars.insert(next_c);
                var_count++;
            }
            else {
                cout << "Error: Invalid character '" << next_c << "' found after backtick." << endl;
                return false;
            }
        }
        else if (c != '+') {
            cout << "Error: Invalid character '" << c << "' found in input." << endl;
            return false;
        }
        prev = c;
        i++;
    }

    if (has_consecutive) {
        cout << "Error: Consecutive duplicate variables found in input." << endl;
        return false;
    }

    cout << "The input string is valid." << endl;
    cout << "Number of variables: " << var_count << endl;
    return true;
}

void bool_function::gen_table()
{
	int size = literals.size();
	int rows = pow(2, size);
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

void bool_function::print_table()
{
	for (int i = 0; i < size; i++)
	{
		cout << literals[i] << setw(3);
	}
	cout << endl;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < size; j++)
		{
			cout << truth_table[i][j] << setw(3);
		}
		cout << endl;
	}
}

void bool_function::canonical_sop() {// take the output of the function, the table of a,b,..., the literals
	string sop_result = "";
	for (int i = 0; i < truth_table.size(); i++) {
		if (truth_table[i][truth_table[0].size()-1] == true) { //if the output of the function equal one(which is in the last colomn)
			for (int j = 0; j < truth_table[0].size(); j++) { //will loop over the colomns (literals) and will see the value of each literal
				if (truth_table[i][j] == false) { //if it is equal zero then we put ' for representing (Not) in literals
					sop_result += literals[j];
					sop_result += "'";
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

void bool_function::canonical_pos() {// take the output of the function, the table of a,b,.., the literals
	string pos_result = "";
	for (int i = 0; i < truth_table.size(); i++) {
		if (truth_table[i][truth_table[0].size() - 1] == false) { //if the output of the function equal zero
			pos_result += "(";   //put every n literals added between prackets
			for (int j = 0; j < truth_table[0].size(); j++) { //will loop over the colomns (literals) and will see the value of each literal
				if (truth_table[i][j] == true) { //if it is equal one then we put ' for representing (Not) in literals
					pos_result += literals[j];
					pos_result += "'";
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


