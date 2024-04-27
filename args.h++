#include <string>
#include <optional>

using namespace std;

string getArg(int argc, char* argv[], const string& long_option, const string& short_option);
bool getFlag(int argc, char* argv[], const string& long_option, const string& short_option);