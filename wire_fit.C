#include<iostream>
#include<TH1D.h>
#include<TF1.h>
#include<TCanvas.h>
#include<TRandom.h>
#include<TStyle.h>
#include<TLegend.h>
#include<TROOT.h>
#include<TPaveText.h>

#include <string>
#include <TFile.h>
#include <cmath>

int main(int argc, char *argv[]){

  std::cout << "File Name: " << argv[1] << std::endl;
  std::cout << "Wire Position: " << argv[2] << std::endl;

  std::string filename(argv[1]);
  int wire_zpos = atoi(argv[2]); // integer for now, can change to float if necessary with atof()

  // Step 1: Normalize the histogram of the wire with this postion

  std::unique_ptr<TFile> myFile( TFile::Open(filename) ); // opening the input file 

  // getting the histogram of the wire we need to fit:
  std::unique_ptr<TH1D> proj1(myFile->Get<TH1D>(hposXZDCT->ProjectionX("test1",wire_zpos,wire_zpos))); 

  // normalizing...
  proj1->Scale( 1./histo->Integral(),"WIDTH"); // normalizing using probability density function
  //proj1->Scale( 1./histo->Integral()); // normalizing with probability distribution ("sum of histogram content is equal to 1")

  // plotting the NORMALIZED histogram projection
  proj1->Draw(); 

  // reminder to later save this histogram to the output file 


  // Step 2: Fitting the normalized histogram using ROOT's Fitting Histograms manual page
  // https://root.cern/manual/fitting/

  // linear fit: 
  double linfit(double* x) {
    double intercept=0; slope=1;
    double* fitval = slope*x + intercept;
    return fitval;
  }

  // "Create a TF1 object using the fitf function. The last three parameters
  // specify the range and the number of parameters for the function."
  int entries = proj1->GetEntries(); // number of entries in the original histogram
  TF1 *func = new TF1("fit",linfit,-200,200); // no parameters in this example 

  // fitting the histogram: 

  // "Set the initial parameters to the mean and RMS of the histogram."
  func->SetParameters(500,proj1->GetMean(),proj1->GetRMS());

  // "Give the parameters names."
  func->SetParNames("Constant","Mean_value","Sigma");

  // "Call TH1::Fit with the name of the TF1 object."
  TH1D* lin_hist = proj1->Fit("fit"); 

  // drawing the fitted histogram
  lin_hist->Draw();

  /*
  // periodic fit: 
  double perfit(double *x,double *par) {
    // func 
    return 0;
  }

  // "Create a TF1 object using the fitf function. The last three parameters
  // specify the range and the number of parameters for the function."
  TF1 *func = new TF1("fit",perfit,-3,3,3);

  // fitting the histogram: 

  // "Set the initial parameters to the mean and RMS of the histogram."
  func->SetParameters(500,proj1->GetMean(),proj1->GetRMS());

  // "Give the parameters names."
  func->SetParNames("Constant","Mean_value","Sigma");

  // "Call TH1::Fit with the name of the TF1 object."
  TH1D* per_hist = proj1->Fit("fit");

  // drawing the fitted histogram
  per_hist->Draw();
  */

  
  // Step 3: Saving the histograms to a file 

  /*
  std::string path = "path/to/file";
  const char* cpath = path.c_str(); // converting the file path and name to a c-string

  std::unique_ptr<TFile> myFile( TFile::Open(cpath, "RECREATE") ); // rewriting/creating the file to save to

  per_hist->Write(); // writing the histogram to the file 
  */

}
