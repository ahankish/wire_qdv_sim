#include<iostream>
#include<TH1D.h>
#include<TF1.h>
#include<TCanvas.h>
#include<TStyle.h>
#include<TLegend.h>
#include<TROOT.h>
#include<TPaveText.h>

#include <string>
#include <TFile.h>
#include <TApplication.h>
#include <TSystem.h> 

int main(){
  // open generated files 

  TApplication *app = new TApplication("app", 0, 0);
  gSystem->ProcessEvents(); // for getting the graphics to display

  // filename format wire_termN_termS_gainN_gainS.root
  std::unique_ptr<TFile> nresFile( TFile::Open("wiresim_files/wire_100_0_1_1.root") ); // opening the input file 
  if (!nresFile || nresFile->IsZombie()) { // checking if the file opened properly
   std::cerr << "Error opening file wire_10_0_1_1.root" << std::endl;
   exit(-1);
  }
  //std::unique_ptr<TH1D> h_div(nresFile->Get<TH1D>("Charge Division")); // recalculated histo

  //TH1D *h_div = nresFile->Get<TH1D>("Charge Division");

  TH1D * h_div = (TH1D *) nresFile->Get("Charge Division");

  TCanvas * pdfCanvas = new TCanvas("pdfCanvas","Wire Charge Distribution Comparison", 200,50,600,600);
  //pdfCanvas->SetLeftMargin(0.15);
  //pdfCanvas->SetRightMargin(0.04);
  //pdfCanvas->SetTopMargin(0.04);
  pdfCanvas->Divide(4, 3); // dividing the canvas up to display multiple histograms

  pdfCanvas->cd(1);
  h_div->DrawClone(); // no changes 
  pdfCanvas->cd(5);
  h_div->DrawClone(); // no changes 
  pdfCanvas->cd(9);
  h_div->DrawClone(); // no changes 


  pdfCanvas->cd(2);
  //std::unique_ptr<TFile> nresFile( TFile::Open("wiresim_files/wire_10_0_1_1.root") ); // opening the input file 
  //std::unique_ptr<TH1D> hist2(nresFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  TH1D *hist2 = (TH1D *) nresFile->Get("Charge Division calculated");
  hist2->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  pdfCanvas->cd(3);
  std::unique_ptr<TFile> sresFile( TFile::Open("wiresim_files/wire_0_100_1_1.root") ); // opening the input file 
  //std::unique_ptr<TH1D> hist3(sresFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  TH1D *hist3 = (TH1D *) sresFile->Get("Charge Division calculated");
  hist3->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  pdfCanvas->cd(4);
  std::unique_ptr<TFile> bresFile( TFile::Open("wiresim_files/wire_100_100_1_1.root") ); // opening the input file 
  //std::unique_ptr<TH1D> hist4(bresFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  TH1D *hist4 = (TH1D *) bresFile->Get("Charge Division calculated");
  hist4->DrawClone();
  h_div->DrawClone("SAME"); // no changes 


  //--------------------------------------------------------------------------------------------------------------------

  pdfCanvas->cd(6);
  std::unique_ptr<TFile> ngainFile( TFile::Open("wiresim_files/wire_0_0_1.5_1.root") ); // opening the input file 
  //std::unique_ptr<TH1D> hist6(ngainFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  TH1D *hist6 = (TH1D *) ngainFile->Get("Charge Division calculated");
  hist6->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  pdfCanvas->cd(7);
  std::unique_ptr<TFile> sgainFile( TFile::Open("wiresim_files/wire_0_0_1_1.5.root") ); // opening the input file 
  std::unique_ptr<TH1D> hist7(sgainFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  hist7->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  pdfCanvas->cd(8);
  std::unique_ptr<TFile> bgainFile( TFile::Open("wiresim_files/wire_0_0_1.5_1.5.root") ); // opening the input file 
  std::unique_ptr<TH1D> hist8(bgainFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  hist8->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  //--------------------------------------------------------------------------------------------------------------------

  pdfCanvas->cd(10);
  std::unique_ptr<TFile> nbothFile( TFile::Open("wiresim_files/wire_100_0_1.5_1.root") ); // both N gain and res
  std::unique_ptr<TH1D> hist10(nbothFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  hist10->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  pdfCanvas->cd(11);
  std::unique_ptr<TFile> sbothFile( TFile::Open("wiresim_files/wire_0_100_1_1.5.root") ); // both S gain and res  
  std::unique_ptr<TH1D> hist11(sbothFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  hist11->DrawClone();
  h_div->DrawClone("SAME"); // no changes 

  pdfCanvas->cd(12);
  std::unique_ptr<TFile> bbothFile( TFile::Open("wiresim_files/wire_100_100_1.5_1.5.root") ); // both N and S gain and res 
  std::unique_ptr<TH1D> hist12(bbothFile->Get<TH1D>("Charge Division calculated")); // recalculated histo
  hist12->DrawClone();
  h_div->DrawClone("SAME"); // no changes 


  pdfCanvas->SaveAs("generate_example.png");
  

  //pdfCanvas->Show(); 

  app->Run(); // running all graphics using TApplication

  return 0;

}

  