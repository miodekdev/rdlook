#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

#define ANSI_SWAP_COLORS "[7m"
#define ANSI_BOLD "[1m"
#define ANSI_RESET "[0m"

using namespace std;

filesystem::path root_path;
string phrase;
size_t total_matches = 0;

void tryToFind(const filesystem::path& candidate) { // TODO count matches in name and display the amount
    auto position = candidate.filename().string().find(phrase); // TODO fix "1 matches"
    if (position != string::npos) {
        position += candidate.parent_path().string().length();
        total_matches++;
        cout << ANSI_SWAP_COLORS << "match in name" << ANSI_RESET << " ";
        cout << candidate.parent_path().string() << string(1, filesystem::path::preferred_separator);
        cout << ANSI_BOLD << candidate.filename().string() << ANSI_RESET << endl;
    }
    if (filesystem::is_regular_file(candidate)) {
        ifstream file(candidate);
        string line;
        size_t matches = 0;
        while (getline(file, line)) {
            position = 0;
            while ((position = line.find(phrase, position)) != string::npos) {
                matches++;
                total_matches++;
                position += phrase.length();
            }
        }
        if (matches) {
            cout << ANSI_SWAP_COLORS << to_string(matches) << " matches" << ANSI_RESET;
            cout << " "<< candidate.string() << endl;
        }
        file.close();
    }
}

void findOccurrences(const filesystem::path& path) {
    tryToFind(path.string());
}

int main(int argc, char *argv[]) {
    switch (argc) {
        case 1:
            cerr << "No directory or file specified";
            return 1;
        case 2:
            root_path = filesystem::current_path();
            phrase = argv[1];
            break;
        default:
            root_path = filesystem::canonical(argv[1]);
            phrase = argv[2];
    }
    if (filesystem::is_regular_file(root_path)) {
        tryToFind(root_path);
    }
    else if (filesystem::is_directory(root_path)) {
        for (auto const& path : filesystem::recursive_directory_iterator(root_path)) {
            tryToFind(path);
        }
    }
    else {
        cerr << "\"" << root_path << "\" is not a valid directory or a file";
        return 1;
    }

    cout << endl << "Found a total of " << ANSI_SWAP_COLORS << to_string(total_matches) << " match";
    if (total_matches != 1) cout << "es" << ANSI_RESET << endl;
    return 0;
}
