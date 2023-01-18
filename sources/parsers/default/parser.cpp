#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string readFile( const string &fileName )
{
    ifstream ifs( fileName.c_str(), ios::in | ios::binary | ios::ate );

    ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg( 0, ios::beg );

    vector<char> bytes( fileSize );
    ifs.read( bytes.data(), fileSize );

    return string( bytes.data(), fileSize );
}

vector<string> readLines( string &str )
{
    vector<string> lines;
    size_t         pos = 0;
    while ( ( pos = str.find( '\n' ) ) != string::npos ) {
        lines.push_back( str.substr( 0, pos ) );
        str.erase( 0, pos + 1 );
    }
    return lines;
}

int main()
{
    string file = readFile( "config/config.conf" );
    // split newlines as new strings

    vector<string> lines = readLines( file );

    cout << "lines: " << lines.size() << endl;
    for (

    )

        return 0;
}
