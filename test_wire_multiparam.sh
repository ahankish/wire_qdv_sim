#!/bin/bash

# changing the wire parameters 
echo "Termination Resistance (N) Range: $1"
echo "Termination Resistance (S) Range: $2"
echo "Gain Factor (N) Range: $3"
echo "Gain Factor (S) Range: $4"
echo "Step Value: $5"

STEP=$5

# checking if a value for STEP was input in the command line 
if [ -z "$STEP" ]; 
then [ STEP = 0.1 ]; # default STEP value 
fi

# d1d2=$(echo "$d1 + $d2" | bc)

tmp=1
tmp1=1
tmp2=1
tmp3=1

# compiling the file ???
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )

#[ bc scale=2 n<=$1 ]
# input values go into the test and a root file is created with that name
for (( n=0 ; tmp ; ));
do
  tmp=$( echo " scale = 2; $n <= $1 "  | bc )
  if [ $tmp == 0 ];
  then
    n=$( echo " scale = 2; $n + $STEP " | bc )
    continue
  fi

  tmp1=1
  for (( i=0 ; tmp1 ; ));
  do 
    tmp1=$( echo " scale = 2; $i <= $2 "  | bc )
    if [ $tmp1 == 0 ];
    then
      i=$( echo " scale = 2; $i + $STEP " | bc )
      continue
    fi

    tmp2=1
    for (( j=0 ; tmp2 ; ));
    do 
      tmp2=$( echo " scale = 2; ( $j <= $3 ) "  | bc )
      if [ $tmp2 == 0 ];
      then
        j=$( echo " scale = 2; $j + $STEP " | bc )
        continue
      fi

      tmp3=1
      for (( k=0 ; tmp3 ; ));
      do 
        tmp3=$( echo " scale = 2; ( $k <= $4 ) "  | bc )
        if [ $tmp3 == 0 ];
        then
          k=$( echo " scale = 2; $k + $STEP " | bc )
          continue
        fi

        ./test_random_v2 $n $i $j $k  
        k=$( echo " scale = 2; $k + $STEP " | bc )

      done
      tmp2=$( echo " scale = 2; $j <= $3 "  | bc )
      j=$( echo " scale = 2; $j + $STEP " | bc )

    done 
    tmp1=$( echo " scale = 2; $i <= $2 "  | bc )
    i=$( echo " scale = 2; $i + $STEP " | bc )

  done
  tmp=$( echo " scale = 2; $n <= $1 "  | bc )
  n=$( echo " scale = 2; $n + $STEP " | bc )

done

: << 'COMMENT'
# customize top and bottom of ranges: 

# changing the wire parameters 
echo "Termination Resistance (N) Range: [$1, $2]"
echo "Termination Resistance (S) Range: [$3, $4]"
echo "Gain Factor (N) Range: [$5, $6]"
echo "Gain Factor (S) Range: [$7, $8]"

STEP = $5

# checking if a value for STEP was input in the command line 
if [ -z "$STEP" ]; 
then [ STEP = 0.1 ]; 
fi


# input values go into the test and a root file is created with that name
for (( n=$1 ; n<=$2 ; n=n+$STEP )); 
do
  for (( i=$3 ; i<=$4 ; i=i+$STEP )); 
  do 
    for (( j=$5 ; j<=$6 ; j=j+$STEP )); 
    do 
      for (( k=$7 ; k<=$8 ; k=k+$STEP )); 
      do 
        ./test_random_v2 $n $i $j $k  
      done
    done 
  done
done
COMMENT 
