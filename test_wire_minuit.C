#include <Minuit2/FCNBase.h>
#include <Minuit2/FumiliStandardChi2FCN.h>
#include <TFitterMinuit.h>
//#include <vector> 

// constants that do NOT change from wire-to-wire

const double DCT_wire_length=450.0; // same as geometry toml file
const double DCT_wire_resistance=2200.0; // in Ohms
const double MIN_CHARGEDIV=0.0;
const double MAX_CHARGEDIV=1.0;
const double fXPos = 0.0;


// creating my own class instance of the function base class of minuit
class WireSimFUNC : public ROOT::Minuit2::FCNBase { 

public: 

  WireSimFUNC(double rN = 0, double rS = 0, double g) : funcN(rN), funcS(rS) funcG(g) {}

  double operator() () const {

    // const std::vector<double> & x

    //return  fA*(x[1] - x[0]*x[0])*(x[1] - x[0]*x[0]) + fB*(1 - x[0])*(1 - x[0]);
    
    // find how the gain ratio fits in here 
    //qdv_new = (north_gain/total_resN)/((north_gain/total_resN) + (south_gain/total_resS))

    return DCT_wire_length * (( (funcG / funcN) / ( (funcG / funcN) + (1 / funcS) ) ) - 0.5);
  } 
  
  //double Up() const { return 1.; } // wtf is this

private: 

  double funcN; // termination resistance at the north end of the wire
  double funcS; // termination resistance at the south end of the wire
  double funcG; // gain ratio (gain at north end / gain at south end)

};


int test_wire_minuit() {
  //gSystem->Load("libMinuit2"); // load library

  TFitterMinuit * minuit = new TFitterMinuit(); // create minuit instance

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