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

    virtual void operator=(const char* value);
    virtual void operator=(const std::string& value);
};

#endif
