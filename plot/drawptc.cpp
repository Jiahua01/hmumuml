#include <TFile.h>
#include <TCanvas.h>
#include <TList.h>
#include <TH1D.h>
#include <iostream>

void drawptc() {
    // 打开 ROOT 文件
    TFile *file = new TFile("bdtscore_SR.root", "READ");

    // 创建画布
    TCanvas *canvas = new TCanvas("c1", "Canvas", 800, 600);


    // 列出要绘制的直方图名称
    const char* histogramNames[] = {"vbf"};
    int nHistos = sizeof(histogramNames)/sizeof(histogramNames[0]);

    // 遍历直方图名称并绘制
    for (int i = 0; i < nHistos; ++i) {
        TH1D *hist = (TH1D*)file->Get(histogramNames[i]);
        canvas->Clear();
        hist->Draw("HIST");
        cout << hist->Integral() << endl;
        canvas->Update();
        canvas->Print(Form("%s_t.png",histogramNames[i]));
    }

    
    // 清理资源
    delete canvas;
    delete file;
}
