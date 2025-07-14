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

/*
double qdv_func(int *ndims, double *params) {
  // takes in the parameters of one model and returns the KS test value for that model
  int termResN_dims = ndims[0]; 
  int termResS_dims = ndims[1]; 
  int gainRatio_dims = ndims[2]; 

  return params[termResN_dims][termResS_dims][gainRatio_dims];
}
  */


// sample array of KS test values
double* params; // to be filled by the model comparisons

double qdv_func2(const double* ndims) {
  // takes in the parameters of one model and returns the KS test value for that model


  // use the test test_wire_multiparam.sh script to generate the files of models 
  // there should be files in the directory wiresim_files<wire location> directory
  // lookup the directory of models for that wire and grab the model histograms 


  int termResN_dims = ndims[0];
  int termResS_dims = ndims[1];
  int gainRatio_dims = ndims[2];

  return params[termResN_dims][termResS_dims][gainRatio_dims];
}

int qdv_minimize(const char * minName = "Minuit2",
                          const char *algoName = "" ,
                          int randomSeed = -1)
{
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
   ROOT::Math::Minimizer* min =
      ROOT::Math::Factory::CreateMinimizer(minName, algoName);

   // set tolerance , etc...
   min->SetMaxFunctionCalls(1000000); // for Minuit/Minuit2
   min->SetMaxIterations(10000);  // for GSL
   min->SetTolerance(0.001);
   min->SetPrintLevel(1);

   // create funciton wrapper for minmizer
   // a IMultiGenFunction type
   ROOT::Math::Functor f(&qdv_func2, 3);
   double step[3] = { 1,1,1 };
   // starting point

   double variable[3] = { 0,0,0 };
   if (randomSeed >= 0) {
      TRandom2 r(randomSeed);
      variable[0] = r.Uniform(0,1);
      variable[1] = r.Uniform(0,1);
      variable[2] = r.Uniform(0,1);
   }

   min->SetFunction(f);

   // Set the free variables to be minimized!
   min->SetVariable(0,"r_n",variable[0], step[0]);
   min->SetVariable(1,"r_s",variable[1], step[1]);
   min->SetVariable(2,"g",variable[2], step[2]);

   // do the minimization
   min->Minimize();

   const double *xs = min->X();
   std::cout << "Minimum: f(" << xs[0] << "," << xs[1] << "," << xs[1] << "): "
             << min->MinValue()  << std::endl;

   // expected minimum is 0
   if ( min->MinValue()  < 1.E-4  && f(xs) < 1.E-4)
      std::cout << "Minimizer " << minName << " - " << algoName
                << "   converged to the right minimum" << std::endl;
   else {
      std::cout << "Minimizer " << minName << " - " << algoName
                << "   failed to converge !!!" << std::endl;
      Error("NumericalMinimization","fail to converge");
   }

   return 0;
}