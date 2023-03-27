#include "utils.hpp"

std::string NumberToString( int Number )
{
    std::ostringstream ss;
    ss << Number;
    return ss.str();
}

std::vector<std::string> ftSplit( std::string text, char separator )
{
    std::string              str;
    std::stringstream        ss( text );
    std::vector<std::string> result;

    while ( getline( ss, str, separator ) ) {
        result.push_back( str );
    }

    return result;
}