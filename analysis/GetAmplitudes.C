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

TF1 * TimewalkCalculation(TH2D * histo2D){
    int nPoints = histo2D->GetXaxis()->GetNbins();
    std::vector<double> means;
    std::vector<double> amps;
    for (int i = 0; i < nPoints; i++) {
        TH1D * projX = (TH1D*)histo2D->ProjectionY("projY", i+1, i+2);
        if (projX->GetMean()==0.0) continue;
        means.push_back(projX->GetMean());
        amps.push_back(histo2D->GetXaxis()->GetBinCenter(i+1));
        // std::cout << "===================> " << means[i] << std::endl;
    }
    TCanvas * timewalk_canvas = new TCanvas("timewalk", "timewalk", 1000, 500);
    timewalk_canvas->Divide(2,1);
    timewalk_canvas->cd(1);
    histo2D->Draw("colz");
    timewalk_canvas->cd(2);
    auto timewalk_curve = new TGraph(means.size(), amps.data(), means.data());
    timewalk_curve->Draw("ACP*");
    timewalk_curve->SetTitle("Means and timewalk fit.");
    TF1 * polyline = new TF1("polyline","[0]+[1]*x+[2]*x^2+[3]*x^3"); // +[4]*x^4+[5]*x^5");
    timewalk_curve->Fit(polyline);
    polyline->SetLineColor(2);
    polyline->SetLineWidth(2);
    polyline->Draw("samel");
    return polyline;
}

void GetAmplitudes() {
    // TFile * file = TFile::Open("/home/aram/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    TFile * file = TFile::Open("/Users/PITLab/3D_sensors/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_collimatedTwice_3days.root");
    TTree * pulse = (TTree*) file -> Get("pulse");
    TString conditions = "baseline_RMS[3]<3 && amp[3]>amp[0] && amp[3]>amp[1] && amp[3]>amp[2] && amp[3]>amp[4] && amp[3]>amp[5] && amp[3]>amp[6] && amp[3]>0 && amp[7]<130 && amp[7]>20"; 
    // && LP2_50[2]<-1.0 && LP2_50[4]<-1.0 && LP2_50[5]<-1.0 && LP2_50[1]<-1.0";
    int trigger = 3, mcp = 7;
    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1011);
    // Amplitudes
    /*
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
    }*/
    
    // Timestamps
    int fit_amps[] = {10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80}; 
    std::vector<double> fit_amps_V = {10.0, 15.0, 20.0, 25.0, 30.0, 35.0, 40.0, 50.0, 60.0, 70.0, 80.0};
    /*
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
    }*/

    std::vector<double> timing_resolution;
    std::vector<double> corr_timing_resolution;
    std::vector<double> timing_resolution_err;
    std::vector<double> corr_timing_resolution_err;
    std::vector<double> chi_sq;
    std::vector<double> corr_chi_sq;
    std::vector<double> fit_amps_err;

    // Delta T
    for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++){
        // for (int i = 0; i < sizeof(fit_amps)/sizeof(fit_amps[0]); i++){
        TCanvas * deltaT = new TCanvas(("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), ("Delta T (LP2_" + to_string(fit_amps[i]) + ")").c_str(), 1500, 1000);
        deltaT->Divide(3,2);
        // for (int j = 0; j < 8; j++) {

        deltaT->cd(1);
        pulse->Draw(("LP2_" + to_string(fit_amps[i]) + "[3]-LP2_"+ to_string(fit_amps[i]) + "[7]>>DT_" + to_string(fit_amps[i]) + "_hist(20,-4.3,-3.85)").c_str(), conditions, "HIST E");
        TH1D * DT = (TH1D*)gDirectory->Get(("DT_" + to_string(fit_amps[i]) + "_hist").c_str());
        // DT->Rebin(4);
        TF1 * fgaus = new TF1("fgaus","gaus",-4.2,-4.0);
        std::cout<<DT->GetMaximum()<<std::endl;
        fgaus->SetParameters(DT->GetMaximum(), DT->GetMean(), DT->GetRMS());
        DT->Fit(fgaus, "R");
        fgaus->SetLineWidth(2);
        fgaus->SetLineColor(2);
        fgaus->Draw("same l");
        DT->SetTitle("DeltaT");
        fit_amps_err.push_back(0.0);
        timing_resolution.push_back(fgaus->GetParameter(2)*1000);
        timing_resolution_err.push_back(fgaus->GetParError(2)*1000);
        std::cout<<"------------> "<<fgaus->GetChisquare()/fgaus->GetNDF()<<std::endl;
        if (fgaus->GetNDF()!=0)
            chi_sq.push_back(fgaus->GetChisquare()/fgaus->GetNDF());
        else 
            chi_sq.push_back(0.0);

        deltaT->cd(2);
        pulse->Draw(("LP2_" + to_string(fit_amps[i]) + "[3]-LP2_"+ to_string(fit_amps[i]) + "[7]:amp[3]>>DT_amp3_" + to_string(fit_amps[i]) + "_hist(30,10,70,30,-4.3,-3.8)").c_str(), conditions, "colz");
        TH2D*DT_amp3 = (TH2D*)gDirectory->Get(("DT_amp3_" + to_string(fit_amps[i]) + "_hist").c_str());
        TF1 * polyline = TimewalkCalculation(DT_amp3); // polyline <===============================================================================>
        double p0 = polyline->GetParameter(0);
        double p1 = polyline->GetParameter(1);
        double p2 = polyline->GetParameter(2);
        double p3 = polyline->GetParameter(3);
        std::cout << ">>>>>>>>>>>>>>>>> " << polyline->Eval(10.0) << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>> " << polyline->Eval(20.0) << std::endl;
        std::cout << ">>>>>>>>>>>>>>>>> " << polyline->Eval(30.0) << std::endl;

        std::cout<<"-------------------------------------------->>>> "<<Form("(%g+%g*amp[3]+%g*amp[3]*amp[3]+%g*amp[3]*amp[3]*amp[3])", p0, p1, p2, p3)<<std::endl;
        DT_amp3->SetTitle("DeltaT vs trigger strip amplitude"); // strigger

        deltaT->cd(3);
        string dt_command = "LP2_" + to_string(fit_amps[i]) + "[3]-LP2_"+ to_string(fit_amps[i]) + "[7]:amp[7]>>DT_amp7_" + to_string(fit_amps[i]) + "_hist(30,10,140,30,-4.3,-3.8)";
        std::cout<<dt_command<<std::endl;
        pulse->Draw((dt_command).c_str(), conditions, "colz");
        TH2D * DT_amp7 = (TH2D*)gDirectory->Get(("DT_amp7_" + to_string(fit_amps[i]) + "_hist").c_str());
        DT_amp7->SetTitle("DeltaT vs MCP amplitude");
        deltaT->SaveAs(("DeltaT_LP2_" + to_string(fit_amps[i]) + ".png").c_str());
        // }

        // ---------------------------- Timewalk corrected --------------------------------
        deltaT->cd(4);
        gStyle->SetOptFit(1011);
	string corrected_dt_command = "LP2_" + to_string(fit_amps[i]) + "[3] - LP2_"+ to_string(fit_amps[i]) + "[7] - "+Form("(%g + %g * amp[3] + %g*amp[3] * amp[3] + %g * amp[3] * amp[3] * amp[3])", p0, p1, p2, p3)+">>DT_" + to_string(fit_amps[i]) + "_hist_corr";
        std::cout<<"--------------------------------------------++++++++++>>>> "<<corrected_dt_command<<std::endl;
        pulse->Draw((corrected_dt_command).c_str(), conditions, "HIST E");
        TH1D * DT_corr = (TH1D*)gDirectory->Get(("DT_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
        // DT_corr->Rebin(4);
        TF1 * fgaus_corr = new TF1("fgaus_corr","gaus",-1.0,1.0);
        std::cout<<DT_corr->GetMaximum()<<std::endl;
        fgaus_corr->SetParameters(DT_corr->GetMaximum(), DT_corr->GetMean(), DT_corr->GetRMS());
        DT->Fit(fgaus_corr, "R");
        fgaus_corr->SetLineWidth(2);
        fgaus_corr->SetLineColor(2);
        fgaus_corr->Draw("same l");
        DT->SetTitle("DeltaT");
        fit_amps_err.push_back(0.0);
        timing_resolution.push_back(fgaus_corr->GetParameter(2)*1000);
        timing_resolution_err.push_back(fgaus_corr->GetParError(2)*1000);
        std::cout<<"------------> "<<fgaus_corr->GetChisquare()/fgaus_corr->GetNDF()<<std::endl;
        if (fgaus_corr->GetNDF()!=0)
            chi_sq.push_back(fgaus_corr->GetChisquare()/fgaus_corr->GetNDF());
        else
            chi_sq.push_back(0.0);
        
        deltaT->cd(5);
        string corrected_dt_vs_amp_command = "LP2_" + to_string(fit_amps[i]) + "[3] - LP2_"+ to_string(fit_amps[i]) + "[7] - "+Form("(%g + %g * amp[3] + %g * amp[3] * amp[3] + %g * amp[3] * amp[3] * amp[3])", p0, p1, p2, p3)+":amp[3]>>DT_amp3_" + to_string(fit_amps[i]) + "_hist_corr";
        std::cout<<"--------------------------------------------++++++++++>>>> "<<corrected_dt_vs_amp_command<<std::endl;
        pulse->Draw((corrected_dt_vs_amp_command).c_str(), conditions, "colz");
        TH2D*DT_amp3_corr = (TH2D*)gDirectory->Get(("DT_amp3_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
        // TF1 * fited_curve = TimewalkCalculation(DT_amp3); // polyline
        DT_amp3_corr->SetTitle("DeltaT vs trigger strip amplitude"); // strigger
        
        deltaT->cd(6);
        string corrected_dt_vs_MCP_amp_command = "LP2_" + to_string(fit_amps[i]) + "[3]-LP2_"+ to_string(fit_amps[i]) + "[7] - "+ Form("(%g + %g * amp[3] + %g * amp[3] * amp[3] + %g * amp[3] * amp[3] * amp[3])", p0, p1, p2, p3) + ":amp[7]>>DT_amp7_" + to_string(fit_amps[i]) + "_hist_corr";
        std::cout<<corrected_dt_vs_MCP_amp_command<<std::endl;
        pulse->Draw((corrected_dt_vs_MCP_amp_command).c_str(), conditions, "colz");
        TH2D * DT_amp7_corr = (TH2D*)gDirectory->Get(("DT_amp7_" + to_string(fit_amps[i]) + "_hist_corr").c_str());
        DT_amp7_corr->SetTitle("DeltaT vs MCP amplitude");

        deltaT->SaveAs(("DeltaT_LP2_" + to_string(fit_amps[i]) + ".png").c_str());
        
    }

    // Timing resolution vs amplitude percentage and chi squared
    auto DT_LP2 = new TGraphErrors(timing_resolution.size(), fit_amps_V.data(), timing_resolution.data(), fit_amps_err.data(), timing_resolution_err.data());
    auto CHI_LP2 = new TGraph(timing_resolution.size(), fit_amps_V.data(), chi_sq.data());

    TCanvas * DT_CHI = new TCanvas("DT_CHI", "DT_CHI", 1000, 500);
    DT_CHI->Divide(2,1);

    DT_CHI->cd(1);
    // DT_LP2->SetLineColor(2);
    // DT_LP2->SetLineWidth(2);
    DT_LP2->SetTitle("Timing resolution");
    DT_LP2->Draw("APL*");

    DT_CHI->cd(2);
    // CHI_LP2->SetLineColor(2);
    // CHI_LP2->SetLineWidth(2); 
    CHI_LP2->SetTitle("Chi^2/NDF");
    CHI_LP2->Draw("APL*");

    DT_CHI->SaveAs("Summary.png");

}
