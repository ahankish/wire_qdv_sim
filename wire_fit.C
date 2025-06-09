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
#include <TH2D.h>
#include <TApplication.h>
#include <TMarker.h>
#include <TSystem.h> 
#include <TVirtualFFT.h>


int main(int argc, char *argv[]){

  std::cout << "File Name: " << argv[1] << std::endl;
  std::cout << "Wire Position: " << argv[2] << std::endl;

  std::string filename(argv[1]);
  int wire_zpos = atoi(argv[2]); // integer for now, can change to float if necessary with atof()

  // Step 1: Normalize the histogram of the wire with this postion

  std::unique_ptr<TFile> myFile( TFile::Open(filename.c_str()) ); // opening the input file 

  if (!myFile || myFile->IsZombie()) { // checking if the file opened properly
   std::cerr << "Error opening file " << filename << std::endl;
   exit(-1);
  }


  // getting the histogram of the wire we need to fit:
  std::unique_ptr<TH2D> proj0(myFile->Get<TH2D>("hposXZDCT")); // 2D histogram 


  TH1D* proj1 = proj0->ProjectionX("test1", wire_zpos, wire_zpos); // 1D histogram of hit pos. likelihood based on z pos.

  //std::unique_ptr<TH1D> proj1(myFile->Get<TH1D>(proj0->ProjectionX("test1", wire_zpos, wire_zpos)));
  
  TApplication *app = new TApplication("app", 0, 0);
  gSystem->ProcessEvents(); // for getting the graphics to display


  TCanvas *c = new TCanvas("c", "hposXZDCT");
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.04);
  c->SetTopMargin(0.04);
  
  TCanvas* c1 = new TCanvas("c1", "c1");
  c1->SetLeftMargin(0.15);
  c1->SetRightMargin(0.04);
  c1->SetTopMargin(0.04);

  c->cd();
  proj0->Draw();
  c1->cd();
  proj1->Draw(); // testing to see if we have the right histogram 

  //app->Run(); // running the graphics using TApplication

  /*
  // normalizing...
  proj1->Scale( 1./histo->Integral(),"WIDTH"); // normalizing using probability density function
  //proj1->Scale( 1./histo->Integral()); // normalizing with probability distribution ("sum of histogram content is equal to 1")

  // plotting the NORMALIZED histogram projection
  proj1->Draw(); 

  // reminder to later save this histogram to the output file 

  */


  // Step 2: Fitting the normalized histogram using ROOT's Fitting Histograms manual page
  // https://root.cern/manual/fitting/

  /*
  // linear fit: 
  double LinFit(double* x) {
    double intercept=0; slope=1;
    double* fitval = slope*x + intercept;
    return fitval;
  }

  // "Create a TF1 object using the fitf function. The last three parameters
  // specify the range and the number of parameters for the function."

  int entries = proj1->GetEntries(); // number of entries in the original histogram

  TF1 *func = new TF1("fit",LinFit,-200,200); // no parameters in this example 

  // fitting the histogram: 

  // "Set the initial parameters to the mean and RMS of the histogram."
  func->SetParameters(500,proj1->GetMean(),proj1->GetRMS());

  // "Give the parameters names."
  func->SetParNames("Constant","Mean_value","Sigma");

  // "Call TH1::Fit with the name of the TF1 object."
  TH1D* lin_hist = proj1->Fit("fit"); 

  // drawing the fitted histogram
  lin_hist->Draw();

  */

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


  //TCanvas* fCanvas = new TCanvas("fCanvas", "FFT Magnitude");
  //TCanvas* fCanvas1 = new TCanvas("fCanvas1", "FFT Phase");

  // Fourier Transform
  /*
  TH1* fftProj1mag = nullptr; 
  fftProj1mag = proj1->FFT(fftProj1mag, "MAG"); // magnitude
  fftProj1mag->SetTitle("Magnitude of the 1st transform");
  fCanvas->cd();
  fftProj1mag->Draw();
  fftProj1mag->SetStats(kFALSE);
  fftProj1mag->GetXaxis()->SetLabelSize(0.05);
  fftProj1mag->GetYaxis()->SetLabelSize(0.05);


  TH1* fftProj1ph = nullptr; 
  fftProj1ph = proj1->FFT(fftProj1ph, "PH"); // phase
  fftProj1ph->SetTitle("Phase of the 1st transform");
  fCanvas1->cd();
  fftProj1ph->Draw();
  fftProj1ph->SetStats(kFALSE);
  fftProj1ph->GetXaxis()->SetLabelSize(0.05);
  fftProj1ph->GetYaxis()->SetLabelSize(0.05);
  */

  TH1* transform = (TH1 *)proj1->FFT(transform, "RE");

  TCanvas* transfCanvas = new TCanvas("transfCanvas", "Transformed Projection");
  transfCanvas->cd();
  transform->Draw();
  //TH1* transform2 = (TH1 *)transform->FFT(transform2, "RE");
  //transfCanvas->cd(2);
  //transform2->Draw();

  //TH1* transform2 = (TH1 *)proj1->FFT(transform2, "R2C");

  //TCanvas* transfCanvas2 = new TCanvas("transfCanvas2", "Second Transformed Projection");
  //transfCanvas2->cd();
  //transform2->Draw();



  /*
  // Rebinning Method
  TH1D* reBin_proj1 = (TH1D *)proj1->Rebin();
  TCanvas* fCanvas2 = new TCanvas("fCanvas2", "Re-Binned");

  fCanvas2->cd();
  reBin_proj1->Draw();
  */


  app->Run();


  // Step 3: Saving the histograms to a file 

  /*
  std::string path = "path/to/file";
  const char* cpath = path.c_str(); // converting the file path and name to a c-string

  std::unique_ptr<TFile> myFile( TFile::Open(cpath, "RECREATE") ); // rewriting/creating the file to save to

  per_hist->Write(); // writing the histogram to the file 
  */

  return 0;
}
