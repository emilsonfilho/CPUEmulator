#ifndef FILE_NOT_FOUND_EXCEPTION_HPP
#define FILE_NOT_FOUND_EXCEPTION_HPP

#include <stdexcept>

using namespace std;

class FileNotFoundException : public runtime_error {
public:
    explicit FileNotFoundException(): runtime_error("Arquivo nao encontrado! Tente novamente.") {}
};

#endif