void generate_example_pdf(){
  // open generated files 

  // filename format wire_termN_termS_gainN_gainS.root

  TCanvas * pdfCanvas = new TCanvas("Comparison","Wire Charge Distribution Comparison", 200,50,600,600);
  pdfCanvas->SetLeftMargin(0.15);
  pdfCanvas->SetRightMargin(0.04);
  pdfCanvas->SetTopMargin(0.04);
  pdfCanvas->Divide(4, 3); // dividing the canvas up to display multiple histograms

  pdfCanvas->cd(1);
  h_div->Draw(); // no changes 
  pdfCanvas->cd(5);
  h_div->Draw(); // no changes 
  pdfCanvas->cd(9);
  h_div->Draw(); // no changes 


  pdfCanvas->cd(2);
  std::unique_ptr<TFile> TFile* nresFile( TFile::Open("wire_10_0_1_1.root") ); // opening the input file 
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo

  pdfCanvas->cd(3);
  std::unique_ptr<TFile> TFile* sresFile( TFile::Open("wire_10_0_1_1.root") ); // opening the input file 
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo


  pdfCanvas->cd(4);
  std::unique_ptr<TFile> TFile* bresFile( TFile::Open("wire_10_0_1_1.root") ); // opening the input file 
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo


  //----------------------------------------------------------------------------

  pdfCanvas->cd(6);
  std::unique_ptr<TFile> TFile* ngainFile( TFile::Open("wire_10_0_1_1.root") ); // opening the input file 
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo


  pdfCanvas->cd(7);
  std::unique_ptr<TFile> TFile* sgainFile( TFile::Open("wire_10_0_1_1.root") ); // opening the input file 
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo


  pdfCanvas->cd(8);
  std::unique_ptr<TFile> TFile* bgainFile( TFile::Open("wire_10_0_1_1.root") ); // opening the input file 
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo


  //----------------------------------------------------------------------------

  pdfCanvas->cd(10);
  std::unique_ptr<TFile> TFile* nbothFile( TFile::Open("wire_10_0_1_1.root") ); // both N gain and res
  std::unique_ptr<TH2D> proj0(nresFile->Get<TH1D>("calculated Charge Division")); // recalculated histo


  pdfCanvas->cd(11);
  std::unique_ptr<TFile> TFile* sbothFile( TFile::Open("wire_10_0_1_1.root") ); // both S gain and res  

  pdfCanvas->cd(12);
  std::unique_ptr<TFile> TFile* bbothFile( TFile::Open("wire_10_0_1_1.root") ); // both N and S gain and res 
  

  //pdfCanvas->Show(); 
}

  