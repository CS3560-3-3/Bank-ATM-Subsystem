#ifndef ERROR_H
#define ERROR_H

#include <string>
using namespace std;

class Error {
private:
    int errorCode;
    string errorType;
    string errorCause;

public:
    Error(int code = 0, const string& type = "", const string& cause = "");

    // Display the error details
    void displayError() const;
};

#endif
