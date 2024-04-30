#include <string>

using namespace std;

class ArgumentParser {
public:
    ArgumentParser(int argc, char* argv[]);
    string getArgument(size_t index);
    string getArgument(const string& long_option, const string& short_option);
    bool getFlag(const string& long_option, const string& short_option);
private:
    int argument_count;
    char** arguments;
};