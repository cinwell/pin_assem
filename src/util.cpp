#include "util.h"
#include "printmacros.h"

void AxOpenFile(std::fstream& file, std::string& filename) {
    file.open(filename, std::ios::out);
    if (file.is_open()) {
        DEBUG(filename << " has been opened ..."); 
    } else {
        ERROR("cannot open " << filename << " !!!");
    }
}
