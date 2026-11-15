#include "Exceptions.h"
using namespace std;

BaseException::BaseException(const string& msg) : message(msg) {}
const char* BaseException::what() const noexcept { return message.c_str(); }

AuthException::AuthException(const string& msg) : BaseException("Auth Exception: " + msg) {}
FileException::FileException(const string& msg) : BaseException("File Exception: " + msg) {}
ValidationException::ValidationException(const string& msg) : BaseException("Validation Exception: " + msg) {}
