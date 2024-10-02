# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++17 -O -Wno-deprecated-declarations

# Build both executables when `make all` is run
all: firstfit bestfit

# Compile the firstfit program from files in the firstfit/ directory
firstfit: first_fit/first_fit.cpp first_fit/main.cpp
	$(CXX) $(CXXFLAGS) -o firstfit first_fit/first_fit.cpp first_fit/main.cpp

# Compile the bestfit program from files in the bestfit/ directory
bestfit: best_fit/best_fit.cpp best_fit/main.cpp
	$(CXX) $(CXXFLAGS) -o bestfit best_fit/best_fit.cpp best_fit/main.cpp

# Clean up object and executable files
clean:
	rm -f firstfit bestfit
