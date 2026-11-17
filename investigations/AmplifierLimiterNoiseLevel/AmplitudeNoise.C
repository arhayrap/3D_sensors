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

void AmplitudeNoise() {

    string base_folder = "/home/daq/3D_sensors/Processed_data/";
    string output_folder = "./Output_plots/";

    TFile * file_80_amp_lim = TFile::Open((base_folder + "proc_C_80tune_MCP_test_attenuator_limiter--00000.root").c_str());
    TFile * file_80_amp     = TFile::Open((base_folder + "proc_C_80tune_MCP_test_attenuator--00000.root").c_str());
    TFile * file_80         = TFile::Open((base_folder + "proc_C_80tune_MCP_test_direct--00000.root").c_str());

    TFile * file_85_amp_lim = TFile::Open((base_folder + "proc_C_85tune_MCP_test_attenuator_limiter--00000.root").c_str());
    TFile * file_85_amp     = TFile::Open((base_folder + "proc_C_85tune_MCP_test_attenuator--00000.root").c_str());
    TFile * file_85         = TFile::Open((base_folder + "proc_C_85tune_MCP_test_direct--00000.root").c_str());

    TFile * file_90_amp_lim = TFile::Open((base_folder + "proc_C_90tune_MCP_test_attenuator_limiter--00000.root").c_str());
    TFile * file_90_amp     = TFile::Open((base_folder + "proc_C_90tune_MCP_test_attenuator--00000.root").c_str());
    TFile * file_90         = TFile::Open((base_folder + "proc_C_90tune_MCP_test_direct--00000.root").c_str());

    // Draw the noise level and the amplitude curves or different configurations

    gStyle -> SetOptStat(1110);
    gStyle -> SetOptFit(1011);

    int rows = 2;
    int cols = 3;
    int trigger = 2;
    int n_bins = 100;
    // string conditions = "";
    
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 1200 * cols, 1000 * rows);
    amplitudes->Divide(cols, rows);
    TLegend * l = new TLegend(0.5, 0.7, 0.9, 0.9);
    string legend_labels[] = {
        "80",
        "85",
        "90"
    };
    
    TFile * files_direct           [] = {file_80, file_85        , file_90        };
    TFile * files_amplifier        [] = {file_80_amp, file_85_amp    , file_90_amp    };
    TFile * files_amplifier_limiter[] = {file_80_amp_lim, file_85_amp_lim, file_90_amp_lim};
    
    gStyle->SetOptStat(0000);
    
    amplitudes->cd(1);
    string conditions = "LP2_50[3]<0.44 && LP2_50[2]<45.04";
    for (int i = 0; i < sizeof(files_direct)/sizeof(files_direct[0]); i++) {
        TTree * pulse = (TTree *) files_direct[i]->Get("pulse");
        if (i != 0) pulse -> Draw(("amp[" + to_string(trigger) + "]>>amp_hist" + to_string(i) + "_direct" + "(" + to_string(n_bins) + ", 0 , 3500)").c_str(), conditions.c_str(), "same hist e");
        else        pulse -> Draw(("amp[" + to_string(trigger) + "]>>amp_hist" + to_string(i) + "_direct" + "(" + to_string(n_bins) + ", 0 , 3500)").c_str(), conditions.c_str(), "hist e");
        TH1D * hist = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(i) + "_direct").c_str());
        hist->Scale(1.0 / pulse->GetEntries());
        std::cout<<hist->GetMean()<<std::endl;

        hist->SetMinimum(0);
        hist->SetMaximum(0.3);
        hist->SetLineWidth(3);
        hist->SetLineColor(i+7);
        hist->SetTitle("Direct connection");
        l->AddEntry(hist, (legend_labels[i]+"% laser tune").c_str());
    }
    l->Draw("same");
    std::cout<<std::endl;
    

    amplitudes->cd(2);
    conditions = "LP2_50[3]>0.36";
    for (int i = 0; i < sizeof(files_amplifier)/sizeof(files_amplifier[0]); i++) {
        TTree * pulse = (TTree *) files_amplifier[i]->Get("pulse");
        if (i != 0) pulse -> Draw(("amp[" + to_string(trigger) + "]>>amp_hist" + to_string(i) + "_amp" + "(" + to_string(n_bins) + ", 0 , 3500)").c_str(), conditions.c_str(), "same hist e");
        else        pulse -> Draw(("amp[" + to_string(trigger) + "]>>amp_hist" + to_string(i) + "_amp" + "(" + to_string(n_bins) + ", 0 , 3500)").c_str(), conditions.c_str(), "hist e");
        TH1D * hist = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(i) + "_amp" ).c_str());
        hist->Scale(1.0 / pulse->GetEntries());
        std::cout<<hist->GetMean()<<std::endl;
        hist->SetMinimum(0);
        hist->SetMaximum(0.3);
        hist->SetLineWidth(3);
        hist->SetLineColor(i+7);
        hist->SetTitle("Amplifier");
        // l->AddEntry(hist, (legend_labels[i]+"% laser tune").c_str());
    }
    l->Draw("same");
    std::cout<<std::endl;

    amplitudes->cd(3);
    conditions = "LP2_50[3]>0.36";
    for (int i = 0; i < sizeof(files_amplifier_limiter)/sizeof(files_amplifier_limiter[0]); i++) {
        TTree * pulse = (TTree *) files_amplifier_limiter[i]->Get("pulse");
        if (i != 0) pulse -> Draw(("amp[" + to_string(trigger) + "]>>amp_hist" + to_string(i) + "_amp_lim" + "(" + to_string(n_bins) + ", 0 , 3500)").c_str(), conditions.c_str(), "same hist e");
        else        pulse -> Draw(("amp[" + to_string(trigger) + "]>>amp_hist" + to_string(i) + "_amp_lim" + "(" + to_string(n_bins) + ", 0 , 3500)").c_str(), conditions.c_str(), "hist e");
        TH1D * hist = (TH1D * ) gDirectory -> Get(("amp_hist" + to_string(i) + "_amp_lim" ).c_str());
        hist->Scale(1.0 / pulse->GetEntries());
        std::cout<<hist->GetMean()<<std::endl;
        hist->SetMinimum(0);
        hist->SetMaximum(0.3);
        hist->SetLineWidth(3);
        hist->SetLineColor(i+7);
        hist->SetTitle("Amplifier and limiter");
        // l->AddEntry(hist, (legend_labels[i]+"% laser tune").c_str());
    }
    l->Draw("same");
    
    std::cout<<std::endl;

    amplitudes->cd(4);
    conditions = "LP2_50[3]<0.44 && LP2_50[2]<45.04";
    for (int i = 0; i < sizeof(files_direct)/sizeof(files_direct[0]); i++) {
        TTree * pulse = (TTree *) files_direct[i]->Get("pulse");
        if (i != 0) pulse -> Draw(("noise[" + to_string(trigger) + "]>>noise_hist" + to_string(i) + "_direct" + "(" + to_string(n_bins) + ", -20 , 20)").c_str(), conditions.c_str(), "same hist e");
        else        pulse -> Draw(("noise[" + to_string(trigger) + "]>>noise_hist" + to_string(i) + "_direct" + "(" + to_string(n_bins) + ", -20 , 20)").c_str(), conditions.c_str(), "hist e");
        TH1D * hist = (TH1D * ) gDirectory -> Get(("noise_hist" + to_string(i) + "_direct").c_str());
        hist->Scale(1.0 / pulse->GetEntries());
        hist->SetMinimum(0);
        hist->SetMaximum(0.15);
        hist->SetLineWidth(3);
        hist->SetLineColor(i+7);
        hist->SetTitle("Direct connection");
    }
    l->Draw("same");
    
    amplitudes->cd(5);
    conditions = "LP2_50[3]>0.36";
    for (int i = 0; i < sizeof(files_amplifier)/sizeof(files_amplifier[0]); i++) {
        TTree * pulse = (TTree *) files_amplifier[i]->Get("pulse");
        if (i != 0) pulse -> Draw(("noise[" + to_string(trigger) + "]>>noise_hist" + to_string(i) + "_amp" + "(" + to_string(n_bins) + ", -20 , 20)").c_str(), conditions.c_str(), "same hist e");
        else        pulse -> Draw(("noise[" + to_string(trigger) + "]>>noise_hist" + to_string(i) + "_amp" + "(" + to_string(n_bins) + ", -20 , 20)").c_str(), conditions.c_str(), "hist e");
        TH1D * hist = (TH1D * ) gDirectory -> Get(("noise_hist" + to_string(i) + "_amp" ).c_str());
        hist->Scale(1.0 / pulse->GetEntries());
        hist->SetMinimum(0);
        hist->SetMaximum(0.15);
        hist->SetLineWidth(3);
        hist->SetLineColor(i+7);
        hist->SetTitle("Amplifier");
    }
    l->Draw("same");

    amplitudes->cd(6);
    conditions = "LP2_50[3]>0.36";
    for (int i = 0; i < sizeof(files_amplifier_limiter)/sizeof(files_amplifier_limiter[0]); i++) {
        TTree * pulse = (TTree *) files_amplifier_limiter[i]->Get("pulse");
        if (i != 0) pulse -> Draw(("noise[" + to_string(trigger) + "]>>noise_hist" + to_string(i) + "_amp_lim" + "(" + to_string(n_bins) + ", -20 , 20)").c_str(), conditions.c_str(), "same hist e");
        else        pulse -> Draw(("noise[" + to_string(trigger) + "]>>noise_hist" + to_string(i) + "_amp_lim" + "(" + to_string(n_bins) + ", -20 , 20)").c_str(), conditions.c_str(), "hist e");
        TH1D * hist = (TH1D * ) gDirectory -> Get(("noise_hist" + to_string(i) + "_amp_lim" ).c_str());
        hist->Scale(1.0 / pulse->GetEntries());
        hist->SetMinimum(0);
        hist->SetMaximum(0.15);
        hist->SetLineWidth(3);
        hist->SetLineColor(i+7);
        hist->SetTitle("Amplifier and limiter");
    }
    l->Draw("same");
    
    /*
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
    */

    amplitudes -> SaveAs((output_folder + "/Amplitudes.png").c_str());

}