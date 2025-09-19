//using namespace MnMatrix;
#include <Minuit2/FCNBase.h>
#include "Minuit2/MnConfig.h"
#include "Minuit2/MnMatrixfwd.h"
#include "Minuit2/LASymMatrix.h"
#include "Minuit2/LAVector.h"
#include "Minuit2/LaInverse.h"
#include "Minuit2/LaOuterProduct.h"
#include "Math/IFunctionfwd.h"
#include "model_generator_v1.C"
// Using the actual Minuit instead of Math::Minimizer


/*
// Derived class from FCNBase
class FCNClass : public ROOT::Minuit2::FCNBase {
  // Member Variables

  // Member Functions 
  double operator()(const std::vector<double>&v) const override {
    // Takes in the qdv parameters and returns a KS Test value for that model vs the data

    if ((v[0] < 0) || (v[1] < 0) || (v[2] < 0)) { // to avoid weird minuit entries
      return - std::log(1);
    }

    // Generating the file
    model_generator_v1(v[0], v[1], v[2], 1,
                        "../../helixfiles/acptsim_merged_excl2.root", v[3]);

    std::ostringstream term_ResN, term_ResS, gain_Ratio, wirepos;
    term_ResN << v[0];
    term_ResS << v[1];
    gain_Ratio << v[2];
    wirepos << v[3];
    std::string termResN = term_ResN.str();
    std::string termResS = term_ResS.str();
    std::string gainRatio = gain_Ratio.str();
    std::string wirelo = wirepos.str();
    std::string backslash("/");

    // making the filename
    std::string filename = "wiresim_files" + wirelo + backslash + "wire_" + termResN + 
                            "_" + termResS + "_" + gainRatio + ".root";

    // opening the model file
    std::unique_ptr<TFile> myFile( TFile::Open(filename.c_str()) );
    if (!myFile || myFile->IsZombie()) { // checking if the file opened properly
      // the file doesn't exist and the program will skip to the next model 
      return - std::log(1);
    }


    std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram
    //std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram


    std::unique_ptr<TFile> testFile( TFile::Open("wiresim_files130/test_hist.root") ); // 10, 10, 1.5 (example of data for test)
    TH1D* test_hist = (TH1D *) testFile->Get("Charge Division calculated");


    double test = model->KolmogorovTest(test_hist);
    if (test < 0) {
      return 1;
    }
    std::cout << "KS Test Result = " << test  << "\n- Log Likelihood Result = " << 0 - std::log(test) << std::endl;
    return 0 - std::log(test);
  }

  double Up() const override {
    return 0;
  }
};
*/

// Derived class from IBaseFunctionMultiDim
class FCNClass : public ROOT::Math::IMultiGenFunction {
  // Member Variables

  // Member Functions 
  ROOT::Math::IBaseFunctionMultiDim * Clone() const override {
    return 0;
  }  

  unsigned int NDim() const override {
    return 4;
  }

  double DoEval(const double * x) const override {
    // Takes in the qdv parameters and returns a KS Test value for that model vs the data

    if ((x[0] < 0) || (x[1] < 0) || (x[2] < 0)) { // to avoid weird minuit entries
      return - std::log(1);
    }

    // Generating the file
    model_generator_v1(x[0], x[1], x[2], 1,
                        "../../helixfiles/acptsim_merged_excl2.root", x[3]);

    std::ostringstream term_ResN, term_ResS, gain_Ratio, wirepos;
    term_ResN << x[0];
    term_ResS << x[1];
    gain_Ratio << x[2];
    wirepos << x[3];
    std::string termResN = term_ResN.str();
    std::string termResS = term_ResS.str();
    std::string gainRatio = gain_Ratio.str();
    std::string wirelo = wirepos.str();
    std::string backslash("/");

    // making the filename
    std::string filename = "wiresim_files" + wirelo + backslash + "wire_" + termResN + 
                            "_" + termResS + "_" + gainRatio + ".root";

    // opening the model file
    std::unique_ptr<TFile> myFile( TFile::Open(filename.c_str()) );
    if (!myFile || myFile->IsZombie()) { // checking if the file opened properly
      // the file doesn't exist and the program will skip to the next model 
      return - std::log(1);
    }


    std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram
    //std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram


    std::unique_ptr<TFile> testFile( TFile::Open("wiresim_files130/test_hist.root") ); // 10, 10, 1.5 (example of data for test)
    TH1D* test_hist = (TH1D *) testFile->Get("Charge Division calculated");


    double test = model->KolmogorovTest(test_hist);
    if (test < 0) {
      return 1;
    }
    std::cout << "KS Test Result = " << test  << "\n- Log Likelihood Result = " << 0 - std::log(test) << std::endl;
    return 0 - std::log(test);
  }

};


void minuit_min(int wirepos) {
  // Create an instance of the minimizer 
  //Minuit2Minimizer* min = new Minuit2Minimizer();
  ROOT::Minuit2::Minuit2Minimizer* min = new ROOT::Minuit2::Minuit2Minimizer();

  FCNClass f;
  //ROOT::Math::IMultiGenFunction f = new FCNClass();

  min->SetFunction(f);
  // Setting Variables
  double step[3] = { 50,50,0.5 }; // close to 50 ohms 
  
  // starting point
  double variable[3];
  variable[0] = 50;
  variable[1] = 50;
  variable[2] = 1.;

  min->SetLowerLimitedVariable(	0, "north termination resistance",variable[0], step[0], 0);
  min->SetLowerLimitedVariable(	1, "south termination resistance",variable[1], step[1], 0);
  min->SetLowerLimitedVariable(	2, "gain ratio",variable[2], step[2], 0);
  min->SetFixedVariable(3, "wire location", wirepos); // position of the wire in the dct

  bool min_status = min->Minimize();

  // Minimum that the function returns 
  std::cout << "Minimum Log Likelihood Value: " << min->MinValue() << std::endl;

  // Minimum parameter values 
  const double* mins = min->X();
  std::cout << "North termination resistance minimum: " << mins[0] << std::endl;
  std::cout << "South termination resistance minimum: " << mins[1] << std::endl;
  std::cout << "Gain ratio minimum: " << mins[2] << std::endl;

}
