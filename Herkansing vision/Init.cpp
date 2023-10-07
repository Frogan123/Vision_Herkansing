#include <iostream>
#include "Init.h"
using namespace std;


Init::Init(const std::string& filename, int numRows, int numColumns)
	: filename(filename), numRows(numRows), numColumns(numColumns) {}

/*
	files_initialize function initializes the array within the .txt file and copies it into a c++ array
	Output:	vector<int> error	| Vector of integers to show what files didn't load
*/
bool Init::files_initialize() {
	ifstream file(filename);

	if (!file) {
		return false;
	}
	else {
		for (int i = 0; i < numRows; i++) {
			vector<int> row;
			for (int j = 0; j < numColumns; j++) {
				float value;
				if (file >> value) {
					if (value == 0)
					{
						continue;
					}
					else {
						row.push_back(value);
					}
				}
			}
			array.push_back(row);
		}
		file.close();
		return true;
	}
}

void Init::print_array(){
	for (const auto& row : array) {
		for (int value : row) {
			cout << value << " ";
		}
		cout << endl;
	}
}