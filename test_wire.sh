#!/bin/bash

# if the directory to store files is not already created, this script will create one 
mkdir wiresim_files
echo $? # if the exit status is 1, then either the directory exists, or there's problems 


# changing the wire parameters - hard-coded (only 2 values per iteration):

# compiling the file ???
g++ -o test_random_v3 test_random_v3.C $( root-config --libs --cflags )

# running for inital parameter values 
for (( n=10 ; n<21 ; n=n+10 ));
do
  for (( i=10 ; i<21 ; i=i+10 )); 
  do 
    for (( j=1 ; j<3 ; j++ )); 
    do 
      for (( k=1 ; k<3 ; k++)); 
      do 
        ./test_random_v3 $n $i $j $k 
      done
    done
  done
  
done

