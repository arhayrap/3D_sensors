#include <iostream>

#include "TFile.h"

#include "TTree.h"

#include "TH1D.h"

#include "TH2D.h"

#include "TF1.h"

#include "TGraphErrors.h"

#include "TGraph.h"

#include "TCanvas.h"

#include "TGraph.h"

#include "TStyle.h"

#include "TLine.h"

#include <string>

TF1 * TimewalkCalculation(TH2D * histo2D) {
    int nPoints = histo2D -> GetXaxis() -> GetNbins();
    std::vector < double > means;
    std::vector < double > amps;
    for (int i = 0; i < nPoints; i++) {
        TH1D * projX = (TH1D * ) histo2D -> ProjectionY("projY", i + 1, i + 2);
        if (projX -> GetMean() == 0.0) continue;
        means.push_back(projX -> GetMean());
        amps.push_back(histo2D -> GetXaxis() -> GetBinCenter(i + 1));
    }
    TCanvas * timewalk_canvas = new TCanvas("timewalk", "timewalk", 1000, 500);
    timewalk_canvas -> Divide(2, 1);
    timewalk_canvas -> cd(1);
    histo2D -> Draw("colz");
    timewalk_canvas -> cd(2);
    auto timewalk_curve = new TGraph(means.size(), amps.data(), means.data());
    timewalk_curve -> Draw("ACP*");
    timewalk_curve -> SetTitle("Means and timewalk fit.");
    TF1 * polyline = new TF1("polyline", "[0]+[1]*x+[2]*x^2+[3]*x^3"); // +[4]*x^4+[5]*x^5");
    timewalk_curve -> Fit(polyline);
    polyline -> SetLineColor(2);
    polyline -> SetLineWidth(2);
    polyline -> Draw("samel");
    return polyline;
}

void AmplitudeNoise(string filename, string output_folder, string conditions, int n_bins, int rebin, float rms_range_low, float rms_range_high, int trigger_strip, int mcp, float amplitude_fraction) {

    TFile * file = TFile::Open(filename.c_str());

    TTree * pulse = (TTree * ) file -> Get("pulse");

    gStyle -> SetOptStat(1110);
    gStyle -> SetOptFit(1011);

    // Amplitudes 
    
    int rows = 1;
    int cols = 2;
    
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 1200 * cols, 1000 * rows);
    amplitudes->Divide(cols, rows);
    TLegend * l = new TLegend(0.6, 0.7, 0.9, 0.9);
    
    int N_events = pulse->GetEntries(conditions.c_str());
    std::cout<<"Number of events: "<<N_events<<std::endl;
    
    gStyle->SetOptStat(0000);
    
    amplitudes->cd(1);
    pulse -> Draw(("amp[" + to_string(trigger_strip-1) + "]>>amp_hist" + to_string(trigger_strip-1) + "(" + to_string(n_bins) + ", 0 , 45)").c_str(), conditions.c_str(), "hist e");
    pulse -> Draw(("amp[" + to_string(trigger_strip)   + "]>>amp_hist" + to_string(trigger_strip  ) + "(" + to_string(n_bins) + ", 0 , 45)").c_str(), conditions.c_str(), "hist e same");
    pulse -> Draw(("amp[" + to_string(trigger_strip+1) + "]>>amp_hist" + to_string(trigger_strip+1) + "(" + to_string(n_bins) + ", 0 , 45)").c_str(), conditions.c_str(), "hist e same");
    TH1D * hist2 = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(trigger_strip-1)).c_str());
    TH1D * hist1 = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(trigger_strip  )).c_str());
    TH1D * hist3 = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(trigger_strip+1)).c_str());
    hist1->Scale(1.0 / N_events);
    hist2->Scale(1.0 / N_events);
    hist3->Scale(1.0 / N_events);
    hist1->SetTitle("");
    hist2->SetTitle("");
    hist3->SetTitle("");
    
    // TF1 * gaus1 = new TF1("gaus1", "gaus", 0, 10);
    // gaus1->SetLineColor(4);
    // hist3->Fit(gaus1);

    // TF1 * gaus2 = new TF1("gaus2", "gaus", 0, 10);
    // gaus2->SetLineColor(2);
    // hist2->Fit(gaus2);


    // gaus1->Draw("same");
    // gaus2->Draw("same");
    
    // hist1->SetMaximum(1.);
    // hist2->SetMaximum(1.);
    // hist3->SetMaximum(1.);
    
    hist1->SetMaximum(0.5);
    hist2->SetMaximum(0.5);
    hist3->SetMaximum(0.5);
    hist1->SetMinimum(0.0);
    hist2->SetMinimum(0.0);
    hist3->SetMinimum(0.0);
    hist1 -> GetXaxis() -> SetTitle("Amplitude [mV]");
    hist1 -> GetYaxis() -> SetTitle("Events");
    hist1 -> SetLineWidth(3);
    hist1 -> SetLineColor(1);
    hist2 -> GetXaxis() -> SetTitle("Amplitude [mV]");
    hist2 -> GetYaxis() -> SetTitle("Events");
    hist2 -> SetLineWidth(3);
    hist2 -> SetLineColor(2);
    hist3 -> GetXaxis() -> SetTitle("Amplitude [mV]");
    hist3 -> GetYaxis() -> SetTitle("Events");
    hist3 -> SetLineWidth(3);
    hist3 -> SetLineColor(4);
    
    TLine * line = new TLine(10.5,0,10.5,1.0);
    line->SetLineWidth(3);
    line->SetLineColor(2);
    line->Draw("same");
    
    l->AddEntry(hist2, "Channel 1");
    l->AddEntry(hist1, "Channel 2 (trigger strip, 10.5 mV)");
    l->AddEntry(hist3, "Channel 3");
    l->Draw("same");
    amplitudes->cd(2);
    pulse -> Draw(("noise[" + to_string(trigger_strip-1) + "]>>noise_hist" + to_string(trigger_strip-1)).c_str(), conditions.c_str(), "hist e");
    pulse -> Draw(("noise[" + to_string(trigger_strip)   + "]>>noise_hist" + to_string(trigger_strip  )).c_str(), conditions.c_str(), "hist e same");
    pulse -> Draw(("noise[" + to_string(trigger_strip+1) + "]>>noise_hist" + to_string(trigger_strip+1)).c_str(), conditions.c_str(), "hist e same");
    hist2 = (TH1D * ) gDirectory -> Get(("noise_hist" + to_string(trigger_strip-1)).c_str());
    hist1 = (TH1D * ) gDirectory -> Get(("noise_hist" + to_string(trigger_strip  )).c_str());
    hist3 = (TH1D * ) gDirectory -> Get(("noise_hist" + to_string(trigger_strip+1)).c_str());
    hist1->Scale(1.0 / N_events);
    hist2->Scale(1.0 / N_events);
    hist3->Scale(1.0 / N_events);
    hist1->SetTitle("");
    hist2->SetTitle("");
    hist3->SetTitle("");
    hist1->SetMaximum(0.1);
    hist2->SetMaximum(0.1);
    hist3->SetMaximum(0.1);
    hist1->SetMinimum(0.0);
    hist2->SetMinimum(0.0);
    hist3->SetMinimum(0.0);
    hist1 -> GetXaxis() -> SetTitle("Noise [mV]");
    hist1 -> GetYaxis() -> SetTitle("Events");
    hist1 -> SetLineWidth(3);
    hist1 -> SetLineColor(1);
    hist2 -> GetXaxis() -> SetTitle("Noise [mV]");
    hist2 -> GetYaxis() -> SetTitle("Events");
    hist2 -> SetLineWidth(3);
    hist2 -> SetLineColor(2);
    hist3 -> GetXaxis() -> SetTitle("Noise [mV]");
    hist3 -> GetYaxis() -> SetTitle("Events");
    hist3 -> SetLineWidth(3);
    hist3 -> SetLineColor(4);
    l->Draw("same");    

    amplitudes -> SaveAs((output_folder + "/Amplitudes.png").c_str());

}