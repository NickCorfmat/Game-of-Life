Nicolas Corfmat
ncorfmat
1890805
Assignment 4: The Game of Life

DESCRIPTION

This program simulates a time-step game where a 2-D grid known as a 'Universe' is created based on an initial state, defined by the user-specified input file. Each cell in the grid can be one of either two states, dead or alive, where '.' represents a dead cell and 'o' represents a live cell.

BUILD

To build, run 'make' or 'make all' on the terminal command line within the assignment 4 directory. This creates the 'life' and 'universe' executable files which can then be run.

CLEAN

Running 'make clean' will remove all the executable (.o) files from the assignment 4 directory.

RUNNING

To run, type './life' followed by these required arguments: -i preceding the path to the desired input file and -o followed by the name of the output file. Optionally, the user can specify the number of generations by including -n followed by a number, as well as change the state of the Universe to toroidal by calling -t. Lastly, -s will skip the step that displays the program on a terminal window and simply print the result to the output file. If the user makes an incorrect call to run, the program will automatically display a help message.
