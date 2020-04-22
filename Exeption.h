#pragma once
#include<exception>

class FileException : public std::exception {
private:
	const char* message;
public:
	FileException(const char* mes) : message(mes) { }
	const char* what() const noexcept { return message; }
};

class WrongSymbolException : public std::exception {
private:
	const char* message;
public:
	WrongSymbolException(const char* mes) : message(mes) { }
	const char* what() const noexcept { return message; }
};

class RepeatingNamesOfColumnsOrRows : public std::exception {
private:
	const char* message;
public:
	RepeatingNamesOfColumnsOrRows(const char* mes) : message(mes) { }
	const char* what() const noexcept { return message; }
};

class TableHasLoop : public std::exception {
private:
	const char* message;
public:
	TableHasLoop(const char* mes) : message(mes) { }
	const char* what() const noexcept { return message; }
};

class WrongResultOfExpr : public std::exception {
private:
	const char* message;
public:
	WrongResultOfExpr(const char* mes) : message(mes) { }
	const char* what() const noexcept { return message; }
};