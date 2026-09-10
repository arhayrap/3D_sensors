#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include <string>

void GetAmplitudes() {
    TFile * file = TFile::Open("/home/aram/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    TTree * pulse = (TTree*) file -> Get("pulse");
    TString conditions = "";
    int trigger = 3, mcp = 7;

    // Amplitudes
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 2000, 1000);
    amplitudes->Divide(4,2);
    for (int i = 0; i < 8; i++) {
        amplitudes->cd(i+1);
        // TTree*new_pulse = (TTree*)pulse->Clone();
        pulse->Draw(("amp["+to_string(i)+"]>>hist" + to_string(i)).c_str());
        TH1D*hist = (TH1D*)gDirectory->Get(("hist" + to_string(i)).c_str());
        if (i == mcp) { hist->SetTitle("MCP"); }
        else if (i == trigger) { hist->SetTitle("Trigger strip"); }
        else { hist->SetTitle(("Channel " + to_string(i+1)).c_str()); }
    }
    
    // Timestamps
    
    // TCanvas * timestamps = new TCanvas("Timestamps (LP2)", "Timestamps (LP2)", 2000, 1000);
    // amplitudes->Divide(4,2);
    int fit_amps[] = {5, 10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80};
    for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++){
        TCanvas * timestamps = new TCanvas(("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 2000, 1000);
        amplitudes->Divide(4,2);
        for (int j = 0; j < 8; j++) {
            amplitudes->cd(i+1);
            pulse->Draw(("LP2_" + to_string(fit_amps[i]) + "["+to_string(j)+"]>>hist" + to_string(j)).c_str());
            TH1D*hist = (TH1D*)gDirectory->Get(("hist" + to_string(j)).c_str());
            if (j == mcp) { hist->SetTitle("MCP"); }
            else if (j == trigger) { hist->SetTitle("Trigger strip"); }
            else { hist->SetTitle(("Channel " + to_string(j+1)).c_str()); }
        }
        timestamps->SaveAs(("Timestamps_LP2_" + to_string(fit_amps[i]) + "_.png").c_str());
    }

}