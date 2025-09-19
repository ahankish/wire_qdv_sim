#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <TH1D.h>
#include <TH2D.h>
#include <TF1.h>
#include <TSystem.h>
#include <TFile.h>
#include <TApplication.h>
#include <cmath>
#include "model_generator_v1.C"


double qdv_func3(const double* ndims) {
  // takes in the parameters of one model and returns the KS test value for that model
  std::cout << "(" << ndims[0] << ", " << ndims[1] << ", " << ndims[2] << ")" << std::endl;

  if ((ndims[0] < 0) || (ndims[1] < 0) || (ndims[2] < 0)) { // to avoid weird minuit 
    return - std::log(1);
  }

  if ((std::isnan(ndims[0])) || (std::isnan(ndims[1])) || (std::isnan(ndims[2]))) { // to avoid weird minuit 
    return - std::log(1);
  }
  // generating the file
  model_generator_v1(ndims[0], ndims[1], ndims[2], 1,
                      "../../helixfiles/acptsim_merged_excl2.root", ndims[3]);

  std::ostringstream term_ResN, term_ResS, gain_Ratio, wirepos;
  term_ResN << ndims[0]; 
  term_ResS << ndims[1]; 
  gain_Ratio << ndims[2]; 
  wirepos << ndims[3];
  std::string termResN = term_ResN.str();
  std::string termResS = term_ResS.str();
  std::string gainRatio = gain_Ratio.str();
  std::string wirelo = wirepos.str();
  std::string backslash("/");
  //std::string folder = "wiresim_files" + wirelo + backslash;
    
  // making the filename
  std::string filename = "wiresim_files" + wirelo + backslash + "wire_" + termResN + 
                          "_" + termResS + "_" + gainRatio + ".root";

  // opening the model file
  std::unique_ptr<TFile> myFile( TFile::Open(filename.c_str()) );
  if (!myFile || myFile->IsZombie()) { // checking if the file opened properly
    // the file doesn't exist and the program will skip to the next model 
    return 1.;
  }

  //std::string canvas_name = "Charge Division calculated" + termResN + 
                          //"_" + termResS + "_" + gainRatio;
  std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram
  //std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram


  std::unique_ptr<TFile> testFile( TFile::Open("wiresim_files130/test_hist.root") ); // 10, 10, 1.5
  TH1D* test_hist = (TH1D *) testFile->Get("Charge Division calculated");
  
  //std::unique_ptr<TFile> testFile( TFile::Open("wiresim_files130/wire_10_10_1.50.root") );
  //std::unique_ptr<TH1> hist_tmp(testFile->Get<TH1>("Charge Division calculated"));
  //TH1D* test_hist = hist_tmp->Get<const TH1D*>();
  
  /*TH1D* test_hist= new TH1D("Test Histogram", "", 200,-0.05,1.05);
  for (int i=0; i<10000000; i++) {
    // Filling with uniform random values
    //test_hist->Fill( gRandom->Uniform(0,1) );
    test_hist->Fill(hist_tmp->GetRandom());
  }*/

  double test = model->KolmogorovTest(test_hist);
  if (test < 0) {
    return 1;
  }
  std::cout << "KS Test Result = " << test  << "\n- Log Likelihood Result = " << 0 - std::log(test) << std::endl;
  return 0 - std::log(test);
}


int main(int argc, char *argv[]){
   // create minimizer giving a name and a name (optionally) for the specific
   // algorithm
   // possible choices are:
   //     minName                  algoName
   // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
   //  Minuit2                     Fumili2
   //  Fumili
   //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
   //                              BFGS2, SteepestDescent
   //  GSLMultiFit
   //   GSLSimAn
   //   Genetic

   int wirepos = atoi(argv[1]); // which wire are we analyzing rn
   // this means the models are stored in files at wiresim_files[wirepos]

   std::string output_file = "logfile_" + std::to_string(wirepos) + ".txt";
   std::ofstream logfile(output_file.c_str(), std::ofstream::app); // appends to the file
   //std::ofstream logfile(output_file.c_str()); // replaces file
   logfile << "\nLog for wire " << wirepos << "... \n";

    // Redirecting cout to write to "output.txt"
    std::cout.rdbuf(logfile.rdbuf());


   ROOT::Math::Minimizer* min =
      ROOT::Math::Factory::CreateMinimizer("Minuit2", "");

   // set tolerance , etc...
   min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
   min->SetMaxIterations(10000);  // for GSL
   //min->SetTolerance(0.001);
   min->SetPrintLevel(3);
   min->SetStrategy(2);

   // create funciton wrapper for minmizer
   // a IMultiGenFunction type
   ROOT::Math::Functor f(&qdv_func3, 4);
   double step[3] = { 50,50,0.5 }; // close to 50 ohms 
   // starting point

   double variable[3];
   variable[0] = 20;
   variable[1] = 20;
   variable[2] = 1.5;

   std::cout << "   Starting Values: " << variable[0] << ", " << variable[1] << ", " << variable[2] << std::endl;
   std::cout << "   Step: " << step[0] << ", " << step[1] << ", " << step[2] << std::endl;
   //TRandom2 r(-1);
   //variable[0] = r.Uniform(10.,200.); // not sure what the ranges for these should be 
   //variable[1] = r.Uniform(10.,200.);
   //variable[2] = r.Uniform(1.,5.);


   min->SetFunction(f);

   // Set the free variables to be minimized!
   //min->SetVariable(0,"north termination resistance",variable[0], step[0]); // try setlimitedvariable
   //min->SetVariable(1,"south termination resistance",variable[1], step[1]);
   //min->SetVariable(2,"gain ratio",variable[2], step[2]);
   min->SetLimitedVariable(	0, "north termination resistance",variable[0], step[0], 0, 100);
   min->SetLimitedVariable(	1, "south termination resistance",variable[1], step[1], 0, 100);
   min->SetLimitedVariable(	2, "gain ratio",variable[2], step[2], 1., 5.);
   min->SetFixedVariable(3, "wire location", wirepos); // position of the wire in the dct


   // do the minimization
   min->Minimize();

   const double *xs = min->X();
   std::cout << "Minimum: " << min->MinValue()  << std::endl;
   std::cout << "  North Term Res: " << xs[0] << std::endl;
   std::cout << "  South Term Res: " << xs[1] << std::endl;
   std::cout << "  Gain Ratio: " << xs[2] << std::endl;

   /*// expected minimum is 0
   if ( min->MinValue()  < 1.E-4  && f(xs) < 1.E-4)
      std::cout << "Minimizer " << "Minuit2" << " - " << ""
                << "   converged to the right minimum" << std::endl;
   else {
      std::cout << "Minimizer " << "Minuit2" << " - " << ""
                << "   failed to converge !!!" << std::endl;
      Error("NumericalMinimization","fail to converge");
   }
   */

   return 0;
}