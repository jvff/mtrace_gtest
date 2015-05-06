#include <string.h>

#include "TempDir.hpp"

TempDir::TempDir() : TempDir("") {
}

TempDir::TempDir(const char* prefix) : TempDir(std::string(prefix)) {
}

TempDir::TempDir(std::string prefix) {
    path = "/tmp/" + prefix + "XXXXXX";
    char* newPath = strdup(path.c_str());

    if (mkdtemp(newPath) == NULL || newPath[0] == 0) {
        free(newPath);
        throw "Failed to create temporary directory";
    }

    path = newPath;
    free(newPath);
}

std::string TempDir::getPath() {
    return path;
}
