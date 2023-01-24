#ifndef PARSERS_H
#define PARSERS_H
#include <fstream>
#include <string>
#include <vector>

using namespace std;

string         readFile( const string &fileName );
vector<string> readLines( string &str );
vector<string> trimLines( vector<string> &lines );
vector<vector<string> >
splitLines( vector<string> &lines, string key, bool useBrackets );

/**
 * @brief Read a string and return a vector of strings with trimmed lines.
 *
 * @param str string buffer to read
 * @return vector<string> vector of strings
 */
vector<string> readTidyLines( string &str );

#endif
