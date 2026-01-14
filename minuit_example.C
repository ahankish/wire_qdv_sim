//using namespace MnMatrix;
#include <Minuit2/FCNBase.h>
#include "Minuit2/MnConfig.h"
#include "Minuit2/MnMatrixfwd.h"
#include "Minuit2/LASymMatrix.h"
#include "Minuit2/LAVector.h"
#include "Minuit2/LaInverse.h"
#include "Minuit2/LaOuterProduct.h"
#include "Minuit2/MnStrategy.h" 
#include "Minuit2/Minuit2Minimizer.h"
#include "Math/IFunctionfwd.h"
#include "model_generator_v1.C"
#include <vector>
#include "TRandom3.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TApplication.h"


// Using the actual Minuit instead of Math::Minimizer
// Testing minimizing the variables one at a time

int EntryNumber = 0; // To keep track of Minuit attempt - for plotting efficacy
// essentially debugging 

// Plotting Residuals: 
std::unique_ptr<TCanvas> newCanvas(new TCanvas("randComp","Residuals of Minimization Parameter", 200,50,600,600));
std::unique_ptr<TH1D> resPlt(new TH1D("residuals","Residuals of Minimization Parameter", 100, 0, 50000));
//std::unique_ptr<TFile> residualFile( TFile::Open("residuals_plot.root", "RECREATE") );
TRandom3* rNum = new TRandom3(0);
//double randInputs[3] = {0.1 + (rNum->Rndm() * 299.9), 0.1 + (rNum->Rndm() * 299.9), 0.1 + (rNum->Rndm() * 9.9)};


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

    if ((x[0] <= 0) || (x[1] <= 0) || (x[2] <= 0)) { // to avoid weird minuit entries
      //return std::log(1);
      return 1000;
    }

    // New pointers 
    //auto wirePos2 = make_unique<clas.>(args)
    auto term_ResN = make_unique<std::string>(std::to_string(x[0]));
    auto term_ResS = make_unique<std::string>(std::to_string(x[1]));
    auto gain_Ratio = make_unique<std::string>(std::to_string(x[2]));
    auto wirelo = make_unique<std::string>(std::to_string(static_cast<int>(x[3])));

    std::string filename("wiresim_files");
    filename.append(*wirelo);
    filename.append("/");
    filename += "wire_";
    filename.append(*term_ResN);
    filename += "_";
    filename.append(*term_ResS);
    filename += "_";
    filename.append(*gain_Ratio);
    filename += ".root";


    model_generator_v1(x[0], x[1], x[2], 1,
                          "../../helixfiles/acptsim_merged_excl2.root", x[3]);
                  
    // Opening the model file
    std::unique_ptr<TFile> myFile( TFile::Open(filename.c_str()) );

    std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram

    std::unique_ptr<TFile> testFile( TFile::Open("wiresim_files130/test_hist.root") ); // 10, 10, 1.5 (example of data for test)
    TH1D* test_hist = (TH1D *) testFile->Get("Charge Division calculated");


    // Performing Chi^2 test: 
    int num_bins = 200;
    double chi2 = 0; 
    for (int i = 1; i <= num_bins; i++) {
              //std::cout << model->GetBinContent(i) << " " << test_hist->GetBinContent(i) << std::endl;
      //std::cout << model->GetEntries() << " " << test_hist->GetEntries() << std::endl;
      if (model->GetBinContent(i) <= 0) {
        continue;
      }
      else {
        chi2 += pow((test_hist->GetBinContent(i)/100) - (model->GetBinContent(i)), 2) / (model->GetBinContent(i));
      }
    }

    std::cout << "Chi^2 Result = " << chi2/num_bins << "\n"<< std::endl;

    // Adding to residuals plot: 
    resPlt->Fill(chi2/num_bins); 

    // Marking Entry Number and adding to efficacy plot 
    EntryNumber++;
    return chi2/num_bins;

    // Deallocating memory
    //delete test_hist;
  }

};


void minuit_min2(int wirepos) {
  // Create an instance of the minimizer 
  //Minuit2Minimizer* min = new Minuit2Minimizer();
  ROOT::Minuit2::Minuit2Minimizer* min = new ROOT::Minuit2::Minuit2Minimizer();

  double *const randInputs = new double[3];
  randInputs[0] = 0.1 + (rNum->Rndm() * 299.9);
  randInputs[1] = 0.1 + (rNum->Rndm() * 299.9);
  randInputs[2] = 0.1 + (rNum->Rndm() * 9.9);
  std::cout << randInputs[0] << ", " << randInputs[1] << ", " << randInputs[2] << std::endl;


  // Instance of the Strategy class MnStrategy
  ROOT::Minuit2::MnStrategy* strat = new ROOT::Minuit2::MnStrategy();
  strat->SetHighStrategy(); // the minimizer will waste no resources looking for the minimum

  FCNClass f;

  min->SetFunction(f);
  // Setting Variables
  double step[3] = { 5,5,0.5 }; // close to 50 ohms 

  // starting point
  double variable[3] = {5,20,0.9};
  double lower[3] = {0.0000001, 0.0000001, 0.0000001};
  double upper[3] = {300, 300, 10};

  
  // Generate random values for the fixed variables
  //TRandom3* rNum = new TRandom3();
  //auto northRand = rNum->Rndm();
  //auto southRand = rNum->Rndm();
  //auto gainRand = rNum->Rndm();

  //double rand[3] = {gRandom->Uniform(0.0000001,300), gRandom->Uniform(0.0000001,300), gRandom->Uniform(0.0000001,10)};
  //double rand[3] = {299.999999*northRand + 0.0000001, 299.999999*southRand + 0.0000001, 9.999999*gainRand + 0.0000001};
  //double randInputs[3] = {0.1 + (rNum->Rndm() * 299.9), 0.1 + (rNum->Rndm() * 299.9), 0.1 + (rNum->Rndm() * 9.9)};

  //std::cout << southRand << " is the random south termination resistance" << std::endl;
  //std::cout << gainRand << " is the random gain ratio" << std::endl;

/*
  min->SetLimitedVariable(0, "north termination resistance", variable[0], step[0], lower[0], upper[0]); 
  min->SetLimitedVariable(1, "south termination resistance",variable[1], step[1], lower[1], upper[1]);
  min->SetLimitedVariable(	2, "gain ratio",variable[2], step[2], lower[2], upper[2]);
  min->SetFixedVariable(3, "wire location", wirepos); // position of the wire in the dct
*/

  min->SetLimitedVariable(0, "north termination resistance", variable[0], step[0], lower[0], upper[0]); 
  min->SetFixedVariable(1, "south termination resistance", randInputs[1]);
  min->SetFixedVariable(2, "gain ratio", randInputs[2]);
  min->SetFixedVariable(3, "wire location", wirepos); // position of the wire in the dct


  //TApplication *app = new TApplication("app", 0, 0);
  //gSystem->ProcessEvents(); // for getting the graphics to display


  bool min_status = min->Minimize();

  // Minimum that the function returns 
  std::cout << "Minimum Log Likelihood Value: " << min->MinValue() << std::endl;

  // Minimum parameter values 
  const double* mins = min->X();
  std::cout << "North termination resistance minimum: " << mins[0] << std::endl;
  std::cout << "South termination resistance minimum: " << mins[1] << std::endl;
  std::cout << "Gain ratio minimum: " << mins[2] << std::endl;

  
  // Saving residuals plot to a root file
  std::unique_ptr<TFile> residualFile( TFile::Open("residuals_plot.root", "RECREATE") );
  resPlt->Write();
  //residualFile->WriteObject(&resPlt, "residuals plot");


  //resPlt->Draw("randComp"); // drawing the residuals plot on the canvas
  //newCanvas->SaveAs("residuals_plot.png"); // saving the plot as a png file
  //app->Run();


  // Deallocating Memory
  delete min;
  delete strat;
  delete &f;
  delete [] randInputs;

}

