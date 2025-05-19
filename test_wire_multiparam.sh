#!/bin/bash

# changing the wire parameters 
echo "Termination Resistance (N) Range: $1"
echo "Termination Resistance (S) Range: $2"
echo "Gain Factor (N) Range: $3"
echo "Gain Factor (S) Range: $4"


# compiling the file ???
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )

# running for inital parameter values 

# test - display the histogram 
#root -l -q filename
#myFile->ls()
#TH1D * chargediv = (TH1D *) _file0->Get("Charge Division calculated") # variable with the comparison histogram 
#TH1D * originalhist = (TH1D *) _file0->Get("True Position") # variable with the original distribution, no changes to parameters

#chargediv->Draw();

#originalhist->Draw();


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
