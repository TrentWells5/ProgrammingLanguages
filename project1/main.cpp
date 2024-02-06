#include <iostream>
#include <fstream>  // Include for ifstream
#include <sstream>  // Include for stringstream
#include <vector>   // Include for vector
#include "scanner.hpp"
#include "parser.hpp"
#include <utility>

using namespace std;

string readFileContent(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    vector<string> filenames = {
        "a1", "a2", "a3", "a4",
        "a5", "a6", "a7", "a8"
    };

    for(int i = 0; i < 8; i++){
        filenames.at(i) = "inputFilesP1/" + filenames.at(i);
    }

    for (const auto& filename : filenames) {
        string content = readFileContent(filename);
        if (!content.empty()) {
            cout << "Parsing " << filename << "..." << endl;
            Parser parser(content);
            parser.parse();
            cout << "Done parsing " << filename << ".\n" << endl;
        }
    }

    return 0;
}