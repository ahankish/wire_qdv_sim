# wire_qdv_sim
wire charge division simulation

Command for compiling and executing the code for testing a single wire: 
```bash
# compile the file 
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )
# running the file with example numbers for termination resistance and gain factors
./test_random_v2 10 10 1 1 
```
