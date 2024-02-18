#include <fstream>
#include <iostream>
#include <sstream>
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

int main() {
    vector<string> filenames = {
        "a1", "a2", "a3", "a4",
        "a5", "a6", "a7", "a8"
    };

    for (const auto& filename : filenames) {
        string filePath = "inputFilesP1/" + filename;
        string fileContent = readFile(filePath);

        if (!fileContent.empty()) {
            cout << "Processing file: " << filePath << endl;
            Scanner scanner(fileContent);
            vector<Token> tokens = scanner.scanTokens();

            //Optionally, print tokens for debugging
            // for (const auto& token : tokens) {
            //     cout << "Token (type: " << scanner.tokenTypeToString(token.type)
            //          << ", value: '" << token.value << "', line: " << token.line << ")" << endl;
            // }
            
            Parser parser(tokens);
            bool success = parser.parse(); // Parse the token sequence and check for success

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

    return 0;
}
