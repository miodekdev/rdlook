#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

filesystem::path main_path;
string phrase;
size_t total_matches = 0;

size_t findAll(string& path, bool highlight = false) {
    size_t matches = 0;
    size_t position = 0;
    while ((position = path.find(phrase, position)) != string::npos) {
        matches++;
        total_matches++;
        position += phrase.length();
    }
    return matches;
}

void tryToFind(const filesystem::path& path, bool output = true) {
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
    if (highlight_position != string::npos || matches && output) {
        if (matches) cout << "\033[1m(" << to_string(matches) << ")\033[0m ";
        if (highlight_position != string::npos) {
            filename.insert(highlight_position + phrase.length(), "\033[0m");
            filename.insert(highlight_position, "\033[7m");
        }
        if (!parent.empty()) cout << parent << "\\";
        cout << filename << endl;
    }
}

int main(int argc, char *argv[]) { // TODO better argument parsing, options
    cout << endl;
    switch (argc) {
        case 1:
            cerr << "No key phrase specified.";
            return 1;
        case 2:
            main_path = filesystem::current_path();
            phrase = argv[1];
            break;
        default:
            main_path = argv[1];
            phrase = argv[2];
    }
    if (filesystem::is_directory(main_path)) {
        for (auto const& path : filesystem::recursive_directory_iterator(main_path)) {
            tryToFind(path);
        }
        cout << endl;
    }
    else if (filesystem::is_regular_file(main_path)) {
        tryToFind(main_path, false);
    }
    else {
        cerr << "\"" << main_path << "\" is not a valid directory or a file.";
        return 1;
    }
    cout << "Total occurrences in file contents: " << to_string(total_matches) << endl;
    return 0;
}
