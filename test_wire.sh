#!/bin/bash

# changing the wire parameters - hard-coded (only 2 values per iteration)

# compiling the file ???
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )

# running for inital parameter values 
for (( n=10 ; n<21 ; n=n+10 ));
do
  for (( i=10 ; i<21 ; i=i+10 )); 
  do 
    for (( j=1 ; j<3 ; j++ )); 
    do 
      for (( k=1 ; k<3 ; k++)); 
      do 
        ./test_random_v2 $n $i $j $k 
      done
    done
  done
  
done

