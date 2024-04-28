#include <iostream>
#include <filesystem>
#include <fstream>
#include "args.h++"

size_t total_matches = 0;

namespace arguments {
    filesystem::path main_path;
    string phrase;
    bool output = true;
    bool recursive = false;
    bool format = false;
}

namespace ansi {
    string bold = "\033[1m";
    string highlight = "\033[7m";
    string reset = "\033[0m";
}

size_t findAll(string& path) {
    size_t matches = 0;
    size_t position = 0;
    while ((position = path.find(arguments::phrase, position)) != string::npos) {
        matches++;
        total_matches++;
        position += arguments::phrase.length();
    }
    return matches;
}

void tryToFind(const filesystem::path& path) {
    size_t matches = 0;
    if (filesystem::is_regular_file(path)) {
        ifstream file(path);
        string line;
        while (getline(file, line)) {
            matches += findAll(line);
        }
        file.close();
    }

    string filename = path.filename().string();
    size_t highlight_position = filename.find(arguments::phrase);
    string parent = filesystem::relative(path, arguments::main_path).parent_path().string();
    if ((highlight_position != string::npos || matches) && arguments::output) {
        if (matches) cout << ansi::bold << "(" << to_string(matches) << ")" << ansi::reset << " ";
        if (highlight_position != string::npos) {
            filename.insert(highlight_position + arguments::phrase.length(), ansi::reset);
            filename.insert(highlight_position, ansi::highlight);
        }
        if (!parent.empty()) cout << parent << "\\";
        cout << filename << endl;
    }
}

int main(int argc, char *argv[]) {
    ArgumentParser argument_parser(argc, argv);
    cout << endl;
    {
        using namespace arguments;
        phrase = argument_parser.getArgument(0);
        main_path = argument_parser.getArgument("--path", "-p");
        output = !argument_parser.getFlag("--silent", "-s");
        recursive = argument_parser.getFlag("--recursive", "-r");
        format = argument_parser.getFlag("--format", "-f");
    }

    if (!arguments::format) {
        using namespace ansi;
        bold.clear();
        highlight.clear();
        reset.clear();
    }

    if (arguments::phrase.empty()) {
        cerr << "No key phrase specified." << endl;
        return 1;
    }

    if (arguments::main_path.empty()) {
        arguments::main_path = filesystem::current_path();
    }
    if (filesystem::is_directory(arguments::main_path)) {
        if (arguments::recursive) {
            for (auto const& path : filesystem::recursive_directory_iterator(arguments::main_path)) tryToFind(path);
        }
        else {
            for (auto const& path : filesystem::directory_iterator(arguments::main_path)) tryToFind(path);
        }
    }
    else if (filesystem::is_regular_file(arguments::main_path)) {
        arguments::output = false;
        tryToFind(arguments::main_path);
    }
    else {
        cerr << "\"" << arguments::main_path << "\" is not a valid directory or a file." << endl;
        return 1;
    }

    cout << "Total occurrences in file contents: " << ansi::bold << to_string(total_matches) << ansi::reset << endl;
    return 0;
}
