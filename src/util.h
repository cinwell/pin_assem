#ifndef _UTIL_H
#define _UTIL_H

#include <fstream>
#include <cassert>
#include <map>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <functional>
#include "pin.H"

//access types
typedef enum {
    ACCESS_TYPE_LOAD,
    ACCESS_TYPE_STORE,
    ACCESS_TYPE_NUM
} ACCESS_TYPE;

void AxOpenFile(std::fstream& file, std::string& filename);
#endif
