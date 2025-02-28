#ifndef FILES_PATH_HPP
#define FILES_PATH_HPP

#include <string>

using namespace std;

class Path {
public:
    static const string DIR;
    static const string EXTENSION;
    static const string getFullPath(const string& filename);
};

#endif