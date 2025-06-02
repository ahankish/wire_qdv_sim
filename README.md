# wire_qdv_sim

## <span style="color:red"> **Wire Charge Division Simulation** </span>

This repository contains files and scripts to test different possible parameters
for a single wire in drift chamber tracker. The main goal is to use these samples 
as models to find the most accurate parameters for each wire in the tracker and
to recalibrate the data that was collected during the 1st flight to reflect 
more accurate hit positions. 

## <span style="color:orange"> What do we already know about the wire? </span>

Constants: 

- **Wire Length** = 450.0 milimeters
- **Wire Resistance** = 2200.0 $\Omega$

Conventions Used:

- **Minimum Charge Division** = 0.0 
- **Maximum Charge Division** = 1.0 
- **South End of the Wire** = 0.0 (x-position)
- **North End of the Wire** = 1.0 (x-position)


## <span style="color:orange"> Parameters: </span> 

There are 4 parameters that are varied in this model: 

1. Termination Resistance for the North end of the wire
2. Termination Resistance for the South end of the wire
3. Gain factor for the North end of the wire
4. Gain factor for the South end of the wire

## <span style="color:orange"> Wire Test File: </span>

Command for compiling and executing the code for testing a single wire: 
```bash
# compile the file 
g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags )
# running the file with example numbers for termination resistance and gain factors
# generates 2 histograms and saves them to a file of the name wire_10.0_10.0_1.0_1.0.root
./test_random_v2 10 10 1 1 
```

### <span style="color:yellow"> *Storage File Format:* </span> 

**Filename:** ```wire_termN_termS_gainN_gainS.root```
 with each parameter being a numerical value

Note: Histograms are stored in a folder called ```wiresim_files/``` that will be 
generated if it is not in your cwd already

eg. ```wire_0_0_0_0.root``` is the filename for the model with 

- North Termination Resistance = 0
- South Termination Resistance = 0 
- North Gain Factor = 0
- South Gain Factor = 0

Each file stores **2 histograms:**

- *"Charge Division"* histogram: 
  - histogram of the wire with each parameter set to 0
- *"Charge Division calculated"* histogram: 
  - histogram of the wire with the adjusted parameters (model)


## <span style="color:orange"> Bash Scripts: </span> 

Running the test file for a hard-coded set of parameters (16 files created total)
- runs for: 
  - North Termination Resistance = 10, 20
  - South Termination Resistance = 10, 20
  - North Gain Factor = 1, 2
  - South Gain Factor = 1, 2

How to run: 

```./test_wire.sh ```


Running the test file with several test values (within a predetermined range) - nested for-loops

How to run: 
- Input parameters - iterate from [0, a]: 
  - North Termination Resistance Range 
  - South Termination Resistance Range
  - North Gain Factor Range 
  - South Gain Factor Range
  - Step Value (default is 1.0)

Example: Each parameter range is for values 0-1 with a 0.5 step (32 files created total) 

```./test_wire_multiparam.sh 1 1 1 1 0.5```


