//______________________________________________________________________________
TCanvas *c1;
TGraph *gr1;
const Int_t NMAX =25;
const Int_t Method =1;
//     1-   A,     2-TSn,        3-TSx,        4-W,        5-R, 
//     6-  <A>,    7-<TSn>,      8-<TSx>,      9-<W>,     10-<R>, 
//    11- <P0>,   12-<P1>,      13-<P2>,      14-<P3>,
//    15-<PW0>,   16-<PW1>,     17-<PW2>,     18-<PW3>, 
//    19-  no,     20-no,
//    21-A_calib, 22-TSn_calib, 23-TSx_calib, 24-W_calib, 25-R_calib, 
//______________________________________________________________________________
void zamt4() {
  Float_t XX[NMAX];// days of year
  Float_t XX[] ={91.,93.,94.,103.,108.,117.,145.,161.,176.,192.,194.,203.,209.,223.,261.,264.,279.,282.,288.,294.,298.,338.,342.,353.,365.};// time points
  Float_t YY[NMAX];// width averaged
  Float_t ZZ[NMAX];// ratio
  //______________________________________________________________________________
  // Create a new canvas.
  c1 = new TCanvas("zamt4","Monte Carlo Study of Z scaling",10,40,800,600);
  c1->Range(0,0,25,18);
  c1->SetFillColor(40);
  pad1 = new TPad("pad1","This is pad1",0.02,0.02,0.98,0.83,33);
  //  pad1 = new TPad("pad1","This is pad1",0.02,0.02,0.48,0.83,33);
  //  pad2 = new TPad("pad2","This is pad2",0.52,0.02,0.98,0.83,33);
   TLatex *t = new TLatex();
   t->SetTextFont(32);
   t->SetTextColor(4);
   t->SetTextSize(0.03);
   t->SetTextAlign(12);
   if(Method == 1) t->DrawLatex(0.9,15.5,"Time dependence of full Amplitude averaged over events of each LED run for some(=9) random HB-depth1 channels");
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
  //  pad2->Draw();
  //______________________________________________________________________________
  pad1->cd();
  pad1->Range(-0.255174,-19.25,2.29657,-6.75);
  //  pad1->Range(-0.255174,  -19.25,  2.29657,  -6.75);
////  pad2->Range(-0.43642,   -23.75,  3.92778,  -6.25);
  //   pad1->SetLogx();
  //   pad1->SetLogy();
  // create a 2-d histogram to define the range
  //              x1   y1     x2     y2
  pad1->DrawFrame(1,   0.,  370,   2.);// 1-A, 
  //______________________________________________________________________________
  //______________________________________________________________________________
  pad1->GetFrame()->SetFillColor(19);
  //______________________________________________________________________________
  //______________________________________________________________________________
  // 
  Int_t NR =1;  // number of imposed dependencies of different channels 
  Int_t r1=0;
  while (r1<NR) {
    
    Int_t NP = NMAX;  // for loop over time points(LED runs)
    for(int k1 = 0; k1<NP; k1++) {
      cout << "for LED run k1=" << k1   <<"      Number of imposed points = " << NR <<endl;
      //______________________________________________________________________________
      if(k1==0) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_194057.root", "READ");
      else if(k1==1) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_194124.root", "READ");
      else if(k1==2) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_194165.root", "READ");
      else if(k1==3) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_194807.root", "READ");
      else if(k1==4) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_195172.root", "READ");
      else if(k1==5) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_195682.root", "READ");
      else if(k1==6) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_198084.root", "READ");
      else if(k1==7) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_199392.root", "READ");
      else if(k1==8) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_200281.root", "READ");
      else if(k1==9) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_201305.root", "READ");
      else if(k1==10) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_201571.root", "READ");
      else if(k1==11) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_202066.root", "READ");
      else if(k1==12) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_202341.root", "READ");
      else if(k1==13) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_203339.root", "READ");
      else if(k1==14) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_205928.root", "READ");
      else if(k1==15) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_206456.root", "READ");
      else if(k1==16) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_207406.root", "READ");
      else if(k1==17) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_207545.root", "READ");
      else if(k1==18) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_207914.root", "READ");
      else if(k1==19) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_208361.root", "READ");
      else if(k1==20) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_208565.root", "READ");
      else if(k1==21) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_209997.root", "READ");
      else if(k1==22) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_210378.root", "READ");
      else if(k1==23) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_211006.root", "READ");
      else if(k1==24) TFile *hfile= new TFile("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_HCALCALIB/HCALMONITORING/RDMweb/histos/LED_211659.root", "READ");
      //
      cout << "OPENED" <<endl;
      //h_mapTSmaxCalib_HB
      // ADCAmpl   - fullAmplitude
      if(Method == 1) TH2F *twod1= (TH2F*)hfile->Get("h_mapDepth1ADCAmpl_HB");
      // TSmeanA   -    TSn
      if(Method == 2) TH2F *twod1= (TH2F*)hfile->Get("h_mapDepth1TSmeanA_HB");
      // TSmaxA    -   TSx
      if(Method == 3) TH2F *twod1= (TH2F*)hfile->Get("h_mapDepth1TSmaxA_HB");
      // Amplitude - W
      if(Method == 4) TH2F *twod1= (TH2F*)hfile->Get("h_mapDepth1Amplitude_HB");
      // Ampl      - Ratio
      if(Method == 5) TH2F *twod1= (TH2F*)hfile->Get("h_mapDepth1Ampl_HB");
      //
      //
      // Calibration channels:
      //
      //  - fullAmplitude
      if(Method == 21) TH2F *twod1= (TH2F*)hfile->Get("h_mapADCCalib_HB");
      //  -    TSn
      if(Method == 22) TH2F *twod1= (TH2F*)hfile->Get("h_mapTSmeanCalib_HB");
      //  -   TSx
      if(Method == 23) TH2F *twod1= (TH2F*)hfile->Get("h_mapTSmaxCalib_HB");
      //  - W
      if(Method == 24) TH2F *twod1= (TH2F*)hfile->Get("h_mapWidthCalib_HB");
      //  - Ratio
      if(Method == 25) TH2F *twod1= (TH2F*)hfile->Get("h_mapRatioCalib_HB");
      //
      //
      //
      //
      if(Method == 6) {
	TH1F *Ceff6= (TH1F*)hfile->Get("h_ADCAmpl_HB");
	YY[k1]  =  Ceff6->GetMean();
	// if(k1==0 ) cout << "for LED run k1=" << k1   <<"GetMean = " << YY[k1] <<endl;
      }
      else if(Method == 7) {
	TH1F *Ceff7= (TH1F*)hfile->Get("h_TSmeanA_HB");
	YY[k1]  =  Ceff7->GetMean();
      }
      else if(Method == 8) {
	TH1F *Ceff8= (TH1F*)hfile->Get("h_TSmaxA_HB");
	YY[k1]  =  Ceff8->GetMean();
      }
      else if(Method == 9) {
	TH1F *Ceff9= (TH1F*)hfile->Get("h_Amplitude_HB");
	YY[k1]  =  Ceff9->GetMean();
      }
      else if(Method == 10) {
	TH1F *Ceff10= (TH1F*)hfile->Get("h_Ampl_HB");
	YY[k1]  =  Ceff10->GetMean();
      }
      else if(Method == 11) {
	TH1F *Ceff11= (TH1F*)hfile->Get("h_pedestal0_HB");
	YY[k1]  =  Ceff11->GetMean();
      }
      else if(Method == 12) {
	TH1F *Ceff12= (TH1F*)hfile->Get("h_pedestal1_HB");
	YY[k1]  =  Ceff12->GetMean();
      }
      else if(Method == 13) {
	TH1F *Ceff13= (TH1F*)hfile->Get("h_pedestal2_HB");
	YY[k1]  =  Ceff13->GetMean();
      }
      else if(Method == 14) {
	TH1F *Ceff14= (TH1F*)hfile->Get("h_pedestal3_HB");
	YY[k1]  =  Ceff14->GetMean();
      }
      else if(Method == 15) {
	TH1F *Ceff15= (TH1F*)hfile->Get("h_pedestalw0_HB");
	YY[k1]  =  Ceff15->GetMean();
      }
      else if(Method == 16) {
	TH1F *Ceff16= (TH1F*)hfile->Get("h_pedestalw1_HB");
	YY[k1]  =  Ceff16->GetMean();
      }
      else if(Method == 17) {
	TH1F *Ceff17= (TH1F*)hfile->Get("h_pedestalw2_HB");
	YY[k1]  =  Ceff17->GetMean();
      }
      else if(Method == 18) {
	TH1F *Ceff18= (TH1F*)hfile->Get("h_pedestalw3_HB");
	YY[k1]  =  Ceff18->GetMean();
      }
      else {
	//
	//
	if(Method < 6) TH2F *twod0= (TH2F*)hfile->Get("h_mapDepth1_HB");
	if(Method > 20) TH2F *twod0= (TH2F*)hfile->Get("h_map_HB");
	//
	//	if(twod0->IsA()->InheritsFrom("TH2F")) {
	TH2F* Ceff = (TH2F*)twod1->Clone("Ceff");
	Ceff->Divide(twod1,twod0, 1, 1, "B");  
	//	}
	int nx = Ceff->GetXaxis()->GetNbins();
	int ny = Ceff->GetYaxis()->GetNbins();
	if(k1==0 ) cout << "for 1st LED run: nx = " << nx <<" ny = " << ny <<endl;
	//
	Int_t rr=0;
	Int_t rrcount=0;
	while (rr==0) {
	  ++rrcount;
	  Int_t i = nx*((rand()%100+2)/100.);
	  Int_t j = ny*((rand()%100+2)/100.);
	  YY[k1]  =  Ceff->GetBinContent(i,j);
	  // cout<<"Point:k1="<<k1<<"r1="<<r1<<"rr="<<rr<<"i="<<i<<"j="<<j<<"YY="<<YY[k1]<<endl;
	  if(rrcount>100) {
	    rr=1;
	    cout<<"rr exceed 100, seems file empty="<<k1<<endl;
	  }
	  if(YY[k1] >0.001 ) rr=1; 
	}//while    rr
	////if(YY[k1]>0.001)cout<<"Point:k1="<<k1<<"r1="<<r1<<"rr="<<rr<<"i="<<i<<"j="<<j<<"YY="<<YY[k1]<<endl;
      }// else
    }//for k1 loop over time points(LED runs)
    //
    /*
      cout<<" gr1: NP= " << NP
      <<"  X0="<<XX[0]<<"Y0="<<YY[0]<<"  X1="<<XX[1]<<"Y1="<<YY[1]<<"  X2="<<XX[2]<<"Y2="<<YY[2]  
      <<"  X3="<<XX[3]<<"Y3="<<YY[3]<<"  X4="<<XX[4]<<"Y4="<<YY[4]<<"  X5="<<XX[5]<<"Y5="<<YY[5]  
      <<"  X6="<<XX[6]<<"Y6="<<YY[6]<<"  X7="<<XX[7]<<"Y7="<<YY[7]<<"  X8="<<XX[8]<<"Y8="<<YY[8]  
      <<"  X9="<<XX[9]<<"Y9="<<YY[9]<<"  X10="<<XX[10]<<"Y10="<<YY[10]<<"  X11="<<XX[11]<<"Y11="<<YY[11]  
      <<"  X12="<<XX[12]<<"Y12="<<YY[12]<<"  X13="<<XX[13]<<"Y13="<<YY[13]<<"  X14="<<XX[14]<<"Y14="<<YY[14]  
      <<"  X15="<<XX[15]<<"Y15="<<YY[15]<<"  X16="<<XX[16]<<"Y16="<<YY[16]<<"  X17="<<XX[17]<<"Y17="<<YY[17]  
      <<"  X18="<<XX[18]<<"Y18="<<YY[18]<<"  X19="<<XX[19]<<"Y19="<<YY[19]<<"  X20="<<XX[20]<<"Y20="<<YY[20]  
      <<"  X21="<<XX[21]<<"Y21="<<YY[21]<<"  X22="<<XX[22]<<"Y22="<<YY[22]<<"  X23="<<XX[23]<<"Y23="<<YY[23]  
      <<"  X24="<<XX[24]<<"Y24="<<YY[24]
      <<endl;
*/
    // normalization on 1st time point
    for(int k2 = 0; k2<NP; k2++) {
      ZZ[k2] = YY[k2]/YY[0];
      //      cout<<"Point:k2="<<k2<<" YY[k2]="<<YY[k2]<<" YY[0]="<<YY[0]<<" ZZ[k2]="<<ZZ[k2]<<endl;
    }//
    //
    gr1 = new TGraph(NP,XX,ZZ);
    gr1->SetMarkerStyle(20);
    gr1->SetMarkerSize(0.9);
    gr1->SetLineColor(r1+1);
    gr1->SetMarkerColor(r1+1);
      gPad->SetGridy();
      //      gPad->SetGridx();
    gr1->Draw("PL");
    ++r1;
    //
  }//r1 -few random point sets
  //______________________________________________________________________________
  //______________________________________________________________________________
  //______________________________________________________________________________
  //  
  c1->Modified();
  c1->Update();
  //
  c1->Print("zamt4.ps");
  gSystem->Exit(0);
  //______________________________________________________________________________
}
//______________________________________________________________________________
