#ifndef ENVIRONMENT_VARIABLE_HPP
#define ENVIRONMENT_VARIABLE_HPP

#include <string>

class EnvironmentVariable {
private:
    char* environmentString;
    std::string name;

public:
    EnvironmentVariable(const char* name);
    ~EnvironmentVariable();

    const std::string& getName();

    void operator=(const char* value);
    void operator=(const std::string& value);
};

#endif
