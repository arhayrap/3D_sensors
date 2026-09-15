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

TF1 * TimewalkCalculation(TH2D * histo2D) {
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

void DataOverview() {
    // TFile * file = TFile::Open("/home/aram/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_AC100W80_strip3mm_bv116_mcp4400_trigChi4_Ch8_collimatedTwice_Ru106_3days_4.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_Ru106_3days_5_from_memory.root");
    TFile * file = TFile::Open("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W40_strip3mm_bv116_mcp4400_trigCh4_Ch8_Ru106_3days.root"); // proc_F_AC100W40_strip3mm_bv116_mcp4400_trigCh4_Ch8_Ru106_3days.root");
    // TFile * file = TFile::Open("/Users/PITLab/3D_sensors/first_files/prep_AC100W80_strip3mm_bv116_mcp4400_linear.root");
    TTree * pulse = (TTree * ) file -> Get("pulse");
    // TString conditions = "";
    // TString conditions = "LP2_10[3]<-1 && LP2_20[3]<-1 && LP2_50[3]<-1 && LP2_80[3]<-1 && LP2_10[7]<-1 && LP2_20[7]<-1 && LP2_50[7]<-1 && LP2_80[7]<-1";
    // TString conditions = "baseline_RMS[3]<3 && amp[3]>10 && LP2_20[3]<0 && amp[7]>20 && amp[7]<80 && amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6]"; // baseline_RMS[3]<3 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6] && amp[3]>35 && amp[7]<80 && amp[7]>20"; 

    int n_bins = 15;
    int rebin = 5;
    float rms_range_high = 1.0;
    float rms_range_low = 1.5;
    int trigger = 5, mcp = 7; // The channels of the triggers and the time reference (MCP). Starts from 0 to 7.

    TString conditions = "baseline_RMS["+to_string(trigger)+"]<3 && LP2_10["+to_string(trigger)+"]<-10 && LP2_80["+to_string(trigger)+"]<-10 && LP2_10["+to_string(trigger)+"]>-12.5 && LP2_80["+to_string(trigger)+"]>-12.5 && amp["+to_string(trigger)+"]>5*baseline_RMS["+to_string(trigger)+"]"; // LP2_10[3]>-13 && && LP2_80[3]>-13";

    // |(((((((((((((((((((((((((((((( Selecting the leading strip )))))))))))))))))))))))))))))))|
    // conditions += "&& amp[0]>amp[1] && amp[0]>amp[2] && amp[0]>amp[3] && amp[0]>amp[4] && amp[0]>amp[5] && amp[0]>amp[6]"; // Channel 1
    // conditions += "&& amp[1]>amp[0] && amp[1]>amp[2] && amp[1]>amp[3] && amp[1]>amp[4] && amp[1]>amp[5] && amp[1]>amp[6]"; // Channel 2
    // conditions += "&& amp[2]>amp[0] && amp[2]>amp[1] && amp[2]>amp[3] && amp[2]>amp[4] && amp[2]>amp[5] && amp[2]>amp[6]"; // Channel 3
    // conditions += "&& amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6]"; // Channel 4: Trigger strip
    // conditions += "&& amp[4]>amp[0] && amp[4]>amp[1] && amp[4]>amp[2] && amp[4]>amp[3] && amp[4]>amp[5] && amp[4]>amp[6]"; // Channel 4
    // conditions += "&& amp[5]>amp[0] && amp[5]>amp[1] && amp[5]>amp[2] && amp[5]>amp[3] && amp[5]>amp[4] && amp[5]>amp[6]"; // Channel 5
    // conditions += "&& amp[6]>amp[0] && amp[6]>amp[1] && amp[6]>amp[2] && amp[6]>amp[3] && amp[6]>amp[4] && amp[6]>amp[5]"; // Channel 6

    conditions += " && amp["+to_string(trigger)+"]>(amp[2]+amp[3]+amp[4])*0.56"; // the lowest applied cut is at 56%

    // conditions += " && LP2_20[2]>-12 && LP2_20[2]<-10";

    // ______________________________________________________________________________________
    // ______________________________ Amplitude proportion cut ______________________________
    // ______________________________________________________________________________________
    
    for (int t = 1; t < 6; t++) {

    std::vector<float> proportion_cut, efficiency;

    for (int i = 10; i <= 70; ++i) {
        std::string base_cut = "baseline_RMS["+std::to_string(t)+"]<3"
            "&&LP2_10["+std::to_string(t)+"]<-1"
            "&&LP2_80["+std::to_string(t)+"]<-1";
            
        std::string cut =
            "baseline_RMS["+std::to_string(t)+"]<3"
            "&&LP2_10["+std::to_string(t)+"]<-1"
            "&&LP2_80["+std::to_string(t)+"]<-1"
            "&&amp["+std::to_string(t)+"]>(amp["+std::to_string(t-1)+"]+amp["+std::to_string(t)+"]+amp["+std::to_string(t+1)+"])*" +
            std::to_string(i) + ".0/100.0";
        
        std::cout << cut << std::endl;
        std::cout << pulse -> GetEntries(cut.c_str()) << std::endl;
        
        proportion_cut.push_back(i);
        efficiency.push_back(pulse->GetEntries(cut.c_str())*100.0/pulse->GetEntries(base_cut.c_str()));
    }
    
    TCanvas * canvas_proportion_cut_vs_efficiency = new TCanvas("Amplitude proportion cut", "Amplitude proportion cut", 600, 500);
    canvas_proportion_cut_vs_efficiency->cd();
    TGraph * proportion_cut_vs_efficiency = new TGraph(proportion_cut.size(), proportion_cut.data(), efficiency.data());
    proportion_cut_vs_efficiency->GetXaxis()->SetTitle("Amplitude proportion cut [mV]");
    proportion_cut_vs_efficiency->GetYaxis()->SetTitle("Efficiency [%]");
    proportion_cut_vs_efficiency->GetYaxis()->SetRangeUser(0,110);
    proportion_cut_vs_efficiency->SetTitle(("Leading strip, channel: "+to_string(t)).c_str());
    proportion_cut_vs_efficiency->Draw("APL*");
    canvas_proportion_cut_vs_efficiency->SaveAs(("proportion_cut_vs_efficiency_lead_channel_"+to_string(t)+".png").c_str());

    }
    
    //  && amp[3]>60 && amp[3]>amp[0]&&amp[3]>amp[1]&&amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6]"; // baseline_RMS[3]<3 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6] && amp[3]>35 && amp[7]<80 && amp[7]>20"; 
    // TString conditions = "baseline_RMS[3]<3 && LP2_10[3]>-10 && LP2_10[3]<-8 && LP2_80[3]>-10 && LP2_80[3]<-8 && amp[3]>65 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6]"; // baseline_RMS[3]<3 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6] && amp[3]>35 && amp[7]<80 && amp[7]>20"; 
    // && LP2_50[2]<-1.0 && LP2_50[4]<-1.0 && LP2_50[5]<-1.0 && LP2_50[1]<-1.0";

    gStyle -> SetOptStat(1110);
    gStyle -> SetOptFit(1011);

    // Amplitudes 
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 2100, 1100);
    amplitudes -> Divide(4, 2);
    for (int i = 0; i < 8; i++) {
        amplitudes -> cd(i + 1);
        pulse -> Draw(("amp[" + to_string(i) + "]>>hist" + to_string(i)).c_str(), conditions);
        TH1D * hist = (TH1D * ) gDirectory -> Get(("hist" + to_string(i)).c_str());
        if (i == mcp) {
            hist -> SetTitle("MCP");
        } else if (i == trigger) {
            hist -> SetTitle("Trigger strip");
        } else {
            hist -> SetTitle(("Channel " + to_string(i + 1)).c_str());
        }
        hist -> GetXaxis() -> SetTitle("Amplitude [mV]");
        hist -> GetYaxis() -> SetTitle("Events");
    }
    amplitudes -> SaveAs("Amplitudes.png");

    // Amplitude dependency
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
    amplitudes_dependency -> SaveAs("Amplitudes_comparison.png");

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
        timestamps -> SaveAs(("Timestamps_LP2_" + to_string(fit_amps[i]) + ".png").c_str());
    }

    for (int i = 0; i < sizeof(fit_amps) / sizeof(fit_amps[0]); i++) {
        TCanvas * timestamps = new TCanvas(("Timestamps (linear_RE_" + to_string(fit_amps[i]) + ")").c_str(), ("Timestamps (linear_RE_" + to_string(fit_amps[i]) + ")").c_str(), 2100, 1100);
        timestamps -> Divide(4, 2);
        for (int j = 0; j < 8; j++) {
            timestamps -> cd(j + 1);
            gPad -> SetLogy(1);
            pulse -> Draw(("linear_RE_" + to_string(fit_amps[i]) + "[" + to_string(j) + "]>>linear_RE_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str(), conditions);
            TH1D * hist = (TH1D * ) gDirectory -> Get(("linear_RE_" + to_string(fit_amps[i]) + "_hist" + to_string(j)).c_str());
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
        timestamps -> SaveAs(("Timestamps_RE_" + to_string(fit_amps[i]) + ".png").c_str());
    }

    std::vector < double > timing_resolution;
    std::vector < double > corr_timing_resolution;
    std::vector < double > timing_resolution_err;
    std::vector < double > corr_timing_resolution_err;
    std::vector < double > chi_sq;
    std::vector < double > corr_chi_sq;
    std::vector < double > fit_amps_err;

    // Delta T
    for (int i = 0; i < sizeof(fit_amps) / sizeof(fit_amps[0]); i++) {
        TCanvas * deltaT = new TCanvas(("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 1600, 1100);

        deltaT -> Divide(3, 2);
        deltaT -> cd(1);
        gPad -> SetLogy(1);
        pulse -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "["+to_string(mcp)+"]>>DT_" + to_string(fit_amps[i]) + "_hist").c_str(), conditions, "HIST E");
        TH1D * DT = (TH1D * ) gDirectory -> Get(("DT_" + to_string(fit_amps[i]) + "_hist").c_str());
        DT -> SetTitle("Delta T");
        DT -> GetXaxis() -> SetTitle("DeltaT [ns]");
        DT -> GetYaxis() -> SetTitle("Events");
        Double_t DT_range_low = DT -> GetMean() - rms_range_low * DT -> GetRMS();
        Double_t DT_range_high = DT -> GetMean() + rms_range_high * DT -> GetRMS();
        std::cout << ")))))))))))))))))))))))> " << DT_range_low << "     " << DT_range_high << std::endl;
        // DT->GetXaxis()->SetRangeUser(DT_range_low, DT_range_high);
        DT -> Rebin(rebin);
        TF1 * fgaus = new TF1("fgaus", "gaus", DT_range_low, DT_range_high);
        std::cout << DT -> GetMaximum() << std::endl;
        fgaus -> SetParameters(DT -> GetMaximum(), DT -> GetMean(), DT -> GetRMS());
        DT -> Fit(fgaus, "R");
        fgaus -> SetLineWidth(2);
        fgaus -> SetLineColor(2);
        fgaus -> Draw("same l");
        DT -> SetTitle("DeltaT");
        fit_amps_err.push_back(0.0);
        if (fgaus -> GetParameter(2) * 1000 < 1000000) {
            timing_resolution.push_back(fgaus -> GetParameter(2) * 1000);
            timing_resolution_err.push_back(fgaus -> GetParError(2) * 1000);
            std::cout << "------------> " << fgaus -> GetChisquare() / fgaus -> GetNDF() << std::endl;
            if (fgaus -> GetNDF() != 0)
                chi_sq.push_back(fgaus -> GetChisquare() / fgaus -> GetNDF());
            else
                chi_sq.push_back(0.0);
        }
        deltaT -> cd(2);
        // gPad->SetLogy(1);
        pulse -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "["+to_string(mcp)+"]:amp[" + to_string(trigger) + "]>>DT_amp"+to_string(trigger)+"_" + to_string(fit_amps[i]) + "_hist").c_str(), conditions, "colz");
        TH2D * DT_amp3 = (TH2D * ) gDirectory -> Get(("DT_amp"+to_string(trigger)+"_" + to_string(fit_amps[i]) + "_hist").c_str());
        // DT_amp3->GetYaxis()->SetRangeUser(DT_range_low, DT_range_high);
        DT_amp3 -> GetXaxis() -> SetTitle("Amplitude [mV]");
        DT_amp3 -> GetYaxis() -> SetTitle("DeltaT [ns]"); // DT_amp7->GetYaxis()->Set(n_bins, DT_range_low, DT_range_high);        
        TF1 * polyline = TimewalkCalculation(DT_amp3); // polyline <===============================================================================>
        double p0 = polyline -> GetParameter(0);
        double p1 = polyline -> GetParameter(1);
        double p2 = polyline -> GetParameter(2);
        double p3 = polyline -> GetParameter(3);
        std::cout << ">>>>>>>>>>>>>>>>> " << polyline -> Eval(10.0) << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>> " << polyline -> Eval(20.0) << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>> " << polyline -> Eval(30.0) << std::endl;

        // std::cout<<"-------------------------------------------->>>> "<<Form("(%g+%g*amp[3]+%g*amp[3]*amp[3]+%g*amp[3]*amp[3]*amp[3])", p0, p1, p2, p3)<<std::endl;
        DT_amp3 -> SetTitle("DeltaT vs trigger strip amplitude"); // strigger

        deltaT -> cd(3);
        // gPad->SetLogy(1);
        string dt_command = "LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "]-LP2_" + to_string(fit_amps[i]) + "["+to_string(mcp)+"]:amp["+to_string(mcp)+"]>>DT_amp"+to_string(mcp)+"_" + to_string(fit_amps[i]) + "_hist";
        pulse -> Draw((dt_command).c_str(), conditions, "colz");
        TH2D * DT_amp7 = (TH2D * ) gDirectory -> Get(("DT_amp"+to_string(mcp)+"_" + to_string(fit_amps[i]) + "_hist").c_str());
        DT_amp7 -> SetTitle("DeltaT vs MCP amplitude");
        DT_amp7 -> GetXaxis() -> SetTitle("Amplitude [mV]");
        DT_amp7 -> GetYaxis() -> SetTitle("DeltaT [ns]"); // DT_amp7->GetYaxis()->Set(n_bins, DT_range_low, DT_range_high);
        deltaT -> SaveAs(("DeltaT_LP2_" + to_string(fit_amps[i]) + ".png").c_str());

        // ---------------------------- Timewalk corrected --------------------------------
        deltaT -> cd(4);
        gPad -> SetLogy(1);
        gStyle -> SetOptFit(1011);
        DT_range_low = -rms_range_low * DT -> GetRMS();
        DT_range_high = rms_range_high * DT -> GetRMS();
        string corrected_dt_command = "LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "] - LP2_" + to_string(fit_amps[i]) + "["+to_string(mcp)+"] - " + Form("(%g + %g * amp[%d] + %g*amp[%d] * amp[%d] + %g * amp[%d] * amp[%d] * amp[%d])", p0, p1, trigger, p2, trigger, trigger, p3, trigger, trigger, trigger) + ">>DT_" + to_string(fit_amps[i]) + "_hist_corr";
        std::cout << "--------------------------------------------++++++++++>>>> " << corrected_dt_command << std::endl;
        pulse -> Draw((corrected_dt_command).c_str(), conditions, "HIST E");
        TH1D * DT_corr = (TH1D * ) gDirectory -> Get(("DT_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
        // DT_corr->GetXaxis()->SetRangeUser(DT_range_low, DT_range_high);
        // DT_corr->Rebin(rebin); // The fit fails if the rebin is here
        DT_corr -> SetTitle("Corrected Delta T");
        DT_corr -> GetXaxis() -> SetTitle("DeltaT [ns]");
        DT_corr -> GetYaxis() -> SetTitle("Events");
        TF1 * fgaus_corr = new TF1("fgaus_corr", "gaus", DT_range_low, DT_range_high);
        std::cout << DT_corr -> GetMaximum() << std::endl;
        fgaus_corr -> SetParameters(DT_corr -> GetMaximum(), DT_corr -> GetMean(), DT_corr -> GetRMS());
        DT_corr -> Rebin(rebin);
        DT_corr -> Fit(fgaus_corr, "R");
        fgaus_corr -> SetLineWidth(2);
        fgaus_corr -> SetLineColor(2);
        fgaus_corr -> Draw("same l");
        fit_amps_err.push_back(0.0);
        corr_timing_resolution.push_back(fgaus_corr -> GetParameter(2) * 1000);
        corr_timing_resolution_err.push_back(fgaus_corr -> GetParError(2) * 1000);
        std::cout << "------CORRECTED------> " << fgaus_corr -> GetChisquare() / n_bins << std::endl;
        corr_chi_sq.push_back(fgaus_corr -> GetChisquare() / n_bins);
        /*
        if (fgaus_corr->GetNDF()!=0)
            corr_chi_sq.push_back(fgaus_corr->GetChisquare()/fgaus_corr->GetNDF());
        else
            corr_chi_sq.push_back(0.0);
        */
        deltaT -> cd(5);
        // gPad->SetLogy(1);
        string corrected_dt_vs_amp_command = "LP2_" + to_string(fit_amps[i]) + "[" + to_string(trigger) + "] - LP2_" + to_string(fit_amps[i]) + "["+to_string(mcp)+"] - " + Form("(%g + %g * amp[%d] + %g*amp[%d] * amp[%d] + %g * amp[%d] * amp[%d] * amp[%d])", p0, p1, trigger, p2, trigger, trigger, p3, trigger, trigger, trigger) + ":amp["+to_string(trigger)+"]>>DT_amp"+to_string(trigger)+"_" + to_string(fit_amps[i]) + "_hist_corr";
        std::cout << "--------------------------------------------++++++++++>>>> " << corrected_dt_vs_amp_command << std::endl;
        pulse -> Draw((corrected_dt_vs_amp_command).c_str(), conditions, "colz");
        TH2D * DT_amp3_corr = (TH2D * ) gDirectory -> Get(("DT_amp"+to_string(trigger)+"_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
        DT_amp3_corr -> GetXaxis() -> SetTitle("Amplitude [mV]");
        DT_amp3_corr -> GetYaxis() -> SetTitle("DeltaT [ns]"); // DT_amp7->GetYaxis()->Set(n_bins, DT_range_low, DT_range_high);
        // DT_amp3_corr->GetYaxis()->SetRangeUser(DT_range_low, DT_range_high);
        // TF1 * fited_curve = TimewalkCalculation(DT_amp3); // polyline
        DT_amp3_corr -> SetTitle("DeltaT vs trigger strip amplitude"); // strigger

        deltaT -> cd(6);
        // gPad->SetLogy(1);
        string corrected_dt_vs_MCP_amp_command = "LP2_" + to_string(fit_amps[i]) + "["+to_string(trigger)+"]-LP2_" + to_string(fit_amps[i]) + "["+to_string(mcp)+"] - " + Form("(%g + %g * amp[%d] + %g*amp[%d] * amp[%d] + %g * amp[%d] * amp[%d] * amp[%d])", p0, p1, trigger, p2, trigger, trigger, p3, trigger, trigger, trigger) + ":amp["+to_string(mcp)+"]>>DT_amp"+to_string(mcp)+"_" + to_string(fit_amps[i]) + "_hist_corr";
        std::cout << corrected_dt_vs_MCP_amp_command << std::endl;
        pulse -> Draw((corrected_dt_vs_MCP_amp_command).c_str(), conditions, "colz");
        TH2D * DT_amp7_corr = (TH2D * ) gDirectory -> Get(("DT_amp"+to_string(mcp)+"_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
        // DT_amp7_corr->GetYaxis()->SetRangeUser(DT_range_low, DT_range_high);
        DT_amp7_corr -> GetXaxis() -> SetTitle("Amplitude [mV]");
        DT_amp7_corr -> GetYaxis() -> SetTitle("DeltaT [ns]"); // DT_amp7->GetYaxis()->Set(n_bins, DT_range_low, DT_range_high);
        DT_amp7_corr -> SetTitle("DeltaT vs MCP amplitude");

        deltaT -> SaveAs(("DeltaT_LP2_" + to_string(fit_amps[i]) + ".png").c_str());

    }

    // Timing resolution vs amplitude percentage and chi squared
    TCanvas * DT_CHI = new TCanvas("DT_CHI", "DT_CHI", 1100, 600);
    DT_CHI -> Divide(2, 1);

    DT_CHI -> cd(1);
    auto DT_LP2 = new TGraphErrors(timing_resolution.size(), fit_amps_V.data(), timing_resolution.data(), fit_amps_err.data(), timing_resolution_err.data());
    // DT_LP2->SetLineColor(2);
    // DT_LP2->SetLineWidth(2);
    DT_LP2 -> SetTitle("");
    DT_LP2 -> GetXaxis() -> SetTitle("Fit %");
    DT_LP2 -> GetYaxis() -> SetTitle("Timing resolution [ns]");
    DT_LP2 -> Draw("APL*");

    DT_CHI -> cd(2);
    auto CHI_LP2 = new TGraph(timing_resolution.size(), fit_amps_V.data(), chi_sq.data());
    // CHI_LP2->SetLineColor(2);
    // CHI_LP2->SetLineWidth(2); 
    CHI_LP2 -> SetTitle("");
    CHI_LP2 -> GetXaxis() -> SetTitle("Fit %");
    CHI_LP2 -> GetYaxis() -> SetTitle("Chi^2/NDF");
    CHI_LP2 -> Draw("APL*");

    DT_CHI -> SaveAs("Summary.png");

    // Corrected timing resolution vs amplitude percentage and chi squared
    TCanvas * DT_corr_CHI = new TCanvas("DT_corr_CHI", "DT_corr_CHI", 1100, 600);
    DT_corr_CHI -> Divide(2, 1);

    DT_corr_CHI -> cd(1);
    auto DT_corr_LP2 = new TGraphErrors(corr_timing_resolution.size(), fit_amps_V.data(), corr_timing_resolution.data(), fit_amps_err.data(), corr_timing_resolution_err.data());
    // DT_corr_LP2->SetLineColor(2);
    // DT_corr_LP2->SetLineWidth(2);
    DT_corr_LP2 -> GetXaxis() -> SetTitle("Fit %");
    DT_corr_LP2 -> GetYaxis() -> SetTitle("Timing resolution [ns]");
    DT_corr_LP2 -> SetTitle("");
    DT_corr_LP2 -> Draw("APL*");

    DT_corr_CHI -> cd(2);
    auto CHI_corr_LP2 = new TGraph(corr_timing_resolution.size(), fit_amps_V.data(), corr_chi_sq.data());
    // CHI_corr_LP2->SetLineColor(2);
    // CHI_corr_LP2->SetLineWidth(2); 
    CHI_corr_LP2 -> GetXaxis() -> SetTitle("Fit %");
    CHI_corr_LP2 -> GetYaxis() -> SetTitle("Chi^2/NDF");
    CHI_corr_LP2 -> SetTitle("");
    CHI_corr_LP2 -> Draw("APL*");

    DT_corr_CHI -> SaveAs("Corrected_summary.png");

}