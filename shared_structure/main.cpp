#include "memory_manager.h"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {

    // file and commandline input validations 
    std::cout <<"** Performing Checks **" << std::endl;

    if (argc !=2){
        std::cout << "required number of commandline intputs incorrect, <allocation type> <datafile>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string allocType = argv[1]; // allocation method defined in commandline 
    std::string dataSource = argv[2]; // save source file

    // source file check and open
    std::ifstream inputFile;
    if(std::filesystem::exists(dataSource)){
        inputFile.open(dataSource);
        std::cout << "Data Source: Confirmed... " << dataSource << std::endl;
        if(!inputFile.is_open()){
            std::cerr << "Error: could not open data source file " << dataSource << std::endl;
            return EXIT_FAILURE;
        }
    } else {
        std::cout << "Data Source: Failed... " << "Source file is incorrect or does not exist" << std::endl;
    }

    if(allocType == "firstfit"){
        // Set the allocation strategy to First Fit
        setAllocationStrategy(firstFitAlloc, firstFitDealloc);
        void* chunk1 = alloc(64);  // Allocate memory using First Fit
        
        // Deallocate memory using First Fit
        dealloc(chunk1);
    }else if(allocType == "bestfit"){
        // Set the allocation strategy to Best Fit
        setAllocationStrategy(bestFitAlloc, bestFitDealloc);
        void* chunk2 = alloc(128); // Allocate memory using Best Fit
        
        // Deallocate memory using Best Fit
        dealloc(chunk2);
    }else{
        std::cout << "allocation type input does not match currently accepted inputs, please check input type is valid." << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
