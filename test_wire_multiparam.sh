#!/bin/bash

# changing the wire parameters 
echo "Termination Resistance (N) Range: $1"
echo "Termination Resistance (S) Range: $2"
echo "Gain Factor (N) Range: $3"
echo "Gain Factor (S) Range: $4"


# compiling the file ???
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )

# input values go into the test and a root file is created with that name
for (( n=0 ; n<=$1 ; n++ )); 
do
  for (( i=0 ; i<=$2 ; i++ )); 
  do 
    for (( j=0 ; j<=$3 ; j++ )); 
    do 
      for (( k=0 ; k<=$4 ; k++ )); 
      do 
        ./test_random_v2 $n $i $j $k  
      done
    done 
  done
done

: << 'COMMENT'
# customize top and bottom of ranges: 

# changing the wire parameters 
echo "Termination Resistance (N) Range: [$1, $2]"
echo "Termination Resistance (S) Range: [$3, $4]"
echo "Gain Factor (N) Range: [$5, $6]"
echo "Gain Factor (S) Range: [$7, $8]"



# input values go into the test and a root file is created with that name
for (( n=$1 ; n<=$2 ; n++ )); 
do
  for (( i=$3 ; i<=$4 ; i++ )); 
  do 
    for (( j=$5 ; j<=$6 ; j++ )); 
    do 
      for (( k=$7 ; k<=$8 ; k++ )); 
      do 
        ./test_random_v2 $n $i $j $k  
      done
    done 
  done
done
COMMENT 
