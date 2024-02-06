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
//REAL MAIN
    // vector<string> filenames = {
    //     "a1", "a2", "a3", "a4",
    //     "a5", "a6", "a7", "a8"
    // };

    // for(int i = 0; i < 8; i++){
    //     filenames.at(i) = "inputFilesP1/" + filenames.at(i);
    // }

    // for (const auto& filename : filenames) {
    //     string content = readFileContent(filename);
    //     if (!content.empty()) {
    //         cout << "Parsing " << filename << "..." << endl;
    //         Parser parser(content);
    //         parser.parse();
    //         cout << "Done parsing " << filename << ".\n" << endl;
    //     }
    // }

    // return 0;


    //new lang scanner test
    string input = "begin\n  a_ = b;\n  a = b+2;\n end.";
    Scanner scanner(input);

    pair<string, string> token = scanner.nextToken();

    while (token.first != Scanner::eoIToken) { // Use the static member variable for comparison
        cout << token.first << ": " << (token.second.empty() ? "N/A" : token.second) << endl;
        token = scanner.nextToken();
    }

    return 0;





//BOTH OF THESE TEST MAINS PASS FOR THE OLD LANGUAGE

//PARSER SIMPLE TEST
    // string s = "a = 1; b = a + 2; if (a*b) a = b+1; while (a) b = a+b";

    // // Instantiate the Parser with the test string
    // Parser parser(s); // Assuming your Parser's constructor accepts a std::string

    // // Call the parse method
    // parser.parse();

    // return 0; // Indicate successful execution


//SCANNER TEST MAIN
    // string input = "int xyz, int a = 12, 13; # Now the while loop # while (xyz >= 0) { xyz = xyz - 1; }";
    // Scanner scanner(input);

    // pair<string, string> token = scanner.nextToken();

    // while (token.first != Scanner::eoIToken) { // Use the static member variable for comparison
    //     cout << token.first << ": " << (token.second.empty() ? "N/A" : token.second) << endl;
    //     token = scanner.nextToken();
    // }

    // return 0;
}
