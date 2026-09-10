#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TStyle.h"
#include <string>

void GetAmplitudes() {
    // TFile * file = TFile::Open("/home/aram/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    TFile * file = TFile::Open("/Users/PITLab/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    TTree * pulse = (TTree*) file -> Get("pulse");
    TString conditions = "baseline_RMS[3]<3 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6] && amp[3]>10 && amp[7]<131 && amp[7]>20";
    int trigger = 3, mcp = 7;
    gStyle->SetOptStat(1110);

    // Amplitudes
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 2000, 1000);
    amplitudes->Divide(4,2);
    for (int i = 0; i < 8; i++) {
        amplitudes->cd(i+1);
        pulse->Draw(("amp["+to_string(i)+"]>>hist" + to_string(i)).c_str(), conditions);
        TH1D*hist = (TH1D*)gDirectory->Get(("hist" + to_string(i)).c_str());
        if (i == mcp) { hist->SetTitle("MCP"); }
        else if (i == trigger) { hist->SetTitle("Trigger strip"); }
        else { hist->SetTitle(("Channel " + to_string(i+1)).c_str()); }
        amplitudes->SaveAs("Amplitudes.png");
    }
    
    // Timestamps
    int fit_amps[] = {5, 10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80};
    for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++){
        TCanvas * timestamps = new TCanvas(("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 2000, 1000);
        timestamps->Divide(4,2);
        for (int j = 0; j < 8; j++) {
            timestamps->cd(j+1);
            pulse->Draw(("LP2_" + to_string(fit_amps[i]) + "["+to_string(j)+"]>>LP2_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str(), conditions);
            TH1D*hist = (TH1D*)gDirectory->Get(("LP2_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str());
            if (j == mcp) { hist->SetTitle("MCP"); }
            else if (j == trigger) { hist->SetTitle("Trigger strip"); }
            else { hist->SetTitle(("Channel " + to_string(j+1)).c_str()); }
        }
        timestamps->SaveAs(("Timestamps_LP2_" + to_string(fit_amps[i]) + ".png").c_str());
    }

    for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++){
        TCanvas * timestamps = new TCanvas(("Timestamps (linear_RE_" + to_string(fit_amps[i]) + ")").c_str(), ("Timestamps (linear_RE_" + to_string(fit_amps[i]) + ")").c_str(), 2000, 1000);
        timestamps->Divide(4,2);
        for (int j = 0; j < 8; j++) {
            timestamps->cd(j+1);
            pulse->Draw(("linear_RE_" + to_string(fit_amps[i]) + "["+to_string(j)+"]>>linear_RE_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str(), conditions);
            TH1D*hist = (TH1D*)gDirectory->Get(("linear_RE_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str());
            if (j == mcp) { hist->SetTitle("MCP"); }
            else if (j == trigger) { hist->SetTitle("Trigger strip"); }
            else { hist->SetTitle(("Channel " + to_string(j+1)).c_str()); }
        }
        timestamps->SaveAs(("Timestamps_RE_" + to_string(fit_amps[i]) + ".png").c_str());
    }

    // Delta T
    for (int i = 7; i < 8; i++){
    // for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++){
        TCanvas * deltaT = new TCanvas(("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 1000, 1000);
        // timestamps->Divide(4,2);
        // for (int j = 0; j < 8; j++) {
        // timestamps->cd(j+1);
        pulse->Draw(("LP2_" + to_string(fit_amps[i]) + "[3]-LP2_"+ to_string(fit_amps[i]) + "[7]>>DT_" + to_string(fit_amps[i]) + "_hist").c_str(), conditions);
        TH1D*hist = (TH1D*)gDirectory->Get(("DT_" + to_string(fit_amps[i]) + "_hist").c_str());
        // if (j == mcp) { hist->SetTitle("MCP"); }
        // else if (j == trigger) { hist->SetTitle("Trigger strip"); }
        // else { hist->SetTitle(("Channel " + to_string(j+1)).c_str()); }
        // }
        hist->SetTitle("DeltaT");
        deltaT->SaveAs(("DeltaT_LP2_" + to_string(fit_amps[i]) + ".png").c_str());
    // }
    }


}
