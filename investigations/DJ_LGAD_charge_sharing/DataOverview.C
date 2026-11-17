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

// void DataOverview(string filename, string output_folder) {
void DataOverview(string filename, string output_folder, int n_bins, int rebin, float rms_range_low, float rms_range_high, int trigger_strip, int mcp, float amplitude_fraction) {

    TFile * file = TFile::Open(filename.c_str());

    TTree * pulse = (TTree * ) file -> Get("pulse");

    TString conditions = "baseline_RMS[" + to_string(trigger_strip) + "]<3 && amp[" + to_string(trigger_strip) + "]<40"; // && LP2_10[" + to_string(trigger) + "]<-10 && LP2_80[" + to_string(trigger) + "]<-10 && LP2_10[" + to_string(trigger) + "]>-13 && LP2_80[" + to_string(trigger) + "]>-13";

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

    // ______________________________________________________________________________________
    // ______________________________ Amplitude proportion cut ______________________________
    // ______________________________________________________________________________________

    /*for (int t = 1; t < 6; t++) {

        std::vector < float > proportion_cut, efficiency;

        for (int i = 10; i <= 70; ++i) {
            std::string base_cut = "baseline_RMS[" + std::to_string(t) + "]<3" +
                "&&LP2_10[" + std::to_string(t) + "]<-1" +
                "&&LP2_80[" + std::to_string(t) + "]<-1" +
                "&&amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2]&&amp[3]>amp[4]&&amp[3]>amp[5]&&amp[3]>amp[6]";

            std::string cut =
                "baseline_RMS[" + std::to_string(t) + "]<3"
            "&&LP2_10[" + std::to_string(t) + "]<-1" +
            "&&LP2_80[" + std::to_string(t) + "]<-1" +
            "&&amp[" + std::to_string(t) + "]>(amp[" + std::to_string(t - 1) + "]+amp[" + std::to_string(t) + "]+amp[" + std::to_string(t + 1) + "])*" +
                std::to_string(i) + ".0/100.0" +
            "&&amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2]&&amp[3]>amp[4]&&amp[3]>amp[5]&&amp[3]>amp[6]";

            std::cout << cut << std::endl;
            std::cout << pulse -> GetEntries(cut.c_str()) << std::endl;

            proportion_cut.push_back(i);
            efficiency.push_back(pulse -> GetEntries(cut.c_str()) * 100.0 / pulse -> GetEntries(base_cut.c_str()));
        }

        TCanvas * canvas_proportion_cut_vs_efficiency = new TCanvas("Amplitude proportion cut", "Amplitude proportion cut", 600, 500);
        canvas_proportion_cut_vs_efficiency -> cd();
        TGraph * proportion_cut_vs_efficiency = new TGraph(proportion_cut.size(), proportion_cut.data(), efficiency.data());
        proportion_cut_vs_efficiency -> GetXaxis() -> SetTitle("Amplitude proportion cut [mV]");
        proportion_cut_vs_efficiency -> GetYaxis() -> SetTitle("Efficiency [%]");
        proportion_cut_vs_efficiency -> GetYaxis() -> SetRangeUser(0, 110);
        proportion_cut_vs_efficiency -> SetTitle(("Leading strip, channel: " + to_string(t)).c_str());
        proportion_cut_vs_efficiency -> Draw("APL*");
        canvas_proportion_cut_vs_efficiency -> SaveAs((output_folder + "/data_overview/" + "proportion_cut_vs_efficiency_lead_channel_" + to_string(t) + ".png").c_str());

    }*/

    gStyle -> SetOptStat(1110);
    gStyle -> SetOptFit(1011);

    // Amplitudes 
    
    int rows = 1;
    int cols = 2;
    
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 1200 * cols, 1000 * rows);
    amplitudes->Divide(cols, rows);
    TLegend * l = new TLegend(0.6, 0.7, 0.9, 0.9);
    
    int N_events = pulse->GetEntries(conditions);
    
    gStyle->SetOptStat(0000);
    
    amplitudes->cd(1);
    pulse -> Draw(("amp[" + to_string(trigger_strip-1) + "]>>amp_hist" + to_string(trigger_strip-1)).c_str(), conditions, "hist e");
    pulse -> Draw(("amp[" + to_string(trigger_strip)   + "]>>amp_hist" + to_string(trigger_strip  )).c_str(), conditions, "hist e same");
    pulse -> Draw(("amp[" + to_string(trigger_strip+1) + "]>>amp_hist" + to_string(trigger_strip+1)).c_str(), conditions, "hist e same");
    TH1D * hist2 = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(trigger_strip-1)).c_str());
    TH1D * hist1 = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(trigger_strip  )).c_str());
    TH1D * hist3 = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(trigger_strip+1)).c_str());
    hist1->Scale(1.0 / N_events);
    hist2->Scale(1.0 / N_events);
    hist3->Scale(1.0 / N_events);
    hist1->SetTitle("");
    hist2->SetTitle("");
    hist3->SetTitle("");
    hist1->SetMaximum(0.2);
    hist2->SetMaximum(0.2);
    hist3->SetMaximum(0.2);
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
    
    TLine * line = new TLine(10.5,0,10.5,0.2);
    line->SetLineWidth(3);
    line->SetLineColor(2);
    //line -> SetColor(3);
    line->Draw("same");
    
    l->AddEntry(hist2, "Channel 1");
    l->AddEntry(hist1, "Channel 2 (trigger strip, 10.5 mV)");
    l->AddEntry(hist3, "Channel 3");
    l->Draw("same");
    amplitudes->cd(2);
    pulse -> Draw(("noise[" + to_string(trigger_strip-1) + "]>>noise_hist" + to_string(trigger_strip-1)).c_str(), conditions, "hist e");
    pulse -> Draw(("noise[" + to_string(trigger_strip)   + "]>>noise_hist" + to_string(trigger_strip  )).c_str(), conditions, "hist e same");
    pulse -> Draw(("noise[" + to_string(trigger_strip+1) + "]>>noise_hist" + to_string(trigger_strip+1)).c_str(), conditions, "hist e same");
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

    // Amplitude dependency
    /*
    TCanvas * amplitudes_dependency = new TCanvas("Amplitude dependency", "Amplitudes dependency", 2100, 1100);
    amplitudes_dependency -> Divide(4, 2);
    for (int i = 0; i < 8; i++) {
        amplitudes_dependency -> cd(i + 1);
        pulse -> Draw(("amp[" + to_string(i) + "]:amp[" + to_string(trigger) + "]>>hist" + to_string(i)).c_str(), conditions, "colz"); // y:x
        TH1D * hist = (TH1D * ) gDirectory -> Get(("hist" + to_string(i)).c_str());
        if (i == mcp) {
            hist -> SetTitle("MCP");
        } else if (i == trigger) {
            hist -> SetTitle("Trigger strip");
        } else {
            hist -> SetTitle(("Channel " + to_string(i + 1)).c_str());
        }
        hist -> GetXaxis() -> SetTitle("Trigger channel Amplitude [mV]");
        hist -> GetYaxis() -> SetTitle(("Channel: " + to_string(i + 1) + " amplitude [mV]").c_str());
    }
    amplitudes_dependency -> SaveAs((output_folder + "/data_overview/" + "Amplitudes_correlation.png").c_str());
    */

    /*
    for (int i = 0; i < sizeof(fit_amps) / sizeof(fit_amps[0]); i++) {
        TCanvas * timestamps = new TCanvas(("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 2100, 1100);
        timestamps -> Divide(4, 2);
        for (int j = 0; j < 8; j++) {
            timestamps -> cd(j + 1);
            gPad -> SetLogy(1);
            pulse -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(j) + "]>>LP2_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str(), conditions);
            TH1D * hist = (TH1D * ) gDirectory -> Get(("LP2_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str());
            if (j == mcp) {
                hist -> SetTitle("MCP");
            } else if (j == trigger) {
                hist -> SetTitle("Trigger strip");
            } else {
                hist -> SetTitle(("Channel " + to_string(j + 1)).c_str());
            }
            hist -> GetXaxis() -> SetTitle("Timestamp [ns]");
            hist -> GetYaxis() -> SetTitle("Events");
        }
        // timestamps->SetLogy();
        timestamps -> SaveAs((output_folder + "/data_overview/" + "Timestamps_LP2_" + to_string(fit_amps[i]) + ".png").c_str());
    }
    */

}