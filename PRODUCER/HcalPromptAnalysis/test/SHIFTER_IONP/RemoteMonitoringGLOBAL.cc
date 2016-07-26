// How to run:
//root -b -q -l RemoteMonitoringGLOBAL.C+
//root -b -q -l 'RemoteMonitoringGLOBAL.C+("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/CMTweb/histos/Global_190707.root")'

#include "LogEleMapdb.h"

#include <iostream>
#include <fstream>

#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TROOT.h"
#include <TMath.h>
#include "TStyle.h"
#include "TSystem.h"
#include "TLegend.h"
#include "TText.h"
#include "TAxis.h"
#include "TFile.h"
#include "TLine.h"
#include "TGraph.h"
#include <THStack.h>
#include <TPaveText.h>

using namespace std;
//inline void HERE(const char *msg) { std::cout << msg << std::endl; }
int copyContents(TH1F **hDest, TString hname, TString htitle,
		 const TH1F *hSrc, int lastBin);

// -----------------------------------------------------   

int main(int argc, char *argv[])
{

    gROOT->Reset();
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(1);

    if(argc<1) return 1;
    char fname[300];
    sprintf(fname,"%s",argv[1]);
    std::cout<<fname<<std::endl;


//======================================================================
// Connect the input files, parameters and get the 2-d histogram in memory
//    TFile *hfile= new TFile("GlobalHist.root", "READ");
    string promt = (string) fname;
    string runnumber ="";
    for (unsigned int i=promt.size()-11; i<promt.size()-5 ; i++) runnumber += fname[i];
    
    TFile *hfile= new TFile( fname, "READ");
    // Cut [test][sub][depth]
    double Cut0[7][5][5]={{{0.,0.,0.,0.,0.}, {0.,1.,1.0,0.,0.},  {0.,1.,1.,1.,0.},   {0.,0.,0.,0.,1.}, {0.,1.,1.,0.,0.}},  //CapID
                          {{0.,0.,0.,0.,0.}, {0.,11.,13.,0.,0.}, {0.,25.,6.,30.,0.}, {0.,0.,0.,0.,28.}, {0.,80.,40.,0.,0.}},  //Amplitude   
			  {{0.,0.,0.,0.,0.}, {0.,3.,3.,0.,0.},   {0.,3.,3.,3.,0.},   {0.,0.,0.,0.,3.}, {0.,2.,2.,0.,0.}}, //Width
			  {{0.,0.,0.,0.,0.}, {0.,0.4,0.4,0.,0.}, {0.,0.4,0.4,0.4,0.},{0.,0.,0.,0.,0.4},{0.,0.8,0.8,0.,0.}}, //Ratio
			  {{0.,0.,0.,0.,0.}, {0.,4.7,4.7,0.,0.}, {0.,4.8,4.8,5.0,0.},{0.,0.,0.,0.,4.8},{0.,4.0,4.0,0.,0.}}, //TSn
			  {{0.,0.,0.,0.,0.}, {0.,3.5,3.5,0.,0.}, {0.,4.0,4.0,4.0,0.},{0.,0.,0.,0.,3.},{0.,3.5,3.5,0.,0.}}, //TSx
			  {{0.,0.,0.,0.,0.},   {0.,0.,0.,0.,0.},    {0.,0.,0.,0.,0.},{0.,0.,0.,0.,0.},{0.,0.,0.,0.,0.}}}; //Empty
    double CutAb[5][5]={{0.,0.,0.,0.,0.},{0.,20.,7.,0.,0.},{0.,16.,13.,4.,0.},{0.,0.,0.,0.,45.},{0.,10.,5.,0.,0.}}; // cut 1
    double CutPo[5][5]={{0.,0.,0.,0.,0.},{0.,9.,3.,0.,0.},{0.,8.,6.,2.,0.},{0.,0.,0.,0.,20.},{0.,5.,3.,0.,0.}}; //cut 3
//======================================================================



//======================================================================
// Prepare histograms and plot them to .png files

 
  TCanvas *cHB = new TCanvas("cHB","cHB",1000,500);
  TCanvas *cHE = new TCanvas("cHE","cHE",1500,500);
  TCanvas *cONE = new TCanvas("cONE","cONE",500,500);
  
//  char *str = (char*)alloca(10000);
  
  int k_min[5]={0,1,1,4,1}; // minimum depth for each subdet
  int k_max[5]={0,2,3,4,2}; // maximum depth for each subdet	

//+++++++++++++++++++++++++++++  
// Lumi iLumi and number of events  
//+++++++++++++++++++++++++++++ 
      cHB->Divide(2,1);
      cHB->cd(1);
      TH1F *LumLum= (TH1F*)hfile->Get("h_lsnumber_per_eachLS");

      int MaxLum= LumLum->GetBinContent(LumLum->GetMaximumBin());
      gPad->SetGridy();
      gPad->SetGridx();
      LumLum->SetMarkerStyle(10);
      LumLum->SetMarkerSize(0.8);
      LumLum->GetYaxis()->SetLabelSize(0.04);
      LumLum->SetTitle("Cont. number per LS  \b");
      LumLum->SetXTitle("Cont.number \b");
      LumLum->SetYTitle("Ls \b");
      LumLum->SetMarkerColor(4);
      LumLum->SetLineColor(0);
      LumLum->SetMinimum(0.8);
      LumLum->GetXaxis()->SetRangeUser(0, MaxLum);
      LumLum->Draw("Error");

      cHB->cd(2);
      TH1F *LumiEv= (TH1F*)hfile->Get("h_nevents_per_eachRealLS");
      gPad->SetGridy();
      gPad->SetGridx();
      gPad->SetLogy();
      //            gPad->SetLogx();
      LumiEv->GetYaxis()->SetLabelSize(0.04);
      LumiEv->SetTitle("Number of events per LS");
      LumiEv->SetXTitle("LS");
      LumiEv->SetYTitle("Number of events ");
      LumiEv->SetMarkerStyle(10);
      LumiEv->SetMarkerSize(0.8);
      LumiEv->SetMarkerColor(4);
      LumiEv->SetLineColor(0);
//      LumiEv->SetMinimum(0.8);
      LumiEv->GetXaxis()->SetRangeUser(0, MaxLum);
      LumiEv->Draw("Error");

      cHB->Print("LumiEvent.png"); cHB->Clear();




//=======================================================================================================
// Shape criteria: 1d histogram first definition
//=======================================================================================================
      TH1F *H_NumBadChanDepth[7][5][5];    // 1d histogramm for test,subdet, depth 
     
 //+++++++++++++++++++++++++++++  
// ADC Amplitude   
//+++++++++++++++++++++++++++++  
 
      H_NumBadChanDepth[1][1][1] = (TH1F*)hfile->Get("h_sumADCAmplLS1");
      H_NumBadChanDepth[1][1][2] = (TH1F*)hfile->Get("h_sumADCAmplLS2");
 
      H_NumBadChanDepth[1][2][1] = (TH1F*)hfile->Get("h_sumADCAmplLS3");
      H_NumBadChanDepth[1][2][2] = (TH1F*)hfile->Get("h_sumADCAmplLS4"); 
      H_NumBadChanDepth[1][2][3] = (TH1F*)hfile->Get("h_sumADCAmplLS5");
      
      H_NumBadChanDepth[1][3][4] = (TH1F*)hfile->Get("h_sumADCAmplLS8");
      
      H_NumBadChanDepth[1][4][1] = (TH1F*)hfile->Get("h_sumADCAmplLS6");
      H_NumBadChanDepth[1][4][2] = (TH1F*)hfile->Get("h_sumADCAmplLS7"); 
         
//+++++++++++++++++++++++++++++  
// Width  
//+++++++++++++++++++++++++++++  
 
      H_NumBadChanDepth[2][1][1] = (TH1F*)hfile->Get("h_sumAmplitudeLS1");
      H_NumBadChanDepth[2][1][2] = (TH1F*)hfile->Get("h_sumAmplitudeLS2");
 
      H_NumBadChanDepth[2][2][1] = (TH1F*)hfile->Get("h_sumAmplitudeLS3");
      H_NumBadChanDepth[2][2][2] = (TH1F*)hfile->Get("h_sumAmplitudeLS4"); 
      H_NumBadChanDepth[2][2][3] = (TH1F*)hfile->Get("h_sumAmplitudeLS5");
      
      H_NumBadChanDepth[2][3][4] = (TH1F*)hfile->Get("h_sumAmplitudeLS8");
      
      H_NumBadChanDepth[2][4][1] = (TH1F*)hfile->Get("h_sumAmplitudeLS6");
      H_NumBadChanDepth[2][4][2] = (TH1F*)hfile->Get("h_sumAmplitudeLS7");        
      
//+++++++++++++++++++++++++++++  
// Ratio   
//+++++++++++++++++++++++++++++   
 
      H_NumBadChanDepth[3][1][1] = (TH1F*)hfile->Get("h_sumAmplLS1");
      H_NumBadChanDepth[3][1][2] = (TH1F*)hfile->Get("h_sumAmplLS2");
 
      H_NumBadChanDepth[3][2][1] = (TH1F*)hfile->Get("h_sumAmplLS3");
      H_NumBadChanDepth[3][2][2] = (TH1F*)hfile->Get("h_sumAmplLS4"); 
      H_NumBadChanDepth[3][2][3] = (TH1F*)hfile->Get("h_sumAmplLS5");
      
      H_NumBadChanDepth[3][3][4] = (TH1F*)hfile->Get("h_sumAmplLS8");
      
      H_NumBadChanDepth[3][4][1] = (TH1F*)hfile->Get("h_sumAmplLS6");
      H_NumBadChanDepth[3][4][2] = (TH1F*)hfile->Get("h_sumAmplLS7"); 
      
//+++++++++++++++++++++++++++++  
// Tmean   
//+++++++++++++++++++++++++++++   
 
      H_NumBadChanDepth[4][1][1] = (TH1F*)hfile->Get("h_sumTSmeanALS1");
      H_NumBadChanDepth[4][1][2] = (TH1F*)hfile->Get("h_sumTSmeanALS2");
 
      H_NumBadChanDepth[4][2][1] = (TH1F*)hfile->Get("h_sumTSmeanALS3");
      H_NumBadChanDepth[4][2][2] = (TH1F*)hfile->Get("h_sumTSmeanALS4"); 
      H_NumBadChanDepth[4][2][3] = (TH1F*)hfile->Get("h_sumTSmeanALS5");
      
      H_NumBadChanDepth[4][3][4] = (TH1F*)hfile->Get("h_sumTSmeanALS8");
     
      H_NumBadChanDepth[4][4][1] = (TH1F*)hfile->Get("h_sumTSmeanALS6");
      H_NumBadChanDepth[4][4][2] = (TH1F*)hfile->Get("h_sumTSmeanALS7"); 
             
//+++++++++++++++++++++++++++++  
// Tmax   
//+++++++++++++++++++++++++++++   
 
      H_NumBadChanDepth[5][1][1] = (TH1F*)hfile->Get("h_sumTSmaxALS1");
      H_NumBadChanDepth[5][1][2] = (TH1F*)hfile->Get("h_sumTSmaxALS2");
 
      H_NumBadChanDepth[5][2][1] = (TH1F*)hfile->Get("h_sumTSmaxALS3");
      H_NumBadChanDepth[5][2][2] = (TH1F*)hfile->Get("h_sumTSmaxALS4"); 
      H_NumBadChanDepth[5][2][3] = (TH1F*)hfile->Get("h_sumTSmaxALS5");
      
      H_NumBadChanDepth[5][3][4] = (TH1F*)hfile->Get("h_sumTSmaxALS8");
      
      H_NumBadChanDepth[5][4][1] = (TH1F*)hfile->Get("h_sumTSmaxALS6");
      H_NumBadChanDepth[5][4][2] = (TH1F*)hfile->Get("h_sumTSmaxALS7");

    gStyle->SetOptStat(110000);                     
                            
    for (int test=1;test<=5;test++) { //Test: 0, 
        for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
            if (sub==1) cHB->Divide(2,1);
            if (sub==2) cHE->Divide(3,1);
            if (sub==3) cONE->Divide(1,1);
            if (sub==4) cHB->Divide(2,1);
            for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
                if (sub==1) cHB->cd(k); 
                if (sub==2) cHE->cd(k);
	        if (sub==3) cONE->cd(k-3);
	        if (sub==4) cHB->cd(k);   
		gPad->SetGridy();
                gPad->SetGridx();
                gPad->SetLogy();   
                H_NumBadChanDepth[test][sub][k]->SetMarkerStyle(20);
                H_NumBadChanDepth[test][sub][k]->SetMarkerSize(0.8);
                if (k == 1) H_NumBadChanDepth[test][sub][k]->SetTitle("Depth 1\b");
		if (k == 2) H_NumBadChanDepth[test][sub][k]->SetTitle("Depth 2\b");
		if (k == 3) H_NumBadChanDepth[test][sub][k]->SetTitle("Depth 3\b");
		if (k == 4) H_NumBadChanDepth[test][sub][k]->SetTitle("Depth 4\b");
		if (test==1) H_NumBadChanDepth[test][sub][k]->SetXTitle(" <Amplitude> \b");
		if (test==2) H_NumBadChanDepth[test][sub][k]->SetXTitle(" <Width> \b");
		if (test==3) H_NumBadChanDepth[test][sub][k]->SetXTitle(" <Ratio> \b");
		if (test==4) H_NumBadChanDepth[test][sub][k]->SetXTitle(" <TS mean> \b");
		if (test==5) H_NumBadChanDepth[test][sub][k]->SetXTitle(" <TS max> \b");
		H_NumBadChanDepth[test][sub][k]->SetYTitle("Number of channel-LSs\b");
		H_NumBadChanDepth[test][sub][k]->SetMarkerColor(4);
                H_NumBadChanDepth[test][sub][k]->SetLineColor(0);
                H_NumBadChanDepth[test][sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
		H_NumBadChanDepth[test][sub][k]->Draw("Error");
		  	   
	    }    
	    if (test==0){ 
	        if (sub==1) {cHB->Print("H_NBCMNHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("H_NBCMNHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("H_NBCMNHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("H_NBCMNHF.png"); cHB->Clear();}  
	    } 
	    
	    if (test==1){ 
	        if (sub==1) {cHB->Print("H_ADCamplHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("H_ADCamplHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("H_ADCamplHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("H_ADCamplHF.png"); cHB->Clear();}  
	    } 
	    if (test==2){ 
	        if (sub==1) {cHB->Print("H_WidthHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("H_WidthHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("H_WidthHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("H_WidthHF.png"); cHB->Clear();}  
	    } 
	    if (test==3){ 
	        if (sub==1) {cHB->Print("H_RatioHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("H_RatioHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("H_RatioHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("H_RatioHF.png"); cHB->Clear();}  
	    } 
	    if (test==4){ 
	        if (sub==1) {cHB->Print("H_TmeanHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("H_TmeanHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("H_TmeanHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("H_TmeanHF.png"); cHB->Clear();}  
	    } 
	    if (test==5){ 
	        if (sub==1) {cHB->Print("H_TmaxHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("H_TmaxHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("H_TmaxHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("H_TmaxHF.png"); cHB->Clear();}  
	    } 	    	   	                  
         }// end sub 
     }//end test	          
      
    gStyle->SetOptStat(0);
//================================================================================================================================ 
 
 
//=======================================================================================================
// 2-d histograms second definition
//=========================================================
      TH2F *MapNumBadChanDepth[7][5][5];    // 1d Mapogramm for test,subdet, depth 
      TH2F *MapNumChanDepth[7][5][5];    // 1d Mapogramm for test,subdet, depth   
      TH2F *MapNumBadChanFull[7];    // 1d Mapogramm for test
      TH2F *MapNumChanFull[7];    // 1d Mapogramm for test  

//+++++++++++++++++++++++++++++  
// CapID    
//+++++++++++++++++++++++++++++   
 
 
      MapNumBadChanDepth[0][1][1] = (TH2F*)hfile->Get("h_mapDepth1Error_HB");
      MapNumBadChanDepth[0][1][2] = (TH2F*)hfile->Get("h_mapDepth2Error_HB");
 
      MapNumBadChanDepth[0][2][1] = (TH2F*)hfile->Get("h_mapDepth1Error_HE");
      MapNumBadChanDepth[0][2][2] = (TH2F*)hfile->Get("h_mapDepth2Error_HE"); 
      MapNumBadChanDepth[0][2][3] = (TH2F*)hfile->Get("h_mapDepth3Error_HE");
      
      MapNumBadChanDepth[0][3][4] = (TH2F*)hfile->Get("h_mapDepth4Error_HO");
      
      MapNumBadChanDepth[0][4][1] = (TH2F*)hfile->Get("h_mapDepth1Error_HF");
      MapNumBadChanDepth[0][4][2] = (TH2F*)hfile->Get("h_mapDepth2Error_HF");  
     
      MapNumBadChanFull[0] = (TH2F*) MapNumBadChanDepth[0][1][1]->Clone();  
      
      
      MapNumChanDepth[0][1][1] = (TH2F*)hfile->Get("h_mapDepth1_HB");     
      MapNumChanDepth[0][1][2] = (TH2F*)hfile->Get("h_mapDepth2_HB");                                               
 
      MapNumChanDepth[0][2][1] = (TH2F*)hfile->Get("h_mapDepth1_HE");     
      MapNumChanDepth[0][2][2] = (TH2F*)hfile->Get("h_mapDepth2_HE"); 
      MapNumChanDepth[0][2][3] = (TH2F*)hfile->Get("h_mapDepth3_HE");     
      
      MapNumChanDepth[0][3][4] = (TH2F*)hfile->Get("h_mapDepth4_HO"); 
      
      MapNumChanDepth[0][4][1] = (TH2F*)hfile->Get("h_mapDepth1_HF");     
      MapNumChanDepth[0][4][2] = (TH2F*)hfile->Get("h_mapDepth2_HF");
      
      MapNumChanFull[0] = (TH2F*) MapNumChanDepth[0][1][1]->Clone();

      
 //+++++++++++++++++++++++++++++  
// ADC Amplitude   
//+++++++++++++++++++++++++++++   
 
 
      MapNumBadChanDepth[1][1][1] = (TH2F*)hfile->Get("h_2DsumADCAmplLS1");
      MapNumBadChanDepth[1][1][2] = (TH2F*)hfile->Get("h_2DsumADCAmplLS2");
 
      MapNumBadChanDepth[1][2][1] = (TH2F*)hfile->Get("h_2DsumADCAmplLS3");
      MapNumBadChanDepth[1][2][2] = (TH2F*)hfile->Get("h_2DsumADCAmplLS4"); 
      MapNumBadChanDepth[1][2][3] = (TH2F*)hfile->Get("h_2DsumADCAmplLS5");
      
      MapNumBadChanDepth[1][3][4] = (TH2F*)hfile->Get("h_2DsumADCAmplLS8");
      
      MapNumBadChanDepth[1][4][1] = (TH2F*)hfile->Get("h_2DsumADCAmplLS6");
      MapNumBadChanDepth[1][4][2] = (TH2F*)hfile->Get("h_2DsumADCAmplLS7");  
     
      MapNumBadChanFull[1] = (TH2F*) MapNumBadChanDepth[1][1][1]->Clone();  
      
      
      MapNumChanDepth[1][1][1] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS1");     
      MapNumChanDepth[1][1][2] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS2");                                               
 
      MapNumChanDepth[1][2][1] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS3");     
      MapNumChanDepth[1][2][2] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS4"); 
      MapNumChanDepth[1][2][3] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS5");     
      
      MapNumChanDepth[1][3][4] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS8"); 
      
      MapNumChanDepth[1][4][1] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS6");     
      MapNumChanDepth[1][4][2] = (TH2F*)hfile->Get("h_2D0sumADCAmplLS7");
      
      MapNumChanFull[1] = (TH2F*) MapNumChanDepth[1][1][1]->Clone();
           
//+++++++++++++++++++++++++++++  
// Width  
//+++++++++++++++++++++++++++++   
 
      MapNumBadChanDepth[2][1][1] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS1");
      MapNumBadChanDepth[2][1][2] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS2");
 
      MapNumBadChanDepth[2][2][1] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS3");
      MapNumBadChanDepth[2][2][2] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS4"); 
      MapNumBadChanDepth[2][2][3] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS5");
      
      MapNumBadChanDepth[2][3][4] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS8");
      
      MapNumBadChanDepth[2][4][1] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS6");
      MapNumBadChanDepth[2][4][2] = (TH2F*)hfile->Get("h_2DsumAmplitudeLS7");    
      
      MapNumBadChanFull[2] = (TH2F*) MapNumBadChanDepth[2][1][1]->Clone(); 
      
      MapNumChanDepth[2][1][1] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS1");     
      MapNumChanDepth[2][1][2] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS2"); 
 
      MapNumChanDepth[2][2][1] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS3");     
      MapNumChanDepth[2][2][2] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS4"); 
      MapNumChanDepth[2][2][3] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS5");     
      
      MapNumChanDepth[2][3][4] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS8"); 
      
      MapNumChanDepth[2][4][1] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS6");     
      MapNumChanDepth[2][4][2] = (TH2F*)hfile->Get("h_2D0sumAmplitudeLS7");
      
      MapNumChanFull[2] = (TH2F*) MapNumChanDepth[2][1][1]->Clone();
      
//+++++++++++++++++++++++++++++  
// Ratio   
//+++++++++++++++++++++++++++++  
 
      MapNumBadChanDepth[3][1][1] = (TH2F*)hfile->Get("h_2DsumAmplLS1");
      MapNumBadChanDepth[3][1][2] = (TH2F*)hfile->Get("h_2DsumAmplLS2");
 
      MapNumBadChanDepth[3][2][1] = (TH2F*)hfile->Get("h_2DsumAmplLS3");
      MapNumBadChanDepth[3][2][2] = (TH2F*)hfile->Get("h_2DsumAmplLS4"); 
      MapNumBadChanDepth[3][2][3] = (TH2F*)hfile->Get("h_2DsumAmplLS5");
      
      MapNumBadChanDepth[3][3][4] = (TH2F*)hfile->Get("h_2DsumAmplLS8");
      
      MapNumBadChanDepth[3][4][1] = (TH2F*)hfile->Get("h_2DsumAmplLS6");
      MapNumBadChanDepth[3][4][2] = (TH2F*)hfile->Get("h_2DsumAmplLS7"); 
      
      MapNumBadChanFull[3] = (TH2F*) MapNumBadChanDepth[3][1][1]->Clone(); 
      
      
      MapNumChanDepth[3][1][1] = (TH2F*)hfile->Get("h_2D0sumAmplLS1");     
      MapNumChanDepth[3][1][2] = (TH2F*)hfile->Get("h_2D0sumAmplLS2"); 
 
      MapNumChanDepth[3][2][1] = (TH2F*)hfile->Get("h_2D0sumAmplLS3");     
      MapNumChanDepth[3][2][2] = (TH2F*)hfile->Get("h_2D0sumAmplLS4"); 
      MapNumChanDepth[3][2][3] = (TH2F*)hfile->Get("h_2D0sumAmplLS5");     
      
      MapNumChanDepth[3][3][4] = (TH2F*)hfile->Get("h_2D0sumAmplLS8"); 
      
      MapNumChanDepth[3][4][1] = (TH2F*)hfile->Get("h_2D0sumAmplLS6");     
      MapNumChanDepth[3][4][2] = (TH2F*)hfile->Get("h_2D0sumAmplLS7");
      
      MapNumChanFull[3] = (TH2F*) MapNumChanDepth[3][1][1]->Clone();
      
//+++++++++++++++++++++++++++++  
// Tmean   
//+++++++++++++++++++++++++++++ 
  
      MapNumBadChanDepth[4][1][1] = (TH2F*)hfile->Get("h_2DsumTSmeanALS1");
      MapNumBadChanDepth[4][1][2] = (TH2F*)hfile->Get("h_2DsumTSmeanALS2");
 
      MapNumBadChanDepth[4][2][1] = (TH2F*)hfile->Get("h_2DsumTSmeanALS3");
      MapNumBadChanDepth[4][2][2] = (TH2F*)hfile->Get("h_2DsumTSmeanALS4"); 
      MapNumBadChanDepth[4][2][3] = (TH2F*)hfile->Get("h_2DsumTSmeanALS5");
      
      MapNumBadChanDepth[4][3][4] = (TH2F*)hfile->Get("h_2DsumTSmeanALS8");
     
      MapNumBadChanDepth[4][4][1] = (TH2F*)hfile->Get("h_2DsumTSmeanALS6");
      MapNumBadChanDepth[4][4][2] = (TH2F*)hfile->Get("h_2DsumTSmeanALS7");
      
      MapNumBadChanFull[4] = (TH2F*) MapNumBadChanDepth[4][1][1]->Clone();  
      
      
      MapNumChanDepth[4][1][1] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS1");     
      MapNumChanDepth[4][1][2] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS2"); 
 
      MapNumChanDepth[4][2][1] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS3");     
      MapNumChanDepth[4][2][2] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS4"); 
      MapNumChanDepth[4][2][3] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS5");     
      
      MapNumChanDepth[4][3][4] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS8"); 
      
      MapNumChanDepth[4][4][1] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS6");     
      MapNumChanDepth[4][4][2] = (TH2F*)hfile->Get("h_2D0sumTSmeanALS7");
      
      MapNumChanFull[4] = (TH2F*) MapNumChanDepth[4][1][1]->Clone(); 
      
//+++++++++++++++++++++++++++++  
// Tmax   
//+++++++++++++++++++++++++++++   
 
      MapNumBadChanDepth[5][1][1] = (TH2F*)hfile->Get("h_2DsumTSmaxALS1");
      MapNumBadChanDepth[5][1][2] = (TH2F*)hfile->Get("h_2DsumTSmaxALS2");
 
      MapNumBadChanDepth[5][2][1] = (TH2F*)hfile->Get("h_2DsumTSmaxALS3");
      MapNumBadChanDepth[5][2][2] = (TH2F*)hfile->Get("h_2DsumTSmaxALS4"); 
      MapNumBadChanDepth[5][2][3] = (TH2F*)hfile->Get("h_2DsumTSmaxALS5");
      
      MapNumBadChanDepth[5][3][4] = (TH2F*)hfile->Get("h_2DsumTSmaxALS8");
      
      MapNumBadChanDepth[5][4][1] = (TH2F*)hfile->Get("h_2DsumTSmaxALS6");
      MapNumBadChanDepth[5][4][2] = (TH2F*)hfile->Get("h_2DsumTSmaxALS7"); 
      
      MapNumBadChanFull[5] = (TH2F*) MapNumBadChanDepth[5][1][1]->Clone();   
           
      MapNumChanDepth[5][1][1] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS1");     
      MapNumChanDepth[5][1][2] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS2"); 
 
      MapNumChanDepth[5][2][1] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS3");     
      MapNumChanDepth[5][2][2] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS4"); 
      MapNumChanDepth[5][2][3] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS5");     
      
      MapNumChanDepth[5][3][4] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS8"); 
      
      MapNumChanDepth[5][4][1] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS6");     
      MapNumChanDepth[5][4][2] = (TH2F*)hfile->Get("h_2D0sumTSmaxALS7");
      
      MapNumChanFull[5] = (TH2F*) MapNumChanDepth[5][1][1]->Clone(); 
                  
      int LSnumber = 0;
      
    for (int test=0;test<=5;test++) { //Test: 0, 
        for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
            if (sub==1) cHB->Divide(2,1);
            if (sub==2) cHE->Divide(3,1);
            if (sub==3) cONE->Divide(1,1);
            if (sub==4) cHB->Divide(2,1);
            for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 		                
                if (sub==1) cHB->cd(k); 
                if (sub==2) cHE->cd(k);
	        if (sub==3) cONE->cd(k-3);
	        if (sub==4) cHB->cd(k);            
		MapNumBadChanDepth[test][sub][k]->Divide(MapNumBadChanDepth[test][sub][k],MapNumChanDepth[test][sub][k], 1, 1, "B");
                for (int x=1;x<=MapNumBadChanFull[test]->GetXaxis()->GetNbins();x++) {
                   for (int y=1;y<=MapNumBadChanFull[test]->GetYaxis()->GetNbins(); y++) {
	               double ccc1 =  MapNumBadChanDepth[test][sub][k]->GetBinContent(x,y);
		       MapNumBadChanFull[test]->SetBinContent(x,y,MapNumBadChanFull[test]->GetBinContent(x,y) + ccc1);
                   }//end y
                }//esnd x
  		    
                if (k == 1) MapNumBadChanDepth[test][sub][k]->SetTitle("Depth 1\b");
		if (k == 2) MapNumBadChanDepth[test][sub][k]->SetTitle("Depth 2\b");
		if (k == 3) MapNumBadChanDepth[test][sub][k]->SetTitle("Depth 3\b");
		if (k == 4) MapNumBadChanDepth[test][sub][k]->SetTitle("Depth 4\b");
		gPad->SetGridy();
                gPad->SetGridx();
                gPad->SetLogz();
                MapNumBadChanDepth[test][sub][k]->SetXTitle("#eta \b");
                MapNumBadChanDepth[test][sub][k]->SetYTitle("#phi \b");
                MapNumBadChanDepth[test][sub][k]->SetZTitle("Average estimator \b");
	        MapNumBadChanDepth[test][sub][k]->SetTitleOffset(0.75,"Z");
                MapNumBadChanDepth[test][sub][k]->Draw("COLZ");
                MapNumBadChanDepth[test][sub][k]->GetYaxis()->SetRangeUser(0, 72.);
//                MapNumBadChanDepth[test][sub][k]->GetZaxis()->SetRangeUser(0.0001, 1.);
	    }    
	    if (test==0){ 
	        if (sub==1) {cHB->Print("MapCapIdErrorHB.png"); cHB->Clear();}
                if (sub==2) {cHE->Print("MapCapIdErrorHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("MapCapIdErrorHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("MapCapIdErrorHF.png"); cHB->Clear();}  	    
	    }
	    if (test==1){ 
	        if (sub==1) {cHB->Print("MapADCamplHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("MapADCamplHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("MapADCamplHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("MapADCamplHF.png"); cHB->Clear();}  
	    } 
	    if (test==2){ 
	        if (sub==1) {cHB->Print("MapWidthHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("MapWidthHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("MapWidthHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("MapWidthHF.png"); cHB->Clear();}  
	    } 
	    if (test==3){ 
	        if (sub==1) {cHB->Print("MapRatioHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("MapRatioHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("MapRatioHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("MapRatioHF.png"); cHB->Clear();}  
	    } 
	    if (test==4){ 
	        if (sub==1) {cHB->Print("MapTmeanHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("MapTmeanHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("MapTmeanHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("MapTmeanHF.png"); cHB->Clear();}  
	    } 
	    if (test==5){ 
	        if (sub==1) {cHB->Print("MapTmaxHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("MapTmaxHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("MapTmaxHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("MapTmaxHF.png"); cHB->Clear();}  
	    } 	    	    	   	                  
         }// end sub	 
         cONE->Divide(1,1);
	 cONE->cd(1);
	 gPad->SetGridy();
         gPad->SetGridx();
         gPad->SetLogz();
         MapNumBadChanFull[test]->SetTitle("All subdetectors\b");
	 MapNumBadChanFull[test]->SetXTitle("#eta \b");
         MapNumBadChanFull[test]->SetYTitle("#phi \b");
         MapNumBadChanFull[test]->SetZTitle("Average estimator \b");
	 MapNumBadChanFull[test]->SetTitleOffset(0.75,"Z");
	 MapNumBadChanFull[test]->Draw("COLZ");
         MapNumBadChanFull[test]->GetYaxis()->SetRangeUser(0, 72.);
//         MapNumBadChanFull[test]->GetZaxis()->SetRangeUser(0.0001, 1.);
	 if (test==0) {cONE->Print("MapCapIdError.png"); cONE->Clear();}	  
         if (test==1) {cONE->Print("MapADCAmpl.png"); cONE->Clear();}
         if (test==2) {cONE->Print("MapWidth.png"); cONE->Clear();}
         if (test==3) {cONE->Print("MapRatio.png"); cONE->Clear();}
	 if (test==4) {cONE->Print("MapTmean.png"); cONE->Clear();}
	 if (test==5) {cONE->Print("MapTmax.png"); cONE->Clear();}			      
     }//end test	          

      
//================================================================================================================================ 
  
 
//=======================================================================================================
// 1-d histograms third definition

      TH1F *HistNumBadChanDepth[7][5][5];    // 1d histogramm for test,subdet, depth 
      TH1F *HistCutNumBadChanDepth[7][5][5];    // 1d histogramm for test,subdet, depth 
      TH1F *HistNumChanDepth[7][5][5];    // 1d histogramm for test,subdet, depth   
      TH1F *HistNumBadChanFull[7];    // 1d histogramm for test
      TH1F *HistNumChanFull[7];    // 1d histogramm for test  

//+++++++++++++++++++++++++++++  
// Rate of Cap ID errors   
//+++++++++++++++++++++++++++++  
 
      HistNumBadChanDepth[0][1][1] = (TH1F*)hfile->Get("h_runnbadchannels_depth1_HB");
      HistNumBadChanDepth[0][1][2] = (TH1F*)hfile->Get("h_runnbadchannels_depth2_HB");
 
      HistNumBadChanDepth[0][2][1] = (TH1F*)hfile->Get("h_runnbadchannels_depth1_HE");
      HistNumBadChanDepth[0][2][2] = (TH1F*)hfile->Get("h_runnbadchannels_depth2_HE"); 
      HistNumBadChanDepth[0][2][3] = (TH1F*)hfile->Get("h_runnbadchannels_depth3_HE");
      
      HistNumBadChanDepth[0][3][4] = (TH1F*)hfile->Get("h_runnbadchannels_depth4_HO");
      
      HistNumBadChanDepth[0][4][1] = (TH1F*)hfile->Get("h_runnbadchannels_depth1_HF");
      HistNumBadChanDepth[0][4][2] = (TH1F*)hfile->Get("h_runnbadchannels_depth2_HF");    
       
      HistNumBadChanFull[0] = (TH1F*) HistNumBadChanDepth[0][1][1]->Clone();
      
      HistCutNumBadChanDepth[0][1][1] = (TH1F*)hfile->Get("h_runnbadchannels_depth1_HB");
      HistCutNumBadChanDepth[0][1][2] = (TH1F*)hfile->Get("h_runnbadchannels_depth2_HB");
 
      HistCutNumBadChanDepth[0][2][1] = (TH1F*)hfile->Get("h_runnbadchannels_depth1_HE");
      HistCutNumBadChanDepth[0][2][2] = (TH1F*)hfile->Get("h_runnbadchannels_depth2_HE"); 
      HistCutNumBadChanDepth[0][2][3] = (TH1F*)hfile->Get("h_runnbadchannels_depth3_HE");
      
      HistCutNumBadChanDepth[0][3][4] = (TH1F*)hfile->Get("h_runnbadchannels_depth4_HO");
      
      HistCutNumBadChanDepth[0][4][1] = (TH1F*)hfile->Get("h_runnbadchannels_depth1_HF");
      HistCutNumBadChanDepth[0][4][2] = (TH1F*)hfile->Get("h_runnbadchannels_depth2_HF"); 
            
      
      HistNumChanDepth[0][1][1] = (TH1F*)hfile->Get("h_runbadrate0_depth1_HB");     
      HistNumChanDepth[0][1][2] = (TH1F*)hfile->Get("h_runbadrate0_depth2_HB"); 
 
      HistNumChanDepth[0][2][1] = (TH1F*)hfile->Get("h_runbadrate0_depth1_HE");     
      HistNumChanDepth[0][2][2] = (TH1F*)hfile->Get("h_runbadrate0_depth2_HE"); 
      HistNumChanDepth[0][2][3] = (TH1F*)hfile->Get("h_runbadrate0_depth3_HE");     
      
      HistNumChanDepth[0][3][4] = (TH1F*)hfile->Get("h_runbadrate0_depth4_HO"); 
      
      HistNumChanDepth[0][4][1] = (TH1F*)hfile->Get("h_runbadrate0_depth1_HF");     
      HistNumChanDepth[0][4][2] = (TH1F*)hfile->Get("h_runbadrate0_depth2_HF");
                                                           
      
 //+++++++++++++++++++++++++++++  
// ADC Amplitude   
//+++++++++++++++++++++++++++++  
 
      HistNumBadChanDepth[1][1][1] = (TH1F*)hfile->Get("h_sumADCAmplperLS1");
      HistNumBadChanDepth[1][1][2] = (TH1F*)hfile->Get("h_sumADCAmplperLS2");
 
      HistNumBadChanDepth[1][2][1] = (TH1F*)hfile->Get("h_sumADCAmplperLS3");
      HistNumBadChanDepth[1][2][2] = (TH1F*)hfile->Get("h_sumADCAmplperLS4"); 
      HistNumBadChanDepth[1][2][3] = (TH1F*)hfile->Get("h_sumADCAmplperLS5");
      
      HistNumBadChanDepth[1][3][4] = (TH1F*)hfile->Get("h_sumADCAmplperLS8");
      
      HistNumBadChanDepth[1][4][1] = (TH1F*)hfile->Get("h_sumADCAmplperLS6");
      HistNumBadChanDepth[1][4][2] = (TH1F*)hfile->Get("h_sumADCAmplperLS7");
      
      HistNumBadChanFull[1] = (TH1F*) HistNumBadChanDepth[1][1][1]->Clone();
      
      HistCutNumBadChanDepth[1][1][1] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS1");
      HistCutNumBadChanDepth[1][1][2] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS2");
 
      HistCutNumBadChanDepth[1][2][1] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS3");
      HistCutNumBadChanDepth[1][2][2] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS4"); 
      HistCutNumBadChanDepth[1][2][3] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS5");
      
      HistCutNumBadChanDepth[1][3][4] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS8");
      
      HistCutNumBadChanDepth[1][4][1] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS6");
      HistCutNumBadChanDepth[1][4][2] = (TH1F*)hfile->Get("h_sumCutADCAmplperLS7");    
      
      
      HistNumChanDepth[1][1][1] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS1");     
      HistNumChanDepth[1][1][2] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS2"); 
 
      HistNumChanDepth[1][2][1] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS3");     
      HistNumChanDepth[1][2][2] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS4"); 
      HistNumChanDepth[1][2][3] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS5");     
      
      HistNumChanDepth[1][3][4] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS8"); 
      
      HistNumChanDepth[1][4][1] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS6");     
      HistNumChanDepth[1][4][2] = (TH1F*)hfile->Get("h_sum0ADCAmplperLS7");
           

//+++++++++++++++++++++++++++++  
// Width  
//+++++++++++++++++++++++++++++  
 
      HistNumBadChanDepth[2][1][1] = (TH1F*)hfile->Get("h_sumAmplitudeperLS1");
      HistNumBadChanDepth[2][1][2] = (TH1F*)hfile->Get("h_sumAmplitudeperLS2");
 
      HistNumBadChanDepth[2][2][1] = (TH1F*)hfile->Get("h_sumAmplitudeperLS3");
      HistNumBadChanDepth[2][2][2] = (TH1F*)hfile->Get("h_sumAmplitudeperLS4"); 
      HistNumBadChanDepth[2][2][3] = (TH1F*)hfile->Get("h_sumAmplitudeperLS5");
      
      HistNumBadChanDepth[2][3][4] = (TH1F*)hfile->Get("h_sumAmplitudeperLS8");
      
      HistNumBadChanDepth[2][4][1] = (TH1F*)hfile->Get("h_sumAmplitudeperLS6");
      HistNumBadChanDepth[2][4][2] = (TH1F*)hfile->Get("h_sumAmplitudeperLS7");    
       
      HistNumBadChanFull[2] = (TH1F*) HistNumBadChanDepth[2][1][1]->Clone();  
      
      HistCutNumBadChanDepth[2][1][1] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS1");
      HistCutNumBadChanDepth[2][1][2] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS2");
 
      HistCutNumBadChanDepth[2][2][1] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS3");
      HistCutNumBadChanDepth[2][2][2] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS4"); 
      HistCutNumBadChanDepth[2][2][3] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS5");
      
      HistCutNumBadChanDepth[2][3][4] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS8");
      
      HistCutNumBadChanDepth[2][4][1] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS6");
      HistCutNumBadChanDepth[2][4][2] = (TH1F*)hfile->Get("h_sumCutAmplitudeperLS7"); 
           
      
      HistNumChanDepth[2][1][1] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS1");     
      HistNumChanDepth[2][1][2] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS2"); 
 
      HistNumChanDepth[2][2][1] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS3");     
      HistNumChanDepth[2][2][2] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS4"); 
      HistNumChanDepth[2][2][3] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS5");     
      
      HistNumChanDepth[2][3][4] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS8"); 
      
      HistNumChanDepth[2][4][1] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS6");     
      HistNumChanDepth[2][4][2] = (TH1F*)hfile->Get("h_sum0AmplitudeperLS7");
      
//+++++++++++++++++++++++++++++  
// Ratio   
//+++++++++++++++++++++++++++++  
 
      HistNumBadChanDepth[3][1][1] = (TH1F*)hfile->Get("h_sumAmplperLS1");
      HistNumBadChanDepth[3][1][2] = (TH1F*)hfile->Get("h_sumAmplperLS2");
 
      HistNumBadChanDepth[3][2][1] = (TH1F*)hfile->Get("h_sumAmplperLS3");
      HistNumBadChanDepth[3][2][2] = (TH1F*)hfile->Get("h_sumAmplperLS4"); 
      HistNumBadChanDepth[3][2][3] = (TH1F*)hfile->Get("h_sumAmplperLS5");
      
      HistNumBadChanDepth[3][3][4] = (TH1F*)hfile->Get("h_sumAmplperLS8");
      
      HistNumBadChanDepth[3][4][1] = (TH1F*)hfile->Get("h_sumAmplperLS6");
      HistNumBadChanDepth[3][4][2] = (TH1F*)hfile->Get("h_sumAmplperLS7");    
      
      HistNumBadChanFull[3] = (TH1F*) HistNumBadChanDepth[3][1][1]->Clone();
      
      HistCutNumBadChanDepth[3][1][1] = (TH1F*)hfile->Get("h_sumCutAmplperLS1");
      HistCutNumBadChanDepth[3][1][2] = (TH1F*)hfile->Get("h_sumCutAmplperLS2");
 
      HistCutNumBadChanDepth[3][2][1] = (TH1F*)hfile->Get("h_sumCutAmplperLS3");
      HistCutNumBadChanDepth[3][2][2] = (TH1F*)hfile->Get("h_sumCutAmplperLS4"); 
      HistCutNumBadChanDepth[3][2][3] = (TH1F*)hfile->Get("h_sumCutAmplperLS5");
      
      HistCutNumBadChanDepth[3][3][4] = (TH1F*)hfile->Get("h_sumCutAmplperLS8");
      
      HistCutNumBadChanDepth[3][4][1] = (TH1F*)hfile->Get("h_sumCutAmplperLS6");
      HistCutNumBadChanDepth[3][4][2] = (TH1F*)hfile->Get("h_sumCutAmplperLS7");       
      
      
      HistNumChanDepth[3][1][1] = (TH1F*)hfile->Get("h_sum0AmplperLS1");     
      HistNumChanDepth[3][1][2] = (TH1F*)hfile->Get("h_sum0AmplperLS2"); 
 
      HistNumChanDepth[3][2][1] = (TH1F*)hfile->Get("h_sum0AmplperLS3");     
      HistNumChanDepth[3][2][2] = (TH1F*)hfile->Get("h_sum0AmplperLS4"); 
      HistNumChanDepth[3][2][3] = (TH1F*)hfile->Get("h_sum0AmplperLS5");     
      
      HistNumChanDepth[3][3][4] = (TH1F*)hfile->Get("h_sum0AmplperLS8"); 
      
      HistNumChanDepth[3][4][1] = (TH1F*)hfile->Get("h_sum0AmplperLS6");     
      HistNumChanDepth[3][4][2] = (TH1F*)hfile->Get("h_sum0AmplperLS7");
      
//+++++++++++++++++++++++++++++  
// Tmean   
//+++++++++++++++++++++++++++++  
 
      HistNumBadChanDepth[4][1][1] = (TH1F*)hfile->Get("h_sumTSmeanAperLS1");
      HistNumBadChanDepth[4][1][2] = (TH1F*)hfile->Get("h_sumTSmeanAperLS2");
 
      HistNumBadChanDepth[4][2][1] = (TH1F*)hfile->Get("h_sumTSmeanAperLS3");
      HistNumBadChanDepth[4][2][2] = (TH1F*)hfile->Get("h_sumTSmeanAperLS4"); 
      HistNumBadChanDepth[4][2][3] = (TH1F*)hfile->Get("h_sumTSmeanAperLS5");
      
      HistNumBadChanDepth[4][3][4] = (TH1F*)hfile->Get("h_sumTSmeanAperLS8");
     
      HistNumBadChanDepth[4][4][1] = (TH1F*)hfile->Get("h_sumTSmeanAperLS6");
      HistNumBadChanDepth[4][4][2] = (TH1F*)hfile->Get("h_sumTSmeanAperLS7");    
            
      HistNumBadChanFull[4] = (TH1F*) HistNumBadChanDepth[4][1][1]->Clone();
      
      HistCutNumBadChanDepth[4][1][1] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS1");
      HistCutNumBadChanDepth[4][1][2] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS2");
 
      HistCutNumBadChanDepth[4][2][1] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS3");
      HistCutNumBadChanDepth[4][2][2] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS4"); 
      HistCutNumBadChanDepth[4][2][3] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS5");
      
      HistCutNumBadChanDepth[4][3][4] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS8");
     
      HistCutNumBadChanDepth[4][4][1] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS6");
      HistCutNumBadChanDepth[4][4][2] = (TH1F*)hfile->Get("h_sumCutTSmeanAperLS7");       
      
      
      HistNumChanDepth[4][1][1] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS1");     
      HistNumChanDepth[4][1][2] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS2"); 
 
      HistNumChanDepth[4][2][1] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS3");     
      HistNumChanDepth[4][2][2] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS4"); 
      HistNumChanDepth[4][2][3] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS5");     
      
      HistNumChanDepth[4][3][4] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS8"); 
      
      HistNumChanDepth[4][4][1] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS6");     
      HistNumChanDepth[4][4][2] = (TH1F*)hfile->Get("h_sum0TSmeanAperLS7");
      
//+++++++++++++++++++++++++++++  
// Tmax   
//+++++++++++++++++++++++++++++  
 
      HistNumBadChanDepth[5][1][1] = (TH1F*)hfile->Get("h_sumTSmaxAperLS1");
      HistNumBadChanDepth[5][1][2] = (TH1F*)hfile->Get("h_sumTSmaxAperLS2");
 
      HistNumBadChanDepth[5][2][1] = (TH1F*)hfile->Get("h_sumTSmaxAperLS3");
      HistNumBadChanDepth[5][2][2] = (TH1F*)hfile->Get("h_sumTSmaxAperLS4"); 
      HistNumBadChanDepth[5][2][3] = (TH1F*)hfile->Get("h_sumTSmaxAperLS5");
      
      HistNumBadChanDepth[5][3][4] = (TH1F*)hfile->Get("h_sumTSmaxAperLS8");
      
      HistNumBadChanDepth[5][4][1] = (TH1F*)hfile->Get("h_sumTSmaxAperLS6");
      HistNumBadChanDepth[5][4][2] = (TH1F*)hfile->Get("h_sumTSmaxAperLS7");    
      
      HistNumBadChanFull[5] = (TH1F*) HistNumBadChanDepth[5][1][1]->Clone();
      
      HistCutNumBadChanDepth[5][1][1] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS1");
      HistCutNumBadChanDepth[5][1][2] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS2");
 
      HistCutNumBadChanDepth[5][2][1] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS3");
      HistCutNumBadChanDepth[5][2][2] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS4"); 
      HistCutNumBadChanDepth[5][2][3] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS5");
      
      HistCutNumBadChanDepth[5][3][4] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS8");
      
      HistCutNumBadChanDepth[5][4][1] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS6");
      HistCutNumBadChanDepth[5][4][2] = (TH1F*)hfile->Get("h_sumCutTSmaxAperLS7"); 
            
      
      HistNumChanDepth[5][1][1] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS1");     
      HistNumChanDepth[5][1][2] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS2"); 
 
      HistNumChanDepth[5][2][1] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS3");     
      HistNumChanDepth[5][2][2] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS4"); 
      HistNumChanDepth[5][2][3] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS5");     
      
      HistNumChanDepth[5][3][4] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS8"); 
      
      HistNumChanDepth[5][4][1] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS6");     
      HistNumChanDepth[5][4][2] = (TH1F*)hfile->Get("h_sum0TSmaxAperLS7");
                  
      LSnumber = 0;
      
    for (int test=0;test<=5;test++) { //Test: 0, 
        for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
            if (sub==1) cHB->Divide(2,1);
            if (sub==2) cHE->Divide(3,1);
            if (sub==3) cONE->Divide(1,1);
            if (sub==4) cHB->Divide(2,1);
            for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
                if (sub==1) cHB->cd(k); 
                if (sub==2) cHE->cd(k);
	        if (sub==3) cONE->cd(k-3);
	        if (sub==4) cHB->cd(k);
		gPad->SetGridy();
                gPad->SetGridx();
//                gPad->SetLogy();      

	        HistNumBadChanDepth[test][sub][k]->Divide(HistNumBadChanDepth[test][sub][k],HistNumChanDepth[test][sub][k], 1, 1, "B");	

                for (int x=1;x<=HistNumBadChanFull[test]->GetXaxis()->GetNbins();x++) {
	               double ccc1 =  HistNumBadChanDepth[test][sub][k]->GetBinContent(x);
		       HistNumBadChanFull[test]->SetBinContent(x,HistNumBadChanFull[test]->GetBinContent(x) + ccc1);
                }//esnd x

                if (k == 1) HistNumBadChanDepth[test][sub][k]->SetTitle("Depth 1\b");
		if (k == 2) HistNumBadChanDepth[test][sub][k]->SetTitle("Depth 2\b");
		if (k == 3) HistNumBadChanDepth[test][sub][k]->SetTitle("Depth 3\b");
		if (k == 4) HistNumBadChanDepth[test][sub][k]->SetTitle("Depth 4\b");
		HistNumBadChanDepth[test][sub][k]->SetXTitle("LS \b");
		if (test == 0) HistNumBadChanDepth[test][sub][k]->SetYTitle("<Number of bad channels> \b");
		if (test != 0) HistNumBadChanDepth[test][sub][k]->SetYTitle("Averaged estimator \b");
                if (MaxLum<=1000){
		   HistNumBadChanDepth[test][sub][k]->SetMarkerStyle(20);
                   HistNumBadChanDepth[test][sub][k]->SetMarkerSize(0.5);	
		   HistNumBadChanDepth[test][sub][k]->SetMarkerColor(2);
                   HistNumBadChanDepth[test][sub][k]->SetLineColor(0);
                   HistNumBadChanDepth[test][sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
		   HistNumBadChanDepth[test][sub][k]->Draw("P");
		}
		else{
                   HistNumBadChanDepth[test][sub][k]->SetLineColor(2);
                   HistNumBadChanDepth[test][sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
		   HistNumBadChanDepth[test][sub][k]->Draw("L");
		}	
	      float min_x[] = {0,10000};
              float min_y[] = {(float)(Cut0[test][sub][k]),(float)(Cut0[test][sub][k])};
              TGraph* MIN = new TGraph(2, min_x, min_y);
              MIN->SetLineStyle(2);
              MIN->SetLineColor(5);
              MIN->SetLineWidth(2 + 100*100);
              MIN->SetFillStyle(3005);
              MIN->SetFillColor(5);
              MIN->Draw("L");
/*	       
	        if (HistNumChanDepth[test][sub][k]->IsA()->InheritsFrom("TH1F"))
	            HistCutNumBadChanDepth[test][sub][k]->Divide(HistCutNumBadChanDepth[test][sub][k],HistNumChanDepth[test][sub][k], 1, 1, "B");
                HistCutNumBadChanDepth[test][sub][k]->SetMarkerStyle(20);
                HistCutNumBadChanDepth[test][sub][k]->SetMarkerSize(0.5);		
                HistCutNumBadChanDepth[test][sub][k]->SetMarkerColor(2);
                HistCutNumBadChanDepth[test][sub][k]->SetLineColor(0);
//		HistCutNumBadChanDepth[test][sub][k]->Draw("SameE");
*/				
                int nx = HistNumBadChanDepth[test][sub][k]->GetXaxis()->GetNbins();
                if ((test==0)&&(sub==1)&&(k==1)) {
		   for (int i=1;i<=nx;i++) {
//	               double ccc1 =  HistNumBadChanDepth[test][sub][k]->GetBinContent(i);
	               if(HistNumBadChanDepth[test][sub][k]->GetBinContent(i)>0) {
//	                  cout<<"depth1_HB iLS = "<<i<<" <Nbcs> per iLS= "<<ccc1<<endl;
		          LSnumber+=1;
	               }
	           }
               } 
	       	if (sub==1) { cHB->Modified();} 
                if (sub==2) { cHE->Modified();}
                if (sub==3) { cONE->Modified();}
                if (sub==4) { cHB->Modified();} 
	    }    
	    if (test==0){ 
	        if (sub==1) {cHB->Print("HistNBCMNHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistNBCMNHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistNBCMNHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistNBCMNHF.png"); cHB->Clear();}  
	    } 
	    
	    if (test==1){ 
	        if (sub==1) {cHB->Print("HistADCamplHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistADCamplHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistADCamplHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistADCamplHF.png"); cHB->Clear();}  
	    } 
	    if (test==2){ 
	        if (sub==1) {cHB->Print("HistWidthHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistWidthHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistWidthHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistWidthHF.png"); cHB->Clear();}  
	    } 
	    if (test==3){ 
	        if (sub==1) {cHB->Print("HistRatioHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistRatioHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistRatioHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistRatioHF.png"); cHB->Clear();}  
	    } 
	    if (test==4){ 
	        if (sub==1) {cHB->Print("HistTmeanHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistTmeanHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistTmeanHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistTmeanHF.png"); cHB->Clear();}  
	    } 
	    if (test==5){ 
	        if (sub==1) {cHB->Print("HistTmaxHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistTmaxHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistTmaxHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistTmaxHF.png"); cHB->Clear();}  
	    } 	    	   	                  
         }// end sub 
         cONE->Divide(1,1);
	 cONE->cd(1);	 
	 HistNumBadChanFull[test]->SetMarkerStyle(20);
         HistNumBadChanFull[test]->SetMarkerSize(0.8);
	 HistNumBadChanFull[test]->SetTitle("All subdetectors\b");
	 HistNumBadChanFull[test]->SetXTitle("LS \b");
	 if (test == 0) HistNumBadChanFull[test]->SetYTitle("<Number of bad channels> \b");
	 if (test != 0) HistNumBadChanFull[test]->SetYTitle("Averaged estimator \b");
         if (MaxLum<=1000){
	    HistNumBadChanFull[test]->SetMarkerColor(1);
            HistNumBadChanFull[test]->SetLineColor(0);
            HistNumBadChanFull[test]->Draw("P"); 
	 }
	 else {
            HistNumBadChanFull[test]->SetLineColor(1);
            HistNumBadChanFull[test]->Draw("L"); 
	 }	  
         if (test==0) {cONE->Print("HistCapID.png"); cONE->Clear();}
         if (test==1) {cONE->Print("HistADCAmpl.png"); cONE->Clear();}
         if (test==2) {cONE->Print("HistWidth.png"); cONE->Clear();}
         if (test==3) {cONE->Print("HistRatio.png"); cONE->Clear();}
	 if (test==4) {cONE->Print("HistTmean.png"); cONE->Clear();}
	 if (test==5) {cONE->Print("HistTmax.png"); cONE->Clear();}	
		      
     }//end test	          
      
//================================================================================================================================ 
 
 
  
 
 
 
//=======================================================================================================333
//CapID 1-d histograms

      TH1F *HistAbnormNumBadChanDepth[5][5];    // 1d histogramm for subdet, depth 


//+++++++++++++++++++++++++++++  
// Abnormal Bad Channels Rate of Cap ID errors  first set of histograms   
//+++++++++++++++++++++++++++++  
 
      HistAbnormNumBadChanDepth[1][1] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth1_HB");
      HistAbnormNumBadChanDepth[1][2] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth2_HB");
      HistAbnormNumBadChanDepth[2][1] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth1_HE");
      HistAbnormNumBadChanDepth[2][2] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth2_HE");
      HistAbnormNumBadChanDepth[2][3] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth3_HE");
      HistAbnormNumBadChanDepth[3][4] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth4_HO");
      HistAbnormNumBadChanDepth[4][1] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth1_HF");
      HistAbnormNumBadChanDepth[4][2] = (TH1F*)hfile->Get("h_runnbadchannelsC_depth2_HF");
                                                           
      for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
	if (sub==1) cHB->Divide(2,1);
	if (sub==2) cHE->Divide(3,1);
	if (sub==3) cONE->Divide(1,1);
	if (sub==4) cHB->Divide(2,1);
	for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
	  if (sub==1) cHB->cd(k); 
	  if (sub==2) cHE->cd(k);
	  if (sub==3) cONE->cd(k-3);
	  if (sub==4) cHB->cd(k);
	  gPad->SetGridy();
	  gPad->SetGridx();
	  //  gPad->SetLogy();      
	  HistAbnormNumBadChanDepth[sub][k]->Divide(HistAbnormNumBadChanDepth[sub][k],HistNumChanDepth[0][sub][k], 1, 1, "B");		
	  if (k == 1)  HistAbnormNumBadChanDepth[sub][k]->SetTitle("Depth 1\b");
	  if (k == 2)  HistAbnormNumBadChanDepth[sub][k]->SetTitle("Depth 2\b");
	  if (k == 3)  HistAbnormNumBadChanDepth[sub][k]->SetTitle("Depth 3\b");
	  if (k == 4)  HistAbnormNumBadChanDepth[sub][k]->SetTitle("Depth 4\b");
	  HistAbnormNumBadChanDepth[sub][k]->SetXTitle("LS \b");
	  HistAbnormNumBadChanDepth[sub][k]->SetYTitle("< Number of bad channels > \b");
	  if (MaxLum<=1000){
	     HistAbnormNumBadChanDepth[sub][k]->SetMarkerStyle(20);
	     HistAbnormNumBadChanDepth[sub][k]->SetMarkerSize(0.5);
             HistAbnormNumBadChanDepth[sub][k]->SetMarkerColor(1);
             HistAbnormNumBadChanDepth[sub][k]->SetLineColor(0);
             HistAbnormNumBadChanDepth[sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
             HistAbnormNumBadChanDepth[sub][k]->Draw("P");
	  }
	  else {
	     HistAbnormNumBadChanDepth[sub][k]->SetLineColor(1);
             HistAbnormNumBadChanDepth[sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
             HistAbnormNumBadChanDepth[sub][k]->Draw("L");
	  }	  
	 }     
	        if (sub==1) {cHB->Print("Hist_CAPID_Abnorm_HB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("Hist_CAPID_Abnorm_HE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("Hist_CAPID_Abnorm_HO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("Hist_CAPID_Abnorm_HF.png"); cHB->Clear();}  	    	    
         }// end sub 



//+++++++++++++++++++++++++++++  
// Portions of bad channels events identified by Cap ID errors   third set of histograms 
//+++++++++++++++++++++++++++++  

      TH1F *HistPortBadEventsDepth[5][5];    // 1d histogramm for subdet, depth 

      HistPortBadEventsDepth[1][1] = (TH1F*)hfile->Get("h_runbadrateC_depth1_HB");
      HistPortBadEventsDepth[1][2] = (TH1F*)hfile->Get("h_runbadrateC_depth2_HB");
      HistPortBadEventsDepth[2][1] = (TH1F*)hfile->Get("h_runbadrateC_depth1_HE");
      HistPortBadEventsDepth[2][2] = (TH1F*)hfile->Get("h_runbadrateC_depth2_HE");
      HistPortBadEventsDepth[2][3] = (TH1F*)hfile->Get("h_runbadrateC_depth3_HE");
      HistPortBadEventsDepth[3][4] = (TH1F*)hfile->Get("h_runbadrateC_depth4_HO");
      HistPortBadEventsDepth[4][1] = (TH1F*)hfile->Get("h_runbadrateC_depth1_HF");
      HistPortBadEventsDepth[4][2] = (TH1F*)hfile->Get("h_runbadrateC_depth2_HF");
     
      
      TH1F *HistNumRateDepth[5][5];
      HistNumRateDepth[1][1] = (TH1F*)hfile->Get("h_runbadrate0_depth1_HB");
      HistNumRateDepth[1][2] = (TH1F*)hfile->Get("h_runbadrate0_depth2_HB");
      HistNumRateDepth[2][1] = (TH1F*)hfile->Get("h_runbadrate0_depth1_HE");
      HistNumRateDepth[2][2] = (TH1F*)hfile->Get("h_runbadrate0_depth2_HE");
      HistNumRateDepth[2][3] = (TH1F*)hfile->Get("h_runbadrate0_depth3_HE");
      HistNumRateDepth[3][4] = (TH1F*)hfile->Get("h_runbadrate0_depth4_HO");
      HistNumRateDepth[4][1] = (TH1F*)hfile->Get("h_runbadrate0_depth1_HF");
      HistNumRateDepth[4][2] = (TH1F*)hfile->Get("h_runbadrate0_depth2_HF");
     
      for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
	if (sub==1) cHB->Divide(2,1);
	if (sub==2) cHE->Divide(3,1);
	if (sub==3) cONE->Divide(1,1);
	if (sub==4) cHB->Divide(2,1);
	for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
	  if (sub==1) cHB->cd(k); 
	  if (sub==2) cHE->cd(k);
	  if (sub==3) cONE->cd(k-3);
	  if (sub==4) cHB->cd(k);
	  gPad->SetGridy();
	  gPad->SetGridx();
	  //  gPad->SetLogy();      
 	  HistPortBadEventsDepth[sub][k]->Divide(HistPortBadEventsDepth[sub][k],HistNumRateDepth[sub][k], 1, 1, "B");
	 		
	  if (k == 1)  HistPortBadEventsDepth[sub][k]->SetTitle("Depth 1\b");
	  if (k == 2)  HistPortBadEventsDepth[sub][k]->SetTitle("Depth 2\b");
	  if (k == 3)  HistPortBadEventsDepth[sub][k]->SetTitle("Depth 3\b");
	  if (k == 4)  HistPortBadEventsDepth[sub][k]->SetTitle("Depth 4\b");
	  HistPortBadEventsDepth[sub][k]->SetXTitle("LS \b");
	  HistPortBadEventsDepth[sub][k]->SetYTitle("< Number of bad channels > \b");
	  if (MaxLum<=1000){
	     HistPortBadEventsDepth[sub][k]->SetMarkerStyle(20);
	     HistPortBadEventsDepth[sub][k]->SetMarkerSize(0.5);	  
	     HistPortBadEventsDepth[sub][k]->SetMarkerColor(1);
	     HistPortBadEventsDepth[sub][k]->SetLineColor(0);
	     HistPortBadEventsDepth[sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
	     HistPortBadEventsDepth[sub][k]->Draw("P");
	  }
	  else {
	     HistPortBadEventsDepth[sub][k]->SetLineColor(1);
	     HistPortBadEventsDepth[sub][k]->GetXaxis()->SetRangeUser(0, MaxLum);
	     HistPortBadEventsDepth[sub][k]->Draw("L");
	  }
	}     
	        if (sub==1) {cHB->Print("HistPortHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistPortHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistPortHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistPortHF.png"); cHB->Clear();}  	    	    
         }// end sub 

//================================================================================================================================  

//+++++++++++++++++++++++++++++  
// Number of bad channels events identified by Cap ID errors   third set of histograms 
//+++++++++++++++++++++++++++++  

      TH1F *HistNBadChsDepth[5][5];    // 1d histogramm for subdet, depth 

      HistNBadChsDepth[1][1] = (TH1F*)hfile->Get("h_nbadchannels_depth1_HB");
      HistNBadChsDepth[1][2] = (TH1F*)hfile->Get("h_nbadchannels_depth2_HB");
      HistNBadChsDepth[2][1] = (TH1F*)hfile->Get("h_nbadchannels_depth1_HE");
      HistNBadChsDepth[2][2] = (TH1F*)hfile->Get("h_nbadchannels_depth2_HE");
      HistNBadChsDepth[2][3] = (TH1F*)hfile->Get("h_nbadchannels_depth3_HE");
      HistNBadChsDepth[3][4] = (TH1F*)hfile->Get("h_nbadchannels_depth4_HO");
      HistNBadChsDepth[4][1] = (TH1F*)hfile->Get("h_nbadchannels_depth1_HF");
      HistNBadChsDepth[4][2] = (TH1F*)hfile->Get("h_nbadchannels_depth2_HF");
     
      for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
	if (sub==1) cHB->Divide(2,1);
	if (sub==2) cHE->Divide(3,1);
	if (sub==3) cONE->Divide(1,1);
	if (sub==4) cHB->Divide(2,1);
	for (int k=k_min[sub];k<=k_max[sub];k++) {  //Depth 
	  if (sub==1) cHB->cd(k); 
	  if (sub==2) cHE->cd(k);
	  if (sub==3) cONE->cd(k-3);
	  if (sub==4) cHB->cd(k);
	  gPad->SetGridy();
	  gPad->SetGridx();
	  gPad->SetLogy();      
	 		
	  if (k == 1)  HistNBadChsDepth[sub][k]->SetTitle("Depth 1\b");
	  if (k == 2)  HistNBadChsDepth[sub][k]->SetTitle("Depth 2\b");
	  if (k == 3)  HistNBadChsDepth[sub][k]->SetTitle("Depth 3\b");
	  if (k == 4)  HistNBadChsDepth[sub][k]->SetTitle("Depth 4\b");
	  HistNBadChsDepth[sub][k]->SetXTitle("Number of bad channels\b");
	  HistNBadChsDepth[sub][k]->SetYTitle("Number of events\b");
//	     HistNBadChsDepth[sub][k]->SetMarkerStyle(20);
//	     HistNBadChsDepth[sub][k]->SetMarkerSize(0.5);	  
//	     HistNBadChsDepth[sub][k]->SetMarkerColor(1);
//	     HistNBadChsDepth[sub][k]->SetLineColor(0);
	     HistNBadChsDepth[sub][k]->Draw("");
	}     
	        if (sub==1) {cHB->Print("HistNBadChsHB.png"); cHB->Clear();} 
                if (sub==2) {cHE->Print("HistNBadChsHE.png"); cHE->Clear();}
                if (sub==3) {cONE->Print("HistNBadChsHO.png"); cONE->Clear();}
                if (sub==4) {cHB->Print("HistNBadChsHF.png"); cHB->Clear();}  	    	    
         }// end sub 


//====================================================================== done by Andrius for HF:
// Special test of errors type A and B in HF

  int flagErrAB_HF[2];
  flagErrAB_HF[0]=-1;
  flagErrAB_HF[1]=-1;
  double avedelta = 0.;
  int lastLumiBin=-1;
  int LSofFirstErrB = -1;
  {
    const int specCountA=4;
    const int specColors[specCountA] = { 1, 2, 3, 4 };
    const TString hnames[specCountA][2] =
      { { "h_sumADCAmplperLS6_P1", "h_sum0ADCAmplperLS6_P1" },
	{ "h_sumADCAmplperLS6_P2", "h_sum0ADCAmplperLS6_P2" },
	{ "h_sumADCAmplperLS6_M1", "h_sum0ADCAmplperLS6_M1" },
	{ "h_sumADCAmplperLS6_M2", "h_sum0ADCAmplperLS6_M2" } };

    std::vector<TH1F*> hV;
    THStack *hs= new THStack("hs","ADCAmplerLS6");
    cHB->Clear();
    //    cHB->cd();
    cHB->Divide(2,1);
    cHB->cd(1);

    ///////////////////////////////////////////////////////////////////////////
    for (int i=0; i<specCountA; i++) {
      if (1) std::cout << "debugger: errA_HF : get histos for i=" << i  << " " << hnames[i][0] << " and " << hnames[i][1] << "\n";
      TH1F *h1= (TH1F*)hfile->Get(hnames[i][0]);
      TH1F *h0= (TH1F*)hfile->Get(hnames[i][1]);
      if (!h1 || !h0) {
	TPaveText *ptext= new TPaveText(0.05,0.85,0.95,0.95);
	ptext->AddText("Missing histo");
	if (!h1) {
	  std::cout << "\tfailed to get " << hnames[i][0] << "\n";
	  ptext->AddText(hnames[i][0]);
	}
	if (!h0) {
	  std::cout << "\tfailed to get " << hnames[i][1] << "\n";
	  ptext->AddText(hnames[i][1]);
	}
	ptext->Draw();
	continue;
      }
      TH1F *hERT1orig= (TH1F*)h1->Clone(Form("ERT1orig_%d",i));
      hERT1orig->Divide(h1, h0, 1,1, "B");

      if ((lastLumiBin<0) && (i==0)) {
	for (int ibin=hERT1orig->GetNbinsX(); ibin>=1; ibin--) {
	  if (hERT1orig->GetBinContent(ibin)==0) lastLumiBin=ibin;
	  else break;
	}
	lastLumiBin+=3; // show more bins
	if (lastLumiBin >= hERT1orig->GetNbinsX()) lastLumiBin=-1;
      }
      TH1F *hERT1=NULL;
      if (lastLumiBin>1) {
	if (!copyContents(&hERT1,Form("ERT1_%d",i),"", hERT1orig,lastLumiBin)) {
	  std::cout << "code failed"<<std::endl;
	  gSystem->Exit(1);
	}
      }
      else hERT1=hERT1orig;

      hERT1->GetXaxis()->SetTitle("<ADCAmpl> per LS HF: black-P1, red-P2,green-M1,blue-M2");
      hV.push_back(hERT1);
      hERT1->SetMarkerStyle(20);
      hERT1->SetMarkerSize(0.4);
      hERT1->SetXTitle("<A>(ev.in LS & ch.) - HF P1     -    iLS \b");
      hERT1->SetMarkerColor(specColors[i]);
      hERT1->SetLineColor(0);
      hs->Add(hERT1);
      delete h1;
      delete h0;
      if (hERT1!=hERT1orig) delete hERT1orig;
    } /////////////////////////////////////////////////////////////////////////
    hs->Draw("LPE1 nostack"); 
    cHB->Update(); // activate the axes
    hs->GetXaxis()->SetTitle("<A> per LS: black-P1, red-P2,green-M1,blue-M2");
    hs->Draw("LPE1 nostack");
    gPad->SetGridy();
    /////////////////////////////////////////////////////////////////////////////////////////

    // AZ corrections 04.02.2016
    cHB->cd(2);
    TH1F* diff = new TH1F("diff","", 100, 0., 4.);

    if (int(hV.size())==specCountA) {
      flagErrAB_HF[0]=0;// If we have the expected number of histograms, set the flag
      double sumdelta = 0.;
      int nnndelta = 0;
      for (int ibin=1; ibin<=hV[0]->GetNbinsX(); ibin++) {
	double delta = 0.; 
	double maxdelta = 0.; 
	for (unsigned int i=0; i<hV.size(); i++) {
	  const TH1F* hi= hV[i];
	  for (unsigned int j=1; j<hV.size(); j++) {
	    const TH1F* hj= hV[j];
	    delta = fabs(hi->GetBinContent(ibin) - hj->GetBinContent(ibin));
	    if(delta > maxdelta) maxdelta=delta;
	  }//for
	}//for
	if(maxdelta> 0.) {
	  diff->Fill(maxdelta);
	  sumdelta+=maxdelta;
	  nnndelta++;
	}
      }//for ibin
      //      avedelta = sumdelta/hV[0]->GetNbinsX();
      avedelta = sumdelta/nnndelta;
      std::cout << "******************>>>>>>      ErrA_HF:  avedelta = " << avedelta <<std::endl;
      if (avedelta>2.4 || (avedelta<0.8 && avedelta>0.)) {
	flagErrAB_HF[0]=1;
      }//if
    }//hV.size
    diff->SetMarkerStyle(20);
    diff->SetMarkerSize(0.8);
    diff->SetXTitle("max difference \b");
    diff->SetMarkerColor(2);
    diff->SetLineColor(0);
    gPad->SetGridx();
    gPad->SetLogy();
    diff->Draw("Error");
    /////////////////////////////////////////////////////////////////////////
    cHB->Update();
    cHB->Print("HistErrA_HF.png");
    cHB->Clear();
    /////////////////////////////////////////////////////////////////////////
    
    // clean-up
    for (unsigned int i=0; i<hV.size(); i++) delete hV[i];
  } // ErrorA in HF


  /////////////////////////


  { // errors type B
    const int specCountB=4;
    const TString hnames[specCountB][2] =
      { { "h_2DsumErrorBLS6", "h_2D0sumErrorBLS6" },
	{ "h_sumErrorBperLS6", "h_sum0ErrorBperLS6" },
	{ "h_2DsumErrorBLS7", "h_2D0sumErrorBLS7" },
	{ "h_sumErrorBperLS7", "h_sum0ErrorBperLS7" } };

    for (int depth=1; depth<=2; depth++) {

      cHB->Clear();
      cHB->Divide(2,1);
      cHB->cd(1);

      TH1F *hRate2orig= NULL;
      TH2F *h2Cefz6= NULL;
      TString hname1= hnames[2*depth-2][0];
      TString hname0= hnames[2*depth-2][1];
      TH2F *twod1= (TH2F*)hfile->Get(hname1);
      TH2F *twod0= (TH2F*)hfile->Get(hname0);
      if (1) std::cout << "debugger: errB_HF depth=" << depth << ". get 2D histos "  << hname1 << " and " << hname0 << "\n";
      if (!twod1 || !twod0) {
	TPaveText *ptext= new TPaveText(0.05,0.85,0.95,0.95);
	ptext->AddText("Missing histos");
	if (!twod1) {
	  std::cout << "\tfailed to get " << hname1 << "\n";
	  ptext->AddText(hname1);
	}
	if (!twod0) {
	  std::cout << "\tfailed to get " << hname0 << "\n";
	  ptext->AddText(hname0);
	}
	ptext->Draw();
      }
      else {
	h2Cefz6= (TH2F*)twod1->Clone(Form("Cefz6_%d",depth));
	h2Cefz6->SetTitle(Form("HF Depth %d \b",depth));
	h2Cefz6->Divide(twod1,twod0, 1, 1, "B");

	gPad->SetGridy();
	gPad->SetGridx();
	gPad->SetLogz();
	h2Cefz6->SetTitle(Form("Depth %d \b",depth));
	h2Cefz6->SetMarkerStyle(20);
	h2Cefz6->SetMarkerSize(0.4);
	//	h2Cefz6->GetZaxis()->SetLabelSize(0.04);
	h2Cefz6->SetXTitle("#eta \b");
	h2Cefz6->SetYTitle("#phi \b");
	h2Cefz6->SetZTitle(Form("<ErrorB>  - HF Depth%d \b",depth));
	h2Cefz6->SetMarkerColor(2);
	h2Cefz6->SetLineColor(2);
	h2Cefz6->Draw("COLZ");

	delete twod1;
	delete twod0;
      } // histos ok

      cHB->cd(2);
      hname1=hnames[2*depth-1][0];
      hname0=hnames[2*depth-1][1];
      TH1F *h1= (TH1F*)hfile->Get(hname1);
      TH1F *h0= (TH1F*)hfile->Get(hname0);
      if (1) std::cout << "errB_HF depth=" << depth << ". get 2D histos "
		       << hname1 << " and " << hname0 << "\n";
      if (!h1 || !h0) {
	TPaveText *ptext= new TPaveText(0.05,0.85,0.95,0.95);
	ptext->AddText("Missing histo");
	if (!h1) {
	  std::cout << "\tfailed to get " << hname1 << "\n";
	  ptext->AddText(hname1);
	}
	if (!h0) {
	  std::cout << "\tfailed to get " << hname0 << "\n";
	  ptext->AddText(hname0);
	}
	ptext->Draw();
      }
      else {
	gPad->SetGridx();
	gPad->SetGridy();
	//	gPad->SetLogy();
	hRate2orig = (TH1F*)h1->Clone(Form("Rate2orig_%d",depth));
	hRate2orig->Divide(h1,h0, 1, 1, "B");

	TH1F* hRate2=NULL;
	if (lastLumiBin>1) {
	  if (!copyContents(&hRate2,Form("Rate2_%d",depth),"",hRate2orig,lastLumiBin)) {
	    std::cout << "code failed" << std::endl;
	    gSystem->Exit(1);
	  }
	}
	else hRate2=hRate2orig;

	hRate2->SetTitle(Form("Depth %d \b",depth));
	hRate2->SetMarkerStyle(20);
	hRate2->SetMarkerSize(0.8);
	//	hRate2->GetZaxis()->SetLabelSize(0.04);
	hRate2->SetXTitle(Form("<ErrorB>(ev.in LS & ch.) - HF depth%d -    iLS \b",depth));
	hRate2->SetMarkerColor(2);
	hRate2->SetLineColor(0);
	hRate2->Draw("Error");


	if(LSofFirstErrB == -1) {
	  int nx = hRate2->GetXaxis()->GetNbins();
	  for (int i=1;i<=nx;i++) {
	    double ccc1 =  hRate2->GetBinContent(i);
	    if(ccc1>0.) {
	      cout<<"****************>>>>>>>>>>> ErrB_HF bad LS start at iLS = "<<i<<" with rate = "<< ccc1 <<endl;
	      LSofFirstErrB = i;
	      break;
	    }
	  }
	}	
	

	delete h1;
	delete h0;
	if (hRate2!=hRate2orig) { delete hRate2orig; hRate2orig=hRate2; }
      }

      cHB->Update();
      cHB->Print(Form("HistErrB_HF_%d.png",depth));
      cHB->Clear();
      if (h2Cefz6) delete h2Cefz6;
      if (hRate2orig) delete hRate2orig;
    }
  } // ErrorsB in HF



//======================================================================
// Creating each test kind for each subdet html pages:
  std::string raw_class;  
  int ind = 0; 
  ofstream htmlFile;
  for (int test=0;test<=5;test++) { //Test: 0,   
      for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HO, 4-HF
           
           if (test==0){
	      if (sub==1) {htmlFile.open("HB_CapID.html");}
              if (sub==2) {htmlFile.open("HE_CapID.html");}
              if (sub==3) {htmlFile.open("HO_CapID.html");}
              if (sub==4) {htmlFile.open("HF_CapID.html");}
	   }	   
	   if (test==1){
	      if (sub==1) {htmlFile.open("HB_ADCampl.html");}
              if (sub==2) {htmlFile.open("HE_ADCampl.html");}
              if (sub==3) {htmlFile.open("HO_ADCampl.html");}
              if (sub==4) {htmlFile.open("HF_ADCampl.html");}
	   }
	   if (test==2){
	      if (sub==1) {htmlFile.open("HB_Width.html");}
              if (sub==2) {htmlFile.open("HE_Width.html");}
              if (sub==3) {htmlFile.open("HO_Width.html");}
              if (sub==4) {htmlFile.open("HF_Width.html");}
	   }
	   if (test==3){
	      if (sub==1) {htmlFile.open("HB_Ratio.html");}
              if (sub==2) {htmlFile.open("HE_Ratio.html");}
              if (sub==3) {htmlFile.open("HO_Ratio.html");}
              if (sub==4) {htmlFile.open("HF_Ratio.html");}
	   }
	   if (test==4){
	      if (sub==1) {htmlFile.open("HB_Tmean.html");}
              if (sub==2) {htmlFile.open("HE_Tmean.html");}
              if (sub==3) {htmlFile.open("HO_Tmean.html");}
              if (sub==4) {htmlFile.open("HF_Tmean.html");}
	   }
	   if (test==5){
	      if (sub==1) {htmlFile.open("HB_Tmax.html");}
              if (sub==2) {htmlFile.open("HE_Tmax.html");}
              if (sub==3) {htmlFile.open("HO_Tmax.html");}
              if (sub==4) {htmlFile.open("HF_Tmax.html");}
	   }

           htmlFile << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
           htmlFile << "<head>"<< std::endl;
           htmlFile << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
           htmlFile << "<title> Certification Monitoring Tool </title>"<< std::endl;
           htmlFile << "<style type=\"text/css\">"<< std::endl;
           htmlFile << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
           htmlFile << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
           htmlFile << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
           htmlFile << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
           htmlFile << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
           htmlFile << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
	   htmlFile << "   td.s5 { font-family: arial, arial ce, helvetica; background-color: #00FF00; }"<< std::endl;
           htmlFile << "   td.s6 { font-family: arial, arial ce, helvetica; background-color: #FF0000; }"<< std::endl;
           htmlFile << "</style>"<< std::endl;
           htmlFile << "<body>"<< std::endl;

           if (test==0) {     
              if (sub==1) htmlFile << "<h1> Cap ID estimator for HB, RUN = "<< runnumber <<" </h1>"<< std::endl; 
              if (sub==2) htmlFile << "<h1> Cap ID estimator for HE, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==3) htmlFile << "<h1> Cap ID estimator for HO, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==4) htmlFile << "<h1> Cap ID estimator for HF, RUN = "<< runnumber <<" </h1>"<< std::endl; 
           }
           if (test==1) {     
              if (sub==1) htmlFile << "<h1> Mean ADC Amplitude estimator for HB, RUN = "<< runnumber <<" </h1>"<< std::endl; 
              if (sub==2) htmlFile << "<h1> Mean ADC Amplitude estimator for HE, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==3) htmlFile << "<h1> Mean ADC Amplitude estimator for HO, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==4) htmlFile << "<h1> Mean ADC Amplitude estimator for HF, RUN = "<< runnumber <<" </h1>"<< std::endl; 
           }     
           if (test==2) {     
              if (sub==1) htmlFile << "<h1> Width estimator for HB, RUN = "<< runnumber <<" </h1>"<< std::endl; 
              if (sub==2) htmlFile << "<h1> Width estimator for HE, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==3) htmlFile << "<h1> Width estimator for HO, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==4) htmlFile << "<h1> Width estimator for HF, RUN = "<< runnumber <<" </h1>"<< std::endl; 
           }     
           if (test==3) {     
              if (sub==1) htmlFile << "<h1> Ratio estimator for HB, RUN = "<< runnumber <<" </h1>"<< std::endl; 
              if (sub==2) htmlFile << "<h1> Ratio estimator for HE, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==3) htmlFile << "<h1> Ratio estimator for HO, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==4) htmlFile << "<h1> Ratio estimator for HF, RUN = "<< runnumber <<" </h1>"<< std::endl; 
           }     
           if (test==4) {     
              if (sub==1) htmlFile << "<h1> Mean bin timing estimator for HB, RUN = "<< runnumber <<" </h1>"<< std::endl; 
              if (sub==2) htmlFile << "<h1> Mean bin timing estimator for HE, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==3) htmlFile << "<h1> Mean bin timing estimator for HO, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==4) htmlFile << "<h1> Mean bin timing estimator for HF, RUN = "<< runnumber <<" </h1>"<< std::endl; 
           }     
           if (test==5) {     
              if (sub==1) htmlFile << "<h1> Maximum bin timing estimator for HB, RUN = "<< runnumber <<" </h1>"<< std::endl; 
              if (sub==2) htmlFile << "<h1> Maximum bin timing estimator for HE, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==3) htmlFile << "<h1> Maximum bin timing estimator for HO, RUN = "<< runnumber <<" </h1>"<< std::endl;
              if (sub==4) htmlFile << "<h1> Maximum bin timing estimator for HF, RUN = "<< runnumber <<" </h1>"<< std::endl; 
           }     
          
           htmlFile << "<br>"<< std::endl;
           if (test==0) {
	      htmlFile << "<h2> 0. Rate of CapID failures over all events of Run </h2>"<< std::endl;
              htmlFile << "<h3> Channel legend: white - good, other colour - bad.  </h3>"<< std::endl; 
                  if (sub==1) htmlFile << " <img src=\"MapCapIdErrorHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"MapCapIdErrorHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"MapCapIdErrorHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"MapCapIdErrorHF.png\" />" << std::endl;
	   }
	      if (test !=0) htmlFile << "<h2> 1. Distribution of estimator averaged over events in LS, histogramed over all channels and all LSs </h2>"<< std::endl;
	      if (test ==0) {
	          if (sub==1) htmlFile << "<h2> 1.  Average Nbcs for only LS containing events with abnormal number of Bad channels >"<<CutAb[sub][1]<<" (Depth1), "<<CutAb[sub][2] <<" (Depth2).</h2>"<< std::endl;
                  if (sub==2) htmlFile << "<h2> 1.  Average Nbcs for only LS containing events with abnormal number of Bad channels >"<<CutAb[sub][1]<<" (Depth1), "<<CutAb[sub][2] <<" (Depth2), "<<CutAb[sub][3] <<" (Depth3).</h2>"<< std::endl;
                  if (sub==3) htmlFile << "<h2> 1.  Average Nbcs for only LS containing events with abnormal number of Bad channels >"<<CutAb[sub][1]<<" (Depth4).</h2>"<< std::endl;
                  if (sub==4) htmlFile << "<h2> 1.  Average Nbcs for only LS containing events with abnormal number of Bad channels >"<<CutAb[sub][1]<<" (Depth1), "<<CutAb[sub][2] <<" (Depth2).</h2>"<< std::endl;
              
	      }
	      
	      if (test !=0) htmlFile << "<h3> Legend: Overflow corresponds to BAD LS candidates.</h3>"<< std::endl;
	      if (test ==0) htmlFile << "<h3> Legend: dots correspond to BAD LS candidates.</h3>"<< std::endl; 

              if (test==0){
                  if (sub==1) htmlFile << " <img src=\"Hist_CAPID_Abnorm_HB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"Hist_CAPID_Abnorm_HE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"Hist_CAPID_Abnorm_HO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"Hist_CAPID_Abnorm_HF.png\" />" << std::endl;
              }
              if (test==1){
                  if (sub==1) htmlFile << " <img src=\"H_ADCamplHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"H_ADCamplHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"H_ADCamplHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"H_ADCamplHF.png\" />" << std::endl;
              }
	      if (test==2){
                  if (sub==1) htmlFile << " <img src=\"H_WidthHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"H_WidthHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"H_WidthHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"H_WidthHF.png\" />" << std::endl;
              }
	      if (test==3){
                  if (sub==1) htmlFile << " <img src=\"H_RatioHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"H_RatioHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"H_RatioHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"H_RatioHF.png\" />" << std::endl;
              }
	      if (test==4){
                  if (sub==1) htmlFile << " <img src=\"H_TmeanHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"H_TmeanHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"H_TmeanHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"H_TmeanHF.png\" />" << std::endl;
              }
	      if (test==5){
                  if (sub==1) htmlFile << " <img src=\"H_TmaxHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"H_TmaxHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"H_TmaxHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"H_TmaxHF.png\" />" << std::endl;
              }	 
              htmlFile << "<br>"<< std::endl; 
       
       
       	       if (test ==0) htmlFile << "<h2> 2a.  Number of bad channels per event distribution in Run</h2>"<< std::endl;
	       if (test ==0) htmlFile << "<h3> Legends: dots correspond to BAD LS candidates.  </h3>"<< std::endl;
	       if (test ==0){
                    if (sub==1) htmlFile << " <img src=\"HistNBadChsHB.png\" />" << std::endl; 
                    if (sub==2) htmlFile << " <img src=\"HistNBadChsHE.png\" />" << std::endl; 
                    if (sub==3) htmlFile << " <img src=\"HistNBadChsHO.png\" />" << std::endl; 
                    if (sub==4) htmlFile << " <img src=\"HistNBadChsHF.png\" />" << std::endl;
               }       
	      
	      if (test !=0) htmlFile << "<h2> 2. Estimator averaged over all events in the RUN for entries in overflow and underflow of corresponding histogram above </h2>"<< std::endl;
//              if (test !=0) htmlFile << "<h2> 2. Estimator averaged over all events in the RUN </h2>"<< std::endl;
	      if (test ==0) htmlFile << "<h2> 2b. Averaged number of bad channels for each LS </h2>"<< std::endl;
//              if (test !=0) htmlFile << "<h3> Channel legend: white - good, other colour - bad.  </h3>"<< std::endl;
	      if (test ==0) {
                 if (sub==1) htmlFile << "<h3> Legends: dots selected with following cuts: <td class=\"s6\" align=\"center\">"<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2) correspond BAD LS.</td></h3>"<< std::endl;
                 if (sub==2) htmlFile << "<h3> Legends: dots selected with following cuts: "<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2), "<<Cut0[test][sub][3]<<" (Depth3) correspond BAD LS.</h3>"<< std::endl;
	         if (sub==3) htmlFile << "<h3> Legends: dots selected with following cuts: "<<Cut0[test][sub][4]<<" (Depth4) correspond BAD LS.</h3>"<< std::endl;
                 if (sub==4) htmlFile << "<h3> Legends: dots selected with following cuts: "<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2) correspond BAD LS.</h3>"<< std::endl;
              }
              if (test==0){
                  if (sub==1) htmlFile << " <img src=\"HistNBCMNHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistNBCMNHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistNBCMNHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistNBCMNHF.png\" />" << std::endl;
              }
              if (test==1){
                  if (sub==1) htmlFile << " <img src=\"MapADCamplHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"MapADCamplHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"MapADCamplHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"MapADCamplHF.png\" />" << std::endl;
              }
	      if (test==2){
                  if (sub==1) htmlFile << " <img src=\"MapWidthHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"MapWidthHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"MapWidthHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"MapWidthHF.png\" />" << std::endl;
              }
	      if (test==3){
                  if (sub==1) htmlFile << " <img src=\"MapRatioHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"MapRatioHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"MapRatioHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"MapRatioHF.png\" />" << std::endl;
              }
	      if (test==4){
                  if (sub==1) htmlFile << " <img src=\"MapTmeanHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"MapTmeanHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"MapTmeanHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"MapTmeanHF.png\" />" << std::endl;
              }
	      if (test==5){
                  if (sub==1) htmlFile << " <img src=\"MapTmaxHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"MapTmaxHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"MapTmaxHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"MapTmaxHF.png\" />" << std::endl;
              }	 
              htmlFile << "<br>"<< std::endl;
             
	    	      
	      if (test !=0) htmlFile << "<h2> 3. Distribution of estimator averaged over events in LS and over all channels for each LS </h2>"<< std::endl;
	      if (test ==0) {
	          if (sub==1) htmlFile << "<h2> 3.  Portion of events with Nbcs>"<<CutPo[sub][1]<<" (Depth1), "<<CutPo[sub][2] <<" (Depth2) in each LS.</h2>"<< std::endl;
                  if (sub==2) htmlFile << "<h2> 3.  Portion of events with Nbcs>"<<CutPo[sub][1]<<" (Depth1), "<<CutPo[sub][2] <<" (Depth2), "<<CutPo[sub][3] <<" (Depth3) in each LS.</h2>"<< std::endl;
                  if (sub==3) htmlFile << "<h2> 3.  Portion of events with Nbcs>"<<CutPo[sub][1]<<" (Depth4) in each LS.</h2>"<< std::endl;
                  if (sub==4) htmlFile << "<h2> 3.  Portion of events with Nbcs>"<<CutPo[sub][1]<<" (Depth1), "<<CutPo[sub][2] <<" (Depth2) in each LS.</h2>"<< std::endl;
                  htmlFile << "<h3> Legend: dots correspond to BAD LS candidates.</h3>"<< std::endl;
              }
              if (test !=0){
                 if (sub==1) htmlFile << "<h3> Legends:  dots selected with following cuts: <td class=\"s6\" align=\"center\">"<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2) correspond BAD LS.</td></h3>"<< std::endl;
                 if (sub==2) htmlFile << "<h3> Legends:  dots selected with following cuts: "<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2), "<<Cut0[test][sub][3]<<" (Depth3) correspond BAD LS. </h3>"<< std::endl;
	         if (sub==3) htmlFile << "<h3> Legends:  dots selected with following cuts: "<<Cut0[test][sub][4]<<" (Depth4) correspond BAD LS. </h3>"<< std::endl;
                 if (sub==4) htmlFile << "<h3> Legends:  dots selected with following cuts: "<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2) correspond BAD LS. </h3>"<< std::endl;
              }
              if (test==0){
                  if (sub==1) htmlFile << " <img src=\"HistPortHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistPortHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistPortHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistPortHF.png\" />" << std::endl;
              }
              if (test==1){
                  if (sub==1) htmlFile << " <img src=\"HistADCamplHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistADCamplHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistADCamplHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistADCamplHF.png\" />" << std::endl;
              }
	      if (test==2){
                  if (sub==1) htmlFile << " <img src=\"HistWidthHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistWidthHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistWidthHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistWidthHF.png\" />" << std::endl;
              }
	      if (test==3){
                  if (sub==1) htmlFile << " <img src=\"HistRatioHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistRatioHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistRatioHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistRatioHF.png\" />" << std::endl;
              }
	      if (test==4){
                  if (sub==1) htmlFile << " <img src=\"HistTmeanHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistTmeanHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistTmeanHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistTmeanHF.png\" />" << std::endl;
              }
	      if (test==5){
                  if (sub==1) htmlFile << " <img src=\"HistTmaxHB.png\" />" << std::endl; 
                  if (sub==2) htmlFile << " <img src=\"HistTmaxHE.png\" />" << std::endl; 
                  if (sub==3) htmlFile << " <img src=\"HistTmaxHO.png\" />" << std::endl; 
                  if (sub==4) htmlFile << " <img src=\"HistTmaxHF.png\" />" << std::endl;
              }	     	    	      
              htmlFile << "<br>"<< std::endl;

	      // Special section for the HF (Andrius)
	      int flagSpecHF=0;
	      if ((test==1) && (sub==4)) {
		flagSpecHF+=1;
		htmlFile << "<h2> 4. Error type A & B</h2>\n";
		htmlFile << "<h2> 4a. Error type A</h2>\n";
		htmlFile << "<h3>Mean of max difference between dependencies to be within: 0.8-2.4 (p-p collisions) </h3>\n";
		htmlFile << " <img src=\"HistErrA_HF.png\" />\n";
		htmlFile << "<br>\n";
		if (flagErrAB_HF[0]==-1) htmlFile<<"<h3>test was not possible</h3>\n";
		else if (flagErrAB_HF[0]==0) htmlFile<<"<h3> Fine:NoErrorA_HF(=Mean of max difference " << avedelta  << "  is within 0.8-2.4) </h3>\n";
		else if (flagErrAB_HF[0]==1) htmlFile<<"<<h3>font color=\"red\">ErrorA_HF is available once Mean of max difference " << avedelta  << " is out 0.8-2.4 (p-p collisions)</font></h3>\n";
		else htmlFile<<"<h3>auto-interpretation is not available</h3>\n";


		htmlFile << "<h2> 4b. Error type B\n";
		htmlFile << "<h3> ErrorB: digi-collection size !=4. </h3>\n";
		htmlFile << " <img src=\"HistErrB_HF_1.png\" />\n<br>\n";
		htmlFile << " <img src=\"HistErrB_HF_2.png\" />\n<br>\n";
		htmlFile << "<br>\n";
		htmlFile <<"<h3> if Error type B is available, it start from:    " << LSofFirstErrB  << "  LS </h3>\n";
		htmlFile << "<br>\n";
	      }

	      // Continue with common sections
              if (sub==1) { 
	          htmlFile << "<h2> 4.Lumisection Status for HB </h2>"<< std::endl;
	          htmlFile << "<h3> Legends: Red boxes correspond BAD LS selected with following cuts: <td class=\"s6\" align=\"center\">"<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2). </td></h3>"<< std::endl;
	      }  
              if (sub==2) {
	          htmlFile << "<h2> 4.Lumisection Status for HE </h2>"<< std::endl;
		  htmlFile << "<h3> Legends: Red boxes correspond BAD LS selected with following cuts: "<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2), "<<Cut0[test][sub][3]<<" (Depth3). </h3>"<< std::endl;
              }
	      if (sub==3) {
		htmlFile << Form("<h2> %d.Lumisection Status for HO </h2>",4+flagSpecHF)<< std::endl;
		 htmlFile << "<h3> Legends: Red boxes correspond BAD LS selected with following cuts: "<<Cut0[test][sub][4]<<" (Depth4). </h3>"<< std::endl;
	      }
              if (sub==4) {
	          htmlFile << "<h2> 4.Lumisection Status for HF </h2>"<< std::endl; 
	          htmlFile << "<h3> Legends: Red boxes correspond BAD LS selected with following cuts: "<<Cut0[test][sub][1]<<" (Depth1), "<<Cut0[test][sub][2]<<" (Depth2). </h3>"<< std::endl;
	      }	  	      
	      htmlFile << "<br>"<< std::endl;
              htmlFile << "<table>"<< std::endl;        
              htmlFile << "<tr>";
              htmlFile << "<td class=\"s4\" align=\"center\">LS</td>"    << std::endl;
//              htmlFile << "<td class=\"s1\" align=\"center\">LS</td>"  << std::endl;
              htmlFile << "<td class=\"s1\" align=\"center\">Number of events</td>"  << std::endl;
              if (test==0) for (int k=k_min[sub];k<=k_max[sub]; k++) htmlFile << "<td class=\"s1\" align=\"center\">< Nbcs > Depth "<< k <<" </td>"  << std::endl;
              if (test==1) for (int k=k_min[sub];k<=k_max[sub]; k++) htmlFile << "<td class=\"s1\" align=\"center\">< A > Depth "<< k <<" </td>"  << std::endl;
              if (test==2) for (int k=k_min[sub];k<=k_max[sub]; k++) htmlFile << "<td class=\"s1\" align=\"center\">< W > Depth "<< k <<" </td>"  << std::endl;
              if (test==3) for (int k=k_min[sub];k<=k_max[sub]; k++) htmlFile << "<td class=\"s1\" align=\"center\">< R > Depth "<< k <<" </td>"  << std::endl;
              if (test==4) for (int k=k_min[sub];k<=k_max[sub]; k++) htmlFile << "<td class=\"s1\" align=\"center\">< TSn > Depth "<< k <<" </td>"  << std::endl;
              if (test==5) for (int k=k_min[sub];k<=k_max[sub]; k++) htmlFile << "<td class=\"s1\" align=\"center\">< TSx > Depth "<< k <<" </td>"  << std::endl;
              htmlFile << "</tr>"   << std::endl;    
              ind = 0;              
	      for (int i=1;i<=MaxLum;i++) {
                  if ((ind%2)==1)   raw_class="<td class=\"s2\" align=\"center\">";
                  else              raw_class="<td class=\"s3\" align=\"center\">";              
	          htmlFile << "<tr>"<< std::endl;
                  htmlFile << "<td class=\"s4\" align=\"center\">" << i <<"</td>"<< std::endl;
//                  htmlFile << raw_class<< LumLum->GetBinContent(i)<<"</td>"<< std::endl;
                  htmlFile << raw_class<< LumiEv->GetBinContent(i)<<"</td>"<< std::endl;	      
                  for (int k=k_min[sub];k<=k_max[sub]; k++) {	              
		      if (HistNumBadChanDepth[test][sub][k]->GetBinContent(i) > Cut0[test][sub][k]) 
		            htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][sub][k]->GetBinContent(i)<<"</td>"<< std::endl;
	              else  htmlFile << raw_class <<HistNumBadChanDepth[test][sub][k]->GetBinContent(i)<<"</td>"<< std::endl;
		  }
	          htmlFile << "</tr>" << std::endl;
                  ind+=1;
	      }	                
              htmlFile << "</table>" << std::endl; 
           htmlFile.close();	   
       }// sub
    }//test
//===============================================================================


//======================================================================
// Creating tests  html pages:
 
 for (int test=0;test<=5;test++) { //Test: 0,   
     if (test==0) htmlFile.open("CapID_GL.html");
     if (test==1) htmlFile.open("ADCampl_GL.html");
     if (test==2) htmlFile.open("Width_GL.html");
     if (test==3) htmlFile.open("Ratio_GL.html");
     if (test==4) htmlFile.open("Tmean_GL.html");
     if (test==5) htmlFile.open("Tmax_GL.html");
     
     htmlFile << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
     htmlFile << "<head>"<< std::endl;
     htmlFile << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
     htmlFile << "<title> Certificate Monitoring Tool </title>"<< std::endl;
     htmlFile << "<style type=\"text/css\">"<< std::endl;
     htmlFile << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
     htmlFile << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
     htmlFile << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
     htmlFile << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
     htmlFile << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
     htmlFile << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
     htmlFile << "   td.s5 { font-family: arial, arial ce, helvetica; background-color: #00FF00; }"<< std::endl;
     htmlFile << "   td.s6 { font-family: arial, arial ce, helvetica; background-color: #FF0000; }"<< std::endl;
     htmlFile << "</style>"<< std::endl;
     htmlFile << "<body>"<< std::endl;
     if (test==0) htmlFile << "<h1> CAP ID ERRORS, GLOBAL RUN = "<< runnumber <<" </h1>"<< std::endl; 
     if (test==1) htmlFile << "<h1> ADC AMPLITIDE, GLOBAL RUN = "<< runnumber <<" </h1>"<< std::endl; 
     if (test==2) htmlFile << "<h1> WIDTH, GLOBAL RUN = "<< runnumber <<" </h1>"<< std::endl;
     if (test==3) htmlFile << "<h1> RATIO, GLOBAL RUN = "<< runnumber <<" </h1>"<< std::endl;
     if (test==4) htmlFile << "<h1> TIMING MEAN, GLOBAL RUN = "<< runnumber <<" </h1>"<< std::endl;
     if (test==5) htmlFile << "<h1> TIMING MAX, GLOBAL RUN = "<< runnumber <<" </h1>"<< std::endl;  
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h2> 1.  Map of bad channels with this criterion for whole HCAL </h2>"<< std::endl; 
     htmlFile << "<h3> Channel legend: white - good, other colour - bad.  </h3>"<< std::endl;  
     htmlFile << "<br>"<< std::endl;      
     if (test==0) htmlFile << " <img src=\"MapCapIdError.png\" />" << std::endl;
     if (test==1) htmlFile << " <img src=\"MapADCAmpl.png\" />" << std::endl;      
     if (test==2) htmlFile << " <img src=\"MapWidth.png\" />" << std::endl;      
     if (test==3) htmlFile << " <img src=\"MapRatio.png\" />" << std::endl;      
     if (test==4) htmlFile << " <img src=\"MapTmean.png\" />" << std::endl;   
     if (test==5) htmlFile << " <img src=\"MapTmax.png\" />" << std::endl;         
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h2> 2.  Average value for whole HCAL in each LS</h2>"<< std::endl;   
     htmlFile << "<br>"<< std::endl;      
     if (test==0) htmlFile << " <img src=\"HistCapID.png\" />" << std::endl; 
     if (test==1) htmlFile << " <img src=\"HistADCAmpl.png\" />" << std::endl;      
     if (test==2) htmlFile << " <img src=\"HistWidth.png\" />" << std::endl;      
     if (test==3) htmlFile << " <img src=\"HistRatio.png\" />" << std::endl;      
     if (test==4) htmlFile << " <img src=\"HistTmean.png\" />" << std::endl;   
     if (test==5) htmlFile << " <img src=\"HistTmax.png\" />" << std::endl;         
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h2> 3. Status of subdetectors </h2>"<< std::endl;   
     htmlFile << "<table width=\"400\">"<< std::endl;
     htmlFile << "<tr>"<< std::endl;
     if (test==0){
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HB_CapID.html\">HB</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HE_CapID.html\">HE</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HO_CapID.html\">HO</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HF_CapID.html\">HF</a></td>"<< std::endl;
     }
     if (test==1){
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HB_ADCampl.html\">HB</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HE_ADCampl.html\">HE</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HO_ADCampl.html\">HO</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HF_ADCampl.html\">HF</a></td>"<< std::endl;
     }
     if (test==2){
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HB_Width.html\">HB</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HE_Width.html\">HE</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HO_Width.html\">HO</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HF_Width.html\">HF</a></td>"<< std::endl;
     }
     if (test==3){
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HB_Ratio.html\">HB</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HE_Ratio.html\">HE</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HO_Ratio.html\">HO</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HF_Ratio.html\">HF</a></td>"<< std::endl;
     }
     if (test==4){
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HB_Tmean.html\">HB</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HE_Tmean.html\">HE</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HO_Tmean.html\">HO</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HF_Tmean.html\">HF</a></td>"<< std::endl;
     }
     if (test==5){
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HB_Tmax.html\">HB</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HE_Tmax.html\">HE</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HO_Tmax.html\">HO</a></td>"<< std::endl;
         htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HF_Tmax.html\">HF</a></td>"<< std::endl;
     }
     
     htmlFile << "</tr>"<< std::endl;
     htmlFile << "</table>"<< std::endl;
     htmlFile << "<br>"<< std::endl;    


     htmlFile << "<h2> 4. Candidates for BAD LS</h3>"<< std::endl;
     htmlFile << "<table>"<< std::endl;     
     htmlFile << "<tr>";
     htmlFile << "<td class=\"s4\" align=\"center\">LS</td>"    << std::endl;
//     htmlFile << "<td class=\"s1\" align=\"center\">LS</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">Number of events</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HB Depth 1</td>"<< std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HB Depth 2</td>"  << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HE Depth 1</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HE Depth 2</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HE Depth 3</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HO Depth 4</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HF Depth 1</td>"   << std::endl;
     htmlFile << "<td class=\"s1\" align=\"center\">HF Depth 2</td>"   << std::endl;
     htmlFile << "</tr>"   << std::endl;     
   
     ind = 0;
     
     for (int i=1;i<=MaxLum;i++) {
         if ((ind%2)==1)   raw_class="<td class=\"s2\" align=\"center\">";
         else              raw_class="<td class=\"s3\" align=\"center\">";                    
         int met =0;
	 for (int sub=1;sub<=4;sub++) {  //Subdetector: 1-HB, 2-HE, 3-HF, 4-HO
	     for (int k=k_min[sub];k<=k_max[sub]; k++) 
	        if (HistNumBadChanDepth[test][sub][k]->GetBinContent(i) > Cut0[test][sub][k]) met = 1; 
	 }
         if (met==1) {
	    htmlFile << "<tr>"<< std::endl;
            htmlFile << "<td class=\"s4\" align=\"center\">" << i <<"</td>"<< std::endl;
//            htmlFile << raw_class<< LumLum->GetBinContent(i)<<"</td>"<< std::endl;
            htmlFile << raw_class<< LumiEv->GetBinContent(i)<<"</td>"<< std::endl;	
	    if (HistNumBadChanDepth[test][1][1]->GetBinContent(i) > Cut0[test][1][1])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][1][1]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][1][1]->GetBinContent(i)<<"</td>"<< std::endl;   
	    if (HistNumBadChanDepth[test][1][2]->GetBinContent(i) > Cut0[test][1][2])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][1][2]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][1][2]->GetBinContent(i)<<"</td>"<< std::endl;	    
	    if (HistNumBadChanDepth[test][2][1]->GetBinContent(i) > Cut0[test][2][1])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][2][1]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][2][1]->GetBinContent(i)<<"</td>"<< std::endl;	    
	    if (HistNumBadChanDepth[test][2][2]->GetBinContent(i) > Cut0[test][2][2])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][2][2]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][2][2]->GetBinContent(i)<<"</td>"<< std::endl;	    
	    if (HistNumBadChanDepth[test][2][3]->GetBinContent(i) > Cut0[test][2][3])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][2][3]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][2][3]->GetBinContent(i)<<"</td>"<< std::endl;	    
	    if (HistNumBadChanDepth[test][3][4]->GetBinContent(i) > Cut0[test][3][4])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][3][4]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][3][4]->GetBinContent(i)<<"</td>"<< std::endl;	    
	    if (HistNumBadChanDepth[test][4][1]->GetBinContent(i) > Cut0[test][4][1])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][4][1]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][4][1]->GetBinContent(i)<<"</td>"<< std::endl;	  
	    if (HistNumBadChanDepth[test][4][2]->GetBinContent(i) > Cut0[test][4][2])  htmlFile << "<td class=\"s6\" align=\"center\">"<<HistNumBadChanDepth[test][4][2]->GetBinContent(i)<<"</td>"<< std::endl;
	    else htmlFile <<raw_class<<HistNumBadChanDepth[test][4][2]->GetBinContent(i)<<"</td>"<< std::endl;
	    ind+=1;
	 }   
	htmlFile << "</tr>" << std::endl;
         
     }		

     htmlFile << "</table>" << std::endl;
     htmlFile << "<br>"<< std::endl;

     htmlFile << "</body> " << std::endl;
     htmlFile << "</html> " << std::endl;
     htmlFile.close();
  }
      
//======================================================================


//======================================================================
// Creating description html file: 
     htmlFile.open("HELP.html");  
     htmlFile << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
     htmlFile << "<head>"<< std::endl;
     htmlFile << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
     htmlFile << "<title> Certificate Monitoring Tool </title>"<< std::endl;
     htmlFile << "<style type=\"text/css\">"<< std::endl;
     htmlFile << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
     htmlFile << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
     htmlFile << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
     htmlFile << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
     htmlFile << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
     htmlFile << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
     htmlFile << "</style>"<< std::endl;
     htmlFile << "<body>"<< std::endl;
     htmlFile << "<h1>  Description of Remote Monitoring Tool criteria for bad channel selection</h1>"<< std::endl;
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h3> -  CAPID Errors assuming we inspect CAPID non-rotation,error & validation bits, and for this criterion - no need to apply any cuts to select bcs.</h3> "<< std::endl;
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h3> - Amplitude is full amplitude collected over all time slices </h3> "<< std::endl;
     htmlFile << "<h3> - Ratio criterion is where we define as a bad, the channels, for which the signal portion in 4 middle TSs(plus one, minus two around TS with maximal amplitude) is out of some range of reasonable values </h3> "<< std::endl;
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h3> - Width of shape distribution. Width is defined as square root from dispersion. </h3> "<< std::endl;
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h3> - Mean TS time slice position of adc signal. </h3> "<< std::endl;
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<h3> - TS number of maximum signal </h3> "<< std::endl;
     htmlFile << "<br>"<< std::endl; 
     htmlFile << "<h3>  For more details see https://twiki.cern.ch/twiki/bin/viewauth/CMS/HcalRemoteCertificationMonitoring </h3> "<< std::endl;   
     htmlFile << "</body> " << std::endl;
     htmlFile << "</html> " << std::endl; 
     htmlFile.close();

//======================================================================

//======================================================================
// Creating main html file: 
  
     htmlFile.open("LumiList.html");  
     htmlFile << "</html><html xmlns=\"http://www.w3.org/1999/xhtml\">"<< std::endl;
     htmlFile << "<head>"<< std::endl;
     htmlFile << "<meta http-equiv=\"Content-Type\" content=\"text/html\"/>"<< std::endl;
     htmlFile << "<title> Certification Monitoring Tool </title>"<< std::endl;
     htmlFile << "<style type=\"text/css\">"<< std::endl;
     htmlFile << " body,td{ background-color: #FFFFCC; font-family: arial, arial ce, helvetica; font-size: 12px; }"<< std::endl;
     htmlFile << "   td.s0 { font-family: arial, arial ce, helvetica; }"<< std::endl;
     htmlFile << "   td.s1 { font-family: arial, arial ce, helvetica; font-weight: bold; background-color: #FFC169; text-align: center;}"<< std::endl;
     htmlFile << "   td.s2 { font-family: arial, arial ce, helvetica; background-color: #eeeeee; }"<< std::endl;
     htmlFile << "   td.s3 { font-family: arial, arial ce, helvetica; background-color: #d0d0d0; }"<< std::endl;
     htmlFile << "   td.s4 { font-family: arial, arial ce, helvetica; background-color: #FFC169; }"<< std::endl;
     htmlFile << "   td.s5 { font-family: arial, arial ce, helvetica; background-color: #00FF00; }"<< std::endl;
     htmlFile << "   td.s6 { font-family: arial, arial ce, helvetica; background-color: #FF0000; }"<< std::endl;
     
     htmlFile << "</style>"<< std::endl;
     htmlFile << "<body>"<< std::endl;
     
     htmlFile << "<h1> Certification Monitoring Tool, GLOBAL RUN = "<< runnumber <<". </h1>"<< std::endl;
     htmlFile << "<br>"<< std::endl; 
       
     htmlFile << "<h2> 1. General information </h2>"<< std::endl;   
     htmlFile << " <img src=\"LumiEvent.png\" />" << std::endl;      
     htmlFile << "<br>"<< std::endl;
     
     
     htmlFile << "<h2> 2. HCAL status for different criteria </h2>"<< std::endl;   
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/HELP.html\"> Description of criteria for bad channel selection</a></td>"<< std::endl;
     htmlFile << "<br>"<< std::endl;
     htmlFile << "<table width=\"400\">"<< std::endl;
     htmlFile << "<tr>"<< std::endl;
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/CapID_GL.html\">Cap ID errors</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/ADCampl_GL.html\">Amplitude</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/Width_GL.html\">Width</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/Ratio_GL.html\">Ratio</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/Tmean_GL.html\">TS mean</a></td>"<< std::endl;
     htmlFile << "  <td><a href=\"https://cms-cpt-software.web.cern.ch/cms-cpt-software/General/Validation/SVSuite/HcalRemoteMonitoring/CMT/GLOBAL_"<<runnumber<<"/Tmax_GL.html\">TS max</a></td>"<< std::endl;
     htmlFile << "</tr>"<< std::endl;
     htmlFile << "</table>"<< std::endl;
     htmlFile << "<br>"<< std::endl;    
     htmlFile << "</body> " << std::endl;
     htmlFile << "</html> " << std::endl; 
     htmlFile.close();  
//======================================================================
     
//======================================================================
// Close and delete all possible things:
    hfile->Close();    
//  hfile->Delete();
//  Exit Root
    gSystem->Exit(0);
//======================================================================

}


// ------------------------------------------------------------

int copyContents(TH1F **hDest, TString hname, TString htitle,
		   const TH1F *hSrc, int lastBin)
{
  if (lastBin > hSrc->GetNbinsX()) {
    std::cout << "copyContents from " << hSrc->GetName() << ": histo has "
	      << hSrc->GetNbinsX() << " bins, when lastBin=" << lastBin
	      << " was requested\n";
    return 0;
  }

  (*hDest)= new TH1F(hname,htitle,lastBin,0,lastBin);
  (*hDest)->SetDirectory(0);
  (*hDest)->SetStats(0);

  for (int ibin=1; ibin<=lastBin; ibin++) {
    (*hDest)->SetBinContent(ibin, hSrc->GetBinContent(ibin));
    (*hDest)->SetBinError(ibin, hSrc->GetBinError(ibin));
  }
  return 1;
}

// ------------------------------------------------------------
