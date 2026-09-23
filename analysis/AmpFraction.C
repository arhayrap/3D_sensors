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

#include <string>

#include <sys/stat.h>

#include <sys/types.h>

#include <stdio.h>

#include <errno.h>

void AmpFraction(string filename, string output_folder, int trigger, int mcp) {
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv125_mcp4400_trigCh4_Ch8_Ru106_2days.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv120_mcp4400_trigCh4_Ch8_Ru106_3days.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv113_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root");

    TFile * file = TFile::Open(filename.c_str());
    TTree * pulse = (TTree * ) file -> Get("pulse");

    // |(((((((((((((((((((((((((((((( Selecting the leading strip )))))))))))))))))))))))))))))))|
    // conditions += "&& amp[0]>amp[1] && amp[0]>amp[2] && amp[0]>amp[3] && amp[0]>amp[4] && amp[0]>amp[5] && amp[0]>amp[6]"; // Channel 1
    // conditions += "&& amp[1]>amp[0] && amp[1]>amp[2] && amp[1]>amp[3] && amp[1]>amp[4] && amp[1]>amp[5] && amp[1]>amp[6]"; // Channel 2
    // conditions += "&& amp[2]>amp[0] && amp[2]>amp[1] && amp[2]>amp[3] && amp[2]>amp[4] && amp[2]>amp[5] && amp[2]>amp[6]"; // Channel 3
    // conditions += "&& amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6]"; // Channel 4: Trigger strip
    // conditions += "&& amp[4]>amp[0] && amp[4]>amp[1] && amp[4]>amp[2] && amp[4]>amp[3] && amp[4]>amp[5] && amp[4]>amp[6]"; // Channel 4
    // conditions += "&& amp[5]>amp[0] && amp[5]>amp[1] && amp[5]>amp[2] && amp[5]>amp[3] && amp[5]>amp[4] && amp[5]>amp[6]"; // Channel 5
    // conditions += "&& amp[6]>amp[0] && amp[6]>amp[1] && amp[6]>amp[2] && amp[6]>amp[3] && amp[6]>amp[4] && amp[6]>amp[5]"; // Channel 6

    // int trigger = 3, mcp = 7;
    // string output_folder = "./AC100W40_110V_0p3mm_Ru106";
    // string output_folder = "./AC100W40_120V_0p3mm_Ru106";
    // string output_folder = "./AC100W40_113V_0p3mm_Ru106";
    string amplitude_fraction_folder = "/amplitude_percentage_scan_" + to_string(trigger) + "_mcp";

    if (mkdir(output_folder.c_str(), 0777) == -1)
        std::cerr << "Error :  " << strerror(errno) << std::endl;
    else
        std::cout << "Directory created" << std::endl;

    if (mkdir((output_folder + amplitude_fraction_folder).c_str(), 0777) == -1)
        std::cerr << "Error :  " << strerror(errno) << std::endl;
    else
        std::cout << "Directory created" << std::endl;

    for (int i = 10; i < 70; i++) {
        std::string conditions =
            "baseline_RMS[" + to_string(trigger) + "]<3" +
            "&&LP2_10[" + to_string(trigger) + "]<-1" +
            "&&LP2_80[" + to_string(trigger) + "]<-1" +
            "&&amp[3]>amp[2]&&amp[3]>amp[4]" + // &&amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2]&&amp[3]>amp[4]&&amp[3]>amp[5]&&amp[3]>amp[6]" +
            "&&amp[" + to_string(trigger) + "]>(amp[" + to_string(trigger - 1) + "]+amp[" + to_string(trigger) + "]+amp[" + to_string(trigger + 1) + "])*" + std::to_string(i) + "/100.0";

        std::cout << conditions << std::endl;
        std::cout << pulse -> GetEntries(conditions.c_str()) << std::endl;

        gStyle -> SetOptStat(1110);
        gStyle -> SetOptFit(1011);

        // Amplitudes 
        TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 2100, 1100);
        amplitudes -> Divide(4, 2);
        for (int j = 0; j < 8; j++) {
            amplitudes -> cd(j + 1);
            pulse -> Draw(("amp[" + to_string(j) + "]>>amp_hist_" + to_string(i) + "_" + to_string(j)).c_str(), conditions.c_str(), "hist e");
            TH1D * hist = (TH1D * ) gDirectory -> Get(("amp_hist_" + to_string(i) + "_" + to_string(j)).c_str());
            if (j == mcp) {
                hist -> SetTitle("MCP");
            } else if (j == trigger) {
                hist -> SetTitle("Trigger strip");
            } else {
                hist -> SetTitle(("Channel " + to_string(j + 1)).c_str());
            }
            hist -> GetXaxis() -> SetTitle("Amplitude [mV]");
            hist -> GetYaxis() -> SetTitle("Events");
        }
        amplitudes -> SaveAs((output_folder + amplitude_fraction_folder + "/Amplitudes" + to_string(i) + ".png").c_str());

        // Amplitude dependency
        TCanvas * amplitudes_dependency = new TCanvas("Amplitude dependency", "Amplitudes dependency", 2100, 1100);
        amplitudes_dependency -> Divide(4, 2);
        for (int j = 0; j < 8; j++) {
            amplitudes_dependency -> cd(j + 1);
            pulse -> Draw(("amp[" + to_string(j) + "]:amp[" + to_string(trigger) + "]>>amp_hist_" + to_string(i) + "_" + to_string(j) + "(80,0,100,80,0,100)").c_str(), conditions.c_str(), "colz"); // y:x
            int x[] = {
                0,
                25,
                50,
                75,
                100
            };
            int y[] = {
                0,
                25,
                50,
                75,
                100
            };
            TGraph * threshold = new TGraph(5, x, y);
            threshold -> SetLineWidth(2);
            threshold -> SetLineColor(2);
            TH2D * hist = (TH2D * ) gDirectory -> Get(("amp_hist_" + to_string(i) + "_" + to_string(j)).c_str());
            if (j == mcp) {
                hist -> SetTitle("MCP");
            } else if (j == trigger) {
                hist -> SetTitle("Trigger strip");
            } else {
                hist -> SetTitle(("Channel " + to_string(j + 1)).c_str());
            }
            threshold -> Draw("SAME L");
            hist -> GetXaxis() -> SetTitle("Trigger channel Amplitude [mV]");
            hist -> GetYaxis() -> SetTitle(("Channel: " + to_string(j + 1) + " amplitude [mV]").c_str());
        }
        amplitudes_dependency -> SaveAs((output_folder + amplitude_fraction_folder + "/Amplitudes_comparison_" + to_string(i) + ".png").c_str());

        // Timestamps
        int fit_amps[] = {
            10,
            15,
            20,
            25,
            30,
            35,
            40,
            50,
            60,
            70,
            80
        };
        std::vector < double > fit_amps_V = {
            10.0,
            15.0,
            20.0,
            25.0,
            30.0,
            35.0,
            40.0,
            50.0,
            60.0,
            70.0,
            80.0
        };

        for (int j = 0; j < sizeof(fit_amps) / sizeof(fit_amps[0]); j++) {
            TCanvas * timestamps = new TCanvas(("Timestamps (LP2_" + to_string(fit_amps[j]) + ")").c_str(), ("Timestamps (LP2_" + to_string(fit_amps[j]) + ")").c_str(), 2100, 1100);
            timestamps -> Divide(4, 2);
            for (int k = 0; k < 8; k++) {
                timestamps -> cd(k + 1);
                gPad -> SetLogy(1);
                // std::cout<<j<<" ----------------- "<<sizeof(fit_amps)/sizeof(fit_amps[0])<<" ----------------- "<<to_string(fit_amps[j])<<" ---------------- "<<"LP2_" + to_string(fit_amps[j]) + "["+to_string(k)+"]>>LP2_" + to_string(fit_amps[j]) + "_hist" + to_string(k)<<std::endl;
                pulse -> Draw(("LP2_" + to_string(fit_amps[j]) + "[" + to_string(k) + "]>>LP2_" + to_string(i) + "_" + to_string(fit_amps[j]) + "_" + to_string(k) + "_hist").c_str(), conditions.c_str(), "");
                TH1D * hist = (TH1D * ) gDirectory -> Get(("LP2_" + to_string(i) + "_" + to_string(fit_amps[j]) + "_" + to_string(k) + "_hist").c_str());
                if (k == mcp) {
                    hist -> SetTitle("MCP");
                } else if (k == trigger) {
                    hist -> SetTitle("Trigger strip");
                } else {
                    hist -> SetTitle(("Channel " + to_string(k + 1)).c_str());
                }
                hist -> GetXaxis() -> SetTitle("Timestamp [ns]");
                hist -> GetYaxis() -> SetTitle("Events");
            }
            // timestamps->SetLogy();
            timestamps -> SaveAs((output_folder + amplitude_fraction_folder + "/Timestamps_LP2_" + to_string(fit_amps[j]) + "_" + to_string(i) + ".png").c_str());
        }

        /*for (int j = 0; j < sizeof(fit_amps)/sizeof(fit_amps[0]); j++) {
            TCanvas * timestamps = new TCanvas(("Timestamps (linear_RE_" + to_string(fit_amps[j]) + ")").c_str(), ("Timestamps (linear_RE_" + to_string(fit_amps[j]) + ")").c_str(), 2100, 1100);
            timestamps->Divide(4,2);
            for (int k = 0; k < 8; k++) {
                timestamps->cd(k+1);
                gPad->SetLogy(1);
                pulse->Draw(("linear_RE_" + to_string(fit_amps[j]) + "["+to_string(k)+"]>>linear_RE_" + to_string(i) + "_"  + to_string(fit_amps[j]) + "_" + to_string(k) + "_hist").c_str(), conditions.c_str());
                TH1D*hist = (TH1D*)gDirectory->Get(("linear_RE_" + to_string(i) + "_"  + to_string(fit_amps[j]) + "_" + to_string(k) + "_hist").c_str());
                if (k == mcp) { hist->SetTitle("MCP"); }
                else if (k == trigger) { hist->SetTitle("Trigger strip"); }
                else { hist->SetTitle(("Channel " + to_string(k+1)).c_str()); }
                hist->GetXaxis()->SetTitle("Timestamp [ns]");
                hist->GetYaxis()->SetTitle("Events");
            }
            timestamps->SaveAs(("./"+ output_folder + amplitude_fraction_folder +"/Timestamps_RE_" + to_string(fit_amps[j]) + "_" + to_string(i) + ".png").c_str());
        }*/

        std::cout << "NEXT FRACTION" << std::endl;

    }

}