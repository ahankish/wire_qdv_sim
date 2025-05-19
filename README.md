# wire_qdv_sim
wire charge division simulation

Command for compiling and executing the code for testing a single wire: 
```bash
# compile the file 
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )
# running the file with example numbers for termination resistance and gain factors
./test_random_v2 10 10 1 1 
```

### Bash Script: 
Running the test file with several test values (within a predetermined range) - nested for-loops

How to run: 
eg. ./test_wire_multiparam.sh (termination resistance N range) (termination resistance S range) (gain N range) (gain S range)

eg. Each range is 0-1 (16 permutations)
```bash 
./test_wire_multiparam.sh 1 1 1 1
```