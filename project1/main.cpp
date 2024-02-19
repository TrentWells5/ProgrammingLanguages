#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "scanner.hpp"
#include "parser.hpp"

using namespace std;

string readFile(const string& filePath) {
    ifstream file(filePath);
    stringstream buffer;

    if (file) {
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    } else {
        cerr << "Could not open file: " << filePath << endl;
        return "";
    }
}

void processFile(const string& filePath) {
    string fileContent = readFile(filePath);

    if (!fileContent.empty()) {
        cout << "Processing file: " << filePath << endl;
        Scanner scanner(fileContent);
        vector<Token> tokens = scanner.scanTokens();

        Parser parser(tokens);
        bool success = parser.parse();

        if (success) {
            cout << "Success! Parsing completed successfully for file " << filePath << endl;
        } else {
            cout << "Unsuccessful! Parsing encountered errors for file " << filePath << endl;
        }
        cout << endl;
    } else {
        cout << "Skipping empty or non-existent file: " << filePath << endl << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>|all" << endl;
        return 1;
    }

    if (string(argv[1]) == "all") {
        vector<string> filenames = {
            "a1", "a2", "a3", "a4",
            "a5", "a6", "a7", "a8"
        };
        
        for (const auto& filename : filenames) {
            string filePath = "inputFilesP1/" + filename;
            processFile(filePath);
        }
    } else {
        string filePath = argv[1];
        processFile(filePath);
    }

    return 0;
}
