#pragma once

#include<iostream>
#include<vector>
#include<fstream>
#include"Exeption.h"

#define DELIM ','
#define ERROR -1
#define VALUE_UNKNOWN -2
#define SUCCESS 0


int StringToInt(std::string elem);

bool isStringEmpty(std::string token);
//are there any duplicate elements
bool isRepeatingInVector(std::vector<std::string> res, std::string token); 
//searching vector element and returning index 
int SearchElementsIndex(std::string str, std::vector<std::string> Vector); 




class CSVTable {

public:

	CSVTable(std::string fileName);

	~CSVTable();
	//cout
	friend std::ostream& operator<<(std::ostream& os, CSVTable& dt); 


private:

	std::string nameCsv;
	//initial values (without column and row names)
	std::vector<std::string> values;
	//row names (1th column)
	std::vector<std::string> rowsNames;
	//column names (1th row)
	std::vector<std::string> columnsNames;
	//calculated values (without column and row names)
	std::vector<int> finalValues;

	int columnCount;

	//reading the table from the file
	void ReadTable();
	//write column names
	void InitTable(std::string first);
	//writing line names and filling the container  values 
	void RowsNumberingAndGetValues(std::string column);

	//filling finalValues
	void FillFinalTable();
	//find the value of cell elem (search for the value, for example, cell A3)
	int GetArgFromTable(int& i, std::string elem);
	//if a formula is specified in a cell, then its splitting into arguments, using \
							GetArgFromTable search for their values, then calculate
	int CalculateFormula(std::string elem);
};