#include <iostream>
#include <memory>

using namespace std; // don't have to type std:: all the time 

// Class to store the slope, intercept, name, and data for this file
class LineTracker {
  float slope, intercept; // these are private so that they can't be changed 

  public:
  string dataSet; // string to hold generated data points (to be written to a .txt file)
  string fileName; // name of the file: details slope and intercept and stores it in the file name

  LineTracker(float m, float b) { // Constructor with slope as m and intercept as b
    slope = m;
    intercept = b;
  };

  float LineTracker::get_slope() { // getter function for slope
    return slope;
  };

  float LineTracker::get_intercept() { // getter function for intercept
    return intercept;
  };
};


// Generating a random slope and random intercept
void lin_data_gen() {
  // Random number generator for slope and intercept
  TRandom3* rNum = new TRandom3(0);
  float rSlope = rNum->Uniform(-10.0, 10.0); // random slope between [-10, 10]
  float rIntercept = rNum->Uniform(-50.0, 50.0); // random intercept between [-50, 50]

  // Construct file name: "lineData_slope_intercept.txt"
  string fName = "lineData_" + to_string(rSlope) + "_" + to_string(rIntercept) \
  + ".txt"; 

  // Creating a LineTracker to hold information about this line
  LineTracker* line = new LineTracker(rSlope, rIntercept);
  line->fileName = fName;

  // Line generation message
  cout << "Line generated. \n" << "Slope: " << line->get_slope() << ", \
  \nIntercept: " << line->get_intercept() << "\n" << \
  "Now generating data points...\n" << endl;

  // Generating data points along and around this line
  for (int i = 0; i < 1000; i++) {
    // Check if this data point is going to be 'askew'
    if (rNum->Rndm() < 0.05) { // 5% chance of being askew
      line->dataSet += to_string(i) + ", " + \
      to_string(rNum->Uniform(-1000, 1000)) + "\n"; // totally random data point

      continue;
    }
    else {
      // Add this data array to the LineTracker instance
      line->dataSet += to_string(i) + ", " + to_string((line->get_slope() * i) \
      + line->get_intercept()) + "\n";
      
      continue;
    }
  }

  cout << "Data points generated. Saving data to file named " << line->fileName \
  << "\n" << endl;

  // Saving this data to a .txt file
  ofstream file(line->fileName);
  file << line->dataSet;
  file.close();

  // Deallocating memory
  delete rNum;
  delete line;
  delete rSlope;
  delete rIntercept;
}

