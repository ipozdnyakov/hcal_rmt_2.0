

#include <iostream>
#include <fstream>
#include <sstream>
#include "TH1.h"
#include "TH2.h"
#include "TLatex.h"
#include "TFrame.h"
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

using namespace std;

// ok change
//     int
//            main(int argc, char *argv[])
//            {
//

//______________________________________________________________________________
TCanvas *c1;
TGraph *gr1[100];
const Int_t NMAX = 40;
const Int_t Method = 1;
//     1-   A,     2-TSn,        3-TSx,        4-W,        5-R, 
//     6-  <A>,    7-<TSn>,      8-<TSx>,      9-<W>,     10-<R>, 
//    11- <P0>,   12-<P1>,      13-<P2>,      14-<P3>,
//    15-<PW0>,   16-<PW1>,     17-<PW2>,     18-<PW3>, 
//    19-  no,     20-no,
//    21-A_calib, 22-TSn_calib, 23-TSx_calib, 24-W_calib, 25-R_calib, 
//______________________________________________________________________________
int main(int argc, char *argv[]) 
{
   std::vector<long long> timer;
   TDatime T0(2012,01,01,00,00,00);
   long long X0 = T0.Convert();
   gStyle->SetTimeOffset(X0);
   gStyle->SetOptStat(0);
       std::ifstream in08("inputcards");
       std::string line;
       std::vector<std::string> filesv; 
       std::vector<int> ieta; 
       int depth=0;
       int iphi=0;
//
// Histogram names
//
       char histADCAmpl[100]; 
       char histTSmean[100];  
       char histTSmax[100];  
       char histAmplitude[100];
       char histAmpl[100];
       char histMap[100];      
       char outfile[100];
       char outleg[100];

       while ( std::getline( in08, line ) ) {
        if ( !line.size() || line[0]=='#' ) { continue; }
        std::istringstream ss(line);
        std::string sal0;
        std::string mystr;
        char sal1[300];
        int year,month,day,hour,minute,second,nrun;
        ss>>sal0;
        std::cout<<sal0<<" "<<sal0.find("eta")<<std::endl;
        if(sal0 == "inputruns") {ss>>year>>month>>day>>hour>>minute>>second>>nrun;
        TDatime T1(year,month,day,hour,minute,second);
        timer.push_back(T1.Convert()-X0);
                sprintf(sal1,"/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_%d.root",nrun);
                filesv.push_back(sal1);
        } // sal0 == "inputruns"
        if(sal0 == "eta") {
 
                     vector<string> tokens{istream_iterator<string>{ss},
                                           istream_iterator<string>{}}; 
 //                     std::cout<<tokens[0]<<" "<<tokens[1]<<" "<<tokens[10]<<std::endl;
            for(int kk=0;kk<tokens.size(); kk++) {
               //ieta.push_back((tokens[kk]).c_toi);

                int myeta = std::atoi(tokens[kk].c_str());
                ieta.push_back(myeta);
                std::cout<<myeta<<std::endl;

            } // int kk=1;kk<tokens.size(); kk++ 
        } //sal0 == "eta"
        if(sal0 == "depth") {
           ss>>depth; 
        }
        if(sal0 == "iphi") {
           ss>>iphi; 
        }
       }

      sprintf(histADCAmpl,"h_mapDepth%dADCAmpl_HF",depth);
      sprintf(histTSmean,"h_mapDepth%dTSmeanA_HF",depth);
      sprintf(histTSmax,"h_mapDepth%dTSmaxA_HF",depth);
      sprintf(histAmplitude,"h_mapDepth%dAmplitude_HF",depth);
      sprintf(histAmpl,"h_mapDepth%dAmpl_HF",depth);
      sprintf(histMap,"h_mapDepth%d_HF",depth);
      sprintf(outfile,"outDepth%d_phi%d_HF.gif",depth,iphi);
      sprintf(outleg,"HF depth %d, iphi=%d",depth, iphi);

  Float_t XX[NMAX];// days of year
  for(int k=0;k<NMAX;k++) {
    XX[k] = (float)(k+1);
  }
  Float_t YY[NMAX];// width averaged
  Float_t ZZ[NMAX];// ratio

  int tt0[NMAX];
  float  xxx0[NMAX];

   for(int ll=0; ll < timer.size();ll++) {
     tt0[ll] = timer[ll];
     xxx0[ll] = (float)tt0[ll];
   }

   int numdates = (int)timer.size();
   int numetas = (int)ieta.size(); 
  std::cout<<tt0[0]<<" "<<tt0[numdates-1]<<" "<<timer.size()<<std::endl;


  //______________________________________________________________________________
  // Create a new canvas.
  TCanvas* c1 = new TCanvas("zamt4","Monte Carlo Study of Z scaling",10,40,800,600);
  c1->Range(0,0,25,18);
  c1->SetFillColor(40);
  TPad* pad1 = new TPad("pad1","This is pad1",0.02,0.02,0.98,0.83,33);
   TLatex *t = new TLatex();
   t->SetTextFont(32);
   t->SetTextColor(4);
   t->SetTextSize(0.03);
   t->SetTextAlign(12);
   if(Method == 1) t->DrawLatex(0.9,15.5,"Time dependence of full Amplitude averaged over events of each LED run for HF channels");
   if(Method == 2) t->DrawLatex(0.9,15.5,"Time dependence of TSn averaged over events of each LED run for some(=9) random HB-depth1 channels");
   if(Method == 3) t->DrawLatex(0.9,15.5,"Time dependence of TSx averaged over events of each LED run for some(=9) random HB-depth1 channels");
   if(Method == 4) t->DrawLatex(0.9,15.5,"Time dependence of RMS averaged over events of each LED run for some(=9) random HB-depth1 channels");
   if(Method == 5) t->DrawLatex(0.9,15.5,"Time dependence of amplitude Ratio averaged over events of each LED run for some(=9) random HB-depth1 channels");
   if(Method == 6) t->DrawLatex(0.9,15.5,"Time dependence of full Amplitude averaged over events of each LED run and over all channels of HB-depth1");
   if(Method == 7) t->DrawLatex(0.9,15.5,"Time dependence of TSn averaged over events of each LED run and over all channels of HB-depth1");
   if(Method == 8) t->DrawLatex(0.9,15.5,"Time dependence of TSx averaged over events of each LED run and over all channels of HB-depth1");
   if(Method == 9) t->DrawLatex(0.9,15.5,"Time dependence of RMS averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==10) t->DrawLatex(0.9,15.5,"Time dependence of amplitude Ratio averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==11) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap0) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==12) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap1) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==13) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap2) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==14) t->DrawLatex(0.9,15.5,"Time dependence of Pedestal(cap3) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==15) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap0) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==16) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap1) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==17) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap2) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==18) t->DrawLatex(0.9,15.5,"Time dependence of PedestalWidth(cap3) averaged over events of each LED run and over all channels of HB-depth1");
   if(Method ==19) t->DrawLatex(0.9,15.5,"no suach a method");
   if(Method ==20) t->DrawLatex(0.9,15.5,"no suach a method");
   if(Method == 21) t->DrawLatex(0.9,15.5,"Time dependence of A_calib averaged over events of each LED run for some(=9) random HB channels");
   if(Method == 22) t->DrawLatex(0.9,15.5,"Time dependence of TSn_calib averaged over events of each LED run for some(=9) random HB channels");
   if(Method == 23) t->DrawLatex(0.9,15.5,"Time dependence of TSx_calib averaged over events of each LED run for some(=9) random HB channels");
   if(Method == 24) t->DrawLatex(0.9,15.5,"Time dependence of W_calib averaged over events of each LED run for some(=9) random HB channels");
   if(Method == 25) t->DrawLatex(0.9,15.5,"Time dependence of R_calib averaged over events of each LED run for some(=9) random HB channels");


  pad1->Draw();
  //______________________________________________________________________________
  pad1->cd();
  pad1->Range(-0.255174,-19.25,2.29657,-6.75);

   //TDatime T0(2012,01,01,00,00,00);
   //int X0 = T0.Convert();
   //gStyle->SetTimeOffset(X0);


  Float_t xmin0 = (float)(tt0[0]);
  Float_t xmax0 = (float)(tt0[numdates-1]);

  TH2F* hh2=new TH2F("hh2","hh2",2,xmin0,xmax0,100,0.8,1.2);
  hh2->GetXaxis()->SetTimeDisplay(1);
  hh2->GetXaxis()->SetLabelSize(0.03);
  hh2->GetXaxis()->SetTimeFormat("%D");
  hh2->GetXaxis()->SetTitle("Time");
  hh2->Draw();

  //pad1->DrawFrame(t0[0]-10.,   0.8,  t0[6]+10.,   1.1);// 1-A, 
  //pad1->SetTimeDisplay(1);
  //pad1->SetTimeFormat("%d\/%H:%M");
  //______________________________________________________________________________
  //______________________________________________________________________________
  pad1->GetFrame()->SetFillColor(19);
  //______________________________________________________________________________
  //______________________________________________________________________________
  // 
  //

  
    TFile* hfile;
    TH2F* twod1;
    TH2F* twod0;

  int r1=0;

   for(std::vector<int>::iterator meta=ieta.begin(); meta<ieta.end();meta++)
   {
    
    Int_t NP = numdates;  // for loop over time points(LED runs)

    for(int k1 = 0; k1<NP; k1++) {

      cout << "for LED run k1=" << k1   <<"      Number of imposed points = " << NP <<" "<<*meta<<endl;

      hfile= new TFile(filesv[k1].c_str(),"READ");

      cout << "OPENED" <<endl;
      //h_mapTSmaxCalib_HB
      // ADCAmpl   - fullAmplitude
      if(Method == 1) twod1= (TH2F*)hfile->Get(histADCAmpl);
      // TSmeanA   -    TSn
      if(Method == 2) twod1= (TH2F*)hfile->Get(histTSmean);
      // TSmaxA    -   TSx
      if(Method == 3) twod1= (TH2F*)hfile->Get(histTSmax);
      // Amplitude - W
      if(Method == 4) twod1= (TH2F*)hfile->Get(histAmplitude);
      // Ampl      - Ratio
      if(Method == 5) twod1= (TH2F*)hfile->Get(histAmpl);
      //
      //
      // Calibration channels:
      //
      //  - fullAmplitude
      if(Method == 21) twod1= (TH2F*)hfile->Get("h_mapADCCalib_HF");
      //  -    TSn
      if(Method == 22) twod1= (TH2F*)hfile->Get("h_mapTSmeanCalib_HF");
      //  -   TSx
      if(Method == 23) twod1= (TH2F*)hfile->Get("h_mapTSmaxCalib_HF");
      //  - W
      if(Method == 24) twod1= (TH2F*)hfile->Get("h_mapWidthCalib_HF");
      //  - Ratio
      if(Method == 25) twod1= (TH2F*)hfile->Get("h_mapRatioCalib_HF");
      //
      //
      //
      //
      if(Method == 6) {
	TH1F *Ceff6= (TH1F*)hfile->Get("h_ADCAmpl_HF");
	YY[k1]  =  Ceff6->GetMean();
	// if(k1==0 ) cout << "for LED run k1=" << k1   <<"GetMean = " << YY[k1] <<endl;
      }
      else if(Method == 7) {
	TH1F *Ceff7= (TH1F*)hfile->Get("h_TSmeanA_HF");
	YY[k1]  =  Ceff7->GetMean();
      }
      else if(Method == 8) {
	TH1F *Ceff8= (TH1F*)hfile->Get("h_TSmaxA_HF");
	YY[k1]  =  Ceff8->GetMean();
      }
      else if(Method == 9) {
	TH1F *Ceff9= (TH1F*)hfile->Get("h_Amplitude_HF");
	YY[k1]  =  Ceff9->GetMean();
      }
      else if(Method == 10) {
	TH1F *Ceff10= (TH1F*)hfile->Get("h_Ampl_HF");
	YY[k1]  =  Ceff10->GetMean();
      }
      else if(Method == 11) {
	TH1F *Ceff11= (TH1F*)hfile->Get("h_pedestal0_HF");
	YY[k1]  =  Ceff11->GetMean();
      }
      else if(Method == 12) {
	TH1F *Ceff12= (TH1F*)hfile->Get("h_pedestal1_HF");
	YY[k1]  =  Ceff12->GetMean();
      }
      else if(Method == 13) {
	TH1F *Ceff13= (TH1F*)hfile->Get("h_pedestal2_HF");
	YY[k1]  =  Ceff13->GetMean();
      }
      else if(Method == 14) {
	TH1F *Ceff14= (TH1F*)hfile->Get("h_pedestal3_HF");
	YY[k1]  =  Ceff14->GetMean();
      }
      else if(Method == 15) {
	TH1F *Ceff15= (TH1F*)hfile->Get("h_pedestalw0_HF");
	YY[k1]  =  Ceff15->GetMean();
      }
      else if(Method == 16) {
	TH1F *Ceff16= (TH1F*)hfile->Get("h_pedestalw1_HF");
	YY[k1]  =  Ceff16->GetMean();
      }
      else if(Method == 17) {
	TH1F *Ceff17= (TH1F*)hfile->Get("h_pedestalw2_HF");
	YY[k1]  =  Ceff17->GetMean();
      }
      else if(Method == 18) {
	TH1F *Ceff18= (TH1F*)hfile->Get("h_pedestalw3_HF");
	YY[k1]  =  Ceff18->GetMean();
      }
      else {
	//
	//
	if(Method < 6) twod0= (TH2F*)hfile->Get(histMap);
	if(Method > 20) twod0= (TH2F*)hfile->Get("h_map_HF");
	//
	//	if(twod0->IsA()->InheritsFrom("TH2F")) {
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");

        //std::cout<<" Bin content "<<twod1->GetBinContent(28,1)<<std::endl;

	Ceff->Divide(twod1,twod0, 1, 1, "B");  
	//	}
	int nx = Ceff->GetXaxis()->GetNbins();
	int ny = Ceff->GetYaxis()->GetNbins();
	if(k1==0 ) {
         cout << "for 1st LED run: nx = " << nx <<" ny = " << ny <<endl;
        }
	//     
         Int_t i = (*meta); 
         Int_t j = iphi;
         YY[k1]  =  Ceff->GetBinContent(i,j);
         if(i == 2) std::cout<<j<<" RRRR "<<YY[k1]<<" "<<i<<std::endl;
      }// else
    }//for k1 loop over time points(LED runs)

    // normalization on 1st time point

    
    for(int k2 = 0; k2<NP; k2++) {
      if(YY[0]<=0.) {ZZ[k2] = 0.;}
       else
      {ZZ[k2] = YY[k2]/YY[0];}
            cout<<"Point:k2="<<k2<<" YY[k2]="<<YY[k2]<<" YY[0]="<<YY[0]<<" ZZ[k2]="<<ZZ[k2]<<endl;
    }//



    gr1[r1] = new TGraph(NP,xxx0,ZZ);
    gr1[r1]->SetMarkerStyle(20);
    gr1[r1]->SetMarkerSize(0.9);
    gr1[r1]->SetLineColor(r1+1);
    gr1[r1]->SetMarkerColor(r1+1);
      gPad->SetGridy();

    gr1[r1]->Draw("PL");
    r1++;

  }//r1 -few random point sets

  //______________________________________________________________________________
  //______________________________________________________________________________
  //______________________________________________________________________________
  //  
TLegend * leg = new TLegend(0.20,0.75,0.55,0.95,outleg);
char mm[100];

std::cout<<"Number of graphs "<<r1<<std::endl;

  for(int m=0;m<r1;m++) {
     sprintf(mm,"ieta = -%d",41-m);
     TLegendEntry* leg1 = leg->AddEntry(gr1[m],mm,"p");
  }
  leg->Draw();
  c1->Modified();
  c1->Update();
  //
  c1->Print(outfile);
  gSystem->Exit(0);
  //______________________________________________________________________________

}
//______________________________________________________________________________
