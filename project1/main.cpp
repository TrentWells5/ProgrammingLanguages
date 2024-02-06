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
    for (int i = 1; i <= 1; ++i) {
        string fileName = "inputFilesP1/a" + to_string(i); // Assuming files are named a1, a2, ..., a8
        string fileContent = readFile(fileName);

        if (!fileContent.empty()) {
            Scanner scanner(fileContent);
            vector<Token> tokens = scanner.scanTokens();

            cout << "Tokens from file " << fileName << ":" << endl;
            for (const auto& token : tokens) {
                // Assuming Token struct has a way to print itself or has accessible members
                // This is a placeholder for how you might output each token's details
                cout << "Token (type: " << scanner.tokenTypeToString(token.type) << ", value: '" << token.value << "', line: " << token.line << ")" << endl;
            }
        }
    }

    return 0;
}
