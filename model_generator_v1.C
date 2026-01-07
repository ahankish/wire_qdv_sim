#include <iostream>
#include <TH1D.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TRandom.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TROOT.h>
#include <TPaveText.h>

#include <string>
#include <TFile.h>
#include <TH2D.h>
#include <TApplication.h>
#include <TMarker.h>
#include <TSystem.h> 
#include <cmath>
//#include <boost/lexical_cast.hpp>
//#include <TVirtualFFT.h>

// constants that do NOT change from wire-to-wire

const double DCT_wire_length=450.0; // same as geometry toml file
const double DCT_wire_resistance=2200.0; // in Ohms
const double MIN_CHARGEDIV=0.0;
const double MAX_CHARGEDIV=1.0;
const double fXPos = 0.0;

double convert_qdv_to_r_n(double in){
  return (1.0-in)*DCT_wire_resistance;
}

double convert_qdv_to_pos(double in){
  double fChargeDivPos = fXPos + DCT_wire_length*((in-MIN_CHARGEDIV)/(MAX_CHARGEDIV-MIN_CHARGEDIV) - 0.5);
  return fChargeDivPos;
}
void make_histo_pretty(TH1D* h,int setblue){
  h->SetLineWidth(2);
  if(setblue==0) h->SetLineColor(kBlue);
  else if(setblue==1) h->SetLineColor(kRed);
  else if(setblue==2) h->SetLineColor(kBlack);
  h->GetXaxis()->SetTitle("Wire Position (cm)"); // I changed this from "idk"
  h->GetYaxis()->SetTitleOffset(1.4);
  h->GetYaxis()->SetTitle("entries");
}
void make_histo_pretty_qdvpos(TH1D* h){
  h->SetLineWidth(2);
  h->SetLineColor(kBlue);
  h->GetXaxis()->SetTitle("South end                                    North end");
  h->GetYaxis()->SetTitleOffset(1.4);
  h->GetYaxis()->SetTitle("entries");
  h->Draw();
}


// main function - grabbing command line arguments 
void model_generator_v1(double termination_resN, double termination_resS, double north_gain, double south_gain,
                        std::string filename, int wirepos) {


  TDirectory::AddDirectory(kFALSE); // setting directory to 0 for all histograms 

  if ((north_gain <= 0) || (south_gain <= 0)) {
    //std::cout << "Gain cannot be negative or equal to 0. " << std::endl;
    //std::cout << "Gain automatically set to 1. " << std::endl;
    north_gain = 1;
    south_gain = 1;
  }

  /*

  // strings for making filenames
  std::ostringstream termresNstr, termresSstr, gain_str, wire_pos;
  termresNstr << termination_resN;
  termresSstr << termination_resS;
  gain_str << north_gain / south_gain;
  wire_pos << wirepos;

  std::string term_resN_str = termresNstr.str();
  std::string term_resS_str = termresSstr.str();
  std::string gainstr = gain_str.str();
  std::string wire_position = wire_pos.str();

  std::string underscore = "_";
  std::string root_str = ".root";
  std::string prefix = "wire_";
  std::string folder = "wiresim_files" + wire_position + "/";
  std::string file_params = term_resN_str + underscore + term_resS_str 
                            + underscore + gainstr;

  std::string path = folder + prefix + file_params + root_str;
  //std::string path = prefix + file_params + root_str;

  */
  auto term_ResN = make_unique<std::string>(std::to_string(termination_resN));
  auto term_ResS = make_unique<std::string>(std::to_string(termination_resS));
  auto gain_Ratio = make_unique<std::string>(std::to_string(north_gain/south_gain));
  auto wirelo = make_unique<std::string>(std::to_string(wirepos));

  std::string file_params;
  file_params.append(*term_ResN);
  file_params += "_";
  file_params.append(*term_ResS);
  file_params += "_";
  file_params.append(*gain_Ratio);


  std::string path("wiresim_files");
  path.append(*wirelo);
  path.append("/");
  path += "wire_";
  path.append(*term_ResN);
  path += "_";
  path.append(*term_ResS);
  path += "_";
  path.append(*gain_Ratio);
  path += ".root";


//======================================================================================================================
  // Step 1: Normalize the histogram of the wire with this postion

  std::unique_ptr<TFile> wireFile( TFile::Open(filename.c_str()) ); // opening the input file 

  if (!wireFile || wireFile->IsZombie()) { // checking if the file opened properly
   std::cerr << "Error opening file " << filename << std::endl;
   exit(-1);
  }
  /*
  else {
    std::cout << "Successfully opened file: " << filename << std::endl;
  }
  */

  //TApplication *app = new TApplication("app", 0, 0);
  gROOT->SetBatch(kTRUE);
  //gSystem->ProcessEvents(); // for getting the graphics to display

  // getting the histogram of the wire we need to fit:
  std::unique_ptr<TH2D> proj0(wireFile->Get<TH2D>("hposXZDCT")); // 2D histogram 
  TH1D* proj1 = proj0->ProjectionX("test1", wirepos, wirepos); // 1D histogram of hit pos. likelihood based on z pos.
  double min_val = proj1->GetBinCenter(proj1->FindFirstBinAbove(0));
  

  std::string proj_name = "cDCTproj" + file_params;
  TCanvas* cDCTproj = new TCanvas(proj_name.c_str(), "X Projection - 1D Histogram");
  cDCTproj->SetLeftMargin(0.15);
  cDCTproj->SetRightMargin(0.04);
  cDCTproj->SetTopMargin(0.04);
  cDCTproj->cd();
  //proj1->Draw(); // testing to see if we have the right histogram 

  //app->Run(); // running the graphics using TApplication

//======================================================================================================================


// graphing the histogram(s)
  int num_entries=10000000; // 8/18: Increased from 1_000_000 to 10_000_000 entries
  //int num_entries=100000000; // 8/18: Increased from 10_000_000 to 100_000_000 entries
  gROOT->Reset();
  TStyle * plain = new TStyle("plain","plain");
  plain->SetCanvasBorderMode(0);
  plain->SetPadBorderMode(0);
  plain->SetPadColor(0);
  plain->SetCanvasColor(0);
  plain->SetTitleColor(1);
  plain->SetStatColor(0);
  plain->SetTitleFillColor(0);
  gROOT->SetStyle("plain");
  gStyle->SetPalette(1);

  // min and max chargediv can be used to first sample for fraction
  //std::string h_div_name = "Charge Division " + file_params;
  //std::string h_true_pos_name = "True Position " + file_params; 
  //std::string h_calcqdv_name = "Charge Division calculated " + file_params;

  TH1D * h_div = new TH1D("Charge Division", "", 200,-0.05,1.05);  
  TH1D * h_true_pos = new TH1D("True Position", "", 200,-1.1*DCT_wire_length/2.0,1.1*DCT_wire_length/2.0); //*********** */
  TH1D * h_calcqdv = new TH1D("Charge Division calculated", "", 200,-0.05,1.05);  

  //TH1D * h_div = new TH1D(h_div_name, "", 200,-0.05,1.05);  
  //TH1D * h_true_pos = new TH1D(h_true_pos_name, "", 200,-1.1*DCT_wire_length/2.0,1.1*DCT_wire_length/2.0); //*********** */
  //TH1D * h_calcqdv = new TH1D(h_calcqdv_name, "", 200,-0.05,1.05);  

  //disable display of histogram statistics
  h_div->SetStats(false);
  h_calcqdv->SetStats(false);
  h_true_pos->SetStats(false);
  //fill with true hit positions
  for(double i = 0; i < num_entries; i++){
    double qdv_true;
    
    // scaling the wire distribution to [0,1]
    qdv_true = (proj1->GetRandom() - min_val) / ((-min_val) - min_val);

    h_div->Fill(qdv_true);

    double qdv_true_pos= convert_qdv_to_pos(qdv_true);
    h_true_pos->Fill(qdv_true_pos);

    // if qdv is close to 1 then the hit position was close to north side and we should get a larger 
    // voltage reading for north side and less resistance from wire contributing!
    double res_north=(1.0-qdv_true)*DCT_wire_resistance;
    double res_south=qdv_true*DCT_wire_resistance;
    double total_resN=res_north+termination_resN;
    double total_resS=res_south+termination_resS;
        
    double qdv_new=(north_gain/total_resN)/((north_gain/total_resN)+(south_gain/total_resS));
    h_calcqdv->Fill(qdv_new);
  }

  // now apply a voltage to this
  /*
  TCanvas * c = new TCanvas(h_div_name.c_str(),"Calculated Wire Charge Distribution", 200,10,600,600);
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.04);
  c->SetTopMargin(0.04);
  //Legend
  TLegend* Leg = new TLegend(0.3,0.8,0.99,0.99);
  Leg->SetFillColor(0);
  Leg->SetTextFont(62);
  make_histo_pretty(h_div,0);
  make_histo_pretty(h_calcqdv,1);

  //h_calcqdv->Draw();
  //h_div->Draw("SAME");

  // c->SaveAs("GainRatio_resistance_model.png"); // SAVING THE HISTOGRAM AS A PNG 

  TCanvas * c2 = new TCanvas(h_calcqdv_name.c_str(),"Original Wire Charge Distribution", 200,50,600,600);
  c2->SetLeftMargin(0.15);
  c2->SetRightMargin(0.04);
  c2->SetTopMargin(0.04);
  make_histo_pretty_qdvpos(h_true_pos);
  */


  //app->Run(); // running all graphics using TApplication

  // saving the histogram: 
  float gain_ratio = north_gain / south_gain;
  if (std::isinf( gain_ratio )) {
    std::cout << "Gain Ratio cannot be infinite. Aborting model creation." << std::endl;
  }


  else {
    const char *path_c = path.c_str();
    std::unique_ptr<TFile> myFile( TFile::Open(path_c, "RECREATE") );

    h_div->Write(); // histogram with original distribution (parameters are 0 0 0 0)
    h_calcqdv->Write(); // histogram with new distribution/parameter values 

    std::cout << "New file created. Path to file: " << path_c << std::endl;
  }

}
