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

#include <stdio.h>

#include <stdlib.h>

TF1 * TimewalkCalculation(TH2D * histo2D, TCanvas * canvas) {
    int nPoints = histo2D -> GetXaxis() -> GetNbins();
    std::vector < double > means;
    std::vector < double > amps;
    for (int i = 0; i < nPoints; i++) {
        TH1D * projX = (TH1D * ) histo2D -> ProjectionY("projY", i + 1, i + 2);
        if (projX -> GetMean() == 0.0) continue;
        means.push_back(projX -> GetMean());
        amps.push_back(histo2D -> GetXaxis() -> GetBinCenter(i + 1));
        // std::cout << "===================> " << means[i] << std::endl;
    }
    // TCanvas * timewalk_canvas = new TCanvas("timewalk", "timewalk", 1000, 500);
    // timewalk_canvas -> Divide(2, 1);
    // timewalk_canvas -> cd(1);
    // histo2D -> Draw("colz");
    // timewalk_canvas -> cd(2);
    // canvas -> Divide(2, 1);
    canvas -> cd(4);
    gPad->SetLeftMargin(0.20);
    // histo2D -> Draw("colz");
    // canvas -> cd(8);
    auto timewalk_curve = new TGraph(means.size(), amps.data(), means.data());
    timewalk_curve -> GetXaxis() -> SetTitle("Amplitude [mV]");
    timewalk_curve -> GetYaxis() -> SetTitle("DeltaT [ns]");
    timewalk_curve -> Draw("ACP*");
    timewalk_curve -> SetTitle("Means and timewalk fit.");
    TF1 * polyline = new TF1("polyline", "[0]+[1]*x+[2]*x^2+[3]*x^3"); // +[4]*x^4+[5]*x^5");
    timewalk_curve -> Fit(polyline);
    polyline -> SetLineColor(2);
    polyline -> SetLineWidth(2);
    polyline -> Draw("samel");
    return polyline;
}

void DataComparison() {

    string filenames[] = {
        // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root", // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root",
        // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_low_threshold_retaken.root", // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root",

        
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv103_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv105_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv108_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root", // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_low_threshold_retaken.root", // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv113_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv116_mcp4400_trigCh4_Ch8_Ru106_2days.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv120_mcp4400_trigCh4_Ch8_Ru106_3days.root",
        "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv125_mcp4400_trigCh4_Ch8_Ru106_2days.root",

        // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_low_threshold_retaken.root", // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root",
        // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_low_threshold_retaken.root", // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv110_mcp4400_trigCh4_Ch8_Ru106_4days.root",
        // "/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv113_mcp4400_trigCh4_Ch8_Ru106_low_threshold.root",
        // "/Users/PITLab/3D_sensors/first_files/prep_AC100W80_strip3mm_bv116_mcp4400_linear.root" // 4 dark blue 
    };

    string legend_labels[] = {
        
        // "Ru106 AC100W40 110V",
        // "Ru106 AC100W40 110V new",
        
        "Ru106 AC100W40 103V",
        "Ru106 AC100W40 105V",
        "Ru106 AC100W40 108V",
        "Ru106 AC100W40 110V",
        "Ru106 AC100W40 110V new",
        "Ru106 AC100W40 113V",
        "Ru106 AC100W40 116V",
        "Ru106 AC100W40 120V",
        "Ru106 AC100W40 125V",   
        
        /*
        "Ru106 AC100W40 103V",
        "Ru106 AC100W40 105V",
        "Ru106 AC100W40 108V",
        "Ru106 AC100W40 110V",
        "Ru106 AC100W40 113V",
        "Ru106 AC100W40 116V",
        "Ru106 AC100W40 120V",
        "Ru106 AC100W40 125V",
        */
        // "Ce137 AC100W80 116V"
    };
    
    
    std::vector<double> bias_voltage;
    std::vector<double> bias_voltage_err;
    
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    bias_voltage_err.push_back(0.0);
    // bias_voltage_err.push_back(0.0);
    // bias_voltage_err.push_back(0.0);
    
    
    // bias_voltage.push_back(110);
    // bias_voltage.push_back(110);

    
    bias_voltage.push_back(103);
    bias_voltage.push_back(105);
    bias_voltage.push_back(108);
    bias_voltage.push_back(110);
    bias_voltage.push_back(110);
    bias_voltage.push_back(113);
    bias_voltage.push_back(116);
    bias_voltage.push_back(120);
    bias_voltage.push_back(125);
    

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

    std::vector < TTree * > pulse;
    int n_bins = 25;
    int rebin = 1; // Used for the amplitude and timestamp histograms
    
    float rms_range_high = 1.0;
    float rms_range_low = 1.5;
    float rms_full_range_high = 5;
    float rms_full_range_low = 5;
    
    float corr_rms_range_high = 3.0;
    float corr_rms_range_low = 3.0;
    float corr_rms_full_range_high = 5;
    float corr_rms_full_range_low = 5;
    
    int trigger = 3, mcp = 7; // The channels of the triggers and the time reference (MCP). Starts from 0 to 7.
    std::vector < float > amplitude_fraction;
    /*amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.44);
    amplitude_fraction.push_back(0.48);
    amplitude_fraction.push_back(0.56);
    amplitude_fraction.push_back(0.48);
    amplitude_fraction.push_back(0.45);*/

    /*amplitude_fraction.push_back(0.0);
    amplitude_fraction.push_back(0.0);
    amplitude_fraction.push_back(0.0);
    amplitude_fraction.push_back(0.0);
    amplitude_fraction.push_back(0.0);
    amplitude_fraction.push_back(0.0);
    amplitude_fraction.push_back(0.0);*/

    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    amplitude_fraction.push_back(0.5);
    
    std::vector<double> timing_resolution_50;
    std::vector<double> timing_resolution_50_err;
    std::vector<double> corr_timing_resolution_50;
    std::vector<double> corr_timing_resolution_50_err;
    
    string output_folder = "./CombinedDatasets";
    string base_conditions = "baseline_RMS[" + to_string(trigger) + "]<3 && LP2_10[" + to_string(trigger) + "]>-12 && LP2_90[" + to_string(trigger) + "]>-12 && LP2_10[" + to_string(trigger) + "]<0 && LP2_90[" + to_string(trigger) + "]<0";
    base_conditions += "&&amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2]&&amp[3]>amp[4]&&amp[3]>amp[5]&&amp[3]>amp[6]";
    
    if (mkdir((output_folder).c_str(), 0777) == -1)
        std::cerr << "Error :  " << strerror(errno) << std::endl;
    else
        std::cout << "Directory created"<< std::endl;

    // LOADING DATA
    std::cout << "LOADING DATA" << std::endl;
    for (int i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
        TFile * file = TFile::Open(filenames[i].c_str());
        pulse.push_back((TTree * ) file -> Get("pulse"));
    }
    std::cout << pulse.size() << std::endl;

    // TString conditions = "";
    // TString conditions = "LP2_10[3]<-1 && LP2_20[3]<-1 && LP2_50[3]<-1 && LP2_80[3]<-1 && LP2_10[7]<-1 && LP2_20[7]<-1 && LP2_50[7]<-1 && LP2_80[7]<-1";
    // TString conditions = "baseline_RMS[3]<3 && amp[3]>10 && LP2_20[3]<0 && amp[7]>20 && amp[7]<80 && amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6]"; // baseline_RMS[3]<3 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6] && amp[3]>35 && amp[7]<80 && amp[7]>20"; 
    // base_conditions += "&&LP2_10[" + to_string(trigger) + "]<-11 && LP2_80[" + to_string(trigger) + "]<-11";
    // TString conditions = "baseline_RMS["+to_string(trigger)+"]<3 && LP2_10["+to_string(trigger)+"]<-10 && LP2_80["+to_string(trigger)+"]<-10 && LP2_10["+to_string(trigger)+"]>-12.5 && LP2_80["+to_string(trigger)+"]>-12.5"; //  && amp["+to_string(trigger)+"]>5*baseline_RMS["+to_string(trigger)+"]"; // LP2_10[3]>-13 && && LP2_80[3]>-13";

    gStyle -> SetOptStat(0000);
    gStyle -> SetOptFit(1011);

    // -----------------------------------------------------------------------------------
    // --------------------------------- Amplitudes --------------------------------------
    // -----------------------------------------------------------------------------------
    
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 2100, 1100);

    amplitudes -> Divide(4, 2);
    for (int i = 0; i < 8; i++) {
        amplitudes -> cd(i + 1);
        gPad->SetLeftMargin(0.20);
        TLegend * legend = new TLegend(0.4, 0.7, 0.9, 0.9);
        std::vector < float > mins, maxes;
        for (int j = 0; j < pulse.size(); j++) {
            maxes.push_back(pulse.at(j) -> GetMaximum(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j)).c_str()));
            mins.push_back(pulse.at(j) -> GetMinimum(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j)).c_str()));
        }

        for (int j = 0; j < pulse.size(); j++) {
            TString conditions = base_conditions; 
            
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            // |=====================================================| Custom selections |=====================================================|
            
            // if (legend_labels[j] == "Ru106 AC100W40 120V" || legend_labels[j] == "Ru106 AC100W40 116V" || legend_labels[j] == "Ru106 AC100W40 113V") {
            //     conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_80["+to_string(trigger)+"]<-10";
            // }
            conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_90["+to_string(trigger)+"]<-10";
            if (legend_labels[j] == "Ru106 AC100W40 105V") {
                conditions += "&&LP2_10[" + to_string(trigger)+"]<-8 && LP2_90["+to_string(trigger)+"]<-8";
            }
            if (legend_labels[j] == "Ru106 AC100W40 108V") {
                conditions += "&&amp[7]<400";
            }
            conditions += " && amp[" + to_string(trigger) + "]>(amp[" + to_string(trigger - 1) + "]+amp[" + to_string(trigger) + "]+amp[" + to_string(trigger + 1) + "])*" + to_string(amplitude_fraction.at(j));
            float min = * std::min_element(mins.begin(), mins.end());
            float max = * std::max_element(maxes.begin(), maxes.end());
            
            if (i != 7) {
                max = 120;
                min = 0;
            }
            
            if (j != 0) pulse.at(j) -> Draw(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions, "HIST E SAME");
            else pulse.at(j) -> Draw(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions, "HIST E");
            TH1D * hist = (TH1D * ) gDirectory -> Get(("hist_" + to_string(i) + "_" + to_string(j)).c_str());
            if (i == mcp) {
                hist -> SetTitle("MCP");
            } else if (i == trigger) {
                hist -> SetTitle("Trigger strip");
            } else {
                hist -> SetTitle(("Channel " + to_string(i + 1)).c_str());
            }
            hist -> GetXaxis() -> SetTitle("Amplitude [mV]");
            hist -> GetYaxis() -> SetTitle("Events");
            std::cout << hist -> GetMean() << std::endl;
            legend -> AddEntry(hist, legend_labels[j].c_str());
            hist -> Scale(1.0 / pulse.at(j) -> GetEntries(conditions));
            hist -> GetYaxis() -> SetRangeUser(0, 0.35);
            hist -> SetLineWidth(3);
            if (j == 4) hist -> SetLineColor(94);
            else hist -> SetLineColor(j + 1);
        }
        legend -> Draw("SAME");
        amplitudes -> Update();
    }

    amplitudes -> SaveAs((output_folder + "/Amplitudes_comparison.png").c_str());

    // -----------------------------------------------------------------------------------
    // --------------------------------- Timestamps --------------------------------------
    // -----------------------------------------------------------------------------------
    

    gStyle -> SetOptStat(0000);
    gStyle -> SetOptFit(1011);
    for (int i = 0; i < sizeof(fit_amps) / sizeof(fit_amps[0]); i++) {
        TCanvas * timestamps = new TCanvas(("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Timestamps (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 2600, 1100);
        timestamps -> Divide(4, 2);
        for (int j = 0; j < 8; j++) {

            TLegend * legend = new TLegend(0.4, 0.7, 0.9, 0.9);
            std::vector < float > mins, maxes;

            for (int k = 0; k < pulse.size(); k++) {
                maxes.push_back(pulse.at(k) -> GetMaximum(("LP2_" + to_string(fit_amps[i])).c_str()));
                mins.push_back (pulse.at(k) -> GetMinimum(("LP2_" + to_string(fit_amps[i])).c_str()));
            }
            
            for (int k = 0; k < pulse.size(); k++) {
                TString conditions = base_conditions;

                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
                // |=====================================================| Custom selections |=====================================================|
    
                // if (k == 0) conditions += "&&LP2_10["+to_string(trigger)+"]<-11"+"&&LP2_80["+to_string(trigger)+"]<-11";
                // if (legend_labels[j] == "Ru106 AC100W40 120V" || legend_labels[j] == "Ru106 AC100W40 116V" || legend_labels[j] == "Ru106 AC100W40 113V") {
                //    conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_80["+to_string(trigger)+"]<-10";
                // }
                conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_90["+to_string(trigger)+"]<-10";
                if (legend_labels[j] == "Ru106 AC100W40 105V") {
                    conditions += "&&LP2_10[" + to_string(trigger)+"]<-8 && LP2_90["+to_string(trigger)+"]<-8";
                }
                if (legend_labels[j] == "Ru106 AC100W40 108V") {
                    conditions += "&&amp[7]<400";
                }                
                float min = -12;
                min = * std::min_element(mins.begin(), mins.end());
                float max = -6;
                max = * std::max_element(maxes.begin(), maxes.end());

                timestamps -> cd(j + 1);
                gPad -> SetLogy(1);

                conditions += " && amp[" + to_string(trigger) + "]>(amp[" + to_string(trigger - 1) + "]+amp[" + to_string(trigger) + "]+amp[" + to_string(trigger + 1) + "])*" + to_string(amplitude_fraction.at(k));

                if (k != 0) pulse.at(k) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(j) + "]>>LP2_" + to_string(fit_amps[i]) + "_hist_" + to_string(j) + "_" + to_string(k) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions, "HIST E SAME");
                else        pulse.at(k) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(j) + "]>>LP2_" + to_string(fit_amps[i]) + "_hist_" + to_string(j) + "_" + to_string(k) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions, "HIST E");

                TH1D * hist = (TH1D * ) gDirectory -> Get(("LP2_" + to_string(fit_amps[i]) + "_hist_" + to_string(j) + "_" + to_string(k)).c_str());
                if (j == mcp) {
                    hist -> SetTitle("MCP");
                } else if (j == trigger) {
                    hist -> SetTitle("Trigger strip");
                } else {
                    hist -> SetTitle(("Channel " + to_string(j + 1)).c_str());
                }
                legend -> AddEntry(hist, legend_labels[k].c_str());
                hist -> Scale(1.0 / pulse.at(k) -> GetEntries(conditions));
                hist -> SetLineWidth(2);
                if (k != 4) hist -> SetLineColor(k + 1);
                else        hist -> SetLineColor(94);
                hist -> GetXaxis() -> SetTitle("Timestamp [ns]");
                hist -> GetYaxis() -> SetTitle("Events");
            }
            timestamps -> SetLogy();
            legend -> Draw("SAME");
        }
        timestamps -> SaveAs((output_folder + "/Timestamps_LP2_" + to_string(fit_amps[i]) + "_comparison.png").c_str());
    }

    TCanvas * DT_CHI = new TCanvas("DT_CHI", "DT_CHI", 1100, 600);
    TCanvas * DT_corr_CHI = new TCanvas("DT_corr_CHI", "DT_corr_CHI", 1100, 600);
    TLegend * summary_comparison_legend = new TLegend(0.5, 0.7, 0.9, 0.9);

    std::vector < TGraph * > CHI_LP2;
    std::vector < TGraphErrors * > DT_LP2;
    std::vector < TGraph * > CHI_corr_LP2;
    std::vector < TGraphErrors * > DT_corr_LP2;

    DT_CHI -> Divide(2, 1);
    DT_corr_CHI -> Divide(2, 1);    

    std::vector < std::vector < double > > timing_resolution;
    std::vector < std::vector < double > > corr_timing_resolution;
    std::vector < std::vector < double > > timing_resolution_err;
    std::vector < std::vector < double > > corr_timing_resolution_err;
    std::vector < std::vector < double > > chi_sq;
    std::vector < std::vector < double > > corr_chi_sq;
    // std::vector < std::vector < double > > fit_amps_err;
    std::vector < double > fit_amps_err;
    std::vector < std::vector < double > > amplitude;
    std::vector < std::vector < double > > noise;
    std::vector < std::vector < double > > risetime;
    std::vector < std::vector < double > > jitterRMS;
    std::vector < std::vector < double > > jitter;
    std::vector < std::vector < double > > risetime_90_10;
    std::vector < std::vector < double > > jitter_90_10;
    std::vector < std::vector < double > > risetime_80_20;
    std::vector < std::vector < double > > jitter_80_20;
    std::vector < std::vector < double > > baseline;
    std::vector < std::vector < double > > baseline_RMS;
    std::vector < std::vector < double > > signal_noise_ratio;
    
    // baseline.at(i).at(j), baselineRMS.at(i).at(j), signal_noise_ratio.at(i).at(j)
    
    for (int dt = 0; dt < pulse.size(); dt++) {
        /*
        timing_resolution.push_back(std::vector < double >);
        corr_timing_resolution.push_back(std::vector < double >);
        timing_resolution_err.push_back(std::vector < double >);
        corr_timing_resolution_err.push_back(std::vector < double >);
        chi_sq.push_back(std::vector < double >);
        corr_chi_sq.push_back(std::vector < double >);
        // fit_amps_err.push_back(std::vector < double >);
        baseline.push_back(std::vector < double >);
        baseline_RMS.push_back(std::vector < double >);
        signal_noise_ratio.push_back(std::vector < double >);
        */

        timing_resolution.emplace_back();
        corr_timing_resolution.emplace_back(); // .push_back(std::vector < double >);
        timing_resolution_err.emplace_back(); // .push_back(std::vector < double >);
        corr_timing_resolution_err.emplace_back(); // .push_back(std::vector < double >);
        chi_sq.emplace_back(); // .push_back(std::vector < double >);
        corr_chi_sq.emplace_back(); // .push_back(std::vector < double >);
        // fit_amps_err.push_back(std::vector < double >);
        risetime.emplace_back();
        noise.emplace_back();
        amplitude.emplace_back();
        risetime_90_10.emplace_back();
        risetime_80_20.emplace_back();
        jitter.emplace_back(); // .push_back(std::vector < double >);
        jitterRMS.emplace_back();
        jitter_90_10.emplace_back(); // .push_back(std::vector < double >);
        jitter_80_20.emplace_back(); // .push_back(std::vector < double >);
        baseline.emplace_back(); // .push_back(std::vector < double >);
        baseline_RMS.emplace_back(); // .push_back(std::vector < double >);
        signal_noise_ratio.emplace_back(); // .push_back(std::vector < double >);

        TString conditions = base_conditions;
        
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        // |=====================================================| Custom selections |=====================================================|
        
        // if (legend_labels[dt] == "Ru106 AC100W40 120V" || legend_labels[dt] == "Ru106 AC100W40 116V" || legend_labels[dt] == "Ru106 AC100W40 113V") {
        //     conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_80["+to_string(trigger)+"]<-10";
        // }
        if (legend_labels[dt] == "Ru106 AC100W40 105V") {
            conditions += "&&LP2_10[" + to_string(trigger)+"]<-8 && LP2_90["+to_string(trigger)+"]<-8";
        }
        conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_90["+to_string(trigger)+"]<-10";
        if (legend_labels[dt] == "Ru106 AC100W40 108V") {
            conditions += "&&amp[7]<400";
        }
        if (legend_labels[dt] == "Ru106 AC100W40 125V") {
            n_bins = 20;
        } else {
            n_bins = 30;
        }
        std::cout<<"))))))))))))))))))))))))))>>>>> "<<legend_labels[dt]<<"      "<<conditions<<std::endl;
        conditions += " && amp[" + to_string(trigger) + "]>(amp[" + to_string(trigger - 1) + "]+amp[" + to_string(trigger) + "]+amp[" + to_string(trigger + 1) + "])*" + to_string(amplitude_fraction.at(dt));

        // --------------------------------------------------------------------------------
        // --------------------------------- Delta T --------------------------------------
        // --------------------------------------------------------------------------------

        for (int i = 0; i < sizeof(fit_amps) / sizeof(fit_amps[0]); i++) {
        
            new TCanvas("tmp", "tmp");
            
            pulse.at(dt) -> Draw(("noise["+to_string(trigger)+"]>>noise").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("baseline["+to_string(trigger)+"]>>baseline").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("amp["+to_string(trigger)+"]>>amplitude").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("baseline_RMS["+to_string(trigger)+"]>>baseline_RMS").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("risetime["+to_string(trigger)+"]>>risetime").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("(LP2_90["+to_string(trigger)+"]-LP2_10["+to_string(trigger)+"])>>risetime_90_10").c_str(), conditions, "colz"); //;"HIST E");
            pulse.at(dt) -> Draw(("(LP2_80["+to_string(trigger)+"]-LP2_20["+to_string(trigger)+"])>>risetime_80_20").c_str(), conditions, "colz"); //;"HIST E");
            
            TCanvas *risetime_canvas = new TCanvas("risetime_canvas", "risetime_canvas");
            // pulse.at(dt) -> Draw(("risetime["+to_string(trigger)+"]*noise["+to_string(trigger)+"]/amp["+to_string(trigger)+"]>>jitter").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("(LP2_90["+to_string(trigger)+"]-LP2_10["+to_string(trigger)+"])*noise["+to_string(trigger)+"]/amp["+to_string(trigger)+"]>>jitter_90_10").c_str(), conditions, "HIST E");
            pulse.at(dt) -> Draw(("(LP2_80["+to_string(trigger)+"]-LP2_20["+to_string(trigger)+"])*noise["+to_string(trigger)+"]/amp["+to_string(trigger)+"]>>jitter_80_20").c_str(), conditions, "HIST E");

            // pulse.at(dt) -> Draw(("(linear_RE_90["+to_string(trigger)+"]-linear_RE_10["+to_string(trigger)+"])>>risetime_90_10").c_str(), conditions, "colz"); //;"HIST E");
            risetime_canvas->SaveAs("jitter_canvas.png");
            
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<conditions<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            std::cout<<"))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))"<<std::endl;
            
            TH1D * amplitude_hist = (TH1D * ) gDirectory -> Get("amplitude");
            TH1D * baseline_hist = (TH1D * ) gDirectory -> Get("baseline");
            TH1D * noise_hist = (TH1D * ) gDirectory -> Get("noise");
            TH1D * baseline_RMS_hist = (TH1D * ) gDirectory -> Get("baseline_RMS");
            TH1D * risetime_hist = (TH1D * ) gDirectory -> Get("risetime");
            TH1D * risetime_90_10_hist = (TH1D * ) gDirectory -> Get("risetime_90_10");
            TH1D * risetime_80_20_hist = (TH1D * ) gDirectory -> Get("risetime_80_20");
            TH1D * jitter_90_10_hist = (TH1D * ) gDirectory -> Get("jitter_90_10");
            TH1D * jitter_80_20_hist = (TH1D * ) gDirectory -> Get("jitter_80_20");
            TH1D * jitter_hist = (TH1D * ) gDirectory -> Get("jitter");
            
            baseline.at(dt).push_back(baseline_hist->GetMean());
            baseline_RMS.at(dt).push_back(baseline_RMS_hist->GetMean());
            signal_noise_ratio.at(dt).push_back(amplitude_hist->GetMean()/noise_hist->GetRMS());
            amplitude.at(dt).push_back(amplitude_hist->GetMean());
            noise.at(dt).push_back(noise_hist->GetRMS());
            risetime.at(dt).push_back(risetime_hist->GetMean());
            jitter_90_10.at(dt).push_back(jitter_90_10_hist->GetRMS()*1000);
            jitter_80_20.at(dt).push_back(jitter_80_20_hist->GetRMS()*1000);
            risetime_90_10.at(dt).push_back(risetime_90_10_hist->GetMean());
            risetime_80_20.at(dt).push_back(risetime_80_20_hist->GetMean());
            // jitter.at(dt).push_back(risetime_hist->GetMean()/(amplitude_hist->GetMean()/noise_hist->GetRMS()));
            // jitterRMS.at(dt).push_back(jitter_hist->GetRMS());
            // jitter_90_10.at(dt).push_back(risetime_90_10_hist->GetMean()*1000/(amplitude_hist->GetMean()/noise_hist->GetRMS()));
            
            // baseline.at(dt).push_back(0.0);
            // baseline_RMS.at(dt).push_back(0.0);
            // signal_noise_ratio.at(dt).push_back(0.0);
            
            TCanvas * deltaT = new TCanvas(("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 2400, 1200);
            deltaT -> Divide(4, 2);
            deltaT -> cd(1);
            // gPad -> SetLogy(1);
            
            string histname = "DT_" + to_string(fit_amps[i]) + "_" + to_string(dt) + "_hist";
            
            pulse.at(dt) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "[" + to_string(mcp) + "]>>"+histname).c_str(), conditions, "HIST E");
            TH1D * DT = (TH1D * ) gDirectory -> Get(("DT_" + to_string(fit_amps[i]) + "_" + to_string(dt)  + "_hist").c_str());
            
            Double_t DT_range_low = DT -> GetXaxis() -> GetBinCenter(DT -> GetMaximumBin()) - rms_range_low * DT -> GetRMS();
            Double_t DT_range_high = DT -> GetXaxis() -> GetBinCenter(DT -> GetMaximumBin()) + rms_range_high * DT -> GetRMS();
            
            Double_t DT_range_low_full_range  = DT -> GetXaxis() -> GetBinCenter(DT -> GetMaximumBin()) - rms_full_range_low  * DT -> GetRMS();
            Double_t DT_range_high_full_range = DT -> GetXaxis() -> GetBinCenter(DT -> GetMaximumBin()) + rms_full_range_high * DT -> GetRMS();
            
            if (legend_labels[dt] == "Ru106 AC100W40 125V") {
                DT->Rebin(2);
            }           
            DT -> GetXaxis() -> SetTitle("DeltaT [ns]");
            DT -> GetYaxis() -> SetTitle("Events");

            // DT -> GetXaxis() -> SetRangeUser(DT_range_low_full_range, DT_range_high_full_range);
            // DT -> Rebin(rebin);
            TF1 * fgaus = new TF1("fgaus", "gaus", DT_range_low, DT_range_high);
            std::cout << DT -> GetMaximum() << std::endl;
            fgaus -> SetParameters(DT -> GetMaximum(), DT -> GetMean(), DT -> GetRMS());
            DT -> Fit(fgaus, "R");
            fgaus -> SetLineWidth(2);
            fgaus -> SetLineColor(2);
            fgaus -> Draw("same l");
            DT -> SetTitle("Delta T");
            fit_amps_err.push_back(0.0);
            if (fgaus -> GetParameter(2) * 1000 < 1000000) {
                timing_resolution.at(dt).push_back(fgaus -> GetParameter(2) * 1000);
                timing_resolution_err.at(dt).push_back(fgaus -> GetParError(2) * 1000);
                if (fit_amps[i] == 50) {
                    timing_resolution_50.push_back(fgaus -> GetParameter(2) * 1000);
                    timing_resolution_50_err.push_back(fgaus -> GetParError(2) * 1000);
                }
                std::cout << "------------> " << fgaus -> GetChisquare() / fgaus -> GetNDF() << std::endl;
                if (fgaus -> GetNDF() != 0)
                    chi_sq.at(dt).push_back(fgaus -> GetChisquare() / fgaus -> GetNDF());
                else
                    chi_sq.at(dt).push_back(0.0);
            }

            deltaT -> cd(2);
            // deltaT -> SetLeftMargin(2);
            gPad->SetLeftMargin(0.20);
            // gPad->SetLogy(1);
            histname = "DT_amp" + to_string(trigger) + "_" + to_string(fit_amps[i]) + "_" + to_string(dt) + "_hist";
            pulse.at(dt) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "[" + to_string(mcp) + "]:amp[" + to_string(trigger) + "]>>"+histname).c_str(), conditions, "colz");
            TH2D * DT_amp3 = (TH2D * ) gDirectory -> Get(("DT_amp" + to_string(trigger) + "_" + to_string(fit_amps[i]) + "_" + to_string(dt) + "_hist").c_str());
            // DT_amp3 -> GetYaxis() -> SetRangeUser(DT_range_low, DT_range_high);
            DT_amp3 -> GetXaxis() -> SetTitle("Amplitude [mV]");
            DT_amp3 -> GetYaxis() -> SetTitle("DeltaT [ns]");
            DT_amp3 -> SetTitle("DeltaT vs trigger strip amplitude"); // strigger
            TF1 * polyline = TimewalkCalculation(DT_amp3, deltaT); 
            double p0 = polyline -> GetParameter(0);
            double p1 = polyline -> GetParameter(1);
            double p2 = polyline -> GetParameter(2);
            double p3 = polyline -> GetParameter(3);

            deltaT -> cd(3);
            gPad->SetLeftMargin(0.20);
            // gPad->SetLogy(1);
            histname = to_string(mcp) + "_" + to_string(fit_amps[i]) + "_" + to_string(dt)  + "_hist";
            pulse.at(dt) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "[" + to_string(mcp) + "]:amp[" + to_string(mcp) + "]>>DT_amp"+histname).c_str(), conditions, "colz");
            TH2D * DT_amp7 = (TH2D * ) gDirectory -> Get(("DT_amp" + to_string(mcp) + "_" + to_string(fit_amps[i]) + "_" + to_string(dt) + "_hist").c_str());
            // DT_amp7 -> GetYaxis() -> SetRangeUser(DT_range_low, DT_range_high);
            DT_amp7 -> SetTitle("DeltaT vs MCP amplitude");
            DT_amp7 -> GetXaxis() -> SetTitle("Amplitude [mV]");
            DT_amp7 -> GetYaxis() -> SetTitle("DeltaT [ns]");

            // --------------------------------------------------------------------------------
            // ---------------------------- Timewalk corrected --------------------------------
            // --------------------------------------------------------------------------------
            
            deltaT -> cd(5);
            gPad->SetLeftMargin(0.20);

            histname = "DT_" + to_string(fit_amps[i]) + "_" + to_string(dt)  + "_hist_corr";
            
            // gPad -> SetLogy(1);
            gStyle -> SetOptFit(1011);
            DT_range_low  = -1 * corr_rms_range_low * DT -> GetRMS();
            DT_range_high = corr_rms_range_high * DT -> GetRMS();
            
            DT_range_low_full_range  = -1 * corr_rms_full_range_low * DT -> GetRMS();
            DT_range_high_full_range = 1 * corr_rms_full_range_high * DT -> GetRMS();

            string corrected_dt_command = "LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "] - LP2_" + to_string(fit_amps[i]) + "[" + to_string(mcp) + "] - " + Form("(%g + %g * amp[%d] + %g*amp[%d] * amp[%d] + %g * amp[%d] * amp[%d] * amp[%d])", p0, p1, trigger, p2, trigger, trigger, p3, trigger, trigger, trigger) + ">>" + histname;
            pulse.at(dt) -> Draw((corrected_dt_command).c_str(), conditions, "HIST E");
            TH1D * DT_corr = (TH1D * ) gDirectory -> Get(("DT_" + to_string(fit_amps[i]) + "_" + to_string(dt)  + "_hist_corr").c_str());

            DT_corr -> GetXaxis() -> SetRangeUser(DT_range_low_full_range, DT_range_high_full_range);
            // DT_corr->Rebin(rebin); // The fit fails if the rebin is here
            DT_corr -> SetTitle("Corrected Delta T");
            DT_corr -> GetXaxis() -> SetTitle("DeltaT [ns]");
            DT_corr -> GetYaxis() -> SetTitle("Events");
            TF1 * fgaus_corr = new TF1("fgaus_corr", "gaus", DT_range_low, DT_range_high);
            std::cout << DT_corr -> GetMaximum() << std::endl;
            fgaus_corr -> SetParameters(DT_corr -> GetMaximum(), DT_corr -> GetMean(), DT_corr -> GetRMS());
            // DT_corr -> Rebin(rebin);
            DT_corr -> Fit(fgaus_corr, "R");
            fgaus_corr -> SetLineWidth(2);
            fgaus_corr -> SetLineColor(2);
            fgaus_corr -> Draw("same l");
            fit_amps_err.push_back(0.0);
            if (fgaus_corr -> GetParameter(2) * 1000 < 1000000) {
                corr_timing_resolution.at(dt).push_back(fgaus_corr -> GetParameter(2) * 1000);
                corr_timing_resolution_err.at(dt).push_back(fgaus_corr -> GetParError(2) * 1000);
                if (fit_amps[i] == 50) {
                    corr_timing_resolution_50.push_back(fgaus_corr -> GetParameter(2) * 1000);
                    corr_timing_resolution_50_err.push_back(fgaus_corr -> GetParError(2) * 1000);
                }
                if (fgaus_corr -> GetNDF() != 0)
                    corr_chi_sq.at(dt).push_back(fgaus_corr -> GetChisquare() / fgaus_corr -> GetNDF());
                else
                    corr_chi_sq.at(dt).push_back(0.0);
            }

            // std::cout << "------CORRECTED------> " << fgaus_corr -> GetChisquare() / n_bins << std::endl;
            // corr_chi_sq.at(dt).push_back(fgaus_corr -> GetChisquare() / fgaus_corr -> GetNDF());

            deltaT -> cd(6);
            gPad->SetLeftMargin(0.20);
            // gPad->SetLogy(1);
            string corrected_dt_vs_amp_command = "LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "] - LP2_" + to_string(fit_amps[i]) + "[" + to_string(mcp) + "] - " + Form("(%g + %g * amp[%d] + %g*amp[%d] * amp[%d] + %g * amp[%d] * amp[%d] * amp[%d])", p0, p1, trigger, p2, trigger, trigger, p3, trigger, trigger, trigger) + ":amp[" + to_string(trigger) + "]>>DT_amp" + to_string(trigger) + "_" + to_string(fit_amps[i]) + "_hist_corr";
            pulse.at(dt) -> Draw((corrected_dt_vs_amp_command).c_str(), conditions, "colz");
            TH2D * DT_amp3_corr = (TH2D * ) gDirectory -> Get(("DT_amp" + to_string(trigger) + "_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
            // DT_amp3_corr -> GetYaxis() -> SetRangeUser(DT_range_low_full_range, DT_range_high_ful_range);
            DT_amp3_corr -> GetXaxis() -> SetTitle("Amplitude [mV]");
            DT_amp3_corr -> GetYaxis() -> SetTitle("DeltaT [ns]"); // DT_amp7->GetYaxis()->Set(n_bins, DT_range_low, DT_range_high);
            // DT_amp3_corr->GetYaxis()->SetRangeUser(DT_range_low, DT_range_high);
            // TF1 * fited_curve = TimewalkCalculation(DT_amp3); // polyline
            DT_amp3_corr -> SetTitle("DeltaT vs trigger strip amplitude"); // strigger

            deltaT -> cd(7);
            gPad->SetLeftMargin(0.20);
            // gPad->SetLogy(1);
            string corrected_dt_vs_MCP_amp_command = "LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "[" + to_string(mcp) + "] - " + Form("(%g + %g * amp[%d] + %g*amp[%d] * amp[%d] + %g * amp[%d] * amp[%d] * amp[%d])", p0, p1, trigger, p2, trigger, trigger, p3, trigger, trigger, trigger) + ":amp[" + to_string(mcp) + "]>>DT_amp" + to_string(mcp) + "_" + to_string(fit_amps[i]) + "_hist_corr";
            std::cout << corrected_dt_vs_MCP_amp_command << std::endl;
            pulse.at(dt) -> Draw((corrected_dt_vs_MCP_amp_command).c_str(), conditions, "colz");
            TH2D * DT_amp7_corr = (TH2D * ) gDirectory -> Get(("DT_amp" + to_string(mcp) + "_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
            // DT_amp7_corr -> GetYaxis() -> SetRangeUser(DT_range_low, DT_range_high);
            // DT_amp7_corr->GetYaxis()->SetRangeUser(DT_range_low, DT_range_high);
            DT_amp7_corr -> GetXaxis() -> SetTitle("Amplitude [mV]");
            DT_amp7_corr -> GetYaxis() -> SetTitle("DeltaT [ns]");
            // DT_amp7->GetYaxis()->Set(n_bins, DT_range_low, DT_range_high);
            DT_amp7_corr -> SetTitle("DeltaT vs MCP amplitude");

            deltaT -> SaveAs((output_folder + "/DeltaT_LP2_" + to_string(fit_amps[i]) + "_" + to_string(dt) + "_comparison.png").c_str());
        }

        // ---------------------------------------------------------------------------------------
        // ------------ Timing resolution vs amplitude percentage and chi squared ----------------
        // ---------------------------------------------------------------------------------------

        DT_CHI -> cd(1);
        DT_LP2.push_back(new TGraphErrors(timing_resolution.at(dt).size(), fit_amps_V.data(), timing_resolution.at(dt).data(), fit_amps_err.data(), timing_resolution_err.at(dt).data()));
        summary_comparison_legend -> AddEntry(DT_LP2.at(dt), legend_labels[dt].c_str());
        // DT_LP2.at(dt)->SetLineColor(2);
        // DT_LP2.at(dt)->SetLineWidth(2);
        if (dt != 4) DT_LP2.at(dt) -> SetLineColor(dt + 1);
        else DT_LP2.at(dt) -> SetLineColor(94);
        // DT_LP2.at(dt) -> SetLineColor(dt + 1);
        DT_LP2.at(dt) -> SetLineWidth(2);
        DT_LP2.at(dt) -> SetTitle("");
        DT_LP2.at(dt) -> GetXaxis() -> SetTitle("Fit %");
        DT_LP2.at(dt) -> GetYaxis() -> SetTitle("Timing resolution [ns]");
        DT_LP2.at(dt) -> GetYaxis() -> SetRangeUser(25, 55);
        // DT_LP2.at(dt) -> Draw("SAME APL*");
        if (dt != 0) DT_LP2.at(dt) -> Draw("SAMEPL*");
        else DT_LP2.at(dt) -> Draw("APL*");
        summary_comparison_legend -> Draw("SAME");

        DT_CHI -> cd(2);
        CHI_LP2.push_back(new TGraph(timing_resolution.at(dt).size(), fit_amps_V.data(), chi_sq.at(dt).data()));
        // CHI_LP2.at(dt)->SetLineColor(2);
        // CHI_LP2.at(dt)->SetLineWidth(2);
        if (dt != 4) CHI_LP2.at(dt) -> SetLineColor(dt + 1);
        else CHI_LP2.at(dt) -> SetLineColor(94);
        // CHI_LP2.at(dt) -> SetLineColor(dt + 1);
        CHI_LP2.at(dt) -> SetLineWidth(2);
        CHI_LP2.at(dt) -> SetTitle("");
        CHI_LP2.at(dt) -> GetXaxis() -> SetTitle("Fit %");
        CHI_LP2.at(dt) -> GetYaxis() -> SetTitle("Chi^2/NDF");
        CHI_LP2.at(dt) -> GetYaxis() -> SetRangeUser(0, 3.5);
        // CHI_LP2.at(dt) -> Draw("SAME APL*");
        if (dt != 0) CHI_LP2.at(dt) -> Draw("SAMEPL*");
        else CHI_LP2.at(dt) -> Draw("APL*");
        summary_comparison_legend -> Draw("SAME");

        DT_CHI -> Update();
        
        // ---------------------------------------------------------------------------------------
        // --------- Corrected timing resolution vs amplitude percentage and chi squared ---------
        // ---------------------------------------------------------------------------------------

        DT_corr_CHI -> cd(1);
        DT_corr_LP2.push_back(new TGraphErrors(corr_timing_resolution.at(dt).size(), fit_amps_V.data(), corr_timing_resolution.at(dt).data(), fit_amps_err.data(), corr_timing_resolution_err.at(dt).data()));
        DT_corr_LP2.at(dt)->GetYaxis()->SetRangeUser(25, 60);
        if (dt != 4) DT_corr_LP2.at(dt) -> SetLineColor(dt + 1);
        else DT_corr_LP2.at(dt) -> SetLineColor(94);
        // DT_corr_LP2.at(dt)->SetLineColor(dt + 1);
        DT_corr_LP2.at(dt)->SetLineWidth(2);
        DT_corr_LP2.at(dt) -> GetXaxis() -> SetTitle("Fit %");
        DT_corr_LP2.at(dt) -> GetYaxis() -> SetTitle("Timing resolution [ns]");
        DT_corr_LP2.at(dt) -> SetTitle("");
        if (dt != 0) DT_corr_LP2.at(dt) -> Draw("SAMEPL*");
        else DT_corr_LP2.at(dt) -> Draw("APL*");
        summary_comparison_legend -> Draw("SAME");

        DT_corr_CHI -> cd(2);
        CHI_corr_LP2.push_back(new TGraph(corr_timing_resolution.at(dt).size(), fit_amps_V.data(), corr_chi_sq.at(dt).data()));
        CHI_corr_LP2.at(dt)->GetYaxis()->SetRangeUser(0, 5);
        if (dt != 4) CHI_corr_LP2.at(dt) -> SetLineColor(dt + 1);
        else CHI_corr_LP2.at(dt) -> SetLineColor(94);
        // CHI_corr_LP2.at(dt)->SetLineColor(dt + 1);
        CHI_corr_LP2.at(dt)->SetLineWidth(2); 
        CHI_corr_LP2.at(dt) -> GetXaxis() -> SetTitle("Fit %");
        CHI_corr_LP2.at(dt) -> GetYaxis() -> SetTitle("Chi^2/NDF");
        CHI_corr_LP2.at(dt) -> SetTitle("");
        if (dt != 0) CHI_corr_LP2.at(dt) -> Draw("SAMEPL*");
        else CHI_corr_LP2.at(dt) -> Draw("APL*");
        summary_comparison_legend -> Draw("SAME");
        
        DT_corr_CHI->Update();

    }

    TCanvas * DT_vs_BV_50_canvas = new TCanvas("DT_vs_BV_50_canvas", "DT_vs_BV_50_canvas", 1100, 600);
    // TLegend * final_comparison_legend = new TLegend(0.5, 0.7, 0.9, 0.9);
    
    DT_vs_BV_50_canvas->cd();
    auto DT_vs_BV_graph = new TGraphErrors(bias_voltage.size(), bias_voltage.data(), timing_resolution_50.data(), bias_voltage_err.data(), timing_resolution_50_err.data());
    DT_vs_BV_graph->SetLineColor(2);
    DT_vs_BV_graph->SetLineWidth(2); 
    DT_vs_BV_graph -> GetXaxis() -> SetTitle("Bias voltage [V]");
    DT_vs_BV_graph -> GetYaxis() -> SetTitle("Timing resolution [ps]");
    DT_vs_BV_graph -> SetTitle("");
    DT_vs_BV_graph -> Draw("APL*");
    DT_vs_BV_50_canvas -> SaveAs((output_folder + "/Final_comparison_50.png").c_str());
    
    TCanvas * Corr_DT_vs_BV_50_canvas = new TCanvas("Corr_DT_vs_BV_50_canvas", "Corr_DT_vs_BV_50_canvas", 1100, 600);
    // TLegend * final_comparison_legend = new TLegend(0.5, 0.7, 0.9, 0.9);
    
    Corr_DT_vs_BV_50_canvas->cd();
    auto Corr_DT_vs_BV_graph = new TGraphErrors(bias_voltage.size(), bias_voltage.data(), corr_timing_resolution_50.data(), bias_voltage_err.data(), corr_timing_resolution_50_err.data());
    Corr_DT_vs_BV_graph->SetLineColor(2);
    Corr_DT_vs_BV_graph->SetLineWidth(2); 
    Corr_DT_vs_BV_graph -> GetXaxis() -> SetTitle("Bias voltage [V]");
    Corr_DT_vs_BV_graph -> GetYaxis() -> SetTitle("Timing resolution [ps]");
    Corr_DT_vs_BV_graph -> SetTitle("");
    Corr_DT_vs_BV_graph -> Draw("APL*");
    Corr_DT_vs_BV_50_canvas -> SaveAs((output_folder + "/Final_comparison_corrected_50.png").c_str());
    
    std::cout << "Reached the end" << std::endl;
    DT_CHI -> SaveAs((output_folder + "/Summary_comparison.png").c_str());
    DT_corr_CHI -> SaveAs((output_folder + "/Corrected_summary.png").c_str());

    for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++) {
        std::cout<<"-==============> "<<i<<std::endl;
        FILE *fp = fopen(("./CombinedDatasets/Summary_"+to_string(fit_amps[i])+".csv").c_str(), "w");
        if (fp == NULL) {
            printf("Error opening file!\n");
        }
        fprintf(fp, "ID,Dataset,TimingResolution[ps],CorrTimingResolution[ps],Baseline[mV],BaselineRMS[mV],Signal[mV],Noise[mV],Risetime 10-90% (ps),Jitter 10-90% (ps),Risetime 20-80% (ps),Jitter 20-80% (ps),Landau Contibution\n"); // ,Risetime(TimingDAQ)[ps],Jitter(TimingDAQ)[ps]
        for (int j = 0; j < bias_voltage.size(); j++) {
            std::cout<<legend_labels[j].c_str()<<std::endl;
            std::cout<<timing_resolution.at(j).at(i)<<std::endl;
            std::cout<<timing_resolution_err.at(j).at(i)<<std::endl;
            std::cout<<corr_timing_resolution.at(j).at(i)<<std::endl;
            std::cout<<corr_timing_resolution_err.at(j).at(i)<<std::endl;
            std::cout<<baseline.at(j).at(i)<<std::endl;
            std::cout<<baseline_RMS.at(j).at(i)<<std::endl;
            std::cout<<signal_noise_ratio.at(j).at(i)<<std::endl;
            fprintf(fp, "%d,%s,%f±%f,%f±%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n", j+1, legend_labels[j].c_str(), timing_resolution.at(j).at(i), timing_resolution_err.at(j).at(i), corr_timing_resolution.at(j).at(i), corr_timing_resolution_err.at(j).at(i), baseline.at(j).at(i), baseline_RMS.at(j).at(i), amplitude.at(j).at(i), noise.at(j).at(i), risetime_90_10.at(j).at(i), jitter_90_10.at(j).at(i), risetime_80_20.at(j).at(i), jitter_80_20.at(j).at(i), sqrt(pow(corr_timing_resolution.at(j).at(i),2)-pow(jitter_90_10.at(j).at(i),2))); // , risetime.at(j).at(i), jitter.at(j).at(i)
        }
        fclose(fp);
    }

}
