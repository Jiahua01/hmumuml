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

    // 设置 PDF 输出
    canvas->Print("your_output.pdf[");

    // 列出要绘制的直方图名称
    const char* histogramNames[] = {"Triboson", "Diboson", "Top", "EWK", "DY", "data", "VBF"};
    int nHistos = sizeof(histogramNames)/sizeof(histogramNames[0]);

    // 遍历直方图名称并绘制
    for (int i = 0; i < nHistos; ++i) {
        TH1D *hist = (TH1D*)file->Get(histogramNames[i]);
        if (!hist) {
            std::cerr << "Warning: Histogram '" << histogramNames[i] << "' not found." << std::endl;
            continue;
        }
        
        // 清除画布并绘制直方图
        canvas->Clear();
        hist->Draw();

        // 更新画布并保存当前页到 PDF
        canvas->Update();
        canvas->Print(histogramNames[i]+".pdf");
    }

    
    // 清理资源
    delete canvas;
    delete file;
}