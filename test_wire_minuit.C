#include <Minuit2/FCNBase.h>
//#include <Minuit2/FumiliStandardChi2FCN.h>
//#include <TVirtualFitter.h>
//#include <TFitterMinuit.h>
#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"
#include <iostream>
#include <string>
#include <TFile.h>
#include <cmath>
#include <TH2D.h>
#include <TApplication.h>

//#include <vector> 

// constants that do NOT change from wire-to-wire - copied from the test_random file

const double DCT_wire_length=450.0; // same as geometry toml file
const double DCT_wire_resistance=2200.0; // in Ohms
const double MIN_CHARGEDIV=0.0;
const double MAX_CHARGEDIV=1.0;
const double fXPos = 0.0;


void func(double &f, double *pars) {
  /*
  * f will point to the output of the function
  * npars is the number of parameters (to minimize??)
  * pars is an array containing the parameters, both "constant and variable"
  */

  // pars in the format pars = {North Res., South Res., Term. Res. N, Term. Res. S, Gain Ratio}

  double northres = pars[0]; // ** randomly generated ** (constant)
  double southres = pars[1]; // ** randomly generated ** (constant)
  double termresn = pars[2]; // north termination resistance being tested (variable)
  double termress = pars[3]; // south termination resistance being tested (variable)
  double gain_ratio = pars[4]; // gain ratio being tested (variable)

  f = (gain_ratio/(termresn+northres)) / ((gain_ratio/(termresn+northres)) + (1/(termress+southres))) - 0.5;
}


// creating my own class instance of the function base class of minuit
class WireSimFUNC : public ROOT::Minuit2::FCNBase { 

public: 

  WireSimFUNC(double rN = 0, double rS = 0, double g = 1) : funcN(rN), funcS(rS), funcG(g) {}

  double operator() (const std::vector<double> & resistance) const {
    
    // find how the gain ratio fits in here 
    //qdv_new = (north_gain/total_resN)/((north_gain/total_resN) + (south_gain/total_resS))

    double northres = resistance[0];
    double southres = resistance[1]; 

    return DCT_wire_length * ((funcG/(funcN+northres)) / ((funcG/(funcN+northres)) + (1/(funcS+southres))) - 0.5);
  } 
  
  double Up() const { return 1.; } // error definition
private: 

  double funcN; // termination resistance at the north end of the wire
  double funcS; // termination resistance at the south end of the wire
  double funcG; // gain ratio (gain at north end / gain at south end)

};

/*
int test_wire_minuit() {
  gSystem->Load("libMinuit2"); // load library

  TFitterMinuit * minuit = new TFitterMinuit(); // create minuit instance

  // create minimizer - method 2
  ROOT::Math::Minimizer *minimizer = ROOT::Math::Factory::CreateMinimizer(minName="Minuit2", algoName="Migrad");


  WireSimFUNC func; // function instance
  minuit->SetMinuitFCN(&func); // tell minuit to use the wire fit function
  
  // starting values for parameters
  
  //minuit->DefineParameter(parNo=1, "Termination Resistance South", initVal=0, initErr=10, lowerLimit=0, upperLimit=500);
  //minuit->DefineParameter(parNo=2, "Termination Resistance North", initVal=0, initErr=10, lowerLimit=0, upperLimit=500);
  //minuit->DefineParameter(parNo=3, "Gain Ratio", initVal=1, initErr=0.01, lowerLimit=1, upperLimit=2);
  
  double startRes = 10.0; 
  double startGain = 0.001; // minimum gain ratio is when the north < south
  double startX = 0;
  
  // if not limited (vhigh <= vlow) 
  minuit->SetParameter(0,"x",startX,0.1,-DCT_wire_length,DCT_wire_length); //?????

  minuit->SetParameter(0,"r_n",startRes,10.0,0,300.0); // do I need these?????
  minuit->SetParameter(1,"r_s",startRes,10.0,0,300.0);
  minuit->SetParameter(2,"g",startGain,0.01,1.0,2.0);
  //minuit->SetPrintLevel(3);
  
  // create Minimizer (default is Migrad)
  minuit->CreateMinimizer();
  int min = minuit->Minimize(); // global minimum?

  // check if minimum is non-zero??
  if (min != 0) { 
    return min; 
  }

}
*/

/*
int test_wire_minuit() {
  //gSystem->Load("libMinuit2"); // load library


  // create minimizer - method 2
  ROOT::Math::Minimizer *minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  minimizer->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2


  // setting the function that minuit is minimizing: 

  //WireSimFUNC::operator *func()(); // function instance
  //WireSimFUNC *func = new WireSimFUNC();
  //*func = new WireSimFUNC(); // what type??
  double *f; 
  double *pars = [];
    // * first two values of pars is the randomly generated point from the projection



  //const double *params = ;
  //double *grad = ;
  //func->ParameterGradient(params, grad); 

  minimizer->SetFunction(func);

  // starting values for parameters

  //minuit->DefineParameter(parNo=1, "Termination Resistance South", initVal=0, initErr=10, lowerLimit=0, upperLimit=500);
  //minuit->DefineParameter(parNo=2, "Termination Resistance North", initVal=0, initErr=10, lowerLimit=0, upperLimit=500);
  //minuit->DefineParameter(parNo=3, "Gain Ratio", initVal=1, initErr=0.01, lowerLimit=1, upperLimit=2);


  double startRes = 10.0;
  double startGain = 0.001; // minimum gain ratio is when the north < south
  //double startX = 0;

  // defining parameters to be minimized 
  minimizer->SetVariable(0,"r_n", 10, 1.0); // in ohms
  minimizer->SetVariable(0,"r_s", 10, 1.0); // in ohms
  minimizer->SetVariable(0,"g", 1.0, 0.01); // unitless

  minimizer->Minimize();

  const double *min = minimizer->X(); // should be three values in this array


  // check if minimum is non-zero??
  //if (min[0] != 0) {
    //return min;
  //}


}
*/

int main(int argc, char *argv[]) {
  //gSystem->Load("libMinuit2"); // load library


  // create minimizer - method 2
  ROOT::Math::Minimizer *minimizer = ROOT::Math::Factory::CreateMinimizer("Minuit2", "Migrad");
  minimizer->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2


  // setting the function that minuit is minimizing: 

  //WireSimFUNC::operator *func()(); // function instance
  //WireSimFUNC *func = new WireSimFUNC();
  //*func = new WireSimFUNC(); // what type??
  double *f; 
  double *pars = new double[5]; // 5 parameters 
    // * first two values of pars is the randomly generated point from the projection

  pars[2] = atof(argv[1]);
  pars[3] = atof(argv[2]);
  pars[4] = atof(argv[3]) / atof(argv[4]);

  // *******************************************************************************************************************
  std::string filename(argv[5]);
  int wire_zpos = atoi(argv[6]); // integer for now, can change to float if necessary with atof()

  // Step 1: Normalize the histogram of the wire with this postion

  std::unique_ptr<TFile> wireFile( TFile::Open(filename.c_str()) ); // opening the input file 

  if (!wireFile || wireFile->IsZombie()) { // checking if the file opened properly
   std::cerr << "Error opening file " << filename << std::endl;
   exit(-1);
  }

  // getting the histogram of the wire we need to fit:
  std::unique_ptr<TH2D> proj0(wireFile->Get<TH2D>("hposXZDCT")); // 2D histogram 
  TH1D* proj1 = proj0->ProjectionX("test1", wire_zpos, wire_zpos); // 1D histogram of hit pos. likelihood based on z pos.
  double min_val = proj1->GetBinCenter(proj1->FindFirstBinAbove(0));

  // scaling the wire distribution to [0,1]
  double qdv_true = (proj1->GetRandom() - min_val) / ((-min_val) - min_val);
  // *******************************************************************************************************************

  pars[0] = (1.0 - qdv_true) * DCT_wire_resistance;
  pars[1] = qdv_true * DCT_wire_resistance;


  //const double *params = ;
  //double *grad = ;
  //func->ParameterGradient(params, grad); 

  //ROOT::Math::Functor fcn(&func, 3);

  WireSimFUNC *func = new WireSimFUNC();
  ROOT::Math::Functor fcn(&(func->operator()()));

  minimizer->SetFunction(fcn); //=========================== FIX THIS =======================================

  // starting values for parameters

  //minuit->DefineParameter(parNo=1, "Termination Resistance South", initVal=0, initErr=10, lowerLimit=0, upperLimit=500);
  //minuit->DefineParameter(parNo=2, "Termination Resistance North", initVal=0, initErr=10, lowerLimit=0, upperLimit=500);
  //minuit->DefineParameter(parNo=3, "Gain Ratio", initVal=1, initErr=0.01, lowerLimit=1, upperLimit=2);


  double startRes = 10.0;
  double startGain = 0.001; // minimum gain ratio is when the north < south
  //double startX = 0;

  // defining parameters to be minimized 
  minimizer->SetVariable(0,"r_n", 10, 1.0); // in ohms
  minimizer->SetVariable(0,"r_s", 10, 1.0); // in ohms
  minimizer->SetVariable(0,"g", 1.0, 0.01); // unitless
  minimizer->SetFixedVariable(0, "North Resistance", pars[0]);
  minimizer->SetFixedVariable(0, "South Resistance", pars[1]);

  minimizer->Minimize();

  const double *min = minimizer->X(); // should be three values in this array, one for each variable parameter

  std::cout << "Minimum North Termination Resistance = " << min[0] << std::endl;
  std::cout << "Minimum South Termination Resistance = " << min[1] << std::endl;
  std::cout << "Minimum Gain Ratio = " << min[2] << std::endl;

  /*
  // check if minimum is non-zero??
  if (min[0] != 0) {
    return min;
  }
  */

  return 0;
}

