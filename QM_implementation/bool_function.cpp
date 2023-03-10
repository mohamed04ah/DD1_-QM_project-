#include "bool_function.h"

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

	for (int i = 0; i < size; i++) 
	{
		cout << literals[i] << setw(3);
	}
	cout << endl;
	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < size; j++) 
		{
			cout << temp_table[i][j] << setw(3);
		}
		cout << endl;
	}
	truth_table = temp_table;
}

void bool_function::print_table()
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < rows; j++)
		{
			cout << truth_table[i][j] << " ";
		}
		cout << endl;
	}
}


