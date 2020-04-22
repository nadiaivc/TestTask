#include<sstream>
#include"CSVTable.h"
#include <stdlib.h>

bool isRepeatingInVector(std::vector<std::string> res, std::string token) {
	for (std::vector<std::string>::iterator checkNames = res.begin(); checkNames != res.end(); checkNames++)
	{
		if (token == *checkNames)
		{
			return true;
		}
	}
	return false;
}

bool isStringEmpty(std::string token)
{
	return ((!token.length()) ? true : false);
}

int StringToInt(std::string elem)
{
	for (int i = 0; i < elem.length(); i++)
	{
		if (elem[i] < '0' || elem[i] > '9')//check there are no symbols other than numbers
		{
			throw WrongSymbolException("Error: the problem in string -> int");
		}
	}
	return(atoi(elem.c_str()));
}

int SearchElementsIndex(std::string str, std::vector<std::string> Vector)
{
	int index = 0;
	for (std::vector<std::string>::iterator it = Vector.begin(); it != Vector.end(); it++)
	{
		if (*it == str)
			return index;
		index++;
	}
	throw WrongSymbolException("Error: the table doesn't have this value");
}



CSVTable::CSVTable(std::string fileName)
{
	columnCount = 0;
	nameCsv = fileName;
	ReadTable();
	FillFinalTable();
}

CSVTable::~CSVTable()
{
	values.clear();
	rowsNames.clear();
	columnsNames.clear();
	finalValues.clear();
}

void CSVTable::ReadTable()
{
	std::ifstream fileCfg(this->nameCsv.c_str());
	if (!fileCfg.is_open())
	{
		throw FileException("Error: can't open the file");
	}
	else
	{
		std::string line;
		std::getline(fileCfg, line);//reading the first row with column names 
		InitTable(line);
		
		while (std::getline(fileCfg, line))
		{
			RowsNumberingAndGetValues(line);//reading the first column with row names
		}
		fileCfg.close();
	}
}

void CSVTable::InitTable(std::string first) {
	for (int i = 0; i < first.length(); i++)//delete spaces
	{
		if (first[i] == ' ') 
		{
			first.erase(i, 1);
			i--;
		}
	}

	std::stringstream str(first);
	std::string token;
	std::getline(str, token, DELIM);
	while (getline(str, token, DELIM)) 
	{
		if (isStringEmpty(token))
			throw WrongSymbolException("Error: table has an empty element");

		if (isRepeatingInVector(columnsNames, token))
			throw RepeatingNamesOfColumnsOrRows("Error: data has repeating names");

		columnsNames.push_back(token);
		columnCount++;
	}
}

void CSVTable::RowsNumberingAndGetValues(std::string column)
{
	for (int i = 0; i < column.length(); i++) //delete spaces
	{
		if (column[i] == ' ') {
			column.erase(i, 1);
			i--;
		}
	}

	std::stringstream str(column);
	std::string token;
	std::getline(str, token, DELIM);

	if (token[0] == '0') //the values ??of the 1st column should be positive, 0 isn't needed 
		throw WrongSymbolException("Error: the first column can contain only positive numbers");

	int count = 0;
	for (int i = 0; i < token.length(); i++) //check: 1) if the element is not empty   \
														2)Is the first element (row name) a number
	{
		if (isStringEmpty(token))
			throw WrongSymbolException("Error: table has an empty element");
		if (token[i] < '0' || token[i] > '9')
		{
			throw WrongSymbolException("Error: rows name can be only numbers");
		}
	}
	if (isRepeatingInVector(rowsNames, token)) //check if there are duplicate names
		throw RepeatingNamesOfColumnsOrRows("Error: data has repeating names");
	rowsNames.push_back(token);


	while (getline(str, token, DELIM)) //write other values
	{
		if (isStringEmpty(token))
			throw WrongSymbolException("Error: table has an empty element");
		values.push_back(token);
		count++;
	}

	if (count != columnCount)//the number of elements inside the table should = the number of columns
	{
		throw WrongSymbolException("Error: different count of columns and elements");
	}
}

void CSVTable::FillFinalTable()
{
	for (int i = 0; i < values.size(); i++)
		finalValues.push_back(-1); // value -1 marks unfilled cells

	int numberOfFilledCells = 0;
	int size = values.size();
	int lastnumberOfFilledCells = 0;
	while (numberOfFilledCells != size)//while all cells can be filled
	{
		for (int i = 0; i < size; i++)
		{
			if (finalValues[i] == -1)
			{
				std::string elem = values[i];
				if (elem[0] >= '0' && elem[0] <= '9') //just a number
				{
					finalValues[i] = StringToInt(elem);
					numberOfFilledCells++;
				}

				else if (elem[0] == '=')
				{
					finalValues[i] = CalculateFormula(elem);
					if (finalValues[i] == VALUE_UNKNOWN)
						finalValues[i] = -1; //cannot be calculated, one of the arguments is unknown
					else
					{
						values[i] = std::to_string(finalValues[i]);
						numberOfFilledCells++;
					}
				}
				else
				{
					throw WrongSymbolException("Error: wrong symbol in the formula");
				}
			}
		}
		if (lastnumberOfFilledCells == numberOfFilledCells)
		{
			throw TableHasLoop("Error: The table contains a loop, cannot be calculated");
			return;
		}
		lastnumberOfFilledCells = numberOfFilledCells;
	}
}

int CSVTable::CalculateFormula(std::string elem)
{
	int index1 = 0;
	int index2 = 0;
	int arg1;
	int arg2;
	int i = 1;

	arg1 = GetArgFromTable(i, elem);
	if (arg1 == VALUE_UNKNOWN)//arguments is unknown
		return VALUE_UNKNOWN;

	char op = elem[i];
	if (!(op == '+' || op == '/' || op == '-' || op == '*'))
	{
		throw WrongSymbolException("Error: no expression symbol");
	}
	i++;

	arg2 = GetArgFromTable(i, elem);
	if (arg2 == VALUE_UNKNOWN)
		return VALUE_UNKNOWN;

	if (elem[i] != '\0')//expression has a clear structure ARG1 op ARG2, then nothing should be
	{
		throw WrongSymbolException("Error: wrong argument structure");
	}

	switch (op)
	{
	case '+':
		return arg1 + arg2;
		break;
	case '-':
		if (arg1 - arg2 < 0)//only non-negative numbers must be stored in the table
			throw WrongResultOfExpr("Error: wrong result of expression");
		return arg1 - arg2;
		break;
	case '/':
		return arg1 / arg2;
		break;
	case '*':
		return arg1 * arg2;
		break;
	}
}

int CSVTable::GetArgFromTable(int& i, std::string elem)
{
	std::string arg_column;
	std::string arg_row;
	int index_column = 0;
	int index_row = 0;
	for (i; (i < elem.length()) && (elem[i] < '0' || elem[i] > '9'); i++)//name of column
	{
		arg_column += elem[i];
	}

	for (i; (i < elem.length()) && (elem[i] >= '0' && elem[i] <= '9'); i++)//name of row
	{
		arg_row += elem[i];
	}

	if (!(elem[i] == '+' || elem[i] == '/' || elem[i] == '-' || elem[i] == '*' || elem[i] == '\0'))//если было так: sds12s.. (неверный порядок)
	{
		throw WrongSymbolException("Error: wrong argument structure");
	}

	index_column = SearchElementsIndex(arg_column, columnsNames);
	index_row = SearchElementsIndex(arg_row, rowsNames);

	std::string arg = values[index_row * columnsNames.size() + index_column];//look for this arg in the table
	if (arg[0] == '=')//arguments is unknown
		return VALUE_UNKNOWN;
	return (StringToInt(arg));
}

std::ostream& operator<<(std::ostream& out, CSVTable& dt)
{
	for (std::vector<std::string>::iterator names = dt.columnsNames.begin(); names != dt.columnsNames.end(); names++)
		out << ", " << *names;
	out << std::endl;
	int j = 0;
	for (std::vector<std::string>::iterator rows = dt.rowsNames.begin(); rows != dt.rowsNames.end(); rows++)
	{
		out << *rows;
		for (int i = j * dt.columnCount; i < (j + 1) * dt.columnCount; i++)
		{
			out << ", " << dt.finalValues[i];
		}
		j++;
		out << std::endl;
	}
	return out;
}