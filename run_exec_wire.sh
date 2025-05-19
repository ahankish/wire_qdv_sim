#!/bin/bash

# changing the wire parameters 
echo "Termination Resistance (N) Range: $1"
echo "Termination Resistance (S) Range: $2"
echo "Gain Factor (N) Range: $3"
echo "Gain Factor (S) Range: $4"

local TERMRES_N = 0
local TERMRES_S = 0
local GAINN = 0
local GAINS = 0

./test_random_v2 TERMRES_N TERMRES_S GAINN GAINS 

# input values go into the test and a root file is created with that name
for n in {1..5}
do
  TERMRES_N = n
  ./test_random_v2 TERMRES_N TERMRES_S GAINN GAINS 

  for i in {1..5}
  do 
    TERMRES_S = i
    ./test_random_v2 TERMRES_N TERMRES_S GAINN GAINS 

    for j in {1..5}
    do 
      GAINN = j 
      ./test_random_v2 TERMRES_N TERMRES_S GAINN GAINS 

      for k in {1..5} 
      do 
        GAINS = k
        ./test_random_v2 TERMRES_N TERMRES_S GAINN GAINS  
    done 
  done
done
