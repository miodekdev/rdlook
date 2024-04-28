#include "args.h++"

ArgumentParser::ArgumentParser(int argc, char **argv) {
    argument_count = argc-1;
    arguments = argv+1*sizeof(char);
}

string ArgumentParser::getArgument(size_t index) {
    if (argument_count > index) return arguments[index];
    else return "";
}

string ArgumentParser::getArgument(const std::string &long_option, const std::string &short_option) {
    for (int index = 0; index < argument_count-1; index++) {
        if ((long_option == arguments[index]) || (short_option == arguments[index])) {
            return {arguments[index+1]};
        }
    }
    return "";
}

bool ArgumentParser::getFlag(const std::string &long_option, const std::string &short_option) {
    for (int index = 0; index < argument_count; index++) {
        if ((long_option == arguments[index]) || (short_option == arguments[index])) {
            return true;
        }
    }
    return false;
}