#include "../headers/Error.h"

//Parameterized constructor with default values
Error::Error(int code, const string& type, const string& cause) : 
	errorCode(code),
	errorType(type),
	errorCause(cause)
{}

void Error::displayError() const {
	cout << errorCode << endl;
	cout << errorType << endl;
	cout << errorCause << endl;
}