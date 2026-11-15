#pragma once
#include <exception>
#include <string>
using namespace std;

class BaseException : public exception {
protected:
	string message;
public:
	explicit BaseException(const string& msg);
	const char* what() const noexcept override;
};

class AuthException : public BaseException {
public:
	explicit AuthException(const string& msg);
};

class FileException : public BaseException {
public:
	explicit FileException(const string& msg);
};

class ValidationException : public BaseException {
public:
	explicit ValidationException(const string& msg);
};