#!/bin/bash

# Script to generate ks test value and models 
# Generate a uniform random histogram 
root -l << EOF | tee ks_test_log100.txt
// Model Histogram
TH1D* unif_histmodel = new TH1D("uniformModel", "Uniform Random Model", 200,0.0,1.00);
unif_histmodel->FillRandom("pol0", 10000000);

// Test Histograms loop
TH1D* test_hist = new TH1D("uniformTest", "Uniform Random Test Hist", 200,0.00,1.00);
TH1D* kstest_hist = new TH1D("ksTest", "KS Test Per Test Hist", 1000000,0.00,1.00);

// Open a log file to send the KS test values
//TFile* logfile = TFile::Open("ks_test_log100.txt", "RECREATE");

for (int i=0; i<10000; i++) {
  test_hist->FillRandom(unif_histmodel, 100);
  double ks = test_hist->KolmogorovTest(unif_histmodel);
  kstest_hist->Fill(ks);
  std::cout << "KS Test Value = " << test_hist->KolmogorovTest(unif_histmodel) << "\n" << std::endl;
  test_hist->Reset("M");
}

// Save to a .root file
std::unique_ptr<TFile> myFile( TFile::Open("newks_graphs100.root", "RECREATE") );
myFile->cd();
unif_histmodel->Write();
kstest_hist->Write("ksTest");
EOF


: << 'COMMENT'
root -b -l << EOF
const double DCT_wire_length=450.0; // same as geometry toml file
const double DCT_wire_resistance=2200.0; // in Ohms
const double MIN_CHARGEDIV=0.0;
const double MAX_CHARGEDIV=1.0;
const double fXPos = 0.0;

// Model Histogram
TH1D* model = new TH1D("accModel", "Acceptance Model", 200,-0.05,1.05);
std::unique_ptr<TFile> wireFile( TFile::Open("../../helixfiles/acptsim_merged_excl2.root") );
std::unique_ptr<TH2D> proj0(wireFile->Get<TH2D>("hposXZDCT")); 
TH1D* proj1 = proj0->ProjectionX("test1", 130, 130); 
double min_val = proj1->GetBinCenter(proj1->FindFirstBinAbove(0));

model->SetStats(false);
double qdv_true;

for(double i = 0; i < 1000000000; i++){
  qdv_true = (proj1->GetRandom() - min_val) / ((-min_val) - min_val);
  
  double res_north=(1.0-qdv_true)*DCT_wire_resistance;
  double res_south=qdv_true*DCT_wire_resistance;
  double total_resN=res_north + 15;
  double total_resS=res_south + 15;

  double qdv_new=(1.5/total_resN)/((1.5/total_resN)+(1/total_resS));
  model->Fill(qdv_new);
}

// Test Histograms loop
TH1D* test_hist = new TH1D("accTest", "Acceptance Random Test Hist", 200,-0.05,1.05);
test_hist->SetStats(false);
TH1D* kstest_hist = new TH1D("accKSTest", "Acceptance KS Test Per Test Hist", 200,-0.05,1.05);

for (int i=0; i<10000; i++) {
  test_hist->FillRandom(model, 1000000000);
  kstest_hist->Fill(test_hist->KolmogorovTest(model));
  test_hist->Reset("M");
}


// Save to a .root file
std::unique_ptr<TFile> myFile( TFile::Open("newks_graphs1000000000.root", "UPDATE") );
myFile->cd();
model->Write();
kstest_hist->Write("accKSTest");
EOF


root -l << EOF
TCanvas* sCanvas = new TCanvas("randComp","Uniform Random Comparison - # of Entries", 200,50,600,600);
TCanvas* aCanvas = new TCanvas("accComp","Acceptance qdv Comparison - # of Entries", 200,50,600,600);
sCanvas->Divide(2, 4);
aCanvas->Divide(2, 4);
TFile *_file1 = TFile::Open("newks_graphs100.root");
TFile *_file2 = TFile::Open("newks_graphs1000.root");
TFile *_file3 = TFile::Open("newks_graphs10000.root");
TFile *_file4 = TFile::Open("newks_graphs100000.root");
TFile *_file5 = TFile::Open("newks_graphs1000000.root");
TFile *_file6 = TFile::Open("newks_graphs10000000.root");
TFile *_file7 = TFile::Open("newks_graphs100000000.root");
TFile *_file8 = TFile::Open("newks_graphs1000000000.root");


sCanvas->cd(1);
TH1D* tHist1 = (TH1D *) _file1->Get("ksTest");
gPad->SetLogx();
tHist1->Draw();

sCanvas->cd(2);
TH1D* tHist2 = (TH1D *) _file2->Get("ksTest");
gPad->SetLogx();
tHist2->Draw();

sCanvas->cd(3);
TH1D* tHist3 = (TH1D *) _file3->Get("ksTest");
gPad->SetLogx();
tHist3->Draw();

sCanvas->cd(4);
TH1D* tHist4 = (TH1D *) _file4->Get("ksTest");
gPad->SetLogx();
tHist4->Draw();

sCanvas->cd(5);
TH1D* tHist5 = (TH1D *) _file5->Get("ksTest");
gPad->SetLogx();
tHist5->Draw();

sCanvas->cd(6);
TH1D* tHist6 = (TH1D *) _file6->Get("ksTest");
gPad->SetLogx();
tHist6->Draw();

sCanvas->cd(7);
TH1D* tHist7 = (TH1D *) _file7->Get("ksTest");
gPad->SetLogx();
tHist7->Draw();

sCanvas->cd(8);
TH1D* tHist8 = (TH1D *) _file8->Get("ksTest");
gPad->SetLogx();
tHist8->Draw();

sCanvas->SaveAs("newLogKSTest_comparison.png");
sCanvas->Show();


aCanvas->cd(1);
TH1D* aHist1 = (TH1D *) _file1->Get("accKSTest");
gPad->SetLogx();
aHist1->Draw();

aCanvas->cd(2);
TH1D* aHist2 = (TH1D *) _file2->Get("accKSTest");
gPad->SetLogx();
aHist2->Draw();

aCanvas->cd(3);
TH1D* aHist3 = (TH1D *) _file3->Get("accKSTest");
gPad->SetLogx();
aHist3->Draw();

aCanvas->cd(4);
TH1D* aHist4 = (TH1D *) _file4->Get("accKSTest");
gPad->SetLogx();
aHist4->Draw();

aCanvas->cd(5);
TH1D* aHist5 = (TH1D *) _file5->Get("accKSTest");
gPad->SetLogx();
aHist5->Draw();

aCanvas->cd(6);
TH1D* aHist6 = (TH1D *) _file6->Get("accKSTest");
gPad->SetLogx();
aHist6->Draw();

aCanvas->cd(7);
TH1D* aHist7 = (TH1D *) _file7->Get("accKSTest");
gPad->SetLogx();
aHist7->Draw();

aCanvas->cd(8);
TH1D* aHist8 = (TH1D *) _file8->Get("accKSTest");
gPad->SetLogx();
aHist8->Draw();

aCanvas->SaveAs("newLogAccKSTest_comparison.png");
aCanvas->Show();
EOF
