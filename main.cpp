#include <iostream>
#include"CSVTable.h"


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Error: less or more than 1 arguments" << std::endl;
		return ERROR;
	}

	CSVTable t(argv[1]);
	std::cout << t;

	return SUCCESS;
}