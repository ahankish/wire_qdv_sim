#!/bin/bash

 g++ -o test_random_v2 test_random_v2.C $( root-config --libs --cflags ) #compiling the file? 

 ./test_random_v2 10 10 1 1 # running for these values 10 10 1 1 

 #using root? 
 root 
 std::unique_ptr<TFile> myFile( TFile::Open("10_10_1_1.root") );
 myFile->ls()
 std::unique_ptr<TH1> hist(myFile->Get<TH1>("c")); # variable with the comparison histogram 
 std::unique_ptr<TH1> hist2(myFile->Get<TH1>("c2")); # variable with the original distribution, no changes to parameters
 .q
