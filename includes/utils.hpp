#ifndef UTILS_HPP
#define UTILS_HPP

#include "webserv.hpp"
#include <cerrno>
#include <fcntl.h>
#include <iostream>

std::string              NumberToString( int Number );
std::vector<std::string> ftSplit( std::string text, char separator = ' ' );

#endif
