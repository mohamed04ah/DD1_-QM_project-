#include "bool_function.h"

bool bool_function::validate(string x) {
    int i = 0;
    vector<bool> User_given_function;

    set<char> vars;  // Set to keep track of input variables

    bool has_consecutive = false;
    char prev = '\0';

    for (char c : x) 
    {
        if (isalpha(c)) {
            if (prev == c) {  // Check if there are consecutive duplicate variables
                has_consecutive = true;
                //return false;
            }
            vars.insert(c);  // Add variable to set
        } else if (c != '+' && c != '`') {  // Check if input contains only valid characters
            cout << "Error: Invalid character '" << c << "' found in input." << endl;
            return false;
        }
        prev = c;
    }

    if (has_consecutive) {  // Print an error message for consecutive duplicate variables
        cout << "Error: Consecutive duplicate variables found in input." << endl;
        return false;
    }

    // If we reach this point, the input string is valid
    cout << "The input string is valid." << endl;
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


