#include "TempDir.hpp"

TempDir::TempDir() : TempDir("") {
}

TempDir::TempDir(const char* prefix) : TempDir(std::string(prefix)) {
}

TempDir::TempDir(std::string prefix) {
    path = "/tmp/" + prefix;
}

std::string TempDir::getPath() {
    return path;
}
