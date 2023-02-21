#include "utils.hpp"

std::string NumberToString( int Number )
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}