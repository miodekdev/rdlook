#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

using namespace std;

filesystem::path main_path;
string phrase;
size_t total_matches = 0;

size_t findAll(const string& candidate) {
    size_t matches = 0;
    size_t position = 0;
    while ((position = candidate.find(phrase, position)) != string::npos) {
        matches++;
        total_matches++;
        position += phrase.length();
    }
    return matches;
}

void tryToFind(const filesystem::path& candidate) {
    size_t name_matches = findAll(candidate.string());
    size_t contents_matches = 0;
    if (filesystem::is_regular_file(candidate)) {
        ifstream file(candidate);
        string line;
        while (getline(file, line)) {
            contents_matches += findAll(line);
        }
        file.close();
    }
    if (name_matches != string::npos || contents_matches) {
        cout << "(" << (contents_matches ? to_string(contents_matches) : "name") << ") ";
        cout << filesystem::relative(candidate, main_path).string() << endl;
    }
}

int main(int argc, char *argv[]) {
    cout << endl;
    switch (argc) {
        case 1:
            cerr << "No directory or file specified";
            return 1;
        case 2:
            main_path = filesystem::current_path();
            phrase = argv[1];
            break;
        default:
            main_path = argv[1];
            phrase = argv[2];
    }
    if (filesystem::is_regular_file(main_path)) {
        tryToFind(main_path);
    }
    else if (filesystem::is_directory(main_path)) {
        for (auto const& path : filesystem::recursive_directory_iterator(main_path)) {
            tryToFind(path);
        }
    }
    else {
        cerr << "\"" << main_path << "\" is not a valid directory or a file";
        return 1;
    }
    cout << endl << "Found " << to_string(total_matches);
    cout << " match" << ((total_matches != 1) ? "es" : "") << "." << endl;
    return 0;
}
