** Running the application **

This project has been tested and is working on titan

Once the project is copied to titan we need to enable c++17, please use the following commands

- scl enable devtoolset-12 zsh
- g++ --version "you should see version 12."

From here we are able to use "make" and "make clean" if needed below are the commands to our two different allocation stratergies.

To run bestfit:

./bestfit datafile

To run firstfit:

./firstfit datafile

