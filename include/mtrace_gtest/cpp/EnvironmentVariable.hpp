#ifndef ENVIRONMENT_VARIABLE_HPP
#define ENVIRONMENT_VARIABLE_HPP

#include <string>

class EnvironmentVariable {
private:
    std::string name;

public:
    EnvironmentVariable(const char* name);

    const std::string& getName();
};

#endif
