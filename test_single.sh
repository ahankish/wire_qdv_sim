#!/bin/bash

# if the directory to store files is not already created, this script will create one 
mkdir wiresim_files
echo $? # if the exit status is 1, then either the directory exists, or there's problems 


# changing the wire parameters - hard-coded (only 2 values per iteration):

# compiling the file ???
g++ -o test_random_v3 test_random_v3.C $( root-config --libs --cflags )


# changing the wire parameters 

./test_random_v3 $1 $2 $3 $4

