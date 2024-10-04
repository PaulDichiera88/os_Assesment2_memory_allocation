#include "first_fit.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

int main(int argc, char* argv[]) {

    std::cout << "** First Fit Allocation **" << std::endl;
    std::cout << std::endl;
    std::cout << "** Performing Checks **" << std::endl;
    std::cout << std::endl;

    if (argc != 2) {
        std::cerr << "Required input: <datafile> " << std::endl;
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
                // std::cout << "Allocated " << size << " bytes." << std::endl;
            } else {
                std::cerr << "Allocation failed for " << size << " bytes." << std::endl;
            }
        } else if (line == "dealloc") {
            if (!allocatedList.empty()) {
                void* chunk = allocatedList.back().space;
                firstFitDealloc(chunk);
                // std::cout << "Deallocated memory." << std::endl;
            } else {
                std::cerr << "No allocated chunks to deallocate." << std::endl;
            }
        }
    }

    std::cout << "** Allocation Summary **" << std::endl;
    for(auto it = allocatedList.begin(); it != allocatedList.end(); it++){
        std::cout << "memory address: " << it->space << "," << " allocated space: " << it->total_size << "," << " space used: " << it->requested_size << std::endl;
    }
    std::cout << std::endl;
    std::cout << "** Free List Summary **" << std::endl;
    for(auto it = freeList.begin(); it != freeList.end(); it++){
        std::cout << "memory address: " << it->space << "," << " allocated space: " << it->total_size << std::endl;
    }

    inputFile.close();
    return 0;
}
