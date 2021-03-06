#include <string.h>

#include "EnvironmentVariable.hpp"

EnvironmentVariable::EnvironmentVariable(const char* name) : name(name) {
    environmentString = NULL;
}

EnvironmentVariable::~EnvironmentVariable() {
    if (environmentString != NULL) {
        unsetenv(name.c_str());
        delete environmentString;
    }
}

const std::string& EnvironmentVariable::getName() {
    return name;
}

void EnvironmentVariable::operator=(const char* value) {
    std::string stringValue{value};

    *this = stringValue;
}

void EnvironmentVariable::operator=(const std::string& value) {
    std::string environmentString = name + "=" + value;

    this->environmentString = strdup(environmentString.c_str());

    putenv(this->environmentString);
}
