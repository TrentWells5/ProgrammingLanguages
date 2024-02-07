#include <fstream>
#include <iostream>
#include <sstream>
#include "scanner.hpp"

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

int main() {
    vector<string> filenames = {
        "a1", "a2", "a3", "a4",
        "a5", "a6", "a7", "a8"
    };

    for (int i = 0; i < filenames.size(); ++i) {
        string filePath = "inputFilesP1/" + filenames[i]; // Adjust directory as needed
        string fileContent = readFile(filePath);
        if (!fileContent.empty()) {
            Scanner scanner(fileContent);
            vector<Token> tokens = scanner.scanTokens();

            cout << "Tokens from file " << filePath << ":" << endl;
            for (const auto& token : tokens) {
                cout << "Token (type: " << scanner.tokenTypeToString(token.type) 
                     << ", value: '" << token.value << "', line: " << token.line << ")" << endl;
            }
        } else {
            cout << "Skipping empty or non-existent file: " << filePath << endl;
        }
        cout << endl; // Add a newline for better separation between files
    }
    return 0;
}