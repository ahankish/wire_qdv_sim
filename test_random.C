#include<iostream>
#include<TH1D.h>
#include<TF1.h>
#include<TCanvas.h>
#include<TRandom.h>
#include<TStyle.h>
#include<TLegend.h>
#include<TROOT.h>
#include<TPaveText.h>

#include <TGNumberEntry.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGClient.h>


class MyMainFrame : public TGMainFrame {
  private: 
  TGMainFrame *fMain;
  TGVerticalFrame *bframe;

  // number entries - user input values 
  TGNumberEntry *term_resN; 
  TGNumberEntry *term_resS;
  TGNumberEntry *gain_ratioN;
  TGNumberEntry *gain_ratioS;
  
  TGGroupFrame *gfResN;
  TGGroupFrame *gfResS;
  TGGroupFrame *gfGainN;
  TGGroupFrame *gfGainS;

  TGLabel *resN;
  TGLabel *resS;
  TGLabel *gainN;
  TGLabel *gainS;


  double termination_resN;
  double termination_resS;
  double Gain_ratio;
  double Gain_ratio2;

  // buttons 
  TGTextButton *quit; // button for exiting the gui and root terminal
  TGTextButton *hist; // button for redrawing the histogram w/ updated conditions

  public: 
  MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);

  void CloseWindow();
  virtual ~MyMainFrame();

  // 
  void SetValue(TGNumberEntry* num_entry, double var_name, double gain_var=1);

  void SetValueResN();
  void SetValueResS();
  void SetValueGainN();
  void SetValueGainS();

  void GraphModel();

};

//constructor of the gui window(s)
MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h){

  SetCleanup(kDeepCleanup); // cleans up the gui windows and canvases
  
  // mainframe
  fMain = new TGMainFrame(p, w, h);


  // setting these to some values so cling doesn't get mad?
  termination_resN=0;
  termination_resS=0;
  Gain_ratio=1;
  Gain_ratio2=1;


  // creating frames for the number entries and buttons
  bframe = new TGVerticalFrame(fMain, 5, 100); 
	fMain->AddFrame(bframe, new TGLayoutHints(kLHintsLeft, 5, 5, 5, 5));

  gfResN = new TGGroupFrame(fMain, "North Termination Resistance");
  gfResS = new TGGroupFrame(fMain, "South Termination Resistance");
  gfGainN = new TGGroupFrame(fMain, "North Gain Factor");
  gfGainS = new TGGroupFrame(fMain, "South Gain Factor");

  fMain->AddFrame(gfResN, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));
  fMain->AddFrame(gfResS, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));
  fMain->AddFrame(gfGainN, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));
  fMain->AddFrame(gfGainS, new TGLayoutHints(kLHintsTop | kLHintsExpandX, 5, 5, 5, 5));


  // number entry boxes for gain and termination resistance 
  term_resN = new TGNumberEntry(gfResN);
  term_resN->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetValueResN()");
  (term_resN->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetValueResN()");
  gfResN->AddFrame(term_resN, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 

  resN = new TGLabel(gfResN, Form("Termination Res (North): %g", termination_resN));
  gfResN->AddFrame(resN, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 


  term_resS = new TGNumberEntry(gfResS);
  term_resS->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetValueResS()");
  (term_resS->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetValueResS()");
  gfResS->AddFrame(term_resS, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 

  resS = new TGLabel(gfResS, Form("Termination Res (South): %g", termination_resS));
  gfResS->AddFrame(resS, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 


  gain_ratioS = new TGNumberEntry(gfGainS);
  gain_ratioS->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetValueGainS()");
  (gain_ratioS->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetValueGainS()");
  gfGainS->AddFrame(gain_ratioS, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 

  gainS = new TGLabel(gfGainS, Form("Gain (South): %g", Gain_ratio));
  gfGainS->AddFrame(gainS, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 


  gain_ratioN = new TGNumberEntry(gfGainN);
  gain_ratioN->Connect("ValueSet(Long_t)", "MyMainFrame", this, "SetValueGainN()");
  (gain_ratioN->GetNumberEntry())->Connect("ReturnPressed()", "MyMainFrame", this, "SetValueGainN()");
  gfGainN->AddFrame(gain_ratioN, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 

  gainN = new TGLabel(gfGainN, Form("Gain (North): %g", Gain_ratio2));
  gfGainN->AddFrame(gainN, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5)); 



  // buttons - for exiting the window and for redrawing the histograms
  hist = new TGTextButton(bframe, "&Draw Histogram");
	bframe->AddFrame(hist, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5));
  hist->Connect("Clicked()", "MyMainFrame", this, "GraphModel()"); 

  quit = new TGTextButton(bframe, "&Exit");
	bframe->AddFrame(quit, new TGLayoutHints(kLHintsLeft | kLHintsExpandX, 5, 5, 5, 5));
  quit->Connect("Clicked()", "TApplication", gApplication, "Terminate()");


  // defaults of the gui
	fMain->SetWindowName("Wire Model Simulation");
	fMain->MapSubwindows();
	fMain->Resize(GetDefaultSize());
	fMain->MapWindow();
}

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
  h->GetXaxis()->SetTitle("idk");
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

// for cleaning up the gui
MyMainFrame::~MyMainFrame() { 
  fMain->Cleanup();
  delete fMain;
}

// for an exit button 
void MyMainFrame::CloseWindow(){
	DeleteWindow();
}


// setting the value of a variable 
/*
void MyMainFrame::SetValue(TGNumberEntry* num_entry, double var_name, double gain_var=1){

  var_name = num_entry->GetNumberEntry()->GetNumber() / gain_var;
}
*/

void MyMainFrame::SetValueResN(){

  termination_resN = term_resN->GetNumberEntry()->GetNumber();
  resN->SetText(Form("Termination Res (North): %g", termination_resN));

  gfResN->Layout();

}

void MyMainFrame::SetValueResS(){

  termination_resS = term_resS->GetNumberEntry()->GetNumber();
  resS->SetText(Form("Termination Res (South): %g", termination_resS));

  gfResS->Layout();

}

void MyMainFrame::SetValueGainN(){

  Gain_ratio2 = gain_ratioN->GetNumberEntry()->GetNumber();
  gainN->SetText(Form("Gain (North): %g", Gain_ratio2));

  gfGainN->Layout();

}


void MyMainFrame::SetValueGainS(){

  Gain_ratio = gain_ratioS->GetNumberEntry()->GetNumber();
  gainS->SetText(Form("Gain (South): %g", Gain_ratio));

  gfGainS->Layout();

}



// graphing the histogram(s)
void MyMainFrame::GraphModel()
{
  // constants that might change from wire-to-wire:

  /*
  double termination_resN=1.5*DCT_wire_resistance;
  double termination_resS=1.1*DCT_wire_resistance;
  double Gain_ratio=0.5;
  double Gain_ratio2=1.3;
  */
  

  int num_entries=100000;
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
  TH1D * h_div = new TH1D("Charge Division", "", 200,-0.05,1.05);  
  //create histogram for true position
  TH1D * h_true_pos = new TH1D("True Position", "", 200,-1.1*DCT_wire_length/2.0,1.1*DCT_wire_length/2.0);
  TH1D * h_calcqdv = new TH1D("Charge Division calculated", "", 200,-0.05,1.05);  
  TH1D * h_calcqdv2 = new TH1D("Charge Division calculated2", "", 200,-0.05,1.05);  

  //disable display of histogram statistics
  h_div->SetStats(false);
  h_calcqdv->SetStats(false);
  h_true_pos->SetStats(false);
  //fill with true hit positions
  for(double i = 0; i < num_entries; i++){
    double qdv_true; 
    if(i<0.01*num_entries) qdv_true = gRandom->Uniform(0,1);
    else qdv_true=gRandom->Gaus(0.5,0.1);
    //double qdv_true =gRandom->Gaus(0.5,0.1);
    h_div->Fill(qdv_true);
    double qdv_true_pos= convert_qdv_to_pos(qdv_true);
    h_true_pos->Fill(qdv_true_pos);
    //h_true_pos->Fill(gRandom->Uniform(-1.0*DCT_wire_length/2.0,DCT_wire_length/2.0));

    // if qdv is close to 1 then the hit position was close to north side and we should get a larger voltage reading for north side and less resistance from wire contributing!
    double res_north=(1.0-qdv_true)*DCT_wire_resistance;
    double res_south=qdv_true*DCT_wire_resistance;
    double total_resN=res_north+termination_resN;
    double total_resS=res_south+termination_resS;
    //double qdv_new=(total_resS)/(total_resN+total_resS);
    //double qdv_new=res_south/(res_south+(Gain_ratio*res_north));
    
    //double qdv_new=total_resS/(total_resS+(Gain_ratio2*total_resN));
    
    double qdv_new=(Gain_ratio2/Gain_ratio)*total_resN/(total_resS+(Gain_ratio2*total_resN/Gain_ratio));

    h_calcqdv->Fill(qdv_new);
    //qdv_new=res_south/(res_south+(Gain_ratio2*res_north));
    //h_calcqdv2->Fill(qdv_new);
    // now we can calculate the 
    //h_true_pos->Fill(gRandom->Uniform(-1.0*DCT_wire_length/2.0,DCT_wire_length/2.0));
  }
  // now apply a voltage to this


/*
  //define fit functions
  TF1 * FitFunc1 = new TF1("FitFunc1","[0]*TMath::Gaus(x,[1],[2])",0,40);
  TF1 * FitFunc2 = new TF1("FitFunc2","[0]*TMath::Landau(x,[1],[2])",0,40);
  TF1 * FitFuncCombined = new TF1("FitFunc2","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Landau(x,[4],[5])",0,40);
  //fit both peaks individually with reasonable initial parameters and fitting range
  FitFunc1->SetParameters(1,3,4);
  h->Fit(FitFunc1,"0","",0,10);
  FitFunc2->SetParameters(1,17,7);
  h->Fit(FitFunc2,"0","",10,40);
  //use fit parameters as initial parameters for combined fit
  FitFuncCombined->SetParameters(FitFunc1->GetParameter(0), FitFunc1->GetParameter(1), FitFunc1->GetParameter(2), FitFunc2->GetParameter(0), FitFunc2->GetParameter(1), FitFunc2->GetParameter(2));
  h->Fit(FitFuncCombined,"0","");
  //display what we did
*/
  TCanvas * c = new TCanvas("c_ref","c_title", 200,10,600,600);
  c->SetLeftMargin(0.15);
  c->SetRightMargin(0.04);
  c->SetTopMargin(0.04);
  //Legend
  TLegend* Leg = new TLegend(0.3,0.8,0.99,0.99);
  Leg->SetFillColor(0);
  Leg->SetTextFont(62);
  make_histo_pretty(h_div,0);
  make_histo_pretty(h_calcqdv,1);
  //make_histo_pretty(h_calcqdv2,2);

  h_calcqdv->Draw();
  //h_calcqdv2->Draw("SAME");
  h_div->Draw("SAME");
  c->SaveAs("GainRatio_resistance_model.png");

  TCanvas * c2 = new TCanvas("c_ref2","c_title2", 200,50,600,600);
  c2->SetLeftMargin(0.15);
  c2->SetRightMargin(0.04);
  c2->SetTopMargin(0.04);
  make_histo_pretty_qdvpos(h_true_pos);

  /*char text[400];
  sprintf(text,"N=%5.0f Mean=%5.1f RMS=%5.1f", h->GetEntries(), h->GetMean(), h->GetRMS());
  Leg->AddEntry(h,text,"l");
  FitFunc1->SetLineStyle(2);
  FitFunc1->SetLineColor(kRed);
  FitFunc1->Draw("same");
  sprintf(text,"Gaus: Mean=%5.1f#pm%5.1f, #sigma=%5.1f#pm%5.1f Landau: MOP=%5.1f#pm%5.1f, #sigma=%5.1f#pm%5.1f", FitFuncCombined->GetParameter(1), 
  FitFuncCombined->GetParError(1), FitFuncCombined->GetParameter(2), FitFuncCombined->GetParError(2), FitFuncCombined->GetParameter(4), FitFuncCombined->GetParError(4), FitFuncCombined->GetParameter(5), FitFuncCombined->GetParError(5));
  Leg->AddEntry(FitFuncCombined,text,"l");
  FitFunc2->SetLineStyle(2);
  FitFunc2->SetLineColor(kRed);
  FitFunc2->Draw("same");
  FitFuncCombined->SetLineColor(kRed);
  FitFuncCombined->Draw("same");
  Leg->Draw();
  //Save canvas
  c->SaveAs("ex1.eps");
  c->SaveAs("ex1.png");
  c->SaveAs("ex1.root");*/
}

void test_random() {
  // for changing the object lifetime management (avoiding memory leaks)
  TH1::AddDirectory(false);

  // new additions for the user input via a gui: 
  new MyMainFrame(gClient->GetRoot(), 500, 500);
}
