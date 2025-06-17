#!/bin/bash

# if the directory to store files is not already created, this script will create one 
DIR=wiresim_files
mkdir ${DIR}$5
echo $? # if the exit status is 1, then either the directory exists, or there's problems 


# changing the wire parameters - hard-coded (only 2 values per iteration):

# compiling the file ???
g++ -o test_random_v4 test_random_v4.C $( root-config --libs --cflags )


# changing the wire parameters 

./test_random_v4 $1 $2 $3 $4 ../../helixfiles/acptsim_merged_excl2.root $5

