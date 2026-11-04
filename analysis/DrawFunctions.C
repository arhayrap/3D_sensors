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

#include <sstream>

int trigger = 3, mcp = 7;

string add_custom_selections(string conditions, int index) {
    std::cout<<legend_labels[index]<<std::endl;
    if (legend_labels[index] == "AC100W40 108V Ru106 ") {
        conditions += "&&amp[7]<400";
    }
    if (legend_labels[index] == "AC100W80 116V Sr90 ") {
        conditions += "&&amp[7]<200";
    }
    // conditions += "&&amp[" + to_string(trigger) + "]<80";
    
    // conditions += "&&amp[3]>12";
    // conditions += "&&amp[" + to_string(trigger) + "]>(amp[" + to_string(trigger - 1) + "]+amp[" + to_string(trigger) + "]+amp[" + to_string(trigger + 1) + "])*" + to_string(amplitude_fraction.at(index));

    // if (k == 0) conditions += "&&LP2_10["+to_string(trigger)+"]<-11"+"&&LP2_80["+to_string(trigger)+"]<-11";
    // if (legend_labels[j] == "Ru106 AC100W40 120V" || legend_labels[j] == "Ru106 AC100W40 116V" || legend_labels[j] == "Ru106 AC100W40 113V") {
    //    conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_80["+to_string(trigger)+"]<-10";
    // }
    // conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_90["+to_string(trigger)+"]<-10";
    /*
    if (legend_labels[k] != "Sr90 AC100W80 116V") {
        conditions += "&&LP2_10[" + to_string(trigger)+"]<-10 && LP2_90["+to_string(trigger)+"]<-10";
    }
    if (legend_labels[k] == "Ru106 AC100W40 105V") {
        conditions += "&&LP2_10[" + to_string(trigger)+"]<-8 && LP2_90["+to_string(trigger)+"]<-8";
    }
    */
    // if (legend_labels[k] == "Ru106 AC100W80 105V") {
    //     conditions += "&&amp["+ to_string(trigger) +"]<80";
    // }

    return conditions;
}

export TF1 * TimewalkCalculation(TH2D * histo2D, TCanvas * canvas) {
    histo2D -> RebinX(2);
    int nPoints = histo2D -> GetXaxis() -> GetNbins();
    std::vector < double > means;
    std::vector < double > amps;
    for (int i = 0; i < nPoints; i++) {
        TH1D * projX = (TH1D * ) histo2D -> ProjectionY("projY", i + 1, i + 2);
        if (projX -> GetMean() == 0.0) continue;
        means.push_back(projX -> GetMean());
        amps.push_back(histo2D -> GetXaxis() -> GetBinCenter(i + 1));
    }
    canvas -> cd(4);
    gPad -> SetLeftMargin(0.20);
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

export void draw_amplitudes(std::vector<TTree*> pulse, string output_folder) {
    TCanvas * amplitudes = new TCanvas("Amplitudes", "Amplitudes", 2100, 1100);
    amplitudes -> Divide(4, 2);
    for (int i = 0; i < 8; i++) {
        amplitudes -> cd(i + 1);
        gPad -> SetLeftMargin(0.20);
        TLegend * legend = new TLegend(0.4, 0.7, 0.9, 0.9);
        std::vector < float > mins, maxes;
        for (int j = 0; j < pulse.size(); j++) {
            maxes.push_back(pulse.at(j) -> GetMaximum(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j)).c_str()));
            mins.push_back(pulse.at(j) -> GetMinimum(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j)).c_str()));
        }
        for (int j = 0; j < pulse.size(); j++) {
            string conditions = base_conditions;
            conditions = add_custom_selections(conditions, j);
            float min = * std::min_element(mins.begin(), mins.end());
            float max = * std::max_element(maxes.begin(), maxes.end());
            if (i == mcp) {
                min = 0;
                max = 800;
            }
            if (j != 0) pulse.at(j) -> Draw(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions.c_str(), "HIST E SAME");
            else        pulse.at(j) -> Draw(("amp[" + to_string(i) + "]>>hist_" + to_string(i) + "_" + to_string(j) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions.c_str(), "HIST E");
            TH1D * hist = (TH1D * ) gDirectory -> Get(("hist_" + to_string(i) + "_" + to_string(j)).c_str());
            hist -> GetXaxis() -> SetRangeUser(0, max);
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
            hist -> Scale(1.0 / pulse.at(j) -> GetEntries(conditions.c_str()));
            hist -> GetYaxis() -> SetRangeUser(0, 0.4);
            // hist -> GetXaxis() -> SetRangeUser(0, 60);
            hist -> SetLineWidth(3);
            if (j == 4) hist -> SetLineColor(94);
            else hist -> SetLineColor(j + 1);
        }
        legend -> Draw("SAME");
        amplitudes -> Update();
    }
    amplitudes -> SaveAs((output_folder + "/Amplitudes_comparison.png").c_str());
}

export void draw_timestamps(std::vector <TTree * > pulse, string output_folder) {
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
                mins.push_back(pulse.at(k) -> GetMinimum(("LP2_" + to_string(fit_amps[i])).c_str()));
            }

            for (int k = 0; k < pulse.size(); k++) {
                string conditions = base_conditions;
                conditions = add_custom_selections(conditions, k);
                
                float min = -12;
                min = * std::min_element(mins.begin(), mins.end());
                float max = -6;
                max = * std::max_element(maxes.begin(), maxes.end());

                if (j != 7) {
                    min = -0.5;
                    max = 0.5;
                } else {
                    min = 3.4;
                    max = 4.2;
                }

                timestamps -> cd(j + 1);
                gPad -> SetLogy(1);

                if (k != 0) pulse.at(k) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(j) + "]>>LP2_" + to_string(fit_amps[i]) + "_hist_" + to_string(j) + "_" + to_string(k) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions.c_str(), "HIST E SAME");
                else pulse.at(k) -> Draw(("LP2_" + to_string(fit_amps[i]) + "[" + to_string(j) + "]>>LP2_" + to_string(fit_amps[i]) + "_hist_" + to_string(j) + "_" + to_string(k) + "(" + to_string(n_bins * rebin) + "," + to_string(min) + "," + to_string(max) + ")").c_str(), conditions.c_str(), "HIST E");

                TH1D * hist = (TH1D * ) gDirectory -> Get(("LP2_" + to_string(fit_amps[i]) + "_hist_" + to_string(j) + "_" + to_string(k)).c_str());
                if (j == mcp) {
                    hist -> SetTitle("MCP");
                } else if (j == trigger) {
                    hist -> SetTitle("Trigger strip");
                } else {
                    hist -> SetTitle(("Channel " + to_string(j + 1)).c_str());
                }
                legend -> AddEntry(hist, legend_labels[k].c_str());
                // hist -> Scale(1.0 / pulse.at(k) -> GetEntries(conditions));
                hist -> SetLineWidth(2);
                if (k != 4) hist -> SetLineColor(k + 1);
                else hist -> SetLineColor(94);
                hist -> GetXaxis() -> SetTitle("Timestamp [ns]");
                hist -> GetYaxis() -> SetTitle("Events");
            }
            // timestamps -> SetLogy();
            legend -> Draw("SAME");
        }
        timestamps -> SaveAs((output_folder + "/Timestamps_LP2_" + to_string(fit_amps[i]) + "_comparison.png").c_str());
    }
}
