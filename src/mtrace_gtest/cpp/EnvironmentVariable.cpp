#include "EnvironmentVariable.hpp"

EnvironmentVariable::EnvironmentVariable(const char* name) : name(name) {
}

const std::string& EnvironmentVariable::getName() {
    return name;
}
