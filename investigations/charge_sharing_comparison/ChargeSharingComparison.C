#include <iostream>
#include "TH1D.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"

void ChargeSharingComparison() {
    
    TFile * file1 = new TFile("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W80_strip3mm_bv116_mcp4400_trigCh4_Ch8_Sr90_high_threshold_trigger.root");
    TFile * file2 = new TFile("/Users/PITLab/3D_sensors/Processed_data/proc_F_AC100W80B3_strip3mm_bv160_mcp4400_trigCh4_Sr90_test_run_2.root");
    
    TTree * tree1 = (TTree *) file1 -> Get("pulse");
    TTree * tree2 = (TTree *) file2 -> Get("pulse");
    
    gStyle->SetOptStat(0000);
    
    TCanvas * c = new TCanvas("c", "c", 700*2, 500);
    
    TLegend * l1 = new TLegend(0.6, 0.7, 0.9, 0.9);
        
    c->Divide(2,1);
    c->cd(1);

    string conditions = "baseline_RMS[3]<3";
    
    tree1->Draw("amp[2]/amp[3]>>h1(30,0,1.0)", conditions.c_str(), "histe");
    TH1D * h1 = (TH1D * ) gDirectory->Get("h1");
    h1->Scale(1.0 / tree1->GetEntries(conditions.c_str()));
    TF1 * gaus1 = new TF1("fgaus", "gaus", 0.2, 0.6);
    h1->Fit(gaus1);
    gaus1->SetLineWidth(3);
    gaus1->SetLineColor(4);
    gaus1->SetLineStyle(2);
    // gaus1->Draw("same");    

    tree2->Draw("amp[2]/amp[3]>>h2(30,0,1.0)", conditions.c_str(), "histe same");
    TH1D * h2 = (TH1D * ) gDirectory->Get("h2");
    h2->Scale(1.0 / tree2->GetEntries(conditions.c_str()));
    TF1 * gaus2 = new TF1("fgaus", "gaus", 0.4, 0.8);
    h2->Fit(gaus2);
    gaus2->SetLineWidth(3);
    gaus2->SetLineColor(2);
    gaus2->SetLineStyle(2);
    // gaus2->Draw("same");
    
    h1->SetLineColor(4);
    h2->SetLineColor(2);
    
    h1->SetLineWidth(4);
    h2->SetLineWidth(4);
    
    h1->SetMinimum(0.0);
    h2->SetMinimum(0.0);
    
    h1->SetMaximum(0.3);
    h2->SetMaximum(0.3);
    
    h1->SetTitle("");
    h2->SetTitle("");
    
    h1->GetXaxis()->SetTitle("Amplitude ratio (adjacent / trigger strip, Ch 3 / Ch 4)");
    h1->GetYaxis()->SetTitle("Event fraction");

    float mean = h1->GetXaxis()->GetBinCenter( h1 -> GetMaximumBin() );
    std::cout<<mean<<std::endl;

    l1->AddEntry(h1, Form("W9, BV 116V, %.2f %%", gaus1->GetParameter(1)*100));
    // l1->AddEntry(gaus1, Form("Mean: %.2f %%", gaus1->GetParameter(1)*100) );

    h2->GetXaxis()->SetTitle("Amplitude ratio (adjacent / trigger strip, Ch 3 / Ch 4)");
    h2->GetYaxis()->SetTitle("Event fraction");
    
    mean = h2->GetXaxis()->GetBinCenter( h2 -> GetMaximumBin() );
    std::cout<<mean<<std::endl;

    l1->AddEntry(h2, Form("W3, BV 160V, %.2f %%", gaus2->GetParameter(1)*100) );
    // l1->AddEntry(gaus2, Form("Mean: %.2f %%", gaus2->GetParameter(1)*100) );
    
    l1->Draw("same");
    
    c->cd(2);
    
    TLegend * l2 = new TLegend(0.6, 0.7, 0.9, 0.9);
    
    tree1->Draw("amp[4]/amp[3]>>h3(30,0,1.0)", conditions.c_str(), "histe");
    TH1D * h3 = (TH1D * ) gDirectory->Get("h3");
    h3->Scale(1.0 / tree1->GetEntries(conditions.c_str()));
    
    TF1 * gaus3 = new TF1("fgaus", "gaus", 0.2, 0.6);
    h3->Fit(gaus3);
    gaus3->SetLineWidth(3);
    gaus3->SetLineColor(4);
    gaus3->SetLineStyle(2);

    // gaus3->Draw("same");

    tree2->Draw("amp[4]/amp[3]>>h4(30,0,1.0)", conditions.c_str(), "histe same");
    TH1D * h4 = (TH1D * ) gDirectory->Get("h4");
    h4->Scale(1.0 / tree2->GetEntries(conditions.c_str()));
    
    TF1 * gaus4 = new TF1("fgaus", "gaus", 0.4, 0.8);
    h4->Fit(gaus4);
    gaus4->SetLineWidth(3);
    gaus4->SetLineColor(2);
    gaus4->SetLineStyle(2);
    // gaus4->Draw("same");
    
    h3->SetLineColor(4);
    h4->SetLineColor(2);
    
    h3->SetLineWidth(4);
    h4->SetLineWidth(4);
    
    h3->SetMinimum(0.0);
    h4->SetMinimum(0.0);
    
    h3->SetMaximum(0.3);
    h4->SetMaximum(0.3);
    
    h3->SetTitle("");
    h4->SetTitle("");
    
    h3->GetXaxis()->SetTitle("Amplitude ratio (adjacent / trigger strip, Ch 5 / Ch 4)");
    h3->GetYaxis()->SetTitle("Event fraction");
    
    mean = h3->GetXaxis()->GetBinCenter( h3 -> GetMaximumBin() );
    std::cout<<mean<<std::endl;
    
    l2->AddEntry(h3, Form("W9, BV 116V, %.2f %%", gaus3->GetParameter(1)*100) );
    // l2->AddEntry(gaus3, Form("Mean: %.2f %%", gaus3->GetParameter(1)*100) );
    
    h4->GetXaxis()->SetTitle("Amplitude ratio (adjacent / trigger strip, Ch 5 / Ch 4)");
    h4->GetYaxis()->SetTitle("Event fraction");

    mean = h4->GetXaxis()->GetBinCenter( h4 -> GetMaximumBin() );
    std::cout<<mean<<std::endl;

    l2->AddEntry(h4, Form("W3, BV 160V, %.2f %%", gaus4->GetParameter(1)*100) );
    // l2->AddEntry(gaus4, Form("Mean: %.2f %%", gaus4->GetParameter(1)*100) );
    
    l2->Draw("same");
    
    c->SaveAs("amplitude_ratio.png");
}