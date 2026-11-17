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

void WaveformComparison(string filename, string output_folder, string conditions, int n_bins, int rebin, float rms_range_low, float rms_range_high, int trigger_strip, int mcp, float amplitude_fraction) {

    TFile * file = TFile::Open(filename.c_str());

    TTree * pulse = (TTree * ) file -> Get("pulse");

    // TString conditions = "baseline_RMS[" + to_string(trigger_strip) + "]<3"; // && LP2_10[" + to_string(trigger) + "]<-10 && LP2_80[" + to_string(trigger) + "]<-10 && LP2_10[" + to_string(trigger) + "]>-13 && LP2_80[" + to_string(trigger) + "]>-13";
    // conditions += " && amp[" + to_string(trigger_strip) + "]<40";
    // conditions += " && amp[" + to_string(trigger_strip-1) + "]<40";
    // conditions += " && amp[" + to_string(trigger_strip+1) + "]<40";

    // TString conditions = "baseline_RMS["+to_string(trigger)+"]<3";
    
    // conditions+="&&amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2]&&amp[3]>amp[4]&&amp[3]>amp[5]&&amp[3]>amp[6]";
    
    // conditions += "&& amp[0]>1.7*amp[1] && amp[0]>1.7*amp[2] && amp[0]>1.7*amp[3] && amp[0]>1.7*amp[4] && amp[0]>1.7*amp[5] && amp[0]>1.7*amp[6]"; // Channel 1

    // float coeff = 1.7;
    // conditions += Form("&& amp[1]>%f*amp[0] && amp[1]>%f*amp[2] && amp[1]>%f*amp[3] && amp[1]>%f*amp[4] && amp[1]>%f*amp[5] && amp[1]>%f*amp[6]", coeff, coeff, coeff, coeff, coeff, coeff); // Channel 2
    // conditions += Form("&& amp[2]>%f*amp[0] && amp[2]>%f*amp[1] && amp[2]>%f*amp[3] && amp[2]>%f*amp[4] && amp[2]>%f*amp[5] && amp[2]>%f*amp[6]", coeff, coeff, coeff, coeff, coeff, coeff); // Channel 3
    // conditions += Form("&& amp[3]>%f*amp[0] && amp[3]>%f*amp[1] && amp[3]>%f*amp[2] && amp[3]>%f*amp[4] && amp[3]>%f*amp[5] && amp[3]>%f*amp[6]", coeff, coeff, coeff, coeff, coeff, coeff); // Channel 4: Trigger strip
    // conditions += Form("&& amp[4]>%f*amp[0] && amp[4]>%f*amp[1] && amp[4]>%f*amp[2] && amp[4]>%f*amp[3] && amp[4]>%f*amp[5] && amp[4]>%f*amp[6]", coeff, coeff, coeff, coeff, coeff, coeff); // Channel 4
    // conditions += Form("&& amp[5]>%f*amp[0] && amp[5]>%f*amp[1] && amp[5]>%f*amp[2] && amp[5]>%f*amp[3] && amp[5]>%f*amp[4] && amp[5]>%f*amp[6]", coeff, coeff, coeff, coeff, coeff, coeff); // Channel 5
    // conditions += Form("&& amp[6]>%f*amp[0] && amp[6]>%f*amp[1] && amp[6]>%f*amp[2] && amp[6]>%f*amp[3] && amp[6]>%f*amp[4] && amp[6]>%f*amp[5]", coeff, coeff, coeff, coeff, coeff, coeff); // Channel 6

    // conditions += " && amp[" + to_string(trigger) + "]>(amp[" + to_string(trigger - 1) + "]+amp[" + to_string(trigger) + "]+amp[" + to_string(trigger + 1) + "])*" + to_string(amplitude_fraction); // the lowest applied cut is at 56%

    gStyle -> SetOptStat(1110);
    gStyle -> SetOptFit(1011);
    gStyle -> SetPalette(55);
   
    // Amplitudes 
    
    int rows = 1;
    int cols = 3;
    
    TLegend * l = new TLegend(0.6, 0.7, 0.9, 0.9);
    
    int N_events = pulse->GetEntries(conditions.c_str());
    
    gStyle->SetOptStat(0000);
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 1200 * cols, 1000 * rows);
    amplitudes->Divide(cols, rows);
    

    amplitudes->cd(1);
    pulse -> Draw(("channel[" + to_string(trigger_strip-1) + "]:time[0]:amp[" + to_string(trigger_strip-1) + "]>>waveforms_" + to_string(trigger_strip-1)).c_str(), conditions.c_str(), "colz");
    auto * w1 = (TH2D * ) gDirectory -> Get(("waveforms_" + to_string(trigger_strip-1)).c_str());
    gPad -> SetRightMargin(0.2);
    w1 -> SetTitle("Channel 1");
    w1 -> GetZaxis() -> SetRangeUser(0, 40);
    w1 -> GetZaxis() -> SetTitle("Amplitude [mV]");
    w1 -> GetYaxis() -> SetTitle("Voltage [mV]");
    w1 -> GetXaxis() -> SetTitle("Time [ns]");

    amplitudes->cd(2);
    pulse -> Draw(("channel[" + to_string(trigger_strip) + "]:time[0]:amp[" + to_string(trigger_strip) + "]>>waveforms_" + to_string(trigger_strip)).c_str(), conditions.c_str(), "colz");
    auto * w2 = (TH2D * ) gDirectory -> Get(("waveforms_" + to_string(trigger_strip)).c_str());
    gPad -> SetRightMargin(0.2);
    w2 -> SetTitle("Channel 2 (Trigger)");
    w2 -> GetZaxis() -> SetRangeUser(0, 40);
    w2 -> GetZaxis() -> SetTitle("Amplitude [mV]");
    w2 -> GetYaxis() -> SetTitle("Voltage [mV]");
    w2 -> GetXaxis() -> SetTitle("Time [ns]");
    
    amplitudes->cd(3);
    pulse -> Draw(("channel[" + to_string(trigger_strip+1) + "]:time[0]:amp[" + to_string(trigger_strip+1) + "]>>waveforms_" + to_string(trigger_strip+1)).c_str(), conditions.c_str(), "colz");
    auto * w3 = (TH2D * ) gDirectory -> Get(("waveforms_" + to_string(trigger_strip+1)).c_str());
    gPad -> SetRightMargin(0.2);
    w3 -> SetTitle("Channel 3");
    w3 -> GetZaxis() -> SetRangeUser(0, 40);
    w3 -> GetZaxis() -> SetTitle("Amplitude [mV]");
    w3 -> GetYaxis() -> SetTitle("Voltage [mV]");
    w3 -> GetXaxis() -> SetTitle("Time [ns]");


    amplitudes -> SaveAs((output_folder + "/WaveformComparison.png").c_str());

}