#include "first_fit.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {
    std::cout << "** First Fit Allocation **" << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: <datafile>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string dataSource = argv[1];

    if (!std::filesystem::exists(dataSource)) {
        std::cerr << "Data Source: Failed... Source file does not exist" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream inputFile(dataSource);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open data source file " << dataSource << std::endl;
        return EXIT_FAILURE;
    }

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.rfind("alloc:", 0) == 0) {
            std::string size_str = line.substr(6);
            std::size_t size = std::stoul(size_str);
            void* chunk = firstFitAlloc(size);
            if (chunk != nullptr) {
                std::cout << "Allocated " << size << " bytes." << std::endl;
            } else {
                std::cerr << "Allocation failed for " << size << " bytes." << std::endl;
            }
        } else if (line == "dealloc") {
            // Deallocate last allocated chunk
            std::cout << "Deallocating memory." << std::endl;
            // firstFitDealloc(/* appropriate chunk */);
        }
    }

    inputFile.close();
    return 0;
}
