#include "parser.hpp"
#include "scanner.hpp"
#include <iostream>

int main() {
    // Array of file names
    std::string files[] = {"a1", "a2", "a3", "a4", "a5", "a6", "a7", "a8"};

    for(int i = 0; i < 8; i++){
        files[i] = "../inputFilesP1/" + files[i];
    }

    for (const std::string& file : files) {
        std::cout << "Processing file: " << file << std::endl;
        try {
            Parser parser(file);
            parser.parseProgram();
            std::cout << "Parsing completed successfully for " << file << ".\n" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error processing " << file << ": " << e.what() << "\n" << std::endl;
        }
    }

    return 0;
}
