#ifndef TEMP_DIR_HPP
#define TEMP_DIR_HPP

#include <string>

class TempDir {
private:
    std::string path;

public:
    TempDir();
    TempDir(const char* prefix);
    TempDir(std::string prefix);

    std::string getPath();
};

#endif
