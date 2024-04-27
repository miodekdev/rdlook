#include <iostream>
#include <filesystem>
#include <fstream>
#include "args.h++"

filesystem::path main_path;
string phrase;
size_t total_matches = 0;
bool output = true;

string bold = "\033[1m";
string highlight = "\033[7m";
string reset = "\033[0m";

size_t findAll(string& path) {
    size_t matches = 0;
    size_t position = 0;
    while ((position = path.find(phrase, position)) != string::npos) {
        matches++;
        total_matches++;
        position += phrase.length();
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
    size_t highlight_position = filename.find(phrase);
    string parent = filesystem::relative(path, main_path).parent_path().string();
    if ((highlight_position != string::npos || matches) && output) {
        if (matches) cout << bold << "(" << to_string(matches) << ")" << reset << " ";
        if (highlight_position != string::npos) {
            filename.insert(highlight_position + phrase.length(), reset);
            filename.insert(highlight_position, highlight);
        }
        if (!parent.empty()) cout << parent << "\\";
        cout << filename << endl;
    }
}

int main(int argc, char *argv[]) {
    cout << endl;
    if (argc < 2) {
        cerr << "No key phrase specified." << endl;
        return 1;
    }
    phrase = string(argv[1]);
    main_path = getArg(argc, argv, "--path", "-p");
    output = !getFlag(argc, argv, "--silent", "-s");
    if (!getFlag(argc, argv, "--format", "-f")) {
        bold = "";
        highlight = "";
        reset = "";
    }

    if (main_path.empty()) {
        main_path = filesystem::current_path();
    }
    if (filesystem::is_directory(main_path)) {
        for (auto const& path : filesystem::recursive_directory_iterator(main_path)) {
            tryToFind(path);
        }
    }
    else if (filesystem::is_regular_file(main_path)) {
        output = false;
        tryToFind(main_path);
    }
    else {
        cerr << "\"" << main_path << "\" is not a valid directory or a file." << endl;
        return 1;
    }

    cout << "Total occurrences in file contents: " << to_string(total_matches) << endl;
    return 0;
}
