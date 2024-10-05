#include "best_fit.h"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <list>

int main(int argc, char* argv[]) {
    std::cout << "** Best Fit Allocation **" << std::endl;
    std::cout << std::endl;
    std::cout << "** Performing Checks **" << std::endl;
    std::cout << std::endl;

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
    std::size_t size;
    bool valid = true;

    while (std::getline(inputFile, line)) {
        valid = true; // reset valid conversion check

        if (line.rfind("alloc:", 0) == 0) {
            std::string size_str = line.substr(6);
            try{
                size = std::stoul(size_str);
            } catch (const std::invalid_argument& e){
                // handle error
                std::cerr << "Invalid size format encountered in data file: " << size_str << std::endl;
                valid = false;
            } catch (const std::out_of_range& e){
                // handle error
                std::cerr << "Encountered Size value that is out of range: " << size_str << std::endl;
                valid = false;
            }

            if(valid){
                void* chunk = alloc(size);
                if (chunk != nullptr) {
                    std::cout << "Allocated " << size << " bytes." << std::endl;
                } else {
                    std::cerr << "Allocation failed for " << size << " bytes." << std::endl;
                }
            }
        } else if (line == "dealloc") {
            if (!allocatedList.empty()) {
                void* chunk = allocatedList.back()->space;
                dealloc(chunk);
                // std::cout << "Deallocated memory." << std::endl;
            } else {
                std::cerr << "No allocated chunks to deallocate." << std::endl;
            }
        }
    }     

    std::cout << "** Allocation Summary **" << std::endl;
    std::cout << std::endl;
    for ( auto it = allocatedList.begin(); it != allocatedList.end(); it++){
        std::cout << "memory address: " << (*it)->space << "," << " allocated space: " << (*it)->total_size << "," << " space used: " << (*it)->requested_size << std::endl;
    }
    std::cout << std::endl;
    std::cout << "** Free List Summary **" << std::endl;
    for(auto it = freeList.begin(); it != freeList.end(); it++){
        std::cout << "memory address: " << (*it)->space << "," << " allocated space: " << (*it)->total_size << std::endl;
    }

    std::cout << "Total number of allocations: " << allocatedList.size() << std::endl;


    inputFile.close();
    return 0;
}
