#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include "args.h++"

namespace arguments {
    filesystem::path main_path;
    string phrase;
    bool output = true;
    bool recursive = false;
    bool ansi_formatting = false;
}
namespace ansi {
    string bold = "\033[1m";
    string highlight = "\033[7m";
    string reset = "\033[0m";
}

size_t total_matches = 0;

vector<size_t> findAll(const string& path) {
    vector<size_t> positions;
    size_t current_position = 0;
    while ((current_position = path.find(arguments::phrase, current_position)) != string::npos) {
        positions.push_back(current_position);
        current_position += arguments::phrase.length();
    }
    total_matches += positions.size();
    return positions;
}

void highlight(string& str, const vector<size_t>& positions) {
    size_t offset = 0;
    for (auto position : positions) {
        str.insert(position+arguments::phrase.length()+offset, ansi::reset);
        str.insert(position+offset, ansi::highlight);
        offset += ansi::reset.length() + ansi::highlight.length();
    }
}

void tryToFind(const filesystem::path& path) {
    size_t matches = 0;
    if (filesystem::is_regular_file(path)) {
        ifstream file(path);
        string line;
        while (getline(file, line)) matches += findAll(line).size();
        file.close();
    }

    string filename = path.filename().string();
    vector<size_t> highlight_positions = findAll(filename);
    string parent = filesystem::relative(path, arguments::main_path).parent_path().string();
    if ((!highlight_positions.empty() || matches) && arguments::output) {
        if (matches) cout << ansi::bold << "(" << to_string(matches) << ")" << ansi::reset << " ";
        if (!highlight_positions.empty()) {
            highlight(filename, highlight_positions);
        }
        if (!parent.empty()) cout << parent << "\\";
        cout << filename << endl;
    }
}

int main(int argc, char *argv[]) {
    cout << endl;

    {using namespace arguments;
        ArgumentParser parser(argc, argv);
        phrase = parser.getArgument(0);
        main_path = parser.getArgument("/path", "/p");
        output = !parser.getFlag("/silent", "/s");
        recursive = parser.getFlag("/recursive", "/r");
        ansi_formatting = parser.getFlag("/format", "/f");

        if (!ansi_formatting) {
            ansi::bold.clear();
            ansi::highlight.clear();
            ansi::reset.clear();
        }

        if (phrase.empty()) {
            cerr << "No key phrase specified." << endl;
            return 1;
        }
    }

    if (arguments::main_path.empty()) arguments::main_path = filesystem::current_path();

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
        cerr << arguments::main_path << " is not a valid directory or a file." << endl;
        return 1;
    }

    cout << "Total occurrences: " << ansi::bold << to_string(total_matches) << ansi::reset << endl;
    return 0;
}
