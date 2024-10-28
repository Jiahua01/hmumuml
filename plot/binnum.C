#include <iostream>
#include <set>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TStyle.h>
#include <TVector.h>
#include <TCanvas.h>
#include <fstream>
#include "TLorentzVector.h"
#include "TPoint.h"
#include "Math/Math.h"

Long64_t create(TString filename, TString dir, TString cat, Int_t k, double xsec, double lumi){
    TFile *oldfile = new TFile(filename);
    
  // TTree *oldtree = (TTree*)gDirectory->Get("passedEvents");
    TTree *oldtree1 = (TTree*)oldfile->Get("ntuple");
    TTree *oldtree2 = (TTree*)oldfile->Get("conditions");

    //calculate sum of genweight
    Long64_t nentries2 = oldtree2->GetEntries();
    cout << nentries2 <<endl;
    double sumweight,gensumweight;
    oldtree2->SetBranchAddress("genEventSumw",&gensumweight);
    oldtree2->GetEntry(0);
    for (Long64_t i=0;i<nentries2;i++){
            oldtree2->GetEntry(i);
            sumweight = sumweight + gensumweight;
        }

    //Create a new file + a clone of old tree in new file
    TFile *newfile = new TFile(
                               Form("%s/mlfile/%s%d_ml.root",dir.Data(), cat.Data(),k)
            ,"recreate");
    TTree *newtree1 = oldtree1->CloneTree(0);
    Long64_t nentries1 = oldtree1->GetEntries();
    float mu_mass = 0.1056583745;
    float genweight=0,weight=0;
    TLorentzVector HP4,mu1P4, mu2P4, jet1P4, jet2P4, dijetP4;
    float H_eta, H_pt, H_phi, H_mass;
    float mu1_eta, mu1_pt, mu1_phi;
    float mu2_eta, mu2_pt, mu2_phi;
    float jet1_eta, jet1_pt, jet1_phi, jet1_mass;
    float jet2_eta, jet2_pt, jet2_phi, jet2_mass;
    oldtree1->SetBranchAddress("genWeight",&genweight);

     oldtree1->SetBranchAddress("H_eta",&H_eta);
     oldtree1->SetBranchAddress("H_mass",&H_mass);
     oldtree1->SetBranchAddress("H_phi",&H_phi);
     oldtree1->SetBranchAddress("H_pt",&H_pt);

     oldtree1->SetBranchAddress("mu1_fromH_eta",&mu1_eta);
     oldtree1->SetBranchAddress("mu1_fromH_pt",&mu1_pt);
     oldtree1->SetBranchAddress("mu1_fromH_phi",&mu1_phi);

     oldtree1->SetBranchAddress("jet1_eta",&jet1_eta);
     oldtree1->SetBranchAddress("jet1_mass",&jet1_mass);
     oldtree1->SetBranchAddress("jet1_phi",&jet1_phi);
     oldtree1->SetBranchAddress("jet1_pt",&jet1_pt);

     oldtree1->SetBranchAddress("jet2_eta",&jet2_eta);
     oldtree1->SetBranchAddress("jet2_mass",&jet2_mass);
     oldtree1->SetBranchAddress("jet2_phi",&jet2_phi);
     oldtree1->SetBranchAddress("jet2_pt",&jet2_pt);

     newtree1->Branch("weight",&weight);//data
     float H_rap=0, dijet_pt=0, dijet_rap=0, mumu_jet1_deltaphi=0, mumu_jet2_deltaphi=0, mumu_jetjet_deltaphi=0;
     newtree1->Branch("H_rap",&H_rap);
     newtree1->Branch("dijet_pt",&dijet_pt);
     newtree1->Branch("dijet_rap",&dijet_rap);
     newtree1->Branch("mumu_jet1_deltaphi",&mumu_jet1_deltaphi);
     newtree1->Branch("mumu_jet2_deltaphi",&mumu_jet2_deltaphi);
     newtree1->Branch("mumu_jetjet_deltaphi",&mumu_jetjet_deltaphi);
    for (Long64_t i=0;i<nentries1; i++) {
     oldtree1->GetEntry(i);
     HP4.SetPtEtaPhiM(H_pt, H_eta, H_phi, H_mass);
     
     mu1P4.SetPtEtaPhiM(mu1_pt, mu1_eta, mu1_phi, mu_mass);

     mu2P4.SetPtEtaPhiM(mu2_pt, mu2_eta, mu2_phi, mu_mass);

     jet1P4.SetPtEtaPhiM(jet1_pt, jet1_eta, jet1_phi, jet1_mass);

     jet2P4.SetPtEtaPhiM(jet2_pt, jet2_eta, jet2_phi, jet2_mass);
     dijetP4 = jet1P4 + jet2P4;
     //   cout << i << " " << H_pt << " " << H_eta <<endl;
     //add new variables

        //if (i%10000==0) std::cout<<i<<"/"<<nentries1<<std::endl;
        if ( cat == "data18") weight = 1;
        else weight = lumi*xsec*genweight/sumweight;//mc:lumi*xsec*weight/sumweight  data:1
        H_rap = HP4.Rapidity();
        dijet_pt = dijetP4.Pt();
        dijet_rap = dijetP4.Rapidity();
        mumu_jet1_deltaphi = HP4.DeltaPhi(jet1P4);
        mumu_jet2_deltaphi = HP4.DeltaPhi(jet2P4);
        mumu_jetjet_deltaphi = HP4.DeltaPhi(dijetP4);
        newtree1->Fill();
     //   cout << i << " " << weight<< " " << H_rap << " " << dijet_pt << " " << mumu_jet1_deltaphi<< endl;
    }

    cout << nentries1 << " " << filename<<endl;

    cout << newtree1->GetEntries() << " " <<endl;

   // newtree1->Print();
    newtree1->AutoSave();
    //newtree2->Print();
   // newtree2->AutoSave();
    //delete oldfile;
    delete newfile;

    return nentries1;
}

void binnum() {
    TString dir = "/afs/cern.ch/user/j/jiahua/work/vbfhmm";
    TString cat = "triboson";
    Long64_t num=0;
    double xs_top[15] = {365340, 88290, 69090, 115300, 34970, 34910, 3740, 75.61, 214.9, 243.2, 8.213, 0.7314, 6.979, 0.3974};
    double xs_diboson[10] = {12178, 6321, 6321, 601, 3696, 1325, 3.29};
    double xs_triboson[10] = {208.6, 165.1, 55.65, 13.98};
    double xs_VBF = 0.8228;
    double xs_ggh = 10.57;
    double xs_dy = 6225400;
    double xs_dy105 = 47120;
    double xs_ewk50 = 1029;
    double xs_ewk105 = 78;
    double lumi = 60;   
    
    cout << TMath::ATanH(0) << endl;

}

