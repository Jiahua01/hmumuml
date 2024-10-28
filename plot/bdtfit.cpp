#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <algorithm>
#include <THStack.h>
#include <TStyle.h>
#include <TColor.h>
#include "boot.h"

using namespace std;

void bdtfit(TString mode = "two_jet", int button = 0)
{

    cout << "\tNOW!!! We will draw the most beautiful PICTURE in the world" << endl;
    cout << "\tIt worth coutless $$$$" << endl;
    cout << "\tLET'S GO!!!!!!" << endl;
    cout << "-------------------------------------------------------------------------------" << endl;

    TString LG[10] = {"diboson", "top", "ewk", "dy","data","vbf"};
    double Y_axis[3] = {0, 0.2, 100};

    TString x_name = "bdt_score";
    TString str_dir = "/eos/user/j/jiahua/vbfhmm/MC_guo/outputs";
    TString stype[7][4] = {{"diboson", "0"}, {"top", "0"}, {"ewk105", "0"},{"dy105","0"},{"data","0"}, {"vbf", "0"}};//, {"mc_med", "0"}};//, "sig"};
    TString ltype[][4] = {{"data","0"}, {"vbf", "0"}};//, {"ggH", "0"}, {"VBF", "0"}, {"WminusH", "WplusH", "ZH", "0"}, {"ttH", "0"}};

    int X_axis[]={10,0,10}, bins;
    int stypes = 6, ltypes = 2;

    cout << "\n\tThere are " << ltypes << " kinds of Higgs generation mode!!!" << endl;
    Double_t num[stypes + ltypes];
    Double_t bin[] = {0.0, 0.08, 0.18, 0.28, 0.38, 0.43, 0.53, 0.62, 0.71, 0.8,1.0};
    int nbins = sizeof(bin)/sizeof(Double_t) - 1;

    cout << "\n\n\tFinish basic setting" << endl;

    Float_t bdt;
    Float_t w, hmass,temp;
    Double_t wp, data;
    Bool_t tag;
    double max=0.0, min=0.0;


    setStyle();
    gStyle->SetTitleXSize(0.07);
    gStyle->SetTitleYSize(0.06);
    TCanvas *can = new TCanvas("can", "can", 800, 600);
    can->SetFillColorAlpha(0, 0);
    can->SetFrameFillColor(0);
    gStyle->SetPadColor(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetOptStat(0);
    can->SetLeftMargin(0.15);
    can->SetTopMargin(0.05);
    can->SetRightMargin(0.05);
    can->SetBottomMargin(0.13);
    // can->SetLogy(1);
    TPad *pad1 = new TPad("pad1", "", 0, 0.23, 1, 1);
    TPad *pad2 = new TPad("pad2", "", 0, 0, 1, 0.35);
    cout << "\n\tFinish Set Style and Canvas" << endl;
    
    int cat = 9;
    THStack *hs = new THStack("hs", "test stacked histograms");
    TH1D *mc_h = new TH1D("mc", Form("%s; %s; Events", mode.Data(), x_name.Data()), X_axis[0], X_axis[1], X_axis[2]);
    TH1D *data_h =  new TH1D("data", Form("%s; %s; Events", mode.Data(), x_name.Data()), X_axis[0], X_axis[1], X_axis[2]);
    TH1D *h_diboson = new TH1D(Form("%s", LG[0].Data()), Form("%s", LG[0].Data()),6,bin[cat],bin[cat+1]); 
    TH1D *h_top = new TH1D(Form("%s", LG[1].Data()), Form("%s", LG[1].Data()),6,bin[cat],bin[cat+1]); 
    TH1D *h_ewk = new TH1D(Form("%s", LG[2].Data()), Form("%s", LG[2].Data()),6,bin[cat],bin[cat+1]); 
    TH1D *h_dy = new TH1D(Form("%s", LG[3].Data()), Form("%s", LG[3].Data()),6,bin[cat],bin[cat+1]); 
    TH1D *h_data = new TH1D(Form("%s", LG[4].Data()), Form("%s", LG[4].Data()),6,bin[cat],bin[cat+1]); 
    TH1D *h_vbf = new TH1D(Form("%s", LG[5].Data()), Form("%s", LG[5].Data()),6,bin[cat],bin[cat+1]); 



    can->cd(0);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();

    double sum = 0.;
    int j = 0;

    for (int typei = 0; typei < stypes; typei++)
    {


        int len = 0;
        num[typei] = 0.0;
        while (stype[typei][len] != "0")
        {
        cout << "\n\tLoading file" << Form("%s/%s/%s.root", str_dir.Data(), mode.Data(), stype[typei][len].Data()) << "!......" << endl;
        TFile *f = new TFile(Form("%s/%s/%s.root", str_dir.Data(), mode.Data(), stype[typei][len].Data()));

        TTree *t = (TTree *)f->Get("test");
        Long64_t nentries = t->GetEntries();
        t->SetBranchAddress(x_name.Data(), &temp);
        t->SetBranchAddress("weight", &w);
        t->SetBranchAddress("H_mass", &hmass);
      //  t->SetBranchAddress("trg_single_mu24", &tag);

        cout << "\n\tStart drawing\n\tentries are : " << nentries << " ! oh my god ! too much !"
             << "\n\tloading...loading...zzzzzz\n"
             << endl;



        for (Long64_t i = 0; i < nentries; i++)
        {
            t->GetEntry(i);
             data = static_cast<double_t>(temp);
            if (hmass>115 && hmass<135) {
            for (j = 0; j <= 9; j++)
             {
                if ((data >= bin[j]) && (data <= bin[j+1]))
                  break;
             }
           // data = TMath::ATanH(data);
            if (j == cat ){//fit
                if (typei == 0) h_diboson->Fill(data,w);
                if (typei == 1) h_top->Fill(data,w);
                if (typei == 2) h_ewk->Fill(data,w);
                if (typei == 3) h_dy->Fill(data,w);
                if (typei == 4) h_data->Fill(data);
                if (typei == 5) h_vbf->Fill(data,w);
            }
          //  mc_h->Fill(j,w);
            num[typei] += w;
            //if (hmass<115 | hmass>135) sum += w;
            } 
        }
        // f->Close();

        len++;
        }
        cout << "\tFinished loading File" << endl;
        cout << "\n\t" << num[typei] << " items are in it!!"<<endl;
    }

   cout << (bin[cat+1]-bin[cat])/0.05  << endl;

    TString hbkroot = "cat9_SR.root";
    TFile* myhbk = TFile::Open(hbkroot,"recreate");
    h_diboson->Write();
    h_top->Write();
    h_ewk->Write();
    h_dy->Write();
    h_data->Write();
    h_vbf->Write();
   
   cout << h_diboson->Integral() << " " 
        << h_top->Integral() << " " 
        << h_ewk->Integral() << " " 
        << h_dy->Integral() << " " 
        << h_vbf->Integral() << endl;
   cout << h_data->Integral()<<endl;



}
