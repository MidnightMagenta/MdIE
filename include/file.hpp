#ifndef MDIE_FILE_H
#define MDIE_FILE_H

#include <error.hpp>
#include <string>

namespace mdie {
mdie::Result read_plaintext(const std::string &path, std::string *data);
std::string read_plaintext(const std::string &path);
}

#endif// !MDIE_FILE_H
