#include "args.h++"

string getArg(int argc, char* argv[], const string& long_option, const string& short_option) {
    for (int index = 0; index < argc-1; index++) {
        if ((long_option == argv[index]) || (short_option == argv[index])) {
            return {argv[index+1]};
        }
    }
    return "";
}

bool getFlag(int argc, char* argv[], const string& long_option, const string& short_option) {
    for (int index = 0; index < argc; index++) {
        if ((long_option == argv[index]) || (short_option == argv[index])) {
            return true;
        }
    }
    return false;
}