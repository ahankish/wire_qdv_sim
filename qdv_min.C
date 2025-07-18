// Example on how to use the new Minimizer class in ROOT
//  Show usage with all the possible minimizers.
// Minimize the Rosenbrock function (a 2D -function)
// This example is described also in
// http://root.cern.ch/drupal/content/numerical-minimization#multidim_minim
// input : minimizer name + algorithm name
// randomSeed: = <0 : fixed value: 0 random with seed 0; >0 random with given seed
//
//Author: L. Moneta Dec 2010

// Adapted by Alexandra Hankish in 2025

#include "Math/Minimizer.h"
#include "Math/Factory.h"
#include "Math/Functor.h"
#include "TRandom2.h"
#include "TError.h"
#include <iostream>
#include<TH1D.h>
#include<TH2D.h>
#include<TF1.h>
#include <TSystem.h>
#include <TFile.h>
#include <TApplication.h>


double qdv_func3(const double* ndims) {
  // takes in the parameters of one model and returns the KS test value for that model

  std::string termResN_dims = std::to_string(static_cast<int>(ndims[0]));
  std::string termResS_dims = std::to_string(static_cast<int>(ndims[1]));
  std::string gainRatio_dims = std::to_string(ndims[2]).substr(0,4); // the gain ratio only goes to the thousandths place right now
  std::string wirelo = std::to_string(static_cast<int>(ndims[3])); // the position of the wire in the dct
  std::string backslash("/");
  std::string folder = "wiresim_files" + wirelo + backslash;
  
  // making the filename 
  std::string filename = folder + "wire_" + termResN_dims + "_" + termResS_dims + "_" + gainRatio_dims + ".root";

  //make sure there is a file that exists with those model parameters 
  //std::string command = "./test_random_v4 " + termResN_dims + " " + termResS_dims + " " + 
  //                      gainRatio_dims + " " + "../../helixfiles/acptsim_merged_excl2.root" + wirelo;
  //gSystem->Exec(command.c_str());
  
  // opening the model file
  std::unique_ptr<TFile> myFile( TFile::Open(filename.c_str()) );
  if (!myFile || myFile->IsZombie()) { // checking if the file opened properly
    // the file doesn't exist and the program will skip to the next model 
    return 1;
  }
  //else {
    //std::cout << "Found the file!" << std::endl;
  //}

  std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Division calculated")); // the model histogram
  //std::unique_ptr<TH1> model(myFile->Get<TH1>("Charge Div KS")); // the model histogram

  // testing against a random histogram
  TH1D* test_hist = new TH1D("Test Histogram", "", 200,-0.05,1.05);
  std::unique_ptr<TFile> wireFile( TFile::Open("../../helixfiles/acptsim_merged_excl2.root") );
  std::unique_ptr<TH2D> proj0(wireFile->Get<TH2D>("hposXZDCT"));
  TH1D* proj1 = proj0->ProjectionX("test1", 130, 130);
  double min_val = proj1->GetBinCenter(proj1->FindFirstBinAbove(0));
  for (int i=0; i<1000000; i++) {
    // Filling with uniform random values
    //test_hist->Fill( gRandom->Uniform(0,1) );
    test_hist->Fill((proj1->GetRandom() - min_val) / ((-min_val) - min_val));
  }

  //TApplication *app = new TApplication("app", 0, 0);
  //gSystem->ProcessEvents(); // for getting the graphics to display
  //test_hist->Draw();
  //model->Draw();
  //app->Run(); // running the graphics using TApplication
  
  /*
  std::unique_ptr<TFile> wireFile( TFile::Open("../../helixfiles/acptsim_merged_excl2.root") );
  std::unique_ptr<TH2D> proj0(wireFile->Get<TH2D>("hposXZDCT"));
  TH1D* proj1 = proj0->ProjectionX("test1", 130, 130);
  TH1* reBinnedProj = proj1->Rebin(5, "Rebinned projection");
  */

  /*
  //----------------------------------------------------------------------------------
  // Fix the bin edges for the K-S test
  TAxis* xaxis = proj1->GetXaxis();
  int lowedge = xaxis->GetBinLowEdge(0);
  std::cout << "Bin Low Edge = " << lowedge << std::endl;
  int upedge = xaxis->GetBinUpEdge(199);
  std::cout << "Bin Up Edge = " << upedge << std::endl;
  //----------------------------------------------------------------------------------
  */

  // compare the histograms - the data (histo) is the one that we want to compare everything to
  //int tmp = static_cast<int>((1 - model->KolmogorovTest(proj1)) * 1000.);
  //int tmp = static_cast<int>((1 - model->KolmogorovTest(reBinnedProj)) * 1000.);
  //return static_cast<float>(tmp/1000.); //  flipping it because the function is going to minimize? 

  //double test = model->KolmogorovTest(proj1);
  //double test = model->KolmogorovTest(reBinnedProj);
  double test = model->KolmogorovTest(test_hist);
  std::cout << "KS Test Result = " << test << std::endl;
  return 1. - test;
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


   // arg 2 is the histogram from the data: 
   //std::string datafile(argv[1]); 
   //std::unique_ptr<TFile> myFile( TFile::Open(datafile.c_str()) );
   //std::unique_ptr<TH1> data(myFile->Get<TH1>("insert name here")); // the data histogram

   ROOT::Math::Minimizer* min =
      ROOT::Math::Factory::CreateMinimizer("Minuit2", "");

   // set tolerance , etc...
   min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
   min->SetMaxIterations(10000);  // for GSL
   //min->SetTolerance(0.001);
   min->SetPrintLevel(1);

   // create funciton wrapper for minmizer
   // a IMultiGenFunction type
   ROOT::Math::Functor f(&qdv_func3, 5);
   double step[3] = { 10,10,0.01 };
   // starting point

   double variable[3];
   variable[0] = 10;
   variable[1] = 10;
   variable[2] = 1.;

   //TRandom2 r(-1);
   //variable[0] = r.Uniform(10.,200.); // not sure what the ranges for these should be 
   //variable[1] = r.Uniform(10.,200.);
   //variable[2] = r.Uniform(1.,5.);


   min->SetFunction(f);

   // Set the free variables to be minimized!
   min->SetVariable(0,"north termination resistance",variable[0], step[0]);
   min->SetVariable(1,"south termination resistance",variable[1], step[1]);
   min->SetVariable(2,"gain",variable[2], step[2]);
   min->SetFixedVariable(3, "wire location", wirepos); // position of the wire in the dct

   // do the minimization
   min->Minimize();

   const double *xs = min->X();
   std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "," << xs[2] << "): "
             << min->MinValue()  << std::endl;

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