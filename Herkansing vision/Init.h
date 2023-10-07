#pragma once
#include <fstream>
#include <vector>

class Init
{
public:
	Init(const std::string& filename, int numRows, int numColumns);
	bool files_initialize();
	void print_array();
	std::vector<std::vector<int>> get_array() {
		return array;
	}

private:
	std::string filename;
	int numRows;
	int numColumns;
	std::vector<std::vector<int>> array;
};